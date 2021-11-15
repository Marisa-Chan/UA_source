#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/glext.h>
#else
#include <GL/glext.h>
#endif

namespace GFX
{

class Glext
{
public:
    static bool init();
    
    
public:
    static PFNGLGENFRAMEBUFFERSPROC GLGenFramebuffers;
    static PFNGLBINDFRAMEBUFFERPROC GLBindFramebuffer;
    static PFNGLGENRENDERBUFFERSPROC GLGenRenderbuffers;
    static PFNGLBINDRENDERBUFFERPROC GLBindRenderbuffer;
    static PFNGLFRAMEBUFFERTEXTUREPROC GLFrameBufferTexture;
    static PFNGLFRAMEBUFFERTEXTURE2DPROC GLFrameBufferTexture2D;
    static PFNGLRENDERBUFFERSTORAGEPROC GLRenderbufferStorage;
    static PFNGLFRAMEBUFFERRENDERBUFFERPROC GLFramebufferRenderbuffer;
    static PFNGLCHECKFRAMEBUFFERSTATUSPROC GLCheckFramebufferStatus;
    static PFNGLGENERATEMIPMAPPROC GLGenerateMipmap;
    static PFNGLCREATESHADERPROC GLCreateShader;
    static PFNGLSHADERSOURCEPROC GLShaderSource;
    static PFNGLDELETESHADERPROC GLDeleteShader;
    static PFNGLCOMPILESHADERPROC GLCompileShader;
    static PFNGLCREATEPROGRAMPROC GLCreateProgram;
    static PFNGLDELETEPROGRAMPROC GLDeleteProgram;
    static PFNGLATTACHSHADERPROC GLAttachShader;
    static PFNGLLINKPROGRAMPROC GLLinkProgram;
    static PFNGLUSEPROGRAMPROC GLUseProgram;
    static PFNGLGETUNIFORMLOCATIONPROC GLGetUniformLocation;
    static PFNGLUNIFORM1FPROC GLUniform1f;
    static PFNGLUNIFORM1IPROC GLUniform1i;
    static PFNGLUNIFORM2FPROC GLUniform2f;
    static PFNGLUNIFORM2IPROC GLUniform2i;
    static PFNGLUNIFORM3FPROC GLUniform3f;
    static PFNGLUNIFORM3IPROC GLUniform3i;
    static PFNGLUNIFORM4FPROC GLUniform4f;
    static PFNGLUNIFORM4IPROC GLUniform4i;
    
    static PFNGLUNIFORMMATRIX2FVPROC GLUniformMatrix2fv;
    static PFNGLUNIFORMMATRIX3FVPROC GLUniformMatrix3fv;
    static PFNGLUNIFORMMATRIX4FVPROC GLUniformMatrix4fv;
    
    static PFNGLGETATTRIBLOCATIONPROC GLGetAttribLocation;
    static PFNGLVERTEXATTRIBPOINTERPROC GLVertexAttribPointer;
    static PFNGLDISABLEVERTEXATTRIBARRAYPROC GLDisableVertexAttribArray;
    static PFNGLENABLEVERTEXATTRIBARRAYPROC GLEnableVertexAttribArray;
    
    static PFNGLBINDVERTEXARRAYPROC GLBindVertexArray;
    static PFNGLDELETEVERTEXARRAYSPROC GLDeleteVertexArrays;
    static PFNGLGENVERTEXARRAYSPROC GLGenVertexArrays;

    static PFNGLGETSHADERIVPROC GLGetShaderiv;
    static PFNGLGETSHADERINFOLOGPROC GLGetShaderInfoLog;
    
    static PFNGLBINDBUFFERPROC GLBindBuffer;
    static PFNGLDELETEBUFFERSPROC GLDeleteBuffers;
    static PFNGLGENBUFFERSPROC GLGenBuffers;
    static PFNGLISBUFFERPROC GLIsBuffer;
    static PFNGLBUFFERDATAPROC GLBufferData;
    static PFNGLBUFFERSUBDATAPROC GLBufferSubData;
    static PFNGLGETBUFFERSUBDATAPROC GLGetBufferSubData;
    static PFNGLMAPBUFFERPROC GLMapBuffer;
    static PFNGLMAPBUFFERRANGEPROC GLMapBufferRange;
    static PFNGLUNMAPBUFFERPROC GLUnmapBuffer;
    static PFNGLGETBUFFERPARAMETERIVPROC GLGetBufferParameteriv;
    static PFNGLGETBUFFERPOINTERVPROC GLGetBufferPointerv;
    
    static PFNGLGETUNIFORMBLOCKINDEXPROC GLGetUniformBlockIndex;
    static PFNGLUNIFORMBLOCKBINDINGPROC GLUniformBlockBinding;
    static PFNGLBINDBUFFERBASEPROC GLBindBufferBase;
};
    
}