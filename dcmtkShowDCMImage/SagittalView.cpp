#include "stdafx.h"
#include "SagittalView.h"
#include "resource.h"

#include "gl/GLU.h"
#include "glext.h"
#include "DICOMImageHelper.h"
#include "DICOMVolume.h"
#include "GLFunctionParse.h"
#include <algorithm>
#include <functional>

IMPLEMENT_DYNCREATE(SagittalView, USSTBaseView)
BEGIN_MESSAGE_MAP(SagittalView, USSTBaseView)

    ON_WM_CREATE()
END_MESSAGE_MAP()

GLfloat dOrthoSize1 = 1.0f;

#define MAP_3DTEXT( TexIndex ) \
			glTexCoord3f(0.0f, 0.0f, ((float)TexIndex+1.0f)/2.0f);  \
		glVertex3f(-dOrthoSize1,-dOrthoSize1,TexIndex);\
		glTexCoord3f(1.0f, 0.0f, ((float)TexIndex+1.0f)/2.0f);  \
		glVertex3f(dOrthoSize1,-dOrthoSize1,TexIndex);\
		glTexCoord3f(1.0f, 1.0f, ((float)TexIndex+1.0f)/2.0f);  \
		glVertex3f(dOrthoSize1,dOrthoSize1,TexIndex);\
		glTexCoord3f(0.0f, 1.0f, ((float)TexIndex+1.0f)/2.0f);  \
		glVertex3f(-dOrthoSize1,dOrthoSize1,TexIndex);

SagittalView::SagittalView(void)
{
    m_n3DTextureID = 0;
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
    if( m_n3DTextureID == 0 )
    {
        CRect rc;
        GetClientRect( &rc );

        CString str = " Sagittal view ";

        pDC->TextOutA( rc.left + 20, rc.top + 20 , str);
    }
    else
    {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        wglMakeCurrent( m_pClientDCSagittal->GetSafeHdc(), m_hGLrcSagittal );
        glEnable(GL_TEXTURE_3D);
		glBindTexture( GL_TEXTURE_3D,  m_n3DTextureID );

        glBegin( GL_QUADS );
        glTexCoord3f(0.0f, 0.0f, 0.5f); 
		glVertex3f(-dOrthoSize1,-dOrthoSize1,0.0f);
		glTexCoord3f(1.0f, 0.0f, 0.5f);  
		glVertex3f(dOrthoSize1,-dOrthoSize1,0.0f);
		glTexCoord3f(1.0f, 1.0f, 0.5f);  
		glVertex3f(dOrthoSize1,dOrthoSize1,0.0f);
		glTexCoord3f(0.0f, 1.0f, 0.5f);  
		glVertex3f(-dOrthoSize1,dOrthoSize1,0.0f);
        glEnd();

  //      glEnable( GL_ALPHA_TEST );
		//glEnable(GL_BLEND);
		//glBlendFunc( GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR );
	

  //      glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

		//glDisable( GL_CULL_FACE );
		////
		//glEnable(GL_TEXTURE_3D);
		//glBindTexture( GL_TEXTURE_3D,  m_n3DTextureID );
		//CRect rc;
		//GetClientRect(rc);
		//glViewport( 0,0, rc.right, rc.bottom );
		//glMatrixMode( GL_PROJECTION );
		//glLoadIdentity();
		//gluPerspective( 45,1,1.0f,10.0f );

		//glMatrixMode( GL_MODELVIEW );
		//glLoadIdentity();
		//gluLookAt( 0.0f,0.0f,4.0f, 0.0f,0.0f,0.0f, 0.0f,1.0f,0.0f );
		//

		//glMatrixMode( GL_TEXTURE );
		//glLoadIdentity();
		//glTranslatef( 0.5f, 0.5f, 0.5f );

		///*glScaled( (float)nWidth/(float)nWidth, 
		//-1.0f*(float)nWidth/(float)(float)nHeight, 
		//(float)nWidth/(float)nDepth);*/
  //      
		////glRotatef( 30, 0.0f,0.0f,1.0f );
		//glTranslatef( -0.5f,-0.5f,-0.5f );

		///*glEnable(GL_TEXTURE_3D);
		//glBindTexture( GL_TEXTURE_3D,  m_n3DTextureID );*/
		//	for ( float fIndx = -1.0f; fIndx <= 1.0f; fIndx+=0.01f )
		//	{
		//		glBegin(GL_QUADS);
		//			MAP_3DTEXT( fIndx );
		//		glEnd();
  //          }
		
		glBindTexture( GL_TEXTURE_3D, 0 );

        glFinish();

        SwapBuffers( m_pClientDCSagittal->GetSafeHdc() );

        wglMakeCurrent( NULL, NULL );


    }
    




}

