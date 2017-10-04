#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include "yw.h"
#include "ypabact.h"
#include "yparobo.h"
#include "ypamissile.h"
#include "yw_net.h"

#include "log.h"



const NewClassDescr NC_STACK_ypabact::description("ypabact.class", &newinstance);


int ypabact_id = 1;
char **dword_5490B0; // ypaworld strings


size_t NC_STACK_ypabact::func0(IDVList *stak)
{
    if ( !NC_STACK_nucleus::func0(stak) )
        return 0;

    __NC_STACK_ypabact *bact_int = &ypabact;

    init_list(&ypabact.attackers_list);
    init_list(&ypabact.subjects_list);
    init_list(&ypabact.missiles_list);

    ypabact.gid = ypabact_id;
    ypabact.bact_type = BACT_TYPES_BACT;
//    ypabact.field_3DA = 0;
    ypabact.host_station = NULL;
    ypabact.viewer_rotation = mat3x3::Ident();
    ypabact.fly_dir = vec3d(0.0, 0.0, 0.0);
    ypabact.fly_dir_length = 0;
    ypabact.target_vec = vec3d(0.0, 0.0, 0.0);
    ypabact.attack_node_prim.bacto = this;
    ypabact.attack_node_scnd.bacto = this;

    ypabact.subject_node.bacto = this;

    ypabact.attack_node_prim.bact = bact_int;

    ypabact.attack_node_scnd.bact = bact_int;

    ypabact_id++;

    ypabact.rotation = ypabact.viewer_rotation;

    ypabact.subject_node.bact = bact_int;

    ypabact.self = this;

    ypabact.mass = 400.0;
    ypabact.force = 5000.0;
    ypabact.airconst = 500.0;
    ypabact.maxrot = 0.5;
    ypabact.height = 150.0;
    ypabact.radius = 20.0;
    ypabact.viewer_radius = 40.0;
    ypabact.overeof = 10.0;
    ypabact.viewer_overeof = 40.0;
    ypabact.energy = 10000;
    ypabact.shield = 0;
    ypabact.heading_speed = 0.7;
    ypabact.yls_time = 3000;
    ypabact.aggr = 50;
    ypabact.energy_max = 10000;
//    ypabact.field_3CE = 0;
    ypabact.height_max_user = 1600.0;
    ypabact.gun_radius = 5.0;
    ypabact.gun_power = 4000.0;
    ypabact.adist_sector = 800.0;
    ypabact.adist_bact = 650.0;
    ypabact.sdist_sector = 200.0;
    ypabact.sdist_bact = 100.0;
    ypabact.oflags = BACT_OFLAG_EXACTCOLL;

    NC_STACK_ypaworld *ywo = (NC_STACK_ypaworld *)stak->GetPointer(BACT_ATT_WORLD, 0);// get ypaworld
    ypabact.ywo = ywo;

    if ( ywo )
    {
        _NC_STACK_ypaworld *yw = &ywo->stack__ypaworld;
        ypabact.yw = yw;

        if (stak)
        {
            for(IDVList::iterator it = stak->begin(); it != stak->end(); it++)
            {
                IDVPair &val = it->second;

                if ( !val.skip() )
                {
                    switch (val.id)
                    {
                    case BACT_ATT_VIEWER:
                    {
                        uamessage_viewer viewMsg;

                        if ( val.value.i_data )
                        {
                            ywo->ypaworld_func131(&ypabact); //Set current bact

                            ypabact.oflags |= BACT_OFLAG_VIEWER;

                            if ( yw->isNetGame )
                                viewMsg.view = 1;

                            startSound(&ypabact.soundcarrier, 8);
                        }
                        else
                        {
                            ypabact.oflags &= ~BACT_OFLAG_VIEWER;

                            if ( yw->isNetGame )
                                viewMsg.view = 0;

                            sub_424000(&ypabact.soundcarrier, 8);
                        }

                        if ( yw->isNetGame ) // Network message send routine?
                        {
                            viewMsg.msgID = UAMSG_VIEWER;
                            viewMsg.owner = ypabact.owner;
                            viewMsg.classID = ypabact.bact_type;
                            viewMsg.id = ypabact.gid;

                            if ( viewMsg.classID == 4 )
                            {
                                NC_STACK_ypamissile *miss = dynamic_cast<NC_STACK_ypamissile *>(this);
                                __NC_STACK_ypabact *a4 = miss->getMISS_launcher();
                                viewMsg.launcher = a4->gid;
                            }

                            yw_arg181 ywMsg;
                            ywMsg.recvID = 0;
                            ywMsg.recvFlags = 2;
                            ywMsg.data = &viewMsg;
                            ywMsg.garant = 1;
                            ywMsg.dataSize = sizeof(viewMsg);

                            ywo->ypaworld_func181(&ywMsg);
                        }
                    }
                    break;

                    case BACT_ATT_INPUTTING:
                        if ( val.value.i_data )
                        {
                            ypabact.oflags |= BACT_OFLAG_USERINPT;
                            ywo->setYW_userVehicle(this);
                        }
                        else
                        {
                            ypabact.oflags &= ~BACT_OFLAG_USERINPT;
                        }
                        break;

                    case BACT_ATT_EXACTCOLL:
                        setBACT_exactCollisions(val.value.i_data);
                        break;

                    case BACT_ATT_BACTCOLL:
                        setBACT_bactCollisions ( val.value.i_data );
                        break;

                    case BACT_ATT_AIRCONST:
                        setBACT_airconst(val.value.i_data);
                        break;

                    case BACT_ATT_LANDINGONWAIT:
                        setBACT_landingOnWait ( val.value.i_data );
                        break;

                    case BACT_ATT_YOURLS:
                        setBACT_yourLastSeconds(val.value.i_data);
                        break;

                    case BACT_ATT_VISPROT:
                        setBACT_visProto( (NC_STACK_base *)val.value.p_data);
                        break;

                    case BACT_ATT_AGGRESSION:
                        setBACT_aggression(val.value.i_data);
                        break;

                    case BACT_ATT_VPTRANSFORM:
                        setBACT_vpTransform( (TForm3D *)val.value.p_data);
                        break;

                    case BACT_ATT_EXTRAVIEWER:
                        setBACT_extraViewer ( val.value.i_data );
                        break;

                    case BACT_ATT_ALWAYSRENDER:
                        setBACT_alwaysRender ( val.value.i_data );
                        break;

                    default:
                        break;
                    }
                }
            }
        }
    }

    ypabact.tForm.locPos = ypabact.position;

    ypabact.tForm.locSclRot = ypabact.rotation;

    ypabact.status = BACT_STATUS_NORMAL;

    int secMaxX = ypabact.ywo->getYW_mapSizeX();
    int secMaxY = ypabact.ywo->getYW_mapSizeY();

    ypabact.wrldX =  secMaxX * 1200.0;
    ypabact.wrldY = -secMaxY * 1200.0;
    ypabact.secMaxX = secMaxX;
    ypabact.secMaxY = secMaxY;

    dword_5490B0 = ypabact.ywo->getYW_localeStrings();

    return 1;
}

size_t NC_STACK_ypabact::func1()
{
    __NC_STACK_ypabact *bact = &ypabact;

    sub_423DD8(&bact->soundcarrier);

    bact->status_flg |= BACT_STFLAG_CLEAN;

    if ( !(bact->status_flg & BACT_STFLAG_DEATH1) )
        Die();

    if ( bact->pSector )
        Remove(bact);

    if ( bact->parent_bacto )
        Remove(&bact->subject_node);

    while (1)
    {
        bact_node *nd = (bact_node *)bact->subjects_list.head;

        if (!nd->next)
            break;

        delete_class_obj(nd->bacto);
    }

    while ( 1 )
    {
        bact_node *nd = (bact_node *)bact->missiles_list.head;

        if (!nd->next)
            break;

        Remove(bact->missiles_list.head);

        delete_class_obj(nd->bacto);
    }

    return NC_STACK_nucleus::func1();
}


void sub_493DB0(__NC_STACK_ypabact *bact, __NC_STACK_ypabact *bact2, NC_STACK_ypaworld *ywo)
{
    __NC_STACK_ypabact *v6 = NULL;

    bact->waypoints_count = 0;
    bact->m_cmdID = 0;
    bact->status_flg &= ~(BACT_STFLAG_WAYPOINT | BACT_STFLAG_WAYPOINTCCL);

    int tgType;
    vec2d wTo;

    if ( bact2->status_flg & BACT_STFLAG_WAYPOINT )
    {
        if ( !bact2->m_cmdID )
        {
            int v9 = bact2->waypoints_count - 1;

            wTo = bact2->waypoints[v9].XZ();

            tgType = BACT_TGT_TYPE_CELL;
        }
        else
        {
            v6 = sub_48C244(ywo, bact2->m_cmdID, bact2->m_owner);

            if ( v6 )
                tgType = BACT_TGT_TYPE_UNIT;
            else
                tgType = BACT_TGT_TYPE_NONE;
        }
    }
    else
    {
        if ( bact2->primTtype == BACT_TGT_TYPE_UNIT )
        {
            v6 = bact2->primT.pbact;
            tgType = BACT_TGT_TYPE_UNIT;
        }
        else if ( bact2->primTtype == BACT_TGT_TYPE_CELL )
        {
            wTo = bact2->primTpos.XZ();
            tgType = BACT_TGT_TYPE_CELL;
        }
        else
            tgType = BACT_TGT_TYPE_NONE;
    }

    if ( !tgType )
    {
        tgType = BACT_TGT_TYPE_UNIT;
        v6 = bact2;
    }

    if ( bact->bact_type != BACT_TYPES_TANK && bact->bact_type != BACT_TYPES_CAR )
    {
        setTarget_msg arg67;
        arg67.tgt_type = tgType;
        arg67.priority = 0;

        if ( tgType == BACT_TGT_TYPE_UNIT )
        {
            arg67.tgt.pbact = v6;
        }
        else
        {
            arg67.tgt_pos.x = wTo.x;
            arg67.tgt_pos.z = wTo.y;
        }

        bact->self->SetTarget(&arg67);
    }
    else
    {
        bact_arg124 arg125;

        if ( tgType == BACT_TGT_TYPE_UNIT )
        {
            arg125.to = v6->position.XZ();
        }
        else
        {
            arg125.to = wTo;
        }

        arg125.steps_cnt = 32;
        arg125.from = bact->position.XZ();
        arg125.field_12 = 1;

        bact->self->SetPath(&arg125);

        if ( tgType == BACT_TGT_TYPE_UNIT )
        {
            bact->m_cmdID = v6->commandID;
            bact->m_owner = v6->owner;
        }
    }
}

size_t NC_STACK_ypabact::func2(IDVList *stak)
{
    NC_STACK_nucleus::func2(stak);

    if (stak)
    {
        for(IDVList::iterator it = stak->begin(); it != stak->end(); it++)
        {
            IDVPair &val = it->second;

            if ( !val.skip() )
            {
                switch (val.id)
                {
                case BACT_ATT_VIEWER:
                    setBACT_viewer(val.value.i_data);
                    break;

                case BACT_ATT_INPUTTING:
                    setBACT_inputting(val.value.i_data);
                    break;

                case BACT_ATT_EXACTCOLL:
                    setBACT_exactCollisions ( val.value.i_data );
                    break;

                case BACT_ATT_BACTCOLL:
                    setBACT_bactCollisions ( val.value.i_data );
                    break;

                case BACT_ATT_AIRCONST:
                    setBACT_airconst(val.value.i_data);
                    break;

                case BACT_ATT_LANDINGONWAIT:
                    setBACT_landingOnWait ( val.value.i_data );
                    break;

                case BACT_ATT_YOURLS:
                    setBACT_yourLastSeconds(val.value.i_data);
                    break;

                case BACT_ATT_VISPROT:
                    setBACT_visProto( (NC_STACK_base *)val.value.p_data);
                    break;

                case BACT_ATT_AGGRESSION:
                    setBACT_aggression(val.value.i_data);
                    break;

                case BACT_ATT_VPTRANSFORM:
                    setBACT_vpTransform((TForm3D *)val.value.p_data);
                    break;

                case BACT_ATT_EXTRAVIEWER:
                    setBACT_extraViewer ( val.value.i_data );
                    break;

                case BACT_ATT_ALWAYSRENDER:
                    setBACT_alwaysRender ( val.value.i_data );
                    break;

                default:
                    break;
                }
            }
        }
    }

    return 1;
}

size_t NC_STACK_ypabact::func3(IDVList *stak)
{
    NC_STACK_nucleus::func3(stak);

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
                    *(NC_STACK_ypaworld **)val.value.p_data = getBACT_pWorld();
                    break;

                case BACT_ATT_PTRANSFORM:
                    *(TForm3D **)val.value.p_data = getBACT_pTransform();
                    break;

                case BACT_ATT_PBACT:
                    *(__NC_STACK_ypabact **)val.value.p_data = getBACT_pBact();
                    break;

                case BACT_ATT_VIEWER:
                    *(int *)val.value.p_data = getBACT_viewer();
                    break;

                case BACT_ATT_INPUTTING:
                    *(int *)val.value.p_data = getBACT_inputting();
                    break;

                case BACT_ATT_EXACTCOLL:
                    *(int *)val.value.p_data = getBACT_exactCollisions();
                    break;

                case BACT_ATT_BACTCOLL:
                    *(int *)val.value.p_data = getBACT_bactCollisions();
                    break;

                case BACT_ATT_ATTACKLIST:
                    *(nlist **)val.value.p_data = getBACT_attackList();
                    break;

                case BACT_ATT_LANDINGONWAIT:
                    *(int *)val.value.p_data = getBACT_landingOnWait();
                    break;

                case BACT_ATT_YOURLS:
                    *(int *)val.value.p_data = getBACT_yourLastSeconds();
                    break;

                case BACT_ATT_VISPROT:
                    *(NC_STACK_base **)val.value.p_data = getBACT_visProto();
                    break;

                case BACT_ATT_AGGRESSION:
                    *(int *)val.value.p_data = getBACT_aggression();
                    break;

                case BACT_ATT_COLLNODES:
                    *(rbcolls **)val.value.p_data = getBACT_collNodes();
                    break;

                case BACT_ATT_VPTRANSFORM:
                    *(TForm3D **)val.value.p_data = getBACT_vpTransform();
                    break;

                case BACT_ATT_EXTRAVIEWER:
                    *(int *)val.value.p_data = getBACT_extraViewer();
                    break;

                case BACT_ATT_P_ATTACKNODE:
                    *(bact_node **)val.value.p_data = getBACT_primAttackNode();
                    break;

                case BACT_ATT_S_ATTACKNODE:
                    *(bact_node **)val.value.p_data = getBACT_secnAttackNode();
                    break;

                case BACT_ATT_ALWAYSRENDER:
                    *(int *)val.value.p_data = getBACT_alwaysRender();
                    break;

                default:
                    break;
                }
            }
        }
    }

    return 1;
}






void sub_481F14(__NC_STACK_ypabact *bact)
{
    ypaworld_arg136 arg136;
    arg136.stPos = vec3d(bact->position.x, -30000.0, bact->position.z);
    arg136.vect = vec3d(0.0, 0.0, 50000.0);
    arg136.flags = 0;

    bact->ywo->ypaworld_func136(&arg136);

    if ( arg136.isect )
        bact->position.y = arg136.isectPos.y - 50.0;
    else
        bact->position.y = bact->pSector->height  - 50.0;
}


void sub_481E0C(__NC_STACK_ypabact *bact)
{
    int maxX = bact->ywo->getYW_mapSizeX();
    int maxY = bact->ywo->getYW_mapSizeY();

    float mx = maxX * 1200.0;
    float my = -(maxY * 1200.0);

    if ( bact->position.x > mx )
        bact->position.x = mx - 600.0;

    if ( bact->position.x < 0.0 )
        bact->position.x = 600.0;

    if ( bact->position.z < my )
        bact->position.z = my + 600.0;

    if ( bact->position.z > 0.0 )
        bact->position.z = -600.0;

    sub_481F14(bact);
}

void sub_481F94(__NC_STACK_ypabact *bact)
{
    bact_node *node = (bact_node *)bact->missiles_list.head;

    while (node->next)
    {
        bact_node * next_node = (bact_node *)node->next; // Save next node before remove

        int a4 = node->bacto->getBACT_yourLastSeconds();

        if ( a4 <= 0 )
        {
            setTarget_msg arg67;
            arg67.tgt_type = BACT_TGT_TYPE_NONE;
            arg67.priority = 0;

            node->bacto->SetTarget(&arg67);

            Remove(node);

            node->bact->parent_bacto = NULL;

            bact->self->Release(node->bacto);
        }

        node = next_node;
    }
}


TForm3D bact_cam;

void NC_STACK_ypabact::Update(update_msg *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    sub_430A20(&bact_cam);

    yw_130arg sect_info;
    sect_info.pos_x = bact->position.x;
    sect_info.pos_z = bact->position.z;

    if ( !bact->ywo->ypaworld_func130(&sect_info) )
    {
        sub_481E0C(bact);

        sect_info.pos_x = bact->position.x;
        sect_info.pos_z = bact->position.z;

        bact->ywo->ypaworld_func130(&sect_info);
    }

    cellArea *oldcell = bact->pSector;

    bact->sectX = sect_info.sec_x;
    bact->sectY = sect_info.sec_z;

//    bact->pos_x_cntr = sect_info.pos_x_cntr;
//    bact->pos_y_cntr = sect_info.pos_y_cntr;

    bact->pSector = sect_info.pcell;

    if ( oldcell != sect_info.pcell ) // If cell changed
    {
        Remove(bact);  //Remove unit from old cell
        AddTail(&bact->pSector->units_list, bact);  // Add unit to new cell
    }

    if ( bact->pSector->height + 1000.0 < bact->position.y )
        sub_481F14(bact);

    NC_STACK_ypabact *retbact = bact->ywo->getYW_userHostStation();

    if ( bact->pSector->w_type == 6 && bact->bact_type == BACT_TYPES_ROBO && retbact == bact->self ) // if bact class == robo
        ypabact_func65__sub0(bact);

    if ( !(bact->status_flg & BACT_STFLAG_DEATH1) && bact->energy <= 0 && bact->bact_type != BACT_TYPES_MISSLE )
    {
        Die();

        if ( !IsDestroyed() )
        {
            setState_msg v38;
            v38.setFlags = 0;
            v38.unsetFlags = 0;
            v38.newStatus = BACT_STATUS_IDLE;

            SetState(&v38);
        }

        bact->status = BACT_STATUS_DEAD;
        bact->status_flg &= ~BACT_STFLAG_LAND;
    }

    bact->clock += arg->frameTime;

    AI_layer1(arg);

    bact_node *vnod = (bact_node *)bact->missiles_list.head;
    while(vnod->next)
    {
        bact_node *next_vnod = (bact_node *)vnod->next;
        vnod->bacto->Update(arg);
        vnod = next_vnod;
    }

    sub_481F94(bact);

    if ( bact->oflags & BACT_OFLAG_VIEWER )
    {
        if ( bact->oflags & BACT_OFLAG_EXTRAVIEW )
            bact_cam.locPos = bact->position + bact->rotation.Transpose().Transform(bact->viewer_position);
        else
            bact_cam.locPos = bact->position;

        if ( bact->oflags & BACT_OFLAG_EXTRAVIEW )
            bact_cam.locSclRot = bact->viewer_rotation;
        else
            bact_cam.locSclRot = bact->rotation;

        GFXEngine::GFXe.getC3D()->matrixAspectCorrection(bact_cam.locSclRot, false);
    }

    bact->tForm.locPos = bact->position;

    if ( bact->status_flg & BACT_STFLAG_SCALE )
        bact->tForm.locSclRot = bact->rotation.Transpose() * mat3x3::Scale( bact->scale );
    else
        bact->tForm.locSclRot = bact->rotation.Transpose();

    int numbid = arg->units_count;

    arg->units_count = 0;

    bact_node *bnod = (bact_node *)bact->subjects_list.head;

    while (bnod->next)
    {
        bact_node *next_node = (bact_node *)bnod->next;

        bnod->bacto->Update(arg);

        arg->units_count++;

        bnod = next_node;
    }

    arg->units_count = numbid;

    bact->soundcarrier.field_0 = bact->position;

    if ( bact->oflags & BACT_OFLAG_VIEWER )
        bact->soundcarrier.field_0 += bact->rotation.AxisY() * 400.0;

    bact->soundcarrier.field_C = bact->fly_dir * bact->fly_dir_length;

    sb_0x4242e0(&bact->soundcarrier);
}

void NC_STACK_ypabact::Render(baseRender_msg *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    if ( bact->current_vp.base )
    {
        if ( !(bact->status_flg & BACT_STFLAG_NORENDER) )
        {
            if ( !(bact->oflags & BACT_OFLAG_VIEWER) || bact->oflags & BACT_OFLAG_ALWAYSREND )
            {
                bact->current_vp.trigo->locPos = bact->tForm.locPos;
                bact->current_vp.trigo->locSclRot = bact->tForm.locSclRot;

                arg->ownerID = bact->gid;
                bact->current_vp.base->base_func77(arg);
            }
        }
    }

    for (int i = 0; i < 3; i++)
    {
        extra_vproto *bd = &bact->vp_extra[i];

        if ( bd->vp.base )
        {
            if ( bd->flags & EVPROTO_FLAG_ACTIVE )
            {
                bd->vp.trigo->locPos = bd->pos;

                if ( bd->flags & EVPROTO_FLAG_SCALE )
                    bd->vp.trigo->locSclRot = bd->rotate.Transpose() * mat3x3::Scale( vec3d(bd->scale, bd->scale, bd->scale) );
                else
                    bd->vp.trigo->locSclRot = bd->rotate.Transpose();

                arg->ownerID = bact->gid;

                bd->vp.base->base_func77(arg);
            }
        }
    }
}

void NC_STACK_ypabact::SetTarget(setTarget_msg *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    bact->assess_time = 0;
    yw_130arg arg130;

    if ( bact->status_flg & BACT_STFLAG_DEATH1 && arg->tgt_type == 2 )
    {
        ypa_log_out("ALARM!!! bact-settarget auf logische Leiche owner %d, class %d, prio %d\n", bact->owner, bact->bact_type, arg->priority);
    }
    else if ( arg->priority )
    {
        if ( bact->secndTtype == BACT_TGT_TYPE_UNIT )
            Remove(&bact->attack_node_scnd);

        switch ( arg->tgt_type )
        {
        case BACT_TGT_TYPE_CELL:
        case BACT_TGT_TYPE_CELL_IND:
            bact->secndTtype = BACT_TGT_TYPE_CELL;

            arg130.pos_x = arg->tgt_pos.x;
            arg130.pos_z = arg->tgt_pos.z;

            if ( arg130.pos_x < 1210.0 )
                arg130.pos_x = 1210.0;

            if ( arg130.pos_x > bact->wrldX - 1210.0 )
                arg130.pos_x = bact->wrldX - 1210.0;

            if ( arg130.pos_z > -1210.0 )
                arg130.pos_z = -1210.0;

            if ( arg130.pos_z < bact->wrldY + 1210.0 )
                arg130.pos_z = bact->wrldY + 1210.0;

            if ( bact->ywo->ypaworld_func130(&arg130) )
            {
                bact->secndT.pcell = arg130.pcell;
                bact->sencdTpos.x = arg130.pos_x;
                bact->sencdTpos.z = arg130.pos_z;
                bact->sencdTpos.y = arg130.pcell->height;
            }
            else
            {
                bact->secndTtype = BACT_TGT_TYPE_NONE;
                bact->secndT.pcell = NULL;
            }
            break;

        case BACT_TGT_TYPE_UNIT:
        case BACT_TGT_TYPE_UNIT_IND:
            bact->secndT.pbact = arg->tgt.pbact;
            bact->secndTtype = BACT_TGT_TYPE_UNIT;

            if ( bact->secndT.pbact )
            {
                if ( bact->secndT.pbact->status_flg & BACT_STFLAG_DEATH1 )
                {
                    ypa_log_out("totes vehicle als nebenziel, owner %d, class %d\n", arg->tgt.pbact->owner, arg->tgt.pbact->bact_type);
                    bact->secndTtype = BACT_TGT_TYPE_NONE;
                }
                else
                {
                    bact->sencdTpos = bact->secndT.pbact->position;

                    nlist *lst = bact->secndT.pbact->self->getBACT_attackList();

                    if ( lst )
                        AddTail(lst, &bact->attack_node_scnd);
                    else
                        ypa_log_out("Net eigeklink\n");
                }
            }
            else
            {
                ypa_log_out("Yppsn\n");
                bact->secndTtype = BACT_TGT_TYPE_NONE;
            }
            break;

        case BACT_TGT_TYPE_FRMT:
            bact->secndTtype = BACT_TGT_TYPE_FRMT;
            bact->sencdTpos = arg->tgt_pos;
            break;

        case BACT_TGT_TYPE_NONE:
            bact->secndT.pbact = NULL;
            bact->secndTtype = BACT_TGT_TYPE_NONE;
            break;

        default:
            bact->secndTtype = arg->tgt_type;
            break;
        }
    }
    else
    {
        if ( bact->primTtype == BACT_TGT_TYPE_UNIT )
            Remove(&bact->attack_node_prim);

        switch ( arg->tgt_type )
        {
        case BACT_TGT_TYPE_CELL:
        case BACT_TGT_TYPE_CELL_IND:
            bact->primT_cmdID = 0;
            bact->primTtype = BACT_TGT_TYPE_CELL;

            arg130.pos_x = arg->tgt_pos.x;
            arg130.pos_z = arg->tgt_pos.z;

            if ( arg130.pos_x < 1210.0 )
                arg130.pos_x = 1210.0;

            if ( arg130.pos_x > bact->wrldX - 1210.0 )
                arg130.pos_x = bact->wrldX - 1210.0;

            if ( arg130.pos_z > -1210.0 )
                arg130.pos_z = -1210.0;

            if ( arg130.pos_z < bact->wrldY + 1210.0 )
                arg130.pos_z = bact->wrldY + 1210.0;

            if ( bact->ywo->ypaworld_func130(&arg130) )
            {
                bact->primT.pcell = arg130.pcell;
                bact->primTpos.x = arg130.pos_x;
                bact->primTpos.z = arg130.pos_z;
                bact->primTpos.y = arg130.pcell->height;
            }
            else
            {
                bact->primTtype = BACT_TGT_TYPE_NONE;
                bact->primT.pcell = NULL;
            }
            break;

        case BACT_TGT_TYPE_UNIT:
        case BACT_TGT_TYPE_UNIT_IND:
            bact->primT.pbact = arg->tgt.pbact;
            bact->primTtype = BACT_TGT_TYPE_UNIT;

            if ( bact->primT.pbact )
            {
                if ( bact->primT.pbact->status_flg & BACT_STFLAG_DEATH1 )
                {
                    ypa_log_out("totes vehicle als hauptziel, owner %d, class %d - ich bin class %d\n", arg->tgt.pbact->owner, arg->tgt.pbact->bact_type, bact->bact_type);
                    bact->primTtype = BACT_TGT_TYPE_NONE;
                    return;
                }

                bact->primTpos = bact->primT.pbact->position;

                nlist *lst = bact->primT.pbact->self->getBACT_attackList();

                if ( lst )
                    AddTail(lst, &bact->attack_node_prim);

                bact->primT_cmdID = arg->tgt.pbact->commandID;
            }
            else
            {
                ypa_log_out("PrimT. without a pointer\n");
                bact->primTtype = BACT_TGT_TYPE_NONE;
            }
            break;

        case BACT_TGT_TYPE_FRMT:
            bact->primTtype = BACT_TGT_TYPE_FRMT;
            bact->primT_cmdID = 0;
            bact->primTpos = arg->tgt_pos;
            break;

        case BACT_TGT_TYPE_NONE:
            bact->primT.pbact = NULL;
            bact->waypoints_count = 0;
            bact->primTtype = BACT_TGT_TYPE_NONE;
            bact->status_flg &= ~BACT_STFLAG_WAYPOINT;
            break;

        case BACT_TGT_TYPE_DRCT:
            bact->target_dir = arg->tgt_pos;
            bact->primTtype = BACT_TGT_TYPE_DRCT;
            bact->primT.pbact = NULL;
            bact->primT_cmdID = 0;
            break;

        default:
            bact->primTtype = arg->tgt_type;
            break;
        }

        if ( arg->tgt_type == BACT_TGT_TYPE_CELL || arg->tgt_type == BACT_TGT_TYPE_UNIT )
        {
            bact_node *node = (bact_node *)bact->subjects_list.head;

            while ( node->next )
            {
                if ( node->bact->status != BACT_STATUS_DEAD)
                {
                    node->bacto->SetTarget(arg);
                    if ( !(bact->status_flg & BACT_STFLAG_WAYPOINT)  )
                        bact->status_flg &= ~(BACT_STFLAG_WAYPOINT | BACT_STFLAG_WAYPOINTCCL);
                }

                node = (bact_node *)node->next;
            }
        }
    }
}

