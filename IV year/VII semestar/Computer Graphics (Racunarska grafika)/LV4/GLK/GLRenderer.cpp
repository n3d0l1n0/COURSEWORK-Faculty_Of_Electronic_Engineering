#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h" //osnovne OpenGL funkcije: glBegin, glVertex, glColor
#include "GL\glu.h" //gluLookAt, gluPerspective
#include "GL\glaux.h"
#include "GL\glut.h"
#pragma comment(lib, "GL\\glut32.lib") //ovo je bilo zakomentarisano
#pragma comment(lib,"GL\\GLU32.lib")   //ovo smo dodali 

CGLRenderer::CGLRenderer(void)
{
	this->angle_v = 0;
	this->angle_h = 0;
	this->angle_cx = 0;
	this->angle_cy = 0;
	this->angle_yellow = 0.0f;
	cam_radius = 2.5f;       // udaljenost kamere
	cam_angle_h = 45.0f;     // horizontalni ugao
	cam_angle_v = 10.0f;     // vertikalni ugao, v=0 kamera je u ravni zemlje, v>0 kamera ide gore, v<0 kamera ide dole
}

CGLRenderer::~CGLRenderer(void)
{
}

void CGLRenderer::SetCamera()
{
	// konvertujemo uglove u radijane
	float h = cam_angle_h * C_PI / 180.0f;
	float v = cam_angle_v * C_PI / 180.0f;

	// polarne koordinate - kartizijanske
	float camX = cam_radius * cosf(v) * sinf(h);
	float camY = cam_radius * sinf(v);
	float camZ = cam_radius * cosf(v) * cosf(h);

	gluLookAt(camX, camY, camZ,   // pozicija kamere
		0.0, 0.6, 0.0,     // centar scene 
		0.0, 1.0, 0.0);    // gore = Y osa
}


bool CGLRenderer::CreateGLContext(CDC* pDC)  //Celo Template
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24; 
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd); 

	if (nPixelFormat == 0) return false;

	BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

	if (!bResult) return false;

	m_hrc = wglCreateContext(pDC->m_hDC);

	if (!m_hrc) return false;

	return true;
}

void CGLRenderer::PrepareScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	glClearColor(0.5f, 0.75f, 1.0f, 1.0f);//POZADINA-plava
	glEnable(GL_DEPTH_TEST); 

	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC* pDC) 
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //S. brisanje buffera 
	glMatrixMode(GL_MODELVIEW); //context openGL-a je setovan na ovaj jedan od 2 moda i sve transformacije se odnose na ovaj mod 
	glLoadIdentity();
	SetCamera();   

	DrawAxis(2.5);
	DrawGrid(2.0, 2.0, 20, 20);
	
	glPushMatrix(); //da jednom transformacijom ne uticemo na drugi objekat, tu transformaciju pushujemo na stack
	glScalef(0.5, 0.5, 0.5);
	
	DrawPot();    
	DrawFigure(); 
	glPopMatrix();
	glFlush();
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (double)w / (double)h, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	wglMakeCurrent(NULL, NULL);
}


void CGLRenderer::DestroyScene(CDC* pDC)  //celo T
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawPrism(float radius, float height, int sides)
{

	float t_height = height / 2.0;

	glPushMatrix();
	glRotatef(45, 0, 1, 0);
	float step = (2 * C_PI) / sides;
	//baza 1
	glBegin(GL_POLYGON);
	for (int i = 0; i < sides; i++)
		glVertex3f(get_x(radius, i * step), t_height, get_y(radius, i * step));
	glEnd();

	//baza 2
	glBegin(GL_POLYGON);
	for (int i = 0; i < sides; i++)
		glVertex3f(get_x(radius, i * step), -t_height, get_y(radius, i * step));
	glEnd();

	//omotac
	
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < sides; i++) {
		glVertex3f(get_x(radius, i * step), -t_height, get_y(radius, i * step));
		glVertex3f(get_x(radius, (i + 1) * step), -t_height, get_y(radius, (i + 1) * step));
		glVertex3f(get_x(radius, i * step), t_height, get_y(radius, i * step));
		glVertex3f(get_x(radius, (i + 1) * step), t_height, get_y(radius, (i + 1) * step));
	}
	glEnd();
	
	glPopMatrix();
}

