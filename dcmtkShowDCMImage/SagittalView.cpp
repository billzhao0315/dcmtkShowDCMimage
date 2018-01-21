#include "stdafx.h"
#include "SagittalView.h"
#include "resource.h"

IMPLEMENT_DYNCREATE(SagittalView, USSTBaseView)
BEGIN_MESSAGE_MAP(SagittalView, USSTBaseView)

END_MESSAGE_MAP()
SagittalView::SagittalView(void)
{
}


SagittalView::~SagittalView(void)
{
}

void SagittalView::OnDraw(CDC* /*pDC*/)
{
    
}

void SagittalView::OnUpdate(CView*, LPARAM, CObject*)
{
    
}

std::unique_ptr<CMenu> SagittalView::createPopUpMenu()
{
    std::unique_ptr<CMenu> pMenu( new CMenu() );
    pMenu->LoadMenu( IDR_SAGITTAL_POPUP );
    return pMenu;
}

