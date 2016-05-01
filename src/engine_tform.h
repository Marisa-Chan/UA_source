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

struct destFX
{
    char type_flag;
    int p1; // Model id. >= 0
    float p2;
    float p3;
    float p4;
};

void mat_mult(mat3x3 *mat1, mat3x3 *mat2, mat3x3 *dst);
void mat_rotate_y(mat3x3 *mat, float a2);

#endif // ENGINE_TFORM_H_INCLUDED
