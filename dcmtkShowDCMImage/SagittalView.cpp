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

void SagittalView::OnDraw(CDC* pDC)
{
    CdcmtkShowDCMImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    CRect rc;
    GetClientRect( &rc );

    CString str = " Sagittal view ";

    pDC->TextOutA( rc.left + 20, rc.top + 20 , str);

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

#ifdef _DEBUG
void SagittalView::AssertValid() const
{
	CView::AssertValid();
}

void SagittalView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CdcmtkShowDCMImageDoc* SagittalView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CdcmtkShowDCMImageDoc)));
	return (CdcmtkShowDCMImageDoc*)m_pDocument;
}
#endif //_DEBUG

