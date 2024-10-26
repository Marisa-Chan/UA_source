#version 410

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

in vec3 vPos;
in vec4 vColor;
in vec2 vUV;

out vec4 smoothColor;
out vec2 texCoords;
void main()
{
    gl_Position = MView * MProj * vec4(vPos, 1.0);
    smoothColor = vColor;
    texCoords = vUV;
}