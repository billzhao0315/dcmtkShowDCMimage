#pragma once
#include "afxwin.h"
#include "dcmtkShowDCMImageDoc.h"
#include "dcmtkShowDCMImageView.h"

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

};