void NC_STACK_ypabact::AI_layer1(update_msg *arg)
{
    setTarget_msg v36;

    __NC_STACK_ypabact *bact = &ypabact;

    if ( bact->mass == 1.0 )
    {
        if ( bact->status_flg & BACT_STFLAG_DEATH2 )
        {
            bact->yls_time -= arg->frameTime;

            if ( bact->yls_time < 0 )
                bact->ywo->ypaworld_func144(this);
        }
        else
        {
            setState_msg v37;
            v37.newStatus = BACT_STATUS_NOPE;
            v37.unsetFlags = 0;
            v37.setFlags = BACT_STFLAG_DEATH2;

            SetState(&v37);

            bact->yls_time = 6000;
        }
    }
    else
    {
        if ( bact->bact_type != BACT_TYPES_MISSLE )
            EnergyInteract(arg);

        if ( bact->status == BACT_STATUS_DEAD )
        {
            if ( bact->status_flg & BACT_STFLAG_LAND )
                bact->yls_time -= arg->frameTime;
        }

        bact->airconst = bact->airconst_static;

        bact->soundcarrier.samples_data[0].pitch = bact->pitch;
        bact->soundcarrier.samples_data[0].volume = bact->volume;

        if ( bact->clock - bact->AI_time1 >= 250 && bact->primTtype != BACT_TGT_TYPE_DRCT && bact->bact_type != BACT_TYPES_GUN && bact->status != BACT_STATUS_DEAD && bact->status != BACT_STATUS_BEAM && bact->status != BACT_STATUS_CREATE )
        {

            bact->AI_time1 = bact->clock;
            bact->target_vec = vec3d(0.0, 0.0, 0.0);

            if ( bact->clock - bact->brkfr_time > 5000 )
            {
                bact->brkfr_time = bact->clock;

                StuckFree(arg);
            }

            if ( bact->status == BACT_STATUS_NORMAL && bact->primTtype )
            {
                if ( bact->primTtype == BACT_TGT_TYPE_UNIT )
                {
                    bact->target_vec = bact->primT.pbact->position - bact->position;

                    if ( bact->primT.pbact->status != BACT_STATUS_DEAD)
                    {
                        bact->primTpos = bact->primT.pbact->position;
                    }
                }
                else
                {
                    bact->target_vec = bact->primTpos - bact->position;
                }

                if ( bact->target_vec.length() > 2000.0 )
                    bact->target_vec.y = 0;

                if ( bact->host_station == bact->parent_bacto )
                {
                    if ( bact->oflags & BACT_OFLAG_VIEWER )
                    {
                        float dist = bact->target_vec.length();

                        bact_node *node = (bact_node *)bact->subjects_list.head;

                        int v44 = 0;

                        while (node->next)
                        {
                            if ( node->bact->status != BACT_STATUS_DEAD )
                            {
                                if ( dist < 800.0 )
                                {
                                    v36.tgt_type = bact->primTtype;
                                    v36.priority = 0;
                                    v36.tgt.pbact = bact->primT.pbact;
                                    v36.tgt_pos = bact->primTpos;
                                }
                                else
                                {
                                    bact_arg94 v35;
                                    v35.field_0 = v44;
                                    GetFormationPosition(&v35);

                                    v36.tgt_type = BACT_TGT_TYPE_FRMT;
                                    v36.priority = 0;
                                    v36.tgt_pos = v35.pos1;
                                }

                                node->bacto->SetTarget(&v36);
                            }

                            node = (bact_node *)node->next;
                            v44++;
                        }
                    }
                }
            }

            if ( bact->primTtype == BACT_TGT_TYPE_NONE)
            {
                if ( bact->host_station && bact->primT_cmdID )
                {
                    v36.priority = bact->primT_cmdID;

                    if ( bact->host_station->yparobo_func132(&v36) )
                    {
                        v36.priority = 0;
                    }
                    else
                    {
                        bact->primT_cmdID = 0;

                        v36.tgt_type = BACT_TGT_TYPE_CELL;
                        v36.priority = 0;
                        v36.tgt_pos = bact->primTpos;
                    }

                    SetTarget(&v36);
                }
            }

            if ( bact->oflags & BACT_OFLAG_USERINPT )
            {
                if ( bact->primTtype == BACT_TGT_TYPE_UNIT )
                {
                    if ( bact->primT.pbact )
                    {
                        if ( !((1 << bact->owner) & bact->primT.pbact->pSector->view_mask) )
                        {
                            v36.tgt_type = BACT_TGT_TYPE_NONE;
                            v36.priority = 0;

                            SetTarget(&v36);
                        }
                    }
                }
            }
            else if ( bact->vp_active == 6 && bact->status == BACT_STATUS_NORMAL )
            {
                setState_msg v38;
                v38.newStatus = BACT_STATUS_NORMAL;
                v38.setFlags = 0;
                v38.unsetFlags = 0;
                SetState(&v38);
            }
        }

        AI_layer2(arg);
    }
}

void NC_STACK_ypabact::AI_layer2(update_msg *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    if ( (bact->clock - bact->AI_time2) >= 250 && bact->owner != 0 && bact->secndTtype != BACT_TGT_TYPE_DRCT && bact->status != BACT_STATUS_CREATE && bact->status != BACT_STATUS_DEAD && bact->status != BACT_STATUS_BEAM )
    {
        bact->AI_time2 = bact->clock;

        if ( bact->status_flg & BACT_STFLAG_ESCAPE )
        {
            float a1 = sqrt(bact->target_vec.x * bact->target_vec.x + bact->target_vec.z * bact->target_vec.z);

            if ( a1 > 3600.0 )
            {
                setTarget_msg arg67;
                arg67.tgt_type = BACT_TGT_TYPE_NONE;
                arg67.priority = 1;

                bact_node *nod = (bact_node *)bact->subjects_list.head;

                while( nod->next )
                {
                    nod->bacto->SetTarget(&arg67);
                    nod = (bact_node *)nod->next;
                }

                SetTarget(&arg67);

                if ( bact->oflags & BACT_OFLAG_USERINPT )
                    User_layer(arg);
                else
                    AI_layer3(arg);
                return;
            }
        }

        NC_STACK_ypabact *wee = bact->ywo->getYW_userHostStation();

        if ( bact->status == BACT_STATUS_NORMAL || bact->status == BACT_STATUS_IDLE )
        {
            if ( bact->clock - bact->search_time1 > 500 )
            {
                bact->search_time1 = bact->clock;

                bact_arg90 arg90;
                arg90.field_8 = 1;
                arg90.unit = bact;
                arg90.ret_unit = NULL;

                GetSectorTarget(&arg90);

                if ( arg90.ret_unit )
                {
                    if ( arg90.ret_unit->bact_type != BACT_TYPES_ROBO && bact->parent_bacto == bact->host_station && bact->host_station == wee && arg90.ret_unit->commandID != bact->fe_cmdID && bact->clock - bact->fe_time > 45000 )
                    {
                        int v32 = 0;
                        if ( arg90.ret_unit->bact_type == BACT_TYPES_GUN )
                        {
                            NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( arg90.ret_unit->self );
                            v32 = gun->getGUN_roboGun();
                        }

                        if ( !v32 )
                        {
                            bact->fe_cmdID = arg90.ret_unit->commandID;
                            bact->fe_time = bact->clock;

                            robo_arg134 arg134;
                            arg134.field_4 = 7;
                            arg134.field_8 = arg90.ret_unit->commandID;
                            arg134.field_C = v32;
                            arg134.field_10 = v32;
                            arg134.unit = bact;
                            arg134.field_14 = 46;

                            bact->host_station->placeMessage(&arg134);
                        }
                    }
                }

                if ( bact->status == BACT_STATUS_IDLE || ( bact->aggr >= 50 && !(bact->status_flg & BACT_STFLAG_ESCAPE) && (bact->secndTtype == BACT_TGT_TYPE_NONE || bact->secndTtype == BACT_TGT_TYPE_CELL || bact->secndTtype == BACT_TGT_TYPE_FRMT) ) )
                {
                    if ( arg90.ret_unit )
                    {
                        bact->secndT_cmdID = arg90.ret_unit->commandID;

                        setTarget_msg arg67;
                        arg67.tgt_type = BACT_TGT_TYPE_UNIT;
                        arg67.priority = 1;
                        arg67.tgt.pbact = arg90.ret_unit;

                        SetTarget(&arg67);
                    }

                    if ( bact->clock - bact->search_time2 > 2000 && bact->aggr == 75 && !(bact->oflags & BACT_OFLAG_VIEWER) && bact->parent_bacto == bact->host_station )
                    {
                        if ( (bact->secndTtype == BACT_TGT_TYPE_FRMT || bact->secndTtype == BACT_TGT_TYPE_NONE)
                                && bact->position.x > 1260.0
                                && bact->position.x < bact->wrldX + -1260.0
                                && bact->position.z < -1260.0
                                && bact->position.z > bact->wrldY + 1260.0 )
                        {
                            bact->search_time2 = bact->clock;

                            if ( bact->owner != bact->pSector->owner )
                            {
                                setTarget_msg arg67;
                                arg67.priority = 1;
                                arg67.tgt_type = BACT_TGT_TYPE_CELL;
                                arg67.tgt_pos.x = bact->position.x;
                                arg67.tgt_pos.z = bact->position.z;

                                SetTarget(&arg67);
                            }
                        }
                    }

                    if ( bact->host_station == bact->parent_bacto && bact->secndTtype == BACT_TGT_TYPE_CELL )
                    {
                        bact_node *nod = (bact_node *)bact->subjects_list.head;

                        while( nod->next )
                        {
                            if ( nod->bact->secndTtype == BACT_TGT_TYPE_NONE || nod->bact->secndTtype == BACT_TGT_TYPE_FRMT )
                            {
                                setTarget_msg arg67;
                                arg67.tgt_type = BACT_TGT_TYPE_CELL;
                                arg67.tgt_pos = bact->sencdTpos;
                                arg67.priority = 1;
                                nod->bacto->SetTarget(&arg67);
                            }

                            nod = (bact_node *)nod->next;
                        }
                    }
                }
            }

            if ( bact->secndTtype == BACT_TGT_TYPE_UNIT )
                bact->target_vec = bact->secndT.pbact->position - bact->position;
            else if ( bact->secndTtype == BACT_TGT_TYPE_CELL)
                bact->target_vec = bact->sencdTpos - bact->position;

            if ( bact->target_vec.length() > 2000.0 )
                bact->target_vec.y = 0;
        }

        if ( bact->oflags & BACT_OFLAG_USERINPT )
        {
            if ( bact->secndTtype == BACT_TGT_TYPE_UNIT )
            {
                if ( bact->secndT.pbact )
                {
                    vec2d v25 = bact->position.XZ() - bact->secndT.pbact->position.XZ();

                    if ( !((1 << bact->owner) & bact->secndT.pbact->pSector->view_mask) || v25.length() > 2160.0 )
                    {
                        setTarget_msg arg67;
                        arg67.priority = 1;
                        arg67.tgt_type = BACT_TGT_TYPE_NONE;
                        SetTarget(&arg67);
                    }
                }
            }
        }
    }

    if ( bact->oflags & BACT_OFLAG_USERINPT )
        User_layer(arg);
    else
        AI_layer3(arg);
}

void AI_layer3__sub1(__NC_STACK_ypabact *bact, update_msg *arg)
{
    bact->thraction = bact->force;

    float v39 = arg->frameTime / 1000.0;

    float top = -bact->target_dir.y;

    if ( top == 1.0 )
        top = 0.99998999;

    if ( top == -1.0 )
        top = -0.99998999;

    float weight = bact->mass * 9.80665;
    float thraction = bact->thraction;

    if ( thraction == 0.0 )
        thraction = 0.1;

    float v5 = sqrt( (1.0 - POW2(top)) ) * (top * -0.5);

    float v6 = (1.0 - 0.25 * POW2(top) + 0.25 * POW2(top) * POW2(top)) * (POW2(weight) / POW2(thraction));

    float v58 = sqrt( (1.0 - v6) ) + (weight * v5 / thraction);

    vec3d tmp;
    tmp.y = -cos( clp_asin(v58) );

    if ( bact->target_dir.z != 0.0 )
    {
        float v62 = (1.0 - POW2(tmp.y)) / (POW2(bact->target_dir.x) / POW2(bact->target_dir.z) + 1.0);

        if ( v62 < 0.0 )
            v62 = 0.0;

        tmp.z = sqrt(v62);

        if ( bact->target_dir.z < 0.0 )
            tmp.z = -tmp.z;
    }
    else
    {
        tmp.z = 0.0;
    }

    if ( bact->target_dir.x != 0.0 )
    {
        float v57 = (1.0 - POW2(tmp.y)) / (POW2(bact->target_dir.z) / POW2(bact->target_dir.x) + 1.0);

        if ( v57 < 0.0 )
            v57 = 0.0;

        tmp.x = sqrt(v57);

        if ( bact->target_dir.x < 0.0 )
            tmp.x = -tmp.x;
    }
    else
    {
        tmp.x = 0.0;
    }

    vec3d vaxis = (-bact->rotation.AxisY()) * tmp;;

    if ( vaxis.normalise() != 0.0 )
    {
        float maxrot = bact->maxrot * v39;

        float v56 = clp_acos( tmp.dot( -bact->rotation.AxisY() ) );

        if ( v56 > maxrot )
            v56 = maxrot;

        if ( fabs(v56) > BACT_MIN_ANGLE )
            bact->rotation *= mat3x3::AxisAngle(vaxis, v56);
    }
}

void AI_layer3__sub0(__NC_STACK_ypabact *bact, int a2)
{
    if ( clp_acos(bact->rotation.m11) > 0.003 && (bact->fly_dir.z != 0.0 || bact->fly_dir.x != 0.0) && bact->fly_dir_length > 0.0 )
    {
        float v11 = a2 / 1000.0;

        vec2d flydir = bact->fly_dir.XZ();
        vec2d axisZ = bact->rotation.AxisZ().XZ();

        float tmpsq = flydir.length();

        NDIV_CARRY(tmpsq);

        float v18 = flydir.dot(axisZ) / tmpsq;

        tmpsq = axisZ.length();

        NDIV_CARRY(tmpsq);

        float v20 = clp_acos( v18 / tmpsq );

        float v13 = bact->maxrot * v11 * (fabs(v20) * 0.8 + 0.2);

        if ( v20 > v13 )
            v20 = v13;

        if ( bact->fly_dir.x * bact->rotation.m22 - bact->rotation.m20 * bact->fly_dir.z < 0.0 )
            v20 = -v20;

        bact->rotation *= mat3x3::RotateY(-v20);
    }
}

void NC_STACK_ypabact::AI_layer3(update_msg *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    float v75 = arg->frameTime / 1000.0;

    float v77 = ypabact.target_vec.length();

    if ( v77 > 0.0 )
        bact->target_dir = bact->target_vec / v77;

    int v82 = bact->oflags & BACT_OFLAG_VIEWER;
    int v70 = bact->oflags & BACT_OFLAG_EXACTCOLL;

    int v80 = bact->ywo->ypaworld_func145(bact);

    int v79;

    if ( v82 )
        v79 = bact->viewer_radius;
    else
        v79 = bact->radius;

    switch ( bact->status )
    {
    case BACT_STATUS_NORMAL:
    {
        if ( bact->oflags & BACT_OFLAG_BACTCOLL )
        {
            if ( (v80 || (bact->secndTtype == BACT_TGT_TYPE_NONE && v77 < 1200.0)) && !(bact->status_flg & BACT_STFLAG_LAND) )
            {
                CollisionWithBact(arg->frameTime);
            }
        }

        if ( !bact->primTtype && !bact->secndTtype )
        {
            bact->status = BACT_STATUS_IDLE;

            if ( bact->status_flg & BACT_STFLAG_FIRE )
            {
                setState_msg arg78;
                arg78.newStatus = BACT_STATUS_NOPE;
                arg78.setFlags = 0;
                arg78.unsetFlags = BACT_STFLAG_FIRE;

                SetState(&arg78);
            }
            break;
        }

        ypaworld_arg136 arg136;

        arg136.isect = false;
        arg136.stPos = bact->old_pos;
        arg136.vect = bact->position - bact->old_pos;
        arg136.vect.y = 0;

        float len = arg136.vect.length();

        if ( len > 0.0 )
            arg136.vect *= 300.0 / len;
        else
            arg136.vect = bact->rotation.AxisZ() * 300.0;

        arg136.isect = false;
        arg136.flags = 0;

        ypaworld_arg136 arg136_1;
        arg136_1.isect = false;
        arg136_1.flags = 0;

        if ( v82 || (bact->status_flg & BACT_STFLAG_DODGE_RIGHT) || (v80 && v70) )
        {
            arg136_1.stPos = bact->old_pos;
            arg136_1.vect.x = arg136.vect.x * 0.93969 - arg136.vect.z * 0.34202;
            arg136_1.vect.y = arg136.vect.y;
            arg136_1.vect.z = arg136.vect.z * 0.93969 + arg136.vect.x * 0.34202;

            bact->ywo->ypaworld_func136(&arg136_1);
        }

        ypaworld_arg136 arg136_2;
        arg136_2.isect = false;
        arg136_2.flags = 0;

        if ( v82 || (bact->status_flg & BACT_STFLAG_DODGE_LEFT) || (v80 && v70) )
        {
            arg136_2.stPos = bact->old_pos;
            arg136_2.vect.x = arg136.vect.x * 0.93969 + arg136.vect.z * 0.34202;
            arg136_2.vect.y = arg136.vect.y;
            arg136_2.vect.z = arg136.vect.z * 0.93969 - arg136.vect.x * 0.34202;

            bact->ywo->ypaworld_func136(&arg136_2);
        }

        if ( v82 || !(bact->status_flg & (BACT_STFLAG_DODGE_LEFT | BACT_STFLAG_DODGE_RIGHT)) || (v80 && v70) )
            bact->ywo->ypaworld_func136(&arg136);

        int v18 = 0;

        bact_arg88 arg88;
        arg88.pos1 = vec3d(0.0, 0.0, 0.0);

        if ( arg136.isect )
        {
            if ( len + v79 > arg136.tVal * 300.0 )
            {
                arg88.pos1 = arg136.skel->polygons[arg136.polyID].Normal();
                v18++;
            }
        }

        if ( arg136_1.isect )
        {
            if ( len + v79 > arg136_1.tVal * 300.0 )
            {
                arg88.pos1 += arg136_1.skel->polygons[arg136_1.polyID].Normal();
                v18++;
            }
        }

        if ( arg136_2.isect )
        {
            if ( len + v79 > arg136_2.tVal * 300.0 )
            {
                arg88.pos1 += arg136_2.skel->polygons[arg136_2.polyID].Normal();
                v18++;
            }
        }

        if ( !arg136.isect && !arg136_1.isect && !arg136_2.isect )
        {
            bact->status_flg &= ~(BACT_STFLAG_DODGE_LEFT | BACT_STFLAG_DODGE_RIGHT | BACT_STFLAG_MOVE);
            bact->status_flg |= BACT_STFLAG_MOVE;
        }

        if ( !(bact->status_flg & (BACT_STFLAG_DODGE_LEFT | BACT_STFLAG_DODGE_RIGHT)) )
        {

            if ( arg136_1.isect == 1 && arg136_2.isect == 1 )
            {
                if ( arg136_1.tVal >= arg136_2.tVal )
                    bact->status_flg |= BACT_STFLAG_DODGE_LEFT;
                else
                    bact->status_flg |= BACT_STFLAG_DODGE_RIGHT;
            }

            if ( arg136_1.isect == 1 && !arg136_2.isect )
                bact->status_flg |= BACT_STFLAG_DODGE_RIGHT;

            if ( !arg136_1.isect && arg136_2.isect == 1 )
                bact->status_flg |= BACT_STFLAG_DODGE_LEFT;

            if ( !arg136_1.isect && !arg136_2.isect && arg136.isect == 1 )
                bact->status_flg |= BACT_STFLAG_DODGE_LEFT;
        }

        float v21 = bact->mass * 9.80665;

        if ( v21 <= bact->force )
            v21 = bact->force;

        float v88;

        if ( bact->airconst >= 10.0 )
            v88 = bact->airconst;
        else
            v88 = 10.0;


        ypaworld_arg136 arg136_3;

        arg136_3.vect.x = (bact->fly_dir.x * 200.0 * bact->fly_dir_length) / (v21 / v88);

        if ( arg136_3.vect.x < -200.0 )
            arg136_3.vect.x = -200.0;

        if ( arg136_3.vect.x > 200.0 )
            arg136_3.vect.x = 200.0;

        arg136_3.vect.y = bact->height;

        arg136_3.vect.z = (bact->fly_dir.z * 200.0 * bact->fly_dir_length) / (v21 / v88);

        if ( arg136_3.vect.z < -200.0 )
            arg136_3.vect.z = -200.0;

        if ( arg136_3.vect.z > 200.0 )
            arg136_3.vect.z = 200.0;

        arg136_3.stPos = bact->old_pos;
        arg136_3.flags = 0;

        bact->ywo->ypaworld_func136(&arg136_3);

        if ( arg136_3.isect )
        {
            bact->target_dir.y = -(1.0 - arg136_3.tVal);
        }
        else
        {
            NC_STACK_ypabact *a4 = bact->ywo->getYW_userVehicle();

            if ( ((bact->secndTtype != BACT_TGT_TYPE_UNIT || (a4 != bact->secndT.pbact->self && bact->secndT.pbact->bact_type != BACT_TYPES_ROBO)) &&
                    (bact->primTtype != BACT_TGT_TYPE_UNIT || (a4 != bact->primT.pbact->self && bact->primT.pbact->bact_type != BACT_TYPES_ROBO)))
                    || bact->target_dir.y >= -0.01 )
            {
                if ( bact->target_dir.y < 0.15 )
                    bact->target_dir.y = 0.15;
            }
        }

        if ( bact->status_flg & (BACT_STFLAG_DODGE_LEFT | BACT_STFLAG_DODGE_RIGHT) )
            bact->target_dir.y = -0.2;

        if ( arg136_3.isect )
        {
            if ( arg136_3.tVal * bact->height < bact->radius && bact->fly_dir.y > 0.0 )
            {
                arg88.pos1 += arg136_3.skel->polygons[arg136_3.polyID].Normal();

                v18++;
            }
        }

        if ( v18 )
        {
            float v29 = v18;

            arg88.pos1 /= v29;

            Recoil(&arg88);
        }
        else
        {
            bact->status_flg &= ~BACT_STFLAG_LCRASH;
        }

        if ( bact->target_dir.y != 0.0 )
            bact->target_dir.normalise();

        float tmpsq = sqrt(POW2(arg136.vect.x) + POW2(arg136.vect.z));

        NDIV_CARRY(tmpsq);

        if ( bact->status_flg & BACT_STFLAG_DODGE_LEFT )
        {
            bact->target_dir.x = -arg136.vect.z / tmpsq;
            bact->target_dir.z = arg136.vect.x / tmpsq;
        }
        else if ( bact->status_flg & BACT_STFLAG_DODGE_RIGHT )
        {
            bact->target_dir.x = arg136.vect.z / tmpsq;
            bact->target_dir.z = -arg136.vect.x / tmpsq;
        }

        AI_layer3__sub1(bact, arg);

        /*if ( bact->status_flg & BACT_STFLAG_DODGE ) //Unused flag
            bact->fly_dir_length *= 0.95;*/

        bact->thraction = (0.85 - bact->target_dir.y) * bact->force;

        move_msg arg74;
        arg74.flag = 0;
        arg74.field_0 = arg->frameTime / 1000.0;

        Move(&arg74);

        AI_layer3__sub0(bact, arg->frameTime);

        bact_arg75 arg75;

        arg75.fperiod = v75;
        arg75.g_time = bact->clock;

        if ( bact->secndTtype == BACT_TGT_TYPE_UNIT )
        {
            arg75.target.pbact = bact->secndT.pbact;
            arg75.prio = 1;

            FightWithBact(&arg75);
        }
        else if ( bact->secndTtype == BACT_TGT_TYPE_CELL )
        {
            arg75.pos = bact->sencdTpos;
            arg75.target.pcell = bact->secndT.pcell;
            arg75.prio = 1;

            FightWithSect(&arg75);
        }
        else if ( bact->primTtype == BACT_TGT_TYPE_UNIT )
        {
            arg75.target.pbact = bact->primT.pbact;
            arg75.prio = 0;

            FightWithBact(&arg75);
        }
        else if ( bact->primTtype == BACT_TGT_TYPE_CELL )
        {
            arg75.pos = bact->primTpos;
            arg75.target.pcell = bact->primT.pcell;
            arg75.prio = 0;

            FightWithSect(&arg75);
        }
        else
        {
            if ( bact->status_flg & BACT_STFLAG_FIRE )
            {
                setState_msg arg78;
                arg78.unsetFlags = BACT_STFLAG_FIRE;
                arg78.newStatus = BACT_STATUS_NOPE;
                arg78.setFlags = 0;

                SetState(&arg78);
            }

            bact->status_flg &= ~(BACT_STFLAG_FIGHT_P | BACT_STFLAG_FIGHT_S);
        }
    }
    break;

    case BACT_STATUS_DEAD:
        DeadTimeUpdate(arg);
        break;

    case BACT_STATUS_IDLE:

        if ( bact->clock - bact->newtarget_time > 500 )
        {
            bact->newtarget_time = bact->clock;

            bact_arg110 arg110;
            arg110.tgType = bact->secndTtype;
            arg110.priority = 1;

            int v46 = TargetAssess(&arg110);

            arg110.priority = 0;
            arg110.tgType = bact->primTtype;
            int v48 = TargetAssess(&arg110);

            if ( v46 != 3 || v48 != 3 )
            {
                setTarget_msg arg67;

                if ( !v46 )
                {
                    arg67.tgt_type = BACT_TGT_TYPE_NONE;
                    arg67.priority = 1;
                    SetTarget(&arg67);
                }

                if ( !v48 )
                {
                    arg67.tgt_type = BACT_TGT_TYPE_CELL;
                    arg67.tgt_pos.x = bact->position.x;
                    arg67.tgt_pos.z = bact->position.z;
                    arg67.priority = 0;
                    SetTarget(&arg67);
                }

                if ( bact->primTtype || bact->secndTtype )
                {
                    setState_msg arg78;
                    arg78.unsetFlags = BACT_STFLAG_LAND;
                    arg78.setFlags = 0;
                    arg78.newStatus = BACT_STATUS_NORMAL;
                    SetState(&arg78);
                    break;
                }
            }
        }

        if ( bact->oflags & BACT_OFLAG_LANDONWAIT )
        {
            bact->thraction = 0;

            if ( bact->status_flg & BACT_STFLAG_LAND )
            {
                setState_msg arg78;
                arg78.unsetFlags = 0;
                arg78.setFlags = 0;
                arg78.newStatus = BACT_STATUS_IDLE;
                SetState(&arg78);

                ypaworld_arg136 v52;
                v52.stPos = bact->position;
                v52.vect = vec3d(0.0, bact->overeof + 50.0, 0.0);
                v52.flags = 0;

                bact->ywo->ypaworld_func136(&v52);

                if ( v52.isect )
                    bact->position.y = v52.isectPos.y - bact->overeof;
            }
            else
            {
                bact_arg86 v65;
                v65.field_one = 0;
                v65.field_two = arg->frameTime;

                CrashOrLand(&v65);
            }
        }
        break;

    case BACT_STATUS_CREATE:
        CreationTimeUpdate(arg);
        break;

    case BACT_STATUS_BEAM:
        BeamingTimeUpdate(arg);
        break;
    }
}

void NC_STACK_ypabact::User_layer(update_msg *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    bact->airconst = bact->airconst_static;

    float v106 = arg->frameTime / 1000.0;

    if ( bact->status == BACT_STATUS_NORMAL || bact->status == BACT_STATUS_IDLE )
    {

        bact->old_pos = bact->position;

        if ( bact->oflags & BACT_OFLAG_BACTCOLL )
        {
            if ( !(bact->status_flg & BACT_STFLAG_LAND) )
            {
                CollisionWithBact(arg->frameTime);
            }
        }

        float v98;

        if ( bact->status_flg & BACT_STFLAG_LAND )
            v98 = 0.1;
        else
            v98 = 1.0;

        setState_msg arg78;

        if ( v98 <= fabs(bact->fly_dir_length) )
        {
            if ( !(bact->status_flg & BACT_STFLAG_FIRE) )
            {
                arg78.newStatus = BACT_STATUS_NORMAL;
                arg78.unsetFlags = 0;
                arg78.setFlags = 0;
                SetState(&arg78);
            }

            bact->status_flg &= ~BACT_STFLAG_LAND;
        }
        else
        {
            ypaworld_arg136 arg136;

            arg136.stPos = bact->position;
            arg136.vect = vec3d(0.0, 0.0, 0.0);

            float v8;

            if ( bact->viewer_overeof <= bact->viewer_radius )
                v8 = bact->viewer_radius;
            else
                v8 = bact->viewer_overeof;

            arg136.flags = 0;
            arg136.vect.y = v8 * 1.5;

            bact->ywo->ypaworld_func136(&arg136);

            if ( arg136.isect && bact->thraction <= bact->mass * 9.80665 )
            {
                bact->fly_dir_length = 0;
                bact->status_flg |= BACT_STFLAG_LAND;
                bact->position.y = arg136.isectPos.y - bact->viewer_overeof;
                bact->thraction = bact->mass * 9.80665;
            }
            else
            {
                bact->status_flg &= ~BACT_STFLAG_LAND;
            }

            if ( bact->primTtype != BACT_TGT_TYPE_CELL || (bact->primTpos.XZ() - bact->position.XZ()).length() <= 800.0 )
            {
                if ( bact->status_flg & BACT_STFLAG_LAND )
                {
                    if ( !(bact->status_flg & BACT_STFLAG_FIRE) )
                    {
                        arg78.unsetFlags = 0;
                        arg78.setFlags = 0;
                        arg78.newStatus = BACT_STATUS_IDLE;
                        SetState(&arg78);
                    }
                }

                bact->status = BACT_STATUS_NORMAL;
            }
            else
            {
                bact->status = BACT_STATUS_IDLE;

                if ( bact->status_flg & BACT_STFLAG_LAND )
                {
                    if ( !(bact->status_flg & BACT_STFLAG_FIRE) )
                    {
                        arg78.newStatus = BACT_STATUS_IDLE;
                        arg78.unsetFlags = 0;
                        arg78.setFlags = 0;
                        SetState(&arg78);
                    }
                }
            }
        }

        float v110 = arg->inpt->sliders_vars[1] * bact->maxrot * v106;
        float v103 = -arg->inpt->sliders_vars[0] * bact->maxrot * v106;

        if ( (fabs(bact->fly_dir.y) > 0.98 || bact->fly_dir_length == 0.0) && bact->rotation.m11 > 0.996 && arg->inpt->sliders_vars[1] == 0.0 )
        {
            vec2d axisX = bact->rotation.AxisX().XZ();;

            if ( axisX.normalise() == 0.0 )
                ypa_log_out("Null on div occur %s:%d\n", __FILE__, __LINE__);

            vec2d axisZ = bact->rotation.AxisZ().XZ();

            if ( axisZ.normalise() == 0.0 )
                ypa_log_out("Null on div occur %s:%d\n", __FILE__, __LINE__);

            bact->rotation.SetX( vec3d::X0Z(axisX) );
            bact->rotation.SetY( vec3d(0.0, 1.0, 0.0) );
            bact->rotation.SetZ( vec3d::X0Z(axisZ) );
        }

//    float v84 = sqrt( POW2(bact->field_651.m20) + POW2(bact->field_651.m22) );
//    v84 /= sqrt( POW2(bact->field_651.m20) + POW2(bact->field_651.m21) + POW2(bact->field_651.m22) );
//
//    float v75 = v84;
//
//    if ( v84 > 1.0 )
//      v75 = 1.0;

        float v111 = clp_acos( bact->rotation.AxisX().XZ().length() / bact->rotation.AxisX().length() );

        if ( bact->rotation.m01 < 0.0 )
            v111 = -v111;

        if ( fabs(v111) < 0.01 )
            v111 = 0.0;

        float v36 = fabs(v111);

        float v101 = bact->heading_speed * v36 + bact->heading_speed * 0.25;

        float v102 = bact->maxrot * v106   *  v101;

        if ( v102 > v36 )
            v102 = v36;

        if ( v111 > 0.0 )
            v102 = -v102;

        float v104 = -v103 + v102;

        if ( fabs(v104 + v111) > 1.0 )
        {
            if ( v104 >= 0.0 )
                v104 = 1.0 - fabs(v111);
            else
                v104 = fabs(v111) - 1.0;
        }


        if ( fabs(v104) > bact->maxrot * 2.0 * v106 * v101 )
        {
            if ( v104 < 0.0 )
                v104 = bact->maxrot * -2.0 * v106 * v101;

            if ( v104 >= 0.0 )
                v104 = bact->maxrot * 2.0 * v106 * v101;
        }

        if ( fabs(v104) < 0.001 )
            v104 = 0.0;

        bact->rotation = mat3x3::RotateX(v110 * 0.5) * bact->rotation;
        bact->rotation = mat3x3::RotateZ(v104 * 0.5) * bact->rotation;
        bact->rotation = mat3x3::RotateY(v103 * 0.5) * bact->rotation;

        bact->thraction += bact->force * v106 * 0.5 * arg->inpt->sliders_vars[2];

        if ( bact->thraction < 0.0 )
            bact->thraction = 0;

        if ( bact->thraction > bact->force )
            bact->thraction = bact->force;

        float v99 = bact->thraction;

        float v47 = bact->pSector->height - bact->position.y;
        float v94 = bact->height_max_user * 0.8;

        if ( v47 > v94 )
        {
            float v91 = bact->mass * 9.80665 - bact->force;
            float v89 = bact->height_max_user * 0.2;
            float v86 = (v47 - v94) * v91 / v89;

            if ( bact->thraction > v86 )
                bact->thraction = v86;

            if ( bact->thraction < 0.0 )
                bact->thraction = 0;
        }

        bact_arg79 v61;

        v61.tgType = BACT_TGT_TYPE_DRCT;
        v61.tgt_pos = bact->rotation.AxisZ();

        bact_arg106 v64;
        v64.field_0 = 5;
        v64.field_4 = bact->rotation.AxisZ();

        if ( UserTargeting(&v64) )
        {
            v61.target.pbact = v64.ret_bact;
            v61.tgType = BACT_TGT_TYPE_UNIT;
        }

        if ( arg->inpt->but_flags & 1 || arg->inpt->but_flags & 2 )
        {
            v61.direction = vec3d(0.0, 0.0, 0.0);
            v61.weapon = bact->weapon;
            v61.g_time = bact->clock;

            if ( v61.g_time % 2 )
                v61.start_point.x = bact->fire_pos.x;
            else
                v61.start_point.x = -bact->fire_pos.x;

            v61.start_point.y = bact->fire_pos.y;
            v61.start_point.z = bact->fire_pos.z;
            v61.flags = ((arg->inpt->but_flags & 2) != 0) | 2;

            LaunchMissile(&v61);
        }

        if ( bact->mgun != -1 )
        {
            if ( bact->status_flg & BACT_STFLAG_FIRE )
            {
                if ( !(arg->inpt->but_flags & 4) )
                {
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
                    arg78.unsetFlags = 0;
                    arg78.newStatus = BACT_STATUS_NOPE;
                    arg78.setFlags = BACT_STFLAG_FIRE;

                    SetState(&arg78);
                }

                bact_arg105 arg105;

                arg105.field_0 = bact->rotation.AxisZ();
                arg105.field_C = v106;
                arg105.field_10 = bact->clock;

                FireMinigun(&arg105);
            }
        }

        if ( arg->inpt->but_flags & 8 )
        {
            HandBrake(arg);

            v99 = bact->thraction;
        }

        if ( bact->status_flg & BACT_STFLAG_LAND )
        {
            move_msg arg74;
            arg74.flag = 0;
            arg74.field_0 = v106;

            Move(&arg74);
        }
        else
        {
            vec3d v81(0.0, 0.0, 0.0);

            yw_137col v43[10];

            for (int i = 3; i >= 0; i--)
            {
                move_msg arg74;
                arg74.flag = 0;
                arg74.field_0 = v106;

                Move(&arg74);

                int v50 = 0;

                ypaworld_arg137 arg137;
                arg137.pos = bact->position;
                arg137.pos2 = bact->fly_dir;
                arg137.radius = 32.0;
                arg137.collisions = v43;
                arg137.field_30 = 0;
                arg137.coll_max = 10;

                bact->ywo->ypaworld_func137(&arg137);

                if ( arg137.coll_count )
                {
                    v81 = vec3d(0.0, 0.0, 0.0);

                    for (int j = arg137.coll_count - 1; j >= 0; j--)
                    {
                        yw_137col *v31 = &arg137.collisions[ j ];

                        v81 += v31->pos2;
                    }

                    bact_arg88 arg88;

                    float ln = v81.length();
                    if ( ln != 0.0 )
                        arg88.pos1 = v81 / ln;
                    else
                        arg88.pos1 = bact->fly_dir;



                    Recoil(&arg88);

                    v50 = 1;
                }

                if ( !v50 )
                {
                    ypaworld_arg136 arg136;
                    arg136.stPos = bact->old_pos;
                    arg136.vect = bact->position - bact->old_pos;
                    arg136.flags = 0;

                    bact->ywo->ypaworld_func136(&arg136);

                    if ( arg136.isect )
                    {
                        bact_arg88 arg88;
                        arg88.pos1 = arg136.skel->polygons[arg136.polyID].Normal();

                        Recoil(&arg88);

                        v50 = 1;
                    }
                }

                if ( !v50 )
                {
                    bact->status_flg &= ~BACT_STFLAG_LCRASH;
                    break;
                }

                if ( !(bact->soundcarrier.samples_data[5].flags & 2) )
                {
                    if ( !(bact->status_flg & BACT_STFLAG_LCRASH) )
                    {
                        bact->status_flg |= BACT_STFLAG_LCRASH;

                        startSound(&bact->soundcarrier, 5);

                        yw_arg180 arg180;
                        arg180.effects_type = 5;
                        arg180.field_4 = 1.0;
                        arg180.field_8 = v81.x * 10.0 + bact->position.x;
                        arg180.field_C = v81.z * 10.0 + bact->position.z;

                        bact->ywo->ypaworld_func180(&arg180);
                    }
                }
            }
        }

        bact->thraction = v99;
    }
    else if ( bact->status == BACT_STATUS_DEAD )
    {
        DeadTimeUpdate(arg);
    }
}

