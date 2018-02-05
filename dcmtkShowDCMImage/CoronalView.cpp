#include "stdafx.h"
#include "CoronalView.h"
#include "Resource.h"

#include "gl/GLU.h"

#include "GLShaderMgr.h"
#include "GLFunctionParse.h"
#include "coronalViewShader.h"
#include "GLShader.h"

#include "..//dependence//glm//glm.hpp"
#include "..//dependence//glm//vec3.hpp"
#include "..//dependence//glm//vec4.hpp"
#include "..//dependence//glm//mat4x4.hpp"
#include "..//dependence//glm//gtc//matrix_transform.hpp"

GLfloat cubeAlpha1 = 0.0f;

GLfloat dOrthoSize2 = 1.0f;

#define BUFFER_OFFSET(offset) ((void*)(offset))

IMPLEMENT_DYNCREATE(CoronalView, USSTBaseView)
BEGIN_MESSAGE_MAP(CoronalView, USSTBaseView)

    ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()

void toglmMatrix( GLfloat* glfloatMatrix, glm::mat4x4& glmMatrix )
    {
        for( int i = 0; i<4; ++i )
        {
            for( int j = 0; j<4; ++j )
            {
                glmMatrix[i][j] = glfloatMatrix[ i*4+j ];
            }
        }
    }

void toGLfloatMatrix( GLfloat* glfoatMatrix, glm::mat4x4 glmMatrix )
    {
        for( int i = 0; i<4; ++i )
        {
            for( int j = 0; j<4; ++j )
            {
                glfoatMatrix[ i*4+j ] = glmMatrix[i][j];
            }
        }
    }

