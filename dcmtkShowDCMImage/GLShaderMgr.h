#pragma once

#include <iostream>
#include <memory>
#include <vector>

class GLShader;

class GLShaderMgr
{
public:
    GLShaderMgr( const char* vertexShader, const char* fragShader );
    ~GLShaderMgr();
    std::vector<std::shared_ptr<GLShader>> getGLShader(  );
    bool insertGLShader( const char* vertexShader, const char* fragShader );
private:
    std::vector<std::shared_ptr<GLShader>> m_pGLShader;
    GLShaderMgr();

};