
#include "gfx.h"

namespace GFX
{

const std::string GFXEngine:: _stdPShaderText =
"#version 120\n\
uniform bool Textured;\
uniform sampler2D texture;\
void main()\
{\
    if (Textured)\
        gl_FragColor = texture2D(texture, gl_TexCoord[0].xy) * gl_Color;\
    else\
        gl_FragColor = gl_Color;\
}";


const std::string GFXEngine:: _stdVShaderText =
"#version 120\n\
attribute vec3 vPos;\
attribute vec4 vColor;\
attribute vec2 vUV;\
uniform vec3 Fog;\
uniform vec3 AlphaFog;\
void main()\
{\
    vec4 tformed = gl_ModelViewMatrix * vec4(vPos, 1.0);\
    gl_Position = gl_ProjectionMatrix * tformed;\
    if (Fog.x != 0.0)\
    {\
        float fg = clamp((tformed.z - Fog.y) / Fog.z, 0.0, 1.0);\
        gl_FrontColor = vec4(mix(vColor.xyz, vec3(0.0), fg), vColor.w);\
    }\
    else\
    {\
        gl_FrontColor = vColor;\
    }\
    \
    if (AlphaFog.x != 0.0)\
    {\
        gl_FrontColor.w *= 1.0 - clamp((tformed.z - AlphaFog.y) / AlphaFog.z, 0.0, 1.0);\
    }\
    gl_TexCoord[0] = vec4(vUV, 0, 0);\
}";
 
}