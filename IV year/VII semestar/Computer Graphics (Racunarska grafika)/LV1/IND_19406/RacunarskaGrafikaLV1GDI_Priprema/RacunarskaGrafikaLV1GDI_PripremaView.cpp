#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "RacunarskaGrafikaLV1GDI_Priprema.h"
#endif

#include "RacunarskaGrafikaLV1GDI_PripremaDoc.h"
#include "RacunarskaGrafikaLV1GDI_PripremaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <vector>

IMPLEMENT_DYNCREATE(CRacunarskaGrafikaLV1GDIPripremaView, CView)

BEGIN_MESSAGE_MAP(CRacunarskaGrafikaLV1GDIPripremaView, CView)
	ON_WM_KEYDOWN() //za taster G kad se pali gasi grid 
	ON_WM_ERASEBKGND() //siva pozadina 
END_MESSAGE_MAP()

CRacunarskaGrafikaLV1GDIPripremaView::CRacunarskaGrafikaLV1GDIPripremaView() noexcept
{
	showGrid = TRUE;
}

CRacunarskaGrafikaLV1GDIPripremaView::~CRacunarskaGrafikaLV1GDIPripremaView()
{
}

BOOL CRacunarskaGrafikaLV1GDIPripremaView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

BOOL CRacunarskaGrafikaLV1GDIPripremaView::OnEraseBkgnd(CDC* pDC)
{
    //pozadina je u primeru siva idk 

	CRect rect;
	GetClientRect(&rect); //uzima velicinu celog prozora
	pDC->FillSolidRect(&rect, RGB(211, 211, 211)); 
	return TRUE;
}

void CRacunarskaGrafikaLV1GDIPripremaView::OnDraw(CDC* pDC)
{
	DrawShapes(pDC);
	if (showGrid)
		DrawGrid(pDC);
}

void SelectPenBrush(CDC* pDC, CPen& pen, CBrush& brush, CPen*& oldPen, CBrush*& oldBrush)
{
    oldPen = pDC->SelectObject(&pen);
    oldBrush = pDC->SelectObject(&brush);
}

void DrawPoly(CDC* pDC, CPoint* pts, int count, COLORREF penColor, COLORREF brushColor, int penWidth = 4)
{
    //za trougao/pravougaonik 

    CPen pen(PS_SOLID, penWidth, penColor);
    CBrush brush(brushColor);
    CPen* oldPen;
    CBrush* oldBrush;

    SelectPenBrush(pDC, pen, brush, oldPen, oldBrush);
    pDC->Polygon(pts, count);
    pDC->SelectObject(oldBrush);
    pDC->SelectObject(oldPen);
}

