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
PFNGLGETUNIFORMLOCATIONPROC GLFunctionParse::glGetUniformLocation = NULL;
PFNGLUNIFORM1FPROC GLFunctionParse::glUniform1f = NULL;
PFNGLUNIFORM2FPROC GLFunctionParse::glUniform2f = NULL;
PFNGLUNIFORM3FPROC GLFunctionParse::glUniform3f = NULL;
PFNGLUNIFORM4FPROC GLFunctionParse::glUniform4f = NULL;
PFNGLUNIFORM1IPROC GLFunctionParse::glUniform1i = NULL;
PFNGLUNIFORM2IPROC GLFunctionParse::glUniform2i = NULL;
PFNGLUNIFORM3IPROC GLFunctionParse::glUniform3i = NULL;
PFNGLUNIFORM4IPROC GLFunctionParse::glUniform4i = NULL;
PFNGLUNIFORMMATRIX2FVPROC GLFunctionParse::glUniformMatrix2fv = NULL;
PFNGLUNIFORMMATRIX3FVPROC GLFunctionParse::glUniformMatrix3fv = NULL;
PFNGLUNIFORMMATRIX4FVPROC GLFunctionParse::glUniformMatrix4fv = NULL;

PFNGLGENVERTEXARRAYSPROC    GLFunctionParse::glGenVertexArrays = NULL;
PFNGLBINDVERTEXARRAYPROC GLFunctionParse::glBindVertexArray = NULL;
PFNGLGENFRAMEBUFFERSPROC GLFunctionParse::glGenFramebuffers = NULL;
PFNGLBINDFRAMEBUFFERPROC GLFunctionParse::glBindFramebuffer = NULL;
PFNGLDELETEFRAMEBUFFERSPROC GLFunctionParse::glDeleteFramebuffers = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC GLFunctionParse::glFramebufferTexture2D = NULL;

PFNGLENABLEVERTEXATTRIBARRAYARBPROC GLFunctionParse::glEnableVertexAttribArrayARB = NULL;

PFNGLTEXIMAGE3DPROC GLFunctionParse::glTexImage3D = NULL;


