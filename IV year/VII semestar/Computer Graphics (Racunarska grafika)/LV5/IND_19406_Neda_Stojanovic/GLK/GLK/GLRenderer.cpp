#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#pragma comment(lib, "GL\\glut32.lib")
#include "CGLMaterial.h"
#include <cmath> 

CGLRenderer::CGLRenderer(void)
{
	light0 = true;
	light1 = true;
	light2 = true;
	normals = false;
	this->angle_v = 0;
	this->angle_h = 0;


	//velicinu postolja menjamo u drawStand pa u DrawPrism()
	//bojr za krugove, boje za vazu

	//BOJE ZA VAZU, SA CS-A
	magenta.SetAmbient(0.2, 0.0, 0.2, 1.0);  //boja objekta pod ambijentalnim svetlom
	magenta.SetDiffuse(1.0, 0.0, 1.0, 1.0); //glavna boja objekta kada na njega pada svetlo
	magenta.SetSpecular(0.5, 0.5, 0.5, 1.0); //odsjaj na objektu
	magenta.SetShininess(32.0); //mala vrednost - sirok sjaj, velika vrednost - mali sjaj

	//RGB 0.00 1.00 1.00
	cyan.SetAmbient(0.0, 0.2, 0.2, 1.0);
	cyan.SetDiffuse(0.0, 1.0, 1.0, 1.0);
	cyan.SetSpecular(0.5, 0.5, 0.5, 1.0);
	cyan.SetShininess(32.0);


	// za sobu boje
	matBase.SetAmbient(0.3, 0.3, 0.3, 0.0); 
	matBase.SetDiffuse(0.8, 0.8, 0.8, 0.0);   // svetlo siva
	matBase.SetSpecular(0.5, 0.5, 0.5, 0.0);  // neutralni sjaj
	matBase.SetShininess(64.0);

	roomBase.SetAmbient(0.3, 0.3, 0.3, 1.0);   // ambijentalno svetlo
	roomBase.SetDiffuse(0.85, 0.85, 0.85, 1.0); // svetlo siva
	roomBase.SetEmission(0.1, 0.1, 0.1, 1.0);   // lagano emitovanje
	roomBase.SetSpecular(0.0, 0.0, 0.0, 1.0); // Iskljucuje odsjaj
	roomBase.SetShininess(128.0);


	// loptice za svetlo

	redSphere.SetAmbient(0.2, 0.0, 0.0, 1.0);   
	redSphere.SetDiffuse(1.0, 0.0, 0.0, 1.0);    
	redSphere.SetSpecular(1.0, 0.0, 0.0, 0.0);  
	redSphere.SetEmission(1.0, 0.0, 0.0, 1.0);  
	redSphere.SetShininess(0);

	blueSphere.SetAmbient(0.0, 1.0, 1.0, 1); //na zid
	blueSphere.SetDiffuse(0.0, 1.0, 1.0, 0.0); //kuglica
	blueSphere.SetSpecular(0, 1, 1, 1);
	blueSphere.SetEmission(0, 1, 1, 1);
	blueSphere.SetShininess(0);

	yellowSphere.SetAmbient(0.2, 0.2, 0.0, 1);
	yellowSphere.SetDiffuse(1.0, 1.0, 0.0, 0);
	yellowSphere.SetSpecular(1, 1, 0, 1);
	yellowSphere.SetEmission(1, 1, 0, 1);
	yellowSphere.SetShininess(0);


	////plava
	//blueSphere.SetAmbient(0.0, 0.0, 0.2, 1); //na zid
	//blueSphere.SetDiffuse(0.0, 0.0, 1.0, 0); //kuglica
	//blueSphere.SetSpecular(0, 0, 1, 1);
	//blueSphere.SetEmission(0, 0, 1, 1);
	//blueSphere.SetShininess(0);


	////crvena
	//redSphere.SetAmbient(0.2, 0.0, 0.0, 1);
	//redSphere.SetDiffuse(1.0, 0.0, 0.0, 0);
	//redSphere.SetSpecular(1, 0, 0, 1);
	//redSphere.SetEmission(1, 0, 0, 1);
	//redSphere.SetShininess(0);


	////zuta
	//yellowSphere.SetAmbient(0.2, 0.2, 0.0, 1);
	//yellowSphere.SetDiffuse(1.0, 1.0, 0.0, 0);
	//yellowSphere.SetSpecular(1, 1, 0, 1);
	//yellowSphere.SetEmission(1, 1, 0, 1);
	//yellowSphere.SetShininess(0);
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
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
	//---------------------------------
	glClearColor(0, 0, 0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	AmbientLight();
	glEnable(GL_LIGHTING);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float dist = 18.0f;

	float rad_h = angle_h * 3.14159f / 180.0f;
	float rad_v = angle_v * 3.14159f / 180.0f;

	float camX = dist * cos(rad_v) * sin(rad_h);
	float camY = dist * sin(rad_v);
	float camZ = dist * cos(rad_v) * cos(rad_h);

	gluLookAt(camX, camY + 12.0, camZ,0.0, 10.0, 0.0,0.0, 1.0, 0.0);

	AmbientLight();
	ViewPointLight();

	glDisable(GL_LIGHTING);

	DrawAxis();

	glEnable(GL_LIGHTING);

	YellowLight();
	RedLight();
	BlueLight();

	glPushMatrix();

	roomBase.Select();
	DrawRoom();
	glScalef(1.5, 1.5, 1.5);
	matBase.Select();
	DrawStand();

	DrawVase();

	glPopMatrix();

	glFlush();
	//---------------------------------
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65, (double)w / (double)h, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC* pDC)
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

void CGLRenderer::DrawAxis()
{
	float size = 30;
	glLineWidth(1.0);

	glBegin(GL_LINES);
	//Crvena X
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(size, 0.0, 0.0);

	//Zelena Y
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, size, 0.0);

	//Plava Z
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, size);
	glEnd();
	glColor3f(0.0, 0.0, 0.0);
}

