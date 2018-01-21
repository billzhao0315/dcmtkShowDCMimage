
// dcmtkShowDCMImageView.cpp : implementation of the COpenGLView class
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
#include "algorithm"

#include "gl/GLU.h"

#include <fstream> 
#include <functional>

#include "GLShaderMgr.h"
#include "GLFunctionParse.h"
#include "vertexfragmentShader.h"
#include "GLShader.h"

#include "..//dependence//glm//glm.hpp"
#include "..//dependence//glm//vec3.hpp"
#include "..//dependence//glm//vec4.hpp"
#include "..//dependence//glm//mat4x4.hpp"
#include "..//dependence//glm//gtc//matrix_transform.hpp"

#define BUFFER_OFFSET(offset) ((void*)(offset))

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//void* pDicomDibits = NULL;
//unsigned char* pRGBDicomDibits;
//LPBITMAPINFOHEADER m_lpBMIH;

#pragma comment( lib, "OpenGL32.lib" )
#pragma comment( lib, "glu32.lib" )

GLfloat dOrthoSize = 1.0f;
bool bRgb = false;

GLfloat newIndex = 0.5f;

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

// COpenGLView
IMPLEMENT_DYNCREATE(COpenGLView, USSTBaseView)

BEGIN_MESSAGE_MAP(COpenGLView, USSTBaseView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
//	ON_WM_CONTEXTMENU()
//	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	//ON_COMMAND(ID_FILE_OPENDICOM, &CsplitDCMView::OnFileOpendicom)
//    ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


class COpenGLView::mathImpl
{
public:
	mathImpl()
	{
		/*mdRotation[0]=mdRotation[5]=mdRotation[10]=mdRotation[15] = 1.0f;
		mdRotation[1]=mdRotation[2]=mdRotation[3]=mdRotation[4] = 0.0f;
		mdRotation[6]=mdRotation[7]=mdRotation[8]=mdRotation[9] = 0.0f;
		mdRotation[11]=mdRotation[12]=mdRotation[13]=mdRotation[14] = 0.0f;*/
        resetRotation();

		mfRot[0]=mfRot[1]=mfRot[2]=0.0f;
        resetModelMatrix();
        m_bDemoCube = true;
	}
	~mathImpl()
	{
		
	}

    void setDemoCubeFlag( bool bFlagDemoCube )
	{
	    m_bDemoCube = bFlagDemoCube;
	}

	const float* getMatrix()
	{
        if( m_bDemoCube )
        {
            calcModelViewProjectionMatrix();
        }
        
		return mdRotation;
	}

	void Rotate( double dx, double dy, double dz )
	{
		mfRot[0] = dx;
		mfRot[1] = dy;
		mfRot[2] = dz;
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glLoadMatrixf( mdRotation );
		glRotated( mfRot[0], 1.0f, 0,0 );
		glRotated( mfRot[1], 0, 1.0f,0 );
		glRotated( mfRot[2], 0, 0,1.0f );
		glGetFloatv( GL_MODELVIEW_MATRIX, mdRotation );
		glLoadIdentity();
	}

    std::vector<GLfloat> crossVector( const std::vector<GLfloat> firstVector, const std::vector<GLfloat> secondVector )
    {
        std::vector<GLfloat> crossResult(3);
        crossResult[0] = firstVector[1]*secondVector[2] - firstVector[2]*secondVector[1];
        crossResult[1] = firstVector[2]*secondVector[0] - firstVector[0]*secondVector[2];
        crossResult[2] = firstVector[0]*secondVector[1] - firstVector[1]*secondVector[0];
        return crossResult;
    }

    void calculateRotateMatrix( CPoint startPoint, CPoint currentPoint )
    {

        double angleX = (currentPoint.y - startPoint.y);
        double angleY = (currentPoint.x - startPoint.x);

        double rotateAngle = sqrt( angleX*angleX + angleY*angleY );

        rotateAngle = abs(rotateAngle) < 1 ? 1 : rotateAngle;

        //double mouseDirection[3] = { point.x - m_pStartLocation.x, point.y - m_pStartLocation.y,0 };

        std::vector<GLfloat> mouseDirection(3);
        mouseDirection[0] = static_cast< GLfloat >( currentPoint.x - startPoint.x );
        mouseDirection[1] = static_cast< GLfloat >( startPoint.y - currentPoint.y );
        mouseDirection[2] = 0;

        std::vector<GLfloat> viewDirection(3);
        viewDirection[0] = 0.0;
        viewDirection[1] = 0.0;
        viewDirection[2] = 1.0;

        /*rotateVector[0] = mouseDirection[1]*viewDirection[2] - mouseDirection[2]*viewDirection[1];
        rotateVector[1] = mouseDirection[2]*viewDirection[0] - mouseDirection[0]*viewDirection[2];
        rotateVector[2] = mouseDirection[0]*viewDirection[1] - mouseDirection[1]*viewDirection[0];

        rotateVector[0] = -rotateVector[0];
        rotateVector[1] = -rotateVector[1];
        rotateVector[2] = -rotateVector[2];*/

        std::vector<GLfloat> rotateVector = crossVector( viewDirection, mouseDirection );


        glm::mat4x4 modelRotateMatrix;
        glm::vec3 directionVector( rotateVector[0],rotateVector[1],rotateVector[2] );
        modelRotateMatrix = glm::rotate( modelRotateMatrix, (float)rotateAngle, directionVector );

        glm::mat4x4 preModelRotateMatrix;

        toglmMatrix( &m_modelMatrix[0], preModelRotateMatrix );

        modelRotateMatrix = modelRotateMatrix * preModelRotateMatrix;

        toGLfloatMatrix( &m_modelMatrix[0], modelRotateMatrix );

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

    void calcModelViewProjectionMatrix()
	{
        glm::mat4x4 modelviewMatrix;
	    modelviewMatrix = glm::lookAt( glm::vec3(0.0,0.0,3.0),glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,1.0,0.0) );
        glm::mat4x4 projectionMatrix;
        projectionMatrix = glm::perspective(45.0,1.0,1.0,10.0);

        glm::mat4x4 modelMatrix;

        toglmMatrix( &m_modelMatrix[0], modelMatrix );

        glm::mat4x4 modelTransformX;
        modelTransformX = glm::translate( modelTransformX,glm::vec3( -0.5,-0.5,-0.5 ) );

        glm::mat4x4 MVP = projectionMatrix*modelviewMatrix * modelMatrix * modelTransformX;

        

        toGLfloatMatrix( &mdRotation[0], MVP );

	}

    void resetRotation()
	{
	    mdRotation[0]=mdRotation[5]=mdRotation[10]=mdRotation[15] = 1.0f;
		mdRotation[1]=mdRotation[2]=mdRotation[3]=mdRotation[4] = 0.0f;
		mdRotation[6]=mdRotation[7]=mdRotation[8]=mdRotation[9] = 0.0f;
		mdRotation[11]=mdRotation[12]=mdRotation[13]=mdRotation[14] = 0.0f;
	}

    void resetModelMatrix()
	{
	    m_modelMatrix[0]=m_modelMatrix[5]=m_modelMatrix[10]=m_modelMatrix[15] = 1.0f;
		m_modelMatrix[1]=m_modelMatrix[2]=m_modelMatrix[3]=m_modelMatrix[4] = 0.0f;
		m_modelMatrix[6]=m_modelMatrix[7]=m_modelMatrix[8]=m_modelMatrix[9] = 0.0f;
		m_modelMatrix[11]=m_modelMatrix[12]=m_modelMatrix[13]=m_modelMatrix[14] = 0.0f;
	}


private:
	double mfRot[3];
	float  mdRotation[16];
    float  m_modelMatrix[16];
    bool   m_bDemoCube;
};

// COpenGLView construction/destruction

COpenGLView::COpenGLView()
{
	// TODO: add construction code here
	m_pClientDC = NULL;
	m_n3DTextureID = 0;
	m_pMathImpl = std::make_shared<mathImpl>();
}

COpenGLView::~COpenGLView()
{
	if( m_n3DTextureID > 0 )
	{
	    glDeleteTextures( 1, &m_n3DTextureID );
	}
}

BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// COpenGLView drawing

void COpenGLView::OnDraw(CDC* /*pDC*/)
{
	CdcmtkShowDCMImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//// TODO: add draw code for native data here

	wglMakeCurrent( m_pClientDC->GetSafeHdc(), m_hGLrc );

	glClearColor( 0.0f,0.0f,0.0f ,1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	CRect rc;
	GetClientRect( &rc );
	glViewport( rc.left, rc.top, rc.right, rc.bottom );
	if( m_n3DTextureID == 0 )
	{
		/*glBegin( GL_TRIANGLES );
		glColor3f(1.0f,0.0f,0.0f);
		glVertex2f( -0.5f,-0.5f );
		glColor3f( 0.0f,1.0f,0.0f );
		glVertex2f( 0.5f,-0.5f );
		glColor3f(0.0f,0.0f,1.0f);
		glVertex2f( 0.0f,0.5f );
		glEnd();*/

        glEnable( GL_CULL_FACE );
        glFrontFace( GL_CCW );
        glCullFace( GL_BACK );
        glPolygonMode( GL_FRONT, GL_FILL );
		m_pGLShaderMgr->getGLShader()[renderType::demoCube]->begin();
		GLFunctionParse::glBindVertexArray(m_nVertexArrayID);
        GLFunctionParse::glBindBuffer( GL_ARRAY_BUFFER, m_nBuffers );
		glDrawArrays(GL_QUADS,0,24);
        GLint mModelViewProjectionMatrix =  GLFunctionParse::glGetUniformLocation( m_pGLShaderMgr->getGLShader()[renderType::demoCube]->getprogramID(), std::string("mModelViewProjectionMatrix").c_str() );
        GLFunctionParse::glUniformMatrix4fv( mModelViewProjectionMatrix,1, GL_FALSE, m_pMathImpl->getMatrix() );
        GLFunctionParse::glBindVertexArray(0);
        GLFunctionParse::glBindBuffer( GL_ARRAY_BUFFER, 0 );
		m_pGLShaderMgr->getGLShader()[renderType::demoCube]->end();

        m_pGLShaderMgr->getGLShader()[renderType::IndexPlane]->begin();
        GLint mModelViewProjectionMatrixIndexPlane =  GLFunctionParse::glGetUniformLocation( m_pGLShaderMgr->getGLShader()[renderType::IndexPlane]->getprogramID(), std::string("mModelViewProjectionMatrix").c_str() );
        GLFunctionParse::glUniformMatrix4fv( mModelViewProjectionMatrixIndexPlane,1, GL_FALSE, m_pMathImpl->getMatrix() );

        GLint indexPlane =  GLFunctionParse::glGetUniformLocation( m_pGLShaderMgr->getGLShader()[renderType::IndexPlane]->getprogramID(), std::string("indexPlane").c_str() );
        GLFunctionParse::glUniform1f( indexPlane,newIndex );

        glDisable( GL_CULL_FACE );
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        GLFunctionParse::glBindVertexArray(m_nVertexArrayIDIndexPlane);
        GLFunctionParse::glBindBuffer(GL_ARRAY_BUFFER, m_nBuffersIndexPlane);
        
        /*GLFunctionParse::glBufferSubData( GL_ARRAY_BUFFER, sizeof(GLfloat)*5, sizeof(GLfloat), &newIndex );
        GLFunctionParse::glBufferSubData( GL_ARRAY_BUFFER, sizeof(GLfloat)*11, sizeof(GLfloat), &newIndex );
        GLFunctionParse::glBufferSubData( GL_ARRAY_BUFFER, sizeof(GLfloat)*17, sizeof(GLfloat), &newIndex );
        GLFunctionParse::glBufferSubData( GL_ARRAY_BUFFER, sizeof(GLfloat)*23, sizeof(GLfloat), &newIndex );*/

        glDrawArrays(GL_QUADS,0,4);

        GLFunctionParse::glBindVertexArray(0);
        GLFunctionParse::glBindBuffer( GL_ARRAY_BUFFER, 0 );
        m_pGLShaderMgr->getGLShader()[renderType::IndexPlane]->end();

        


	}
	else
	{
		glEnable( GL_ALPHA_TEST );
		//glAlphaFunc( GL_GREATER, 0.05f );
		glEnable(GL_BLEND);
		if( bRgb )
		{
			glBlendFunc( GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR );
		}
		else
		{
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		}

        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

		glDisable( GL_CULL_FACE );
		//
		glEnable(GL_TEXTURE_3D);
		glBindTexture( GL_TEXTURE_3D,  m_n3DTextureID );
		CRect rc;
		GetClientRect(rc);
		glViewport( 0,0, rc.right, rc.bottom );
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluPerspective( 45,1,1.0f,10.0f );

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		gluLookAt( 0.0f,0.0f,4.0f, 0.0f,0.0f,0.0f, 0.0f,1.0f,0.0f );
		

		glMatrixMode( GL_TEXTURE );
		glLoadIdentity();
		glTranslatef( 0.5f, 0.5f, 0.5f );

		/*glScaled( (float)nWidth/(float)nWidth, 
		-1.0f*(float)nWidth/(float)(float)nHeight, 
		(float)nWidth/(float)nDepth);*/
        
		glMultMatrixf( m_pMathImpl->getMatrix() ); 
		//glRotatef( 30, 0.0f,0.0f,1.0f );
		glTranslatef( -0.5f,-0.5f,-0.5f );

		/*glEnable(GL_TEXTURE_3D);
		glBindTexture( GL_TEXTURE_3D,  m_n3DTextureID );*/
		if( bRgb )
		{
			for ( float fIndx = -1.0f; fIndx <= 1.0f; fIndx+=0.01f )
			{
				glBegin(GL_QUADS);
					MAP_3DTEXT( fIndx );
				glEnd();
			}
		}
		else
		{
			/*float TexIndex = -0.5;
			glBegin( GL_QUADS );
			glTexCoord3f(0.0f, 0.0f, (TexIndex+1.0f)/2.0f);  
			glVertex3f(-dOrthoSize,-dOrthoSize,TexIndex);
			glTexCoord3f(1.0f, 0.0f, (TexIndex+1.0f)/2.0f);  
			glVertex3f(dOrthoSize,-dOrthoSize,TexIndex);
			glTexCoord3f(1.0f, 1.0f, (TexIndex+1.0f)/2.0f);  
			glVertex3f(dOrthoSize,dOrthoSize,TexIndex);
			glTexCoord3f(0.0f, 1.0f, (TexIndex+1.0f)/2.0f);  
			glVertex3f(-dOrthoSize,dOrthoSize,TexIndex);
			glEnd();*/
			for ( float fIndx = -1.0f; fIndx <= 1.0f; fIndx+=0.002f )
			{
				glBegin(GL_QUADS);
					MAP_3DTEXT( fIndx );
				glEnd();
			}
		}
		
		glBindTexture( GL_TEXTURE_3D, 0 );


		/*glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
		glLoadIdentity();
		unsigned int nCurrentIndex = 2 * pDoc->getCurrentImageIndex();
		unsigned int nImageSize = pDoc->getDicomImageHelper()->getDICOMVolume()->getDICOMSeriesImage().size();
		double nCurrentPosition = (double)nCurrentIndex / (double)nImageSize -1;
		glMultMatrixd( m_pMathImpl->getMatrix() );
		glBegin( GL_QUADS );
			glColor4f( 1.0f,0.0f,0.0f,0.5f );
			glVertex3f( -1.0f, -1.0f, nCurrentPosition );
			glVertex3f(  1.0f, -1.0f, nCurrentPosition );
			glVertex3f(  1.0f, 1.0f, nCurrentPosition );
			glVertex3f( -1.0f, 1.0f, nCurrentPosition );
		glEnd();
		glPopMatrix();*/


	}
	glFinish();
	SwapBuffers( m_pClientDC->GetSafeHdc() );

	wglMakeCurrent( NULL, NULL );

}


// COpenGLView printing


//void COpenGLView::OnFilePrintPreview()
//{
//#ifndef SHARED_HANDLERS
//	AFXPrintPreview(this);
//#endif
//}

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

//void COpenGLView::OnRButtonUp(UINT /* nFlags */, CPoint point)
//{
//	ClientToScreen(&point);
//	OnContextMenu(this, point);
//}

//void COpenGLView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
//{
//#ifndef SHARED_HANDLERS
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
//#endif
//}


// COpenGLView diagnostics

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


// COpenGLView message handlers



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

	HDC safeHDC = m_pClientDC->GetSafeHdc();

	int nPixelFormID = ChoosePixelFormat( safeHDC, &pfd );

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

	m_hGLrc = wglGetCurrentContext();

	bool bfnInit = GLFunctionParse::initGLFunction();
	if( bfnInit )
	{
		//initBufferData();
        initCubeData();
	}
	else
	{
		MessageBox(_T("fail to initGLFunction"), _T("error"), MB_OK);
		return false;
	}

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


//void COpenGLView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
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

    newIndex += 0.1f;
    if( abs(newIndex - 1.0f) < 1e-6 )
    {
        newIndex = 0.0f;
    }

    /*GLFunctionParse::glBindBuffer(GL_ARRAY_BUFFER, m_nBuffersIndexPlane);
        
    void* ptr = GLFunctionParse::glMapBuffer( GL_ARRAY_BUFFER, GL_READ_WRITE );
    ((GLfloat*)ptr)[1*3+2] = newIndex;
    ((GLfloat*)ptr)[3*3+2] = newIndex;
    ((GLfloat*)ptr)[5*3+2] = newIndex;
    ((GLfloat*)ptr)[7*3+2] = newIndex;
    GLFunctionParse::glUnmapBuffer( GL_ARRAY_BUFFER );
    GLFunctionParse::glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glFinish();*/

    Invalidate( FALSE );
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void COpenGLView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	HGLRC glRc = wglGetCurrentContext();
	HDC hDC =  wglGetCurrentDC();
	if( m_hGLrc == glRc && m_pClientDC->GetSafeHdc() == hDC )
	{
		wglMakeCurrent( NULL, NULL );
	}

	if( m_hGLrc )
	{
		wglDeleteContext( m_hGLrc );
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
        m_pMathImpl->resetRotation();
        m_pMathImpl->resetModelMatrix();
		initVolumeData();
        m_pMathImpl->setDemoCubeFlag( false );
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
	wglMakeCurrent( m_pClientDC->GetSafeHdc(), m_hGLrc );
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

	//bool bRgb = true;

	if( bRgb )
	{
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



		GLFunctionParse::glTexImage3D( GL_TEXTURE_3D,0,GL_RGB, nWidth , nHeight,nDepth ,
			0,GL_RGB, GL_UNSIGNED_BYTE, pRGBBuffer );
		
		delete[] pRGBBuffer;
	}
	else
	{
		if( pDicomHelper->getDICOMVolume()->getDICOMSeriesImage()[0]->m_eDataType == DICOMSerieImage::eDataType::DATATYPE_uSHORT )
		{
			unsigned char* pIntensity = new unsigned char[ nWidth*nHeight*4*nDepth ];
			if( !pIntensity )
			{
				return false;
			}

			/*std::string fileName("F:/vs2012program/readTFFfileTest/readTFFfileTest/tfflung.dat");*/
            std::string fileName("../dependence/TFFFiles/tfflung.dat");
            
			pDicomHelper->getDICOMVolume()->loadtffFile(fileName);

			int nImageLen = nWidth * nHeight *4;
			std::vector<std::shared_ptr<DICOMSerieImage>> imageSeries = pDicomHelper->getDICOMVolume()->getDICOMSeriesImage();
			auto itrBegin = imageSeries.begin();
			auto itrEnd = imageSeries.end();
			std::for_each( itrBegin, itrEnd, std::mem_fn( &DICOMSerieImage::mapToGray ) );
			//void* pTemp ;
			for( int k = 0; k < nDepth; ++k )
			{
				//pTemp = (unsigned short*)pIntensity + k*nImageLen;
				//memcpy( pIntensity + k*nImageLen, (unsigned short*)pDicomHelper->getDICOMVolume()->getDICOMSeriesImage()[k]->m_pOriginPixelData, nImageLen );
				memcpy( pIntensity + k*nImageLen, pDicomHelper->getDICOMVolume()->getDICOMSeriesImage()[k]->m_pRGBAPixelData, nImageLen );
				/*for( int len = 0; len < nImageLen; ++len )
				{
					unsigned short tempValue = ((unsigned short*)pDicomHelper->getDICOMVolume()->getDICOMSeriesImage()[k]->m_pOriginPixelData)[len];
					double maxValue = pDicomHelper->getDICOMVolume()->getDICOMSeriesImage()[k]->m_maxValue;
					double minValue = pDicomHelper->getDICOMVolume()->getDICOMSeriesImage()[k]->m_minValue;
					unsigned char resultValue = (tempValue - minValue) * 255/( maxValue - minValue );

					pIntensity[ k*nImageLen*4 + 4*len ] = resultValue;
					pIntensity[ k*nImageLen*4 + 4*len+1 ] = resultValue;
					pIntensity[ k*nImageLen*4 + 4*len+2 ] = resultValue;
					pIntensity[ k*nImageLen*4 + 4*len+3 ] = resultValue;
				}*/
				/*if( k == nDepth/2 )
				{
					std::ofstream out("out.txt");
					if( out.is_open() )
					{
						unsigned char* pTemp = pIntensity + k*nImageLen;
						for( int i = 0; i < nWidth; ++i  )
						{
							for( int j = 0; j < nHeight; ++j )
							{
								out<< pTemp[i*nWidth+j]<< " ";
							}
							out<<"\n";
						}
						out.close();
					}
				}*/
				
			}

			GLFunctionParse::glTexImage3D( GL_TEXTURE_3D,0,GL_RGBA, nWidth, nHeight,nDepth,0, GL_RGBA, GL_UNSIGNED_BYTE, pIntensity );
			delete[] pIntensity;
		}
		else
		{
			unsigned char* pIntensity = new unsigned char[ nWidth*nHeight*nDepth ];
			if( !pIntensity )
			{
				return false;
			}
			int nImageLen = nWidth * nHeight;
			for( int k=0; k< nDepth; ++k )
			{
				memcpy( pIntensity + k*nImageLen, pDicomHelper->getDICOMVolume()->getDICOMSeriesImage()[k]->m_pOriginPixelData, nImageLen );
			}
			GLFunctionParse::glTexImage3D( GL_TEXTURE_3D,0,GL_LUMINANCE8, nWidth,nHeight,nDepth,0,GL_LUMINANCE8, GL_UNSIGNED_BYTE, pIntensity );
			delete[] pIntensity;
		}
	}

	
	glBindTexture( GL_TEXTURE_3D, 0 );
	
	wglMakeCurrent( NULL, NULL );
	return true;
}




void COpenGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_ReferencePoint = point;
	CView::OnLButtonDown(nFlags, point);
}


void COpenGLView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if( nFlags & MK_LBUTTON )
	{
		if( m_ReferencePoint != point )
		{
			wglMakeCurrent( m_pClientDC->GetSafeHdc(), m_hGLrc );
            if( m_n3DTextureID > 0 )
            {
                m_pMathImpl->Rotate( m_ReferencePoint.y - point.y, m_ReferencePoint.x - point.x, 0 );
            }
            else if( m_n3DTextureID == 0 )
            {
                m_pMathImpl->calculateRotateMatrix( m_ReferencePoint, point );
            }
			wglMakeCurrent( NULL, NULL );
			m_ReferencePoint = point;
			Invalidate( FALSE );
		}
		
		::SetCursor( LoadCursor( AfxGetResourceHandle(),
								 MAKEINTRESOURCE( IDC_EYE_ROTATE_CURSOR ) ) );
	}
	CView::OnMouseMove(nFlags, point);
	
}

