
// dcmtkShowDCMImageView.cpp : implementation of the TransverseView class
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

#include "gl/GLU.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#pragma comment( lib, "OpenGL32.lib" )
#pragma comment( lib, "glu32.lib" )
std::map<int, std::shared_ptr<TransverseView::ByteBlendLUT> > TransverseView::m_mapBlend;
const int TransverseView::BYTE_RANGE = 256;
void* pDicomDibits = NULL;
unsigned char* pRGBDicomDibits;
LPBITMAPINFOHEADER m_lpBMIH;

GLfloat cubeAlpha = 0.5f;
bool cdcBlendOpenGL = false;

AttributeTag TransverseView::tagMouseWheel;

// TransverseView
IMPLEMENT_DYNCREATE(TransverseView, USSTBaseView)

BEGIN_MESSAGE_MAP(TransverseView, USSTBaseView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &TransverseView::OnFilePrintPreview)
//	ON_WM_CONTEXTMENU()
//	ON_WM_RBUTTONUP()
    ON_WM_CREATE()
    //ON_COMMAND(ID_FILE_OPENDICOM, &TransverseView::OnFileOpendicom)
//    ON_WM_MOUSEHWHEEL()
    ON_WM_MOUSEWHEEL()
//    ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// TransverseView construction/destruction

TransverseView::TransverseView()
{
	// TODO: add construction code here
}

TransverseView::~TransverseView()
{

}

BOOL TransverseView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// TransverseView drawing

