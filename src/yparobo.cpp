#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "log.h"
#include "yw.h"
#include "yparobo.h"

#include "yw_net.h"

#include <math.h>

const NewClassDescr NC_STACK_yparobo::description("yparobo.class", &newinstance);


char **dword_54B0E0; // ypaworld strings
int dword_5B1128 = 1;

key_value_stru yparobo_keys[2] =
{
    {"game.newai", KEY_TYPE_BOOL, 1},
    {"game.timeline", KEY_TYPE_DIGIT, 600000}
};

robo_t2 stru_5B0628[100];
int dword_515138[8];

struct rbo_xy
{
    int x;
    int y;
};

rbo_xy word_5182AE[8] = { {-1, -1}, {0, -1}, {1, -1}, {-1, 0},
    { 1,  0}, {-1, 1}, {0,  1}, { 1, 1}
};


cellArea * yparobo_func0__sub0(__NC_STACK_yparobo *robo)
{

    yw_130arg sect_info;

    sect_info.pos_x = 600.0;
    sect_info.pos_z = -600.0;

    robo->wrld->ypaworld_func130(&sect_info);

    return sect_info.pcell;
}

size_t NC_STACK_yparobo::func0(IDVList *stak)
{
    if ( !NC_STACK_ypabact::func0(stak) )
        return 0;

    stack__yparobo.bact_internal = &ypabact;
    stack__yparobo.roboo = this;

    stack__yparobo.epConquer = 30;
    stack__yparobo.epDefense = 90;
    stack__yparobo.epRadar = 30;
    stack__yparobo.epPower = 50;
    stack__yparobo.epSafety = 50;
    stack__yparobo.epRecon = 80;
    stack__yparobo.epChangePlace = 100;
    stack__yparobo.field_1DA = 0;
    stack__yparobo.field_501 = 15;

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
                    stack__yparobo.wrld = (NC_STACK_ypaworld *)val.value.p_data;
                    stack__yparobo.wrld_yw = &stack__yparobo.wrld->stack__ypaworld;
                    break;

                case BACT_ATT_INPUTTING:
                    setBACT_inputting(val.value.i_data);
                    break;

                case ROBO_ATT_EP_CONQUER:
                    setROBO_epConquer(val.value.i_data);
                    break;

                case ROBO_ATT_EP_DEFENSE:
                    setROBO_epDefense(val.value.i_data);
                    break;

                case ROBO_ATT_EP_RADAR:
                    setROBO_epRadar(val.value.i_data);
                    break;

                case ROBO_ATT_EP_POWER:
                    setROBO_epPower(val.value.i_data);
                    break;

                case ROBO_ATT_EP_SAFETY:
                    setROBO_epSafety(val.value.i_data);
                    break;

                case ROBO_ATT_BATT_VEHICLE:
                    setROBO_battVehicle(val.value.i_data);
                    break;

                case ROBO_ATT_BATT_BUILDING:
                    setROBO_battBuilding(val.value.i_data);
                    break;

                case ROBO_ATT_BATT_BEAM:
                    setROBO_battBeam(val.value.i_data);
                    break;

                case ROBO_ATT_FILL_MODE:
                    setROBO_fillMode(val.value.i_data);
                    break;

                case ROBO_ATT_WAIT_SWAY:
                    setROBO_waitSway( val.value.i_data );
                    break;

                case ROBO_ATT_WAIT_ROTATE:
                    setROBO_waitRotate( val.value.i_data );
                    break;

                case ROBO_ATT_EP_CHANGEPLACE:
                    setROBO_epChangeplace(val.value.i_data);
                    break;

                case ROBO_ATT_EP_RECONNOITRE:
                    setROBO_epReconnoitre(val.value.i_data);
                    break;

                case ROBO_ATT_EP_ROBO:
                    setROBO_epRobo(val.value.i_data);
                    break;

                case ROBO_ATT_VIEWANGLE:
                    setROBO_viewAngle(val.value.i_data);
                    break;

                case ROBO_ATT_SAFDELAY:
                    setROBO_safDelay(val.value.i_data);
                    break;

                case ROBO_ATT_POWDELAY:
                    setROBO_powDelay(val.value.i_data);
                    break;

                case ROBO_ATT_RADDELAY:
                    setROBO_radDelay(val.value.i_data);
                    break;

                case ROBO_ATT_CPLDELAY:
                    setROBO_cplDelay(val.value.i_data);
                    break;

                case ROBO_ATT_DEFDELAY:
                    setROBO_defDelay(val.value.i_data);
                    break;

                case ROBO_ATT_CONDELAY:
                    setROBO_conDelay(val.value.i_data);
                    break;

                case ROBO_ATT_ROBDELAY:
                    setROBO_robDelay(val.value.i_data);
                    break;

                case ROBO_ATT_RECDELAY:
                    setROBO_recDelay(val.value.i_data);
                    break;

                default:
                    break;
                }
            }
        }
    }

    ypabact.bact_type = BACT_TYPES_ROBO;

    stack__yparobo.pcell = yparobo_func0__sub0(&stack__yparobo);

    dword_54B0E0 = stack__yparobo.wrld->getYW_localeStrings();

    return 1;
}

size_t NC_STACK_yparobo::func1()
{
    return NC_STACK_ypabact::func1();
}

size_t NC_STACK_yparobo::func2(IDVList *stak)
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
                case BACT_ATT_INPUTTING:
                    setBACT_inputting(val.value.i_data);
                    break;

                case ROBO_ATT_PROTO:
                    setROBO_proto((roboProto *)val.value.p_data);
                    break;

                case ROBO_ATT_EP_CONQUER:
                    setROBO_epConquer(val.value.i_data);
                    break;

                case ROBO_ATT_EP_DEFENSE:
                    setROBO_epDefense(val.value.i_data);
                    break;

                case ROBO_ATT_EP_RADAR:
                    setROBO_epRadar(val.value.i_data);
                    break;

                case ROBO_ATT_EP_POWER:
                    setROBO_epPower(val.value.i_data);
                    break;

                case ROBO_ATT_EP_SAFETY:
                    setROBO_epSafety(val.value.i_data);
                    break;

                case ROBO_ATT_COMM_COUNT:
                    setROBO_commCount(val.value.i_data);
                    break;

                case ROBO_ATT_BATT_VEHICLE:
                    setROBO_battVehicle(val.value.i_data);
                    break;

                case ROBO_ATT_BATT_BUILDING:
                    setROBO_battBuilding(val.value.i_data);
                    break;

                case ROBO_ATT_BATT_BEAM:
                    setROBO_battBeam(val.value.i_data);
                    break;

                case ROBO_ATT_FILL_MODE:
                    setROBO_fillMode(val.value.i_data);
                    break;

                case ROBO_ATT_WAIT_SWAY:
                    setROBO_waitSway( val.value.i_data );
                    break;

                case ROBO_ATT_WAIT_ROTATE:
                    setROBO_waitRotate( val.value.i_data );
                    break;

                case ROBO_ATT_EP_CHANGEPLACE:
                    setROBO_epChangeplace(val.value.i_data);
                    break;

                case ROBO_ATT_EP_RECONNOITRE:
                    setROBO_epReconnoitre(val.value.i_data);
                    break;

                case ROBO_ATT_EP_ROBO:
                    setROBO_epRobo(val.value.i_data);
                    break;

                case ROBO_ATT_VIEWANGLE:
                    setROBO_viewAngle(val.value.i_data);
                    break;

                case ROBO_ATT_SAFDELAY:
                    setROBO_safDelay(val.value.i_data);
                    break;

                case ROBO_ATT_POWDELAY:
                    setROBO_powDelay(val.value.i_data);
                    break;

                case ROBO_ATT_RADDELAY:
                    setROBO_radDelay(val.value.i_data);
                    break;

                case ROBO_ATT_CPLDELAY:
                    setROBO_cplDelay(val.value.i_data);
                    break;

                case ROBO_ATT_DEFDELAY:
                    setROBO_defDelay(val.value.i_data);
                    break;

                case ROBO_ATT_CONDELAY:
                    setROBO_conDelay(val.value.i_data);
                    break;

                case ROBO_ATT_ROBDELAY:
                    setROBO_robDelay(val.value.i_data);
                    break;

                case ROBO_ATT_RECDELAY:
                    setROBO_recDelay(val.value.i_data);
                    break;

                default:
                    break;
                }
            }
        }
    }

    return 1;
}

size_t NC_STACK_yparobo::func3(IDVList *stak)
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
                case BACT_ATT_COLLNODES:
                    *(rbcolls **)val.value.p_data = getBACT_collNodes();
                    break;

                case ROBO_ATT_EP_CONQUER:
                    *(int *)val.value.p_data = getROBO_epConquer();
                    break;

                case ROBO_ATT_EP_DEFENSE:
                    *(int *)val.value.p_data = getROBO_epDefense();
                    break;

                case ROBO_ATT_EP_RADAR:
                    *(int *)val.value.p_data = getROBO_epRadar();
                    break;

                case ROBO_ATT_EP_POWER:
                    *(int *)val.value.p_data = getROBO_epPower();
                    break;

                case ROBO_ATT_EP_SAFETY:
                    *(int *)val.value.p_data = getROBO_epSafety();
                    break;

                case ROBO_ATT_COMM_COUNT:
                    *(int *)val.value.p_data = getROBO_commCount();
                    break;

                case ROBO_ATT_BATT_VEHICLE:
                    *(int *)val.value.p_data = getROBO_battVehicle();
                    break;

                case ROBO_ATT_BATT_BUILDING:
                    *(int *)val.value.p_data = getROBO_battBuilding();
                    break;

                case ROBO_ATT_BATT_BEAM:
                    *(int *)val.value.p_data = getROBO_battBeam();
                    break;

                case ROBO_ATT_FILL_MODE:
                    *(int *)val.value.p_data = getROBO_fillMode();
                    break;

                case ROBO_ATT_GUNS:
                    *(roboGun **)val.value.p_data = getROBO_guns();
                    break;

                case ROBO_ATT_EP_CHANGEPLACE:
                    *(int *)val.value.p_data = getROBO_epChangeplace();
                    break;

                case ROBO_ATT_EP_RECONNOITRE:
                    *(int *)val.value.p_data = getROBO_epReconnoitre();
                    break;

                case ROBO_ATT_EP_ROBO:
                    *(int *)val.value.p_data = getROBO_epRobo();
                    break;

                case ROBO_ATT_ROBOSTATE:
                    *(int *)val.value.p_data = getROBO_roboState();
                    break;

                case ROBO_ATT_SAFDELAY:
                    *(int *)val.value.p_data = getROBO_safDelay();
                    break;

                case ROBO_ATT_POWDELAY:
                    *(int *)val.value.p_data = getROBO_powDelay();
                    break;

                case ROBO_ATT_RADDELAY:
                    *(int *)val.value.p_data = getROBO_radDelay();
                    break;

                case ROBO_ATT_CPLDELAY:
                    *(int *)val.value.p_data = getROBO_cplDelay();
                    break;

                case ROBO_ATT_DEFDELAY:
                    *(int *)val.value.p_data = getROBO_defDelay();
                    break;

                case ROBO_ATT_CONDELAY:
                    *(int *)val.value.p_data = getROBO_conDelay();
                    break;

                case ROBO_ATT_ROBDELAY:
                    *(int *)val.value.p_data = getROBO_robDelay();
                    break;

                case ROBO_ATT_RECDELAY:
                    *(int *)val.value.p_data = getROBO_recDelay();
                    break;

                case ROBO_ATT_LOADFLAGS:
                    *(int *)val.value.p_data = getROBO_loadFlags();
                    break;

                case ROBO_ATT_LOSSFLAGS:
                    *(int *)val.value.p_data = getROBO_lossFlags();
                    break;

                case ROBO_ATT_ABSRELOAD:
                    *(int *)val.value.p_data = getROBO_absReload();
                    break;

                default:
                    break;
                }
            }
        }
    }

    return 1;
}

void NC_STACK_yparobo::AI_layer1(update_msg *arg)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;

    if ( robo->bact_internal->status == BACT_STATUS_DEAD )
    {
        int a4 = getBACT_yourLastSeconds();
        a4 -= arg->frameTime;
        setBACT_yourLastSeconds(a4);
    }

    robo->bact_internal->airconst = robo->bact_internal->airconst_static;

    robo->bact_internal->soundcarrier.samples_data[0].pitch = robo->bact_internal->pitch;
    robo->bact_internal->soundcarrier.samples_data[0].volume = robo->bact_internal->volume;

    for (int i = 0; i < 8; i++)
    {
        if (robo->guns[i].gun_obj)
        {
            __NC_STACK_ypabact *gun_bact = robo->guns[i].gun_obj->getBACT_pBact();

            gun_bact->owner = robo->bact_internal->owner;
        }
    }

    EnergyInteract(arg);

    if ( robo->bact_internal->primTtype == BACT_TGT_TYPE_CELL )
        robo->bact_internal->target_vec = robo->bact_internal->primTpos - robo->bact_internal->position;
    else if ( robo->bact_internal->primTtype != BACT_TGT_TYPE_UNIT )
        robo->bact_internal->target_vec = vec3d(0.0, 0.0, 0.0);

    int v9 = getBACT_inputting();

    if ( v9 )
        User_layer(arg);
    else
        AI_layer3(arg);
}




void sub_4A6720(NC_STACK_ypaworld *ywo, __NC_STACK_ypabact *bact)
{
    yw_arg184 arg184;
    arg184.type = 4;
    arg184.t34.field_1 = bact->owner;
    arg184.t34.field_2 = bact->vehicleID;
    arg184.t34.field_4 = bact->position.x * 256.0 / bact->wrldX;
    arg184.t34.field_5 = bact->position.z * 256.0 / bact->wrldY;

    ywo->ypaworld_func184(&arg184);
}

int groundVehicleInSquad(__NC_STACK_ypabact *bact)
{
    if ( bact->bact_type == BACT_TYPES_TANK || bact->bact_type == BACT_TYPES_CAR )
        return 1;

    bact_node *node = (bact_node *)bact->subjects_list.head;

    while (node->next)
    {
        if (node->bact->bact_type == BACT_TYPES_TANK || node->bact->bact_type == BACT_TYPES_CAR)
            return 1;

        node = (bact_node *)node->next;
    }

    return 0;
}

void NC_STACK_yparobo::sub_4A9F24(bact_node *node)
{
    //__NC_STACK_ypabact *bact = robo->bact_internal;
    __NC_STACK_yparobo *robo = &stack__yparobo;

    if ( robo->roboState & ROBOSTATE_SETDOCKTARGET )
    {
        setTarget_msg arg67;
        arg67.tgt_type = robo->dock_tgType;
        arg67.priority = 0;
        arg67.tgt.pbact = robo->dock_tgt_bact;
        arg67.tgt_pos.x = robo->dock_tgt_pos.x;
        arg67.tgt_pos.z = robo->dock_tgt_pos.z;

        if ( arg67.tgt_type == 2 )
        {
            arg67.priority = robo->dock_tgt_comm_id;

            if ( yparobo_func132(&arg67) )
            {
                if ( !((1 << robo->bact_internal->owner) & arg67.tgt.pbact->pSector->view_mask) )
                {
                    arg67.tgt_type = BACT_TGT_TYPE_CELL;
                    arg67.tgt_pos.x = robo->dock_tgt_pos.x;
                    arg67.tgt_pos.z = robo->dock_tgt_pos.z;

                    robo->dock_aggr = 50;
                }
            }
            else
            {
                arg67.tgt_type = BACT_TGT_TYPE_CELL;
                arg67.tgt_pos.x = node->bact->position.x;
                arg67.tgt_pos.z = node->bact->position.z;
            }
            arg67.priority = 0;
        }

        if ( node->bact->status != BACT_STATUS_DEAD )
        {
            bact_arg124 arg124;
            arg124.from = node->bact->position.XZ();

            if ( arg67.tgt_type == 2 )
            {
                arg124.to = arg67.tgt.pbact->position.XZ();
            }
            else
            {
                arg124.to = arg67.tgt_pos.XZ();
            }

            arg124.field_12 = 1;
            arg124.steps_cnt = 32;

            if ( groundVehicleInSquad(node->bact) && node->bacto->PathFinder(&arg124) )
            {
                arg124.steps_cnt = 32;

                node->bacto->SetPath(&arg124);

                if ( arg67.tgt_type == 2 )
                {
                    node->bact->m_cmdID = arg67.tgt.pbact->commandID;
                    node->bact->m_owner = arg67.tgt.pbact->owner;
                }
            }
            else
            {
                node->bacto->SetTarget(&arg67);
            }
        }

        robo->roboState &= ~ROBOSTATE_SETDOCKTARGET;
    }

    node->bacto->setBACT_aggression(robo->dock_aggr);
    robo->dock_cnt = 0;
    robo->dock_user = 0;
    robo->dock_time = 2000;
    robo->roboState &= ~ROBOSTATE_USEVHCLPARTS;
}

void NC_STACK_yparobo::initForce(bact_node *unit)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = &ypabact;

    if ( robo->dock_time <= 0 )
    {
        if ( robo->dock_energ > 0 )
        {
            NC_STACK_ypabact *unt = NULL;

            int v6 = bact->energy - bact->energy_max * 0.2;

            if ( robo->roboState & ROBOSTATE_USEVHCLPARTS )
                v6 += robo->field_50D;

            bact_arg81 arg81;
            arg81.enrg_type = 3;
            arg81.enrg_sum = 0;

            GetSummary(&arg81);


            int v23 = (arg81.enrg_sum * 0.05 + 1.0) * (float)unit->bact->energy_max;

            if ( v6 > (signed int)v23 )
            {
                ypaworld_arg146 arg146;
                arg146.vehicle_id = unit->bact->vehicleID;
                arg146.pos = bact->position + robo->dock_pos;

                unt = robo->wrld->ypaworld_func146(&arg146);
            }

            if ( unt )
            {
                __NC_STACK_ypabact *untbct;
                untbct = unt->getBACT_pBact();

                untbct->status = BACT_STATUS_CREATE;
                untbct->owner = bact->owner;
                untbct->commandID = unit->bact->commandID;
                untbct->host_station = this;

                int v20 = getBACT_bactCollisions();
                unt->setBACT_bactCollisions(v20);

                unit->bacto->AddSubject(unt);

                setTarget_msg arg67;
                arg67.tgt_type = BACT_TGT_TYPE_CELL;
                arg67.priority = 0;
                arg67.tgt_pos.x = bact->position.x + bact->rotation.m20 * 1200.0 * 0.5;
                arg67.tgt_pos.z = bact->position.z + bact->rotation.m22 * 1200.0 * 0.5;
                unt->SetTarget(&arg67);

                if ( robo->wrld->stack__ypaworld.isNetGame )
                {
                    untbct->gid |= untbct->owner << 24;

                    uamessage_newVhcl nvMsg;
                    nvMsg.msgID = UAMSG_NEWVHCL;
                    nvMsg.owner = untbct->owner;
                    nvMsg.pos = untbct->position;
                    nvMsg.type = untbct->vehicleID;
                    nvMsg.parent = untbct->parent_bact->gid;
                    nvMsg.id = untbct->gid;
                    nvMsg.kind = 2;
                    nvMsg.commandID = unit->bact->commandID;

                    yw_arg181 arg181;
                    arg181.recvFlags = 2;
                    arg181.dataSize = sizeof(nvMsg);
                    arg181.recvID = 0;
                    arg181.garant = 1;
                    arg181.data = &nvMsg;

                    robo->wrld->ypaworld_func181(&arg181);
                }

                if ( robo->roboState & ROBOSTATE_USEVHCLPARTS )
                {
                    if ( v23 > robo->field_50D )
                    {
                        bact->energy -= v23 - robo->field_50D;
                        robo->field_50D = 0;
                    }
                    else
                    {
                        robo->field_50D -= v23;
                    }
                }
                else
                {
                    bact->energy -= v23;
                }

                robo->dock_energ -= untbct->energy;

                setState_msg arg78;
                arg78.setFlags = 0;
                arg78.unsetFlags = 0;
                arg78.newStatus = BACT_STATUS_CREATE;
                unt->SetState(&arg78);

                untbct->scale_time = untbct->energy_max * 0.2;
                robo->dock_time = untbct->scale_time + 2000;
                robo->dock_cnt++;

                sub_4A6720(robo->wrld, untbct);
            }
            else
            {
                sub_4A9F24(unit);
            }
        }
        else
        {
            sub_4A9F24(unit);
        }
    }
}


int sub_4A5A08(__NC_STACK_ypabact *bact, float a2, float a3)
{
    int v5 = 0;
    if ( bact->bact_type == BACT_TYPES_TANK || bact->bact_type == BACT_TYPES_CAR )
    {
        v5 = 1;
    }
    else
    {
        bact_node *node = (bact_node *)bact->subjects_list.head;
        while (node->next)
        {
            if (node->bact->bact_type == BACT_TYPES_TANK || node->bact->bact_type == BACT_TYPES_CAR)
            {
                v5 = 1;
                break;
            }

            node = (bact_node *)node->next;
        }
    }

    if ( v5 )
    {
        bact_arg124 arg125;
        arg125.steps_cnt = 32;
        arg125.from = bact->position.XZ();
        arg125.to = vec2d(a2, a3);
        arg125.field_12 = 1;
        if ( !bact->self->SetPath(&arg125) )
            return 0;
    }
    else
    {
        setTarget_msg arg67;
        arg67.tgt_pos.x = a2;
        arg67.tgt_pos.z = a3;
        arg67.tgt_type = BACT_TGT_TYPE_CELL;
        arg67.priority = 0;
        bact->self->SetTarget(&arg67);
    }
    return 1;
}

int sub_4A58C0(__NC_STACK_ypabact *bact, __NC_STACK_ypabact *bact2)
{
    int v5 = 0;
    if ( bact->bact_type == BACT_TYPES_TANK || bact->bact_type == BACT_TYPES_CAR )
    {
        v5 = 1;
    }
    else
    {
        bact_node *node = (bact_node *)bact->subjects_list.head;
        while (node->next)
        {
            if (node->bact->bact_type == BACT_TYPES_TANK || node->bact->bact_type == BACT_TYPES_CAR)
            {
                v5 = 1;
                break;
            }

            node = (bact_node *)node->next;
        }
    }

    if ( v5 )
    {
        bact_arg124 arg124;
        arg124.steps_cnt = 32;
        arg124.from = bact->position.XZ();
        arg124.to = bact2->position.XZ();
        arg124.field_12 = 1;

        if ( !bact->self->PathFinder(&arg124) )
            return 0;

        if ( arg124.steps_cnt > 1 )
        {
            arg124.steps_cnt = 32;
            bact->self->SetPath(&arg124);

            bact->m_cmdID = bact2->commandID;
            bact->m_owner = bact2->owner;
            return 1;
        }
    }

    setTarget_msg arg67;
    arg67.tgt.pbact = bact2;
    arg67.priority = 0;
    arg67.tgt_type = BACT_TGT_TYPE_UNIT;
    bact->self->SetTarget(&arg67);
    return 1;
}

void sb_0x4a7010__sub1__sub0(NC_STACK_ypabact *unit1, NC_STACK_ypabact *unit2)
{
    __NC_STACK_ypabact *bact1 = &unit1->ypabact;
    __NC_STACK_ypabact *bact2 = &unit2->ypabact;

    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            int v20 = i + bact1->sectX;

            if ( v20 > 0 && v20 < bact1->secMaxX - 1 )
            {
                int v21 = j + bact1->sectY;

                if ( v21 > 0 && v21 < bact1->secMaxY - 1 )
                {
                    cellArea *pcell = &bact1->pSector[i + j * bact1->secMaxX];

                    if ( pcell->owner == bact1->owner && pcell->w_type == 2 )
                    {
                        float yy = (v21 + 0.5) * -1200.0;
                        float xx = (v20 + 0.5) * 1200.0;
                        sub_4A5A08(bact2, xx, yy);

                        unit2->setBACT_aggression(25);
                        return;
                    }

                    if ( (1 << bact2->owner) & pcell->view_mask )
                    {
                        __NC_STACK_ypabact *v10 = (__NC_STACK_ypabact *)pcell->units_list.head;

                        while( v10->next )
                        {
                            if ( v10->bact_type == BACT_TYPES_GUN && bact1->owner == v10->owner && v10->status != BACT_STATUS_DEAD )
                            {
                                NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( v10->self );
                                int a4 = gun->getGUN_roboGun();

                                if ( !a4 )
                                {
                                    if ( v10->parent_bacto != v10->host_station )
                                        v10 = v10->parent_bact;

                                    sub_4A58C0(bact2, v10);

                                    unit2->setBACT_aggression(25);
                                    return;
                                }
                            }
                            v10 = (__NC_STACK_ypabact *)v10->next;
                        }
                    }
                }
            }
        }
    }

    if ( (1 << bact2->owner) & bact1->pSector->view_mask )
    {
        sub_4A58C0(bact2, bact1);

        unit2->setBACT_aggression(25);
    }
    else
    {
        float yy = (bact1->sectY + 0.5) * -1200.0;
        float xx = (bact1->sectX + 0.5) * 1200.0;

        sub_4A5A08(bact2, xx, yy);

        unit2->setBACT_aggression(50);
    }
}

