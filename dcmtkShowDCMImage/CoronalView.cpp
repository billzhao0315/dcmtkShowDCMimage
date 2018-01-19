#include "stdafx.h"
#include "CoronalView.h"

IMPLEMENT_DYNCREATE(CoronalView, CView)
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
    return nullptr;
}

