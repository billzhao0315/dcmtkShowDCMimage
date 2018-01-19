#include "stdafx.h"
#include "USSTBaseView.h"

IMPLEMENT_DYNAMIC( USSTBaseView, CView )

USSTBaseView::USSTBaseView(void)
{
}


USSTBaseView::~USSTBaseView(void)
{
}

void USSTBaseView::OnUpdate(CView*, LPARAM, CObject*)
{
    
}

std::unique_ptr<CMenu> USSTBaseView::createPopUpMenu()
{
    return nullptr;
}


