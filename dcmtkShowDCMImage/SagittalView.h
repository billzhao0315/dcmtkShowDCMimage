#pragma once
#include "afxwin.h"
#include "dcmtkShowDCMImageDoc.h"
#include "dcmtkShowDCMImageView.h"
class SagittalView :
    public CView
{
public:
    DECLARE_DYNCREATE(SagittalView)
    SagittalView(void);
    ~SagittalView(void);
    virtual void OnDraw(CDC* pDC);
};