void CGLRenderer::SetAngles(float angle_h, float angle_v)
{
	this->angle_v = angle_h;
	this->angle_h = angle_v;
}

void CGLRenderer::SwitchLight0()
{
	light0 = !light0;
}

void CGLRenderer::SwitchLight1()
{
	light1 = !light1;
}

void CGLRenderer::SwitchLight2()
{
	light2 = !light2;
}

void CGLRenderer::SwitchNormals()
{
	normals = !normals;
}

float CGLRenderer::get_x(float radius, float angle)
{
	return radius * cosf(angle);
}

float CGLRenderer::get_y(float radius, float angle)
{
	return radius * sinf(angle);
}

void CGLRenderer::DrawRoom()
{
	float a = 40; //dimenzija zida/poda

	roomBase.Select();

	glBegin(GL_QUADS);

	float step = 1; //velicina kvadratica u mrezi, manji step-vise  kvadratica - glatko
	//bottom
	glNormal3f(0, 1, 0);
	for (float i = -a / 2; i < a / 2; i += step)
	{
		for (float j = -a / 2; j < a / 2; j += step)
		{
			glVertex3f(i, 0, j);
			glVertex3f(i, 0, j + step);
			glVertex3f(i + step, 0, j + step);
			glVertex3f(i + step, 0, j);
		}
	}
	glEnd();



	glBegin(GL_QUADS);
	//up
	glNormal3f(0, -1, 0);
	for (float i = -a / 2; i < a / 2; i += step)
	{
		for (float j = -a / 2; j < a / 2; j += step)
		{
			glVertex3f(i, a, j);
			glVertex3f(i, a, j + step);
			glVertex3f(i + step, a, j + step);
			glVertex3f(i + step, a, j);
		}
	}
	glEnd();


	//front
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	for (float i = 0; i < a; i += step)
	{
		for (float j = -a / 2; j < a / 2; j += step)
		{
			glVertex3f(a / 2, i, j);
			glVertex3f(a / 2, i, j + step);
			glVertex3f(a / 2, i + step, j + step);
			glVertex3f(a / 2, i + step, j);
		}
	}
	glEnd();


	//back

	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	for (float i = 0; i < a; i += step)
	{
		for (float j = -a / 2; j < a / 2; j += step)
		{
			glVertex3f(-a / 2, i, j);
			glVertex3f(-a / 2, i, j + step);
			glVertex3f(-a / 2, i + step, j + step);
			glVertex3f(-a / 2, i + step, j);
		}
	}
	glEnd();



	glBegin(GL_QUADS);
	//left
	glNormal3f(0, 0, 1);
	for (float i = 0; i < a; i += step)
	{
		for (float j = -a / 2; j < a / 2; j += step)
		{
			glVertex3f(j, i, -a / 2);
			glVertex3f(j + step, i, -a / 2);
			glVertex3f(j + step, i + step, -a / 2);
			glVertex3f(j, i + step, -a / 2);
		}
	}
	glEnd();



	//right
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	for (float i = 0; i < a; i += step)
	{
		for (float j = -a / 2; j < a / 2; j += step)
		{
			glVertex3f(j, i, a / 2);
			glVertex3f(j + step, i, a / 2);
			glVertex3f(j + step, i + step, a / 2);
			glVertex3f(j, i + step, a / 2);
		}
	}
	glEnd();

}

