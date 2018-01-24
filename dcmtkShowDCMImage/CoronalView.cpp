#include "stdafx.h"
#include "CoronalView.h"
#include "Resource.h"

#include "gl/GLU.h"

GLfloat cubeAlpha1 = 0.0f;

GLfloat dOrthoSize2 = 1.0f;

IMPLEMENT_DYNCREATE(CoronalView, USSTBaseView)
BEGIN_MESSAGE_MAP(CoronalView, USSTBaseView)

    ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()
CoronalView::CoronalView(void)
{
    m_n3DTextureID = 0;
}


CoronalView::~CoronalView(void)
{
}

void CoronalView::OnDraw(CDC* /*pDC*/)
{
    CdcmtkShowDCMImageDoc* pDoc = GetDocument();
    ASSERT_VALID( pDoc );
    if( !pDoc )
    {
        return;
    }

    if( m_n3DTextureID == 0 )
    {
        wglMakeCurrent( m_pClientDCCoronal->GetSafeHdc(), m_hGLrcCoronal );
        glClearColor( 0.0f,0.0f,0.0f,1.0f );
        drawCube();
        SwapBuffers( m_pClientDCCoronal->GetSafeHdc() );
        wglMakeCurrent( NULL, NULL );
    }
    else
    {
        
        //glDrawBuffer( GL_FRONT );
        wglMakeCurrent( m_pClientDCCoronal->GetSafeHdc(), m_hGLrcCoronal );
        glClearColor( 0.0f,0.0f,0.0f ,1.0f);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glDisable( GL_BLEND );
        glFrontFace( GL_CCW );
        glCullFace( GL_BACK );
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glEnable(GL_TEXTURE_3D);
		glBindTexture( GL_TEXTURE_3D,  m_n3DTextureID );

        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluPerspective( 45.0f, 1.0f, 1.0f, 6.0f );

        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        gluLookAt( 0, 0,4, 0,0,0, 0,1,0 );
        
        glColor4f( 1.0f,1.0f,1.0f ,1.0f);
        glBegin( GL_QUADS );
            glTexCoord3f(0.0f, 0.0f, 0.5f); 
		    glVertex3f(-dOrthoSize2,-dOrthoSize2,0.0f);
		    glTexCoord3f(1.0f, 0.0f, 0.5f);  
		    glVertex3f(dOrthoSize2,-dOrthoSize2,0.0f);
		    glTexCoord3f(1.0f, 1.0f, 0.5f);  
		    glVertex3f(dOrthoSize2,dOrthoSize2,0.0f);
		    glTexCoord3f(0.0f, 1.0f, 0.5f);  
		    glVertex3f(-dOrthoSize2,dOrthoSize2,0.0f);
        glEnd();
		
		glBindTexture( GL_TEXTURE_3D, 0 );

        glFinish();
        /*GLenum err = glGetError(  );
        switch( err )
        {
            case GL_INVALID_ENUM :
                MessageBox(_T("GL_INVALID_ENUM"),_T(" sagittal and coronal "),MB_OK|MB_ICONERROR);
            break;
            case GL_INVALID_VALUE :
                MessageBox(_T("GL_INVALID_VALUE"),_T(" sagittal and coronal "),MB_OK|MB_ICONERROR);
            break;
            case GL_INVALID_OPERATION :
                MessageBox(_T("GL_INVALID_OPERATION"),_T(" sagittal and coronal "),MB_OK|MB_ICONERROR);
            break;
            case GL_NO_ERROR :
                MessageBox(_T("GL_NO_ERROR"),_T(" sagittal and coronal "),MB_OK|MB_ICONERROR);
            break;
            case GL_STACK_OVERFLOW :
                MessageBox(_T("GL_STACK_OVERFLOW"),_T(" sagittal and coronal "),MB_OK|MB_ICONERROR);
            break;
            case GL_STACK_UNDERFLOW :
                MessageBox(_T("GL_STACK_UNDERFLOW"),_T(" sagittal and coronal "),MB_OK|MB_ICONERROR);
            break;
            case GL_OUT_OF_MEMORY :
                MessageBox(_T("GL_OUT_OF_MEMORY"),_T(" sagittal and coronal "),MB_OK|MB_ICONERROR);
            break;
            default:
                break;
        }*/

        SwapBuffers( m_pClientDCCoronal->GetSafeHdc() );

        wglMakeCurrent( NULL, NULL );
    }
    
        


}

