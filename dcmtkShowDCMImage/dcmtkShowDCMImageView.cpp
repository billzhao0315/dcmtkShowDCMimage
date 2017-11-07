
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
    ON_COMMAND(ID_FILE_OPENDICOM, &CdcmtkShowDCMImageView::OnFileOpendicom)
//    ON_WM_MOUSEHWHEEL()
    ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CdcmtkShowDCMImageView construction/destruction

CdcmtkShowDCMImageView::CdcmtkShowDCMImageView()
{
	// TODO: add construction code here
    m_pDataSet = NULL;
    m_pDicomImageHelper = nullptr;
    m_nSeriesImageIndex = 0;
}

CdcmtkShowDCMImageView::~CdcmtkShowDCMImageView()
{
    if (m_pDataSet != NULL)
    {
        delete m_pDataSet;
    }
    
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
    if( m_pDicomImageHelper != nullptr )
    {
        CDC dcMem;
        if( dcMem.CreateCompatibleDC( pDC ) == FALSE )
        {
            return;
        }

        auto arrPDicomSeries = m_pDicomImageHelper->getDICOMVolume()->getDICOMSeriesImage();
        m_lpBMIH = arrPDicomSeries[m_nSeriesImageIndex]->m_pBitMapInfoHeader;
        pDicomDibits = arrPDicomSeries[m_nSeriesImageIndex]->m_pPixelData;
        if( pDicomDibits != NULL )
        {

            CRect rcSatus;
            CMainFrame* pWndFrame = (CMainFrame*)GetParent();
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
            bmih.biWidth = arrPDicomSeries[m_nSeriesImageIndex]->m_nWidth;
            bmih.biHeight = arrPDicomSeries[m_nSeriesImageIndex]->m_nHeight;
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
                memcpy( pBits, pDicomDibits, sizeof(BYTE) * arrPDicomSeries[m_nSeriesImageIndex]->m_nLength );
                ::SelectObject( dcMem.GetSafeHdc(), hBitmap );
                ::DeleteObject( hBitmap );
            }

           /* StretchDIBits(pDC->GetSafeHdc(),
                          xDst,0,dstWidth,rc.bottom,
                          0,0,m_lpBMIH->biWidth,m_lpBMIH->biHeight,
                          pDicomDibits, (LPBITMAPINFO) m_lpBMIH,DIB_RGB_COLORS, SRCCOPY);*/
           
            CString str;
            str.Format("current Slice: %d/%d",m_nSeriesImageIndex,arrPDicomSeries.size());
            pWndFrame->m_wndStatusBar.SetWindowTextA(str);
            CDC dcBG;
            bool bFlag = dcBG.CreateCompatibleDC(pDC);
            if ( bFlag )
            {
                dcBG.BitBlt(0, 0, rc.Width(), rc.Height(), NULL, 0, 0, BLACKNESS);
            }

            pDC->BitBlt( 0, 0, rc.Width(), rc.Height(), &dcBG, 0, 0, SRCCOPY );
            pDC->BitBlt( xDst, 0, rc.Width(), rc.Height(), &dcMem, 0, 0, SRCCOPY );

            //set text color
            COLORREF oldColor = pDC->SetTextColor( RGB(255,0,0) );
            COLORREF oldBkcolor = pDC->SetBkColor( RGB(0,0,0) );
            pDC->TextOutA( 0,rc.bottom - 20,str );
            pDC->SetTextColor( oldColor );
            pDC->SetBkColor( oldBkcolor );
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

void CdcmtkShowDCMImageView::openDicoms( std::string pDicomFileIndex)
{
    
    std::string sDicomPath;
    int iPos = pDicomFileIndex.find_last_of("\\");
    sDicomPath = pDicomFileIndex.substr(0,iPos+1);
    iPos = pDicomFileIndex.rfind(".DCM");
    std::string strDicomIndex = sDicomPath +"*"+ pDicomFileIndex.substr(iPos, pDicomFileIndex.length());
    intptr_t hFile = 0;
    _finddata_t fileinfo;
    hFile = _findfirst(strDicomIndex.c_str(), &fileinfo);
    if (hFile == -1)
    {
        std::cout << "The file is not exist!" << std::endl;
    }

    do
    {
        m_vDicomFileSet.push_back(sDicomPath + std::string(fileinfo.name));
    } while (_findnext(hFile, &fileinfo) == 0);

    _findclose(hFile);
}

void CdcmtkShowDCMImageView::OnFileOpendicom()
{
    // TODO: Add your command handler code here
    //open only the dicom file.
    CString m_sFile;
    CString szFilter = _T("Text Files (*.dcm)|*.dcm||"); //使打开文件对话框仅显示dcm文件
    CFileDialog  dicomFile(TRUE,
                          (LPCTSTR)".dcm",
                          m_sFile,
                          OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
                          szFilter);
    if( dicomFile.DoModal()==TRUE )
    {
        m_sFile = dicomFile.GetPathName();
        openDicoms( m_sFile.GetString() );
        //DcmFileFormat* pDicomFile = new DcmFileFormat();
        ////读DICOM文件
        //pDicomFile->loadFile(m_sFile.GetString());
        ////得到数据集
        //m_pDataSet = pDicomFile->getDataset();

        //E_TransferSyntax xfer = m_pDataSet->getOriginalXfer();
        ////根据传输语法构造DicomImage从fstart帧开始一共fcount帧
        //DicomImage* pDicomImg = new DicomImage (m_pDataSet, xfer/*, 0, 0, 1*/);
        //const char* pElementValue;
        //m_pDataSet->findAndGetString( DCM_WindowWidth, pElementValue );
        //std::string sTemp = std::string( pElementValue );
        //auto iPos = sTemp.find_first_of("\\");
        //int dcmWidth = atoi( sTemp.substr(0,iPos).c_str() );

        //m_pDataSet->findAndGetString( DCM_WindowCenter, pElementValue );
        //sTemp = std::string( pElementValue );
        //iPos = sTemp.find_first_of("\\");
        //int dcmCenter = atoi( sTemp.substr(0,iPos).c_str() );
        //pDicomImg->setWindow( dcmCenter, dcmWidth );
        
        ////通过以下的方法得到并用BitmapHeadInformation的结构体来保存DICOM文件的信息
        //m_lpBMIH = (LPBITMAPINFOHEADER) new char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];
        //m_lpBMIH->biSize = sizeof(BITMAPINFOHEADER);
        //m_lpBMIH->biWidth = pDicomImg->getWidth();
        //m_lpBMIH->biHeight = pDicomImg->getHeight();
        //m_lpBMIH->biPlanes = 1;
        //m_lpBMIH->biBitCount = 24;
        //m_lpBMIH->biCompression = BI_RGB;
        //m_lpBMIH->biSizeImage = 0;
        //m_lpBMIH->biXPelsPerMeter = 0;
        //m_lpBMIH->biYPelsPerMeter = 0;

        //
        ////得到DICOM文件第frame的DIB数据(假设是24位的)
        //unsigned long pixelLen = pDicomImg->createWindowsDIB(pDicomDibits, 0, 0, 24, 1, 1);
        BeginWaitCursor();
        m_pDicomImageHelper =std::make_shared<DICOMImageHelper>();
        m_pDicomImageHelper->DicomParse(m_vDicomFileSet);
        EndWaitCursor();
        //auto arrPDicomSeries = m_pDicomImageHelper->getDICOMVolume()->getDICOMSeriesImage();
        //unsigned long PixelLen = arrPDicomSeries[0]->m_nLength;
        //pDicomDibits = new unsigned char[PixelLen];
        //m_lpBMIH = arrPDicomSeries[0]->m_pBitMapInfoHeader;
        ///*for(int i = 0; i < PixelLen; ++i )
        //{
        //    ((unsigned char*)pDicomDibits)[i] = arrPDicomSeries[0]->m_pPixelData[i];
        //}*/
        //pDicomDibits = arrPDicomSeries[0]->m_pPixelData;
        Invalidate(FALSE);
    }

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
    if( m_pDicomImageHelper != nullptr && m_pDicomImageHelper->getDICOMVolume()->getDICOMSeriesImage().size() > 0 )
    {
        m_nSeriesImageIndex += 1;
        m_nSeriesImageIndex = m_nSeriesImageIndex % m_pDicomImageHelper->getDICOMVolume()->getDICOMSeriesImage().size();
        Invalidate(FALSE);
    }
    return CView::OnMouseWheel(nFlags, zDelta, pt);
}