void CRacunarskaGrafikaLV1GDIPripremaView::DrawShapes(CDC* pDC)
{
    /*
    int cellSize = 25;

    CPen* oldPen;
    CBrush* oldBrush;

    // veliki pravougaonik
    {
        CPen outline(PS_SOLID, 4, RGB(0, 255, 255));
        CBrush emptyBrush;
        emptyBrush.CreateStockObject(NULL_BRUSH);

        SelectPenBrush(pDC, outline, emptyBrush, oldPen, oldBrush);
        pDC->Rectangle(1 * cellSize, 3 * cellSize, 500 - cellSize, 500 - 3 * cellSize);
        pDC->SelectObject(oldBrush);
        pDC->SelectObject(oldPen);
    }


    // crveni trougao
    {
        int width = 9 * cellSize;
        int height = 4.5 * cellSize;
        int center = left + width / 2;

        CPoint pts[3] = {
            {center - width / 2, top},
            {center + width / 2, top},
            {center, top + height}
        };

        DrawPoly(pDC, pts, 3, RGB(0, 255, 255), RGB(255, 0, 0));

        // pravilni sedmougao
        CPen pen(PS_SOLID, 3, RGB(0, 255, 255));
        CBrush brush(RGB(255, 0, 0));
        SelectPenBrush(pDC, pen, brush, oldPen, oldBrush);

        DrawRegularPolygon(pDC, center,top + height / 3,(int)(min(width, height) / 4),7,-90.f);

        pDC->SelectObject(oldBrush);
        pDC->SelectObject(oldPen);
    }

    // 2) ljubicasti trougao
    {
        int width = 9 * cellSize;
        int center = left + width / 2;

        CPoint pts[3] = {
            {center + width / 2, top},
            {center + width * 3 / 2, top},
            {right, top + 9 * cellSize}
        };

        DrawPoly(pDC, pts, 3, RGB(0, 255, 255), RGB(128, 0, 128));

        //pravilni petougao
        int X = (center + width / 2 + center + width * 3 / 2 + right) / 3;
        int Y = (3 * top + 9 * cellSize) / 3;
        int R = (int)(min(width, 4.5 * cellSize) / 3);

        CPen pen(PS_SOLID, 3, RGB(0, 255, 255));
        CBrush brush(RGB(128, 0, 128));
        SelectPenBrush(pDC, pen, brush, oldPen, oldBrush);

        DrawRegularPolygon(pDC, X, Y, R, 5, -60.f);

        pDC->SelectObject(oldBrush);
        pDC->SelectObject(oldPen);
    }


    // roze trougao
    {
        CPoint pts[3] = {
            { (int)(right - 4.5 * cellSize), (int)(top + 4.5 * cellSize) },
            { (int)(right - 4.5 * cellSize), (int)(top + 9 * cellSize) },
            { (int)right, (int)(top + 9 * cellSize) }
        };

        DrawPoly(pDC, pts, 3, RGB(0, 255, 255), RGB(255, 182, 193));

        //pravilni cetvorougao
        int X = (int)((right - 4.5 * cellSize + right - 4.5 * cellSize + right) / 3);
        int Y = (int)((3 * top + 22.5 * cellSize) / 3);
        int R = (int)(9 * cellSize / 10);

        CPen pen(PS_SOLID, 3, RGB(0, 255, 255));
        CBrush brush(RGB(255, 182, 193));
        SelectPenBrush(pDC, pen, brush, oldPen, oldBrush);

        DrawRegularPolygon(pDC, X, Y, R, 4, -120.f);

        pDC->SelectObject(oldBrush);
        pDC->SelectObject(oldPen);
    }


    // beli pravougaonik
    {
        CRect rect(
            (int)(left + 13.5 * cellSize),
            (int)(top + 9 * cellSize),
            (int)right,
            (int)bottom
        );

        CPen pen(PS_SOLID, 4, RGB(0, 255, 255));
        CBrush white(RGB(255, 255, 255));
        SelectPenBrush(pDC, pen, white, oldPen, oldBrush);
        pDC->Rectangle(rect);

        pDC->SetBkColor(RGB(255, 255, 255));
        pDC->SetBkMode(OPAQUE);

        CBrush hatchBrush(HS_FDIAGONAL, RGB(0, 255, 255));
        pDC->SelectObject(&hatchBrush);

        pDC->Rectangle(rect);

        pDC->SelectObject(oldBrush);
        pDC->SelectObject(oldPen);
    }


    // zuti trougao
    {
        CPoint pts[3] = {
            { (int)(left + 13.5 * cellSize), (int)(top + 9 * cellSize) },
            { (int)(right - 4.5 * cellSize), (int)(top + 14 * cellSize) },
            { (int)(left + 9 * cellSize), (int)(top + 14 * cellSize) }
        };
        DrawPoly(pDC, pts, 3, RGB(0, 255, 255), RGB(255, 255, 0));

        //pravilni sestougao
        int Xs = (int)((2 * left + right + 18 * cellSize) / 3);
        int Ys = (int)((3 * top + 37 * cellSize) / 3);
        int Rs = (int)(9 * cellSize / 10);

        CPen pen(PS_SOLID, 3, RGB(0, 255, 255));
        CBrush brush(RGB(255, 255, 0));
        SelectPenBrush(pDC, pen, brush, oldPen, oldBrush);

        DrawRegularPolygon(pDC, Xs, Ys, Rs, 6, -90.f);

        pDC->SelectObject(oldBrush);
        pDC->SelectObject(oldPen);
    }


    // zeleni trougao
    {
        CPoint pts[3] = {
            { (int)left, (int)(top + 5 * cellSize) },
            { (int)(left + 9 * cellSize), (int)bottom },
            { (int)left, (int)bottom }
        };
        DrawPoly(pDC, pts, 3, RGB(0, 255, 255), RGB(0, 255, 0));

        //pravilni sedmougao
        int X = (int)((3 * left + 9 * cellSize) / 3);
        int Y = (int)((top + 5 * cellSize + 2 * bottom) / 3);
        int R = (int)(9 * cellSize / 6);

        CPen pen(PS_SOLID, 3, RGB(0, 255, 255));
        CBrush brush(RGB(0, 255, 0));
        SelectPenBrush(pDC, pen, brush, oldPen, oldBrush);

        DrawRegularPolygon(pDC, X, Y, R, 7, -60.f);

        pDC->SelectObject(oldBrush);
        pDC->SelectObject(oldPen);
    }


    // narandzasti romb
    {
        CPoint pts[4] = {
            { (int)left, (int)top },
            { (int)left, (int)(top + 5 * cellSize) },
            { (int)(left + 4.5 * cellSize), (int)(top + 9.5 * cellSize) },
            { (int)(left + 4.5 * cellSize), (int)(top + 4.5 * cellSize) }
        };
        DrawPoly(pDC, pts, 4, RGB(0, 255, 255), RGB(255, 165, 0));
    }
    */
    //beli trougao 
    int cellSize = 25;
    CPen* oldPen;
    CBrush* oldBrush;

    CPoint pts[3] = {
        { (int)5.5 * cellSize, (int)10 * cellSize },
            { (int)11.5 * cellSize, (int)10 * cellSize },
            { (int)11.5 * cellSize, (int)4*cellSize }, };

    CPen pen6(PS_SOLID, 3, RGB(0, 0, 255));
    CBrush brush6(RGB(255, 255, 255));
    SelectPenBrush(pDC, pen6, brush6, oldPen, oldBrush);    
    CBrush hatchBrush(HS_CROSS, RGB(0, 255, 255));
    pDC->SelectStockObject(NULL_BRUSH);
    oldBrush = pDC->SelectObject(&hatchBrush);
    pDC->Polygon(pts, 3);
    pDC->SelectObject(oldBrush);
   
    //pravilni mnogougao-petougao
        CPen pen5(PS_SOLID, 3, RGB(0, 0, 255));
        CBrush brush5(RGB(255, 255, 255));
        SelectPenBrush(pDC, pen5, brush5, oldPen, oldBrush);

        DrawRegularPolygon(pDC, 9.5 * cellSize, 8 * cellSize, 25, 5, 0);

        pDC->SelectObject(oldBrush);
        pDC->SelectObject(oldPen);
    //ljubicasti trougao 
    CPoint ptss[3] = {
        { (int)5.5 * cellSize, (int)10 * cellSize },
            { (int)11.5 * cellSize, (int)10 * cellSize },
            { (int)11.5 * cellSize, (int)16 * cellSize }, };
    DrawPoly(pDC, ptss, 3, RGB(0, 0, 255), RGB(153, 0, 204), 5);
        //pravilni mnogougao-kvadrat
        CPen pen1(PS_SOLID, 5, RGB(0, 0, 255));
        CBrush brush1(RGB(153, 0, 204));
        SelectPenBrush(pDC, pen1, brush1, oldPen, oldBrush);

        DrawRegularPolygon(pDC, 9 * cellSize, 12 * cellSize, 25, 4, 0);

        pDC->SelectObject(oldBrush);
        pDC->SelectObject(oldPen);

    //zeleni trougao 
    CPoint pts1[3] = {
        { (int)14.5 * cellSize, (int)(500- cellSize )},
            { (int)14.5 * cellSize, (int)16 * cellSize },
            { (int)11.5 * cellSize, (int)16 * cellSize }, };
    DrawPoly(pDC, pts1, 3, RGB(0, 0, 255), RGB(0, 255, 0), 5);
        //pravilni mnogougao-sedmougao
        CPen pen4(PS_SOLID, 3, RGB(0, 0, 255));
        CBrush brush4(RGB(0, 255, 0));
        SelectPenBrush(pDC, pen4, brush4, oldPen, oldBrush);

        DrawRegularPolygon(pDC, 13.25 * cellSize, 17 * cellSize, 10, 8, 0);

        pDC->SelectObject(oldBrush);
        pDC->SelectObject(oldPen);
    //roze trougao 
    CPoint pts2[3] = {
        { (int)11.5 * cellSize, (int)13* cellSize},
            { (int)14.5 * cellSize, (int)13 * cellSize },
            { (int)14.5 * cellSize, (int)10 * cellSize }, };
    DrawPoly(pDC, pts2, 3, RGB(0, 0, 255), RGB(255, 192, 203), 5);
        //pravilni mnogougao-sedmougao
        CPen pen3(PS_SOLID, 3, RGB(0, 0, 255));
        CBrush brush3(RGB(255, 192, 203));
        SelectPenBrush(pDC, pen3, brush3, oldPen, oldBrush);

        DrawRegularPolygon(pDC, 13.25 * cellSize, 12 * cellSize, 10, 7, 0);

        pDC->SelectObject(oldBrush);
        pDC->SelectObject(oldPen);
    //zuti trougao 
    CPoint pts3[3] = {
        { (int)14.5 * cellSize, (int)4 * cellSize},
            { (int)11.5 * cellSize, (int)7 * cellSize },
            { (int)14.5 * cellSize, (int)10 * cellSize }, };
    DrawPoly(pDC, pts3, 3, RGB(0, 0, 255), RGB(255, 255, 0), 5);
        //pravilni mnogougao-sestougao
        CPen pen2(PS_SOLID, 3, RGB(0, 0, 255));
        CBrush brush2(RGB(255, 255, 0));
        SelectPenBrush(pDC, pen2, brush2, oldPen, oldBrush);

        DrawRegularPolygon(pDC, 13 * cellSize, 7 * cellSize, 20, 6, 0);

        pDC->SelectObject(oldBrush);
        pDC->SelectObject(oldPen);
    //crveni romb 
    CPoint pts4[4] = {
            { (int)14.5*cellSize, cellSize },
            { (int)14.5*cellSize, (int)4 * cellSize },
            { (int)11.5* cellSize, (int)7 * cellSize },
            { (int)11.5* cellSize, (int)4* cellSize}
    };
    DrawPoly(pDC, pts4, 4, RGB(0, 0, 255), RGB(255, 0, 0), 5);

    //narandzasti kvadrat
    CPen pen(PS_SOLID, 5, RGB(0, 0, 255));
    CBrush brush(RGB(255, 153, 51));
    SelectPenBrush(pDC, pen, brush, oldPen, oldBrush);

    DrawRegularPolygon(pDC, 12.55*cellSize, 500-5.5*cellSize, 53, 4, 45);

    pDC->SelectObject(oldBrush);
    pDC->SelectObject(oldPen);
}

