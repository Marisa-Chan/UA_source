#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "log.h"
#include "yw.h"
#include "yparobo.h"

#include "yw_net.h"
#include "fmtlib/printf.h"

#include <math.h>

const Nucleus::ClassDescr NC_STACK_yparobo::description("yparobo.class", &newinstance);


char **dword_54B0E0; // ypaworld strings
int dword_5B1128 = 1;

Common::Ini::KeyList yparobo_keys
{
    Common::Ini::Key("game.newai",    Common::Ini::KT_BOOL, true),
    Common::Ini::Key("game.timeline", Common::Ini::KT_DIGIT, (int32_t)600000)
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


cellArea * NC_STACK_yparobo::yparobo_func0__sub0()
{
    yw_130arg sect_info;

    sect_info.pos_x = 600.0;
    sect_info.pos_z = -600.0;

    _world->ypaworld_func130(&sect_info);

    return sect_info.pcell;
}

size_t NC_STACK_yparobo::func0(IDVList &stak)
{
    if ( !NC_STACK_ypabact::func0(stak) )
        return 0;

    _roboEpConquer = 30;
    _roboEpDefense = 90;
    _roboEpRadar = 30;
    _roboEpPower = 50;
    _roboEpSafety = 50;
    _roboEpRecon = 80;
    _roboEpChangePlace = 100;
    _roboWFlags = 0;
    _roboFillMode = 15;

    for( auto& it : stak )
    {
        IDVPair &val = it.second;

        if ( !val.Skip )
        {
            switch (val.ID)
            {
            case BACT_ATT_INPUTTING:
                setBACT_inputting(val.Get<int32_t>());
                break;

            case ROBO_ATT_EP_CONQUER:
                setROBO_epConquer(val.Get<int32_t>());
                break;

            case ROBO_ATT_EP_DEFENSE:
                setROBO_epDefense(val.Get<int32_t>());
                break;

            case ROBO_ATT_EP_RADAR:
                setROBO_epRadar(val.Get<int32_t>());
                break;

            case ROBO_ATT_EP_POWER:
                setROBO_epPower(val.Get<int32_t>());
                break;

            case ROBO_ATT_EP_SAFETY:
                setROBO_epSafety(val.Get<int32_t>());
                break;

            case ROBO_ATT_BATT_VEHICLE:
                setROBO_battVehicle(val.Get<int32_t>());
                break;

            case ROBO_ATT_BATT_BUILDING:
                setROBO_battBuilding(val.Get<int32_t>());
                break;

            case ROBO_ATT_BATT_BEAM:
                setROBO_battBeam(val.Get<int32_t>());
                break;

            case ROBO_ATT_FILL_MODE:
                setROBO_fillMode(val.Get<int32_t>());
                break;

            case ROBO_ATT_WAIT_SWAY:
                setROBO_waitSway( val.Get<int32_t>() );
                break;

            case ROBO_ATT_WAIT_ROTATE:
                setROBO_waitRotate( val.Get<int32_t>() );
                break;

            case ROBO_ATT_EP_CHANGEPLACE:
                setROBO_epChangeplace(val.Get<int32_t>());
                break;

            case ROBO_ATT_EP_RECONNOITRE:
                setROBO_epReconnoitre(val.Get<int32_t>());
                break;

            case ROBO_ATT_EP_ROBO:
                setROBO_epRobo(val.Get<int32_t>());
                break;

            case ROBO_ATT_VIEWANGLE:
                setROBO_viewAngle(val.Get<int32_t>());
                break;

            case ROBO_ATT_SAFDELAY:
                setROBO_safDelay(val.Get<int32_t>());
                break;

            case ROBO_ATT_POWDELAY:
                setROBO_powDelay(val.Get<int32_t>());
                break;

            case ROBO_ATT_RADDELAY:
                setROBO_radDelay(val.Get<int32_t>());
                break;

            case ROBO_ATT_CPLDELAY:
                setROBO_cplDelay(val.Get<int32_t>());
                break;

            case ROBO_ATT_DEFDELAY:
                setROBO_defDelay(val.Get<int32_t>());
                break;

            case ROBO_ATT_CONDELAY:
                setROBO_conDelay(val.Get<int32_t>());
                break;

            case ROBO_ATT_ROBDELAY:
                setROBO_robDelay(val.Get<int32_t>());
                break;

            case ROBO_ATT_RECDELAY:
                setROBO_recDelay(val.Get<int32_t>());
                break;

            default:
                break;
            }
        }
    }

    _bact_type = BACT_TYPES_ROBO;

    _roboPCells = yparobo_func0__sub0();

    dword_54B0E0 = _world->getYW_localeStrings();

    return 1;
}

size_t NC_STACK_yparobo::func1()
{
    return NC_STACK_ypabact::func1();
}

size_t NC_STACK_yparobo::func2(IDVList &stak)
{
    NC_STACK_ypabact::func2(stak);

    for( auto& it : stak )
    {
        IDVPair &val = it.second;

        if ( !val.Skip )
        {
            switch (val.ID)
            {
            case BACT_ATT_INPUTTING:
                setBACT_inputting(val.Get<int32_t>());
                break;

            case ROBO_ATT_PROTO:
                setROBO_proto(val.Get<roboProto *>());
                break;

            case ROBO_ATT_EP_CONQUER:
                setROBO_epConquer(val.Get<int32_t>());
                break;

            case ROBO_ATT_EP_DEFENSE:
                setROBO_epDefense(val.Get<int32_t>());
                break;

            case ROBO_ATT_EP_RADAR:
                setROBO_epRadar(val.Get<int32_t>());
                break;

            case ROBO_ATT_EP_POWER:
                setROBO_epPower(val.Get<int32_t>());
                break;

            case ROBO_ATT_EP_SAFETY:
                setROBO_epSafety(val.Get<int32_t>());
                break;

            case ROBO_ATT_COMM_COUNT:
                setROBO_commCount(val.Get<int32_t>());
                break;

            case ROBO_ATT_BATT_VEHICLE:
                setROBO_battVehicle(val.Get<int32_t>());
                break;

            case ROBO_ATT_BATT_BUILDING:
                setROBO_battBuilding(val.Get<int32_t>());
                break;

            case ROBO_ATT_BATT_BEAM:
                setROBO_battBeam(val.Get<int32_t>());
                break;

            case ROBO_ATT_FILL_MODE:
                setROBO_fillMode(val.Get<int32_t>());
                break;

            case ROBO_ATT_WAIT_SWAY:
                setROBO_waitSway( val.Get<int32_t>() );
                break;

            case ROBO_ATT_WAIT_ROTATE:
                setROBO_waitRotate( val.Get<int32_t>() );
                break;

            case ROBO_ATT_EP_CHANGEPLACE:
                setROBO_epChangeplace(val.Get<int32_t>());
                break;

            case ROBO_ATT_EP_RECONNOITRE:
                setROBO_epReconnoitre(val.Get<int32_t>());
                break;

            case ROBO_ATT_EP_ROBO:
                setROBO_epRobo(val.Get<int32_t>());
                break;

            case ROBO_ATT_VIEWANGLE:
                setROBO_viewAngle(val.Get<int32_t>());
                break;

            case ROBO_ATT_SAFDELAY:
                setROBO_safDelay(val.Get<int32_t>());
                break;

            case ROBO_ATT_POWDELAY:
                setROBO_powDelay(val.Get<int32_t>());
                break;

            case ROBO_ATT_RADDELAY:
                setROBO_radDelay(val.Get<int32_t>());
                break;

            case ROBO_ATT_CPLDELAY:
                setROBO_cplDelay(val.Get<int32_t>());
                break;

            case ROBO_ATT_DEFDELAY:
                setROBO_defDelay(val.Get<int32_t>());
                break;

            case ROBO_ATT_CONDELAY:
                setROBO_conDelay(val.Get<int32_t>());
                break;

            case ROBO_ATT_ROBDELAY:
                setROBO_robDelay(val.Get<int32_t>());
                break;

            case ROBO_ATT_RECDELAY:
                setROBO_recDelay(val.Get<int32_t>());
                break;

            default:
                break;
            }
        }
    }

    return 1;
}

void NC_STACK_yparobo::AI_layer1(update_msg *arg)
{
    if ( _status == BACT_STATUS_DEAD )
    {
        int a4 = getBACT_yourLastSeconds();
        a4 -= arg->frameTime;
        setBACT_yourLastSeconds(a4);
    }

    _airconst = _airconst_static;

    _soundcarrier.samples_data[0].pitch = _pitch;
    _soundcarrier.samples_data[0].volume = _volume;

    for (int i = 0; i < 8; i++)
    {
        if (_roboGuns[i].gun_obj)
            _roboGuns[i].gun_obj->_owner = _owner;
    }

    EnergyInteract(arg);

    if ( _primTtype == BACT_TGT_TYPE_CELL )
        _target_vec = _primTpos - _position;
    else if ( _primTtype != BACT_TGT_TYPE_UNIT )
        _target_vec = vec3d(0.0, 0.0, 0.0);

    int v9 = getBACT_inputting();

    if ( v9 )
        User_layer(arg);
    else
        AI_layer3(arg);
}


void NC_STACK_yparobo::sub_4A9F24(NC_STACK_ypabact *unit)
{
    if ( _roboState & ROBOSTATE_SETDOCKTARGET )
    {
        setTarget_msg arg67;
        arg67.tgt_type = _roboDockTargetType;
        arg67.priority = 0;
        arg67.tgt.pbact = _roboDockTargetBact;
        arg67.tgt_pos.x = _roboDockTargetPos.x;
        arg67.tgt_pos.z = _roboDockTargetPos.z;

        if ( arg67.tgt_type == BACT_TGT_TYPE_UNIT )
        {
            arg67.priority = _roboDockTargetCommandID;

            if ( yparobo_func132(&arg67) )
            {
                if ( !((1 << _owner) & arg67.tgt.pbact->_pSector->view_mask) )
                {
                    arg67.tgt_type = BACT_TGT_TYPE_CELL;
                    arg67.tgt_pos.x = _roboDockTargetPos.x;
                    arg67.tgt_pos.z = _roboDockTargetPos.z;

                    _roboDockAggr = 50;
                }
            }
            else
            {
                arg67.tgt_type = BACT_TGT_TYPE_CELL;
                arg67.tgt_pos.x = unit->_position.x;
                arg67.tgt_pos.z = unit->_position.z;
            }
            arg67.priority = 0;
        }

        if ( unit->_status != BACT_STATUS_DEAD )
        {
            bact_arg124 arg124;
            arg124.from = unit->_position.XZ();

            if ( arg67.tgt_type == BACT_TGT_TYPE_UNIT )
            {
                arg124.to = arg67.tgt.pbact->_position.XZ();
            }
            else
            {
                arg124.to = arg67.tgt_pos.XZ();
            }

            arg124.field_12 = 1;
            arg124.steps_cnt = 32;

            if ( IsNeedsWaypoints(unit) && unit->PathFinder(&arg124) )
            {
                arg124.steps_cnt = 32;

                unit->SetPath(&arg124);

                if ( arg67.tgt_type == BACT_TGT_TYPE_UNIT )
                {
                    unit->_m_cmdID = arg67.tgt.pbact->_commandID;
                    unit->_m_owner = arg67.tgt.pbact->_owner;
                }
            }
            else
            {
                unit->SetTarget(&arg67);
            }
        }

        _roboState &= ~ROBOSTATE_SETDOCKTARGET;
    }

    unit->setBACT_aggression(_roboDockAggr);
    _roboDockCnt = 0;
    _roboDockUser = 0;
    _roboDockTime = 2000;
    _roboState &= ~ROBOSTATE_USEVHCLPARTS;
}

void NC_STACK_yparobo::InitForce(NC_STACK_ypabact *unit)
{
    if ( _roboDockTime > 0 )
        return;
    
    if ( _roboDockEnerg <= 0 )
    {
        sub_4A9F24(unit);
        return;
    }

    int v6 = _energy - _energy_max * 0.2;

    if ( _roboState & ROBOSTATE_USEVHCLPARTS )
        v6 += _roboVehicleSpare;

    bact_arg81 arg81;
    arg81.enrg_type = 3;
    arg81.enrg_sum = 0;

    GetSummary(&arg81);

    NC_STACK_ypabact *newUnit = NULL;

    int v23 = (arg81.enrg_sum * 0.05 + 1.0) * (float)unit->_energy_max;
    
    if ( v6 > (signed int)v23 )
    {
        ypaworld_arg146 arg146;
        arg146.vehicle_id = unit->_vehicleID;
        arg146.pos = _position + _roboDockPos;

        newUnit = _world->ypaworld_func146(&arg146);
    }

    if ( !newUnit )
    {
        sub_4A9F24(unit);
        return;
    }
    
    newUnit->_status = BACT_STATUS_CREATE;
    newUnit->_owner = _owner;
    newUnit->_commandID = unit->_commandID;
    newUnit->_host_station = this;

    int v20 = getBACT_bactCollisions();
    newUnit->setBACT_bactCollisions(v20);

    unit->AddSubject(newUnit);

    setTarget_msg arg67;
    arg67.tgt_type = BACT_TGT_TYPE_CELL;
    arg67.priority = 0;
    arg67.tgt_pos.x = _position.x + _rotation.m20 * 1200.0 * 0.5;
    arg67.tgt_pos.z = _position.z + _rotation.m22 * 1200.0 * 0.5;
    newUnit->SetTarget(&arg67);

    if ( _world->isNetGame )
    {
        newUnit->_gid |= newUnit->_owner << 24;

        uamessage_newVhcl nvMsg;
        nvMsg.msgID = UAMSG_NEWVHCL;
        nvMsg.owner = newUnit->_owner;
        nvMsg.pos = newUnit->_position;
        nvMsg.type = newUnit->_vehicleID;
        nvMsg.parent = newUnit->_parent->_gid;
        nvMsg.id = newUnit->_gid;
        nvMsg.kind = 2;
        nvMsg.commandID = unit->_commandID;

        yw_arg181 arg181;
        arg181.recvFlags = 2;
        arg181.dataSize = sizeof(nvMsg);
        arg181.recvID = 0;
        arg181.garant = 1;
        arg181.data = &nvMsg;

        _world->ypaworld_func181(&arg181);
    }

    if ( _roboState & ROBOSTATE_USEVHCLPARTS )
    {
        if ( v23 > _roboVehicleSpare )
        {
            _energy -= v23 - _roboVehicleSpare;
            _roboVehicleSpare = 0;
        }
        else
        {
            _roboVehicleSpare -= v23;
        }
    }
    else
    {
        _energy -= v23;
    }

    _roboDockEnerg -= newUnit->_energy;

    setState_msg arg78;
    arg78.setFlags = 0;
    arg78.unsetFlags = 0;
    arg78.newStatus = BACT_STATUS_CREATE;
    newUnit->SetState(&arg78);

    newUnit->_scale_time = newUnit->_energy_max * 0.2;
    _roboDockTime = newUnit->_scale_time + 2000;
    _roboDockCnt++;

    _world->HistoryAktCreate(newUnit);
}


int NC_STACK_yparobo::sub_4A5A08(NC_STACK_ypabact *bact, float a2, float a3)
{
    int v5 = 0;
    if ( bact->_bact_type == BACT_TYPES_TANK || bact->_bact_type == BACT_TYPES_CAR )
    {
        v5 = 1;
    }
    else
    {
        for ( NC_STACK_ypabact* &node : bact->_kidList )
        {
            if (node->_bact_type == BACT_TYPES_TANK || node->_bact_type == BACT_TYPES_CAR)
            {
                v5 = 1;
                break;
            }
        }
    }

    if ( v5 )
    {
        bact_arg124 arg125;
        arg125.steps_cnt = 32;
        arg125.from = bact->_position.XZ();
        arg125.to = vec2d(a2, a3);
        arg125.field_12 = 1;
        if ( !bact->SetPath(&arg125) )
            return 0;
    }
    else
    {
        setTarget_msg arg67;
        arg67.tgt_pos.x = a2;
        arg67.tgt_pos.z = a3;
        arg67.tgt_type = BACT_TGT_TYPE_CELL;
        arg67.priority = 0;
        bact->SetTarget(&arg67);
    }
    return 1;
}

int NC_STACK_yparobo::sub_4A58C0(NC_STACK_ypabact *bact, NC_STACK_ypabact *bact2)
{
    int v5 = 0;
    if ( bact->_bact_type == BACT_TYPES_TANK || bact->_bact_type == BACT_TYPES_CAR )
    {
        v5 = 1;
    }
    else
    {
        for ( NC_STACK_ypabact* &node : bact->_kidList )
        {
            if (node->_bact_type == BACT_TYPES_TANK || node->_bact_type == BACT_TYPES_CAR)
            {
                v5 = 1;
                break;
            }
        }
    }

    if ( v5 )
    {
        bact_arg124 arg124;
        arg124.steps_cnt = 32;
        arg124.from = bact->_position.XZ();
        arg124.to = bact2->_position.XZ();
        arg124.field_12 = 1;

        if ( !bact->PathFinder(&arg124) )
            return 0;

        if ( arg124.steps_cnt > 1 )
        {
            arg124.steps_cnt = 32;
            bact->SetPath(&arg124);

            bact->_m_cmdID = bact2->_commandID;
            bact->_m_owner = bact2->_owner;
            return 1;
        }
    }

    setTarget_msg arg67;
    arg67.tgt.pbact = bact2;
    arg67.priority = 0;
    arg67.tgt_type = BACT_TGT_TYPE_UNIT;
    bact->SetTarget(&arg67);
    return 1;
}

void NC_STACK_yparobo::sb_0x4a7010__sub1__sub0(NC_STACK_ypabact *unit1, NC_STACK_ypabact *unit2)
{
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            int v20 = i + unit1->_sectX;

            if ( v20 > 0 && v20 < unit1->_secMaxX - 1 )
            {
                int v21 = j + unit1->_sectY;

                if ( v21 > 0 && v21 < unit1->_secMaxY - 1 )
                {
                    cellArea *pcell = &unit1->_pSector[i + j * unit1->_secMaxX];

                    if ( pcell->owner == unit1->_owner && pcell->w_type == 2 )
                    {
                        float yy = (v21 + 0.5) * -1200.0;
                        float xx = (v20 + 0.5) * 1200.0;
                        sub_4A5A08(unit2, xx, yy);

                        unit2->setBACT_aggression(25);
                        return;
                    }

                    if ( (1 << unit2->_owner) & pcell->view_mask )
                    {
                        for( NC_STACK_ypabact* &v10 : pcell->unitsList )
                        {
                            if ( v10->_bact_type == BACT_TYPES_GUN && unit1->_owner == v10->_owner && v10->_status != BACT_STATUS_DEAD )
                            {
                                NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( v10 );

                                if ( !gun->IsRoboGun() )
                                {
                                    if ( v10->IsParentMyRobo() )
                                        sub_4A58C0(unit2, v10);
                                    else if (v10->_parent)
                                        sub_4A58C0(unit2, v10->_parent);

                                    unit2->setBACT_aggression(25);
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if ( (1 << unit2->_owner) & unit1->_pSector->view_mask )
    {
        sub_4A58C0(unit2, unit1);

        unit2->setBACT_aggression(25);
    }
    else
    {
        float yy = (unit1->_sectY + 0.5) * -1200.0;
        float xx = (unit1->_sectX + 0.5) * 1200.0;

        sub_4A5A08(unit2, xx, yy);

        unit2->setBACT_aggression(50);
    }
}

void NC_STACK_yparobo::sb_0x4a7010__sub1(NC_STACK_ypabact *unit, robo_t1 *rbt)
{
    NC_STACK_ypabact *foundBact = NULL;
    
    if (unit->_parent->_kidRef)
    {
        for(NC_STACK_ypabact* &nod : *unit->_parent->_kidRef.PList())
        {
            if ( nod->_commandID == rbt->field_4 )
            {
                foundBact = nod;
                break;
            }
        }
    }

    if ( foundBact )
    {
        if ( unit->_primTtype == BACT_TGT_TYPE_UNIT )
        {
            if ( unit->_primT.pbact->_bact_type == BACT_TYPES_ROBO )
                sb_0x4a7010__sub1__sub0(foundBact, unit);
        }
        else if ( unit->_primTtype == BACT_TGT_TYPE_CELL )
        {
            cellArea *cell = unit->_primT.pcell;

            if (cell->owner == unit->_owner || foundBact->_pSector == cell)
                sb_0x4a7010__sub1__sub0(foundBact, unit);
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
    _roboBeamTimePre -= a2;

    if ( _roboBeamTimePre > 0 )
    {
        if ( _roboBeamFXTime <= 0 )
        {
            int v23 = (1500 - _roboBeamTimePre) / 10;
            int v10 = _roboBeamTimePre / 10;

            if ( _vp_extra[0].flags & EVPROTO_FLAG_ACTIVE )
            {
                _roboBeamFXTime = v10;
                _vp_extra[0].flags &= ~EVPROTO_FLAG_ACTIVE;
            }
            else
            {
                _roboBeamFXTime = v23;

                _vp_extra[0].pos = _position;
                _vp_extra[0].rotate = _rotation;
                _vp_extra[0].flags = (EVPROTO_FLAG_ACTIVE | EVPROTO_FLAG_SCALE);
                _vp_extra[0].scale = 1.0;
                _vp_extra[0].vp = _vp_genesis;
            }

            if ( _vp_extra[1].flags & EVPROTO_FLAG_ACTIVE )
            {
                _roboBeamFXTime = v10;
                _vp_extra[1].flags &= ~EVPROTO_FLAG_ACTIVE;
            }
            else
            {
                _roboBeamFXTime = v23;

                _vp_extra[1].pos = _roboBeamPos;
                _vp_extra[1].rotate = _rotation;
                _vp_extra[1].flags = EVPROTO_FLAG_ACTIVE;
                _vp_extra[1].vp = _vp_genesis;
            }
        }

        _roboBeamFXTime -= a2;
    }
    else
    {
        _roboBeamTimePre = 0;

        vec3d tmp = _position;

        tmp.y = _roboYPos;

        bact_arg80 v18;
        v18.pos = _roboBeamPos;
        v18.field_C = 0;

        SetPosition(&v18);

        if ( _world->isNetGame )
        {
            _status_flg |= BACT_STFLAG_DSETTED;

            for (NC_STACK_ypabact* &node : _kidList)
            {
                if ( node->_bact_type == BACT_TYPES_GUN )
                {
                    NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( node );
                    if ( gun->IsRoboGun() )
                        node->_status_flg |= BACT_STFLAG_DSETTED;
                }
            }
        }

        _old_pos = tmp;
        _old_pos.y = tmp.y;

        SFXEngine::SFXe.startSound(&_soundcarrier, 10);

        _roboState &= ~ROBOSTATE_MOVE;
        _vp_extra[0].flags = 0;
        _vp_extra[1].flags = 0;

        if ( _world->isNetGame )
        {
            uamessage_endBeam ebMsg;
            ebMsg.msgID = UAMSG_ENDBEAM;
            ebMsg.owner = _owner;
            ebMsg.id = _gid;

            yw_arg181 v16;
            v16.recvID = 0;
            v16.recvFlags = 2;
            v16.dataSize = sizeof(ebMsg);
            v16.garant = 1;
            v16.data = &ebMsg;

            _world->ypaworld_func181(&v16);
        }
    }
}

void NC_STACK_yparobo::sub_4A1270(const vec3d &vaxis, float angle)
{
    for (int i = 0; i < 8; i++)
    {
        if ( _roboGuns[i].gun_obj )
        {
            NC_STACK_ypagun *gungun = dynamic_cast<NC_STACK_ypagun *>(_roboGuns[i].gun_obj);

            if (gungun)
                _roboGuns[i].dir = gungun->ypagun_func129(vaxis, -angle);
        }
    }
}

void NC_STACK_yparobo::sub_4A1014(float angle)
{
    _rotation = mat3x3::RotateY(angle) * _rotation;

    sub_4A1270(_rotation.AxisY(), angle);

    _roboColls.field_0 = angle;
}

void NC_STACK_yparobo::sub_4A10E8(float angle)
{
    _rotation = mat3x3::RotateY(angle) * _rotation;

    sub_4A1270(vec3d::OY(1.0), angle);

    _roboColls.field_0 = angle;
}

size_t NC_STACK_yparobo::checkCollisions(float a2)
{
    int a4 = getBACT_viewer();
    int v81 = getBACT_inputting();
    int v79 = getBACT_exactCollisions();

    if ( _position == _old_pos )
        return 0;

    bact_arg88 arg88_2;
    arg88_2.pos1 = vec3d(0.0, 0.0, 0.0);

    int v86 = 0;

    ypaworld_arg136 arg136;

    for (int i = 0; i < _roboColls.robo_coll_num; i++)
    {
        if ( !v81 || _primTtype )
        {
            vec3d tmp = _roboColls.roboColls[i].field_10;
            vec3d t = _position + _rotation.Transform( _roboColls.roboColls[i].coll_pos );

            arg136.stPos = tmp;
            arg136.flags = 0;

            float v82 = (tmp.XZ() - t.XZ()).length();

            if ( v82 <= 1.0 )
                arg136.vect = _fly_dir * 300.0;
            else
                arg136.vect = vec3d::X0Z( (t.XZ() - tmp.XZ()) * 300.0 / v82 );

            _world->ypaworld_func136(&arg136);


            if ( arg136.isect )
            {
                if ( arg136.tVal * 300.0 >= _roboColls.roboColls[i].robo_coll_radius + v82)
                {
                    _status_flg &= ~BACT_STFLAG_LCRASH;
                    _status_flg |= BACT_STFLAG_UPWRD;
                    return 0;
                }
                else if ( _roboColls.roboColls[i].robo_coll_radius > 0.01 )
                {
                    bact_arg88 arg88;
                    arg88.pos1 = arg136.skel->polygons[ arg136.polyID ].Normal();

                    Recoil(&arg88);

                    if ( fabs(_roboColls.field_0) > 0.0 )
                    {
                        sub_4A1014(-_roboColls.field_0);

                        _roboColls.field_0 = 0;
                    }

                    yw_130arg arg130;
                    arg130.pos_x = arg136.isectPos.x;
                    arg130.pos_z = arg136.isectPos.z;

                    _world->ypaworld_func130(&arg130);

                    if ( v81 || !arg130.pcell->w_type )
                    {
                        yw_arg129 v60;
                        v60.pos = arg136.isectPos;
                        v60.unit = NULL;
                        v60.field_14 = 255;
                        v60.field_10 = v82 * 15000.0 / a2;

                        ChangeSectorEnergy(&v60);
                    }

                    _fly_dir_length *= 0.4;
                    _status_flg |= BACT_STFLAG_UPWRD;

                    return 1;
                }
            }
        }


        int v26 = _world->ypaworld_func145(this);

        if ( (a4 || (v26 && v79)) && _roboColls.roboColls[i].robo_coll_radius > 0.01 )
        {
            vec3d tmp = _roboColls.roboColls[i].field_10;
            vec3d t = _position + _rotation.Transform( _roboColls.roboColls[i].coll_pos );

            ypaworld_arg137 arg137;
            arg137.pos = tmp;

            float v83 = (tmp.XZ() - t.XZ()).length();

            if ( v83 <= 1.0 )
                arg137.pos2 = _fly_dir;
            else
            {
                arg137.pos2 = vec3d::X0Z( (t.XZ() - tmp.XZ()) ) / v83;

                // TYPO??
                /*arg137.pos2.x = 1.0 / v83 * (t.x - tmp.x);
                arg137.pos2.y = 0;
                arg137.pos2.z = 1.0 / v83 * (t.x - tmp.x); */
            }

            yw_137col v57[32];

            arg137.radius = _roboColls.roboColls[i].robo_coll_radius;
            arg137.collisions = v57;
            arg137.field_30 = 0;
            arg137.coll_max = 32;

            _world->ypaworld_func137(&arg137);

            if ( arg137.coll_count )
            {
                v86++;

                float v89 = v83 * 15000.0;

                vec3d ttmp(0.0, 0.0, 0.0);

                for (int j = arg137.coll_count - 1; j >= 0; j--)
                {
                    yw_137col *clzn = &arg137.collisions[j];

                    ttmp += clzn->pos2;

                    yw_130arg arg130;
                    arg130.pos_x = clzn->pos1.x;
                    arg130.pos_z = clzn->pos1.z;

                    _world->ypaworld_func130(&arg130);

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
                    arg88_2.pos1 += _fly_dir;
            }

            if ( v86 )
            {
                arg88_2.pos1 /= (float)v86;

                _position = _old_pos;


                Recoil(&arg88_2);

                if ( fabs(_roboColls.field_0) > 0.0 )
                {
                    sub_4A1014(-_roboColls.field_0);
                    _roboColls.field_0 = 0;
                }

                _fly_dir_length *= 0.4;
                _status_flg |= BACT_STFLAG_UPWRD;

                return 1;
            }
        }
    }

    _status_flg &= ~BACT_STFLAG_LCRASH;

    arg136.stPos = _position;
    arg136.vect = vec3d::OY( _height * 1.5 ) + _fly_dir.X0Z() * 100.0;
    arg136.flags = 0;

    _world->ypaworld_func136(&arg136);

    if ( arg136.isect )
    {
        if ( arg136.tVal < 0.66 )
        {
            _target_dir.y = -1.0;
            _status_flg |= BACT_STFLAG_UPWRD;
        }
    }
    else
    {
        _target_dir.y = 1.0;
        _status_flg &= ~BACT_STFLAG_UPWRD;
    }

    _roboColls.field_0 = 0;

    return 0;
}

void NC_STACK_yparobo::wallow(update_msg *arg)
{
    _position.y = sin(arg->gTime * C_PI / 3000.0) * 25.0 + _roboYPos;

    for (int i = 0; i < 8; i++)
    {
        roboGun *gun = &_roboGuns[i];

        if (gun->gun_obj)
        {
            bact_arg80 v11;
            v11.pos = _position + _rotation.Transpose().Transform(gun->pos);
            v11.field_C = 4;

            gun->gun_obj->SetPosition(&v11);
        }
    }
}

void NC_STACK_yparobo::yparobo_func70__sub2__sub0()
{
    for (int i = 0; i < _roboColls.robo_coll_num; i++)
        _roboColls.roboColls[i].field_10 = _position + _rotation.Transform( _roboColls.roboColls[i].coll_pos );
}

bool NC_STACK_yparobo::yparobo_func70__sub2__sub1()
{
    NC_STACK_ypabact *yw_1b7c = _world->getYW_userVehicle();

    if ( yw_1b7c == this )
        return false;

    for (int i = 0; i < 8; i++)
        if (_roboGuns[i].gun_obj && _roboGuns[i].gun_obj == yw_1b7c)
            return false;

    return true;
}

void NC_STACK_yparobo::AI_doMove(update_msg *arg)
{
    move_msg arg74;

    yparobo_func70__sub2__sub0();

    float v49 = arg->frameTime / 1000.0;

    _roboFlotage = _mass * 9.80665;

    if ( _target_dir.y < -0.7 )
        _roboFlotage = _mass * 3.0 * 9.80665;

    if ( _target_dir.y > 0.7 )
        _roboFlotage = _mass * 0.91 * 9.80665;

    float v39;
    if ( _primTtype )
        v39 = _target_vec.XZ().length();
    else
        v39 = 0.0;

    if ( v39 >= 100.0 )
    {
        if ( _status_flg & BACT_STFLAG_UPWRD )
        {
            _airconst *= 3.0;

            _roboFlotage = _mass * 2.5 * 9.80665;
            _thraction = 0;

            float v38 = 1.0 - v49 * 4.0;
            if ( v38 < 0.0 )
                v38 = 0.1;

            _fly_dir_length *= v38;
        }
        else if ( _primTtype )
        {
            _thraction = _force;
        }
        else
        {
            _thraction = 0;
        }

        if ( !(_status_flg & BACT_STFLAG_UPWRD) )
        {
            float v52 = _rotation.AxisZ().XZ().length();
            float v51 = _target_dir.XZ().length();

            if ( v52 <= 0.0001 || v51 <= 0.0001 )
            {
                _slider_time = 0;
                _status_flg &= ~BACT_STFLAG_XLEFT;
            }
            else
            {
                float v47 = _target_dir.XZ().dot( _rotation.AxisZ().XZ() ) / (v51 * v52);

                float v23 = clp_acos(v47);

                float v24 = fabs(v23);

                if ( v24 >= 0.001 )
                {
                    float mxx = v49 * _maxrot;

                    if ( v24 > mxx )
                        v23 = mxx;

                    if ( _rotation.AxisZ().XZ().cross( _target_dir.XZ() ) < 0.0 )
                        v23 = -v23;

                    float v28;

                    if ( v23 <= 0.0 )
                    {
                        if ( !(_status_flg & BACT_STFLAG_XLEFT) || !_slider_time )
                            _slider_time = arg->gTime;

                        v28 = (arg->gTime - _slider_time) * 0.9 * 0.001 + 0.1;
                        _status_flg |= BACT_STFLAG_XLEFT;
                    }
                    else
                    {
                        if ( _status_flg & BACT_STFLAG_XLEFT || !_slider_time )
                            _slider_time = arg->gTime;

                        v28 = (arg->gTime - _slider_time) * 0.9 * 0.001 + 0.1;
                        _status_flg &= ~BACT_STFLAG_XLEFT;
                    }

                    if ( v28 > 1.0 )
                        v28 = 1.0;

                    v23 *= v28;

                    sub_4A10E8(v23);
                }
            }
        }

        arg74.field_0 = v49;
        arg74.flag = 0;
        arg74.vec = _target_vec.X0Z() / v39;
    }
    else
    {
        _thraction = 0;

        if ( fabs(_fly_dir.x * _fly_dir_length) <= 0.1 && fabs(_fly_dir.z * _fly_dir_length) <= 0.1 )
        {
            if ( !(_roboState & ROBOSTATE_BUILDRADAR) &&
                    !(_roboState & ROBOSTATE_BUILDPOWER) &&
                    !(_roboState & ROBOSTATE_BUILDSAFE) )
            {
                bool v14 = yparobo_func70__sub2__sub1();

                if ( v14 && _roboWFlags & 1 )
                {
                    float v18;
                    if ( _status_flg & BACT_STFLAG_UPWRD )
                        v18 = _mass * 1.5;
                    else
                        v18 = _mass * 0.82;

                    _roboFlotage = v18 * 9.80665;
                }
                else
                {
                    if ( fabs(_target_dir.y) < 0.7 )
                        _fly_dir_length *= 0.35;
                }

                if ( v14 && _roboWFlags & 2 )
                {
                    sub_4A1014(_maxrot * 0.3 * v49);
                }
            }
        }
        else
        {
            _airconst *= 3.0;
            _fly_dir_length *= 0.8;

            if ( fabs(_fly_dir.x * _fly_dir_length) < 0.1 && fabs(_fly_dir.z * _fly_dir_length) < 0.1 )
            {
                _fly_dir.z = 0;
                _fly_dir.x = 0;

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

void NC_STACK_yparobo::sub_4A4538(NC_STACK_ypabact *bact)
{
    setTarget_msg arg67;
    arg67.tgt_type = BACT_TGT_TYPE_NONE;
    arg67.priority = 1;

    bact->SetTarget(&arg67);

    for( NC_STACK_ypabact* &node : bact->_kidList )
        node->SetTarget(&arg67);
}

void NC_STACK_yparobo::sub_4A448C(NC_STACK_ypabact *bact)
{
    for( NC_STACK_ypabact* &node : bact->_kidList )
    {
        node->_waypoints_count = bact->_waypoints_count;

        if ( node->_waypoints_count == 1 )
            node->_current_waypoint = 0;

        node->_status_flg |= BACT_STFLAG_WAYPOINT;

        if ( bact->_status_flg & BACT_STFLAG_WAYPOINTCCL )
            node->_status_flg |= BACT_STFLAG_WAYPOINTCCL;
        else
            node->_status_flg &= ~BACT_STFLAG_WAYPOINTCCL;

        for (int i = 0; i < 32; i++)
            node->_waypoints[i] = bact->_waypoints[i];
    }
}

void NC_STACK_yparobo::sub_4AB69C(NC_STACK_ypabact *bact, char a2)
{
    if ( a2 )
        bact->_status_flg |= BACT_STFLAG_ESCAPE;
    else
        bact->_status_flg &= ~BACT_STFLAG_ESCAPE;

    for( NC_STACK_ypabact* &node : bact->_kidList )
    {
        if ( a2 )
            node->_status_flg |= BACT_STFLAG_ESCAPE;
        else
            node->_status_flg &= ~BACT_STFLAG_ESCAPE;
    }
}

int NC_STACK_yparobo::sb_0x4a45cc__sub0(NC_STACK_ypabact *bact)
{
    vec3d tmp;
    if ( bact->_primTtype == BACT_TGT_TYPE_UNIT )
    {
        tmp = bact->_primT.pbact->_position;
    }
    else if ( bact->_primTtype == BACT_TGT_TYPE_CELL )
    {
        tmp = bact->_primTpos;
    }
    else
        return 1;

    vec3d tmp2 = bact->_position - tmp;

    float v19 = tmp2.length();

    for( NC_STACK_ypabact* &node : bact->_kidList )
    {
        vec3d tmp3 = node->_position - tmp;

        if ( tmp3.length() < v19 )
            return 0;
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

    int v6;

    switch ( arg->user_action )
    {
    case 2: //Move squad
        arg->selectBact->_status_flg &= ~(BACT_STFLAG_WAYPOINT | BACT_STFLAG_WAYPOINTCCL);

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
        arg->selectBact->SetTarget(&arg67);

        if ( !sb_0x4a45cc__sub0(arg->selectBact) )
        {
            arg109.field_4 = NULL;
            arg109.field_0 = 6;

            arg->selectBact->ReorganizeGroup(&arg109);
        }

        if ( arg->selectBact->_host_station != arg->selectBact->_parent &&
             arg->selectBact->_parent )
            arg->selectBact = arg->selectBact->_parent;

        if ( arg->target_Sect )
            v6 = sub_4A5A08(arg->selectBact, arg->target_point.x, arg->target_point.z);
        else
            v6 = sub_4A58C0(arg->selectBact, arg->target_Bact);

        if ( !v6 )
        {
            if ( _world->GameShell )
                SFXEngine::SFXe.startSound(&_world->GameShell->samples2_info, 1);
        }
        else
        {

            NC_STACK_ypabact *v8 = arg->selectBact;

            if ( v8->_parent != v8->_host_station &&
                 v8->_parent )
                v8 = v8->_parent;

            arg134.unit = v8;
            arg134.field_C = 0;
            arg134.field_8 = 0;
            arg134.field_10 = 0;
            arg134.field_14 = 38;

            if ( v8->_primTtype == BACT_TGT_TYPE_CELL )
            {
                cellArea *cell = v8->_primT.pcell;
                if ( cell->owner == _owner )
                {
                    arg134.field_4 = 15;
                }
                else
                {
                    arg134.field_4 = 16;
                }
                
                placeMessage(&arg134);
            }
            else if ( v8->_primTtype == BACT_TGT_TYPE_UNIT )
            {
                if ( v8->_primT.pbact->_owner == _owner )
                {
                    arg134.field_4 = 15;
                }
                else
                {
                    arg134.field_4 = 16;
                }

                placeMessage(&arg134);
            }
        }
        sub_4A4538(arg->selectBact);
        sub_4AB69C(arg->selectBact, 0);
        break;

    case 3:
        if ( arg->energy <= _roboEnergyLife )
        {
            arg146.pos = arg->target_point;
            arg146.vehicle_id = arg->protoID;

            NC_STACK_ypabact *newbact = _world->ypaworld_func146(&arg146);

            if ( newbact )
            {
                arg67.tgt_type = BACT_TGT_TYPE_CELL;
                arg67.priority = 0;
                arg67.tgt_pos.x = (arg146.pos.x - _position.x) * 4.0 + _position.x;
                arg67.tgt_pos.y = arg146.pos.y;
                arg67.tgt_pos.z = (arg146.pos.z - _position.z) * 4.0 + _position.z;

                newbact->SetTarget(&arg67);

                arg78.newStatus = BACT_STATUS_CREATE;
                arg78.setFlags = 0;
                arg78.unsetFlags = 0;

                newbact->SetState(&arg78);

                newbact->_scale_time = newbact->_energy_max * 0.2;
                newbact->_commandID = dword_5B1128;
                dword_5B1128++;

                newbact->_owner = _owner;
                AddSubject(newbact);

                if ( _world->isNetGame )
                {
                    newbact->_gid |= newbact->_owner << 24;
                    newbact->_commandID |= newbact->_owner << 24;

                    uamessage_newVhcl nvMsg;
                    nvMsg.msgID = UAMSG_NEWVHCL;
                    nvMsg.owner = newbact->_owner;
                    nvMsg.pos = newbact->_position;
                    nvMsg.type = newbact->_vehicleID;
                    nvMsg.parent = newbact->_parent->_gid;
                    nvMsg.id = newbact->_gid;
                    nvMsg.kind = 1;
                    nvMsg.commandID = newbact->_commandID;

                    arg181.recvFlags = 2;
                    arg181.dataSize = sizeof(nvMsg);
                    arg181.recvID = 0;
                    arg181.garant = 1;
                    arg181.data = &nvMsg;
                    _world->ypaworld_func181(&arg181);
                }

                if ( newbact->_bact_type == BACT_TYPES_UFO )
                    newbact->setBACT_aggression(25);
                else
                    newbact->setBACT_aggression(60);

                int v67 = getBACT_bactCollisions();
                newbact->setBACT_bactCollisions(v67);

                newbact->_host_station = this;
                _roboEnergyLife -= arg->energy;

                _world->HistoryAktCreate(newbact);
            }
        }
        break;

    case 4:
        if ( arg->energy <= _roboEnergyLife )
        {
            arg146.pos = arg->target_point;
            arg146.vehicle_id = arg->protoID;
            NC_STACK_ypabact *newbact2 = _world->ypaworld_func146(&arg146);

            if ( newbact2 )
            {
                arg78.newStatus = BACT_STATUS_CREATE;
                arg78.setFlags = 0;
                arg78.unsetFlags = 0;
                newbact2->SetState(&arg78);

                newbact2->_scale_time = newbact2->_energy_max * 0.2;
                newbact2->_owner = _owner;
                newbact2->_commandID = arg->selectBact->_commandID;

                arg->selectBact->AddSubject(newbact2);

                newbact2->_aggr = arg->selectBact->_aggr;

                if ( _world->isNetGame )
                {
                    newbact2->_gid |= newbact2->_owner << 24;

                    uamessage_newVhcl nvMsg;
                    nvMsg.msgID = UAMSG_NEWVHCL;
                    nvMsg.owner = newbact2->_owner;
                    nvMsg.pos = newbact2->_position;
                    nvMsg.type = newbact2->_vehicleID;
                    nvMsg.parent = newbact2->_parent->_gid;
                    nvMsg.id = newbact2->_gid;
                    nvMsg.kind = 2;
                    nvMsg.commandID = newbact2->_commandID;

                    arg181.recvID = 0;
                    arg181.recvFlags = 2;
                    arg181.data = &nvMsg;
                    arg181.dataSize = sizeof(nvMsg);
                    arg181.garant = 1;

                    _world->ypaworld_func181(&arg181);
                }

                int v67 = getBACT_bactCollisions();
                newbact2->setBACT_bactCollisions(v67);

                newbact2->_host_station = this;
                _roboEnergyLife -= arg->energy;

                _world->HistoryAktCreate(newbact2);
                newbact2->CopyTargetOf(arg->selectBact);
            }
        }
        break;

    case 6:
        if ( arg->energy <= _roboEnergyLife )
        {
            arg148.ownerID = _owner;
            arg148.ownerID2 = _owner;
            arg148.blg_ID = arg->protoID;
            arg148.field_C = 0;
            arg148.x = arg->target_sect_x;
            arg148.y = arg->target_sect_y;
            arg148.field_18 = 0;
            if ( _world->ypaworld_func148(&arg148) )
            {
                _roboEnergyLife -= arg->energy;

                if ( _world->isNetGame )
                {
                    uamessage_startBuild sbMsg;
                    sbMsg.msgID = UAMSG_STARTBUILD;
                    sbMsg.owner = _owner;
                    sbMsg.bproto = arg->protoID;
                    sbMsg.sec_x = arg->target_sect_x;
                    sbMsg.sec_y = arg->target_sect_y;

                    arg181.recvID = 0;
                    arg181.recvFlags = 2;
                    arg181.data = &sbMsg;
                    arg181.garant = 1;
                    arg181.dataSize = sizeof(sbMsg);

                    _world->ypaworld_func181(&arg181);
                }
            }
        }
        break;

    case 10:
        if ( !(_roboState & ROBOSTATE_MOVE) )
        {
            SFXEngine::SFXe.startSound(&_soundcarrier, 9);

            _roboState |= ROBOSTATE_MOVE;
            _roboEnergyMove -= arg->energy;
            _roboBeamPos = arg->target_point;
            _roboBeamTimePre = 1500;
            if ( _world->isNetGame )
            {
                uamessage_startBeam stbMsg;
                stbMsg.msgID = UAMSG_STARTBEAM;
                stbMsg.owner = _owner;
                stbMsg.id = _gid;
                stbMsg.pos = _roboBeamPos;

                arg181.recvID = 0;
                arg181.recvFlags = 2;
                arg181.data = &stbMsg;
                arg181.dataSize = sizeof(stbMsg);
                arg181.garant = 1;

                _world->ypaworld_func181(&arg181);
            }
        }
        break;

    case 11:
    {
        arg->selectBact->_status_flg |= BACT_STFLAG_WAYPOINT;
        arg->selectBact->_m_cmdID = 0;

        int v11 = 0;

        if ( arg->selectBact->_bact_type != BACT_TYPES_TANK && arg->selectBact->_bact_type != BACT_TYPES_CAR )
        {
            for( NC_STACK_ypabact* &node : arg->selectBact->_kidList )
            {
                if ( node->_bact_type == BACT_TYPES_TANK || node->_bact_type == BACT_TYPES_CAR )
                {
                    v11 = 1;
                    break;
                }
            }
        }
        else
            v11 = 1;

        if ( v11 )
        {
            arg124.from = arg->selectBact->_position.XZ();
            arg124.to = arg->target_point.XZ();
            arg124.steps_cnt = 32;
            arg124.field_12 = 1;

            int v14 = PathFinder(&arg124);

            if ( v14 && arg124.steps_cnt > 0)
            {
                for (int i = 0; i < arg124.steps_cnt; i++)
                {
                    arg->selectBact->_waypoints[i].x = arg124.waypoints[i].x;
                    arg->selectBact->_waypoints[i].z = arg124.waypoints[i].z;
                }
                arg->selectBact->_waypoints_count = arg124.steps_cnt;
            }
            else
            {
                arg->selectBact->_waypoints_count = 1;
                arg->selectBact->_waypoints[0].x = arg->target_point.x;
                arg->selectBact->_waypoints[0].z = arg->target_point.z;

                if ( _world->GameShell )
                    SFXEngine::SFXe.startSound(&_world->GameShell->samples2_info, 1);
            }
        }
        else
        {
            arg->selectBact->_waypoints_count = 1;
            arg->selectBact->_waypoints[0].x = arg->target_point.x;
            arg->selectBact->_waypoints[0].z = arg->target_point.z;
        }

        arg->selectBact->_current_waypoint = 0;

        arg67.tgt_pos.x = arg->selectBact->_waypoints[0].x;
        arg67.tgt_pos.z = arg->selectBact->_waypoints[0].z;
        arg67.priority = 0;
        arg67.tgt_type = BACT_TGT_TYPE_CELL;
        arg->selectBact->SetTarget(&arg67);

        sub_4A448C(arg->selectBact);
        sub_4A4538(arg->selectBact);
        sub_4AB69C(arg->selectBact, 0);
    }
    break;

    case 12:
        if ( arg->selectBact->_waypoints_count < 32 && (arg->selectBact->_status_flg & BACT_STFLAG_WAYPOINT) )
        {
            int v72 = arg->selectBact->_waypoints_count;
            int v20 = 0;

            if ( arg->selectBact->_bact_type != BACT_TYPES_TANK && arg->selectBact->_bact_type != BACT_TYPES_CAR )
            {
                for(NC_STACK_ypabact* &nod : arg->selectBact->_kidList)
                {
                    if ( nod->_bact_type == BACT_TYPES_TANK || nod->_bact_type == BACT_TYPES_CAR )
                    {
                        v20 = 1;
                        break;
                    }
                }
            }
            else
                v20 = 1;

            if ( v20 )
            {
                arg124.from = arg->selectBact->_waypoints[v72 - 1].XZ();
                arg124.to = arg->target_point.XZ();
                arg124.steps_cnt = 32 - v72;
                arg124.field_12 = 1;

                if ( PathFinder(&arg124) && arg124.steps_cnt > 0 )
                {
                    for (int i = 0; i < arg124.steps_cnt; i++)
                    {
                        arg->selectBact->_waypoints[i + v72].x = arg124.waypoints[i].x;
                        arg->selectBact->_waypoints[i + v72].z = arg124.waypoints[i].z;
                    }
                    arg->selectBact->_waypoints_count += arg124.steps_cnt;

                }
                else
                {
                    arg->selectBact->_waypoints[v72].x = arg->target_point.x;
                    arg->selectBact->_waypoints[v72].z = arg->target_point.z;
                    arg->selectBact->_waypoints_count++;

                    if ( _world->GameShell )
                        SFXEngine::SFXe.startSound(&_world->GameShell->samples2_info, 1);
                }
            }
            else
            {
                arg->selectBact->_waypoints[v72].x = arg->target_point.x;
                arg->selectBact->_waypoints[v72].z = arg->target_point.z;
                arg->selectBact->_waypoints_count++;
            }
            if ( !(arg->selectBact->_status_flg & BACT_STFLAG_WAYPOINT) )
            {
                arg->selectBact->_status_flg |= BACT_STFLAG_WAYPOINT;

                arg67.tgt_pos.x = arg->selectBact->_waypoints[v72].x;
                arg67.tgt_pos.z = arg->selectBact->_waypoints[v72].z;
                arg67.priority = 0;
                arg67.tgt_type = BACT_TGT_TYPE_CELL;
                arg->selectBact->SetTarget(&arg67);
            }
            sub_4A448C(arg->selectBact);
        }
        break;

    case 13:
        arg->selectBact->_status_flg |= BACT_STFLAG_WAYPOINTCCL;
        sub_4A448C(arg->selectBact);
        break;

    default:
        break;
    }
}

int NC_STACK_yparobo::yparobo_func70__sub4__sub0__sub0(TBuildingProto *protos)
{
    int id = -1;
    int power = 0;

    for (int i = 0; i < 128; i++)
    {
        TBuildingProto *proto = &protos[i];

        if ( proto->EnableMask & (1 << _owner) )
        {
            if ( proto->ModelID == 2 )
            {
                if ( (_roboBuildSpare + _energy - _energy_max * 0.2) > proto->Energy )
                {
                    int protoPow = proto->Power;

                    for (TBuildingProto::TGun &gun : proto->Guns)
                    {
                        if ( gun.VhclID )
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

void NC_STACK_yparobo::sub_4F4FF4(int sectID, setTarget_msg *parg67)
{
    int sX = sectID % _secMaxX;
    int sY = sectID / _secMaxX;

    if ( sX == _sectX && sY == _sectY )
    {
        if ( sX >= 2 )
            sX--;
        else
            sX++;
    }
    else
    {
        if ( sX > _sectX )
            sX--;
        if ( sX < _sectX )
            sX++;

        if ( sY > _sectY )
            sY--;
        if ( sY < _sectY )
            sY++;
    }

    parg67->tgt_type = BACT_TGT_TYPE_CELL;
    parg67->priority = 0;
    parg67->tgt_pos.x = (sX + 0.5) * 1200.0;
    parg67->tgt_pos.z = -(sY + 0.5) * 1200.0;
}

void NC_STACK_yparobo::buildRadar(update_msg *arg)
{
    int xx = _roboBuildingCellID % _secMaxX;
    int yy = _roboBuildingCellID / _secMaxX;

    float v35 = arg->frameTime * 0.001;

    if ( _roboPCells[ _roboBuildingCellID ].w_type )
    {
        setTarget_msg arg67;
        arg67.tgt_type = BACT_TGT_TYPE_NONE;
        arg67.priority = 0;

        SetTarget(&arg67);
        _roboBuildingDuty = 0;
    }
    else
    {
        TBuildingProto *buildprotos = _world->getYW_buildProtos();

        int build_id = yparobo_func70__sub4__sub0__sub0(buildprotos);

        if ( build_id == -1 )
        {
            setTarget_msg arg67;
            arg67.tgt_type = BACT_TGT_TYPE_NONE;
            arg67.priority = 0;

            SetTarget(&arg67);
            _roboBuildingDuty = 0;
        }
        else
        {

            if ( abs(_sectX - xx) > 1 || abs(_sectY - yy) > 1 )
            {
                setTarget_msg arg67;
                sub_4F4FF4(_roboBuildingCellID, &arg67);

                arg67.tgt_type = BACT_TGT_TYPE_CELL_IND;

                SetTarget(&arg67);
            }
            else
            {

                if ( _sectX != xx || _sectY != yy )
                {
                    setTarget_msg arg67;
                    arg67.tgt_type = BACT_TGT_TYPE_NONE;
                    arg67.priority = 0;
                    SetTarget(&arg67);

                    vec2d t ( (xx + 0.5) * 1200.0 - _position.x,
                              -(yy + 0.5) * 1200.0 - _position.z );

                    t.normalise();

                    float v41 = t.dot( _rotation.AxisZ().XZ() );

                    if ( v41 <= 0.9 )
                    {
                        float a2 = clp_acos(v41);

                        if ( t.cross( _rotation.AxisZ().XZ() ) > 0.0 )
                            a2 = -a2;

                        if ( a2 < -_maxrot * v35 )
                            a2 = -_maxrot * v35;

                        if ( a2 > _maxrot * v35 )
                            a2 = _maxrot * v35;

                        sub_4A10E8(a2);
                    }
                    else
                    {
                        ypaworld_arg148 arg148;

                        arg148.ownerID2 = _owner;
                        arg148.ownerID = _owner;
                        arg148.blg_ID = build_id;
                        arg148.x = xx;
                        arg148.y = yy;
                        arg148.field_C = 0;
                        arg148.field_18 = 0;

                        if ( _world->ypaworld_func148(&arg148) )
                        {
                            TBuildingProto *proto = &buildprotos[build_id];

                            if ( _roboBuildSpare >= proto->Energy )
                            {
                                _roboBuildSpare -= proto->Energy;
                            }
                            else
                            {
                                _energy -= proto->Energy - _roboBuildSpare;
                                _roboBuildSpare = 0;
                            }

                            _roboBuildingDuty = 0;

                            if ( _roboNewAI )
                                _roboRadarDelay = _roboTimeScale * (100 - _roboEpRadar) / 100;
                        }
                        else
                        {
                            setTarget_msg arg67_1;
                            arg67_1.tgt_type = BACT_TGT_TYPE_NONE;
                            arg67_1.priority = 0;

                            SetTarget(&arg67_1);

                            _roboBuildingDuty = 0;
                        }
                    }
                }
                else
                {
                    setTarget_msg arg67;

                    sub_4F4FF4(_roboBuildingCellID, &arg67);

                    arg67.tgt_type = BACT_TGT_TYPE_CELL_IND;

                    SetTarget(&arg67);
                }

            }

        }
    }
}

int NC_STACK_yparobo::yparobo_func70__sub4__sub2__sub0(TBuildingProto *protos)
{
    int id = -1;
    int power = 0;

    for (int i = 0; i < 128; i++)
    {
        TBuildingProto *proto = &protos[i];

        if ( proto->EnableMask & (1 << _owner) )
        {
            if ( proto->ModelID == 1 )
            {
                if ( (_roboBuildSpare + _energy - _energy_max * 0.2) > proto->Energy )
                {
                    int protoPow = proto->Power;

                    for (TBuildingProto::TGun &gun : proto->Guns)
                    {
                        if ( gun.VhclID )
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
    int xx = _roboBuildingCellID % _secMaxX;
    int yy = _roboBuildingCellID / _secMaxX;

    float v38 = arg->frameTime * 0.001;

    if ( _roboPCells[ _roboBuildingCellID ].w_type )
    {
        setTarget_msg arg67;
        arg67.tgt_type = BACT_TGT_TYPE_NONE;
        arg67.priority = 0;
        SetTarget(&arg67);

        _roboBuildingDuty = 0;
    }
    else
    {
        yw_arg176 arg176;
        arg176.owner = _owner;
        _world->ypaworld_func176(&arg176);

        if ( _roboPCells[ _roboBuildingCellID ].energy_power != -1 && ( arg176.field_4 >= 0.9 || arg176.field_4 <= 0.001) )
        {
            TBuildingProto *buildprotos = _world->getYW_buildProtos();

            int bldid = yparobo_func70__sub4__sub2__sub0(buildprotos);

            if ( bldid == -1 )
            {
                setTarget_msg arg67;
                arg67.tgt_type = BACT_TGT_TYPE_NONE;
                arg67.priority = 0;
                SetTarget(&arg67);
                _roboBuildingDuty = 0;
            }
            else
            {
                if ( abs(_sectX - xx) > 1 || abs(_sectY - yy) > 1 )
                {
                    setTarget_msg arg67;
                    sub_4F4FF4(_roboBuildingCellID, &arg67);
                    arg67.tgt_type = BACT_TGT_TYPE_CELL_IND;
                    SetTarget(&arg67);
                }
                else
                {

                    if ( _sectX != xx || _sectY != yy )
                    {
                        setTarget_msg arg67;
                        arg67.tgt_type = BACT_TGT_TYPE_NONE;
                        arg67.priority = 0;
                        SetTarget(&arg67);

                        vec2d t ( (xx + 0.5) * 1200.0 - _position.x,
                                  -(yy + 0.5) * 1200.0 - _position.z );

                        t.normalise();

                        float v44 = t.dot( _rotation.AxisZ().XZ() );

                        if ( v44 <= 0.9 )
                        {
                            float a2 = clp_acos(v44);

                            if ( t.cross( _rotation.AxisZ().XZ() ) > 0.0 )
                                a2 = -a2;

                            if ( a2 < -_maxrot * v38 )
                                a2 = -_maxrot * v38;

                            if ( a2 > _maxrot * v38 )
                                a2 = _maxrot * v38;

                            sub_4A10E8(a2);
                        }
                        else
                        {
                            ypaworld_arg148 arg148;
                            arg148.ownerID2 = _owner;
                            arg148.ownerID = _owner;
                            arg148.blg_ID = bldid;
                            arg148.x = xx;
                            arg148.y = yy;
                            arg148.field_C = 0;
                            arg148.field_18 = 0;

                            if ( _world->ypaworld_func148(&arg148) )
                            {
                                TBuildingProto *proto = &buildprotos[bldid];

                                if ( _roboBuildSpare >= proto->Energy )
                                {
                                    _roboBuildSpare -= proto->Energy;
                                }
                                else
                                {
                                    _energy -= proto->Energy - _roboBuildSpare;
                                    _roboBuildSpare = 0;
                                }

                                _roboBuildingDuty = 0;

                                if ( _roboNewAI )
                                    _roboPowerDelay = _roboTimeScale * (100 - _roboEpPower) / 100;
                            }
                            else
                            {
                                setTarget_msg arg67_1;
                                arg67_1.tgt_type = BACT_TGT_TYPE_NONE;
                                arg67_1.priority = 0;
                                SetTarget(&arg67_1);
                                _roboBuildingDuty = 0;
                            }
                        }
                    }
                    else
                    {
                        setTarget_msg arg67;
                        sub_4F4FF4(_roboBuildingCellID, &arg67);
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
            _roboBuildingDuty = 0;
        }
    }
}

int NC_STACK_yparobo::yparobo_func70__sub4__sub1__sub0(TBuildingProto *protos)
{
    int id = -1;
    int power = 0;

    for (int i = 0; i < 128; i++)
    {
        TBuildingProto *proto = &protos[i];

        if ( proto->EnableMask & (1 << _owner) )
        {
            if ( proto->ModelID == 3 )
            {
                if ( (_roboBuildSpare + _energy - _energy_max * 0.2) > proto->Energy )
                {
                    int protoPow = proto->Power;

                    for (TBuildingProto::TGun &gun : proto->Guns)
                    {
                        if ( gun.VhclID )
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
    int xx = _roboBuildingCellID % _secMaxX;
    int yy = _roboBuildingCellID / _secMaxX;

    float v35 = arg->frameTime * 0.001;

    if ( _roboPCells[ _roboBuildingCellID ].w_type )
    {
        setTarget_msg arg67;
        arg67.tgt_type = BACT_TGT_TYPE_NONE;
        arg67.priority = 0;

        SetTarget(&arg67);
        _roboBuildingDuty = 0;
    }
    else
    {
        TBuildingProto *buildprotos = _world->getYW_buildProtos();

        int build_id = yparobo_func70__sub4__sub1__sub0(buildprotos);

        if ( build_id == -1 )
        {
            setTarget_msg arg67;
            arg67.tgt_type = BACT_TGT_TYPE_NONE;
            arg67.priority = 0;

            SetTarget(&arg67);
            _roboBuildingDuty = 0;
        }
        else
        {

            if ( abs(_sectX - xx) > 1 || abs(_sectY - yy) > 1 )
            {
                setTarget_msg arg67;
                sub_4F4FF4(_roboBuildingCellID, &arg67);

                arg67.tgt_type = BACT_TGT_TYPE_CELL_IND;

                SetTarget(&arg67);
            }
            else
            {

                if ( _sectX != xx || _sectY != yy )
                {
                    setTarget_msg arg67;
                    arg67.tgt_type = BACT_TGT_TYPE_NONE;
                    arg67.priority = 0;
                    SetTarget(&arg67);

                    vec2d t ( (xx + 0.5) * 1200.0 - _position.x,
                              -(yy + 0.5) * 1200.0 - _position.z );

                    t.normalise();

                    float v41 = t.dot( _rotation.AxisZ().XZ() );

                    if ( v41 <= 0.9 )
                    {
                        float a2 = clp_acos(v41);

                        if ( t.cross( _rotation.AxisZ().XZ() ) > 0.0 )
                            a2 = -a2;

                        if ( a2 < -_maxrot * v35 )
                            a2 = -_maxrot * v35;

                        if ( a2 > _maxrot * v35 )
                            a2 = _maxrot * v35;

                        sub_4A10E8(a2);
                    }
                    else
                    {
                        ypaworld_arg148 arg148;

                        arg148.ownerID2 = _owner;
                        arg148.ownerID = _owner;
                        arg148.blg_ID = build_id;
                        arg148.x = xx;
                        arg148.y = yy;
                        arg148.field_C = 0;
                        arg148.field_18 = 0;

                        if ( _world->ypaworld_func148(&arg148) )
                        {
                            TBuildingProto *proto = &buildprotos[build_id];

                            if ( _roboBuildSpare >= proto->Energy )
                            {
                                _roboBuildSpare -= proto->Energy;
                            }
                            else
                            {
                                _energy -= proto->Energy - _roboBuildSpare;
                                _roboBuildSpare = 0;
                            }

                            _roboBuildingDuty = 0;

                            if ( _roboNewAI )
                                _roboSafetyDelay = _roboTimeScale * (100 - _roboEpSafety) / 100;
                        }
                        else
                        {
                            setTarget_msg arg67_1;
                            arg67_1.tgt_type = BACT_TGT_TYPE_NONE;
                            arg67_1.priority = 0;

                            SetTarget(&arg67_1);

                            _roboBuildingDuty = 0;
                        }
                    }
                }
                else
                {
                    setTarget_msg arg67;

                    sub_4F4FF4(_roboBuildingCellID, &arg67);

                    arg67.tgt_type = BACT_TGT_TYPE_CELL_IND;

                    SetTarget(&arg67);
                }

            }

        }
    }
}

void NC_STACK_yparobo::changePlace()
{
    int xx = _roboBuildingCellID % _secMaxX;
    int yy = _roboBuildingCellID / _secMaxX;

    if ( xx >= 1 && xx <= (_secMaxX - 2) && yy >= 1 && yy <= (_secMaxY - 2) )
    {
        int tmp1 = _sectY - yy;
        int tmp2 = _sectX - xx;

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
            _roboSafetyCellID = 0;
            _roboSafetyValue = 0;
            _roboSafetyCellIndex = 0;
            _roboPositionCellID = 0;
            _roboRadarCellID = 0;
            _roboPowerCellID = 0;
            _roboPositionValue = 0;
            _roboRadarValue = 0;
            _roboPowerValue = 0;
            _roboPositionCellIndex = 0;
            _roboRadarCellIndex = 0;
            _roboPowerCellIndex = 0;

            _roboBuildingDuty = 0;

            _roboState &= ~(ROBOSTATE_READYRADAR | ROBOSTATE_READYPOWER
                                 | ROBOSTATE_READYSAFE | ROBOSTATE_FOUNDPLACE);

            setTarget_msg arg67;
            arg67.tgt_type = BACT_TGT_TYPE_NONE;
            arg67.priority = 0;
            SetTarget(&arg67);

            if ( _roboNewAI )
                _roboPositionDelay = _roboTimeScale * (100 - _roboEpChangePlace) / 100;
        }
    }
    else
    {
        _roboBuildingDuty = 0;
    }
}


NC_STACK_ypabact *NC_STACK_yparobo::AllocForce(robo_loct1 *arg)
{
    int v72 = 0;
    float v73 = 0.0;

    int v85 = -1;
    int v86 = -1;
    int v87 = -1;

    int v75 = 10000000;
    int v2 = 0;
    int v77 = 0;
    int v82 = -2;
    float v76 = 153600.0;
    NC_STACK_ypabact *selectedCommander = NULL;

    for (NC_STACK_ypabact* &node : _kidList)
    {
        //Unused flag
        if ( node->_status == BACT_STATUS_IDLE && /*!( node->status_flg & BACT_STFLAG_AKKU ) &&*/ !( node->_status_flg & BACT_STFLAG_UNUSE ) && !( node->_status_flg & BACT_STFLAG_ESCAPE ) &&  node->_bact_type != BACT_TYPES_GUN )
        {
            v2++;

            int v8 = _world->TestVehicle(node->_vehicleID, arg->job);

            if ( abs(node->_sectX - _sectX) < 2 && abs(node->_sectY - _sectY) < 2 )
                v77 = 1;

            if ( v8 > -1 && v8 >= v82 )
            {

                bact_arg81 arg81;
                arg81.enrg_type = 1;
                arg81.enrg_sum = 0;
                node->GetSummary(&arg81);

                int v11 = abs(arg81.enrg_sum - arg->energ);
                int v15 = 0;

                int v16;

                if ( v11 )
                    v16 = arg->energ / v11;
                else
                    v16 = 6;

                float v78 = (node->_position.XZ() - arg->tgt_pos.XZ()).length();

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
                    selectedCommander = node;
                    v82 = v8;
                }

            }
        }
    }

    if ( !(_roboState & ROBOSTATE_DOCKINUSE) )
    {
        bact_arg81 arg81_2;
        arg81_2.enrg_sum = 0;
        arg81_2.enrg_type = 3;

        GetSummary(&arg81_2);

        v85 = -1;
        v86 = -1;
        v87 = -1;

        v73 = arg81_2.enrg_sum * 0.05 + 1.0;

        VhclProto *vhcl_protos = _world->getYW_vhclProtos();

        for (int i = 0; i < 256; i++)
        {
            if ( vhcl_protos[i].model_id )
            {
                float v20 = _energy_max * 0.2;
                int v67 = _energy - v20;

                if ( _roboState & ROBOSTATE_BUILDROBO )
                    v67 += _roboVehicleSpare;

                VhclProto *vhclprt = &vhcl_protos[i];
                if ( (1 << _owner) & vhclprt->disable_enable_bitmask )
                {
                    if ( vhclprt->model_id != 3 && vhclprt->model_id != 9 && vhclprt->model_id != 4 )
                    {
                        if ( v67 > ((float)vhclprt->energy * v73) )
                        {
                            int v80 = _world->TestVehicle(i, arg->job);

                            int v27 = 0;
                            if ( v2 < (_secMaxX + _secMaxY) / 5 )
                                v27 = 1;

                            if ( _energy < ((float)_energy_max * 0.9) )
                                v27 = 0;

                            if ( _roboState & ROBOSTATE_DOCKINUSE )
                                v27 = 0;

                            if ( v77 )
                                v27 = 0;

                            if ( v80 > -1 && (v80 > v82 || (v80 == v82 && v27)) )
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

    if ( v85 == -1 && !selectedCommander )
        return NULL;

    if ( v85 == -1 )
    {
        if ( !(_roboState & ROBOSTATE_DOCKINUSE) )
        {
            bact_arg81 arg81_3;
            arg81_3.enrg_type = 1;
            arg81_3.enrg_sum = 0;

            selectedCommander->GetSummary(&arg81_3);

            if ( arg81_3.enrg_sum < arg->energ )
            {
                _roboDockUser = selectedCommander->_commandID;
                _roboDockEnerg = arg->energ - arg81_3.enrg_sum;
                v72 = 1;
            }
        }
    }
    else
    {
        int v32 = _clock % 10;
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
        arg146.pos = _position + _roboDockPos;

        NC_STACK_ypabact *newUnit = _world->ypaworld_func146(&arg146);

        if ( !newUnit )
            return NULL;

        newUnit->_commandID = dword_5B1128;
        dword_5B1128++;

        newUnit->_owner = _owner;
        newUnit->_host_station = this;

        int v69 = getBACT_bactCollisions();
        newUnit->setBACT_bactCollisions(v69);

        AddSubject(newUnit);
        if ( _world->isNetGame )
        {
            newUnit->_gid |= newUnit->_owner << 24;
            newUnit->_commandID |= newUnit->_owner << 24;

            uamessage_newVhcl nvMsg;

            nvMsg.msgID = UAMSG_NEWVHCL;
            nvMsg.owner = newUnit->_owner;
            nvMsg.pos = newUnit->_position;
            nvMsg.type = newUnit->_vehicleID;
            nvMsg.parent = newUnit->_parent->_gid;
            nvMsg.id = newUnit->_gid;
            nvMsg.kind = 1;
            nvMsg.commandID = newUnit->_commandID;

            yw_arg181 arg181;
            arg181.recvID = 0;
            arg181.recvFlags = 2;
            arg181.data = &nvMsg;
            arg181.dataSize = sizeof(nvMsg);
            arg181.garant = 1;

            _world->ypaworld_func181(&arg181);
        }

        _roboState |= ROBOSTATE_DOCKINUSE;

        newUnit->setBACT_aggression(60);

        _roboDockUser = newUnit->_commandID;
        _roboDockEnerg = arg->energ - newUnit->_energy;

        newUnit->_scale_time = (float)newUnit->_energy_max * 0.2;

        _roboDockTime = newUnit->_scale_time + 2000;

        newUnit->_status_flg &= ~BACT_STFLAG_ESCAPE;

        for (NC_STACK_ypabact* &nnode : newUnit->_kidList )
            nnode->_status_flg &= ~BACT_STFLAG_ESCAPE;

        setState_msg arg78;
        arg78.newStatus = BACT_STATUS_CREATE;
        arg78.setFlags = 0;
        arg78.unsetFlags = 0;

        newUnit->SetState(&arg78);


        setTarget_msg arg67;
        arg67.tgt_type = BACT_TGT_TYPE_CELL;
        arg67.priority = 0;
        arg67.tgt_pos.x = _rotation.m20 * 1200.0 * 0.5 + _position.x;
        arg67.tgt_pos.z = _rotation.m22 * 1200.0 * 0.5 + _position.z;

        newUnit->SetTarget(&arg67);

        int v70 = (float)newUnit->_energy_max * v73;

        if ( _roboState & ROBOSTATE_BUILDROBO )
        {
            if ( v70 > _roboVehicleSpare )
            {
                _energy -= v70 - _roboVehicleSpare;
                _roboVehicleSpare = 0;
            }
            else
            {
                _roboVehicleSpare -= v70;
            }
        }
        else
        {
            _energy -= v70;
        }

        selectedCommander = newUnit;
        v72 = 1;
        _world->HistoryAktCreate(newUnit);
    }

    if ( v72 )
    {
        _roboState |= (ROBOSTATE_SETDOCKTARGET | ROBOSTATE_DOCKINUSE);
        _roboDockTargetBact = arg->tgt_bact;
        _roboDockTargetType = arg->tgType;
        _roboDockTargetCommandID = arg->command_id;

        _roboDockTargetPos = arg->tgt_pos;
        _roboDockAggr = arg->aggr;

        if ( arg->tgType == BACT_TGT_TYPE_UNIT )
            _roboDockTargetPos = arg->tgt_bact->_position;
    }
    else
    {
        setTarget_msg arg67_2;
        arg67_2.tgt_pos = arg->tgt_pos;
        arg67_2.priority = 0;
        arg67_2.tgt.pbact = arg->tgt_bact;
        arg67_2.tgt_type = arg->tgType;

        selectedCommander->SetTarget(&arg67_2);

        selectedCommander->setBACT_aggression(arg->aggr);
    }

    return selectedCommander;
}



void NC_STACK_yparobo::buildConquer()
{
    bact_arg92 arg92;
    arg92.field_14 = 2;
    arg92.pos.x = ((_roboVehicleCellID % _secMaxX) + 0.5) * 1200.0;
    arg92.pos.z = -( (_roboVehicleCellID / _secMaxX ) + 0.5) * 1200.0;

    GetForcesRatio(&arg92);

    int v20 = (arg92.energ2 - arg92.energ1) * 0.5;

    if ( v20 < 15000 )
        v20 = 15000;

    yw_130arg arg130;
    arg130.pos_x = arg92.pos.x;
    arg130.pos_z = arg92.pos.z;

    _world->ypaworld_func130(&arg130);

    int enrg = 0;

    for ( NC_STACK_ypabact* &cell_unit : arg130.pcell->unitsList )
    {
        if ( _owner != cell_unit->_owner && cell_unit->_bact_type == BACT_TYPES_GUN && (cell_unit->_weapon != -1 || cell_unit->_mgun != -1) )
        {
            NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( cell_unit );
            int a4 = gun->IsRoboGun();

            if ( !a4 )
            {
                for (NC_STACK_ypabact* &bctt : _world->_unitsList)
                {
                    if ( bctt->_bact_type == BACT_TYPES_ROBO && bctt->_status != BACT_STATUS_DEAD && cell_unit->_owner == bctt->_owner )
                    {
                        int v6 = bctt->_energy_max / 2;

                        if ( v6 < 120000 )
                            v6 = 120000;

                        enrg = v6;
                        break;
                    }
                }
            }
        }

        if ( enrg )
            break;
    }

    if ( enrg > v20 )
        v20 = enrg;

    ypaworld_arg136 arg136;
    arg136.stPos = _position + _roboDockPos - vec3d::OY(100.0);
    arg136.vect = vec3d::OY(20000.0);
    arg136.flags = 0;

    _world->ypaworld_func136(&arg136);

    if ( _pSector->height - arg136.isectPos.y >= 50.0 )
    {
        setTarget_msg arg67;
        arg67.tgt_pos.x = _position.x + 200.0;
        arg67.tgt_pos.z = _position.z + 300.0;
        arg67.priority = 0;
        arg67.tgt_type = BACT_TGT_TYPE_CELL_IND;

        SetTarget(&arg67);
    }
    else
    {
        robo_loct1 loct;

        loct.distance = (arg92.pos.XZ() - _position.XZ()).length();
        loct.tgt_pos = arg92.pos.X0Z();

        loct.tgt_bact = NULL;

        if ( v20 <= 20000 )
            loct.energ = 20000;
        else
            loct.energ = v20;

        loct.tgType = BACT_TGT_TYPE_CELL;

        if ( _roboVehicleCell->w_type == 2 )
            loct.aggr = 49;
        else
            loct.aggr = 60;

        loct.forbidden = 1;
        loct.want = 32;
        loct.good = 130;
        loct.bad = 0;
        loct.job = 6;

        AllocForce(&loct);

        if ( _roboNewAI )
            _roboConqDelay = _roboTimeScale * (100 - _roboEpConquer) / 100;

        _roboVehicleDuty = 0;
    }
}

void NC_STACK_yparobo::buildDefense()
{
    setTarget_msg arg132;
    arg132.priority = _roboVehicleCommandID;

    if ( yparobo_func132(&arg132) )
    {
        bact_arg81 arg81;
        arg81.enrg_sum = 0;
        arg81.enrg_type = 1;

        arg132.tgt.pbact->GetSummary(&arg81);

        int v18 = arg81.enrg_sum / 2;

        if ( v18 < 15000 )
            v18 = 15000;

        if ( arg132.tgt.pbact->_bact_type == BACT_TYPES_GUN && (arg132.tgt.pbact->_weapon != -1 || arg132.tgt.pbact->_mgun != -1) )
        {
            NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( arg132.tgt.pbact );
            int a4 = gun->IsRoboGun();

            if ( !a4 )
            {
                for (NC_STACK_ypabact* &node : _world->_unitsList)
                {
                    if ( node->_bact_type == BACT_TYPES_ROBO && node->_status != BACT_STATUS_DEAD && arg132.tgt.pbact->_owner == node->_owner )
                    {
                        int v4 = node->_energy_max / 2;

                        if ( v4 < 120000 )
                            v4 = 120000;

                        v18 = v4;
                        break;
                    }
                }
            }
        }

        ypaworld_arg136 arg136;
        arg136.stPos = _position + _roboDockPos - vec3d::OY(100.0);
        arg136.vect = vec3d::OY(20000.0);
        arg136.flags = 0;

        _world->ypaworld_func136(&arg136);

        if ( _pSector->height - arg136.isectPos.y >= 50.0 )
        {
            arg132.tgt_pos.x = _position.x + 200.0;
            arg132.tgt_pos.z = _position.z + 300.0;
            arg132.priority = 0;
            arg132.tgt_type = BACT_TGT_TYPE_CELL_IND;

            SetTarget(&arg132);
        }
        else
        {
            robo_loct1 loct;
            loct.distance = (arg132.tgt.pbact->_position.XZ() - _position.XZ()).length();
            loct.tgt_pos = arg132.tgt.pbact->_position;

            int v9 = v18;
            if ( v9 >= 40000 )
                v9 = 40000;

            loct.energ = v9;
            loct.tgType = BACT_TGT_TYPE_UNIT;
            loct.tgt_bact = arg132.tgt.pbact;
            loct.command_id = _roboVehicleCommandID;
            loct.aggr = 60;

            if ( arg132.tgt.pbact->_bact_type != BACT_TYPES_TANK && arg132.tgt.pbact->_bact_type != BACT_TYPES_CAR && arg132.tgt.pbact->_bact_type != BACT_TYPES_HOVER )
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

            switch ( arg132.tgt.pbact->_bact_type )
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

            AllocForce(&loct);

            if ( _roboNewAI )
                _roboEnemyDelay = _roboTimeScale * (100 - _roboEpDefense) / 100;

            _roboVehicleDuty = 0;
        }
    }
    else
    {
        _roboVehicleDuty = 0;
    }
}

void NC_STACK_yparobo::buildRobo()
{
    setTarget_msg arg132;
    arg132.priority = _roboVehicleCommandID;
    if ( yparobo_func132(&arg132) )
    {
        ypaworld_arg136 arg136;
        arg136.stPos = _position + _roboDockPos - vec3d::OY(100.0);
        arg136.vect = vec3d::OY(20000.0);
        arg136.flags = 0;

        _world->ypaworld_func136(&arg136);

        if ( _pSector->height - arg136.isectPos.y >= 50.0 )
        {
            arg132.tgt_pos.x = _position.x + 200.0;
            arg132.tgt_pos.z = _position.z + 300.0;
            arg132.tgt_type = BACT_TGT_TYPE_CELL_IND;
            arg132.priority = 0;
            SetTarget(&arg132);
        }
        else
        {
            robo_loct1 loct;
            loct.distance = (arg132.tgt.pbact->_position.XZ() - _position.XZ()).length();
            loct.tgt_pos = arg132.tgt.pbact->_position;

            int v6 = arg132.tgt.pbact->_energy_max / 2;
            if ( v6 < 120000 )
                v6 = 120000;

            loct.energ = v6;
            loct.tgType = BACT_TGT_TYPE_UNIT;
            loct.tgt_bact = arg132.tgt.pbact;
            loct.command_id = _roboVehicleCommandID;
            loct.aggr = 60;
            loct.want = 32;
            loct.good = 852;
            loct.bad = 2;
            loct.forbidden = 1;
            loct.job = 1;
            _roboState |= ROBOSTATE_BUILDROBO;

            NC_STACK_ypabact *force = AllocForce(&loct);
            if ( force )
            {
                for (int i = 0; i < 16; i++)
                {
                    if ( !_roboAttackers[i].field_0 )
                    {
                        _roboAttackers[i].field_0 = force->_commandID;
                        _roboAttackers[i].field_4 = arg132.tgt.pbact->_commandID;
                        break;
                    }
                }
            }

            if ( _roboNewAI )
                _roboDangerDelay = _roboTimeScale * (100 - _roboEpRobo) / 100;

            _roboVehicleDuty = 0;
        }
    }
    else
    {
        _roboVehicleDuty = 0;
    }
}

void NC_STACK_yparobo::buildReconnoitre()
{
    ypaworld_arg136 arg136;
    arg136.stPos = _position + _roboDockPos - vec3d::OY(100.0);
    arg136.vect = vec3d::OY(20000.0);
    arg136.flags = 0;

    _world->ypaworld_func136(&arg136);

    if ( _pSector->height - arg136.isectPos.y >= 50.0 )
    {
        setTarget_msg arg67;
        arg67.tgt_pos.x = _position.x + 200.0;
        arg67.tgt_pos.z = _position.z + 300.0;
        arg67.tgt_type = BACT_TGT_TYPE_CELL_IND;
        arg67.priority = 0;

        SetTarget(&arg67);
    }
    else
    {
        robo_loct1 loct;
        loct.tgt_pos.x = ((_roboVehicleCellID % _secMaxX) + 0.5) * 1200.0;
        loct.tgt_pos.y = 0;
        loct.tgt_pos.z = -((_roboVehicleCellID / _secMaxX) + 0.5) * 1200.0;

        loct.energ = 10;
        loct.aggr = 25;
        loct.forbidden = 0;
        loct.want = 1024;
        loct.good = 9;
        loct.job = 5;
        loct.tgType = BACT_TGT_TYPE_CELL;
        loct.bad = 32;
        loct.distance = (loct.tgt_pos.XZ() - _position.XZ()).length();
        loct.tgt_bact = NULL;

        NC_STACK_ypabact *force = AllocForce(&loct);

        if ( force )
            force->_aggr = 25;

        if ( _roboNewAI )
            _roboExploreDelay = _roboTimeScale * (100 - _roboEpRecon) / 100;

        _roboVehicleDuty = 0;
    }
}


void NC_STACK_yparobo::AI_doWish(update_msg *arg)
{
    if ( !_roboBuildingDuty )
    {
        int found_time = 0;
        int found = 0;

        if ( _roboState & ROBOSTATE_READYPOWER )
        {
            if ( _roboPowerValue )
            {
                if ( -_roboPowerDelay > found_time )
                {
                    found = ROBOSTATE_BUILDPOWER;
                    found_time = -_roboPowerDelay;
                }
            }
        }
        if ( _roboState & ROBOSTATE_READYRADAR )
        {
            if ( _roboRadarValue )
            {
                if ( -_roboRadarDelay > found_time )
                {
                    found = ROBOSTATE_BUILDRADAR;
                    found_time = -_roboRadarDelay;
                }
            }
        }
        if ( _roboState & ROBOSTATE_READYSAFE )
        {
            if ( _roboSafetyValue )
            {
                if ( -_roboSafetyDelay > found_time )
                {
                    found = ROBOSTATE_BUILDSAFE;
                    found_time = -_roboSafetyDelay;
                }
            }
        }
        if ( _roboState & ROBOSTATE_FOUNDPLACE )
        {
            if ( _roboPositionValue )
            {
                if ( -_roboPositionDelay > found_time )
                {
                    found = ROBOSTATE_CHANGEPLACE;
                    found_time = -_roboPositionDelay;
                }
            }
        }

        if ( found_time > 0 )
        {
            if ( found == ROBOSTATE_BUILDRADAR )
            {
                _roboRadarValue = 0;
                _roboRadarCellIndex = 0;
                _roboBuildingDuty = ROBOSTATE_BUILDRADAR;
                _roboBuildingCellID = _roboRadarCellID;
                _roboBuildingCell = _roboRadarCell;
                _roboState &= ~ROBOSTATE_READYRADAR;
            }
            else if ( found == ROBOSTATE_BUILDPOWER )
            {
                _roboPowerValue = 0;
                _roboPowerCellIndex = 0;
                _roboBuildingDuty = ROBOSTATE_BUILDPOWER;
                _roboBuildingCellID = _roboPowerCellID;
                _roboState &= ~ROBOSTATE_READYPOWER;
                _roboBuildingCell = _roboPowerCell;
            }
            else if ( found == ROBOSTATE_BUILDSAFE )
            {
                _roboSafetyValue = 0;
                _roboSafetyCellIndex = 0;
                _roboBuildingCellID = _roboSafetyCellID;
                _roboBuildingCell = _roboSafetyCell;
                _roboBuildingDuty = ROBOSTATE_BUILDSAFE;
                _roboState &= ~ROBOSTATE_READYSAFE;
            }
            else if ( found == ROBOSTATE_CHANGEPLACE )
            {
                _roboPositionValue = 0;
                _roboPositionCellIndex = 0;
                _roboBuildingCellID = _roboPositionCellID;
                _roboBuildingCell = _roboPositionCell;
                _roboBuildingDuty = ROBOSTATE_CHANGEPLACE;
                _roboState &= ~ROBOSTATE_FOUNDPLACE;
            }
        }
    }

    if ( !_roboVehicleDuty && !(_roboState & ROBOSTATE_DOCKINUSE) )
    {
        int found_time = 0;
        int found = 0;

        if ( _roboState & ROBOSTATE_READYDEFENSE )
        {
            if ( _roboEnemyValue )
            {
                if ( -_roboEnemyDelay > found_time )
                {
                    found = ROBOSTATE_BUILDDEFENSE;
                    found_time = -_roboEnemyDelay;
                }
            }
        }

        if ( _roboState & ROBOSTATE_READYCONQUER )
        {
            if ( _roboConqValue )
            {
                if ( -_roboConqDelay > found_time )
                {
                    found = ROBOSTATE_BUILDCONQUER;
                    found_time = -_roboConqDelay;
                }
            }
        }

        if ( _roboState & ROBOSTATE_READYRECON )
        {
            if ( _roboExploreValue )
            {
                if ( -_roboExploreDelay > found_time )
                {
                    found = ROBOSTATE_BUILDRECON;
                    found_time = -_roboExploreDelay;
                }
            }
        }
        if ( _roboState & ROBOSTATE_READYROBO )
        {
            if ( _roboDangerValue )
            {
                if ( -_roboDangerDelay > found_time )
                {
                    found = ROBOSTATE_BUILDROBO;
                    found_time = -_roboDangerDelay;
                }
            }
        }

        if ( found_time > 0 )
        {
            if ( found == ROBOSTATE_BUILDCONQUER )
            {
                _roboConqValue = 0;
                _roboConqCellIndex = 0;
                _roboVehicleCellID = _roboConqCellID;
                _roboVehicleCell = _roboConqCell;
                _roboVehicleDuty = ROBOSTATE_BUILDCONQUER;
                _roboState &= ~ROBOSTATE_READYCONQUER;
            }
            else if ( found == ROBOSTATE_BUILDDEFENSE )
            {
                _roboVehicleDuty = ROBOSTATE_BUILDDEFENSE;
                _roboEnemyValue = 0;
                _roboVehicleCommandID = _roboEnemyCommandID;
                _roboState &= ~ROBOSTATE_READYDEFENSE;
                _roboVehicleCellID = _roboEnemyCellID;
                _roboVehicleCell = _roboEnemyCell;
            }
            else if ( found == ROBOSTATE_BUILDRECON )
            {
                _roboVehicleDuty = ROBOSTATE_BUILDRECON;
                _roboExploreValue = 0;
                _roboExploreCellIndex = 0;
                _roboVehicleCellID = _roboExploreCellID;
                _roboVehicleCell = _roboExploreCell;
                _roboState &= ~ROBOSTATE_READYRECON;
            }
            else if ( found == ROBOSTATE_BUILDROBO )
            {
                _roboDangerValue = 0;
                _roboVehicleCommandID = _roboDangerCommandID;
                _roboVehicleCellID = _roboDangerCellID;
                _roboVehicleCell = _roboDangerCell;
                _roboVehicleDuty = ROBOSTATE_BUILDROBO;
                _roboState &= ~ROBOSTATE_READYROBO;
            }
        }
    }

    switch (_roboBuildingDuty)
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

    switch (_roboVehicleDuty)
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
    if ( _roboDockUser )
    {
        int v4 = 0;
        for( NC_STACK_ypabact* &node : _kidList )
        {
            if (_roboDockUser == node->_commandID)
            {
                v4 = 1;
                break;
            }
        }

        if ( !v4 )
        {
            _roboDockTime = 0;
            _roboDockUser = 0;
            _roboState &= ~ROBOSTATE_DOCKINUSE;
        }
    }

    if ( _roboDockTime > 0 )
    {
        _roboDockTime -= arg->frameTime;
        if ( _roboDockTime <= 0 )
        {
            _roboDockTime = 0;

            if ( !_roboDockUser )
                _roboState &= ~ROBOSTATE_DOCKINUSE;
        }
    }

    if ( _roboDockTime <= 0 && !_roboDockUser )
        _roboState &= ~ROBOSTATE_DOCKINUSE;
}

void NC_STACK_yparobo::AI_solveTrouble(update_msg *arg)
{
    if ( (float)_energy / (float)_energy_max < 0.2 && (arg->gTime - _roboTestEnemyTime) > 20000 )
    {
        _roboTestEnemyTime = arg->gTime;

        int v3 = (float)_energy / (float)_energy_max < 0.1;

        for (NC_STACK_ypabact* &unit : _kidList )
        {
            if ( unit->_status == BACT_STATUS_NORMAL || unit->_status == BACT_STATUS_IDLE )
            {
                if ( unit->_bact_type != BACT_TYPES_GUN )
                {
                    int v7;

                    if ( v3 )
                    {
                        v7 = 1;
                        unit->_status_flg &= ~BACT_STFLAG_UNUSE;
                    }
                    else
                    {
                        v7 = !(unit->_status_flg & BACT_STFLAG_UNUSE) && !unit->_secndTtype;
                    }

                    float v10 = unit->_position.x - _position.x;
                    float v11 = unit->_position.z - _position.z;

                    if ( (v10 * v10 + v11 * v11) < 4320000.0 && unit->_primTtype == BACT_TGT_TYPE_UNIT && this == unit->_primT.pbact )
                    {
                        if ( v3 )
                        {
                            unit->setBACT_aggression(99);
                            unit->_status_flg &= ~BACT_STFLAG_ESCAPE;
                        }
                        else
                        {
                            unit->setBACT_aggression(60);
                        }
                    }

                    if ( v7 )
                    {
                        setTarget_msg arg67;

                        arg67.tgt_type = BACT_TGT_TYPE_UNIT;
                        arg67.priority = 0;
                        arg67.tgt.pbact = this;

                        unit->SetTarget(&arg67);
                    }
                }
            }
        }
    }
}

void NC_STACK_yparobo::AI_clearAttackerSlots()
{
    if ( _clock - _roboAttackersClearTime > 4000 )
    {
        _roboAttackersClearTime = _clock;
        for (int i = 0; i < 16; i++)
        {
            if ( _roboAttackers[i].field_0 )
            {
                int v6 = 0;
                for (NC_STACK_ypabact* &node : _kidList )
                {
                    if (node->_commandID == _roboAttackers[i].field_0)
                    {
                        v6 = 1;
                        break;
                    }
                }

                if ( !v6 )
                {
                    _roboAttackers[i].field_0 = 0;
                    _roboAttackers[i].field_4 = 0;
                }
            }
        }
    }
}

void NC_STACK_yparobo::searchEnemyRobo()
{
    for (NC_STACK_ypabact* &unit : _world->_unitsList)
    {
        if ( unit->_status != BACT_STATUS_DEAD &&
             unit != this && unit->_bact_type == BACT_TYPES_ROBO )
        {
            if ( (1 << _owner) & unit->_pSector->view_mask )
            {
                NC_STACK_ypabact *sel = NULL;
                float selfl = 0.0;

                for (NC_STACK_ypabact* &kbct : _kidList )
                {
                    if (kbct->_status != BACT_STATUS_DEAD)
                    {
                        float v14 = (kbct->_position.XZ() - unit->_position.XZ()).square();

                        if ( !sel || v14 < selfl )
                        {
                            sel = kbct;
                            selfl = v14;
                        }
                    }
                }

                if ( sel )
                {
                    robo_arg134 v12;

                    v12.field_4 = 6;
                    v12.field_8 = unit->_owner;
                    v12.field_14 = 48;
                    v12.unit = sel;
                    v12.field_C = 0;
                    v12.field_10 = 0;

                    placeMessage(&v12);
                }
            }
        }
    }
}

void NC_STACK_yparobo::usersRoboEnergyCheck()
{
    float v1 = (float)_energy / (float)_energy_max;

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
        v2.unit = this;

        placeMessage(&v2);
    }
}

void NC_STACK_yparobo::checkCommander()
{
    bool userRobo =  (this == _world->getYW_userHostStation());

    float v31;

    for ( NC_STACK_ypabact* &commander : _kidList )
    {
        if ( commander->_status != BACT_STATUS_DEAD )
        {
            if ( _roboDockUser == commander->_commandID && _roboDockUser )
            {
                InitForce(commander);
            }
            else
            {
                if ( commander->_bact_type != BACT_TYPES_GUN )
                {
                    if ( !(commander->_status_flg & BACT_STFLAG_ESCAPE) )
                    {
                        if ( commander->GetFightMotivation(&v31) )
                        {
                            if ( v31 < 0.5 && userRobo )
                            {
                                bact_arg81 arg81;
                                arg81.enrg_sum = 0;
                                arg81.enrg_type = 3;

                                commander->GetSummary(&arg81);

                                int v18 = arg81.enrg_sum;

                                arg81.enrg_sum = 0;
                                arg81.enrg_type = 5;
                                commander->GetSummary(&arg81);

                                if ( v18 < arg81.enrg_sum )
                                {
                                    robo_arg134 arg134;
                                    arg134.field_8 = 20000;
                                    arg134.field_10 = 0;
                                    arg134.field_C = 0;
                                    arg134.field_4 = 18;
                                    arg134.unit = commander;
                                    arg134.field_14 = 42;

                                    placeMessage(&arg134);
                                }
                            }

                            if ( !userRobo )
                            {
                                if ( _clock - _roboAttackersTime > 1000 )
                                {
                                    _roboAttackersTime = _clock;

                                    for (int i = 0; i < 16; i++)
                                    {
                                        if ( _roboAttackers[i].field_0 == commander->_commandID)
                                        {
                                            sb_0x4a7010__sub1(commander, &_roboAttackers[i]);
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            commander->_status_flg |= BACT_STFLAG_ESCAPE;

                            for (NC_STACK_ypabact* &nod : commander->_kidList)
                                nod->_status_flg |= BACT_STFLAG_ESCAPE;

                            commander->_status_flg &= ~(BACT_STFLAG_WAYPOINT | BACT_STFLAG_WAYPOINTCCL);
                            commander->_waypoints_count = 0;
                            commander->_status_flg &= ~BACT_STFLAG_UNUSE;

                            
                            for (NC_STACK_ypabact* &nod : commander->_kidList)
                                nod->_status_flg &= ~BACT_STFLAG_UNUSE;

                            if ( IsNeedsWaypoints(commander) )
                            {
                                bact_arg124 arg124;
                                arg124.steps_cnt = 32;
                                arg124.from = commander->_position.XZ();
                                arg124.to = _position.XZ();
                                arg124.field_12 = 1;

                                if ( ! commander->PathFinder(&arg124) )
                                    break;

                                if ( arg124.steps_cnt > 1 )
                                {
                                    arg124.steps_cnt = 32;
                                    commander->SetPath(&arg124);

                                    commander->_m_cmdID = _commandID;
                                    commander->_m_owner = _owner;
                                }
                                else
                                {
                                    setTarget_msg arg67;
                                    arg67.tgt.pbact = this;
                                    arg67.priority = 0;
                                    arg67.tgt_type = BACT_TGT_TYPE_UNIT;
                                    commander->SetTarget(&arg67);
                                }
                            }
                            else
                            {
                                setTarget_msg arg67;
                                arg67.tgt.pbact = this;
                                arg67.tgt_type = BACT_TGT_TYPE_UNIT;
                                arg67.priority = 0;
                                commander->SetTarget(&arg67);
                            }

                            if ( userRobo )
                            {
                                robo_arg134 arg134;
                                arg134.field_8 = commander->_commandID;
                                arg134.field_10 = 0;
                                arg134.field_C = 0;
                                arg134.field_4 = 9;
                                arg134.unit = commander;
                                arg134.field_14 = 40;

                                placeMessage(&arg134);

                                if ( _world->isNetGame  )
                                {
                                    bact_arg90 arg90;
                                    arg90.unit = commander;
                                    arg90.field_8 = 1;
                                    arg90.ret_unit = NULL;
                                    GetSectorTarget(&arg90);

                                    if ( arg90.ret_unit )
                                    {
                                        uamessage_logmsg logMsg;
                                        logMsg.msgID = UAMSG_LOGMSG;
                                        logMsg.owner = _owner;
                                        logMsg.sender = arg90.ret_unit->_gid;
                                        logMsg.senderOwner = arg90.ret_unit->_owner;
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
                                        _world->ypaworld_func181(&arg181);
                                    }
                                }
                            }
                            else
                            {
                                for ( NC_STACK_ypabact *v33 : commander->_attackersList )
                                {
                                    if (_world->UserRobo->_owner == v33->_owner)
                                    {                                       
                                        if ( v33->_bact_type == BACT_TYPES_MISSLE ) //If missile
                                        {
                                            NC_STACK_ypamissile *miss = dynamic_cast<NC_STACK_ypamissile *>(v33);
                                            v33 = miss->getMISS_launcher(); //Get emitter bact
                                        }

                                        if ( v33->_host_station != v33->_parent &&
                                             v33->_parent )
                                            v33 = v33->_parent;

                                        robo_arg134 arg134;
                                        arg134.field_8 = 0;
                                        arg134.field_10 = 0;
                                        arg134.field_C = 0;
                                        arg134.field_4 = 19;
                                        arg134.unit = v33;
                                        arg134.field_14 = 34;
                                        placeMessage(&arg134);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    else if ( commander->GetFightMotivation(&v31) )
                    {
                        if ( commander->_owner == commander->_pSector->owner && commander->_pSector->energy_power )
                        {
                            setTarget_msg arg67;
                            arg67.tgt_type = BACT_TGT_TYPE_CELL;
                            arg67.priority = 0;
                            arg67.tgt_pos = commander->_position;

                            commander->SetTarget(&arg67);
                        }

                        commander->_status_flg &= ~BACT_STFLAG_ESCAPE;

                        for (NC_STACK_ypabact* &nod : commander->_kidList)
                            nod->_status_flg &= ~BACT_STFLAG_ESCAPE;

                        if ( userRobo )
                        {
                            robo_arg134 arg134;
                            arg134.field_8 = commander->_commandID;
                            arg134.field_4 = 10;
                            arg134.field_10 = 0;
                            arg134.field_C = 0;
                            arg134.unit = commander;
                            arg134.field_14 = 28;

                            placeMessage(&arg134);
                        }
                    }
                }
            }
        }
    }
}

void NC_STACK_yparobo::checkDanger()
{
    int v11 = 0;
    int v12;

    for(int i = -1; i < 2; i++)
    {
        for(int j = -1; j < 2; j++)
        {
            int v4 = _sectY + j;
            int v5 = _sectX + i;

            if ( v5 > 0 && v5 < _secMaxX - 2 && v4 > 0 && v4 < _secMaxY - 2 )
            {
                for ( NC_STACK_ypabact* &v7 : _pSector[i + j * _secMaxX].unitsList )
                {
                    if (v7->_owner != _owner && v7->_status != BACT_STATUS_DEAD && v7->_weapon != -1 && v7->_mgun != -1)
                    {
                        v11 = 1;
                        v12 = v7->_commandID;
                        break;
                    }
                }
            }

        }
    }

    if ( v11 )
    {
        if ( !(_roboState & ROBOSTATE_DANGER) && _roboState & ROBOSTATE_PLAYERROBO )
        {
            robo_arg134 arg134;
            arg134.field_4 = 13;
            arg134.field_8 = v12;
            arg134.field_10 = 0;
            arg134.field_C = 0;
            arg134.unit = this;
            arg134.field_14 = 99;

            placeMessage(&arg134);
        }

        _roboState |= ROBOSTATE_DANGER;
    }
    else
    {
        _roboState &= ~ROBOSTATE_DANGER;
    }
}

int NC_STACK_yparobo::yparobo_func70__sub6__sub0(cellArea *cell)
{
    int xx = _roboRadarCellIndex % _secMaxX;
    int yy = _roboRadarCellIndex / _secMaxX;

    if ( !(cell->view_mask & (1 << _owner)) )
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

    if ( cell->owner != _owner )
        return -1;

    if ( cell->w_type )
        return -1;

    float v8 = sqrt( POW2(_sectY - yy) + POW2(_sectX - xx) );

    float v14 = ( v8 <= 0.01 ? 500.0 : 1000.0 / v8 );

    int v9 = 0;

    for (int i = 0; i < 8; i++)
    {
        if ( !((1 << _owner) & cell[ word_5182AE[i].x + word_5182AE[i].y * _secMaxX ].view_mask ) )
        {
            v9 = 1;
            v14 += 10.0;
        }
    }

    if ( !v9 || xx <= 1 || xx >= _secMaxX - 2 || yy <= 1 || yy >= _secMaxY - 2 )
        return -1;

    return v14;
}

int NC_STACK_yparobo::yparobo_func70__sub6__sub1(cellArea *cell)
{
    int xx = _roboExploreCellIndex % _secMaxX;
    int yy = _roboExploreCellIndex / _secMaxX;

    if ( cell->view_mask & (1 << _owner) )
        return -1;

    float v13 = sqrt(POW2(_sectY - yy) + POW2(_sectX - xx));

    if ( v13 <= 0.01 )
        v13 = 0.0;

    for (int i = 0; i < 8; i++)
    {
        if ( !((1 << _owner) & cell[ word_5182AE[i].x + word_5182AE[i].y * _secMaxX ].view_mask ) )
        {
            v13 += 5.0;
        }
    }

    return v13;
}

int NC_STACK_yparobo::yparobo_func70__sub6__sub2(cellArea *cell)
{
    int xx = _roboSafetyCellIndex % _secMaxX;
    int yy = _roboSafetyCellIndex / _secMaxX;

    if ( !(cell->view_mask & (1 << _owner)) )
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

    if ( cell->owner != _owner || cell->w_type )
        return -1;

    float v9 = sqrt( POW2(_sectY - yy) + POW2(_sectX - xx) );

    float v14;

    if ( v9 <= 0.01 )
        v14 = 500.0;
    else
        v14 = 1000.0 / v9;

    for (int i = 0; i < 8; i++)
    {
        cellArea *cll = &cell[ word_5182AE[i].x + word_5182AE[i].y * _secMaxX ];

        if ( cll->w_type && _owner == cll->owner)
            v14 += 5.0;

    }

    return v14;
}

int NC_STACK_yparobo::yparobo_func70__sub6__sub3(cellArea *cell)
{
    int xx = _roboPowerCellIndex % _secMaxX;
    int yy = _roboPowerCellIndex / _secMaxX;

    if ( !(cell->view_mask & (1 << _owner)) )
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

    if ( cell->owner != _owner )
        return -1;

    float v9 = sqrt( POW2(_sectY - yy) + POW2(_sectX - xx) );
    float v26 = v9 <= 0.01 ? 500.0 : 1000.0 / v9;

    if ( v9 > 8.0 )
        return -1;

    float v27 = v26 + (255 - cell->energy_power) / 3.0;

    for (int i = 0; i < 8; i++)
    {
        cellArea *cll = &cell[ word_5182AE[i].x + word_5182AE[i].y * _secMaxX ];

        if ( cll->owner == _owner )
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

    v27 -= 5.0 * cell->unitsList.size();

    return v27;
}

int NC_STACK_yparobo::yparobo_func70__sub6__sub4(cellArea *cell)
{
    int xx = _roboConqCellIndex % _secMaxX;
    int yy = _roboConqCellIndex / _secMaxX;

    if ( cell->owner == _owner )
        return -1;

    float v7 = sqrt( POW2(_sectY - yy) + POW2(_sectX - xx) );
    float v25;

    if ( v7 <= 0.01 )
        v25 = 1500.0;
    else
        v25 = 1000.0 / v7;

    if ( (1 << _owner) & cell->view_mask )
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

        if ( tx >= 1 && ty >= 1 && tx <= _secMaxX - 2 && ty <= _secMaxY - 2)
        {
            if ( cell[ word_5182AE[i].x + word_5182AE[i].y * _secMaxX ].owner == _owner )
                v25 = v25 + 3.0;
        }
    }

    if ( (1 << _owner) & cell->view_mask )
    {
        for ( NC_STACK_ypabact* &bct : cell->unitsList )
        {
            if ( bct->_owner != _owner && bct->_owner)
            {
                if ( bct->_bact_type != BACT_TYPES_MISSLE)
                {
                    if ( bct->_bact_type == BACT_TYPES_ROBO)
                        v25 += 100.0;
                    else
                        v25 += 5.0;
                }
            }
        }
    }

    return v25;
}

int NC_STACK_yparobo::yparobo_func70__sub6__sub8()
{
    int num = _secMaxX * _secMaxY;

    int v4 = 0;

    for (int i = 0; i < num; i++)
    {
        cellArea *cll = &_roboPCells[i];

        if ( cll->w_type == 3 )
        {
            for ( NC_STACK_ypabact* &bct : cll->unitsList )
            {
                if ( bct->_bact_type == BACT_TYPES_GUN && bct->_status != BACT_STATUS_DEAD && _owner == bct->_owner )
                {
                    NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( bct );
                    int a4 = gun->IsRoboGun();

                    if ( a4 == 0 && ( bct->_weapon != -1 || bct->_mgun != -1 ) )
                    {
                        v4++;
                        break;
                    }
                }
            }
        }
    }

    return v4;
}

int NC_STACK_yparobo::yparobo_func70__sub6__sub9()
{
    int num = _secMaxX * _secMaxY;

    int v4 = 0;

    for (int i = 0; i < num; i++)
    {
        cellArea *cll = &_roboPCells[i];

        if ( cll->w_type == 3 )
        {
            for (NC_STACK_ypabact* &bct : cll->unitsList )
            {
                if ( bct->_bact_type == BACT_TYPES_GUN && bct->_status != BACT_STATUS_DEAD && _owner == bct->_owner )
                {
                    NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( bct );
                    int a4 = gun->IsRoboGun();

                    if ( a4 == 0 && bct->_weapon == -1 && bct->_mgun == -1 )
                    {
                        v4++;
                        break;
                    }
                }
            }
        }
    }

    return v4;
}

int NC_STACK_yparobo::yparobo_func70__sub6__sub10()
{
    int num = _secMaxX * _secMaxY;

    int v4 = 0;

    for (int i = 0; i < num; i++)
    {
        if ( _roboPCells[i].owner == _owner )
            v4++;
    }

    return v4;
}

int NC_STACK_yparobo::yparobo_func70__sub6__sub11()
{
    int num = _secMaxX * _secMaxY;

    int v4 = 0;

    for (int i = 0; i < num; i++)
    {
        if ( !((1 << _owner) & _roboPCells[i].view_mask) )
            v4++;
    }

    return v4;
}

int NC_STACK_yparobo::yparobo_func70__sub6__sub12()
{
    if (_kidRef)
    {
        for(NC_STACK_ypabact* &node : *_kidRef.PList())
        {
            if ( node->_bact_type == BACT_TYPES_ROBO)
            {
                if ( _owner != node->_owner )
                {
                    int v9 = _sectX - node->_sectX;
                    int v10 = _sectY - node->_sectY;

                    if ( abs(v9) < 3 && abs(v10) < 3 )
                        return 1;
                }
            }
        }
    }

    return 0;
}

int NC_STACK_yparobo::yparobo_func70__sub6__sub13()
{
    for (int i = -2; i <= 2; i++)
    {
        for (int j = -2; j <= 2; j++)
        {
            int xx = _sectX + i;
            int yy = _sectY + j;

            if ( xx > 0 && xx < _secMaxX - 1 && yy > 0 && yy < _secMaxY - 1 )
            {
                for ( NC_STACK_ypabact* &bct : _pSector[ i + j * _secMaxX ].unitsList )
                {
                    if ( bct->_owner )
                    {
                        if ( bct->_owner != _owner )
                        {
                            if ( bct->_bact_type != BACT_TYPES_MISSLE && bct->_bact_type != BACT_TYPES_ROBO && bct->_status != BACT_STATUS_DEAD )
                                return 1;
                        }
                    }
                }
            }
        }
    }

    return 0;
}


float NC_STACK_yparobo::sub_4F4C6C(NC_STACK_ypabact *bact, int a4, int a3)
{
    if ( bact->_status_flg & BACT_STFLAG_DEATH1 )
        return -1.0;

    float v8 = sqrt( POW2(a3 - bact->_sectY) + POW2(a4 - bact->_sectX) );

    if (v8 != 0.0)
        v8 = 100.0 / v8;
    else
        v8 = 200.0;

    if ( bact->_pSector->owner == _owner )
        v8 += 100.0;

    return v8;
}

int NC_STACK_yparobo::yparobo_func70__sub6__sub5(int *a2, int *px, int *py)
{
    float v32 = -0.5;

    int v29 = 0;
    
    if (_kidRef)
    {
        for(NC_STACK_ypabact* &node : *_kidRef.PList())
        {
            if ( node->_owner != _owner  &&  node->_owner  &&  node->_status != BACT_STATUS_DEAD )
            {
                if ( abs(node->_sectX - _sectX) <= 2 &&
                        abs(node->_sectY - _sectY) <= 2 )
                {
                    *a2 = node->_commandID;
                    *px = node->_sectX;
                    *py = node->_sectY;

                    return 500;
                }

                for(NC_STACK_ypabact* &ndbct : node->_kidList)
                {

                    if ( (ndbct->_gid & 0xF) == _roboEnemyDiv )
                    {
                        int v26 = 0;

                        if ( ndbct->_bact_type == BACT_TYPES_GUN )
                        {
                            NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( ndbct );
                            v26 = gun->IsRoboGun();
                        }

                        if ( (1 << _owner) & ndbct->_pSector->view_mask )
                        {
                            if ( ndbct->_status != BACT_STATUS_DEAD && !v26 )
                            {
                                float tmp = sub_4F4C6C(ndbct, _sectX, _sectY);
                                if ( tmp > v32 )
                                {
                                    v32 = tmp;
                                    v29 = ndbct->_commandID;
                                    *px = ndbct->_sectX;
                                    *py = ndbct->_sectY;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    _roboEnemyDiv++;

    if ( _roboEnemyDiv > 15 )
        _roboEnemyDiv = 0;

    if ( v32 < 0.0 )
    {
        *a2 = 0;
        return -1;
    }

    *a2 = v29;
    return v32;
}

int NC_STACK_yparobo::yparobo_func70__sub6__sub6(int *a2, int *px, int *py)
{
    float v32 = -0.5;
    int v21 = 0;

    if (_kidRef)
    {
        for(NC_STACK_ypabact* &node : *_kidRef.PList())
        {
            if ( node->_owner != _owner && node->_owner && node->_status != BACT_STATUS_DEAD )
            {
                if ( (1 << _owner) & node->_pSector->view_mask )
                {
                    float v13 = sub_4F4C6C(node, _sectX, _sectY);
                    if ( v13 > v32 )
                    {
                        v21 = node->_commandID;
                        v32 = v13;
                        *px = node->_sectX;
                        *py = node->_sectY;
                    }
                }
            }
        }
    }

    if ( v32 < 0.0 )
    {
        *a2 = 0;
        return -1;
    }

    *a2 = v21;
    return v32;
}


int NC_STACK_yparobo::sub_4F4E48(int x, int y)
{
    float v14 = 0.0;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            cellArea *cll = &_roboPCells[(y + j) * _secMaxX + x + i];

            yw_arg176 arg176;
            arg176.owner = cll->owner;

            _world->ypaworld_func176(&arg176);

            float v15;

            if ( i || j )
                v15 = 1.0;
            else
                v15 = 3.0;

            if ( _owner == cll->owner )
                v14 += cll->energy_power * arg176.field_4 + v15;
            else
                v14 -= cll->energy_power * arg176.field_4 + v15;

            for ( NC_STACK_ypabact* &bct : cll->unitsList )
            {
                float v16 = 0;

                if ( bct->_bact_type == BACT_TYPES_ROBO && bct != this )
                {
                    v16 = 5.0;
                }
                else
                {
                    if ( bct->_bact_type == BACT_TYPES_GUN )
                    {
                        NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( bct );

                        if ( !gun->IsRoboGun() )
                            v16 = 1.0;
                    }
                }

                if ( v16 > 0.0 )
                {
                    if ( bct->_owner == _owner )
                        v14 += v16;
                    else
                        v14 -= v16;
                }
            }
        }
    }

    return v14;
}

int NC_STACK_yparobo::yparobo_func70__sub6__sub7()
{
    int xx = _roboPositionCellIndex % _secMaxX;
    int yy = _roboPositionCellIndex / _secMaxX;

    int a1_4 = sub_4F4E48(_sectX, _sectY);
    int v14 = sub_4F4E48(xx, yy);

    float v12 = sqrt( POW2(_sectY - yy) + POW2(_sectX - xx) );

    if ( v12 > 0.0 && a1_4 < v14 )
        v14 = ((float)v14 * (1.0 - v12 * 0.8 / 91.0));

    if ( v14 <= a1_4 )
        return -1;

    return v14;
}

void NC_STACK_yparobo::AI_checkWorld(update_msg *arg)
{
    int v6 = 1;
    if ( _roboNewAI )
    {
        _roboConqDelay -= arg->frameTime;
        if ( _roboConqDelay > 0 )
            v6 = 0;
    }

    robo_arg128 arg128;

    if ( _roboEpConquer && v6 )
    {
        int v89 = (_secMaxY + _secMaxX) / 20 + 1;

        for (int i = v89; i > 0; i--)
        {
            if ( _secMaxX * _secMaxY == _roboConqCellIndex )
            {
                _roboConqCellIndex = 0;
                _roboState |= ROBOSTATE_READYCONQUER;
                return;
            }

            cellArea *pcell = &_roboPCells[ _roboConqCellIndex ];

            int xx = _roboConqCellIndex % _secMaxX;
            int yy = _roboConqCellIndex / _secMaxX;

            if ( xx && xx != _secMaxX - 1 && yy && yy != _secMaxY - 1 )
            {
                int v12 = yparobo_func70__sub6__sub4(pcell);
                if ( v12 > _roboConqValue )
                {
                    arg128.flags = 2;
                    arg128.tgType = BACT_TGT_TYPE_CELL;
                    arg128.tgt_pos.x = (xx + 0.5) * 1200.0;
                    arg128.tgt_pos.z = -(yy + 0.5) * 1200.0;

                    yparobo_func128(&arg128);

                    if ( !arg128.comm_bacto )
                    {
                        _roboConqValue = v12;
                        _roboConqCellID = _roboConqCellIndex;
                        _roboConqCell = pcell;
                        _roboConqTime = arg->gTime;
                    }
                }
            }

            _roboConqCellIndex++;
        }
    }
    else
    {
        _roboConqCellIndex = 0;
        _roboConqValue = 0;
        _roboState &= ~ROBOSTATE_READYCONQUER;
    }

    v6 = 1;
    if ( _roboNewAI )
    {
        _roboEnemyDelay -= arg->frameTime;

        if ( _roboEnemyDelay > 0 )
            v6 = 0;
    }

    if ( yparobo_func70__sub6__sub13() )
        v6 = 1;

    if ( _roboEpDefense && v6 )
    {
        int v91, xx, yy;

        int v17 = yparobo_func70__sub6__sub5(&v91, &xx, &yy);
        if ( v17 > _roboEnemyValue )
        {
            arg128.flags = 2;
            arg128.tgType = BACT_TGT_TYPE_UNIT;
            arg128.prim_comm_id = v91;
            yparobo_func128(&arg128);

            if ( !arg128.comm_bacto )
            {
                _roboEnemyValue = v17;
                _roboEnemyCommandID = v91;
                _roboEnemyTime = arg->gTime;
                _roboEnemyCellID = yy * _secMaxX + xx;
            }
        }
        _roboState |= ROBOSTATE_READYDEFENSE;
    }
    else
    {
        _roboEnemyValue = 0;
        _roboState &= ~ROBOSTATE_READYDEFENSE;
    }

    v6 = 1;
    if ( _roboNewAI )
    {
        _roboDangerDelay -= arg->frameTime;
        if ( _roboDangerDelay > 0 )
            v6 = 0;
    }

    if ( yparobo_func70__sub6__sub12() )
        v6 = 1;

    if ( _roboEpRobo && v6 )
    {
        int v91, xx, yy;
        int v22 = yparobo_func70__sub6__sub6(&v91, &xx, &yy);

        if ( v22 > _roboDangerValue )
        {
            robo_arg128 arg128_1;
            arg128_1.flags = 2;
            arg128_1.tgType = BACT_TGT_TYPE_UNIT;
            arg128_1.prim_comm_id = v91;

            yparobo_func128(&arg128_1);

            if ( !arg128_1.comm_bacto )
            {
                _roboDangerValue = v22;
                _roboDangerCommandID = v91;
                _roboDangerTime = arg->gTime;
                _roboDangerCellID = _secMaxX * yy + xx;
            }
        }
        _roboState |= ROBOSTATE_READYROBO;
    }
    else
    {
        _roboDangerValue = 0;
        _roboState &= ~ROBOSTATE_READYROBO;
    }

    v6 = 1;
    if ( _roboNewAI )
    {
        _roboPositionDelay -= arg->frameTime;
        if ( _roboPositionDelay > 0 )
            v6 = 0;
    }

    if ( _roboEpChangePlace && v6 )
    {
        int v25 = (_secMaxX + _secMaxY) / 10 + 1;

        for (int i = v25; i > 0; i--)
        {
            if ( _secMaxX * _secMaxY == _roboPositionCellIndex )
            {
                _roboPositionCellIndex = 0;
                _roboState |= ROBOSTATE_FOUNDPLACE;
                return;
            }

            int xx = _roboPositionCellIndex % _secMaxX;
            int yy = _roboPositionCellIndex / _secMaxX;

            if ( xx && xx != _secMaxX - 1 && yy && yy != _secMaxY - 1 )
            {
                int v34 = yparobo_func70__sub6__sub7();
                if ( v34 > _roboPositionValue )
                {
                    _roboPositionValue = v34;
                    _roboPositionCellID = _roboPositionCellIndex;
                    _roboPositionTime = arg->gTime;
                }
            }
            _roboPositionCellIndex++;
        }
    }
    else
    {
        _roboPositionValue = 0;
        _roboPositionCellIndex = 0;
        _roboState &= ~ROBOSTATE_FOUNDPLACE;
    }

    yw_arg176 v85;
    v85.owner = _owner;
    _world->ypaworld_func176(&v85);

    v6 = 1;
    if ( _roboNewAI )
    {
        _roboPowerDelay -= arg->frameTime;
        if ( _roboPowerDelay > 0 )
            v6 = 0;
    }

    if ( v85.field_4 == 0.0 )
    {
        if ( _roboEpPower )
            v6 = 1;
    }

    float v93;
    if ( _roboEpPower )
        v93 = 100.0 / (float)_roboEpPower * 0.9;
    else
        v93 = 0.89999998;

    if ( _roboEpPower && (v85.field_8 > v93 || v85.field_4 < 0.0001) && v6 )
    {
        int v99 = (_secMaxX + _secMaxY) / 20 + 1;
        for (int i = v99; i > 0; i--)
        {
            if ( _secMaxX * _secMaxY == _roboPowerCellIndex )
            {
                _roboPowerCellIndex = 0;
                _roboState |= ROBOSTATE_READYPOWER;
                return;
            }
            int xx = _roboPowerCellIndex % _secMaxX;
            int yy = _roboPowerCellIndex / _secMaxX;

            if ( xx && xx != _secMaxX - 1 && yy && yy != _secMaxY - 1 )
            {
                cellArea *pcell = &_roboPCells[_roboPowerCellIndex];
                int v46 = yparobo_func70__sub6__sub3(pcell);
                if ( v46 > _roboPowerValue )
                {
                    _roboPowerValue = v46;
                    _roboPowerCell = pcell;
                    _roboPowerCellID = _roboPowerCellIndex;
                    _roboPowerTime = arg->gTime;
                }
            }
            _roboPowerCellIndex++;
        }
    }
    else
    {
        _roboPowerValue = 0;
        _roboPowerCellIndex = 0;
        _roboState &= ~ROBOSTATE_READYPOWER;
    }

    v6 = 1;
    if ( _roboNewAI )
    {
        _roboRadarDelay -= arg->frameTime;
        if ( _roboRadarDelay > 0 )
            v6 = 0;
    }

    if ( _roboEpRadar && (yparobo_func70__sub6__sub11() / 64 > yparobo_func70__sub6__sub9()) && v6 )
    {
        int v48 = (_secMaxY + _secMaxX) / 20 + 1;

        for(int i = v48; i > 0; i--)
        {
            if ( _secMaxX * _secMaxY == _roboRadarCellIndex )
            {
                _roboRadarCellIndex = 0;
                _roboState |= ROBOSTATE_READYRADAR;
                return;
            }

            int xx = _roboRadarCellIndex % _secMaxX;
            int yy = _roboRadarCellIndex / _secMaxX;

            if ( xx && xx != _secMaxX - 1 && yy && yy != _secMaxY - 1 )
            {
                cellArea *pcell = &_roboPCells[_roboRadarCellIndex];

                int v58 = yparobo_func70__sub6__sub0(pcell);

                if ( v58 > _roboRadarValue )
                {
                    _roboRadarValue = v58;
                    _roboRadarCell = pcell;
                    _roboRadarCellID = _roboRadarCellIndex;
                    _roboRadarTime = arg->gTime;
                }
            }
            _roboRadarCellIndex++;
        }
    }
    else
    {
        _roboRadarCellIndex = 0;
        _roboRadarValue = 0;
        _roboState &= ~ROBOSTATE_READYRADAR;
    }


    v6 = 1;
    if ( _roboNewAI )
    {
        _roboSafetyDelay -= arg->frameTime;
        if ( _roboSafetyDelay > 0 )
            v6 = 0;
    }

    int v92;
    if ( _roboEpSafety )
        v92 = 10 * (100 / _roboEpSafety);
    else
        v92 = 10;

    if ( _roboEpSafety && (yparobo_func70__sub6__sub10() / v92) >= yparobo_func70__sub6__sub8() && v6 )
    {
        int v60 = (_secMaxY + _secMaxX) / 20 + 1;

        for(int i = v60; i > 0; i--)
        {
            if ( _secMaxX * _secMaxY == _roboSafetyCellIndex )
            {
                _roboSafetyCellIndex = 0;
                _roboState |= ROBOSTATE_READYSAFE;
                return;
            }

            int xx = _roboSafetyCellIndex % _secMaxX;
            int yy = _roboSafetyCellIndex / _secMaxX;

            if ( xx && xx != _secMaxX - 1 && yy && yy != _secMaxY - 1 )
            {
                cellArea *pcell = &_roboPCells[_roboSafetyCellIndex];
                int v71 = yparobo_func70__sub6__sub2(pcell);

                if ( v71 > _roboSafetyValue )
                {
                    _roboSafetyValue = v71;
                    _roboSafetyCell = pcell;
                    _roboSafetyCellID = _roboSafetyCellIndex;
                    _roboSafetyTime = arg->gTime;
                }
            }
            _roboSafetyCellIndex++;
        }
    }
    else
    {
        _roboSafetyValue = 0;
        _roboSafetyCellIndex = 0;
        _roboState &= ~ROBOSTATE_READYSAFE;
    }


    v6 = 1;
    if ( _roboNewAI )
    {
        _roboExploreDelay -= arg->frameTime;
        if ( _roboExploreDelay > 0 )
            v6 = 0;
    }

    if ( _roboEpRecon && v6 )
    {
        int v90 = (_secMaxY + _secMaxX) / 20 + 1;

        for (int i = v90; i > 0; i-- )
        {
            if ( _secMaxX * _secMaxY == _roboExploreCellIndex )
            {
                _roboExploreCellIndex = 0;
                _roboState |= ROBOSTATE_READYRECON;
                return;
            }

            int xx = _roboExploreCellIndex % _secMaxX;
            int yy = _roboExploreCellIndex / _secMaxX;

            if ( xx > 1 && xx < _secMaxX - 2 && yy > 1 && yy < _secMaxY - 2 )
            {
                cellArea *pcell = &_roboPCells[_roboExploreCellIndex];
                int v81 = yparobo_func70__sub6__sub1(pcell);

                if ( v81 > _roboExploreValue )
                {
                    robo_arg128 arg128_1;
                    arg128_1.tgType = BACT_TGT_TYPE_CELL;
                    arg128_1.flags = 2;
                    arg128_1.tgt_pos.x = (xx + 0.5) * 1200.0;
                    arg128_1.tgt_pos.z = -(yy + 0.5) * 1200.0;

                    yparobo_func128(&arg128_1);

                    if ( !arg128_1.comm_bacto )
                    {
                        _roboExploreValue = v81;
                        _roboExploreCellID = _roboExploreCellIndex;
                        _roboExploreCell = pcell;
                        _roboExploreTime = arg->gTime;
                    }
                }
            }
            _roboExploreCellIndex++;
        }
    }
    else
    {
        _roboExploreValue = 0;
        _roboExploreCellIndex = 0;
        _roboState &= ~ROBOSTATE_READYRECON;
    }
}

void NC_STACK_yparobo::AI_layer3(update_msg *arg)
{
    int a4 = getBACT_bactCollisions();

    float tmp = _target_vec.length();

    if ( tmp <= 0.0 )
        _target_dir = vec3d(0.0, 0.0, 0.0);
    else
        _target_dir = _target_vec / tmp;

    if ( _status == BACT_STATUS_NORMAL )
    {
        if ( _roboState & ROBOSTATE_MOVE )
            doBeamUpdate(arg->frameTime);

        if ( !a4 || ! CollisionWithBact(arg->frameTime) )
        {
            checkCollisions(arg->frameTime * 0.001);

            if ( _roboState & ROBOSTATE_PLAYERROBO )
                wallow(arg);
            else
                AI_doMove(arg);

            if ( _roboState & ROBOSTATE_PLAYERROBO )
            {
                searchEnemyRobo();
                usersRoboEnergyCheck();
                doUserCommands(arg);
            }

            if ( !(_roboState & ROBOSTATE_PLAYERROBO) )
            {
                AI_checkWorld(arg);
                AI_doWish(arg);
                AI_service(arg);
                AI_clearAttackerSlots();
            }

            checkCommander();

            checkDanger();

            if ( _roboState & ROBOSTATE_DANGER && !(_roboState & ROBOSTATE_PLAYERROBO) )
                AI_solveTrouble(arg);
        }
    }
    else if ( _status == BACT_STATUS_DEAD )
    {
        DeadTimeUpdate(arg);
    }
}

void NC_STACK_yparobo::yparobo_func71__sub0(update_msg *arg)
{
    InputState *inpt = arg->inpt;
    float v18 = arg->frameTime / 1000.0;


    if ( inpt->Sliders[1] < -0.001 )
    {
        _viewer_vert_angle += inpt->Sliders[1] * _maxrot * v18 * 2.0;

        if ( _viewer_vert_angle > _viewer_max_up )
            _viewer_vert_angle = _viewer_max_up;

        if ( _viewer_vert_angle < -_viewer_max_down )
            _viewer_vert_angle = -_viewer_max_down;
    }
    else if (inpt->Sliders[1] > 0.001)
    {
        _viewer_vert_angle += inpt->Sliders[1] * _maxrot * v18 * 2.0;

        if ( _viewer_vert_angle > _viewer_max_up )
            _viewer_vert_angle = _viewer_max_up;

        if ( _viewer_vert_angle < -_viewer_max_down )
            _viewer_vert_angle = -_viewer_max_down;
    }

    if ( inpt->Sliders[0] < -0.001 )
    {
        _viewer_horiz_angle -= inpt->Sliders[0] * _maxrot * v18 * 2.0;

        if ( _viewer_max_side < 3.15 )
        {
            if ( _viewer_horiz_angle > _viewer_max_side )
                _viewer_horiz_angle = _viewer_max_side;

            if ( _viewer_horiz_angle < -_viewer_max_side )
                _viewer_horiz_angle = -_viewer_max_side;
        }
    }
    else if (inpt->Sliders[0] > 0.001)
    {
        _viewer_horiz_angle -= inpt->Sliders[0] * _maxrot * v18 * 2.0;

        if ( _viewer_max_side < 3.15 )
        {
            if ( _viewer_horiz_angle > _viewer_max_side )
                _viewer_horiz_angle = _viewer_max_side;

            if ( _viewer_horiz_angle < -_viewer_max_side )
                _viewer_horiz_angle = -_viewer_max_side;
        }
    }
}

void NC_STACK_yparobo::User_layer(update_msg *arg)
{
    int a4 = getBACT_bactCollisions();

    float v36 = _target_vec.length();

    if ( v36 <= 0.0 )
        _target_dir = vec3d(0.0, 0.0, 0.0);
    else
        _target_dir = _target_vec / v36;

    if ( _status == BACT_STATUS_NORMAL )
    {
        if ( _roboState & ROBOSTATE_MOVE )
            doBeamUpdate(arg->frameTime);

        if ( !a4 || !CollisionWithBact(arg->frameTime) )
        {
            checkCommander();
            checkDanger();
            searchEnemyRobo();
            usersRoboEnergyCheck();

            if ( arg->inpt->Buttons.Is(3) )
                HandBrake(arg);

            doUserCommands(arg);
            yparobo_func71__sub0(arg);
            wallow(arg);

            _viewer_rotation = _rotation;

            _viewer_rotation = mat3x3::RotateY(_viewer_horiz_angle) * _viewer_rotation;
            _viewer_rotation = mat3x3::RotateX(_viewer_vert_angle) * _viewer_rotation;
        }
    }
    else if ( _status == BACT_STATUS_DEAD )
    {
        DeadTimeUpdate(arg);
    }
    else if ( _status == BACT_STATUS_IDLE )
    {
        setState_msg arg78;
        arg78.newStatus = BACT_STATUS_NORMAL;
        arg78.setFlags = 0;
        arg78.unsetFlags = 0;

        SetState(&arg78);

        _status_flg &= ~BACT_STFLAG_LAND;
    }
}

void NC_STACK_yparobo::Move(move_msg *arg)
{
    _old_pos = _position;

    vec3d v63;
    if ( _status == BACT_STATUS_DEAD )
        v63 = vec3d::OY(_mass * 39.2266);
    else
        v63 = vec3d::OY(_mass * 9.80665);


    vec3d v52(0.0, 0.0, 0.0);
    if ( !(arg->flag & 1) )
        v52 = vec3d::OY(-_roboFlotage);

    vec3d v51(0.0, 0.0, 0.0);
    if ( !(arg->flag & 1) )
        v51 = arg->vec * _thraction;

    vec3d v57 = v63 + v51 - _fly_dir * (_fly_dir_length * _airconst) + v52;

    if ( v57.length() > 0.0 )
    {
        vec3d v67 = _fly_dir * _fly_dir_length + v57 * (arg->field_0 / _mass);

        _fly_dir_length = v67.normalise();
        _fly_dir = v67;
    }

    if ( fabs(_fly_dir_length) > 0.1 )
        _position += _fly_dir * (_fly_dir_length * arg->field_0 * 6.0);

    CorrectPositionInLevelBox(NULL);

    for (int i = 0; i < 8; i++)
    {
        if (_roboGuns[i].gun_obj)
        {
            bact_arg80 arg80;
            arg80.pos = _position + _rotation.Transpose().Transform( _roboGuns[i].pos );
            arg80.field_C = 4;

            _roboGuns[i].gun_obj->SetPosition(&arg80);
        }
    }

    _soundcarrier.samples_data[0].pitch = _pitch;
    _soundcarrier.samples_data[0].volume = _volume;

    float v60 = fabs(_fly_dir_length) / (_force / _airconst_static) * 1.4;

    if ( _soundcarrier.samples_data[0].psampl )
        _soundcarrier.samples_data[0].pitch = (_soundcarrier.samples_data[0].psampl->SampleRate + _soundcarrier.samples_data[0].pitch) * v60;
}

void NC_STACK_yparobo::Die()
{
    int a4 = getBACT_yourLastSeconds();

    uamessage_hostDie hdMsg;

    if ( !(_status_flg & BACT_STFLAG_DEATH1) )
    {
        hdMsg.msgID = UAMSG_HOSTDIE;
        hdMsg.owner = _owner;

        if (_killer)
            hdMsg.killer = _killer->_gid;
        else
            hdMsg.killer = 0;

        hdMsg.killerOwner = _killer_owner;

        if ( _killer_owner )
        {
            if ( !(_status_flg & BACT_STFLAG_CLEAN) )
            {
                yw_arg177 arg177;
                arg177.field_4 = _killer_owner;
                arg177.bact = this;

                _world->ypaworld_func177(&arg177); //Reown sectors for new owner
            }
        }

        for (World::RefBactList::iterator kdIt = _kidList.begin(); kdIt != _kidList.end();)
        {
            NC_STACK_ypabact *node = *kdIt;
            kdIt++;

            for (World::RefBactList::iterator sbIt = node->_kidList.begin(); sbIt != node->_kidList.end();)
            {
                NC_STACK_ypabact *subnode = *sbIt;
                sbIt++;

                subnode->_killer = _killer;

                if ( _status_flg & BACT_STFLAG_CLEAN )
                    subnode->_status_flg |= BACT_STFLAG_CLEAN;

                subnode->Die();

                subnode->_status_flg &= ~BACT_STFLAG_LAND;

                setState_msg arg119;
                arg119.unsetFlags = 0;
                arg119.setFlags = 0;
                arg119.newStatus = BACT_STATUS_CREATE;
                subnode->SetStateInternal(&arg119);

                subnode->_status = BACT_STATUS_DEAD;

                SFXEngine::SFXe.sub_424000(&subnode->_soundcarrier, 3);
                SFXEngine::SFXe.startSound(&subnode->_soundcarrier, 7);

                subnode->_soundFlags &= 0xFFFFFFF7;
                subnode->_soundFlags |= 0x80;

                subnode->setBACT_yourLastSeconds(a4);
            }

            node->_killer = _killer;

            if ( _status_flg & BACT_STFLAG_CLEAN )
                node->_status_flg |= BACT_STFLAG_CLEAN;

            node->Die();

            node->_status_flg &= ~BACT_STFLAG_LAND;

            setState_msg arg119;
            arg119.unsetFlags = 0;
            arg119.setFlags = 0;
            arg119.newStatus = BACT_STATUS_CREATE;
            node->SetStateInternal(&arg119);

            node->_status = BACT_STATUS_DEAD;

            SFXEngine::SFXe.sub_424000(&node->_soundcarrier, 3);
            SFXEngine::SFXe.startSound(&node->_soundcarrier, 7);

            node->_soundFlags &= 0xFFFFFFF7;
            node->_soundFlags |= 0x80;

            node->setBACT_yourLastSeconds(a4);
        }

        NC_STACK_ypabact *v20 = _world->getYW_userHostStation();

        if ( this == v20 )
        {
            if ( !(_status_flg & BACT_STFLAG_CLEAN) )
            {
                robo_arg134 arg134;
                arg134.field_4 = 12;
                arg134.field_8 = 0;
                arg134.field_10 = 0;
                arg134.field_C = 0;
                arg134.unit = this;
                arg134.field_14 = 100;

                placeMessage(&arg134);
            }
        }
        else
        {
            robo_arg134 arg134;
            arg134.field_4 = 11;
            arg134.field_8 = _owner;
            arg134.field_10 = 0;
            arg134.field_C = 0;
            arg134.unit = _killer;
            arg134.field_14 = 50;

            placeMessage(&arg134);
        }

        NC_STACK_ypabact::Die();

        _vp_extra[1].flags = 0;

        if ( _world->isNetGame )
        {
            if ( _owner )
            {
                yw_arg181 arg181;
                arg181.data = &hdMsg;
                arg181.dataSize = sizeof(hdMsg);
                arg181.recvFlags = 2;
                arg181.recvID = 0;
                arg181.garant = 1;
                _world->ypaworld_func181(&arg181);

                if ( this == _world->UserRobo )
                {
                    _world->UserUnit->setBACT_inputting(0);
                    _world->UserUnit->setBACT_viewer(0);

                    setBACT_inputting(1);
                    setBACT_viewer(1);
                }
            }
        }
    }
}

size_t NC_STACK_yparobo::SetPosition(bact_arg80 *arg)
{
    int v5 = NC_STACK_ypabact::SetPosition(arg);
    if ( !v5 )
        return 0;

    _roboFlotage = _mass * 9.80665;

    for (int i = 0; i < 8; i++)
    {
        roboGun &gun = _roboGuns[i];

        if (gun.gun_obj)
        {
            bact_arg80 v11;
            v11.pos = _position + _rotation.Transpose().Transform(gun.pos);
            v11.field_C = 4;

            gun.gun_obj->SetPosition(&v11);
        }
    }

    _roboYPos = _position.y;

    return 1;
}

// Update robo energys
void NC_STACK_yparobo::EnergyInteract(update_msg *arg)
{
    float v65 = _reload_const;
    float v63 = (_clock - _energy_time) / 1000.0;

    if ( _status != BACT_STATUS_DEAD && v63 >= 0.25 )
    {
        int v62 = _energy;

        int v61 = _roboEnergyLife;
        int v59 = _roboEnergyMove;

        _energy_time = _clock;

        yw_arg176 arg176;
        arg176.owner = _pSector->owner;

        _world->ypaworld_func176(&arg176);

        float v64 = _pSector->energy_power;

        float v70 = v65 * v63 * v64 * arg176.field_4;

        if ( _roboState & ROBOSTATE_PLAYERROBO )
        {
            int v67 = 0;

            float v68 = v70 / 6000.0;

            if ( _pSector->owner )
            {
                if ( _pSector->owner != _owner )
                    v68 = -v68;
            }
            else
            {
                v68 = 0.0;
            }

            if ( _roboFillMode & 1 || v68 < 0.0 )
                v67++;

            if ( _roboFillMode & 4 )
                v67++;

            if ( _roboFillMode & 8 )
                v67++;

            _roboEnergyReloadPS = v65 * v64 / 6000.0;

            if ( v67 > 0 )
            {
                int v25 = 0;
                int v26 = 0;
                int v66 = 0;

                v68 = v68 / (float)v67;

                if ( _roboFillMode & 1 || v68 < 0.0 )
                    _energy += v68;

                if ( _roboFillMode & 4 )
                    _roboEnergyLife += v68;

                if ( _roboFillMode & 8 )
                    _roboEnergyMove += v68;

                float v60 = v63 * v65 / 30.0;

                if ( _roboFillMode & 1 )
                    v25 = _energy;

                if ( _roboFillMode & 4 )
                    v25 += _roboEnergyLife;

                if ( _roboFillMode & 8 )
                    v25 += _roboEnergyMove;

                int v34 = v25 / v67;

                if ( _roboFillMode & 1 )
                {
                    if ( v34 > _energy )
                        v26++;
                    else if ( v34 < _energy )
                        v66++;
                }

                if ( _roboFillMode & 4 )
                {
                    if ( v34 > _roboEnergyLife )
                        v26++;
                    else if ( v34 < _roboEnergyLife )
                        v66++;
                }

                if ( _roboFillMode & 8 )
                {
                    if ( v34 > _roboEnergyMove )
                        v26++;
                    else if ( v34 < _roboEnergyMove )
                        v66++;
                }

                if ( v26 )
                    v26 = (int)v60 / v26;

                if ( v66 )
                    v66 = (int)v60 / v66;

                if ( _roboFillMode & 1 )
                {
                    if ( _energy <= v34 )
                    {
                        _energy += v26;

                        if ( _energy >= v34 )
                            _energy = v34;
                    }
                    else
                    {
                        _energy -= v66;
                        if ( _energy <= v34 )
                            _energy = v34;
                    }
                }

                if ( _roboFillMode & 4 )
                {
                    if ( _roboEnergyLife <= v34 )
                    {
                        _roboEnergyLife += v26;
                        if ( _roboEnergyLife >= v34)
                            _roboEnergyLife = v34;
                    }
                    else
                    {
                        _roboEnergyLife -= v66;
                        if ( _roboEnergyLife <= v34 )
                            _roboEnergyLife = v34;
                    }
                }

                if ( _roboFillMode & 8 )
                {
                    if ( v34 >= _roboEnergyMove )
                    {
                        _roboEnergyMove += v26;
                        if ( _roboEnergyMove >= v34)
                            _roboEnergyMove = v34;
                    }
                    else
                    {
                        _roboEnergyMove -= v66;
                        if ( _roboEnergyMove <= v34 )
                            _roboEnergyMove = v34;
                    }
                }

            }
        }
        else
        {
            _roboEnergyReloadPS = v65 * v64 / 7000.0;
            float v71 = v70 / 7000.0;
            if ( _owner == _pSector->owner )
            {
                _roboBuildSpare += v71 * 0.15;
                _roboVehicleSpare += v71 * 0.7;
                _energy += v71 * 0.7;
            }
            else
            {
                _energy -= v71;
            }
        }

        if ( _energy < 0 )
            _energy = 0;

        if ( _roboEnergyLife < 0 )
            _roboEnergyLife = 0;

        if ( _roboEnergyMove < 0 )
            _roboEnergyMove = 0;

        if ( _roboBuildSpare < 0 )
            _roboBuildSpare = 0;

        if ( _roboVehicleSpare < 0 )
            _roboVehicleSpare = 0;

        if ( _energy_max < _energy )
            _energy = _energy_max;

        if ( _energy_max < _roboEnergyLife )
            _roboEnergyLife = _energy_max;

        if ( _energy_max < _roboEnergyMove )
            _roboEnergyMove = _energy_max;

        if ( _energy_max < _roboBuildSpare )
            _roboBuildSpare = _energy_max;

        if ( _energy_max < _roboVehicleSpare )
            _roboVehicleSpare = _energy_max;

        _roboEnergyLossFlags = 0;
        _roboEnergyLoadFlags = 0;

        if ( v62 < _energy )
            _roboEnergyLoadFlags |= 1;
        else if ( v62 > _energy )
            _roboEnergyLossFlags |= 1;

        if ( v61 < _roboEnergyLife )
            _roboEnergyLoadFlags |= 4;
        else if ( v61 > _roboEnergyLife )
            _roboEnergyLossFlags |= 4;

        if ( v59 < _roboEnergyMove )
            _roboEnergyLoadFlags |= 8;
        else if ( v59 > _roboEnergyMove )
            _roboEnergyLossFlags |= 8;
    }
}

void NC_STACK_yparobo::Renew()
{
    NC_STACK_ypabact::Renew();

    _roboRadarValue = 0;
    _roboEnemyValue = 0;
    _roboPowerValue = 0;
    _roboSafetyValue = 0;
    _roboConqValue = 0;
    _roboPositionValue = 0;
    _roboDangerValue = 0;
    _roboExploreValue = 0;
    _roboPowerCellIndex = 0;
    _roboRadarCellIndex = 0;
    _roboSafetyCellIndex = 0;
    _roboConqCellIndex = 0;
    _roboRadarDelay = 0;
    _roboEnemyDelay = 0;
    _roboPowerDelay = 0;
    _roboSafetyDelay = 0;
    _roboConqDelay = 0;
    _roboDangerDelay = 0;
    _roboExploreDelay = 0;
    _roboPositionDelay = 0;
    _roboBuildSpare = 0;

    yw_130arg arg130;
    arg130.pos_x = 600.0;
    arg130.pos_z = -600.0;

    _world->ypaworld_func130(&arg130);

    _roboDockEnerg = 0;
    _roboDockCnt = 0;
    _roboDockTime = 0;
    _roboDockTargetType = BACT_TGT_TYPE_NONE;
    _roboDockAggr = 0;
    _roboDockTargetCommandID = 0;
    _roboDockTargetPos.x = 0;
    _roboDockTargetPos.y = 0;
    _roboDockTargetPos.z = 0;
    _roboDockTargetBact = NULL;
    _roboTestEnemyTime = 0;
    _roboBeamTimePre = 0;
    _roboAttackersClearTime = 0;
    _roboAttackersTime = 0;
    _roboState = 0;

    _roboPCells = arg130.pcell;

    _roboDockTime = 0;

    for(auto &g : _roboGuns)
        g.clear();
    memset(_roboAttackers, 0, sizeof(_roboAttackers));

    _commandID = dword_5B1128;

    dword_5B1128++;

    setBACT_yourLastSeconds(3000);

    Common::Ini::ParseIniFile(DefaultIniFile, &yparobo_keys);

    _roboNewAI = yparobo_keys[0].Get<bool>();
    _roboTimeScale = yparobo_keys[1].Get<int>();
}

void NC_STACK_yparobo::HandBrake(update_msg *)
{
    _fly_dir_length *= 0.8;
    _thraction = 0;
    _roboFlotage = _mass * 9.80665;
}

void NC_STACK_yparobo::CorrectPositionOnLand()
{
}

void NC_STACK_yparobo::DeadTimeUpdate(update_msg *arg)
{
    int a4 = getBACT_yourLastSeconds();

    if ( !(_status_flg & BACT_STFLAG_DEATH2) )
    {
        setState_msg arg78;
        arg78.newStatus = BACT_STATUS_NOPE;
        arg78.unsetFlags = 0;
        arg78.setFlags = BACT_STFLAG_DEATH2;
        SetState(&arg78);
    }

    _status_flg |= BACT_STFLAG_LAND;

    if ( !_owner || !_vp_genesis.base )
    {
        if ( a4 > 0 )
            return;

        int v31 = getBACT_inputting();

        if ( v31 )
            _status_flg |= BACT_STFLAG_NORENDER;
        else
            Release();

        return;
    }

    int v33 = (float)_energy_max * 0.7;
    if ( v33 < 10000 )
        v33 = 10000;
    if ( v33 > 25000 )
        v33 = 25000;

    if ( _vp_extra[0].flags & EVPROTO_FLAG_ACTIVE )
    {
        _scale_time -= arg->frameTime;

        if ( _scale_time <= 0 )
        {
            _vp_extra[0].vp.base = NULL;
            _vp_extra[0].vp.trigo = NULL;

            if ( a4 > 0 )
                return;

            int v30 = getBACT_inputting();

            if ( v30 )
                _status_flg |= BACT_STFLAG_NORENDER;
            else
                Release();

            return;
        }

        _vp_extra[0].scale = sqrt((float)_scale_time / (float)v33) * 0.75;

        if ( _vp_extra[0].scale < 0.0 )
            _vp_extra[0].scale = 0;

        float v16 = _maxrot * 2.0 * ((float)arg->frameTime / 1000.0);

        _vp_extra[0].rotate = mat3x3::RotateY(v16) * _vp_extra[0].rotate;

        if ( a4 <= 0 )
            _status_flg |= BACT_STFLAG_NORENDER;
    }
    else
    {
        _scale_time = v33;
        _vp_extra[0].scale = 0.75;
        _vp_extra[0].pos = _position;
        _vp_extra[0].rotate = _rotation;
        _vp_extra[0].vp = _vp_genesis;
        _vp_extra[0].flags |= (EVPROTO_FLAG_ACTIVE | EVPROTO_FLAG_SCALE);

        if ( _world->isNetGame )
        {
            uamessage_startPlasma sbMsg;
            sbMsg.msgID = UAMSG_STARTPLASMA;
            sbMsg.owner = _owner;
            sbMsg.scale = 0.75;
            sbMsg.time = v33;
            sbMsg.id = _gid;
            sbMsg.pos = _position;
            sbMsg.dir = _rotation;

            yw_arg181 arg181;
            arg181.data = &sbMsg;
            arg181.garant = 1;
            arg181.recvID = 0;
            arg181.recvFlags = 2;
            arg181.dataSize = sizeof(sbMsg);
            _world->ypaworld_func181(&arg181);
        }
    }
}

void NC_STACK_yparobo::yparobo_func128(robo_arg128 *arg)
{
    arg->comm_bacto = NULL;
    arg->comm_bact = NULL;

    for ( NC_STACK_ypabact* &node : _kidList )
    {
        if ( !(node->_status_flg & BACT_STFLAG_DEATH1) )
        {
            int v7;

            if ( arg->flags & 1 )
            {
                if ( arg->comm_id == node->_commandID )
                {
                    arg->comm_bact = node;
                    arg->comm_bacto = node;
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
                if ( node->_primTtype == arg->tgType || _roboDockTargetType == arg->tgType )
                {
                    int v28 = arg->tgt_pos.x / 1200.0;
                    int v29 = (-arg->tgt_pos.z) / 1200.0;

                    int v30, v31, v16;
                    if ( node->_commandID == _roboDockUser && _roboDockUser )
                    {
                        v30 = _roboDockTargetPos.x / 1200.0;
                        v31 = (-_roboDockTargetPos.z) / 1200.0;
                        v16 = _roboDockTargetCommandID;
                    }
                    else
                    {
                        v30 = node->_primTpos.x / 1200.0;
                        v31 = (-node->_primTpos.z) / 1200.0;
                        v16 = node->_primT_cmdID;
                    }

                    if ( (arg->tgType == BACT_TGT_TYPE_UNIT && !arg->prim_comm_id && arg->tgt.pbact == node->_primT.pbact)
                            || (arg->tgType == BACT_TGT_TYPE_UNIT && arg->prim_comm_id != 0 && v16 == arg->prim_comm_id)
                            || (arg->tgType == BACT_TGT_TYPE_CELL && v30 == v28 && v31 == v29) )
                    {
                        arg->comm_bact = node;
                        arg->comm_bacto = node;
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
                if ( node->_primTtype == BACT_TGT_TYPE_CELL )
                {
                    float tx, ty;

                    if ( node->_commandID == _roboDockUser && _roboDockUser )
                    {
                        tx = _roboDockTargetPos.x;
                        ty = _roboDockTargetPos.z;
                    }
                    else
                    {
                        tx = node->_primTpos.x;
                        ty = node->_primTpos.z;
                    }
                    if ( POW2(arg->distance) > POW2(tx - arg->tgt_pos.x) + POW2(ty - arg->tgt_pos.z) )
                    {
                        arg->comm_bact = node;
                        arg->comm_bacto = node;

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
    if ( !arg->priority )
    {
        arg->tgt_type = BACT_TGT_TYPE_NONE;
        return 0;
    }

    if (_kidRef) //FIXME Think it's must be yw->_unitsList
    {
        for (NC_STACK_ypabact* &node : *_kidRef.PList())
        {
            if (arg->priority == node->_commandID)
            {
                if ( node->_status != BACT_STATUS_DEAD )
                {
                    arg->priority = 0;
                    arg->tgt_type = BACT_TGT_TYPE_UNIT;
                    arg->tgt.pbact = node;
                    return 1;
                }
                else
                {
                    arg->priority = 0;
                    arg->tgt_type = BACT_TGT_TYPE_NONE;
                    return 0;
                }
            }

            for (NC_STACK_ypabact* &subnode : node->_kidList)
            {
                if (arg->priority == subnode->_commandID)
                {
                    if ( subnode->_status != BACT_STATUS_DEAD )
                    {
                        arg->priority = 0;
                        arg->tgt_type = BACT_TGT_TYPE_UNIT;
                        arg->tgt.pbact = subnode;
                        return 1;
                    }
                    else
                    {
                        arg->priority = 0;
                        arg->tgt_type = BACT_TGT_TYPE_NONE;
                        return 0;
                    }
                }
            }
        }
    }

    arg->tgt_type = BACT_TGT_TYPE_NONE;
    arg->priority = 0;
    return 0;
}

// Create squad for robo
bool NC_STACK_yparobo::MakeSquad(const std::vector<int> &VhclIDS, vec3d pos, bool usable)
{
    size_t curid = 0;
    int col = sqrt(VhclIDS.size()) + 2;

    ypaworld_arg146 arg146;
    arg146.pos.y = pos.y;
    arg146.pos.x = 100.0 * (curid % col - col / 2.0) + pos.x;
    arg146.pos.z = 100.0 * (curid / col) + pos.z;
    
    arg146.vehicle_id = VhclIDS[0];

    NC_STACK_ypabact *squad_commander = _world->ypaworld_func146(&arg146); //Create first bact

    if ( !squad_commander )
        return false;

    squad_commander->setBACT_bactCollisions( getBACT_bactCollisions() );

    if ( !usable )
        squad_commander->_status_flg |= BACT_STFLAG_UNUSE;

    squad_commander->_owner = _owner;
    squad_commander->_commandID = dword_5B1128;
    squad_commander->_host_station = this;
    squad_commander->_aggr = 60;

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

    for ( curid = 1; curid < VhclIDS.size(); curid++)
    {
        arg146.vehicle_id = VhclIDS[VhclIDS.size() - curid]; // CHECK IT Why from end ?

        arg146.pos.x = 100.0 * (curid % col - col / 2.0) + pos.x;
        arg146.pos.z = 100.0 * (curid / col) + pos.z;

        NC_STACK_ypabact *next_bact = (NC_STACK_ypabact *)_world->ypaworld_func146(&arg146);

        if ( !next_bact )
            break;

        squad_commander->AddSubject(next_bact); // Add to squad commander list

        arg67.tgt_pos.x = arg146.pos.x;
        arg67.tgt_pos.z = arg146.pos.z;
        next_bact->SetTarget(&arg67); //Set target

        next_bact->setBACT_bactCollisions( getBACT_bactCollisions() );

        if ( !usable )
            next_bact->_status_flg |= BACT_STFLAG_UNUSE;

        next_bact->_owner = _owner;
        next_bact->_commandID = dword_5B1128;
        next_bact->_host_station = this;
        next_bact->_aggr = 60;

        next_bact->SetState(&arg78);
    }

    dword_5B1128++;

    AddSubject(squad_commander);  //Add squad commander into robo list

    return true;
}


const char * NC_STACK_yparobo::yparobo_func134__sub0(char **strings, int a2)
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

int NC_STACK_yparobo::yparobo_func134__sub1(robo_arg134 *arg)
{
    int v2 = 1;

    switch(arg->field_4)
    {
    case 1:
    {
        NC_STACK_ypabact *bacto = _world->getYW_userVehicle();

        if ( bacto->_bact_type != BACT_TYPES_MISSLE )
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
        v2 = (_clock - dword_515138[ arg->field_8 ] > 45000) || dword_515138[ arg->field_8 ] == 0;
        dword_515138[ arg->field_8 ] = _clock;
        break;

    case 7:
        v2 = 1;
        if ( arg->unit && arg->unit->_bact_type == BACT_TYPES_GUN )
        {
            NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( arg->unit );

            if ( gun->IsRoboGun() )
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
        if ( (_clock - stru_5B0628[ arg->field_4 ].field_10) > arg->field_8 )
        {
            stru_5B0628[ arg->field_4 ].field_10 = _clock;
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
    if ( arg->unit && arg->unit->_bact_type == BACT_TYPES_GUN && arg->field_4 != 7 && arg->field_4 != 19 && arg->field_4 != 6 )
        return 0;

    char **strngs = _world->getYW_localeStrings();

    if ( !yparobo_func134__sub1(arg) )
        return 0;

    yw_arg159 v8;
    v8.field_4 = arg->field_14;
    v8.txt = yparobo_func134__sub0(strngs, arg->field_4);
    v8.unit = arg->unit;
    v8.field_C = arg->field_4;
    _world->ypaworld_func159(&v8);

    return 1;
}


void NC_STACK_yparobo::ypabact_func65__sub0()
{
    if ( _beam_time )
    {
        if ( _clock - _beam_time > 2000 )
        {
            int i = 0;

            for ( NC_STACK_ypabact* &v1 : _pSector->unitsList )
            {
                int a4 = v1->getBACT_viewer();

                if ( v1->_status == BACT_STATUS_BEAM && v1->_owner == _owner && !a4 )
                    i = 1;
            }

            if ( !i )
            {
                _world->ypaworld_func168(this);

                vec3d tt = _position;

                _position = _old_pos;
                _roboYPos = _old_pos.y;

                if ( !_world->SaveGame( fmt::sprintf("save:%s/%d.fin", _world->GameShell->user_name, _world->_levelInfo->LevelID) ) )
                    ypa_log_out("Warning, final sgm save error\n");

                _position = tt;

                if ( _world->GameShell )
                    uaDeleteFile( fmt::sprintf("save:%s/%d.rst", _world->GameShell->user_name, _world->_levelInfo->LevelID) );
                
                _status_flg |= BACT_STFLAG_CLEAN;

                Die();
            }

        }
    }
    else
    {

        _position.x = (_sectX + 0.5) * 1200.0;
        _position.z = -(_sectY + 0.5) * 1200.0;

        for (NC_STACK_ypabact* &unit : _kidList )
        {
            unit->_status_flg |= BACT_STFLAG_CLEAN;

            for (NC_STACK_ypabact* &sub_unit : unit->_kidList )
                sub_unit->_status_flg |= BACT_STFLAG_CLEAN;
        }

        _world->_levelInfo->Buddies.clear();

        int v15 = 0;

        for ( NC_STACK_ypabact* &bct : _pSector->unitsList )
        {
            if  ( bct->_status != BACT_STATUS_DEAD )
            {
                if ( bct->_bact_type != BACT_TYPES_ROBO && bct->_bact_type != BACT_TYPES_MISSLE && bct->_bact_type != BACT_TYPES_GUN && bct->_owner == _owner )
                {
                    float v17 = fabs(bct->_position.x - (_sectX + 0.5) * 1200.0);
                    float v29 = fabs(bct->_position.z + (_sectY + 0.5) * 1200.0);

                    if ( v17 <= 450.0 || v29 <= 450.0 )
                    {
                        bct->_status = BACT_STATUS_BEAM;

                        bct->_scale_time = 0;

                        bct->_scale_delay = v15 + 1000;
                        v15 += 400;
                    }
                }
            }
        }

        _beam_time = _clock;
    }
}



void NC_STACK_yparobo::setBACT_inputting(int inpt)
{
    NC_STACK_ypabact::setBACT_inputting(inpt);

    if ( inpt )
    {
        _world->setYW_userHostStation(this);

        _roboState |= ROBOSTATE_PLAYERROBO;
    }
}

void NC_STACK_yparobo::setROBO_proto(roboProto *proto)
{
    for (int i = 0; i < 8; i++)
    {
        if ( _roboGuns[i].gun_obj )
        {
            if ( !( _roboGuns[i].gun_obj->_status_flg & BACT_STFLAG_DEATH1 ) )
                _roboGuns[i].gun_obj->Die();

            _roboGuns[i].gun_obj->Release();

            _roboGuns[i].gun_obj = NULL;
        }
    }

    for (int i = 0; i < proto->robo_num_guns; i++)
    {
        _roboGuns[i] = proto->guns[i];

        ypaworld_arg146 gun_req;

        gun_req.pos = _position + _rotation.Transpose().Transform(_roboGuns[i].pos);

        gun_req.vehicle_id = _roboGuns[i].robo_gun_type;

        NC_STACK_ypabact *gun_obj = _world->ypaworld_func146(&gun_req);
        NC_STACK_ypagun *gungun = dynamic_cast<NC_STACK_ypagun *>(gun_obj);

        _roboGuns[i].gun_obj = gun_obj;

        if ( gun_obj )
        {
            if (gungun)
            {
                gungun->ypagun_func128(_roboGuns[i].dir, false);

                gungun->setGUN_roboGun(1);
            }

            gun_obj->_owner = _owner;
            gun_obj->_commandID = dword_5B1128;
            gun_obj->_host_station = this;

            dword_5B1128++;

            if ( _world->isNetGame )
            {
                gun_obj->_gid |= gun_obj->_owner << 24;
                gun_obj->_commandID |= gun_obj->_owner << 24;
            }

            gun_obj->_aggr = 60;

            AddSubject(gun_obj);
        }
        else
        {
            ypa_log_out("Unable to create Robo-Gun\n");
        }
    }

    _roboDockPos = proto->dock;
    _viewer_position = proto->viewer;
    _viewer_max_up = proto->robo_viewer_max_up;
    _viewer_max_down = proto->robo_viewer_max_down;
    _viewer_max_side = proto->robo_viewer_max_side;

    setBACT_extraViewer(1);
    setBACT_alwaysRender(1);

    _roboColls = proto->coll;
}

void NC_STACK_yparobo::setROBO_epConquer(int ep)
{
    _roboEpConquer = ep;
}

void NC_STACK_yparobo::setROBO_epDefense(int ep)
{
    _roboEpDefense = ep;
}

void NC_STACK_yparobo::setROBO_epRadar(int ep)
{
    _roboEpRadar = ep;
}

void NC_STACK_yparobo::setROBO_epPower(int ep)
{
    _roboEpPower = ep;
}

void NC_STACK_yparobo::setROBO_epSafety(int ep)
{
    _roboEpSafety = ep;
}

void NC_STACK_yparobo::setROBO_commCount(int comm)
{
    dword_5B1128 = comm;
}

void NC_STACK_yparobo::setROBO_battVehicle(int batt)
{
    _roboEnergyLife = batt;
}

void NC_STACK_yparobo::setROBO_battBuilding(int batt)
{
    _roboEnergyBuilding = batt;
}

void NC_STACK_yparobo::setROBO_battBeam(int batt)
{
    _roboEnergyMove = batt;
}

void NC_STACK_yparobo::setROBO_fillMode(int fil)
{
    _roboFillMode = fil;
}

void NC_STACK_yparobo::setROBO_waitSway(int wait)
{
    if ( wait )
        _roboWFlags |= 1;
    else
        _roboWFlags &= ~1;
}

void NC_STACK_yparobo::setROBO_waitRotate(int wait)
{
    if ( wait )
        _roboWFlags |= 2;
    else
        _roboWFlags &= ~2;
}

void NC_STACK_yparobo::setROBO_epChangeplace(int ep)
{
    _roboEpChangePlace = ep;
}

void NC_STACK_yparobo::setROBO_epReconnoitre(int ep)
{
    _roboEpRecon = ep;
}

void NC_STACK_yparobo::setROBO_epRobo(int ep)
{
    _roboEpRobo = ep;
}

void NC_STACK_yparobo::setROBO_viewAngle(int angl)
{
    _viewer_rotation = mat3x3::Ident();
    _viewer_horiz_angle = angl * C_PI_180;
    _viewer_rotation = mat3x3::RotateY(_viewer_horiz_angle) * _viewer_rotation;
}

void NC_STACK_yparobo::setROBO_safDelay(int delay)
{
    _roboSafetyDelay = delay;
}

void NC_STACK_yparobo::setROBO_powDelay(int delay)
{
    _roboPowerDelay = delay;
}

void NC_STACK_yparobo::setROBO_radDelay(int delay)
{
    _roboRadarDelay = delay;
}

void NC_STACK_yparobo::setROBO_cplDelay(int delay)
{
    _roboPositionDelay = delay;
}

void NC_STACK_yparobo::setROBO_defDelay(int delay)
{
    _roboEnemyDelay = delay;
}

void NC_STACK_yparobo::setROBO_conDelay(int delay)
{
    _roboConqDelay = delay;
}

void NC_STACK_yparobo::setROBO_robDelay(int delay)
{
    _roboDangerDelay = delay;
}

void NC_STACK_yparobo::setROBO_recDelay(int delay)
{
    _roboExploreDelay = delay;
}



rbcolls *NC_STACK_yparobo::getBACT_collNodes()
{
    return &_roboColls;
}

int NC_STACK_yparobo::getROBO_epConquer()
{
    return _roboEpConquer;
}

int NC_STACK_yparobo::getROBO_epDefense()
{
    return _roboEpDefense;
}

int NC_STACK_yparobo::getROBO_epRadar()
{
    return _roboEpRadar;
}

int NC_STACK_yparobo::getROBO_epPower()
{
    return _roboEpPower;
}

int NC_STACK_yparobo::getROBO_epSafety()
{
    return _roboEpSafety;
}

int NC_STACK_yparobo::getROBO_commCount()
{
    return dword_5B1128;
}

int NC_STACK_yparobo::getROBO_battVehicle()
{
    return _roboEnergyLife;
}

int NC_STACK_yparobo::getROBO_battBuilding()
{
    return _roboEnergyBuilding;
}

int NC_STACK_yparobo::getROBO_battBeam()
{
    return _roboEnergyMove;
}

int NC_STACK_yparobo::getROBO_fillMode()
{
    return _roboFillMode;
}

roboGun *NC_STACK_yparobo::getROBO_guns()
{
    return _roboGuns.data();
}

int NC_STACK_yparobo::getROBO_epChangeplace()
{
    return _roboEpChangePlace;
}

int NC_STACK_yparobo::getROBO_epReconnoitre()
{
    return _roboEpRecon;
}

int NC_STACK_yparobo::getROBO_epRobo()
{
    return _roboEpRobo;
}

int NC_STACK_yparobo::getROBO_roboState()
{
    return _roboState;
}

int NC_STACK_yparobo::getROBO_safDelay()
{
    return _roboSafetyDelay;
}

int NC_STACK_yparobo::getROBO_powDelay()
{
    return _roboPowerDelay;
}

int NC_STACK_yparobo::getROBO_radDelay()
{
    return _roboRadarDelay;
}

int NC_STACK_yparobo::getROBO_cplDelay()
{
    return _roboPositionDelay;
}

int NC_STACK_yparobo::getROBO_defDelay()
{
    return _roboEnemyDelay;
}

int NC_STACK_yparobo::getROBO_conDelay()
{
    return _roboConqDelay;
}

int NC_STACK_yparobo::getROBO_robDelay()
{
    return _roboDangerDelay;
}

int NC_STACK_yparobo::getROBO_recDelay()
{
    return _roboExploreDelay;
}

int NC_STACK_yparobo::getROBO_loadFlags()
{
    return _roboEnergyLoadFlags;
}

int NC_STACK_yparobo::getROBO_lossFlags()
{
    return _roboEnergyLossFlags;
}

int NC_STACK_yparobo::getROBO_absReload()
{
    return _roboEnergyReloadPS;
}




NC_STACK_yparobo::NC_STACK_yparobo()
{
    _roboFlotage = 0.;
    _roboColls.clear();
    _roboYPos = 0.;

    _roboWFlags = 0;
    _roboState = 0;
    _roboNewAI = 0;
    _roboTimeScale = 0;

    _roboEpConquer = 0;
    _roboEpRadar = 0;
    _roboEpPower = 0;
    _roboEpDefense = 0;
    _roboEpSafety = 0;
    _roboEpRecon = 0;
    _roboEpChangePlace = 0;
    _roboEpRobo = 0;

    _roboDockEnerg = 0;
    _roboDockCnt = 0;
    _roboDockUser = 0;
    _roboDockTime = 0;
    _roboDockTargetPos = vec3d();
    _roboDockTargetBact = NULL;
    _roboDockTargetType = 0;
    _roboDockAggr = 0;
    _roboDockTargetCommandID = 0;
    _roboDockPos = vec3d();

    _roboRadarValue = 0;
    _roboRadarCell = NULL;
    _roboRadarCellID = 0;  //For AI
    _roboRadarCellIndex = 0;
    _roboRadarTime = 0;
    _roboRadarDelay = 0; //??

    _roboSafetyValue = 0;
    _roboSafetyCell = NULL;  //For AI
    _roboSafetyCellID = 0;  //For AI
    _roboSafetyCellIndex = 0;
    _roboSafetyTime = 0;
    _roboSafetyDelay = 0; //??

    //Power
    _roboPowerValue = 0;
    _roboPowerCell = NULL;  //For AI
    _roboPowerCellID = 0;  //For AI
    _roboPowerCellIndex = 0;
    _roboPowerTime = 0;
    _roboPowerDelay = 0; //??

    //Enemy
    _roboEnemyValue = 0;
    _roboEnemyCell = NULL;  //For AI
    _roboEnemyCellID = 0;  //For AI
    _roboEnemyCommandID = 0;  //For AI
    _roboEnemyTime = 0;
    _roboEnemyDiv = 0;
    _roboEnemyDelay = 0; //??

    _roboConqValue = 0;
    _roboConqCell = NULL;  //For AI
    _roboConqCellID = 0;  //For AI
    _roboConqCellIndex = 0;
    _roboConqTime = 0;
    _roboConqDelay = 0; //??

    _roboPositionValue = 0;
    _roboPositionCell = NULL;  //For AI
    _roboPositionCellID = 0;  //For AI
    _roboPositionCellIndex = 0;  //For AI
    _roboPositionTime = 0;
    _roboPositionDelay = 0; //??
    _roboExploreValue = 0;
    _roboExploreCell = NULL;  //For AI
    _roboExploreCellID = 0;  //For AI
    _roboExploreCellIndex = 0;  //For AI
    _roboExploreTime = 0;
    _roboExploreDelay = 0; //??
    _roboDangerValue = 0;
    _roboDangerCell = NULL;  //For AI
    _roboDangerCellID = 0;  //For AI
    _roboDangerCommandID = 0;  //For AI
    _roboDangerTime = 0;
    _roboDangerDelay = 0; //??
    _roboVehicleCellID = 0;  //For AI
    _roboVehicleCell = NULL;  //For AI

    _roboVehicleDuty = 0;  //For AI
    _roboVehicleCommandID = 0;  //For AI
    _roboBuildingCellID = 0;  //For AI
    _roboBuildingCell = NULL;  //For AI
    _roboBuildingDuty = 0;  //For AI
    _roboPCells = NULL;

    _roboTestEnemyTime = 0;

    for(auto &g : _roboGuns)
        g.clear();

    _roboEnergyLife = 0; //??
    _roboEnergyBuilding = 0; //??
    _roboEnergyMove = 0; //??

    _roboFillMode = 0;
    _roboEnergyLoadFlags = 0;
    _roboEnergyLossFlags = 0;

    _roboEnergyReloadPS = 0;
    _roboBuildSpare = 0;
    _roboVehicleSpare = 0;
    _roboBeamTimePre = 0;
    _roboBeamPos = vec3d();
    _roboBeamFXTime = 0;

    for (auto &t : _roboAttackers)
    {
        t.field_0 = 0;
        t.field_4 = 0;
    }

    _roboAttackersTime = 0;
    _roboAttackersClearTime = 0;
}
