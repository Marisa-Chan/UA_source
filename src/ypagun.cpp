#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include "yw.h"
#include "ypagun.h"
#include "yparobo.h"


const NewClassDescr NC_STACK_ypagun::description("ypagun.class", &newinstance);

size_t NC_STACK_ypagun::func0(IDVList *stak)
{
    if ( !NC_STACK_ypabact::func0(stak) )
        return 0;

    ypabact.bact_type = BACT_TYPES_GUN;

    ypagun.gunType = GUN_TYPE_REAL;
    ypagun.fireTime = 100;

    if (stak)
    {
        for(IDVList::iterator it = stak->begin(); it != stak->end(); it++)
        {
            IDVPair &val = it->second;

            if ( !val.skip() )
            {
                switch (val.id)
                {
                case GUN_ATT_SIDEANGLE:
                    setGUN_sideAngle(val.value.i_data);
                    break;

                case GUN_ATT_UPANGLE:
                    setGUN_upAngle(val.value.i_data);
                    break;

                case GUN_ATT_DOWNANGLE:
                    setGUN_downAngle(val.value.i_data);
                    break;

                case GUN_ATT_FIRETYPE:
                    setGUN_fireType(val.value.i_data);
                    break;

                case GUN_ATT_FIRETIME:
                    setGUN_fireTime(val.value.i_data);
                    break;

                case GUN_ATT_SETGROUND:
                    setGUN_setGround ( val.value.i_data );
                    break;

                case GUN_ATT_ROBOGUN:
                    setGUN_roboGun ( val.value.i_data );
                    break;

                default:
                    break;
                }
            }
        }
    }

    return 1;
}

size_t NC_STACK_ypagun::func1()
{
    return NC_STACK_ypabact::func1();
}

size_t NC_STACK_ypagun::func2(IDVList *stak)
{
    NC_STACK_ypabact::func2(stak);

    if (stak)
    {
        for(IDVList::iterator it = stak->begin(); it != stak->end(); it++)
        {
            IDVPair &val = it->second;

            if ( !val.skip() )
            {
                switch (val.id)
                {
                case GUN_ATT_SIDEANGLE:
                    setGUN_sideAngle(val.value.i_data);
                    break;

                case GUN_ATT_UPANGLE:
                    setGUN_upAngle(val.value.i_data);
                    break;

                case GUN_ATT_DOWNANGLE:
                    setGUN_downAngle(val.value.i_data);
                    break;

                case GUN_ATT_FIRETYPE:
                    setGUN_fireType(val.value.i_data);
                    break;

                case GUN_ATT_FIRETIME:
                    setGUN_fireTime(val.value.i_data);
                    break;

                case GUN_ATT_SETGROUND:
                    setGUN_setGround ( val.value.i_data );
                    break;

                case GUN_ATT_ROBOGUN:
                    setGUN_roboGun ( val.value.i_data );
                    break;

                default:
                    break;
                }
            }
        }
    }

    return 1;
}

size_t NC_STACK_ypagun::func3(IDVList *stak)
{
    NC_STACK_ypabact::func3(stak);

    if (stak)
    {
        for(IDVList::iterator it = stak->begin(); it != stak->end(); it++)
        {
            IDVPair &val = it->second;

            if ( !val.skip() )
            {
                switch (val.id)
                {
                case GUN_ATT_SIDEANGLE:
                    *(int *)val.value.p_data = getGUN_sideAngle();
                    break;

                case GUN_ATT_UPANGLE:
                    *(int *)val.value.p_data = getGUN_upAngle();
                    break;

                case GUN_ATT_DOWNANGLE:
                    *(int *)val.value.p_data = getGUN_downAngle();
                    break;

                case GUN_ATT_FIRETYPE:
                    *(int *)val.value.p_data = getGUN_fireType();
                    break;

                case GUN_ATT_FIRETIME:
                    *(int *)val.value.p_data = getGUN_fireTime();
                    break;

                case GUN_ATT_SETGROUND:
                    *(int *)val.value.p_data = getGUN_setGround();
                    break;

                case GUN_ATT_ROBOGUN:
                    *(int *)val.value.p_data = getGUN_roboGun();
                    break;

                default:
                    break;
                }
            }
        }
    }

    return 1;
}