void sb_0x4a7010__sub1(bact_node *unit, robo_t1 *rbt)
{
    __NC_STACK_ypabact *prnt = unit->bact->parent_bact;

    bact_node *v8 = NULL;

    bact_node *v6 = &prnt->subject_node;

    while (v6->prev) // Go to head
        v6 = (bact_node *)v6->prev;

    v6 = (bact_node *)v6->next; // Go to first

    while (v6->next)
    {
        if ( v6->bact->commandID == rbt->field_4 )
        {
            v8 = v6;
            break;
        }

        v6 = (bact_node *)v6->next;
    }

    if ( v8 )
    {
        if ( unit->bact->primTtype == BACT_TGT_TYPE_UNIT )
        {
            if ( unit->bact->primT.pbact->bact_type == BACT_TYPES_ROBO )
                sb_0x4a7010__sub1__sub0(v8->bacto, unit->bacto);
        }
        else if ( unit->bact->primTtype == BACT_TGT_TYPE_CELL )
        {
            cellArea *cell = unit->bact->primT.pcell;

            if (cell->owner == unit->bact->owner || v8->bact->pSector == cell)
                sb_0x4a7010__sub1__sub0(v8->bacto, unit->bacto);
        }
    }
    else
    {
        rbt->field_4 = 0;
        rbt->field_0 = 0;
    }
}

void NC_STACK_yparobo::doBeamUpdate(int a2)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = &ypabact;

    _NC_STACK_ypaworld *yw = &robo->wrld->stack__ypaworld;

    robo->field_511 -= a2;

    if ( robo->field_511 > 0 )
    {
        if ( robo->field_521 <= 0 )
        {
            int v23 = (1500 - robo->field_511) / 10;
            int v10 = robo->field_511 / 10;

            if ( bact->vp_extra[0].flags & EVPROTO_FLAG_ACTIVE )
            {
                robo->field_521 = v10;
                bact->vp_extra[0].flags &= ~EVPROTO_FLAG_ACTIVE;
            }
            else
            {
                robo->field_521 = v23;

                bact->vp_extra[0].pos = bact->position;
                bact->vp_extra[0].rotate = bact->rotation;
                bact->vp_extra[0].flags = (EVPROTO_FLAG_ACTIVE | EVPROTO_FLAG_SCALE);
                bact->vp_extra[0].scale = 1.0;
                bact->vp_extra[0].vp = bact->vp_genesis;
            }

            if ( robo->bact_internal->vp_extra[1].flags & EVPROTO_FLAG_ACTIVE )
            {
                robo->field_521 = v10;
                bact->vp_extra[1].flags &= ~EVPROTO_FLAG_ACTIVE;
            }
            else
            {
                robo->field_521 = v23;

                bact->vp_extra[1].pos = robo->field_515;
                bact->vp_extra[1].rotate = bact->rotation;
                bact->vp_extra[1].flags = EVPROTO_FLAG_ACTIVE;
                bact->vp_extra[1].vp = bact->vp_genesis;
            }
        }

        robo->field_521 -= a2;
    }
    else
    {
        robo->field_511 = 0;

        vec3d tmp = bact->position;

        tmp.y = robo->field_1D5;

        bact_arg80 v18;
        v18.pos = robo->field_515;
        v18.field_C = 0;

        SetPosition(&v18);

        if ( yw->isNetGame )
        {
            bact->status_flg |= BACT_STFLAG_DSETTED;

            bact_node *node = (bact_node *)bact->subjects_list.head;

            while ( node->next )
            {
                if ( node->bact->bact_type == BACT_TYPES_GUN )
                {
                    NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( node->bacto );
                    int a4 = gun->getGUN_roboGun();

                    if ( a4 )
                        node->bact->status_flg |= BACT_STFLAG_DSETTED;
                }
                node = (bact_node *)node->next;
            }
        }

        bact->old_pos = tmp;
        bact->old_pos.y = tmp.y;

        startSound(&bact->soundcarrier, 10);

        robo->roboState &= ~ROBOSTATE_MOVE;
        bact->vp_extra[0].flags = 0;
        bact->vp_extra[1].flags = 0;

        if ( yw->isNetGame )
        {
            uamessage_startBeam ebMsg;
            ebMsg.msgID = UAMSG_ENDBEAM;
            ebMsg.owner = bact->owner;
            ebMsg.id = bact->gid;

            yw_arg181 v16;
            v16.recvID = 0;
            v16.recvFlags = 2;
            v16.dataSize = sizeof(ebMsg);
            v16.garant = 1;
            v16.data = &ebMsg;

            yw->self_full->ypaworld_func181(&v16);
        }
    }
}

void sub_4A1270(__NC_STACK_yparobo *robo,const vec3d &vaxis, float angle)
{
    for (int i = 0; i < 8; i++)
    {
        if ( robo->guns[i].gun_obj )
        {
            NC_STACK_ypagun *gungun = dynamic_cast<NC_STACK_ypagun *>(robo->guns[i].gun_obj);

            if (gungun)
                robo->guns[i].dir = gungun->ypagun_func129(vaxis, -angle);
        }
    }
}

void sub_4A1014(__NC_STACK_yparobo *robo, float angle)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    bact->rotation = mat3x3::RotateY(angle) * bact->rotation;

    sub_4A1270(robo, bact->rotation.AxisY(), angle);

    robo->coll.field_0 = angle;
}

void sub_4A10E8(__NC_STACK_yparobo *robo, float angle)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    bact->rotation = mat3x3::RotateY(angle) * bact->rotation;

    sub_4A1270(robo, vec3d::OY(1.0), angle);

    robo->coll.field_0 = angle;
}

size_t NC_STACK_yparobo::checkCollisions(float a2)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = &ypabact;

    int a4 = getBACT_viewer();
    int v81 = getBACT_inputting();
    int v79 = getBACT_exactCollisions();

    if ( bact->position == bact->old_pos )
        return 0;

    bact_arg88 arg88_2;
    arg88_2.pos1 = vec3d(0.0, 0.0, 0.0);

    int v86 = 0;

    ypaworld_arg136 arg136;

    for (int i = 0; i < robo->coll.robo_coll_num; i++)
    {
        if ( !v81 || bact->primTtype )
        {
            vec3d tmp = robo->coll.roboColls[i].field_10;
            vec3d t = bact->position + bact->rotation.Transform( robo->coll.roboColls[i].coll_pos );

            arg136.stPos = tmp;
            arg136.flags = 0;

            float v82 = (tmp.XZ() - t.XZ()).length();

            if ( v82 <= 1.0 )
                arg136.vect = bact->fly_dir * 300.0;
            else
                arg136.vect = vec3d::X0Z( (t.XZ() - tmp.XZ()) * 300.0 / v82 );

            robo->wrld->ypaworld_func136(&arg136);


            if ( arg136.isect )
            {
                if ( arg136.tVal * 300.0 >= robo->coll.roboColls[i].robo_coll_radius + v82)
                {
                    bact->status_flg &= ~BACT_STFLAG_LCRASH;
                    bact->status_flg |= BACT_STFLAG_UPWRD;
                    return 0;
                }
                else if ( robo->coll.roboColls[i].robo_coll_radius > 0.01 )
                {
                    bact_arg88 arg88;
                    arg88.pos1 = arg136.skel->polygons[ arg136.polyID ].Normal();

                    Recoil(&arg88);

                    if ( fabs(robo->coll.field_0) > 0.0 )
                    {
                        sub_4A1014(robo, -robo->coll.field_0);

                        robo->coll.field_0 = 0;
                    }

                    yw_130arg arg130;
                    arg130.pos_x = arg136.isectPos.x;
                    arg130.pos_z = arg136.isectPos.z;

                    robo->wrld->ypaworld_func130(&arg130);

                    if ( v81 || !arg130.pcell->w_type )
                    {
                        yw_arg129 v60;
                        v60.pos = arg136.isectPos;
                        v60.unit = NULL;
                        v60.field_14 = 255;
                        v60.field_10 = v82 * 15000.0 / a2;

                        ChangeSectorEnergy(&v60);
                    }

                    bact->fly_dir_length *= 0.4;
                    bact->status_flg |= BACT_STFLAG_UPWRD;

                    return 1;
                }
            }
        }


        int v26 = robo->wrld->ypaworld_func145(bact);

        if ( (a4 || (v26 && v79)) && robo->coll.roboColls[i].robo_coll_radius > 0.01 )
        {
            vec3d tmp = robo->coll.roboColls[i].field_10;
            vec3d t = bact->position + bact->rotation.Transform( robo->coll.roboColls[i].coll_pos );

            ypaworld_arg137 arg137;
            arg137.pos = tmp;

            float v83 = (tmp.XZ() - t.XZ()).length();

            if ( v83 <= 1.0 )
                arg137.pos2 = bact->fly_dir;
            else
            {
                arg137.pos2 = vec3d::X0Z( (t.XZ() - tmp.XZ()) ) / v83;

                // TYPO??
                /*arg137.pos2.x = 1.0 / v83 * (t.x - tmp.x);
                arg137.pos2.y = 0;
                arg137.pos2.z = 1.0 / v83 * (t.x - tmp.x); */
            }

            yw_137col v57[32];

            arg137.radius = robo->coll.roboColls[i].robo_coll_radius;
            arg137.collisions = v57;
            arg137.field_30 = 0;
            arg137.coll_max = 32;

            robo->wrld->ypaworld_func137(&arg137);

            if ( arg137.coll_count )
            {
                v86++;

                float v89 = v83 * 15000.0;

                vec3d ttmp(0.0, 0.0, 0.0);

                for (int i = arg137.coll_count - 1; i >= 0; i--)
                {
                    yw_137col *clzn = &arg137.collisions[i];

                    ttmp += clzn->pos2;

                    yw_130arg arg130;
                    arg130.pos_x = clzn->pos1.x;
                    arg130.pos_z = clzn->pos1.z;

                    robo->wrld->ypaworld_func130(&arg130);

                    if ( v81 || !arg130.pcell->w_type )
                    {
                        yw_arg129 v60;
                        v60.pos = clzn->pos1;
                        v60.unit = NULL;
                        v60.field_14 = 255;
                        v60.field_10 = v89 / a2;

                        ChangeSectorEnergy(&v60);
                    }
                }

                float v84 = ttmp.length();

                if ( v84 != 0.0 )
                    arg88_2.pos1 += ttmp / v84;
                else
                    arg88_2.pos1 += bact->fly_dir;
            }

            if ( v86 )
            {
                arg88_2.pos1 /= (float)v86;

                bact->position = bact->old_pos;


                Recoil(&arg88_2);

                if ( fabs(robo->coll.field_0) > 0.0 )
                {
                    sub_4A1014(robo, -robo->coll.field_0);
                    robo->coll.field_0 = 0;
                }

                bact->fly_dir_length *= 0.4;
                bact->status_flg |= BACT_STFLAG_UPWRD;

                return 1;
            }
        }
    }

    bact->status_flg &= ~BACT_STFLAG_LCRASH;

    arg136.stPos = bact->position;
    arg136.vect = vec3d::OY( bact->height * 1.5 ) + bact->fly_dir.X0Z() * 100.0;
    arg136.flags = 0;

    robo->wrld->ypaworld_func136(&arg136);

    if ( arg136.isect )
    {
        if ( arg136.tVal < 0.66 )
        {
            bact->target_dir.y = -1.0;
            bact->status_flg |= BACT_STFLAG_UPWRD;
        }
    }
    else
    {
        bact->target_dir.y = 1.0;
        bact->status_flg &= ~BACT_STFLAG_UPWRD;
    }

    robo->coll.field_0 = 0;

    return 0;
}

void NC_STACK_yparobo::wallow(update_msg *arg)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = &ypabact;

    bact->position.y = sin(arg->gTime * C_PI / 3000.0) * 25.0 + robo->field_1D5;

    for (int i = 0; i < 8; i++)
    {
        roboGun *gun = &robo->guns[i];

        if (gun->gun_obj)
        {
            bact_arg80 v11;
            v11.pos = bact->position + bact->rotation.Transpose().Transform(gun->pos);
            v11.field_C = 4;

            gun->gun_obj->SetPosition(&v11);
        }
    }
}

void yparobo_func70__sub2__sub0(__NC_STACK_yparobo *robo)
{

    __NC_STACK_ypabact *bact = robo->bact_internal;

    for (int i = 0; i < robo->coll.robo_coll_num; i++)
        robo->coll.roboColls[i].field_10 = bact->position + bact->rotation.Transform( robo->coll.roboColls[i].coll_pos );
}

int yparobo_func70__sub2__sub1(__NC_STACK_yparobo *robo)
{
    NC_STACK_ypabact *yw_1b7c = robo->wrld->getYW_userVehicle();

    if ( yw_1b7c == robo->bact_internal->self )
        return 0;

    for (int i = 0; i < 8; i++)
        if (robo->guns[i].gun_obj && robo->guns[i].gun_obj == yw_1b7c)
            return 0;

    return 1;
}

void NC_STACK_yparobo::AI_doMove(update_msg *arg)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = &ypabact;

    move_msg arg74;

    yparobo_func70__sub2__sub0(robo);

    float v49 = arg->frameTime / 1000.0;

    robo->field_c = bact->mass * 9.80665;

    if ( bact->target_dir.y < -0.7 )
        robo->field_c = bact->mass * 3.0 * 9.80665;

    if ( bact->target_dir.y > 0.7 )
        robo->field_c = bact->mass * 0.91 * 9.80665;

    float v39;
    if ( bact->primTtype )
        v39 = bact->target_vec.XZ().length();
    else
        v39 = 0.0;

    if ( v39 >= 100.0 )
    {
        if ( bact->status_flg & BACT_STFLAG_UPWRD )
        {
            bact->airconst *= 3.0;

            robo->field_c = bact->mass * 2.5 * 9.80665;
            bact->thraction = 0;

            float v38 = 1.0 - v49 * 4.0;
            if ( v38 < 0.0 )
                v38 = 0.1;

            bact->fly_dir_length *= v38;
        }
        else if ( bact->primTtype )
        {
            bact->thraction = bact->force;
        }
        else
        {
            bact->thraction = 0;
        }

        if ( !(bact->status_flg & BACT_STFLAG_UPWRD) )
        {
            float v52 = ypabact.rotation.AxisZ().XZ().length();
            float v51 = ypabact.target_dir.XZ().length();

            if ( v52 <= 0.0001 || v51 <= 0.0001 )
            {
                bact->slider_time = 0;
                bact->status_flg &= ~BACT_STFLAG_XLEFT;
            }
            else
            {
                float v47 = ypabact.target_dir.XZ().dot( ypabact.rotation.AxisZ().XZ() ) / (v51 * v52);

                float v23 = clp_acos(v47);

                float v24 = fabs(v23);

                if ( v24 >= 0.001 )
                {
                    float mxx = v49 * bact->maxrot;

                    if ( v24 > mxx )
                        v23 = mxx;

                    if ( ypabact.rotation.AxisZ().XZ().cross( ypabact.target_dir.XZ() ) < 0.0 )
                        v23 = -v23;

                    float v28;

                    if ( v23 <= 0.0 )
                    {
                        if ( !(bact->status_flg & BACT_STFLAG_XLEFT) || !bact->slider_time )
                            bact->slider_time = arg->gTime;

                        v28 = (arg->gTime - bact->slider_time) * 0.9 * 0.001 + 0.1;
                        bact->status_flg |= BACT_STFLAG_XLEFT;
                    }
                    else
                    {
                        if ( bact->status_flg & BACT_STFLAG_XLEFT || !bact->slider_time )
                            bact->slider_time = arg->gTime;

                        v28 = (arg->gTime - bact->slider_time) * 0.9 * 0.001 + 0.1;
                        bact->status_flg &= ~BACT_STFLAG_XLEFT;
                    }

                    if ( v28 > 1.0 )
                        v28 = 1.0;

                    v23 *= v28;

                    sub_4A10E8(robo, v23);
                }
            }
        }

        arg74.field_0 = v49;
        arg74.flag = 0;
        arg74.vec = bact->target_vec.X0Z() / v39;
    }
    else
    {
        bact->thraction = 0;

        if ( fabs(bact->fly_dir.x * bact->fly_dir_length) <= 0.1 && fabs(bact->fly_dir.z * bact->fly_dir_length) <= 0.1 )
        {
            if ( !(robo->roboState & ROBOSTATE_BUILDRADAR) &&
                    !(robo->roboState & ROBOSTATE_BUILDPOWER) &&
                    !(robo->roboState & ROBOSTATE_BUILDSAFE) )
            {
                int v14 = yparobo_func70__sub2__sub1(robo);

                if ( v14 && robo->field_1DA & 1 )
                {
                    float v18;
                    if ( bact->status_flg & BACT_STFLAG_UPWRD )
                        v18 = bact->mass * 1.5;
                    else
                        v18 = bact->mass * 0.82;

                    robo->field_c = v18 * 9.80665;
                }
                else
                {
                    if ( fabs(bact->target_dir.y) < 0.7 )
                        bact->fly_dir_length *= 0.35;
                }

                if ( v14 && robo->field_1DA & 2 )
                {
                    sub_4A1014(robo, bact->maxrot * 0.3 * v49);
                }
            }
        }
        else
        {
            bact->airconst *= 3.0;
            bact->fly_dir_length *= 0.8;

            if ( fabs(bact->fly_dir.x * bact->fly_dir_length) < 0.1 && fabs(bact->fly_dir.z * bact->fly_dir_length) < 0.1 )
            {
                bact->fly_dir.z = 0;
                bact->fly_dir.x = 0;

                setTarget_msg v34;
                v34.tgt_type = BACT_TGT_TYPE_NONE;
                v34.priority = 0;
                SetTarget(&v34);
            }
        }
        arg74.vec = vec3d(0.0, 0.0, 0.0);
        arg74.flag = 0;
        arg74.field_0 = v49;
    }

    Move(&arg74);
}

void sub_4A4538(__NC_STACK_ypabact *bact)
{
    setTarget_msg arg67;
    arg67.tgt_type = BACT_TGT_TYPE_NONE;
    arg67.priority = 1;

    bact->self->SetTarget(&arg67);

    bact_node *node = (bact_node *)bact->subjects_list.head;

    while( node->next )
    {
        node->bacto->SetTarget(&arg67);

        node = (bact_node *)node->next;
    }
}

void sub_4A448C(__NC_STACK_ypabact *bact)
{

    bact_node *node = (bact_node *)bact->subjects_list.head;

    while( node->next )
    {
        node->bact->waypoints_count = bact->waypoints_count;

        if ( node->bact->waypoints_count == 1 )
            node->bact->current_waypoint = 0;

        node->bact->status_flg |= BACT_STFLAG_WAYPOINT;

        if ( bact->status_flg & BACT_STFLAG_WAYPOINTCCL )
            node->bact->status_flg |= BACT_STFLAG_WAYPOINTCCL;
        else
            node->bact->status_flg &= ~BACT_STFLAG_WAYPOINTCCL;

        for (int i = 0; i < 32; i++)
            node->bact->waypoints[i] = bact->waypoints[i];

        node = (bact_node *)node->next;
    }
}

void sub_4AB69C(__NC_STACK_ypabact *bact, char a2)
{
    if ( a2 )
        bact->status_flg |= BACT_STFLAG_ESCAPE;
    else
        bact->status_flg &= ~BACT_STFLAG_ESCAPE;

    bact_node *node = (bact_node *)bact->subjects_list.head;

    while( node->next )
    {
        if ( a2 )
            node->bact->status_flg |= BACT_STFLAG_ESCAPE;
        else
            node->bact->status_flg &= ~BACT_STFLAG_ESCAPE;

        node = (bact_node *)node->next;
    }
}

int sb_0x4a45cc__sub0(__NC_STACK_ypabact *bact)
{
    vec3d tmp;
    if ( bact->primTtype == BACT_TGT_TYPE_UNIT )
    {
        tmp = bact->primT.pbact->position;
    }
    else if ( bact->primTtype == BACT_TGT_TYPE_CELL )
    {
        tmp = bact->primTpos;
    }
    else
        return 1;

    vec3d tmp2 = bact->position - tmp;

    float v19 = tmp2.length();

    bact_node *nod = (bact_node *)bact->subjects_list.head;

    while( nod->next)
    {
        __NC_STACK_ypabact *bct = nod->bact;

        vec3d tmp3 = bct->position - tmp;

        if ( tmp3.length() < v19 )
            return 0;

        nod = (bact_node *)nod->next;
    }
    return 1;
}

