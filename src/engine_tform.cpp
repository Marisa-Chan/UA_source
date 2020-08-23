#include "includes.h"
#include "engine_tform.h"
#include "math.h"
#include "utils.h"


TFEngine TFEngine::Engine;

Common::Ini::KeyList tform_keys
{
    Common::Ini::Key("tform.backplane",  Common::Ini::KT_DIGIT, (int32_t)4096),
    Common::Ini::Key("tform.frontplane", Common::Ini::KT_DIGIT, (int32_t)16),
    Common::Ini::Key("tform.zoomx",      Common::Ini::KT_DIGIT, (int32_t)320),
    Common::Ini::Key("tform.zoomy",      Common::Ini::KT_DIGIT, (int32_t)200)
};

int TFEngine::init()
{
    Common::Ini::ParseIniFile(NC_STACK_nucleus::DefaultIniFile, &tform_keys);

    SinCos_table = new SinCos[361];

    if ( !SinCos_table)
        return 0;

    for (int i = 0; i < 361; i++)
    {
        float rad = i * C_PI_180;

        SinCos_table[i].sin = sin(rad);
        SinCos_table[i].cos = cos(rad);
    }

    return 1;
}

void TFEngine::deinit()
{
    if (SinCos_table)
        delete[] SinCos_table;

    SinCos_table = NULL;
}



// rot_x == 0   rot_y == 0   rot_z == 0
void TFEngine::TForm3D::scale_rot_0()
{
    locSclRot.m00 = scale.x;
    locSclRot.m01 = 0;
    locSclRot.m02 = 0;

    locSclRot.m10 = 0;
    locSclRot.m11 = scale.y;
    locSclRot.m12 = 0;

    locSclRot.m20 = 0;
    locSclRot.m21 = 0;
    locSclRot.m22 = scale.z;
}

// rot_x != 0   rot_y == 0   rot_z == 0
void TFEngine::TForm3D::scale_rot_1()
{
    SinCos &angl = Engine.Angle(ax >> 16);

    locSclRot.m00 = scale.x;
    locSclRot.m01 = 0;
    locSclRot.m02 = 0;

    locSclRot.m10 = 0;
    locSclRot.m11 = scale.y *  angl.cos;
    locSclRot.m12 = scale.y * -angl.sin;

    locSclRot.m20 = 0;
    locSclRot.m21 = scale.z * angl.sin;
    locSclRot.m22 = scale.z * angl.cos;

}

// rot_x == 0   rot_y != 0   rot_z == 0
void TFEngine::TForm3D::scale_rot_2()
{
    SinCos &angl = Engine.Angle(ay >> 16);

    locSclRot.m00 = scale.x * angl.cos;
    locSclRot.m01 = 0;
    locSclRot.m02 = scale.x * angl.sin;

    locSclRot.m10 = 0;
    locSclRot.m11 = scale.y;
    locSclRot.m12 = 0;

    locSclRot.m20 = -angl.sin * scale.z;
    locSclRot.m21 = 0;
    locSclRot.m22 =  angl.cos * scale.z;
}

// rot_x != 0   rot_y != 0   rot_z == 0
void TFEngine::TForm3D::scale_rot_3()
{
    SinCos &angl_x = Engine.Angle(ax >> 16);
    SinCos &angl_y = Engine.Angle(ay >> 16);

    locSclRot.m00 = scale.x * angl_y.cos;
    locSclRot.m01 = 0;
    locSclRot.m02 = scale.x * angl_y.sin;

    locSclRot.m10 =  angl_x.sin * angl_y.sin * scale.y;
    locSclRot.m11 =   scale.y * angl_x.cos;
    locSclRot.m12 = -angl_x.sin * angl_y.cos * scale.y;

    locSclRot.m20 = -angl_x.cos * angl_y.sin * scale.z;
    locSclRot.m21 =   scale.z * angl_x.sin;
    locSclRot.m22 =  angl_x.cos * angl_y.cos * scale.z;
}

// rot_x == 0   rot_y == 0   rot_z != 0
void TFEngine::TForm3D::scale_rot_4()
{
    TFEngine::SinCos &angl = TFEngine::Engine.Angle(az >> 16);

    locSclRot.m00 =  angl.cos * scale.x;
    locSclRot.m01 = -angl.sin * scale.x;
    locSclRot.m02 = 0;

    locSclRot.m10 = scale.y * angl.sin;
    locSclRot.m11 = scale.y * angl.cos;
    locSclRot.m12 = 0;

    locSclRot.m20 = 0;
    locSclRot.m21 = 0;
    locSclRot.m22 = scale.z;
}