CoronalView::CoronalView(void)
{
    m_n3DTextureID = 0;

    m_modelMatrix[0]=m_modelMatrix[5]=m_modelMatrix[10]=m_modelMatrix[15] = 1.0f;
    m_modelMatrix[1]=m_modelMatrix[2]=m_modelMatrix[3]=m_modelMatrix[4] = 0.0f;
	m_modelMatrix[6]=m_modelMatrix[7]=m_modelMatrix[8]=m_modelMatrix[9] = 0.0f;
	m_modelMatrix[11]=m_modelMatrix[12]=m_modelMatrix[13]=m_modelMatrix[14] = 0.0f;

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
        CRect rc;
        GetClientRect( &rc );
        glViewport( rc.left, rc.top, rc.right, rc.bottom );
        //glDisable( GL_BLEND );
        glFrontFace( GL_CCW );
        glCullFace( GL_BACK );
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glEnable(GL_TEXTURE_3D);
		glBindTexture( GL_TEXTURE_3D,  m_n3DTextureID );

        /*glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluPerspective( 90.0f, rc.right/rc.bottom, 0.9f, 6.0f );

        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        gluLookAt( 0, 0,3, 0,0,0, 0,1,0 );*/
        
        glColor4f( 1.0f,1.0f,1.0f ,1.0f);


        glm::mat4x4 modelviewMatrix;
	    modelviewMatrix = glm::lookAt( glm::vec3(0.0,0.0,1.0),glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,1.0,0.0) );
        glm::mat4x4 projectionMatrix;
        projectionMatrix = glm::perspective(90.0, (double)rc.right/rc.bottom, 0.9, 6.0);

        glm::mat4x4 modelMatrix;

        modelMatrix = projectionMatrix*modelviewMatrix;

        toGLfloatMatrix( &m_modelMatrix[0], modelMatrix );

        

        m_pGLShaderMgr->getGLShader()[1]->begin();
        GLFunctionParse::glBindBuffer( GL_ARRAY_BUFFER, m_nFrameDataVBO );
        GLFunctionParse::glBindVertexArray( m_nCoronalViewVAO );
        GLuint tex = GLFunctionParse::glGetUniformLocation( m_pGLShaderMgr->getGLShader()[1]->getprogramID(), "baseTexture" );

        GLFunctionParse::glUniform1i( tex, 0 ); // default, the active texture uint is zero, so this value should be set to zero.

        GLuint mModelViewProjectionMatrixIndexPlane = GLFunctionParse::glGetUniformLocation( m_pGLShaderMgr->getGLShader()[1]->getprogramID(), "mModelViewProjectionMatrix" );

        GLFunctionParse::glUniformMatrix4fv( mModelViewProjectionMatrixIndexPlane,1, GL_FALSE, m_modelMatrix );

        glDrawArrays( GL_QUADS, 0, 4 );
        GLFunctionParse::glBindBuffer( GL_ARRAY_BUFFER, 0 );
        GLFunctionParse::glBindVertexArray( 0 );
        m_pGLShaderMgr->getGLShader()[1]->end();

        /*glBegin( GL_QUADS );
            glTexCoord3f( 0.0f, 0.5f, 0.0f ); 
		    glVertex3f(-dOrthoSize2,-dOrthoSize2,0.0f);
		    glTexCoord3f( 1.0f, 0.5f, 0.0f );  
		    glVertex3f(dOrthoSize2,-dOrthoSize2,0.0f);
		    glTexCoord3f( 1.0f, 0.5f, 1.0f );  
		    glVertex3f(dOrthoSize2,dOrthoSize2,0.0f);
		    glTexCoord3f( 0.0f, 0.5f, 1.0f );  
		    glVertex3f(-dOrthoSize2,dOrthoSize2,0.0f);
        glEnd();*/
		
		glBindTexture( GL_TEXTURE_3D, 0 );

        glLineWidth( 2.0f );
        /*glBegin( GL_LINES );
            glColor4f( 0.0f,1.0f,0.0f, 1.0f );
            glVertex3f( -2.0f, 0.0f, -0.5f );
            glVertex3f( 2.0f, 0.0f, -0.5f );
            glVertex3f( 0.0f,-2.0f, -0.5f );
            glVertex3f( 0.0f,2.0f,-0.5f );
        glEnd();*/

        m_pGLShaderMgr->getGLShader()[0]->begin();
        GLFunctionParse::glBindBuffer( GL_ARRAY_BUFFER, m_nBufferVBO );
        GLFunctionParse::glBindVertexArray( m_nLineIndexVAO );
        GLuint nLineIndex = GLFunctionParse::glGetUniformLocation( m_pGLShaderMgr->getGLShader()[0]->getprogramID(), "vLineColor" );

        GLuint mModelViewProjectionMatrixIndexPlane1 = GLFunctionParse::glGetUniformLocation( m_pGLShaderMgr->getGLShader()[0]->getprogramID(), "mModelViewProjectionMatrix" );

        GLFunctionParse::glUniformMatrix4fv( mModelViewProjectionMatrixIndexPlane1,1, GL_FALSE, m_modelMatrix );

        GLFunctionParse::glUniform4f( nLineIndex, 0.0f,1.0f,0.0f, 1.0f );

        glDrawArrays( GL_LINES, 0,4 );
        GLFunctionParse::glBindBuffer( GL_ARRAY_BUFFER, 0 );
        GLFunctionParse::glBindVertexArray( 0 );
        m_pGLShaderMgr->getGLShader()[0]->end();

        

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
    if( lHint == static_cast<LPARAM>( CdcmtkShowDCMImageDoc::tagTexture3DChange.getValue() ) )
	{
        CdcmtkShowDCMImageDoc* pDoc = GetDocument();
        m_n3DTextureID = pDoc->getTexture3DSPV();
        initializeData();
		Invalidate();
	}




    
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

