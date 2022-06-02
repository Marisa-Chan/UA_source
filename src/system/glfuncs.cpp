#include "glfuncs.h"
#include <stdexcept>

namespace GFX
{   
PFNGLGENFRAMEBUFFERSPROC Glext::GLGenFramebuffers = NULL;
PFNGLBINDFRAMEBUFFERPROC Glext::GLBindFramebuffer = NULL;
PFNGLGENRENDERBUFFERSPROC Glext::GLGenRenderbuffers = NULL;
PFNGLBINDRENDERBUFFERPROC Glext::GLBindRenderbuffer = NULL;
PFNGLFRAMEBUFFERTEXTUREPROC Glext::GLFrameBufferTexture = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC Glext::GLFrameBufferTexture2D = NULL;
PFNGLRENDERBUFFERSTORAGEPROC Glext::GLRenderbufferStorage = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC Glext::GLFramebufferRenderbuffer = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC Glext::GLCheckFramebufferStatus = NULL;
PFNGLGENERATEMIPMAPPROC Glext::GLGenerateMipmap = NULL;
PFNGLCREATESHADERPROC Glext::GLCreateShader = NULL;
PFNGLSHADERSOURCEPROC Glext::GLShaderSource = NULL;
PFNGLDELETESHADERPROC Glext::GLDeleteShader = NULL;
PFNGLCOMPILESHADERPROC Glext::GLCompileShader = NULL;
PFNGLCREATEPROGRAMPROC Glext::GLCreateProgram = NULL;
PFNGLDELETEPROGRAMPROC Glext::GLDeleteProgram = NULL;
PFNGLATTACHSHADERPROC Glext::GLAttachShader = NULL;
PFNGLLINKPROGRAMPROC Glext::GLLinkProgram = NULL;
PFNGLUSEPROGRAMPROC Glext::GLUseProgram = NULL;
PFNGLGETUNIFORMLOCATIONPROC Glext::GLGetUniformLocation = NULL;
PFNGLUNIFORM1FPROC Glext::GLUniform1f = NULL;
PFNGLUNIFORM1IPROC Glext::GLUniform1i = NULL;
PFNGLUNIFORM2FPROC Glext::GLUniform2f = NULL;
PFNGLUNIFORM2IPROC Glext::GLUniform2i = NULL;
PFNGLUNIFORM3FPROC Glext::GLUniform3f = NULL;
PFNGLUNIFORM3IPROC Glext::GLUniform3i = NULL;
PFNGLUNIFORM4FPROC Glext::GLUniform4f = NULL;
PFNGLUNIFORM4IPROC Glext::GLUniform4i = NULL;

PFNGLUNIFORMMATRIX2FVPROC Glext::GLUniformMatrix2fv = NULL;
PFNGLUNIFORMMATRIX3FVPROC Glext::GLUniformMatrix3fv = NULL;
PFNGLUNIFORMMATRIX4FVPROC Glext::GLUniformMatrix4fv = NULL;

PFNGLGETATTRIBLOCATIONPROC Glext::GLGetAttribLocation = NULL;
PFNGLVERTEXATTRIBPOINTERPROC Glext::GLVertexAttribPointer = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC Glext::GLDisableVertexAttribArray = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC Glext::GLEnableVertexAttribArray = NULL;

PFNGLBINDVERTEXARRAYPROC Glext::GLBindVertexArray = NULL;
PFNGLDELETEVERTEXARRAYSPROC Glext::GLDeleteVertexArrays = NULL;
PFNGLGENVERTEXARRAYSPROC Glext::GLGenVertexArrays = NULL;

PFNGLGETSHADERIVPROC Glext::GLGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC Glext::GLGetShaderInfoLog = NULL;

PFNGLBINDBUFFERPROC Glext::GLBindBuffer = NULL;
PFNGLDELETEBUFFERSPROC Glext::GLDeleteBuffers = NULL;
PFNGLGENBUFFERSPROC Glext::GLGenBuffers = NULL;
PFNGLISBUFFERPROC Glext::GLIsBuffer = NULL;
PFNGLBUFFERDATAPROC Glext::GLBufferData = NULL;
PFNGLBUFFERSUBDATAPROC Glext::GLBufferSubData = NULL;
PFNGLGETBUFFERSUBDATAPROC Glext::GLGetBufferSubData = NULL;
PFNGLMAPBUFFERPROC Glext::GLMapBuffer = NULL;
PFNGLMAPBUFFERRANGEPROC Glext::GLMapBufferRange = NULL;
PFNGLUNMAPBUFFERPROC Glext::GLUnmapBuffer = NULL;
PFNGLGETBUFFERPARAMETERIVPROC Glext::GLGetBufferParameteriv = NULL;
PFNGLGETBUFFERPOINTERVPROC Glext::GLGetBufferPointerv = NULL;

PFNGLGETUNIFORMBLOCKINDEXPROC Glext::GLGetUniformBlockIndex = NULL;
PFNGLUNIFORMBLOCKBINDINGPROC Glext::GLUniformBlockBinding = NULL;
PFNGLBINDBUFFERBASEPROC Glext::GLBindBufferBase = NULL;

template<typename T>
inline void SetGLFunc(T &pFunc, const char *name)
{
    pFunc = (T)SDL_GL_GetProcAddress(name);
    
    if (!pFunc) 
        throw std::runtime_error(std::string("Can't get GL extension address for ") + std::string(name));
}


bool Glext::init()
{
    int major = 0, minor = 0;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
    
    if (major < 2)
        return false;
    
    try {
    SetGLFunc(GLGenFramebuffers, "glGenFramebuffers");
    SetGLFunc(GLBindFramebuffer, "glBindFramebuffer");
    SetGLFunc(GLGenRenderbuffers, "glGenRenderbuffers");
    SetGLFunc(GLBindRenderbuffer, "glBindRenderbuffer");
    SetGLFunc(GLFrameBufferTexture, "glFramebufferTexture");
    SetGLFunc(GLFrameBufferTexture2D, "glFramebufferTexture2D");
    SetGLFunc(GLRenderbufferStorage, "glRenderbufferStorage");
    SetGLFunc(GLFramebufferRenderbuffer, "glFramebufferRenderbuffer");
    SetGLFunc(GLCheckFramebufferStatus, "glCheckFramebufferStatus");
    SetGLFunc(GLGenerateMipmap, "glGenerateMipmap");
    SetGLFunc(GLCreateShader, "glCreateShader");
    SetGLFunc(GLShaderSource, "glShaderSource");
    SetGLFunc(GLCompileShader, "glCompileShader");
    SetGLFunc(GLDeleteShader, "glDeleteShader");
    SetGLFunc(GLCreateProgram, "glCreateProgram");
    SetGLFunc(GLDeleteProgram, "glDeleteProgram");
    SetGLFunc(GLAttachShader, "glAttachShader");
    SetGLFunc(GLLinkProgram, "glLinkProgram");
    SetGLFunc(GLUseProgram, "glUseProgram");
    SetGLFunc(GLGetUniformLocation, "glGetUniformLocation");

    SetGLFunc(GLUniform1f, "glUniform1f");
    SetGLFunc(GLUniform1i, "glUniform1i");
    SetGLFunc(GLUniform2f, "glUniform2f");
    SetGLFunc(GLUniform2i, "glUniform2i");
    SetGLFunc(GLUniform3f, "glUniform3f");
    SetGLFunc(GLUniform3i, "glUniform3i");
    SetGLFunc(GLUniform4f, "glUniform4f");
    SetGLFunc(GLUniform4i, "glUniform4i");
    
    SetGLFunc(GLUniformMatrix2fv, "glUniformMatrix2fv");
    SetGLFunc(GLUniformMatrix3fv, "glUniformMatrix3fv");
    SetGLFunc(GLUniformMatrix4fv, "glUniformMatrix4fv");
    
    SetGLFunc(GLGetAttribLocation, "glGetAttribLocation");
    SetGLFunc(GLVertexAttribPointer, "glVertexAttribPointer");
    SetGLFunc(GLDisableVertexAttribArray, "glDisableVertexAttribArray");
    SetGLFunc(GLEnableVertexAttribArray, "glEnableVertexAttribArray");
    
    SetGLFunc(GLBindVertexArray, "glBindVertexArray");
    SetGLFunc(GLDeleteVertexArrays, "glDeleteVertexArrays");
    SetGLFunc(GLGenVertexArrays, "glGenVertexArrays");

    SetGLFunc(GLGetShaderiv, "glGetShaderiv");
    SetGLFunc(GLGetShaderInfoLog, "glGetShaderInfoLog");
    
    
    SetGLFunc(GLBindBuffer, "glBindBuffer");
    SetGLFunc(GLDeleteBuffers, "glDeleteBuffers");
    SetGLFunc(GLGenBuffers, "glGenBuffers");
    SetGLFunc(GLIsBuffer, "glIsBuffer");
    SetGLFunc(GLBufferData, "glBufferData");
    SetGLFunc(GLBufferSubData, "glBufferSubData");
    SetGLFunc(GLGetBufferSubData, "glGetBufferSubData");
    SetGLFunc(GLMapBuffer, "glMapBuffer");
    SetGLFunc(GLMapBufferRange, "glMapBufferRange");
    SetGLFunc(GLUnmapBuffer, "glUnmapBuffer");
    SetGLFunc(GLGetBufferParameteriv, "glGetBufferParameteriv");
    SetGLFunc(GLGetBufferPointerv, "glGetBufferPointerv");
    
    SetGLFunc(GLGetUniformBlockIndex, "glGetUniformBlockIndex");
    SetGLFunc(GLUniformBlockBinding, "glUniformBlockBinding");
    SetGLFunc(GLBindBufferBase, "glBindBufferBase");
    }
    catch(const std::runtime_error &e)
    {
        return false;
    }
    
    return true;
}


}