void CRacunarskaGrafikaLV1GDIPripremaView::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle)
{
    //samo za PRAVILNE mnogouglove!

	if (n < 3) return;

	float rot = rotAngle * 3.1415926535f / 180.0f;
	std::vector<CPoint> pts(n);

	for (int i = 0; i < n; i++)
	{
		float a = rot + i * (2.0f * 3.1415926535f / n); //2pi/n
		float x = cx + r * cos(a);
		float y = cy + r * sin(a);

		pts[i] = CPoint((int)x, (int)y);
	}

	pDC->Polygon(pts.data(), n);
}

void CRacunarskaGrafikaLV1GDIPripremaView::DrawGrid(CDC* pDC)
{
	CPen pen(PS_SOLID, 1, RGB(0, 255, 255));
	CPen* old = pDC->SelectObject(&pen);
	for (int i = 0; i <= 500; i += 25)
	{
		pDC->MoveTo(i, 0);
		pDC->LineTo(i, 500);

		pDC->MoveTo(0, i);
		pDC->LineTo(500, i);
	}

	pDC->SelectObject(old);
}

void CRacunarskaGrafikaLV1GDIPripremaView:: OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    //taster G pali/gasi grid 

	if (nChar == 'G')
	{
		showGrid = !showGrid;
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

#ifdef _DEBUG
void CRacunarskaGrafikaLV1GDIPripremaView::AssertValid() const
{
	CView::AssertValid();
}

void CRacunarskaGrafikaLV1GDIPripremaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif