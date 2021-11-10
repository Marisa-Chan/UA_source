#version 140

layout(std140) uniform Parameters
{
    mat4 MProj;
    mat4 MView;
    vec3 Fog;
    vec3 AlphaFog;
    bool Textured;
    bool Flat;
    bool ATest;
};

attribute vec3 vPos;
attribute vec4 vColor;
attribute vec2 vUV;

out vec4 smoothColor;

void main()
{
    gl_Position = MView * MProj * vec4(vPos, 1.0);
    smoothColor = vColor;
    gl_TexCoord[0] = vec4(vUV, 0, 0);
}