void NC_STACK_yparobo::doUserCommands(update_msg *arg)
{
    setTarget_msg arg67;
    ypaworld_arg146 arg146;
    setState_msg arg78;
    bact_arg109 arg109;
    ypaworld_arg148 arg148;
    yw_arg181 arg181;
    robo_arg134 arg134;
    bact_arg124 arg124;

    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = &ypabact;
    _NC_STACK_ypaworld *yw = robo->wrld_yw;
    NC_STACK_ypaworld *ywo = robo->wrld;


    int v6;

    switch ( arg->user_action )
    {
    case 2: //Move squad
        arg->selectBact->status_flg &= ~(BACT_STFLAG_WAYPOINT | BACT_STFLAG_WAYPOINTCCL);

        if ( arg->target_Sect )
        {
            arg67.tgt_pos.x = arg->target_point.x;
            arg67.tgt_pos.z = arg->target_point.z;
            arg67.tgt_type = BACT_TGT_TYPE_CELL;
        }
        else
        {
            arg67.tgt_type = BACT_TGT_TYPE_UNIT;
            arg67.tgt.pbact = arg->target_Bact;
        }

        arg67.priority = 0;
        arg->selectBact->self->SetTarget(&arg67);

        if ( !sb_0x4a45cc__sub0(arg->selectBact) )
        {
            arg109.field_4 = NULL;
            arg109.field_0 = 6;

            arg->selectBact->self->ReorganizeGroup(&arg109);
        }

        if ( arg->selectBact->host_station != arg->selectBact->parent_bacto )
            arg->selectBact = arg->selectBact->parent_bact;

        if ( arg->target_Sect )
            v6 = sub_4A5A08(arg->selectBact, arg->target_point.x, arg->target_point.z);
        else
            v6 = sub_4A58C0(arg->selectBact, arg->target_Bact);

        if ( !v6 )
        {
            if ( yw->GameShell )
                startSound(&yw->GameShell->samples2_info, 1);
        }
        else
        {

            __NC_STACK_ypabact *v8 = arg->selectBact;

            if ( v8->parent_bacto != v8->host_station )
                v8 = v8->parent_bact;

            arg134.unit = v8;
            arg134.field_C = 0;
            arg134.field_8 = 0;
            arg134.field_10 = 0;

            if ( v8->primTtype == BACT_TGT_TYPE_CELL )
            {
                cellArea *cell = v8->primT.pcell;
                if ( cell->owner == bact->owner )
                {
                    arg134.field_4 = 15;
                }
                else
                {
                    arg134.field_4 = 16;
                }
            }
            else
            {
                if ( v8->primT.pbact->owner == bact->owner )
                {
                    arg134.field_4 = 15;
                }
                else
                {
                    arg134.field_4 = 16;
                }
            }

            arg134.field_14 = 38;
            placeMessage(&arg134);
        }
        sub_4A4538(arg->selectBact);
        sub_4AB69C(arg->selectBact, 0);
        break;

    case 3:
        if ( arg->energy <= robo->field_4F5 )
        {
            arg146.pos = arg->target_point;
            arg146.vehicle_id = arg->protoID;

            NC_STACK_ypabact *newbact = ywo->ypaworld_func146(&arg146);

            if ( newbact )
            {
                arg67.tgt_type = BACT_TGT_TYPE_CELL;
                arg67.priority = 0;
                arg67.tgt_pos.x = (arg146.pos.x - bact->position.x) * 4.0 + bact->position.x;
                arg67.tgt_pos.y = arg146.pos.y;
                arg67.tgt_pos.z = (arg146.pos.z - bact->position.z) * 4.0 + bact->position.z;

                newbact->SetTarget(&arg67);

                arg78.newStatus = BACT_STATUS_CREATE;
                arg78.setFlags = 0;
                arg78.unsetFlags = 0;

                newbact->SetState(&arg78);

                __NC_STACK_ypabact *bct;
                bct = newbact->getBACT_pBact();

                bct->scale_time = bct->energy_max * 0.2;
                bct->commandID = dword_5B1128;
                dword_5B1128++;

                bct->owner = bact->owner;
                AddSubject(newbact);

                if ( yw->isNetGame )
                {
                    bct->gid |= bct->owner << 24;
                    bct->commandID |= bct->owner << 24;

                    uamessage_newVhcl nvMsg;
                    nvMsg.msgID = UAMSG_NEWVHCL;
                    nvMsg.owner = bct->owner;
                    nvMsg.pos = bct->position;
                    nvMsg.type = bct->vehicleID;
                    nvMsg.parent = bct->parent_bact->gid;
                    nvMsg.id = bct->gid;
                    nvMsg.kind = 1;
                    nvMsg.commandID = bct->commandID;

                    arg181.recvFlags = 2;
                    arg181.dataSize = sizeof(nvMsg);
                    arg181.recvID = 0;
                    arg181.garant = 1;
                    arg181.data = &nvMsg;
                    ywo->ypaworld_func181(&arg181);
                }

                if ( bct->bact_type == BACT_TYPES_UFO )
                    newbact->setBACT_aggression(25);
                else
                    newbact->setBACT_aggression(60);

                int v67 = getBACT_bactCollisions();
                newbact->setBACT_bactCollisions(v67);

                bct->host_station = this;
                robo->field_4F5 -= arg->energy;

                sub_4A6720(ywo, bct);
            }
        }
        break;

    case 4:
        if ( arg->energy <= robo->field_4F5 )
        {
            arg146.pos = arg->target_point;
            arg146.vehicle_id = arg->protoID;
            NC_STACK_ypabact *newbact2 = ywo->ypaworld_func146(&arg146);

            if ( newbact2 )
            {
                arg78.newStatus = BACT_STATUS_CREATE;
                arg78.setFlags = 0;
                arg78.unsetFlags = 0;
                newbact2->SetState(&arg78);

                __NC_STACK_ypabact *bct;
                bct = newbact2->getBACT_pBact();

                bct->scale_time = bct->energy_max * 0.2;
                bct->owner = bact->owner;
                bct->commandID = arg->selectBact->commandID;

                arg->selectBact->self->AddSubject(newbact2);

                bct->aggr = arg->selectBact->aggr;

                if ( yw->isNetGame )
                {
                    bct->gid |= bct->owner << 24;

                    uamessage_newVhcl nvMsg;
                    nvMsg.msgID = UAMSG_NEWVHCL;
                    nvMsg.owner = bct->owner;
                    nvMsg.pos = bct->position;
                    nvMsg.type = bct->vehicleID;
                    nvMsg.parent = bct->parent_bact->gid;
                    nvMsg.id = bct->gid;
                    nvMsg.kind = 2;
                    nvMsg.commandID = bct->commandID;

                    arg181.recvID = 0;
                    arg181.recvFlags = 2;
                    arg181.data = &nvMsg;
                    arg181.dataSize = sizeof(nvMsg);
                    arg181.garant = 1;

                    ywo->ypaworld_func181(&arg181);
                }

                int v67 = getBACT_bactCollisions();
                newbact2->setBACT_bactCollisions(v67);

                bct->host_station = this;
                robo->field_4F5 -= arg->energy;

                sub_4A6720(ywo, bct);
                sub_493DB0(bct, arg->selectBact, ywo);
            }
        }
        break;

    case 6:
        if ( arg->energy <= robo->field_4F5 )
        {
            arg148.ownerID = bact->owner;
            arg148.ownerID2 = bact->owner;
            arg148.blg_ID = arg->protoID;
            arg148.field_C = 0;
            arg148.x = arg->target_sect_x;
            arg148.y = arg->target_sect_y;
            arg148.field_18 = 0;
            if ( ywo->ypaworld_func148(&arg148) )
            {
                robo->field_4F5 -= arg->energy;

                if ( yw->isNetGame )
                {
                    uamessage_startBuild sbMsg;
                    sbMsg.msgID = UAMSG_STARTBUILD;
                    sbMsg.owner = bact->owner;
                    sbMsg.bproto = arg->protoID;
                    sbMsg.sec_x = arg->target_sect_x;
                    sbMsg.sec_y = arg->target_sect_y;

                    arg181.recvID = 0;
                    arg181.recvFlags = 2;
                    arg181.data = &sbMsg;
                    arg181.garant = 1;
                    arg181.dataSize = sizeof(sbMsg);

                    ywo->ypaworld_func181(&arg181);
                }
            }
        }
        break;

    case 10:
        if ( !(robo->roboState & ROBOSTATE_MOVE) )
        {
            startSound(&bact->soundcarrier, 9);

            robo->roboState |= ROBOSTATE_MOVE;
            robo->field_4FD -= arg->energy;
            robo->field_515 = arg->target_point;
            robo->field_511 = 1500;
            if ( yw->isNetGame )
            {
                uamessage_startBeam stbMsg;
                stbMsg.msgID = UAMSG_STARTBEAM;
                stbMsg.owner = bact->owner;
                stbMsg.id = bact->gid;
                stbMsg.pos = robo->field_515;

                arg181.recvID = 0;
                arg181.recvFlags = 2;
                arg181.data = &stbMsg;
                arg181.dataSize = sizeof(stbMsg);
                arg181.garant = 1;

                ywo->ypaworld_func181(&arg181);
            }
        }
        break;

    case 11:
    {
        arg->selectBact->status_flg |= BACT_STFLAG_WAYPOINT;
        arg->selectBact->m_cmdID = 0;

        int v11 = 0;

        if ( arg->selectBact->bact_type != BACT_TYPES_TANK && arg->selectBact->bact_type != BACT_TYPES_CAR )
        {
            bact_node *nod = (bact_node *)arg->selectBact->subjects_list.head;
            while (nod->next)
            {
                if ( nod->bact->bact_type == BACT_TYPES_TANK || nod->bact->bact_type == BACT_TYPES_CAR )
                {
                    v11 = 1;
                    break;
                }

                nod = (bact_node *)nod->next;
            }
        }
        else
            v11 = 1;

        if ( v11 )
        {
            arg124.from = arg->selectBact->position.XZ();
            arg124.to = arg->target_point.XZ();
            arg124.steps_cnt = 32;
            arg124.field_12 = 1;

            int v14 = PathFinder(&arg124);

            if ( v14 && arg124.steps_cnt > 0)
            {
                for (int i = 0; i < arg124.steps_cnt; i++)
                {
                    arg->selectBact->waypoints[i].x = arg124.waypoints[i].x;
                    arg->selectBact->waypoints[i].z = arg124.waypoints[i].z;
                }
                arg->selectBact->waypoints_count = arg124.steps_cnt;
            }
            else
            {
                arg->selectBact->waypoints_count = 1;
                arg->selectBact->waypoints[0].x = arg->target_point.x;
                arg->selectBact->waypoints[0].z = arg->target_point.z;

                if ( yw->GameShell )
                    startSound(&yw->GameShell->samples2_info, 1);
            }
        }
        else
        {
            arg->selectBact->waypoints_count = 1;
            arg->selectBact->waypoints[0].x = arg->target_point.x;
            arg->selectBact->waypoints[0].z = arg->target_point.z;
        }

        arg->selectBact->current_waypoint = 0;

        arg67.tgt_pos.x = arg->selectBact->waypoints[0].x;
        arg67.tgt_pos.z = arg->selectBact->waypoints[0].z;
        arg67.priority = 0;
        arg67.tgt_type = BACT_TGT_TYPE_CELL;
        arg->selectBact->self->SetTarget(&arg67);

        sub_4A448C(arg->selectBact);
        sub_4A4538(arg->selectBact);
        sub_4AB69C(arg->selectBact, 0);
    }
    break;

    case 12:
        if ( arg->selectBact->waypoints_count < 32 && (arg->selectBact->status_flg & BACT_STFLAG_WAYPOINT) )
        {
            int v72 = arg->selectBact->waypoints_count;
            int v20 = 0;

            if ( arg->selectBact->bact_type != BACT_TYPES_TANK && arg->selectBact->bact_type != BACT_TYPES_CAR )
            {
                bact_node *nod = (bact_node *)arg->selectBact->subjects_list.head;
                while (nod->next)
                {
                    if ( nod->bact->bact_type == BACT_TYPES_TANK || nod->bact->bact_type == BACT_TYPES_CAR )
                    {
                        v20 = 1;
                        break;
                    }

                    nod = (bact_node *)nod->next;
                }
            }
            else
                v20 = 1;

            if ( v20 )
            {
                arg124.from = arg->selectBact->waypoints[v72 - 1].XZ();
                arg124.to = arg->target_point.XZ();
                arg124.steps_cnt = 32 - v72;
                arg124.field_12 = 1;

                if ( PathFinder(&arg124) && arg124.steps_cnt > 0 )
                {
                    for (int i = 0; i < arg124.steps_cnt; i++)
                    {
                        arg->selectBact->waypoints[i + v72].x = arg124.waypoints[i].x;
                        arg->selectBact->waypoints[i + v72].z = arg124.waypoints[i].z;
                    }
                    arg->selectBact->waypoints_count += arg124.steps_cnt;

                }
                else
                {
                    arg->selectBact->waypoints[v72].x = arg->target_point.x;
                    arg->selectBact->waypoints[v72].z = arg->target_point.z;
                    arg->selectBact->waypoints_count++;

                    if ( yw->GameShell )
                        startSound(&yw->GameShell->samples2_info, 1);
                }
            }
            else
            {
                arg->selectBact->waypoints[v72].x = arg->target_point.x;
                arg->selectBact->waypoints[v72].z = arg->target_point.z;
                arg->selectBact->waypoints_count++;
            }
            if ( !(arg->selectBact->status_flg & BACT_STFLAG_WAYPOINT) )
            {
                arg->selectBact->status_flg |= BACT_STFLAG_WAYPOINT;

                arg67.tgt_pos.x = arg->selectBact->waypoints[v72].x;
                arg67.tgt_pos.z = arg->selectBact->waypoints[v72].z;
                arg67.priority = 0;
                arg67.tgt_type = BACT_TGT_TYPE_CELL;
                arg->selectBact->self->SetTarget(&arg67);
            }
            sub_4A448C(arg->selectBact);
        }
        break;

    case 13:
        arg->selectBact->status_flg |= BACT_STFLAG_WAYPOINTCCL;
        sub_4A448C(arg->selectBact);
        break;

    default:
        break;
    }
}

int yparobo_func70__sub4__sub0__sub0(__NC_STACK_yparobo *robo, BuildProto *protos)
{
    int id = -1;
    int power = 0;

    __NC_STACK_ypabact *bact = robo->bact_internal;

    for (int i = 0; i < 128; i++)
    {
        BuildProto *proto = &protos[i];

        if ( proto->enable_mask & (1 << bact->owner) )
        {
            if ( proto->model_id == 2 )
            {
                if ( (robo->field_509 + bact->energy - bact->energy_max * 0.2) > proto->energy )
                {
                    int protoPow = proto->power;

                    for (int j = 0; j < 8; j++)
                    {
                        if ( proto->sbacts[j].sbact_vehicle )
                            protoPow += 10;
                    }

                    if ( protoPow > power )
                    {
                        power = protoPow;
                        id = i;
                    }
                }
            }
        }
    }

    return id;
}

void sub_4F4FF4(__NC_STACK_yparobo *robo, int a2, setTarget_msg *parg67)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int v6 = a2 % bact->secMaxX;
    int v5_2 = a2 / bact->secMaxX;

    if ( v6 == bact->sectX && v5_2 == bact->sectY )
    {
        if ( v6 >= 2 )
            v6--;
        else
            v6++;
    }
    else
    {
        if ( v6 > bact->sectX )
            v6--;
        if ( v6 < bact->sectX )
            v6++;

        if ( v5_2 > bact->sectY )
            v5_2--;
        if ( v5_2 < bact->sectY )
            v5_2++;
    }

    parg67->tgt_type = BACT_TGT_TYPE_CELL;
    parg67->priority = 0;
    parg67->tgt_pos.x = (v6 + 0.5) * 1200.0;
    parg67->tgt_pos.z = -(v5_2 + 0.5) * 1200.0;
}

void NC_STACK_yparobo::buildRadar(update_msg *arg)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = &ypabact;

    int xx = robo->field_2F5 % bact->secMaxX;
    int yy = robo->field_2F5 / bact->secMaxX;

    float v35 = arg->frameTime * 0.001;

    if ( robo->pcell[ robo->field_2F5 ].w_type )
    {
        setTarget_msg arg67;
        arg67.tgt_type = BACT_TGT_TYPE_NONE;
        arg67.priority = 0;

        SetTarget(&arg67);
        robo->field_2FD = 0;
    }
    else
    {
        BuildProto *buildprotos = robo->wrld->getYW_buildProtos();

        int build_id = yparobo_func70__sub4__sub0__sub0(robo, buildprotos);

        if ( build_id == -1 )
        {
            setTarget_msg arg67;
            arg67.tgt_type = BACT_TGT_TYPE_NONE;
            arg67.priority = 0;

            SetTarget(&arg67);
            robo->field_2FD = 0;
        }
        else
        {

            if ( abs(bact->sectX - xx) > 1 || abs(bact->sectY - yy) > 1 )
            {
                setTarget_msg arg67;
                sub_4F4FF4(robo, robo->field_2F5, &arg67);

                arg67.tgt_type = BACT_TGT_TYPE_CELL_IND;

                SetTarget(&arg67);
            }
            else
            {

                if ( bact->sectX != xx || bact->sectY != yy )
                {
                    setTarget_msg arg67;
                    arg67.tgt_type = BACT_TGT_TYPE_NONE;
                    arg67.priority = 0;
                    SetTarget(&arg67);

                    vec2d t ( (xx + 0.5) * 1200.0 - bact->position.x,
                              -(yy + 0.5) * 1200.0 - bact->position.z );

                    t.normalise();

                    float v41 = t.dot( bact->rotation.AxisZ().XZ() );

                    if ( v41 <= 0.9 )
                    {
                        float a2 = clp_acos(v41);

                        if ( t.cross( bact->rotation.AxisZ().XZ() ) > 0.0 )
                            a2 = -a2;

                        if ( a2 < -bact->maxrot * v35 )
                            a2 = -bact->maxrot * v35;

                        if ( a2 > bact->maxrot * v35 )
                            a2 = bact->maxrot * v35;

                        sub_4A10E8(robo, a2);
                    }
                    else
                    {
                        ypaworld_arg148 arg148;

                        arg148.ownerID2 = bact->owner;
                        arg148.ownerID = bact->owner;
                        arg148.blg_ID = build_id;
                        arg148.x = xx;
                        arg148.y = yy;
                        arg148.field_C = 0;
                        arg148.field_18 = 0;

                        if ( robo->wrld->ypaworld_func148(&arg148) )
                        {
                            BuildProto *proto = &buildprotos[build_id];

                            if ( robo->field_509 >= proto->energy )
                            {
                                robo->field_509 -= proto->energy;
                            }
                            else
                            {
                                bact->energy -= proto->energy - robo->field_509;
                                robo->field_509 = 0;
                            }

                            robo->field_2FD = 0;

                            if ( robo->newAI )
                                robo->radar_delay = robo->field_1E3 * (100 - robo->epRadar) / 100;
                        }
                        else
                        {
                            setTarget_msg arg67;
                            arg67.tgt_type = BACT_TGT_TYPE_NONE;
                            arg67.priority = 0;

                            SetTarget(&arg67);

                            robo->field_2FD = 0;
                        }
                    }
                }
                else
                {
                    setTarget_msg arg67;

                    sub_4F4FF4(robo, robo->field_2F5, &arg67);

                    arg67.tgt_type = BACT_TGT_TYPE_CELL_IND;

                    SetTarget(&arg67);
                }

            }

        }
    }
}

int yparobo_func70__sub4__sub2__sub0(__NC_STACK_yparobo *robo, BuildProto *protos)
{
    int id = -1;
    int power = 0;

    __NC_STACK_ypabact *bact = robo->bact_internal;

    for (int i = 0; i < 128; i++)
    {
        BuildProto *proto = &protos[i];

        if ( proto->enable_mask & (1 << bact->owner) )
        {
            if ( proto->model_id == 1 )
            {
                if ( (robo->field_509 + bact->energy - bact->energy_max * 0.2) > proto->energy )
                {
                    int protoPow = proto->power;

                    for (int j = 0; j < 8; j++)
                    {
                        if ( proto->sbacts[j].sbact_vehicle )
                            protoPow += 10;
                    }

                    if ( protoPow > power )
                    {
                        power = protoPow;
                        id = i;
                    }
                }
            }
        }
    }

    return id;
}

void NC_STACK_yparobo::buildPower(update_msg *arg)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = &ypabact;

    int xx = robo->field_2F5 % bact->secMaxX;
    int yy = robo->field_2F5 / bact->secMaxX;

    float v38 = arg->frameTime * 0.001;

    if ( robo->pcell[ robo->field_2F5 ].w_type )
    {
        setTarget_msg arg67;
        arg67.tgt_type = BACT_TGT_TYPE_NONE;
        arg67.priority = 0;
        SetTarget(&arg67);

        robo->field_2FD = 0;
    }
    else
    {
        yw_arg176 arg176;
        arg176.owner = bact->owner;
        robo->wrld->ypaworld_func176(&arg176);

        if ( robo->pcell[ robo->field_2F5 ].energy_power != -1 && ( arg176.field_4 >= 0.9 || arg176.field_4 <= 0.001) )
        {
            BuildProto *buildprotos = robo->wrld->getYW_buildProtos();

            int bldid = yparobo_func70__sub4__sub2__sub0(robo, buildprotos);

            if ( bldid == -1 )
            {
                setTarget_msg arg67;
                arg67.tgt_type = BACT_TGT_TYPE_NONE;
                arg67.priority = 0;
                SetTarget(&arg67);
                robo->field_2FD = 0;
            }
            else
            {
                if ( abs(bact->sectX - xx) > 1 || abs(bact->sectY - yy) > 1 )
                {
                    setTarget_msg arg67;
                    sub_4F4FF4(robo, robo->field_2F5, &arg67);
                    arg67.tgt_type = BACT_TGT_TYPE_CELL_IND;
                    SetTarget(&arg67);
                }
                else
                {

                    if ( bact->sectX != xx || bact->sectY != yy )
                    {
                        setTarget_msg arg67;
                        arg67.tgt_type = BACT_TGT_TYPE_NONE;
                        arg67.priority = 0;
                        SetTarget(&arg67);

                        vec2d t ( (xx + 0.5) * 1200.0 - bact->position.x,
                                  -(yy + 0.5) * 1200.0 - bact->position.z );

                        t.normalise();

                        float v44 = t.dot( bact->rotation.AxisZ().XZ() );

                        if ( v44 <= 0.9 )
                        {
                            float a2 = clp_acos(v44);

                            if ( t.cross( bact->rotation.AxisZ().XZ() ) > 0.0 )
                                a2 = -a2;

                            if ( a2 < -bact->maxrot * v38 )
                                a2 = -bact->maxrot * v38;

                            if ( a2 > bact->maxrot * v38 )
                                a2 = bact->maxrot * v38;

                            sub_4A10E8(robo, a2);
                        }
                        else
                        {
                            ypaworld_arg148 arg148;
                            arg148.ownerID2 = bact->owner;
                            arg148.ownerID = bact->owner;
                            arg148.blg_ID = bldid;
                            arg148.x = xx;
                            arg148.y = yy;
                            arg148.field_C = 0;
                            arg148.field_18 = 0;

                            if ( robo->wrld->ypaworld_func148(&arg148) )
                            {
                                BuildProto *proto = &buildprotos[bldid];

                                if ( robo->field_509 >= proto->energy )
                                {
                                    robo->field_509 -= proto->energy;
                                }
                                else
                                {
                                    bact->energy -= proto->energy - robo->field_509;
                                    robo->field_509 = 0;
                                }

                                robo->field_2FD = 0;

                                if ( robo->newAI )
                                    robo->field_265 = robo->field_1E3 * (100 - robo->epPower) / 100;
                            }
                            else
                            {
                                setTarget_msg arg67;
                                arg67.tgt_type = BACT_TGT_TYPE_NONE;
                                arg67.priority = 0;
                                SetTarget(&arg67);
                                robo->field_2FD = 0;
                            }
                        }
                    }
                    else
                    {
                        setTarget_msg arg67;
                        sub_4F4FF4(robo, robo->field_2F5, &arg67);
                        arg67.tgt_type = BACT_TGT_TYPE_CELL_IND;
                        SetTarget(&arg67);
                    }
                }
            }
        }
        else
        {
            setTarget_msg arg67;
            arg67.tgt_type = BACT_TGT_TYPE_NONE;
            arg67.priority = 0;
            SetTarget(&arg67);
            robo->field_2FD = 0;
        }
    }
}

int yparobo_func70__sub4__sub1__sub0(__NC_STACK_yparobo *robo, BuildProto *protos)
{
    int id = -1;
    int power = 0;

    __NC_STACK_ypabact *bact = robo->bact_internal;

    for (int i = 0; i < 128; i++)
    {
        BuildProto *proto = &protos[i];

        if ( proto->enable_mask & (1 << bact->owner) )
        {
            if ( proto->model_id == 3 )
            {
                if ( (robo->field_509 + bact->energy - bact->energy_max * 0.2) > proto->energy )
                {
                    int protoPow = proto->power;

                    for (int j = 0; j < 8; j++)
                    {
                        if ( proto->sbacts[j].sbact_vehicle )
                            protoPow += 10;
                    }

                    if ( protoPow > power )
                    {
                        power = protoPow;
                        id = i;
                    }
                }
            }
        }
    }

    return id;
}


void NC_STACK_yparobo::buildSafe(update_msg *arg)
{
    // Copy of sub4_sub0 with minor changes
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = &ypabact;

    int xx = robo->field_2F5 % bact->secMaxX;
    int yy = robo->field_2F5 / bact->secMaxX;

    float v35 = arg->frameTime * 0.001;

    if ( robo->pcell[ robo->field_2F5 ].w_type )
    {
        setTarget_msg arg67;
        arg67.tgt_type = BACT_TGT_TYPE_NONE;
        arg67.priority = 0;

        bact->self->SetTarget(&arg67);
        robo->field_2FD = 0;
    }
    else
    {
        BuildProto *buildprotos = robo->wrld->getYW_buildProtos();

        int build_id = yparobo_func70__sub4__sub1__sub0(robo, buildprotos);

        if ( build_id == -1 )
        {
            setTarget_msg arg67;
            arg67.tgt_type = BACT_TGT_TYPE_NONE;
            arg67.priority = 0;

            SetTarget(&arg67);
            robo->field_2FD = 0;
        }
        else
        {

            if ( abs(bact->sectX - xx) > 1 || abs(bact->sectY - yy) > 1 )
            {
                setTarget_msg arg67;
                sub_4F4FF4(robo, robo->field_2F5, &arg67);

                arg67.tgt_type = BACT_TGT_TYPE_CELL_IND;

                SetTarget(&arg67);
            }
            else
            {

                if ( bact->sectX != xx || bact->sectY != yy )
                {
                    setTarget_msg arg67;
                    arg67.tgt_type = BACT_TGT_TYPE_NONE;
                    arg67.priority = 0;
                    SetTarget(&arg67);

                    vec2d t ( (xx + 0.5) * 1200.0 - bact->position.x,
                              -(yy + 0.5) * 1200.0 - bact->position.z );

                    t.normalise();

                    float v41 = t.dot( bact->rotation.AxisZ().XZ() );

                    if ( v41 <= 0.9 )
                    {
                        float a2 = clp_acos(v41);

                        if ( t.cross( bact->rotation.AxisZ().XZ() ) > 0.0 )
                            a2 = -a2;

                        if ( a2 < -bact->maxrot * v35 )
                            a2 = -bact->maxrot * v35;

                        if ( a2 > bact->maxrot * v35 )
                            a2 = bact->maxrot * v35;

                        sub_4A10E8(robo, a2);
                    }
                    else
                    {
                        ypaworld_arg148 arg148;

                        arg148.ownerID2 = bact->owner;
                        arg148.ownerID = bact->owner;
                        arg148.blg_ID = build_id;
                        arg148.x = xx;
                        arg148.y = yy;
                        arg148.field_C = 0;
                        arg148.field_18 = 0;

                        if ( robo->wrld->ypaworld_func148(&arg148) )
                        {
                            BuildProto *proto = &buildprotos[build_id];

                            if ( robo->field_509 >= proto->energy )
                            {
                                robo->field_509 -= proto->energy;
                            }
                            else
                            {
                                bact->energy -= proto->energy - robo->field_509;
                                robo->field_509 = 0;
                            }

                            robo->field_2FD = 0;

                            if ( robo->newAI )
                                robo->safety_delay = robo->field_1E3 * (100 - robo->epSafety) / 100;
                        }
                        else
                        {
                            setTarget_msg arg67;
                            arg67.tgt_type = BACT_TGT_TYPE_NONE;
                            arg67.priority = 0;

                            SetTarget(&arg67);

                            robo->field_2FD = 0;
                        }
                    }
                }
                else
                {
                    setTarget_msg arg67;

                    sub_4F4FF4(robo, robo->field_2F5, &arg67);

                    arg67.tgt_type = BACT_TGT_TYPE_CELL_IND;

                    SetTarget(&arg67);
                }

            }

        }
    }
}

