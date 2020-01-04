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
    _carBlast = false;
}
    
size_t NC_STACK_ypacar::func0(IDVList &stak)
{
    if ( !NC_STACK_ypatank::func0(stak) )
        return 0;

    ypabact.bact_type = BACT_TYPES_CAR;

    for(IDVList::iterator it = stak.begin(); it != stak.end(); it++)
    {
        IDVPair &val = it->second;

        if ( !val.skip() )
        {
            switch (val.id)
            {
            case CAR_ATT_KAMIKAZE:
                setCAR_kamikaze(val.value.i_data);
                break;

            case CAR_ATT_BLAST:
                setCAR_blast(val.value.i_data);
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

    for(IDVList::iterator it = stak.begin(); it != stak.end(); it++)
    {
        IDVPair &val = it->second;

        if ( !val.skip() )
        {
            switch (val.id)
            {
            case CAR_ATT_KAMIKAZE:
                setCAR_kamikaze(val.value.i_data);
                break;

            case CAR_ATT_BLAST:
                setCAR_blast(val.value.i_data);
                break;

            default:
                break;
            }
        }
    }

    return 1;
}

size_t NC_STACK_ypacar::func3(IDVList &stak)
{
    NC_STACK_ypatank::func3(stak);

    for(IDVList::iterator it = stak.begin(); it != stak.end(); it++)
    {
        IDVPair &val = it->second;

        if ( !val.skip() )
        {
            switch (val.id)
            {
            case CAR_ATT_KAMIKAZE:
                *(int *)val.value.p_data = getCAR_kamikaze();
                break;

            case CAR_ATT_BLAST:
                *(int *)val.value.p_data = getCAR_blast();
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
            arg120.pos = vec3d(i, 0, j) * 300.0 + ypabact.position.X0Z();
            arg120.unit = &ypabact;

            if ( arg120.pos.x > 1200.0  &&  arg120.pos.x < ypabact.wrldX - 1200.0  &&  arg120.pos.z < -1200.0  &&  arg120.pos.z > ypabact.wrldY + 1200.0 )
                ChangeSectorEnergy(&arg120);

            if ( i )
            {
                arg120.pos.x = ypabact.position.x - i * 300.0;
                arg120.pos.z = ypabact.position.z + j * 300.0;

                if ( arg120.pos.x > 1200.0 )
                {
                    if ( arg120.pos.x < ypabact.wrldX - 1200.0 && arg120.pos.z < -1200.0 && arg120.pos.z > ypabact.wrldY + 1200.0 )
                        ChangeSectorEnergy(&arg120);
                }
            }

            if ( j )
            {
                arg120.pos.x = ypabact.position.x + i * 300.0;
                arg120.pos.z = ypabact.position.z - j * 300.0;

                if ( arg120.pos.x > 1200.0 )
                {
                    if ( arg120.pos.x < ypabact.wrldX - 1200.0  &&  arg120.pos.z < -1200.0  &&  arg120.pos.z > ypabact.wrldY + 1200.0 )
                        ChangeSectorEnergy(&arg120);
                }
            }

            if ( i && j )
            {
                arg120.pos.x = ypabact.position.x - i * 300.0;
                arg120.pos.z = ypabact.position.z - j * 300.0;

                if ( arg120.pos.x > 1200.0 )
                {
                    if ( arg120.pos.x < ypabact.wrldX - 1200.0  &&  arg120.pos.z < -1200.0  &&  arg120.pos.z > ypabact.wrldY + 1200.0 )
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
            int v16 = i + ypabact.sectX;
            int v18 = j + ypabact.sectY;

            if ( v16 > 0 && v16 < ypabact.secMaxX - 1  &&  v18 > 0 && v18 < ypabact.secMaxY - 1 )
            {
                __NC_STACK_ypabact *v19 = (__NC_STACK_ypabact *)ypabact.pSector[i + j * ypabact.secMaxX].units_list.head;

                while ( v19->next )
                {
                    int v63 = 0;

                    if ( v19 != &ypabact )
                    {
                        if ( v19->bact_type != BACT_TYPES_MISSLE )
                        {
                            vec3d tmp = ypabact.position - v19->position;

                            int v26 = exp(tmp.length() * -2.8 / 1200.0) * _carBlast;
                            int v67 = ((1.0 - (float)v19->shield * 0.01) * (float)v26);

                            v19->energy -= v67;

                            if ( _world->ypaworld.isNetGame )
                            {
                                v63 = 1;

                                uamessage_vhclEnergy veMsg;
                                veMsg.msgID = UAMSG_VHCLENERGY;
                                veMsg.tstamp = _world->ypaworld.timeStamp;
                                veMsg.owner = v19->owner;
                                veMsg.id = v19->gid;
                                veMsg.energy = -v67;

                                yw_arg181 arg181;
                                arg181.recvID = 0;
                                arg181.recvFlags = 2;
                                arg181.senderFlags = 1;
                                arg181.dataSize = sizeof(veMsg);
                                arg181.senderID = _world->ypaworld.GameShell->callSIGN.c_str();
                                arg181.garant = 1;
                                arg181.data = &veMsg;

                                _world->ypaworld_func181(&arg181);
                            }
                        }

                        if ( v19->energy <= 0 || v19->bact_type == BACT_TYPES_MISSLE )
                        {
                            setState_msg arg78;
                            arg78.setFlags = 0;
                            arg78.unsetFlags = 0;
                            arg78.newStatus = BACT_STATUS_DEAD;

                            if ( v19->bact_type == BACT_TYPES_MISSLE )
                                v19->self->SetState(&arg78);
                            else
                                v19->self->SetStateInternal(&arg78);

                            if ( v19->bact_type == BACT_TYPES_ROBO )
                            {
                                if ( !v63 )
                                {
                                    yw_arg177 arg177;
                                    arg177.bact = v19;
                                    arg177.field_4 = ypabact.owner;

                                    _world->ypaworld_func177(&arg177);
                                }
                            }
                        }
                    }

                    v19 = (__NC_STACK_ypabact *)v19->next;
                }

            }
        }
    }

    ypabact.rotation.m21 = 1.9;

    vec3d az = ypabact.rotation.AxisZ();
    az.normalise();

    ypabact.rotation.SetZ(az);

    if ( fabs(ypabact.rotation.m22) <= 0.1 )
    {
        float m20 = ypabact.rotation.m20;

        NDIV_CARRY(m20);

        ypabact.rotation.m02 = sqrt( 1.0 / (POW2(ypabact.rotation.m22) / (POW2(m20)) + 1.0) );
        ypabact.rotation.m00 = -ypabact.rotation.m22 * ypabact.rotation.m02 / m20;
    }
    else
    {
        float m22 = ypabact.rotation.m22;

        NDIV_CARRY(m22);

        ypabact.rotation.m00 = sqrt( 1.0 / (POW2(ypabact.rotation.m20) / (POW2(m22)) + 1.0) );
        ypabact.rotation.m02 = -ypabact.rotation.m20 * ypabact.rotation.m00 / m22;
    }

    ypabact.rotation.m01 = 0.0;

    if ( ypabact.rotation.AxisZ().XZ().cross( ypabact.rotation.AxisX().XZ() ) > 0.0 )
    {
        ypabact.rotation.m00 = -ypabact.rotation.m00;
        ypabact.rotation.m02 = -ypabact.rotation.m02;
    }

    ypabact.rotation.SetY( ypabact.rotation.AxisZ() * ypabact.rotation.AxisX() );

    ypabact.fly_dir = -ypabact.rotation.AxisZ();

    ypabact.fly_dir_length = 200.0;
    ypabact.status_flg &= ~BACT_STFLAG_LAND;
    ypabact.bact_type = BACT_TYPES_FLYER;

    setState_msg arg78;
    arg78.newStatus = BACT_STATUS_DEAD;
    arg78.setFlags = 0;
    arg78.unsetFlags = 0;

    SetState(&arg78);

    ypabact.bact_type = BACT_TYPES_CAR;
    ypabact.energy = -10;
}

void NC_STACK_ypacar::User_layer(update_msg *arg)
{
    ypabact.airconst = ypabact.airconst_static;

    int a4 = getBACT_bactCollisions();

    ypabact.old_pos = ypabact.position;

    float v78 = arg->frameTime / 1000.0;
    vec3d oldPos = ypabact.rotation.AxisZ();

    if (ypabact.status == BACT_STATUS_DEAD)
        DeadTimeUpdate(arg);
    else if (ypabact.status == BACT_STATUS_NORMAL || ypabact.status == BACT_STATUS_IDLE)
    {
        if ( ypabact.fly_dir_length != 0.0 )
        {
            if ( ! (ypabact.status_flg & BACT_STFLAG_FIRE) )
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
            if ( ypabact.primTtype != BACT_TGT_TYPE_CELL || (ypabact.primTpos.XZ() - ypabact.position.XZ()).length() <= 800.0 )
            {
                if ( !(ypabact.status_flg & BACT_STFLAG_FIRE) )
                {
                    setState_msg arg78;
                    arg78.newStatus = BACT_STATUS_IDLE;
                    arg78.unsetFlags = 0;
                    arg78.setFlags = 0;

                    SetState(&arg78);
                }

                ypabact.status = BACT_STATUS_NORMAL;
            }
            else
            {

                if ( ypabact.status_flg & BACT_STFLAG_FIRE )
                {
                    ypabact.status = BACT_STATUS_IDLE;
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

        if ( arg->inpt->sliders_vars[3] != 0.0 )
        {
            if ( ypabact.fly_dir_length != 0.0 )
            {
                float v63 = fabs(ypabact.fly_dir_length);
                float angle = -arg->inpt->sliders_vars[3] * ypabact.maxrot * v78 * (sqrt(v63) * 0.2);

                ypabact.rotation = mat3x3::RotateY(angle) * ypabact.rotation;
            }
        }

        if ( (ypabact.fly_dir_length < 0.0 && ypabact.thraction > 0.0) || (ypabact.fly_dir_length > 0.0 && ypabact.thraction < 0.0) )
        {
            if ( fabs(ypabact.fly_dir.y) > 0.1 )
            {
                float v74 = 1.0 - (v78 * 4.0);

                if ( v74 < 0.1 )
                    v74 = 0.1;

                ypabact.fly_dir_length *= v74;
            }
        }

        float v76 = arg->inpt->sliders_vars[4];
        float v65 = fabs(arg->inpt->sliders_vars[4]);

        if ( v76 < -1.0 )
            v76 = -1.0;
        else if ( v76 > 1.0)
            v76 = 1.0;

        ypabact.thraction += ypabact.force * (v78 * 0.75) * v76;

        float v69;
        if ( arg->inpt->but_flags & 0x80000000 )
            v69 = ypabact.force * v65;
        else
            v69 = ypabact.force;

        if ( ypabact.thraction > v69 )
            ypabact.thraction = v69;

        if ( ypabact.thraction < -v69 )
            ypabact.thraction = -v69;

        if ( fabs(v76) > 0.001 )
            ypabact.status_flg |= BACT_STFLAG_MOVE;

        ypabact.gun_angle_user += v78 * arg->inpt->sliders_vars[5];

        if ( ypabact.gun_angle_user < -0.3 )
            ypabact.gun_angle_user = -0.3;

        if ( ypabact.gun_angle_user > 0.8 )
            ypabact.gun_angle_user = 0.8;

        bact_arg79 arg79;
        arg79.tgType = BACT_TGT_TYPE_DRCT;
        arg79.tgt_pos = ypabact.rotation.AxisZ();

        float corW, corH;
        GFXEngine::GFXe.getC3D()->getAspectCorrection(corW, corH, true);

        bact_arg106 arg106;
        arg106.field_0 = 5;
        arg106.field_4 = ypabact.rotation.AxisZ() - vec3d::OY(ypabact.gun_angle_user * corH);

        if ( UserTargeting(&arg106) )
        {
            arg79.tgType = BACT_TGT_TYPE_UNIT;
            arg79.target.pbact = arg106.ret_bact;
        }

        if ( arg->inpt->but_flags & 1 || arg->inpt->but_flags & 2 )
        {
            if ( _carKamikaze )
            {
                DoKamikaze();
            }
            else
            {
                arg79.weapon = ypabact.weapon;
                arg79.direction = ypabact.rotation.AxisZ() - ypabact.rotation.AxisY() * ypabact.gun_angle_user * corH;
                arg79.g_time = ypabact.clock;

                if ( ypabact.clock % 2 )
                    arg79.start_point.x = -ypabact.fire_pos.x;
                else
                    arg79.start_point.x = ypabact.fire_pos.x;

                arg79.start_point.y = ypabact.fire_pos.y;
                arg79.start_point.z = ypabact.fire_pos.z;

                arg79.flags = ((arg->inpt->but_flags & 2) != 0) | 2;

                LaunchMissile(&arg79);
            }
        }

        if ( ypabact.mgun != -1 )
        {
            if ( ypabact.status_flg & BACT_STFLAG_FIRE )
            {
                if ( !(arg->inpt->but_flags & 4) )
                {
                    setState_msg arg78;
                    arg78.setFlags = 0;
                    arg78.newStatus = BACT_STATUS_NOPE;
                    arg78.unsetFlags = BACT_STFLAG_FIRE;

                    SetState(&arg78);
                }
            }

            if ( arg->inpt->but_flags & 4 )
            {
                if ( !(ypabact.status_flg & BACT_STFLAG_FIRE) )
                {
                    setState_msg arg78;
                    arg78.setFlags = BACT_STFLAG_FIRE;
                    arg78.unsetFlags = 0;
                    arg78.newStatus = BACT_STATUS_NOPE;

                    SetState(&arg78);
                }

                bact_arg105 arg105;
                arg105.field_0 = ypabact.rotation.AxisZ() - ypabact.rotation.AxisY() * ypabact.gun_angle_user * corH;
                arg105.field_C = v78;
                arg105.field_10 = ypabact.clock;

                FireMinigun(&arg105);
            }
        }

        if ( ypabact.status_flg & BACT_STFLAG_LAND )
        {
            move_msg arg74;

            if ( arg->inpt->but_flags & 8 )
            {
                ypabact.thraction = 0;

                if ( fabs(ypabact.fly_dir_length) >= 4.0 )
                {
                    float v75 = 1.0 - v78 * 4.0;

                    if ( v75 < 0.1 )
                        v75 = 0.1;

                    ypabact.fly_dir_length *= v75 * 0.1;
                }
                else
                {
                    ypabact.status_flg &= ~BACT_STFLAG_MOVE;
                    ypabact.fly_dir_length = 0;
                }

                arg74.flag = 2;
            }
            else
            {
                arg74.flag = 0;
            }

            arg74.flag = 0;
            arg74.field_0 = v78;

            if ( ypabact.status_flg & BACT_STFLAG_MOVE )
                Move(&arg74);

            if ( a4 && CollisionWithBact(arg->frameTime) )
            {
                AlignVehicleUser(v78, oldPos);
            }
            else
            {
                ypaworld_arg136 arg136;

                arg136.stPos = ypabact.old_pos;
                arg136.vect = ypabact.position - ypabact.old_pos;
                arg136.flags = 0;

                _world->ypaworld_func136(&arg136);

                if ( arg136.isect )
                {

                    ypabact.position = ypabact.old_pos;

                    ypabact.fly_dir_length = 0;
                    ypabact.thraction = 0;
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
        v5 = ypabact.viewer_overeof;
    else
        v5 = ypabact.overeof;

    ypaworld_arg136 arg136;
    arg136.stPos = ypabact.rotation.AxisY() * v5 + ypabact.position - vec3d::OY(50.0);
    arg136.vect = vec3d::OY( ypabact.overeof + 110.0 );
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

    vec3d vaxis = ypabact.rotation.AxisY() * normal;

    if ( vaxis.normalise() > 0.0 )
    {
        float v56 = clp_acos( ypabact.rotation.AxisY().dot( normal ) );
        float v45 = ypabact.maxrot * 2.0 * dtime;

        if ( v56 > v45 )
            v56 = v45;

        if ( fabs(v56) < 0.01 )
            v56 = 0.0;

        ypabact.rotation *= mat3x3::AxisAngle(vaxis, v56);
    }

    ypabact.position = arg136.isectPos - ypabact.rotation.AxisY() * v5;

    return ALIGN_DONE;
}

vec3d NC_STACK_ypacar::CarTip(float dtime, const vec3d &oldDir, vec3d rot)
{
    vec2d tmp = ypabact.rotation.AxisZ().XZ();
    vec2d varg = oldDir.XZ();

    int a4 = getBACT_viewer();
    float v73;

    if ( a4 )
        v73 = -0.5;
    else
        v73 = -0.2;

    float v78 = ypabact.thraction - ypabact.airconst * ypabact.fly_dir_length;

    if ( fabs(v78) > 0.1 )
    {
        float v8 = v73 * v78 / ypabact.force;

        rot = mat3x3::AxisAngle(ypabact.rotation.AxisX(), v8).Transform(rot);
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
        float v77 = clp_acos(v76) * (fabs(ypabact.fly_dir_length) * 0.002 / dtime);

        if ( v77 > 0.001 )
        {
            if ( tmp.cross(varg) < 0.0 )
                v77 = -v77;

            rot = mat3x3::AxisAngle(ypabact.rotation.AxisZ(), v77).Transform(rot);
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
        v5 = ypabact.viewer_overeof;
    else
        v5 = ypabact.overeof;

    vec3d t1, t2, t3;
    float v167;

    if ( ypabact.fly_dir_length < 0.0 )
    {
        t1 = ypabact.position + (ypabact.rotation.AxisX() - ypabact.rotation.AxisZ()) * ypabact.viewer_radius * 0.7071;

        t2 = ypabact.position + ypabact.rotation.AxisZ() * ypabact.viewer_radius;

        t3 = ypabact.position + (-ypabact.rotation.AxisX() - ypabact.rotation.AxisZ()) * ypabact.viewer_radius * 0.7071;

        v167 = -1.0;
    }
    else
    {
        t1 = ypabact.position + (ypabact.rotation.AxisX() + ypabact.rotation.AxisZ()) * ypabact.viewer_radius * 0.7071;

        t2 = ypabact.position - ypabact.rotation.AxisZ() * ypabact.viewer_radius;

        t3 = ypabact.position + (-ypabact.rotation.AxisX() + ypabact.rotation.AxisZ()) * ypabact.viewer_radius * 0.7071;

        v167 = 1.0;
    }

    t1 += vec3d::OY(ypabact.viewer_overeof);
    t2 += vec3d::OY(ypabact.viewer_overeof);
    t3 += vec3d::OY(ypabact.viewer_overeof);

    ypaworld_arg136 arg136, arg136_1, arg136_2;

    arg136.stPos = ypabact.position - vec3d::OY(v162 * ypabact.viewer_radius);
    arg136.vect = (t1 - arg136.stPos) * v166;
    arg136.flags = 0;

    arg136_1.stPos = ypabact.position - vec3d::OY(v162 * ypabact.viewer_radius);
    arg136_1.vect = (t2 - arg136_1.stPos) * v166;
    arg136_1.flags = 0;

    arg136_2.stPos = ypabact.position - vec3d::OY(v162 * ypabact.viewer_radius);
    arg136_2.vect = (t3 - arg136_2.stPos) * v166;
    arg136_2.flags = 0;

    _world->ypaworld_func136(&arg136);

    if ( arg136.isect )
    {
        UAskeleton::Polygon *v48 = &arg136.skel->polygons[ arg136.polyID ];

        if ( fabs(v48->B) < 0.6 )
        {
            float v176 = ypabact.rotation.AxisZ().dot( v48->Normal() ) * v167;

            if ( v176 <= 0.0 )
            {
                ypabact.thraction = 0;
            }
            else
            {
                if ( v176 >= 0.82 )
                {
                    ypabact.thraction = 0;
                    ypabact.fly_dir_length = 0;
                    ypabact.position = ypabact.old_pos;
                }
                else
                {
                    ypabact.position.x = ypabact.old_pos.x - v48->A * 10.0;
                    ypabact.position.z = ypabact.old_pos.z - v48->C * 10.0;
                }

                if ( ypabact.fly_dir_length > 2.333333333333334 )
                {
                    SFXEngine::SFXe.startSound(&ypabact.soundcarrier, 5);

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

        arg136.isectPos = ypabact.position + arg136.vect - vec3d::OY(v162 * ypabact.viewer_radius);

        v158 = 1;
    }

    _world->ypaworld_func136(&arg136_1);

    if ( arg136_1.isect )
    {
        UAskeleton::Polygon *v54 = &arg136_1.skel->polygons[ arg136_1.polyID ];

        if ( fabs(v54->B) < 0.6 )
        {
            float v182 = ypabact.rotation.AxisZ().dot( v54->Normal() ) * v167;

            if ( v182 <= 0.0 )
            {
                ypabact.thraction = 0;
            }
            else
            {
                if ( v182 >= 0.82 )
                {
                    ypabact.fly_dir_length = 0;
                    ypabact.position = ypabact.old_pos;
                    ypabact.thraction = 0;
                }
                else
                {
                    ypabact.position.x = ypabact.old_pos.x - v54->A * 10.0;
                    ypabact.position.z = ypabact.old_pos.z - v54->C * 10.0;
                }

                if ( ypabact.fly_dir_length > 2.333333333333334 )
                {
                    SFXEngine::SFXe.startSound(&ypabact.soundcarrier, 5);

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

        arg136_1.isectPos = ypabact.position + arg136_1.vect - vec3d::OY(v162 * ypabact.viewer_radius);

        v160 = 1;
    }

    _world->ypaworld_func136(&arg136_2);

    if ( arg136_2.isect )
    {
        UAskeleton::Polygon *v54 = &arg136_2.skel->polygons[ arg136_2.polyID ];

        if ( fabs(v54->B) < 0.6 )
        {
            ypabact.position += ypabact.fly_dir * ypabact.fly_dir_length * dtime * 6.0;

            ypabact.thraction = 0;

            if ( ypabact.fly_dir_length < -2.333333333333334 )
            {
                SFXEngine::SFXe.startSound(&ypabact.soundcarrier, 5);

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
        arg136_2.isectPos = ypabact.position + arg136_2.vect - vec3d::OY(v162 * ypabact.viewer_radius);

        v161 = 1;
    }

    if ( a4 )
    {
        if ( v167 > 0.0 )
        {
            ypaworld_arg136 arg136_3;
            arg136_3.stPos = ypabact.position;

            arg136_3.vect = ypabact.rotation.AxisZ() * ypabact.viewer_radius;

            arg136_3.flags = 0;

            _world->ypaworld_func136(&arg136_3);

            if ( arg136_3.isect )
            {
                if ( arg136_3.skel->polygons[ arg136_3.polyID ].B < 0.6 )
                {
                    if ( ypabact.fly_dir_length > 2.333333333333334 )
                    {
                        SFXEngine::SFXe.startSound(&ypabact.soundcarrier, 5);

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

                    ypabact.thraction = 0;
                    ypabact.fly_dir_length = 0;
                    ypabact.position = ypabact.old_pos;
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


        float v72 = fabs(ypabact.fly_dir_length);

        vec3d vaxis = ypabact.rotation.AxisY() * tmp3;

        float v173;

        if ( v72 >= 0.5 )
            v173 = 0.007;
        else
            v173 = 0.01;

        if ( vaxis.normalise() > 0.0 )
        {
            float angle = clp_acos( ypabact.rotation.AxisY().dot( tmp3 ) );
            float maxrot = ypabact.maxrot * 2.0 * dtime;

            if ( angle > maxrot )
                angle = maxrot;

            if ( fabs(angle) > v173 )
                ypabact.rotation *= mat3x3::AxisAngle(vaxis, angle);
        }
    }

    if ( !arg136.isect && !arg136_1.isect && !arg136_2.isect )
    {
        ypabact.status_flg &= ~BACT_STFLAG_LAND;
        return 0;
    }

    if ( !v158 && !v160 && !v161 )
    {
        ypabact.position.y = (arg136.isectPos.y + arg136_1.isectPos.y + arg136_2.isectPos.y) * 0.33333334 - v5;
        return 0;
    }

    float v90 = ypabact.viewer_overeof * v166 * 0.8;

    ypaworld_arg136 arg136_4;
    arg136_4.stPos = ypabact.position - vec3d::OY(v90);
    arg136_4.vect = vec3d::OY(v90 * 2.0);
    arg136_4.flags = 0;

    _world->ypaworld_func136(&arg136_4);

    if ( arg136_4.isect && (!arg136_4.isect || arg136_4.skel->polygons[ arg136_4.polyID ].B >= 0.6) )
    {
        ypabact.position.y = arg136_4.isectPos.y - v5;
        return 0;
    }

    vec3d tf(0.0, 0.0, 0.0);

    ypabact.status_flg &= ~BACT_STFLAG_LAND;

    if ( v158 )
        tf += ypabact.rotation.AxisZ() * v167 - ypabact.rotation.AxisX();

    if ( v160 )
        tf += ypabact.rotation.AxisZ() * v167 + ypabact.rotation.AxisX();

    if ( v161 )
        tf -= ypabact.rotation.AxisZ() * v167;

    ypabact.position += tf * (dtime * 400.0);

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


size_t NC_STACK_ypacar::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( *(IDVList *)data );
    case 1:
        return (size_t)func1();
    case 2:
        return func2( *(IDVList *)data );
    case 3:
        return func3( *(IDVList *)data );
    case 71:
        User_layer( (update_msg *)data );
        return 1;
    default:
        break;
    }
    return NC_STACK_ypatank::compatcall(method_id, data);
}