void CoronalView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
{
    if( lHint == static_cast<LPARAM>( CdcmtkShowDCMImageDoc::tagDICOMImport.getValue() ) )
	{

        
		Invalidate();
	}

    CdcmtkShowDCMImageDoc* pDoc = GetDocument();
    m_n3DTextureID = pDoc->getTexture3DSPV();
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

void CoronalView::setGLRenderContext(HGLRC hGLrcCoronal)
{
    m_hGLrcCoronal = hGLrcCoronal;
}

void CoronalView::setClientDC(CClientDC* pClientDCCoronal)
{
    m_pClientDCCoronal = pClientDCCoronal;
}

HGLRC CoronalView::getGLRenderContext()
{
    return m_hGLrcCoronal;
}


void CoronalView::drawCube()
{
    //glDrawBuffer( GL_FRONT_AND_BACK );
    //glDrawBuffer( GL_BACK );
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
        glColor4f( 0.0f,0.0f,1.0f ,cubeAlpha1);
        glVertex3f( 0.0f,0.0f,1.0f );
        glColor4f( 1.0f,0.0f,1.0f ,cubeAlpha1);
        glVertex3f( 1.0f,0.0f,1.0f );
        glColor4f( 1.0f,1.0f,1.0f ,cubeAlpha1);
        glVertex3f( 1.0f,1.0f,1.0f );
        glColor4f( 0.0f,1.0f,1.0f ,cubeAlpha1);
        glVertex3f( 0.0f,1.0f,1.0f );

        //left face
        glColor4f( 0.0f,0.0f,1.0f ,cubeAlpha1);
        glVertex3f( 0.0f,0.0f,1.0f );
        glColor4f( 0.0f,1.0f,1.0f ,cubeAlpha1);
        glVertex3f( 0.0f,1.0f,1.0f );
        glColor4f( 0.0f,1.0f,0.0f ,cubeAlpha1);
        glVertex3f( 0.0f,1.0f,0.0f );
        glColor4f( 0.0f,0.0f,0.0f ,cubeAlpha1);
        glVertex3f( 0.0f,0.0f,0.0f );

        // top face
        glColor4f( 0.0f,1.0f,1.0f ,cubeAlpha1);
        glVertex3f( 0.0f,1.0f,1.0f );
        glColor4f( 1.0f,1.0f,1.0f ,cubeAlpha1);
        glVertex3f( 1.0f,1.0f,1.0f );
        glColor4f( 1.0f,1.0f,0.0f ,cubeAlpha1);
        glVertex3f( 1.0f,1.0f,0.0f );
        glColor4f( 0.0f,1.0f,0.0f ,cubeAlpha1);
        glVertex3f( 0.0f,1.0f,0.0f );

        //right face
        glColor4f( 1.0f,1.0f,1.0f ,cubeAlpha1);
        glVertex3f( 1.0f,1.0f,1.0f );
        glColor4f( 1.0f,0.0f,1.0f ,cubeAlpha1);
        glVertex3f( 1.0f,0.0f,1.0f );
        glColor4f( 1.0f,0.0f,0.0f ,cubeAlpha1);
        glVertex3f( 1.0f,0.0f,0.0f );
        glColor4f( 1.0f,1.0f,0.0f ,cubeAlpha1);
        glVertex3f( 1.0f,1.0f,0.0f );

        // bottom face
        glColor4f( 0.0f,0.0f,1.0f ,cubeAlpha1);
        glVertex3f( 0.0f,0.0f,1.0f );
        glColor4f( 0.0f,0.0f,0.0f ,cubeAlpha1);
        glVertex3f( 0.0f,0.0f,0.0f );
        glColor4f( 1.0f,0.0f,0.0f ,cubeAlpha1);
        glVertex3f( 1.0f,0.0f,0.0f );
        glColor4f( 1.0f,0.0f,1.0f ,cubeAlpha1);
        glVertex3f( 1.0f,0.0f,1.0f );

        // back face
        glColor4f( 0.0f,0.0f,0.0f ,cubeAlpha1);
        glVertex3f( 0.0f,0.0f,0.0f );
        glColor4f( 0.0f,1.0f,0.0f ,cubeAlpha1);
        glVertex3f( 0.0f,1.0f,0.0f );
        glColor4f( 1.0f,1.0f,0.0f ,cubeAlpha1);
        glVertex3f( 1.0f,1.0f,0.0f );
        glColor4f( 1.0f,0.0f,0.0f ,cubeAlpha1);
        glVertex3f( 1.0f,0.0f,0.0f );
    glEnd();
    glFinish();
}


int CoronalView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (USSTBaseView::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  Add your specialized creation code here
    m_pClientDCCoronal = new CClientDC(this);

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

    HDC safeHDC = m_pClientDCCoronal->GetSafeHdc();

    int nPixelFormID = ChoosePixelFormat( safeHDC, &pfd );

    if( nPixelFormID == 0 )
    {
        MessageBox(_T("fail to choosePixelFormat"));
        return false;
    }

    if( SetPixelFormat( m_pClientDCCoronal->GetSafeHdc(), nPixelFormID,&pfd ) == false )
    {
        MessageBox(_T("fail to setPixelFormat"));
        return false;
    }

    glRc = wglCreateContext( m_pClientDCCoronal->GetSafeHdc() );

    wglMakeCurrent( m_pClientDCCoronal->GetSafeHdc(), glRc );
    glClearColor( 0.0f,0.0f,0.0f,1.0f );
    m_hGLrcCoronal = wglGetCurrentContext();

    return 0;
}


void CoronalView::OnSize(UINT nType, int cx, int cy)
{
    USSTBaseView::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here

    CdcmtkShowDCMImageDoc* pDoc = GetDocument();
    m_n3DTextureID = pDoc->getTexture3DSPV();
}