void NC_STACK_ypabact::AddSubject(NC_STACK_ypabact *kid)
{
    __NC_STACK_ypabact *bact = &ypabact;

    newMaster_msg arg73;

    arg73.bacto = this;
    arg73.bact = bact;
    arg73.list = &bact->subjects_list;

    kid->SetNewMaster(&arg73);
}

void NC_STACK_ypabact::SetNewMaster(newMaster_msg *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    if ( bact->parent_bacto )
        Remove(&bact->subject_node);

    AddHead(arg->list, &bact->subject_node);

    bact->parent_bacto = arg->bacto;
    bact->parent_bact = arg->bact;
}

void NC_STACK_ypabact::Move(move_msg *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    bact->old_pos = bact->position;

    float weight;

    if ( bact->status == BACT_STATUS_DEAD )
        weight = bact->mass * 39.2266;
    else
        weight = bact->mass * 9.80665;

    float thraction;

    vec3d v54;

    if ( arg->flag & 1 )
    {
        v54 = vec3d(0.0, 0.0, 0.0);

        thraction = 0.0;
    }
    else
    {
        v54 = -bact->rotation.AxisY();

        thraction = bact->thraction;

        if ( bact->oflags & BACT_OFLAG_USERINPT )
        {
            v54.x = fSign(v54.x) * sqrt( fabs(v54.x) );
            v54.y = fSign(v54.y) * sqrt( fabs(v54.y) );
            v54.z = fSign(v54.z) * sqrt( fabs(v54.z) );
        }
    }

    float v11 = bact->fly_dir_length * bact->airconst;

    vec3d v41 = vec3d(0.0, weight, 0.0) + v54 * thraction - bact->fly_dir * v11;

    float len = v41.length();

    if ( bact->oflags & BACT_OFLAG_USERINPT )
    {
        if ( v41.y >= 0.0 )
            v41.y *= 3.0;
        else
            v41.y *= 5.0;
    }

    if ( len > 0.0 )
    {
        //vec3d v42 = bact->fly_dir * bact->fly_dir_length + (v41 / len) * (len / bact->mass * arg->field_0);
        vec3d v42 = bact->fly_dir * bact->fly_dir_length + v41 / bact->mass * arg->field_0;

        bact->fly_dir_length = v42.length();

        if ( bact->fly_dir_length > 0.0 )
            bact->fly_dir = v42 / bact->fly_dir_length;
    }

    if ( fabs(bact->fly_dir_length) > 0.1 )
        bact->position += bact->fly_dir * bact->fly_dir_length * arg->field_0 * 6.0;

    CorrectPositionInLevelBox(NULL);

    bact->soundcarrier.samples_data[0].pitch = bact->pitch;
    bact->soundcarrier.samples_data[0].volume = bact->volume;

    float v50;
    if ( bact->pitch_max <= -0.8 )
        v50 = 1.2;
    else
        v50 = bact->pitch_max;

    float v30 = fabs(bact->fly_dir_length) * v50;
    float v31 = bact->force * bact->force - bact->mass * 100.0 * bact->mass;

    float v43 = v30 / (sqrt(v31) / bact->airconst_static);

    if ( v43 > v50 )
        v43 = v50;

    if ( bact->soundcarrier.samples_data[0].psampl )
        bact->soundcarrier.samples_data[0].pitch += (bact->soundcarrier.samples_data[0].psampl->SampleRate + bact->soundcarrier.samples_data[0].pitch) * v43;
}

void NC_STACK_ypabact::FightWithBact(bact_arg75 *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    arg->pos = arg->target.pbact->position;

    vec3d v40 = arg->target.pbact->position - bact->position;
    float v45 = v40.normalise();

    bact_arg110 arg110;

    vec3d *fui;
    int v54 = 0;
    int v51 = 0;

    if ( bact->secndT.pbact == arg->target.pbact )
    {
        fui = &bact->secndT.pbact->position;
        arg110.priority = 1;
        v54 = 1;
    }
    else
    {
        fui = &bact->primT.pbact->position;
        arg110.priority = 0;
        v51 = 1;
    }

    NC_STACK_ypabact *a4 = bact->ywo->getYW_userHostStation();

    int v16 = 0;

    if ( bact->clock - bact->assess_time <= 500 && bact->clock >= 500 )
    {
        v16 = bact->atk_ret;
    }
    else
    {
        bact->assess_time = bact->clock;

        arg110.tgType = BACT_TGT_TYPE_UNIT;
        v16 = TargetAssess(&arg110);

        bact->atk_ret = v16;
    }

    if ( v16 == 2 )
    {
        float v19 = fui->x - bact->position.x;
        float v18 = fui->z - bact->position.z;

        float v48 = sqrt( POW2(v19) + POW2(v18) );

        if ( bact->status_flg & BACT_STFLAG_APPROACH )
        {
            bact->status_flg &= ~BACT_STFLAG_ATTACK;

            if ( (bact->position.x < 1320.0 || bact->position.z > -1320.0 || bact->position.x > bact->wrldX - 1320.0 || bact->position.z < bact->wrldY + 1320.0) || bact->adist_bact < v48 )
            {
                bact->status_flg &= ~BACT_STFLAG_APPROACH;
            }
            else
            {
                bact->AI_time2 = bact->clock;
                bact->AI_time1 = bact->clock;
            }
        }
        else
        {
            if ( bact->sdist_bact <= v48 )
            {
                if ( bact->adist_sector <= v48 )
                    bact->status_flg &= ~BACT_STFLAG_ATTACK;
                else
                    bact->status_flg |= BACT_STFLAG_ATTACK;
            }
            else
            {
                bact->status_flg &= ~BACT_STFLAG_ATTACK;

                /*if ( bact->field_3D1 == 2 || (arg->g_time & 1 && bact->field_3D1 == 3) )
                {
                    bact->target_vec.x = bact->fly_dir.x;
                    bact->target_vec.z = bact->fly_dir.z;
                }
                else*/
                {
                    bact->target_vec.x = -bact->fly_dir.x;
                    bact->target_vec.z = -bact->fly_dir.z;
                }

                bact->AI_time2 = bact->clock;
                bact->AI_time1 = bact->clock;
                bact->status_flg |= BACT_STFLAG_APPROACH;
            }
        }
    }
    else
    {
        bact->status_flg &= ~(BACT_STFLAG_APPROACH | BACT_STFLAG_ATTACK);
    }

    if ( v16 )
    {
        if ( v16 == 3 )
        {
            bact->status_flg &= ~BACT_STFLAG_APPROACH;

            if ( bact->status_flg & BACT_STFLAG_FIRE )
            {
                setState_msg arg78;
                arg78.setFlags = 0;
                arg78.newStatus = BACT_STATUS_NOPE;
                arg78.unsetFlags = BACT_STFLAG_FIRE;

                SetState(&arg78);
            }

            if ( bact->secndT.pbact == arg->target.pbact )
            {
                bact->status_flg &= ~BACT_STFLAG_FIGHT_S;

                setTarget_msg arg67;
                arg67.tgt_type = BACT_TGT_TYPE_NONE;
                arg67.priority = 1;

                SetTarget(&arg67);

                v54 = 0;
            }
            else
            {
                bact->status_flg &= ~BACT_STFLAG_FIGHT_P;

                if ( (bact->parent_bacto == bact->host_station && bact->host_station && bact->host_station == a4) && bact->status != BACT_STATUS_IDLE && !(bact->status_flg & BACT_STFLAG_ESCAPE) )
                {
                    robo_arg134 arg134;
                    arg134.unit = bact;
                    arg134.field_4 = 1;
                    arg134.field_10 = 0;
                    arg134.field_C = 0;
                    arg134.field_8 = 0;
                    arg134.field_14 = 32;

                    bact->host_station->placeMessage(&arg134);
                }

                setState_msg arg78;
                arg78.unsetFlags = 0;
                arg78.setFlags = 0;
                arg78.newStatus = BACT_STATUS_NORMAL;

                SetState(&arg78);

                bact->status = BACT_STATUS_IDLE;
            }
        }

        if ( v16 == 1 )
        {
            if ( bact->status_flg & BACT_STFLAG_FIRE )
            {
                setState_msg arg78;
                arg78.setFlags = 0;
                arg78.newStatus = BACT_STATUS_NOPE;
                arg78.unsetFlags = BACT_STFLAG_FIRE;

                SetState(&arg78);
            }
        }

        if ( v16 == 2 )
        {
            bact_arg101 arg101;
            arg101.pos = arg->target.pbact->position;
            arg101.unkn = 2;
            arg101.radius = arg->target.pbact->radius;

            if ( CheckFireAI(&arg101) )
            {
                if ( v54 )
                    bact->status_flg |= BACT_STFLAG_FIGHT_S;
                else
                    bact->status_flg |= BACT_STFLAG_FIGHT_P;

                vec3d rotZ = bact->rotation.AxisZ();

                bact_arg79 arg79;

                arg79.direction.x = rotZ.x;

                if ( bact->bact_type == BACT_TYPES_TANK )
                    arg79.direction.y = v40.y;
                else
                    arg79.direction.y = rotZ.y - bact->gun_angle;

                arg79.direction.z = rotZ.z;
                arg79.tgType = BACT_TGT_TYPE_UNIT;
                arg79.target.pbact = arg->target.pbact;
                arg79.tgt_pos = arg->pos;
                arg79.weapon = bact->weapon;
                arg79.g_time = bact->clock;

                if ( arg->g_time & 1 )
                    arg79.start_point.x = bact->fire_pos.x;
                else
                    arg79.start_point.x = -bact->fire_pos.x;

                arg79.start_point.y = bact->fire_pos.y;
                arg79.start_point.z = bact->fire_pos.z;
                arg79.flags = 0;

                LaunchMissile(&arg79);
            }
            else
            {
                bact->status_flg &= ~BACT_STFLAG_ATTACK;
            }

            if ( v45 < 1000.0 &&   bact->mgun != -1 &&   v40.dot(bact->rotation.AxisZ()) > 0.85 )
            {
                if ( v54 )
                    bact->status_flg |= BACT_STFLAG_FIGHT_S;
                else
                    bact->status_flg |= BACT_STFLAG_FIGHT_P;

                if ( !(bact->status_flg & BACT_STFLAG_FIRE) )
                {
                    setState_msg arg78;
                    arg78.unsetFlags = 0;
                    arg78.newStatus = BACT_STATUS_NOPE;
                    arg78.setFlags = BACT_STFLAG_FIRE;

                    SetState(&arg78);
                }

                bact_arg105 arg105;

                arg105.field_C = arg->fperiod;
                arg105.field_10 = bact->clock;
                arg105.field_0 = bact->rotation.AxisZ();

                FireMinigun(&arg105);
            }
            else if ( bact->status_flg & BACT_STFLAG_FIRE )
            {
                setState_msg arg78;
                arg78.setFlags = 0;
                arg78.newStatus = BACT_STATUS_NOPE;
                arg78.unsetFlags = BACT_STFLAG_FIRE;

                SetState(&arg78);
            }
        }
    }
    else
    {
        if ( v51 )
        {
            bact->status_flg &= ~BACT_STFLAG_FIGHT_P;

            setTarget_msg arg67;
            arg67.priority = 0;
            arg67.tgt_type = BACT_TGT_TYPE_CELL;
            arg67.tgt_pos = bact->primTpos;

            SetTarget(&arg67);
        }

        if ( v54 )
        {
            bact->status_flg &= ~BACT_STFLAG_FIGHT_S;

            setTarget_msg arg67;
            arg67.priority = 1;
            arg67.tgt_type = BACT_TGT_TYPE_NONE;

            SetTarget(&arg67);
        }

        bact->status_flg &= ~BACT_STFLAG_APPROACH;

        if ( bact->status_flg & BACT_STFLAG_FIRE )
        {
            setState_msg arg78;
            arg78.setFlags = 0;
            arg78.newStatus = BACT_STATUS_NOPE;
            arg78.unsetFlags = BACT_STFLAG_FIRE;

            SetState(&arg78);
        }
    }
}

void NC_STACK_ypabact::FightWithSect(bact_arg75 *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    int v64 = 0;
    int v68 = 0;

    vec3d *fui;

    bact_arg110 arg110;

    if ( bact->secndT.pcell == arg->target.pcell )
    {
        fui = &bact->sencdTpos;
        v64 = 1;

        arg110.priority = 1;
    }
    else
    {
        fui = &bact->primTpos;
        v68 = 1;

        arg110.priority = 0;
    }

    NC_STACK_ypabact *a4 = bact->ywo->getYW_userHostStation();

    int v65 = bact->parent_bacto == bact->host_station && bact->host_station && bact->host_station == a4;

    float v12 = bact->position.z - fui->z;
    float v13 = bact->position.x - fui->x;

    float v62 = sqrt(POW2(v12) + POW2(v13));

    int v15 = 0;

    if ( bact->clock - bact->assess_time <= 500 && bact->clock >= 500 )
    {
        v15 = bact->atk_ret;
    }
    else
    {
        bact->assess_time = bact->clock;

        arg110.tgType = BACT_TGT_TYPE_CELL;
        v15 = TargetAssess(&arg110);

        bact->atk_ret = v15;
    }

    if ( v15 == 2 )
    {
        float v17 = fui->z - bact->position.z;
        float v18 = fui->x - bact->position.x;

        float v63 = sqrt( POW2(v17) + POW2(v18) );

        if ( bact->status_flg & BACT_STFLAG_APPROACH )
        {
            bact->status_flg &= ~BACT_STFLAG_ATTACK;

            if ( (bact->position.x < 1320 || bact->position.z > -1320.0 || bact->position.x > bact->wrldX - 1320.0 || bact->position.z < bact->wrldY + 1320.0) || bact->adist_sector < v63 )
            {
                bact->status_flg &= ~BACT_STFLAG_APPROACH;
            }
            else
            {
                bact->AI_time2 = bact->clock;
                bact->AI_time1 = bact->clock;
            }
        }
        else if ( bact->sdist_sector <= v63 )
        {
            if ( bact->adist_sector <= v63 )
                bact->status_flg &= ~BACT_STFLAG_ATTACK;
            else
                bact->status_flg |= BACT_STFLAG_ATTACK;
        }
        else
        {
            bact->status_flg &= ~BACT_STFLAG_ATTACK;

            /*if ( bact->field_3D1 == 2 || (arg->g_time & 1 && bact->field_3D1 == 3) )
            {
                bact->target_vec.x = bact->fly_dir.x;
                bact->target_vec.z = bact->fly_dir.z;
                bact->target_vec.y = bact->fly_dir.y;
            }
            else*/
            {
                bact->target_vec = -bact->fly_dir;
            }

            bact->AI_time2 = bact->clock;
            bact->AI_time1 = bact->clock;

            bact->status_flg |= BACT_STFLAG_APPROACH;
        }
    }
    else
    {
        bact->status_flg &= ~(BACT_STFLAG_APPROACH | BACT_STFLAG_ATTACK);
    }

    if ( bact->status_flg & BACT_STFLAG_FIRE )
    {
        setState_msg arg78;
        arg78.unsetFlags = BACT_STFLAG_FIRE;
        arg78.setFlags = 0;
        arg78.newStatus = BACT_STATUS_NOPE;

        SetState(&arg78);
    }

    if ( v15 )
    {
        if ( v15 == 3 )
        {
            bact->status_flg &= ~BACT_STFLAG_APPROACH;

            if ( v64 )
            {
                bact->status_flg &= ~BACT_STFLAG_FIGHT_S;

                if ( v65 && bact->secndT.pcell != bact->primT.pcell )
                {
                    robo_arg134 arg134;

                    arg134.field_4 = 2;
                    arg134.field_8 = bact->sencdTpos.x / 1200.0;
                    arg134.field_C = -bact->sencdTpos.z / 1200.0;
                    arg134.field_10 = 0;
                    arg134.field_14 = 22;
                    arg134.unit = bact;

                    bact->host_station->placeMessage(&arg134);
                }

                setTarget_msg arg67;
                arg67.priority = 1;
                arg67.tgt_type = BACT_TGT_TYPE_NONE;

                SetTarget(&arg67);

                v64 = 0;
            }
            else
            {
                bact->status_flg &= ~BACT_STFLAG_FIGHT_P;

                if ( v65 && bact->status != BACT_STATUS_IDLE )
                {
                    robo_arg134 arg134;

                    arg134.field_10 = 0;
                    arg134.field_C = 0;
                    arg134.field_8 = 0;
                    arg134.unit = bact;
                    arg134.field_4 = 1;
                    arg134.field_14 = 32;

                    bact->host_station->placeMessage(&arg134);
                }

                bact->status = BACT_STATUS_IDLE;
            }
        }

        if ( v15 == 2 )
        {
            if ( v68 )
            {
                if ( v62 < 1200.0 )
                {
                    if ( !(bact->status_flg & BACT_STFLAG_FIGHT_P) && v65 && bact->secndT.pcell != bact->primT.pcell )
                    {
                        robo_arg134 arg134;

                        arg134.field_4 = 3;
                        arg134.field_8 = bact->primTpos.x / 1200.0;
                        arg134.field_C = -bact->primTpos.z / 1200.0;
                        arg134.unit = bact;
                        arg134.field_10 = 0;
                        arg134.field_14 = 20;

                        bact->host_station->placeMessage(&arg134);
                    }

                    bact->status_flg |= BACT_STFLAG_FIGHT_P;
                }

                GetBestSectorPart(&bact->primTpos);

                arg->pos = bact->primTpos;
            }

            if ( v64 )
            {
                if ( v62 < 1200.0 )
                {
                    if ( v65 && !(bact->status_flg & BACT_STFLAG_FIGHT_S) )
                    {
                        robo_arg134 arg134;

                        arg134.field_4 = 3;
                        arg134.field_8 = bact->sencdTpos.x / 1200.0;
                        arg134.field_C = -bact->sencdTpos.z / 1200.0;
                        arg134.unit = bact;
                        arg134.field_10 = 0;
                        arg134.field_14 = 20;

                        bact->host_station->placeMessage(&arg134);
                    }

                    bact->status_flg |= BACT_STFLAG_FIGHT_S;
                }

                GetBestSectorPart(&bact->sencdTpos);

                arg->pos = bact->sencdTpos;
            }

            bact_arg101 arg101;
            arg101.unkn = 1;
            arg101.pos = arg->pos;

            if ( CheckFireAI(&arg101) )
            {
                vec3d tmp = bact->position + bact->fire_pos - arg->pos;

                float v60 = tmp.length();

                if ( v60 < 0.01 )
                    v60 = 0.01;

                if ( v64 )
                    bact->status_flg |= BACT_STFLAG_FIGHT_S;
                else
                    bact->status_flg |= BACT_STFLAG_FIGHT_P;

                bact_arg79 arg79;

                arg79.direction = -(bact->position + bact->fire_pos - arg->pos) / v60;
                arg79.tgType = BACT_TGT_TYPE_CELL;
                arg79.target.pbact = arg->target.pbact;
                arg79.tgt_pos = arg->pos;
                arg79.weapon = bact->weapon;
                arg79.g_time = bact->clock;

                if ( arg->g_time & 1 )
                    arg79.start_point.x = bact->fire_pos.x;
                else
                    arg79.start_point.x = -bact->fire_pos.x;

                arg79.start_point.y = bact->fire_pos.y;
                arg79.start_point.z = bact->fire_pos.z;
                arg79.flags = 0;

                LaunchMissile(&arg79);
            }
            else
            {
                bact->status_flg &= ~BACT_STFLAG_ATTACK;
            }
        }
    }
    else
    {
        bact->status_flg &= ~BACT_STFLAG_APPROACH;

        if ( v68 )
        {
            if ( v65 )
            {
                robo_arg134 arg134;

                arg134.unit = bact;
                arg134.field_4 = 4;
                arg134.field_8 = bact->primTpos.x / 1200.0;
                arg134.field_C = -bact->primTpos.z / 1200.0;
                arg134.field_14 = 18;
                arg134.field_10 = 0;

                bact->host_station->placeMessage(&arg134);
            }

            bact->status_flg &= ~BACT_STFLAG_FIGHT_P;

            setTarget_msg arg67;
            arg67.tgt_type = BACT_TGT_TYPE_CELL;
            arg67.tgt_pos.x = bact->position.x;
            arg67.tgt_pos.z = bact->position.z;
            arg67.priority = 0;

            SetTarget(&arg67);
        }

        if ( v64 )
        {
            if ( v65 )
            {
                robo_arg134 arg134;

                arg134.unit = bact;
                arg134.field_4 = 4;
                arg134.field_8 = bact->sencdTpos.x / 1200.0;
                arg134.field_10 = 0;
                arg134.field_14 = 18;
                arg134.field_C = -bact->sencdTpos.z / 1200.0;

                bact->host_station->placeMessage(&arg134);
            }

            bact->status_flg &= ~BACT_STFLAG_FIGHT_S;

            setTarget_msg arg67;
            arg67.tgt_type = BACT_TGT_TYPE_NONE;
            arg67.priority = 1;
            SetTarget(&arg67);
        }
    }
}

void ypabact_func77__sub0(__NC_STACK_ypabact *bact1, __NC_STACK_ypabact *bact2)
{
    if ( bact1->status_flg & BACT_STFLAG_WAYPOINT )
    {
        for (int i = 0; i < 32; i++)
            bact2->waypoints[i] = bact1->waypoints[i];

        bact2->status_flg |= BACT_STFLAG_WAYPOINT;

        if ( bact1->status_flg & BACT_STFLAG_WAYPOINTCCL )
            bact2->status_flg |= BACT_STFLAG_WAYPOINTCCL;
        else
            bact2->status_flg &= ~BACT_STFLAG_WAYPOINTCCL;

        bact2->waypoints_count = bact1->waypoints_count;
        bact2->current_waypoint = bact1->current_waypoint;
    }
}