bool NC_STACK_ypagun::CheckPedestal()
{
    ypaworld_arg136 arg136;
    arg136.stPos.x = ypabact.position.x;
    arg136.stPos.y = ypabact.position.y;
    arg136.stPos.z = ypabact.position.z;
    arg136.vect.x = 0;

    if ( ypagun.gunFlags & GUN_FLAGS_FALLDOWN )
        arg136.vect.y = -ypabact.height;
    else
        arg136.vect.y = ypabact.height;

    arg136.vect.z = 0;
    arg136.flags = 0;

    ypabact.ywo->ypaworld_func136(&arg136);

    return arg136.isect != 0;
}

void NC_STACK_ypagun::AI_layer3(update_msg *arg)
{
    float fTime = arg->frameTime / 1000.0;

    switch ( ypabact.status )
    {
    case BACT_STATUS_NORMAL:
    case BACT_STATUS_IDLE:
    {
        if ( !(ypagun.gunFlags & GUN_FLAGS_ROBO) )
        {
            if ( ypabact.clock - ypagun.downTime > 800 )
            {
                ypagun.downTime = ypabact.clock;

                if ( !CheckPedestal() )
                {
                    ypabact.energy = -10;
                    break;
                }
            }
        }

        if ( ypagun.gunType == GUN_TYPE_REAL && ypagun.fireCount > 0 )
        {
            ypagun.fireCount -= arg->frameTime;

            if ( ypagun.fireCount <= 0 )
            {
                ypagun.fireCount = 0;

                setState_msg arg78;
                arg78.unsetFlags = BACT_STFLAG_FIRE;
                arg78.newStatus = BACT_STATUS_NOPE;
                arg78.setFlags = 0;

                SetState(&arg78);
            }
        }

        if ( ypabact.secndTtype != BACT_TGT_TYPE_UNIT )
        {
            if ( !ypabact.secndTtype && ypagun.gunType == GUN_TYPE_PROTO )
            {
                if ( ypabact.status_flg & BACT_STFLAG_FIRE )
                {
                    setState_msg arg78;
                    arg78.unsetFlags = BACT_STFLAG_FIRE;
                    arg78.newStatus = BACT_STATUS_NOPE;
                    arg78.setFlags = 0;

                    SetState(&arg78);
                }
            }
            break;
        }

        vec3d vTgt = ypabact.secndT.pbact->position - ypabact.position;

        float dist = vTgt.length();

        if ( dist <= 0.001 )
            break;

        vTgt /= dist;

        //vec3d norm = ypagun.basis * ypagun.rott;

        float xzAngle = 0.0; // Current
        float xzWanted = 0.0; // Wanted

        // local X coordinate axis. Used for calculate rotation on XZ-plane
        // because always lie on XZ-plane
        vec3d lx = ypagun.rott * ypagun.basis;

        // Calculate projection gun rotation X-axis onto XZ basis plane
        vec2d xzRot( ypabact.rotation.AxisX().dot(lx),
                     ypabact.rotation.AxisX().dot(ypagun.basis));

        // Normolise for unit circle.
        // If it's too short - X-rotation-axis near -/+rott axis,
        if (xzRot.normalise() > 0.001)
            xzAngle = xzRot.xyAngle();

        vec2d xzWant( vTgt.dot(ypagun.basis),
                      vTgt.dot(-lx) );

        if ( xzWant.normalise() > 0.001 )
            xzWanted = xzWant.xyAngle();
        else
            xzWanted = xzAngle;

        if ( ypagun.maxSide <= 3.1 )
        {
            if ( xzWanted < -ypagun.maxSide )
                xzWanted = -ypagun.maxSide;

            if ( xzWanted > ypagun.maxSide )
                xzWanted = ypagun.maxSide;
        }

        float xz_delta = xzWanted - xzAngle;

        if ( ypagun.maxSide > 3.1 )
        {
            if ( fabs(xz_delta) > C_PI )
            {
                if ( xz_delta < -C_PI )
                    xz_delta += C_2PI;

                if ( xz_delta > C_PI )
                    xz_delta -= C_2PI;
            }
        }

        float MaxRot = ypabact.maxrot * fTime;

        if ( xz_delta >= 0.0 )
        {
            if ( xz_delta > MaxRot )
                xz_delta = MaxRot;
        }
        else
        {
            if ( xz_delta < -MaxRot )
                xz_delta = -MaxRot;
        }

        if ( fabs(xz_delta) > 0.001 )
            ypabact.rotation *= mat3x3(ypagun.rott, xz_delta);

        vec3d invRed = -ypagun.rott;

        float y_angle = clp_asin( invRed.dot(ypabact.rotation.AxisZ()) );

        float y_want = clp_asin( invRed.dot( vTgt ) );

        if ( y_want > ypagun.maxUp )
            y_want = ypagun.maxUp;

        if ( y_want < -ypagun.maxDown )
            y_want = -ypagun.maxDown;

        float y_delta = y_want - y_angle;

        if ( y_delta > MaxRot )
            y_delta = MaxRot;
        else if ( y_delta < -MaxRot )
            y_delta = -MaxRot;

        if ( fabs(y_delta) > 0.001 )
            ypabact.rotation = mat3x3::RotateX(y_delta * 0.3) * ypabact.rotation;

        bact_arg75 arg75;
        arg75.fperiod = fTime;
        arg75.g_time = ypabact.clock;
        arg75.target.pbact = ypabact.secndT.pbact;

        FightWithBact(&arg75);

        if ( !TestTargetSector(ypabact.secndT.pbact) )
        {
            setTarget_msg arg67;
            arg67.tgt_type = BACT_TGT_TYPE_NONE;
            arg67.priority = 1;

            SetTarget(&arg67);

            if ( ypagun.gunType == GUN_TYPE_PROTO )
            {
                setState_msg arg78;
                arg78.unsetFlags = BACT_STFLAG_FIRE;
                arg78.newStatus = BACT_STATUS_NOPE;
                arg78.setFlags = 0;

                SetState(&arg78);
            }
        }
    }
    break;

    case BACT_STATUS_DEAD:
        DeadTimeUpdate(arg);
        break;

    case BACT_STATUS_CREATE:
        ypabact.scale_time -= arg->frameTime;

        if ( ypabact.scale_time <= 0 )
        {
            setState_msg arg78;
            arg78.newStatus = BACT_STATUS_NORMAL;
            arg78.setFlags = 0;
            arg78.unsetFlags = 0;

            SetState(&arg78);
        }
        break;

    default:
        break;
    }
}

