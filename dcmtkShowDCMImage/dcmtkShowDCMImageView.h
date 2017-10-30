
// dcmtkShowDCMImageView.h : interface of the CdcmtkShowDCMImageView class
//

#pragma once

class DcmDataset;


class CdcmtkShowDCMImageView : public CView
{
protected: // create from serialization only
	CdcmtkShowDCMImageView();
	DECLARE_DYNCREATE(CdcmtkShowDCMImageView)

// Attributes
public:
	CdcmtkShowDCMImageDoc* GetDocument() const;

// Operations
public:

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
	virtual ~CdcmtkShowDCMImageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
    DcmDataset* m_pDataSet;
protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#ifndef _DEBUG  // debug version in dcmtkShowDCMImageView.cpp
inline CdcmtkShowDCMImageDoc* CdcmtkShowDCMImageView::GetDocument() const
   { return reinterpret_cast<CdcmtkShowDCMImageDoc*>(m_pDocument); }
#endif