void NC_STACK_ypabact::Die()
{
    __NC_STACK_ypabact *bact = &ypabact;

    if ( !(bact->status_flg & BACT_STFLAG_DEATH1) )
    {
        int maxy = bact->ywo->getYW_mapMaxY();
        int maxx = bact->ywo->getYW_mapMaxX();

        uamessage_dead deadMsg;
        deadMsg.msgID = UAMSG_DEAD;
        deadMsg.owner = bact->owner;
        deadMsg.id = bact->gid;
        deadMsg.newParent = 0;
        deadMsg.landed = 0;
        deadMsg.classID = bact->bact_type;

        if ( bact->killer )
            deadMsg.killer = bact->killer->gid;
        else
            deadMsg.killer = 0;

        deadMsg.killerOwner = bact->killer_owner;

        _NC_STACK_ypaworld *yw = &bact->ywo->stack__ypaworld;

        bact_node *v74 = NULL;

        bact_node *v4 = (bact_node *)bact->subjects_list.head;

        if (v4->next)
        {

            while (v4->next)
            {
                bact_node *next_node = (bact_node *)v4->next;

                if ( v4->bact->status == BACT_STATUS_DEAD )
                {
                    if ( (size_t)bact->parent_bacto <= 2 )
                        bact->ywo->ypaworld_func134(v4->bacto);
                    else
                        bact->parent_bacto->AddSubject(v4->bacto);

                    v4->bact->status_flg |= BACT_STFLAG_NOMSG;
                }
                else
                {
                    float v9 = v4->bact->position.x - bact->position.x;
                    float v10 = v4->bact->position.z - bact->position.z;

                    float v73 = POW2(v9) + POW2(v10);

                    float v14;

                    if ( v74 )
                        v14 = POW2(v74->bact->position.x - bact->position.x) + POW2(v74->bact->position.z - bact->position.z);
                    else
                        v14 = (POW2(maxx) + POW2(maxy)) * 1200.0 * 1200.0;

                    if ( v4->bact->bact_type == BACT_TYPES_UFO )
                        v73 = (POW2(maxx) + POW2(maxy)) * 1200.0 * 1200.0 - 1000.0;

                    if ( v73 <= v14 )
                        v74 = v4;
                }

                v4 = next_node;
            }

            if ( v74 )
            {
                if ( (size_t)bact->parent_bacto != 1 )
                    bact->parent_bacto->AddSubject(v74->bacto);
                else
                    bact->ywo->ypaworld_func134(v74->bacto);

                while ( 1 )
                {
                    bact_node *v16 = (bact_node *)bact->subjects_list.head;

                    if ( !v16->next )
                        break;

                    v74->bacto->AddSubject(v16->bacto);
                }

                setTarget_msg arg67;
                arg67.tgt_pos = bact->primTpos;
                arg67.tgt.pbact = bact->primT.pbact;
                arg67.tgt_type = bact->primTtype;
                arg67.priority = 0;

                v74->bacto->SetTarget(&arg67);

                ypabact_func77__sub0(bact, v74->bact);

                v74->bact->commandID = bact->commandID;
                v74->bact->aggr = bact->aggr;

                if ( yw->isNetGame )
                {
                    if (bact->owner)
                        deadMsg.newParent = v74->bact->gid;
                }
            }
            else
            {
                bact_node *v64 = (bact_node *)bact->subjects_list.head;

                while(v64->next)
                {
                    bact_node *v21 = (bact_node *)v64->bact->subjects_list.head;

                    while (v21->next)
                    {
                        bact_node *next2 = (bact_node *)v21->next;

                        bact->ywo->ypaworld_func134(v21->bacto);

                        if ( v21->bact->status != BACT_STATUS_DEAD )
                            ypa_log_out("Scheisse, da hфngt noch ein Lebendiger unter der Leiche! owner %d, state %d, class %d\n", v21->bact->owner, v21->bact->status, bact->bact_type);

                        v21 = next2;
                    }

                    bact_node *next1 = (bact_node *)v64->next;

                    bact->ywo->ypaworld_func134(v64->bacto);

                    v64 = next1;
                }
            }
        }

        NC_STACK_ypabact *v76 = bact->ywo->getYW_userHostStation();

        if ( !v74 && bact->host_station == bact->parent_bacto && !(bact->status_flg & BACT_STFLAG_NOMSG) )
        {
            robo_arg134 v53;

            if ( v76 == bact->host_station )
            {
                if ( bact->bact_type == BACT_TYPES_GUN )
                {
                    if ( bact->weapon != -1 || -1 != bact->mgun )
                    {
                        v53.field_14 = 80;
                        v53.field_4 = 31;
                    }
                    else
                    {
                        v53.field_14 = 80;
                        v53.field_4 = 32;
                    }

                    v53.field_10 = 0;
                    v53.field_C = 0;
                    v53.field_8 = 0;
                    v53.unit = bact;

                    bact->host_station->placeMessage(&v53);
                }
                else
                {
                    if ( !(bact->status_flg & BACT_STFLAG_CLEAN) )
                    {
                        v53.field_8 = bact->commandID;
                        v53.unit = bact;
                        v53.field_10 = 0;
                        v53.field_14 = 44;
                        v53.field_C = 0;
                        v53.field_4 = 8;

                        bact->host_station->placeMessage(&v53);
                    }
                }
            }
            else
            {
                if ( bact->killer && v76 == bact->killer->host_station )
                {
                    v53.field_4 = 5;
                    v53.unit = bact->killer;
                    v53.field_8 = bact->primT_cmdID;
                    v53.field_10 = 0;
                    v53.field_C = 0;
                    v53.field_14 = 36;

                    bact->host_station->placeMessage(&v53);
                }
            }

        }

        while ( 1 )
        {
            bact_node *v30 = (bact_node *)RemHead(&bact->attackers_list);

            if ( !v30 )
                break;

            bact_node *v68 = v30->bacto->getBACT_primAttackNode();
            bact_node *v69 = v30->bacto->getBACT_secnAttackNode();

            if ( v30 == v68 )
            {
                v30->bact->primT.pbact = NULL;
                v30->bact->primTtype = BACT_TGT_TYPE_NONE;
                v30->bact->assess_time = 0;
            }
            else if ( v30 == v69 )
            {
                v30->bact->secndT.pbact = NULL;
                v30->bact->secndTtype = BACT_TGT_TYPE_NONE;
                v30->bact->assess_time = 0;
            }
            else
            {
                ypa_log_out("Hein BlЎd\n");
            }
        }

        if ( (size_t)bact->parent_bacto <= 2 )
        {
            while ( 1 )
            {
                bact_node *v36 = (bact_node *)RemHead(&bact->missiles_list);

                if ( !v36 )
                    break;

                NC_STACK_ypamissile *miss = dynamic_cast<NC_STACK_ypamissile *>(v36->bacto);

                miss->ypamissile_func128(NULL);

                setState_msg arg119;
                arg119.newStatus = BACT_STATUS_DEAD;
                arg119.unsetFlags = 0;
                arg119.setFlags = 0;
                miss->SetStateInternal(&arg119);

                setTarget_msg arg67;
                arg67.tgt_type = BACT_TGT_TYPE_NONE;
                arg67.priority = 0;
                miss->SetTarget(&arg67);

                v36->bact->parent_bacto = NULL;

                bact->ywo->ypaworld_func144(miss);
            }
        }
        else
        {
            while ( 1 )
            {
                bact_node *v33 = (bact_node *)RemHead(&bact->missiles_list);

                if ( !v33 )
                    break;

                AddTail(&bact->parent_bacto->ypabact.missiles_list, v33);

                NC_STACK_ypamissile *miss = dynamic_cast<NC_STACK_ypamissile *>(v33->bacto);

                miss->setMISS_launcher( bact->parent_bact );
            }
        }

        if ( bact->secndTtype == BACT_TGT_TYPE_UNIT )
            Remove(&bact->attack_node_scnd);

        if ( bact->primTtype == BACT_TGT_TYPE_UNIT )
            Remove(&bact->attack_node_prim);


        bact->secndTtype = BACT_TGT_TYPE_NONE;
        bact->primTtype = BACT_TGT_TYPE_NONE;

        if ( bact->attack_node_scnd.next && bact->attack_node_scnd.prev )
        {
            ypa_log_out("ALARM!!! st-Node noch in liste!!! owner %d, class %d\n", bact->owner, bact->bact_type);
            Remove(&bact->attack_node_scnd);
        }

        if ( bact->attack_node_prim.next && bact->attack_node_prim.prev )
        {
            ypa_log_out("ALARM!!! pt-Node noch in liste!!! owner %d, class %d\n", bact->owner, bact->bact_type);
            Remove(&bact->attack_node_prim);
        }

        bact->status = BACT_STATUS_DEAD;
        bact->commandID = 0;
        bact->status_flg |= BACT_STFLAG_DEATH1;
        bact->dead_time = bact->clock;

        if ( bact->status_flg & BACT_STFLAG_LAND )
        {
            if ( bact->vp_active == 1 || bact->vp_active == 6 )
            {
                setState_msg arg119;
                arg119.unsetFlags = 0;
                arg119.newStatus = BACT_STATUS_NOPE;
                arg119.setFlags = BACT_STFLAG_DEATH2;

                SetStateInternal(&arg119);

                if ( yw->isNetGame )
                {
                    if (bact->owner)
                        deadMsg.landed = 1;
                }
            }
        }

        if ( bact->oflags & BACT_OFLAG_USERINPT )
        {
            if ( !(bact->oflags & BACT_OFLAG_VIEWER) )
            {
                if ( bact->parent_bact )
                    setBACT_inputting(0);
            }
        }

        if ( yw->isNetGame )
        {
            if ( bact->owner )
            {
                if ( bact->bact_type != BACT_TYPES_ROBO )
                {
                    yw_arg181 arg181;
                    arg181.data = &deadMsg;
                    arg181.recvFlags = 2;
                    arg181.dataSize = sizeof(deadMsg);
                    arg181.recvID = 0;
                    arg181.garant = 1;

                    yw->self_full->ypaworld_func181(&arg181);
                }
            }
        }

        if ( bact->owner )
        {
            if ( !(bact->status_flg & BACT_STFLAG_CLEAN) )
            {
                if ( bact->killer )
                {
                    yw_arg184 arg184;
                    arg184.type = 3;
                    arg184.t34.field_1 = 8 * bact->killer->owner | bact->owner;

                    int v70 = bact->killer->self->getBACT_viewer();

                    if ( v70 || bact->killer->status_flg & BACT_STFLAG_ISVIEW )
                        arg184.t34.field_1 |= 0x80;

                    v70 = bact->self->getBACT_viewer();

                    if ( v70 || bact->status_flg & BACT_STFLAG_ISVIEW )
                        arg184.t34.field_1 |= 0x40;

                    arg184.t34.field_2 = bact->vehicleID;

                    if ( bact->bact_type == BACT_TYPES_ROBO )
                        arg184.t34.field_2 |= 0x8000;

                    arg184.t34.field_4 = bact->position.x * 256.0 / bact->wrldX;
                    arg184.t34.field_5 = bact->position.z * 256.0 / bact->wrldY;

                    bact->ywo->ypaworld_func184(&arg184);
                }
            }
        }
    }
}

void NC_STACK_ypabact::SetState(setState_msg *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    if ( (bact->bact_type == BACT_TYPES_TANK || bact->bact_type == BACT_TYPES_CAR) && arg->newStatus == 2 )
    {
        setState_msg newarg;
        newarg.unsetFlags = 0;
        newarg.newStatus = BACT_STATUS_NOPE;
        newarg.setFlags = BACT_STFLAG_DEATH2;

        SetState(&newarg);
    }
    else
    {
        int v6 = SetStateInternal(arg);

        _NC_STACK_ypaworld *yw = &bact->ywo->stack__ypaworld;

        if ( yw->isNetGame )
        {
            if ( v6 && bact->owner && bact->bact_type != BACT_TYPES_MISSLE )
            {
                uamessage_setState ssMsg;
                ssMsg.msgID = UAMSG_SETSTATE;
                ssMsg.owner = bact->owner;
                ssMsg.id = bact->gid;
                ssMsg.newStatus = arg->newStatus;
                ssMsg.setFlags = arg->setFlags;
                ssMsg.unsetFlags = arg->unsetFlags;
                ssMsg.classID = bact->bact_type;

                yw_arg181 v9;
                v9.recvFlags = 2;
                v9.dataSize = sizeof(ssMsg);
                v9.garant = 1;
                v9.recvID = 0;
                v9.data = &ssMsg;

                bact->ywo->ypaworld_func181(&v9);
            }
        }
    }
}

size_t NC_STACK_ypabact::LaunchMissile(bact_arg79 *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    NC_STACK_ypamissile *wobj = NULL;

    WeapProto *wprotos = bact->ywo->getYW_weaponProtos();

    if ( arg->weapon == -1 )
        return 0;

    if ( bact->weapon_time )
    {
        int v4;

        if ( bact->oflags & BACT_OFLAG_USERINPT )
            v4 = wprotos[arg->weapon].shot_time_user;
        else
            v4 = wprotos[arg->weapon].shot_time;

        if ( wprotos[arg->weapon].salve_shots )
        {
            if ( wprotos[arg->weapon].salve_shots <= bact->salve_counter )
                v4 = wprotos[arg->weapon].salve_delay;
        }

        if ( arg->g_time - bact->weapon_time < v4 )
            return 0;
    }

    if ( bact->salve_counter < wprotos[arg->weapon].salve_shots )
        bact->salve_counter += 1;
    else
        bact->salve_counter = 1;

    if ( bact->oflags & BACT_OFLAG_USERINPT )
    {
        yw_arg180 v26;

        if ( wprotos[arg->weapon].model_id & 2 )
            v26.effects_type = 0;
        else if ( wprotos[arg->weapon].model_id & 0x10 )
            v26.effects_type = 1;
        else
            v26.effects_type = 2;

        bact->ywo->ypaworld_func180(&v26);
    }

    int v13;

    if ( bact->num_weapons <= 1 )
        v13 = 1;
    else
        v13 = bact->num_weapons;

    for (int i = 0; i < v13; i++)
    {
        float v37;

        if ( v13 == 1 )
            v37 = arg->start_point.x;
        else
        {
            float v14 = fabs(arg->start_point.x);
            v37 = (i * 2) * v14 / (v13 - 1) - v14;
        }

        ypaworld_arg146 arg147;
        arg147.vehicle_id = arg->weapon;
        arg147.pos = bact->position + bact->rotation.Transpose().Transform( vec3d(v37, arg->start_point.y, arg->start_point.z) );

        wobj = bact->ywo->ypaworld_func147(&arg147);

        if ( !wobj )
            return 0;

        __NC_STACK_ypabact *wbact;
        wbact = wobj->getBACT_pBact();

        wobj->setMISS_launcher(bact);

        wobj->setMISS_startHeight(arg147.pos.y);

        wbact->owner = bact->owner;

        if ( bact->bact_type != BACT_TYPES_GUN )
            bact->energy -= wbact->energy / 300;

        if ( arg->direction.x != 0.0 || arg->direction.y != 0.0 || arg->direction.z != 0.0 )
        {
            wbact->fly_dir = arg->direction;
        }
        else
        {
            wbact->fly_dir = bact->rotation.AxisZ();
        }

        wbact->fly_dir_length = bact->fly_dir_length + wprotos[ arg->weapon ].start_speed;

        if ( !(wprotos[arg->weapon].model_id & 0x12) )
            wbact->fly_dir_length *= 0.2;

        wbact->rotation.SetZ( wbact->fly_dir );

        wbact->rotation.SetX( bact->rotation.AxisX() );

        wbact->rotation.SetY( wbact->rotation.AxisZ() * wbact->rotation.AxisX() );

        if ( i == 0 )
        {
            if ( arg->flags & 1 )
                wbact->position = wbact->position - wbact->rotation.AxisZ() * 30.0;
        }

        if ( wbact->parent_bacto )
        {
            Remove(&wbact->subject_node);
            wbact->parent_bacto = NULL;
        }

        bact_node *a2a = wobj->getMISS_pNode();

        AddTail(&bact->missiles_list, a2a);

        int v42 = wobj->getMISS_type();

        if ( v42 == 3 )
        {
            setTarget_msg arg67;

            arg67.tgt = arg->target;
            arg67.tgt_type = arg->tgType;
            arg67.priority = 0;
            arg67.tgt_pos = arg->tgt_pos;

            wobj->SetTarget(&arg67);

            if ( arg->flags & 2 )
            {
                if ( arg->tgType == BACT_TGT_TYPE_CELL )
                    wbact->primTpos.y = arg->tgt_pos.y;
            }
        }

        uamessage_newWeapon wpnMsg;
        wpnMsg.targetPos = arg->tgt_pos;

        if ( v42 == 2 )
        {
            wbact->primTtype = BACT_TGT_TYPE_DRCT;
            wbact->target_dir = wbact->fly_dir;
        }

        wbact->host_station = bact->host_station;
        bact->weapon_time = arg->g_time;

        startSound(&wbact->soundcarrier, 1);

        _NC_STACK_ypaworld *yw = &bact->ywo->stack__ypaworld;

        if ( yw->isNetGame )
        {
            wbact->gid |= bact->owner << 24;

            wpnMsg.msgID = UAMSG_NEWWEAPON;
            wpnMsg.owner = bact->owner;
            wpnMsg.id = wbact->gid;
            wpnMsg.launcher = bact->gid;
            wpnMsg.type = arg->weapon;
            wpnMsg.pos = arg147.pos;
            wpnMsg.flags = 0;
            wpnMsg.dir = wbact->fly_dir * wbact->fly_dir_length;
            wpnMsg.targetType = wbact->primTtype;

            if ( wbact->primTtype == BACT_TGT_TYPE_UNIT )
            {
                wpnMsg.target = wbact->primT.pbact->gid;
                wpnMsg.targetOwner = wbact->primT.pbact->owner;
            }

            yw_arg181 arg181;
            arg181.data = &wpnMsg;
            arg181.dataSize = sizeof(wpnMsg);
            arg181.recvFlags = 2;
            arg181.recvID = 0;
            arg181.garant = 1;

            yw->self_full->ypaworld_func181(&arg181);
        }

        if ( arg->flags & 1 )
        {
            if ( i == 0 )
            {
                if ( bact->oflags & BACT_OFLAG_VIEWER )
                {
                    setBACT_viewer(0);
                    wobj->setBACT_viewer(1);
                }
            }
        }

        if ( arg->tgType != BACT_TGT_TYPE_UNIT )
        {
            int life_time_nt = wprotos[ arg->weapon ].life_time_nt;

            if ( life_time_nt )
                wobj->setMISS_lifeTime(life_time_nt);
        }
    }

    if ( bact->kill_after_shot )
    {
        if ( bact->oflags & BACT_OFLAG_USERINPT )
        {
            setBACT_viewer(0);
            wobj->setBACT_viewer(1);
        }

        bact_arg84 arg84;
        arg84.unit = bact->parent_bact;
        arg84.energy = -2 * bact->energy_max;

        ModifyEnergy(&arg84);
    }
    return 1;
}

size_t NC_STACK_ypabact::SetPosition(bact_arg80 *arg)
{
    __NC_STACK_ypabact *bct = &ypabact;

    yw_130arg sect_info;

    sect_info.pos_x = arg->pos.x;
    sect_info.pos_z = arg->pos.z;
    if (!bct->ywo->ypaworld_func130(&sect_info))
        return 0;

    if ( bct->pSector )
        Remove(bct);

    AddTail(&sect_info.pcell->units_list, bct);

    bct->pSector = sect_info.pcell;
    bct->old_pos = arg->pos;
    bct->position = arg->pos;
    bct->sectX = sect_info.sec_x;
    bct->sectY = sect_info.sec_z;

    if ( !(arg->field_C & 2) )
        CorrectPositionInLevelBox(NULL);

    return 1;
}

void NC_STACK_ypabact::GetSummary(bact_arg81 *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    bact_node *node = (bact_node *)bact->subjects_list.head;

    while ( node->next )
    {
        node->bacto->GetSummary(arg);

        node = (bact_node *)node->next;
    }

    if ( bact->status != BACT_STATUS_DEAD )
    {
        switch ( arg->enrg_type )
        {
        case 1:
            arg->enrg_sum += bact->energy;
            break;

        case 3:
            arg->enrg_sum++;
            break;

        case 2:
            arg->enrg_sum += bact->shield;
            break;

        case 4:
            arg->enrg_sum += bact->energy_max;
            break;

        case 5:
        {
            bact_node *nd = (bact_node *)bact->attackers_list.head;

            while (nd->next)
            {
                arg->enrg_sum++;
                nd = (bact_node *)nd->next;
            }
        }
        break;

        default:
            break;
        }
    }
}

// Update bact energy
void NC_STACK_ypabact::EnergyInteract(update_msg *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    cellArea *cell = bact->pSector;

    if ( bact->status != BACT_STATUS_DEAD )
    {
        int v16 = bact->clock - bact->energy_time;

        if ( v16 >= 1500 )
        {
            bact->energy_time = bact->clock;

            yw_arg176 arg176;
            arg176.owner = cell->owner;

            bact->ywo->ypaworld_func176(&arg176);

            float v14 = v16 / 1000.0;

            float denerg = bact->energy_max * v14 * cell->energy_power * arg176.field_4 / 7000.0;

            if ( bact->owner == cell->owner )
                bact->energy += denerg;
            else
                bact->energy -= denerg;

            if ( bact->energy < 0 )
                bact->energy = 0;

            if ( bact->energy > bact->energy_max )
                bact->energy = bact->energy_max;
        }
    }
}

void NC_STACK_ypabact::ApplyImpulse(bact_arg83 *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    float v81 = 50.0 / bact->mass;
    float v79 = arg->energ * 0.0004;

    vec3d v60 = bact->position - arg->pos;

    float distance = v60.length();

    if ( distance <= bact->radius )
    {
        vec3d v63 = (arg->pos2 * (2.5 * arg->mass * arg->force) + bact->fly_dir * bact->fly_dir_length) / (bact->mass + arg->mass);

        bact->fly_dir_length = v63.normalise();

        if ( bact->fly_dir_length > 0.0 )
            bact->fly_dir = v63;

        v60 = arg->pos2;

        distance = 1.0;
    }
    else
    {
        v60 /= distance;

        vec3d v63 = bact->fly_dir * bact->fly_dir_length + (v60 * v81 * v79) / distance;

        bact->fly_dir_length = v63.normalise();

        if ( bact->fly_dir_length > 0.0 )
            bact->fly_dir = v63;
    }

    CorrectPositionInLevelBox(NULL);

    bact->status_flg &= ~BACT_STFLAG_LAND;

    float angle = v81 * 0.01 * v79 / distance;

    float cos_len = v60.dot(bact->rotation.AxisZ());

    // cos(45) == 0.7071
    if ( fabs(cos_len) > 0.7071 )
    {
        if ( cos_len > 0.7071 )
            bact->rotation = mat3x3::RotateX(-angle) * bact->rotation;
        else
            bact->rotation = mat3x3::RotateX(angle) * bact->rotation;
    }
    else
    {
        if ( v60.XZ().cross( bact->rotation.AxisZ().XZ() ) >= 0.0 )
            bact->rotation = mat3x3::RotateZ(angle) * bact->rotation;
        else
            bact->rotation = mat3x3::RotateZ(-angle) * bact->rotation;
    }
}

void NC_STACK_ypabact::ModifyEnergy(bact_arg84 *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    int v6 = 0;

    int a4 = bact->ywo->getYW_invulnerable();

    if ( !bact->ywo || !(bact->oflags & BACT_OFLAG_VIEWER) || !a4 || arg->energy <= -1000000 )
    {
        _NC_STACK_ypaworld *yw = &bact->ywo->stack__ypaworld;

        if ( yw->isNetGame )
            v6 = 1;

        if ( !v6 || !arg->unit || bact->owner == arg->unit->owner )
        {
            bact->energy += arg->energy;

            if ( arg->unit )
                bact->killer_owner = arg->unit->owner;
            else
                bact->killer_owner = 0;

            if ( bact->energy <= 0 )
            {
                bact->killer = arg->unit;
                bact->status_flg &= ~BACT_STFLAG_LAND;

                setState_msg v16;
                v16.newStatus = BACT_STATUS_DEAD;
                v16.unsetFlags = 0;
                v16.setFlags = 0;

                SetState(&v16);

                Die();
            }
        }
        else
        {
            uamessage_vhclEnergy vhclEnrgy;
            vhclEnrgy.msgID = UAMSG_VHCLENERGY;
            vhclEnrgy.owner = bact->owner;
            vhclEnrgy.id = bact->gid;
            vhclEnrgy.energy = arg->energy;

            if ( arg->unit )
            {
                vhclEnrgy.killer = arg->unit->gid;
                vhclEnrgy.killerOwner = arg->unit->owner;
            }
            else
            {
                vhclEnrgy.killer = 0;
                vhclEnrgy.killerOwner = 0;
            }

            yw_arg181 arg181;

            arg181.recvID = 0;
            arg181.recvFlags = 2;
            arg181.data = &vhclEnrgy;
            arg181.dataSize = sizeof(vhclEnrgy);
            arg181.garant = 1;

            yw->self_full->ypaworld_func181(&arg181);
        }
    }
}

bool NC_STACK_ypabact::ypabact_func85(vec3d *arg)
{
    float tmp2 = arg->dot( ypabact.fly_dir * ypabact.fly_dir_length );

    if ( fabs(tmp2) > 15.0 )
        return true;

    return false;
}


void CrashOrLand__sub1(__NC_STACK_ypabact *bact)
{
    if ( bact->fly_dir.x < 0.0 )
        bact->fly_dir.x -= 7.0;

    if ( bact->fly_dir.z < 0.0 )
        bact->fly_dir.z -= 7.0;

    if ( bact->fly_dir.x >= 0.0 )
        bact->fly_dir.x += 7.0;

    if ( bact->fly_dir.z >= 0.0 )
        bact->fly_dir.z += 7.0;

    if ( bact->fly_dir_length < 15.0 )
        bact->fly_dir_length = 15.0;

    float v4 = bact->fly_dir.length();

    if ( v4 <= 0.001 )
        bact->fly_dir = vec3d(0.0, 1.0, 0.0);
    else
        bact->fly_dir /= v4;
}

void sub_48AB14(__NC_STACK_ypabact *bact, const vec3d &vec)
{
    vec3d vaxis = bact->rotation.AxisY() * vec;

    if ( vaxis.normalise() != 0.0 )
    {
        float angle = clp_acos( vec.dot(bact->rotation.AxisY()) );

        if ( angle > 0.001 )
            bact->rotation *= mat3x3::AxisAngle(vaxis, angle);
    }
}

void CrashOrLand__sub0(__NC_STACK_ypabact *bact, int a2)
{
    bact->status_flg |= BACT_STFLAG_SCALE;

    if ( bact->scale_duration > bact->scale_pos )
    {
        float v5 = bact->maxrot * a2 / 1000.0;


        bact->scale_speed += bact->scale_accel * a2 / 1000.0;
        bact->scale_start += bact->scale_speed * (a2 / 1000.0);

        bact->scale = vec3d(bact->scale_start);

        bact->rotation = mat3x3::RotateY(v5) * bact->rotation;

        int v14 = 0;
        for (int i = 0; i < 32; i++)
        {
            if ( bact->vp_fx_models[i] )
                v14++;
        }

        if ( v14 )
        {
            int v15 = bact->scale_pos * v14 / bact->scale_duration;

            bact->current_vp.base = bact->vp_fx_models[v15];
            bact->current_vp.trigo = bact->vp_fx_tform[v15];
        }

        bact->scale_pos += a2;
    }
    else
    {
        bact->yls_time = -1;
        bact->self->Release(bact->self);
    }
}

size_t NC_STACK_ypabact::CrashOrLand(bact_arg86 *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    yw_137col v58[10];

    int v85 = 0;

    if ( bact->status_flg & BACT_STFLAG_SEFFECT )
    {
        CrashOrLand__sub0(bact, arg->field_two);
    }
    else
    {
        float v84;
        float v90;

        if ( bact->oflags & BACT_OFLAG_VIEWER )
        {
            v84 = bact->viewer_radius;
            v90 = bact->viewer_overeof;
        }
        else
        {
            v84 = bact->radius;
            v90 = bact->overeof;
        }

        if ( bact->bact_type == BACT_TYPES_ROBO )
            v90 = 60.0;

        vec3d vaxis = vec3d( -bact->rotation.m12, 0.0, bact->rotation.m10 );

        float v94 = arg->field_two / 1000.0;

        if ( vaxis.normalise() > 0.001 && !(arg->field_one & 1) )
        {
            float angle = clp_acos( bact->rotation.m11 );
            float maxrot = bact->maxrot * v94;

            if ( angle > maxrot )
                angle = maxrot;

            if ( fabs(angle) > BACT_MIN_ANGLE )
            {
                bact->rotation *= mat3x3::AxisAngle(vaxis, angle);
            }
        }

        if ( arg->field_one & 2 )
        {
            float v18 = fabs(bact->fly_dir_length) * v94 * 0.08;

            bact->rotation = mat3x3::RotateZ(v18) * bact->rotation;
        }

        if ( !(bact->status_flg & BACT_STFLAG_LAND) )
        {
            if ( arg->field_one & 1 )
                bact->airconst = 0;
            else
                bact->airconst = 500.0;

            for (int i = 0; i < 3; i++)
            {

                move_msg v66;

                v66.field_0 = v94;
                v66.flag = 1;

                Move(&v66);

                int v20 = 0;

                if ( bact->oflags & BACT_OFLAG_BACTCOLL )
                {
                    if ( CollisionWithBact(arg->field_two) )
                    {
                        if ( bact->bact_type == BACT_TYPES_TANK || bact->bact_type == BACT_TYPES_CAR )
                        {
                            CrashOrLand__sub1(bact);
                            return 0;
                        }

                        return 0;
                    }
                }

                if ( bact->oflags & BACT_OFLAG_VIEWER )
                {
                    ypaworld_arg137 arg137;
                    arg137.pos = bact->fly_dir * bact->fly_dir_length * v94 * 6.0 + bact->position;
                    arg137.pos2 = bact->fly_dir;
                    arg137.radius = v84;
                    arg137.collisions = v58;
                    arg137.field_30 = 0;
                    arg137.coll_max = 10;

                    bact->ywo->ypaworld_func137(&arg137);

                    if ( arg137.coll_count )
                    {
                        int v24 = 0;
                        v85 = 1;

                        vec3d v98;

                        for (int j = arg137.coll_count - 1; j >= 0; j--)
                        {
                            yw_137col *v25 = &arg137.collisions[ j ];

                            v98 += v25->pos2;

                            if ( v98.y > 0.6 )
                                v24 = 1;
                        }

                        bact_arg88 arg88;
                        vec3d a2a;

                        float lnn = v98.length();

                        if ( lnn != 0.0 )
                        {
                            arg88.pos1 = v98 / lnn;

                            a2a = arg88.pos1;
                        }
                        else
                        {
                            a2a = bact->fly_dir;
                            arg88.pos1 = bact->fly_dir;
                        }

                        if ( arg->field_one & 1 )
                        {
                            bact->energy -= fabs(bact->fly_dir_length) * 10.0;

                            if ( bact->energy <= 0 || (bact->current_vp.base == bact->vp_dead.base && bact->status == BACT_STATUS_DEAD) )
                            {
                                setState_msg arg78;
                                arg78.setFlags = BACT_STFLAG_DEATH2;
                                arg78.unsetFlags = 0;
                                arg78.newStatus = BACT_STATUS_NOPE;

                                SetState(&arg78);
                            }

                            if ( bact->oflags & BACT_OFLAG_USERINPT )
                            {
                                if ( fabs(bact->fly_dir_length) > 7.0 )
                                    startSound(&bact->soundcarrier, 5);

                                yw_arg180 arg180_1;

                                arg180_1.effects_type = 5;
                                arg180_1.field_4 = 1.0;
                                arg180_1.field_8 = v98.x * 10.0 + bact->position.x;
                                arg180_1.field_C = v98.z * 10.0 + bact->position.z;

                                bact->ywo->ypaworld_func180(&arg180_1);
                            }

                            if ( v98.y >= 0.6 && v24 )
                            {
                                bact->position.y = bact->old_pos.y;

                                bact->status_flg |= BACT_STFLAG_LAND;

                                bact->fly_dir_length *= bact->fly_dir.XZ().length();

                                sub_48AB14(bact, a2a);

                                bact->reb_count = 0;
                            }
                            else
                            {
                                Recoil(&arg88);

                                bact->reb_count++;

                                v20 = 1;

                                if ( bact->reb_count > 50 )
                                {
                                    bact->energy = -10000;
                                    bact->status_flg |= BACT_STFLAG_LAND;
                                }
                            }
                        }
                        else if ( v98.y < 0.6 )
                        {
                            Recoil(&arg88);

                            v20 = 1;
                        }
                        else
                        {
                            bact->position.y = bact->old_pos.y;
                            bact->fly_dir_length = 0;
                            bact->reb_count = 0;
                            bact->status_flg |= BACT_STFLAG_LAND;
                        }
                    }
                }

                if ( !v85 )
                {
                    ypaworld_arg136 arg136;
                    arg136.stPos = bact->old_pos;
                    arg136.vect = bact->position - bact->old_pos + vec3d(0.0, v90, 0.0);
                    arg136.flags = 0;

                    bact->ywo->ypaworld_func136(&arg136);

                    if ( arg136.isect )
                    {
                        bact_arg88 arg88;

                        arg88.pos1 = arg136.skel->polygons[arg136.polyID].Normal();

                        vec3d a2a = arg88.pos1;

                        if ( arg->field_one & 1 )
                        {
                            bact->energy -= fabs(bact->fly_dir_length) * 10.0;

                            if ( bact->energy <= 0 || (bact->current_vp.base == bact->vp_dead.base && bact->status == BACT_STATUS_DEAD) )
                            {
                                setState_msg arg78;
                                arg78.setFlags = BACT_STFLAG_DEATH2;
                                arg78.unsetFlags = 0;
                                arg78.newStatus = BACT_STATUS_NOPE;

                                bact->self->SetState(&arg78);
                            }

                            if ( bact->oflags & BACT_OFLAG_USERINPT )
                            {
                                if ( fabs(bact->fly_dir_length) > 7.0 )
                                    startSound(&bact->soundcarrier, 5);

                                yw_arg180 arg180;

                                arg180.effects_type = 5;
                                arg180.field_4 = 1.0;
                                arg180.field_8 = a2a.x * 10.0 + bact->position.x;
                                arg180.field_C = a2a.z * 10.0 + bact->position.z;

                                bact->ywo->ypaworld_func180(&arg180);
                            }

                            if ( arg136.skel->polygons[arg136.polyID].B < 0.6 )
                            {
                                Recoil(&arg88);

                                bact->reb_count++;

                                v20 = 1;

                                if ( bact->reb_count > 50 )
                                {
                                    bact->energy = -10000;
                                    bact->status_flg |= BACT_STFLAG_LAND;
                                }
                            }
                            else
                            {
                                bact->position = arg136.isectPos - vec3d(0.0, v90, 0.0);

                                bact->status_flg |= BACT_STFLAG_LAND;

                                bact->fly_dir_length *= bact->fly_dir.XZ().length();

                                sub_48AB14(bact, a2a);

                                bact->reb_count = 0;
                            }
                        }
                        else if ( arg136.skel->polygons[arg136.polyID].B < 0.6 )
                        {
                            Recoil(&arg88);

                            v20 = 1;
                        }
                        else
                        {
                            bact->position.y = arg136.isectPos.y - v90;

                            bact->fly_dir_length = 0;
                            bact->reb_count = 0;
                            bact->status_flg |= BACT_STFLAG_LAND;
                        }
                    }
                }

                if ( !v20 ) // Alternative exit from loop
                    break;
            }

        }
        if ( bact->status_flg & BACT_STFLAG_LAND )
            return 1;
    }
    return 0;
}


