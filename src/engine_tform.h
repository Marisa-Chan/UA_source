#ifndef ENGINE_TFORM_H_INCLUDED
#define ENGINE_TFORM_H_INCLUDED

struct SinCos
{
    float sin;
    float cos;
};

#define MAT_FLAG_INV_SIN   1
#define MAT_FLAG_INV_COS   2

struct __attribute__((packed)) xyz
{
    float sx;
    float sy;
    float sz;

    xyz()
    {
        sx = 0.0;
        sy = 0.0;
        sz = 0.0;
    }
};

struct __attribute__((packed)) mat3x3
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

    mat3x3()
    {
        m00 = 0.0;
        m01 = 0.0;
        m02 = 0.0;
        m10 = 0.0;
        m11 = 0.0;
        m12 = 0.0;
        m20 = 0.0;
        m21 = 0.0;
        m22 = 0.0;
    }
};

struct destFX
{
    char type_flag;
    int p1; // Model id. >= 0
    float p2;
    float p3;
    float p4;
};

class TFEngine
{
public:
    int init();
    void deinit();

    static SinCos *SinCos_table;
};

extern TFEngine TFe;


void mat_mult(mat3x3 *mat1, mat3x3 *mat2, mat3x3 *dst);
void mat_rotate_y(mat3x3 *mat, float a2);

void mat_gen_axis_rotate(xyz *u, float angle, mat3x3 *out, int flags = 0);

#endif // ENGINE_TFORM_H_INCLUDED
