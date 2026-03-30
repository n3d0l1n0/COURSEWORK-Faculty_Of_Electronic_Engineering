
// GrafikaTreciLabView.cpp : implementation of the CGrafikaTreciLabView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GrafikaTreciLab.h"
#endif

#include "GrafikaTreciLabDoc.h"
#include "GrafikaTreciLabView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGrafikaTreciLabView

IMPLEMENT_DYNCREATE(CGrafikaTreciLabView, CView)

BEGIN_MESSAGE_MAP(CGrafikaTreciLabView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CGrafikaTreciLabView construction/destruction

CGrafikaTreciLabView::CGrafikaTreciLabView() noexcept
{
	showGrid = false;

}

CGrafikaTreciLabView::~CGrafikaTreciLabView()
{
}

BOOL CGrafikaTreciLabView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	if (!CView::PreCreateWindow(cs))
		return FALSE;

	cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(nullptr, IDC_ARROW), (HBRUSH)::GetStockObject(NULL_BRUSH), nullptr);
	//omogucen double-click,kursor misa standardna strelica,pozadina prozora na NULL_BRUSH zbog flickera, nema dodeljena ikonica
	return TRUE;
}

// CGrafikaTreciLabView drawing

void CGrafikaTreciLabView::OnDraw(CDC* pDC)
{
	CGrafikaTreciLabDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect rect;
	GetClientRect(&rect);
	int width = rect.Width();
	int height = rect.Height();
	//antiflicker
	CDC memDC; //memorijski uredjaj za crtanje
	memDC.CreateCompatibleDC(pDC); //device context u RAM-u koji je kompatibilan sa onim na racunaru

	CBitmap memBitmap; //velicina prozora u kom crtamo off screen - slika u memoriji po kojoj memDC crta
	memBitmap.CreateCompatibleBitmap(pDC, width, height);

	CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);
	memDC.SetMapMode(pDC->GetMapMode()); //postavljamo isti sistem koordinata
	memDC.SetGraphicsMode(GM_ADVANCED); //za transformacije
	memDC.FillSolidRect(rect, RGB(255, 255, 255));  //bela pozadina u memorijskoj bitmapi/cisto platno

	XFORM old;
	memDC.GetWorldTransform(&old);
	//sve se crta u memorijskom DC off screen (ne na ekranu) - bez treperenja!
	
	// ROTACIJA - LEVO
		//Translate(&memDC, 20 * pomeraj, 0, FALSE);
		//Rotate(&memDC, 90, FALSE);

	//ROTACIJA - DESNO
		Translate(&memDC, 10 * pomeraj, 10 * pomeraj, FALSE);
		Rotate(&memDC, 270, FALSE);
		Translate(&memDC, -10 * pomeraj, -10 * pomeraj, FALSE);

	//ROTACIJA - USPRAVNO
		//Translate(&memDC, 10 * pomeraj, 10 * pomeraj, FALSE);
		//Rotate(&memDC, 180, FALSE);
		//Translate(&memDC, -10 * pomeraj, -10 * pomeraj, FALSE);

	DrawMonaLisa(&memDC);
	DrawGrid(&memDC);
	memDC.SetWorldTransform(&old); //da naredna crtanja nemaju ove iste transformacije

	pDC->BitBlt(0, 0, width, height, &memDC, 0, 0, SRCCOPY); //kad se sve nacrta, tek tad se prebacuje na pdc, piksel po piksel
	//SCRCOPY - kopiranje piksela direktno na ekran
	memDC.SelectObject(pOldBitmap); //vraca staru bitmapu}
}

void CGrafikaTreciLabView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'G')
	{
		showGrid = !showGrid;
		Invalidate(); //osvezi prozor - poziva on draw
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGrafikaTreciLabView::DrawGrid(CDC* pDC)
{
	if (!showGrid) return;

	int size = 500;
	int cells = 20;
	int spacing = size / cells;

	CPen gridPen(PS_SOLID, 1, RGB(200, 200, 200));
	CPen* oldPen = pDC->SelectObject(&gridPen);

	for (int i = 0; i <= cells; i++)
	{
		int y = i * spacing;
		pDC->MoveTo(0, y);
		pDC->LineTo(size, y);
	}

	for (int j = 0; j <= cells; j++)
	{
		int x = j * spacing;
		pDC->MoveTo(x, 0);
		pDC->LineTo(x, size);
	}

	pDC->SelectObject(oldPen);
}

void CGrafikaTreciLabView::Translate(CDC* pDC, float dx, float dy, bool rightMultiply)
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

void CGrafikaTreciLabView::Scale(CDC* pDC, float dx, float dy, bool rightMultiply) {

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

void CGrafikaTreciLabView::Rotate(CDC* pDC, double angle, bool rightMultiply)
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
	transformation.eM11 = cos(angle * 3.14 / 180);
	transformation.eM12 = sin(angle * 3.14 / 180);
	transformation.eM21 = -sin(angle * 3.14 / 180);
	transformation.eM22 = cos(angle * 3.14 / 180);
	transformation.eDx = 0;
	transformation.eDy = 0;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&transformation, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&transformation, MWT_LEFTMULTIPLY);
}

