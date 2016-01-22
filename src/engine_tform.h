#ifndef ENGINE_TFORM_H_INCLUDED
#define ENGINE_TFORM_H_INCLUDED

extern class_stored tform_engine_off;

struct SinCos
{
    float sin;
    float cos;
};

extern SinCos *SinCos_table;

struct xyz
{
    float sx;
    float sy;
    float sz;
};

struct mat3x3
{
    float m00;
    float m01;
    float m02;
    float m10;
    float m11;
    float m12;
    float m20;
    float m21;
    float m22;
};

#endif // ENGINE_TFORM_H_INCLUDED
