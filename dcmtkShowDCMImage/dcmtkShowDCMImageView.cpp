
// dcmtkShowDCMImageView.cpp : implementation of the CdcmtkShowDCMImageView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "dcmtkShowDCMImage.h"
#endif
#include <io.h>
#include <vector>
#include "dcmtkShowDCMImageDoc.h"
#include "dcmtkShowDCMImageView.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmdata/dcdatset.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmdata/dcfilefo.h"
//#include "DicomDictionary.h"
#include "DICOMImageHelper.h"
#include "DICOMVolume.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
void* pDicomDibits = NULL;
unsigned char* pRGBDicomDibits;
LPBITMAPINFOHEADER m_lpBMIH;

AttributeTag CdcmtkShowDCMImageView::tagMouseWheel;

// CdcmtkShowDCMImageView
IMPLEMENT_DYNCREATE(CdcmtkShowDCMImageView, CView)

BEGIN_MESSAGE_MAP(CdcmtkShowDCMImageView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CdcmtkShowDCMImageView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
    ON_WM_CREATE()
    //ON_COMMAND(ID_FILE_OPENDICOM, &CdcmtkShowDCMImageView::OnFileOpendicom)
//    ON_WM_MOUSEHWHEEL()
    ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CdcmtkShowDCMImageView construction/destruction

CdcmtkShowDCMImageView::CdcmtkShowDCMImageView()
{
	// TODO: add construction code here
}

CdcmtkShowDCMImageView::~CdcmtkShowDCMImageView()
{

}

BOOL CdcmtkShowDCMImageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CdcmtkShowDCMImageView drawing

void CdcmtkShowDCMImageView::OnDraw(CDC* pDC)
{
	CdcmtkShowDCMImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//// TODO: add draw code for native data here
    std::shared_ptr<DICOMImageHelper>  pDicomImageHelper = pDoc->getDicomImageHelper();
    if( pDicomImageHelper != nullptr )
    {
        CDC dcMem;
        if( dcMem.CreateCompatibleDC( pDC ) == FALSE )
        {
            return;
        }
        
        unsigned int nSeriesImageIndex = pDoc->getCurrentImageIndex();
        auto arrPDicomSeries = pDicomImageHelper->getDICOMVolume()->getDICOMSeriesImage();
        m_lpBMIH = arrPDicomSeries[nSeriesImageIndex]->m_pBitMapInfoHeader;
        pDicomDibits = arrPDicomSeries[nSeriesImageIndex]->m_pPixelData;
        if( pDicomDibits != NULL )
        {

            CRect rcSatus;
            CMainFrame* pWndFrame = (CMainFrame*)GetParentFrame();
            pWndFrame->m_wndStatusBar.GetWindowRect(rcSatus);
            ScreenToClient(rcSatus);
            pDC->ExcludeClipRect(rcSatus);

            CRect rc;
            GetClientRect(&rc);
            int xDst = rc.right/2 - m_lpBMIH->biWidth/2;

            HBITMAP hBitmap = NULL;

            void* pBits;
            BITMAPINFOHEADER bmih;

            // Setup the bitmap info structure
            bmih.biSize = sizeof(BITMAPINFOHEADER);
            bmih.biWidth = arrPDicomSeries[nSeriesImageIndex]->m_nWidth;
            bmih.biHeight = arrPDicomSeries[nSeriesImageIndex]->m_nHeight;
            bmih.biPlanes = 1;
            bmih.biBitCount = 24;
            bmih.biCompression = BI_RGB;
            bmih.biSizeImage = 0;
            bmih.biXPelsPerMeter = 0;
            bmih.biYPelsPerMeter = 0;
            bmih.biClrUsed = 0;
            bmih.biClrImportant = 0;

            // Create DIB 
            hBitmap = CreateDIBSection(NULL, (BITMAPINFO*)&bmih, DIB_RGB_COLORS, &pBits, NULL, 0);

            if ( hBitmap != NULL )
            {
                memcpy( pBits, pDicomDibits, sizeof(BYTE) * arrPDicomSeries[nSeriesImageIndex]->m_nLength );
                ::SelectObject( dcMem.GetSafeHdc(), hBitmap );
                ::DeleteObject( hBitmap );
            }

           /* StretchDIBits(pDC->GetSafeHdc(),
                          xDst,0,dstWidth,rc.bottom,
                          0,0,m_lpBMIH->biWidth,m_lpBMIH->biHeight,
                          pDicomDibits, (LPBITMAPINFO) m_lpBMIH,DIB_RGB_COLORS, SRCCOPY);*/
           
            CString str;
            str.Format("current Slice: %d/%d",nSeriesImageIndex + 1,arrPDicomSeries.size());
            pWndFrame->m_wndStatusBar.SetWindowTextA(str);
            CDC dcBG;
            bool bFlag = dcBG.CreateCompatibleDC(pDC);
            if ( bFlag )
            {
                dcBG.BitBlt(0, 0, rc.Width(), rc.Height(), NULL, 0, 0, BLACKNESS);
            }

            pDC->BitBlt( 0, 0, rc.Width(), rc.Height(), &dcBG, 0, 0, SRCCOPY );
            pDC->BitBlt( 0, 0, rc.Width(), rc.Height(), &dcMem, 0, 0, SRCCOPY );

            //set text color
            COLORREF oldColor = pDC->SetTextColor( RGB(255,0,0) );
            //COLORREF oldBkcolor = pDC->SetBkColor( RGB(0,0,0) );
            pDC->TextOutA( 0,rc.bottom - 20,str );
            pDC->SetTextColor( oldColor );
            //pDC->SetBkColor( oldBkcolor );
            if (bFlag)
            {
                dcBG.DeleteDC();
            }
        }

        dcMem.DeleteDC();
        
    }
    else
    {
        CRect rect;
        GetClientRect( &rect );

        pDC->BitBlt( 0, 0, rect.Width(), rect.Height(), NULL, 0, 0, BLACKNESS );
    }
    
    
    
}


// CdcmtkShowDCMImageView printing


void CdcmtkShowDCMImageView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CdcmtkShowDCMImageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CdcmtkShowDCMImageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CdcmtkShowDCMImageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CdcmtkShowDCMImageView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CdcmtkShowDCMImageView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CdcmtkShowDCMImageView diagnostics

#ifdef _DEBUG
void CdcmtkShowDCMImageView::AssertValid() const
{
	CView::AssertValid();
}

void CdcmtkShowDCMImageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CdcmtkShowDCMImageDoc* CdcmtkShowDCMImageView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CdcmtkShowDCMImageDoc)));
	return (CdcmtkShowDCMImageDoc*)m_pDocument;
}
#endif //_DEBUG