void CollisionWithBact__sub0(__NC_STACK_ypabact *bact, __NC_STACK_ypabact *a2)
{
    int v2 = (int)((float)a2->energy_max * 0.7);

    if ( v2 < 10000 )
        v2 = 10000;

    if ( v2 > 25000 )
        v2 = 25000;

    int v3 = (float)a2->scale_time * 0.2 / (float)v2 * (float)a2->energy_max;

    if ( bact->energy + v3 > bact->energy_max )
    {
        NC_STACK_yparobo *robj = bact->host_station;
        __NC_STACK_yparobo *robo = &robj->stack__yparobo;
        __NC_STACK_ypabact *rbact = robo->bact_internal;

        int v10 = v3 - (bact->energy_max - bact->energy);

        bact->energy = bact->energy_max;

        if ( rbact->energy + v10 > rbact->energy_max )
        {
            int v14 = v10 - (rbact->energy_max - rbact->energy);

            rbact->energy = rbact->energy_max;

            if ( robo->field_4F5 + v14 >= rbact->energy_max )
            {
                robo->field_4FD += v14 - (rbact->energy_max - robo->field_4F5);

                robo->field_4F5 = rbact->energy_max;

                if ( robo->field_4FD > rbact->energy_max )
                    robo->field_4FD = rbact->energy_max;
            }
            else
            {
                robo->field_4F5 += v14;
            }
        }
        else
        {
            rbact->energy += v10;
        }
    }
    else
    {
        bact->energy += v3;
    }
}

size_t NC_STACK_ypabact::CollisionWithBact(int arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    _NC_STACK_ypaworld *yw = &bact->ywo->stack__ypaworld;

    int a4 = bact->self->getBACT_viewer();

    float trad;
    if ( a4 )
        trad = bact->viewer_radius;
    else
        trad = bact->radius;


    int v49 = 0;

    rbcolls *v46 = getBACT_collNodes();

    if ( bact->fly_dir_length == 0.0 )
        return 0;

    cellArea *cell = bact->pSector;

    vec3d stru_5150E8(0.0, 0.0, 0.0);

    int v45 = 0;

    rbcolls *v55;

    __NC_STACK_ypabact *bnode = (__NC_STACK_ypabact *)cell->units_list.head;

    while ( bnode->next )
    {
        int v53 = bnode->status == BACT_STATUS_DEAD && (bnode->vp_extra[0].flags & 1) && (bact->oflags & BACT_OFLAG_USERINPT) && bnode->scale_time > 0 ;

        if ( bnode->self != this && bnode->bact_type != BACT_TYPES_MISSLE && (!bnode->self->IsDestroyed() || v53) )
        {

            v55 = bnode->self->getBACT_collNodes();

            int v9;

            if ( v55 )
            {
                v9 = v55->robo_coll_num;
                v49 = 1;
            }
            else
            {
                v9 = 1;
            }

            for (int i = v9 - 1; i >= 0; i--)
            {
                float ttrad;
                vec3d v41;

                if (!v55)
                {
                    ttrad = trad;
                    v41 = bnode->position;
                }
                else
                {
                    roboColl *v10 = &v55->roboColls[i];
                    ttrad = v10->robo_coll_radius;

                    v41 = bnode->position + bnode->rotation.Transpose().Transform(v10->coll_pos);

                    if ( ttrad < 0.01 )
                        continue;
                }

                if ( (bact->position - v41).length() <= trad + ttrad )
                {
                    if ( !v53 )
                    {
                        stru_5150E8 += v41;

                        v45++;
                    }
                    else
                    {
                        CollisionWithBact__sub0(bact, bnode);

                        bnode->scale_time = -1;

                        if ( yw->GameShell )
                            startSound(&yw->GameShell->samples2_info, 4);

                        if ( yw->isNetGame )
                        {
                            uamessage_endPlasma epMsg;
                            epMsg.msgID = UAMSG_ENDPLASMA;
                            epMsg.owner = bnode->owner;
                            epMsg.id = bnode->gid;

                            yw_arg181 v32;
                            v32.recvFlags = 2;
                            v32.dataSize = sizeof(epMsg);
                            v32.recvID = 0;
                            v32.garant = 1;
                            v32.data = &epMsg;

                            yw->self_full->ypaworld_func181(&v32);

                            if ( bnode->owner != bact->owner )
                            {
                                bnode->vp_extra[0].flags = 0;
                                bnode->vp_extra[0].vp.trigo = NULL;
                                bnode->vp_extra[0].vp.base = NULL;
                            }
                        }
                        break;
                    }
                }
            }
        }

        bnode = (__NC_STACK_ypabact *)bnode->next;
    }

    if ( !v45 || (v46 && !v49) )
    {
        bact->status_flg &= ~BACT_STFLAG_BCRASH;
        return 0;
    }

    stru_5150E8 /= (double)v45;

    vec3d stru_5150F4 = stru_5150E8 - bact->position;

    float v26 = stru_5150F4.length();

    if ( v26 < 0.0001)
        return 0;

    bact_arg88 v33;
    v33.pos1 = stru_5150F4 / v26;

    // FIX MY MATH
    // stru_5150F4 should be normalised?
    // May be replace it with "dot < 0.0" ?
    // Because cos of 1.0...0 is 0..PI/2 and 0...-1.0 is PI/2..PI
    if ( clp_acos( stru_5150F4.dot( bact->fly_dir ) ) > C_PI_2 )
        return 0;

    if ( !(bact->status_flg & BACT_STFLAG_BCRASH) )
    {
        if ( a4 )
        {
            startSound(&bact->soundcarrier, 6);

            bact->status_flg |= BACT_STFLAG_BCRASH;

            yw_arg180 v40;
            v40.field_4 = 1.0;
            v40.field_8 = stru_5150E8.x;
            v40.field_C = stru_5150E8.z;
            v40.effects_type = 5;

            bact->ywo->ypaworld_func180(&v40);
        }
    }

    if ( fabs(bact->fly_dir_length) < 0.1 )
        bact->fly_dir_length = 1.0;

    Recoil(&v33);

    bact->target_vec = bact->fly_dir;

    bact->AI_time1 = bact->clock;
    bact->AI_time2 = bact->clock;

    return 1;
}

void NC_STACK_ypabact::Recoil(bact_arg88 *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    if ( !(bact->status_flg & BACT_STFLAG_LAND) )
    {
        if ( bact->fly_dir.dot(arg->pos1) >= 0.0 )
        {
            if ( bact->fly_dir_length != 0.0 )
            {
                bact->position = bact->old_pos;

                float v4 = bact->fly_dir.dot(arg->pos1) * 2.0;

                bact->fly_dir -= arg->pos1 * v4;

                bact->fly_dir_length *= 25.0 / (fabs(bact->fly_dir_length) + 10.0);
            }
        }
    }
}

void NC_STACK_ypabact::ypabact_func89(IDVPair *arg)
{
    dprintf("MAKE ME %s\n","ypabact_func89");
    //call_parent(zis, obj, 89, arg);
}


int GetSectorTarget__sub0__sub0(__NC_STACK_ypabact *unit)
{
    bact_node *node = (bact_node *)unit->subjects_list.head;
    while ( node->next )
    {
        if ( node->bact->secndTtype != BACT_TGT_TYPE_UNIT )
            return 1;

        node = (bact_node *)node->next;
    }
    return 0;
}

