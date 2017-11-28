#pragma once
#include <gl/GL.h>
#include "glext.h"
class GLFunctionParse
{
public:
    GLFunctionParse();
    ~GLFunctionParse();
    static bool initGLFunction();

    //GL_VERSION_1_5
    static PFNGLGENBUFFERSPROC glGenBuffers;
    static PFNGLBINDBUFFERPROC glBindBuffer;
    static PFNGLBUFFERDATAPROC glBufferData;


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
    static PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
    //GL_VERSION_3_0
    static PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
    static PFNGLBINDVERTEXARRAYPROC glBindVertexArray;

    //
    static PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArrayARB;
    static PFNGLDRAWARRAYSEXTPROC glDrawArraysEXT;
private:
    static bool variant();
};