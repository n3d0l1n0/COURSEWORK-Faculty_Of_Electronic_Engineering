
// GLKView.cpp : implementation of the CGLKView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GLK.h"
#endif

#include "GLKDoc.h"
#include "GLKView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGLKView

IMPLEMENT_DYNCREATE(CGLKView, CView)

BEGIN_MESSAGE_MAP(CGLKView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CGLKView construction/destruction

CGLKView::CGLKView()
{
	// TODO: add construction code here
	is_clicked = false;
}

CGLKView::~CGLKView()
{
}

BOOL CGLKView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGLKView drawing

void CGLKView::OnDraw(CDC* pDC) //celo T
{
	CGLKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_glRenderer.DrawScene(pDC);
}


// CGLKView printing

BOOL CGLKView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGLKView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGLKView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGLKView diagnostics

#ifdef _DEBUG
void CGLKView::AssertValid() const
{
	CView::AssertValid();
}

void CGLKView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGLKDoc* CGLKView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGLKDoc)));
	return (CGLKDoc*)m_pDocument;
}
#endif //_DEBUG


// CGLKView message handlers


int CGLKView::OnCreate(LPCREATESTRUCT lpCreateStruct) //T
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}


void CGLKView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void CGLKView::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}


BOOL CGLKView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CGLKView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}


void CGLKView::OnLButtonDown(UINT nFlags, CPoint point)
{
	is_clicked = true;
	old_point = point;
	SetCapture();
	CView::OnLButtonDown(nFlags, point);
}

void CGLKView::OnLButtonUp(UINT nFlags, CPoint point)
{
	is_clicked = false;
	ReleaseCapture();
	CView::OnLButtonUp(nFlags, point);
}

float CGLRenderer::GetCamAngleH() { return cam_angle_h; }
float CGLRenderer::GetCamAngleV() { return cam_angle_v; }

void CGLRenderer::SetCamAngles(float h, float v) {
	cam_angle_h = h;
	cam_angle_v = v;
}

void CGLKView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (is_clicked) {
		float h_angle = m_glRenderer.GetCamAngleH();
		float v_angle = m_glRenderer.GetCamAngleV();

		int diff_x = point.x - old_point.x;
		int diff_y = point.y - old_point.y;

		old_point = point;

		h_angle += diff_x * 0.2f;
		v_angle += diff_y * 0.2f;

		if (v_angle > 89.0f) v_angle = 89.0f;
		if (v_angle < -89.0f) v_angle = -89.0f;

		m_glRenderer.SetCamAngles(h_angle, v_angle);
		Invalidate(); 
	}
	CView::OnMouseMove(nFlags, point);
}


void CGLKView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	int step = 5;
	switch (nChar)
	{
	case 'A':
	case 'a':
		m_glRenderer.RotateYellow(step);
		Invalidate();
		break;
	case 'D':
	case 'd':
		m_glRenderer.RotateYellow(-step);
		Invalidate();
		break;
	case VK_UP:
		m_glRenderer.AngleCX(step);
		Invalidate();
		break;
	case VK_DOWN:
		m_glRenderer.AngleCX(-step);
		Invalidate();
		break;
	case VK_LEFT:
		m_glRenderer.AngleCY(-step);
		Invalidate();
		break;
	case VK_RIGHT:
		m_glRenderer.AngleCY(step);
		Invalidate();
		break;
	default:
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