void TransverseView::OnDraw(CDC* pDC)
{
	CdcmtkShowDCMImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//// TODO: add draw code for native data here
    CRect rc;
    GetClientRect( &rc );
    glViewport( rc.left, rc.top, rc.right, rc.bottom );
    std::shared_ptr<DICOMImageHelper>  pDicomImageHelper = pDoc->getDicomImageHelper();
    if( pDicomImageHelper != nullptr )
    {

        if( cdcBlendOpenGL )
        {
            wglMakeCurrent( m_pClientDCSPV->GetSafeHdc(), m_hGLrcSPV );
            glClearColor( 0.0f,0.0f,0.0f,0.0f );
            drawCube();
            //SwapBuffers( m_pClientDCSPV->GetSafeHdc() );

            CRect rect;
            GetClientRect( &rect );

            int nWidthBytes = rect.Width() * 4;
            int nBytes = rect.Height() * nWidthBytes;

            // Grab the pixels from the open gl buffer.
            glReadBuffer(GL_BACK);

            BYTE* pBits = new BYTE[nBytes];

            glReadPixels( 0, 0, rect.Width(), rect.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, pBits ); //Copy the image to the array imageData

            // Need to mirror vertically as it is flipped between opengl and gdi
            BYTE* pMirroredBits = new BYTE[nBytes];
            for ( int nH = 0; nH < rect.Height(); ++nH )
            {
                memcpy( pMirroredBits + nH * nWidthBytes, pBits + ( rect.Height() - nH - 1 ) * nWidthBytes, nWidthBytes );
            }

            delete pBits;

            BYTE* pTemp = pMirroredBits;
            auto colorTable = getByteBlendLUT( 30 );
            unsigned int nSeriesImageIndex = pDoc->getCurrentImageIndex();
            auto arrPDicomSeries = pDicomImageHelper->getDICOMVolume()->getDICOMSeriesImage();
            //m_lpBMIH = arrPDicomSeries[nSeriesImageIndex]->m_pBitMapInfoHeader;
            int nWidthDcm = arrPDicomSeries[nSeriesImageIndex]->m_nWidth;
            int nHeightDcm = arrPDicomSeries[nSeriesImageIndex]->m_nHeight;
            pDicomDibits = arrPDicomSeries[nSeriesImageIndex]->m_pPixelData;
            for( int i = 0; i < nWidthDcm; ++i )
            {
                for( int j = 0; j< nHeightDcm; ++j )
                {
                    pTemp[ i*rect.Width()*4 + j*4 ] = colorTable->table[ ((BYTE*)pDicomDibits)[ i* nWidthDcm *3 + j*3] ][ pTemp[ i*rect.Width()*4 + j*4 ] ];
                    pTemp[ i*rect.Width()*4 + j*4 +1 ] = colorTable->table[ ((BYTE*)pDicomDibits)[ i* nWidthDcm *3 + j*3 +1 ] ][ pTemp[ i*rect.Width()*4 + j*4 +1 ] ];
                    pTemp[ i*rect.Width()*4 + j*4 +2 ] = colorTable->table[ ((BYTE*)pDicomDibits)[ i* nWidthDcm *3 + j*3 +2 ] ][ pTemp[ i*rect.Width()*4 + j*4 +2 ] ];
                }
            }

            // Create a bitmap from the pixels
            CBitmap bitmap;
            if ( bitmap.CreateBitmap( rect.Width(), rect.Height(), 1, 32, pMirroredBits ) == FALSE )
                return;

            CDC memDC;
            memDC.CreateCompatibleDC(pDC);

            // Select the new bitmap
            CBitmap *pBmpPrevious = memDC.SelectObject(&bitmap);

            // Copy the bits from the memory DC into the current dc
            pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
            /*CString str("test");
            pDC->TextOutA( 0,rect.bottom - 20,str );*/
            CString str;
            str.Format("current Slice: %d/%d",nSeriesImageIndex + 1,arrPDicomSeries.size());
            CMainFrame* pWndFrame = (CMainFrame*)GetParentFrame();
            pWndFrame->m_wndStatusBar.SetWindowTextA(str);

            // Restore the old bitmap
            memDC.SelectObject(pBmpPrevious);

            //Clean up DC
            memDC.DeleteDC();
            delete pMirroredBits;

            wglMakeCurrent( NULL, NULL );
        }
        else
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
                /*int xDst = static_cast<int>( static_cast< double >(rc.right)/2.0 - static_cast< double >( m_lpBMIH->biWidth )/2.0 );*/

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
                /*CDC dcBG;
                bool bFlag = dcBG.CreateCompatibleDC(pDC);
                if ( bFlag )
                {
                    dcBG.BitBlt(0, 0, rc.Width(), rc.Height(), NULL, 0, 0, BLACKNESS);
                }

                pDC->BitBlt( 0, 0, rc.Width(), rc.Height(), &dcBG, 0, 0, SRCCOPY );*/
                pDC->BitBlt( 0, 0, rc.Width(), rc.Height(), &dcMem, 0, 0, SRCCOPY );

                //BLENDFUNCTION bf;    
                //memset(&bf, 0, sizeof(bf));
                //bf.SourceConstantAlpha = 0x1f; //半透明  
                //CRect src(rect);
                //src.left += 50;
                //src.top  += 50;
                //::AlphaBlend( pDC->GetSafeHdc(), rect.left + 50, rect.top, rect.right - rect.left - 50, rect.bottom - rect.top, 
                //    dcMem.GetSafeHdc(), src.left, src.top, src.right - src.left, src.bottom - src.top, bf );


                //set text color
                COLORREF oldColor = pDC->SetTextColor( RGB(255,0,0) );
                //COLORREF oldBkcolor = pDC->SetBkColor( RGB(0,0,0) );
                pDC->TextOutA( 0,rc.bottom - 20,str );
                pDC->SetTextColor( oldColor );
                //pDC->SetBkColor( oldBkcolor );
                /*if (bFlag)
                {
                    dcBG.DeleteDC();
                }*/
            }

            dcMem.DeleteDC();
        }
        


        
        
    }
    else
    {
        wglMakeCurrent( m_pClientDCSPV->GetSafeHdc(), m_hGLrcSPV );
        glClearColor( 0.0f,1.0f,0.0f,1.0f );
        drawCube();

        CRect rect;
        GetClientRect( &rect );

        int nWidthBytes = rect.Width() * 4;
        int nBytes = rect.Height() * nWidthBytes;

        // Grab the pixels from the open gl buffer.
        glReadBuffer(GL_BACK);

        BYTE* pBits = new BYTE[nBytes];

        glReadPixels( 0, 0, rect.Width(), rect.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, pBits ); //Copy the image to the array imageData

        // Need to mirror vertically as it is flipped between opengl and gdi
        BYTE* pMirroredBits = new BYTE[nBytes];
        for ( int nH = 0; nH < rect.Height(); ++nH )
        {
            memcpy( pMirroredBits + nH * nWidthBytes, pBits + ( rect.Height() - nH - 1 ) * nWidthBytes, nWidthBytes );
        }

        delete pBits;

        // Create a bitmap from the pixels
        CBitmap bitmap;
        if ( bitmap.CreateBitmap( rect.Width(), rect.Height(), 1, 32, pMirroredBits ) == FALSE )
            return;

        CDC memDC;
        memDC.CreateCompatibleDC(pDC);

        // Select the new bitmap
        CBitmap *pBmpPrevious = memDC.SelectObject(&bitmap);

        // Copy the bits from the memory DC into the current dc
        pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
        CString str("test");
        pDC->TextOutA( 0,rect.bottom - 20,str );

        // Restore the old bitmap
        memDC.SelectObject(pBmpPrevious);

        //Clean up DC
        memDC.DeleteDC();
        delete pMirroredBits;

        wglMakeCurrent( NULL, NULL );


        CDC cdc;
        cdc.CreateCompatibleDC(pDC);

        CBitmap bitmap1,*pOldBitmap;
        bitmap1.CreateCompatibleBitmap(pDC, rect.right, rect.bottom);
        CRect src(rect);
        src.OffsetRect(CSize(-rect.left, -rect.top));
        pOldBitmap = cdc.SelectObject(&bitmap1);
        COLORREF color = RGB(0,0,255);
        cdc.FillSolidRect(src, color); 
        BLENDFUNCTION bf;    
        memset(&bf, 0, sizeof(bf));
        bf.SourceConstantAlpha = 0x7f; //半透明    
        ::AlphaBlend( pDC->GetSafeHdc(), rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 
                cdc.GetSafeHdc(), src.left, src.top, src.right - src.left, src.bottom - src.top, bf );

        cdc.SelectObject(pOldBitmap);
    }
    
    
    
}


