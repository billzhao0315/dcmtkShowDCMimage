
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
#include "COpenGLView.h"
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
//void* pDicomDibits = NULL;
//unsigned char* pRGBDicomDibits;
//LPBITMAPINFOHEADER m_lpBMIH;

#pragma comment( lib, "OpenGL32.lib" )
#pragma comment( lib, "glu32.lib" )

GLfloat dOrthoSize = 1.0f;

// Macro to draw the quad.
 // Performance can be achieved by making a call list.
 // To make it simple i am not using that now :-)
  #define MAP_3DTEXT( TexIndex ) \
            glTexCoord3f(0.0f, 0.0f, ((float)TexIndex+1.0f)/2.0f);  \
        glVertex3f(-dOrthoSize,-dOrthoSize,TexIndex);\
        glTexCoord3f(1.0f, 0.0f, ((float)TexIndex+1.0f)/2.0f);  \
        glVertex3f(dOrthoSize,-dOrthoSize,TexIndex);\
        glTexCoord3f(1.0f, 1.0f, ((float)TexIndex+1.0f)/2.0f);  \
        glVertex3f(dOrthoSize,dOrthoSize,TexIndex);\
        glTexCoord3f(0.0f, 1.0f, ((float)TexIndex+1.0f)/2.0f);  \
        glVertex3f(-dOrthoSize,dOrthoSize,TexIndex);

// CdcmtkShowDCMImageView
IMPLEMENT_DYNCREATE(COpenGLView, CView)

BEGIN_MESSAGE_MAP(COpenGLView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
    ON_WM_CREATE()
    //ON_COMMAND(ID_FILE_OPENDICOM, &CsplitDCMView::OnFileOpendicom)
//    ON_WM_MOUSEHWHEEL()
    ON_WM_MOUSEWHEEL()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

// CdcmtkShowDCMImageView construction/destruction

COpenGLView::COpenGLView()
{
	// TODO: add construction code here
    m_pClientDC = NULL;
    m_n3DTextureID = 0;
}

COpenGLView::~COpenGLView()
{
    
}

BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CdcmtkShowDCMImageView drawing

void COpenGLView::OnDraw(CDC* pDC)
{
	CdcmtkShowDCMImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//// TODO: add draw code for native data here

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

    if( m_n3DTextureID == 0 )
    {
        glBegin( GL_TRIANGLES );
        glColor3f(1.0f,0.0f,0.0f);
        glVertex2f( -0.5f,-0.5f );
        glColor3f( 0.0f,1.0f,0.0f );
        glVertex2f( 0.5f,-0.5f );
        glColor3f(0.0f,0.0f,1.0f);
        glVertex2f( 0.0f,0.5f );
        glEnd();
    }
    else
    {
        glEnable(GL_BLEND);
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        glEnable(GL_TEXTURE_3D);
        glBindTexture( GL_TEXTURE_3D,  m_n3DTextureID );
        for ( float fIndx = -1.0f; fIndx <= 1.0f; fIndx+=0.01f )
        {
            glBegin(GL_QUADS);
                MAP_3DTEXT( fIndx );
            glEnd();
        }
    }
    glFinish();
    SwapBuffers( m_pClientDC->GetSafeHdc() );
}


// CdcmtkShowDCMImageView printing


void COpenGLView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL COpenGLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COpenGLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COpenGLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void COpenGLView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void COpenGLView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CdcmtkShowDCMImageView diagnostics

#ifdef _DEBUG
void COpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CdcmtkShowDCMImageDoc* COpenGLView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CdcmtkShowDCMImageDoc)));
	return (CdcmtkShowDCMImageDoc*)m_pDocument;
}
#endif //_DEBUG


// CdcmtkShowDCMImageView message handlers



bool COpenGLView::GLSetting()
{

    m_pClientDC = new CClientDC(this);

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

    int nPixelFormID = ChoosePixelFormat( m_pClientDC->GetSafeHdc(), &pfd );

    if( nPixelFormID == 0 )
    {
        MessageBox(_T("fail to choosePixelFormat"));
        return false;
    }

    if( SetPixelFormat( m_pClientDC->GetSafeHdc(), nPixelFormID,&pfd ) == false )
    {
        MessageBox(_T("fail to setPixelFormat"));
        return false;
    }

    glRc = wglCreateContext( m_pClientDC->GetSafeHdc() );

    wglMakeCurrent( m_pClientDC->GetSafeHdc(), glRc );

    return true;
}

int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
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


//void CdcmtkShowDCMImageView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//    // This feature requires Windows Vista or greater.
//    // The symbol _WIN32_WINNT must be >= 0x0600.
//    // TODO: Add your message handler code here and/or call default
//    m_nSeriesImageIndex += 1;
//    Invalidate(FALSE);
//    CView::OnMouseHWheel(nFlags, zDelta, pt);
//}


BOOL COpenGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: Add your message handler code here and/or call default
    return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void COpenGLView::OnDestroy()
{
    CView::OnDestroy();

    // TODO: Add your message handler code here
    HGLRC glRc = wglGetCurrentContext();
    wglMakeCurrent( NULL, NULL );
    if( glRc )
    {
        wglDeleteContext( glRc );
    }
    if( m_pClientDC )
    {
        delete m_pClientDC;
    }
}


void COpenGLView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
{
    // TODO: Add your specialized code here and/or call the base class

    if( lHint == static_cast<LPARAM>( CdcmtkShowDCMImageDoc::tagDICOMImport.getValue() ) )
    {
        initVolumeData();
        Invalidate();
    }

}

bool COpenGLView::initVolumeData()
{
    CdcmtkShowDCMImageDoc* pDoc = GetDocument();
    ASSERT_VALID( pDoc );

    if(!pDoc)
    {
        return false;
    }

    std::shared_ptr<DICOMImageHelper> pDicomHelper = pDoc->getDicomImageHelper();

    if( m_n3DTextureID != 0 )
    {
        glDeleteTextures( 1, &m_n3DTextureID );
    }
    glGenTextures( 1, &m_n3DTextureID );

    glBindTexture( GL_TEXTURE_3D, m_n3DTextureID );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    PFNGLTEXIMAGE3DPROC glTexImage3D = reinterpret_cast<PFNGLTEXIMAGE3DPROC>(wglGetProcAddress("glTexImage3D"));

    int nWidth = pDicomHelper->getDICOMVolume()->getDICOMSeriesImage()[0]->m_nWidth;
    int nHeight = pDicomHelper->getDICOMVolume()->getDICOMSeriesImage()[0]->m_nHeight;
    int nDepth = pDicomHelper->getDICOMVolume()->getDepth();

    int nImageLen = pDicomHelper->getDICOMVolume()->getDICOMSeriesImage()[0]->m_nLength;

    char* pRGBBuffer = new char[ nWidth*nHeight*nDepth*3 ];
    if( !pRGBBuffer)
    {
        return false;
    }

    for( int k = 0; k < nDepth; ++k )
    {
        for( int i = 0; i < nImageLen; ++i )
        {
            pRGBBuffer[ k*nImageLen + i ] = pDicomHelper->getDICOMVolume()->getDICOMSeriesImage()[k]->m_pPixelData[i];
        }
    }



    glTexImage3D( GL_TEXTURE_3D,0,GL_RGB, nWidth , nHeight,nDepth ,
        0,GL_RGB, GL_UNSIGNED_BYTE, pRGBBuffer );

    delete[] pRGBBuffer;

    return true;
}
