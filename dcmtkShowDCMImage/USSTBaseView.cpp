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


BEGIN_MESSAGE_MAP(USSTBaseView, CView)
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


void USSTBaseView::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    //CWnd::OnRButtonDown( nFlags, point );
    std::unique_ptr<CMenu> pMenu = createPopUpMenu();

    if( pMenu != nullptr )
    {
        CMenu* pSubMenu =  pMenu->GetSubMenu( 0 ) ;

        if( pSubMenu != nullptr )
        {
            ClientToScreen( &point );

            pSubMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
        }
        
    }
    

    CView::OnRButtonDown(nFlags, point);
}


void USSTBaseView::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CView::OnRButtonUp(nFlags, point);
}