void CGrafikaTreciLabView::Mirror(CDC* pDC, bool vertical, bool horisontal, bool rightMultiply)
{
	XFORM transformation;
	/*
	[ 1 0 dx ]   [ eM11  eM12  eDx ]
	[ 0 1 dy ] = [ eM21  eM22  eDy ] - XFORM matrica (opisuje transformacije)
	[ 0 0 1  ]   [  0     0     1  ]

		[ x']   [ eM11  eM12  eDx ] [x]   [x*eM11  y*EM12   eDx]
	p'= [ y'] = [ eM21  eM22  eDy ] [y] = [x*eM21  y*EM22   eDx]
		[ 1 ]   [  0     0     1  ] [1]	  [	0        0       1 ]

		x' = eM11*x + eM12*y + eDx
		y' = eM21*x + eM22*y + eDy

		//vertikalni mirror
			[ -1 0 0 ]
			[ 0  1 0 ]
			[ 0  0 1 ]
				=> 	x' = eM11*x + eM12*y + eDx = −1*x+0*y+0=−x  //x se menja y ostaje isto
					y' = eM21*x + eM22*y + eDy = 0*x+1*y+0=y


		//horizontalni mirror
			[ 1 0 0  ]
			[ 0 -1 0 ]
			[ 0  0 1 ]
				=> 	x' = eM11*x + eM12*y + eDx = 1*x+0*y+0=x
					y' = eM21*x + eM22*y + eDy = 0*x-1*y+0=-y //y se menja x ostaje isto

	*/
	if (vertical) { //vertikalna osa
		transformation.eM11 = -1;
		transformation.eM12 = 0;
		transformation.eM21 = 0;
		transformation.eM22 = 1;
		transformation.eDx = 0;
		transformation.eDy = 0;
	}
	if (horisontal) { //horizontalna osa
		transformation.eM11 = 1;
		transformation.eM12 = 0;
		transformation.eM21 = 0;
		transformation.eM22 = -1;
		transformation.eDx = 0;
		transformation.eDy = 0;
	}

	if (rightMultiply)
		pDC->ModifyWorldTransform(&transformation, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&transformation, MWT_LEFTMULTIPLY);
}

void CGrafikaTreciLabView::DrawNoBckg(CDC* pDC, DImage* img, bool spec)
{
	CBitmap* pBmpImage = img->GetBitmap();
	BITMAP bm;
	pBmpImage->GetBitmap(&bm);

	CBitmap bmpMask;
	bmpMask.CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

	CDC* srcDC = new CDC();
	srcDC->CreateCompatibleDC(pDC);
	CDC* dstDC = new CDC();
	dstDC->CreateCompatibleDC(pDC);
	CBitmap* oldSrcBmp = srcDC->SelectObject(pBmpImage);
	CBitmap* oldDstBmp = dstDC->SelectObject(&bmpMask);

	COLORREF background = srcDC->GetPixel(0, 0);
	COLORREF oldBkColor = srcDC->SetBkColor(background);
	dstDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, srcDC, 0, 0, SRCCOPY);

	srcDC->SelectObject(oldSrcBmp);
	dstDC->SelectObject(oldDstBmp);
	FilterDImage(img, spec);
	srcDC->SelectObject(pBmpImage);
	dstDC->SelectObject(&bmpMask);

	COLORREF oldTextColor = srcDC->SetTextColor(RGB(255, 255, 255));
	srcDC->SetBkColor(RGB(0, 0, 0));
	srcDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, dstDC, 0, 0, SRCAND);

	dstDC->SetTextColor(oldTextColor);
	srcDC->SetBkColor(oldBkColor);
	srcDC->SelectObject(oldSrcBmp);
	dstDC->SelectObject(oldDstBmp);
	delete srcDC, dstDC;

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);

	CBitmap* oldBmp = memDC->SelectObject(&bmpMask);
	pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, memDC, 0, 0, SRCAND);

	memDC->SelectObject(pBmpImage);
	pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, memDC, 0, 0, SRCPAINT);

	memDC->SelectObject(&oldBmp);
	delete memDC;
}

