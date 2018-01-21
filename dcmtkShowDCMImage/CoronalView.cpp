#include "stdafx.h"
#include "CoronalView.h"
#include "Resource.h"

IMPLEMENT_DYNCREATE(CoronalView, USSTBaseView)
BEGIN_MESSAGE_MAP(CoronalView, USSTBaseView)

END_MESSAGE_MAP()
CoronalView::CoronalView(void)
{
}


CoronalView::~CoronalView(void)
{
}

void CoronalView::OnDraw(CDC* /*pDC*/)
{
    
}

void CoronalView::OnUpdate(CView*, LPARAM, CObject*)
{
    
}

std::unique_ptr<CMenu> CoronalView::createPopUpMenu()
{
    std::unique_ptr<CMenu> pMenu( new CMenu() );

    pMenu->LoadMenu( IDR_CORONAL_POPUP );

    return pMenu;
}

