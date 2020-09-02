#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

#include "yw.h"
#include "ypacar.h"
#include "yw_net.h"

#include "log.h"


const Nucleus::ClassDescr NC_STACK_ypacar::description("ypacar.class", &newinstance);

NC_STACK_ypacar::NC_STACK_ypacar() 
{
    _carKamikaze = false;
    _carBlast = 0;
}
    
size_t NC_STACK_ypacar::func0(IDVList &stak)
{
    if ( !NC_STACK_ypatank::func0(stak) )
        return 0;

    _bact_type = BACT_TYPES_CAR;

    for( auto& it : stak )
    {
        IDVPair &val = it.second;

        if ( !val.Skip )
        {
            switch (val.ID)
            {
            case CAR_ATT_KAMIKAZE:
                setCAR_kamikaze(val.Get<int32_t>());
                break;

            case CAR_ATT_BLAST:
                setCAR_blast(val.Get<int32_t>());
                break;

            default:
                break;
            }
        }
    }

    return 1;
}

size_t NC_STACK_ypacar::func1()
{
    return NC_STACK_ypatank::func1();
}

size_t NC_STACK_ypacar::func2(IDVList &stak)
{
    NC_STACK_ypatank::func2(stak);

    for( auto& it : stak )
    {
        IDVPair &val = it.second;

        if ( !val.Skip )
        {
            switch (val.ID)
            {
            case CAR_ATT_KAMIKAZE:
                setCAR_kamikaze(val.Get<int32_t>());
                break;

            case CAR_ATT_BLAST:
                setCAR_blast(val.Get<int32_t>());
                break;

            default:
                break;
            }
        }
    }

    return 1;
}

