
// dcmtkShowDCMImageView.cpp : implementation of the CdcmtkShowDCMImageView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "dcmtkShowDCMImage.h"
#endif

#include "dcmtkShowDCMImageDoc.h"
#include "dcmtkShowDCMImageView.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmdata/dcdatset.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
END_MESSAGE_MAP()

// CdcmtkShowDCMImageView construction/destruction

CdcmtkShowDCMImageView::CdcmtkShowDCMImageView()
{
	// TODO: add construction code here

}

CdcmtkShowDCMImageView::~CdcmtkShowDCMImageView()
{
    delete m_pDataSet;
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

    CRect rc;
    GetClientRect(&rc);

    //DcmFileFormat* pDicomFile = new DcmFileFormat();
    ////读DICOM文件
    //pDicomFile->loadFile("MR.dcm");
    ////得到数据集
    //DcmDataset *pDataset = pDicomFile->getDataset();

    //得到传输语法
    E_TransferSyntax xfer = m_pDataSet->getOriginalXfer();
    //根据传输语法构造DicomImage从fstart帧开始一共fcount帧
    DicomImage* pDicomImg = new DicomImage (m_pDataSet, xfer/*, 0, 0, 1*/);
    /*DcmTagKey dcmwidth( 0x0028,0x0050 );
    DcmTagKey dcmCenter( 0x0028,0x0051 );
    DcmElement* dcmWidthValue = NULL;
    m_pDataSet->findAndGetElement(dcmwidth, dcmWidthValue);
    auto length =   dcmWidthValue->getLength();*/
    pDicomImg->setWindow( 509, 885 );
    //DicomImage* pDicomImg = new DicomImage ("MR.dcm");
    //通过以下的方法得到并用BitmapHeadInformation的结构体来保存DICOM文件的信息
    LPBITMAPINFOHEADER m_lpBMIH;
    m_lpBMIH = (LPBITMAPINFOHEADER) new char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];
        m_lpBMIH->biSize = sizeof(BITMAPINFOHEADER);
        m_lpBMIH->biWidth = pDicomImg->getWidth();
        m_lpBMIH->biHeight = pDicomImg->getHeight();
        m_lpBMIH->biPlanes = 1;
        m_lpBMIH->biBitCount = 24;
        m_lpBMIH->biCompression = BI_RGB;
        m_lpBMIH->biSizeImage = 0;
        m_lpBMIH->biXPelsPerMeter = 0;
        m_lpBMIH->biYPelsPerMeter = 0;
    void* pDicomDibits;
    //得到DICOM文件第frame的DIB数据(假设是24位的)
    pDicomImg->createWindowsDIB(pDicomDibits, 0, 0, 24, 1, 1);
    StretchDIBits(pDC->GetSafeHdc(),0,0,rc.right/*m_lpBMIH->biWidth*/,rc.bottom/*m_lpBMIH ->biHeight*/,0,0,m_lpBMIH->biWidth,m_lpBMIH->biHeight,pDicomDibits, (LPBITMAPINFO) m_lpBMIH,DIB_RGB_COLORS, SRCCOPY);
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
    DcmFileFormat* pDicomFile = new DcmFileFormat();
    //读DICOM文件
    pDicomFile->loadFile("MR.dcm");
    //得到数据集
    m_pDataSet = pDicomFile->getDataset();
    return 0;
}
