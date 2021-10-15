#include "glfuncs.h"

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
PFNGLUNMAPBUFFERPROC Glext::GLUnmapBuffer = NULL;
PFNGLGETBUFFERPARAMETERIVPROC Glext::GLGetBufferParameteriv = NULL;
PFNGLGETBUFFERPOINTERVPROC Glext::GLGetBufferPointerv = NULL;

bool Glext::init()
{
    GLGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)SDL_GL_GetProcAddress("glGenFramebuffers");
    GLBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)SDL_GL_GetProcAddress("glBindFramebuffer");
    GLGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)SDL_GL_GetProcAddress("glGenRenderbuffers");
    GLBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)SDL_GL_GetProcAddress("glBindRenderbuffer");
    GLFrameBufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)SDL_GL_GetProcAddress("glFramebufferTexture");
    GLFrameBufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)SDL_GL_GetProcAddress("glFramebufferTexture2D");
    GLRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)SDL_GL_GetProcAddress("glRenderbufferStorage");
    GLFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)SDL_GL_GetProcAddress("glFramebufferRenderbuffer");
    GLCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)SDL_GL_GetProcAddress("glCheckFramebufferStatus");
    GLGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)SDL_GL_GetProcAddress("glGenerateMipmap");
    GLCreateShader = (PFNGLCREATESHADERPROC)SDL_GL_GetProcAddress("glCreateShader");
    GLShaderSource = (PFNGLSHADERSOURCEPROC)SDL_GL_GetProcAddress("glShaderSource");
    GLCompileShader = (PFNGLCOMPILESHADERPROC)SDL_GL_GetProcAddress("glCompileShader");
    GLDeleteShader = (PFNGLDELETESHADERPROC)SDL_GL_GetProcAddress("glDeleteShader");
    GLCreateProgram = (PFNGLCREATEPROGRAMPROC)SDL_GL_GetProcAddress("glCreateProgram");
    GLDeleteProgram = (PFNGLDELETEPROGRAMPROC)SDL_GL_GetProcAddress("glDeleteProgram");
    GLAttachShader = (PFNGLATTACHSHADERPROC)SDL_GL_GetProcAddress("glAttachShader");
    GLLinkProgram = (PFNGLLINKPROGRAMPROC)SDL_GL_GetProcAddress("glLinkProgram");
    GLUseProgram = (PFNGLUSEPROGRAMPROC)SDL_GL_GetProcAddress("glUseProgram");
    GLGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)SDL_GL_GetProcAddress("glGetUniformLocation");

    GLUniform1f = (PFNGLUNIFORM1FPROC)SDL_GL_GetProcAddress("glUniform1f");
    GLUniform1i = (PFNGLUNIFORM1IPROC)SDL_GL_GetProcAddress("glUniform1i");
    GLUniform2f = (PFNGLUNIFORM2FPROC)SDL_GL_GetProcAddress("glUniform2f");
    GLUniform2i = (PFNGLUNIFORM2IPROC)SDL_GL_GetProcAddress("glUniform2i");
    GLUniform3f = (PFNGLUNIFORM3FPROC)SDL_GL_GetProcAddress("glUniform3f");
    GLUniform3i = (PFNGLUNIFORM3IPROC)SDL_GL_GetProcAddress("glUniform3i");
    GLUniform4f = (PFNGLUNIFORM4FPROC)SDL_GL_GetProcAddress("glUniform4f");
    GLUniform4i = (PFNGLUNIFORM4IPROC)SDL_GL_GetProcAddress("glUniform4i");
    
    GLGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)SDL_GL_GetProcAddress("glGetAttribLocation");
    GLVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)SDL_GL_GetProcAddress("glVertexAttribPointer");
    GLDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)SDL_GL_GetProcAddress("glDisableVertexAttribArray");
    GLEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)SDL_GL_GetProcAddress("glEnableVertexAttribArray");
    
    GLBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)SDL_GL_GetProcAddress("glBindVertexArray");
    GLDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)SDL_GL_GetProcAddress("glDeleteVertexArrays");
    GLGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)SDL_GL_GetProcAddress("glGenVertexArrays");

    GLGetShaderiv = (PFNGLGETSHADERIVPROC)SDL_GL_GetProcAddress("glGetShaderiv");
    GLGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)SDL_GL_GetProcAddress("glGetShaderInfoLog");
    
    
    GLBindBuffer = (PFNGLBINDBUFFERPROC)SDL_GL_GetProcAddress("glBindBuffer");
    GLDeleteBuffers = (PFNGLDELETEBUFFERSPROC )SDL_GL_GetProcAddress("glDeleteBuffers");
    GLGenBuffers = (PFNGLGENBUFFERSPROC )SDL_GL_GetProcAddress("glGenBuffers");
    GLIsBuffer = (PFNGLISBUFFERPROC )SDL_GL_GetProcAddress("glIsBuffer");
    GLBufferData = (PFNGLBUFFERDATAPROC )SDL_GL_GetProcAddress("glBufferData");
    GLBufferSubData = (PFNGLBUFFERSUBDATAPROC )SDL_GL_GetProcAddress("glBufferSubData");
    GLGetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC )SDL_GL_GetProcAddress("glGetBufferSubData");
    GLMapBuffer = (PFNGLMAPBUFFERPROC )SDL_GL_GetProcAddress("glMapBuffer");
    GLUnmapBuffer = (PFNGLUNMAPBUFFERPROC )SDL_GL_GetProcAddress("glUnmapBuffer");
    GLGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC )SDL_GL_GetProcAddress("glGetBufferParameteriv");
    GLGetBufferPointerv = (PFNGLGETBUFFERPOINTERVPROC )SDL_GL_GetProcAddress("glGetBufferPointerv");
    
    return true;
}


}