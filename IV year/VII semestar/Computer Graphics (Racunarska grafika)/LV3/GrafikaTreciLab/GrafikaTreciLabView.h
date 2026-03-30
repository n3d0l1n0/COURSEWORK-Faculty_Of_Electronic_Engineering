
// GrafikaTreciLabView.h : interface of the CGrafikaTreciLabView class
//

#pragma once


class CGrafikaTreciLabView : public CView
{
protected: // create from serialization only
	CGrafikaTreciLabView() noexcept;
	DECLARE_DYNCREATE(CGrafikaTreciLabView)

// Attributes
public:
	CGrafikaTreciLabDoc* GetDocument() const;
	bool showGrid;
	int pomeraj = 25;
// Operations
public:
	void FilterDImage(DImage* img, bool makeBlue = false);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void DrawGrid(CDC* pDC);
	void Scale(CDC* pDC, float dx, float dy, bool rightMultiply);
	void Translate(CDC* pDC, float dx, float dy, bool rightMultiply);
	void Rotate(CDC* pDC, double angle, bool rightMultiply);
	void Mirror(CDC* pDC, bool vertical, bool horisontal, bool rightMultiply);
	virtual void DrawNoBckg(CDC* pDC, DImage* pImage, bool spec);
	virtual void DrawMonaLisa(CDC* pDC);
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
	virtual ~CGrafikaTreciLabView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GrafikaTreciLabView.cpp
inline CGrafikaTreciLabDoc* CGrafikaTreciLabView::GetDocument() const
   { return reinterpret_cast<CGrafikaTreciLabDoc*>(m_pDocument); }
#endif

