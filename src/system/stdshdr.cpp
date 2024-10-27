
#include "gfx.h"

namespace GFX
{

const std::string GFXEngine:: _stdPShaderText =
"\
#version 410 core\n\
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
uniform sampler2D texSample;\
in vec4 smoothColor;\
flat in vec4 flatColor;\
in vec2 texCoords;\
out vec4 fragColor;\
void main()\
{\
    vec4 clr;\
    if (Flat) clr = flatColor; else clr = smoothColor;\
    if (Textured)\
        clr = texture(texSample, texCoords) * clr;\
    if (ATest && clr.w <= 0.0)\
        discard;\
    fragColor = clr;\
}";


const std::string GFXEngine:: _stdVShaderText =
"\
#version 410 core\n\
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
in vec3 vPos;\
in vec4 vColor;\
in vec2 vUV;\
\
out vec4 smoothColor;\
flat out vec4 flatColor;\
out vec2 texCoords;\
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
    if (Textured) texCoords = vUV;\
}";

}