void NC_STACK_yparobo::changePlace()
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = &ypabact;

    int xx = robo->field_2F5 % bact->secMaxX;
    int yy = robo->field_2F5 / bact->secMaxX;

    if ( xx >= 1 && xx <= (bact->secMaxX - 2) && yy >= 1 && yy <= (bact->secMaxY - 2) )
    {
        int tmp1 = bact->sectY - yy;
        int tmp2 = bact->sectX - xx;

        float v12 = sqrt(tmp1 * tmp1 + tmp2 * tmp2);

        if ( v12 >= 0.1 )
        {
            setTarget_msg arg67;
            arg67.tgt_type = BACT_TGT_TYPE_CELL_IND;
            arg67.priority = 0;
            arg67.tgt_pos.x = (xx + 0.5) * 1200.0;
            arg67.tgt_pos.z = -(yy + 0.5) * 1200.0;

            SetTarget(&arg67);
        }
        else
        {
            robo->safety_cellID = 0;
            robo->safety_value = 0;
            robo->safety_cellIDpos = 0;
            robo->field_2A5 = 0;
            robo->radar_cellID = 0;
            robo->field_259 = 0;
            robo->field_29D = 0;
            robo->radar_value = 0;
            robo->field_251 = 0;
            robo->field_2A9 = 0;
            robo->radar_cellIDpos = 0;
            robo->field_25D = 0;

            robo->field_2FD = 0;

            robo->roboState &= ~(ROBOSTATE_READYRADAR | ROBOSTATE_READYPOWER
                                 | ROBOSTATE_READYSAFE | ROBOSTATE_FOUNDPLACE);

            setTarget_msg arg67;
            arg67.tgt_type = BACT_TGT_TYPE_NONE;
            arg67.priority = 0;
            SetTarget(&arg67);

            if ( robo->newAI )
                robo->field_2B1 = robo->field_1E3 * (100 - robo->epChangePlace) / 100;
        }
    }
    else
    {
        robo->field_2FD = 0;
    }
}


float TestVehicle(NC_STACK_ypaworld *yw, void *src, int bact_ref, int job)
{
    VhclProto *vhcl_protos = yw->getYW_vhclProtos();
    WeapProto *weapon_protos = yw->getYW_weaponProtos();

    VhclProto *proto;

    if ( bact_ref )
        proto = &vhcl_protos[ ((__NC_STACK_ypabact *)src)->vehicleID ];
    else
        proto = (VhclProto *)src;


    WeapProto *wpn;

    if ( proto->weapon == -1 )
        wpn = NULL;
    else
        wpn = &weapon_protos[ proto->weapon ];

    switch ( job )
    {
    case 1:
        if ( (proto->mgun == -1 && !wpn) || proto->model_id == 7 )
            return -1.0;

        return proto->job_fightrobo;
        break;

    case 2:
        if ( (proto->mgun == -1 && !wpn) || proto->model_id == 7 )
            return -1.0;

        return proto->job_fighttank;
        break;

    case 4:
        if ( (proto->mgun == -1 && !wpn) || proto->model_id == 7 )
            return -1.0;

        return proto->job_fighthelicopter;
        break;

    case 3:
        if ( (proto->mgun == -1 && !wpn) || proto->model_id == 7 )
            return -1.0;

        return proto->job_fightflyer;
        break;

    case 5:
        return proto->job_reconnoitre;
        break;

    case 6:
        if ( !wpn || proto->model_id == 7 )
            return -1.0;

        return proto->job_conquer;
        break;

    default:
        break;
    }
    return -1.0;
}

bact_node *NC_STACK_yparobo::allocForce(robo_loct1 *arg)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = &ypabact;

    int v72 = 0;
    float v73 = 0.0;

    int v85 = -1;
    int v86 = -1;
    int v87 = -1;

    int v75 = 10000000;
    int v2 = 0;
    int v77 = 0;
    float v82 = -2.0;
    float v76 = 153600.0;
    bact_node *v74 = NULL;

    bact_node *node = (bact_node *)bact->subjects_list.head;

    while ( node->next )
    {
        //Unused flag
        if ( node->bact->status == BACT_STATUS_IDLE && /*!( node->bact->status_flg & BACT_STFLAG_AKKU ) &&*/ !( node->bact->status_flg & BACT_STFLAG_UNUSE ) && !( node->bact->status_flg & BACT_STFLAG_ESCAPE ) &&  node->bact->bact_type != BACT_TYPES_GUN )
        {
            v2++;

            float v8 = TestVehicle(robo->wrld, node->bact, 1, arg->job);

            if ( abs(node->bact->sectX - bact->sectX) < 2 && abs(node->bact->sectY - bact->sectY) < 2 )
                v77 = 1;

            if ( v8 > -1.0 && v8 >= v82 )
            {

                bact_arg81 arg81;
                arg81.enrg_type = 1;
                arg81.enrg_sum = 0;
                node->bacto->GetSummary(&arg81);

                int v11 = abs(arg81.enrg_sum - arg->energ);
                int v15 = 0;

                int v16;

                if ( v11 )
                    v16 = arg->energ / v11;
                else
                    v16 = 6;

                float v78 = (node->bact->position.XZ() - arg->tgt_pos.XZ()).length();

                if ( v16 <= 5 )
                {
                    if ( v11 < v75 )
                    {
                        v15 = 1;
                    }
                }
                else if ( v78 < v76 )
                {
                    v15 = 1;
                }

                if ( v8 > v82 || (v8 == v82 && v15) )
                {
                    v75 = v11;
                    v76 = v78;
                    v74 = node;
                    v82 = v8;
                }

            }
        }
        node = (bact_node *)node->next;
    }

    if ( !(robo->roboState & ROBOSTATE_DOCKINUSE) )
    {
        bact_arg81 arg81_2;
        arg81_2.enrg_sum = 0;
        arg81_2.enrg_type = 3;

        GetSummary(&arg81_2);

        v85 = -1;
        v86 = -1;
        v87 = -1;

        v73 = arg81_2.enrg_sum * 0.05 + 1.0;

        VhclProto *vhcl_protos = robo->wrld->getYW_vhclProtos();

        for (int i = 0; i < 256; i++)
        {
            if ( vhcl_protos[i].model_id )
            {
                float v20 = bact->energy_max * 0.2;
                int v67 = bact->energy - v20;

                if ( robo->roboState & ROBOSTATE_BUILDROBO )
                    v67 += robo->field_50D;

                VhclProto *vhclprt = &vhcl_protos[i];
                if ( (1 << bact->owner) & vhclprt->disable_enable_bitmask )
                {
                    if ( vhclprt->model_id != 3 && vhclprt->model_id != 9 && vhclprt->model_id != 4 )
                    {
                        if ( v67 > ((float)vhclprt->energy * v73) )
                        {
                            float v80 = TestVehicle(robo->wrld, vhclprt, 0, arg->job);

                            int v27 = 0;
                            if ( v2 < (bact->secMaxX + bact->secMaxY) / 5 )
                                v27 = 1;

                            if ( bact->energy < ((float)bact->energy_max * 0.9) )
                                v27 = 0;

                            if ( robo->roboState & ROBOSTATE_DOCKINUSE )
                                v27 = 0;

                            if ( v77 )
                                v27 = 0;

                            if ( v80 > -1.0 && (v80 > v82 || (v80 == v82 && v27)) )
                            {
                                v82 = v80;
                                v87 = v86;
                                v86 = v85;
                                v85 = i;
                            }
                        }
                    }
                }
            }
        }
    }

    //there must be switch, but it's originally deleted
    //arg->field_34;

    if ( v85 == -1 && !v74 )
        return NULL;

    if ( v85 == -1 )
    {
        if ( !(robo->roboState & ROBOSTATE_DOCKINUSE) )
        {
            bact_arg81 arg81_3;
            arg81_3.enrg_type = 1;
            arg81_3.enrg_sum = 0;

            v74->bacto->GetSummary(&arg81_3);

            if ( arg81_3.enrg_sum < arg->energ )
            {
                robo->dock_user = v74->bact->commandID;
                robo->dock_energ = arg->energ - arg81_3.enrg_sum;
                v72 = 1;
            }
        }
    }
    else
    {
        int v32 = bact->clock % 10;
        int v33;

        if ( v32 <= 8 || v87 == -1 )
        {
            if ( v32 <= 5 || v86 == -1 )
                v33 = v85;
            else
                v33 = v86;
        }
        else
        {
            v33 = v87;
        }

        ypaworld_arg146 arg146;
        arg146.vehicle_id = v33;
        arg146.pos = bact->position + robo->dock_pos;

        NC_STACK_ypabact *new_unit = robo->wrld->ypaworld_func146(&arg146);

        if ( !new_unit )
            return NULL;

        __NC_STACK_ypabact *unt_bct = new_unit->getBACT_pBact();

        unt_bct->commandID = dword_5B1128;
        dword_5B1128++;

        unt_bct->owner = bact->owner;
        unt_bct->host_station = this;

        int v69 = getBACT_bactCollisions();
        new_unit->setBACT_bactCollisions(v69);

        AddSubject(new_unit);
        if ( robo->wrld_yw->isNetGame )
        {
            unt_bct->gid |= unt_bct->owner << 24;
            unt_bct->commandID |= unt_bct->owner << 24;

            uamessage_newVhcl nvMsg;

            nvMsg.msgID = UAMSG_NEWVHCL;
            nvMsg.owner = unt_bct->owner;
            nvMsg.pos = unt_bct->position;
            nvMsg.type = unt_bct->vehicleID;
            nvMsg.parent = unt_bct->parent_bact->gid;
            nvMsg.id = unt_bct->gid;
            nvMsg.kind = 1;
            nvMsg.commandID = unt_bct->commandID;

            yw_arg181 arg181;
            arg181.recvID = 0;
            arg181.recvFlags = 2;
            arg181.data = &nvMsg;
            arg181.dataSize = sizeof(nvMsg);
            arg181.garant = 1;

            robo->wrld->ypaworld_func181(&arg181);
        }

        robo->roboState |= ROBOSTATE_DOCKINUSE;

        new_unit->setBACT_aggression(60);

        robo->dock_user = unt_bct->commandID;
        robo->dock_energ = arg->energ - unt_bct->energy;

        unt_bct->scale_time = (float)unt_bct->energy_max * 0.2;

        robo->dock_time = unt_bct->scale_time + 2000;

        unt_bct->status_flg &= ~BACT_STFLAG_ESCAPE;

        bact_node *nnode = (bact_node *)unt_bct->subjects_list.head;
        while ( nnode->next )
        {
            nnode->bact->status_flg &= ~BACT_STFLAG_ESCAPE;

            nnode = (bact_node *)nnode->next;
        }

        setState_msg arg78;
        arg78.newStatus = BACT_STATUS_CREATE;
        arg78.setFlags = 0;
        arg78.unsetFlags = 0;

        new_unit->SetState(&arg78);


        setTarget_msg arg67;
        arg67.tgt_type = BACT_TGT_TYPE_CELL;
        arg67.priority = 0;
        arg67.tgt_pos.x = bact->rotation.m20 * 1200.0 * 0.5 + bact->position.x;
        arg67.tgt_pos.z = bact->rotation.m22 * 1200.0 * 0.5 + bact->position.z;

        new_unit->SetTarget(&arg67);

        int v70 = (float)unt_bct->energy_max * v73;

        if ( robo->roboState & ROBOSTATE_BUILDROBO )
        {
            if ( v70 > robo->field_50D )
            {
                bact->energy -= v70 - robo->field_50D;
                robo->field_50D = 0;
            }
            else
            {
                robo->field_50D -= v70;
            }
        }
        else
        {
            bact->energy -= v70;
        }

        v74 = &unt_bct->subject_node;
        v72 = 1;
        sub_4A6720(robo->wrld, unt_bct);
    }

    if ( v72 )
    {
        robo->roboState |= (ROBOSTATE_SETDOCKTARGET | ROBOSTATE_DOCKINUSE);
        robo->dock_tgt_bact = arg->tgt_bact;
        robo->dock_tgType = arg->tgType;
        robo->dock_tgt_comm_id = arg->command_id;

        robo->dock_tgt_pos = arg->tgt_pos;
        robo->dock_aggr = arg->aggr;

        if ( arg->tgType == BACT_TGT_TYPE_UNIT )
            robo->dock_tgt_pos = arg->tgt_bact->position;
    }
    else
    {
        setTarget_msg arg67_2;
        arg67_2.tgt_pos = arg->tgt_pos;
        arg67_2.priority = 0;
        arg67_2.tgt.pbact = arg->tgt_bact;
        arg67_2.tgt_type = arg->tgType;

        v74->bacto->SetTarget(&arg67_2);

        v74->bacto->setBACT_aggression(arg->aggr);
    }

    return v74;
}



void NC_STACK_yparobo::buildConquer()
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = &ypabact;

    bact_arg92 arg92;
    arg92.field_14 = 2;
    arg92.pos.x = ((robo->vehicle_cellID % bact->secMaxX) + 0.5) * 1200.0;
    arg92.pos.z = -( (robo->vehicle_cellID / bact->secMaxX ) + 0.5) * 1200.0;

    GetForcesRatio(&arg92);

    int v20 = (arg92.energ2 - arg92.energ1) * 0.5;

    if ( v20 < 15000 )
        v20 = 15000;

    yw_130arg arg130;
    arg130.pos_x = arg92.pos.x;
    arg130.pos_z = arg92.pos.z;

    robo->wrld->ypaworld_func130(&arg130);

    int enrg = 0;

    __NC_STACK_ypabact *cell_unit = (__NC_STACK_ypabact *)arg130.pcell->units_list.head;

    while (cell_unit->next)
    {
        if ( bact->owner != cell_unit->owner && cell_unit->bact_type == BACT_TYPES_GUN && (cell_unit->weapon != -1 || cell_unit->mgun != -1) )
        {
            NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( cell_unit->self );
            int a4 = gun->getGUN_roboGun();

            if ( !a4 )
            {
                bact_node *bctt = (bact_node *)robo->wrld_yw->bact_list.head;

                while (bctt->next)
                {
                    if ( bctt->bact->bact_type == BACT_TYPES_ROBO && bctt->bact->status != BACT_STATUS_DEAD && cell_unit->owner == bctt->bact->owner )
                    {
                        int v6 = bctt->bact->energy_max / 2;

                        if ( v6 < 120000 )
                            v6 = 120000;

                        enrg = v6;
                        break;
                    }

                    bctt = (bact_node *)bctt->next;
                }
            }
        }

        if ( enrg )
            break;

        cell_unit = (__NC_STACK_ypabact *)cell_unit->next;
    }

    if ( enrg > v20 )
        v20 = enrg;

    ypaworld_arg136 arg136;
    arg136.stPos = bact->position + robo->dock_pos - vec3d::OY(100.0);
    arg136.vect = vec3d::OY(20000.0);
    arg136.flags = 0;

    robo->wrld->ypaworld_func136(&arg136);

    if ( bact->pSector->height - arg136.isectPos.y >= 50.0 )
    {
        setTarget_msg arg67;
        arg67.tgt_pos.x = bact->position.x + 200.0;
        arg67.tgt_pos.z = bact->position.z + 300.0;
        arg67.priority = 0;
        arg67.tgt_type = BACT_TGT_TYPE_CELL_IND;

        SetTarget(&arg67);
    }
    else
    {
        robo_loct1 loct;

        loct.distance = (arg92.pos.XZ() - bact->position.XZ()).length();
        loct.tgt_pos = arg92.pos.X0Z();

        loct.tgt_bact = NULL;

        if ( v20 <= 20000 )
            loct.energ = 20000;
        else
            loct.energ = v20;

        loct.tgType = BACT_TGT_TYPE_CELL;

        if ( robo->vehicle_cell->w_type == 2 )
            loct.aggr = 49;
        else
            loct.aggr = 60;

        loct.forbidden = 1;
        loct.want = 32;
        loct.good = 130;
        loct.bad = 0;
        loct.job = 6;

        allocForce(&loct);

        if ( robo->newAI )
            robo->conq_delay = robo->field_1E3 * (100 - robo->epConquer) / 100;

        robo->vehicle_type = 0;
    }
}

void NC_STACK_yparobo::buildDefense()
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = &ypabact;

    setTarget_msg arg132;
    arg132.priority = robo->field_2F1;

    if ( yparobo_func132(&arg132) )
    {
        bact_arg81 arg81;
        arg81.enrg_sum = 0;
        arg81.enrg_type = 1;

        arg132.tgt.pbact->self->GetSummary(&arg81);

        int v18 = arg81.enrg_sum / 2;

        if ( v18 < 15000 )
            v18 = 15000;

        if ( arg132.tgt.pbact->bact_type == BACT_TYPES_GUN && (arg132.tgt.pbact->weapon != -1 || arg132.tgt.pbact->mgun != -1) )
        {
            NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( arg132.tgt.pbact->self );
            int a4 = gun->getGUN_roboGun();

            if ( !a4 )
            {
                bact_node *node = (bact_node *)robo->wrld_yw->bact_list.head;
                while( node->next )
                {
                    if ( node->bact->bact_type == BACT_TYPES_ROBO && node->bact->status != BACT_STATUS_DEAD && arg132.tgt.pbact->owner == node->bact->owner )
                    {
                        int v4 = node->bact->energy_max / 2;

                        if ( v4 < 120000 )
                            v4 = 120000;

                        v18 = v4;
                        break;
                    }

                    node = (bact_node *)node->next;
                }
            }
        }

        ypaworld_arg136 arg136;
        arg136.stPos = bact->position + robo->dock_pos - vec3d::OY(100.0);
        arg136.vect = vec3d::OY(20000.0);
        arg136.flags = 0;

        robo->wrld->ypaworld_func136(&arg136);

        if ( bact->pSector->height - arg136.isectPos.y >= 50.0 )
        {
            arg132.tgt_pos.x = bact->position.x + 200.0;
            arg132.tgt_pos.z = bact->position.z + 300.0;
            arg132.priority = 0;
            arg132.tgt_type = BACT_TGT_TYPE_CELL_IND;

            SetTarget(&arg132);
        }
        else
        {
            robo_loct1 loct;
            loct.distance = (arg132.tgt.pbact->position.XZ() - bact->position.XZ()).length();
            loct.tgt_pos = arg132.tgt.pbact->position;

            int v9 = v18;
            if ( v9 >= 40000 )
                v9 = 40000;

            loct.energ = v9;
            loct.tgType = BACT_TGT_TYPE_UNIT;
            loct.tgt_bact = arg132.tgt.pbact;
            loct.command_id = robo->field_2F1;
            loct.aggr = 60;

            if ( arg132.tgt.pbact->bact_type != BACT_TYPES_TANK && arg132.tgt.pbact->bact_type != BACT_TYPES_CAR && arg132.tgt.pbact->bact_type != BACT_TYPES_HOVER )
            {
                loct.bad = 2;
                loct.want = 32;
                loct.good = 852;
                loct.forbidden = 129;
            }
            else
            {
                loct.want = 32;
                loct.good = 578;
                loct.bad = 256;
                loct.forbidden = 1;
            }

            switch ( arg132.tgt.pbact->bact_type )
            {
            case BACT_TYPES_TANK:
            case BACT_TYPES_CAR:
            case BACT_TYPES_HOVER:
                loct.job = 2;
                break;

            case BACT_TYPES_FLYER:
            case BACT_TYPES_UFO:
                loct.job = 3;
                break;

            default:
                loct.job = 4;
                break;
            }

            allocForce(&loct);

            if ( robo->newAI )
                robo->field_281 = robo->field_1E3 * (100 - robo->epDefense) / 100;

            robo->vehicle_type = 0;
        }
    }
    else
    {
        robo->vehicle_type = 0;
    }
}

void NC_STACK_yparobo::buildRobo()
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = &ypabact;

    setTarget_msg arg132;
    arg132.priority = robo->field_2F1;
    if ( yparobo_func132(&arg132) )
    {
        ypaworld_arg136 arg136;
        arg136.stPos = bact->position + robo->dock_pos - vec3d::OY(100.0);
        arg136.vect = vec3d::OY(20000.0);
        arg136.flags = 0;

        robo->wrld->ypaworld_func136(&arg136);

        if ( bact->pSector->height - arg136.isectPos.y >= 50.0 )
        {
            arg132.tgt_pos.x = bact->position.x + 200.0;
            arg132.tgt_pos.z = bact->position.z + 300.0;
            arg132.tgt_type = BACT_TGT_TYPE_CELL_IND;
            arg132.priority = 0;
            SetTarget(&arg132);
        }
        else
        {
            robo_loct1 loct;
            loct.distance = (arg132.tgt.pbact->position.XZ() - bact->position.XZ()).length();
            loct.tgt_pos = arg132.tgt.pbact->position;

            int v6 = arg132.tgt.pbact->energy_max / 2;
            if ( v6 < 120000 )
                v6 = 120000;

            loct.energ = v6;
            loct.tgType = BACT_TGT_TYPE_UNIT;
            loct.tgt_bact = arg132.tgt.pbact;
            loct.command_id = robo->field_2F1;
            loct.aggr = 60;
            loct.want = 32;
            loct.good = 852;
            loct.bad = 2;
            loct.forbidden = 1;
            loct.job = 1;
            robo->roboState |= ROBOSTATE_BUILDROBO;

            bact_node *nod = allocForce(&loct);
            if ( nod )
            {
                for (int i = 0; i < 16; i++)
                {
                    if ( !robo->field_525[i].field_0 )
                    {
                        robo->field_525[i].field_0 = nod->bact->commandID;
                        robo->field_525[i].field_4 = arg132.tgt.pbact->commandID;
                        break;
                    }
                }
            }

            if ( robo->newAI )
                robo->field_2E1 = robo->field_1E3 * (100 - robo->epRobo) / 100;

            robo->vehicle_type = 0;
        }
    }
    else
    {
        robo->vehicle_type = 0;
    }
}

void NC_STACK_yparobo::buildReconnoitre()
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = &ypabact;

    ypaworld_arg136 arg136;
    arg136.stPos = bact->position + robo->dock_pos - vec3d::OY(100.0);
    arg136.vect = vec3d::OY(20000.0);
    arg136.flags = 0;

    robo->wrld->ypaworld_func136(&arg136);

    if ( bact->pSector->height - arg136.isectPos.y >= 50.0 )
    {
        setTarget_msg arg67;
        arg67.tgt_pos.x = bact->position.x + 200.0;
        arg67.tgt_pos.z = bact->position.z + 300.0;
        arg67.tgt_type = BACT_TGT_TYPE_CELL_IND;
        arg67.priority = 0;

        SetTarget(&arg67);
    }
    else
    {
        robo_loct1 loct;
        loct.tgt_pos.x = ((robo->vehicle_cellID % bact->secMaxX) + 0.5) * 1200.0;
        loct.tgt_pos.y = 0;
        loct.tgt_pos.z = -((robo->vehicle_cellID / bact->secMaxX) + 0.5) * 1200.0;

        loct.energ = 10;
        loct.aggr = 25;
        loct.forbidden = 0;
        loct.want = 1024;
        loct.good = 9;
        loct.job = 5;
        loct.tgType = BACT_TGT_TYPE_CELL;
        loct.bad = 32;
        loct.distance = (loct.tgt_pos.XZ() - bact->position.XZ()).length();
        loct.tgt_bact = NULL;

        bact_node *nod = allocForce(&loct);

        if ( nod )
            nod->bact->aggr = 25;

        if ( robo->newAI )
            robo->field_2C9 = robo->field_1E3 * (100 - robo->epRecon) / 100;

        robo->vehicle_type = 0;
    }
}


