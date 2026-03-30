
// RacunarskaGrafikaLV1GDI_PripremaView.h : interface of the CRacunarskaGrafikaLV1GDIPripremaView class
//

#pragma once


class CRacunarskaGrafikaLV1GDIPripremaView : public CView
{
protected: // create from serialization only
	CRacunarskaGrafikaLV1GDIPripremaView() noexcept;
	DECLARE_DYNCREATE(CRacunarskaGrafikaLV1GDIPripremaView)
	BOOL showGrid; 

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	//
	void DrawGrid(CDC* pDC);
	void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle);
	void DrawShapes(CDC* pDC);

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
// Implementation
public:
	virtual ~CRacunarskaGrafikaLV1GDIPripremaView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RacunarskaGrafikaLV1GDI_PripremaView.cpp
inline CRacunarskaGrafikaLV1GDIPripremaDoc* CRacunarskaGrafikaLV1GDIPripremaView::GetDocument() const
   { return reinterpret_cast<CRacunarskaGrafikaLV1GDIPripremaDoc*>(m_pDocument); }
#endif

