#version 120

attribute vec3 vPos;
attribute vec4 vColor;
attribute vec2 vUV;

void main()
{
    gl_Position = vec4(vPos, 1.0);
    gl_FrontColor = vColor;
    gl_TexCoord[0] = vec4(vUV, 0, 0);
}