void NC_STACK_ypacar::DoKamikaze()
{
    float v53;

    if ( _carBlast >= 10000 )
        v53 = log(10000.0 / _carBlast) * -428.5714285714286;
    else
        v53 = 1.0;

    int v3 = (v53 / 300.0);

    for (int i = 0; i < v3; i++)
    {
        float v4 = sqrt( POW2(i) + POW2(v3) );

        for (int j = 0; j <= v4; j++)
        {
            yw_arg129 arg120;
            arg120.field_10 = exp( sqrt( POW2(i) + POW2(j) ) * (1200.0 * -2.8) / 1200.0 ) * _carBlast;
            arg120.pos = vec3d(i, 0, j) * 300.0 + _position.X0Z();
            arg120.unit = this;

            if ( arg120.pos.x > 1200.0  &&  arg120.pos.x < _wrldX - 1200.0  &&  arg120.pos.z < -1200.0  &&  arg120.pos.z > _wrldY + 1200.0 )
                ChangeSectorEnergy(&arg120);

            if ( i )
            {
                arg120.pos.x = _position.x - i * 300.0;
                arg120.pos.z = _position.z + j * 300.0;

                if ( arg120.pos.x > 1200.0 )
                {
                    if ( arg120.pos.x < _wrldX - 1200.0 && arg120.pos.z < -1200.0 && arg120.pos.z > _wrldY + 1200.0 )
                        ChangeSectorEnergy(&arg120);
                }
            }

            if ( j )
            {
                arg120.pos.x = _position.x + i * 300.0;
                arg120.pos.z = _position.z - j * 300.0;

                if ( arg120.pos.x > 1200.0 )
                {
                    if ( arg120.pos.x < _wrldX - 1200.0  &&  arg120.pos.z < -1200.0  &&  arg120.pos.z > _wrldY + 1200.0 )
                        ChangeSectorEnergy(&arg120);
                }
            }

            if ( i && j )
            {
                arg120.pos.x = _position.x - i * 300.0;
                arg120.pos.z = _position.z - j * 300.0;

                if ( arg120.pos.x > 1200.0 )
                {
                    if ( arg120.pos.x < _wrldX - 1200.0  &&  arg120.pos.z < -1200.0  &&  arg120.pos.z > _wrldY + 1200.0 )
                        ChangeSectorEnergy(&arg120);
                }
            }
        }
    }

    int v13 = v53 / 1200.0;

    for (int i = 0; i < v13; i++)
    {
        int v59 = sqrt( POW2( i ) + POW2( v13 ) );

        for (int j = 0; j <= v59; j++ )
        {
            int v16 = i + _sectX;
            int v18 = j + _sectY;

            if ( v16 > 0 && v16 < _secMaxX - 1  &&  v18 > 0 && v18 < _secMaxY - 1 )
            {
                for ( NC_STACK_ypabact* &v19 : _pSector[i + j * _secMaxX].unitsList )
                {
                    int v63 = 0;

                    if ( v19 != this )
                    {
                        if ( v19->_bact_type != BACT_TYPES_MISSLE )
                        {
                            vec3d tmp = _position - v19->_position;

                            int v26 = exp(tmp.length() * -2.8 / 1200.0) * _carBlast;
                            int v67 = ((1.0 - (float)v19->_shield * 0.01) * (float)v26);

                            v19->_energy -= v67;

                            if ( _world->isNetGame )
                            {
                                v63 = 1;

                                uamessage_vhclEnergy veMsg;
                                veMsg.msgID = UAMSG_VHCLENERGY;
                                veMsg.tstamp = _world->timeStamp;
                                veMsg.owner = v19->_owner;
                                veMsg.id = v19->_gid;
                                veMsg.energy = -v67;

                                yw_arg181 arg181;
                                arg181.recvID = 0;
                                arg181.recvFlags = 2;
                                arg181.senderFlags = 1;
                                arg181.dataSize = sizeof(veMsg);
                                arg181.senderID = _world->GameShell->callSIGN.c_str();
                                arg181.garant = 1;
                                arg181.data = &veMsg;

                                _world->ypaworld_func181(&arg181);
                            }
                        }

                        if ( v19->_energy <= 0 || v19->_bact_type == BACT_TYPES_MISSLE )
                        {
                            setState_msg arg78;
                            arg78.setFlags = 0;
                            arg78.unsetFlags = 0;
                            arg78.newStatus = BACT_STATUS_DEAD;

                            if ( v19->_bact_type == BACT_TYPES_MISSLE )
                                v19->SetState(&arg78);
                            else
                                v19->SetStateInternal(&arg78);

                            if ( v19->_bact_type == BACT_TYPES_ROBO )
                            {
                                if ( !v63 )
                                {
                                    yw_arg177 arg177;
                                    arg177.bact = v19;
                                    arg177.field_4 = _owner;

                                    _world->ypaworld_func177(&arg177);
                                }
                            }
                        }
                    }
                }

            }
        }
    }

    _rotation.m21 = 1.9;

    vec3d az = _rotation.AxisZ();
    az.normalise();

    _rotation.SetZ(az);

    if ( fabs(_rotation.m22) <= 0.1 )
    {
        float m20 = _rotation.m20;

        NDIV_CARRY(m20);

        _rotation.m02 = sqrt( 1.0 / (POW2(_rotation.m22) / (POW2(m20)) + 1.0) );
        _rotation.m00 = -_rotation.m22 * _rotation.m02 / m20;
    }
    else
    {
        float m22 = _rotation.m22;

        NDIV_CARRY(m22);

        _rotation.m00 = sqrt( 1.0 / (POW2(_rotation.m20) / (POW2(m22)) + 1.0) );
        _rotation.m02 = -_rotation.m20 * _rotation.m00 / m22;
    }

    _rotation.m01 = 0.0;

    if ( _rotation.AxisZ().XZ().cross( _rotation.AxisX().XZ() ) > 0.0 )
    {
        _rotation.m00 = -_rotation.m00;
        _rotation.m02 = -_rotation.m02;
    }

    _rotation.SetY( _rotation.AxisZ() * _rotation.AxisX() );

    _fly_dir = -_rotation.AxisZ();

    _fly_dir_length = 200.0;
    _status_flg &= ~BACT_STFLAG_LAND;
    _bact_type = BACT_TYPES_FLYER;

    setState_msg arg78;
    arg78.newStatus = BACT_STATUS_DEAD;
    arg78.setFlags = 0;
    arg78.unsetFlags = 0;

    SetState(&arg78);

    _bact_type = BACT_TYPES_CAR;
    _energy = -10;
}