void NC_STACK_yparobo::AI_doWish(update_msg *arg)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;

    if ( !robo->field_2FD )
    {
        int found_time = 0;
        int found = 0;

        if ( robo->roboState & ROBOSTATE_READYPOWER )
        {
            if ( robo->field_251 )
            {
                if ( -robo->field_265 > found_time )
                {
                    found = ROBOSTATE_BUILDPOWER;
                    found_time = -robo->field_265;
                }
            }
        }
        if ( robo->roboState & ROBOSTATE_READYRADAR )
        {
            if ( robo->radar_value )
            {
                if ( -robo->radar_delay > found_time )
                {
                    found = ROBOSTATE_BUILDRADAR;
                    found_time = -robo->radar_delay;
                }
            }
        }
        if ( robo->roboState & ROBOSTATE_READYSAFE )
        {
            if ( robo->safety_value )
            {
                if ( -robo->safety_delay > found_time )
                {
                    found = ROBOSTATE_BUILDSAFE;
                    found_time = -robo->safety_delay;
                }
            }
        }
        if ( robo->roboState & ROBOSTATE_FOUNDPLACE )
        {
            if ( robo->field_29D )
            {
                if ( -robo->field_2B1 > found_time )
                {
                    found = ROBOSTATE_CHANGEPLACE;
                    found_time = -robo->field_2B1;
                }
            }
        }

        if ( found_time > 0 )
        {
            if ( found == ROBOSTATE_BUILDRADAR )
            {
                robo->radar_value = 0;
                robo->radar_cellIDpos = 0;
                robo->field_2FD = ROBOSTATE_BUILDRADAR;
                robo->field_2F5 = robo->radar_cellID;
                robo->field_2F9 = robo->radar_cell;
                robo->roboState &= ~ROBOSTATE_READYRADAR;
            }
            else if ( found == ROBOSTATE_BUILDPOWER )
            {
                robo->field_251 = 0;
                robo->field_25D = 0;
                robo->field_2FD = ROBOSTATE_BUILDPOWER;
                robo->field_2F5 = robo->field_259;
                robo->roboState &= ~ROBOSTATE_READYPOWER;
                robo->field_2F9 = robo->field_255;
            }
            else if ( found == ROBOSTATE_BUILDSAFE )
            {
                robo->safety_value = 0;
                robo->safety_cellIDpos = 0;
                robo->field_2F5 = robo->safety_cellID;
                robo->field_2F9 = robo->safety_cell;
                robo->field_2FD = ROBOSTATE_BUILDSAFE;
                robo->roboState &= ~ROBOSTATE_READYSAFE;
            }
            else if ( found == ROBOSTATE_CHANGEPLACE )
            {
                robo->field_29D = 0;
                robo->field_2A9 = 0;
                robo->field_2F5 = robo->field_2A5;
                robo->field_2F9 = robo->field_2A1;
                robo->field_2FD = ROBOSTATE_CHANGEPLACE;
                robo->roboState &= ~ROBOSTATE_FOUNDPLACE;
            }
        }
    }

    if ( !robo->vehicle_type && !(robo->roboState & ROBOSTATE_DOCKINUSE) )
    {
        int found_time = 0;
        int found = 0;

        if ( robo->roboState & ROBOSTATE_READYDEFENSE )
        {
            if ( robo->field_269 )
            {
                if ( -robo->field_281 > found_time )
                {
                    found = ROBOSTATE_BUILDDEFENSE;
                    found_time = -robo->field_281;
                }
            }
        }

        if ( robo->roboState & ROBOSTATE_READYCONQUER )
        {
            if ( robo->conq_value )
            {
                if ( -robo->conq_delay > found_time )
                {
                    found = ROBOSTATE_BUILDCONQUER;
                    found_time = -robo->conq_delay;
                }
            }
        }

        if ( robo->roboState & ROBOSTATE_READYRECON )
        {
            if ( robo->field_2B5 )
            {
                if ( -robo->field_2C9 > found_time )
                {
                    found = ROBOSTATE_BUILDRECON;
                    found_time = -robo->field_2C9;
                }
            }
        }
        if ( robo->roboState & ROBOSTATE_READYROBO )
        {
            if ( robo->field_2CD )
            {
                if ( -robo->field_2E1 > found_time )
                {
                    found = ROBOSTATE_BUILDROBO;
                    found_time = -robo->field_2E1;
                }
            }
        }

        if ( found_time > 0 )
        {
            if ( found == ROBOSTATE_BUILDCONQUER )
            {
                robo->conq_value = 0;
                robo->conq_cellIDpos = 0;
                robo->vehicle_cellID = robo->conq_cellID;
                robo->vehicle_cell = robo->conq_cell;
                robo->vehicle_type = ROBOSTATE_BUILDCONQUER;
                robo->roboState &= ~ROBOSTATE_READYCONQUER;
            }
            else if ( found == ROBOSTATE_BUILDDEFENSE )
            {
                robo->vehicle_type = ROBOSTATE_BUILDDEFENSE;
                robo->field_269 = 0;
                robo->field_2F1 = robo->field_275;
                robo->roboState &= ~ROBOSTATE_READYDEFENSE;
                robo->vehicle_cellID = robo->field_271;
                robo->vehicle_cell = robo->field_26D;
            }
            else if ( found == ROBOSTATE_BUILDRECON )
            {
                robo->vehicle_type = ROBOSTATE_BUILDRECON;
                robo->field_2B5 = 0;
                robo->field_2C1 = 0;
                robo->vehicle_cellID = robo->field_2BD;
                robo->vehicle_cell = robo->field_2B9;
                robo->roboState &= ~ROBOSTATE_READYRECON;
            }
            else if ( found == ROBOSTATE_BUILDROBO )
            {
                robo->field_2CD = 0;
                robo->field_2F1 = robo->field_2D9;
                robo->vehicle_cellID = robo->field_2D5;
                robo->vehicle_cell = robo->field_2D1;
                robo->vehicle_type = ROBOSTATE_BUILDROBO;
                robo->roboState &= ~ROBOSTATE_READYROBO;
            }
        }
    }

    switch (robo->field_2FD)
    {
    case ROBOSTATE_BUILDRADAR:
        buildRadar(arg);
        break;

    case ROBOSTATE_BUILDPOWER:
        buildPower(arg);
        break;

    case ROBOSTATE_BUILDSAFE:
        buildSafe(arg);
        break;

    case ROBOSTATE_CHANGEPLACE:
        changePlace();
        break;

    default:
        break;
    }

    switch (robo->vehicle_type)
    {
    case ROBOSTATE_BUILDCONQUER:
        buildConquer();
        break;

    case ROBOSTATE_BUILDDEFENSE:
        buildDefense();
        break;

    case ROBOSTATE_BUILDRECON:
        buildReconnoitre();
        break;

    case ROBOSTATE_BUILDROBO:
        buildRobo();
        break;

    default:
        break;
    }
}

void NC_STACK_yparobo::AI_service(update_msg *arg)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;

    if ( robo->dock_user )
    {
        int v4 = 0;
        bact_node *node = (bact_node *)robo->bact_internal->subjects_list.head;

        while( node->next )
        {
            if (robo->dock_user == node->bact->commandID)
            {
                v4 = 1;
                break;
            }

            node = (bact_node *)node->next;
        }

        if ( !v4 )
        {
            robo->dock_time = 0;
            robo->dock_user = 0;
            robo->roboState &= ~ROBOSTATE_DOCKINUSE;
        }
    }

    if ( robo->dock_time > 0 )
    {
        robo->dock_time -= arg->frameTime;
        if ( robo->dock_time <= 0 )
        {
            robo->dock_time = 0;

            if ( !robo->dock_user )
                robo->roboState &= ~ROBOSTATE_DOCKINUSE;
        }
    }

    if ( robo->dock_time <= 0 && !robo->dock_user )
        robo->roboState &= ~ROBOSTATE_DOCKINUSE;
}

void NC_STACK_yparobo::AI_solveTrouble(update_msg *arg)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = &ypabact;

    if ( (float)bact->energy / (float)bact->energy_max < 0.2 && (arg->gTime - robo->field_309) > 20000 )
    {
        robo->field_309 = arg->gTime;

        int v3 = (float)bact->energy / (float)bact->energy_max < 0.1;

        bact_node *unit = (bact_node *)bact->subjects_list.head;

        while ( unit->next )
        {
            if ( unit->bact->status == BACT_STATUS_NORMAL || unit->bact->status == BACT_STATUS_IDLE )
            {
                if ( unit->bact->bact_type != BACT_TYPES_GUN )
                {
                    int v7;

                    if ( v3 )
                    {
                        v7 = 1;
                        unit->bact->status_flg &= ~BACT_STFLAG_UNUSE;
                    }
                    else
                    {
                        v7 = !(unit->bact->status_flg & BACT_STFLAG_UNUSE) && !unit->bact->secndTtype;
                    }

                    float v10 = unit->bact->position.x - bact->position.x;
                    float v11 = unit->bact->position.z - bact->position.z;

                    if ( (v10 * v10 + v11 * v11) < 4320000.0 && unit->bact->primTtype == BACT_TGT_TYPE_UNIT && bact == unit->bact->primT.pbact )
                    {
                        if ( v3 )
                        {
                            unit->bacto->setBACT_aggression(99);
                            unit->bact->status_flg &= ~BACT_STFLAG_ESCAPE;
                        }
                        else
                        {
                            unit->bacto->setBACT_aggression(60);
                        }
                    }

                    if ( v7 )
                    {
                        setTarget_msg arg67;

                        arg67.tgt_type = BACT_TGT_TYPE_UNIT;
                        arg67.priority = 0;
                        arg67.tgt.pbact = bact;

                        unit->bacto->SetTarget(&arg67);
                    }
                }
            }

            unit = (bact_node *)unit->next;
        }
    }
}

void NC_STACK_yparobo::AI_clearAttackerSlots()
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = &ypabact;

    if ( bact->clock - robo->field_5A9 > 4000 )
    {
        for (int i = 0; i < 16; i++)
        {
            if ( robo->field_525[i].field_0 )
            {
                int v6 = 0;
                bact_node *node = (bact_node *)bact->subjects_list.head;

                while (node->next)
                {
                    if (node->bact->commandID == robo->field_525[i].field_0)
                    {
                        v6 = 1;
                        break;
                    }

                    node = (bact_node *)node->next;
                }

                if ( !v6 )
                {
                    robo->field_525[i].field_0 = 0;
                    robo->field_525[i].field_4 = 0;
                }
            }
        }
    }
}

void NC_STACK_yparobo::searchEnemyRobo()
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = &ypabact;

    bact_node *node = (bact_node *)robo->wrld->stack__ypaworld.bact_list.head;

    while ( node->next )
    {
        __NC_STACK_ypabact *unit = node->bact;

        if ( unit->status != BACT_STATUS_DEAD )
        {
            if ( unit != bact && unit->bact_type == BACT_TYPES_ROBO )
            {
                if ( (1 << bact->owner) & unit->pSector->view_mask )
                {
                    bact_node *kids = (bact_node *)robo->bact_internal->subjects_list.head;

                    __NC_STACK_ypabact *sel = 0;
                    float selfl = 0.0;

                    while ( kids->next )
                    {
                        __NC_STACK_ypabact *kbct = kids->bact;

                        if (kbct->status != BACT_STATUS_DEAD)
                        {
                            float xx = kbct->position.x - unit->position.x;
                            float zz = kbct->position.z - unit->position.z;
                            float v14 = xx * xx + zz * zz;

                            if ( !sel || v14 < selfl )
                            {
                                sel = kids->bact;
                                selfl = v14;
                            }
                        }
                        kids = (bact_node *)kids->next;
                    }

                    if ( sel )
                    {
                        robo_arg134 v12;

                        v12.field_4 = 6;
                        v12.field_8 = node->bact->owner;
                        v12.field_14 = 48;
                        v12.unit = sel;
                        v12.field_C = 0;
                        v12.field_10 = 0;

                        placeMessage(&v12);
                    }
                }
            }
        }

        node = (bact_node *)node->next;
    }
}

void NC_STACK_yparobo::usersRoboEnergyCheck()
{
    __NC_STACK_ypabact *bact = &ypabact;

    float v1 = (float)bact->energy / (float)bact->energy_max;

    if ( v1 < 0.2 )
    {
        if ( v1 < 0.07 )
            v1 = 0.01;

        robo_arg134 v2;
        v2.field_4 = 40;
        v2.field_C = 0;
        v2.field_10 = 0;
        v2.field_14 = 98;
        v2.field_8 = (v1 * 60000.0 * 5.0);
        v2.unit = bact;

        placeMessage(&v2);
    }
}

void NC_STACK_yparobo::checkCommander()
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = &ypabact;

    _NC_STACK_ypaworld *yw = &robo->wrld->stack__ypaworld;

    NC_STACK_ypabact *wee_bact = robo->wrld->getYW_userHostStation();

    bool userRobo =  (wee_bact == this);

    float v31;

    bact_node *commander = (bact_node *)bact->subjects_list.head;
    while (commander->next)
    {
        __NC_STACK_ypabact * commander_bact = commander->bact;

        if ( commander_bact->status != BACT_STATUS_DEAD )
        {
            if ( robo->dock_user == commander_bact->commandID && robo->dock_user )
            {
                initForce(commander);
            }
            else
            {
                if ( commander_bact->bact_type != BACT_TYPES_GUN )
                {
                    if ( !(commander_bact->status_flg & BACT_STFLAG_ESCAPE) )
                    {
                        if ( commander->bacto->GetFightMotivation(&v31) )
                        {
                            if ( v31 < 0.5 && userRobo )
                            {
                                bact_arg81 arg81;
                                arg81.enrg_sum = 0;
                                arg81.enrg_type = 3;

                                commander->bacto->GetSummary(&arg81);

                                int v18 = arg81.enrg_sum;

                                arg81.enrg_sum = 0;
                                arg81.enrg_type = 5;
                                commander->bacto->GetSummary(&arg81);

                                if ( v18 < arg81.enrg_sum )
                                {
                                    robo_arg134 arg134;
                                    arg134.field_8 = 20000;
                                    arg134.field_10 = 0;
                                    arg134.field_C = 0;
                                    arg134.field_4 = 18;
                                    arg134.unit = commander_bact;
                                    arg134.field_14 = 42;

                                    placeMessage(&arg134);
                                }
                            }

                            if ( !userRobo )
                            {
                                if ( bact->clock - robo->fe_time > 1000 )
                                {
                                    robo->fe_time = bact->clock;

                                    for (int i = 0; i < 16; i++)
                                    {
                                        if ( robo->field_525[i].field_0 == commander_bact->commandID)
                                        {
                                            sb_0x4a7010__sub1(commander, &robo->field_525[i]);
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            commander_bact->status_flg |= BACT_STFLAG_ESCAPE;

                            bact_node *nod = (bact_node *)commander_bact->subjects_list.head;

                            while (nod->next)
                            {
                                nod->bact->status_flg |= BACT_STFLAG_ESCAPE;

                                nod = (bact_node *)nod->next;
                            }

                            commander_bact->status_flg &= ~(BACT_STFLAG_WAYPOINT | BACT_STFLAG_WAYPOINTCCL);
                            commander_bact->waypoints_count = 0;
                            commander_bact->status_flg &= ~BACT_STFLAG_UNUSE;

                            nod = (bact_node *)commander_bact->subjects_list.head;

                            while (nod->next)
                            {
                                nod->bact->status_flg &= ~BACT_STFLAG_UNUSE;

                                nod = (bact_node *)nod->next;
                            }

                            if ( groundVehicleInSquad(commander_bact) )
                            {
                                bact_arg124 arg124;
                                arg124.steps_cnt = 32;
                                arg124.from = commander_bact->position.XZ();
                                arg124.to = bact->position.XZ();
                                arg124.field_12 = 1;

                                if ( ! commander->bacto->PathFinder(&arg124) )
                                    break;

                                if ( arg124.steps_cnt > 1 )
                                {
                                    arg124.steps_cnt = 32;
                                    commander->bacto->SetPath(&arg124);

                                    commander_bact->m_cmdID = bact->commandID;
                                    commander_bact->m_owner = bact->owner;
                                }
                                else
                                {
                                    setTarget_msg arg67;
                                    arg67.tgt.pbact = bact;
                                    arg67.priority = 0;
                                    arg67.tgt_type = BACT_TGT_TYPE_UNIT;
                                    commander->bacto->SetTarget(&arg67);
                                }
                            }
                            else
                            {
                                setTarget_msg arg67;
                                arg67.tgt.pbact = bact;
                                arg67.tgt_type = BACT_TGT_TYPE_UNIT;
                                arg67.priority = 0;
                                commander->bacto->SetTarget(&arg67);
                            }

                            if ( userRobo )
                            {
                                robo_arg134 arg134;
                                arg134.field_8 = commander_bact->commandID;
                                arg134.field_10 = 0;
                                arg134.field_C = 0;
                                arg134.field_4 = 9;
                                arg134.unit = commander_bact;
                                arg134.field_14 = 40;

                                placeMessage(&arg134);

                                if ( robo->wrld->stack__ypaworld.isNetGame  )
                                {
                                    bact_arg90 arg90;
                                    arg90.unit = commander_bact;
                                    arg90.field_8 = 1;
                                    arg90.ret_unit = NULL;
                                    GetSectorTarget(&arg90);

                                    if ( arg90.ret_unit )
                                    {
                                        uamessage_logmsg logMsg;
                                        logMsg.msgID = UAMSG_LOGMSG;
                                        logMsg.owner = bact->owner;
                                        logMsg.sender = arg90.ret_unit->gid;
                                        logMsg.senderOwner = arg90.ret_unit->owner;
                                        logMsg.pr1 = 0;
                                        logMsg.pr2 = 0;
                                        logMsg.pr3 = 0;
                                        logMsg.pri = 34;
                                        logMsg.id = 19;

                                        yw_arg181 arg181;
                                        arg181.recvID = 0;
                                        arg181.recvFlags = 2;
                                        arg181.data = &logMsg;
                                        arg181.dataSize = sizeof(logMsg);
                                        arg181.garant = 1;
                                        robo->wrld->ypaworld_func181(&arg181);
                                    }
                                }
                            }
                            else
                            {
                                __NC_STACK_ypabact *v33 = NULL;

                                nlist *v30 = commander->bacto->getBACT_attackList();

                                bact_node *v17 = (bact_node *)v30->head;
                                while (v17->next)
                                {
                                    if (yw->field_1b80->owner == v17->bact->owner)
                                    {
                                        v33 = v17->bact;
                                        break;
                                    }

                                    v17 = (bact_node *)v17->next;
                                }

                                if ( v33 )
                                {
                                    if ( v33->bact_type == BACT_TYPES_MISSLE ) //If missile
                                    {
                                        NC_STACK_ypamissile *miss = dynamic_cast<NC_STACK_ypamissile *>(v33->self);
                                        v33 = miss->getMISS_launcher(); //Get emitter bact
                                    }

                                    if ( v33->host_station != v33->parent_bacto )
                                        v33 = v33->parent_bact;

                                    robo_arg134 arg134;
                                    arg134.field_8 = 0;
                                    arg134.field_10 = 0;
                                    arg134.field_C = 0;
                                    arg134.field_4 = 19;
                                    arg134.unit = v33;
                                    arg134.field_14 = 34;
                                    placeMessage(&arg134);
                                }
                            }
                        }
                    }
                    else if ( commander->bacto->GetFightMotivation(&v31) )
                    {
                        if ( commander_bact->owner == commander_bact->pSector->owner && commander_bact->pSector->energy_power )
                        {
                            setTarget_msg arg67;
                            arg67.tgt_type = BACT_TGT_TYPE_CELL;
                            arg67.priority = 0;
                            arg67.tgt_pos = commander_bact->position;

                            commander->bacto->SetTarget(&arg67);
                        }

                        commander_bact->status_flg &= ~BACT_STFLAG_ESCAPE;

                        bact_node *nod = (bact_node *)commander_bact->subjects_list.head;

                        while (nod->next)
                        {
                            nod->bact->status_flg &= ~BACT_STFLAG_ESCAPE;

                            nod = (bact_node *)nod->next;
                        }

                        if ( userRobo )
                        {
                            robo_arg134 arg134;
                            arg134.field_8 = commander_bact->commandID;
                            arg134.field_4 = 10;
                            arg134.field_10 = 0;
                            arg134.field_C = 0;
                            arg134.unit = commander_bact;
                            arg134.field_14 = 28;

                            placeMessage(&arg134);
                        }
                    }
                }
            }
        }

        commander = (bact_node *)commander->next;
    }
}

void NC_STACK_yparobo::checkDanger()
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = &ypabact;

    int v11 = 0;
    int v12;

    for(int i = -1; i < 2; i++)
    {
        for(int j = -1; j < 2; j++)
        {
            int v4 = bact->sectY + j;
            int v5 = bact->sectX + i;

            if ( v5 > 0 && v5 < bact->secMaxX - 2 && v4 > 0 && v4 < bact->secMaxY - 2 )
            {

                __NC_STACK_ypabact *v7 = (__NC_STACK_ypabact *)bact->pSector[i + j * bact->secMaxX].units_list.head;

                while (v7->next)
                {
                    if (v7->owner != robo->bact_internal->owner && v7->status != BACT_STATUS_DEAD && v7->weapon != -1 && v7->mgun != -1)
                    {
                        v11 = 1;
                        v12 = v7->commandID;
                        break;
                    }

                    v7 = (__NC_STACK_ypabact *)v7->next;
                }
            }

        }
    }

    if ( v11 )
    {
        if ( !(robo->roboState & ROBOSTATE_DANGER) && robo->roboState & ROBOSTATE_PLAYERROBO )
        {
            robo_arg134 arg134;
            arg134.field_4 = 13;
            arg134.field_8 = v12;
            arg134.field_10 = 0;
            arg134.field_C = 0;
            arg134.unit = bact;
            arg134.field_14 = 99;

            placeMessage(&arg134);
        }

        robo->roboState |= ROBOSTATE_DANGER;
    }
    else
    {
        robo->roboState &= ~ROBOSTATE_DANGER;
    }
}

int yparobo_func70__sub6__sub0(__NC_STACK_yparobo *robo, cellArea *cell)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int xx = robo->radar_cellIDpos % bact->secMaxX;
    int yy = robo->radar_cellIDpos / bact->secMaxX;

    if ( !(cell->view_mask & (1 << bact->owner)) )
        return -1;

//  if ( cell->field_2E != 1 )
//  {
//      for (int i = 0; i < 3; i++)
//      {
//          for (int j = 0; j < 3; j++)
//          {
//
//          }
//      }
//  }

    if ( cell->owner != robo->bact_internal->owner )
        return -1;

    if ( cell->w_type )
        return -1;

    float v8 = sqrt( POW2(bact->sectY - yy) + POW2(bact->sectX - xx) );

    float v14 = ( v8 <= 0.01 ? 500.0 : 1000.0 / v8 );

    int v9 = 0;

    for (int i = 0; i < 8; i++)
    {
        if ( !((1 << bact->owner) & cell[ word_5182AE[i].x + word_5182AE[i].y * bact->secMaxX ].view_mask ) )
        {
            v9 = 1;
            v14 += 10.0;
        }
    }

    if ( !v9 || xx <= 1 || xx >= bact->secMaxX - 2 || yy <= 1 || yy >= bact->secMaxY - 2 )
        return -1;

    return v14;
}

int yparobo_func70__sub6__sub1(__NC_STACK_yparobo *robo, cellArea *cell)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int xx = robo->field_2C1 % bact->secMaxX;
    int yy = robo->field_2C1 / bact->secMaxX;

    if ( cell->view_mask & (1 << bact->owner) )
        return -1;

    float v13 = sqrt(POW2(bact->sectY - yy) + POW2(bact->sectX - xx));

    if ( v13 <= 0.01 )
        v13 = 0.0;

    for (int i = 0; i < 8; i++)
    {
        if ( !((1 << bact->owner) & cell[ word_5182AE[i].x + word_5182AE[i].y * bact->secMaxX ].view_mask ) )
        {
            v13 += 5.0;
        }
    }

    return v13;
}

int yparobo_func70__sub6__sub2(__NC_STACK_yparobo *robo, cellArea *cell)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int xx = robo->safety_cellIDpos % bact->secMaxX;
    int yy = robo->safety_cellIDpos / bact->secMaxX;

    if ( !(cell->view_mask & (1 << bact->owner)) )
        return -1;

//  if ( cell->field_2E != 1 )
//  {
//      for (int i = 0; i < 3; i++)
//      {
//          for (int j = 0; j < 3; j++)
//          {
//
//          }
//      }
//  }

    if ( cell->owner != robo->bact_internal->owner || cell->w_type )
        return -1;

    float v9 = sqrt( POW2(bact->sectY - yy) + POW2(bact->sectX - xx) );

    float v14;

    if ( v9 <= 0.01 )
        v14 = 500.0;
    else
        v14 = 1000.0 / v9;

    for (int i = 0; i < 8; i++)
    {
        cellArea *cll = &cell[ word_5182AE[i].x + word_5182AE[i].y * bact->secMaxX ];

        if ( cll->w_type && bact->owner == cll->owner)
            v14 += 5.0;

    }

    return v14;
}

int yparobo_func70__sub6__sub3(__NC_STACK_yparobo *robo, cellArea *cell)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int xx = robo->field_25D % bact->secMaxX;
    int yy = robo->field_25D / bact->secMaxX;

    if ( !(cell->view_mask & (1 << bact->owner)) )
        return -1;

    if ( cell->energy_power >= 255 )
        return -1;

    if ( cell->w_type )
        return -1;

//  if ( cell->field_2E != 1 )
//  {
//      for (int i = 0; i < 3; i++)
//      {
//          for (int j = 0; j < 3; j++)
//          {
//
//          }
//      }
//  }

    if ( cell->owner != bact->owner )
        return -1;

    float v9 = sqrt( POW2(bact->sectY - yy) + POW2(bact->sectX - xx) );
    float v26 = v9 <= 0.01 ? 500.0 : 1000.0 / v9;

    if ( v9 > 8.0 )
        return -1;

    float v27 = v26 + (255 - cell->energy_power) / 3.0;

    for (int i = 0; i < 8; i++)
    {
        cellArea *cll = &cell[ word_5182AE[i].x + word_5182AE[i].y * bact->secMaxX ];

        if ( cll->owner == bact->owner )
        {
            int v11 = 0;

            if ( cll->comp_type == 1 )
            {
                v11 = cll->buildings_health[0][0];
            }
            else
            {
                for (int j = 0; j < 3; j++)
                {
                    for (int k = 0; k < 3; k++)
                    {
                        v11 += cll->buildings_health[j][k];
                    }
                }
            }
            v27 += (255 - v11) * 0.05 + 10.0;
        }

        if ( cll->w_type == 2 )
            v27 *= 0.7;
    }

    __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)cell->units_list.head;

    while ( bct->next )
    {
        v27 -= 5.0;
        bct = (__NC_STACK_ypabact *)bct->next;
    }

    return v27;
}