void CGLRenderer::DrawStand()
{
	glClipPlane(GL_CLIP_PLANE0, new double[4]{ 0,1,0,0 });
	glEnable(GL_CLIP_PLANE0);
	glColor3f(0.8, 0.8, 0.8);
	DrawSphere(2.5);

	glTranslatef(0, 3, 0);
	glColor3f(0.8, 0.8, 0.8);
	DrawRoller(1, 2);

	glTranslatef(0, 1.5, 0);
	glColor3f(0.8, 0.8, 0.8);
	DrawPrism(4, 1);
	glDisable(GL_CLIP_PLANE0);

}

void CGLRenderer::DrawPrism(float radius, float height, int sides)
{

	float t_height = height / 2.0;

	glPushMatrix();
	glRotatef(45, 0, 1, 0);
	float step = (2 * C_PI) / sides;
	//Baza 1
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < sides; i++) {
		glNormal3f(0, 1, 0);
		glVertex3f(get_x(radius, i * step), t_height, get_y(radius, i * step));
	}
	glEnd();

	//Baza 2
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < sides; i++) {
		glNormal3f(0, -1, 0);
		glVertex3f(get_x(radius, i * step), -t_height, get_y(radius, i * step));
	}
	glEnd();

	//Omotac

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= sides; i++) {
		glNormal3f(get_x(1, i * step), 0, get_y(1, i * step));
		glVertex3f(get_x(radius, i * step), t_height, get_y(radius, i * step));

		glNormal3f(get_x(1, i * step), 0, get_y(1, i * step));
		glVertex3f(get_x(radius, i * step), -t_height, get_y(radius, i * step));
	}
	glEnd();

	glPopMatrix();
}

void CGLRenderer::DrawRoller(float radius, float height, int sides)
{
	float step = (2 * C_PI) / sides;
	float t_height = height / 2.0;

	//Baza 1
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < sides; i++) {
		glNormal3f(0, 1, 0);
		glVertex3f(get_x(radius, i * step), t_height, get_y(radius, i * step));
	}
	glEnd();

	//Baza 2
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < sides; i++) {
		glNormal3f(0, -1, 0);
		glVertex3f(get_x(radius, i * step), -t_height, get_y(radius, i * step));
	}
	glEnd();


	//Omotac
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= sides; i++) {
		glNormal3f(get_x(1, i * step), 0, get_y(1, i * step));
		glVertex3f(get_x(radius, i * step), t_height, get_y(radius, i * step));

		glNormal3f(get_x(1, i * step), 0, get_y(1, i * step));
		glVertex3f(get_x(radius, i * step), -t_height, get_y(radius, i * step));
	}
	glEnd();

}

void CGLRenderer::DrawSphere(float radius)
{
	int step = 3;
	glBegin(GL_QUAD_STRIP);
	for (int alpha = -90; alpha < 90; alpha += step) {
		for (int beta = 0; beta <= 360; beta++) {
			float normX = cos(beta * C_PI_RAD) * cos(alpha * C_PI_RAD);
			float normY = sin(alpha * C_PI_RAD);
			float normZ = sin(beta * C_PI_RAD) * cos(alpha * C_PI_RAD);

			glNormal3f(normX, normY, normZ);

			glVertex3f(radius * normX, radius * normY, radius * normZ);

			float normX1 = cos(beta * C_PI_RAD) * cos((alpha + step) * C_PI_RAD);
			float normY1 = sin((alpha + step) * C_PI_RAD);
			float normZ1 = sin(beta * C_PI_RAD) * cos((alpha + step) * C_PI_RAD);
			glNormal3f(normX1, normY1, normZ1);

			glVertex3f(radius * normX1, radius * normY1, radius * normZ1);
		}
	}
	glEnd();

}

void CGLRenderer::DrawVase()
{
	glPushMatrix();
	glTranslatef(0, 0.25, 0);
	glScalef(0.5, 0.5, 0.5);

	float step = 1;
	radius1 = 3;
	radius2 = 4;
	float height = 1;
	float pom;
	bool color = true;

	// 1. prsten
	if (color) cyan.Select(); else magenta.Select();
	color = !color;
	glTranslatef(0, step, 0);
	DrawStrip(radius2, radius1, height, 50); //rad 2 je donji krug i on je inicijalno veci cyan /______\

	// 2. prsten - malo manji magenta na dole
	// smanji oba radijusa
	DecRing(step); //rad1, rad2-=1 - hocemo isti smer samo da smanjimo oba kruga /_____\ da se gradi na gore
	if (color) cyan.Select(); else magenta.Select();
	color = !color;
	glTranslatef(0, step, 0);
	DrawStrip(radius2, radius1, height, 50);

	// 3. prsten
	radius2 -= step; //-hocemo da budu jednaki znaci smanjimo samo jedan (izgled stuba) |____|
	if (color) cyan.Select(); else magenta.Select();
	color = !color;
	glTranslatef(0, step, 0);
	DrawStrip(radius2, radius1, height, 50);

	// 4. prsten
	// magenta prsten, ne radimo nista hocemo da budu iste velicine! |____|
	if (color) cyan.Select(); else magenta.Select();
	color = !color;
	glTranslatef(0, step, 0);
	DrawStrip(radius2, radius1, height, 50);

	// 5. prsten
	// da bi sad cyan krug isao kao \____/ povecamao SAMO rad1!
	radius1 += step;
	if (color) cyan.Select(); else magenta.Select();
	color = !color;
	glTranslatef(0, step, 0);
	DrawStrip(radius2, radius1, height, 50);

	// 6. prsten
	// magenta prsten /______\ iste velicine
	//IncRing(step); ovo bi napravilo veci magenta \_______/
	radius2 += step;
	radius1 -= step;
	if (color) cyan.Select(); else magenta.Select();
	color = !color;
	glTranslatef(0, step, 0);
	DrawStrip(radius2, radius1, height, 50);

	// 7. prsten
	// opet cyan \______/ iste velicine kao malopre
	radius2 -= step;
	radius1 += step;
	if (color) cyan.Select(); else magenta.Select();
	color = !color;
	glTranslatef(0, step, 0);
	DrawStrip(radius2, radius1, height, 50);

	// 8. prsten
	// hocemo da se magenta nastavi isto na gore \_________/
	IncRing(step); //rad1,2+=1
	if (color) cyan.Select(); else magenta.Select();
	color = !color;
	glTranslatef(0, step, 0);
	DrawStrip(radius2, radius1, height, 50);

	// 9. prsten
	// hocemo cyan \______/ vece velicine
	IncRing(step); //rad1,2+=1	
	if (color) cyan.Select(); else magenta.Select();
	color = !color;
	glTranslatef(0, step, 0);
	DrawStrip(radius2, radius1, height, 50);

	// 10. prsten
	// hocemo /_____\ da nastavimo magenta ali manji 
	IncRing(step); //rad1,2+=1 
	if (color) cyan.Select(); else magenta.Select();
	color = !color;
	glTranslatef(0, step, 0);
	DrawStrip(radius2, radius1, height, 50);

	// 11. prsten
	// menjamo samo smer ali ista velicina za cyan \_____/ sada na gore
	pom = radius1; radius1 = radius2; radius2 = pom;
	if (color) cyan.Select(); else magenta.Select();
	color = !color;
	glTranslatef(0, step, 0);
	DrawStrip(radius2, radius1, height, 50);

	// 12. prsten
	// isti smer /_____\ 
	DecRing(step);
	if (color) cyan.Select(); else magenta.Select();
	color = !color;
	glTranslatef(0, step, 0);
	DrawStrip(radius2, radius1, height, 50);

	// 13. prsten
	// opet menjamo smer ali ista velicina za \____/ cyan
	pom = radius1; radius1 = radius2; radius2 = pom;
	if (color) cyan.Select(); else magenta.Select();
	color = !color;
	glTranslatef(0, step, 0);
	DrawStrip(radius2, radius1, height, 50);

	// 14. prsten
	// ne menjamo smer hocemo samo da povecamo \_______/ magenta 
	//pom = radius1; radius1 = radius2; radius2 = pom;
	IncRing(step);
	if (color) cyan.Select(); else magenta.Select();
	color = !color;
	glTranslatef(0, step, 0);
	DrawStrip(radius2, radius1, height, 50);

	glPopMatrix();
}

void CGLRenderer::DrawRing(float radius1, float radius2, float height)
{
	float step = C_PI_RAD * 2;
	float half = height / 2.0;
	glColor3f(0.9, 0.9, 0.9);
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < 180; i++)
	{
		float x1 = get_x(radius1, i * step);
		float y1 = half;
		float z1 = get_y(radius1, i * step);


		float x2 = get_x(radius1, (i + 1) * step);
		float y2 = -half;
		float z2 = get_y(radius1, (i + 1) * step);

		float x3 = get_x(radius2, i * step);
		float z3 = get_y(radius2, i * step);

		float x4 = get_x(radius2, (i + 1) * step);
		float z4 = get_y(radius2, (i + 1) * step);

		Vertex A = { radius1 * cos(i * step), half, radius1 * sin(i * step) };
		Vertex B = { radius2 * cos(i * step), -half, radius2 * sin(i * step) };
		Vertex vecraz = SubstractVectors(A, B);
		float hip = Scalar(vecraz);
		float stranica = abs(radius1 - radius2);
		float ugao = acos(stranica / hip);


		glNormal3f(cos(i * step), cos(ugao), sin(i * step));
		glVertex3f(x1, y1, z1);
		glNormal3f(cos(i * step), cos(ugao), sin(i * step));
		glVertex3f(x2, y1, z2);

		glNormal3f(cos(i * step), cos(ugao), sin(i * step));
		glVertex3f(x3, y2, z3);
		glNormal3f(cos(i * step), cos(ugao), sin(i * step));
		glVertex3f(x4, y2, z4);

	}
	glEnd();


	if (normals)
	{
		glDisable(GL_LIGHTING); //bez osvetljenja

		step = C_PI_RAD * 10;
		glColor3f(0, 1, 0);
		glBegin(GL_LINES); //za svaki par tacaka - linija
		for (int i = 0; i <= 36; i++) { //36 linija od 0 do 360 sa korakom 10

			//svaka tacka na r1 i r2

			Vertex A = { radius1 * cos(i * step), half, radius1 * sin(i * step) }; //y=half
			Vertex B = { radius2 * cos(i * step), -half, radius2 * sin(i * step) }; //y=-half
			Vertex vecraz = SubstractVectors(A, B);
			float hip = Scalar(vecraz);
			float stranica = abs(radius1 - radius2);
			float ugao = acos(stranica / hip);

			glVertex3f(radius1 * cos(i * step), half, radius1 * sin(i * step));
			glVertex3f(radius1 * cos(i * step) + cos(i * step), half + cos(ugao), radius1 * sin(i * step) + sin(i * step));

			glVertex3f(radius2 * cos(i * step), -half, radius2 * sin(i * step));
			glVertex3f(radius2 * cos(i * step) + cos(i * step), -half + cos(ugao), radius2 * sin(i * step) + sin(i * step));
		}
		glEnd();
		glEnable(GL_LIGHTING);
	}
}

