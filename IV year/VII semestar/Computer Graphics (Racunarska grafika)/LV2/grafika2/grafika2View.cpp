#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "grafika2.h"
#endif

#include "grafika2Doc.h"
#include "grafika2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.14159
#define ZELENA 0x00cc00
#define LIME 0x00ff00
#define POMERAJ 5.625

// Cgrafika2View

IMPLEMENT_DYNCREATE(Cgrafika2View, CView)

BEGIN_MESSAGE_MAP(Cgrafika2View, CView)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Cgrafika2View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// Cgrafika2View construction/destruction

Cgrafika2View::Cgrafika2View() noexcept
{
	grid = false;

	cactus = GetEnhMetaFile(L"C:\\Users\\milena\\Downloads\\RESURSI\\cactus_part.emf");
	cactuslight = GetEnhMetaFile(L"C:\\Users\\milena\\Downloads\\RESURSI\\cactus_part_light.emf");

	ugao1 = 0;
	ugao2 = 0;
}

Cgrafika2View::~Cgrafika2View()
{
	if (cactus) DeleteEnhMetaFile(cactus);
	if (cactuslight) DeleteEnhMetaFile(cactuslight);
}

BOOL Cgrafika2View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

// Cgrafika2View drawing

void Cgrafika2View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xForm;
	xForm.eM11 = 1;
	xForm.eM12 = 0;
	xForm.eM21 = 0;
	xForm.eM22 = 1;
	xForm.eDx = dX;
	xForm.eDy = dY;

	DWORD mode = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&xForm, mode);
}

void Cgrafika2View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xForm;
	xForm.eM11 = sX;
	xForm.eM12 = 0;
	xForm.eM21 = 0;
	xForm.eM22 = sY;
	xForm.eDx = 0;
	xForm.eDy = 0;

	DWORD mode = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&xForm, mode);
}

void Cgrafika2View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float ugao = PI / 180 * angle;
	XFORM xForm;
	xForm.eM11 = cos(ugao);
	xForm.eM12 = sin(ugao);
	xForm.eM21 = -sin(ugao);
	xForm.eM22 = cos(ugao);
	xForm.eDx = 0;
	xForm.eDy = 0;

	DWORD mode = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	pDC->ModifyWorldTransform(&xForm, mode);
}

void Cgrafika2View::DrawFigure(CDC* pDC)
{
	Nebo(pDC);

	int n = (pow(3, 5) - 1) / 2;
	int* brDece = new int[n]();
	brDece[0] = 1; brDece[1] = 3; brDece[4] = 2; brDece[14] = 1; brDece[6] = 2; brDece[20] = 2;

	int nSavedDC = pDC->SaveDC();
	pDC->SetGraphicsMode(GM_ADVANCED);
	Translate(pDC, 250, 425, true);

	Kaktus(pDC, 0, brDece, 0, 18);
	pDC->RestoreDC(nSavedDC);

	Saksija(pDC);

	if (grid)
		Grid(pDC);
}

void Cgrafika2View::Grid(CDC* pDC)
{
	CPen pen(PS_SOLID, 2, RGB(180, 180, 180));
	CPen* oldPen = pDC->SelectObject(&pen);
	pDC->SetROP2(R2_MERGEPEN);

	for (int i = 0; i <= 500; i += 25) {
		pDC->MoveTo(0, i);
		pDC->LineTo(500, i);

		pDC->MoveTo(i, 0);
		pDC->LineTo(i, 500);
	}

	pDC->SelectObject(oldPen);
}

void Cgrafika2View::Nebo(CDC* pDC)
{
	CBrush brushPozadina(0xebce87);
	CBrush* oldBrush = pDC->SelectObject(&brushPozadina);
	pDC->SelectStockObject(NULL_PEN);
	pDC->Rectangle(0, 0, 500, 500);
	pDC->SelectObject(oldBrush);
	pDC->SelectStockObject(BLACK_PEN);
}

void Cgrafika2View::Saksija(CDC* pDC)
{
	CBrush brushSaksija(0x0094de);
	CBrush* oldBrush = pDC->SelectObject(&brushSaksija);
	CPoint saksija[4] = { {200, 450}, {300, 450}, {290, 499}, {210, 499} };
	pDC->Rectangle(190, 430, 310, 450);
	pDC->SelectStockObject(NULL_PEN);
	pDC->Polygon(saksija, 4);
	pDC->SelectStockObject(BLACK_PEN);
	pDC->MoveTo(200, 450);
	pDC->LineTo(210, 499);
	pDC->MoveTo(300, 450);
	pDC->LineTo(290, 499);
	pDC->SelectObject(oldBrush);
}