void CGLRenderer::DrawCone(double h, double r, int nSeg)
{
	double step = 2 * C_PI / nSeg;
	double half = h / 2.0;

	// baza
	glBegin(GL_POLYGON);
	for (int i = 0; i < nSeg; i++)
		glVertex3f(r * cos(i * step), -half, r * sin(i * step));
	glEnd();

	// omotac
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, half, 0); // vrh
	for (int i = 0; i <= nSeg; i++)
		glVertex3f(r * cos(i * step), -half, r * sin(i * step));
	glEnd();
}


void CGLRenderer::DrawCylinder(double h, double r1, double r2, int nSeg)
{
	double step = 2 * C_PI / nSeg;
	double half = h / 2.0;

	// donja baza
	glBegin(GL_POLYGON);
	for (int i = 0; i < nSeg; i++)
		glVertex3f(r1 * cos(i * step), -half, r1 * sin(i * step));
	glEnd();

	// gornja baza
	glBegin(GL_POLYGON);
	for (int i = 0; i < nSeg; i++)
		glVertex3f(r2 * cos(i * step), half, r2 * sin(i * step));
	glEnd();

	// omotac
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= nSeg; i++) {
		glVertex3f(r1 * cos(i * step), -half, r1 * sin(i * step));
		glVertex3f(r2 * cos(i * step), half, r2 * sin(i * step));
	}
	glEnd();
}

void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta)
{
	for (int i = 0; i <= nSegAlpha; i++) {
		double alpha0 = C_PI * (-0.5 + (double)(i - 1) / nSegAlpha);
		double alpha1 = C_PI * (-0.5 + (double)i / nSegAlpha);

		double z0 = sin(alpha0);
		double z1 = sin(alpha1);
		double zr0 = cos(alpha0);
		double zr1 = cos(alpha1);

		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= nSegBeta; j++) {
			double beta = 2 * C_PI * (double)(j - 1) / nSegBeta;
			double x = cos(beta);
			double y = sin(beta);

			glVertex3f(r * x * zr0, r * y * zr0, r * z0);
			glVertex3f(r * x * zr1, r * y * zr1, r * z1);
		}
		glEnd();
	}
}

void CGLRenderer::DrawAxis(double width)
{
	glLineWidth(2.0);
	glBegin(GL_LINES);

	glColor3f(1, 0, 0); // X
	glVertex3f(0, 0, 0);
	glVertex3f(width, 0, 0);

	glColor3f(1, 1, 0); // Y
	glVertex3f(0, 0, 0);
	glVertex3f(0, width, 0);

	glColor3f(0, 0, 1); // Z
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, width);

	glEnd();
}


void CGLRenderer::DrawGrid(double width, double height, int nSegW, int nSegH)
{
	glColor3f(1, 0.6, 0);
	glBegin(GL_LINES);

	double dx = width / nSegW;
	double dz = height / nSegH;

	for (int i = 0; i <= nSegW; i++) {
		double x = -width / 2 + i * dx;
		glVertex3f(x, 0, -height / 2);
		glVertex3f(x, 0, height / 2);
	}

	for (int i = 0; i <= nSegH; i++) {
		double z = -height / 2 + i * dz;
		glVertex3f(-width / 2, 0, z);
		glVertex3f(width / 2, 0, z);
	}

	glEnd();
}


void CGLRenderer::SetAngles(float angle_h, float angle_v)
{
	this->angle_v = angle_v;
	this->angle_h = angle_h;
}

float CGLRenderer::get_x(float radius, float angle)
{
	return radius * cosf(angle);
}

float CGLRenderer::get_y(float radius, float angle)
{
	return radius * sinf(angle);
}