// rot_x != 0   rot_y == 0   rot_z != 0
void TFEngine::TForm3D::scale_rot_5()
{
    SinCos &angl_x = Engine.Angle(ax >> 16);
    SinCos &angl_z = Engine.Angle(az >> 16);

    locSclRot.m00 =   scale.x * angl_z.cos;
    locSclRot.m01 = -angl_z.sin * angl_x.cos * scale.x;
    locSclRot.m02 =  angl_z.sin * angl_x.sin * scale.x;

    locSclRot.m10 =   scale.y * angl_z.sin;
    locSclRot.m11 =  angl_z.cos * angl_x.cos * scale.y;
    locSclRot.m12 = -angl_z.cos * angl_x.sin * scale.y;

    locSclRot.m20 = 0;
    locSclRot.m21 = scale.z * angl_x.sin;
    locSclRot.m22 = scale.z * angl_x.cos;
}

// rot_x == 0   rot_y != 0   rot_z != 0
void TFEngine::TForm3D::scale_rot_6()
{
    SinCos &angl_y = Engine.Angle(ay >> 16);
    SinCos &angl_z = Engine.Angle(az >> 16);

    locSclRot.m00 =  angl_z.cos * angl_y.cos * scale.x;
    locSclRot.m01 = -angl_z.sin * scale.x;
    locSclRot.m02 =  angl_z.cos * angl_y.sin * scale.x;

    locSclRot.m10 =  angl_z.sin * angl_y.cos * scale.y;
    locSclRot.m11 =  angl_z.cos * scale.y;
    locSclRot.m12 =  angl_z.sin * angl_y.sin * scale.y;

    locSclRot.m20 = -angl_y.sin * scale.z;
    locSclRot.m21 = 0;
    locSclRot.m22 =  angl_y.cos * scale.z;

}

// rot_x != 0   rot_y != 0   rot_z != 0
void TFEngine::TForm3D::scale_rot_7()
{
    SinCos &angl_x = Engine.Angle(ax >> 16);
    SinCos &angl_y = Engine.Angle(ay >> 16);
    SinCos &angl_z = Engine.Angle(az >> 16);


    locSclRot.m00 = (angl_z.cos * angl_y.cos - angl_z.sin * angl_x.sin * angl_y.sin) * scale.x;
    locSclRot.m01 = -angl_z.sin * angl_x.cos * scale.x;
    locSclRot.m02 = (angl_z.cos * angl_y.sin + angl_z.sin * angl_x.sin * angl_y.cos) * scale.x;

    locSclRot.m10 = (angl_z.sin * angl_y.cos + angl_z.cos * angl_x.sin * angl_y.sin) * scale.y;
    locSclRot.m11 =  angl_z.cos * angl_x.cos * scale.y;
    locSclRot.m12 = (angl_z.sin * angl_y.sin - angl_z.cos * angl_x.sin * angl_y.cos) * scale.y;

    locSclRot.m20 = -angl_x.cos * angl_y.sin * scale.z;
    locSclRot.m21 =  angl_x.sin * scale.z;
    locSclRot.m22 =  angl_x.cos * angl_y.cos * scale.z;
}


void TFEngine::TForm3D::MakeScaleRotationMatrix()
{
    typedef void (TFEngine::TForm3D::*matrfunc)();
    const static matrfunc pTFormFunc[8] = {&TFEngine::TForm3D::scale_rot_0,
                                           &TFEngine::TForm3D::scale_rot_1,
                                           &TFEngine::TForm3D::scale_rot_2,
                                           &TFEngine::TForm3D::scale_rot_3,
                                           &TFEngine::TForm3D::scale_rot_4,
                                           &TFEngine::TForm3D::scale_rot_5,
                                           &TFEngine::TForm3D::scale_rot_6,
                                           &TFEngine::TForm3D::scale_rot_7
                                          };

    uint8_t func_id = 0;

    if ( ax )
        func_id |= 1;

    if ( ay )
        func_id |= 2;

    if ( az )
        func_id |= 4;

    (this->*(pTFormFunc[func_id]))();
}


void TFEngine::TForm3D::CalcGlobal()
{
    if ( parent_1c && (flags & TFEngine::TForm3D::FLAG_FOLLOW_PARENT) )
    {
        globPos = parent_1c->globSclRot.Transform( locPos );
        globSclRot = parent_1c->globSclRot * locSclRot;

        mat4x4 loc = locSclRot;
        loc += locPos;
        tform = loc * tform;
    }
    else
    {
        globPos = locPos;
        globSclRot = locSclRot;

        tform = locSclRot;
        tform += locPos;
    }
}