void NC_STACK_ypagun::User_layer(update_msg *arg)
{
    float fTime = arg->frameTime / 1000.0;

    if ( ypabact.status == BACT_STATUS_NORMAL )
    {
        if ( ypagun.gunFlags & GUN_FLAGS_ROBO || CheckPedestal() )
        {
            if ( ypabact.viewer_position.length() >= 3.0 )
            {
                float v33 = arg->frameTime + 50.0;

                ypabact.viewer_position *= 50.0 / v33;
            }
            else
            {
                ypabact.viewer_position = 0;
            }

            if ( ypabact.status_flg & BACT_STFLAG_FIRE )
            {
                if ( !(arg->inpt->but_flags & 1) && !(arg->inpt->but_flags & 2) )
                {
                    setState_msg arg78;
                    arg78.setFlags = 0;
                    arg78.newStatus = BACT_STATUS_NOPE;
                    arg78.unsetFlags = BACT_STFLAG_FIRE;

                    SetState(&arg78);
                }
            }

            bact_arg79 arg79;
            arg79.tgType = BACT_TGT_TYPE_CELL;
            arg79.tgt_pos = ypabact.position + ypabact.rotation.AxisZ() * (1200.0 * 3.0);

            bact_arg106 arg106;
            arg106.field_4 = ypabact.rotation.AxisZ();
            arg106.field_0 = 5;

            if ( UserTargeting(&arg106) )
            {
                arg79.target.pbact = arg106.ret_bact;
                arg79.tgType = BACT_TGT_TYPE_UNIT;
            }

            if ( arg->inpt->but_flags & 1 || arg->inpt->but_flags & 2 )
            {
                if ( ypagun.gunType == GUN_TYPE_REAL )
                {
                    arg79.weapon = ypabact.weapon;
                    arg79.direction = ypabact.rotation.AxisZ();
                    arg79.g_time = ypabact.clock;
                    arg79.start_point = ypabact.fire_pos;
                    arg79.flags = ((arg->inpt->but_flags & 2) != 0) | 2;

                    if ( LaunchMissile(&arg79) )
                    {
                        ypabact.viewer_position = vec3d(0, 0, -25.0);
                    }
                }
                else if ( ypagun.gunType == GUN_TYPE_PROTO )
                {
//            if ( ~(ypabact.field_3D6 & 0x100) ) CHECK IT
                    if ( !(ypabact.status_flg & BACT_STFLAG_FIRE) )
                    {
                        setState_msg arg78;
                        arg78.unsetFlags = 0;
                        arg78.newStatus = BACT_STATUS_NOPE;
                        arg78.setFlags = BACT_STFLAG_FIRE;

                        SetState(&arg78);
                    }

                    bact_arg105 arg105;
                    arg105.field_0 = ypabact.rotation.AxisZ();
                    arg105.field_C = fTime;
                    arg105.field_10 = ypabact.clock;

                    FireMinigun(&arg105);

                    if ( ypagun.gunFlags & GUN_FLAGS_SHOT )
                    {
                        ypagun.gunFlags &= ~GUN_FLAGS_SHOT;
                    }
                    else
                    {
                        ypagun.gunFlags |= GUN_FLAGS_SHOT;

                        ypabact.viewer_position = vec3d(0, -20.0, -30.0);
                    }
                }
            }

            float yRot = arg->inpt->sliders_vars[0] * ypabact.maxrot * fTime;

            if ( fabs( yRot ) > 0.001 )
            {
                vec3d tmp = ypagun.rott * ypagun.basis;

                float yAngle = clp_acos( tmp.dot( ypabact.rotation.AxisX() ) );

                if ( tmp.x * ypabact.rotation.m02 - tmp.z * ypabact.rotation.m00 > 0.0 )
                    yAngle = -yAngle;

                if ( yAngle + yRot < -ypagun.maxSide )
                    yRot = -ypagun.maxSide - yAngle;

                if ( yAngle + yRot > ypagun.maxSide )
                    yRot = ypagun.maxSide - yAngle;

                ypabact.rotation *= mat3x3(ypagun.rott, yRot, MAT_FLAG_INV_SIN);
            }

            float xRot = arg->inpt->sliders_vars[1] * ypabact.maxrot * fTime;

            if ( fabs(xRot) > 0.001 )
            {
                vec3d tmp = -ypagun.rott;
                float xAngle = clp_asin( tmp.dot( ypabact.rotation.AxisZ() ) );

                if ( xAngle + xRot < -ypagun.maxDown )
                    xRot = -ypagun.maxDown - xAngle;

                if ( xAngle + xRot > ypagun.maxUp )
                    xRot = ypagun.maxUp - xAngle;

                ypabact.rotation = mat3x3::RotateX(xRot) * ypabact.rotation;
            }

            ypabact.viewer_rotation = ypabact.rotation;
        }
        else
        {
            ypabact.energy = -10;
        }
    }
    else if ( ypabact.status == BACT_STATUS_DEAD )
        DeadTimeUpdate(arg);
}