bool CoronalView::initializeData()
{
    wglMakeCurrent( m_pClientDCCoronal->GetSafeHdc(), m_hGLrcCoronal );

    GLfloat lineIndexData[][4] = 
    {
        { -1.0f, 0.0f, -0.5f, 1.0f },// line index data --- begin
        { 1.0f, 0.0f, -0.5f, 1.0f },
        { 0.0f, -1.0f, -0.5f, 1.0f },
        { 0.0f, 1.0f, -0.5f, 1.0f }// line index data ---end
        
    };

    GLfloat frameData[][4] = 
    {
        { -1.0f, -1.0f, 0.0f, 1.0f },// coronal view data ---begin
        { 1.0f, -1.0f, 0.0f, 1.0f },
        { 1.0f, 1.0f, 0.0f, 1.0f },
        { -1.0f, 1.0f, 0.0f, 1.0f }//coronal view data --- end
    };

    GLfloat frameTextureData[][3] = 
    {
        { 0.0f, 0.5f, 0.0f },// texture coord
        { 1.0f, 0.5f, 0.0f },
        { 1.0f, 0.5f, 1.0f },
        { 0.0f, 0.5f, 1.0f }
    };

    //GLfloat frameColorData[][4] = 
    //{
    //    { 0.0f, 0.5f, 0.0f ,1.0f},// texture coord
    //    { 1.0f, 0.5f, 0.0f ,1.0f},
    //    { 1.0f, 0.5f, 1.0f ,1.0f},
    //    { 0.0f, 0.5f, 1.0f ,1.0f}
    //};

    GLFunctionParse::glGenVertexArrays( 1, &m_nLineIndexVAO );
    GLFunctionParse::glBindVertexArray( m_nLineIndexVAO );
    GLFunctionParse::glGenBuffers( 1, &m_nBufferVBO );
    GLFunctionParse::glBindBuffer( GL_ARRAY_BUFFER, m_nBufferVBO );
    GLFunctionParse::glBufferData( GL_ARRAY_BUFFER, sizeof( lineIndexData ) , NULL, GL_STATIC_DRAW );
    
    GLFunctionParse::glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( lineIndexData ), lineIndexData );
    

    // line index
    m_pGLShaderMgr = std::make_shared< GLShaderMgr >( vertexLineIndexForMPR, fragLineIndexForMPR );
    m_pGLShaderMgr->getGLShader()[0]->begin();
    GLFunctionParse::glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    GLFunctionParse::glEnableVertexAttribArrayARB( 0 );
    m_pGLShaderMgr->getGLShader()[0]->end();
    GLFunctionParse::glBindVertexArray( 0);
    GLFunctionParse::glBindBuffer( GL_ARRAY_BUFFER, 0 );



    // frame data
    GLFunctionParse::glGenVertexArrays( 1, &m_nCoronalViewVAO );
    GLFunctionParse::glBindVertexArray( m_nCoronalViewVAO );
    GLFunctionParse::glGenBuffers( 1, &m_nFrameDataVBO );
    GLFunctionParse::glBindBuffer( GL_ARRAY_BUFFER, m_nFrameDataVBO );
    GLFunctionParse::glBufferData( GL_ARRAY_BUFFER, sizeof( frameData ) + sizeof( frameTextureData ) , NULL, GL_STATIC_DRAW );
    GLFunctionParse::glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( frameData ), frameData );
    GLFunctionParse::glBufferSubData( GL_ARRAY_BUFFER, sizeof( frameData ), sizeof( frameTextureData ), frameTextureData );

    m_pGLShaderMgr->insertGLShader( vertexFrameDataForMPR, frahFrameDataForMPR );
    m_pGLShaderMgr->getGLShader()[1]->begin();
    GLFunctionParse::glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( 0 ) );
    GLFunctionParse::glEnableVertexAttribArrayARB( 0 );
    GLFunctionParse::glVertexAttribPointer( 1,3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( sizeof( frameData ) ) );
    GLFunctionParse::glEnableVertexAttribArrayARB( 1 );
    m_pGLShaderMgr->getGLShader()[1]->end();

    GLFunctionParse::glBindVertexArray( 0);
    GLFunctionParse::glBindBuffer( GL_ARRAY_BUFFER, 0 );


    wglMakeCurrent( NULL, NULL );
    return true;
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

    //initializeData();

    return 0;
}


void CoronalView::OnSize(UINT nType, int cx, int cy)
{
    USSTBaseView::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here

    /*CdcmtkShowDCMImageDoc* pDoc = GetDocument();
    m_n3DTextureID = pDoc->getTexture3DSPV();*/
}