__NC_STACK_ypabact * GetSectorTarget__sub0(cellArea *cell, __NC_STACK_ypabact *unit, float *radius, char *job)
{
    NC_STACK_ypaworld *wrld = unit->self->getBACT_pWorld();

    VhclProto *vhcl_protos = wrld->getYW_vhclProtos();

    __NC_STACK_ypabact *v40 = NULL;

    __NC_STACK_ypabact *cel_unit = (__NC_STACK_ypabact *)cell->units_list.head;

    VhclProto *proto = &vhcl_protos[unit->vehicleID];

    while (cel_unit->next)
    {
        if ( cel_unit->bact_type != BACT_TYPES_MISSLE && cel_unit->status != BACT_STATUS_DEAD )
        {
            if ( cel_unit->owner != unit->owner && cel_unit->owner )
            {
                int job_id;

                switch ( cel_unit->bact_type )
                {
                case BACT_TYPES_BACT:
                    job_id = proto->job_fighthelicopter;
                    break;

                case BACT_TYPES_TANK:
                case BACT_TYPES_CAR:
                    job_id = proto->job_fighttank;
                    break;

                case BACT_TYPES_FLYER:
                case BACT_TYPES_UFO:
                    job_id = proto->job_fightflyer;
                    break;

                case BACT_TYPES_ROBO:
                    job_id = proto->job_fightrobo;
                    break;

                default:
                    job_id = 5;
                    break;
                }

                if ( *job <= job_id )
                {
                    float radivs = (unit->position - cel_unit->position).length();

                    int v33 = cel_unit->self->getBACT_viewer();

                    if ( *radius >= radivs || v33 )
                    {
                        if ( unit->bact_type == BACT_TYPES_GUN || unit->bact_type == BACT_TYPES_ROBO )
                        {
                            if ( unit->self->TestTargetSector(cel_unit) )
                            {
                                *radius = radivs;
                                *job = job_id;
                                v40 = cel_unit;
                            }
                        }
                        else
                        {
                            vec3d tmp;

                            int v20;

                            if ( unit->parent_bacto == unit->host_station )
                            {
                                if ( unit->primTtype == BACT_TGT_TYPE_CELL )
                                {
                                    tmp = unit->primTpos;

                                }
                                else if ( unit->primTtype == BACT_TGT_TYPE_UNIT )
                                {
                                    tmp = unit->primT.pbact->position;
                                }
                                else
                                {
                                    tmp = unit->position;
                                }

                                v20 = 1;
                            }
                            else
                            {
                                __NC_STACK_ypabact *prnt_bct = unit->parent_bacto->getBACT_pBact();

                                if ( prnt_bct->primTtype == BACT_TGT_TYPE_CELL )
                                {
                                    tmp = prnt_bct->primTpos;
                                }
                                else if ( prnt_bct->primTtype == BACT_TGT_TYPE_UNIT )
                                {
                                    tmp = prnt_bct->primT.pbact->position;
                                }
                                else
                                {
                                    tmp = unit->position;
                                }

                                v20 = 0;
                            }

                            if ( (tmp.XZ() - unit->position.XZ()).length() <= 3600.0 )
                            {
                                if ( unit->self->TestTargetSector(cel_unit) )
                                {
                                    *radius = radivs;
                                    *job = job_id;
                                    v40 = cel_unit;
                                }
                            }
                            else
                            {
                                int v29 = 0;

                                nlist *lst = cel_unit->self->getBACT_attackList();

                                bact_node *bct_nd = (bact_node *)lst->head;
                                while ( bct_nd->next )
                                {
                                    bact_node *int_bct_nd = bct_nd->bacto->getBACT_secnAttackNode();

                                    if ( bct_nd == int_bct_nd && int_bct_nd->bact->owner == unit->owner )
                                        v29++;

                                    if ( v29 > 1 ) // Looks like hack
                                        break;

                                    bct_nd = (bact_node *)bct_nd->next;
                                }

                                if ( v29 <= 1 && (!v20 || !GetSectorTarget__sub0__sub0(unit)) )
                                {
                                    if ( unit->self->TestTargetSector(cel_unit) )
                                    {
                                        *radius = radivs;
                                        *job = job_id;
                                        v40 = cel_unit;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        cel_unit = (__NC_STACK_ypabact *)cel_unit->next;
    }

    return v40;
}

void NC_STACK_ypabact::GetSectorTarget(bact_arg90 *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    yw_130arg arg130;
    if ( arg->field_8 & 1 )
    {
        arg130.pos_x = arg->unit->position.x;
        arg130.pos_z = arg->unit->position.z;
    }
    else
    {
        arg130.pos_x = arg->pos_x;
        arg130.pos_z = arg->pos_z;
    }

    if ( bact->ywo->ypaworld_func130(&arg130) )
    {
        float rad = 1800.0;
        char job = 0;

        arg->ret_unit = NULL;

        for (int x = -1; x < 2; x++)
        {
            for (int y = -1; y < 2; y++)
            {
                __NC_STACK_ypabact *v7 = GetSectorTarget__sub0( &arg130.pcell[x + y * bact->secMaxX], arg->unit, &rad, &job);

                if ( v7 )
                    arg->ret_unit = v7;
            }
        }
    }
}

void NC_STACK_ypabact::GetBestSectorPart(vec3d *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    yw_130arg arg130;
    arg130.pos_x = arg->x;
    arg130.pos_z = arg->z;

    bact->ywo->ypaworld_func130(&arg130);

    float v15 = arg130.sec_x * 1200.0 + 600.0;
    float v13 = -(arg130.sec_z * 1200.0 + 600.0);

    arg->x = v15;
    arg->z = v13;

    if ( arg130.pcell->comp_type != 1 )
    {
        int v7 = 0;

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if ( arg130.pcell->buildings_health[j][i] > v7 )
                {
                    arg->z = 300.0 * (-1 + i) + v13;
                    arg->x = 300.0 * (-1 + j) + v15;

                    v7 = arg130.pcell->buildings_health[j][i];
                }
            }
        }
    }
}

void NC_STACK_ypabact::GetForcesRatio(bact_arg92 *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    yw_130arg arg130;

    int v4 = bact->secMaxX;
    int v5 = bact->secMaxY;

    arg->energ1 = 0;
    arg->energ2 = 0;

    if ( arg->field_14 & 1 )
    {
        arg130.pos_x = bact->position.x;
        arg130.pos_z = bact->position.z;
    }
    else
    {
        arg130.pos_x = arg->pos.x;
        arg130.pos_z = arg->pos.z;
    }

    if ( bact->ywo->ypaworld_func130(&arg130) )
    {
        cellArea *cell = arg130.pcell;

        if ( arg130.sec_x != 0 &&  arg130.sec_z != 0 )
        {
            // left-up
            cellArea *tcell = &cell[-v4 - 1];

            if ( (1 << bact->owner) & tcell->view_mask )
            {
                __NC_STACK_ypabact *cl_unit = (__NC_STACK_ypabact *)tcell->units_list.head;

                while (cl_unit->next)
                {
                    if ( cl_unit->owner )
                    {
                        if ( cl_unit->status != BACT_STATUS_DEAD && (cl_unit->bact_type != BACT_TYPES_ROBO || cl_unit->owner != bact->owner) && cl_unit->bact_type != BACT_TYPES_MISSLE )
                        {
                            if ( cl_unit->owner == bact->owner )
                                arg->energ1 += cl_unit->energy;
                            else
                                arg->energ2 += cl_unit->energy;
                        }
                    }
                    cl_unit = (__NC_STACK_ypabact *)cl_unit->next;
                }
            }
        }

        if ( arg130.sec_z )
        {
            // up
            cellArea *tcell = &cell[-v4];

            if ( (1 << bact->owner) & tcell->view_mask )
            {
                __NC_STACK_ypabact *cl_unit = (__NC_STACK_ypabact *)tcell->units_list.head;

                while (cl_unit->next)
                {
                    if ( cl_unit->owner )
                    {
                        if ( cl_unit->status != BACT_STATUS_DEAD && (cl_unit->bact_type != BACT_TYPES_ROBO || cl_unit->owner != bact->owner) && cl_unit->bact_type != BACT_TYPES_MISSLE )
                        {
                            if ( cl_unit->owner == bact->owner )
                                arg->energ1 += cl_unit->energy;
                            else
                                arg->energ2 += cl_unit->energy;
                        }
                    }
                    cl_unit = (__NC_STACK_ypabact *)cl_unit->next;
                }
            }
        }

        if ( arg130.sec_x < v4 - 1 && arg130.sec_z )
        {
            // right-up
            cellArea *tcell = &cell[-v4 + 1];

            if ( (1 << bact->owner) & tcell->view_mask )
            {
                __NC_STACK_ypabact *cl_unit = (__NC_STACK_ypabact *)tcell->units_list.head;

                while (cl_unit->next)
                {
                    if ( cl_unit->owner )
                    {
                        if ( cl_unit->status != BACT_STATUS_DEAD && (cl_unit->bact_type != BACT_TYPES_ROBO || cl_unit->owner != bact->owner) && cl_unit->bact_type != BACT_TYPES_MISSLE )
                        {
                            if ( cl_unit->owner == bact->owner )
                                arg->energ1 += cl_unit->energy;
                            else
                                arg->energ2 += cl_unit->energy;
                        }
                    }
                    cl_unit = (__NC_STACK_ypabact *)cl_unit->next;
                }
            }
        }

        if ( arg130.sec_x )
        {
            // left
            cellArea *tcell = &cell[-1];

            if ( (1 << bact->owner) & tcell->view_mask )
            {
                __NC_STACK_ypabact *cl_unit = (__NC_STACK_ypabact *)tcell->units_list.head;

                while (cl_unit->next)
                {
                    if ( cl_unit->owner )
                    {
                        if ( cl_unit->status != BACT_STATUS_DEAD && (cl_unit->bact_type != BACT_TYPES_ROBO || cl_unit->owner != bact->owner) && cl_unit->bact_type != BACT_TYPES_MISSLE )
                        {
                            if ( cl_unit->owner == bact->owner )
                                arg->energ1 += cl_unit->energy;
                            else
                                arg->energ2 += cl_unit->energy;
                        }
                    }
                    cl_unit = (__NC_STACK_ypabact *)cl_unit->next;
                }
            }
        }

        // center
        if ( (1 << bact->owner) & cell->view_mask )
        {
            __NC_STACK_ypabact *cl_unit = (__NC_STACK_ypabact *)cell->units_list.head;

            while (cl_unit->next)
            {
                if ( cl_unit->owner )
                {
                    if ( cl_unit->status != BACT_STATUS_DEAD && (cl_unit->bact_type != BACT_TYPES_ROBO || cl_unit->owner != bact->owner) && cl_unit->bact_type != BACT_TYPES_MISSLE )
                    {
                        if ( cl_unit->owner == bact->owner )
                            arg->energ1 += cl_unit->energy;
                        else
                            arg->energ2 += cl_unit->energy;
                    }
                }
                cl_unit = (__NC_STACK_ypabact *)cl_unit->next;
            }
        }

        if ( arg130.sec_x < v4 - 1 )
        {
            // right
            cellArea *tcell = &cell[1];

            if ( (1 << bact->owner) & tcell->view_mask )
            {
                __NC_STACK_ypabact *cl_unit = (__NC_STACK_ypabact *)tcell->units_list.head;

                while (cl_unit->next)
                {
                    if ( cl_unit->owner )
                    {
                        if ( cl_unit->status != BACT_STATUS_DEAD && (cl_unit->bact_type != BACT_TYPES_ROBO || cl_unit->owner != bact->owner) && cl_unit->bact_type != BACT_TYPES_MISSLE )
                        {
                            if ( cl_unit->owner == bact->owner )
                                arg->energ1 += cl_unit->energy;
                            else
                                arg->energ2 += cl_unit->energy;
                        }
                    }
                    cl_unit = (__NC_STACK_ypabact *)cl_unit->next;
                }
            }
        }

        if ( arg130.sec_x != 0 && arg130.sec_z < v5 - 1 )
        {
            // left-down
            cellArea *tcell = &cell[ v4 - 1 ];

            if ( (1 << bact->owner) & tcell->view_mask )
            {
                __NC_STACK_ypabact *cl_unit = (__NC_STACK_ypabact *)tcell->units_list.head;

                while (cl_unit->next)
                {
                    if ( cl_unit->owner )
                    {
                        if ( cl_unit->status != BACT_STATUS_DEAD && (cl_unit->bact_type != BACT_TYPES_ROBO || cl_unit->owner != bact->owner) && cl_unit->bact_type != BACT_TYPES_MISSLE )
                        {
                            if ( cl_unit->owner == bact->owner )
                                arg->energ1 += cl_unit->energy;
                            else
                                arg->energ2 += cl_unit->energy;
                        }
                    }
                    cl_unit = (__NC_STACK_ypabact *)cl_unit->next;
                }
            }
        }

        if ( arg130.sec_z < v5 - 1  )
        {
            // down
            cellArea *tcell = &cell[ v4 ];

            if ( (1 << bact->owner) & tcell->view_mask )
            {
                __NC_STACK_ypabact *cl_unit = (__NC_STACK_ypabact *)tcell->units_list.head;

                while (cl_unit->next)
                {
                    if ( cl_unit->owner )
                    {
                        if ( cl_unit->status != BACT_STATUS_DEAD && (cl_unit->bact_type != BACT_TYPES_ROBO || cl_unit->owner != bact->owner) && cl_unit->bact_type != BACT_TYPES_MISSLE )
                        {
                            if ( cl_unit->owner == bact->owner )
                                arg->energ1 += cl_unit->energy;
                            else
                                arg->energ2 += cl_unit->energy;
                        }
                    }
                    cl_unit = (__NC_STACK_ypabact *)cl_unit->next;
                }
            }
        }

        if ( arg130.sec_x < v4 - 1 && arg130.sec_z < v5 - 1 )
        {
            // down-right
            cellArea *tcell = &cell[ v4 + 1 ];

            if ( (1 << bact->owner) & tcell->view_mask )
            {
                __NC_STACK_ypabact *cl_unit = (__NC_STACK_ypabact *)tcell->units_list.head;

                while (cl_unit->next)
                {
                    if ( cl_unit->owner )
                    {
                        if ( cl_unit->status != BACT_STATUS_DEAD && (cl_unit->bact_type != BACT_TYPES_ROBO || cl_unit->owner != bact->owner) && cl_unit->bact_type != BACT_TYPES_MISSLE )
                        {
                            if ( cl_unit->owner == bact->owner )
                                arg->energ1 += cl_unit->energy;
                            else
                                arg->energ2 += cl_unit->energy;
                        }
                    }
                    cl_unit = (__NC_STACK_ypabact *)cl_unit->next;
                }
            }
        }

        if ( !(arg->field_14 & 2) )
        {
            int v33 = 0;

            if ( cell->comp_type == 1 )
            {
                v33 = cell->buildings_health[0][0];
            }
            else
            {
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if ( cell->buildings_health[i][j] ) //LOL
                            v33 += cell->buildings_health[i][j];
                    }
                }

                v33 /= 9;
            }

            if ( cell->owner == bact->owner )
            {
                if ( arg->field_14 & 4 )
                    arg->energ1 += v33 * 120;
            }
            else
            {
                arg->energ2 += v33 * 120;
            }
        }
    }
}

void NC_STACK_ypabact::ypabact_func93(IDVPair *arg)
{
    dprintf("MAKE ME %s\n","ypabact_func93");
//    call_parent(zis, obj, 93, arg);
}

void NC_STACK_ypabact::GetFormationPosition(bact_arg94 *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    vec3d v2d = bact->rotation.AxisZ().X0Z();
    v2d.normalise();

    arg->pos1 = bact->position - v2d * ( (arg->field_0 / 3 + 1) * 150.0 );

    int v6 = arg->field_0 % 3;

    if ( v6 == 0 )
    {
        arg->pos1.x += 100.0 * v2d.z;
        arg->pos1.z += -100.0 * v2d.x;
    }
    else if ( v6 == 2 )
    {
        arg->pos1.x += -100.0 * v2d.z;
        arg->pos1.z += 100.0 * v2d.x;
    }

    // With y = 0
    arg->pos2 = vec3d::X0Z( arg->pos1.XZ() - bact->position.XZ() );
}

void NC_STACK_ypabact::ypabact_func95(IDVPair *arg)
{
    dprintf("MAKE ME %s\n","ypabact_func95");
//    call_parent(zis, obj, 95, arg);
}

// Reset
void NC_STACK_ypabact::Renew()
{
    __NC_STACK_ypabact * bact = &ypabact;

    bact->oflags = BACT_OFLAG_EXACTCOLL;
    bact->status_flg = 0;
    bact->host_station = NULL;
    bact->yls_time = 3000;
    bact->primTtype = BACT_TGT_TYPE_NONE;

    bact->secndTtype = BACT_TGT_TYPE_NONE;
    bact->primT_cmdID = 0;

    int maxX = bact->ywo->getYW_mapSizeX();
    int maxY = bact->ywo->getYW_mapSizeY();

    bact->wrldX = maxX * 1200.0;
    bact->wrldY = -maxY * 1200.0;
    bact->secMaxX = maxX;
    bact->secMaxY = maxY;

    bact->commandID = 0;
//    bact->field_3D1 = 1;
    bact->killer = NULL;
    bact->brkfr_time = 0;
    bact->brkfr_time2 = 0;
    bact->mpos.x = 0;
    bact->mpos.y = 0;
    bact->mpos.z = 0;
    bact->gun_leftright = 0.0;
    bact->scale_time = 0;
    bact->clock = 0;
    bact->AI_time1 = 0;
    bact->AI_time2 = 0;
//    bact->field_921 = 0;
//    bact->field_925 = 0;
    bact->search_time1 = 0;
    bact->search_time2 = 0;
    bact->slider_time = 0;
//    bact->field_951 = 0;
    bact->mgun_time = 0;
    bact->weapon_time = 0;
    bact->newtarget_time = 0;
    bact->assess_time = 0;
    bact->scale_pos = 0;
    bact->scale_delay = 0;
    bact->beam_time = 0;
    bact->energy_time = 0;
    bact->fe_time = -45000;
    bact->salve_counter = 0;
    bact->kill_after_shot = 0;

    bact->vp_active = 0;
    bact->volume = bact->soundcarrier.samples_data[0].volume;
    bact->pitch = bact->soundcarrier.samples_data[0].pitch;

    bact->m_cmdID = 0;
    bact->gun_angle_user = bact->gun_angle;
    bact->oflags |= BACT_OFLAG_LANDONWAIT;

    for (int i = 0; i < 16; i++)
        bact->destroyFX[i].clear();

    memset(&bact->vp_extra, 0, sizeof(extra_vproto) * 3);

    init_list(&bact->attackers_list);
    init_list(&bact->subjects_list);
    init_list(&bact->missiles_list);

    bact->attack_node_scnd.next = NULL;
    bact->attack_node_prim.next = NULL;
    bact->attack_node_scnd.prev = NULL;
    bact->attack_node_prim.prev = NULL;
}

void NC_STACK_ypabact::HandBrake(update_msg *arg)
{
    __NC_STACK_ypabact * bact = &ypabact;

    bact->thraction = bact->mass * 9.77665;

    float v53 = arg->frameTime * 0.001;

    vec3d vaxis = bact->rotation.AxisY() * vec3d(0.0, 1.0, 0.0);

    if ( vaxis.normalise() > 0.001 )
    {
        float v62 = clp_acos( bact->rotation.AxisY().dot( vec3d(0.0, 1.0, 0.0) ) );
        float v11 = bact->maxrot * v53;

        if ( v62 > v11 )
            v62 = (v62 * 1.5) * v11;

        if ( fabs(v62) <= 0.0015 )
        {
            bact->rotation.SetY( vec3d(0.0, 1.0, 0.0) );

            vec3d axisX = bact->rotation.AxisX().X0Z();
            axisX.normalise();

            bact->rotation.SetX( axisX );

            vec3d axisZ = bact->rotation.AxisZ().X0Z();
            axisZ.normalise();

            bact->rotation.SetZ( axisZ );

            if ( fabs(bact->fly_dir_length) < 0.1 )
            {
                bact->fly_dir = vec3d(0.0, 1.0, 0.0);

                bact->fly_dir_length = 0;
            }
        }
        else
        {
            bact->rotation *= mat3x3::AxisAngle(vaxis, v62);
        }
    }

    bact->fly_dir_length *= 0.8;
}

void NC_STACK_ypabact::ypabact_func98(IDVPair *arg)
{
    dprintf("MAKE ME %s\n","ypabact_func98");
//    call_parent(zis, obj, 98, arg);
}

void NC_STACK_ypabact::CreationTimeUpdate(update_msg *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    bact->scale_time -= arg->frameTime;

    float v30 = arg->frameTime / 1000.0;

    if ( bact->scale_time > 0 )
    {
        bact->status_flg |= BACT_STFLAG_SCALE;

        if ( bact->scale_time < 0 )
            bact->scale = vec3d(1.0);
        else
            bact->scale = vec3d( 0.9 / ((float)bact->scale_time / 1000.0 + 0.9) + 0.1 );

        bact->rotation = mat3x3::RotateY( 2.5 / bact->scale.x * v30 ) * bact->rotation;
    }
    else
    {
        setState_msg v25;
        v25.newStatus = BACT_STATUS_NORMAL;
        v25.setFlags = 0;
        v25.unsetFlags = 0;

        SetState(&v25);

        bact->status_flg &= ~BACT_STFLAG_SCALE;

        bact_arg80 v24;

        v24.pos = bact->position;
        v24.field_C = 0;

        SetPosition(&v24);

        NC_STACK_ypabact *a4 = bact->ywo->getYW_userHostStation();

        if ( bact->host_station == a4 )
        {

            if ( bact->host_station == bact->parent_bacto )
            {
                robo_arg134 v23;
                v23.unit = bact;
                v23.field_4 = 14;
                v23.field_8 = 0;
                v23.field_C = 0;
                v23.field_10 = 0;
                v23.field_14 = 26;

                bact->host_station->placeMessage(&v23);
            }
        }

        if ( bact->host_station )
        {
            if ( bact->bact_type != BACT_TYPES_GUN )
            {
                __NC_STACK_ypabact *v27 = bact->host_station->getBACT_pBact();

                bact->fly_dir = v24.pos - v27->position;

                float fly_len = bact->fly_dir.length();

                if ( fly_len > 0.001 )
                    bact->fly_dir /= fly_len;

                bact->fly_dir_length = 20.0;
            }
        }
    }
}

size_t NC_STACK_ypabact::IsDestroyed()
{
    __NC_STACK_ypabact *bact = &ypabact;

    return (bact->current_vp.base == bact->vp_dead.base || bact->current_vp.base == bact->vp_genesis.base || bact->current_vp.base == bact->vp_megadeth.base) && bact->status == BACT_STATUS_DEAD;
}

size_t NC_STACK_ypabact::CheckFireAI(bact_arg101 *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    vec3d tmp;

    if ( arg->unkn == 2 )
        tmp = arg->pos - bact->position;
    else
        tmp = arg->pos.X0Z() - bact->position.X0Z() + vec3d::OY(bact->height);

    float len = tmp.normalise();

    if ( len == 0.0 )
        return 0;

    WeapProto *a4 = bact->ywo->getYW_weaponProtos();

    WeapProto *v8 = NULL;

    int v36;

    if ( bact->weapon != -1 )
    {
        v8 = &a4[ bact->weapon ];


        if ( v8->model_id & 1 )
            v36 = v8->model_id & 0xFE;
        else
            v8 = NULL;
    }

    if ( !v8 )
    {
        if ( bact->mgun == -1 )
            return 0;

        v36 = 2;
    }

    if ( arg->unkn == 2 )
    {
        float v32;

        if ( v8 )
        {

            float v38 = arg->radius * 0.8 + v8->radius;

            if ( v38 >= 40.0 )
            {
                v32 = v38;
            }
            else
            {
                v32 = 3.0625;
            }
        }
        else
        {
            float v41 = arg->radius * 0.8;

            if ( v41 >= 40.0 )
                v32 = v41;
            else
                v32 = 40.0;
        }

        if ( v36 )
        {
            if ( v36 == 16 )
            {
                if ( len < 1200.0 && tmp.XZ().dot( bact->rotation.AxisZ().XZ() ) > 0.93 )
                    return 1;
            }
            else
            {
                vec3d tmp2 = tmp * bact->rotation.AxisZ();

                if ( len < 1200.0 && (tmp.dot( bact->rotation.AxisZ() ) > 0.0) && v32 / len > tmp2.length() )
                    return 1;
            }
        }
        else
        {
            if ( (arg->pos.XZ() - bact->position.XZ()).length() < v32 && arg->pos.y > bact->position.y )
                return 1;
        }
    }
    else if ( v8 )
    {
        if ( v36 )
        {
            if ( v36 == 16 )
            {
                if ( len < 1200.0 && tmp.XZ().dot( bact->rotation.AxisZ().XZ() ) > 0.91 )
                    return 1;
            }
            else if ( len < 1200.0 && tmp.dot( bact->rotation.AxisZ() ) > 0.91 )
            {
                return 1;
            }
        }
        else
        {
            if ( (arg->pos.XZ() - bact->position.XZ()).length() < v8->radius )
                return 1;
        }
    }
    return 0;
}

void NC_STACK_ypabact::MarkSectorsForView(IDVPair *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    if ( bact->bact_type != BACT_TYPES_MISSLE )
    {
        if ( bact->status != BACT_STATUS_DEAD && bact->status != BACT_STATUS_CREATE )
        {
            int v8 = 0;

            if (!bact->parent_bact)
                v8 = 1;
            else
            {
                if (bact->radar >= bact->parent_bact->radar)
                    v8 = 1;
                else if (bact->sectX != bact->parent_bact->sectX)
                    v8 = 1;
                else if (bact->sectY != bact->parent_bact->sectY)
                    v8 = 1;
            }

            if ( v8 )
            {
                if ( bact->owner < 8 )
                {
                    cellArea *v19 = bact->pSector;

                    for (int i = -bact->radar; i <= bact->radar; i++)
                    {
                        int yy = bact->sectY + i;

                        int v20 = bact->secMaxX * i;

                        if ( bact->radar == 1 )
                        {
                            if ( yy > 0 && yy < bact->secMaxY - 1 )
                            {
                                if ( bact->sectX - 1 > 0 )
                                    v19[v20 - 1].view_mask |= 1 << bact->owner;

                                v19[v20].view_mask |= 1 << bact->owner;

                                if ( bact->sectX + 1 < bact->secMaxX - 1 )
                                    v19[v20 + 1].view_mask |= 1 << bact->owner;
                            }
                        }
                        else
                        {
                            float vtmp = POW2((float)bact->radar) - POW2((float)i);

                            if (vtmp < 0.0)
                                vtmp = 0.0;

                            int tmp = dround( sqrt(vtmp) );

                            for (int j = -tmp; j <= tmp; j++)
                            {
                                int xx = bact->sectX + j;

                                if ( xx > 0 && xx < bact->secMaxX - 1 && yy > 0 && yy < bact->secMaxY - 1 )
                                    v19[v20 + j].view_mask |= 1 << bact->owner;
                            }
                        }
                    }
                }
            }

            bact_node *v15 = (bact_node *)bact->subjects_list.head;
            while( v15->next )
            {
                v15->bacto->MarkSectorsForView(NULL);

                v15 = (bact_node *)v15->next;
            }
        }
    }
}

void NC_STACK_ypabact::ypabact_func103(IDVPair *arg)
{
    dprintf("MAKE ME %s\n","ypabact_func103");
//    call_parent(zis, obj, 103, arg);
}

void NC_STACK_ypabact::StuckFree(update_msg *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

//    if ( bact->field_93D > 0 )
//        bact->field_93D -= arg->field_4;

//    if ( bact->field_93D < 0 )
//        bact->field_93D = 0;

    if ( bact->bflags & BACT_OFLAG_BACTCOLL )
    {
//        if ( !bact->field_93D )
        bact->oflags |= BACT_OFLAG_BACTCOLL;
    }

    if ( bact->status != BACT_STATUS_NORMAL || bact->oflags & BACT_OFLAG_USERINPT )
    {
        bact->mpos = bact->position;
        bact->brkfr_time2 = bact->clock;
    }
    else
    {
        vec3d tmp = bact->mpos - bact->position;

        if (tmp.length() >= 12.0)
        {
            bact->mpos = bact->position;
            bact->brkfr_time2 = bact->clock;
        }
        else
        {
            if ( bact->oflags & BACT_OFLAG_BACTCOLL )
                bact->bflags |= BACT_OFLAG_BACTCOLL;

            if ( bact->clock - bact->brkfr_time2 > 10000 )
            {
                if ( (bact->bact_type == BACT_TYPES_TANK || bact->bact_type == BACT_TYPES_CAR) && !(bact->status_flg & BACT_STFLAG_ATTACK) )
                {
                    bact->old_pos = bact->position;

                    bact->position += -bact->rotation.AxisZ() * 10.0;

                    CorrectPositionInLevelBox(NULL);

                    bact->rotation = mat3x3::RotateY(0.1) * bact->rotation;

                    ypaworld_arg136 arg136;
                    arg136.stPos = bact->old_pos;
                    arg136.vect = bact->position - bact->old_pos;
                    arg136.flags = 1;

                    bact->ywo->ypaworld_func136(&arg136);

                    if ( arg136.isect )
                    {
                        bact->position = arg136.isectPos - vec3d::OY(5.0);
                    }
                }
            }
        }
    }
}

size_t NC_STACK_ypabact::FireMinigun(bact_arg105 *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    int a5 = 0;

    if ( bact->ywo->stack__ypaworld.isNetGame )
        a5 = 1;

    if ( bact->mgun == -1 )
        return 0;

    __NC_STACK_ypabact *v108 = NULL;
    float v123 = 0.0;
    float v121 = 0.0;
    vec3d v66;

    yw_130arg arg130;
    arg130.pos_x = bact->position.x;
    arg130.pos_z = bact->position.z;

    vec2d tmp = bact->position.XZ() + arg->field_0.XZ() * 1000.0;

    bact->ywo->ypaworld_func130(&arg130);

    cellArea *v70[3];
    v70[0] = arg130.pcell;

    arg130.pos_x = tmp.x;
    arg130.pos_z = tmp.y;
    bact->ywo->ypaworld_func130(&arg130);

    v70[2] = arg130.pcell;

    if ( arg130.pcell == v70[0] )
    {
        v70[1] = v70[0];
    }
    else
    {
        vec2d tmp2 = bact->position.XZ() + (tmp - bact->position.XZ()) * 0.5;
        arg130.pos_x = tmp2.x;
        arg130.pos_z = tmp2.y;

        bact->ywo->ypaworld_func130(&arg130);

        v70[1] = arg130.pcell;
    }

    int v107 = 0;
    if ( bact->bact_type == BACT_TYPES_GUN )
    {
        NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( this );
        int a4 = gun->getGUN_roboGun();

        if ( a4 )
            v107 = 1;
    }
    else
    {
        bact->energy -= bact->gun_power * arg->field_C / 300.0;
    }

    for(int i = 0; i < 3; i++)
    {
        if ( i <= 0 || v70[ i ] != v70[ i - 1 ] )
        {
            __NC_STACK_ypabact *v21 = (__NC_STACK_ypabact *)v70[ i ]->units_list.head;

            while ( v21->next )
            {
                if ( v21 != bact && v21->bact_type != BACT_TYPES_MISSLE && v21->status != BACT_STATUS_DEAD )
                {
                    int v89 = 0;
                    if (v21->bact_type == BACT_TYPES_GUN)
                    {
                        NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( v21->self );
                        v89 = gun->getGUN_roboGun();
                    }

                    if ( v21->bact_type != BACT_TYPES_GUN || v21->shield > 100 || !v89 )
                    {
                        if ( (bact->oflags & BACT_OFLAG_USERINPT || v21->owner != bact->owner) && (!v107 || v21->self != bact->host_station) )
                        {

                            rbcolls *v93 = v21->self->getBACT_collNodes();

                            int v109;
                            if ( v93 )
                                v109 = v93->robo_coll_num;
                            else
                                v109 = 1;

                            int v22 = 0;

                            for (int j = v109 - 1; j >= 0; j-- )
                            {
                                vec3d v77;
                                float v27;

                                if ( v93 )
                                {
                                    v77 = v21->position + v21->rotation.Transpose().Transform( v93->roboColls[j].coll_pos );

                                    v27 = v93->roboColls[j].robo_coll_radius;
                                }
                                else
                                {
                                    v77 = v21->position;

                                    v27 = v21->radius;
                                }

                                if ( !v93 || v27 >= 0.01 )
                                {
                                    v121 = v27;

                                    vec3d v63 = v77 - bact->old_pos;

                                    if ( v63.dot( bact->rotation.AxisZ() ) >= 0.3 )
                                    {
                                        vec3d v33 = arg->field_0 * v63;

                                        float v111 = v63.length();
                                        float v110 = v33.length();

                                        float v37 = v27 + bact->gun_radius;

                                        if ( v37 > v110 )
                                        {
                                            if ( sqrt( POW2(v110) + 1000000.0 ) > v111 )
                                            {
                                                if ( !v22 )
                                                {
                                                    int v88 = v21->self->getBACT_inputting();

                                                    int energ;

                                                    if ( v88 || v21->status_flg & BACT_STFLAG_ISVIEW )
                                                    {
                                                        float v39 = (bact->gun_power * arg->field_C) * (100.0 - (float)v21->shield);
                                                        energ = (v39 * 0.004);
                                                    }
                                                    else
                                                    {

                                                        float v41 = (bact->gun_power * arg->field_C) * (100.0 - (float)v21->shield);
                                                        energ = v41 / 100;
                                                    }

                                                    bact_arg84 v86;
                                                    v86.unit = bact;
                                                    v86.energy = -energ;

                                                    if ( energ )
                                                        v21->self->ModifyEnergy(&v86);
                                                }

                                                v22 = 1;

                                                if ( !v108 || v123 > v111 )
                                                {
                                                    v108 = v21;
                                                    v123 = v111;

                                                    v66 = v21->position;
                                                }
                                            }
                                        }
                                    }

                                }

                            }


                        }
                    }
                }

                v21 = (__NC_STACK_ypabact *)v21->next;
            }
        }
    }

    int v88 = getBACT_inputting();

    if ( (v88 || bact->ywo->ypaworld_func145(bact)) && !a5 )
    {
        WeapProto *v90 = bact->ywo->getYW_weaponProtos();

        int v45;

        if ( v88 )
        {
            int v43 = v90[bact->mgun].shot_time_user;
            float v42 = arg->field_C * 1000.0;

            if ( v43 <= v42 )
                v45 = v42;
            else
                v45 = v43;
        }
        else
        {
            int v47 = v90[bact->mgun].shot_time;
            float v46 = arg->field_C * 1000.0;

            if ( v47 <= v46 )
                v45 = v46;
            else
                v45 = v47;
        }

        if ( arg->field_10 - bact->mgun_time > v45 )
        {
            bact->mgun_time = arg->field_10;

            int v55 = 0;
            int v96 = 0;

            ypaworld_arg136 v59;

            vec3d v80;

            if ( v108 )
            {
                float v50 = v121 * 0.7;

                NDIV_CARRY(v123);

                v55 = 1;
                v96 = 0;

                v80 = v66 - (v66 - bact->position) * v50 / v123;
            }
            else
            {
                v59.stPos = bact->position;
                v59.vect = arg->field_0 * 1000.0;
                v59.flags = 0;

                bact->ywo->ypaworld_func149(&v59);

                if ( v59.isect )
                {
                    v80 = v59.isectPos;

                    v96 = 1;
                    v55 = 1;
                }
                else
                {
                    v55 = 0;
                }
            }

            if ( v55 )
            {
                ypaworld_arg146 arg147;
                arg147.pos = v80;
                arg147.vehicle_id = bact->mgun;

                NC_STACK_ypamissile *v57 = bact->ywo->ypaworld_func147(&arg147);

                if ( v57 )
                {
                    __NC_STACK_ypabact *v103;
                    v103 = v57->getBACT_pBact();

                    v103->owner = bact->owner;

                    if ( v103->parent_bacto )
                    {
                        Remove(&v103->subject_node);
                        v103->parent_bacto = NULL;
                    }

                    bact_node *a2a = v57->getMISS_pNode();

                    AddTail(&bact->missiles_list, a2a);

                    setState_msg v69;
                    v69.newStatus = BACT_STATUS_DEAD;
                    v69.setFlags = 0;
                    v69.unsetFlags = 0;

                    v57->SetStateInternal(&v69);

                    if ( v96 )
                    {
                        v69.setFlags = BACT_STFLAG_DEATH2;
                        v69.newStatus = BACT_STATUS_NOPE;
                        v69.unsetFlags = 0;
                        v57->SetStateInternal(&v69);

                        miss_arg130 v61;
                        v61.pos = v59.skel->polygons[ v59.polyID ].Normal();

                        v57->ypamissile_func131(&v61);
                    }
                }
            }
        }
    }
    return 1;
}


void sub_4843BC(__NC_STACK_ypabact *bact1, __NC_STACK_ypabact *bact2, int a3)
{
    bact_hudi hudi;

    float v23;
    float v24;

    if ( bact2 )
    {
        vec3d v17 = bact2->position - bact1->position;

        mat3x3 corrected = bact1->rotation;
        GFXEngine::GFXe.getC3D()->matrixAspectCorrection(corrected, false);

        vec3d v20 = corrected.Transform(v17);

        if ( v20.z != 0.0 )
        {
            v23 = v20.x / v20.z;
            v24 = v20.y / v20.z;
        }
        else
        {
            v24 = 0.0;
            v23 = 0.0;
        }

        hudi.field_18 = bact2;
    }
    else
    {
        v23 = -bact1->gun_leftright;
        v24 = -bact1->gun_angle_user;

        hudi.field_18 = NULL;
    }

    if ( bact1->mgun == -1 )
    {
        hudi.field_0 = 0;
    }
    else
    {
        hudi.field_0 = 1;
        hudi.field_8 = -bact1->gun_leftright;
        hudi.field_C = -bact1->gun_angle_user;
    }

    if ( bact1->weapon == -1 || a3 )
    {
        hudi.field_4 = 0;
    }
    else
    {
        if ( bact1->weapon_flags & 4 )
        {
            hudi.field_4 = 4;
            hudi.field_10 = v23;
            hudi.field_14 = v24;
        }
        else
        {
            if ( (bact1->weapon_flags & 4) || !(bact1->weapon_flags & 2) )
                hudi.field_4 = 2;
            else
                hudi.field_4 = 3;

            hudi.field_10 = -bact1->gun_leftright;
            hudi.field_14 = -bact1->gun_angle_user;
        }
    }

    bact1->ywo->ypaworld_func153(&hudi);
}

size_t NC_STACK_ypabact::UserTargeting(bact_arg106 *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    WeapProto *weaps = bact->ywo->getYW_weaponProtos();

    __NC_STACK_ypabact *targeto = 0;
    float v56 = 0.0;

    float v55;

    if ( bact->weapon == -1 )
        v55 = 0.0;
    else
        v55 = weaps[bact->weapon].radius;

    int a3a = !(bact->weapon_flags & 2) && !(bact->weapon_flags & 0x10);

    if ( bact->weapon != -1 && !a3a )
    {
        yw_130arg arg130;
        arg130.pos_x = bact->position.x;
        arg130.pos_z = bact->position.z;

        bact->ywo->ypaworld_func130(&arg130);

        vec2d tmp = bact->rotation.AxisZ().XZ() * 1200.0 + bact->position.XZ();

        cellArea *v41[3];

        v41[0] = arg130.pcell;

        arg130.pos_x = tmp.x;
        arg130.pos_z = tmp.y;

        bact->ywo->ypaworld_func130(&arg130);

        v41[2] = arg130.pcell;

        if ( arg130.pcell == v41[0] )
        {
            v41[1] = v41[0];
        }
        else
        {
            vec2d tmp2 = bact->position.XZ() + (tmp - bact->position.XZ()) * 0.5;
            arg130.pos_x = tmp2.x;
            arg130.pos_z = tmp2.y;

            bact->ywo->ypaworld_func130(&arg130);

            v41[1] = arg130.pcell;
        }

        for (int i = 0; i < 3; i++)
        {
            if ( i <= 0 || v41[i] != v41[i - 1] )
            {
                if ( v41[i] )
                {
                    __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)v41[i]->units_list.head;

                    while (bct->next)
                    {
                        if ( bct != bact )
                        {

                            if ( bct->bact_type != BACT_TYPES_MISSLE && bct->status != BACT_STATUS_DEAD )
                            {
                                int v53 = 0;
                                if (bct->bact_type == BACT_TYPES_GUN)
                                {
                                    NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( bct->self );
                                    v53 = gun->getGUN_roboGun();
                                }

                                if ( !v53 )
                                {
                                    if ( arg->field_0 & 2 || bct->owner != bact->owner )
                                    {
                                        if ( arg->field_0 & 1 || bct->owner == bact->owner || !bct->owner )
                                        {
                                            if ( arg->field_0 & 4 || bct->owner )
                                            {
                                                vec3d mv = bct->position - bact->old_pos;

                                                if ( mv.dot( bact->rotation.AxisZ() ) >= 0.0 )
                                                {
                                                    float mv_len = mv.length();

                                                    vec3d mvd = arg->field_4 * mv;

                                                    float v59 = mv_len * 1000.0 * 0.0005 + 20.0;
                                                    float mvd_len = mvd.length();

                                                    if ( ((mvd_len < v59 && (bact->weapon_flags & 4)) || (bct->radius + v55 > mvd_len && !(bact->weapon_flags & 4)) )
                                                            && mv_len < 2000.0
                                                            && (v56 > mvd_len || !targeto) )
                                                    {
                                                        targeto = bct;
                                                        v56 = mvd_len;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }

                            }
                        }

                        bct = (__NC_STACK_ypabact *)bct->next;
                    }
                }
            }
        }
    }

    if ( targeto )
    {
        sub_4843BC(bact, targeto, a3a);

        setTarget_msg arg67;
        arg67.tgt_type = BACT_TGT_TYPE_UNIT;
        arg67.priority = 1;
        arg67.tgt.pbact = targeto;

        SetTarget(&arg67);

        arg->ret_bact = targeto;
        return 1;
    }

    sub_4843BC(bact, NULL, a3a);
    arg->ret_bact = NULL;

    return 0;
}

void NC_STACK_ypabact::HandleVisChildrens(int *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    NC_STACK_base *vps[7];
    vps[0] = bact->vp_normal.base;
    vps[1] = bact->vp_dead.base;
    vps[2] = bact->vp_fire.base;
    vps[3] = bact->vp_genesis.base;
    vps[4] = bact->vp_wait.base;
    vps[5] = bact->vp_megadeth.base;
    vps[6] = 0;

    int v13 = 0;

    while ( vps[v13] )
    {
        nlist *kids_list = vps[v13]->getBASE_kidList();

        base_node *kid = (base_node *)kids_list->head;
        while (kid->next)
        {
            if ( *arg == 1 )
            {
                kid->self_full->setBASE_parentFollow(1);

                flag_xyz arg68;

                arg68.flag = 7;
                arg68.v = kid->self_full->getBASE_pos() - bact->position;

                kid->self_full->base_func68(&arg68);
            }
            else if ( *arg == 2 )
            {
                kid->self_full->setBASE_parentFollow(1);

                flag_xyz arg68;

                arg68.flag = 7;
                arg68.v = kid->self_full->getBASE_pos() + bact->position;

                kid->self_full->base_func68(&arg68);
            }

            kid = (base_node *)kid->next;
        }

        v13++;
    }
}

size_t NC_STACK_ypabact::GetFightMotivation(float *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    if ( bact->aggr == 100 )
        return 1;

    if ( bact->aggr )
    {
        bact_arg81 arg81;
        arg81.enrg_sum = 0;
        arg81.enrg_type = 1;

        GetSummary(&arg81);

        float v11 = arg81.enrg_sum;

        arg81.enrg_sum = 0;
        arg81.enrg_type = 4;

        GetSummary(&arg81);

        if (arg81.enrg_sum == 0) // Possible devision by zero
            arg81.enrg_sum = 1;

        v11 = v11 / (float)arg81.enrg_sum;

        if ( arg )
            *arg = v11;

        if ( bact->status_flg & BACT_STFLAG_ESCAPE )
        {
            if ( v11 > 0.5 )
                return 1;
        }
        else if ( v11 > 0.2 )
        {
            return 1;
        }
    }
    return 0;
}

__NC_STACK_ypabact *sb_0x493984__sub1(__NC_STACK_ypabact *bact)
{
    vec3d v12;

    if ( bact->primTtype == BACT_TGT_TYPE_CELL )
        v12 = bact->primTpos;
    else if ( bact->primTtype == BACT_TGT_TYPE_UNIT )
        v12 = bact->primT.pbact->position;
    else
        return NULL;

    float v14 = 215040.0;

    __NC_STACK_ypabact *new_leader = NULL;

    bact_node *kid_unit = (bact_node *)bact->subjects_list.head;
    while ( kid_unit->next )
    {
        if ( kid_unit->bact->status != BACT_STATUS_DEAD )
        {
            int a4 = kid_unit->bacto->getBACT_inputting();

            if ( !a4 )
            {

                float v17 = sqrt(POW2(v12.z - kid_unit->bact->position.z) + POW2(v12.x - kid_unit->bact->position.x));

                if ( !new_leader || (kid_unit->bact->bact_type != BACT_TYPES_UFO && v14 > v17) || (new_leader->bact_type == BACT_TYPES_UFO && (kid_unit->bact->bact_type != BACT_TYPES_UFO || v14 > v17 )) )
                {
                    new_leader = kid_unit->bact;
                    v14 = v17;
                }
            }
        }

        kid_unit = (bact_node *)kid_unit->next;
    }
    return new_leader;
}

__NC_STACK_ypabact *sb_0x493984__sub0(__NC_STACK_ypabact *bact)
{
    float tmp = 0.0;
    __NC_STACK_ypabact *new_leader = NULL;

    bact_node *kid_unit = (bact_node *)bact->subjects_list.head;
    while ( kid_unit->next )
    {
        __NC_STACK_ypabact *v4 = kid_unit->bact;

        if ( v4->status != BACT_STATUS_DEAD )
        {
            float v10;
            if ( v4->bact_type == BACT_TYPES_UFO )
            {
                v10 = 0.0;
            }
            else
            {
                float v8 = 1.0 - ( (bact->position.XZ() - v4->position.XZ()).length() / 110400.0);
                v10 = (float)v4->energy / (float)v4->energy_max + v8;
            }

            if ( !new_leader || tmp < v10 )
            {
                new_leader = v4;
                tmp = v10;
            }
        }
        kid_unit = (bact_node *)kid_unit->next;
    }

    return new_leader;
}

__NC_STACK_ypabact *sb_0x493984(__NC_STACK_ypabact *bact, int a2)
{
    if ( bact->subjects_list.head->next )
    {
        NC_STACK_ypaworld *ywo = bact->self->getBACT_pWorld();

        __NC_STACK_ypabact *new_leader = NULL;

        if (a2)
            new_leader = sb_0x493984__sub1(bact);
        else
            new_leader = sb_0x493984__sub0(bact);

        if (!new_leader)
            return NULL;

        if (new_leader->bact_type != BACT_TYPES_UFO || bact->bact_type == BACT_TYPES_UFO)
        {
            bact->host_station->AddSubject(new_leader->self);

            sub_493DB0(new_leader, bact, ywo);

            bact_node *kid_unit = (bact_node *)bact->subjects_list.head;

            while ( kid_unit->next )
            {
                bact_node *next1 = (bact_node *)kid_unit->next;

                new_leader->self->AddSubject(kid_unit->bacto);

                sub_493DB0(kid_unit->bact, new_leader, ywo);

                kid_unit = next1;
            }
            new_leader->commandID = bact->commandID;
            return new_leader;
        }

    }
    return NULL;
}

void sub_493480(__NC_STACK_ypabact *bact, __NC_STACK_ypabact *bact2, int mode)
{
    if ( bact->ywo->stack__ypaworld.isNetGame )
    {
        static uamessage_reorder ordMsg;

        ordMsg.comm = bact2->gid;
        ordMsg.num = 0;
        ordMsg.commID = bact2->commandID;

        for ( bact_node *bct = (bact_node *)bact2->subjects_list.head; bct->next; bct = (bact_node *)bct->next )
        {
            if ( ordMsg.num < 500 )
            {
                ordMsg.units[ordMsg.num] = bct->bact->gid;
                ordMsg.num++;
            }
        }

        ordMsg.owner = bact->owner;
        ordMsg.sz = (char *)&ordMsg.units[ordMsg.num] - (char *)&ordMsg;
        ordMsg.mode = mode;
        ordMsg.msgID = UAMSG_REORDER;

        yw_arg181 arg181;
        arg181.dataSize = ordMsg.sz;
        arg181.recvID = 0;
        arg181.garant = 1;
        arg181.data = &ordMsg;
        arg181.recvFlags = 2;

        bact->ywo->ypaworld_func181(&arg181);
    }
}

void NC_STACK_ypabact::ReorganizeGroup(bact_arg109 *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    switch ( arg->field_0 )
    {
    case 1:
        if ( arg->field_4 )
        {
            if ( arg->field_4->status == BACT_STATUS_DEAD )
            {
                ypa_log_out("ORG_NEWCHIEF: Dead master\n");
            }
            else if ( arg->field_4->self != bact->parent_bacto && bact != arg->field_4 )
            {
                bact->commandID = arg->field_4->commandID;
                bact->aggr = arg->field_4->aggr;

                arg->field_4->self->AddSubject(this);

                while ( 1 )
                {
                    bact_node *v6 = (bact_node *)bact->subjects_list.head;
                    if ( !v6->next )
                        break;

                    v6->bact->aggr = arg->field_4->aggr;
                    v6->bact->commandID = arg->field_4->commandID;

                    sub_493DB0(v6->bact, arg->field_4, bact->ywo);

                    arg->field_4->self->AddSubject(v6->bacto);
                }

                sub_493DB0(bact, arg->field_4, bact->ywo);
                sub_493480(bact, arg->field_4, 1);
            }
        }
        break;

    case 2:
        if ( bact->host_station != bact->parent_bacto || bact != arg->field_4 )
        {
            if ( bact->status == BACT_STATUS_DEAD )
            {
                ypa_log_out("ORG_BECOMECHIEF dead vehicle\n");
            }
            else
            {
                if ( bact->host_station != bact->parent_bacto )
                    bact->host_station->AddSubject(this);

                if ( arg->field_4 )
                {
                    sub_493DB0(bact, arg->field_4, bact->ywo);

                    bact->aggr = arg->field_4->aggr;
                    bact->commandID = arg->field_4->commandID;

                    AddSubject(arg->field_4->self);

                    while ( 1 )
                    {
                        bact_node *v10 = (bact_node *)arg->field_4->subjects_list.head;;
                        if ( !v10->next )
                            break;

                        AddSubject(v10->bacto);
                        v10->bact->aggr = arg->field_4->aggr;

                        sub_493DB0(v10->bact, bact, bact->ywo);
                    }

                    bact->commandID = arg->field_4->commandID;
                    sub_493480(bact, bact, 2);
                }
                else
                {
                    if ( bact->host_station != bact->parent_bacto )
                    {
                        int a4 = bact->host_station->getROBO_commCount();

                        bact->commandID = a4;

                        bact->host_station->setROBO_commCount(a4 + 1);
                    }
                    sub_493480(bact, bact, 2);
                }
            }
        }
        break;

    case 3:
        if ( bact->status == BACT_STATUS_DEAD )
        {
            ypa_log_out("ORG_NEWCOMMAND: dead vehicle\n");
        }
        else
        {

            if ( bact->host_station == bact->parent_bacto )
            {
                __NC_STACK_ypabact *v14 = sb_0x493984(bact, 0);

                if ( v14 )
                    sub_493480(bact, v14, 13);
            }
            else
            {
                bact->host_station->AddSubject(bact->self);
            }

            int a4 = bact->host_station->getROBO_commCount();
            bact->commandID = a4;
            bact->commandID |= bact->owner << 24;
            bact->host_station->setROBO_commCount(a4 + 1);
            sub_493480(bact, bact, 3);
        }
        break;

    case 4:
        if ( arg->field_4->parent_bacto == arg->field_4->host_station )
        {
            __NC_STACK_ypabact *v19 = sb_0x493984(arg->field_4, 0);

            if ( v19 )
                sub_493480(bact, v19, 14);
        }

        AddSubject(arg->field_4->self);

        arg->field_4->commandID = bact->commandID;

        sub_493DB0(arg->field_4, bact, bact->ywo);
        sub_493480(bact, bact, 4);
        break;

    case 6:
    {
        int a4 = getBACT_inputting();

        if ( !a4 )
        {
            __NC_STACK_ypabact *v21 = sb_0x493984(bact, 1);

            if ( v21 )
            {
                v21->self->AddSubject(this);
                v21->commandID = bact->commandID;

                sub_493480(bact, v21, 6);
            }
        }
    }
    break;

    default:
        break;
    }
}

int TargetAssess__sub0(__NC_STACK_ypabact *bact)
{
    if ( ( bact->position.XZ() - bact->primTpos.XZ() ).length() >= 300.0 )
        return 1;

    if ( !(bact->status_flg & BACT_STFLAG_WAYPOINTCCL) )
    {
        bact->current_waypoint++;

        setTarget_msg arg67;

        if ( bact->waypoints_count > 1 )
        {
            arg67.tgt_type = BACT_TGT_TYPE_CELL_IND;
            arg67.priority = 0;
            arg67.tgt_pos = bact->waypoints[ bact->current_waypoint ];

            bact->self->SetTarget(&arg67);
        }

        if ( bact->current_waypoint >= bact->waypoints_count - 1 )
        {
            if ( bact->m_cmdID )
            {
                __NC_STACK_ypabact *v9 = sub_48C244(bact->ywo, bact->m_cmdID, bact->m_owner);

                if ( v9 )
                {
                    if ( (1 << bact->owner) & v9->pSector->view_mask )
                    {
                        arg67.tgt.pbact = v9;
                        arg67.tgt_type = BACT_TGT_TYPE_UNIT;
                        arg67.priority = 0;

                        bact->self->SetTarget(&arg67);
                    }
                }
            }

            bact->m_owner = 0;
            bact->m_cmdID = 0;
            bact->status_flg &= ~(BACT_STFLAG_WAYPOINT | BACT_STFLAG_WAYPOINTCCL);
        }
    }
    else
    {

        bact->current_waypoint++;

        int v5 = bact->current_waypoint;

        if ( bact->current_waypoint >= bact->waypoints_count )
        {
            bact->current_waypoint = 0;
            v5 = 0;
        }

        setTarget_msg arg67;

        arg67.tgt_type = BACT_TGT_TYPE_CELL_IND;
        arg67.priority = 0;
        arg67.tgt_pos = bact->waypoints[ v5 ];

        bact->self->SetTarget(&arg67);
    }
    return 1;
}

size_t NC_STACK_ypabact::TargetAssess(bact_arg110 *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    int v6 = 0;
    int v9 = 0;

    if ( arg->tgType == BACT_TGT_TYPE_FRMT && (bact->primTtype == BACT_TGT_TYPE_FRMT || bact->secndTtype == BACT_TGT_TYPE_FRMT) )
        return 1;

    if ( !arg->tgType )
        return 3;


    if ( bact->primTtype == BACT_TGT_TYPE_CELL )
    {
        if ( (bact->position.XZ() - bact->primTpos.XZ()).length() < 1800.0 )
            v9 = 1;

        if ( bact->owner == bact->primT.pcell->owner )
            v6 = 1;
    }

    if ( bact->primTtype == BACT_TGT_TYPE_UNIT )
    {
        if ( (bact->position.XZ() - bact->primT.pbact->position.XZ()).length() < 1800.0 )
            v9 = 1;

        if ( bact->owner == bact->primT.pbact->owner )
            v6 = 1;
    }

    if ( arg->tgType == BACT_TGT_TYPE_UNIT )
    {
        __NC_STACK_ypabact *v16 = NULL;
        int v51, v54;

        if ( arg->priority == 1 )
        {
            v16 = bact->secndT.pbact;
            v51 = 1;
            v54 = 50;
        }
        else if ( arg->priority == 0)
        {
            v16 = bact->primT.pbact;
            v51 = 0;
            v54 = 25;
        }

        if ( v16 )
        {
            float v50 = (v16->position.XZ() - bact->position.XZ()).length();

            if ( !( (1 << bact->owner) & v16->pSector->view_mask) )
                return 0;

            if ( bact->aggr >= 100 )
            {
                if ( v51 && v50 > 2160.0 )
                    return 0;

                return 2;
            }

            if ( !v16->owner || v16->owner == bact->owner )
            {
                if ( v50 < 300.0 )
                    return 3;

                return 1;
            }

            if ( bact->status_flg & BACT_STFLAG_ESCAPE )
            {
                if ( !v9 )
                    return 0;

                return 2;
            }

            if ( bact->aggr >= v54 )
            {
                if ( !v51 || bact->bact_type == BACT_TYPES_GUN )
                    return 2;

                if ( v50 > 2160.0 )
                    return 0;

                vec3d v21;

                if ( bact->host_station == bact->parent_bacto )
                {

                    if ( bact->primTtype == BACT_TGT_TYPE_CELL )
                        v21 = bact->primTpos;
                    else if ( bact->primTtype == BACT_TGT_TYPE_UNIT )
                        v21 = bact->primT.pbact->position;
                    else
                        v21 = bact->position;
                }
                else
                {
                    __NC_STACK_ypabact *a4 = bact->parent_bacto->getBACT_pBact();

                    if ( a4->primTtype == BACT_TGT_TYPE_CELL )
                        v21 = a4->primTpos;
                    else if ( a4->primTtype == BACT_TGT_TYPE_UNIT )
                        v21 = a4->primT.pbact->position;
                    else
                        v21 = bact->position;
                }

                if ( (v21.XZ() - bact->position.XZ()).length() > 3600.0 )
                {
                    nlist *lst = bact->secndT.pbact->self->getBACT_attackList();

                    bact_node *v43 = (bact_node *)lst->head;

                    int v28 = 0;

                    while(v43->next)
                    {
                        bact_node *v44 = v43->bacto->getBACT_secnAttackNode();

                        if ( v43 == v44 && v44->bact->owner == bact->owner )
                            v28++;

                        if ( v28 > 2 )
                            break;

                        v43 = (bact_node *)v43->next;
                    }

                    if ( v28 > 2 )
                        return 0;
                }
                return 2;
            }

            if ( !v9 || !v6 )
                return 0;

            return 2;
        }
    }
    else if ( arg->tgType == BACT_TGT_TYPE_CELL )
    {

        cellArea *v52 = NULL;
        int v51 = 0;
        int v54 = 0;

        vec2d tmp;

        if ( bact->secndTtype == BACT_TGT_TYPE_CELL )
        {
            v52 = bact->secndT.pcell;

            tmp = bact->sencdTpos.XZ();

            v54 = 75;
            v51 = 1;
        }
        else if ( bact->primTtype == BACT_TGT_TYPE_CELL )
        {
            v52 = bact->primT.pcell;

            tmp = bact->primTpos.XZ();

            v54 = 25;
            v51 = 0;
        }

        if ( bact->status_flg & BACT_STFLAG_WAYPOINT && !v51 )
            return TargetAssess__sub0(bact);

        if ( !v52 )
            return 3;

        int v32 = 0;

        if ( v52->comp_type == 1 )
        {
            v32 = v52->buildings_health[0][0];
        }
        else
        {
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    v32 += v52->buildings_health[i][j];
                }
            }
        }

        float v49 = (bact->position.XZ() - tmp).length();

        if ( bact->aggr >= 100 )
        {
            if ( v32 <= 0 && v52->owner == bact->owner )
            {
                if ( v49 < 300.0 )
                    return 3;

                return 1;
            }

            return 2;
        }

        if ( v49 >= 300.0 )
        {
            if ( bact->owner != v52->owner )
            {
                if ( bact->status_flg & BACT_STFLAG_ESCAPE || bact->aggr < v54 )
                    return 0;

                return 2;
            }

            if ( v51 )
                return 0;

            return 1;
        }

        if ( bact->owner == v52->owner )
        {
            if ( v51 )
                return 0;

            return 3;
        }

        if ( bact->status_flg & BACT_STFLAG_ESCAPE || bact->aggr < v54 )
            return 0;
        else
            return 2;
    }
    else
        return 3;

    return 3;
}

size_t NC_STACK_ypabact::TestTargetSector(__NC_STACK_ypabact *)
{
    return 1;
}

void NC_STACK_ypabact::BeamingTimeUpdate(update_msg *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    float v14 = 0.66;

    if ( bact->scale_delay <= 0 )
    {
        if ( bact->scale_time >= 1980.0 )
        {
            if ( bact->scale_time >= 3000 )
            {
                bact->ywo->ypaworld_func168(&bact);

                bact->status_flg |= BACT_STFLAG_CLEAN;

                Die();

                if ( bact->oflags & BACT_OFLAG_USERINPT )
                    bact->status_flg |= BACT_STFLAG_NORENDER;
                else
                    Release(this);

                bact->status_flg &= ~BACT_STFLAG_SCALE;
            }
            else
            {
                bact->status_flg |= BACT_STFLAG_SCALE;

                bact->scale = vec3d(1.0, 30.0, 1.0) - vec3d::OY( (bact->scale_time - 1980.0) * 30.0 / 1020.0 );
            }
        }
        else
        {
            if ( bact->vp_genesis.base != bact->current_vp.base )
            {
                setState_msg arg78;
                arg78.newStatus = BACT_STATUS_BEAM;
                arg78.setFlags = 0;
                arg78.unsetFlags = 0;

                SetState(&arg78);
            }

            bact->status_flg |= BACT_STFLAG_SCALE;

            bact->scale = vec3d(1.0, 0.0, 1.0) + vec3d::OY( (30 * bact->scale_time)/ (v14 * 3000.0) );
        }

        bact->scale_time += arg->frameTime;
    }
    else
    {
        bact->scale_delay -= arg->frameTime;
    }
}

void StartDestFX__sub0(__NC_STACK_ypabact *main, const destFX &fx)
{
    ypaworld_arg146 arg146;

    arg146.pos = main->position;
    arg146.vehicle_id = fx.ModelID;

    if ( main->radius > 31.0 )    // 31.0
    {
        float len = fx.pos.length();

        if ( len > 0.1 )
        {
            vec3d pos = fx.pos / len * main->radius;

            arg146.pos += main->rotation.Transform(pos);
        }
    }

    NC_STACK_ypabact *bah = main->ywo->ypaworld_func146(&arg146);

    if ( bah )
    {
        main->ywo->ypaworld_func134(bah);

        setState_msg v18;
        v18.newStatus = BACT_STATUS_DEAD;
        v18.setFlags = 0;
        v18.unsetFlags = 0;

        bah->SetStateInternal(&v18);

        bah->ypabact.fly_dir = main->rotation.Transform(fx.pos);

        if ( fx.type_flag & 0x10 )
            bah->ypabact.fly_dir += main->fly_dir * main->fly_dir_length;

        float len = bah->ypabact.fly_dir.length();

        if ( len > 0.001 )
            bah->ypabact.fly_dir_length = len;
    }
}

void NC_STACK_ypabact::StartDestFX(uint8_t arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    if ( bact->ywo->ypaworld_func145(bact) )
    {
        int a4 = bact->ywo->getYW_destroyFX();

        if (a4 > 16)
            a4 = 16;

        for (int i = 0; i < a4; i++)
        {
            if ( bact->destroyFX[i].ModelID )
            {
                const destFX &fx = bact->destroyFX[i];

                if ( (fx.type_flag & 2 && arg == 2) || (fx.type_flag & 1 && arg == 1) || (fx.type_flag & 4 && arg == 4) || (fx.type_flag & 8 && arg == 8) )
                    StartDestFX__sub0(bact, fx);
            }
        }
    }
}

void NC_STACK_ypabact::CorrectPositionOnLand(void *)
{
    __NC_STACK_ypabact *bact = &ypabact;

    float radius;
    if ( bact->viewer_radius >= 32.0 )
        radius = bact->viewer_radius;
    else
        radius = 32.0;

    yw_137col coltmp[10];

    ypaworld_arg137 arg137;
    arg137.pos = bact->position;
    arg137.pos2 = bact->rotation.AxisX();
    arg137.coll_max = 10;
    arg137.radius = radius;
    arg137.field_30 = 0;
    arg137.collisions = coltmp;

    bact->ywo->ypaworld_func137(&arg137);

    vec3d tmp(0.0, 0.0, 0.0);

    float trad = 0.0;

    for (int i = arg137.coll_count - 1; i >= 0; i-- )
    {
        yw_137col *clsn = &arg137.collisions[i];

        if ( clsn->pos2.y < 0.6 )
        {
            vec3d tmp2 = bact->position - clsn->pos1;

            tmp += clsn->pos2;

            float v36 = radius - tmp2.length();

            if ( trad == 0.0 || trad < v36 )
                trad = v36;
        }
    }

    if ( bact->viewer_radius >= 32.0 )
        radius = bact->viewer_radius;
    else
        radius = 32.0;

    arg137.pos = bact->position;
    arg137.pos2 = -bact->rotation.AxisX();
    arg137.coll_max = 10;
    arg137.radius = radius;
    arg137.field_30 = 0;
    arg137.collisions = coltmp;

    bact->ywo->ypaworld_func137(&arg137);

    for (int i = arg137.coll_count - 1; i >= 0; i-- )
    {
        yw_137col *clsn = &arg137.collisions[i];

        if ( clsn->pos2.y < 0.6 )
        {
            vec3d tmp2 = bact->position - clsn->pos1;

            tmp += clsn->pos2;

            float v36 = radius - tmp2.length();

            if ( trad == 0.0 || trad < v36 )
                trad = v36;
        }
    }

    float v25 = tmp.length();

    if ( v25 > 0.0001 )
        tmp /= v25;

    bact->position -= tmp * trad;
}


void NC_STACK_ypabact::CorrectPositionInLevelBox(void *)
{
    __NC_STACK_ypabact *bact = &ypabact;

    int v4 = 0;

    if ( bact->position.x > bact->wrldX - 1210.0 )
    {
        v4 = 1;
        bact->position.x = bact->wrldX - 1210.0;
    }

    if ( bact->position.x < 1210.0 )
    {
        v4 = 1;
        bact->position.x = 1210.0;
    }

    if ( bact->position.z > -1210.0 )
    {
        v4 = 1;
        bact->position.z = -1210.0;
    }

    if ( bact->position.z < bact->wrldY + 1210.0 )
    {
        v4 = 1;
        bact->position.z = bact->wrldY + 1210.0;
    }

    if ( bact->oflags & BACT_OFLAG_VIEWER )
    {
        if ( v4 )
        {
            if ( bact->bact_type != BACT_TYPES_TANK && bact->bact_type != BACT_TYPES_CAR )
            {
                ypaworld_arg136 arg136;

                arg136.stPos = bact->position - vec3d::OY(100.0);

                arg136.vect = vec3d::OY(bact->viewer_overeof + 100.0);
                arg136.flags = 0;

                bact->ywo->ypaworld_func136(&arg136);

                if ( arg136.isect )
                    bact->position.y = arg136.isectPos.y - bact->viewer_overeof;
            }
        }
    }
}

void ypabact_NetUpdate_VPHACKS(__NC_STACK_ypabact *bact, update_msg *upd)
{
    if ( bact->vp_extra_mode == 1 )
    {
        int engy = bact->energy_max * 0.7;

        if ( engy < 10000 )
            engy = 10000;

        if ( engy > 25000 )
            engy = 25000;

        sb_0x4874c4(bact, engy, upd->frameTime, 0.75);
        bact->scale_time -= upd->frameTime;

        if ( bact->scale_time < 0 )
        {
            bact->vp_extra[0].vp.trigo = 0;
            bact->vp_extra[0].vp.base = 0;
        }
    }

    if ( bact->vp_extra_mode == 2 )
    {
        NC_STACK_yparobo *roboo = dynamic_cast<NC_STACK_yparobo *>(bact->self);

        if (roboo)
        {
            __NC_STACK_yparobo *robo = &roboo->stack__yparobo;

            robo->field_511 -= upd->frameTime;
            if ( robo->field_511 <= 0 )
            {
                robo->field_511 = 0;
                startSound(&bact->soundcarrier, 10);

                robo->roboState &= ~NC_STACK_yparobo::ROBOSTATE_MOVE;
                bact->vp_extra[0].flags = 0;
                bact->vp_extra[1].flags = 0;
            }
            else
            {
                if ( robo->field_521 <= 0 )
                {
                    if ( bact->vp_extra[0].flags & EVPROTO_FLAG_ACTIVE )
                    {
                        robo->field_521 = robo->field_511 / 10;
                        bact->vp_extra[0].flags &= ~EVPROTO_FLAG_ACTIVE;
                    }
                    else
                    {
                        robo->field_521 = (1500 - robo->field_511) / 10;
                        bact->vp_extra[0].pos = bact->position;
                        bact->vp_extra[0].rotate = bact->rotation;;
                        bact->vp_extra[0].flags = 3;
                        bact->vp_extra[0].scale = 1.25;
                        bact->vp_extra[0].vp.base = bact->vp_genesis.base;
                        bact->vp_extra[0].vp.trigo = bact->vp_genesis.trigo;
                    }

                    if ( robo->bact_internal->vp_extra[1].flags & EVPROTO_FLAG_ACTIVE )
                    {
                        robo->field_521 = robo->field_511 / 10;
                        bact->vp_extra[1].flags &= ~EVPROTO_FLAG_ACTIVE;
                    }
                    else
                    {
                        robo->field_521 = (1500 - robo->field_511) / 10;
                        bact->vp_extra[1].pos = robo->field_515;
                        bact->vp_extra[1].rotate = bact->rotation;
                        bact->vp_extra[1].flags = 1;
                        bact->vp_extra[1].vp.base = bact->vp_genesis.base;
                        bact->vp_extra[1].vp.trigo = bact->vp_genesis.trigo;
                    }
                }
                robo->field_521 -= upd->frameTime;
            }

        }
    }
}

void NC_STACK_ypabact::NetUpdate(update_msg *upd)
{
    __NC_STACK_ypabact *bact = &ypabact;

    ypabact_NetUpdate_VPHACKS(bact, upd);

    yw_130arg arg130;
    arg130.pos_x = bact->position.x;
    arg130.pos_z = bact->position.z;
    if ( !bact->ywo->ypaworld_func130(&arg130) )
    {
        sub_481E0C(bact);

        arg130.pos_x = bact->position.x;
        arg130.pos_z = bact->position.z;
        bact->ywo->ypaworld_func130(&arg130);
    }

    cellArea *oldSect = bact->pSector;

    bact->sectX = arg130.sec_x;
    bact->sectY = arg130.sec_z;
//  bact->pos_x_cntr = arg130.pos_x_cntr;
//  bact->pos_y_cntr = arg130.pos_y_cntr;
    bact->pSector = arg130.pcell;

    if ( oldSect != arg130.pcell )
    {
        Remove(bact);
        AddTail(&bact->pSector->units_list, bact);
    }

    bact->clock += upd->frameTime;

    ypabact_func117(upd);

    for ( bact_node *bct = (bact_node *)bact->missiles_list.head; bct->next; bct = (bact_node *)bct->next )
    {
        NC_STACK_ypamissile *misl = dynamic_cast<NC_STACK_ypamissile *>(bct->bacto);
        if (misl)
        {
            misl->setMISS_launcher(bact);
            misl->Update(upd);
        }
    }

    sub_481F94(bact);

    bact->tForm.locPos = bact->position;

    if ( bact->status_flg & BACT_STFLAG_SCALE )
        bact->tForm.locSclRot = bact->rotation.Transpose() * mat3x3::Scale(bact->scale);
    else
        bact->tForm.locSclRot = bact->rotation.Transpose();

    int units_cnt = upd->units_count;

    upd->units_count = 0;

    for ( bact_node *bct = (bact_node *)bact->subjects_list.head; bct->next; bct = (bact_node *)bct->next )
    {
        bct->bacto->NetUpdate(upd);
        upd->units_count++;
    }

    upd->units_count = units_cnt;

    bact->soundcarrier.field_0 = bact->position;
    bact->soundcarrier.field_C = bact->fly_dir * bact->fly_dir_length;

    sb_0x4242e0(&bact->soundcarrier);
}

void NC_STACK_ypabact::ypabact_func117(update_msg *upd)
{
    __NC_STACK_ypabact *bact = &ypabact;

    if (bact->ywo->stack__ypaworld.netInterpolate)
        ypabact_func122(upd);
    else
        ypabact_func123(upd);
}

void NC_STACK_ypabact::Release(NC_STACK_ypabact *b_bacto)
{
    __NC_STACK_ypabact *bact = &ypabact;

    _NC_STACK_ypaworld *yw = &bact->ywo->stack__ypaworld;

    __NC_STACK_ypabact *b_bact = b_bacto->getBACT_pBact();

    if ( b_bact->owner )
    {
        if ( yw->isNetGame )
        {
            if ( b_bact->bact_type != BACT_TYPES_MISSLE )
            {
                uamessage_destroyVhcl destrMsg;

                destrMsg.msgID = UAMSG_DESTROYVHCL;
                destrMsg.owner = b_bact->owner;
                destrMsg.id = b_bact->gid;
                destrMsg.type = b_bact->bact_type;

                yw_arg181 v6;
                v6.recvFlags = 2;
                v6.recvID = 0;
                v6.data = &destrMsg;
                v6.garant = 1;
                v6.dataSize = sizeof(destrMsg);

                yw->self_full->ypaworld_func181(&v6);
            }
        }
    }

    bact->ywo->ypaworld_func144(b_bacto);
}

size_t NC_STACK_ypabact::SetStateInternal(setState_msg *arg)
{
    int result = 0;

    __NC_STACK_ypabact *bact = &ypabact;

    if ( arg->newStatus )
        bact->status = arg->newStatus;

    if ( arg->setFlags )
        bact->status_flg |= arg->setFlags;

    if ( arg->unsetFlags )
        bact->status_flg &= ~arg->unsetFlags;

    if ( arg->newStatus == BACT_STATUS_DEAD && (bact->vp_active != 2 && bact->vp_active != 3) )
    {
        bact->energy = -10000;

        bact->current_vp.base = bact->vp_dead.base;
        bact->current_vp.trigo = bact->vp_dead.trigo;

        bact->vp_active = 2;

        if ( bact->soundFlags & 2 )
        {
            if ( bact->oflags & BACT_OFLAG_USERINPT )
            {
                yw_arg180 v43;
                v43.effects_type = 4;

                bact->ywo->ypaworld_func180(&v43);
            }

            sub_424000(&bact->soundcarrier, 1);
            bact->soundFlags &= ~2;
        }

        if ( bact->oflags & BACT_OFLAG_USERINPT )
            sub_424000(&bact->soundcarrier, 8);

        if ( bact->soundFlags & 1 )
        {
            bact->soundFlags &= ~1;
            sub_424000(&bact->soundcarrier, 0);
        }

        if ( bact->soundFlags & 8 )
        {
            bact->soundFlags &= ~8;
            sub_424000(&bact->soundcarrier, 3);
        }

        if ( bact->soundFlags & 4 )
        {
            bact->soundFlags &= ~4;
            sub_424000(&bact->soundcarrier, 2);
        }

        startSound(&bact->soundcarrier, 7);

        bact->soundFlags |= 0x80;

        StartDestFX(1);

        result = 1;
    }

    if ( arg->newStatus == BACT_STATUS_NORMAL && 1 != bact->vp_active )
    {
        bact->current_vp.base = bact->vp_normal.base;
        bact->current_vp.trigo = bact->vp_normal.trigo;

        bact->vp_active = 1;

        if ( bact->soundFlags & 8 )
        {
            bact->soundFlags &= ~8;
            sub_424000(&bact->soundcarrier, 3);
        }

        if ( bact->soundFlags & 4 )
        {
            bact->soundFlags &= ~4;
            sub_424000(&bact->soundcarrier, 2);
        }

        if ( bact->soundFlags & 0x80 )
        {
            bact->soundFlags &= ~0x80;
            sub_424000(&bact->soundcarrier, 7);
        }

        if ( !(bact->soundFlags & 1) )
        {
            bact->soundFlags |= 1;
            startSound(&bact->soundcarrier, 0);
        }

        result = 1;
    }

    if ( arg->newStatus == BACT_STATUS_BEAM && 5 != bact->vp_active )
    {
        bact->vp_active = 5;
        bact->current_vp.base = bact->vp_genesis.base;
        bact->current_vp.trigo = bact->vp_genesis.trigo;

        if ( bact->soundFlags & 8 )
        {
            bact->soundFlags &= ~8;
            sub_424000(&bact->soundcarrier, 3);
        }

        if ( bact->soundFlags & 4 )
        {
            bact->soundFlags &= ~4;
            sub_424000(&bact->soundcarrier, 2);
        }

        if ( bact->soundFlags & 0x80 )
        {
            bact->soundFlags &= ~0x80;
            sub_424000(&bact->soundcarrier, 7);
        }

        if ( !(bact->soundFlags & 0x200) )
        {
            bact->soundFlags |= 0x200;
            startSound(&bact->soundcarrier, 9);
        }

        StartDestFX(8);

        result = 1;
    }

    if ( arg->newStatus == BACT_STATUS_IDLE && bact->vp_active != 6 )
    {
        bact->current_vp.base = bact->vp_wait.base;
        bact->current_vp.trigo = bact->vp_wait.trigo;
        bact->vp_active = 6;

        if ( bact->soundFlags & 1 )
        {
            bact->soundFlags &= ~1;
            sub_424000(&bact->soundcarrier, 0);
        }

        if ( bact->soundFlags & 8 )
        {
            bact->soundFlags &= ~8;
            sub_424000(&bact->soundcarrier, 3);
        }

        if ( bact->soundFlags & 0x80 )
        {
            bact->soundFlags &= ~0x80;
            sub_424000(&bact->soundcarrier, 7);
        }

        if ( !(bact->soundFlags & 4) )
        {
            bact->soundFlags |= 4;
            startSound(&bact->soundcarrier, 2);
        }

        result = 1;
    }

    if ( arg->newStatus == BACT_STATUS_CREATE && 4 != bact->vp_active )
    {
        bact->vp_active = arg->newStatus;
        bact->current_vp.base = bact->vp_genesis.base;
        bact->current_vp.trigo = bact->vp_genesis.trigo;

        if ( bact->soundFlags & 2 )
        {
            if ( bact->oflags & BACT_OFLAG_USERINPT )
            {
                yw_arg180 v46;
                v46.effects_type = 4;

                bact->ywo->ypaworld_func180(&v46);
            }

            sub_424000(&bact->soundcarrier, 1);
            bact->soundFlags &= ~2;
        }

        if ( bact->soundFlags & 1 )
        {
            bact->soundFlags &= ~1;
            sub_424000(&bact->soundcarrier, 0);
        }

        if ( bact->soundFlags & 4 )
        {
            bact->soundFlags &= ~4;
            sub_424000(&bact->soundcarrier, 2);
        }

        if ( bact->soundFlags & 0x80 )
        {
            bact->soundFlags &= ~0x80;
            sub_424000(&bact->soundcarrier, 7);
        }

        if ( !(bact->soundFlags & 8) )
        {
            bact->soundFlags |= 8;
            startSound(&bact->soundcarrier, 3);
        }

        StartDestFX(4);

        result = 1;
    }

    if ( arg->unsetFlags == BACT_STFLAG_FIRE && bact->vp_active == 7 )
    {
        if ( bact->oflags & BACT_OFLAG_USERINPT )
        {
            yw_arg180 v45;
            v45.effects_type = 4;
            bact->ywo->ypaworld_func180(&v45);
        }

        bact->current_vp.base = bact->vp_normal.base;
        bact->current_vp.trigo = bact->vp_normal.trigo;
        bact->vp_active = 1;

        sub_424000(&bact->soundcarrier, 1);

        bact->soundFlags &= ~2;

        result = 1;
    }

    if ( arg->unsetFlags == BACT_STFLAG_DEATH2 && bact->vp_active == 3 )
    {
        bact->vp_active = 1;
        bact->current_vp.base = bact->vp_normal.base;
        bact->current_vp.trigo = bact->vp_normal.trigo;

        result = 1;
    }

    if ( arg->setFlags == BACT_STFLAG_FIRE && bact->vp_active != 7 )
    {
        bact->vp_active = 7;
        bact->current_vp.base = bact->vp_fire.base;
        bact->current_vp.trigo = bact->vp_fire.trigo;

        if ( !(bact->soundFlags & 2) )
        {
            if ( bact->oflags & BACT_OFLAG_USERINPT )
            {
                yw_arg180 v42;
                v42.effects_type = 3;
                bact->ywo->ypaworld_func180(&v42);
            }

            bact->soundFlags |= 2;
            startSound(&bact->soundcarrier, 1);
        }
        result = 1;
    }

    if ( arg->setFlags == BACT_STFLAG_DEATH2 )
    {
        bact->status = BACT_STATUS_DEAD;

        if ( bact->vp_active != 3 )
        {
            bact->current_vp.base = bact->vp_megadeth.base;
            bact->current_vp.trigo = bact->vp_megadeth.trigo;
            bact->vp_active = 3;

            if ( bact->soundFlags & 2 )
            {
                if ( bact->oflags & BACT_OFLAG_USERINPT )
                {
                    yw_arg180 v44;
                    v44.effects_type = 4;
                    bact->ywo->ypaworld_func180(&v44);
                }

                sub_424000(&bact->soundcarrier, 1);
                bact->soundFlags &= ~2;
            }

            if ( bact->oflags & BACT_OFLAG_USERINPT )
                sub_424000(&bact->soundcarrier, 8);

            if ( bact->soundFlags & 1 )
            {
                bact->soundFlags &= ~2;
                sub_424000(&bact->soundcarrier, 0);
            }

            if ( bact->soundFlags & 8 )
            {
                bact->soundFlags &= ~8;
                sub_424000(&bact->soundcarrier, 3);
            }

            if ( bact->soundFlags & 4 )
            {
                bact->soundFlags &= ~4;
                sub_424000(&bact->soundcarrier, 2);
            }

            if ( bact->soundFlags & 0x80 )
            {
                bact->soundFlags &= ~0x80;
                sub_424000(&bact->soundcarrier, 7);
            }

            startSound(&bact->soundcarrier, 4);

            StartDestFX(2);

            bact->fly_dir_length = 0;

            result = 1;
        }
    }
    return result;
}

void NC_STACK_ypabact::ChangeSectorEnergy(yw_arg129 *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    arg->field_14 = 255;

    bact->ywo->ypaworld_func129(arg);

    yw_130arg arg130;
    arg130.pos_x = arg->pos.x;
    arg130.pos_z = arg->pos.z;

    int v5;

    if ( bact->ywo->ypaworld_func130(&arg130) )
        v5 = arg130.pcell->owner;
    else
        v5 = 0;

    if ( bact->ywo->stack__ypaworld.isNetGame )
    {
        uamessage_sectorEnergy seMsg;
        seMsg.msgID = UAMSG_SECTORENERGY;
        seMsg.owner = bact->owner;
        seMsg.pos = arg->pos;
        seMsg.energy = arg->field_10;
        seMsg.sectOwner = v5;

        if ( arg->unit )
            seMsg.whoHit = arg->unit->gid;
        else
            seMsg.whoHit = 0;

        yw_arg181 arg181;
        arg181.recvID = 0;
        arg181.recvFlags = 2;
        arg181.data = &seMsg;
        arg181.dataSize = sizeof(seMsg);
        arg181.garant = 1;

        bact->ywo->ypaworld_func181(&arg181);
    }
}

void sb_0x4874c4(__NC_STACK_ypabact *bact, int a2, int a3, float a4)
{
    if (a2 == 0)
        a2 = 1;

    bact->vp_extra[0].scale = sqrt( (float)bact->scale_time / (float)a2 ) * a4;

    if ( bact->vp_extra[0].scale < 0.0 )
        bact->vp_extra[0].scale = 0;

    bact->vp_extra[0].rotate = mat3x3::RotateY(bact->maxrot * 2.0 * (float)a3 * 0.001) * bact->vp_extra[0].rotate;
}

void NC_STACK_ypabact::DeadTimeUpdate(update_msg *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    if ( bact->status_flg & BACT_STFLAG_LAND || (bact->clock - bact->dead_time > 5000 && bact->status_flg & BACT_STFLAG_DEATH1 ) )
    {
        if ( !(bact->status_flg & BACT_STFLAG_DEATH2) )
        {
            setState_msg arg78;
            arg78.newStatus = BACT_STATUS_NOPE;
            arg78.unsetFlags = 0;
            arg78.setFlags = BACT_STFLAG_DEATH2;

            SetState(&arg78);
        }

        bact->status_flg |= BACT_STFLAG_LAND;

        if ( bact->owner && bact->bact_type != BACT_TYPES_MISSLE && bact->vp_genesis.base )
        {
            int a2 = bact->energy_max * 0.7;

            if ( a2 < 10000 )
                a2 = 10000;

            if ( a2 > 25000 )
                a2 = 25000;

            if ( bact->vp_extra[0].flags & EVPROTO_FLAG_ACTIVE )
            {
                bact->scale_time -= arg->frameTime;

                if ( bact->scale_time <= 0 )
                {
                    bact->vp_extra[0].vp.trigo = NULL;
                    bact->vp_extra[0].vp.base = NULL;

                    if ( bact->yls_time <= 0 )
                    {

                        if ( bact->oflags & BACT_OFLAG_USERINPT )
                            bact->status_flg |= BACT_STFLAG_NORENDER;
                        else
                            Release(this);

                    }
                }
                else
                {
                    sb_0x4874c4(bact, a2, arg->frameTime, 0.75);

                    if ( bact->yls_time <= 0 )
                        bact->status_flg |= BACT_STFLAG_NORENDER;
                }
            }
            else
            {
                bact->scale_time = a2;
                bact->vp_extra[0].scale = 0.75;
                bact->vp_extra[0].pos = bact->position;
                bact->vp_extra[0].rotate = bact->rotation;
                bact->vp_extra[0].vp = bact->vp_genesis;
                bact->vp_extra[0].flags |= (EVPROTO_FLAG_ACTIVE | EVPROTO_FLAG_SCALE);

                if ( bact->ywo->stack__ypaworld.isNetGame )
                {
                    uamessage_startPlasma splMsg;
                    splMsg.msgID = UAMSG_STARTPLASMA;
                    splMsg.owner = bact->owner;
                    splMsg.scale = 0.75;
                    splMsg.time = a2;
                    splMsg.id = bact->gid;
                    splMsg.pos = bact->position;
                    splMsg.dir = bact->rotation;

                    yw_arg181 arg181;
                    arg181.data = &splMsg;
                    arg181.dataSize = sizeof(splMsg);
                    arg181.recvID = 0;
                    arg181.recvFlags = 2;
                    arg181.garant = 1;

                    bact->ywo->ypaworld_func181(&arg181);
                }
            }
        }
        else if ( bact->yls_time <= 0 )
        {
            if ( bact->oflags & BACT_OFLAG_USERINPT )
                bact->status_flg |= BACT_STFLAG_NORENDER;
            else
                Release(this);
        }
    }
    else
    {
        bact_arg86 arg86;
        arg86.field_one = 3;
        arg86.field_two = arg->frameTime;

        CrashOrLand(&arg86);
    }
}

void NC_STACK_ypabact::ypabact_func122(update_msg *upd)
{
    __NC_STACK_ypabact *bact = &ypabact;

    float ftime = upd->frameTime * 0.001;

    if ( 0.001 * (upd->gTime - bact->lastFrmStamp) > 0.0 )
    {
        // Interpolate rotation
        bact->rotation += bact->netDRot * ftime;

        vec3d axis = bact->rotation.AxisX();

        if (axis.normalise() > 0.0001)
            bact->rotation.SetX( axis );
        else
            bact->rotation.SetX( vec3d::OX(1.0) );

        axis = bact->rotation.AxisY();

        if (axis.normalise() > 0.0001)
            bact->rotation.SetY( axis );
        else
            bact->rotation.SetY( vec3d::OY(1.0) );

        // Get "90 - angle" between interpolated X and Y
        float as = C_PI_2 - clp_acos( bact->rotation.AxisX().dot( bact->rotation.AxisY() ));

        // Calculate correction axis
        vec3d axs = bact->rotation.AxisX() * bact->rotation.AxisY();

        // FIX MY MATH ?
        // axs must be 1.0 normalised?

        // Rotate Y axis for 90" between X and Y
        vec3d newY = mat3x3::AxisAngle(axs, -as).Transform( bact->rotation.AxisY() );

        bact->rotation.SetY( newY );

        bact->rotation.SetZ( bact->rotation.AxisX() * bact->rotation.AxisY() );

        bact->position += bact->fly_dir * (bact->fly_dir_length * ftime * 6.0);

        CorrectPositionInLevelBox(NULL);
    }
}

void NC_STACK_ypabact::ypabact_func123(update_msg *upd)
{
    __NC_STACK_ypabact *bact = &ypabact;

    float ftime = upd->frameTime * 0.001;
    float stupd = (upd->gTime - bact->lastFrmStamp) * 0.001;

    if ( stupd > 0.0 )
    {
        bact->rotation += bact->netDRot * ftime;

        vec3d axis = bact->rotation.AxisX();

        if (axis.normalise() > 0.0001)
            bact->rotation.SetX( axis );
        else
            bact->rotation.SetX( vec3d::OX(1.0) );

        axis = bact->rotation.AxisY();

        if (axis.normalise() > 0.0001)
            bact->rotation.SetY( axis );
        else
            bact->rotation.SetY( vec3d::OY(1.0) );

        axis = bact->rotation.AxisZ();

        if (axis.normalise() > 0.0001)
            bact->rotation.SetZ( axis );
        else
            bact->rotation.SetZ( vec3d::OZ(1.0) );

        vec3d spd = bact->fly_dir * bact->fly_dir_length + bact->netDSpeed * stupd;

        bool hgun = false;
        if (bact->bact_type == BACT_TYPES_GUN)
        {
            NC_STACK_ypagun *guno = dynamic_cast<NC_STACK_ypagun *>(this);
            if (guno)
            {
                hgun = guno->getGUN_roboGun();
            }
        }

        if (bact->bact_type != BACT_TYPES_GUN || hgun)
            bact->position = spd * ftime * 6.0;

        CorrectPositionInLevelBox(NULL);

        if ( bact->status_flg & BACT_STFLAG_LAND )
        {
            ypaworld_arg136 arg136;
            arg136.stPos = bact->position;
            arg136.vect = bact->rotation.AxisY() * 200.0;
            arg136.flags = 0;

            bact->ywo->ypaworld_func136(&arg136);

            if ( arg136.isect )
                bact->position = arg136.isectPos - bact->rotation.AxisY() * bact->overeof;
        }
    }
}

size_t NC_STACK_ypabact::PathFinder(bact_arg124 *arg)
{
    //path find for ground units (tank & car)
    __NC_STACK_ypabact *bact = &ypabact;
    _NC_STACK_ypaworld *yw = bact->yw;

    int maxsteps = arg->steps_cnt;

    for (int xx = 0; xx < yw->sectors_maxX2; xx++)
    {
        for (int yy = 0; yy < yw->sectors_maxY2; yy++)
        {
            cellArea *cll = &yw->cells[xx + yy * yw->sectors_maxX2];

            cll->pf_flags = 0;
            cll->cost_to_this = 0;
            cll->cost_to_target = 0;
            cll->pf_treenode.next = NULL;
            cll->pf_treenode.prev = NULL;
            cll->pf_treeup= NULL;
        }
    }

    int from_sec_x = arg->from.x / 1200.0;
    int from_sec_z = -arg->from.y / 1200.0;
    int to_sec_x = arg->to.x / 1200.0;
    int to_sec_z = -arg->to.y / 1200.0;

    cellArea *target_pcell = NULL;

    if ( to_sec_x >= 0 && to_sec_z >= 0 && to_sec_x < yw->sectors_maxX2 && to_sec_z < yw->sectors_maxY2 )
    {
        target_pcell = &yw->cells[to_sec_x + to_sec_z * yw->sectors_maxX2];
        target_pcell->pos_x = to_sec_x;
        target_pcell->pos_y = to_sec_z;
    }

    if ( to_sec_x == from_sec_x && from_sec_z == to_sec_z )
    {
        arg->steps_cnt = 1;
        arg->waypoints[0].x = arg->to.x;
        arg->waypoints[0].z = arg->to.y;
        return 1;
    }

    nlist open_list;
    init_list(&open_list);

    cellArea *start_pcell = NULL;

    if ( from_sec_x >= 0 && from_sec_z >= 0 && from_sec_x < yw->sectors_maxX2 && from_sec_z < yw->sectors_maxY2 )
    {
        start_pcell = &yw->cells[from_sec_x + from_sec_z * yw->sectors_maxX2];
        start_pcell->pos_x = from_sec_x;
        start_pcell->pos_y = from_sec_z;
    }

    start_pcell->pf_flags |= CELL_PFLAGS_IN_CLST;

    int current_sec_x = from_sec_x;
    int current_sec_z = from_sec_z;

    cellArea *current_pcell = start_pcell;

    init_list(&start_pcell->pf_treelist);
    start_pcell->cost_to_this = 0;

    int v23 = abs(to_sec_x - from_sec_x);
    int v24 = abs(to_sec_z - from_sec_z);

    float sq2 = sqrt(2.0);

    start_pcell->cost_to_target = MMIN(v23, v24) * sq2 + abs(v23 - v24);

    while ( 1 )
    {

        for(int dx = -1; dx < 2; dx++)
        {
            for(int dz = -1; dz < 2; dz++)
            {
                if ( dx == 0.0 && dz == 0.0 )
                    continue;

                int t_x = current_sec_x + dx;
                int t_z = current_sec_z + dz;

                if ( t_x > 0 && t_x < bact->secMaxX - 1 && t_z > 0 && t_z < bact->secMaxY - 1 )
                {
                    cellArea *cell_tzx = NULL;
                    if ( t_x >= 0 && t_z >= 0 && t_x < yw->sectors_maxX2 && t_z < yw->sectors_maxY2 )
                    {
                        cell_tzx = &yw->cells[yw->sectors_maxX2 * t_z + t_x];
                        cell_tzx->pos_x = t_x;
                        cell_tzx->pos_y = t_z;
                    }

                    if ( cell_tzx->pf_flags & CELL_PFLAGS_IN_CLST )
                        continue;

                    if ( cell_tzx->addit_cost >= 100 )
                        continue;

                    if (fabs(current_pcell->height - cell_tzx->height) >= 500.0 )
                        continue;

                    if (cell_tzx->comp_type == 1 && cell_tzx != target_pcell)
                    {
                        subSec *v33 = yw->secTypes[ cell_tzx->type_id ].buildings[0][0];
                        int hlth = v33->health_models[ yw->build_hp_ref[ cell_tzx->buildings_health[0][0] ] ];

                        if (yw->legos[hlth].selected_sklt_intern != yw->legos[hlth].sklt_obj_intern)
                            continue;
                    }

                    if ( dx != 0 && dz != 0)
                    {
                        cellArea *cell_tz = NULL;

                        if (current_sec_x >= 0 && t_z >= 0 && current_sec_x < yw->sectors_maxX2 && t_z < yw->sectors_maxY2)
                        {
                            cell_tz = &yw->cells[current_sec_x + yw->sectors_maxX2 * t_z];
                            cell_tz->pos_x = current_sec_x;
                            cell_tz->pos_y = t_z;
                        }

                        cellArea *cell_tx = NULL;

                        if (t_x >= 0 && current_sec_z >= 0 && t_x < yw->sectors_maxX2 && current_sec_z < yw->sectors_maxY2)
                        {
                            cell_tx = &yw->cells[current_sec_z * yw->sectors_maxX2 + t_x];
                            cell_tx->pos_x = t_x;
                            cell_tx->pos_y = current_sec_z;
                        }

                        if ( fabs(current_pcell->height - cell_tzx->height) > 300.0
                                || fabs(current_pcell->height - cell_tz->height) > 300.0
                                || fabs(current_pcell->height - cell_tx->height) > 300.0
                                || fabs(cell_tz->height - cell_tx->height) > 300.0
                                || fabs(cell_tzx->height - cell_tz->height) > 300.0
                                || fabs(cell_tzx->height - cell_tx->height) > 300.0)
                            continue;
                    }

                    float new_cost_to_this = sqrt(POW2(dx) + POW2(dz)) + cell_tzx->addit_cost + current_pcell->cost_to_this;

                    int v40 = abs(to_sec_x - t_x);
                    int v41 = abs(to_sec_z - t_z);

                    float new_cost_to_target = MMIN(v40, v41) * sq2 + abs(v40 - v41);

                    if ( (cell_tzx->pf_flags & CELL_PFLAGS_IN_OLST)
                            && new_cost_to_this + new_cost_to_target > cell_tzx->cost_to_this + cell_tzx->cost_to_target )
                        continue;

                    cell_tzx->cost_to_this = new_cost_to_this;
                    cell_tzx->cost_to_target = new_cost_to_target;

                    if ( !(cell_tzx->pf_flags & CELL_PFLAGS_IN_OLST) )
                        AddTail(&open_list, cell_tzx);

                    if ( cell_tzx->pf_flags & CELL_PFLAGS_IN_OLST )
                        Remove(&cell_tzx->pf_treenode);

                    AddTail(&current_pcell->pf_treelist, &cell_tzx->pf_treenode);

                    cell_tzx->pf_treeup = current_pcell;
                    cell_tzx->pf_flags |= CELL_PFLAGS_IN_OLST;
                }
            }
        }



        if ( !open_list.head->next )
        {
            arg->steps_cnt = 0;
            return 0;
        }

        cellArea *selected_cell = (cellArea *)open_list.head;
        float selected_value = selected_cell->cost_to_this + selected_cell->cost_to_target;

        cellArea *tmp_cell = (cellArea *)open_list.head;
        while (tmp_cell->next)
        {
            float v49 = tmp_cell->cost_to_this + tmp_cell->cost_to_target;

            if ( v49 < selected_value )
            {
                selected_cell = tmp_cell;
                selected_value = v49;
            }

            tmp_cell = (cellArea *)tmp_cell->next;
        }

        current_sec_x = selected_cell->pos_x;
        current_sec_z = selected_cell->pos_y;
        current_pcell = selected_cell;

        init_list(&selected_cell->pf_treelist);

        Remove(selected_cell);

        selected_cell->pf_flags &= ~CELL_PFLAGS_IN_OLST;
        selected_cell->pf_flags |= CELL_PFLAGS_IN_CLST;

        if ( current_sec_x == to_sec_x && current_sec_z == to_sec_z )
            break;
    }

    nlist way_list;
    init_list(&way_list);

    cellArea *iter_cell = NULL;

    if ( to_sec_x >= 0 && to_sec_z >= 0 && to_sec_x < yw->sectors_maxX2 && to_sec_z < yw->sectors_maxY2 )
    {
        iter_cell = &yw->cells[to_sec_x + yw->sectors_maxX2 * to_sec_z];
        iter_cell->pos_x = to_sec_x;
        iter_cell->pos_y = to_sec_z;
    }

    while(iter_cell)
    {
        AddHead(&way_list, iter_cell);
        iter_cell = iter_cell->pf_treeup;
    }

    cellArea *curcell = (cellArea *)way_list.head;
    cellArea *nextcell = (cellArea *)way_list.head->next;

    int v61 = nextcell->pos_x - curcell->pos_x;
    int v62 = nextcell->pos_y - curcell->pos_y;

    int step_id = 0;

    while ( nextcell->next )
    {
        if ( maxsteps <= 1 || nextcell == target_pcell)
        {
            arg->waypoints[ step_id ].x = arg->to.x;
            arg->waypoints[ step_id ].z = arg->to.y;
            break;
        }

        cellArea *curcell = nextcell;
        nextcell = (cellArea *)nextcell->next;

        if ( nextcell->pos_x - curcell->pos_x != v61 || nextcell->pos_y - curcell->pos_y != v62 )
        {
            float tx, tz;

            if ( abs(v61) < abs(v62) )
            {
                if ( v61 > 0 )
                {
                    tz = 0.0;
                    tx = -200.0;
                }
                else
                {
                    tz = 0.0;
                    tx = 200.0;
                }
            }
            else
            {
                if ( v62 > 0 )
                {
                    tz = 200.0;
                    tx = 0.0;
                }
                else
                {
                    tz = -200.0;
                    tx = 0.0;
                }
            }

            v61 = nextcell->pos_x - curcell->pos_x;
            v62 = nextcell->pos_y - curcell->pos_y;

            arg->waypoints[ step_id ].x = (curcell->pos_x + 0.5) * 1200.0 + tx;
            arg->waypoints[ step_id ].z = -(curcell->pos_y + 0.5) * 1200.0 + tz;
            maxsteps--;
            step_id++;
        }
    }

    while ( way_list.head->next )
        Remove(way_list.head);

    while ( open_list.head->next )
        Remove(open_list.head);

    arg->steps_cnt = step_id + 1;
    return 1;
}

void SetPath__sub0(__NC_STACK_ypabact *bact, int a2)
{
    bact_node *kidunit = (bact_node *)bact->subjects_list.head;

    while ( kidunit->next )
    {
        kidunit->bact->waypoints_count = bact->waypoints_count;
        kidunit->bact->current_waypoint = a2;

        kidunit->bact->status_flg |= BACT_STFLAG_WAYPOINT;

        if ( bact->status_flg & BACT_STFLAG_WAYPOINTCCL )
            kidunit->bact->status_flg |= BACT_STFLAG_WAYPOINTCCL;
        else
            kidunit->bact->status_flg &= ~BACT_STFLAG_WAYPOINTCCL;

        for (int i = 0; i < 32; i++)
        {
            kidunit->bact->waypoints[i] = bact->waypoints[i];
        }

        kidunit = (bact_node *)kidunit->next;
    }
}

size_t NC_STACK_ypabact::SetPath(bact_arg124 *arg)
{
    // path find caller for ground squads
    __NC_STACK_ypabact *bact = &ypabact;

    int maxsteps = arg->steps_cnt;

    if ( arg->field_12 >= 2 || arg->field_12 != 1 )
        return 0; //may be 1   CHECK IT

    if ( !PathFinder(arg) )
        return 0;

    setTarget_msg arg67;
    if ( arg->steps_cnt <= 1 )
    {
        arg67.tgt_pos.x = arg->to.x;
        arg67.tgt_pos.z = arg->to.y;
    }
    else
    {
        for (int i = 0; i < arg->steps_cnt; i++)
        {
            bact->waypoints[i] = arg->waypoints[i];
        }

        bact->status_flg |= BACT_STFLAG_WAYPOINT;

        bact->current_waypoint = 0;
        bact->waypoints_count = arg->steps_cnt;

        SetPath__sub0(bact, 0);

        arg67.tgt_pos.x = arg->waypoints[0].x;
        arg67.tgt_pos.z = arg->waypoints[0].z;
    }

    arg67.tgt_type = BACT_TGT_TYPE_CELL;
    arg67.priority = 0;
    SetTarget(&arg67);

    bact_node * kidunit = (bact_node *)bact->subjects_list.head;

    while ( kidunit->next )
    {
        if ( (kidunit->bact->bact_type == BACT_TYPES_CAR || kidunit->bact->bact_type == BACT_TYPES_TANK) && bact->pSector != kidunit->bact->pSector )
        {
            bact_arg124 arg125;
            arg125.steps_cnt = maxsteps;
            arg125.from = kidunit->bact->position.XZ();
            arg125.to = arg->to;
            arg125.field_12 = arg->field_12;

            kidunit->bacto->SetPath(&arg125);
        }

        kidunit = (bact_node *)kidunit->next;
    }

    return 1;
}



void NC_STACK_ypabact::setBACT_viewer(int vwr)
{
    __NC_STACK_ypabact *bact = &ypabact;
    uamessage_viewer viewMsg;

    if ( vwr )
    {
        if (bact->yw->current_bact)
        {
            if ( bact->yw->current_bact->bact_type != BACT_TYPES_MISSLE )
                bact->salve_counter = 0;
        }

        bact->ywo->ypaworld_func131(bact); //Set current bact

        bact->oflags |= BACT_OFLAG_VIEWER;

        if ( bact->yw->isNetGame )
            viewMsg.view = 1;

        if ( bact->bact_type == BACT_TYPES_BACT && !(bact->status_flg & BACT_STFLAG_LAND) && bact->status == BACT_STATUS_NORMAL )
            bact->thraction = bact->force;

        startSound(&bact->soundcarrier, 8);
    }
    else
    {
        bact->oflags &= ~BACT_OFLAG_VIEWER;

        if ( bact->yw->isNetGame )
            viewMsg.view = 0;

        sub_424000(&bact->soundcarrier, 8);

        if ( bact->bact_type != BACT_TYPES_MISSLE && bact->bact_type != BACT_TYPES_ROBO && bact->status != BACT_STATUS_DEAD )
        {
            if ( bact->host_station == bact->parent_bacto )
            {
                if ( !(bact->status_flg & BACT_STFLAG_WAYPOINT) || !(bact->status_flg & BACT_STFLAG_WAYPOINTCCL) )
                {
                    bact_node *node = (bact_node *)bact->subjects_list.head;

                    while(node->next)
                    {
                        sub_493DB0(node->bact, bact, bact->ywo);

                        node = (bact_node *)node->next;
                    }
                }
            }
            else
            {
                if ( !(bact->status_flg & BACT_STFLAG_WAYPOINT) || !(bact->status_flg & BACT_STFLAG_WAYPOINTCCL) )
                    sub_493DB0(bact, bact->parent_bact, bact->ywo);
            }
        }
    }

    if ( bact->yw->isNetGame ) // Network message send routine?
    {
        viewMsg.msgID = UAMSG_VIEWER;
        viewMsg.owner = bact->owner;
        viewMsg.classID = bact->bact_type;
        viewMsg.id = bact->gid;

        if ( viewMsg.classID == BACT_TYPES_MISSLE )
        {
            NC_STACK_ypamissile *miss = dynamic_cast<NC_STACK_ypamissile *>(this);
            __NC_STACK_ypabact *a4 = miss->getMISS_launcher();
            viewMsg.launcher = a4->gid;
        }

        yw_arg181 v13;
        v13.recvID = 0;
        v13.recvFlags = 2;
        v13.data = &viewMsg;
        v13.garant = 1;
        v13.dataSize = sizeof(viewMsg);

        bact->ywo->ypaworld_func181(&v13);
    }
}

void NC_STACK_ypabact::setBACT_inputting(int inpt)
{
    __NC_STACK_ypabact *bact = &ypabact;

    if ( inpt )
    {
        bact->oflags |= BACT_OFLAG_USERINPT;
        bact->ywo->setYW_userVehicle(this);

        if ( bact->bact_type != BACT_TYPES_GUN )
            CorrectPositionOnLand(NULL);
    }
    else
    {
        bact->oflags &= ~BACT_OFLAG_USERINPT;
    }
}

void NC_STACK_ypabact::setBACT_exactCollisions(int col)
{
    if ( col )
        ypabact.oflags |= BACT_OFLAG_EXACTCOLL;
    else
        ypabact.oflags &= ~BACT_OFLAG_EXACTCOLL;
}

void NC_STACK_ypabact::setBACT_bactCollisions(int col)
{
    if ( col )
        ypabact.oflags |= BACT_OFLAG_BACTCOLL;
    else
        ypabact.oflags &= ~BACT_OFLAG_BACTCOLL;
}

void NC_STACK_ypabact::setBACT_airconst(int air)
{
    ypabact.airconst = air;
    ypabact.airconst_static = air;
}

void NC_STACK_ypabact::setBACT_landingOnWait(int lnding)
{
    if ( lnding )
        ypabact.oflags |= BACT_OFLAG_LANDONWAIT;
    else
        ypabact.oflags &= ~BACT_OFLAG_LANDONWAIT;
}

void NC_STACK_ypabact::setBACT_yourLastSeconds(int ls)
{
    ypabact.yls_time = ls;
}

void NC_STACK_ypabact::setBACT_visProto(NC_STACK_base *vp)
{
    ypabact.current_vp.base = vp;
}

void NC_STACK_ypabact::setBACT_aggression(int aggr)
{
    ypabact.aggr = aggr;

    bact_node *nod = (bact_node *)ypabact.subjects_list.head;

    while ( nod->next )
    {
        nod->bact->aggr = aggr;

        nod = (bact_node *)nod->next;
    }
}

void NC_STACK_ypabact::setBACT_vpTransform(TForm3D *tr)
{
    ypabact.current_vp.trigo = tr;
}

void NC_STACK_ypabact::setBACT_extraViewer(int vwr)
{
    if ( vwr )
        ypabact.oflags |= BACT_OFLAG_EXTRAVIEW;
    else
        ypabact.oflags &= ~BACT_OFLAG_EXTRAVIEW;
}

void NC_STACK_ypabact::setBACT_alwaysRender(int rndr)
{
    if ( rndr )
        ypabact.oflags |= BACT_OFLAG_ALWAYSREND;
    else
        ypabact.oflags &= ~BACT_OFLAG_ALWAYSREND;
}



NC_STACK_ypaworld *NC_STACK_ypabact::getBACT_pWorld()
{
    return ypabact.ywo;
}

TForm3D *NC_STACK_ypabact::getBACT_pTransform()
{
    return &ypabact.tForm;
}

__NC_STACK_ypabact *NC_STACK_ypabact::getBACT_pBact()
{
    return &ypabact;
}

int NC_STACK_ypabact::getBACT_viewer()
{
    return (ypabact.oflags & BACT_OFLAG_VIEWER) != 0;
}

int NC_STACK_ypabact::getBACT_inputting()
{
    return (ypabact.oflags & BACT_OFLAG_USERINPT) != 0;
}

int NC_STACK_ypabact::getBACT_exactCollisions()
{
    return (ypabact.oflags & BACT_OFLAG_EXACTCOLL) != 0;
}

int NC_STACK_ypabact::getBACT_bactCollisions()
{
    return (ypabact.oflags & BACT_OFLAG_BACTCOLL) != 0;
}

nlist *NC_STACK_ypabact::getBACT_attackList()
{
    return &ypabact.attackers_list;
}

int NC_STACK_ypabact::getBACT_landingOnWait()
{
    return (ypabact.oflags & BACT_OFLAG_LANDONWAIT) != 0;
}

int NC_STACK_ypabact::getBACT_yourLastSeconds()
{
    return ypabact.yls_time;
}

NC_STACK_base *NC_STACK_ypabact::getBACT_visProto()
{
    return ypabact.current_vp.base;
}

int NC_STACK_ypabact::getBACT_aggression()
{
    return ypabact.aggr;
}

rbcolls *NC_STACK_ypabact::getBACT_collNodes()
{
    return NULL;
}

TForm3D *NC_STACK_ypabact::getBACT_vpTransform()
{
    return ypabact.current_vp.trigo;
}

int NC_STACK_ypabact::getBACT_extraViewer()
{
    return (ypabact.oflags & BACT_OFLAG_EXTRAVIEW) != 0;
}

bact_node *NC_STACK_ypabact::getBACT_primAttackNode()
{
    return &ypabact.attack_node_prim;
}

bact_node *NC_STACK_ypabact::getBACT_secnAttackNode()
{
    return &ypabact.attack_node_scnd;
}

int NC_STACK_ypabact::getBACT_alwaysRender()
{
    return (ypabact.oflags & BACT_OFLAG_ALWAYSREND) != 0;
}



size_t NC_STACK_ypabact::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (IDVList *)data );
    case 1:
        return (size_t)func1();
    case 2:
        return (size_t)func2( (IDVList *)data );
    case 3:
        return (size_t)func3( (IDVList *)data );
    case 65:
        Update( (update_msg *)data );
        return 1;
    case 66:
        Render( (baseRender_msg *)data );
        return 1;
    case 67:
        SetTarget( (setTarget_msg *)data );
        return 1;
    case 68:
        AI_layer1( (update_msg *)data );
        return 1;
    case 69:
        AI_layer2( (update_msg *)data );
        return 1;
    case 70:
        AI_layer3( (update_msg *)data );
        return 1;
    case 71:
        User_layer( (update_msg *)data );
        return 1;
    case 72:
        AddSubject( (NC_STACK_ypabact *)data );
        return 1;
    case 73:
        SetNewMaster( (newMaster_msg *)data );
        return 1;
    case 74:
        Move( (move_msg *)data );
        return 1;
    case 75:
        FightWithBact( (bact_arg75 *)data );
        return 1;
    case 76:
        FightWithSect( (bact_arg75 *)data );
        return 1;
    case 77:
        Die();
        return 1;
    case 78:
        SetState( (setState_msg *)data );
        return 1;
    case 79:
        return (size_t)LaunchMissile( (bact_arg79 *)data );
    case 80:
        return (size_t)SetPosition( (bact_arg80 *)data );
    case 81:
        GetSummary( (bact_arg81 *)data );
        return 1;
    case 82:
        EnergyInteract( (update_msg *)data );
        return 1;
    case 83:
        ApplyImpulse( (bact_arg83 *)data );
        return 1;
    case 84:
        ModifyEnergy( (bact_arg84 *)data );
        return 1;
    case 85:
        return ypabact_func85( (vec3d *)data );
    case 86:
        return (size_t)CrashOrLand( (bact_arg86 *)data );
    case 87:
        return (size_t)CollisionWithBact( (int)(size_t)data );
    case 88:
        Recoil( (bact_arg88 *)data );
        return 1;
    case 89:
        ypabact_func89( (IDVPair *)data );
        return 1;
    case 90:
        GetSectorTarget( (bact_arg90 *)data );
        return 1;
    case 91:
        GetBestSectorPart( (vec3d *)data );
        return 1;
    case 92:
        GetForcesRatio( (bact_arg92 *)data );
        return 1;
    case 93:
        ypabact_func93( (IDVPair *)data );
        return 1;
    case 94:
        GetFormationPosition( (bact_arg94 *)data );
        return 1;
    case 95:
        ypabact_func95( (IDVPair *)data );
        return 1;
    case 96:
        Renew();
        return 1;
    case 97:
        HandBrake( (update_msg *)data );
        return 1;
    case 98:
        ypabact_func98( (IDVPair *)data );
        return 1;
    case 99:
        CreationTimeUpdate( (update_msg *)data );
        return 1;
    case 100:
        return (size_t)IsDestroyed();
    case 101:
        return (size_t)CheckFireAI( (bact_arg101 *)data );
    case 102:
        MarkSectorsForView( (IDVPair *)data );
        return 1;
    case 103:
        ypabact_func103( (IDVPair *)data );
        return 1;
    case 104:
        StuckFree( (update_msg *)data );
        return 1;
    case 105:
        return (size_t)FireMinigun( (bact_arg105 *)data );
    case 106:
        return (size_t)UserTargeting( (bact_arg106 *)data );
    case 107:
        HandleVisChildrens( (int *)data );
        return 1;
    case 108:
        return (size_t)GetFightMotivation( (float *)data );
    case 109:
        ReorganizeGroup( (bact_arg109 *)data );
        return 1;
    case 110:
        return (size_t)TargetAssess( (bact_arg110 *)data );
    case 111:
        return (size_t)TestTargetSector( (__NC_STACK_ypabact *)data );
    case 112:
        BeamingTimeUpdate( (update_msg *)data );
        return 1;
    case 113:
        StartDestFX( (uint8_t)(size_t)data );
        return 1;
    case 114:
        CorrectPositionOnLand( (void *)data );
        return 1;
    case 115:
        CorrectPositionInLevelBox( (void *)data );
        return 1;
    case 116:
        NetUpdate( (update_msg *)data );
        return 1;
    case 117:
        ypabact_func117( (update_msg *)data );
        return 1;
    case 118:
        Release( (NC_STACK_ypabact *)data );
        return 1;
    case 119:
        return (size_t)SetStateInternal( (setState_msg *)data );
    case 120:
        ChangeSectorEnergy( (yw_arg129 *)data );
        return 1;
    case 121:
        DeadTimeUpdate( (update_msg *)data );
        return 1;
    case 122:
        ypabact_func122( (update_msg *)data );
        return 1;
    case 123:
        ypabact_func123( (update_msg *)data );
        return 1;
    case 124:
        return (size_t)PathFinder( (bact_arg124 *)data );
    case 125:
        return (size_t)SetPath( (bact_arg124 *)data );
    default:
        break;
    }
    return NC_STACK_nucleus::compatcall(method_id, data);
}
