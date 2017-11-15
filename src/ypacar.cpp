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


const NewClassDescr NC_STACK_ypacar::description("ypacar.class", &newinstance);


size_t NC_STACK_ypacar::func0(IDVList *stak)
{
    if ( !NC_STACK_ypatank::func0(stak) )
        return 0;

    stack__ypacar.bact_internal = &ypabact;

    ypabact.bact_type = BACT_TYPES_CAR;

    if (stak)
    {
        for(IDVList::iterator it = stak->begin(); it != stak->end(); it++)
        {
            IDVPair &val = it->second;

            if ( !val.skip() )
            {
                switch (val.id)
                {
                case BACT_ATT_WORLD:
                    stack__ypacar.ywo = (NC_STACK_ypaworld *)val.value.p_data;
                    stack__ypacar.yw = &stack__ypacar.ywo->stack__ypaworld;
                    break;

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
    }

    return 1;
}

size_t NC_STACK_ypacar::func1()
{
    return NC_STACK_ypatank::func1();
}

size_t NC_STACK_ypacar::func2(IDVList *stak)
{
    NC_STACK_ypatank::func2(stak);

    if (stak)
    {
        for(IDVList::iterator it = stak->begin(); it != stak->end(); it++)
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
    }

    return 1;
}

size_t NC_STACK_ypacar::func3(IDVList *stak)
{
    NC_STACK_ypatank::func3(stak);

    if (stak)
    {
        for(IDVList::iterator it = stak->begin(); it != stak->end(); it++)
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
    }

    return 1;
}

void ypacar_func71__sub0(NC_STACK_ypacar *caro)
{
    __NC_STACK_ypacar *car = &caro->stack__ypacar;
    __NC_STACK_ypabact *bact = &caro->ypabact;

    float v53;

    if ( car->field_10 >= 10000 )
        v53 = log(10000.0 / car->field_10) * -428.5714285714286;
    else
        v53 = 1.0;

    int v3 = (v53 / 300.0);

    for (int i = 0; i < v3; i++)
    {
        float v4 = sqrt( POW2(i) + POW2(v3) );

        for (int j = 0; j <= v4; j++)
        {
            yw_arg129 arg120;
            arg120.field_10 = exp( sqrt( POW2(i) + POW2(j) ) * (1200.0 * -2.8) / 1200.0 ) * car->field_10;
            arg120.pos = vec3d(i, 0, j) * 300.0 + bact->position.X0Z();
            arg120.unit = bact;

            if ( arg120.pos.x > 1200.0  &&  arg120.pos.x < bact->wrldX - 1200.0  &&  arg120.pos.z < -1200.0  &&  arg120.pos.z > bact->wrldY + 1200.0 )
                caro->ChangeSectorEnergy(&arg120);

            if ( i )
            {
                arg120.pos.x = bact->position.x - i * 300.0;
                arg120.pos.z = bact->position.z + j * 300.0;

                if ( arg120.pos.x > 1200.0 )
                {
                    if ( arg120.pos.x < bact->wrldX - 1200.0 && arg120.pos.z < -1200.0 && arg120.pos.z > bact->wrldY + 1200.0 )
                        caro->ChangeSectorEnergy(&arg120);
                }
            }

            if ( j )
            {
                arg120.pos.x = bact->position.x + i * 300.0;
                arg120.pos.z = bact->position.z - j * 300.0;

                if ( arg120.pos.x > 1200.0 )
                {
                    if ( arg120.pos.x < bact->wrldX - 1200.0  &&  arg120.pos.z < -1200.0  &&  arg120.pos.z > bact->wrldY + 1200.0 )
                        caro->ChangeSectorEnergy(&arg120);
                }
            }

            if ( i && j )
            {
                arg120.pos.x = bact->position.x - i * 300.0;
                arg120.pos.z = bact->position.z - j * 300.0;

                if ( arg120.pos.x > 1200.0 )
                {
                    if ( arg120.pos.x < bact->wrldX - 1200.0  &&  arg120.pos.z < -1200.0  &&  arg120.pos.z > bact->wrldY + 1200.0 )
                        caro->ChangeSectorEnergy(&arg120);
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
            int v16 = i + bact->sectX;
            int v18 = j + bact->sectY;

            if ( v16 > 0 && v16 < bact->secMaxX - 1  &&  v18 > 0 && v18 < bact->secMaxY - 1 )
            {
                __NC_STACK_ypabact *v19 = (__NC_STACK_ypabact *)bact->pSector[i + j * bact->secMaxX].units_list.head;

                while ( v19->next )
                {
                    int v63 = 0;

                    if ( v19 != bact )
                    {
                        if ( v19->bact_type != BACT_TYPES_MISSLE )
                        {
                            vec3d tmp = bact->position - v19->position;

                            int v26 = exp(tmp.length() * -2.8 / 1200.0) * car->field_10;
                            int v67 = ((1.0 - (float)v19->shield * 0.01) * (float)v26);

                            v19->energy -= v67;

                            if ( car->yw->isNetGame )
                            {
                                v63 = 1;

                                uamessage_vhclEnergy veMsg;
                                veMsg.msgID = UAMSG_VHCLENERGY;
                                veMsg.tstamp = car->yw->timeStamp;
                                veMsg.owner = v19->owner;
                                veMsg.id = v19->gid;
                                veMsg.energy = -v67;

                                yw_arg181 arg181;
                                arg181.recvID = 0;
                                arg181.recvFlags = 2;
                                arg181.senderFlags = 1;
                                arg181.dataSize = sizeof(veMsg);
                                arg181.senderID = car->yw->GameShell->callSIGN;
                                arg181.garant = 1;
                                arg181.data = &veMsg;

                                car->ywo->ypaworld_func181(&arg181);
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
                                    arg177.field_4 = bact->owner;

                                    car->ywo->ypaworld_func177(&arg177);
                                }
                            }
                        }
                    }

                    v19 = (__NC_STACK_ypabact *)v19->next;
                }

            }
        }
    }

    bact->rotation.m21 = 1.9;

    vec3d az = bact->rotation.AxisZ();
    az.normalise();

    bact->rotation.SetZ(az);

    if ( fabs(bact->rotation.m22) <= 0.1 )
    {
        float m20 = bact->rotation.m20;

        NDIV_CARRY(m20);

        bact->rotation.m02 = sqrt( 1.0 / (POW2(bact->rotation.m22) / (POW2(m20)) + 1.0) );
        bact->rotation.m00 = -bact->rotation.m22 * bact->rotation.m02 / m20;
    }
    else
    {
        float m22 = bact->rotation.m22;

        NDIV_CARRY(m22);

        bact->rotation.m00 = sqrt( 1.0 / (POW2(bact->rotation.m20) / (POW2(m22)) + 1.0) );
        bact->rotation.m02 = -bact->rotation.m20 * bact->rotation.m00 / m22;
    }

    bact->rotation.m01 = 0.0;

    if ( bact->rotation.AxisZ().XZ().cross( bact->rotation.AxisX().XZ() ) > 0.0 )
    {
        bact->rotation.m00 = -bact->rotation.m00;
        bact->rotation.m02 = -bact->rotation.m02;
    }

    bact->rotation.SetY( bact->rotation.AxisZ() * bact->rotation.AxisX() );

    bact->fly_dir = -bact->rotation.AxisZ();

    bact->fly_dir_length = 200.0;
    bact->status_flg &= ~BACT_STFLAG_LAND;
    bact->bact_type = BACT_TYPES_FLYER;

    setState_msg arg78;
    arg78.newStatus = BACT_STATUS_DEAD;
    arg78.setFlags = 0;
    arg78.unsetFlags = 0;

    caro->SetState(&arg78);

    bact->bact_type = BACT_TYPES_CAR;
    bact->energy = -10;
}

void NC_STACK_ypacar::User_layer(update_msg *arg)
{
    __NC_STACK_ypacar *car = &stack__ypacar;
    __NC_STACK_ypabact *bact = &ypabact;

    bact->airconst = bact->airconst_static;

    int a4 = getBACT_bactCollisions();

    bact->old_pos = bact->position;

    float v78 = arg->frameTime / 1000.0;

    tank_arg129 arg129;
    arg129.field_0 = v78;
    arg129.field_4 = bact->rotation.AxisZ();

    if (bact->status == BACT_STATUS_DEAD)
        DeadTimeUpdate(arg);
    else if (bact->status == BACT_STATUS_NORMAL || bact->status == BACT_STATUS_IDLE)
    {
        if ( bact->fly_dir_length != 0.0 )
        {
            if ( ! (bact->status_flg & BACT_STFLAG_FIRE) )
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
            if ( bact->primTtype != BACT_TGT_TYPE_CELL || (bact->primTpos.XZ() - bact->position.XZ()).length() <= 800.0 )
            {
                if ( !(bact->status_flg & BACT_STFLAG_FIRE) )
                {
                    setState_msg arg78;
                    arg78.newStatus = BACT_STATUS_IDLE;
                    arg78.unsetFlags = 0;
                    arg78.setFlags = 0;

                    SetState(&arg78);
                }

                bact->status = BACT_STATUS_NORMAL;
            }
            else
            {

                if ( bact->status_flg & BACT_STFLAG_FIRE )
                {
                    bact->status = BACT_STATUS_IDLE;
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
            if ( bact->fly_dir_length != 0.0 )
            {
                float v63 = fabs(bact->fly_dir_length);
                float angle = -arg->inpt->sliders_vars[3] * bact->maxrot * v78 * (sqrt(v63) * 0.2);

                bact->rotation = mat3x3::RotateY(angle) * bact->rotation;
            }
        }

        if ( (bact->fly_dir_length < 0.0 && bact->thraction > 0.0) || (bact->fly_dir_length > 0.0 && bact->thraction < 0.0) )
        {
            if ( fabs(bact->fly_dir.y) > 0.1 )
            {
                float v74 = 1.0 - (v78 * 4.0);

                if ( v74 < 0.1 )
                    v74 = 0.1;

                bact->fly_dir_length *= v74;
            }
        }

        float v76 = arg->inpt->sliders_vars[4];
        float v65 = fabs(arg->inpt->sliders_vars[4]);

        if ( v76 < -1.0 )
            v76 = -1.0;
        else if ( v76 > 1.0)
            v76 = 1.0;

        bact->thraction += bact->force * (v78 * 0.75) * v76;

        float v69;
        if ( arg->inpt->but_flags & 0x80000000 )
            v69 = bact->force * v65;
        else
            v69 = bact->force;

        if ( bact->thraction > v69 )
            bact->thraction = v69;

        if ( bact->thraction < -v69 )
            bact->thraction = -v69;

        if ( fabs(v76) > 0.001 )
            bact->status_flg |= BACT_STFLAG_MOVE;

        bact->gun_angle_user += v78 * arg->inpt->sliders_vars[5];

        if ( bact->gun_angle_user < -0.3 )
            bact->gun_angle_user = -0.3;

        if ( bact->gun_angle_user > 0.8 )
            bact->gun_angle_user = 0.8;

        bact_arg79 arg79;
        arg79.tgType = BACT_TGT_TYPE_DRCT;
        arg79.tgt_pos = bact->rotation.AxisZ();

        float corW, corH;
        GFXEngine::GFXe.getC3D()->getAspectCorrection(corW, corH, true);

        bact_arg106 arg106;
        arg106.field_0 = 5;
        arg106.field_4 = bact->rotation.AxisZ() - vec3d::OY(bact->gun_angle_user * corH);

        if ( UserTargeting(&arg106) )
        {
            arg79.tgType = BACT_TGT_TYPE_UNIT;
            arg79.target.pbact = arg106.ret_bact;
        }

        if ( arg->inpt->but_flags & 1 || arg->inpt->but_flags & 2 )
        {
            if ( car->field_c )
            {
                ypacar_func71__sub0(this);
            }
            else
            {
                arg79.weapon = bact->weapon;
                arg79.direction = bact->rotation.AxisZ() - bact->rotation.AxisY() * bact->gun_angle_user * corH;
                arg79.g_time = bact->clock;

                if ( bact->clock % 2 )
                    arg79.start_point.x = -bact->fire_pos.x;
                else
                    arg79.start_point.x = bact->fire_pos.x;

                arg79.start_point.y = bact->fire_pos.y;
                arg79.start_point.z = bact->fire_pos.z;

                arg79.flags = ((arg->inpt->but_flags & 2) != 0) | 2;

                LaunchMissile(&arg79);
            }
        }

        if ( bact->mgun != -1 )
        {
            if ( bact->status_flg & BACT_STFLAG_FIRE )
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
                if ( !(bact->status_flg & BACT_STFLAG_FIRE) )
                {
                    setState_msg arg78;
                    arg78.setFlags = BACT_STFLAG_FIRE;
                    arg78.unsetFlags = 0;
                    arg78.newStatus = BACT_STATUS_NOPE;

                    SetState(&arg78);
                }

                bact_arg105 arg105;
                arg105.field_0 = bact->rotation.AxisZ() - bact->rotation.AxisY() * bact->gun_angle_user * corH;
                arg105.field_C = v78;
                arg105.field_10 = bact->clock;

                FireMinigun(&arg105);
            }
        }

        if ( bact->status_flg & BACT_STFLAG_LAND )
        {
            move_msg arg74;

            if ( arg->inpt->but_flags & 8 )
            {
                bact->thraction = 0;

                if ( fabs(bact->fly_dir_length) >= 4.0 )
                {
                    float v75 = 1.0 - v78 * 4.0;

                    if ( v75 < 0.1 )
                        v75 = 0.1;

                    bact->fly_dir_length *= v75 * 0.1;
                }
                else
                {
                    bact->status_flg &= ~BACT_STFLAG_MOVE;
                    bact->fly_dir_length = 0;
                }

                arg74.flag = 2;
            }
            else
            {
                arg74.flag = 0;
            }

            arg74.flag = 0;
            arg74.field_0 = v78;

            if ( bact->status_flg & BACT_STFLAG_MOVE )
                Move(&arg74);

            if ( a4 && CollisionWithBact(arg->frameTime) )
            {
                ypatank_func129(&arg129);
            }
            else
            {
                ypaworld_arg136 arg136;

                arg136.stPos = bact->old_pos;
                arg136.vect = bact->position - bact->old_pos;
                arg136.flags = 0;

                car->ywo->ypaworld_func136(&arg136);

                if ( arg136.isect )
                {

                    bact->position = bact->old_pos;

                    bact->fly_dir_length = 0;
                    bact->thraction = 0;
                }
                else
                {
                    ypatank_func129(&arg129);
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

size_t NC_STACK_ypacar::ypatank_func128(tank_arg128 *arg)
{
    __NC_STACK_ypacar *car = &stack__ypacar;
    __NC_STACK_ypabact *bact = &ypabact;

    arg->field_10 = 0;

    int a4 = getBACT_viewer();

    float v5;

    if ( a4 )
        v5 = bact->viewer_overeof;
    else
        v5 = bact->overeof;

    ypaworld_arg136 arg136;
    arg136.stPos = bact->rotation.AxisY() * v5 + bact->position - vec3d::OY(50.0);
    arg136.vect = vec3d::OY( bact->overeof + 110.0 );
    arg136.flags = 0;

    car->ywo->ypaworld_func136(&arg136);

    if ( !arg136.isect )
        return 0;

    UAskeleton::Polygon *v8 = &arg136.skel->polygons[ arg136.polyID ];

    if ( v8->B < 0.6 )
    {
        arg->field_10 |= 1;
        arg->field_14 = arg136.skel->polygons[ arg136.polyID ].Normal();

        return 0;
    }

    vec3d vaxis = bact->rotation.AxisY() * v8->Normal();

    if ( vaxis.normalise() > 0.0 )
    {
        float v56 = clp_acos( bact->rotation.AxisY().dot( v8->Normal() ) );
        float v45 = bact->maxrot * 2.0 * arg->field_0;

        if ( v56 > v45 )
            v56 = v45;

        if ( fabs(v56) < 0.01 )
            v56 = 0.0;

        bact->rotation *= mat3x3::AxisAngle(vaxis, v56);
    }

    bact->position = arg136.isectPos - bact->rotation.AxisY() * v5;

    return 1;
}

void ypacar_func129__sub0(NC_STACK_ypacar *caro, tank_arg129 *arg, vec3d &darg)
{
    //__NC_STACK_ypacar *car = &caro->stack__ypacar;
    __NC_STACK_ypabact *bact = &caro->ypabact;

    vec2d tmp = bact->rotation.AxisZ().XZ();
    vec2d varg = arg->field_4.XZ();

    int a4 = bact->self->getBACT_viewer();
    float v73;

    if ( a4 )
        v73 = -0.5;
    else
        v73 = -0.2;

    float v78 = bact->thraction - bact->airconst * bact->fly_dir_length;

    if ( fabs(v78) > 0.1 )
    {
        float v8 = v73 * v78 / bact->force;

        darg = mat3x3::AxisAngle(bact->rotation.AxisX(), v8).Transform(darg);
    }

    float v76 = tmp.dot( varg );

    float tmpsq = tmp.length();

    NDIV_CARRY(tmpsq);

    v76 = v76 / tmpsq;

    tmpsq = varg.length();

    NDIV_CARRY(tmpsq);

    v76 = v76 / tmpsq;


    if (arg->field_0 != 0.0)
    {
        float v77 = clp_acos(v76) * (fabs(bact->fly_dir_length) * 0.002 / arg->field_0);

        if ( v77 > 0.001 )
        {
            if ( tmp.cross(varg) < 0.0 )
                v77 = -v77;

            darg = mat3x3::AxisAngle(bact->rotation.AxisZ(), v77).Transform(darg);
        }
    }
}

size_t NC_STACK_ypacar::ypatank_func129(tank_arg129 *arg)
{
    __NC_STACK_ypacar *car = &stack__ypacar;
    __NC_STACK_ypabact *bact = &ypabact;

    float v162 = 1.73;
    float v166 = 1.7;
    int v158 = 0;
    int v160 = 0;
    int v161 = 0;

    int a4 = getBACT_viewer();
    int v149 = getBACT_inputting();

    float v5;

    if ( a4 )
        v5 = bact->viewer_overeof;
    else
        v5 = bact->overeof;

    vec3d t1, t2, t3;
    float v167;

    if ( bact->fly_dir_length < 0.0 )
    {
        t1 = bact->position + (bact->rotation.AxisX() - bact->rotation.AxisZ()) * bact->viewer_radius * 0.7071;

        t2 = bact->position + bact->rotation.AxisZ() * bact->viewer_radius;

        t3 = bact->position + (-bact->rotation.AxisX() - bact->rotation.AxisZ()) * bact->viewer_radius * 0.7071;

        v167 = -1.0;
    }
    else
    {
        t1 = bact->position + (bact->rotation.AxisX() + bact->rotation.AxisZ()) * bact->viewer_radius * 0.7071;

        t2 = bact->position - bact->rotation.AxisZ() * bact->viewer_radius;

        t3 = bact->position + (-bact->rotation.AxisX() + bact->rotation.AxisZ()) * bact->viewer_radius * 0.7071;

        v167 = 1.0;
    }

    t1 += vec3d::OY(bact->viewer_overeof);
    t2 += vec3d::OY(bact->viewer_overeof);
    t3 += vec3d::OY(bact->viewer_overeof);

    ypaworld_arg136 arg136, arg136_1, arg136_2;

    arg136.stPos = bact->position - vec3d::OY(v162 * bact->viewer_radius);
    arg136.vect = (t1 - arg136.stPos) * v166;
    arg136.flags = 0;

    arg136_1.stPos = bact->position - vec3d::OY(v162 * bact->viewer_radius);
    arg136_1.vect = (t2 - arg136_1.stPos) * v166;
    arg136_1.flags = 0;

    arg136_2.stPos = bact->position - vec3d::OY(v162 * bact->viewer_radius);
    arg136_2.vect = (t3 - arg136_2.stPos) * v166;
    arg136_2.flags = 0;

    car->ywo->ypaworld_func136(&arg136);

    if ( arg136.isect )
    {
        UAskeleton::Polygon *v48 = &arg136.skel->polygons[ arg136.polyID ];

        if ( fabs(v48->B) < 0.6 )
        {
            float v176 = bact->rotation.AxisZ().dot( v48->Normal() ) * v167;

            if ( v176 <= 0.0 )
            {
                bact->thraction = 0;
            }
            else
            {
                if ( v176 >= 0.82 )
                {
                    bact->thraction = 0;
                    bact->fly_dir_length = 0;
                    bact->position = bact->old_pos;
                }
                else
                {
                    bact->position.x = bact->old_pos.x - v48->A * 10.0;
                    bact->position.z = bact->old_pos.z - v48->C * 10.0;
                }

                if ( bact->fly_dir_length > 2.333333333333334 )
                {
                    SFXEngine::SFXe.startSound(&bact->soundcarrier, 5);

                    if ( v149 )
                    {
                        yw_arg180 arg180;

                        arg180.field_8 = arg136.isectPos.x;
                        arg180.field_C = arg136.isectPos.z;
                        arg180.effects_type = 5;
                        arg180.field_4 = 1.0;

                        car->ywo->ypaworld_func180(&arg180);
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

        arg136.isectPos = bact->position + arg136.vect - vec3d::OY(v162 * bact->viewer_radius);

        v158 = 1;
    }

    car->ywo->ypaworld_func136(&arg136_1);

    if ( arg136_1.isect )
    {
        UAskeleton::Polygon *v54 = &arg136_1.skel->polygons[ arg136_1.polyID ];

        if ( fabs(v54->B) < 0.6 )
        {
            float v182 = bact->rotation.AxisZ().dot( v54->Normal() ) * v167;

            if ( v182 <= 0.0 )
            {
                bact->thraction = 0;
            }
            else
            {
                if ( v182 >= 0.82 )
                {
                    bact->fly_dir_length = 0;
                    bact->position = bact->old_pos;
                    bact->thraction = 0;
                }
                else
                {
                    bact->position.x = bact->old_pos.x - v54->A * 10.0;
                    bact->position.z = bact->old_pos.z - v54->C * 10.0;
                }

                if ( bact->fly_dir_length > 2.333333333333334 )
                {
                    SFXEngine::SFXe.startSound(&bact->soundcarrier, 5);

                    if ( v149 )
                    {
                        yw_arg180 arg180;
                        arg180.field_4 = 1.0;
                        arg180.field_8 = arg136_1.isectPos.x;
                        arg180.field_C = arg136_1.isectPos.z;
                        arg180.effects_type = 5;

                        car->ywo->ypaworld_func180(&arg180);
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

        arg136_1.isectPos = bact->position + arg136_1.vect - vec3d::OY(v162 * bact->viewer_radius);

        v160 = 1;
    }

    car->ywo->ypaworld_func136(&arg136_2);

    if ( arg136_2.isect )
    {
        UAskeleton::Polygon *v54 = &arg136_2.skel->polygons[ arg136_2.polyID ];

        if ( fabs(v54->B) < 0.6 )
        {
            bact->position += bact->fly_dir * bact->fly_dir_length * arg->field_0 * 6.0;

            bact->thraction = 0;

            if ( bact->fly_dir_length < -2.333333333333334 )
            {
                SFXEngine::SFXe.startSound(&bact->soundcarrier, 5);

                if ( v149 )
                {
                    yw_arg180 arg180;
                    arg180.field_4 = 1.0;
                    arg180.field_8 = arg136_2.isectPos.x;
                    arg180.field_C = arg136_2.isectPos.z;
                    arg180.effects_type = 5;

                    car->ywo->ypaworld_func180(&arg180);
                }
            }
            return 3;
        }
    }
    else
    {
        arg136_2.isectPos = bact->position + arg136_2.vect - vec3d::OY(v162 * bact->viewer_radius);

        v161 = 1;
    }

    if ( a4 )
    {
        if ( v167 > 0.0 )
        {
            ypaworld_arg136 arg136_3;
            arg136_3.stPos = bact->position;

            arg136_3.vect = bact->rotation.AxisZ() * bact->viewer_radius;

            arg136_3.flags = 0;

            car->ywo->ypaworld_func136(&arg136_3);

            if ( arg136_3.isect )
            {
                if ( arg136_3.skel->polygons[ arg136_3.polyID ].B < 0.6 )
                {
                    if ( bact->fly_dir_length > 2.333333333333334 )
                    {
                        SFXEngine::SFXe.startSound(&bact->soundcarrier, 5);

                        if ( v149 )
                        {
                            yw_arg180 arg180;
                            arg180.field_4 = 1.0;
                            arg180.field_8 = arg136_3.isectPos.x;
                            arg180.field_C = arg136_3.isectPos.z;
                            arg180.effects_type = 5;

                            car->ywo->ypaworld_func180(&arg180);
                        }
                    }

                    bact->thraction = 0;
                    bact->fly_dir_length = 0;
                    bact->position = bact->old_pos;
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
            ypacar_func129__sub0(this, arg, tmp3);


        float v72 = fabs(bact->fly_dir_length);

        vec3d vaxis = bact->rotation.AxisY() * tmp3;

        float v173;

        if ( v72 >= 0.5 )
            v173 = 0.007;
        else
            v173 = 0.01;

        if ( vaxis.normalise() > 0.0 )
        {
            float angle = clp_acos( bact->rotation.AxisY().dot( tmp3 ) );
            float maxrot = bact->maxrot * 2.0 * arg->field_0;

            if ( angle > maxrot )
                angle = maxrot;

            if ( fabs(angle) > v173 )
                bact->rotation *= mat3x3::AxisAngle(vaxis, angle);
        }
    }

    if ( !arg136.isect && !arg136_1.isect && !arg136_2.isect )
    {
        bact->status_flg &= ~BACT_STFLAG_LAND;
        return 0;
    }

    if ( !v158 && !v160 && !v161 )
    {
        bact->position.y = (arg136.isectPos.y + arg136_1.isectPos.y + arg136_2.isectPos.y) * 0.33333334 - v5;
        return 0;
    }

    float v90 = bact->viewer_overeof * v166 * 0.8;

    ypaworld_arg136 arg136_4;
    arg136_4.stPos = bact->position - vec3d::OY(v90);
    arg136_4.vect = vec3d::OY(v90 * 2.0);
    arg136_4.flags = 0;

    car->ywo->ypaworld_func136(&arg136_4);

    if ( arg136_4.isect && (!arg136_4.isect || arg136_4.skel->polygons[ arg136_4.polyID ].B >= 0.6) )
    {
        bact->position.y = arg136_4.isectPos.y - v5;
        return 0;
    }

    vec3d tf(0.0, 0.0, 0.0);

    bact->status_flg &= ~BACT_STFLAG_LAND;

    if ( v158 )
        tf += bact->rotation.AxisZ() * v167 - bact->rotation.AxisX();

    if ( v160 )
        tf += bact->rotation.AxisZ() * v167 + bact->rotation.AxisX();

    if ( v161 )
        tf -= bact->rotation.AxisZ() * v167;

    bact->position += tf * (arg->field_0 * 400.0);

    return 0;
}


void NC_STACK_ypacar::setCAR_kamikaze(int kam)
{
    if (kam)
        stack__ypacar.field_c |= 1;
    else
        stack__ypacar.field_c &= ~1;
}

void NC_STACK_ypacar::setCAR_blast(int bla)
{
    stack__ypacar.field_10 = bla;
}


int NC_STACK_ypacar::getCAR_kamikaze()
{
    if (stack__ypacar.field_c & 1)
        return 1;
    return 0;
}

int NC_STACK_ypacar::getCAR_blast()
{
    return stack__ypacar.field_10;
}


size_t NC_STACK_ypacar::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (IDVList *)data );
    case 1:
        return (size_t)func1();
    case 2:
        return func2( (IDVList *)data );
    case 3:
        return func3( (IDVList *)data );
    case 71:
        User_layer( (update_msg *)data );
        return 1;
    case 128:
        return (size_t)ypatank_func128( (tank_arg128 *)data );
    case 129:
        return (size_t)ypatank_func129( (tank_arg129 *)data );
    default:
        break;
    }
    return NC_STACK_ypatank::compatcall(method_id, data);
}