//PFNGLGENTEXTURESEXTPROC GLFunctionParse::glGenTexturesEXT = NULL;
//PFNGLBINDTEXTUREEXTPROC GLFunctionParse::glBindTextureEXT = NULL;
//PFNGLDELETETEXTURESEXTPROC GLFunctionParse::glDeleteTexturesEXT = NULL;


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
        glGetUniformLocation = reinterpret_cast<PFNGLGETUNIFORMLOCATIONPROC>( wglGetProcAddress("glGetUniformLocation") );
        glUniform1f = reinterpret_cast<PFNGLUNIFORM1FPROC>( wglGetProcAddress( "glUniform1f" ) );
        glUniform2f = reinterpret_cast<PFNGLUNIFORM2FPROC>( wglGetProcAddress( "glUniform2f" ) );
        glUniform3f = reinterpret_cast<PFNGLUNIFORM3FPROC>( wglGetProcAddress( "glUniform3f" ) );
        glUniform4f = reinterpret_cast<PFNGLUNIFORM4FPROC>( wglGetProcAddress( "glUniform4f" ) );
        glUniform1i = reinterpret_cast<PFNGLUNIFORM1IPROC>( wglGetProcAddress( "glUniform1i" ) );
        glUniform2i = reinterpret_cast<PFNGLUNIFORM2IPROC>( wglGetProcAddress( "glUniform2i" ) );
        glUniform3i = reinterpret_cast<PFNGLUNIFORM3IPROC>( wglGetProcAddress( "glUniform3i" ) );
        glUniform4i = reinterpret_cast<PFNGLUNIFORM4IPROC>( wglGetProcAddress( "glUniform4i" ) );
        glUniformMatrix2fv = reinterpret_cast<PFNGLUNIFORMMATRIX2FVPROC>( wglGetProcAddress( "glUniformMatrix2fv" ) );
        glUniformMatrix3fv = reinterpret_cast<PFNGLUNIFORMMATRIX3FVPROC>( wglGetProcAddress( "glUniformMatrix3fv" ) );
        glUniformMatrix4fv = reinterpret_cast<PFNGLUNIFORMMATRIX4FVPROC>( wglGetProcAddress( "glUniformMatrix4fv" ) );



        glVertexAttribPointer = reinterpret_cast<PFNGLVERTEXATTRIBPOINTERPROC>( wglGetProcAddress("glVertexAttribPointer") );
        glGenVertexArrays = reinterpret_cast<PFNGLGENVERTEXARRAYSPROC>( wglGetProcAddress("glGenVertexArrays") );
        glBindVertexArray = reinterpret_cast<PFNGLBINDVERTEXARRAYPROC>( wglGetProcAddress("glBindVertexArray") );
        glEnableVertexAttribArrayARB = reinterpret_cast<PFNGLENABLEVERTEXATTRIBARRAYARBPROC>( wglGetProcAddress("glEnableVertexAttribArrayARB") );
        glGenFramebuffers = reinterpret_cast<PFNGLGENFRAMEBUFFERSPROC>( wglGetProcAddress("glGenFramebuffers") );
        glBindFramebuffer = reinterpret_cast<PFNGLBINDFRAMEBUFFERPROC>( wglGetProcAddress("glBindFramebuffer") );
        glDeleteFramebuffers = reinterpret_cast<PFNGLDELETEFRAMEBUFFERSPROC>( wglGetProcAddress("glDeleteFramebuffers") );
        glFramebufferTexture2D = reinterpret_cast<PFNGLFRAMEBUFFERTEXTURE2DPROC>( wglGetProcAddress("glFramebufferTexture2D") );

        /*glGenTexturesEXT = reinterpret_cast<PFNGLGENTEXTURESEXTPROC>( wglGetProcAddress("glGenTexturesEXT") );
        glBindTextureEXT = reinterpret_cast<PFNGLBINDTEXTUREEXTPROC>( wglGetProcAddress("glBindTextureEXT") );
        glDeleteTexturesEXT = reinterpret_cast<PFNGLDELETETEXTURESEXTPROC>( wglGetProcAddress("glDeleteTexturesEXT") );*/

        glTexImage3D = reinterpret_cast<PFNGLTEXIMAGE3DPROC>(wglGetProcAddress("glTexImage3D"));

        return variant();
    }

    return true;
    
}

bool GLFunctionParse::variant()
{
    if( glCreateShader == NULL || glShaderSource == NULL || glCompileShader == NULL || glAttachShader == NULL || glCreateProgram == NULL || glLinkProgram == NULL || glUseProgram == NULL || glGetObjectParameterivARB == NULL ||
        glGetShaderiv == NULL || glGetShaderInfoLog == NULL || glGetProgramiv == NULL || glGetProgramInfoLog == NULL || glGenVertexArrays == NULL || glBindVertexArray == NULL||
        glGenBuffers == NULL || glBindBuffer == NULL || glBufferData == NULL || glVertexAttribPointer == NULL || glEnableVertexAttribArrayARB == NULL || glGetUniformLocation == NULL || glUniform1f == NULL ||
        glUniform2f == NULL || glUniform3f == NULL || glUniform4f == NULL || glUniform1i == NULL || glUniform2i == NULL || glUniform3i == NULL || glUniformMatrix2fv == NULL || glUniformMatrix3fv == NULL || glUniformMatrix4fv == NULL
        /*|| glGenTexturesEXT == NULL || glBindTextureEXT == NULL || glDeleteTexturesEXT == NULL*/
        || glGenFramebuffers == NULL || glBindFramebuffer == NULL || glDeleteFramebuffers == NULL || glFramebufferTexture2D == NULL || glTexImage3D == NULL
        )
    {
        return false;
    }

    return true;
}