// TransverseView printing


void TransverseView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL TransverseView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void TransverseView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void TransverseView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

//void TransverseView::OnRButtonUp(UINT /* nFlags */, CPoint point)
//{
//	ClientToScreen(&point);
//	OnContextMenu(this, point);
//}

//void TransverseView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
//{
//#ifndef SHARED_HANDLERS
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
//#endif
//}


// TransverseView diagnostics

#ifdef _DEBUG
void TransverseView::AssertValid() const
{
	CView::AssertValid();
}

void TransverseView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CdcmtkShowDCMImageDoc* TransverseView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CdcmtkShowDCMImageDoc)));
	return (CdcmtkShowDCMImageDoc*)m_pDocument;
}
#endif //_DEBUG


// TransverseView message handlers


int TransverseView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  Add your specialized creation code here
    bool bFlag = GLSetting();

    if( !bFlag )
    {
        MessageBox(_T("fail to initlize opengl"));
        return -1;
    }

    return 0;
}


//void TransverseView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//    // This feature requires Windows Vista or greater.
//    // The symbol _WIN32_WINNT must be >= 0x0600.
//    // TODO: Add your message handler code here and/or call default
//    m_nSeriesImageIndex += 1;
//    Invalidate(FALSE);
//    CView::OnMouseHWheel(nFlags, zDelta, pt);
//}


BOOL TransverseView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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

void TransverseView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
{
    // TODO: Add your specialized code here and/or call the base class
    if( lHint == static_cast<LPARAM>(CdcmtkShowDCMImageDoc::tagDICOMImport.getValue()) ||
        lHint == static_cast<LPARAM>(CdcmtkShowDCMImageDoc::tagDICOMSliceChange.getValue()) )
    {
        Invalidate(TRUE);
    }
}


void TransverseView::onSubjectNotify(Subject* /*pSubject*/, AttributeTag /*tag*/, void* /*pOldValue*/, void* /*pNewValue*/)
{
    
}