void CGrafikaTreciLabView::DrawMonaLisa(CDC* pDC)
{
	DImage* slika1 = new DImage();
	DImage* slika2 = new DImage();
	DImage* slika3 = new DImage();
	DImage* slika4 = new DImage();
	DImage* slika5 = new DImage();
	DImage* slika6 = new DImage();
	DImage* slika7 = new DImage();
	DImage* slika8 = new DImage();
	DImage* slika9 = new DImage();

	slika1->Load(CString("1.bmp"));
	slika2->Load(CString("2.bmp"));
	slika3->Load(CString("3.bmp"));
	slika4->Load(CString("4.bmp"));
	slika5->Load(CString("5.bmp"));
	slika6->Load(CString("6.bmp"));
	slika7->Load(CString("7.bmp"));
	slika8->Load(CString("8.bmp"));
	slika9->Load(CString("9.bmp"));
	XFORM old;
	pDC->GetWorldTransform(&old); //cuvamo trenutnu matricu transformacija
	//prvi red desno
	Translate(pDC, 0 * pomeraj, 10 * pomeraj, false); //pomeramo koordinatni pocetak
	Mirror(pDC, false, true, false); //horizontalni mirror

	Translate(pDC, -0.5 * pomeraj, -11 * pomeraj, false); //red i kolona u gridu

	Translate(pDC, 5 * pomeraj, 5 * pomeraj, false); //pomeraj u centar slike
	Rotate(pDC, 70, false);
	Translate(pDC, -5 * pomeraj, -5 * pomeraj, false); //vracanje sistema

	//Mirror(pDC, true, false, false);
	DrawNoBckg(pDC, slika8, false);
	pDC->SetWorldTransform(&old); //vracanje prethodnog stanja transformacija

	//prvi red druga
	pDC->GetWorldTransform(&old);

	Translate(pDC, 0 * pomeraj, 10 * pomeraj, false);
	Mirror(pDC, false, true, false);

	Translate(pDC, 5.2 * pomeraj, -10.5 * pomeraj, false);

	Translate(pDC, 5 * pomeraj, 5 * pomeraj, false);
	Rotate(pDC, 150, false);
	Translate(pDC, -5 * pomeraj, -5 * pomeraj, false);

	//Mirror(pDC, true, false, false);
	DrawNoBckg(pDC, slika3, false);

	pDC->SetWorldTransform(&old);

	//prvi red levo
	pDC->GetWorldTransform(&old);

	//  Translate(pDC, 0*pomeraj, 10*pomeraj, false);
	  //Mirror(pDC, false, true, false);

	Translate(pDC, 10.25 * pomeraj, 11 * pomeraj, false);

	Translate(pDC, 5 * pomeraj, 5 * pomeraj, false);
	Mirror(pDC, true, false, false);	
	Rotate(pDC, 458.75, false);
	Translate(pDC, -5 * pomeraj, -5 * pomeraj, false);

	DrawNoBckg(pDC, slika9, true);
	pDC->SetWorldTransform(&old);

	//drugi red desno
	pDC->GetWorldTransform(&old);
	Translate(pDC, 0 * pomeraj, 10 * pomeraj, false);
	Mirror(pDC, false, true, false);
	Translate(pDC, -0.6 * pomeraj, -5 * pomeraj, false);
	Translate(pDC, 5 * pomeraj, 5 * pomeraj, false);
	Rotate(pDC, 94, false);
	Translate(pDC, -5 * pomeraj, -5 * pomeraj, false);
	DrawNoBckg(pDC, slika5, false);
	pDC->SetWorldTransform(&old);

	//drugi red sredina
	pDC->GetWorldTransform(&old);
	Translate(pDC, 0 * pomeraj, 10 * pomeraj, false);
	Mirror(pDC, false, true, false);
	Translate(pDC, 5.35 * pomeraj, -4.9 * pomeraj, false);
	Translate(pDC, 5 * pomeraj, 5 * pomeraj, false);
	Rotate(pDC, -255, false);
	Translate(pDC, -5 * pomeraj, -5 * pomeraj, false);
	//Translate(pDC, 1.4 * pomeraj, 0.4 * pomeraj, false);
	DrawNoBckg(pDC, slika7, false);
	pDC->SetWorldTransform(&old);

	//drugi red levo
	pDC->GetWorldTransform(&old);

	Translate(pDC, 0 * pomeraj, 10 * pomeraj, false);
	Mirror(pDC, false, true, false);
	Translate(pDC, 10.35 * pomeraj, -4.9 * pomeraj, false);
	Translate(pDC, 5 * pomeraj, 5 * pomeraj, false);
	Rotate(pDC, -94.5, false);
	Translate(pDC, -5 * pomeraj, -5 * pomeraj, false);
	DrawNoBckg(pDC, slika1, false);
	pDC->SetWorldTransform(&old);

	//treci red desno
	pDC->GetWorldTransform(&old);
	Translate(pDC, 0 * pomeraj, 10 * pomeraj, false);
	Mirror(pDC, false, true, false);
	Translate(pDC, -1 * pomeraj, 1.5 * pomeraj, false);
	Translate(pDC, 5 * pomeraj, 5 * pomeraj, false);
	Rotate(pDC, -188, false);
	Translate(pDC, -5 * pomeraj, -5 * pomeraj, false);
	DrawNoBckg(pDC, slika6,false);
	pDC->SetWorldTransform(&old);

	//treci red sredina
	pDC->GetWorldTransform(&old);
	Translate(pDC, 0 * pomeraj, 10 * pomeraj, false);
	Mirror(pDC, false, true, false);
	Translate(pDC, 5 * pomeraj, 0.6 * pomeraj, false);
	Translate(pDC, 5 * pomeraj, 5 * pomeraj, false);
	Rotate(pDC, 13, false);
	Translate(pDC, -5 * pomeraj, -5 * pomeraj, false);
	//Translate(pDC, 1.4 * pomeraj, 0.4 * pomeraj, false);
	DrawNoBckg(pDC, slika4, false);
	pDC->SetWorldTransform(&old);

	//treci red levo
	pDC->GetWorldTransform(&old);

	Translate(pDC, 0 * pomeraj, 10 * pomeraj, false);
	Mirror(pDC, false, true, false);
	Translate(pDC, 11 * pomeraj, 0.7 * pomeraj, false);
	Translate(pDC, 5 * pomeraj, 5 * pomeraj, false);
	Rotate(pDC, 27, false);
	Translate(pDC, -5 * pomeraj, -5 * pomeraj, false);
	DrawNoBckg(pDC, slika2, false);
	pDC->SetWorldTransform(&old);
}

