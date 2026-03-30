#pragma once

class CGLRenderer
{
public:
	float cam_radius;      // udaljenost kamere od centra scene
	float cam_angle_h = 0;     // horizontalni ugao (oko Y ose)
	float cam_angle_v = 0;     // vertikalni ugao (od -90 do +90)
	float angle_yellow;

	CGLRenderer(void);
	virtual ~CGLRenderer(void);

	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void DrawPrism(float radius, float height = 0.3, int sides = 4);
	void DrawSphere(double r, int nSegAlpha, int nSegBeta);
	void DrawCone(double h, double r, int nSeg);
	void DrawCylinder(double h, double r1, double r2, int nSeg);
	void DrawAxis(double width);
	void DrawGrid(double width, double height, int nSegW, int nSegH);
	void SetCamera();
	void SetCamAngles(float h, float v);
	float GetCamAngleH();
	float GetCamAngleV();
	void RightMiddleBranch();
	void LeftMiddleBranch();
	void RotateYellow(float angle);

	CPoint GetAngle() {
		return CPoint(angle_v, angle_h);
	}
	void SetAngles(float angle_h, float angle_v);

	inline void AngleCX(int angle) {
		angle_cx += angle;
	}

	inline void AngleCY(int angle) {
		angle_cy += angle;
	}

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	void DrawPot();
	void DrawFigure();

	float get_x(float radius, float angle);
	float get_y(float radius, float angle);

	float angle_v;
	float angle_h;

	float angle_cx;
	float angle_cy;

	const double C_PI = 3.14159265358979323846;
	const double C_PI_RAD = 3.14159265358979323846 / 180.0;

	void MiddleBranch();
	void RightBranch();
	void LeftBranch();
};
