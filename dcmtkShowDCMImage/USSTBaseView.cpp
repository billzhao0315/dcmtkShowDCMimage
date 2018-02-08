#include "stdafx.h"
#include "USSTBaseView.h"
#include "..//dependence//glm//gtc//matrix_transform.hpp"
#include "resource.h"

IMPLEMENT_DYNAMIC( USSTBaseView, CView )

USSTBaseView::USSTBaseView(void)
{
    m_mModelMatrix = glm::mat4x4( 1.0 );
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
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
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


void USSTBaseView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    m_nLBdownPointForModelMatrix = point;

    CView::OnLButtonDown(nFlags, point);
}


void USSTBaseView::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if( nFlags && MK_LBUTTON )
    {
        if( point != m_nLBdownPointForModelMatrix )
        {
            //glm::vec3 vAxisForRotation = glm::cross( glm::vec3( 0,0,1 ), glm::vec3( point.x - m_nLBdownPointForModelMatrix.x, point.y - m_nLBdownPointForModelMatrix.y, 0 ) );

            float angleForRotation = ::sqrtf( static_cast< float >( point.x - m_nLBdownPointForModelMatrix.x )*( point.x - m_nLBdownPointForModelMatrix.x ) + static_cast< float >( point.y - m_nLBdownPointForModelMatrix.y )*( point.y - m_nLBdownPointForModelMatrix.y ) );

            glm::mat4x4 previousModelMatrix = m_mModelMatrix;
            glm::mat4x4 mIdentityMatrix;
            m_mModelMatrix = glm::rotate( mIdentityMatrix, angleForRotation, glm::vec3( 0,0,1 ) );
            m_mModelMatrix = m_mModelMatrix * previousModelMatrix;

            m_nLBdownPointForModelMatrix = point;

            Invalidate( TRUE );
            ::SetCursor( ::LoadCursorA( AfxGetResourceHandle(), MAKEINTRESOURCE( IDC_ROTATECURSORSM ) ) );
        }
    }
    CView::OnMouseMove(nFlags, point);
}