void NC_STACK_ypagun::FightWithBact(bact_arg75 *arg)
{
    vec3d vTgt = arg->target.pbact->position - ypabact.position;

    float dist = vTgt.length();

    if ( dist < 0.001 )
        return;

    vTgt /= dist;

    if ( dist <= 2400.0 && vTgt.dot( ypabact.rotation.AxisZ() ) >= 0.95 )
    {
        if ( ypagun.gunType == GUN_TYPE_REAL )
        {
            bact_arg79 arg79;
            arg79.direction = vTgt;
            arg79.start_point = ypabact.fire_pos;
            arg79.tgType = BACT_TGT_TYPE_UNIT;
            arg79.target.pbact = arg->target.pbact;
            arg79.weapon = ypabact.weapon;
            arg79.g_time = arg->g_time;
            arg79.flags = 0;

            if ( LaunchMissile(&arg79) )
            {
                int a5 = getBACT_inputting();

                if ( !a5 )
                {
                    /*bact_node *v15 = (bact_node *)ypabact.list3.tailpred; //Check it,
                    if ( v15->next ) //on null list it will work too */

                    //must be like:
                    bact_node *v15 = (bact_node *)ypabact.missiles_list.tailpred; //Check it,
                    if ( v15 != (bact_node *)&ypabact.missiles_list )
                    {
                        NC_STACK_ypamissile *miss = dynamic_cast<NC_STACK_ypamissile *>(v15->bacto);
                        miss->setMISS_ignoreBuilds(1);
                    }
                }
                ypagun.fireCount = ypagun.fireTime;

                setState_msg arg78;
                arg78.unsetFlags = 0;
                arg78.newStatus = BACT_STATUS_NOPE;
                arg78.setFlags = BACT_STFLAG_FIRE;

                SetState(&arg78);
            }
        }
        else if ( ypagun.gunType == GUN_TYPE_PROTO )
        {
            if ( !(ypabact.status_flg & BACT_STFLAG_FIRE) )
            {
                setState_msg arg78;
                arg78.unsetFlags = 0;
                arg78.newStatus = BACT_STATUS_NOPE;
                arg78.setFlags = BACT_STFLAG_FIRE;

                SetState(&arg78);
            }

            bact_arg105 arg105;
            arg105.field_C = arg->fperiod;
            arg105.field_10 = arg->g_time;
            arg105.field_0 = ypabact.rotation.AxisZ();

            FireMinigun(&arg105);
        }
    }
    else if ( ypagun.gunType == GUN_TYPE_PROTO )
    {
        setState_msg arg78;
        arg78.setFlags = 0;
        arg78.newStatus = BACT_STATUS_NOPE;
        arg78.unsetFlags = BACT_STFLAG_FIRE;

        SetState(&arg78);
    }
}

