#pragma once

#include <iostream>
#include <memory>

class GLShader;

class GLShaderMgr
{
public:
    GLShaderMgr( const char* vertexShader, const char* fragShader );
    ~GLShaderMgr();
    std::shared_ptr<GLShader> getGLShader(  );
private:
    std::shared_ptr<GLShader> m_pGLShader;
    GLShaderMgr();

};