void CGLRenderer::DrawPot()
{
	int sides = 8;
	float t_height = 0.4 / 2.0; //u sustini menjaj visinu vaze

	glPushMatrix();

	glTranslatef(0, 0.2, 0); //malo podizes koord sistem (koji se sad zove lokalni?) da bi postavili vazu tik iznad grida, a ne da ga sece

	float step = (2 * C_PI) / sides;

	glColor3f(0.0, 0.8, 0.0); //BOJA, DONJEG DELA VAZE
	//Baza 1 //donji deo donjeg dela vaze
	glBegin(GL_POLYGON);
	for (int i = 0; i < sides; i++)
		glVertex3f(get_x(0.45, i * step), t_height, get_y(0.45, i * step));
	glEnd();

	//Baza 2 //gornji deo donjeg dela vaze
	glBegin(GL_POLYGON);
	for (int i = 0; i < sides; i++)
		glVertex3f(get_x(0.4, i * step), -t_height, get_y(0.4, i * step));
	glEnd();

	//Omotac
	glBegin(GL_POLYGON);
	for (int i = 0; i < sides; i++) {
		glVertex3f(get_x(0.4, i * step), -t_height, get_y(0.4, i * step));
		glVertex3f(get_x(0.4, (i + 1) * step), -t_height, get_y(0.4, (i + 1) * step));
		glVertex3f(get_x(0.45, i * step), t_height, get_y(0.45, i * step));
		glVertex3f(get_x(0.45, (i + 1) * step), t_height, get_y(0.45, (i + 1) * step));
	}
	glEnd();

	//Translirano je vec na 0.2 , dodamo jos 0.2 da postavimo na
	//pocetak donjeg dela + 0.1 da centriramo gornji deo koji je debljine 0.2
	//+ 0.01 cisto da se lepo vidi depth

	glTranslatef(0, 0.3, 0);
	glColor4f(0.0, 0.8, 0.0, 0.5);
	this->DrawPrism(0.5, 0.2, 8); //crta gornji deo vaze

	glPopMatrix();

}

void CGLRenderer::DrawFigure()
{
	//PRVI CILINDAR
	glPushMatrix();

	glTranslatef(0, 0.5, 0);
	glRotatef(angle_yellow, 1, 0, 0);

	glTranslatef(0.0f, 0.3f, 0.0f); 
	glColor3f(1.0f, 1.0f, 0.0f); 
	DrawCylinder(0.4, 0.115, 0.12, 20);

	glTranslatef(0.0f, 0.3f, 0.0f);

	glColor3f(0.47f, 0.0f, 0.0f);
	DrawSphere(0.1, 15, 15);

	MiddleBranch();
	RightBranch();
	LeftBranch();

	glPopMatrix();

}

void CGLRenderer::MiddleBranch()
{

	glPushMatrix(); //cuvamo koord sistem kakav je bio pre ulazka u fju 
	glTranslatef(0.0f, 0.3f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);//bela
	DrawCone(0.4f, 0.12f, 20);
	glTranslatef(0.0f, 0.3f, 0.0f);

	glColor3f(0.47f, 0.0f, 0.0f);
	DrawSphere(0.1, 15, 15);
	//cilindar 
	glTranslatef(0, 0.3, 0);
	glColor3f(1.0f, 1.0f, 1.0f);//bela
	DrawCylinder(0.4, 0.115, 0.12, 20);

	glTranslatef(0.0f, 0.3f, 0.0f);
	glColor3f(0.47f, 0.0f, 0.0f);
	DrawSphere(0.1, 15, 15);
	//cone
	glTranslatef(0, 0.3, 0);
	glColor3f(1.0f, 1.0f, 1.0f);//bela
	DrawCone(0.4f, 0.12f, 20);

	glTranslatef(0.0f, 0.3f, 0.0f);
	glColor3f(0.47f, 0.0f, 0.0f);
	DrawSphere(0.1, 15, 15);

	//RightMiddleBranch();
	//LeftMiddleBranch();
	glPopMatrix();
}

void CGLRenderer::RightMiddleBranch()
{
	glPushMatrix();
	glColor3f(0.4f, 0.7f, 0.4f);//svetlo zelena
	glRotatef(-45, 1, 0, 0); //rotirani smo za 45 stepeni
	//glColor3f(1, 1, 0); //zuta 
	glTranslatef(0, 0.3,0);
	DrawCylinder(0.4, 0.115, 0.12, 20);

	glTranslatef(0, 0.3, 0);
	glColor3f(0.0f, 0.5f, 0.0f); //tamno zelena
	DrawSphere(0.1, 15, 15);
	glPushMatrix();

	glRotatef(angle_cx, 1, 0, 0);
	glTranslatef(0, 0.3, 0);

	//glColor3f(1, 1, 0); zuta
	glColor3f(0.4f, 0.7f, 0.4f);
	DrawCylinder(0.4, 0.115, 0.12, 20);

	glTranslatef(0, 0.3, 0);

	glColor3f(0.0f, 0.5f, 0.0f);
	DrawSphere(0.1, 15, 15);
	glPopMatrix();
	glPopMatrix();

}