int yparobo_func70__sub6__sub4(__NC_STACK_yparobo *robo, cellArea *cell)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int xx = robo->conq_cellIDpos % bact->secMaxX;
    int yy = robo->conq_cellIDpos / bact->secMaxX;

    if ( cell->owner == bact->owner )
        return -1;

    float v7 = sqrt( POW2(bact->sectY - yy) + POW2(bact->sectX - xx) );
    float v25;

    if ( v7 <= 0.01 )
        v25 = 1500.0;
    else
        v25 = 1000.0 / v7;

    if ( (1 << bact->owner) & cell->view_mask )
    {
        if ( cell->w_type )
            v25 = v25 + 20.0;
        if ( cell->w_type == 6 )
            v25 = v25 + 40.0;
        if ( cell->w_type == 2 )
            v25 = v25 + 50.0;
    }

    for (int i = 0; i < 8; i++)
    {
        int tx = xx + word_5182AE[i].x;
        int ty = yy + word_5182AE[i].y;

        if ( tx >= 1 && ty >= 1 && tx <= bact->secMaxX - 2 && ty <= bact->secMaxY - 2)
        {
            if ( cell[ word_5182AE[i].x + word_5182AE[i].y * bact->secMaxX ].owner == bact->owner )
                v25 = v25 + 3.0;
        }
    }

    if ( (1 << bact->owner) & cell->view_mask )
    {
        __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)cell->units_list.head;

        while ( bct->next )
        {
            if ( bct->owner != bact->owner && bct->owner)
            {
                if ( bct->bact_type != BACT_TYPES_MISSLE)
                {
                    if ( bct->bact_type == BACT_TYPES_ROBO)
                        v25 += 100.0;
                    else
                        v25 += 5.0;
                }
            }

            bct = (__NC_STACK_ypabact *)bct->next;
        }
    }

    return v25;
}

int yparobo_func70__sub6__sub8(__NC_STACK_yparobo *robo)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int num = bact->secMaxX * bact->secMaxY;

    int v4 = 0;

    for (int i = 0; i < num; i++)
    {
        cellArea *cll = &robo->pcell[i];

        if ( cll->w_type == 3 )
        {
            __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)cll->units_list.head;
            while (bct->next)
            {
                if ( bct->bact_type == BACT_TYPES_GUN && bct->status != BACT_STATUS_DEAD && bact->owner == bct->owner )
                {
                    NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( bct->self );
                    int a4 = gun->getGUN_roboGun();

                    if ( a4 == 0 && ( bct->weapon != -1 || bct->mgun != -1 ) )
                    {
                        v4++;
                        break;
                    }
                }

                bct = (__NC_STACK_ypabact *)bct->next;
            }
        }
    }

    return v4;
}

int yparobo_func70__sub6__sub9(__NC_STACK_yparobo *robo)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int num = bact->secMaxX * bact->secMaxY;

    int v4 = 0;

    for (int i = 0; i < num; i++)
    {
        cellArea *cll = &robo->pcell[i];

        if ( cll->w_type == 3 )
        {
            __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)cll->units_list.head;
            while (bct->next)
            {
                if ( bct->bact_type == BACT_TYPES_GUN && bct->status != BACT_STATUS_DEAD && bact->owner == bct->owner )
                {
                    NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( bct->self );
                    int a4 = gun->getGUN_roboGun();

                    if ( a4 == 0 && bct->weapon == -1 && bct->mgun == -1 )
                    {
                        v4++;
                        break;
                    }
                }

                bct = (__NC_STACK_ypabact *)bct->next;
            }
        }
    }

    return v4;
}

int yparobo_func70__sub6__sub10(__NC_STACK_yparobo *robo)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int num = bact->secMaxX * bact->secMaxY;

    int v4 = 0;

    for (int i = 0; i < num; i++)
    {
        if ( robo->pcell[i].owner == bact->owner )
            v4++;
    }

    return v4;
}

int yparobo_func70__sub6__sub11(__NC_STACK_yparobo *robo)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int num = bact->secMaxX * bact->secMaxY;

    int v4 = 0;

    for (int i = 0; i < num; i++)
    {
        if ( !((1 << bact->owner) & robo->pcell[i].view_mask) )
            v4++;
    }

    return v4;
}

int yparobo_func70__sub6__sub12(__NC_STACK_yparobo *robo)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    bact_node *node = &bact->subject_node;

    while (node->prev)
        node = (bact_node *)node->prev;

    node = (bact_node *)node->next;

    while (node->next)
    {
        if ( node->bact->bact_type == BACT_TYPES_ROBO)
        {
            if ( bact->owner != node->bact->owner )
            {
                int v9 = bact->sectX - node->bact->sectX;
                int v10 = bact->sectY - node->bact->sectY;

                if ( abs(v9) < 3 && abs(v10) < 3 )
                    return 1;
            }
        }

        node = (bact_node *)node->next;
    }

    return 0;
}

int yparobo_func70__sub6__sub13(__NC_STACK_yparobo *robo)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    for (int i = -2; i <= 2; i++)
    {
        for (int j = -2; j <= 2; j++)
        {
            int xx = bact->sectX + i;
            int yy = bact->sectY + j;

            if ( xx > 0 && xx < bact->secMaxX - 1 && yy > 0 && yy < bact->secMaxY - 1 )
            {
                __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *) bact->pSector[ i + j * bact->secMaxX ].units_list.head;

                while ( bct->next )
                {
                    if ( bct->owner )
                    {
                        if ( bct->owner != bact->owner )
                        {
                            if ( bct->bact_type != BACT_TYPES_MISSLE && bct->bact_type != BACT_TYPES_ROBO && bct->status != BACT_STATUS_DEAD )
                                return 1;
                        }
                    }
                    bct = (__NC_STACK_ypabact *)bct->next;
                }
            }
        }
    }

    return 0;
}


float sub_4F4C6C(bact_node *bctnd, __NC_STACK_yparobo *robo, int a4, int a3)
{
    __NC_STACK_ypabact *bct = bctnd->bact;

    if ( bct->status_flg & BACT_STFLAG_DEATH1 )
        return -1.0;

    float v8 = sqrt( POW2(a3 - bct->sectY) + POW2(a4 - bct->sectX) );

    if (v8 != 0.0)
        v8 = 100.0 / v8;
    else
        v8 = 200.0;

    if ( bct->pSector->owner == robo->bact_internal->owner )
        v8 += 100.0;

    return v8;
}

int yparobo_func70__sub6__sub5(__NC_STACK_yparobo *robo, int *a2, int *px, int *py)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    bact_node *node = &bact->subject_node;

    while (node->prev)
        node = (bact_node *)node->prev;

    node = (bact_node *)node->next;

    float v32 = -0.5;

    int v29 = 0;

    while (node->next)
    {

        if ( node->bact->owner != bact->owner  &&  node->bact->owner  &&  node->bact->status != BACT_STATUS_DEAD )
        {
            if ( abs(node->bact->sectX - bact->sectX) <= 2 &&
                    abs(node->bact->sectY - bact->sectY) <= 2 )
            {
                *a2 = node->bact->commandID;
                *px = node->bact->sectX;
                *py = node->bact->sectY;

                return 500;
            }

            bact_node *ndbct = (bact_node *)node->bact->subjects_list.head;

            while (ndbct->next)
            {

                if ( (ndbct->bact->gid & 0xF) == robo->field_27D )
                {
                    int v26 = 0;

                    if ( ndbct->bact->bact_type == BACT_TYPES_GUN )
                    {
                        NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( ndbct->bacto );
                        v26 = gun->getGUN_roboGun();
                    }

                    if ( (1 << bact->owner) & ndbct->bact->pSector->view_mask )
                    {
                        if ( ndbct->bact->status != BACT_STATUS_DEAD && !v26 )
                        {
                            float tmp = sub_4F4C6C(ndbct, robo, bact->sectX, bact->sectY);
                            if ( tmp > v32 )
                            {
                                v32 = tmp;
                                v29 = ndbct->bact->commandID;
                                *px = ndbct->bact->sectX;
                                *py = ndbct->bact->sectY;
                            }
                        }
                    }
                }
                ndbct = (bact_node *)ndbct->next;
            }
        }


        node = (bact_node *)node->next;
    }

    robo->field_27D++;

    if ( robo->field_27D > 15 )
        robo->field_27D = 0;

    if ( v32 < 0.0 )
    {
        *a2 = 0;
        return -1;
    }

    *a2 = v29;
    return v32;
}

int yparobo_func70__sub6__sub6(__NC_STACK_yparobo *robo, int *a2, int *px, int *py)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    float v32 = -0.5;
    int v21 = 0;

    bact_node *node = &bact->subject_node;

    while (node->prev)
        node = (bact_node *)node->prev;

    node = (bact_node *)node->next;

    while (node->next)
    {
        if ( node->bact->owner != robo->bact_internal->owner && node->bact->owner && node->bact->status != BACT_STATUS_DEAD )
        {
            if ( (1 << bact->owner) & node->bact->pSector->view_mask )
            {
                float v13 = sub_4F4C6C(node, robo, bact->sectX, bact->sectY);
                if ( v13 > v32 )
                {
                    v21 = node->bact->commandID;
                    v32 = v13;
                    *px = node->bact->sectX;
                    *py = node->bact->sectY;
                }
            }
        }
        node = (bact_node *)node->next;
    }

    if ( v32 < 0.0 )
    {
        *a2 = 0;
        return -1;
    }

    *a2 = v21;
    return v32;
}


int sub_4F4E48(__NC_STACK_yparobo *robo, int x, int y)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    float v14 = 0.0;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            cellArea *cll = &robo->pcell[(y + j) * bact->secMaxX + x + i];

            yw_arg176 arg176;
            arg176.owner = cll->owner;

            robo->wrld->ypaworld_func176(&arg176);

            float v15;

            if ( i || j )
                v15 = 1.0;
            else
                v15 = 3.0;

            if ( bact->owner == cll->owner )
                v14 += cll->energy_power * arg176.field_4 + v15;
            else
                v14 -= cll->energy_power * arg176.field_4 + v15;

            __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)cll->units_list.head;

            while ( bct->next )
            {
                float v16 = 0;

                if ( bct->bact_type == BACT_TYPES_ROBO && bct != bact )
                {
                    v16 = 5.0;
                }
                else
                {
                    if ( bct->bact_type == BACT_TYPES_GUN )
                    {
                        NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( bct->self );

                        if ( !gun->getGUN_roboGun() )
                            v16 = 1.0;
                    }
                }

                if ( v16 > 0.0 )
                {
                    if ( bct->owner == bact->owner )
                        v14 += v16;
                    else
                        v14 -= v16;
                }

                bct = (__NC_STACK_ypabact *)bct->next;
            }
        }
    }

    return v14;
}

int yparobo_func70__sub6__sub7(__NC_STACK_yparobo *robo)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int xx = robo->field_2A9 % bact->secMaxX;
    int yy = robo->field_2A9 / bact->secMaxX;

    int a1_4 = sub_4F4E48(robo, bact->sectX, bact->sectY);
    int v14 = sub_4F4E48(robo, xx, yy);

    float v12 = sqrt( POW2(bact->sectY - yy) + POW2(bact->sectX - xx) );

    if ( v12 > 0.0 && a1_4 < v14 )
        v14 = ((float)v14 * (1.0 - v12 * 0.8 / 91.0));

    if ( v14 <= a1_4 )
        return -1;

    return v14;
}

void NC_STACK_yparobo::AI_checkWorld(update_msg *arg)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = &ypabact;

    int v6 = 1;
    if ( robo->newAI )
    {
        robo->conq_delay -= arg->frameTime;
        if ( robo->conq_delay > 0 )
            v6 = 0;
    }

    robo_arg128 arg128;

    if ( robo->epConquer && v6 )
    {
        int v89 = (bact->secMaxY + bact->secMaxX) / 20 + 1;

        for (int i = v89; i > 0; i--)
        {
            if ( bact->secMaxX * bact->secMaxY == robo->conq_cellIDpos )
            {
                robo->conq_cellIDpos = 0;
                robo->roboState |= ROBOSTATE_READYCONQUER;
                return;
            }

            cellArea *pcell = &robo->pcell[ robo->conq_cellIDpos ];

            int xx = robo->conq_cellIDpos % bact->secMaxX;
            int yy = robo->conq_cellIDpos / bact->secMaxX;

            if ( xx && xx != bact->secMaxX - 1 && yy && yy != bact->secMaxY - 1 )
            {
                int v12 = yparobo_func70__sub6__sub4(robo, pcell);
                if ( v12 > robo->conq_value )
                {
                    arg128.flags = 2;
                    arg128.tgType = BACT_TGT_TYPE_CELL;
                    arg128.tgt_pos.x = (xx + 0.5) * 1200.0;
                    arg128.tgt_pos.z = -(yy + 0.5) * 1200.0;

                    robo->roboo->yparobo_func128(&arg128);

                    if ( !arg128.comm_bacto )
                    {
                        robo->conq_value = v12;
                        robo->conq_cellID = robo->conq_cellIDpos;
                        robo->conq_cell = pcell;
                        robo->conq_time = arg->gTime;
                    }
                }
            }

            robo->conq_cellIDpos++;
        }
    }
    else
    {
        robo->conq_cellIDpos = 0;
        robo->conq_value = 0;
        robo->roboState &= ~ROBOSTATE_READYCONQUER;
    }

    v6 = 1;
    if ( robo->newAI )
    {
        robo->field_281 -= arg->frameTime;

        if ( robo->field_281 > 0 )
            v6 = 0;
    }

    if ( yparobo_func70__sub6__sub13(robo) )
        v6 = 1;

    if ( robo->epDefense && v6 )
    {
        int v91, xx, yy;

        int v17 = yparobo_func70__sub6__sub5(robo, &v91, &xx, &yy);
        if ( v17 > robo->field_269 )
        {
            arg128.flags = 2;
            arg128.tgType = BACT_TGT_TYPE_UNIT;
            arg128.prim_comm_id = v91;
            robo->roboo->yparobo_func128(&arg128);

            if ( !arg128.comm_bacto )
            {
                robo->field_269 = v17;
                robo->field_275 = v91;
                robo->field_279 = arg->gTime;
                robo->field_271 = yy * bact->secMaxX + xx;
            }
        }
        robo->roboState |= ROBOSTATE_READYDEFENSE;
    }
    else
    {
        robo->field_269 = 0;
        robo->roboState &= ~ROBOSTATE_READYDEFENSE;
    }

    v6 = 1;
    if ( robo->newAI )
    {
        robo->field_2E1 -= arg->frameTime;
        if ( robo->field_2E1 > 0 )
            v6 = 0;
    }

    if ( yparobo_func70__sub6__sub12(robo) )
        v6 = 1;

    if ( robo->epRobo && v6 )
    {
        int v91, xx, yy;
        int v22 = yparobo_func70__sub6__sub6(robo, &v91, &xx, &yy);

        if ( v22 > robo->field_2CD )
        {
            robo_arg128 arg128;
            arg128.flags = 2;
            arg128.tgType = BACT_TGT_TYPE_UNIT;
            arg128.prim_comm_id = v91;

            robo->roboo->yparobo_func128(&arg128);

            if ( !arg128.comm_bacto )
            {
                robo->field_2CD = v22;
                robo->field_2D9 = v91;
                robo->field_2DD = arg->gTime;
                robo->field_2D5 = bact->secMaxX * yy + xx;
            }
        }
        robo->roboState |= ROBOSTATE_READYROBO;
    }
    else
    {
        robo->field_2CD = 0;
        robo->roboState &= ~ROBOSTATE_READYROBO;
    }

    v6 = 1;
    if ( robo->newAI )
    {
        robo->field_2B1 -= arg->frameTime;
        if ( robo->field_2B1 > 0 )
            v6 = 0;
    }

    if ( robo->epChangePlace && v6 )
    {
        int v25 = (bact->secMaxX + bact->secMaxY) / 10 + 1;

        for (int i = v25; i > 0; i--)
        {
            if ( bact->secMaxX * bact->secMaxY == robo->field_2A9 )
            {
                robo->field_2A9 = 0;
                robo->roboState |= ROBOSTATE_FOUNDPLACE;
                return;
            }

            int xx = robo->field_2A9 % bact->secMaxX;
            int yy = robo->field_2A9 / bact->secMaxX;

            if ( xx && xx != bact->secMaxX - 1 && yy && yy != bact->secMaxY - 1 )
            {
                int v34 = yparobo_func70__sub6__sub7(robo);
                if ( v34 > robo->field_29D )
                {
                    robo->field_29D = v34;
                    robo->field_2A5 = robo->field_2A9;
                    robo->field_2AD = arg->gTime;
                }
            }
            robo->field_2A9++;
        }
    }
    else
    {
        robo->field_29D = 0;
        robo->field_2A9 = 0;
        robo->roboState &= ~ROBOSTATE_FOUNDPLACE;
    }

    yw_arg176 v85;
    v85.owner = bact->owner;
    robo->wrld->ypaworld_func176(&v85);

    v6 = 1;
    if ( robo->newAI )
    {
        robo->field_265 -= arg->frameTime;
        if ( robo->field_265 > 0 )
            v6 = 0;
    }

    if ( v85.field_4 == 0.0 )
    {
        if ( robo->epPower )
            v6 = 1;
    }

    float v93;
    if ( robo->epPower )
        v93 = 100.0 / (float)robo->epPower * 0.9;
    else
        v93 = 0.89999998;

    if ( robo->epPower && (v85.field_8 > v93 || v85.field_4 < 0.0001) && v6 )
    {
        int v99 = (bact->secMaxX + bact->secMaxY) / 20 + 1;
        for (int i = v99; i > 0; i--)
        {
            if ( bact->secMaxX * bact->secMaxY == robo->field_25D )
            {
                robo->field_25D = 0;
                robo->roboState |= ROBOSTATE_READYPOWER;
                return;
            }
            int xx = robo->field_25D % bact->secMaxX;
            int yy = robo->field_25D / bact->secMaxX;

            if ( xx && xx != bact->secMaxX - 1 && yy && yy != bact->secMaxY - 1 )
            {
                cellArea *pcell = &robo->pcell[robo->field_25D];
                int v46 = yparobo_func70__sub6__sub3(robo, pcell);
                if ( v46 > robo->field_251 )
                {
                    robo->field_251 = v46;
                    robo->field_255 = pcell;
                    robo->field_259 = robo->field_25D;
                    robo->field_261 = arg->gTime;
                }
            }
            robo->field_25D++;
        }
    }
    else
    {
        robo->field_251 = 0;
        robo->field_25D = 0;
        robo->roboState &= ~ROBOSTATE_READYPOWER;
    }

    v6 = 1;
    if ( robo->newAI )
    {
        robo->radar_delay -= arg->frameTime;
        if ( robo->radar_delay > 0 )
            v6 = 0;
    }

    if ( robo->epRadar && (yparobo_func70__sub6__sub11(robo) / 64 > yparobo_func70__sub6__sub9(robo)) && v6 )
    {
        int v48 = (bact->secMaxY + bact->secMaxX) / 20 + 1;

        for(int i = v48; i > 0; i--)
        {
            if ( bact->secMaxX * bact->secMaxY == robo->radar_cellIDpos )
            {
                robo->radar_cellIDpos = 0;
                robo->roboState |= ROBOSTATE_READYRADAR;
                return;
            }

            int xx = robo->radar_cellIDpos % bact->secMaxX;
            int yy = robo->radar_cellIDpos / bact->secMaxX;

            if ( xx && xx != bact->secMaxX - 1 && yy && yy != bact->secMaxY - 1 )
            {
                cellArea *pcell = &robo->pcell[robo->radar_cellIDpos];

                int v58 = yparobo_func70__sub6__sub0(robo, pcell);

                if ( v58 > robo->radar_value )
                {
                    robo->radar_value = v58;
                    robo->radar_cell = pcell;
                    robo->radar_cellID = robo->radar_cellIDpos;
                    robo->radar_time = arg->gTime;
                }
            }
            robo->radar_cellIDpos++;
        }
    }
    else
    {
        robo->radar_cellIDpos = 0;
        robo->radar_value = 0;
        robo->roboState &= ~ROBOSTATE_READYRADAR;
    }


    v6 = 1;
    if ( robo->newAI )
    {
        robo->safety_delay -= arg->frameTime;
        if ( robo->safety_delay > 0 )
            v6 = 0;
    }

    int v92;
    if ( robo->epSafety )
        v92 = 10 * (100 / robo->epSafety);
    else
        v92 = 10;

    if ( robo->epSafety && (yparobo_func70__sub6__sub10(robo) / v92) >= yparobo_func70__sub6__sub8(robo) && v6 )
    {
        int v60 = (bact->secMaxY + bact->secMaxX) / 20 + 1;

        for(int i = v60; i > 0; i--)
        {
            if ( bact->secMaxX * bact->secMaxY == robo->safety_cellIDpos )
            {
                robo->safety_cellIDpos = 0;
                robo->roboState |= ROBOSTATE_READYSAFE;
                return;
            }

            int xx = robo->safety_cellIDpos % bact->secMaxX;
            int yy = robo->safety_cellIDpos / bact->secMaxX;

            if ( xx && xx != bact->secMaxX - 1 && yy && yy != bact->secMaxY - 1 )
            {
                cellArea *pcell = &robo->pcell[robo->safety_cellIDpos];
                int v71 = yparobo_func70__sub6__sub2(robo, pcell);

                if ( v71 > robo->safety_value )
                {
                    robo->safety_value = v71;
                    robo->safety_cell = pcell;
                    robo->safety_cellID = robo->safety_cellIDpos;
                    robo->safety_time = arg->gTime;
                }
            }
            robo->safety_cellIDpos++;
        }
    }
    else
    {
        robo->safety_value = 0;
        robo->safety_cellIDpos = 0;
        robo->roboState &= ~ROBOSTATE_READYSAFE;
    }


    v6 = 1;
    if ( robo->newAI )
    {
        robo->field_2C9 -= arg->frameTime;
        if ( robo->field_2C9 > 0 )
            v6 = 0;
    }

    if ( robo->epRecon && v6 )
    {
        int v90 = (bact->secMaxY + bact->secMaxX) / 20 + 1;

        for (int i = v90; i > 0; i-- )
        {
            if ( bact->secMaxX * bact->secMaxY == robo->field_2C1 )
            {
                robo->field_2C1 = 0;
                robo->roboState |= ROBOSTATE_READYRECON;
                return;
            }

            int xx = robo->field_2C1 % bact->secMaxX;
            int yy = robo->field_2C1 / bact->secMaxX;

            if ( xx > 1 && xx < bact->secMaxX - 2 && yy > 1 && yy < bact->secMaxY - 2 )
            {
                cellArea *pcell = &robo->pcell[robo->field_2C1];
                int v81 = yparobo_func70__sub6__sub1(robo, pcell);

                if ( v81 > robo->field_2B5 )
                {
                    robo_arg128 arg128;
                    arg128.tgType = BACT_TGT_TYPE_CELL;
                    arg128.flags = 2;
                    arg128.tgt_pos.x = (xx + 0.5) * 1200.0;
                    arg128.tgt_pos.z = -(yy + 0.5) * 1200.0;

                    robo->roboo->yparobo_func128(&arg128);

                    if ( !arg128.comm_bacto )
                    {
                        robo->field_2B5 = v81;
                        robo->field_2BD = robo->field_2C1;
                        robo->field_2B9 = pcell;
                        robo->field_2C5 = arg->gTime;
                    }
                }
            }
            robo->field_2C1++;
        }
    }
    else
    {
        robo->field_2B5 = 0;
        robo->field_2C1 = 0;
        robo->roboState &= ~ROBOSTATE_READYRECON;
    }
}

void NC_STACK_yparobo::AI_layer3(update_msg *arg)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int a4 = getBACT_bactCollisions();

    float tmp = bact->target_vec.length();

    if ( tmp <= 0.0 )
        bact->target_dir = vec3d(0.0, 0.0, 0.0);
    else
        bact->target_dir = bact->target_vec / tmp;

    if ( bact->status == BACT_STATUS_NORMAL )
    {
        if ( robo->roboState & ROBOSTATE_MOVE )
            doBeamUpdate(arg->frameTime);

        if ( !a4 || ! CollisionWithBact(arg->frameTime) )
        {
            checkCollisions(arg->frameTime * 0.001);

            if ( robo->roboState & ROBOSTATE_PLAYERROBO )
                wallow(arg);
            else
                AI_doMove(arg);

            if ( robo->roboState & ROBOSTATE_PLAYERROBO )
            {
                searchEnemyRobo();
                usersRoboEnergyCheck();
                doUserCommands(arg);
            }

            if ( !(robo->roboState & ROBOSTATE_PLAYERROBO) )
            {
                AI_checkWorld(arg);
                AI_doWish(arg);
                AI_service(arg);
                AI_clearAttackerSlots();
            }

            checkCommander();

            checkDanger();

            if ( robo->roboState & ROBOSTATE_DANGER && !(robo->roboState & ROBOSTATE_PLAYERROBO) )
                AI_solveTrouble(arg);
        }
    }
    else if ( bact->status == BACT_STATUS_DEAD )
    {
        DeadTimeUpdate(arg);
    }
}