void NC_STACK_ypagun::Die()
{
    if ( !(ypabact.status_flg & BACT_STFLAG_DEATH1) )
    {
        NC_STACK_ypabact::Die();

        int v6 = 1;
        HandleVisChildrens(&v6);

        if ( ypagun.gunFlags & GUN_FLAGS_ROBO )
        {
            roboGun *hostGun = ypabact.host_station->getROBO_guns();

            for (int i = 0; i < 8; i++)
            {
                if ( this == hostGun[i].gun_obj )
                    hostGun[i].gun_obj = NULL;
            }
        }
    }
}

size_t NC_STACK_ypagun::SetPosition(bact_arg80 *arg)
{
    if ( !NC_STACK_ypabact::SetPosition(arg) )
        return 0;

    if ( ypagun.gunFlags & GUN_FLAGS_GROUND )
    {
        ypaworld_arg136 arg136;
        arg136.stPos.x = arg->pos.x;
        arg136.stPos.y = arg->pos.y + -10000.0;
        arg136.stPos.z = arg->pos.z;
        arg136.vect.x = 0;
        arg136.vect.y = 20000.0;
        arg136.vect.z = 0;
        arg136.flags = 0;

        ypabact.ywo->ypaworld_func136(&arg136);

        if ( arg136.isect )
            ypabact.position.y = arg136.isectPos.y - ypabact.overeof;

        ypagun_func128(vec3d(0, 0, 1.0), false);
    }
    else if ( !(arg->field_C & 4) )
    {
        ypabact.position.y = arg->pos.y + ypabact.pSector->height;
    }

    ypabact.old_pos = ypabact.position;

    int v12 = 2;
    HandleVisChildrens(&v12);

    return 1;
}

