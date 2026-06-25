
// GrafikaLabView.cpp : implementation of the CGrafikaLabView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GrafikaLab.h"
#endif

#include "GrafikaLabDoc.h"
#include "GrafikaLabView.h"

#define _USE_MATH_DEFINES
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGrafikaLabView

IMPLEMENT_DYNCREATE(CGrafikaLabView, CView)

BEGIN_MESSAGE_MAP(CGrafikaLabView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CGrafikaLabView construction/destruction

CGrafikaLabView::CGrafikaLabView() noexcept
{
	//handleri na metafajlove
	this->partDark = GetEnhMetaFile(CString("cactus_part.emf"));
	this->partLight = GetEnhMetaFile(CString("cactus_part_light.emf"));

	this->angle2 = 0;
	this->angle1 = 0;

	this->gridOn = false;
}

CGrafikaLabView::~CGrafikaLabView()
{
}

BOOL CGrafikaLabView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void Translate(CDC* pDC, float dx, float dy, bool rightMultiply) 
{
	XFORM transformation;
	/*
	[ 1 0 dx ]   [ eM11  eM12  eDx ]
	[ 0 1 dy ] = [ eM21  eM22  eDy ] - XFORM matrica (opisuje transformacije)
	[ 0 0 1  ]   [  0     0     1  ]

		[ x']   [ eM11  eM12  eDx ] [x]
	p'= [ y'] = [ eM21  eM22  eDy ] [y]
		[ 1 ]   [  0     0     1  ] [1]

		x' = eM11*x + eM12*y + eDx = 1*x + 0*y + dx = x + dx
		y' = eM21*x + eM22*y + eDy = 0*x + 1*y + dy = y + dy

	*/
	transformation.eM11 = 1;
	transformation.eM12 = 0;
	transformation.eM21 = 0;
	transformation.eM22 = 1;
	transformation.eDx = dx;
	transformation.eDy = dy;
	if (rightMultiply) //mnozenje matrica nije komutativno!!
		pDC->ModifyWorldTransform(&transformation, MWT_RIGHTMULTIPLY);  
	else pDC->ModifyWorldTransform(&transformation, MWT_LEFTMULTIPLY); 
}

void Scale(CDC* pDC, float dx, float dy, bool rightMultiply) {

	/*
	[ dx  0  0 ]
	[  0 dy  0 ]
	[  0  0  1 ]

	efekat: 
		x' = dx * x
		y' = dy * y
	*/
	XFORM transformation;
	transformation.eM11 = dx;
	transformation.eM12 = 0;
	transformation.eM21 = 0;
	transformation.eM22 = dy;
	transformation.eDx = 0;
	transformation.eDy = 0;
	if (rightMultiply)
		pDC->ModifyWorldTransform(&transformation, MWT_RIGHTMULTIPLY);
	else pDC->ModifyWorldTransform(&transformation, MWT_LEFTMULTIPLY);
}
                                   //UVEK FALSE
void Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	/*
		[  cos -sin  0 ]
		[  sin  cos  0 ]
		[   0    0   1 ]

		efekat: 
			x' = x*cos - y*sin
			y' = x*sin + y*cos

	*/
	XFORM transformation;
	transformation.eM11 = cos(angle);
	transformation.eM12 = -sin(angle);
	transformation.eM21 = sin(angle);
	transformation.eM22 = cos(angle);
	transformation.eDx = 0;
	transformation.eDy = 0;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&transformation, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&transformation, MWT_LEFTMULTIPLY);
}

// CGrafikaLabView drawing

