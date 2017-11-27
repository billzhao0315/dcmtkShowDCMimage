#pragma once

#include <iostream>
#include <memory>

class GLShader;

class GLShaderMgr
{
public:
    GLShaderMgr();
    ~GLShaderMgr();
    std::shared_ptr<GLShader> getGLShader( const char* vertexShader, const char* fragShader );

};