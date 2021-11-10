
#include "gfx.h"

namespace GFX
{

const std::string GFXEngine:: _stdPShaderText =
"\
#version 140\n\
layout(std140) uniform Parameters\
{\
    mat4 MProj;\
    mat4 MView;\
    vec3 Fog;\
    vec3 AlphaFog;\
    bool Textured;\
    bool Flat;\
    bool ATest;\
};\
uniform sampler2D texture;\
in vec4 smoothColor;\
flat in vec4 flatColor;\
void main()\
{\
    vec4 clr;\
    if (Flat) clr = flatColor; else clr = smoothColor;\
    if (Textured)\
        gl_FragColor = texture2D(texture, gl_TexCoord[0].xy) * clr;\
    else\
        gl_FragColor = clr;\
    if (ATest && gl_FragColor.w <= 0.0)\
        discard;\
}";


const std::string GFXEngine:: _stdVShaderText =
"\
#version 140\n\
layout(std140) uniform Parameters\
{\
    mat4 MProj;\
    mat4 MView;\
    vec3 Fog;\
    vec3 AlphaFog;\
    bool Textured;\
    bool Flat;\
    bool ATest;\
};\
attribute vec3 vPos;\
attribute vec4 vColor;\
attribute vec2 vUV;\
\
out vec4 smoothColor;\
flat out vec4 flatColor;\
void main()\
{\
    vec4 tformed = MView * vec4(vPos, 1.0);\
    gl_Position = MProj * tformed;\
\
    vec4 clr;\
    if (Fog.x != 0.0)\
    {\
        float fg = clamp((tformed.z - Fog.y) / Fog.z, 0.0, 1.0);\
        clr = vec4(mix(vColor.xyz, vec3(0.0), fg), vColor.w);\
    }\
    else\
    {\
        clr = vColor;\
    }\
    \
    if (AlphaFog.x != 0.0)\
    {\
        clr.w *= 1.0 - clamp((tformed.z - AlphaFog.y) / AlphaFog.z, 0.0, 1.0);\
    }\
    flatColor = clr;\
    smoothColor = clr;\
    if (Textured) gl_TexCoord[0] = vec4(vUV, 0, 0);\
}";
 
}