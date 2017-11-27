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

    

    m_nFragShader   = GLFunctionParse::glCreateShader( GL_FRAGMENT_SHADER );
    GLFunctionParse::glShaderSource( m_nFragShader, 1, &fragShader, NULL );
    GLFunctionParse::glCompileShader( m_nFragShader );

    m_nProgram = GLFunctionParse::glCreateProgram();
    GLFunctionParse::glAttachShader( m_nProgram, m_nVexterShader );
    GLFunctionParse::glAttachShader(m_nProgram,m_nFragShader);
    GLFunctionParse::glLinkProgram( m_nProgram );

}
