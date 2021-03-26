#include "tform.h"
#include "../nucleas.h"
#include "inivals.h"


namespace TF
{
TFEngine TFEngine::Instance;

int TFEngine::Init()
{
    System::IniConf::ReadFromNucleusIni();

    for (size_t i = 0; i < SinCos_table.size(); i++)
    {
        double rad = i * 0.01745329251994329577; // to rad

        SinCos_table.at(i).sin = sin(rad);
        SinCos_table.at(i).cos = cos(rad);
    }

    return 1;
}

void TFEngine::Deinit()
{
}



// rot_x == 0   rot_y == 0   rot_z == 0
void TForm3D::scale_rot_0()
{
    SclRot.m00 = Scale.x;
    SclRot.m01 = 0;
    SclRot.m02 = 0;

    SclRot.m10 = 0;
    SclRot.m11 = Scale.y;
    SclRot.m12 = 0;

    SclRot.m20 = 0;
    SclRot.m21 = 0;
    SclRot.m22 = Scale.z;
}

// rot_x != 0   rot_y == 0   rot_z == 0
void TForm3D::scale_rot_1()
{
    TFEngine::SinCos &angl = Engine.Angle(ax);

    SclRot.m00 = Scale.x;
    SclRot.m01 = 0;
    SclRot.m02 = 0;

    SclRot.m10 = 0;
    SclRot.m11 = Scale.y *  angl.cos;
    SclRot.m12 = Scale.y * -angl.sin;

    SclRot.m20 = 0;
    SclRot.m21 = Scale.z * angl.sin;
    SclRot.m22 = Scale.z * angl.cos;

}

// rot_x == 0   rot_y != 0   rot_z == 0
void TForm3D::scale_rot_2()
{
    TFEngine::SinCos &angl = Engine.Angle(ay);

    SclRot.m00 = Scale.x * angl.cos;
    SclRot.m01 = 0;
    SclRot.m02 = Scale.x * angl.sin;

    SclRot.m10 = 0;
    SclRot.m11 = Scale.y;
    SclRot.m12 = 0;

    SclRot.m20 = -angl.sin * Scale.z;
    SclRot.m21 = 0;
    SclRot.m22 =  angl.cos * Scale.z;
}

// rot_x != 0   rot_y != 0   rot_z == 0
void TForm3D::scale_rot_3()
{
    TFEngine::SinCos &angl_x = Engine.Angle(ax);
    TFEngine::SinCos &angl_y = Engine.Angle(ay);

    SclRot.m00 = Scale.x * angl_y.cos;
    SclRot.m01 = 0;
    SclRot.m02 = Scale.x * angl_y.sin;

    SclRot.m10 =  angl_x.sin * angl_y.sin * Scale.y;
    SclRot.m11 =   Scale.y * angl_x.cos;
    SclRot.m12 = -angl_x.sin * angl_y.cos * Scale.y;

    SclRot.m20 = -angl_x.cos * angl_y.sin * Scale.z;
    SclRot.m21 =   Scale.z * angl_x.sin;
    SclRot.m22 =  angl_x.cos * angl_y.cos * Scale.z;
}

// rot_x == 0   rot_y == 0   rot_z != 0
void TForm3D::scale_rot_4()
{
    TFEngine::SinCos &angl = Engine.Angle(az);

    SclRot.m00 =  angl.cos * Scale.x;
    SclRot.m01 = -angl.sin * Scale.x;
    SclRot.m02 = 0;

    SclRot.m10 = Scale.y * angl.sin;
    SclRot.m11 = Scale.y * angl.cos;
    SclRot.m12 = 0;

    SclRot.m20 = 0;
    SclRot.m21 = 0;
    SclRot.m22 = Scale.z;
}

// rot_x != 0   rot_y == 0   rot_z != 0
void TForm3D::scale_rot_5()
{
    TFEngine::SinCos &angl_x = Engine.Angle(ax);
    TFEngine::SinCos &angl_z = Engine.Angle(az);

    SclRot.m00 =   Scale.x * angl_z.cos;
    SclRot.m01 = -angl_z.sin * angl_x.cos * Scale.x;
    SclRot.m02 =  angl_z.sin * angl_x.sin * Scale.x;

    SclRot.m10 =   Scale.y * angl_z.sin;
    SclRot.m11 =  angl_z.cos * angl_x.cos * Scale.y;
    SclRot.m12 = -angl_z.cos * angl_x.sin * Scale.y;

    SclRot.m20 = 0;
    SclRot.m21 = Scale.z * angl_x.sin;
    SclRot.m22 = Scale.z * angl_x.cos;
}

// rot_x == 0   rot_y != 0   rot_z != 0
void TForm3D::scale_rot_6()
{
    TFEngine::SinCos &angl_y = Engine.Angle(ay);
    TFEngine::SinCos &angl_z = Engine.Angle(az);

    SclRot.m00 =  angl_z.cos * angl_y.cos * Scale.x;
    SclRot.m01 = -angl_z.sin * Scale.x;
    SclRot.m02 =  angl_z.cos * angl_y.sin * Scale.x;

    SclRot.m10 =  angl_z.sin * angl_y.cos * Scale.y;
    SclRot.m11 =  angl_z.cos * Scale.y;
    SclRot.m12 =  angl_z.sin * angl_y.sin * Scale.y;

    SclRot.m20 = -angl_y.sin * Scale.z;
    SclRot.m21 = 0;
    SclRot.m22 =  angl_y.cos * Scale.z;

}

// rot_x != 0   rot_y != 0   rot_z != 0
void TForm3D::scale_rot_7()
{
    TFEngine::SinCos &angl_x = Engine.Angle(ax);
    TFEngine::SinCos &angl_y = Engine.Angle(ay);
    TFEngine::SinCos &angl_z = Engine.Angle(az);


    SclRot.m00 = (angl_z.cos * angl_y.cos - angl_z.sin * angl_x.sin * angl_y.sin) * Scale.x;
    SclRot.m01 = -angl_z.sin * angl_x.cos * Scale.x;
    SclRot.m02 = (angl_z.cos * angl_y.sin + angl_z.sin * angl_x.sin * angl_y.cos) * Scale.x;

    SclRot.m10 = (angl_z.sin * angl_y.cos + angl_z.cos * angl_x.sin * angl_y.sin) * Scale.y;
    SclRot.m11 =  angl_z.cos * angl_x.cos * Scale.y;
    SclRot.m12 = (angl_z.sin * angl_y.sin - angl_z.cos * angl_x.sin * angl_y.cos) * Scale.y;

    SclRot.m20 = -angl_x.cos * angl_y.sin * Scale.z;
    SclRot.m21 =  angl_x.sin * Scale.z;
    SclRot.m22 =  angl_x.cos * angl_y.cos * Scale.z;
}


void TForm3D::MakeScaleRotationMatrix()
{
    typedef void (TForm3D::*matrfunc)();
    const static matrfunc pTFormFunc[] = {  &TForm3D::scale_rot_0,
                                            &TForm3D::scale_rot_1,
                                            &TForm3D::scale_rot_2,
                                            &TForm3D::scale_rot_3,
                                            &TForm3D::scale_rot_4,
                                            &TForm3D::scale_rot_5,
                                            &TForm3D::scale_rot_6,
                                            &TForm3D::scale_rot_7
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


void TForm3D::CalcGlobal()
{
    if ( Parent && (flags & TForm3D::FLAG_FOLLOW_PARENT) )
    {
        CalcPos = Parent->CalcSclRot.Transform( Pos );
        CalcSclRot = Parent->CalcSclRot * SclRot;

        mat4x4 loc = SclRot;
        loc += Pos;
        TForm = loc * TForm;
    }
    else
    {
        CalcPos = Pos;
        CalcSclRot = SclRot;

        TForm = SclRot;
        TForm += Pos;
    }
}


int32_t TForm3D::IntAngle::ClampRawAngle(int32_t angle)
{
    angle %= (360 << 16);
    
    if (angle < 0)
        angle += (360 << 16);
    
    return angle;
}

TForm3D::IntAngle &TForm3D::IntAngle::operator=(int32_t angle)
{
    RawAngle = ClampRawAngle(angle << 16);
    return *this;
}
        
TForm3D::IntAngle &TForm3D::IntAngle::operator+=(int32_t angle)
{
    RawAngle = ClampRawAngle(RawAngle + (angle << 16));
    return *this;
}
        
TForm3D::IntAngle::operator int32_t()
{
    return (RawAngle) >> 16;
}

TForm3D::IntAngle::operator double()
{
    return (double)(RawAngle) / (double)(1 << 16);
}

}