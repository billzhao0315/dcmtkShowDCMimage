#pragma once
#include <gl/GL.h>
#include "glext.h"
class GLFunctionParse
{
public:
    GLFunctionParse();
    ~GLFunctionParse();
    static bool initGLFunction();
    static PFNGLCREATESHADERPROC glCreateShader;
    static PFNGLSHADERSOURCEPROC glShaderSource;
    static PFNGLCOMPILESHADERPROC glCompileShader;
    static PFNGLATTACHSHADERPROC glAttachShader;
    static PFNGLCREATEPROGRAMPROC glCreateProgram;
    static PFNGLLINKPROGRAMPROC glLinkProgram;
    static PFNGLUSEPROGRAMPROC glUseProgram;
    static PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
    static PFNGLGETSHADERIVPROC glGetShaderiv;
    static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
    static PFNGLGETPROGRAMIVPROC glGetProgramiv;
    static PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;

private:
    static bool variant();
};