void NC_STACK_ypagun::EnergyInteract(update_msg *arg)
{
    if ( ypabact.status != BACT_STATUS_DEAD )
    {
        if ( ypabact.pSector->owner == ypabact.owner )
            ypabact.energy += ypabact.energy_max * (arg->frameTime / 1000.0) * ypabact.pSector->energy_power / 40000.0;

        if ( ypabact.energy > ypabact.energy_max )
            ypabact.energy = ypabact.energy_max;
    }
}

void NC_STACK_ypagun::Renew()
{
    NC_STACK_ypabact::Renew();

    ypagun.gunFlags = 0;
    ypagun.downTime = 0;

    setBACT_extraViewer(1);

    ypabact.viewer_position = vec3d(0, 0, 0);

    ypabact.viewer_rotation = ypabact.rotation;
}

size_t NC_STACK_ypagun::TestTargetSector(__NC_STACK_ypabact *cel_unit)
{
    vec3d vTgt = cel_unit->position - ypabact.position;

    float dist = vTgt.length();

    if ( dist < 1.0 )
        return 1;

    if ( dist > 2400.0 )
        return 0;

    vTgt /= dist;

    if ( C_PI_2 - clp_acos( vTgt.dot(-ypagun.rott) ) > ypagun.maxUp )
        return 0;

    if ( C_PI_2 - clp_acos( vTgt.dot(ypagun.rott) ) > ypagun.maxDown )
        return 0;

    vec3d tmp = vTgt - ypagun.rott * vTgt.dot(ypagun.rott);

    float ln = tmp.length();

    if ( ln <= 0.1 )
        return 1;

    if ( clp_acos( tmp.dot(ypagun.basis) / ln ) <= ypagun.maxSide )
        return 1;

    return 0;
}

void NC_STACK_ypagun::ypagun_func128(const vec3d &_basis, bool directDown)
{
    vec3d basis = _basis;
    float ln = basis.length();

    if ( ln > 0.001 )
        basis /= ln;

    ypagun.basis = basis;
    ypabact.rotation.SetZ( basis );

    if ( basis.y != 0.0 )
    {
        if ( basis.x != 0.0 || basis.z != 0.0 )
        {
            float v12 = -1.0 / ( basis.y / sqrt( POW2(basis.x) + POW2(basis.z) ) );

            ypabact.rotation.m11 = sqrt(POW2(v12) / (POW2(v12) + 1.0));

            if ( basis.x != 0.0 )
            {
                float v14 = 1.0 - POW2(ypabact.rotation.m11);
                ypabact.rotation.m10 = sqrt( v14 / (POW2(basis.z) / (POW2(basis.x)) + 1.0) );
                ypabact.rotation.m12 = sqrt( v14 - POW2(ypabact.rotation.m10) );
            }
            else
            {
                float v17 = 1.0 - POW2(ypabact.rotation.m11);
                ypabact.rotation.m12 = sqrt( v17 / (POW2(basis.x) / (POW2(basis.z)) + 1.0) );
                ypabact.rotation.m10 = sqrt( v17 - POW2(ypabact.rotation.m12) );
            }

            if ( basis.x < 0.0 )
                ypabact.rotation.m10 = -ypabact.rotation.m10;

            if ( basis.z < 0.0 )
                ypabact.rotation.m12 = -ypabact.rotation.m12;

            if ( basis.y > 0.0 )
            {
                ypabact.rotation.m10 = -ypabact.rotation.m10;
                ypabact.rotation.m12 = -ypabact.rotation.m12;
            }
        }
        else
        {
            ypabact.rotation.SetY( vec3d(0.0, 0.0, 1.0) );
        }
    }
    else
    {
        ypabact.rotation.SetY( vec3d(0.0, 1.0, 0.0) );
    }

    if ( directDown )
    {
        ypagun.gunFlags |= GUN_FLAGS_FALLDOWN;
        ypabact.rotation.SetY( -ypabact.rotation.AxisY() );
    }

    ypabact.rotation.SetX( ypabact.rotation.AxisY() * ypabact.rotation.AxisZ() );

    ypagun.rott = ypabact.rotation.AxisY();
}