void CGrafikaLabView::OnDraw(CDC* pDC)
{
	CGrafikaLabDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//standardni koordinatni pocetak (0,0) je u gornjem levom uglu
	CRect rect;
	GetClientRect(&rect);
	pDC->SetGraphicsMode(GM_ADVANCED); //omogucili smo napredne transformacije rotaciju, skaliranje i translaciju
	pDC->SetMapMode(MM_ISOTROPIC); //jedinice na X i Y ostaju iste velicine, sprecava deformaciju slike pri promeni velicine prozora
	pDC->SetWindowExt(500, 500); //logicki prozor - moje platno za crtanje je 500x500 bez obzira na stvarnu velicinu prozora
	pDC->SetViewportExt(rect.Width(), -rect.Height()); //preslikavanje logickog prozora na fizicki prozor, - kako bi Y vrednosti rasle nagore a ne nadole
	pDC->SetViewportOrg(0, rect.Height());	//koordinatni pocetak u donjem levom uglu
	XFORM old;
	GetWorldTransform(pDC->m_hDC, &old); //cuvamo osnovnu transformaciju, kako bi se vratili u pocetno stanje posle!

	//background
	CBrush backgroundBrush(RGB(135, 206, 235));
	CBrush* oldBrush = pDC->SelectObject(&backgroundBrush);

	CPen backgroundPen(PS_SOLID, 0, RGB(221, 221, 221));
	CPen* oldPen = pDC->SelectObject(&backgroundPen);

	pDC->Rectangle(0, 0, 500, 500); //samo 500x500 treba da bude plavo
	//cactus
	CBrush cactusBrush(RGB(0, 204, 0));
	oldBrush = pDC->SelectObject(&cactusBrush);

	CPen cactusPen(PS_SOLID, 1, RGB(0, 0, 0));
	oldPen = pDC->SelectObject(&cactusPen);
	
	//M_PI=180 
	//Rotate(pDC, M_PI, false);
	//Translate(pDC, -500, -500, false); za rotaciju kaktusa 
	DrawCactus(pDC, old);
	
	//pot
	//Rotate(pDC, M_PI , false);
	//Translate(pDC, -500, -500, false); za rotaciju vaze
	DrawPot(oldBrush, pDC, oldPen);

	//grid
	DrawGrid(oldPen, pDC);

	GetClientRect(&rect);
	pDC->SetGraphicsMode(GM_ADVANCED); //omogucili smo napredne transformacije rotaciju, skaliranje i translaciju
	pDC->SetMapMode(MM_ISOTROPIC); //jedinice na X i Y ostaju iste velicine, sprecava deformaciju slike pri promeni velicine prozora
	pDC->SetWindowExt(500, 500); //logicki prozor - moje platno za crtanje je 500x500 bez obzira na stvarnu velicinu prozora
	pDC->SetViewportExt(rect.Width(), rect.Height()); //preslikavanje logickog prozora na fizicki prozor, - kako bi Y vrednosti rasle nagore a ne nadole
	pDC->SetViewportOrg(0, 0);	//koordinatni pocetak u donjem levom uglu
	GetWorldTransform(pDC->m_hDC, &old);
	//text
	DrawText(pDC);

	SetWorldTransform(pDC->m_hDC, &old);
}

void CGrafikaLabView::DrawCactus(CDC* pDC, XFORM& old)
{
	Translate(pDC, 75, 250, false); 
	Rotate(pDC, M_PI / 2, false);
	pDC->PlayMetaFile(this->partDark, CRect(-30, 0, 30, 75)); //crtamo donji/glavni deo kaktusa u odnosu na novi/pomereni koordinatni pocetak
	//pDC-platno na kom se crta, partDark metafajl koji crtamo unutar pravougaonika sirine: -30, 0 i visine: 30, 75

	DrawFigure(pDC);
	SetWorldTransform(pDC->m_hDC, &old);

}

void CGrafikaLabView::DrawFigure(CDC* pDC)
{
	pDC->Ellipse(10, 10, -10, -10);
	Translate(pDC, 0, 75, false); //pomeramo se na vrh glavnog kaktusa (250,75)+(0,75)=(250,150)
	pDC->Ellipse(-10, -10, 10, 10);

	XFORM baseState;
	pDC->GetWorldTransform(&baseState); //stanje na prvom spoju 

	//tanka leva grana 
	{
		Rotate(pDC, 3 * M_PI / 4, false); //ugao tanke grane levo: pocetna pozicija joj je na 135 stepeni, pa je grana u sredini + pi/4 pa je grana desno + pi/4
		Scale(pDC, 0.75f, 1.0f, false); // sirina 1.5x, visina ostaje 1x
		Rotate(pDC, this->angle2, false); //vrtimo CEO levi deo
		pDC->PlayMetaFile(this->partLight, CRect(-10, 0, 10, -75)); //crtamo tanku levu granu
		Scale(pDC, 1.0f / 0.75f, 1.0f, false);
		Translate(pDC, 0, -75, false); //pomeramo se na vrh tanke LEVE grane 
		pDC->Ellipse(-10, -10, 10, 10); //elipsa na vrhu tanke leve grane
		Translate(pDC, 0, -75, false);
		pDC->PlayMetaFile(this->partDark, CRect(-30, 0, 30, 75));
		pDC->Ellipse(-10, -10, 10, 10);
		Rotate(pDC, 7 * M_PI / 4, false);
		Scale(pDC, 1.0f, 1.2f, false); 
		pDC->PlayMetaFile(this->partDark, CRect(-20, 0, 20, -60)); //grana na levo
		Rotate(pDC, 2 * M_PI / 4, false);
		Scale(pDC, 1.0f, 1.2f, false); 
		Rotate(pDC, this->angle1, false);
		pDC->PlayMetaFile(this->partLight, CRect(-20, 0, 20, -60)); //uspravna grana
	}
	pDC->SetWorldTransform(&baseState); //vrati org stanje

	//tanka desna grana 
	{
		Rotate(pDC, 5* M_PI / 4, false);
		Scale(pDC, 0.75f, 1.0f, false); // sirina 1.5x, visina ostaje 1x
		pDC->PlayMetaFile(this->partDark, CRect(-10, 0, 10, -75));
		Scale(pDC, 1.0f / 0.75f, 1.0f, false);
		
	}
	pDC->SetWorldTransform(&baseState);
	
	//tanka grana u sredini 
	{
		Rotate(pDC, M_PI, false);
		Scale(pDC, 0.75f, 1.0f, false); // sirina 1.5x, visina ostaje 1x
		pDC->PlayMetaFile(this->partDark, CRect(-10, 0, 10, -75));
		Scale(pDC, 1.0f / 0.75f, 1.0f, false);
		Translate(pDC, 0, -75, false);
		pDC->Ellipse(-10, -10, 10, 10);
		//grananje tri tanke grane

		Rotate(pDC, -M_PI/4, false);
		Scale(pDC, 0.75f, 1.0f, false); 
		pDC->PlayMetaFile(this->partDark, CRect(-10, 0, 10, -75)); //grana levo
		Scale(pDC, 1.0f / 0.75f, 1.0f, false);
		Rotate(pDC, M_PI/4, false);
		Scale(pDC, 0.75f, 1.0f, false);
		pDC->PlayMetaFile(this->partDark, CRect(-10, 0, 10, -75));//grana u sredini
		Scale(pDC, 1.0f / 0.75f, 1.0f, false);
		Translate(pDC, 0, -75, false);
		pDC->Ellipse(-10, -10, 10, 10);
		Translate(pDC, 0, -75, false);
		pDC->PlayMetaFile(this->partDark, CRect(-30, 0, 30, 75));
		Translate(pDC, 0, 150, false);
		Rotate(pDC, M_PI / 4, false);
		Scale(pDC, 0.75f, 1.0f, false); 
		pDC->PlayMetaFile(this->partDark, CRect(-10, 0, 10, -75));//grana desno
		Scale(pDC, 1.0f / 0.75f, 1.0f, false);
 
	}
	pDC->SetWorldTransform(&baseState);
}


