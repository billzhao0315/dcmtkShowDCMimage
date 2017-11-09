#pragma once
#include <afxsplitterwndex.h>
#include "dcmtkShowDCMImageDoc.h"
#include "dcmtkShowDCMImageView.h"
class CsplitDCMView : public CView
{
protected: // create from serialization only
	CsplitDCMView();
	DECLARE_DYNCREATE(CsplitDCMView)

// Attributes
public:
	CdcmtkShowDCMImageDoc* GetDocument() const;

// Operations
public:
    void openDicoms( std::string pDicomFileIndex );
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
	virtual ~CsplitDCMView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
    DcmDataset* m_pDataSet;
    std::shared_ptr<short> m_pPixelDatas;
    std::vector<std::string> m_vDicomFileSet;
    std::shared_ptr<DICOMImageHelper> m_pDicomImageHelper;
    unsigned int m_nSeriesImageIndex;
protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnFileOpendicom();
//    afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // debug version in dcmtkShowDCMImageView.cpp
inline CdcmtkShowDCMImageDoc* CdcmtkShowDCMImageView::GetDocument() const
   { return reinterpret_cast<CdcmtkShowDCMImageDoc*>(m_pDocument); }
#endif