void Cgrafika2View::Grana(CDC* pDC, float x, float y, int brDece, float angle, int* boje)
{
	int nSavedDC = pDC->SaveDC();
	pDC->SetGraphicsMode(GM_ADVANCED);

	Translate(pDC, x, y, true);
	CBrush zelena(ZELENA);
	CBrush* oldBrush = pDC->SelectObject(&zelena);
	pDC->Ellipse(-10, -10, 10, 10);
	pDC->SelectObject(oldBrush);

	pDC->RestoreDC(nSavedDC);

	float* angles;
	float k = 1.0f;
	if (brDece == 1) { angles = new float[1]; angles[0] = 0; }
	else if (brDece == 2) { angles = new float[2]; angles[0] = -45; angles[1] = 45; k = 2.0f / 3; }
	else if (brDece == 3) { angles = new float[3]; angles[0] = -45; angles[1] = 0; angles[2] = 45; k = 1.0f / 3; }
	else return;

	for (int i = 0; i < brDece; i++) {
		int nBranchSavedDC = pDC->SaveDC();
		pDC->SetGraphicsMode(GM_ADVANCED);

		Scale(pDC, 1.0f * k , 1, true);
		Rotate(pDC, angles[i] + angle, true);
		Translate(pDC, x, y, true);

		CBrush brush(boje[i]);
		oldBrush = pDC->SelectObject(&brush);
		pDC->Ellipse(-23, -65, 23, -10);
		pDC->Ellipse(-12, -65, 12, -10);
		pDC->SelectObject(oldBrush);

		pDC->RestoreDC(nBranchSavedDC);
	}
	delete[] angles;
}

CPoint Cgrafika2View::Centar(CDC* pDC)
{
	XFORM currentXForm;
	pDC->GetWorldTransform(&currentXForm);
	CPoint centar;
	centar.x = currentXForm.eDx;
	centar.y = currentXForm.eDy;
	return centar;
}

void Cgrafika2View::Kaktus(CDC* pDC, int ind, int* brDece, int lime1, int lime2)
{
	float* angles;
	float k = 1.0f;
	if (brDece[ind] == 1) { angles = new float[1]; angles[0] = 0; }
	else if (brDece[ind] == 2) { angles = new float[2]; angles[0] = -45; angles[1] = 45; k = 2.0f / 3; }
	else if (brDece[ind] == 3) { angles = new float[3]; angles[0] = -45; angles[1] = 0; angles[2] = 45; k = 1.0f / 3; }
	else return;

	for (int i = 0; i < brDece[ind]; i++) {
		int nRotateSavedDC = pDC->SaveDC();
		pDC->SetGraphicsMode(GM_ADVANCED);

		CPoint centar = Centar(pDC);
		float ugao = angles[i];
		if (ind == lime1) 
			ugao += ugao1;
		if (ind * 3 + i == lime2) 
			ugao += ugao2;

		Translate(pDC, -centar.x, -centar.y, true);
		Rotate(pDC, ugao, true);

		int nScaleSavedDC = pDC->SaveDC();
		pDC->SetGraphicsMode(GM_ADVANCED);

		Scale(pDC, k, 1, false);
		Translate(pDC, centar.x, centar.y, true);

		HENHMETAFILE grana = NULL;
		if (ind * 3 + i == lime1 || ind * 3 + i == lime2) 
			grana = cactuslight;
		else 
			grana = cactus;
		CRect rect(-30, -75, 30, 0);
		if (grana)
			pDC->PlayMetaFile(grana, rect);

		pDC->RestoreDC(nScaleSavedDC);
		Translate(pDC, centar.x, centar.y, true);

		if (brDece[ind * 3 + i + 1] != 0) {
			int nRecurseSavedDC = pDC->SaveDC();
			pDC->SetGraphicsMode(GM_ADVANCED);

			Translate(pDC, 0, -75, false);
			Kaktus(pDC, ind * 3 + i + 1, brDece, lime1, lime2);

			pDC->RestoreDC(nRecurseSavedDC);
		}
		pDC->RestoreDC(nRotateSavedDC);
	}

	delete[] angles;

	CBrush zelena(ZELENA);
	CBrush* oldBrush = pDC->SelectObject(&zelena);
	pDC->Ellipse(-10, -10, 10, 10);
	pDC->SelectObject(oldBrush);
}

void Cgrafika2View::OnDraw(CDC* pDC)
{
	Cgrafika2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	DrawFigure(pDC);
}


// Cgrafika2View printing


void Cgrafika2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Cgrafika2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cgrafika2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cgrafika2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void Cgrafika2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Cgrafika2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Cgrafika2View diagnostics

#ifdef _DEBUG
void Cgrafika2View::AssertValid() const
{
	CView::AssertValid();
}

void Cgrafika2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cgrafika2Doc* Cgrafika2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cgrafika2Doc)));
	return (Cgrafika2Doc*)m_pDocument;
}
#endif //_DEBUG


// Cgrafika2View message handlers


void Cgrafika2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'G') {
		grid = !grid;
		Invalidate();
	}

	if (nChar == 'L') {
		ugao1 -= POMERAJ;
		Invalidate();
	}

	if (nChar == 'R') {
		ugao1 += POMERAJ;
		Invalidate();
	}

	if (nChar == 'Q') {
		ugao2 -= POMERAJ;
		Invalidate();
	}

	if (nChar == 'W') {
		ugao2 += POMERAJ;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