void CGrafikaLabView::DrawPot(CBrush*& oldBrush, CDC* pDC, CPen*& oldPen)
{
	CBrush potBrush(RGB(222, 148, 0));
	oldBrush = pDC->SelectObject(&potBrush);

	CPen potPen(PS_SOLID, 1, RGB(0, 0, 0));
	oldPen = pDC->SelectObject(&potPen);

	POINT potBasePoints[] = { 0, 210, 50, 200, 50, 300, 0, 290 };
	pDC->Polygon(potBasePoints, 4);
	pDC->Rectangle(50, 190, 72, 310); 
}

void CGrafikaLabView::DrawText(CDC* pDC)
{
	COLORREF oldColor = pDC->SetTextColor(RGB(255, 255, 0));
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);

	CFont font;
	font.CreateFont(35, 0, -900, -900, 0, FALSE, FALSE, 0, 0, 0, 0, 0, 0, L"Arial");

	CFont* oldFont = pDC->SelectObject(&font);

	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->TextOut(477, 27, CString("Biljka")); 

	pDC->SetTextColor(RGB(255, 255, 0));
	pDC->TextOut(475, 25, CString("Biljka"));

	pDC->SelectObject(oldFont);
	pDC->SetTextColor(oldColor);
	pDC->SetBkMode(oldBkMode);
	font.DeleteObject();
}

void CGrafikaLabView::DrawGrid(CPen*& oldPen, CDC* pDC)
{
	CPen gridPen(PS_SOLID, 1, RGB(255, 255, 255));
	oldPen = pDC->SelectObject(&gridPen);

	if (this->gridOn) {
		int pos = 0;
		for (int i = 0; i <= 20; i++)
		{
			pDC->MoveTo(pos, 0);
			pDC->LineTo(pos, 500);
			pDC->MoveTo(0, pos);
			pDC->LineTo(500, pos);
			pos += 25;
		}
	}
}


// CGrafikaLabView printing

BOOL CGrafikaLabView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGrafikaLabView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGrafikaLabView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGrafikaLabView diagnostics

#ifdef _DEBUG
void CGrafikaLabView::AssertValid() const
{
	CView::AssertValid();
}

void CGrafikaLabView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGrafikaLabDoc* CGrafikaLabView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGrafikaLabDoc)));
	return (CGrafikaLabDoc*)m_pDocument;
}
#endif //_DEBUG


// CGrafikaLabView message handlers


void CGrafikaLabView::OnLButtonDown(UINT nFlags, CPoint point)
{
	this->gridOn = !this->gridOn;
	Invalidate();

	CView::OnLButtonDown(nFlags, point);
}


void CGrafikaLabView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	if (nChar == 'D')
	{
		this->angle1 += M_PI / 12;

		Invalidate();
	}

	if (nChar == 'L')
	{
		this->angle1 -= M_PI / 12;

		Invalidate();
	}

	if (nChar == 'R')
	{
		this->angle2 += M_PI / 12;

		Invalidate();
	}

	if (nChar == 'E')
	{
		this->angle2 -= M_PI / 12;

		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