//bool COpenGLView::initBufferData()
//{
//	GLfloat vertexData[6][6] = 
//	{
//		{ -0.9f, -0.9f, 1.0f,0.0f,0.0f,1.0f },//first triangle
//		{ 0.85f, -0.9f, 0.0f,1.0f,0.0f,1.0f },
//		{ -0.9f, 0.85f, 0.0f,0.0f,1.0f,1.0f },
//		{ 0.9f, -0.85f, 1.0f,0.0f,0.0f,1.0f },//second triangle
//		{ 0.9f,   0.9f, 0.0f,1.0f,0.0f,1.0f },
//		{ -0.85f, 0.9f, 0.0f,0.0f,1.0f,1.0f }
//	};
//
//	GLFunctionParse::glGenVertexArrays(1, &m_nVertexArrayID);
//	GLFunctionParse::glBindVertexArray(m_nVertexArrayID);
//	GLFunctionParse::glGenBuffers(1, &m_nBuffers);
//	GLFunctionParse::glBindBuffer(GL_ARRAY_BUFFER, m_nVertexArrayID);
//	GLFunctionParse::glBufferData(GL_ARRAY_BUFFER,sizeof( vertexData ), vertexData, GL_STATIC_DRAW);
//
//	m_pGLShaderMgr = std::make_shared<GLShaderMgr>( vertextShader, fragShader );
//	m_pGLShaderMgr->getGLShader()[renderType::demoCube]->begin();
//	GLFunctionParse::glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), NULL);
//	GLFunctionParse::glEnableVertexAttribArrayARB(0);
//	GLFunctionParse::glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),BUFFER_OFFSET(2*sizeof(GLfloat)));
//	GLFunctionParse::glEnableVertexAttribArrayARB(1);
//	GLFunctionParse::glBindVertexArray(0);
//	m_pGLShaderMgr->getGLShader()[renderType::demoCube]->end();
//
//	return true;
//}

