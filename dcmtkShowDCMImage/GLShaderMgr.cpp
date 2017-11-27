#include "stdafx.h"

#include "GLShaderMgr.h"

#include "GLShader.h"

GLShaderMgr::GLShaderMgr()
{
    
}

GLShaderMgr::~GLShaderMgr()
{
    
}

std::shared_ptr<GLShader> GLShaderMgr::getGLShader( const char* vertexShader, const char* fragShader )
{
    return std::make_shared<GLShader>( vertexShader, fragShader );
}