void CGLRenderer::LeftMiddleBranch()
{
	glPushMatrix();
	glRotatef(angle_yellow, 1, 0, 0);
	glRotatef(45, 1, 0, 0);

	glTranslatef(0, 0.3, 0);
	glColor3f(1, 1, 0);
	DrawCylinder(0.4, 0.115, 0.12, 20);

	glTranslatef(0, 0.3, 0);
	glColor3f(0.0f, 0.5f, 0.0f);
	DrawSphere(0.1, 15, 15);

	glPushMatrix();
	glRotatef(angle_cx, 1, 0, 0);
	glTranslatef(0, 0.3, 0);
	glColor3f(0.4f, 0.7f, 0.4f);
	DrawCone(0.4f, 0.15f, 20);

	glTranslatef(0, 0.3, 0);
	glColor3f(0.0f, 0.5f, 0.0f);
	DrawSphere(0.1, 15, 15);
	glPopMatrix();

	glPopMatrix();
}


void CGLRenderer::RotateYellow(float angle) {
	this->angle_yellow += angle;
}

void CGLRenderer::RightBranch()
{
	glPushMatrix();
	glColor3f(0.4f, 0.7f, 0.4f);//svetlo zelena
	glRotatef(-45, 1, 0, 0); //rotirani smo za 45 stepeni
	glTranslatef(0, 0.3, 0);
	glColor3f(1.0f, 1.0f, 1.0f);//bela
	DrawCylinder(0.4, 0.115, 0.12, 20);

	glTranslatef(0, 0.3, 0);
	glColor3f(0.47f, 0.0f, 0.0f); //bordo
	DrawSphere(0.1, 15, 15);
	glPushMatrix();

	//uspravno 
	glPushMatrix();
	glRotatef(-45, 1, 0, 0);
	glTranslatef(0, 0.3, 0);

	glColor3f(1.0f, 1.0f, 1.0f);//bela
	DrawCylinder(0.4, 0.115, 0.12, 20);

	glTranslatef(0, 0.3, 0);

	glColor3f(0.47f, 0.0f, 0.0f); //bordo
	DrawSphere(0.1, 15, 15);
	glPopMatrix();

	//na desno prvi cilindar
	glRotatef(45, 1, 0, 0);
	glTranslatef(0, 0.3, 0);

	glColor3f(1.0f, 1.0f, 1.0f);//bela
	DrawCylinder(0.4, 0.115, 0.12, 20);

	glTranslatef(0, 0.3, 0);
	glColor3f(0.47f, 0.0f, 0.0f); //bordo
	DrawSphere(0.1, 15, 15);


	glPopMatrix();
	glPopMatrix();
}

void CGLRenderer::LeftBranch()
{
	glPushMatrix();
	glRotatef(45, 1, 0, 0);
	glTranslatef(0, 0.3, 0);
	glColor3f(1.0f, 1.0f, 1.0f);//bela
	DrawCone(0.4f, 0.12f, 20);

	glTranslatef(0, 0.3, 0);
	glColor3f(0.47f, 0.0f, 0.0f); //bordo
	DrawSphere(0.1, 15, 15);
	glPushMatrix();

	//uspravno 
	glPushMatrix();
	glRotatef(-45, 1, 0, 0);
	glTranslatef(0, 0.3, 0);

	glColor3f(1.0f, 1.0f, 1.0f);//bela
	DrawCone(0.4f, 0.15f, 20);

	glTranslatef(0, 0.3, 0);

	glColor3f(0.47f, 0.0f, 0.0f); //bordo
	DrawSphere(0.1, 15, 15);
	glPopMatrix();

	//na levo prvi cilindar
	glRotatef(45, 1, 0, 0);
	glTranslatef(0, 0.3, 0);

	glColor3f(1.0f, 1.0f, 1.0f);//bela
	DrawCylinder(0.4, 0.115, 0.12, 20);

	glTranslatef(0, 0.3, 0);
	glColor3f(0.47f, 0.0f, 0.0f); //bordo
	DrawSphere(0.1, 15, 15);

	//na levo drugi cone
	//glRotatef(45, 1, 0, 0);
	glTranslatef(0, 0.3, 0);

	glColor3f(1.0f, 1.0f, 1.0f);//bela
	DrawCone(0.4f, 0.15f, 20);

	glTranslatef(0, 0.3, 0);
	glColor3f(0.47f, 0.0f, 0.0f); //bordo
	DrawSphere(0.1, 15, 15);

	glPopMatrix(); 
	glPopMatrix();
}