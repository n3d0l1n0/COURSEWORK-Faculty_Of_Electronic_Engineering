
// grafika2View.h : interface of the Cgrafika2View class
//

#pragma once


class Cgrafika2View : public CView
{
protected: // create from serialization only
	Cgrafika2View() noexcept;
	DECLARE_DYNCREATE(Cgrafika2View)

// Attributes
public:
	Cgrafika2Doc* GetDocument() const;
private:
	bool grid;
	HENHMETAFILE cactus;
	HENHMETAFILE cactuslight;
	float ugao1, ugao2;

// Operations
public:
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);

	void DrawFigure(CDC* pDC);
protected:
	void Grid(CDC* pDC);
	void Nebo(CDC* pDC);
	void Saksija(CDC* pDC);
	void Grana(CDC* pDC, float x, float y, int brDece, float angle, int* boje);
	void Kaktus(CDC* pDC, int ind, int* brDece, int lime1, int lime2);
	CPoint Centar(CDC* pDC);

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~Cgrafika2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in grafika2View.cpp
inline Cgrafika2Doc* Cgrafika2View::GetDocument() const
   { return reinterpret_cast<Cgrafika2Doc*>(m_pDocument); }
#endif

