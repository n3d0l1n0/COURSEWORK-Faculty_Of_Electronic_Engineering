
// RacunarskaGrafikaLV1GDI_PripremaDoc.h : interface of the CRacunarskaGrafikaLV1GDIPripremaDoc class
//


#pragma once


class CRacunarskaGrafikaLV1GDIPripremaDoc : public CDocument
{
protected: // create from serialization only
	CRacunarskaGrafikaLV1GDIPripremaDoc() noexcept;
	DECLARE_DYNCREATE(CRacunarskaGrafikaLV1GDIPripremaDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CRacunarskaGrafikaLV1GDIPripremaDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
