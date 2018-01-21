#pragma once
#include "afxwin.h"
#include "dcmtkShowDCMImageDoc.h"

#include "USSTBaseView.h"

class SagittalView : public USSTBaseView
{
public:
    DECLARE_DYNCREATE(SagittalView)
    SagittalView(void);
    ~SagittalView(void);

    virtual void OnUpdate( CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/ );

    virtual void OnDraw(CDC* pDC);

    virtual std::unique_ptr<CMenu> createPopUpMenu();

	CdcmtkShowDCMImageDoc* GetDocument() const;

    #ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
    #endif

protected:
    DECLARE_MESSAGE_MAP()

};

#ifndef _DEBUG  // debug version in dcmtkShowDCMImageView.cpp
inline CdcmtkShowDCMImageDoc* TransverseView::GetDocument() const
   { return reinterpret_cast<CdcmtkShowDCMImageDoc*>(m_pDocument); }
#endif