void yparobo_func71__sub0(__NC_STACK_yparobo *robo, update_msg *arg)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    struC5 *inpt = arg->inpt;
    float v18 = arg->frameTime / 1000.0;


    if ( inpt->sliders_vars[1] < -0.001 )
    {
        bact->viewer_vert_angle += inpt->sliders_vars[1] * bact->maxrot * v18 * 2.0;

        if ( bact->viewer_vert_angle > bact->viewer_max_up )
            bact->viewer_vert_angle = bact->viewer_max_up;

        if ( bact->viewer_vert_angle < -bact->viewer_max_down )
            bact->viewer_vert_angle = -bact->viewer_max_down;
    }
    else if (inpt->sliders_vars[1] > 0.001)
    {
        bact->viewer_vert_angle += inpt->sliders_vars[1] * bact->maxrot * v18 * 2.0;

        if ( bact->viewer_vert_angle > bact->viewer_max_up )
            bact->viewer_vert_angle = bact->viewer_max_up;

        if ( bact->viewer_vert_angle < -bact->viewer_max_down )
            bact->viewer_vert_angle = -bact->viewer_max_down;
    }

    if ( inpt->sliders_vars[0] < -0.001 )
    {
        bact->viewer_horiz_angle -= inpt->sliders_vars[0] * bact->maxrot * v18 * 2.0;

        if ( bact->viewer_max_side < 3.15 )
        {
            if ( bact->viewer_horiz_angle > bact->viewer_max_side )
                bact->viewer_horiz_angle = bact->viewer_max_side;

            if ( bact->viewer_horiz_angle < -bact->viewer_max_side )
                bact->viewer_horiz_angle = -bact->viewer_max_side;
        }
    }
    else if (inpt->sliders_vars[0] > 0.001)
    {
        bact->viewer_horiz_angle -= inpt->sliders_vars[0] * bact->maxrot * v18 * 2.0;

        if ( bact->viewer_max_side < 3.15 )
        {
            if ( bact->viewer_horiz_angle > bact->viewer_max_side )
                bact->viewer_horiz_angle = bact->viewer_max_side;

            if ( bact->viewer_horiz_angle < -bact->viewer_max_side )
                bact->viewer_horiz_angle = -bact->viewer_max_side;
        }
    }
}

void NC_STACK_yparobo::User_layer(update_msg *arg)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;

    int a4 = getBACT_bactCollisions();

    __NC_STACK_ypabact *bact = robo->bact_internal;

    float v36 = bact->target_vec.length();

    if ( v36 <= 0.0 )
        bact->target_dir = vec3d(0.0, 0.0, 0.0);
    else
        bact->target_dir = bact->target_vec / v36;

    if ( bact->status == BACT_STATUS_NORMAL )
    {
        if ( robo->roboState & ROBOSTATE_MOVE )
            doBeamUpdate(arg->frameTime);

        if ( !a4 || !CollisionWithBact(arg->frameTime) )
        {
            checkCommander();
            checkDanger();
            searchEnemyRobo();
            usersRoboEnergyCheck();

            if ( arg->inpt->but_flags & 8 )
                HandBrake(arg);

            doUserCommands(arg);
            yparobo_func71__sub0(robo, arg);
            wallow(arg);

            bact->viewer_rotation = bact->rotation;

            bact->viewer_rotation = mat3x3::RotateY(bact->viewer_horiz_angle) * bact->viewer_rotation;
            bact->viewer_rotation = mat3x3::RotateX(bact->viewer_vert_angle) * bact->viewer_rotation;
        }
    }
    else if ( bact->status == BACT_STATUS_DEAD )
    {
        DeadTimeUpdate(arg);
    }
    else if ( bact->status == BACT_STATUS_IDLE )
    {
        setState_msg arg78;
        arg78.newStatus = BACT_STATUS_NORMAL;
        arg78.setFlags = 0;
        arg78.unsetFlags = 0;

        SetState(&arg78);

        bact->status_flg &= ~BACT_STFLAG_LAND;
    }
}

void NC_STACK_yparobo::Move(move_msg *arg)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = robo->bact_internal;

    bact->old_pos = bact->position;

    vec3d v63;
    if ( bact->status == BACT_STATUS_DEAD )
        v63 = vec3d::OY(bact->mass * 39.2266);
    else
        v63 = vec3d::OY(bact->mass * 9.80665);


    vec3d v52(0.0, 0.0, 0.0);
    if ( !(arg->flag & 1) )
        v52 = vec3d::OY(-robo->field_c);

    vec3d v51(0.0, 0.0, 0.0);
    if ( !(arg->flag & 1) )
        v51 = arg->vec * bact->thraction;

    vec3d v57 = v63 + v51 - bact->fly_dir * (bact->fly_dir_length * bact->airconst) + v52;

    if ( v57.length() > 0.0 )
    {
        vec3d v67 = bact->fly_dir * bact->fly_dir_length + v57 * (arg->field_0 / bact->mass);

        robo->bact_internal->fly_dir_length = v67.normalise();
        robo->bact_internal->fly_dir = v67;
    }

    if ( fabs(bact->fly_dir_length) > 0.1 )
        bact->position += bact->fly_dir * (bact->fly_dir_length * arg->field_0 * 6.0);

    CorrectPositionInLevelBox(NULL);

    for (int i = 0; i < 8; i++)
    {
        if (robo->guns[i].gun_obj)
        {
            bact_arg80 arg80;
            arg80.pos = bact->position + bact->rotation.Transpose().Transform( robo->guns[i].pos );
            arg80.field_C = 4;

            robo->guns[i].gun_obj->SetPosition(&arg80);
        }
    }

    robo->bact_internal->soundcarrier.samples_data[0].pitch = robo->bact_internal->pitch;
    robo->bact_internal->soundcarrier.samples_data[0].volume = robo->bact_internal->volume;

    float v60 = fabs(bact->fly_dir_length) / (bact->force / bact->airconst_static) * 1.4;

    if ( bact->soundcarrier.samples_data[0].psampl )
        bact->soundcarrier.samples_data[0].pitch = (bact->soundcarrier.samples_data[0].psampl->SampleRate + bact->soundcarrier.samples_data[0].pitch) * v60;
}

void NC_STACK_yparobo::Die()
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = robo->bact_internal;

    _NC_STACK_ypaworld *yw = robo->wrld_yw;

    int a4 = getBACT_yourLastSeconds();

    uamessage_hostDie hdMsg;

    if ( !(bact->status_flg & BACT_STFLAG_DEATH1) )
    {
        hdMsg.msgID = UAMSG_HOSTDIE;
        hdMsg.owner = bact->owner;

        if (bact->killer)
            hdMsg.killer = bact->killer->gid;
        else
            hdMsg.killer = 0;

        hdMsg.killerOwner = bact->killer_owner;

        if ( bact->killer_owner )
        {
            if ( !(bact->status_flg & BACT_STFLAG_CLEAN) )
            {
                yw_arg177 arg177;
                arg177.field_4 = bact->killer_owner;
                arg177.bact = bact;

                robo->wrld->ypaworld_func177(&arg177); //Reown sectors for new owner
            }
        }

        bact_node *node = (bact_node *)bact->subjects_list.head;

        while (node->next)
        {
            bact_node *subnode = (bact_node *)node->bact->subjects_list.head;

            while(subnode->next)
            {
                bact_node *next2 = (bact_node *)subnode->next;

                subnode->bact->killer = bact->killer;

                if ( bact->status_flg & BACT_STFLAG_CLEAN )
                    subnode->bact->status_flg |= BACT_STFLAG_CLEAN;

                subnode->bacto->Die();

                subnode->bact->status_flg &= ~BACT_STFLAG_LAND;

                setState_msg arg119;
                arg119.unsetFlags = 0;
                arg119.setFlags = 0;
                arg119.newStatus = BACT_STATUS_CREATE;
                subnode->bacto->SetStateInternal(&arg119);

                subnode->bact->status = BACT_STATUS_DEAD;

                sub_424000(&subnode->bact->soundcarrier, 3);
                startSound(&subnode->bact->soundcarrier, 7);

                subnode->bact->soundFlags &= 0xFFFFFFF7;
                subnode->bact->soundFlags |= 0x80;

                subnode->bacto->setBACT_yourLastSeconds(a4);

                subnode = next2;
            }


            bact_node *next1 = (bact_node *)node->next;

            node->bact->killer = bact->killer;

            if ( bact->status_flg & BACT_STFLAG_CLEAN )
                node->bact->status_flg |= BACT_STFLAG_CLEAN;

            node->bacto->Die();

            node->bact->status_flg &= ~BACT_STFLAG_LAND;

            setState_msg arg119;
            arg119.unsetFlags = 0;
            arg119.setFlags = 0;
            arg119.newStatus = BACT_STATUS_CREATE;
            node->bacto->SetStateInternal(&arg119);

            node->bact->status = BACT_STATUS_DEAD;

            sub_424000(&node->bact->soundcarrier, 3);
            startSound(&node->bact->soundcarrier, 7);

            node->bact->soundFlags &= 0xFFFFFFF7;
            node->bact->soundFlags |= 0x80;

            node->bacto->setBACT_yourLastSeconds(a4);

            node = next1;
        }

        NC_STACK_ypabact *v20 = robo->wrld->getYW_userHostStation();

        if ( this == v20 )
        {
            if ( !(bact->status_flg & BACT_STFLAG_CLEAN) )
            {
                robo_arg134 arg134;
                arg134.field_4 = 12;
                arg134.field_8 = 0;
                arg134.field_10 = 0;
                arg134.field_C = 0;
                arg134.unit = bact;
                arg134.field_14 = 100;

                placeMessage(&arg134);
            }
        }
        else
        {
            robo_arg134 arg134;
            arg134.field_4 = 11;
            arg134.field_8 = bact->owner;
            arg134.field_10 = 0;
            arg134.field_C = 0;
            arg134.unit = bact->killer;
            arg134.field_14 = 50;

            placeMessage(&arg134);
        }

        NC_STACK_ypabact::Die();

        bact->vp_extra[1].flags = 0;

        if ( yw->isNetGame )
        {
            if ( bact->owner )
            {
                yw_arg181 arg181;
                arg181.data = &hdMsg;
                arg181.dataSize = sizeof(hdMsg);
                arg181.recvFlags = 2;
                arg181.recvID = 0;
                arg181.garant = 1;
                yw->self_full->ypaworld_func181(&arg181);

                if ( this == yw->field_1b78 )
                {
                    yw->field_1b7c->setBACT_inputting(0);
                    yw->field_1b7c->setBACT_viewer(0);

                    setBACT_inputting(1);
                    setBACT_viewer(1);
                }
            }
        }
    }
}

size_t NC_STACK_yparobo::SetPosition(bact_arg80 *arg)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int v5 = NC_STACK_ypabact::SetPosition(arg);
    if ( !v5 )
        return 0;

    robo->field_c = bact->mass * 9.80665;

    for (int i = 0; i < 8; i++)
    {
        roboGun &gun = robo->guns[i];

        if (gun.gun_obj)
        {
            bact_arg80 v11;
            v11.pos = bact->position + bact->rotation.Transpose().Transform(gun.pos);
            v11.field_C = 4;

            gun.gun_obj->SetPosition(&v11);
        }
    }

    robo->field_1D5 = bact->position.y;

    return 1;
}

// Update robo energys
void NC_STACK_yparobo::EnergyInteract(update_msg *arg)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = robo->bact_internal;

    float v65 = bact->reload_const;
    float v63 = (bact->clock - bact->energy_time) / 1000.0;

    if ( bact->status != BACT_STATUS_DEAD && v63 >= 0.25 )
    {
        int v62 = bact->energy;

        int v61 = robo->field_4F5;
        int v59 = robo->field_4FD;

        bact->energy_time = bact->clock;

        yw_arg176 arg176;
        arg176.owner = bact->pSector->owner;

        robo->wrld->ypaworld_func176(&arg176);

        float v64 = bact->pSector->energy_power;

        float v70 = v65 * v63 * v64 * arg176.field_4;

        if ( robo->roboState & ROBOSTATE_PLAYERROBO )
        {
            int v67 = 0;

            float v68 = v70 / 6000.0;

            if ( bact->pSector->owner )
            {
                if ( bact->pSector->owner != bact->owner )
                    v68 = -v68;
            }
            else
            {
                v68 = 0.0;
            }

            if ( robo->field_501 & 1 || v68 < 0.0 )
                v67++;

            if ( robo->field_501 & 4 )
                v67++;

            if ( robo->field_501 & 8 )
                v67++;

            robo->field_505 = v65 * v64 / 6000.0;

            if ( v67 > 0 )
            {
                int v25 = 0;
                int v26 = 0;
                int v66 = 0;

                v68 = v68 / (float)v67;

                if ( robo->field_501 & 1 || v68 < 0.0 )
                    bact->energy += v68;

                if ( robo->field_501 & 4 )
                    robo->field_4F5 += v68;

                if ( robo->field_501 & 8 )
                    robo->field_4FD += v68;

                float v60 = v63 * v65 / 30.0;

                if ( robo->field_501 & 1 )
                    v25 = bact->energy;

                if ( robo->field_501 & 4 )
                    v25 += robo->field_4F5;

                if ( robo->field_501 & 8 )
                    v25 += robo->field_4FD;

                int v34 = v25 / v67;

                if ( robo->field_501 & 1 )
                {
                    if ( v34 > bact->energy )
                        v26++;
                    else if ( v34 < bact->energy )
                        v66++;
                }

                if ( robo->field_501 & 4 )
                {
                    if ( v34 > robo->field_4F5 )
                        v26++;
                    else if ( v34 < robo->field_4F5 )
                        v66++;
                }

                if ( robo->field_501 & 8 )
                {
                    if ( v34 > robo->field_4FD )
                        v26++;
                    else if ( v34 < robo->field_4FD )
                        v66++;
                }

                if ( v26 )
                    v26 = (int)v60 / v26;

                if ( v66 )
                    v66 = (int)v60 / v66;

                if ( robo->field_501 & 1 )
                {
                    if ( bact->energy <= v34 )
                    {
                        bact->energy += v26;

                        if ( bact->energy >= v34 )
                            bact->energy = v34;
                    }
                    else
                    {
                        bact->energy -= v66;
                        if ( bact->energy <= v34 )
                            bact->energy = v34;
                    }
                }

                if ( robo->field_501 & 4 )
                {
                    if ( robo->field_4F5 <= v34 )
                    {
                        robo->field_4F5 += v26;
                        if ( robo->field_4F5 >= v34)
                            robo->field_4F5 = v34;
                    }
                    else
                    {
                        robo->field_4F5 -= v66;
                        if ( robo->field_4F5 <= v34 )
                            robo->field_4F5 = v34;
                    }
                }

                if ( robo->field_501 & 8 )
                {
                    if ( v34 >= robo->field_4FD )
                    {
                        robo->field_4FD += v26;
                        if ( robo->field_4FD >= v34)
                            robo->field_4FD = v34;
                    }
                    else
                    {
                        robo->field_4FD -= v66;
                        if ( robo->field_4FD <= v34 )
                            robo->field_4FD = v34;
                    }
                }

            }
        }
        else
        {
            robo->field_505 = v65 * v64 / 7000.0;
            float v71 = v70 / 7000.0;
            if ( bact->owner == bact->pSector->owner )
            {
                robo->field_509 += v71 * 0.15;
                robo->field_50D += v71 * 0.7;
                bact->energy += v71 * 0.7;
            }
            else
            {
                bact->energy -= v71;
            }
        }

        if ( bact->energy < 0 )
            bact->energy = 0;

        if ( robo->field_4F5 < 0 )
            robo->field_4F5 = 0;

        if ( robo->field_4FD < 0 )
            robo->field_4FD = 0;

        if ( robo->field_509 < 0 )
            robo->field_509 = 0;

        if ( robo->field_50D < 0 )
            robo->field_50D = 0;

        if ( bact->energy_max < bact->energy )
            bact->energy = bact->energy_max;

        if ( bact->energy_max < robo->field_4F5 )
            robo->field_4F5 = bact->energy_max;

        if ( bact->energy_max < robo->field_4FD )
            robo->field_4FD = bact->energy_max;

        if ( bact->energy_max < robo->field_509 )
            robo->field_509 = bact->energy_max;

        if ( bact->energy_max < robo->field_50D )
            robo->field_50D = bact->energy_max;

        robo->field_503 = 0;
        robo->field_502 = 0;

        if ( v62 < bact->energy )
            robo->field_502 |= 1;
        else if ( v62 > bact->energy )
            robo->field_503 |= 1;

        if ( v61 < robo->field_4F5 )
            robo->field_502 |= 4;
        else if ( v61 > robo->field_4F5 )
            robo->field_503 |= 4;

        if ( v59 < robo->field_4FD )
            robo->field_502 |= 8;
        else if ( v59 > robo->field_4FD )
            robo->field_503 |= 8;
    }
}

void NC_STACK_yparobo::Renew()
{
    __NC_STACK_yparobo *robo = &stack__yparobo;

    NC_STACK_ypabact::Renew();

    robo->radar_value = 0;
    robo->field_269 = 0;
    robo->field_251 = 0;
    robo->safety_value = 0;
    robo->conq_value = 0;
    robo->field_29D = 0;
    robo->field_2CD = 0;
    robo->field_2B5 = 0;
    robo->field_25D = 0;
    robo->radar_cellIDpos = 0;
    robo->safety_cellIDpos = 0;
    robo->conq_cellIDpos = 0;
    robo->radar_delay = 0;
    robo->field_281 = 0;
    robo->field_265 = 0;
    robo->safety_delay = 0;
    robo->conq_delay = 0;
    robo->field_2E1 = 0;
    robo->field_2C9 = 0;
    robo->field_2B1 = 0;
    robo->field_509 = 0;

    yw_130arg arg130;
    arg130.pos_x = 600.0;
    arg130.pos_z = -600.0;

    robo->wrld->ypaworld_func130(&arg130);

    robo->dock_energ = 0;
    robo->dock_cnt = 0;
    robo->dock_time = 0;
    robo->dock_tgType = BACT_TGT_TYPE_NONE;
    robo->dock_aggr = 0;
    robo->dock_tgt_comm_id = 0;
    robo->dock_tgt_pos.x = 0;
    robo->dock_tgt_pos.y = 0;
    robo->dock_tgt_pos.z = 0;
    robo->dock_tgt_bact = NULL;
    robo->field_309 = 0;
    robo->field_511 = 0;
    robo->field_5A9 = 0;
    robo->fe_time = 0;
    robo->roboState = 0;

    robo->pcell = arg130.pcell;

    robo->dock_time = 0;

    memset(robo->guns, 0, sizeof(robo->guns));
    memset(robo->field_525, 0, sizeof(robo->field_525));

    robo->bact_internal->commandID = dword_5B1128;

    dword_5B1128++;

    setBACT_yourLastSeconds(3000);

    get_keyvalue_from_ini(NULL, yparobo_keys, 2);

    robo->newAI = yparobo_keys[0].value.val;
    robo->field_1E3 = yparobo_keys[1].value.val;
}

void NC_STACK_yparobo::HandBrake(update_msg *)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = &ypabact;

    bact->fly_dir_length *= 0.8;
    bact->thraction = 0;
    robo->field_c = bact->mass * 9.80665;
}

void NC_STACK_yparobo::CorrectPositionOnLand(void *arg)
{
}

void NC_STACK_yparobo::DeadTimeUpdate(update_msg *arg)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = robo->bact_internal;
    _NC_STACK_ypaworld *yw = robo->wrld_yw;

    int a4 = getBACT_yourLastSeconds();

    if ( !(bact->status_flg & BACT_STFLAG_DEATH2) )
    {
        setState_msg arg78;
        arg78.newStatus = BACT_STATUS_NOPE;
        arg78.unsetFlags = 0;
        arg78.setFlags = BACT_STFLAG_DEATH2;
        SetState(&arg78);
    }

    bact->status_flg |= BACT_STFLAG_LAND;

    if ( !bact->owner || !bact->vp_genesis.base )
    {
        if ( a4 > 0 )
            return;

        int v31 = getBACT_inputting();

        if ( v31 )
            bact->status_flg |= BACT_STFLAG_NORENDER;
        else
            Release(this);

        return;
    }

    int v33 = (float)bact->energy_max * 0.7;
    if ( v33 < 10000 )
        v33 = 10000;
    if ( v33 > 25000 )
        v33 = 25000;

    if ( bact->vp_extra[0].flags & EVPROTO_FLAG_ACTIVE )
    {
        bact->scale_time -= arg->frameTime;

        if ( bact->scale_time <= 0 )
        {
            bact->vp_extra[0].vp.base = NULL;
            bact->vp_extra[0].vp.trigo = NULL;

            if ( a4 > 0 )
                return;

            int v30 = getBACT_inputting();

            if ( v30 )
                bact->status_flg |= BACT_STFLAG_NORENDER;
            else
                Release(this);

            return;
        }

        bact->vp_extra[0].scale = sqrt((float)bact->scale_time / (float)v33) * 0.75;

        if ( bact->vp_extra[0].scale < 0.0 )
            bact->vp_extra[0].scale = 0;

        float v16 = bact->maxrot * 2.0 * ((float)arg->frameTime / 1000.0);

        bact->vp_extra[0].rotate = mat3x3::RotateY(v16) * bact->vp_extra[0].rotate;

        if ( a4 <= 0 )
            bact->status_flg |= BACT_STFLAG_NORENDER;
    }
    else
    {
        bact->scale_time = v33;
        bact->vp_extra[0].scale = 0.75;
        bact->vp_extra[0].pos = bact->position;
        bact->vp_extra[0].rotate = bact->rotation;
        bact->vp_extra[0].vp = bact->vp_genesis;
        bact->vp_extra[0].flags |= (EVPROTO_FLAG_ACTIVE | EVPROTO_FLAG_SCALE);

        if ( yw->isNetGame )
        {
            uamessage_startPlasma sbMsg;
            sbMsg.msgID = UAMSG_STARTPLASMA;
            sbMsg.owner = bact->owner;
            sbMsg.scale = 0.75;
            sbMsg.time = v33;
            sbMsg.id = bact->gid;
            sbMsg.pos = bact->position;
            sbMsg.dir = bact->rotation;

            yw_arg181 arg181;
            arg181.data = &sbMsg;
            arg181.garant = 1;
            arg181.recvID = 0;
            arg181.recvFlags = 2;
            arg181.dataSize = sizeof(sbMsg);
            yw->self_full->ypaworld_func181(&arg181);
        }
    }
}

void NC_STACK_yparobo::yparobo_func128(robo_arg128 *arg)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = robo->bact_internal;

    arg->comm_bacto = NULL;
    arg->comm_bact = NULL;

    bact_node *node = (bact_node *)bact->subjects_list.head;

    while ( node->next )
    {
        if ( !(node->bact->status_flg & BACT_STFLAG_DEATH1) )
        {
            int v7;

            if ( arg->flags & 1 )
            {
                if ( arg->comm_id == node->bact->commandID )
                {
                    arg->comm_bact = node->bact;
                    arg->comm_bacto = node->bacto;
                    v7 = 1;
                }
                else
                {
                    v7 = 0;
                }
            }
            else
            {
                v7 = 1;
            }

            int v21;
            if ( arg->flags & 2 )
            {
                if ( node->bact->primTtype == arg->tgType || robo->dock_tgType == arg->tgType )
                {
                    int v28 = arg->tgt_pos.x / 1200.0;
                    int v29 = (-arg->tgt_pos.z) / 1200.0;

                    int v30, v31, v16;
                    if ( node->bact->commandID == robo->dock_user && robo->dock_user )
                    {
                        v30 = robo->dock_tgt_pos.x / 1200.0;
                        v31 = (-robo->dock_tgt_pos.z) / 1200.0;
                        v16 = robo->dock_tgt_comm_id;
                    }
                    else
                    {
                        v30 = node->bact->primTpos.x / 1200.0;
                        v31 = (-node->bact->primTpos.z) / 1200.0;
                        v16 = node->bact->primT_cmdID;
                    }

                    if ( (arg->tgType == BACT_TGT_TYPE_UNIT && !arg->prim_comm_id && arg->tgt.pbact == node->bact->primT.pbact)
                            || (arg->tgType == BACT_TGT_TYPE_UNIT && arg->prim_comm_id != 0 && v16 == arg->prim_comm_id)
                            || (arg->tgType == BACT_TGT_TYPE_CELL && v30 == v28 && v31 == v29) )
                    {
                        arg->comm_bact = node->bact;
                        arg->comm_bacto = node->bacto;
                    }
                }
                v21 = 0;
            }
            else
            {
                v21 = 1;
            }

            int v26;
            if ( !(arg->flags & 4) )
            {
                v26 = 1;
            }
            else
            {
                if ( node->bact->primTtype == BACT_TGT_TYPE_CELL )
                {
                    float tx, ty;

                    if ( node->bact->commandID == robo->dock_user && robo->dock_user )
                    {
                        tx = robo->dock_tgt_pos.x;
                        ty = robo->dock_tgt_pos.z;
                    }
                    else
                    {
                        tx = node->bact->primTpos.x;
                        ty = node->bact->primTpos.z;
                    }
                    if ( POW2(arg->distance) > POW2(tx - arg->tgt_pos.x) + POW2(ty - arg->tgt_pos.z) )
                    {
                        arg->comm_bact = node->bact;
                        arg->comm_bacto = node->bacto;

                        v26 = 1;
                    }
                    else
                    {
                        v26 = 0;
                    }
                }
                else
                {
                    v26 = 0;
                }
            }

            if ( v7 && v21 && v26 )
                break;
        }

        node = (bact_node *)node->next;
    }
}

