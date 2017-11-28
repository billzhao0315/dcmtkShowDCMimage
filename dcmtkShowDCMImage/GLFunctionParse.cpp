#include "stdafx.h"
#include "GLFunctionParse.h"

PFNGLGENBUFFERSPROC GLFunctionParse::glGenBuffers = NULL;
PFNGLBINDBUFFERPROC GLFunctionParse::glBindBuffer = NULL;
PFNGLBUFFERDATAPROC GLFunctionParse::glBufferData = NULL;

PFNGLCREATESHADERPROC  GLFunctionParse::glCreateShader  = NULL;
PFNGLSHADERSOURCEPROC  GLFunctionParse::glShaderSource  = NULL;
PFNGLCOMPILESHADERPROC GLFunctionParse::glCompileShader = NULL;
PFNGLATTACHSHADERPROC  GLFunctionParse::glAttachShader  = NULL;
PFNGLCREATEPROGRAMPROC GLFunctionParse::glCreateProgram = NULL;
PFNGLLINKPROGRAMPROC   GLFunctionParse::glLinkProgram   = NULL;
PFNGLUSEPROGRAMPROC    GLFunctionParse::glUseProgram    = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC GLFunctionParse::glGetObjectParameterivARB = NULL;
PFNGLGETSHADERIVPROC GLFunctionParse::glGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC GLFunctionParse::glGetShaderInfoLog = NULL;
PFNGLGETPROGRAMIVPROC GLFunctionParse::glGetProgramiv = NULL;
PFNGLGETPROGRAMINFOLOGPROC GLFunctionParse::glGetProgramInfoLog = NULL;
PFNGLVERTEXATTRIBPOINTERPROC GLFunctionParse::glVertexAttribPointer = NULL;

PFNGLGENVERTEXARRAYSPROC    GLFunctionParse::glGenVertexArrays = NULL;
PFNGLBINDVERTEXARRAYPROC GLFunctionParse::glBindVertexArray = NULL;

PFNGLENABLEVERTEXATTRIBARRAYARBPROC GLFunctionParse::glEnableVertexAttribArrayARB = NULL;
PFNGLDRAWARRAYSEXTPROC GLFunctionParse::glDrawArraysEXT = NULL;



GLFunctionParse::GLFunctionParse()
{

}

GLFunctionParse::~GLFunctionParse()
{
    
}

bool GLFunctionParse::initGLFunction()
{
    if( variant() == false )
    {
        glGenBuffers = reinterpret_cast<PFNGLGENBUFFERSPROC>( wglGetProcAddress("glGenBuffers") );
        glBindBuffer = reinterpret_cast<PFNGLBINDBUFFERPROC>( wglGetProcAddress("glBindBuffer") );
        glBufferData = reinterpret_cast<PFNGLBUFFERDATAPROC>( wglGetProcAddress("glBufferData") );
        glCreateShader = reinterpret_cast<PFNGLCREATESHADERPROC>( wglGetProcAddress("glCreateShader") );
        glShaderSource = reinterpret_cast<PFNGLSHADERSOURCEPROC>( wglGetProcAddress("glShaderSource") );
        glCompileShader = reinterpret_cast<PFNGLCOMPILESHADERPROC>( wglGetProcAddress("glCompileShader") );
        glAttachShader = reinterpret_cast<PFNGLATTACHSHADERPROC>(wglGetProcAddress("glAttachShader"));
        glCreateProgram = reinterpret_cast<PFNGLCREATEPROGRAMPROC>( wglGetProcAddress("glCreateProgram") );
        glLinkProgram = reinterpret_cast<PFNGLLINKPROGRAMPROC>(wglGetProcAddress("glLinkProgram"));
        glUseProgram = reinterpret_cast<PFNGLUSEPROGRAMPROC>(wglGetProcAddress("glUseProgram"));
        glGetObjectParameterivARB = reinterpret_cast<PFNGLGETOBJECTPARAMETERIVARBPROC>( wglGetProcAddress("glGetObjectParameterivARB") );
        glGetShaderiv = reinterpret_cast<PFNGLGETSHADERIVPROC>( wglGetProcAddress("glGetShaderiv") );
        glGetShaderInfoLog = reinterpret_cast<PFNGLGETSHADERINFOLOGPROC>( wglGetProcAddress("glGetShaderInfoLog") );
        glGetProgramiv = reinterpret_cast<PFNGLGETPROGRAMIVPROC>( wglGetProcAddress("glGetProgramiv") );
        glGetProgramInfoLog = reinterpret_cast<PFNGLGETPROGRAMINFOLOGPROC>( wglGetProcAddress("glGetProgramInfoLog") );
        glVertexAttribPointer = reinterpret_cast<PFNGLVERTEXATTRIBPOINTERPROC>( wglGetProcAddress("glVertexAttribPointer") );
        glGenVertexArrays = reinterpret_cast<PFNGLGENVERTEXARRAYSPROC>( wglGetProcAddress("glGenVertexArrays") );
        glBindVertexArray = reinterpret_cast<PFNGLBINDVERTEXARRAYPROC>( wglGetProcAddress("glBindVertexArray") );
        glEnableVertexAttribArrayARB = reinterpret_cast<PFNGLENABLEVERTEXATTRIBARRAYARBPROC>( wglGetProcAddress("glEnableVertexAttribArrayARB") );
        glDrawArraysEXT = reinterpret_cast<PFNGLDRAWARRAYSEXTPROC>( wglGetProcAddress("glDrawArraysEXT") );
        return variant();
    }

    return true;
    
}

bool GLFunctionParse::variant()
{
    if( glCreateShader == NULL || glShaderSource == NULL || glCompileShader == NULL || glAttachShader == NULL || glCreateProgram == NULL || glLinkProgram == NULL || glUseProgram == NULL || glGetObjectParameterivARB == NULL ||
        glGetShaderiv == NULL || glGetShaderInfoLog == NULL || glGetProgramiv == NULL || glGetProgramInfoLog == NULL || glGenVertexArrays == NULL || glBindVertexArray == NULL||
        glGenBuffers == NULL || glBindBuffer == NULL || glBufferData == NULL || glVertexAttribPointer == NULL || glEnableVertexAttribArrayARB == NULL || glDrawArraysEXT == NULL)
    {
        return false;
    }

    return true;
}


