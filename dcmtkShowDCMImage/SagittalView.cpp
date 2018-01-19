#include "stdafx.h"
#include "SagittalView.h"

IMPLEMENT_DYNCREATE(SagittalView, CView)
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
    return nullptr;
}