void NC_STACK_yparobo::yparobo_func129(IDVPair *arg)
{
    dprintf("MAKE ME %s\n","yparobo_func129");
//	call_parent(zis, obj, 129, arg);
}

void NC_STACK_yparobo::yparobo_func130(IDVPair *arg)
{
    dprintf("MAKE ME %s\n","yparobo_func130");
//	call_parent(zis, obj, 130, arg);
}

void NC_STACK_yparobo::yparobo_func131(IDVPair *arg)
{
    dprintf("MAKE ME %s\n","yparobo_func131");
//	call_parent(zis, obj, 131, arg);
}

size_t NC_STACK_yparobo::yparobo_func132(setTarget_msg *arg)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;
    __NC_STACK_ypabact *bact = robo->bact_internal;

    if ( !arg->priority )
    {
        arg->tgt_type = BACT_TGT_TYPE_NONE;
        return 0;
    }


    bact_node *node = &bact->subject_node;

    while (node->prev)
        node = (bact_node *)node->prev;

    node = (bact_node *)node->next;

    while (node->next)
    {
        if (arg->priority == node->bact->commandID)
        {
            if ( node->bact->status != BACT_STATUS_DEAD )
            {
                arg->priority = 0;
                arg->tgt_type = BACT_TGT_TYPE_UNIT;
                arg->tgt.pbact = node->bact;
                return 1;
            }
            else
            {
                arg->priority = 0;
                arg->tgt_type = BACT_TGT_TYPE_NONE;
                return 0;
            }
        }

        bact_node *subnode = (bact_node *)node->bact->subjects_list.head;

        while (subnode->next)
        {
            if (arg->priority == subnode->bact->commandID)
            {
                if ( subnode->bact->status != BACT_STATUS_DEAD )
                {
                    arg->priority = 0;
                    arg->tgt_type = BACT_TGT_TYPE_UNIT;
                    arg->tgt.pbact = subnode->bact;
                    return 1;
                }
                else
                {
                    arg->priority = 0;
                    arg->tgt_type = BACT_TGT_TYPE_NONE;
                    return 0;
                }
            }

            subnode = (bact_node *)subnode->next;
        }

        node = (bact_node *)node->next;
    }

    arg->tgt_type = BACT_TGT_TYPE_NONE;
    arg->priority = 0;
    return 0;
}

// Create squad for robo
int NC_STACK_yparobo::yparobo_func133(robo_arg133 *arg)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;

    int curid = 0;
    int col = sqrt(arg->num) + 2;

    ypaworld_arg146 arg146;
    arg146.pos.y = arg->pos.y;
    arg146.pos.x = 100 * (curid % col - col / 2) + arg->pos.x;
    arg146.pos.z = 100 * (curid / col) + arg->pos.z;

    if ( arg->hetero_vehicles )
        arg146.vehicle_id = arg->hetero_vehicles[0];
    else
        arg146.vehicle_id = arg->type;

    NC_STACK_ypabact *squad_commander = robo->wrld->ypaworld_func146(&arg146); //Create first bact

    if ( !squad_commander )
        return 0;

    __NC_STACK_ypabact *bact_intern = squad_commander->getBACT_pBact();

    int v32 = getBACT_bactCollisions();
    squad_commander->setBACT_bactCollisions(v32);

    if ( arg->field_14 & 1 )
        bact_intern->status_flg |= BACT_STFLAG_UNUSE;

    bact_intern->owner = robo->bact_internal->owner;
    bact_intern->commandID = dword_5B1128;
    bact_intern->host_station = this;
    bact_intern->aggr = 60;

    setState_msg arg78;
    arg78.newStatus = BACT_STATUS_NORMAL;
    arg78.setFlags = 0;
    arg78.unsetFlags = 0;

    squad_commander->SetState(&arg78);

    setTarget_msg arg67;
    arg67.tgt_type = BACT_TGT_TYPE_CELL;
    arg67.priority = 0;
    arg67.tgt_pos.x = arg146.pos.x;
    arg67.tgt_pos.z = arg146.pos.z;

    squad_commander->SetTarget(&arg67); //Set target

    for ( curid = 1; curid < arg->num; curid++)
    {
        if ( arg->hetero_vehicles ) // there must be a joke about gay and straight
            arg146.vehicle_id = arg->hetero_vehicles[arg->num - curid];
        else
            arg146.vehicle_id = arg->type;

        arg146.pos.x = 100 * (curid % col - col / 2) + arg->pos.x;
        arg146.pos.z = 100 * (curid / col) + arg->pos.z;

        NC_STACK_ypabact *next_bact = (NC_STACK_ypabact *)robo->wrld->ypaworld_func146(&arg146);

        if ( !next_bact )
            break;

        squad_commander->AddSubject(next_bact); // Add to squad commander list

        arg67.tgt_pos.x = arg146.pos.x;
        arg67.tgt_pos.z = arg146.pos.z;
        next_bact->SetTarget(&arg67); //Set target

        bact_intern = next_bact->getBACT_pBact();

        next_bact->setBACT_bactCollisions(v32);

        if ( arg->field_14 & 1 )
            bact_intern->status_flg |= BACT_STFLAG_UNUSE;

        bact_intern->owner = robo->bact_internal->owner;
        bact_intern->commandID = dword_5B1128;
        bact_intern->host_station = this;
        bact_intern->aggr = 60;

        next_bact->SetState(&arg78);
    }

    dword_5B1128++;

    AddSubject(squad_commander);  //Add squad commander into robo list

    return 1;
}


const char * yparobo_func134__sub0(char **strings, int a2)
{
    const char *result;

    switch ( a2 )
    {
    case 2:
        result = get_lang_string(strings, 202, "SECTOR CONQUERED");
        break;

    case 6:
        result = get_lang_string(strings, 206, "FOUND ENEMY STATION");
        break;

    case 11:
        result = get_lang_string(strings, 211, "STATION IS DEAD ");
        break;

    case 12:
        result = get_lang_string(strings, 212, "YOUR STATION IS DEAD!");
        break;

    case 13:
        result = get_lang_string(strings, 213, "ENEMY NEAR STATION!");
        break;

    case 18:
        result = get_lang_string(strings, 218, "REQUEST SUPPORT");
        break;

    case 31:
        result = get_lang_string(strings, 231, "FLAK DESTROYED");
        break;

    case 32:
        result = get_lang_string(strings, 232, "RADAR STATION DESTROYED");
        break;

    case 33:
        result = get_lang_string(strings, 233, "ATTACK POWERSTATION");
        break;

    case 34:
        result = get_lang_string(strings, 234, "ATTACK FLAK");
        break;

    case 35:
        result = get_lang_string(strings, 235, "ATTACK RADAR STATION");
        break;

    case 40:
        result = get_lang_string(strings, 240, "HOST ENERGY CRITICAL");
        break;

    default:
        result = NULL;
        break;
    }
    return result;
}

int yparobo_func134__sub1(__NC_STACK_yparobo *robo, robo_arg134 *arg)
{
    int v2 = 1;

    __NC_STACK_ypabact *bact = robo->bact_internal;

    switch(arg->field_4)
    {
    case 1:
    {
        NC_STACK_ypabact *bacto = robo->wrld->getYW_userVehicle();
        __NC_STACK_ypabact *v11 = bacto->getBACT_pBact();

        if ( v11->bact_type != BACT_TYPES_MISSLE )
            v2 = 1;
        else
            v2 = 0;
    }
    break;

    case 2:
    case 3:
    case 4:
        if ( arg->field_8 == stru_5B0628[ arg->field_4 ].field_0 && arg->field_C == stru_5B0628[ arg->field_4 ].field_4 )
            v2 = 0;
        else
            v2 = 1;
        break;

    case 6:
        v2 = (robo->bact_internal->clock - dword_515138[ arg->field_8 ] > 45000) || dword_515138[ arg->field_8 ] == 0;
        dword_515138[ arg->field_8 ] = robo->bact_internal->clock;
        break;

    case 7:
        v2 = 1;
        if ( arg->unit && arg->unit->bact_type == BACT_TYPES_GUN )
        {
            NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( arg->unit->self );

            if ( gun->getGUN_roboGun() )
                v2 = 0;
        }

        break;

    case 9:
    case 10:
    case 13:
        if ( arg->field_8 == stru_5B0628[arg->field_4].field_0 )
            v2 = 0;
        else
            v2 = 1;
        break;

    case 5:
    case 8:
    case 11:
    case 12:
    case 14:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
        v2 = 1;
        break;

    case 18:
    case 40:
        if ( (bact->clock - stru_5B0628[ arg->field_4 ].field_10) > arg->field_8 )
        {
            stru_5B0628[ arg->field_4 ].field_10 = bact->clock;
            v2 = 1;
        }
        else
            v2 = 0;

        break;

    default:
        break;
    }

    if ( v2 )
    {
        stru_5B0628[arg->field_4].field_0 = arg->field_8;
        stru_5B0628[arg->field_4].field_4 = arg->field_C;
        stru_5B0628[arg->field_4].field_8 = arg->field_10;
    }

    return v2;
}

int NC_STACK_yparobo::placeMessage(robo_arg134 *arg)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;

    if ( arg->unit && arg->unit->bact_type == BACT_TYPES_GUN && arg->field_4 != 7 && arg->field_4 != 19 && arg->field_4 != 6 )
        return 0;

    char **strngs = robo->wrld->getYW_localeStrings();

    if ( !yparobo_func134__sub1(robo, arg) )
        return 0;

    yw_arg159 v8;
    v8.field_4 = arg->field_14;
    v8.txt = yparobo_func134__sub0(strngs, arg->field_4);
    v8.unit = arg->unit;
    v8.field_C = arg->field_4;
    robo->wrld->ypaworld_func159(&v8);

    return 1;
}


void ypabact_func65__sub0(__NC_STACK_ypabact *bact) // This bact is robo!
{
    _NC_STACK_ypaworld *yw = &bact->ywo->stack__ypaworld;

    if ( bact->beam_time )
    {
        if ( bact->clock - bact->beam_time > 2000 )
        {
            int i = 0;

            __NC_STACK_ypabact *v1 = (__NC_STACK_ypabact *)bact->pSector->units_list.head;

            while ( v1->next )
            {
                int a4 = v1->self->getBACT_viewer();

                if ( v1->status == BACT_STATUS_BEAM && v1->owner == bact->owner && !a4 )
                    i = 1;

                v1 = (__NC_STACK_ypabact *)v1->next;
            }

            if ( !i )
            {
                NC_STACK_yparobo *robj = dynamic_cast<NC_STACK_yparobo *>(bact->self);
                __NC_STACK_yparobo *robo = &robj->stack__yparobo;

                __NC_STACK_ypabact *tmp = bact;

                bact->ywo->ypaworld_func168(&tmp);

                vec3d tt = bact->position;

                bact->position = bact->old_pos;
                robo->field_1D5 = bact->old_pos.y;

                char a1a[200];
                sprintf(a1a, "save:%s/%d.fin", yw->GameShell->user_name, yw->field_2d90->levelID);

                yw_arg169 v23;
                v23.usr = yw->GameShell;
                v23.saveFile = a1a;

                if ( !bact->ywo->ypaworld_func170(&v23) )
                    ypa_log_out("Warning, final sgm save error\n");

                bact->position = tt;

                if ( yw->GameShell )
                {
                    char v18[300];
                    sprintf(v18, "save:%s/%d.rst", yw->GameShell->user_name, yw->field_2d90->levelID);

                    uaDeleteFile(v18);
                }
                bact->status_flg |= BACT_STFLAG_CLEAN;

                bact->self->Die();
            }

        }
    }
    else
    {

        bact->position.x = (bact->sectX + 0.5) * 1200.0;
        bact->position.z = -(bact->sectY + 0.5) * 1200.0;

        bact_node *unit = (bact_node *)bact->subjects_list.head;

        while ( unit->next )
        {
            unit->bact->status_flg |= BACT_STFLAG_CLEAN;

            bact_node *sub_unit = (bact_node *)unit->bact->subjects_list.head;

            while ( sub_unit->next )
            {
                sub_unit->bact->status_flg |= BACT_STFLAG_CLEAN;

                sub_unit = (bact_node *)sub_unit->next;
            }

            unit = (bact_node *)unit->next;
        }

        __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)bact->pSector->units_list.head;


        yw->field_2d90->buddies_count = 0;
        memset(yw->field_2d90->buddies, 0, sizeof(yw->field_2d90->buddies));

        int v15 = 0;

        while ( bct->next )
        {
            if  ( bct->status != BACT_STATUS_DEAD )
            {
                if ( bct->bact_type != BACT_TYPES_ROBO && bct->bact_type != BACT_TYPES_MISSLE && bct->bact_type != BACT_TYPES_GUN && bct->owner == bact->owner )
                {
                    float v17 = fabs(bct->position.x - (bact->sectX + 0.5) * 1200.0);
                    float v29 = fabs(bct->position.z + (bact->sectY + 0.5) * 1200.0);

                    if ( v17 <= 450.0 || v29 <= 450.0 )
                    {
                        bct->status = BACT_STATUS_BEAM;

                        bct->scale_time = 0;

                        bct->scale_delay = v15 + 1000;
                        v15 += 400;
                    }
                }
            }

            bct = (__NC_STACK_ypabact *)bct->next;
        }

        bact->beam_time = bact->clock;
    }
}



void NC_STACK_yparobo::setBACT_inputting(int inpt)
{
    NC_STACK_ypabact::setBACT_inputting(inpt);

    if ( inpt )
    {
        stack__yparobo.wrld->setYW_userHostStation(this);

        stack__yparobo.roboState |= ROBOSTATE_PLAYERROBO;
    }
}

void NC_STACK_yparobo::setROBO_proto(roboProto *proto)
{
    __NC_STACK_yparobo *robo = &stack__yparobo;

    for (int i = 0; i < 8; i++)
    {
        if ( robo->guns[i].gun_obj )
        {
            __NC_STACK_ypabact *gun_bact = robo->guns[i].gun_obj->getBACT_pBact();

            if ( !( gun_bact->status_flg & BACT_STFLAG_DEATH1 ) )
                robo->guns[i].gun_obj->Die();

            Release(robo->guns[i].gun_obj);

            robo->guns[i].gun_obj = NULL;
        }
    }

    __NC_STACK_ypabact *bact = robo->bact_internal;

    for (int i = 0; i < proto->robo_num_guns; i++)
    {
        robo->guns[i] = proto->guns[i];

        ypaworld_arg146 gun_req;

        gun_req.pos = bact->position + bact->rotation.Transpose().Transform(robo->guns[i].pos);

        gun_req.vehicle_id = robo->guns[i].robo_gun_type;

        NC_STACK_ypabact *gun_obj = robo->wrld->ypaworld_func146(&gun_req);
        NC_STACK_ypagun *gungun = dynamic_cast<NC_STACK_ypagun *>(gun_obj);

        robo->guns[i].gun_obj = gun_obj;

        if ( gun_obj )
        {
            if (gungun)
            {
                gungun->ypagun_func128(robo->guns[i].dir, false);

                gungun->setGUN_roboGun(1);
            }

            __NC_STACK_ypabact *gun_bact = gun_obj->getBACT_pBact();

            gun_bact->owner = bact->owner;
            gun_bact->commandID = dword_5B1128;
            gun_bact->host_station = this;

            dword_5B1128++;

            if ( robo->wrld->stack__ypaworld.isNetGame )
            {
                gun_bact->gid |= gun_bact->owner << 24;
                gun_bact->commandID |= gun_bact->owner << 24;
            }

            gun_bact->aggr = 60;

            AddSubject(gun_obj);
        }
        else
        {
            ypa_log_out("Unable to create Robo-Gun\n");
        }
    }

    robo->dock_pos = proto->dock;
    bact->viewer_position = proto->viewer;
    bact->viewer_max_up = proto->robo_viewer_max_up;
    bact->viewer_max_down = proto->robo_viewer_max_down;
    bact->viewer_max_side = proto->robo_viewer_max_side;

    setBACT_extraViewer(1);
    setBACT_alwaysRender(1);

    robo->coll = proto->coll;
}

void NC_STACK_yparobo::setROBO_epConquer(int ep)
{
    stack__yparobo.epConquer = ep;
}

void NC_STACK_yparobo::setROBO_epDefense(int ep)
{
    stack__yparobo.epDefense = ep;
}

void NC_STACK_yparobo::setROBO_epRadar(int ep)
{
    stack__yparobo.epRadar = ep;
}

void NC_STACK_yparobo::setROBO_epPower(int ep)
{
    stack__yparobo.epPower = ep;
}

void NC_STACK_yparobo::setROBO_epSafety(int ep)
{
    stack__yparobo.epSafety = ep;
}

void NC_STACK_yparobo::setROBO_commCount(int comm)
{
    dword_5B1128 = comm;
}

void NC_STACK_yparobo::setROBO_battVehicle(int batt)
{
    stack__yparobo.field_4F5 = batt;
}

void NC_STACK_yparobo::setROBO_battBuilding(int batt)
{
    stack__yparobo.field_4F9 = batt;
}

void NC_STACK_yparobo::setROBO_battBeam(int batt)
{
    stack__yparobo.field_4FD = batt;
}

void NC_STACK_yparobo::setROBO_fillMode(int fil)
{
    stack__yparobo.field_501 = fil;
}

void NC_STACK_yparobo::setROBO_waitSway(int wait)
{
    if ( wait )
        stack__yparobo.field_1DA |= 1;
    else
        stack__yparobo.field_1DA &= ~1;
}

void NC_STACK_yparobo::setROBO_waitRotate(int wait)
{
    if ( wait )
        stack__yparobo.field_1DA |= 2;
    else
        stack__yparobo.field_1DA &= ~2;
}

void NC_STACK_yparobo::setROBO_epChangeplace(int ep)
{
    stack__yparobo.epChangePlace = ep;
}

void NC_STACK_yparobo::setROBO_epReconnoitre(int ep)
{
    stack__yparobo.epRecon = ep;
}

void NC_STACK_yparobo::setROBO_epRobo(int ep)
{
    stack__yparobo.epRobo = ep;
}

void NC_STACK_yparobo::setROBO_viewAngle(int angl)
{
    ypabact.viewer_rotation = mat3x3::Ident();
    ypabact.viewer_horiz_angle = angl * C_PI_180;
    ypabact.viewer_rotation = mat3x3::RotateY(ypabact.viewer_horiz_angle) * ypabact.viewer_rotation;
}

void NC_STACK_yparobo::setROBO_safDelay(int delay)
{
    stack__yparobo.safety_delay = delay;
}

void NC_STACK_yparobo::setROBO_powDelay(int delay)
{
    stack__yparobo.field_265 = delay;
}

void NC_STACK_yparobo::setROBO_radDelay(int delay)
{
    stack__yparobo.radar_delay = delay;
}

void NC_STACK_yparobo::setROBO_cplDelay(int delay)
{
    stack__yparobo.field_2B1 = delay;
}

void NC_STACK_yparobo::setROBO_defDelay(int delay)
{
    stack__yparobo.field_281 = delay;
}

void NC_STACK_yparobo::setROBO_conDelay(int delay)
{
    stack__yparobo.conq_delay = delay;
}

void NC_STACK_yparobo::setROBO_robDelay(int delay)
{
    stack__yparobo.field_2E1 = delay;
}

void NC_STACK_yparobo::setROBO_recDelay(int delay)
{
    stack__yparobo.field_2C9 = delay;
}



rbcolls *NC_STACK_yparobo::getBACT_collNodes()
{
    return &stack__yparobo.coll;
}

int NC_STACK_yparobo::getROBO_epConquer()
{
    return stack__yparobo.epConquer;
}

int NC_STACK_yparobo::getROBO_epDefense()
{
    return stack__yparobo.epDefense;
}

int NC_STACK_yparobo::getROBO_epRadar()
{
    return stack__yparobo.epRadar;
}

int NC_STACK_yparobo::getROBO_epPower()
{
    return stack__yparobo.epPower;
}

int NC_STACK_yparobo::getROBO_epSafety()
{
    return stack__yparobo.epSafety;
}

int NC_STACK_yparobo::getROBO_commCount()
{
    return dword_5B1128;
}

int NC_STACK_yparobo::getROBO_battVehicle()
{
    return stack__yparobo.field_4F5;
}

int NC_STACK_yparobo::getROBO_battBuilding()
{
    return stack__yparobo.field_4F9;
}

int NC_STACK_yparobo::getROBO_battBeam()
{
    return stack__yparobo.field_4FD;
}

int NC_STACK_yparobo::getROBO_fillMode()
{
    return stack__yparobo.field_501;
}

roboGun *NC_STACK_yparobo::getROBO_guns()
{
    return stack__yparobo.guns;
}

int NC_STACK_yparobo::getROBO_epChangeplace()
{
    return stack__yparobo.epChangePlace;
}

int NC_STACK_yparobo::getROBO_epReconnoitre()
{
    return stack__yparobo.epRecon;
}

int NC_STACK_yparobo::getROBO_epRobo()
{
    return stack__yparobo.epRobo;
}

int NC_STACK_yparobo::getROBO_roboState()
{
    return stack__yparobo.roboState;
}

int NC_STACK_yparobo::getROBO_safDelay()
{
    return stack__yparobo.safety_delay;
}

int NC_STACK_yparobo::getROBO_powDelay()
{
    return stack__yparobo.field_265;
}

int NC_STACK_yparobo::getROBO_radDelay()
{
    return stack__yparobo.radar_delay;
}

int NC_STACK_yparobo::getROBO_cplDelay()
{
    return stack__yparobo.field_2B1;
}

int NC_STACK_yparobo::getROBO_defDelay()
{
    return stack__yparobo.field_281;
}

int NC_STACK_yparobo::getROBO_conDelay()
{
    return stack__yparobo.conq_delay;
}

int NC_STACK_yparobo::getROBO_robDelay()
{
    return stack__yparobo.field_2E1;
}

int NC_STACK_yparobo::getROBO_recDelay()
{
    return stack__yparobo.field_2C9;
}

int NC_STACK_yparobo::getROBO_loadFlags()
{
    return stack__yparobo.field_502;
}

int NC_STACK_yparobo::getROBO_lossFlags()
{
    return stack__yparobo.field_503;
}

int NC_STACK_yparobo::getROBO_absReload()
{
    return stack__yparobo.field_505;
}



size_t NC_STACK_yparobo::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (IDVList *)data );
    case 1:
        return (size_t)func1();
    case 2:
        func2( (IDVList *)data );
        return 1;
    case 3:
        func3( (IDVList *)data );
        return 1;
    case 68:
        AI_layer1( (update_msg *)data );
        return 1;
    case 70:
        AI_layer3( (update_msg *)data );
        return 1;
    case 71:
        User_layer( (update_msg *)data );
        return 1;
    case 74:
        Move( (move_msg *)data );
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
    case 97:
        HandBrake( (update_msg *)data );
        return 1;
    case 114:
        CorrectPositionOnLand( (void *)data );
        return 1;
    case 121:
        DeadTimeUpdate( (update_msg *)data );
        return 1;
    case 128:
        yparobo_func128( (robo_arg128 *)data );
        return 1;
    case 129:
        yparobo_func129( (IDVPair *)data );
        return 1;
    case 130:
        yparobo_func130( (IDVPair *)data );
        return 1;
    case 131:
        yparobo_func131( (IDVPair *)data );
        return 1;
    case 132:
        return (size_t)yparobo_func132( (setTarget_msg *)data );
    case 133:
        return (size_t)yparobo_func133( (robo_arg133 *)data );
    case 134:
        return (size_t)placeMessage( (robo_arg134 *)data );
    default:
        break;
    }
    return NC_STACK_ypabact::compatcall(method_id, data);
}
