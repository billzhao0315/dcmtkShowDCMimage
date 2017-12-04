#include "stdafx.h"

#include "GLShaderMgr.h"

#include "GLShader.h"

GLShaderMgr::GLShaderMgr( const char* vertexShader, const char* fragShader )
{
    m_pGLShader.push_back( std::make_shared<GLShader>( vertexShader, fragShader ) );
}

GLShaderMgr::~GLShaderMgr()
{
    
}

std::vector<std::shared_ptr<GLShader>> GLShaderMgr::getGLShader(  )
{
    return m_pGLShader;
}

bool GLShaderMgr::insertGLShader( const char* vertexShader, const char* fragShader )
{
    m_pGLShader.push_back( std::make_shared<GLShader>( vertexShader, fragShader ) );
    return true;
}
