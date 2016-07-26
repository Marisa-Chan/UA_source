#include "includes.h"
#include "engine_tform.h"
#include "math.h"
#include "utils.h"


SinCos *TFEngine::SinCos_table = NULL;

key_value_stru tform_keys[4] = {{"tform.backplane", KEY_TYPE_DIGIT, 0x1000},
    {"tform.frontplane", KEY_TYPE_DIGIT, 0x10},
    {"tform.zoomx", KEY_TYPE_DIGIT, 0x140},
    {"tform.zoomy", KEY_TYPE_DIGIT, 0xC8}
};

TFEngine TFe;

int TFEngine::init()
{
    get_keyvalue_from_ini(0, tform_keys, 4);

    SinCos_table = (SinCos *)AllocVec(sizeof(SinCos) * 361, 1);

    if ( !SinCos_table)
        return 0;

    for (int i = 0; i < 361; i++)
    {
        float rad = i * 3.141592653589793 / 180.0;

        SinCos_table[i].sin = sin(rad);
        SinCos_table[i].cos = cos(rad);
    }

    return 1;
}

void TFEngine::deinit()
{
    if (SinCos_table)
        nc_FreeMem(SinCos_table);

    SinCos_table = NULL;
}

void mat_mult(mat3x3 *mat1, mat3x3 *mat2, mat3x3 *dst)
{
    dst->m00 = mat1->m01 * mat2->m10 + mat1->m00 * mat2->m00 + mat1->m02 * mat2->m20;
    dst->m01 = mat1->m01 * mat2->m11 + mat1->m00 * mat2->m01 + mat1->m02 * mat2->m21;
    dst->m02 = mat1->m01 * mat2->m12 + mat1->m00 * mat2->m02 + mat1->m02 * mat2->m22;
    dst->m10 = mat1->m11 * mat2->m10 + mat1->m10 * mat2->m00 + mat1->m12 * mat2->m20;
    dst->m11 = mat1->m10 * mat2->m01 + mat1->m11 * mat2->m11 + mat1->m12 * mat2->m21;
    dst->m12 = mat1->m10 * mat2->m02 + mat1->m11 * mat2->m12 + mat1->m12 * mat2->m22;
    dst->m20 = mat1->m21 * mat2->m10 + mat1->m20 * mat2->m00 + mat1->m22 * mat2->m20;
    dst->m21 = mat1->m20 * mat2->m01 + mat1->m21 * mat2->m11 + mat1->m22 * mat2->m21;
    dst->m22 = mat1->m20 * mat2->m02 + mat1->m21 * mat2->m12 + mat1->m22 * mat2->m22;
}

void mat_rotate_y(mat3x3 *mat, float a2)
{
    mat3x3 tmp;
    tmp.m00 = cos(a2);
    tmp.m01 = 0;
    tmp.m02 = sin(a2);

    tmp.m10 = 0;
    tmp.m11 = 1.0;
    tmp.m12 = 0;

    tmp.m20 = -sin(a2);
    tmp.m21 = 0;
    tmp.m22 = cos(a2);

    mat3x3 dst;

    mat_mult(&tmp, mat, &dst);

    *mat = dst;
}

void mat_gen_axis_rotate(xyz *u, float angle, mat3x3 *out, int flags)
{
    float cs = cos(angle);
    float sn = sin(angle);

    if (flags & MAT_FLAG_INV_COS)
        cs = -cs;

    if (flags & MAT_FLAG_INV_SIN)
        sn = -sn;

    float ics = 1.0 - cs;

    out->m00 = ics * u->sx * u->sx + cs;
    out->m01 = ics * u->sx * u->sy - sn * u->sz;
    out->m02 = ics * u->sz * u->sx + sn * u->sy;
    out->m10 = ics * u->sx * u->sy + sn * u->sz;
    out->m11 = ics * u->sy * u->sy + cs;
    out->m12 = ics * u->sy * u->sz - sn * u->sx;
    out->m20 = ics * u->sz * u->sx - sn * u->sy;
    out->m21 = ics * u->sy * u->sz + sn * u->sx;
    out->m22 = ics * u->sz * u->sz + cs;
}