void CGLRenderer::DrawStrip(double r1, double r2, double h, int nSeg)
{
	//traka izmedju r1 i r2 sa visinom h
	double step = 2 * C_PI / nSeg; //ugao koraka oko kruga

	int v = r2 > r1 ? -1 : 1;
	double r = std::abs(r1 - r2);
	double L = sqrt(r * r + h * h);
	double ny = r / L;
	double nr = h / L;

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= nSeg; i++) {
		glNormal3f(nr * cos(step * i), ny * v, nr * sin(step * i));
		glVertex3f(cos(step * i) * r1, 0, sin(step * i) * r1); //pozicije tacaka na unutrasnjem i spoljasnjem radijusu
		glVertex3f(cos(step * i) * r2, h, sin(step * i) * r2);
	}
	glEnd();

	if (normals) {
		//glNormal3f();
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);  //uzima verteks po verteks i spaja ih u linije par po par
		glColor3f(0, 1, 0); //boja normala
		for (int i = 0; i < nSeg + 1; i++) { //veci broj segmenata daje vise glatku povrsinu

			//tacka dole
			glVertex3f(cos(step * i) * r1, 0, sin(step * i) * r1);
			glVertex3f(cos(step * i) * r1 + cos(step * i) * nr, ny * v, sin(step * i) * r1 + sin(step * i) * nr);
			//tacka gore
			glVertex3f(cos(step * i) * r2, h, sin(step * i) * r2);
			glVertex3f(cos(step * i) * r2 + cos(step * i) * nr, h + ny * v, sin(step * i) * r2 + sin(step * i) * nr);
		}

		glEnd();
		glEnable(GL_LIGHTING);
	}
}

void CGLRenderer::IncRing(float step)
{
	radius1 += step;
	radius2 += step;
}

void CGLRenderer::DecRing(float step)
{
	radius1 -= step;
	radius2 -= step;
}

Vertex CGLRenderer::SubstractVectors(Vertex A, Vertex B)
{
	return Vertex() = { A.x - B.x, A.y - B.y, A.z - B.z };
}

Vertex CGLRenderer::CrossProductVectors(Vertex A, Vertex B)
{
	return Vertex() = { A.y * B.z - A.z * B.y,
						A.z * B.x - A.x * B.z,
						A.x * B.y - A.y * B.x };
}

float CGLRenderer::Scalar(Vertex A)
{
	return sqrtf(A.x * A.x + A.y * A.y + A.z * A.z);
}

Vertex CGLRenderer::Normalise(Vertex A)
{
	float sum = A.x + A.y + A.z;
	return Vertex() = { A.x / sum , A.y / sum ,A.z / sum };
}

Vertex CGLRenderer::CalculateNormal(Vertex A, Vertex B, Vertex C)
{
	Vertex X = SubstractVectors(B, A);
	Vertex Y = SubstractVectors(C, A);
	Vertex N = CrossProductVectors(X, Y);
	return Normalise(N);
}

void CGLRenderer::AmbientLight()
{
	//uniformno, osvetljava sve objekte
	GLfloat lmodel_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

}