void SagittalView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
{
    if( lHint == static_cast<LPARAM>( CdcmtkShowDCMImageDoc::tagDICOMImport.getValue() ) )
	{
		initVolumeData();

        CdcmtkShowDCMImageDoc* pDoc = GetDocument();
        pDoc->setTexture3DSPV( m_n3DTextureID );

		Invalidate();
	}

    

}

GLuint SagittalView::getTexture3D()
{
    return m_n3DTextureID;
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

HGLRC SagittalView::getGLRenderContext()
{
    return m_hGLrcSagittal;
}


int SagittalView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (USSTBaseView::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  Add your specialized creation code here
    m_pClientDCSagittal = new CClientDC( this );

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

    HDC safeHDC = m_pClientDCSagittal->GetSafeHdc();

    int nPixelFormID = ChoosePixelFormat( safeHDC, &pfd );

    if( nPixelFormID == 0 )
    {
        MessageBox(_T("fail to choosePixelFormat"));
        return false;
    }

    if( SetPixelFormat( m_pClientDCSagittal->GetSafeHdc(), nPixelFormID,&pfd ) == false )
    {
        MessageBox(_T("fail to setPixelFormat"));
        return false;
    }

    glRc = wglCreateContext( m_pClientDCSagittal->GetSafeHdc() );

    wglMakeCurrent( m_pClientDCSagittal->GetSafeHdc(), glRc );

    m_hGLrcSagittal = wglGetCurrentContext();
    //glClearColor( 1.0f,1.0f,1.0f ,1.0f);
    return true;
}


bool SagittalView::initVolumeData()
{
	CdcmtkShowDCMImageDoc* pDoc = GetDocument();
	ASSERT_VALID( pDoc );

	if(!pDoc)
	{
		return false;
	}
	wglMakeCurrent( m_pClientDCSagittal->GetSafeHdc(), m_hGLrcSagittal );
	std::shared_ptr<DICOMImageHelper> pDicomHelper = pDoc->getDicomImageHelper();

	if( m_n3DTextureID != 0 )
	{
		glDeleteTextures( 1, &m_n3DTextureID );
	}
	glGenTextures( 1, &m_n3DTextureID );

	glBindTexture( GL_TEXTURE_3D, m_n3DTextureID );
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	

	int nWidth = pDicomHelper->getDICOMVolume()->getDICOMSeriesImage()[0]->m_nWidth;
	int nHeight = pDicomHelper->getDICOMVolume()->getDICOMSeriesImage()[0]->m_nHeight;
	int nDepth = pDicomHelper->getDICOMVolume()->getDepth();

    int nImageLen = pDicomHelper->getDICOMVolume()->getDICOMSeriesImage()[0]->m_nLength;
	//int nImageLen = pDicomHelper->getDICOMVolume()->getDICOMSeriesImage()[0]->m_nWidth * pDicomHelper->getDICOMVolume()->getDICOMSeriesImage()[0]->m_nHeight *4;
	unsigned char* pRGBBuffer = new unsigned char[ nImageLen*nDepth ];
	if( !pRGBBuffer)
	{
		return false;
	}
	unsigned char* pTemp = pRGBBuffer;
	for( int k = 0; k < nDepth; ++k )
	{
		memcpy( pTemp + k*nImageLen, pDicomHelper->getDICOMVolume()->getDICOMSeriesImage()[k]->m_pPixelData, nImageLen );
	}
    GLFunctionParse::glTexImage3D( GL_TEXTURE_3D,0,GL_RGB, nWidth , nHeight,nDepth , 0,GL_RGB, GL_UNSIGNED_BYTE, pRGBBuffer );
		
	delete[] pRGBBuffer;
	
	glBindTexture( GL_TEXTURE_3D, 0 );
	
	wglMakeCurrent( NULL, NULL );
	return true;
}