void NC_STACK_ypacar::User_layer(update_msg *arg)
{
    _airconst = _airconst_static;

    int a4 = getBACT_bactCollisions();

    _old_pos = _position;

    float v78 = arg->frameTime / 1000.0;
    vec3d oldPos = _rotation.AxisZ();

    if (_status == BACT_STATUS_DEAD)
        DeadTimeUpdate(arg);
    else if (_status == BACT_STATUS_NORMAL || _status == BACT_STATUS_IDLE)
    {
        if ( _fly_dir_length != 0.0 )
        {
            if ( ! (_status_flg & BACT_STFLAG_FIRE) )
            {
                setState_msg arg78;
                arg78.newStatus = BACT_STATUS_NORMAL;
                arg78.unsetFlags = 0;
                arg78.setFlags = 0;

                SetState(&arg78);
            }
        }
        else
        {
            if ( _primTtype != BACT_TGT_TYPE_CELL || (_primTpos.XZ() - _position.XZ()).length() <= 800.0 )
            {
                if ( !(_status_flg & BACT_STFLAG_FIRE) )
                {
                    setState_msg arg78;
                    arg78.newStatus = BACT_STATUS_IDLE;
                    arg78.unsetFlags = 0;
                    arg78.setFlags = 0;

                    SetState(&arg78);
                }

                _status = BACT_STATUS_NORMAL;
            }
            else
            {

                if ( _status_flg & BACT_STFLAG_FIRE )
                {
                    _status = BACT_STATUS_IDLE;
                }
                else
                {
                    setState_msg arg78;
                    arg78.newStatus = BACT_STATUS_IDLE;
                    arg78.unsetFlags = 0;
                    arg78.setFlags = 0;

                    SetState(&arg78);
                }
            }
        }

        if ( arg->inpt->Sliders[3] != 0.0 )
        {
            if ( _fly_dir_length != 0.0 )
            {
                float v63 = fabs(_fly_dir_length);
                float angle = -arg->inpt->Sliders[3] * _maxrot * v78 * (sqrt(v63) * 0.2);

                _rotation = mat3x3::RotateY(angle) * _rotation;
            }
        }

        if ( (_fly_dir_length < 0.0 && _thraction > 0.0) || (_fly_dir_length > 0.0 && _thraction < 0.0) )
        {
            if ( fabs(_fly_dir.y) > 0.1 )
            {
                float v74 = 1.0 - (v78 * 4.0);

                if ( v74 < 0.1 )
                    v74 = 0.1;

                _fly_dir_length *= v74;
            }
        }

        float v76 = arg->inpt->Sliders[4];
        float v65 = fabs(arg->inpt->Sliders[4]);

        if ( v76 < -1.0 )
            v76 = -1.0;
        else if ( v76 > 1.0)
            v76 = 1.0;

        _thraction += _force * (v78 * 0.75) * v76;

        float v69;
        if ( arg->inpt->Buttons.Is(31) )
            v69 = _force * v65;
        else
            v69 = _force;

        if ( _thraction > v69 )
            _thraction = v69;

        if ( _thraction < -v69 )
            _thraction = -v69;

        if ( fabs(v76) > 0.001 )
            _status_flg |= BACT_STFLAG_MOVE;

        _gun_angle_user += v78 * arg->inpt->Sliders[5];

        if ( _gun_angle_user < -0.3 )
            _gun_angle_user = -0.3;

        if ( _gun_angle_user > 0.8 )
            _gun_angle_user = 0.8;

        bact_arg79 arg79;
        arg79.tgType = BACT_TGT_TYPE_DRCT;
        arg79.tgt_pos = _rotation.AxisZ();

        float corW, corH;
        GFXEngine::GFXe.getC3D()->getAspectCorrection(corW, corH, true);

        bact_arg106 arg106;
        arg106.field_0 = 5;
        arg106.field_4 = _rotation.AxisZ() - vec3d::OY(_gun_angle_user * corH);

        if ( UserTargeting(&arg106) )
        {
            arg79.tgType = BACT_TGT_TYPE_UNIT;
            arg79.target.pbact = arg106.ret_bact;
        }

        if ( arg->inpt->Buttons.IsAny({0, 1}) )
        {
            if ( _carKamikaze )
            {
                DoKamikaze();
            }
            else
            {
                arg79.weapon = _weapon;
                arg79.direction = _rotation.AxisZ() - _rotation.AxisY() * _gun_angle_user * corH;
                arg79.g_time = _clock;

                if ( _clock % 2 )
                    arg79.start_point.x = -_fire_pos.x;
                else
                    arg79.start_point.x = _fire_pos.x;

                arg79.start_point.y = _fire_pos.y;
                arg79.start_point.z = _fire_pos.z;

                arg79.flags = (arg->inpt->Buttons.Is(1) ? 1 : 0) | 2;

                LaunchMissile(&arg79);
            }
        }

        if ( _mgun != -1 )
        {
            if ( _status_flg & BACT_STFLAG_FIRE )
            {
                if ( !arg->inpt->Buttons.Is(2) )
                {
                    setState_msg arg78;
                    arg78.setFlags = 0;
                    arg78.newStatus = BACT_STATUS_NOPE;
                    arg78.unsetFlags = BACT_STFLAG_FIRE;

                    SetState(&arg78);
                }
            }

            if ( arg->inpt->Buttons.Is(2) )
            {
                if ( !(_status_flg & BACT_STFLAG_FIRE) )
                {
                    setState_msg arg78;
                    arg78.setFlags = BACT_STFLAG_FIRE;
                    arg78.unsetFlags = 0;
                    arg78.newStatus = BACT_STATUS_NOPE;

                    SetState(&arg78);
                }

                bact_arg105 arg105;
                arg105.field_0 = _rotation.AxisZ() - _rotation.AxisY() * _gun_angle_user * corH;
                arg105.field_C = v78;
                arg105.field_10 = _clock;

                FireMinigun(&arg105);
            }
        }

        if ( _status_flg & BACT_STFLAG_LAND )
        {
            move_msg arg74;

            if ( arg->inpt->Buttons.Is(3) )
            {
                _thraction = 0;

                if ( fabs(_fly_dir_length) >= 4.0 )
                {
                    float v75 = 1.0 - v78 * 4.0;

                    if ( v75 < 0.1 )
                        v75 = 0.1;

                    _fly_dir_length *= v75 * 0.1;
                }
                else
                {
                    _status_flg &= ~BACT_STFLAG_MOVE;
                    _fly_dir_length = 0;
                }

                arg74.flag = 2;
            }
            else
            {
                arg74.flag = 0;
            }

            arg74.flag = 0;
            arg74.field_0 = v78;

            if ( _status_flg & BACT_STFLAG_MOVE )
                Move(&arg74);

            if ( a4 && CollisionWithBact(arg->frameTime) )
            {
                AlignVehicleUser(v78, oldPos);
            }
            else
            {
                ypaworld_arg136 arg136;

                arg136.stPos = _old_pos;
                arg136.vect = _position - _old_pos;
                arg136.flags = 0;

                _world->ypaworld_func136(&arg136);

                if ( arg136.isect )
                {

                    _position = _old_pos;

                    _fly_dir_length = 0;
                    _thraction = 0;
                }
                else
                {
                    AlignVehicleUser(v78, oldPos);
                }
            }
        }
        else
        {
            bact_arg86 arg86;
            arg86.field_one = 1;
            arg86.field_two = arg->frameTime;

            CrashOrLand(&arg86);
        }
    }
}

int NC_STACK_ypacar::AlignVehicleAI(float dtime, vec3d *pNormal)
{
    int a4 = getBACT_viewer();

    float v5;

    if ( a4 )
        v5 = _viewer_overeof;
    else
        v5 = _overeof;

    ypaworld_arg136 arg136;
    arg136.stPos = _rotation.AxisY() * v5 + _position - vec3d::OY(50.0);
    arg136.vect = vec3d::OY( _overeof + 110.0 );
    arg136.flags = 0;

    _world->ypaworld_func136(&arg136);

    if ( !arg136.isect )
        return ALIGN_NONE;

    vec3d normal = arg136.skel->polygons[ arg136.polyID ].Normal();

    if ( normal.y < 0.6 )
    {
        if (pNormal)
            *pNormal = normal;
        return ALIGN_NORMAL;
    }

    vec3d vaxis = _rotation.AxisY() * normal;

    if ( vaxis.normalise() > 0.0 )
    {
        float v56 = clp_acos( _rotation.AxisY().dot( normal ) );
        float v45 = _maxrot * 2.0 * dtime;

        if ( v56 > v45 )
            v56 = v45;

        if ( fabs(v56) < 0.01 )
            v56 = 0.0;

        _rotation *= mat3x3::AxisAngle(vaxis, v56);
    }

    _position = arg136.isectPos - _rotation.AxisY() * v5;

    return ALIGN_DONE;
}