void CGrafikaTreciLabView::FilterDImage(DImage* img, bool makeBlue)
{
	CBitmap* bmpObj = img->GetBitmap();
	BITMAP bmp;
	bmpObj->GetBitmap(&bmp);

	int width = bmp.bmWidth;
	int height = bmp.bmHeight;
	int bytesPerPixel = bmp.bmBitsPixel / 8;
	int stride = ((width * bytesPerPixel + 3) / 4) * 4;
	int imageSize = stride * height;

	BYTE* buffer = new BYTE[imageSize]; //upisivanje svih piksela

	bmpObj->GetBitmapBits(imageSize, buffer); //cita piksele iz bitmape u memoriju

	for (int y = 0; y < height; y++)
	{
		BYTE* row = buffer + y * stride; //gde pocinje linija u memoriji
		for (int x = 0; x < width; x++)
		{
			BYTE* px = row + x * bytesPerPixel; //pointer na piksel

			BYTE B = px[0]; //redosled je B G R
			BYTE G = px[1];
			BYTE R = px[2];

			if (makeBlue)
			{
				BYTE blue = min(255, (R + G + B) / 3 + 64); //pojacavamo saturaciju
				px[0] = 0; // B postavljamo samo plavu komponentu, ostale brisemo
				px[1] = 0;       // G
				px[2] = blue;       // R
			}
			else
			{
				BYTE gr = min(255, 64 + (R + G + B) / 3);
				px[0] = gr; // B
				px[1] = gr; // G
				px[2] = gr; // R
			}
		}
	}

	bmpObj->SetBitmapBits(imageSize, buffer); //vracamo piksele u bitmapu
	delete[] buffer;
}


// CGrafikaTreciLabView printing

BOOL CGrafikaTreciLabView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGrafikaTreciLabView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGrafikaTreciLabView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGrafikaTreciLabView diagnostics

#ifdef _DEBUG
void CGrafikaTreciLabView::AssertValid() const
{
	CView::AssertValid();
}

void CGrafikaTreciLabView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGrafikaTreciLabDoc* CGrafikaTreciLabView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGrafikaTreciLabDoc)));
	return (CGrafikaTreciLabDoc*)m_pDocument;
}
#endif //_DEBUG


// CGrafikaTreciLabView message handlers
