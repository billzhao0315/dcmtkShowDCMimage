#pragma once

#include <gl/GL.h>
class GLShader
{
public:
    GLShader( const char* vertexShader, const char* fragShader );
    ~GLShader();
    void begin();
    void end();
private:
    bool initOpenGLFunction();
    void createShader( const char* vertexShader, const char* fragShader );
    GLuint m_nVexterShader;
    GLuint m_nFragShader;
    GLuint m_nProgram;
};