std::unique_ptr<CMenu> COpenGLView::createPopUpMenu()
{
    std::unique_ptr<CMenu> pMenu( new CMenu() );

    pMenu->LoadMenu( /*IDR_TRANSVERSE_POPUP*/IDR_OPENGL_3D );

    return pMenu;
}



void COpenGLView::initCubeData()
{
    GLfloat cubeData[24][3] = 
    {
        //front face
        { 0.0f,0.0f,1.0f },
        { 1.0f,0.0f,1.0f },
        { 1.0f,1.0f,1.0f },
        { 0.0f,1.0f,1.0f },

        //left face
        { 0.0f,0.0f,1.0f },
        { 0.0f,1.0f,1.0f },
        { 0.0f,1.0f,0.0f },
        { 0.0f,0.0f,0.0f },

        // top face
        { 0.0f,1.0f,1.0f },
        { 1.0f,1.0f,1.0f },
        { 1.0f,1.0f,0.0f },
        { 0.0f,1.0f,0.0f },

        //right face
        { 1.0f,1.0f,1.0f },
        { 1.0f,0.0f,1.0f },
        { 1.0f,0.0f,0.0f },
        { 1.0f,1.0f,0.0f },

        // bottom face
        { 0.0f,0.0f,1.0f },
        { 0.0f,0.0f,0.0f },
        { 1.0f,0.0f,0.0f },
        { 1.0f,0.0f,1.0f },

        // back face
        { 0.0f,0.0f,0.0f },
        { 0.0f,1.0f,0.0f },
        { 1.0f,1.0f,0.0f },
        { 1.0f,0.0f,0.0f }
    };

    /*for( int i = 0; i < 24; ++i )
    {
        for( int j = 0; j < 3; ++j )
        {
            cubeData[i][j] = cubeData[i][j] - 0.5;
        }
    }*/

    GLFunctionParse::glGenVertexArrays(1,&m_nVertexArrayID);
    GLFunctionParse::glBindVertexArray(m_nVertexArrayID);
    GLFunctionParse::glGenBuffers(1,&m_nBuffers);
    GLFunctionParse::glBindBuffer( GL_ARRAY_BUFFER, m_nBuffers );
    GLFunctionParse::glBufferData(GL_ARRAY_BUFFER,sizeof(cubeData), cubeData, GL_STATIC_DRAW);

    m_pGLShaderMgr = std::make_shared<GLShaderMgr>( vertextShader, fragShader );
	m_pGLShaderMgr->getGLShader()[renderType::demoCube]->begin();
    //GLint mModelViewProjectionMatrix =  GLFunctionParse::glGetUniformLocation( m_pGLShaderMgr->getGLShader()->getprogramID(), std::string("mModelViewProjectionMatrix").c_str() );
    GLFunctionParse::glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(0));
    GLFunctionParse::glEnableVertexAttribArrayARB(0);
    GLFunctionParse::glBindVertexArray(0);
    GLFunctionParse::glBindBuffer( GL_ARRAY_BUFFER, 0 );
    m_pGLShaderMgr->getGLShader()[renderType::demoCube]->end();

    m_pGLShaderMgr->insertGLShader( vertextIndexPlaneShader, fragIndexPlaneShader );

    GLfloat indexPlane[8][3] = 
    {
        {  1.0f, 0.0f, 0.0f },
        { -0.2f,-0.2f, 0.5f },
        {  0.0f, 1.0f, 0.0f },
        {  1.2f,-0.2f, 0.5f },
        {  0.0f, 0.0f, 1.0f },
        {  1.2f, 1.2f, 0.5f },
        {  1.0f, 1.0f, 0.0f },
        { -0.2f, 1.2f, 0.5f }
    };
    GLFunctionParse::glGenVertexArrays(1, &m_nVertexArrayIDIndexPlane);
    GLFunctionParse::glBindVertexArray(m_nVertexArrayIDIndexPlane);
    GLFunctionParse::glGenBuffers(1,&m_nBuffersIndexPlane);
    GLFunctionParse::glBindBuffer(GL_ARRAY_BUFFER, m_nBuffersIndexPlane);
    GLFunctionParse::glBufferData( GL_ARRAY_BUFFER,sizeof(indexPlane), indexPlane, GL_STATIC_DRAW );
    GLFunctionParse::glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(GLfloat)*6,BUFFER_OFFSET(0));
    GLFunctionParse::glEnableVertexAttribArrayARB(1);
    GLFunctionParse::glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, sizeof(GLfloat)*6, BUFFER_OFFSET( 3* sizeof(GLfloat) ));
    GLFunctionParse::glEnableVertexAttribArrayARB(0);
    GLFunctionParse::glBindVertexArray(0);
    GLFunctionParse::glBindBuffer( GL_ARRAY_BUFFER, 0 );

}