vec3d NC_STACK_ypagun::ypagun_func129(const vec3d &axis, float angle)
{
    ypagun.basis = mat3x3(axis, -angle, MAT_FLAG_INV_SIN).Transform(ypagun.basis);

    ypabact.rotation *= mat3x3(axis, angle, MAT_FLAG_INV_SIN);

    return ypagun.basis;
}


void NC_STACK_ypagun::setGUN_sideAngle(int angl)
{
    ypagun.maxSide = angl / 1000.0;
}

void NC_STACK_ypagun::setGUN_upAngle(int angl)
{
    ypagun.maxUp = angl / 1000.0;
}

void NC_STACK_ypagun::setGUN_downAngle(int angl)
{
    ypagun.maxDown = angl / 1000.0;
}

void NC_STACK_ypagun::setGUN_fireType(int tp)
{
    ypagun.gunType = tp;
}

void NC_STACK_ypagun::setGUN_fireTime(int time)
{
    ypagun.fireTime = time;
}

void NC_STACK_ypagun::setGUN_setGround(int grnd)
{
    if ( grnd )
        ypagun.gunFlags |= GUN_FLAGS_GROUND;
    else
        ypagun.gunFlags &= ~GUN_FLAGS_GROUND;
}

void NC_STACK_ypagun::setGUN_roboGun(int rbo)
{
    if ( rbo )
        ypagun.gunFlags |= GUN_FLAGS_ROBO;
    else
        ypagun.gunFlags &= ~GUN_FLAGS_ROBO;
}



int NC_STACK_ypagun::getGUN_sideAngle()
{
    return ypagun.maxSide * 1000.0;
}

int NC_STACK_ypagun::getGUN_upAngle()
{
    return ypagun.maxUp * 1000.0;
}

int NC_STACK_ypagun::getGUN_downAngle()
{
    return ypagun.maxDown * 1000.0;
}

int NC_STACK_ypagun::getGUN_fireType()
{
    return ypagun.gunType;
}

int NC_STACK_ypagun::getGUN_fireTime()
{
    return ypagun.fireTime;
}

bool NC_STACK_ypagun::getGUN_setGround()
{
    return (ypagun.gunFlags & GUN_FLAGS_GROUND) != 0;
}

bool NC_STACK_ypagun::getGUN_roboGun()
{
    return (ypagun.gunFlags & GUN_FLAGS_ROBO) != 0;
}


size_t NC_STACK_ypagun::compatcall(int method_id, void *data)
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
    case 70:
        AI_layer3( (update_msg *)data );
        return 1;
    case 71:
        User_layer( (update_msg *)data );
        return 1;
    case 75:
        FightWithBact( (bact_arg75 *)data );
        return 1;
    case 77:
        Die();
        return 1;
    case 80:
        return (size_t)SetPosition( (bact_arg80 *)data );
    case 82:
        EnergyInteract( (update_msg *)data );
        return 1;
    case 96:
        Renew();
        return 1;
    case 111:
        return (size_t)TestTargetSector( (__NC_STACK_ypabact *)data );
    case 128:
        //ypagun_func128( (gun_arg128 *)data );
        return 1;
    case 129:
        //ypagun_func129( (gun_arg129 *)data );
        return 1;
    default:
        break;
    }
    return NC_STACK_ypabact::compatcall(method_id, data);
}
