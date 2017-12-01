#include "stdafx.h"

#include "GLShader.h"

#include "GLFunctionParse.h"

#include "glext.h"

#pragma comment( lib, "OpenGL32.lib" )
#pragma comment( lib, "glu32.lib" )

GLShader::GLShader( const char* vertexShader, const char* fragShader )
{
    createShader( vertexShader, fragShader );
}

GLShader::~GLShader()
{
    
}

bool GLShader::initOpenGLFunction()
{
    return true;

}

void GLShader::createShader( const char* vertexShader, const char* fragShader )
{
    m_nVexterShader = GLFunctionParse::glCreateShader( GL_VERTEX_SHADER );
    GLFunctionParse::glShaderSource( m_nVexterShader,1, &vertexShader, NULL );
    GLFunctionParse::glCompileShader(m_nVexterShader);

    GLint nComplieStatus;
    GLFunctionParse::glGetObjectParameterivARB(m_nVexterShader,GL_COMPILE_STATUS, &nComplieStatus );
    if( nComplieStatus )
    {
        GLint nLogLength = 0;
        
        GLFunctionParse::glGetShaderiv(m_nVexterShader, GL_INFO_LOG_LENGTH, &nLogLength );
        if( nLogLength > 1 )
        {
            GLchar* logInfo;
            logInfo = new GLchar[ nLogLength +1 ];
            GLFunctionParse::glGetShaderInfoLog(m_nVexterShader,nLogLength,&nLogLength,logInfo);
            logInfo[nLogLength] = '\0';
            MessageBoxA(NULL, CString(logInfo),_T("error"), MB_OK);
            delete[] logInfo;
            return;
        }
    }

    m_nFragShader   = GLFunctionParse::glCreateShader( GL_FRAGMENT_SHADER );
    GLFunctionParse::glShaderSource( m_nFragShader, 1, &fragShader, NULL );
    GLFunctionParse::glCompileShader( m_nFragShader );


    GLFunctionParse::glGetObjectParameterivARB(m_nFragShader, GL_COMPILE_STATUS, &nComplieStatus);
    if( nComplieStatus )
    {
        GLint nLogLength = 0;
        
        GLFunctionParse::glGetShaderiv(m_nFragShader, GL_INFO_LOG_LENGTH, &nLogLength );
        if( nLogLength > 1 )
        {
            GLchar* logInfo;
            logInfo = new GLchar[ nLogLength +1 ];
            GLFunctionParse::glGetShaderInfoLog(m_nFragShader,nLogLength,&nLogLength,logInfo);
            logInfo[nLogLength] = '\0';
            MessageBoxA(NULL, CString(logInfo),_T("error"), MB_OK);
            delete[] logInfo;
            return;
        }
    }

    m_nProgram = GLFunctionParse::glCreateProgram();
    GLFunctionParse::glAttachShader( m_nProgram, m_nVexterShader );
    GLFunctionParse::glAttachShader(m_nProgram,m_nFragShader);
    GLFunctionParse::glLinkProgram( m_nProgram );

    GLint nLinkStatus;
    GLFunctionParse::glGetObjectParameterivARB(m_nProgram, GL_LINK_STATUS, &nLinkStatus);
    if( nLinkStatus )
    {
        GLint nLogLength = 0;
        GLFunctionParse::glGetProgramiv(m_nProgram, GL_INFO_LOG_LENGTH, &nLogLength);
        if( nLogLength > 1 )
        {
            GLchar* logInfo = new GLchar[nLogLength + 1];
            GLFunctionParse::glGetProgramInfoLog( m_nProgram,nLogLength, &nLogLength, logInfo );
            logInfo[nLogLength] = '\0';
            MessageBoxA(NULL, CString(logInfo),_T("error"), MB_OK);
            delete[] logInfo;
        }
    }

}

void GLShader::begin()
{
    GLFunctionParse::glUseProgram(m_nProgram);
}

void GLShader::end()
{
    GLFunctionParse::glUseProgram(0);
}

GLuint GLShader::getprogramID()
{
    return m_nProgram;
}

