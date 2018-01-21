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

void CoronalView::OnDraw(CDC* pDC)
{
    CdcmtkShowDCMImageDoc* pDoc = GetDocument();
    ASSERT_VALID( pDoc );
    if( !pDoc )
    {
        return;
    }

    CRect rc;
    GetClientRect( &rc );

    CString str = " Coronal view ";

    pDC->TextOutA( rc.left + 20, rc.top + 20 , str);

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

#ifdef _DEBUG
void CoronalView::AssertValid() const
{
	CView::AssertValid();
}

void CoronalView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CdcmtkShowDCMImageDoc* CoronalView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CdcmtkShowDCMImageDoc)));
	return (CdcmtkShowDCMImageDoc*)m_pDocument;
}
#endif //_DEBUG

