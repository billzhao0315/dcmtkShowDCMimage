#pragma once
#include "afxwin.h"
#include "dcmtkShowDCMImageDoc.h"
#include "dcmtkShowDCMImageView.h"

#include "USSTBaseView.h"

class CoronalView : public USSTBaseView
{
public:
    DECLARE_DYNCREATE(CoronalView)
    CoronalView(void);
    ~CoronalView(void);
    virtual void OnUpdate( CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/ );
    public:
	virtual void OnDraw(CDC* pDC);

public:
    virtual std::unique_ptr<CMenu> createPopUpMenu();
};

