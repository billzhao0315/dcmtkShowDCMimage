
// dcmtkShowDCMImageView.h : interface of the CdcmtkShowDCMImageView class
//

#pragma once
#include <memory>
#include <string>
#include <vector>
#include "AttributeTag.h"
#include "Observer.h"
#include <map>

#include <gl/GL.h>
#include "glext.h"
#include "wglext.h"
class DcmDataset;
class DICOMImageHelper;

class CdcmtkShowDCMImageView : public CView, public Observer
{
protected: // create from serialization only
	CdcmtkShowDCMImageView();
	DECLARE_DYNCREATE(CdcmtkShowDCMImageView)
public:
    static AttributeTag tagMouseWheel;
// Attributes
public:
	CdcmtkShowDCMImageDoc* GetDocument() const;

// Operations
public:
    virtual void onSubjectNotify(
                         Subject* pSubject,
                         AttributeTag tag,
                         void* pOldValue = NULL,
                         void* pNewValue = NULL
                        );
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

protected:
    bool GLSetting();
private:
    CClientDC* m_pClientDCSPV;
    HGLRC      m_hGLrcSPV;

    

    typedef unsigned char ByteBlendTable[256][256];
    struct ByteBlendLUT
    {
        ByteBlendTable table;
    };
    static std::map<int, std::shared_ptr<ByteBlendLUT> >   m_mapBlend;
    static const int BYTE_RANGE;
private:
    std::shared_ptr<ByteBlendLUT> CdcmtkShowDCMImageView::getByteBlendLUT( int nPercentAlpha );
    void drawCube();

// Implementation
public:
	virtual ~CdcmtkShowDCMImageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
    /*std::vector<std::string> m_vDicomFileSet;
    std::shared_ptr<DICOMImageHelper> m_pDicomImageHelper;
    unsigned int m_nSeriesImageIndex;*/
protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//    afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in dcmtkShowDCMImageView.cpp
inline CdcmtkShowDCMImageDoc* CdcmtkShowDCMImageView::GetDocument() const
   { return reinterpret_cast<CdcmtkShowDCMImageDoc*>(m_pDocument); }
#endif

