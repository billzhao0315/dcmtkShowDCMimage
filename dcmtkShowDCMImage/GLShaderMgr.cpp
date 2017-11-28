#include "stdafx.h"

#include "GLShaderMgr.h"

#include "GLShader.h"

GLShaderMgr::GLShaderMgr( const char* vertexShader, const char* fragShader )
{
    m_pGLShader = std::make_shared<GLShader>( vertexShader, fragShader );
}

GLShaderMgr::~GLShaderMgr()
{
    
}

std::shared_ptr<GLShader> GLShaderMgr::getGLShader(  )
{
    return m_pGLShader;
}
