#include "stdafx.h"
#include "GLFunctionParse.h"


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
        glCreateShader = reinterpret_cast<PFNGLCREATESHADERPROC>( wglGetProcAddress("glCreateShader") );
        glShaderSource = reinterpret_cast<PFNGLSHADERSOURCEPROC>( wglGetProcAddress("glShaderSource") );
        glCompileShader = reinterpret_cast<PFNGLCOMPILESHADERPROC>( wglGetProcAddress("glCompileShader") );
        glAttachShader = reinterpret_cast<PFNGLATTACHSHADERPROC>(wglGetProcAddress("glAttachShader"));
        glCreateProgram = reinterpret_cast<PFNGLCREATEPROGRAMPROC>( wglGetProcAddress("glCreateProgram") );
        glLinkProgram = reinterpret_cast<PFNGLLINKPROGRAMPROC>(wglGetProcAddress("glLinkProgram"));
        glUseProgram = reinterpret_cast<PFNGLUSEPROGRAMPROC>(wglGetProcAddress("glUseProgram"));
        glGetObjectParameterivARB = reinterpret_cast<PFNGLGETOBJECTPARAMETERIVARBPROC>( wglGetProcAddress("glGetObjectParameterivARB") );
        glGetShaderiv = reinterpret_cast<PFNGLGETSHADERIVPROC>( wglGetProcAddress("glGetShaderiv") );
        glGetShaderInfoLog = reinterpret_cast<PFNGLGETSHADERINFOLOGPROC>( wglGetProcAddress("PFNGLGETSHADERINFOLOGPROC") );
        glGetProgramiv = reinterpret_cast<PFNGLGETPROGRAMIVPROC>( wglGetProcAddress("glGetProgramiv") );



        return variant();
    }

    return true;
    
}

bool GLFunctionParse::variant()
{
    if( glCreateShader == NULL || glShaderSource == NULL || glCompileShader == NULL || glAttachShader == NULL || glCreateProgram == NULL || glLinkProgram == NULL || glUseProgram == NULL || glGetObjectParameterivARB == NULL ||
        glGetShaderiv == NULL || glGetProgramiv == NULL)
    {
        return false;
    }

    return true;
}