vec3d NC_STACK_ypacar::CarTip(float dtime, const vec3d &oldDir, vec3d rot)
{
    vec2d tmp = _rotation.AxisZ().XZ();
    vec2d varg = oldDir.XZ();

    int a4 = getBACT_viewer();
    float v73;

    if ( a4 )
        v73 = -0.5;
    else
        v73 = -0.2;

    float v78 = _thraction - _airconst * _fly_dir_length;

    if ( fabs(v78) > 0.1 )
    {
        float v8 = v73 * v78 / _force;

        rot = mat3x3::AxisAngle(_rotation.AxisX(), v8).Transform(rot);
    }

    float v76 = tmp.dot( varg );

    float tmpsq = tmp.length();

    NDIV_CARRY(tmpsq);

    v76 = v76 / tmpsq;

    tmpsq = varg.length();

    NDIV_CARRY(tmpsq);

    v76 = v76 / tmpsq;


    if (dtime != 0.0)
    {
        float v77 = clp_acos(v76) * (fabs(_fly_dir_length) * 0.002 / dtime);

        if ( v77 > 0.001 )
        {
            if ( tmp.cross(varg) < 0.0 )
                v77 = -v77;

            rot = mat3x3::AxisAngle(_rotation.AxisZ(), v77).Transform(rot);
        }
    }
    
    return rot;
}