bool TransverseView::GLSetting()
{

    m_pClientDCSPV = new CClientDC(this);

    HGLRC glRc;

    PIXELFORMATDESCRIPTOR pfd = 
    {
        sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd                         //
        1,                              // version number                           //
        PFD_DRAW_TO_WINDOW|
        PFD_SUPPORT_OPENGL|
        PFD_DOUBLEBUFFER,                         // option flags                             //
        PFD_TYPE_RGBA,                  // Pixel type (RGBA)                        //
        24,                             // number of color bits                     //
        0, 0, 0, 0, 0, 0,               // color field widths and shifts ignored    //
        8,                              // 8-bit alpha buffer                       //
        0,                              // shift bit ignored                        //
        0,                              // no accumulation buffer                   //
        0, 0, 0, 0,                     // accumulation buffer field widths ignored //
        32,                             // 32-bit z-buffer                          //
        8,                              // 8-bit stencil buffer                     //
        0,                              // no auxiliary buffer                      //
        PFD_MAIN_PLANE,                 // main layer                               //
        0,                              // reserved                                 //
        0, 0, 0                         // layer masks ignored                      //
    };

    HDC safeHDC = m_pClientDCSPV->GetSafeHdc();

    int nPixelFormID = ChoosePixelFormat( safeHDC, &pfd );

    if( nPixelFormID == 0 )
    {
        MessageBox(_T("fail to choosePixelFormat"));
        return false;
    }

    if( SetPixelFormat( m_pClientDCSPV->GetSafeHdc(), nPixelFormID,&pfd ) == false )
    {
        MessageBox(_T("fail to setPixelFormat"));
        return false;
    }

    glRc = wglCreateContext( m_pClientDCSPV->GetSafeHdc() );

    wglMakeCurrent( m_pClientDCSPV->GetSafeHdc(), glRc );

    m_hGLrcSPV = wglGetCurrentContext();
    //glClearColor( 1.0f,1.0f,1.0f ,1.0f);
    return true;
}




std::shared_ptr<TransverseView::ByteBlendLUT> TransverseView::getByteBlendLUT( int nPercentAlpha )
{
    // Clamp the alpha percentage to a valid percentage range.
    nPercentAlpha = std::max<int>( nPercentAlpha, 0 );
    nPercentAlpha = std::min<int>( nPercentAlpha, 100 );

    // Look up the map for the clamped range.
    // We declare as a reference to a shared_ptr so that
    // if we have to construct it, the map is updated "for free".
    std::shared_ptr<ByteBlendLUT>& pLUT = m_mapBlend[nPercentAlpha];

    // Construct the map if necessary.
    if ( pLUT == NULL )
    {
        pLUT.reset( new ByteBlendLUT );

        // Compute the primary and secondary fractions, as integer
        //      values from 0 to 65536 (1<<16)
        // This allows us to us fixed-point calculations to create the blending table,
        // which is faster than floating-point.
        // Note that we multiply by 65536.0 / 100.0, to convert the percentage to
        // an alpha value in [0.0,1.0] at the same time we're making it fixed-point.
        // Note also that the alpha percentage passed in is the percentage of the
        // primary image intended to overwrite the secondary image.
        int nFracPrimary = static_cast<int>( static_cast<double>(nPercentAlpha) * 655.360 );
        int nFracSecondary = 65536 - nFracPrimary;

        // variables to store the components
        BYTE nPrimaryComponent;
        BYTE nSecondaryComponent;

        // For efficiency.
        ByteBlendTable& table = pLUT->table;

        // NOTE: The bit-shifting in the loop below is is done to convert the fixed-point 
        // fractional components back to the integer realm we want as our result. 
        // We right shift by 16 because the components incorporate a 16 bit left shift
        // from the fixed-point fractions.
        for ( int nSecondary = 0; nSecondary < BYTE_RANGE; ++nSecondary )
        {
            // Compute the secondary component.
            nSecondaryComponent  = static_cast<BYTE>( ( nSecondary * nFracSecondary ) >> 16 );

            for ( int nPrimary = 0; nPrimary < BYTE_RANGE; ++nPrimary )
            {
                // Compute the primary component.
                nPrimaryComponent  = static_cast<BYTE>( ( nPrimary * nFracPrimary ) >> 16 );

                table[nPrimary][nSecondary] = nPrimaryComponent + nSecondaryComponent;
            }
        }
    }

    return pLUT;
}