// CdcmtkShowDCMImageView message handlers


int CdcmtkShowDCMImageView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  Add your specialized creation code here
    

    return 0;
}


//void CdcmtkShowDCMImageView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//    // This feature requires Windows Vista or greater.
//    // The symbol _WIN32_WINNT must be >= 0x0600.
//    // TODO: Add your message handler code here and/or call default
//    m_nSeriesImageIndex += 1;
//    Invalidate(FALSE);
//    CView::OnMouseHWheel(nFlags, zDelta, pt);
//}


BOOL CdcmtkShowDCMImageView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: Add your message handler code here and/or call default
    CdcmtkShowDCMImageDoc* pDoc = GetDocument();

    ASSERT_VALID(pDoc);
	if (!pDoc)
		return false;
    if( zDelta >= 0 )
    {
        pDoc->increaseImageIndex();
    }
    else
    {
        pDoc->decreaseImageIndex();
    }
    

    return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CdcmtkShowDCMImageView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
{
    // TODO: Add your specialized code here and/or call the base class
    if( lHint == static_cast<LPARAM>(CdcmtkShowDCMImageDoc::tagDICOMImport.getValue()) ||
        lHint == static_cast<LPARAM>(CdcmtkShowDCMImageDoc::tagDICOMSliceChange.getValue()) )
    {
        Invalidate(TRUE);
    }
}


void CdcmtkShowDCMImageView::onSubjectNotify(Subject* /*pSubject*/, AttributeTag /*tag*/, void* /*pOldValue*/, void* /*pNewValue*/)
{
    
}