int NC_STACK_ypacar::AlignVehicleUser(float dtime, const vec3d &oldDir)
{
    float v162 = 1.73;
    float v166 = 1.7;
    int v158 = 0;
    int v160 = 0;
    int v161 = 0;

    int a4 = getBACT_viewer();
    int v149 = getBACT_inputting();

    float v5;

    if ( a4 )
        v5 = _viewer_overeof;
    else
        v5 = _overeof;

    vec3d t1, t2, t3;
    float v167;

    if ( _fly_dir_length < 0.0 )
    {
        t1 = _position + (_rotation.AxisX() - _rotation.AxisZ()) * _viewer_radius * 0.7071;

        t2 = _position + _rotation.AxisZ() * _viewer_radius;

        t3 = _position + (-_rotation.AxisX() - _rotation.AxisZ()) * _viewer_radius * 0.7071;

        v167 = -1.0;
    }
    else
    {
        t1 = _position + (_rotation.AxisX() + _rotation.AxisZ()) * _viewer_radius * 0.7071;

        t2 = _position - _rotation.AxisZ() * _viewer_radius;

        t3 = _position + (-_rotation.AxisX() + _rotation.AxisZ()) * _viewer_radius * 0.7071;

        v167 = 1.0;
    }

    t1 += vec3d::OY(_viewer_overeof);
    t2 += vec3d::OY(_viewer_overeof);
    t3 += vec3d::OY(_viewer_overeof);

    ypaworld_arg136 arg136, arg136_1, arg136_2;

    arg136.stPos = _position - vec3d::OY(v162 * _viewer_radius);
    arg136.vect = (t1 - arg136.stPos) * v166;
    arg136.flags = 0;

    arg136_1.stPos = _position - vec3d::OY(v162 * _viewer_radius);
    arg136_1.vect = (t2 - arg136_1.stPos) * v166;
    arg136_1.flags = 0;

    arg136_2.stPos = _position - vec3d::OY(v162 * _viewer_radius);
    arg136_2.vect = (t3 - arg136_2.stPos) * v166;
    arg136_2.flags = 0;

    _world->ypaworld_func136(&arg136);

    if ( arg136.isect )
    {
        UAskeleton::Polygon *v48 = &arg136.skel->polygons[ arg136.polyID ];

        if ( fabs(v48->B) < 0.6 )
        {
            float v176 = _rotation.AxisZ().dot( v48->Normal() ) * v167;

            if ( v176 <= 0.0 )
            {
                _thraction = 0;
            }
            else
            {
                if ( v176 >= 0.82 )
                {
                    _thraction = 0;
                    _fly_dir_length = 0;
                    _position = _old_pos;
                }
                else
                {
                    _position.x = _old_pos.x - v48->A * 10.0;
                    _position.z = _old_pos.z - v48->C * 10.0;
                }

                if ( _fly_dir_length > 2.333333333333334 )
                {
                    SFXEngine::SFXe.startSound(&_soundcarrier, 5);

                    if ( v149 )
                    {
                        yw_arg180 arg180;

                        arg180.field_8 = arg136.isectPos.x;
                        arg180.field_C = arg136.isectPos.z;
                        arg180.effects_type = 5;
                        arg180.field_4 = 1.0;

                        _world->ypaworld_func180(&arg180);
                    }
                }
            }
            return 2;
        }
    }
    else
    {
        if ( !v149 )
            return 2;

        arg136.isectPos = _position + arg136.vect - vec3d::OY(v162 * _viewer_radius);

        v158 = 1;
    }

    _world->ypaworld_func136(&arg136_1);

    if ( arg136_1.isect )
    {
        UAskeleton::Polygon *v54 = &arg136_1.skel->polygons[ arg136_1.polyID ];

        if ( fabs(v54->B) < 0.6 )
        {
            float v182 = _rotation.AxisZ().dot( v54->Normal() ) * v167;

            if ( v182 <= 0.0 )
            {
                _thraction = 0;
            }
            else
            {
                if ( v182 >= 0.82 )
                {
                    _fly_dir_length = 0;
                    _position = _old_pos;
                    _thraction = 0;
                }
                else
                {
                    _position.x = _old_pos.x - v54->A * 10.0;
                    _position.z = _old_pos.z - v54->C * 10.0;
                }

                if ( _fly_dir_length > 2.333333333333334 )
                {
                    SFXEngine::SFXe.startSound(&_soundcarrier, 5);

                    if ( v149 )
                    {
                        yw_arg180 arg180;
                        arg180.field_4 = 1.0;
                        arg180.field_8 = arg136_1.isectPos.x;
                        arg180.field_C = arg136_1.isectPos.z;
                        arg180.effects_type = 5;

                        _world->ypaworld_func180(&arg180);
                    }
                }
            }

            return 1;
        }
    }
    else
    {
        if ( !v149 )
            return 1;

        arg136_1.isectPos = _position + arg136_1.vect - vec3d::OY(v162 * _viewer_radius);

        v160 = 1;
    }

    _world->ypaworld_func136(&arg136_2);

    if ( arg136_2.isect )
    {
        UAskeleton::Polygon *v54 = &arg136_2.skel->polygons[ arg136_2.polyID ];

        if ( fabs(v54->B) < 0.6 )
        {
            _position += _fly_dir * _fly_dir_length * dtime * 6.0;

            _thraction = 0;

            if ( _fly_dir_length < -2.333333333333334 )
            {
                SFXEngine::SFXe.startSound(&_soundcarrier, 5);

                if ( v149 )
                {
                    yw_arg180 arg180;
                    arg180.field_4 = 1.0;
                    arg180.field_8 = arg136_2.isectPos.x;
                    arg180.field_C = arg136_2.isectPos.z;
                    arg180.effects_type = 5;

                    _world->ypaworld_func180(&arg180);
                }
            }
            return 3;
        }
    }
    else
    {
        arg136_2.isectPos = _position + arg136_2.vect - vec3d::OY(v162 * _viewer_radius);

        v161 = 1;
    }

    if ( a4 )
    {
        if ( v167 > 0.0 )
        {
            ypaworld_arg136 arg136_3;
            arg136_3.stPos = _position;

            arg136_3.vect = _rotation.AxisZ() * _viewer_radius;

            arg136_3.flags = 0;

            _world->ypaworld_func136(&arg136_3);

            if ( arg136_3.isect )
            {
                if ( arg136_3.skel->polygons[ arg136_3.polyID ].B < 0.6 )
                {
                    if ( _fly_dir_length > 2.333333333333334 )
                    {
                        SFXEngine::SFXe.startSound(&_soundcarrier, 5);

                        if ( v149 )
                        {
                            yw_arg180 arg180;
                            arg180.field_4 = 1.0;
                            arg180.field_8 = arg136_3.isectPos.x;
                            arg180.field_C = arg136_3.isectPos.z;
                            arg180.effects_type = 5;

                            _world->ypaworld_func180(&arg180);
                        }
                    }

                    _thraction = 0;
                    _fly_dir_length = 0;
                    _position = _old_pos;
                }
            }
        }
    }

    if ( arg136_1.isect && arg136.isect && arg136_2.isect )
    {
        vec3d tmp = arg136.isectPos - arg136_2.isectPos;
        vec3d tmp2 = arg136_1.isectPos - arg136_2.isectPos;

        vec3d tmp3 = tmp * tmp2;

        float v157 = tmp3.length() * v167;
        if ( v157 != 0.0 )
            tmp3 /= v157;
        else
            tmp3 = vec3d::OY(1.0);

        if ( tmp3.y < -0.1 )
            tmp3 = -tmp3;

        int v151 = getTANK_tip();

        if ( v151 )
            tmp3 = CarTip(dtime, oldDir, tmp3);


        float v72 = fabs(_fly_dir_length);

        vec3d vaxis = _rotation.AxisY() * tmp3;

        float v173;

        if ( v72 >= 0.5 )
            v173 = 0.007;
        else
            v173 = 0.01;

        if ( vaxis.normalise() > 0.0 )
        {
            float angle = clp_acos( _rotation.AxisY().dot( tmp3 ) );
            float maxrot = _maxrot * 2.0 * dtime;

            if ( angle > maxrot )
                angle = maxrot;

            if ( fabs(angle) > v173 )
                _rotation *= mat3x3::AxisAngle(vaxis, angle);
        }
    }

    if ( !arg136.isect && !arg136_1.isect && !arg136_2.isect )
    {
        _status_flg &= ~BACT_STFLAG_LAND;
        return 0;
    }

    if ( !v158 && !v160 && !v161 )
    {
        _position.y = (arg136.isectPos.y + arg136_1.isectPos.y + arg136_2.isectPos.y) * 0.33333334 - v5;
        return 0;
    }

    float v90 = _viewer_overeof * v166 * 0.8;

    ypaworld_arg136 arg136_4;
    arg136_4.stPos = _position - vec3d::OY(v90);
    arg136_4.vect = vec3d::OY(v90 * 2.0);
    arg136_4.flags = 0;

    _world->ypaworld_func136(&arg136_4);

    if ( arg136_4.isect && (!arg136_4.isect || arg136_4.skel->polygons[ arg136_4.polyID ].B >= 0.6) )
    {
        _position.y = arg136_4.isectPos.y - v5;
        return 0;
    }

    vec3d tf(0.0, 0.0, 0.0);

    _status_flg &= ~BACT_STFLAG_LAND;

    if ( v158 )
        tf += _rotation.AxisZ() * v167 - _rotation.AxisX();

    if ( v160 )
        tf += _rotation.AxisZ() * v167 + _rotation.AxisX();

    if ( v161 )
        tf -= _rotation.AxisZ() * v167;

    _position += tf * (dtime * 400.0);

    return 0;
}


void NC_STACK_ypacar::setCAR_kamikaze(bool kam)
{
    _carKamikaze = kam;
}

void NC_STACK_ypacar::setCAR_blast(int blast)
{
    _carBlast = blast;
}


bool NC_STACK_ypacar::getCAR_kamikaze()
{
    return _carKamikaze;
}

int NC_STACK_ypacar::getCAR_blast()
{
    return _carBlast;
}