void TransverseView::drawCube()
{


    //glDrawBuffer( GL_FRONT_AND_BACK );
    glDrawBuffer( GL_BACK );
    //glClearColor( 0.0f,1.0f,0.0f,1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 60, 1,1,5 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt( 0,0,2, 0,0,0,0,1,0 );
    /*glTranslatef( -0.5f,-0.5f, -6.0f );
    glTranslatef( 0.5f,0.5f, 0.5f );*/
    glRotatef( 45, 0.0f,1.0f,0.0f );
    glRotatef( 45,1.0f,0.0f,0.0f );
    glTranslatef( -0.5f,-0.5f, -0.5f );
    glBegin(GL_QUADS);
        //front face
        glColor4f( 0.0f,0.0f,1.0f ,cubeAlpha);
        glVertex3f( 0.0f,0.0f,1.0f );
        glColor4f( 1.0f,0.0f,1.0f ,cubeAlpha);
        glVertex3f( 1.0f,0.0f,1.0f );
        glColor4f( 1.0f,1.0f,1.0f ,cubeAlpha);
        glVertex3f( 1.0f,1.0f,1.0f );
        glColor4f( 0.0f,1.0f,1.0f ,cubeAlpha);
        glVertex3f( 0.0f,1.0f,1.0f );

        //left face
        glColor4f( 0.0f,0.0f,1.0f ,cubeAlpha);
        glVertex3f( 0.0f,0.0f,1.0f );
        glColor4f( 0.0f,1.0f,1.0f ,cubeAlpha);
        glVertex3f( 0.0f,1.0f,1.0f );
        glColor4f( 0.0f,1.0f,0.0f ,cubeAlpha);
        glVertex3f( 0.0f,1.0f,0.0f );
        glColor4f( 0.0f,0.0f,0.0f ,cubeAlpha);
        glVertex3f( 0.0f,0.0f,0.0f );

        // top face
        glColor4f( 0.0f,1.0f,1.0f ,cubeAlpha);
        glVertex3f( 0.0f,1.0f,1.0f );
        glColor4f( 1.0f,1.0f,1.0f ,cubeAlpha);
        glVertex3f( 1.0f,1.0f,1.0f );
        glColor4f( 1.0f,1.0f,0.0f ,cubeAlpha);
        glVertex3f( 1.0f,1.0f,0.0f );
        glColor4f( 0.0f,1.0f,0.0f ,cubeAlpha);
        glVertex3f( 0.0f,1.0f,0.0f );

        //right face
        glColor4f( 1.0f,1.0f,1.0f ,cubeAlpha);
        glVertex3f( 1.0f,1.0f,1.0f );
        glColor4f( 1.0f,0.0f,1.0f ,cubeAlpha);
        glVertex3f( 1.0f,0.0f,1.0f );
        glColor4f( 1.0f,0.0f,0.0f ,cubeAlpha);
        glVertex3f( 1.0f,0.0f,0.0f );
        glColor4f( 1.0f,1.0f,0.0f ,cubeAlpha);
        glVertex3f( 1.0f,1.0f,0.0f );

        // bottom face
        glColor4f( 0.0f,0.0f,1.0f ,cubeAlpha);
        glVertex3f( 0.0f,0.0f,1.0f );
        glColor4f( 0.0f,0.0f,0.0f ,cubeAlpha);
        glVertex3f( 0.0f,0.0f,0.0f );
        glColor4f( 1.0f,0.0f,0.0f ,cubeAlpha);
        glVertex3f( 1.0f,0.0f,0.0f );
        glColor4f( 1.0f,0.0f,1.0f ,cubeAlpha);
        glVertex3f( 1.0f,0.0f,1.0f );

        // back face
        glColor4f( 0.0f,0.0f,0.0f ,cubeAlpha);
        glVertex3f( 0.0f,0.0f,0.0f );
        glColor4f( 0.0f,1.0f,0.0f ,cubeAlpha);
        glVertex3f( 0.0f,1.0f,0.0f );
        glColor4f( 1.0f,1.0f,0.0f ,cubeAlpha);
        glVertex3f( 1.0f,1.0f,0.0f );
        glColor4f( 1.0f,0.0f,0.0f ,cubeAlpha);
        glVertex3f( 1.0f,0.0f,0.0f );
    glEnd();
    glFinish();
}


std::unique_ptr<CMenu> TransverseView::createPopUpMenu()
{
    std::unique_ptr<CMenu> pMenu( new CMenu() );
    pMenu->LoadMenu( IDR_TRANSVERSE_POPUP );

    return  std::move( pMenu );
}


//void TransverseView::OnRButtonDown(UINT nFlags, CPoint point)
//{
//    // TODO: Add your message handler code here and/or call default
//
//    USSTBaseView::OnRButtonDown( nFlags, point);
//}