void CGLRenderer::ViewPointLight()
{

	float mat[] = { 0.5, 0.5,0.5, 1.0 };

	GLfloat light_position[] = { 13, 39, 13, 0 };


	//Boja i intenzitet svetlosti
	glLightfv(GL_LIGHT1, GL_AMBIENT, mat);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, mat);
	glLightfv(GL_LIGHT1, GL_SPECULAR, mat);

	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 180);
	GLfloat spot_direction[] = { 0, -1, 0 };
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);

	//Aktiviranje
	glEnable(GL_LIGHT1);

}

void CGLRenderer::YellowLight()
{

	float light_ambient[] = { 1.0, 1.0, 0.0, 1.0 };
	float light_diffuse[] = { 1.0, 1.0, 0.0, 1.0 };
	float light_specular[] = { 1.0, 1.0, 0.0, 1.0 };

	GLfloat light_position[] = { 20, 15, 0,1 };


	//Boja i intenzitet svetlosti
	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);

	glLightfv(GL_LIGHT2, GL_POSITION, light_position);

	GLfloat spot_direction[] = { -1, 0, 0 };
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
	//Slabljenje
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0);

	//Usmerenje izvora
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 20);
	//glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 7.0);

	if (this->light0) {
		glPushMatrix();
		yellowSphere.Select();
		glTranslatef(18, 15, 0);
		DrawSphere(1);
		glPopMatrix();
		glEnable(GL_LIGHT2);
	}
	else
		glDisable(GL_LIGHT2);
}

void CGLRenderer::BlueLight()
{
	float light_ambient[] = { 0.0, 1.0, 1.0, 1.0 };  
	float light_diffuse[] = { 0.0, 1.0, 1.0, 1.0 };   
	float light_specular[] = { 0.0, 1.0, 1.0, 1.0 };  

	GLfloat light_position[] = { 0, 40, 0, 1 };

	//Boja i intenzitet svetlosti
	glLightfv(GL_LIGHT4, GL_AMBIENT, light_ambient); //light, name, params, ambijentalna kompozicija
	glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse); //difuzna
	glLightfv(GL_LIGHT4, GL_SPECULAR, light_specular); //

	glLightfv(GL_LIGHT4, GL_POSITION, light_position); //pozicija svetla

	GLfloat spot_direction[] = { 0, -1, 0.0 };

	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, spot_direction); //pravac reflektora
	//Slabljenje
	glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 1);

	//Usmerenje izvora
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 20);
	//glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 7.0);

	if (this->light2) {
		glPushMatrix();
		blueSphere.Select();
		glTranslatef(0, 38, 0);
		DrawSphere(1.5);
		glPopMatrix();
		glEnable(GL_LIGHT4);
	}
	else
		glDisable(GL_LIGHT4);
}

void CGLRenderer::RedLight()
{
	float light_ambient[] = { 1.0, 0.0, 0.0, 0.0 };   
	float light_diffuse[] = { 1.0, 0.0, 0.0, 0.0 };
	float light_specular[] = { 1.0, 0.0, 0.0, 0.0 };  

	GLfloat light_position[] = { -20, 15, 0, 1 }; //nalazi se levo u sobi


	//Boja i intenzitet svetlosti
	glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular);

	glLightfv(GL_LIGHT3, GL_POSITION, light_position); //izvor svetla
	GLfloat spot_direction[] = { 1, 0, 0.0 }; //gleda u desno
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction); //gde gleda reflektor
	//Slabljenje
	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1.0);

	//Usmerenje izvora
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 20); //ugao-levak svetlosti
	//glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 0);

	if (this->light1) {
		glPushMatrix();
		redSphere.Select();
		glTranslatef(-18, 15, 0);
		DrawSphere(1.5);
		glPopMatrix();
		glEnable(GL_LIGHT3); //ukljucuje izvor svetla
	}
	else
		glDisable(GL_LIGHT3);


}