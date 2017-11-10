#pragma once
#include "afxwin.h"
#include "dcmtkShowDCMImageDoc.h"
#include "dcmtkShowDCMImageView.h"
class CoronalView :
    public CView
{
public:
    DECLARE_DYNCREATE(CoronalView)
    CoronalView(void);
    ~CoronalView(void);
    public:
	virtual void OnDraw(CDC* pDC);
};

