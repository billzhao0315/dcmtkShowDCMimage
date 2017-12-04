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

    //GL_VERSION_2_0
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
    static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
    static PFNGLUNIFORM1FPROC glUniform1f;
    static PFNGLUNIFORM2FPROC glUniform2f;
    static PFNGLUNIFORM3FPROC glUniform3f;
    static PFNGLUNIFORM4FPROC glUniform4f;
    static PFNGLUNIFORM1IPROC glUniform1i;
    static PFNGLUNIFORM2IPROC glUniform2i;
    static PFNGLUNIFORM3IPROC glUniform3i;
    static PFNGLUNIFORM4IPROC glUniform4i;
    static PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
    static PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
    static PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

    //GL_VERSION_3_0
    static PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
    static PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
    static PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
    static PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
    static PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
    static PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;

    //
    static PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArrayARB;

    ////GL_EXT_texture_object
    //static PFNGLGENTEXTURESEXTPROC glGenTexturesEXT;
    //static PFNGLBINDTEXTUREEXTPROC glBindTextureEXT;
    //static PFNGLDELETETEXTURESEXTPROC glDeleteTexturesEXT;

private:
    static bool variant();
};