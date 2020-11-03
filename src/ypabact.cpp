#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <stack>
#include "yw.h"
#include "ypabact.h"
#include "yparobo.h"
#include "ypamissile.h"
#include "yw_net.h"

#include "log.h"



const Nucleus::ClassDescr NC_STACK_ypabact::description("ypabact.class", &newinstance);


int ypabact_id = 1;
char **dword_5490B0; // ypaworld strings


NC_STACK_ypabact::NC_STACK_ypabact()
: _kidList(this, GetKidRefNode, World::BLIST_KIDS)
{
    _sectX = 0;
    _sectY = 0;
    _pSector = NULL;
    _wrldX = 0.0;
    _wrldY = 0.0;
    _secMaxX = 0;
    _secMaxY = 0;
    _bact_type = 0;
    _gid = 0;
    _vehicleID = 0;
    _bflags = 0;
    _commandID = 0;
    _host_station = NULL;
    _parent = NULL;
    
    memset(&_soundcarrier, 0, sizeof(_soundcarrier) ); //FIX IT

    _soundFlags = 0;
    _volume = 0;
    _pitch = 0;
    _pitch_max = 0.0;
    _energy = 0;
    _energy_max = 0;
    _reload_const = 0;
    _shield = 0;
    _radar = 0;
    _owner = 0;
    _aggr = 0;
    _status = 0;
    _status_flg = 0;
    _primTtype = 0;
    _secndTtype = 0;
    _primT_cmdID = 0;
    _secndT_cmdID = 0;
    _primT.pbact = NULL;
    _secndT.pbact = NULL;
    _adist_sector = 0.0;
    _adist_bact = 0.0;
    _sdist_sector = 0.0;
    _sdist_bact = 0.0;
    _current_waypoint = 0;
    _waypoints_count = 0;
    _m_cmdID = 0;
    _m_owner = 0;
    _fe_cmdID = 0;
    _fe_time = 0;
    _mass = 0.0;
    _force = 0.0;
    _airconst = 0.0;
    _airconst_static = 0.0;
    _maxrot = 0.0;
    _viewer_horiz_angle = 0.0;
    _viewer_vert_angle = 0.0;
    _viewer_max_up = 0.0;
    _viewer_max_down = 0.0;
    _viewer_max_side = 0.0;
    _thraction = 0.0;
    _fly_dir_length = 0.0;
    _height = 0.0;
    _height_max_user = 0.0;

    _vp_active = 0;
    memset(_vp_extra, 0, sizeof(_vp_extra));
    _vp_extra_mode = 0;

    _radius = 0.0;
    _viewer_radius = 0.0;
    _overeof = 0.0;
    _viewer_overeof = 0.0;
    _clock = 0;
    _AI_time1 = 0;
    _AI_time2 = 0;
    _search_time1 = 0;
    _search_time2 = 0;
    _scale_time = 0;
    _brkfr_time = 0;
    _brkfr_time2 = 0;
    _newtarget_time = 0;
    _assess_time = 0;
    _waitCol_time = 0;
    _slider_time = 0;
    _dead_time = 0;
    _beam_time = 0;
    _energy_time = 0;
    _weapon = 0;
    _weapon_flags = 0;
    _mgun = 0;
    _num_weapons = 0;
    _weapon_time = 0;
    _gun_angle = 0.0;
    _gun_angle_user = 0.0;
    _gun_leftright = 0.0;
    _gun_radius = 0.0;
    _gun_power = 0.0;
    _mgun_time = 0;
    _salve_counter = 0;
    _kill_after_shot = 0;
    _heading_speed = 0.0;
    _killer = NULL;
    _killer_owner = 0;
    _reb_count = 0;
    _atk_ret = 0;
    _lastFrmStamp = 0;
    _scale_start = 0.0;
    _scale_speed = 0.0;
    _scale_accel = 0.0;
    _scale_duration = 0;
    _scale_pos = 0;
    _scale_delay = 0;

    for (int i = 0; i < 32; i++)
    {
        _vp_fx_models[i] = NULL;
        _vp_fx_tform[i] = NULL;
    }

    _oflags = 0;
    _yls_time = 0;
    
    _world = NULL;
}


size_t NC_STACK_ypabact::func0(IDVList &stak)
{
    if ( !NC_STACK_nucleus::func0(stak) )
        return 0;

    _attackersList.clear();
    _kidList.clear();
    _missiles_list.clear();

    _gid = ypabact_id;
    _bact_type = BACT_TYPES_BACT;
//    ypabact.field_3DA = 0;
    _host_station = NULL;
    _viewer_rotation = mat3x3::Ident();
    _fly_dir = vec3d(0.0, 0.0, 0.0);
    _fly_dir_length = 0;
    _target_vec = vec3d(0.0, 0.0, 0.0);
    
    //_kidRef.bact = this;



    ypabact_id++;

    _rotation = _viewer_rotation;

    _mass = 400.0;
    _force = 5000.0;
    _airconst = 500.0;
    _maxrot = 0.5;
    _height = 150.0;
    _radius = 20.0;
    _viewer_radius = 40.0;
    _overeof = 10.0;
    _viewer_overeof = 40.0;
    _energy = 10000;
    _shield = 0;
    _heading_speed = 0.7;
    _yls_time = 3000;
    _aggr = 50;
    _energy_max = 10000;
//    ypabact.field_3CE = 0;
    _height_max_user = 1600.0;
    _gun_radius = 5.0;
    _gun_power = 4000.0;
    _adist_sector = 800.0;
    _adist_bact = 650.0;
    _sdist_sector = 200.0;
    _sdist_bact = 100.0;
    _oflags = BACT_OFLAG_EXACTCOLL;

    _world = stak.Get<NC_STACK_ypaworld *>(BACT_ATT_WORLD, NULL);// get ypaworld

    if ( _world )
    {
        for( auto& it : stak )
        {
            IDVPair &val = it.second;

            if ( !val.Skip )
            {
                switch (val.ID)
                {
                case BACT_ATT_VIEWER:
                {
                    uamessage_viewer viewMsg;

                    if ( val.Get<int32_t>() )
                    {
                        _world->ypaworld_func131(this); //Set current bact

                        _oflags |= BACT_OFLAG_VIEWER;

                        if ( _world->isNetGame )
                            viewMsg.view = 1;

                        SFXEngine::SFXe.startSound(&_soundcarrier, 8);
                    }
                    else
                    {
                        _oflags &= ~BACT_OFLAG_VIEWER;

                        if ( _world->isNetGame )
                            viewMsg.view = 0;

                        SFXEngine::SFXe.sub_424000(&_soundcarrier, 8);
                    }

                    if ( _world->isNetGame ) // Network message send routine?
                    {
                        viewMsg.msgID = UAMSG_VIEWER;
                        viewMsg.owner = _owner;
                        viewMsg.classID = _bact_type;
                        viewMsg.id = _gid;

                        if ( viewMsg.classID == 4 )
                        {
                            NC_STACK_ypamissile *miss = dynamic_cast<NC_STACK_ypamissile *>(this);
                            viewMsg.launcher = miss->getMISS_launcher()->_gid;
                        }

                        yw_arg181 ywMsg;
                        ywMsg.recvID = 0;
                        ywMsg.recvFlags = 2;
                        ywMsg.data = &viewMsg;
                        ywMsg.garant = 1;
                        ywMsg.dataSize = sizeof(viewMsg);

                        _world->ypaworld_func181(&ywMsg);
                    }
                }
                break;

                case BACT_ATT_INPUTTING:
                    if ( val.Get<int32_t>() )
                    {
                        _oflags |= BACT_OFLAG_USERINPT;
                        _world->setYW_userVehicle(this);
                    }
                    else
                    {
                        _oflags &= ~BACT_OFLAG_USERINPT;
                    }
                    break;

                case BACT_ATT_EXACTCOLL:
                    setBACT_exactCollisions(val.Get<int32_t>());
                    break;

                case BACT_ATT_BACTCOLL:
                    setBACT_bactCollisions ( val.Get<int32_t>() );
                    break;

                case BACT_ATT_AIRCONST:
                    setBACT_airconst(val.Get<int32_t>());
                    break;

                case BACT_ATT_LANDINGONWAIT:
                    setBACT_landingOnWait ( val.Get<int32_t>() );
                    break;

                case BACT_ATT_YOURLS:
                    setBACT_yourLastSeconds(val.Get<int32_t>());
                    break;

                case BACT_ATT_VISPROT:
                    setBACT_visProto( val.Get<NC_STACK_base *>());
                    break;

                case BACT_ATT_AGGRESSION:
                    setBACT_aggression(val.Get<int32_t>());
                    break;

                case BACT_ATT_VPTRANSFORM:
                    setBACT_vpTransform( val.Get<TFEngine::TForm3D *>());
                    break;

                case BACT_ATT_EXTRAVIEWER:
                    setBACT_extraViewer ( val.Get<int32_t>() );
                    break;

                case BACT_ATT_ALWAYSRENDER:
                    setBACT_alwaysRender ( val.Get<int32_t>() );
                    break;

                default:
                    break;
                }
            }
        }
    }

    _tForm.locPos = _position;

    _tForm.locSclRot = _rotation;

    _status = BACT_STATUS_NORMAL;

    int secMaxX = _world->getYW_mapSizeX();
    int secMaxY = _world->getYW_mapSizeY();

    _wrldX =  secMaxX * 1200.0;
    _wrldY = -secMaxY * 1200.0;
    _secMaxX = secMaxX;
    _secMaxY = secMaxY;

    dword_5490B0 = _world->getYW_localeStrings();

    return 1;
}

size_t NC_STACK_ypabact::func1()
{
    SFXEngine::SFXe.sub_423DD8(&_soundcarrier);

    _status_flg |= BACT_STFLAG_CLEAN;

    if ( !(_status_flg & BACT_STFLAG_DEATH1) )
        Die();

    if ( _pSector )
        _cellRef.Detach();

    _kidRef.Detach();

    while (!_kidList.empty())
        Nucleus::Delete(_kidList.front());

    while (!_missiles_list.empty())
    {
        Nucleus::Delete(_missiles_list.front());
        _missiles_list.pop_front();        
    }

    return NC_STACK_nucleus::func1();
}


void NC_STACK_ypabact::CopyTargetOf(NC_STACK_ypabact *unit)
{
    NC_STACK_ypabact *v6 = NULL;

    _waypoints_count = 0;
    _m_cmdID = 0;
    _status_flg &= ~(BACT_STFLAG_WAYPOINT | BACT_STFLAG_WAYPOINTCCL);

    int tgType;
    vec2d wTo;

    if ( unit->_status_flg & BACT_STFLAG_WAYPOINT )
    {
        if ( !unit->_m_cmdID )
        {
            int v9 = unit->_waypoints_count - 1;

            wTo = unit->_waypoints[v9].XZ();

            tgType = BACT_TGT_TYPE_CELL;
        }
        else
        {
            v6 = _world->FindBactByCmdOwn(unit->_m_cmdID, unit->_m_owner);

            if ( v6 )
                tgType = BACT_TGT_TYPE_UNIT;
            else
                tgType = BACT_TGT_TYPE_NONE;
        }
    }
    else
    {
        if ( unit->_primTtype == BACT_TGT_TYPE_UNIT )
        {
            v6 = unit->_primT.pbact;
            tgType = BACT_TGT_TYPE_UNIT;
        }
        else if ( unit->_primTtype == BACT_TGT_TYPE_CELL )
        {
            wTo = unit->_primTpos.XZ();
            tgType = BACT_TGT_TYPE_CELL;
        }
        else
            tgType = BACT_TGT_TYPE_NONE;
    }

    if ( tgType == BACT_TGT_TYPE_NONE )
    {
        tgType = BACT_TGT_TYPE_UNIT;
        v6 = unit;
    }

    if ( _bact_type != BACT_TYPES_TANK && _bact_type != BACT_TYPES_CAR )
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

        SetTarget(&arg67);
    }
    else
    {
        bact_arg124 arg125;

        if ( tgType == BACT_TGT_TYPE_UNIT )
        {
            arg125.to = v6->_position.XZ();
        }
        else
        {
            arg125.to = wTo;
        }

        arg125.steps_cnt = 32;
        arg125.from = _position.XZ();
        arg125.field_12 = 1;

        SetPath(&arg125);

        if ( tgType == BACT_TGT_TYPE_UNIT )
        {
            _m_cmdID = v6->_commandID;
            _m_owner = v6->_owner;
        }
    }
}

size_t NC_STACK_ypabact::func2(IDVList &stak)
{
    for( auto& it : stak )
    {
        IDVPair &val = it.second;

        if ( !val.Skip )
        {
            switch (val.ID)
            {
            case BACT_ATT_VIEWER:
                setBACT_viewer(val.Get<int32_t>());
                break;

            case BACT_ATT_INPUTTING:
                setBACT_inputting(val.Get<int32_t>());
                break;

            case BACT_ATT_EXACTCOLL:
                setBACT_exactCollisions ( val.Get<int32_t>() );
                break;

            case BACT_ATT_BACTCOLL:
                setBACT_bactCollisions ( val.Get<int32_t>() );
                break;

            case BACT_ATT_AIRCONST:
                setBACT_airconst(val.Get<int32_t>());
                break;

            case BACT_ATT_LANDINGONWAIT:
                setBACT_landingOnWait ( val.Get<int32_t>() );
                break;

            case BACT_ATT_YOURLS:
                setBACT_yourLastSeconds(val.Get<int32_t>());
                break;

            case BACT_ATT_VISPROT:
                setBACT_visProto( val.Get<NC_STACK_base *>());
                break;

            case BACT_ATT_AGGRESSION:
                setBACT_aggression(val.Get<int32_t>());
                break;

            case BACT_ATT_VPTRANSFORM:
                setBACT_vpTransform( val.Get<TFEngine::TForm3D *>());
                break;

            case BACT_ATT_EXTRAVIEWER:
                setBACT_extraViewer ( val.Get<int32_t>() );
                break;

            case BACT_ATT_ALWAYSRENDER:
                setBACT_alwaysRender ( val.Get<int32_t>() );
                break;

            default:
                break;
            }
        }
    }

    return 1;
}


void NC_STACK_ypabact::FixSectorFall()
{
    ypaworld_arg136 arg136;
    arg136.stPos = vec3d(_position.x, -30000.0, _position.z);
    arg136.vect = vec3d(0.0, 50000.0, 0.0);
    arg136.flags = 0;

    _world->ypaworld_func136(&arg136);

    if ( arg136.isect )
        _position.y = arg136.isectPos.y - 50.0;
    else
        _position.y = _pSector->height  - 50.0;
}


void NC_STACK_ypabact::FixBeyondTheWorld()
{
    int maxX = _world->getYW_mapSizeX();
    int maxY = _world->getYW_mapSizeY();

    float mx = maxX * 1200.0;
    float my = -(maxY * 1200.0);

    if ( _position.x > mx )
        _position.x = mx - 600.0;

    if ( _position.x < 0.0 )
        _position.x = 600.0;

    if ( _position.z < my )
        _position.z = my + 600.0;

    if ( _position.z > 0.0 )
        _position.z = -600.0;

    FixSectorFall();
}

void sub_481F94(NC_STACK_ypabact *bact)
{
    for (World::MissileList::iterator it = bact->_missiles_list.begin(); it != bact->_missiles_list.end(); )
    {
        NC_STACK_ypamissile *misl = *it;
        if ( misl->getBACT_yourLastSeconds() <= 0 )
        {
            setTarget_msg arg67;
            arg67.tgt_type = BACT_TGT_TYPE_NONE;
            arg67.priority = 0;

            misl->SetTarget(&arg67);

            misl->_parent = NULL;

            misl->Release();

            it = bact->_missiles_list.erase(it);
        }
        else
            it++;
    }
}


void NC_STACK_ypabact::Update(update_msg *arg)
{
    if ( _kidRef.IsListType(World::BLIST_CACHE) ) // Do not update units in dead list
        return;
        
    static TFEngine::TForm3D bact_cam;
    TFEngine::Engine.SetViewPoint(&bact_cam);

    yw_130arg sect_info;
    sect_info.pos_x = _position.x;
    sect_info.pos_z = _position.z;

    if ( !_world->ypaworld_func130(&sect_info) )
    {
        FixBeyondTheWorld();

        sect_info.pos_x = _position.x;
        sect_info.pos_z = _position.z;

        _world->ypaworld_func130(&sect_info);
    }

    cellArea *oldcell = _pSector;

    _sectX = sect_info.sec_x;
    _sectY = sect_info.sec_z;

//    bact->pos_x_cntr = sect_info.pos_x_cntr;
//    bact->pos_y_cntr = sect_info.pos_y_cntr;

    _pSector = sect_info.pcell;

    if ( oldcell != sect_info.pcell ) // If cell changed
    {
        _cellRef.Detach();  //Remove unit from old cell
        _cellRef = _pSector->unitsList.push_back(this);  // Add unit to new cell
    }

    // Test if bact fall through sector
    if ( _pSector->height + 1000.0 < _position.y )
        FixSectorFall();

    NC_STACK_ypabact *roboHost = _world->getYW_userHostStation();

    if ( _pSector->w_type == 6 && _bact_type == BACT_TYPES_ROBO && this == roboHost )
        ((NC_STACK_yparobo *)roboHost)->ypabact_func65__sub0();

    if ( !(_status_flg & BACT_STFLAG_DEATH1) && _energy <= 0 && _bact_type != BACT_TYPES_MISSLE )
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

        _status = BACT_STATUS_DEAD;
        _status_flg &= ~BACT_STFLAG_LAND;
    }

    _clock += arg->frameTime;

    AI_layer1(arg);

    for( NC_STACK_ypamissile *misl : Utils::IterateListCopy<NC_STACK_ypamissile *>(_missiles_list))
        misl->Update(arg);

    sub_481F94(this);

    if ( _oflags & BACT_OFLAG_VIEWER )
    {
        if ( _oflags & BACT_OFLAG_EXTRAVIEW )
            bact_cam.locPos = _position + _rotation.Transpose().Transform(_viewer_position);
        else
            bact_cam.locPos = _position;

        if ( _oflags & BACT_OFLAG_EXTRAVIEW )
            bact_cam.locSclRot = _viewer_rotation;
        else
            bact_cam.locSclRot = _rotation;

        GFXEngine::GFXe.getC3D()->matrixAspectCorrection(bact_cam.locSclRot, false);
    }

    _tForm.locPos = _position;

    if ( _status_flg & BACT_STFLAG_SCALE )
        _tForm.locSclRot = _rotation.Transpose() * mat3x3::Scale( _scale );
    else
        _tForm.locSclRot = _rotation.Transpose();

    int numbid = arg->units_count;

    arg->units_count = 0;

    /** 
     * Because missiles can cause 'ModifyEnergy' and 'Die' methods of upper bact
     * in hierarchy Update->Update - it can remove all bacts from list in
     * iteration. So we just needs to get safe copy of list for modify without
     * worry of lists modify.
     **/
    for ( NC_STACK_ypabact *bnod : _kidList.safe_iter() )
    {
        bnod->Update(arg);

        arg->units_count++;
    }

    arg->units_count = numbid;

    _soundcarrier.field_0 = _position;

    if ( _oflags & BACT_OFLAG_VIEWER )
        _soundcarrier.field_0 += _rotation.AxisY() * 400.0;

    _soundcarrier.field_C = _fly_dir * _fly_dir_length;

    SFXEngine::SFXe.sb_0x4242e0(&_soundcarrier);
}

void NC_STACK_ypabact::Render(baseRender_msg *arg)
{
    if ( _current_vp.base )
    {
        if ( !(_status_flg & BACT_STFLAG_NORENDER) )
        {
            if ( !(_oflags & BACT_OFLAG_VIEWER) || _oflags & BACT_OFLAG_ALWAYSREND )
            {
                _current_vp.trigo->locPos = _tForm.locPos;
                _current_vp.trigo->locSclRot = _tForm.locSclRot;

                arg->ownerID = _gid;
                _current_vp.base->base_func77(arg);
            }
        }
    }

    for (int i = 0; i < 3; i++)
    {
        extra_vproto *bd = &_vp_extra[i];

        if ( bd->vp.base )
        {
            if ( bd->flags & EVPROTO_FLAG_ACTIVE )
            {
                bd->vp.trigo->locPos = bd->pos;

                if ( bd->flags & EVPROTO_FLAG_SCALE )
                    bd->vp.trigo->locSclRot = bd->rotate.Transpose() * mat3x3::Scale( vec3d(bd->scale, bd->scale, bd->scale) );
                else
                    bd->vp.trigo->locSclRot = bd->rotate.Transpose();

                arg->ownerID = _gid;

                bd->vp.base->base_func77(arg);
            }
        }
    }
}

void NC_STACK_ypabact::SetTarget(setTarget_msg *arg)
{
    _assess_time = 0;
    yw_130arg arg130;

    if ( _status_flg & BACT_STFLAG_DEATH1 && arg->tgt_type == BACT_TGT_TYPE_UNIT )
    {
        ypa_log_out("ALARM!!! bact-settarget auf logische Leiche owner %d, class %d, prio %d\n", _owner, _bact_type, arg->priority);
    }
    else if ( arg->priority )
    {
        if ( _secndTtype == BACT_TGT_TYPE_UNIT )
            _secndT.pbact->_attackersList.remove(this);

        switch ( arg->tgt_type )
        {
        case BACT_TGT_TYPE_CELL:
        case BACT_TGT_TYPE_CELL_IND:
            _secndTtype = BACT_TGT_TYPE_CELL;

            arg130.pos_x = arg->tgt_pos.x;
            arg130.pos_z = arg->tgt_pos.z;

            if ( arg130.pos_x < 1210.0 )
                arg130.pos_x = 1210.0;

            if ( arg130.pos_x > _wrldX - 1210.0 )
                arg130.pos_x = _wrldX - 1210.0;

            if ( arg130.pos_z > -1210.0 )
                arg130.pos_z = -1210.0;

            if ( arg130.pos_z < _wrldY + 1210.0 )
                arg130.pos_z = _wrldY + 1210.0;

            if ( _world->ypaworld_func130(&arg130) )
            {
                _secndT.pcell = arg130.pcell;
                _sencdTpos.x = arg130.pos_x;
                _sencdTpos.z = arg130.pos_z;
                _sencdTpos.y = arg130.pcell->height;
            }
            else
            {
                _secndTtype = BACT_TGT_TYPE_NONE;
                _secndT.pcell = NULL;
            }
            break;

        case BACT_TGT_TYPE_UNIT:
        case BACT_TGT_TYPE_UNIT_IND:
            _secndT.pbact = arg->tgt.pbact;
            _secndTtype = BACT_TGT_TYPE_UNIT;

            if ( _secndT.pbact )
            {
                if ( _secndT.pbact->_status_flg & BACT_STFLAG_DEATH1 )
                {
                    ypa_log_out("totes vehicle als nebenziel, owner %d, class %d\n", arg->tgt.pbact->_owner, arg->tgt.pbact->_bact_type);
                    _secndTtype = BACT_TGT_TYPE_NONE;
                }
                else
                {
                    _sencdTpos = _secndT.pbact->_position;
                    _secndT.pbact->_attackersList.push_back(this);
                }
            }
            else
            {
                ypa_log_out("Yppsn\n");
                _secndTtype = BACT_TGT_TYPE_NONE;
            }
            break;

        case BACT_TGT_TYPE_FRMT:
            _secndTtype = BACT_TGT_TYPE_FRMT;
            _sencdTpos = arg->tgt_pos;
            break;

        case BACT_TGT_TYPE_NONE:
            _secndT.pbact = NULL;
            _secndTtype = BACT_TGT_TYPE_NONE;
            break;

        default:
            _secndTtype = arg->tgt_type;
            break;
        }
    }
    else
    {
        if ( _primTtype == BACT_TGT_TYPE_UNIT )
            _primT.pbact->_attackersList.remove(this);

        switch ( arg->tgt_type )
        {
        case BACT_TGT_TYPE_CELL:
        case BACT_TGT_TYPE_CELL_IND:
            _primT_cmdID = 0;
            _primTtype = BACT_TGT_TYPE_CELL;

            arg130.pos_x = arg->tgt_pos.x;
            arg130.pos_z = arg->tgt_pos.z;

            if ( arg130.pos_x < 1210.0 )
                arg130.pos_x = 1210.0;

            if ( arg130.pos_x > _wrldX - 1210.0 )
                arg130.pos_x = _wrldX - 1210.0;

            if ( arg130.pos_z > -1210.0 )
                arg130.pos_z = -1210.0;

            if ( arg130.pos_z < _wrldY + 1210.0 )
                arg130.pos_z = _wrldY + 1210.0;

            if ( _world->ypaworld_func130(&arg130) )
            {
                _primT.pcell = arg130.pcell;
                _primTpos.x = arg130.pos_x;
                _primTpos.z = arg130.pos_z;
                _primTpos.y = arg130.pcell->height;
            }
            else
            {
                _primTtype = BACT_TGT_TYPE_NONE;
                _primT.pcell = NULL;
            }
            break;

        case BACT_TGT_TYPE_UNIT:
        case BACT_TGT_TYPE_UNIT_IND:
            _primT.pbact = arg->tgt.pbact;
            _primTtype = BACT_TGT_TYPE_UNIT;

            if ( _primT.pbact )
            {
                if ( _primT.pbact->_status_flg & BACT_STFLAG_DEATH1 )
                {
                    ypa_log_out("totes vehicle als hauptziel, owner %d, class %d - ich bin class %d\n", arg->tgt.pbact->_owner, arg->tgt.pbact->_bact_type, _bact_type);
                    _primTtype = BACT_TGT_TYPE_NONE;
                    return;
                }

                _primTpos = _primT.pbact->_position;

                _primT.pbact->_attackersList.push_back(this);

                _primT_cmdID = arg->tgt.pbact->_commandID;
            }
            else
            {
                ypa_log_out("PrimT. without a pointer\n");
                _primTtype = BACT_TGT_TYPE_NONE;
            }
            break;

        case BACT_TGT_TYPE_FRMT:
            _primTtype = BACT_TGT_TYPE_FRMT;
            _primT_cmdID = 0;
            _primTpos = arg->tgt_pos;
            break;

        case BACT_TGT_TYPE_NONE:
            _primT.pbact = NULL;
            _waypoints_count = 0;
            _primTtype = BACT_TGT_TYPE_NONE;
            _status_flg &= ~BACT_STFLAG_WAYPOINT;
            break;

        case BACT_TGT_TYPE_DRCT:
            _target_dir = arg->tgt_pos;
            _primTtype = BACT_TGT_TYPE_DRCT;
            _primT.pbact = NULL;
            _primT_cmdID = 0;
            break;

        default:
            _primTtype = arg->tgt_type;
            break;
        }

        if ( arg->tgt_type == BACT_TGT_TYPE_CELL || arg->tgt_type == BACT_TGT_TYPE_UNIT )
        {
            for ( NC_STACK_ypabact* &node : _kidList )
            {
                if ( node->_status != BACT_STATUS_DEAD)
                {
                    node->SetTarget(arg);
                    if ( !(_status_flg & BACT_STFLAG_WAYPOINT)  )
                        node->_status_flg &= ~(BACT_STFLAG_WAYPOINT | BACT_STFLAG_WAYPOINTCCL);
                }
            }
        }
    }
}

void NC_STACK_ypabact::AI_layer1(update_msg *arg)
{
    setTarget_msg v36;

    if ( _mass == 1.0 )
    {
        if ( _status_flg & BACT_STFLAG_DEATH2 )
        {
            _yls_time -= arg->frameTime;

            if ( _yls_time < 0 )
                _world->ypaworld_func144(this);
        }
        else
        {
            setState_msg v37;
            v37.newStatus = BACT_STATUS_NOPE;
            v37.unsetFlags = 0;
            v37.setFlags = BACT_STFLAG_DEATH2;

            SetState(&v37);

            _yls_time = 6000;
        }
        return;
    }

    if ( _bact_type != BACT_TYPES_MISSLE )
        EnergyInteract(arg);

    if ( _status == BACT_STATUS_DEAD )
    {
        if ( _status_flg & BACT_STFLAG_LAND )
            _yls_time -= arg->frameTime;
    }

    _airconst = _airconst_static;

    _soundcarrier.samples_data[0].pitch = _pitch;
    _soundcarrier.samples_data[0].volume = _volume;

    if ( _clock - _AI_time1 < 250 || 
         _primTtype == BACT_TGT_TYPE_DRCT || 
         _bact_type == BACT_TYPES_GUN || 
         _status == BACT_STATUS_DEAD || 
         _status == BACT_STATUS_BEAM ||
         _status == BACT_STATUS_CREATE )
    {
        AI_layer2(arg);
        return;
    }

    _AI_time1 = _clock;
    _target_vec = vec3d(0.0, 0.0, 0.0);

    if ( _clock - _brkfr_time > 5000 )
    {
        _brkfr_time = _clock;

        StuckFree(arg);
    }

    if ( _status == BACT_STATUS_NORMAL && _primTtype != BACT_TGT_TYPE_NONE )
    {
        if ( _primTtype == BACT_TGT_TYPE_UNIT )
        {
            _target_vec = _primT.pbact->_position - _position;

            if ( _primT.pbact->_status != BACT_STATUS_DEAD)
                _primTpos = _primT.pbact->_position;
        }
        else
        {
            _target_vec = _primTpos - _position;
        }

        if ( _target_vec.length() > 2000.0 )
            _target_vec.y = 0;

        if ( IsParentMyRobo() &&
             (_oflags & BACT_OFLAG_VIEWER) )
        {
            bool doFight = _target_vec.length() < 800.0;

            int unitId = 0;
            for (NC_STACK_ypabact* &node : _kidList)
            {
                if ( node->_status != BACT_STATUS_DEAD )
                {
                    if ( doFight )
                    {
                        v36.tgt_type = _primTtype;
                        v36.priority = 0;
                        v36.tgt.pbact = _primT.pbact;
                        v36.tgt_pos = _primTpos;
                    }
                    else
                    {
                        bact_arg94 v35;
                        v35.field_0 = unitId;
                        GetFormationPosition(&v35);

                        v36.tgt_type = BACT_TGT_TYPE_FRMT;
                        v36.priority = 0;
                        v36.tgt_pos = v35.pos1;
                    }

                    node->SetTarget(&v36);
                }
                unitId++;
            }
        }
    }

    if ( _primTtype == BACT_TGT_TYPE_NONE)
    {
        if ( _host_station && _primT_cmdID )
        {
            v36.priority = _primT_cmdID;

            if ( _host_station->yparobo_func132(&v36) )
            {
                v36.priority = 0;
            }
            else
            {
                _primT_cmdID = 0;

                v36.tgt_type = BACT_TGT_TYPE_CELL;
                v36.priority = 0;
                v36.tgt_pos = _primTpos;
            }

            SetTarget(&v36);
        }
    }

    if ( _oflags & BACT_OFLAG_USERINPT )
    {
        if ( _primTtype == BACT_TGT_TYPE_UNIT && 
             _primT.pbact )
        {
            if ( !((1 << _owner) & _primT.pbact->_pSector->view_mask) )
            {
                v36.tgt_type = BACT_TGT_TYPE_NONE;
                v36.priority = 0;

                SetTarget(&v36);
            }
        }
    }
    else if ( _vp_active == 6 && _status == BACT_STATUS_NORMAL )
    {
        setState_msg v38;
        v38.newStatus = BACT_STATUS_NORMAL;
        v38.setFlags = 0;
        v38.unsetFlags = 0;
        SetState(&v38);
    }

    AI_layer2(arg);
}

void NC_STACK_ypabact::AI_layer2(update_msg *arg)
{
    if ( (_clock - _AI_time2) < 250 
       || _owner == 0 
       || _secndTtype == BACT_TGT_TYPE_DRCT 
       || _status == BACT_STATUS_CREATE 
       || _status == BACT_STATUS_DEAD 
       || _status == BACT_STATUS_BEAM )
    {
        if ( _oflags & BACT_OFLAG_USERINPT )
            User_layer(arg);
        else
            AI_layer3(arg);
        
        return;
    }
    
    _AI_time2 = _clock;

    if ( _status_flg & BACT_STFLAG_ESCAPE && 
         _target_vec.XZ().length() > 3600.0 )
    {
        setTarget_msg arg67;
        arg67.tgt_type = BACT_TGT_TYPE_NONE;
        arg67.priority = 1;

        for(NC_STACK_ypabact* &nod : _kidList)
            nod->SetTarget(&arg67);

        SetTarget(&arg67);

        if ( _oflags & BACT_OFLAG_USERINPT )
            User_layer(arg);
        else
            AI_layer3(arg);
        return;
    }

    NC_STACK_ypabact *wee = _world->getYW_userHostStation();

    if ( _status == BACT_STATUS_NORMAL || _status == BACT_STATUS_IDLE )
    {
        if ( _clock - _search_time1 > 500 )
        {
            _search_time1 = _clock;

            bact_arg90 arg90;
            arg90.field_8 = 1;
            arg90.unit = this;
            arg90.ret_unit = NULL;

            GetSectorTarget(&arg90);

            if ( arg90.ret_unit )
            {
                if ( arg90.ret_unit->_bact_type != BACT_TYPES_ROBO && IsParentMyRobo() && _host_station == wee && arg90.ret_unit->_commandID != _fe_cmdID && _clock - _fe_time > 45000 )
                {
                    bool isRoboGun = false;
                    if ( arg90.ret_unit->_bact_type == BACT_TYPES_GUN )
                    {
                        NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( arg90.ret_unit );
                        isRoboGun = gun->IsRoboGun();
                    }

                    if ( !isRoboGun )
                    {
                        _fe_cmdID = arg90.ret_unit->_commandID;
                        _fe_time = _clock;

                        robo_arg134 arg134;
                        arg134.field_4 = 7;
                        arg134.field_8 = arg90.ret_unit->_commandID;
                        arg134.field_C = 0;
                        arg134.field_10 = 0;
                        arg134.unit = this;
                        arg134.field_14 = 46;

                        _host_station->placeMessage(&arg134);
                    }
                }
            }

            if ( _status == BACT_STATUS_IDLE || 
                 (  _aggr >= 50 && 
                  !(_status_flg & BACT_STFLAG_ESCAPE) && 
                     (_secndTtype == BACT_TGT_TYPE_NONE || 
                      _secndTtype == BACT_TGT_TYPE_CELL || 
                      _secndTtype == BACT_TGT_TYPE_FRMT) ) )
            {
                if ( arg90.ret_unit )
                {
                    _secndT_cmdID = arg90.ret_unit->_commandID;

                    setTarget_msg arg67;
                    arg67.tgt_type = BACT_TGT_TYPE_UNIT;
                    arg67.priority = 1;
                    arg67.tgt.pbact = arg90.ret_unit;

                    SetTarget(&arg67);
                }

                if ( (_clock - _search_time2) > 2000 && 
                     _aggr == 75 && 
                    !(_oflags & BACT_OFLAG_VIEWER) && 
                     IsParentMyRobo() &&
                     (_secndTtype == BACT_TGT_TYPE_FRMT || 
                      _secndTtype == BACT_TGT_TYPE_NONE) )
                {
                    if (  _position.x > 1260.0 && 
                          _position.x < _wrldX + -1260.0 && 
                          _position.z < -1260.0 && 
                          _position.z > _wrldY + 1260.0 )
                    {
                        _search_time2 = _clock;

                        if ( _owner != _pSector->owner )
                        {
                            setTarget_msg arg67;
                            arg67.priority = 1;
                            arg67.tgt_type = BACT_TGT_TYPE_CELL;
                            arg67.tgt_pos.x = _position.x;
                            arg67.tgt_pos.z = _position.z;

                            SetTarget(&arg67);
                        }
                    }
                }

                if ( IsParentMyRobo() && _secndTtype == BACT_TGT_TYPE_CELL )
                {
                    for(NC_STACK_ypabact* &nod : _kidList)
                    {
                        if ( nod->_secndTtype == BACT_TGT_TYPE_NONE || nod->_secndTtype == BACT_TGT_TYPE_FRMT )
                        {
                            setTarget_msg arg67;
                            arg67.tgt_type = BACT_TGT_TYPE_CELL;
                            arg67.tgt_pos = _sencdTpos;
                            arg67.priority = 1;
                            nod->SetTarget(&arg67);
                        }
                    }
                }
            }
        }

        if ( _secndTtype == BACT_TGT_TYPE_UNIT )
            _target_vec = _secndT.pbact->_position - _position;
        else if ( _secndTtype == BACT_TGT_TYPE_CELL)
            _target_vec = _sencdTpos - _position;

        if ( _target_vec.length() > 2000.0 )
            _target_vec.y = 0;
    }

    if ( _oflags & BACT_OFLAG_USERINPT )
    {
        if ( _secndTtype == BACT_TGT_TYPE_UNIT && _secndT.pbact )
        {
            if ( !((1 << _owner) & _secndT.pbact->_pSector->view_mask) || 
                  (_position.XZ() - _secndT.pbact->_position.XZ()).length() > 2160.0 )
            {
                setTarget_msg arg67;
                arg67.priority = 1;
                arg67.tgt_type = BACT_TGT_TYPE_NONE;
                SetTarget(&arg67);
            }
        }
    }
    
    if ( _oflags & BACT_OFLAG_USERINPT )
        User_layer(arg);
    else
        AI_layer3(arg);
}

void AI_layer3__sub1(NC_STACK_ypabact *bact, update_msg *arg)
{
    bact->_thraction = bact->_force;

    float v39 = arg->frameTime / 1000.0;

    float top = -bact->_target_dir.y;

    if ( top == 1.0 )
        top = 0.99998999;

    if ( top == -1.0 )
        top = -0.99998999;

    float weight = bact->_mass * 9.80665;
    float thraction = bact->_thraction;

    if ( thraction == 0.0 )
        thraction = 0.1;

    float v5 = sqrt( (1.0 - POW2(top)) ) * (top * -0.5);

    float v6 = (1.0 - 0.25 * POW2(top) + 0.25 * POW2(top) * POW2(top)) * (POW2(weight) / POW2(thraction));

    float v58 = sqrt( (1.0 - v6) ) + (weight * v5 / thraction);

    vec3d tmp;
    tmp.y = -cos( clp_asin(v58) );

    if ( bact->_target_dir.z != 0.0 )
    {
        float v62 = (1.0 - POW2(tmp.y)) / (POW2(bact->_target_dir.x) / POW2(bact->_target_dir.z) + 1.0);

        if ( v62 < 0.0 )
            v62 = 0.0;

        tmp.z = sqrt(v62);

        if ( bact->_target_dir.z < 0.0 )
            tmp.z = -tmp.z;
    }
    else
    {
        tmp.z = 0.0;
    }

    if ( bact->_target_dir.x != 0.0 )
    {
        float v57 = (1.0 - POW2(tmp.y)) / (POW2(bact->_target_dir.z) / POW2(bact->_target_dir.x) + 1.0);

        if ( v57 < 0.0 )
            v57 = 0.0;

        tmp.x = sqrt(v57);

        if ( bact->_target_dir.x < 0.0 )
            tmp.x = -tmp.x;
    }
    else
    {
        tmp.x = 0.0;
    }

    vec3d vaxis = (-bact->_rotation.AxisY()) * tmp;;

    if ( vaxis.normalise() != 0.0 )
    {
        float maxrot = bact->_maxrot * v39;

        float v56 = clp_acos( tmp.dot( -bact->_rotation.AxisY() ) );

        if ( v56 > maxrot )
            v56 = maxrot;

        if ( fabs(v56) > BACT_MIN_ANGLE )
            bact->_rotation *= mat3x3::AxisAngle(vaxis, v56);
    }
}

void AI_layer3__sub0(NC_STACK_ypabact *bact, int a2)
{
    if ( clp_acos(bact->_rotation.m11) > 0.003 && (bact->_fly_dir.z != 0.0 || bact->_fly_dir.x != 0.0) && bact->_fly_dir_length > 0.0 )
    {
        float v11 = a2 / 1000.0;

        vec2d flydir = bact->_fly_dir.XZ();
        vec2d axisZ = bact->_rotation.AxisZ().XZ();

        float tmpsq = flydir.length();

        NDIV_CARRY(tmpsq);

        float v18 = flydir.dot(axisZ) / tmpsq;

        tmpsq = axisZ.length();

        NDIV_CARRY(tmpsq);

        float v20 = clp_acos( v18 / tmpsq );

        float v13 = bact->_maxrot * v11 * (fabs(v20) * 0.8 + 0.2);

        if ( v20 > v13 )
            v20 = v13;

        if ( bact->_fly_dir.x * bact->_rotation.m22 - bact->_rotation.m20 * bact->_fly_dir.z < 0.0 )
            v20 = -v20;

        bact->_rotation *= mat3x3::RotateY(-v20);
    }
}

void NC_STACK_ypabact::AI_layer3(update_msg *arg)
{
    float v75 = arg->frameTime / 1000.0;

    float v77 = _target_vec.length();

    if ( v77 > 0.0 )
        _target_dir = _target_vec / v77;

    int v82 = _oflags & BACT_OFLAG_VIEWER;
    int v70 = _oflags & BACT_OFLAG_EXACTCOLL;

    int v80 = _world->ypaworld_func145(this);

    int v79;

    if ( v82 )
        v79 = _viewer_radius;
    else
        v79 = _radius;

    switch ( _status )
    {
    case BACT_STATUS_NORMAL:
    {
        if ( _oflags & BACT_OFLAG_BACTCOLL )
        {
            if ( (v80 || (_secndTtype == BACT_TGT_TYPE_NONE && v77 < 1200.0)) && !(_status_flg & BACT_STFLAG_LAND) )
            {
                CollisionWithBact(arg->frameTime);
            }
        }

        if ( !_primTtype && !_secndTtype )
        {
            _status = BACT_STATUS_IDLE;

            if ( _status_flg & BACT_STFLAG_FIRE )
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
        arg136.stPos = _old_pos;
        arg136.vect = _position - _old_pos;
        arg136.vect.y = 0;

        float len = arg136.vect.length();

        if ( len > 0.0 )
            arg136.vect *= 300.0 / len;
        else
            arg136.vect = _rotation.AxisZ() * 300.0;

        arg136.isect = false;
        arg136.flags = 0;

        ypaworld_arg136 arg136_1;
        arg136_1.isect = false;
        arg136_1.flags = 0;

        if ( v82 || (_status_flg & BACT_STFLAG_DODGE_RIGHT) || (v80 && v70) )
        {
            arg136_1.stPos = _old_pos;
            arg136_1.vect.x = arg136.vect.x * 0.93969 - arg136.vect.z * 0.34202;
            arg136_1.vect.y = arg136.vect.y;
            arg136_1.vect.z = arg136.vect.z * 0.93969 + arg136.vect.x * 0.34202;

            _world->ypaworld_func136(&arg136_1);
        }

        ypaworld_arg136 arg136_2;
        arg136_2.isect = false;
        arg136_2.flags = 0;

        if ( v82 || (_status_flg & BACT_STFLAG_DODGE_LEFT) || (v80 && v70) )
        {
            arg136_2.stPos = _old_pos;
            arg136_2.vect.x = arg136.vect.x * 0.93969 + arg136.vect.z * 0.34202;
            arg136_2.vect.y = arg136.vect.y;
            arg136_2.vect.z = arg136.vect.z * 0.93969 - arg136.vect.x * 0.34202;

            _world->ypaworld_func136(&arg136_2);
        }

        if ( v82 || !(_status_flg & (BACT_STFLAG_DODGE_LEFT | BACT_STFLAG_DODGE_RIGHT)) || (v80 && v70) )
            _world->ypaworld_func136(&arg136);

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
            _status_flg &= ~(BACT_STFLAG_DODGE_LEFT | BACT_STFLAG_DODGE_RIGHT | BACT_STFLAG_MOVE);
            _status_flg |= BACT_STFLAG_MOVE;
        }

        if ( !(_status_flg & (BACT_STFLAG_DODGE_LEFT | BACT_STFLAG_DODGE_RIGHT)) )
        {

            if ( arg136_1.isect == 1 && arg136_2.isect == 1 )
            {
                if ( arg136_1.tVal >= arg136_2.tVal )
                    _status_flg |= BACT_STFLAG_DODGE_LEFT;
                else
                    _status_flg |= BACT_STFLAG_DODGE_RIGHT;
            }

            if ( arg136_1.isect == 1 && !arg136_2.isect )
                _status_flg |= BACT_STFLAG_DODGE_RIGHT;

            if ( !arg136_1.isect && arg136_2.isect == 1 )
                _status_flg |= BACT_STFLAG_DODGE_LEFT;

            if ( !arg136_1.isect && !arg136_2.isect && arg136.isect == 1 )
                _status_flg |= BACT_STFLAG_DODGE_LEFT;
        }

        float v21 = _mass * 9.80665;

        if ( v21 <= _force )
            v21 = _force;

        float v88;

        if ( _airconst >= 10.0 )
            v88 = _airconst;
        else
            v88 = 10.0;


        ypaworld_arg136 arg136_3;

        arg136_3.vect.x = (_fly_dir.x * 200.0 * _fly_dir_length) / (v21 / v88);

        if ( arg136_3.vect.x < -200.0 )
            arg136_3.vect.x = -200.0;

        if ( arg136_3.vect.x > 200.0 )
            arg136_3.vect.x = 200.0;

        arg136_3.vect.y = _height;

        arg136_3.vect.z = (_fly_dir.z * 200.0 * _fly_dir_length) / (v21 / v88);

        if ( arg136_3.vect.z < -200.0 )
            arg136_3.vect.z = -200.0;

        if ( arg136_3.vect.z > 200.0 )
            arg136_3.vect.z = 200.0;

        arg136_3.stPos = _old_pos;
        arg136_3.flags = 0;

        _world->ypaworld_func136(&arg136_3);

        if ( arg136_3.isect )
        {
            _target_dir.y = -(1.0 - arg136_3.tVal);
        }
        else
        {
            NC_STACK_ypabact *a4 = _world->getYW_userVehicle();

            if ( ((_secndTtype != BACT_TGT_TYPE_UNIT || (a4 != _secndT.pbact && _secndT.pbact->_bact_type != BACT_TYPES_ROBO)) &&
                    (_primTtype != BACT_TGT_TYPE_UNIT || (a4 != _primT.pbact && _primT.pbact->_bact_type != BACT_TYPES_ROBO)))
                    || _target_dir.y >= -0.01 )
            {
                if ( _target_dir.y < 0.15 )
                    _target_dir.y = 0.15;
            }
        }

        if ( _status_flg & (BACT_STFLAG_DODGE_LEFT | BACT_STFLAG_DODGE_RIGHT) )
            _target_dir.y = -0.2;

        if ( arg136_3.isect )
        {
            if ( arg136_3.tVal * _height < _radius && _fly_dir.y > 0.0 )
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
            _status_flg &= ~BACT_STFLAG_LCRASH;
        }

        if ( _target_dir.y != 0.0 )
            _target_dir.normalise();

        float tmpsq = arg136.vect.XZ().length();

        NDIV_CARRY(tmpsq);

        if ( _status_flg & BACT_STFLAG_DODGE_LEFT )
        {
            _target_dir.x = -arg136.vect.z / tmpsq;
            _target_dir.z = arg136.vect.x / tmpsq;
        }
        else if ( _status_flg & BACT_STFLAG_DODGE_RIGHT )
        {
            _target_dir.x = arg136.vect.z / tmpsq;
            _target_dir.z = -arg136.vect.x / tmpsq;
        }

        AI_layer3__sub1(this, arg);

        /*if ( bact->status_flg & BACT_STFLAG_DODGE ) //Unused flag
            bact->fly_dir_length *= 0.95;*/

        _thraction = (0.85 - _target_dir.y) * _force;

        move_msg arg74;
        arg74.flag = 0;
        arg74.field_0 = arg->frameTime / 1000.0;

        Move(&arg74);

        AI_layer3__sub0(this, arg->frameTime);

        bact_arg75 arg75;

        arg75.fperiod = v75;
        arg75.g_time = _clock;

        if ( _secndTtype == BACT_TGT_TYPE_UNIT )
        {
            arg75.target.pbact = _secndT.pbact;
            arg75.prio = 1;

            FightWithBact(&arg75);
        }
        else if ( _secndTtype == BACT_TGT_TYPE_CELL )
        {
            arg75.pos = _sencdTpos;
            arg75.target.pcell = _secndT.pcell;
            arg75.prio = 1;

            FightWithSect(&arg75);
        }
        else if ( _primTtype == BACT_TGT_TYPE_UNIT )
        {
            arg75.target.pbact = _primT.pbact;
            arg75.prio = 0;

            FightWithBact(&arg75);
        }
        else if ( _primTtype == BACT_TGT_TYPE_CELL )
        {
            arg75.pos = _primTpos;
            arg75.target.pcell = _primT.pcell;
            arg75.prio = 0;

            FightWithSect(&arg75);
        }
        else
        {
            if ( _status_flg & BACT_STFLAG_FIRE )
            {
                setState_msg arg78;
                arg78.unsetFlags = BACT_STFLAG_FIRE;
                arg78.newStatus = BACT_STATUS_NOPE;
                arg78.setFlags = 0;

                SetState(&arg78);
            }

            _status_flg &= ~(BACT_STFLAG_FIGHT_P | BACT_STFLAG_FIGHT_S);
        }
    }
    break;

    case BACT_STATUS_DEAD:
        DeadTimeUpdate(arg);
        break;

    case BACT_STATUS_IDLE:

        if ( _clock - _newtarget_time > 500 )
        {
            _newtarget_time = _clock;

            bact_arg110 arg110;
            arg110.tgType = _secndTtype;
            arg110.priority = 1;

            int v46 = TargetAssess(&arg110);

            arg110.priority = 0;
            arg110.tgType = _primTtype;
            int v48 = TargetAssess(&arg110);

            if ( v46 != TA_IGNORE || v48 != TA_IGNORE )
            {
                setTarget_msg arg67;

                if ( v46 == TA_CANCEL )
                {
                    arg67.tgt_type = BACT_TGT_TYPE_NONE;
                    arg67.priority = 1;
                    SetTarget(&arg67);
                }

                if ( v48 == TA_CANCEL )
                {
                    arg67.tgt_type = BACT_TGT_TYPE_CELL;
                    arg67.tgt_pos.x = _position.x;
                    arg67.tgt_pos.z = _position.z;
                    arg67.priority = 0;
                    SetTarget(&arg67);
                }

                if ( _primTtype || _secndTtype )
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

        if ( _oflags & BACT_OFLAG_LANDONWAIT )
        {
            _thraction = 0;

            if ( _status_flg & BACT_STFLAG_LAND )
            {
                setState_msg arg78;
                arg78.unsetFlags = 0;
                arg78.setFlags = 0;
                arg78.newStatus = BACT_STATUS_IDLE;
                SetState(&arg78);

                ypaworld_arg136 v52;
                v52.stPos = _position;
                v52.vect = vec3d(0.0, _overeof + 50.0, 0.0);
                v52.flags = 0;

                _world->ypaworld_func136(&v52);

                if ( v52.isect )
                    _position.y = v52.isectPos.y - _overeof;
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
    _airconst = _airconst_static;

    float v106 = arg->frameTime / 1000.0;

    if ( _status == BACT_STATUS_NORMAL || _status == BACT_STATUS_IDLE )
    {

        _old_pos = _position;

        if ( _oflags & BACT_OFLAG_BACTCOLL )
        {
            if ( !(_status_flg & BACT_STFLAG_LAND) )
            {
                CollisionWithBact(arg->frameTime);
            }
        }

        float v98;

        if ( _status_flg & BACT_STFLAG_LAND )
            v98 = 0.1;
        else
            v98 = 1.0;

        setState_msg arg78;

        if ( v98 <= fabs(_fly_dir_length) )
        {
            if ( !(_status_flg & BACT_STFLAG_FIRE) )
            {
                arg78.newStatus = BACT_STATUS_NORMAL;
                arg78.unsetFlags = 0;
                arg78.setFlags = 0;
                SetState(&arg78);
            }

            _status_flg &= ~BACT_STFLAG_LAND;
        }
        else
        {
            ypaworld_arg136 arg136;

            arg136.stPos = _position;
            arg136.vect = vec3d(0.0, 0.0, 0.0);

            float v8;

            if ( _viewer_overeof <= _viewer_radius )
                v8 = _viewer_radius;
            else
                v8 = _viewer_overeof;

            arg136.flags = 0;
            arg136.vect.y = v8 * 1.5;

            _world->ypaworld_func136(&arg136);

            if ( arg136.isect && _thraction <= _mass * 9.80665 )
            {
                _fly_dir_length = 0;
                _status_flg |= BACT_STFLAG_LAND;
                _position.y = arg136.isectPos.y - _viewer_overeof;
                _thraction = _mass * 9.80665;
            }
            else
            {
                _status_flg &= ~BACT_STFLAG_LAND;
            }

            if ( _primTtype != BACT_TGT_TYPE_CELL || (_primTpos.XZ() - _position.XZ()).length() <= 800.0 )
            {
                if ( _status_flg & BACT_STFLAG_LAND )
                {
                    if ( !(_status_flg & BACT_STFLAG_FIRE) )
                    {
                        arg78.unsetFlags = 0;
                        arg78.setFlags = 0;
                        arg78.newStatus = BACT_STATUS_IDLE;
                        SetState(&arg78);
                    }
                }

                _status = BACT_STATUS_NORMAL;
            }
            else
            {
                _status = BACT_STATUS_IDLE;

                if ( _status_flg & BACT_STFLAG_LAND )
                {
                    if ( !(_status_flg & BACT_STFLAG_FIRE) )
                    {
                        arg78.newStatus = BACT_STATUS_IDLE;
                        arg78.unsetFlags = 0;
                        arg78.setFlags = 0;
                        SetState(&arg78);
                    }
                }
            }
        }

        float v110 = arg->inpt->Sliders[1] * _maxrot * v106;
        float v103 = -arg->inpt->Sliders[0] * _maxrot * v106;

        if ( (fabs(_fly_dir.y) > 0.98 || _fly_dir_length == 0.0) && _rotation.m11 > 0.996 && arg->inpt->Sliders[1] == 0.0 )
        {
            vec2d axisX = _rotation.AxisX().XZ();;

            if ( axisX.normalise() == 0.0 )
                ypa_log_out("Null on div occur %s:%d\n", __FILE__, __LINE__);

            vec2d axisZ = _rotation.AxisZ().XZ();

            if ( axisZ.normalise() == 0.0 )
                ypa_log_out("Null on div occur %s:%d\n", __FILE__, __LINE__);

            _rotation.SetX( vec3d::X0Z(axisX) );
            _rotation.SetY( vec3d(0.0, 1.0, 0.0) );
            _rotation.SetZ( vec3d::X0Z(axisZ) );
        }

//    float v84 = sqrt( POW2(bact->field_651.m20) + POW2(bact->field_651.m22) );
//    v84 /= sqrt( POW2(bact->field_651.m20) + POW2(bact->field_651.m21) + POW2(bact->field_651.m22) );
//
//    float v75 = v84;
//
//    if ( v84 > 1.0 )
//      v75 = 1.0;

        float v111 = clp_acos( _rotation.AxisX().XZ().length() / _rotation.AxisX().length() );

        if ( _rotation.m01 < 0.0 )
            v111 = -v111;

        if ( fabs(v111) < 0.01 )
            v111 = 0.0;

        float v36 = fabs(v111);

        float v101 = _heading_speed * v36 + _heading_speed * 0.25;

        float v102 = _maxrot * v106   *  v101;

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


        if ( fabs(v104) > _maxrot * 2.0 * v106 * v101 )
        {
            if ( v104 < 0.0 )
                v104 = _maxrot * -2.0 * v106 * v101;

            if ( v104 >= 0.0 )
                v104 = _maxrot * 2.0 * v106 * v101;
        }

        if ( fabs(v104) < 0.001 )
            v104 = 0.0;

        _rotation = mat3x3::RotateX(v110 * 0.5) * _rotation; // local
        _rotation = mat3x3::RotateZ(v104 * 0.5) * _rotation; // local
        _rotation *= mat3x3::RotateY(v103 * 0.5); // global

        _thraction += _force * v106 * 0.5 * arg->inpt->Sliders[2];

        if ( _thraction < 0.0 )
            _thraction = 0;

        if ( _thraction > _force )
            _thraction = _force;

        float v99 = _thraction;

        float v47 = _pSector->height - _position.y;
        float v94 = _height_max_user * 0.8;

        if ( v47 > v94 )
        {
            float v91 = _mass * 9.80665 - _force;
            float v89 = _height_max_user * 0.2;
            float v86 = (v47 - v94) * v91 / v89;

            if ( _thraction > v86 )
                _thraction = v86;

            if ( _thraction < 0.0 )
                _thraction = 0;
        }

        bact_arg79 v61;

        v61.tgType = BACT_TGT_TYPE_DRCT;
        v61.tgt_pos = _rotation.AxisZ();

        bact_arg106 v64;
        v64.field_0 = 5;
        v64.field_4 = _rotation.AxisZ();

        if ( UserTargeting(&v64) )
        {
            v61.target.pbact = v64.ret_bact;
            v61.tgType = BACT_TGT_TYPE_UNIT;
        }

        if ( arg->inpt->Buttons.IsAny({0, 1}) )
        {
            v61.direction = vec3d(0.0, 0.0, 0.0);
            v61.weapon = _weapon;
            v61.g_time = _clock;

            if ( v61.g_time % 2 )
                v61.start_point.x = _fire_pos.x;
            else
                v61.start_point.x = -_fire_pos.x;

            v61.start_point.y = _fire_pos.y;
            v61.start_point.z = _fire_pos.z;
            v61.flags = (arg->inpt->Buttons.Is(1) ? 1 : 0) | 2;

            LaunchMissile(&v61);
        }

        if ( _mgun != -1 )
        {
            if ( _status_flg & BACT_STFLAG_FIRE )
            {
                if ( !(arg->inpt->Buttons.Is(2)) )
                {
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
                    arg78.unsetFlags = 0;
                    arg78.newStatus = BACT_STATUS_NOPE;
                    arg78.setFlags = BACT_STFLAG_FIRE;

                    SetState(&arg78);
                }

                bact_arg105 arg105;

                arg105.field_0 = _rotation.AxisZ();
                arg105.field_C = v106;
                arg105.field_10 = _clock;

                FireMinigun(&arg105);
            }
        }

        if ( arg->inpt->Buttons.Is(3) )
        {
            HandBrake(arg);

            v99 = _thraction;
        }

        if ( _status_flg & BACT_STFLAG_LAND )
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
                arg137.pos = _position;
                arg137.pos2 = _fly_dir;
                arg137.radius = 32.0;
                arg137.collisions = v43;
                arg137.field_30 = 0;
                arg137.coll_max = 10;

                _world->ypaworld_func137(&arg137);

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
                        arg88.pos1 = _fly_dir;



                    Recoil(&arg88);

                    v50 = 1;
                }

                if ( !v50 )
                {
                    ypaworld_arg136 arg136;
                    arg136.stPos = _old_pos;
                    arg136.vect = _position - _old_pos;
                    arg136.flags = 0;

                    _world->ypaworld_func136(&arg136);

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
                    _status_flg &= ~BACT_STFLAG_LCRASH;
                    break;
                }

                if ( !(_soundcarrier.samples_data[5].flags & 2) )
                {
                    if ( !(_status_flg & BACT_STFLAG_LCRASH) )
                    {
                        _status_flg |= BACT_STFLAG_LCRASH;

                        SFXEngine::SFXe.startSound(&_soundcarrier, 5);

                        yw_arg180 arg180;
                        arg180.effects_type = 5;
                        arg180.field_4 = 1.0;
                        arg180.field_8 = v81.x * 10.0 + _position.x;
                        arg180.field_C = v81.z * 10.0 + _position.z;

                        _world->ypaworld_func180(&arg180);
                    }
                }
            }
        }

        _thraction = v99;
    }
    else if ( _status == BACT_STATUS_DEAD )
    {
        DeadTimeUpdate(arg);
    }
}

void NC_STACK_ypabact::AddSubject(NC_STACK_ypabact *kid)
{
    newMaster_msg arg73;

    arg73.bact = this;
    arg73.list = &_kidList;

    kid->SetNewMaster(&arg73);
}

void NC_STACK_ypabact::SetNewMaster(newMaster_msg *arg)
{
    _kidRef.Detach();

    _kidRef = arg->list->push_front(this);
    
    _parent = arg->bact;
}

void NC_STACK_ypabact::Move(move_msg *arg)
{
    _old_pos = _position;

    float weight;

    if ( _status == BACT_STATUS_DEAD )
        weight = _mass * 39.2266;
    else
        weight = _mass * 9.80665;

    float thraction = 0.0;
    vec3d v54(0.0, 0.0, 0.0);

    if ( !(arg->flag & 1) )
    {
        v54 = -_rotation.AxisY();

        thraction = _thraction;

        if ( _oflags & BACT_OFLAG_USERINPT )
        {
            v54.x = fSign(v54.x) * sqrt( fabs(v54.x) );
            v54.y = fSign(v54.y) * v54.y * v54.y;
            v54.z = fSign(v54.z) * sqrt( fabs(v54.z) );
        }
    }

    vec3d v41 = vec3d::OY(weight) + v54 * thraction - _fly_dir * (_fly_dir_length * _airconst);

    float len = v41.length();

    if ( _oflags & BACT_OFLAG_USERINPT )
    {
        if ( v41.y >= 0.0 )
            v41.y *= 3.0;
        else
            v41.y *= 5.0;
    }

    if ( len > 0.0 )
    {
        //vec3d v42 = bact->fly_dir * bact->fly_dir_length + (v41 / len) * (len / bact->mass * arg->field_0);
        vec3d v42 = _fly_dir * _fly_dir_length + v41 * (arg->field_0 / _mass);

        _fly_dir_length = v42.length();

        if ( _fly_dir_length > 0.0 )
            _fly_dir = v42 / _fly_dir_length;
    }

    if ( fabs(_fly_dir_length) > 0.1 )
        _position += _fly_dir * (_fly_dir_length * arg->field_0 * 6.0);

    CorrectPositionInLevelBox(NULL);

    _soundcarrier.samples_data[0].pitch = _pitch;
    _soundcarrier.samples_data[0].volume = _volume;

    float v50;
    if ( _pitch_max <= -0.8 )
        v50 = 1.2;
    else
        v50 = _pitch_max;

    float v30 = fabs(_fly_dir_length) * v50;
    float v31 = _force * _force - _mass * 100.0 * _mass;

    float v43 = v30 / (sqrt(v31) / _airconst_static);

    if ( v43 > v50 )
        v43 = v50;

    if ( _soundcarrier.samples_data[0].psampl )
        _soundcarrier.samples_data[0].pitch += (_soundcarrier.samples_data[0].psampl->SampleRate + _soundcarrier.samples_data[0].pitch) * v43;
}

void NC_STACK_ypabact::FightWithBact(bact_arg75 *arg)
{
    arg->pos = arg->target.pbact->_position;

    vec3d v40 = arg->target.pbact->_position - _position;
    float v45 = v40.normalise();

    bact_arg110 arg110;

    vec3d *foePos;
    bool isSecTarget = false;
    bool isPrimTarget = 0;

    if ( _secndT.pbact == arg->target.pbact )
    {
        foePos = &_secndT.pbact->_position;
        arg110.priority = 1;
        isSecTarget = true;
    }
    else
    {
        foePos = &_primT.pbact->_position;
        arg110.priority = 0;
        isPrimTarget = true;
    }

    NC_STACK_ypabact *a4 = _world->getYW_userHostStation();

    if ( _clock - _assess_time > 500 || _clock < 500 )
    {
        _assess_time = _clock;

        arg110.tgType = BACT_TGT_TYPE_UNIT;
        _atk_ret = TargetAssess(&arg110);
    }

    if ( _atk_ret == TA_FIGHT )
    {
        float foeDistance = ( foePos->XZ() - _position.XZ() ).length();

        if ( _status_flg & BACT_STFLAG_APPROACH )
        {
            _status_flg &= ~BACT_STFLAG_ATTACK;

            if ( (_position.x < 1320.0 || _position.z > -1320.0 || _position.x > _wrldX - 1320.0 || _position.z < _wrldY + 1320.0) || _adist_bact < foeDistance )
            {
                _status_flg &= ~BACT_STFLAG_APPROACH;
            }
            else
            {
                _AI_time2 = _clock;
                _AI_time1 = _clock;
            }
        }
        else
        {
            if ( _sdist_bact <= foeDistance )
            {
                if ( _adist_sector <= foeDistance )
                    _status_flg &= ~BACT_STFLAG_ATTACK;
                else
                    _status_flg |= BACT_STFLAG_ATTACK;
            }
            else
            {
                _status_flg &= ~BACT_STFLAG_ATTACK;

                /*if ( bact->field_3D1 == 2 || (arg->g_time & 1 && bact->field_3D1 == 3) )
                {
                    bact->target_vec.x = bact->fly_dir.x;
                    bact->target_vec.z = bact->fly_dir.z;
                }
                else*/
                {
                    _target_vec.x = -_fly_dir.x;
                    _target_vec.z = -_fly_dir.z;
                }

                _AI_time2 = _clock;
                _AI_time1 = _clock;
                _status_flg |= BACT_STFLAG_APPROACH;
            }
        }
    }
    else
    {
        _status_flg &= ~(BACT_STFLAG_APPROACH | BACT_STFLAG_ATTACK);
    }

    switch( _atk_ret )
    {
        case TA_CANCEL:
        {
            if ( isPrimTarget )
            {
                _status_flg &= ~BACT_STFLAG_FIGHT_P;

                setTarget_msg arg67;
                arg67.priority = 0;
                arg67.tgt_type = BACT_TGT_TYPE_CELL;
                arg67.tgt_pos = _primTpos;

                SetTarget(&arg67);
            }

            if ( isSecTarget )
            {
                _status_flg &= ~BACT_STFLAG_FIGHT_S;

                setTarget_msg arg67;
                arg67.priority = 1;
                arg67.tgt_type = BACT_TGT_TYPE_NONE;

                SetTarget(&arg67);
            }

            _status_flg &= ~BACT_STFLAG_APPROACH;

            if ( _status_flg & BACT_STFLAG_FIRE )
            {
                setState_msg arg78;
                arg78.setFlags = 0;
                arg78.newStatus = BACT_STATUS_NOPE;
                arg78.unsetFlags = BACT_STFLAG_FIRE;

                SetState(&arg78);
            }
        }
        break;
        
        case TA_MOVE:
        {
            if ( _status_flg & BACT_STFLAG_FIRE )
            {
                setState_msg arg78;
                arg78.setFlags = 0;
                arg78.newStatus = BACT_STATUS_NOPE;
                arg78.unsetFlags = BACT_STFLAG_FIRE;

                SetState(&arg78);
            }
        }
        break;
        
        case TA_FIGHT:
        {
            bact_arg101 arg101;
            arg101.pos = arg->target.pbact->_position;
            arg101.unkn = 2;
            arg101.radius = arg->target.pbact->_radius;

            if ( CheckFireAI(&arg101) )
            {
                if ( isSecTarget )
                    _status_flg |= BACT_STFLAG_FIGHT_S;
                else
                    _status_flg |= BACT_STFLAG_FIGHT_P;

                vec3d rotZ = _rotation.AxisZ();

                bact_arg79 arg79;

                arg79.direction.x = rotZ.x;

                if ( _bact_type == BACT_TYPES_TANK )
                    arg79.direction.y = v40.y;
                else
                    arg79.direction.y = rotZ.y - _gun_angle;

                arg79.direction.z = rotZ.z;
                arg79.tgType = BACT_TGT_TYPE_UNIT;
                arg79.target.pbact = arg->target.pbact;
                arg79.tgt_pos = arg->pos;
                arg79.weapon = _weapon;
                arg79.g_time = _clock;

                if ( arg->g_time & 1 )
                    arg79.start_point.x = _fire_pos.x;
                else
                    arg79.start_point.x = -_fire_pos.x;

                arg79.start_point.y = _fire_pos.y;
                arg79.start_point.z = _fire_pos.z;
                arg79.flags = 0;

                LaunchMissile(&arg79);
            }
            else
            {
                _status_flg &= ~BACT_STFLAG_ATTACK;
            }

            if ( v45 < 1000.0 &&   _mgun != -1 &&   v40.dot(_rotation.AxisZ()) > 0.85 )
            {
                if ( isSecTarget )
                    _status_flg |= BACT_STFLAG_FIGHT_S;
                else
                    _status_flg |= BACT_STFLAG_FIGHT_P;

                if ( !(_status_flg & BACT_STFLAG_FIRE) )
                {
                    setState_msg arg78;
                    arg78.unsetFlags = 0;
                    arg78.newStatus = BACT_STATUS_NOPE;
                    arg78.setFlags = BACT_STFLAG_FIRE;

                    SetState(&arg78);
                }

                bact_arg105 arg105;

                arg105.field_C = arg->fperiod;
                arg105.field_10 = _clock;
                arg105.field_0 = _rotation.AxisZ();

                FireMinigun(&arg105);
            }
            else if ( _status_flg & BACT_STFLAG_FIRE )
            {
                setState_msg arg78;
                arg78.setFlags = 0;
                arg78.newStatus = BACT_STATUS_NOPE;
                arg78.unsetFlags = BACT_STFLAG_FIRE;

                SetState(&arg78);
            }
        }
        break;
        
        case TA_IGNORE:
        {
            _status_flg &= ~BACT_STFLAG_APPROACH;

            if ( _status_flg & BACT_STFLAG_FIRE )
            {
                setState_msg arg78;
                arg78.setFlags = 0;
                arg78.newStatus = BACT_STATUS_NOPE;
                arg78.unsetFlags = BACT_STFLAG_FIRE;

                SetState(&arg78);
            }

            if ( _secndT.pbact == arg->target.pbact )
            {
                _status_flg &= ~BACT_STFLAG_FIGHT_S;

                setTarget_msg arg67;
                arg67.tgt_type = BACT_TGT_TYPE_NONE;
                arg67.priority = 1;

                SetTarget(&arg67);

                isSecTarget = 0;
            }
            else
            {
                _status_flg &= ~BACT_STFLAG_FIGHT_P;

                if ( (IsParentMyRobo() && _host_station == a4) && _status != BACT_STATUS_IDLE && !(_status_flg & BACT_STFLAG_ESCAPE) )
                {
                    robo_arg134 arg134;
                    arg134.unit = this;
                    arg134.field_4 = 1;
                    arg134.field_10 = 0;
                    arg134.field_C = 0;
                    arg134.field_8 = 0;
                    arg134.field_14 = 32;

                    _host_station->placeMessage(&arg134);
                }

                setState_msg arg78;
                arg78.unsetFlags = 0;
                arg78.setFlags = 0;
                arg78.newStatus = BACT_STATUS_NORMAL;

                SetState(&arg78);

                _status = BACT_STATUS_IDLE;
            }
        }
        break;
        
        default:
        break;
    }
}

void NC_STACK_ypabact::FightWithSect(bact_arg75 *arg)
{
    int v64 = 0;
    int v68 = 0;

    vec3d *cellPos;

    bact_arg110 arg110;

    if ( _secndT.pcell == arg->target.pcell )
    {
        cellPos = &_sencdTpos;
        v64 = 1;

        arg110.priority = 1;
    }
    else
    {
        cellPos = &_primTpos;
        v68 = 1;

        arg110.priority = 0;
    }

    NC_STACK_ypabact *a4 = _world->getYW_userHostStation();

    int v65 = IsParentMyRobo() && _host_station == a4;

    float v62 = (_position.XZ() - cellPos->XZ()).length();

    if ( _clock - _assess_time > 500 || _clock < 500 )
    {
        _assess_time = _clock;

        arg110.tgType = BACT_TGT_TYPE_CELL;
        _atk_ret = TargetAssess(&arg110);
    }

    if ( _atk_ret == TA_FIGHT )
    {
        float cellDistance = (cellPos->XZ() - _position.XZ()).length();

        if ( _status_flg & BACT_STFLAG_APPROACH )
        {
            _status_flg &= ~BACT_STFLAG_ATTACK;

            if ( (_position.x < 1320 || _position.z > -1320.0 || _position.x > _wrldX - 1320.0 || _position.z < _wrldY + 1320.0) || _adist_sector < cellDistance )
            {
                _status_flg &= ~BACT_STFLAG_APPROACH;
            }
            else
            {
                _AI_time2 = _clock;
                _AI_time1 = _clock;
            }
        }
        else if ( _sdist_sector <= cellDistance )
        {
            if ( _adist_sector <= cellDistance )
                _status_flg &= ~BACT_STFLAG_ATTACK;
            else
                _status_flg |= BACT_STFLAG_ATTACK;
        }
        else
        {
            _status_flg &= ~BACT_STFLAG_ATTACK;

            /*if ( bact->field_3D1 == 2 || (arg->g_time & 1 && bact->field_3D1 == 3) )
            {
                bact->target_vec.x = bact->fly_dir.x;
                bact->target_vec.z = bact->fly_dir.z;
                bact->target_vec.y = bact->fly_dir.y;
            }
            else*/
            {
                _target_vec = -_fly_dir;
            }

            _AI_time2 = _clock;
            _AI_time1 = _clock;

            _status_flg |= BACT_STFLAG_APPROACH;
        }
    }
    else
    {
        _status_flg &= ~(BACT_STFLAG_APPROACH | BACT_STFLAG_ATTACK);
    }

    if ( _status_flg & BACT_STFLAG_FIRE )
    {
        setState_msg arg78;
        arg78.unsetFlags = BACT_STFLAG_FIRE;
        arg78.setFlags = 0;
        arg78.newStatus = BACT_STATUS_NOPE;

        SetState(&arg78);
    }
    
    switch(_atk_ret)
    {
        case TA_CANCEL:
        {
            _status_flg &= ~BACT_STFLAG_APPROACH;

            if ( v68 )
            {
                if ( v65 )
                {
                    robo_arg134 arg134;

                    arg134.unit = this;
                    arg134.field_4 = 4;
                    arg134.field_8 = _primTpos.x / 1200.0;
                    arg134.field_C = -_primTpos.z / 1200.0;
                    arg134.field_14 = 18;
                    arg134.field_10 = 0;

                    _host_station->placeMessage(&arg134);
                }

                _status_flg &= ~BACT_STFLAG_FIGHT_P;

                setTarget_msg arg67;
                arg67.tgt_type = BACT_TGT_TYPE_CELL;
                arg67.tgt_pos.x = _position.x;
                arg67.tgt_pos.z = _position.z;
                arg67.priority = 0;

                SetTarget(&arg67);
            }

            if ( v64 )
            {
                if ( v65 )
                {
                    robo_arg134 arg134;

                    arg134.unit = this;
                    arg134.field_4 = 4;
                    arg134.field_8 = _sencdTpos.x / 1200.0;
                    arg134.field_10 = 0;
                    arg134.field_14 = 18;
                    arg134.field_C = -_sencdTpos.z / 1200.0;

                    _host_station->placeMessage(&arg134);
                }

                _status_flg &= ~BACT_STFLAG_FIGHT_S;

                setTarget_msg arg67;
                arg67.tgt_type = BACT_TGT_TYPE_NONE;
                arg67.priority = 1;
                SetTarget(&arg67);
            }
        }
        break;
        
        case TA_MOVE:
        {
        }
        break;
        
        case TA_FIGHT:
        {
            if ( v68 )
            {
                if ( v62 < 1200.0 )
                {
                    if ( !(_status_flg & BACT_STFLAG_FIGHT_P) && v65 && _secndT.pcell != _primT.pcell )
                    {
                        robo_arg134 arg134;

                        arg134.field_4 = 3;
                        arg134.field_8 = _primTpos.x / 1200.0;
                        arg134.field_C = -_primTpos.z / 1200.0;
                        arg134.unit = this;
                        arg134.field_10 = 0;
                        arg134.field_14 = 20;

                        _host_station->placeMessage(&arg134);
                    }

                    _status_flg |= BACT_STFLAG_FIGHT_P;
                }

                GetBestSectorPart(&_primTpos);

                arg->pos = _primTpos;
            }

            if ( v64 )
            {
                if ( v62 < 1200.0 )
                {
                    if ( v65 && !(_status_flg & BACT_STFLAG_FIGHT_S) )
                    {
                        robo_arg134 arg134;

                        arg134.field_4 = 3;
                        arg134.field_8 = _sencdTpos.x / 1200.0;
                        arg134.field_C = -_sencdTpos.z / 1200.0;
                        arg134.unit = this;
                        arg134.field_10 = 0;
                        arg134.field_14 = 20;

                        _host_station->placeMessage(&arg134);
                    }

                    _status_flg |= BACT_STFLAG_FIGHT_S;
                }

                GetBestSectorPart(&_sencdTpos);

                arg->pos = _sencdTpos;
            }

            bact_arg101 arg101;
            arg101.unkn = 1;
            arg101.pos = arg->pos;

            if ( CheckFireAI(&arg101) )
            {
                vec3d tmp = _position + _fire_pos - arg->pos;

                float v60 = tmp.length();

                if ( v60 < 0.01 )
                    v60 = 0.01;

                if ( v64 )
                    _status_flg |= BACT_STFLAG_FIGHT_S;
                else
                    _status_flg |= BACT_STFLAG_FIGHT_P;

                bact_arg79 arg79;

                arg79.direction = -(_position + _fire_pos - arg->pos) / v60;
                arg79.tgType = BACT_TGT_TYPE_CELL;
                arg79.target.pbact = arg->target.pbact;
                arg79.tgt_pos = arg->pos;
                arg79.weapon = _weapon;
                arg79.g_time = _clock;

                if ( arg->g_time & 1 )
                    arg79.start_point.x = _fire_pos.x;
                else
                    arg79.start_point.x = -_fire_pos.x;

                arg79.start_point.y = _fire_pos.y;
                arg79.start_point.z = _fire_pos.z;
                arg79.flags = 0;

                LaunchMissile(&arg79);
            }
            else
            {
                _status_flg &= ~BACT_STFLAG_ATTACK;
            }
        }
        break;
        
        case TA_IGNORE:
        {
            _status_flg &= ~BACT_STFLAG_APPROACH;

            if ( v64 )
            {
                _status_flg &= ~BACT_STFLAG_FIGHT_S;

                if ( v65 && _secndT.pcell != _primT.pcell )
                {
                    robo_arg134 arg134;

                    arg134.field_4 = 2;
                    arg134.field_8 = _sencdTpos.x / 1200.0;
                    arg134.field_C = -_sencdTpos.z / 1200.0;
                    arg134.field_10 = 0;
                    arg134.field_14 = 22;
                    arg134.unit = this;

                    _host_station->placeMessage(&arg134);
                }

                setTarget_msg arg67;
                arg67.priority = 1;
                arg67.tgt_type = BACT_TGT_TYPE_NONE;

                SetTarget(&arg67);

                v64 = 0;
            }
            else
            {
                _status_flg &= ~BACT_STFLAG_FIGHT_P;

                if ( v65 && _status != BACT_STATUS_IDLE )
                {
                    robo_arg134 arg134;

                    arg134.field_10 = 0;
                    arg134.field_C = 0;
                    arg134.field_8 = 0;
                    arg134.unit = this;
                    arg134.field_4 = 1;
                    arg134.field_14 = 32;

                    _host_station->placeMessage(&arg134);
                }

                _status = BACT_STATUS_IDLE;
            }
        }
        break;
        
        default:
            break;
    }
}

void ypabact_func77__sub0(NC_STACK_ypabact *bact1, NC_STACK_ypabact *bact2)
{
    if ( bact1->_status_flg & BACT_STFLAG_WAYPOINT )
    {
        for (int i = 0; i < 32; i++)
            bact2->_waypoints[i] = bact1->_waypoints[i];

        bact2->_status_flg |= BACT_STFLAG_WAYPOINT;

        if ( bact1->_status_flg & BACT_STFLAG_WAYPOINTCCL )
            bact2->_status_flg |= BACT_STFLAG_WAYPOINTCCL;
        else
            bact2->_status_flg &= ~BACT_STFLAG_WAYPOINTCCL;

        bact2->_waypoints_count = bact1->_waypoints_count;
        bact2->_current_waypoint = bact1->_current_waypoint;
    }
}

void NC_STACK_ypabact::Die()
{
    if ( _status_flg & BACT_STFLAG_DEATH1 )
        return;
    
    int maxy = _world->getYW_mapMaxY();
    int maxx = _world->getYW_mapMaxX();

    uamessage_dead deadMsg;
    deadMsg.msgID = UAMSG_DEAD;
    deadMsg.owner = _owner;
    deadMsg.id = _gid;
    deadMsg.newParent = 0;
    deadMsg.landed = 0;
    deadMsg.classID = _bact_type;

    if ( _killer )
        deadMsg.killer = _killer->_gid;
    else
        deadMsg.killer = 0;

    deadMsg.killerOwner = _killer_owner;

    NC_STACK_ypabact *deputy = NULL;

    if (!_kidList.empty())
    {

        for (World::RefBactList::iterator it = _kidList.begin(); it != _kidList.end(); )
        {
            // Forward dereference iterator and next
            NC_STACK_ypabact *kid = *it;
            it++;

            if ( kid->_status == BACT_STATUS_DEAD )
            {
                if ( _parent )
                    _parent->AddSubject(kid);
                else
                    _world->ypaworld_func134(kid);                  

                kid->_status_flg |= BACT_STFLAG_NOMSG;
            }
            else
            {
                float kidLen = (kid->_position.XZ() - _position.XZ()).square();

                float deputyLen;

                if ( deputy )
                    deputyLen = (deputy->_position.XZ() - _position.XZ()).square();
                else
                    deputyLen = (POW2(maxx) + POW2(maxy)) * 1200.0 * 1200.0;

                if ( kid->_bact_type == BACT_TYPES_UFO )
                    kidLen = (POW2(maxx) + POW2(maxy)) * 1200.0 * 1200.0 - 1000.0;

                if ( kidLen <= deputyLen )
                    deputy = kid;
            }
        }

        if ( deputy )
        {
            if ( _parent )
                _parent->AddSubject(deputy);
            else
                _world->ypaworld_func134(deputy);

            while ( !_kidList.empty() )
                deputy->AddSubject(_kidList.front());

            setTarget_msg arg67;
            arg67.tgt_pos = _primTpos;
            arg67.tgt.pbact = _primT.pbact;
            arg67.tgt_type = _primTtype;
            arg67.priority = 0;

            deputy->SetTarget(&arg67);

            ypabact_func77__sub0(this, deputy);

            deputy->_commandID = _commandID;
            deputy->_aggr = _aggr;

            if ( _world->isNetGame )
            {
                if (_owner)
                    deadMsg.newParent = deputy->_gid;
            }
        }
        else
        {
            for(World::RefBactList::iterator kidXit = _kidList.begin(); kidXit != _kidList.end(); )
            {
                NC_STACK_ypabact *kidX = *kidXit;
                kidXit++;
                
                for ( World::RefBactList::iterator kidYit = kidX->_kidList.begin(); kidYit != kidX->_kidList.end(); )
                {
                    NC_STACK_ypabact *kidY = *kidYit;
                    kidYit++;

                    _world->ypaworld_func134(kidY);

                    if ( kidY->_status != BACT_STATUS_DEAD )
                        ypa_log_out("Scheisse, da hфngt noch ein Lebendiger unter der Leiche! owner %d, state %d, class %d\n", kidY->_owner, kidY->_status, _bact_type);
                }
                _world->ypaworld_func134(kidX);
            }
        }
    }

    NC_STACK_ypabact *v76 = _world->getYW_userHostStation();

    if ( !deputy && IsParentMyRobo()&& !(_status_flg & BACT_STFLAG_NOMSG) )
    {
        robo_arg134 v53;

        if ( v76 == _host_station )
        {
            if ( _bact_type == BACT_TYPES_GUN )
            {
                if ( _weapon != -1 || -1 != _mgun )
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
                v53.unit = this;

                _host_station->placeMessage(&v53);
            }
            else
            {
                if ( !(_status_flg & BACT_STFLAG_CLEAN) )
                {
                    v53.field_8 = _commandID;
                    v53.unit = this;
                    v53.field_10 = 0;
                    v53.field_14 = 44;
                    v53.field_C = 0;
                    v53.field_4 = 8;

                    _host_station->placeMessage(&v53);
                }
            }
        }
        else
        {
            if ( _killer && v76 == _killer->_host_station )
            {
                v53.field_4 = 5;
                v53.unit = _killer;
                v53.field_8 = _primT_cmdID;
                v53.field_10 = 0;
                v53.field_C = 0;
                v53.field_14 = 36;

                _host_station->placeMessage(&v53);
            }
        }

    }

    CleanAttackersTarget();

    if ( _parent )
    {
        for (World::MissileList::iterator it = _missiles_list.begin(); it != _missiles_list.end(); it = _missiles_list.erase(it))
        {
            NC_STACK_ypamissile *miss = *it;

            _parent->_missiles_list.push_back(miss);
            miss->setMISS_launcher( _parent );
        }
    }
    else
    {
        for (World::MissileList::iterator it = _missiles_list.begin(); it != _missiles_list.end(); it = _missiles_list.erase(it))
        {
            NC_STACK_ypamissile *miss = *it;

            miss->ResetViewing();

            setState_msg arg119;
            arg119.newStatus = BACT_STATUS_DEAD;
            arg119.unsetFlags = 0;
            arg119.setFlags = 0;
            miss->SetStateInternal(&arg119);

            setTarget_msg arg67;
            arg67.tgt_type = BACT_TGT_TYPE_NONE;
            arg67.priority = 0;
            miss->SetTarget(&arg67);

            miss->_parent = NULL;

            _world->ypaworld_func144(miss);
        }
    }
    

    if ( _secndTtype == BACT_TGT_TYPE_UNIT )
        _secndT.pbact->_attackersList.remove(this);

    if ( _primTtype == BACT_TGT_TYPE_UNIT )
        _primT.pbact->_attackersList.remove(this);


    _secndTtype = BACT_TGT_TYPE_NONE;
    _primTtype = BACT_TGT_TYPE_NONE;

    _status = BACT_STATUS_DEAD;
    _commandID = 0;
    _status_flg |= BACT_STFLAG_DEATH1;
    _dead_time = _clock;

    if ( _status_flg & BACT_STFLAG_LAND )
    {
        if ( _vp_active == 1 || _vp_active == 6 )
        {
            setState_msg arg119;
            arg119.unsetFlags = 0;
            arg119.newStatus = BACT_STATUS_NOPE;
            arg119.setFlags = BACT_STFLAG_DEATH2;

            SetStateInternal(&arg119);

            if ( _world->isNetGame )
            {
                if (_owner)
                    deadMsg.landed = 1;
            }
        }
    }

    if ( _oflags & BACT_OFLAG_USERINPT )
    {
        if ( !(_oflags & BACT_OFLAG_VIEWER) )
        {
            if ( _parent )
                setBACT_inputting(0);
        }
    }

    if ( _world->isNetGame )
    {
        if ( _owner )
        {
            if ( _bact_type != BACT_TYPES_ROBO )
            {
                yw_arg181 arg181;
                arg181.data = &deadMsg;
                arg181.recvFlags = 2;
                arg181.dataSize = sizeof(deadMsg);
                arg181.recvID = 0;
                arg181.garant = 1;

                _world->ypaworld_func181(&arg181);
            }
        }
    }

    if ( _owner )
    {
        if ( !(_status_flg & BACT_STFLAG_CLEAN) )
            _world->HistoryAktKill(this);
    }
}

void NC_STACK_ypabact::SetState(setState_msg *arg)
{
    if ( (_bact_type == BACT_TYPES_TANK || _bact_type == BACT_TYPES_CAR) && arg->newStatus == 2 )
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

        if ( _world->isNetGame )
        {
            if ( v6 && _owner && _bact_type != BACT_TYPES_MISSLE )
            {
                uamessage_setState ssMsg;
                ssMsg.msgID = UAMSG_SETSTATE;
                ssMsg.owner = _owner;
                ssMsg.id = _gid;
                ssMsg.newStatus = arg->newStatus;
                ssMsg.setFlags = arg->setFlags;
                ssMsg.unsetFlags = arg->unsetFlags;
                ssMsg.classID = _bact_type;

                yw_arg181 v9;
                v9.recvFlags = 2;
                v9.dataSize = sizeof(ssMsg);
                v9.garant = 1;
                v9.recvID = 0;
                v9.data = &ssMsg;

                _world->ypaworld_func181(&v9);
            }
        }
    }
}

size_t NC_STACK_ypabact::LaunchMissile(bact_arg79 *arg)
{
    NC_STACK_ypamissile *wobj = NULL;

    WeapProto *wprotos = _world->getYW_weaponProtos();

    if ( arg->weapon == -1 )
        return 0;

    if ( _weapon_time )
    {
        int v4;

        if ( _oflags & BACT_OFLAG_USERINPT )
            v4 = wprotos[arg->weapon].shot_time_user;
        else
            v4 = wprotos[arg->weapon].shot_time;

        if ( wprotos[arg->weapon].salve_shots )
        {
            if ( wprotos[arg->weapon].salve_shots <= _salve_counter )
                v4 = wprotos[arg->weapon].salve_delay;
        }

        if ( arg->g_time - _weapon_time < v4 )
            return 0;
    }

    if ( _salve_counter < wprotos[arg->weapon].salve_shots )
        _salve_counter += 1;
    else
        _salve_counter = 1;

    if ( _oflags & BACT_OFLAG_USERINPT )
    {
        yw_arg180 v26;

        if ( wprotos[arg->weapon].model_id & 2 )
            v26.effects_type = 0;
        else if ( wprotos[arg->weapon].model_id & 0x10 )
            v26.effects_type = 1;
        else
            v26.effects_type = 2;

        _world->ypaworld_func180(&v26);
    }

    int v13;

    if ( _num_weapons <= 1 )
        v13 = 1;
    else
        v13 = _num_weapons;

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
        arg147.pos = _position + _rotation.Transpose().Transform( vec3d(v37, arg->start_point.y, arg->start_point.z) );

        wobj = _world->ypaworld_func147(&arg147);

        if ( !wobj )
            return 0;

        wobj->setMISS_launcher(this);

        wobj->setMISS_startHeight(arg147.pos.y);

        wobj->_owner = _owner;

        if ( _bact_type != BACT_TYPES_GUN )
            _energy -= wobj->_energy / 300;

        if ( arg->direction.x != 0.0 || arg->direction.y != 0.0 || arg->direction.z != 0.0 )
        {
            wobj->_fly_dir = arg->direction;
        }
        else
        {
            wobj->_fly_dir = _rotation.AxisZ();
        }

        wobj->_fly_dir_length = _fly_dir_length + wprotos[ arg->weapon ].start_speed;

        if ( !(wprotos[arg->weapon].model_id & 0x12) )
            wobj->_fly_dir_length *= 0.2;

        wobj->_rotation.SetZ( wobj->_fly_dir );

        wobj->_rotation.SetX( _rotation.AxisX() );

        wobj->_rotation.SetY( wobj->_rotation.AxisZ() * wobj->_rotation.AxisX() );

        if ( i == 0 )
        {
            if ( arg->flags & 1 )
                wobj->_position = wobj->_position - wobj->_rotation.AxisZ() * 30.0;
        }

        /** Missiles will be stored in another list
         *  so kidref will be not attached to anything.
         *  Looks it's somehow related to mentioned problem with dead cache.
        **/
        
        wobj->_kidRef.Detach();
        wobj->_parent = NULL;

        _missiles_list.push_back(wobj);

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
                    wobj->_primTpos.y = arg->tgt_pos.y;
            }
        }

        uamessage_newWeapon wpnMsg;
        wpnMsg.targetPos = arg->tgt_pos;

        if ( v42 == 2 )
        {
            wobj->_primTtype = BACT_TGT_TYPE_DRCT;
            wobj->_target_dir = wobj->_fly_dir;
        }

        wobj->_host_station = _host_station;
        _weapon_time = arg->g_time;

        SFXEngine::SFXe.startSound(&wobj->_soundcarrier, 1);

        if ( _world->isNetGame )
        {
            wobj->_gid |= _owner << 24;

            wpnMsg.msgID = UAMSG_NEWWEAPON;
            wpnMsg.owner = _owner;
            wpnMsg.id = wobj->_gid;
            wpnMsg.launcher = _gid;
            wpnMsg.type = arg->weapon;
            wpnMsg.pos = arg147.pos;
            wpnMsg.flags = 0;
            wpnMsg.dir = wobj->_fly_dir * wobj->_fly_dir_length;
            wpnMsg.targetType = wobj->_primTtype;

            if ( wobj->_primTtype == BACT_TGT_TYPE_UNIT )
            {
                wpnMsg.target = wobj->_primT.pbact->_gid;
                wpnMsg.targetOwner = wobj->_primT.pbact->_owner;
            }

            yw_arg181 arg181;
            arg181.data = &wpnMsg;
            arg181.dataSize = sizeof(wpnMsg);
            arg181.recvFlags = 2;
            arg181.recvID = 0;
            arg181.garant = 1;

            _world->ypaworld_func181(&arg181);
        }

        if ( arg->flags & 1 )
        {
            if ( i == 0 )
            {
                if ( _oflags & BACT_OFLAG_VIEWER )
                {
                    setBACT_viewer(0);
                    wobj->setBACT_viewer(1);
                }
            }
        }
        
        if ( arg->flags & 4 )
            wobj->setMISS_ignoreBuilds(1);
            

        if ( arg->tgType != BACT_TGT_TYPE_UNIT )
        {
            int life_time_nt = wprotos[ arg->weapon ].life_time_nt;

            if ( life_time_nt )
                wobj->setMISS_lifeTime(life_time_nt);
        }
    }

    if ( _kill_after_shot )
    {
        if ( _oflags & BACT_OFLAG_USERINPT )
        {
            setBACT_viewer(0);
            wobj->setBACT_viewer(1);
        }

        bact_arg84 arg84;
        arg84.unit = _parent;
        arg84.energy = -2 * _energy_max;

        ModifyEnergy(&arg84);
    }
    return 1;
}

size_t NC_STACK_ypabact::SetPosition(bact_arg80 *arg)
{
    yw_130arg sect_info;

    sect_info.pos_x = arg->pos.x;
    sect_info.pos_z = arg->pos.z;
    if (!_world->ypaworld_func130(&sect_info))
        return 0;

    if ( _pSector )
        _cellRef.Detach();

    _cellRef = sect_info.pcell->unitsList.push_back(this);

    _pSector = sect_info.pcell;
    _old_pos = arg->pos;
    _position = arg->pos;
    _sectX = sect_info.sec_x;
    _sectY = sect_info.sec_z;

    if ( !(arg->field_C & 2) )
        CorrectPositionInLevelBox(NULL);

    return 1;
}

void NC_STACK_ypabact::GetSummary(bact_arg81 *arg)
{
    for ( NC_STACK_ypabact* &node : _kidList )
        node->GetSummary(arg);

    if ( _status != BACT_STATUS_DEAD )
    {
        switch ( arg->enrg_type )
        {
        case 1:
            arg->enrg_sum += _energy;
            break;

        case 3:
            arg->enrg_sum++;
            break;

        case 2:
            arg->enrg_sum += _shield;
            break;

        case 4:
            arg->enrg_sum += _energy_max;
            break;

        case 5:
        {
            arg->enrg_sum += _attackersList.size();
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
    cellArea *cell = _pSector;

    if ( _status != BACT_STATUS_DEAD )
    {
        int v16 = _clock - _energy_time;

        if ( v16 >= 1500 )
        {
            _energy_time = _clock;

            yw_arg176 arg176;
            arg176.owner = cell->owner;

            _world->ypaworld_func176(&arg176);

            float v14 = v16 / 1000.0;

            float denerg = _energy_max * v14 * cell->energy_power * arg176.field_4 / 7000.0;

            if ( _owner == cell->owner )
                _energy += denerg;
            else
                _energy -= denerg;

            if ( _energy < 0 )
                _energy = 0;

            if ( _energy > _energy_max )
                _energy = _energy_max;
        }
    }
}

void NC_STACK_ypabact::ApplyImpulse(bact_arg83 *arg)
{
    float v81 = 50.0 / _mass;
    float v79 = arg->energ * 0.0004;

    vec3d v60 = _position - arg->pos;

    float distance = v60.length();

    if ( distance <= _radius )
    {
        vec3d v63 = (arg->pos2 * (2.5 * arg->mass * arg->force) + _fly_dir * _mass * _fly_dir_length) / (_mass + arg->mass);

        _fly_dir_length = v63.normalise();

        if ( _fly_dir_length > 0.0 )
            _fly_dir = v63;

        v60 = arg->pos2;

        distance = 1.0;
    }
    else
    {
        v60 /= distance;

        vec3d v63 = _fly_dir * _fly_dir_length + (v60 * v81 * v79) / distance;

        _fly_dir_length = v63.normalise();

        if ( _fly_dir_length > 0.0 )
            _fly_dir = v63;
    }

    CorrectPositionInLevelBox(NULL);

    _status_flg &= ~BACT_STFLAG_LAND;

    float angle = v81 * 0.01 * v79 / distance;

    float cos_len = v60.dot(_rotation.AxisZ());

    // cos(45) == 0.7071
    if ( fabs(cos_len) > 0.7071 )
    {
        if ( cos_len > 0.7071 )
            _rotation = mat3x3::RotateX(-angle) * _rotation;
        else
            _rotation = mat3x3::RotateX(angle) * _rotation;
    }
    else
    {
        if ( v60.XZ().cross( _rotation.AxisZ().XZ() ) >= 0.0 )
            _rotation = mat3x3::RotateZ(angle) * _rotation;
        else
            _rotation = mat3x3::RotateZ(-angle) * _rotation;
    }
}

void NC_STACK_ypabact::ModifyEnergy(bact_arg84 *arg)
{
    int v6 = 0;

    int a4 = _world->getYW_invulnerable();

    if ( !_world || !(_oflags & BACT_OFLAG_VIEWER) || !a4 || arg->energy <= -1000000 )
    {
        if ( _world->isNetGame )
            v6 = 1;

        if ( !v6 || !arg->unit || _owner == arg->unit->_owner )
        {
            _energy += arg->energy;

            if ( arg->unit )
                _killer_owner = arg->unit->_owner;
            else
                _killer_owner = 0;

            if ( _energy <= 0 )
            {
                _killer = arg->unit;
                _status_flg &= ~BACT_STFLAG_LAND;

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
            vhclEnrgy.owner = _owner;
            vhclEnrgy.id = _gid;
            vhclEnrgy.energy = arg->energy;

            if ( arg->unit )
            {
                vhclEnrgy.killer = arg->unit->_gid;
                vhclEnrgy.killerOwner = arg->unit->_owner;
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

            _world->ypaworld_func181(&arg181);
        }
    }
}

bool NC_STACK_ypabact::ypabact_func85(vec3d *arg)
{
    float tmp2 = arg->dot( _fly_dir * _fly_dir_length );

    if ( fabs(tmp2) > 15.0 )
        return true;

    return false;
}


void CrashOrLand__sub1(NC_STACK_ypabact *bact)
{
    if ( bact->_fly_dir.x < 0.0 )
        bact->_fly_dir.x -= 7.0;

    if ( bact->_fly_dir.z < 0.0 )
        bact->_fly_dir.z -= 7.0;

    if ( bact->_fly_dir.x >= 0.0 )
        bact->_fly_dir.x += 7.0;

    if ( bact->_fly_dir.z >= 0.0 )
        bact->_fly_dir.z += 7.0;

    if ( bact->_fly_dir_length < 15.0 )
        bact->_fly_dir_length = 15.0;

    float v4 = bact->_fly_dir.length();

    if ( v4 <= 0.001 )
        bact->_fly_dir = vec3d(0.0, 1.0, 0.0);
    else
        bact->_fly_dir /= v4;
}

void sub_48AB14(NC_STACK_ypabact *bact, const vec3d &vec)
{
    vec3d vaxis = bact->_rotation.AxisY() * vec;

    if ( vaxis.normalise() != 0.0 )
    {
        float angle = clp_acos( vec.dot(bact->_rotation.AxisY()) );

        if ( angle > 0.001 )
            bact->_rotation *= mat3x3::AxisAngle(vaxis, angle);
    }
}

void CrashOrLand__sub0(NC_STACK_ypabact *bact, int a2)
{
    bact->_status_flg |= BACT_STFLAG_SCALE;

    if ( bact->_scale_duration > bact->_scale_pos )
    {
        float v5 = bact->_maxrot * a2 / 1000.0;


        bact->_scale_speed += bact->_scale_accel * a2 / 1000.0;
        bact->_scale_start += bact->_scale_speed * (a2 / 1000.0);

        bact->_scale = vec3d(bact->_scale_start);

        bact->_rotation = mat3x3::RotateY(v5) * bact->_rotation;

        int v14 = 0;
        for (int i = 0; i < 32; i++)
        {
            if ( bact->_vp_fx_models[i] )
                v14++;
        }

        if ( v14 )
        {
            int v15 = bact->_scale_pos * v14 / bact->_scale_duration;

            bact->_current_vp.base = bact->_vp_fx_models[v15];
            bact->_current_vp.trigo = bact->_vp_fx_tform[v15];
        }

        bact->_scale_pos += a2;
    }
    else
    {
        bact->_yls_time = -1;
        bact->Release();
    }
}

size_t NC_STACK_ypabact::CrashOrLand(bact_arg86 *arg)
{
    yw_137col v58[10];

    int v85 = 0;

    if ( _status_flg & BACT_STFLAG_SEFFECT )
    {
        CrashOrLand__sub0(this, arg->field_two);
    }
    else
    {
        float v84;
        float v90;

        if ( _oflags & BACT_OFLAG_VIEWER )
        {
            v84 = _viewer_radius;
            v90 = _viewer_overeof;
        }
        else
        {
            v84 = _radius;
            v90 = _overeof;
        }

        if ( _bact_type == BACT_TYPES_ROBO )
            v90 = 60.0;

        vec3d vaxis = vec3d( -_rotation.m12, 0.0, _rotation.m10 );

        float v94 = arg->field_two / 1000.0;

        if ( vaxis.normalise() > 0.001 && !(arg->field_one & 1) )
        {
            float angle = clp_acos( _rotation.m11 );
            float maxrot = _maxrot * v94;

            if ( angle > maxrot )
                angle = maxrot;

            if ( fabs(angle) > BACT_MIN_ANGLE )
            {
                _rotation *= mat3x3::AxisAngle(vaxis, angle);
            }
        }

        if ( arg->field_one & 2 )
        {
            float v18 = fabs(_fly_dir_length) * v94 * 0.08;

            _rotation = mat3x3::RotateZ(v18) * _rotation;
        }

        if ( !(_status_flg & BACT_STFLAG_LAND) )
        {
            if ( arg->field_one & 1 )
                _airconst = 0;
            else
                _airconst = 500.0;

            for (int i = 0; i < 3; i++)
            {

                move_msg v66;

                v66.field_0 = v94;
                v66.flag = 1;

                Move(&v66);

                int v20 = 0;

                if ( _oflags & BACT_OFLAG_BACTCOLL )
                {
                    if ( CollisionWithBact(arg->field_two) )
                    {
                        if ( _bact_type == BACT_TYPES_TANK || _bact_type == BACT_TYPES_CAR )
                        {
                            CrashOrLand__sub1(this);
                            return 0;
                        }

                        return 0;
                    }
                }

                if ( _oflags & BACT_OFLAG_VIEWER )
                {
                    ypaworld_arg137 arg137;
                    arg137.pos = _fly_dir * _fly_dir_length * v94 * 6.0 + _position;
                    arg137.pos2 = _fly_dir;
                    arg137.radius = v84;
                    arg137.collisions = v58;
                    arg137.field_30 = 0;
                    arg137.coll_max = 10;

                    _world->ypaworld_func137(&arg137);

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
                            a2a = _fly_dir;
                            arg88.pos1 = _fly_dir;
                        }

                        if ( arg->field_one & 1 )
                        {
                            _energy -= fabs(_fly_dir_length) * 10.0;

                            if ( _energy <= 0 || (_current_vp.base == _vp_dead.base && _status == BACT_STATUS_DEAD) )
                            {
                                setState_msg arg78;
                                arg78.setFlags = BACT_STFLAG_DEATH2;
                                arg78.unsetFlags = 0;
                                arg78.newStatus = BACT_STATUS_NOPE;

                                SetState(&arg78);
                            }

                            if ( _oflags & BACT_OFLAG_USERINPT )
                            {
                                if ( fabs(_fly_dir_length) > 7.0 )
                                    SFXEngine::SFXe.startSound(&_soundcarrier, 5);

                                yw_arg180 arg180_1;

                                arg180_1.effects_type = 5;
                                arg180_1.field_4 = 1.0;
                                arg180_1.field_8 = v98.x * 10.0 + _position.x;
                                arg180_1.field_C = v98.z * 10.0 + _position.z;

                                _world->ypaworld_func180(&arg180_1);
                            }

                            if ( v98.y >= 0.6 && v24 )
                            {
                                _position.y = _old_pos.y;

                                _status_flg |= BACT_STFLAG_LAND;

                                _fly_dir_length *= _fly_dir.XZ().length();

                                sub_48AB14(this, a2a);

                                _reb_count = 0;
                            }
                            else
                            {
                                Recoil(&arg88);

                                _reb_count++;

                                v20 = 1;

                                if ( _reb_count > 50 )
                                {
                                    _energy = -10000;
                                    _status_flg |= BACT_STFLAG_LAND;
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
                            _position.y = _old_pos.y;
                            _fly_dir_length = 0;
                            _reb_count = 0;
                            _status_flg |= BACT_STFLAG_LAND;
                        }
                    }
                }

                if ( !v85 )
                {
                    ypaworld_arg136 arg136;
                    arg136.stPos = _old_pos;
                    arg136.vect = _position - _old_pos + vec3d(0.0, v90, 0.0);
                    arg136.flags = 0;

                    _world->ypaworld_func136(&arg136);

                    if ( arg136.isect )
                    {
                        bact_arg88 arg88;

                        arg88.pos1 = arg136.skel->polygons[arg136.polyID].Normal();

                        vec3d a2a = arg88.pos1;

                        if ( arg->field_one & 1 )
                        {
                            _energy -= fabs(_fly_dir_length) * 10.0;

                            if ( _energy <= 0 || (_current_vp.base == _vp_dead.base && _status == BACT_STATUS_DEAD) )
                            {
                                setState_msg arg78;
                                arg78.setFlags = BACT_STFLAG_DEATH2;
                                arg78.unsetFlags = 0;
                                arg78.newStatus = BACT_STATUS_NOPE;

                                SetState(&arg78);
                            }

                            if ( _oflags & BACT_OFLAG_USERINPT )
                            {
                                if ( fabs(_fly_dir_length) > 7.0 )
                                    SFXEngine::SFXe.startSound(&_soundcarrier, 5);

                                yw_arg180 arg180;

                                arg180.effects_type = 5;
                                arg180.field_4 = 1.0;
                                arg180.field_8 = a2a.x * 10.0 + _position.x;
                                arg180.field_C = a2a.z * 10.0 + _position.z;

                                _world->ypaworld_func180(&arg180);
                            }

                            if ( arg136.skel->polygons[arg136.polyID].B < 0.6 )
                            {
                                Recoil(&arg88);

                                _reb_count++;

                                v20 = 1;

                                if ( _reb_count > 50 )
                                {
                                    _energy = -10000;
                                    _status_flg |= BACT_STFLAG_LAND;
                                }
                            }
                            else
                            {
                                _position = arg136.isectPos - vec3d(0.0, v90, 0.0);

                                _status_flg |= BACT_STFLAG_LAND;

                                _fly_dir_length *= _fly_dir.XZ().length();

                                sub_48AB14(this, a2a);

                                _reb_count = 0;
                            }
                        }
                        else if ( arg136.skel->polygons[arg136.polyID].B < 0.6 )
                        {
                            Recoil(&arg88);

                            v20 = 1;
                        }
                        else
                        {
                            _position.y = arg136.isectPos.y - v90;

                            _fly_dir_length = 0;
                            _reb_count = 0;
                            _status_flg |= BACT_STFLAG_LAND;
                        }
                    }
                }

                if ( !v20 ) // Alternative exit from loop
                    break;
            }

        }
        if ( _status_flg & BACT_STFLAG_LAND )
            return 1;
    }
    return 0;
}


void CollisionWithBact__sub0(NC_STACK_ypabact *bact, NC_STACK_ypabact *a2)
{
    int v2 = (int)((float)a2->_energy_max * 0.7);

    if ( v2 < 10000 )
        v2 = 10000;

    if ( v2 > 25000 )
        v2 = 25000;

    int v3 = (float)a2->_scale_time * 0.2 / (float)v2 * (float)a2->_energy_max;

    if ( bact->_energy + v3 > bact->_energy_max )
    {
        NC_STACK_yparobo *robo = bact->_host_station;

        int v10 = v3 - (bact->_energy_max - bact->_energy);

        bact->_energy = bact->_energy_max;

        if ( robo->_energy + v10 > robo->_energy_max )
        {
            int v14 = v10 - (robo->_energy_max - robo->_energy);

            robo->_energy = robo->_energy_max;

            if ( robo->_roboEnergyLife + v14 >= robo->_energy_max )
            {
                robo->_roboEnergyMove += v14 - (robo->_energy_max - robo->_roboEnergyLife);

                robo->_roboEnergyLife = robo->_energy_max;

                if ( robo->_roboEnergyMove > robo->_energy_max )
                    robo->_roboEnergyMove = robo->_energy_max;
            }
            else
            {
                robo->_roboEnergyLife += v14;
            }
        }
        else
        {
            robo->_energy += v10;
        }
    }
    else
    {
        bact->_energy += v3;
    }
}

size_t NC_STACK_ypabact::CollisionWithBact(int arg)
{
    int a4 = getBACT_viewer();

    float trad;
    if ( a4 )
        trad = _viewer_radius;
    else
        trad = _radius;


    int v49 = 0;

    rbcolls *v46 = getBACT_collNodes();

    if ( _fly_dir_length == 0.0 )
        return 0;

    cellArea *cell = _pSector;

    vec3d stru_5150E8(0.0, 0.0, 0.0);

    int v45 = 0;

    rbcolls *v55;

    for ( NC_STACK_ypabact* &bnode : cell->unitsList )
    {
        int v53 = bnode->_status == BACT_STATUS_DEAD && (bnode->_vp_extra[0].flags & 1) && (_oflags & BACT_OFLAG_USERINPT) && bnode->_scale_time > 0 ;

        if ( bnode != this && bnode->_bact_type != BACT_TYPES_MISSLE && (!bnode->IsDestroyed() || v53) )
        {

            v55 = bnode->getBACT_collNodes();

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
                    v41 = bnode->_position;
                }
                else
                {
                    roboColl *v10 = &v55->roboColls[i];
                    ttrad = v10->robo_coll_radius;

                    v41 = bnode->_position + bnode->_rotation.Transpose().Transform(v10->coll_pos);

                    if ( ttrad < 0.01 )
                        continue;
                }

                if ( (_position - v41).length() <= trad + ttrad )
                {
                    if ( !v53 )
                    {
                        stru_5150E8 += v41;

                        v45++;
                    }
                    else
                    {
                        CollisionWithBact__sub0(this, bnode);

                        bnode->_scale_time = -1;

                        if ( _world->GameShell )
                            SFXEngine::SFXe.startSound(&_world->GameShell->samples2_info, 4);

                        if ( _world->isNetGame )
                        {
                            uamessage_endPlasma epMsg;
                            epMsg.msgID = UAMSG_ENDPLASMA;
                            epMsg.owner = bnode->_owner;
                            epMsg.id = bnode->_gid;

                            yw_arg181 v32;
                            v32.recvFlags = 2;
                            v32.dataSize = sizeof(epMsg);
                            v32.recvID = 0;
                            v32.garant = 1;
                            v32.data = &epMsg;

                            _world->ypaworld_func181(&v32);

                            if ( bnode->_owner != _owner )
                            {
                                bnode->_vp_extra[0].flags = 0;
                                bnode->_vp_extra[0].vp.trigo = NULL;
                                bnode->_vp_extra[0].vp.base = NULL;
                            }
                        }
                        break;
                    }
                }
            }
        }
    }

    if ( !v45 || (v46 && !v49) )
    {
        _status_flg &= ~BACT_STFLAG_BCRASH;
        return 0;
    }

    stru_5150E8 /= (double)v45;

    vec3d stru_5150F4 = stru_5150E8 - _position;

    float v26 = stru_5150F4.length();

    if ( v26 < 0.0001)
        return 0;

    bact_arg88 v33;
    v33.pos1 = stru_5150F4 / v26;

    // FIX MY MATH
    // stru_5150F4 should be normalised?
    // May be replace it with "dot < 0.0" ?
    // Because cos of 1.0...0 is 0..PI/2 and 0...-1.0 is PI/2..PI
    if ( clp_acos( stru_5150F4.dot( _fly_dir ) ) > C_PI_2 )
        return 0;

    if ( !(_status_flg & BACT_STFLAG_BCRASH) )
    {
        if ( a4 )
        {
            SFXEngine::SFXe.startSound(&_soundcarrier, 6);

            _status_flg |= BACT_STFLAG_BCRASH;

            yw_arg180 v40;
            v40.field_4 = 1.0;
            v40.field_8 = stru_5150E8.x;
            v40.field_C = stru_5150E8.z;
            v40.effects_type = 5;

            _world->ypaworld_func180(&v40);
        }
    }

    if ( fabs(_fly_dir_length) < 0.1 )
        _fly_dir_length = 1.0;

    Recoil(&v33);

    _target_vec = _fly_dir;

    _AI_time1 = _clock;
    _AI_time2 = _clock;

    return 1;
}

void NC_STACK_ypabact::Recoil(bact_arg88 *arg)
{
    if ( !(_status_flg & BACT_STFLAG_LAND) )
    {
        if ( _fly_dir.dot(arg->pos1) >= 0.0 )
        {
            if ( _fly_dir_length != 0.0 )
            {
                _position = _old_pos;

                float v4 = _fly_dir.dot(arg->pos1) * 2.0;

                _fly_dir -= arg->pos1 * v4;

                _fly_dir_length *= 25.0 / (fabs(_fly_dir_length) + 10.0);
            }
        }
    }
}

void NC_STACK_ypabact::ypabact_func89(IDVPair *arg)
{
    dprintf("MAKE ME %s\n","ypabact_func89");
    //call_parent(zis, obj, 89, arg);
}


bool GetSectorTarget__sub0__sub0(NC_STACK_ypabact *unit)
{
    for ( NC_STACK_ypabact* &node : unit->_kidList )
    {
        if ( node->_secndTtype != BACT_TGT_TYPE_UNIT )
            return true;
    }
    return false;
}

NC_STACK_ypabact * GetSectorTarget__sub0(cellArea *cell, NC_STACK_ypabact *unit, float *radius, char *job)
{
    NC_STACK_ypaworld *wrld = unit->getBACT_pWorld();

    VhclProto *vhcl_protos = wrld->getYW_vhclProtos();

    NC_STACK_ypabact *v40 = NULL;

    VhclProto *proto = &vhcl_protos[unit->_vehicleID];

    for( NC_STACK_ypabact* &cel_unit : cell->unitsList )
    {
        if ( cel_unit->_bact_type != BACT_TYPES_MISSLE && cel_unit->_status != BACT_STATUS_DEAD )
        {
            if ( cel_unit->_owner != unit->_owner && cel_unit->_owner )
            {
                int job_id;

                switch ( cel_unit->_bact_type )
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
                    float radivs = (unit->_position - cel_unit->_position).length();

                    int v33 = cel_unit->getBACT_viewer();

                    if ( *radius >= radivs || v33 )
                    {
                        if ( unit->_bact_type == BACT_TYPES_GUN || unit->_bact_type == BACT_TYPES_ROBO )
                        {
                            if ( unit->TestTargetSector(cel_unit) )
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

                            if ( unit->IsParentMyRobo() )
                            {
                                if ( unit->_primTtype == BACT_TGT_TYPE_CELL )
                                {
                                    tmp = unit->_primTpos;

                                }
                                else if ( unit->_primTtype == BACT_TGT_TYPE_UNIT )
                                {
                                    tmp = unit->_primT.pbact->_position;
                                }
                                else
                                {
                                    tmp = unit->_position;
                                }

                                v20 = 1;
                            }
                            else
                            {
                                NC_STACK_ypabact *prnt_bct = unit->_parent;

                                if ( prnt_bct->_primTtype == BACT_TGT_TYPE_CELL )
                                {
                                    tmp = prnt_bct->_primTpos;
                                }
                                else if ( prnt_bct->_primTtype == BACT_TGT_TYPE_UNIT )
                                {
                                    tmp = prnt_bct->_primT.pbact->_position;
                                }
                                else
                                {
                                    tmp = unit->_position;
                                }

                                v20 = 0;
                            }

                            if ( (tmp.XZ() - unit->_position.XZ()).length() <= 3600.0 )
                            {
                                if ( unit->TestTargetSector(cel_unit) )
                                {
                                    *radius = radivs;
                                    *job = job_id;
                                    v40 = cel_unit;
                                }
                            }
                            else
                            {
                                int v29 = 0;

                                for ( NC_STACK_ypabact *bct_nd : cel_unit->_attackersList )
                                {
                                    if ( bct_nd->_secndTtype == BACT_TGT_TYPE_UNIT &&
                                         bct_nd->_secndT.pbact == cel_unit && 
                                         bct_nd->_owner == unit->_owner )
                                        v29++;

                                    if ( v29 > 1 ) // Looks like hack
                                        break;
                                }

                                if ( v29 <= 1 && (!v20 || !GetSectorTarget__sub0__sub0(unit)) )
                                {
                                    if ( unit->TestTargetSector(cel_unit) )
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
    }

    return v40;
}

void NC_STACK_ypabact::GetSectorTarget(bact_arg90 *arg)
{
    yw_130arg arg130;
    if ( arg->field_8 & 1 )
    {
        arg130.pos_x = arg->unit->_position.x;
        arg130.pos_z = arg->unit->_position.z;
    }
    else
    {
        arg130.pos_x = arg->pos_x;
        arg130.pos_z = arg->pos_z;
    }

    if ( _world->ypaworld_func130(&arg130) )
    {
        float rad = 1800.0;
        char job = 0;

        arg->ret_unit = NULL;

        for (int x = -1; x < 2; x++)
        {
            for (int y = -1; y < 2; y++)
            {
                NC_STACK_ypabact *v7 = GetSectorTarget__sub0( &arg130.pcell[x + y * _secMaxX], arg->unit, &rad, &job);

                if ( v7 )
                    arg->ret_unit = v7;
            }
        }
    }
}

void NC_STACK_ypabact::GetBestSectorPart(vec3d *arg)
{
    yw_130arg arg130;
    arg130.pos_x = arg->x;
    arg130.pos_z = arg->z;

    _world->ypaworld_func130(&arg130);

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
    yw_130arg arg130;

    int v4 = _secMaxX;
    int v5 = _secMaxY;

    arg->energ1 = 0;
    arg->energ2 = 0;

    if ( arg->field_14 & 1 )
    {
        arg130.pos_x = _position.x;
        arg130.pos_z = _position.z;
    }
    else
    {
        arg130.pos_x = arg->pos.x;
        arg130.pos_z = arg->pos.z;
    }

    if ( _world->ypaworld_func130(&arg130) )
    {
        cellArea *cell = arg130.pcell;

        if ( arg130.sec_x != 0 &&  arg130.sec_z != 0 )
        {
            // left-up
            cellArea *tcell = &cell[-v4 - 1];

            if ( (1 << _owner) & tcell->view_mask )
            {
                for (NC_STACK_ypabact* &cl_unit : tcell->unitsList)
                {
                    if ( cl_unit->_owner )
                    {
                        if ( cl_unit->_status != BACT_STATUS_DEAD && 
                            (cl_unit->_bact_type != BACT_TYPES_ROBO || cl_unit->_owner != _owner) && 
                             cl_unit->_bact_type != BACT_TYPES_MISSLE )
                        {
                            if ( cl_unit->_owner == _owner )
                                arg->energ1 += cl_unit->_energy;
                            else
                                arg->energ2 += cl_unit->_energy;
                        }
                    }
                }
            }
        }

        if ( arg130.sec_z )
        {
            // up
            cellArea *tcell = &cell[-v4];

            if ( (1 << _owner) & tcell->view_mask )
            {
                for (NC_STACK_ypabact* &cl_unit : tcell->unitsList)
                {
                    if ( cl_unit->_owner )
                    {
                        if ( cl_unit->_status != BACT_STATUS_DEAD && 
                            (cl_unit->_bact_type != BACT_TYPES_ROBO || cl_unit->_owner != _owner) && 
                             cl_unit->_bact_type != BACT_TYPES_MISSLE )
                        {
                            if ( cl_unit->_owner == _owner )
                                arg->energ1 += cl_unit->_energy;
                            else
                                arg->energ2 += cl_unit->_energy;
                        }
                    }
                }
            }
        }

        if ( arg130.sec_x < v4 - 1 && arg130.sec_z )
        {
            // right-up
            cellArea *tcell = &cell[-v4 + 1];

            if ( (1 << _owner) & tcell->view_mask )
            {
                for (NC_STACK_ypabact* &cl_unit : tcell->unitsList)
                {
                    if ( cl_unit->_owner )
                    {
                        if ( cl_unit->_status != BACT_STATUS_DEAD && 
                            (cl_unit->_bact_type != BACT_TYPES_ROBO || cl_unit->_owner != _owner) && 
                             cl_unit->_bact_type != BACT_TYPES_MISSLE )
                        {
                            if ( cl_unit->_owner == _owner )
                                arg->energ1 += cl_unit->_energy;
                            else
                                arg->energ2 += cl_unit->_energy;
                        }
                    }
                }
            }
        }

        if ( arg130.sec_x )
        {
            // left
            cellArea *tcell = &cell[-1];

            if ( (1 << _owner) & tcell->view_mask )
            {
                for (NC_STACK_ypabact* &cl_unit : tcell->unitsList)
                {
                    if ( cl_unit->_owner )
                    {
                        if ( cl_unit->_status != BACT_STATUS_DEAD && 
                            (cl_unit->_bact_type != BACT_TYPES_ROBO || cl_unit->_owner != _owner) && 
                             cl_unit->_bact_type != BACT_TYPES_MISSLE )
                        {
                            if ( cl_unit->_owner == _owner )
                                arg->energ1 += cl_unit->_energy;
                            else
                                arg->energ2 += cl_unit->_energy;
                        }
                    }
                }
            }
        }

        // center
        if ( (1 << _owner) & cell->view_mask )
        {
            for (NC_STACK_ypabact* &cl_unit : cell->unitsList)
                {
                    if ( cl_unit->_owner )
                    {
                        if ( cl_unit->_status != BACT_STATUS_DEAD && 
                            (cl_unit->_bact_type != BACT_TYPES_ROBO || cl_unit->_owner != _owner) && 
                             cl_unit->_bact_type != BACT_TYPES_MISSLE )
                        {
                            if ( cl_unit->_owner == _owner )
                                arg->energ1 += cl_unit->_energy;
                            else
                                arg->energ2 += cl_unit->_energy;
                        }
                    }
                }
        }

        if ( arg130.sec_x < v4 - 1 )
        {
            // right
            cellArea *tcell = &cell[1];

            if ( (1 << _owner) & tcell->view_mask )
            {
               for (NC_STACK_ypabact* &cl_unit : tcell->unitsList)
                {
                    if ( cl_unit->_owner )
                    {
                        if ( cl_unit->_status != BACT_STATUS_DEAD && 
                            (cl_unit->_bact_type != BACT_TYPES_ROBO || cl_unit->_owner != _owner) && 
                             cl_unit->_bact_type != BACT_TYPES_MISSLE )
                        {
                            if ( cl_unit->_owner == _owner )
                                arg->energ1 += cl_unit->_energy;
                            else
                                arg->energ2 += cl_unit->_energy;
                        }
                    }
                }
            }
        }

        if ( arg130.sec_x != 0 && arg130.sec_z < v5 - 1 )
        {
            // left-down
            cellArea *tcell = &cell[ v4 - 1 ];

            if ( (1 << _owner) & tcell->view_mask )
            {
                for (NC_STACK_ypabact* &cl_unit : tcell->unitsList)
                {
                    if ( cl_unit->_owner )
                    {
                        if ( cl_unit->_status != BACT_STATUS_DEAD && 
                            (cl_unit->_bact_type != BACT_TYPES_ROBO || cl_unit->_owner != _owner) && 
                             cl_unit->_bact_type != BACT_TYPES_MISSLE )
                        {
                            if ( cl_unit->_owner == _owner )
                                arg->energ1 += cl_unit->_energy;
                            else
                                arg->energ2 += cl_unit->_energy;
                        }
                    }
                }
            }
        }

        if ( arg130.sec_z < v5 - 1  )
        {
            // down
            cellArea *tcell = &cell[ v4 ];

            if ( (1 << _owner) & tcell->view_mask )
            {
                for (NC_STACK_ypabact* &cl_unit : tcell->unitsList)
                {
                    if ( cl_unit->_owner )
                    {
                        if ( cl_unit->_status != BACT_STATUS_DEAD && 
                            (cl_unit->_bact_type != BACT_TYPES_ROBO || cl_unit->_owner != _owner) && 
                             cl_unit->_bact_type != BACT_TYPES_MISSLE )
                        {
                            if ( cl_unit->_owner == _owner )
                                arg->energ1 += cl_unit->_energy;
                            else
                                arg->energ2 += cl_unit->_energy;
                        }
                    }
                }
            }
        }

        if ( arg130.sec_x < v4 - 1 && arg130.sec_z < v5 - 1 )
        {
            // down-right
            cellArea *tcell = &cell[ v4 + 1 ];

            if ( (1 << _owner) & tcell->view_mask )
            {
                for (NC_STACK_ypabact* &cl_unit : tcell->unitsList)
                {
                    if ( cl_unit->_owner )
                    {
                        if ( cl_unit->_status != BACT_STATUS_DEAD && 
                            (cl_unit->_bact_type != BACT_TYPES_ROBO || cl_unit->_owner != _owner) && 
                             cl_unit->_bact_type != BACT_TYPES_MISSLE )
                        {
                            if ( cl_unit->_owner == _owner )
                                arg->energ1 += cl_unit->_energy;
                            else
                                arg->energ2 += cl_unit->_energy;
                        }
                    }
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

            if ( cell->owner == _owner )
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
    vec3d v2d = _rotation.AxisZ().X0Z();
    v2d.normalise();

    arg->pos1 = _position - v2d * ( (arg->field_0 / 3 + 1) * 150.0 );

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
    //arg->pos2 = vec3d::X0Z( arg->pos1.XZ() - _position.XZ() );
}

void NC_STACK_ypabact::ypabact_func95(IDVPair *arg)
{
    dprintf("MAKE ME %s\n","ypabact_func95");
//    call_parent(zis, obj, 95, arg);
}

// Reset
void NC_STACK_ypabact::Renew()
{
    _oflags = BACT_OFLAG_EXACTCOLL;
    _status_flg = 0;
    _host_station = NULL;
    _yls_time = 3000;
    _primTtype = BACT_TGT_TYPE_NONE;

    _secndTtype = BACT_TGT_TYPE_NONE;
    _primT_cmdID = 0;

    int maxX = _world->getYW_mapSizeX();
    int maxY = _world->getYW_mapSizeY();

    _wrldX = maxX * 1200.0;
    _wrldY = -maxY * 1200.0;
    _secMaxX = maxX;
    _secMaxY = maxY;

    _commandID = 0;
//    bact->field_3D1 = 1;
    _killer = NULL;
    _brkfr_time = 0;
    _brkfr_time2 = 0;
    _mpos.x = 0;
    _mpos.y = 0;
    _mpos.z = 0;
    _gun_leftright = 0.0;
    _scale_time = 0;
    _clock = 0;
    _AI_time1 = 0;
    _AI_time2 = 0;
//    bact->field_921 = 0;
//    bact->field_925 = 0;
    _search_time1 = 0;
    _search_time2 = 0;
    _slider_time = 0;
//    bact->field_951 = 0;
    _mgun_time = 0;
    _weapon_time = 0;
    _newtarget_time = 0;
    _assess_time = 0;
    _scale_pos = 0;
    _scale_delay = 0;
    _beam_time = 0;
    _energy_time = 0;
    _fe_time = -45000;
    _salve_counter = 0;
    _kill_after_shot = 0;

    _vp_active = 0;
    _volume = _soundcarrier.samples_data[0].volume;
    _pitch = _soundcarrier.samples_data[0].pitch;

    _m_cmdID = 0;
    _gun_angle_user = _gun_angle;
    _oflags |= BACT_OFLAG_LANDONWAIT;

    for (int i = 0; i < 16; i++)
        _destroyFX[i].clear();

    memset(&_vp_extra, 0, sizeof(extra_vproto) * 3);

    _attackersList.clear();
    _kidList.clear();
    _missiles_list.clear();
}

void NC_STACK_ypabact::HandBrake(update_msg *arg)
{
    _thraction = _mass * 9.77665;

    float v53 = arg->frameTime * 0.001;

    vec3d vaxis = _rotation.AxisY() * vec3d(0.0, 1.0, 0.0);

    if ( vaxis.normalise() > 0.001 )
    {
        float v62 = clp_acos( _rotation.AxisY().dot( vec3d(0.0, 1.0, 0.0) ) );
        float v11 = _maxrot * v53;

        if ( v62 > v11 )
            v62 = (v62 * 1.5) * v11;

        if ( fabs(v62) <= 0.0015 )
        {
            _rotation.SetY( vec3d(0.0, 1.0, 0.0) );

            vec3d axisX = _rotation.AxisX().X0Z();
            axisX.normalise();

            _rotation.SetX( axisX );

            vec3d axisZ = _rotation.AxisZ().X0Z();
            axisZ.normalise();

            _rotation.SetZ( axisZ );

            if ( fabs(_fly_dir_length) < 0.1 )
            {
                _fly_dir = vec3d(0.0, 1.0, 0.0);

                _fly_dir_length = 0;
            }
        }
        else
        {
            _rotation *= mat3x3::AxisAngle(vaxis, v62);
        }
    }

    _fly_dir_length *= 0.8;
}

void NC_STACK_ypabact::ypabact_func98(IDVPair *arg)
{
    dprintf("MAKE ME %s\n","ypabact_func98");
//    call_parent(zis, obj, 98, arg);
}

void NC_STACK_ypabact::CreationTimeUpdate(update_msg *arg)
{
    _scale_time -= arg->frameTime;

    float v30 = arg->frameTime / 1000.0;

    if ( _scale_time > 0 )
    {
        _status_flg |= BACT_STFLAG_SCALE;

        if ( _scale_time < 0 )
            _scale = vec3d(1.0);
        else
            _scale = vec3d( 0.9 / ((float)_scale_time / 1000.0 + 0.9) + 0.1 );

        _rotation = mat3x3::RotateY( 2.5 / _scale.x * v30 ) * _rotation;
    }
    else
    {
        setState_msg v25;
        v25.newStatus = BACT_STATUS_NORMAL;
        v25.setFlags = 0;
        v25.unsetFlags = 0;

        SetState(&v25);

        _status_flg &= ~BACT_STFLAG_SCALE;

        bact_arg80 v24;

        v24.pos = _position;
        v24.field_C = 0;

        SetPosition(&v24);

        NC_STACK_ypabact *a4 = _world->getYW_userHostStation();

        if ( _host_station == a4 )
        {

            if ( IsParentMyRobo() )
            {
                robo_arg134 v23;
                v23.unit = this;
                v23.field_4 = 14;
                v23.field_8 = 0;
                v23.field_C = 0;
                v23.field_10 = 0;
                v23.field_14 = 26;

                _host_station->placeMessage(&v23);
            }
        }

        if ( _host_station )
        {
            if ( _bact_type != BACT_TYPES_GUN )
            {
                _fly_dir = v24.pos - _host_station->_position;

                float fly_len = _fly_dir.length();

                if ( fly_len > 0.001 )
                    _fly_dir /= fly_len;

                _fly_dir_length = 20.0;
            }
        }
    }
}

size_t NC_STACK_ypabact::IsDestroyed()
{
    return (_current_vp.base == _vp_dead.base || _current_vp.base == _vp_genesis.base || _current_vp.base == _vp_megadeth.base) && _status == BACT_STATUS_DEAD;
}

size_t NC_STACK_ypabact::CheckFireAI(bact_arg101 *arg)
{
    vec3d tmp;

    if ( arg->unkn == 2 )
        tmp = arg->pos - _position;
    else
        tmp = arg->pos.X0Z() - _position.X0Z() + vec3d::OY(_height);

    float len = tmp.normalise();

    if ( len == 0.0 )
        return 0;

    WeapProto *a4 = _world->getYW_weaponProtos();

    WeapProto *v8 = NULL;

    int v36;

    if ( _weapon != -1 )
    {
        v8 = &a4[ _weapon ];


        if ( v8->model_id & 1 )
            v36 = v8->model_id & 0xFE;
        else
            v8 = NULL;
    }

    if ( !v8 )
    {
        if ( _mgun == -1 )
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
                if ( len < 1200.0 && tmp.XZ().dot( _rotation.AxisZ().XZ() ) > 0.93 )
                    return 1;
            }
            else
            {
                vec3d tmp2 = tmp * _rotation.AxisZ();

                if ( len < 1200.0 && (tmp.dot( _rotation.AxisZ() ) > 0.0) && v32 / len > tmp2.length() )
                    return 1;
            }
        }
        else
        {
            if ( (arg->pos.XZ() - _position.XZ()).length() < v32 && arg->pos.y > _position.y )
                return 1;
        }
    }
    else if ( v8 )
    {
        if ( v36 )
        {
            if ( v36 == 16 )
            {
                if ( len < 1200.0 && tmp.XZ().dot( _rotation.AxisZ().XZ() ) > 0.91 )
                    return 1;
            }
            else if ( len < 1200.0 && tmp.dot( _rotation.AxisZ() ) > 0.91 )
            {
                return 1;
            }
        }
        else
        {
            if ( (arg->pos.XZ() - _position.XZ()).length() < v8->radius )
                return 1;
        }
    }
    return 0;
}

void NC_STACK_ypabact::MarkSectorsForView()
{
    if ( _bact_type != BACT_TYPES_MISSLE )
    {
        if ( _status != BACT_STATUS_DEAD && _status != BACT_STATUS_CREATE )
        {
            int v8 = 0;

            if ( _parent )
            {
                if (_radar >= _parent->_radar)
                    v8 = 1;
                else if (_sectX != _parent->_sectX)
                    v8 = 1;
                else if (_sectY != _parent->_sectY)
                    v8 = 1;
            }
            else
                v8 = 1;            

            if ( v8 )
            {
                if ( _owner < 8 )
                {
                    cellArea *v19 = _pSector;

                    for (int i = -_radar; i <= _radar; i++)
                    {
                        int yy = _sectY + i;

                        int v20 = _secMaxX * i;

                        if ( _radar == 1 )
                        {
                            if ( yy > 0 && yy < _secMaxY - 1 )
                            {
                                if ( _sectX - 1 > 0 )
                                    v19[v20 - 1].view_mask |= 1 << _owner;

                                v19[v20].view_mask |= 1 << _owner;

                                if ( _sectX + 1 < _secMaxX - 1 )
                                    v19[v20 + 1].view_mask |= 1 << _owner;
                            }
                        }
                        else
                        {
                            float vtmp = POW2((float)_radar) - POW2((float)i);

                            if (vtmp < 0.0)
                                vtmp = 0.0;

                            int tmp = dround( sqrt(vtmp) );

                            for (int j = -tmp; j <= tmp; j++)
                            {
                                int xx = _sectX + j;

                                if ( xx > 0 && xx < _secMaxX - 1 && yy > 0 && yy < _secMaxY - 1 )
                                    v19[v20 + j].view_mask |= 1 << _owner;
                            }
                        }
                    }
                }
            }

            for( NC_STACK_ypabact* &kid : _kidList )
                kid->MarkSectorsForView();
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
//    if ( bact->field_93D > 0 )
//        bact->field_93D -= arg->field_4;

//    if ( bact->field_93D < 0 )
//        bact->field_93D = 0;

    if ( _bflags & BACT_OFLAG_BACTCOLL )
    {
//        if ( !bact->field_93D )
        _oflags |= BACT_OFLAG_BACTCOLL;
    }

    if ( _status != BACT_STATUS_NORMAL || _oflags & BACT_OFLAG_USERINPT )
    {
        _mpos = _position;
        _brkfr_time2 = _clock;
    }
    else
    {
        vec3d tmp = _mpos - _position;

        if (tmp.length() >= 12.0)
        {
            _mpos = _position;
            _brkfr_time2 = _clock;
        }
        else
        {
            if ( _oflags & BACT_OFLAG_BACTCOLL )
                _bflags |= BACT_OFLAG_BACTCOLL;

            if ( _clock - _brkfr_time2 > 10000 )
            {
                if ( (_bact_type == BACT_TYPES_TANK || _bact_type == BACT_TYPES_CAR) && !(_status_flg & BACT_STFLAG_ATTACK) )
                {
                    _old_pos = _position;

                    _position += -_rotation.AxisZ() * 10.0;

                    CorrectPositionInLevelBox(NULL);

                    _rotation = mat3x3::RotateY(0.1) * _rotation;

                    ypaworld_arg136 arg136;
                    arg136.stPos = _old_pos;
                    arg136.vect = _position - _old_pos;
                    arg136.flags = 1;

                    _world->ypaworld_func136(&arg136);

                    if ( arg136.isect )
                    {
                        _position = arg136.isectPos - vec3d::OY(5.0);
                    }
                }
            }
        }
    }
}

size_t NC_STACK_ypabact::FireMinigun(bact_arg105 *arg)
{
    int a5 = 0;

    if ( _world->isNetGame )
        a5 = 1;

    if ( _mgun == -1 )
        return 0;

    NC_STACK_ypabact *v108 = NULL;
    float v123 = 0.0;
    float v121 = 0.0;
    vec3d v66;

    yw_130arg arg130;
    arg130.pos_x = _position.x;
    arg130.pos_z = _position.z;

    vec2d tmp = _position.XZ() + arg->field_0.XZ() * 1000.0;

    _world->ypaworld_func130(&arg130);

    cellArea *pCells[3];
    pCells[0] = arg130.pcell;

    arg130.pos_x = tmp.x;
    arg130.pos_z = tmp.y;
    _world->ypaworld_func130(&arg130);

    pCells[2] = arg130.pcell;

    if ( arg130.pcell == pCells[0] )
    {
        pCells[1] = pCells[0];
    }
    else
    {
        vec2d tmp2 = _position.XZ() + (tmp - _position.XZ()) * 0.5;
        arg130.pos_x = tmp2.x;
        arg130.pos_z = tmp2.y;

        _world->ypaworld_func130(&arg130);

        pCells[1] = arg130.pcell;
    }

    int v107 = 0;
    if ( _bact_type == BACT_TYPES_GUN )
    {
        NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( this );
        int a4 = gun->IsRoboGun();

        if ( a4 )
            v107 = 1;
    }
    else
    {
        _energy -= _gun_power * arg->field_C / 300.0;
    }

    for(int i = 0; i < 3; i++)
    {
        if ( i <= 0 || pCells[ i ] != pCells[ i - 1 ] )
        {
            for ( NC_STACK_ypabact* &cellUnit : pCells[ i ]->unitsList )
            {
                if ( cellUnit != this && cellUnit->_bact_type != BACT_TYPES_MISSLE && cellUnit->_status != BACT_STATUS_DEAD )
                {
                    int v89 = 0;
                    if (cellUnit->_bact_type == BACT_TYPES_GUN)
                    {
                        NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( cellUnit );
                        v89 = gun->IsRoboGun();
                    }

                    if ( cellUnit->_bact_type != BACT_TYPES_GUN || cellUnit->_shield > 100 || !v89 )
                    {
                        if ( (_oflags & BACT_OFLAG_USERINPT || cellUnit->_owner != _owner) && (!v107 || cellUnit != _host_station) )
                        {

                            rbcolls *v93 = cellUnit->getBACT_collNodes();

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
                                    v77 = cellUnit->_position + cellUnit->_rotation.Transpose().Transform( v93->roboColls[j].coll_pos );

                                    v27 = v93->roboColls[j].robo_coll_radius;
                                }
                                else
                                {
                                    v77 = cellUnit->_position;

                                    v27 = cellUnit->_radius;
                                }

                                if ( !v93 || v27 >= 0.01 )
                                {
                                    v121 = v27;

                                    vec3d v63 = v77 - _old_pos;

                                    if ( v63.dot( _rotation.AxisZ() ) >= 0.3 )
                                    {
                                        vec3d v33 = arg->field_0 * v63;

                                        float v111 = v63.length();
                                        float v110 = v33.length();

                                        float v37 = v27 + _gun_radius;

                                        if ( v37 > v110 )
                                        {
                                            if ( sqrt( POW2(v110) + 1000000.0 ) > v111 )
                                            {
                                                if ( !v22 )
                                                {
                                                    int v88 = cellUnit->getBACT_inputting();

                                                    int energ;

                                                    if ( v88 || cellUnit->_status_flg & BACT_STFLAG_ISVIEW )
                                                    {
                                                        float v39 = (_gun_power * arg->field_C) * (100.0 - (float)cellUnit->_shield);
                                                        energ = (v39 * 0.004);
                                                    }
                                                    else
                                                    {

                                                        float v41 = (_gun_power * arg->field_C) * (100.0 - (float)cellUnit->_shield);
                                                        energ = v41 / 100;
                                                    }

                                                    bact_arg84 v86;
                                                    v86.unit = this;
                                                    v86.energy = -energ;

                                                    if ( energ )
                                                        cellUnit->ModifyEnergy(&v86);
                                                }

                                                v22 = 1;

                                                if ( !v108 || v123 > v111 )
                                                {
                                                    v108 = cellUnit;
                                                    v123 = v111;

                                                    v66 = cellUnit->_position;
                                                }
                                            }
                                        }
                                    }

                                }

                            }


                        }
                    }
                }
            }
        }
    }

    int v88 = getBACT_inputting();

    if ( (v88 || _world->ypaworld_func145(this)) && !a5 )
    {
        WeapProto *v90 = _world->getYW_weaponProtos();

        int v45;

        if ( v88 )
        {
            int v43 = v90[_mgun].shot_time_user;
            float v42 = arg->field_C * 1000.0;

            if ( v43 <= v42 )
                v45 = v42;
            else
                v45 = v43;
        }
        else
        {
            int v47 = v90[_mgun].shot_time;
            float v46 = arg->field_C * 1000.0;

            if ( v47 <= v46 )
                v45 = v46;
            else
                v45 = v47;
        }

        if ( arg->field_10 - _mgun_time > v45 )
        {
            _mgun_time = arg->field_10;

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

                v80 = v66 - (v66 - _position) * v50 / v123;
            }
            else
            {
                v59.stPos = _position;
                v59.vect = arg->field_0 * 1000.0;
                v59.flags = 0;

                _world->ypaworld_func149(&v59);

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
                arg147.vehicle_id = _mgun;

                NC_STACK_ypamissile *gunFireBact = _world->ypaworld_func147(&arg147);

                if ( gunFireBact )
                {
                    gunFireBact->_owner = _owner;

                    gunFireBact->_kidRef.Detach();
                    gunFireBact->_parent = NULL;

                    _missiles_list.push_back(gunFireBact);

                    setState_msg v69;
                    v69.newStatus = BACT_STATUS_DEAD;
                    v69.setFlags = 0;
                    v69.unsetFlags = 0;

                    gunFireBact->SetStateInternal(&v69);

                    if ( v96 )
                    {
                        v69.setFlags = BACT_STFLAG_DEATH2;
                        v69.newStatus = BACT_STATUS_NOPE;
                        v69.unsetFlags = 0;
                        gunFireBact->SetStateInternal(&v69);

                        gunFireBact->AlignMissileByNormal( v59.skel->polygons[ v59.polyID ].Normal() );
                    }
                }
            }
        }
    }
    return 1;
}


void NC_STACK_ypabact::sub_4843BC(NC_STACK_ypabact *bact2, int a3)
{
    bact_hudi hudi;

    float v23;
    float v24;

    if ( bact2 )
    {
        vec3d v17 = bact2->_position - _position;

        mat3x3 corrected = _rotation;
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
        v23 = -_gun_leftright;
        v24 = -_gun_angle_user;

        hudi.field_18 = NULL;
    }

    if ( _mgun == -1 )
    {
        hudi.field_0 = 0;
    }
    else
    {
        hudi.field_0 = 1;
        hudi.field_8 = -_gun_leftright;
        hudi.field_C = -_gun_angle_user;
    }

    if ( _weapon == -1 || a3 )
    {
        hudi.field_4 = 0;
    }
    else
    {
        if ( _weapon_flags & 4 )
        {
            hudi.field_4 = 4;
            hudi.field_10 = v23;
            hudi.field_14 = v24;
        }
        else
        {
            if ( (_weapon_flags & 4) || !(_weapon_flags & 2) )
                hudi.field_4 = 2;
            else
                hudi.field_4 = 3;

            hudi.field_10 = -_gun_leftright;
            hudi.field_14 = -_gun_angle_user;
        }
    }

    _world->ypaworld_func153(&hudi);
}

size_t NC_STACK_ypabact::UserTargeting(bact_arg106 *arg)
{
    WeapProto *weaps = _world->getYW_weaponProtos();

    NC_STACK_ypabact *targeto = 0;
    float v56 = 0.0;

    float v55;

    if ( _weapon == -1 )
        v55 = 0.0;
    else
        v55 = weaps[_weapon].radius;

    int a3a = !(_weapon_flags & 2) && !(_weapon_flags & 0x10);

    if ( _weapon != -1 && !a3a )
    {
        yw_130arg arg130;
        arg130.pos_x = _position.x;
        arg130.pos_z = _position.z;

        _world->ypaworld_func130(&arg130);

        vec2d tmp = _rotation.AxisZ().XZ() * 1200.0 + _position.XZ();

        cellArea *pCells[3];

        pCells[0] = arg130.pcell;

        arg130.pos_x = tmp.x;
        arg130.pos_z = tmp.y;

        _world->ypaworld_func130(&arg130);

        pCells[2] = arg130.pcell;

        if ( arg130.pcell == pCells[0] )
        {
            pCells[1] = pCells[0];
        }
        else
        {
            vec2d tmp2 = _position.XZ() + (tmp - _position.XZ()) * 0.5;
            arg130.pos_x = tmp2.x;
            arg130.pos_z = tmp2.y;

            _world->ypaworld_func130(&arg130);

            pCells[1] = arg130.pcell;
        }

        for (int i = 0; i < 3; i++)
        {
            if ( i <= 0 || pCells[i] != pCells[i - 1] )
            {
                if ( pCells[i] )
                {
                    for ( NC_STACK_ypabact* &bct : pCells[i]->unitsList )
                    {
                        if ( bct != this )
                        {
                            if ( bct->_bact_type != BACT_TYPES_MISSLE && bct->_status != BACT_STATUS_DEAD )
                            {
                                int v53 = 0;
                                if (bct->_bact_type == BACT_TYPES_GUN)
                                {
                                    NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( bct );
                                    v53 = gun->IsRoboGun();
                                }

                                if ( !v53 )
                                {
                                    if ( arg->field_0 & 2 || bct->_owner != _owner )
                                    {
                                        if ( arg->field_0 & 1 || bct->_owner == _owner || !bct->_owner )
                                        {
                                            if ( arg->field_0 & 4 || bct->_owner )
                                            {
                                                vec3d mv = bct->_position - _old_pos;

                                                if ( mv.dot( _rotation.AxisZ() ) >= 0.0 )
                                                {
                                                    float mv_len = mv.length();

                                                    vec3d mvd = arg->field_4 * mv;

                                                    float v59 = mv_len * 1000.0 * 0.0005 + 20.0;
                                                    float mvd_len = mvd.length();

                                                    if ( ((mvd_len < v59 && (_weapon_flags & 4)) || (bct->_radius + v55 > mvd_len && !(_weapon_flags & 4)) )
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
                    }
                }
            }
        }
    }

    if ( targeto )
    {
        sub_4843BC(targeto, a3a);

        setTarget_msg arg67;
        arg67.tgt_type = BACT_TGT_TYPE_UNIT;
        arg67.priority = 1;
        arg67.tgt.pbact = targeto;

        SetTarget(&arg67);

        arg->ret_bact = targeto;
        return 1;
    }

    sub_4843BC(NULL, a3a);
    arg->ret_bact = NULL;

    return 0;
}

void NC_STACK_ypabact::HandleVisChildrens(int *arg)
{
    NC_STACK_base *vps[7];
    vps[0] = _vp_normal.base;
    vps[1] = _vp_dead.base;
    vps[2] = _vp_fire.base;
    vps[3] = _vp_genesis.base;
    vps[4] = _vp_wait.base;
    vps[5] = _vp_megadeth.base;
    vps[6] = NULL;

    int v13 = 0;

    while ( vps[v13] )
    {
        BaseList &kids_list = vps[v13]->getBASE_kidList();

        for(BaseList::iterator it = kids_list.begin(); it != kids_list.end(); it++)
        {
            if ( *arg == 1 )
            {
                (*it)->setBASE_parentFollow(1);

                flag_xyz arg68;

                arg68.flag = 7;
                arg68.v = (*it)->getBASE_pos() - _position;

                (*it)->base_func68(&arg68);
            }
            else if ( *arg == 2 )
            {
                (*it)->setBASE_parentFollow(1);

                flag_xyz arg68;

                arg68.flag = 7;
                arg68.v = (*it)->getBASE_pos() + _position;

                (*it)->base_func68(&arg68);
            }
        }

        v13++;
    }
}

size_t NC_STACK_ypabact::GetFightMotivation(float *arg)
{
    if ( _aggr == 100 )
        return 1;

    if ( _aggr )
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

        if ( _status_flg & BACT_STFLAG_ESCAPE )
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

NC_STACK_ypabact *sb_0x493984__sub1(NC_STACK_ypabact *bact)
{
    vec3d v12;

    if ( bact->_primTtype == BACT_TGT_TYPE_CELL )
        v12 = bact->_primTpos;
    else if ( bact->_primTtype == BACT_TGT_TYPE_UNIT )
        v12 = bact->_primT.pbact->_position;
    else
        return NULL;

    float v14 = 215040.0;

    NC_STACK_ypabact *new_leader = NULL;

    for ( NC_STACK_ypabact* &kid_unit : bact->_kidList )
    {
        if ( kid_unit->_status != BACT_STATUS_DEAD )
        {
            int a4 = kid_unit->getBACT_inputting();

            if ( !a4 )
            {

                float v17 = (v12.XZ() - kid_unit->_position.XZ()).length();

                if ( !new_leader || (kid_unit->_bact_type != BACT_TYPES_UFO && v14 > v17) || (new_leader->_bact_type == BACT_TYPES_UFO && (kid_unit->_bact_type != BACT_TYPES_UFO || v14 > v17 )) )
                {
                    new_leader = kid_unit;
                    v14 = v17;
                }
            }
        }
    }
    return new_leader;
}

NC_STACK_ypabact *sb_0x493984__sub0(NC_STACK_ypabact *bact)
{
    float tmp = 0.0;
    NC_STACK_ypabact *new_leader = NULL;

    for ( NC_STACK_ypabact* &kid_unit : bact->_kidList )
    {
        if ( kid_unit->_status != BACT_STATUS_DEAD )
        {
            float v10;
            if ( kid_unit->_bact_type == BACT_TYPES_UFO )
            {
                v10 = 0.0;
            }
            else
            {
                float v8 = 1.0 - ( (bact->_position.XZ() - kid_unit->_position.XZ()).length() / 110400.0);
                v10 = (float)kid_unit->_energy / (float)kid_unit->_energy_max + v8;
            }

            if ( !new_leader || tmp < v10 )
            {
                new_leader = kid_unit;
                tmp = v10;
            }
        }
    }

    return new_leader;
}

NC_STACK_ypabact *sb_0x493984(NC_STACK_ypabact *bact, int a2)
{
    if ( !bact->_kidList.empty() )
    {
        NC_STACK_ypabact *new_leader = NULL;

        if (a2)
            new_leader = sb_0x493984__sub1(bact);
        else
            new_leader = sb_0x493984__sub0(bact);

        if (!new_leader)
            return NULL;

        if (new_leader->_bact_type != BACT_TYPES_UFO || bact->_bact_type == BACT_TYPES_UFO)
        {
            bact->_host_station->AddSubject(new_leader);

            new_leader->CopyTargetOf(bact);

            for ( World::RefBactList::iterator it = bact->_kidList.begin(); it != bact->_kidList.end(); )
            {
                NC_STACK_ypabact *kid_unit = *it;
                it++;

                new_leader->AddSubject(kid_unit);

                kid_unit->CopyTargetOf(new_leader);
            }
            new_leader->_commandID = bact->_commandID;
            return new_leader;
        }

    }
    return NULL;
}

void NC_STACK_ypabact::sub_493480(NC_STACK_ypabact *bact2, int mode)
{
    if ( _world->isNetGame )
    {
        static uamessage_reorder ordMsg;

        ordMsg.comm = bact2->_gid;
        ordMsg.num = 0;
        ordMsg.commID = bact2->_commandID;

        for ( NC_STACK_ypabact* &bct : bact2->_kidList )
        {
            if ( ordMsg.num < 500 )
            {
                ordMsg.units[ordMsg.num] = bct->_gid;
                ordMsg.num++;
            }
        }

        ordMsg.owner = _owner;
        ordMsg.sz = (char *)&ordMsg.units[ordMsg.num] - (char *)&ordMsg;
        ordMsg.mode = mode;
        ordMsg.msgID = UAMSG_REORDER;

        yw_arg181 arg181;
        arg181.dataSize = ordMsg.sz;
        arg181.recvID = 0;
        arg181.garant = 1;
        arg181.data = &ordMsg;
        arg181.recvFlags = 2;

        _world->ypaworld_func181(&arg181);
    }
}

void NC_STACK_ypabact::ReorganizeGroup(bact_arg109 *arg)
{
    switch ( arg->field_0 )
    {
    case 1:
        if ( arg->field_4 )
        {
            if ( arg->field_4->_status == BACT_STATUS_DEAD )
            {
                ypa_log_out("ORG_NEWCHIEF: Dead master\n");
            }
            else if ( arg->field_4 != _parent && arg->field_4 != this )
            {
                _commandID = arg->field_4->_commandID;
                _aggr = arg->field_4->_aggr;

                arg->field_4->AddSubject(this);

                while ( !_kidList.empty() )
                {
                    NC_STACK_ypabact *kid = _kidList.front();

                    kid->_aggr = arg->field_4->_aggr;
                    kid->_commandID = arg->field_4->_commandID;

                    arg->field_4->AddSubject(kid);
                    
                    kid->CopyTargetOf(arg->field_4);
                }

                CopyTargetOf(arg->field_4);
                sub_493480(arg->field_4, 1);
            }
        }
        break;

    case 2:
        if ( _host_station != _parent || arg->field_4 != this )
        {
            if ( _status == BACT_STATUS_DEAD )
            {
                ypa_log_out("ORG_BECOMECHIEF dead vehicle\n");
            }
            else
            {
                if ( _host_station != _parent && _host_station )
                    _host_station->AddSubject(this);

                if ( arg->field_4 )
                {
                    CopyTargetOf(arg->field_4);

                    _aggr = arg->field_4->_aggr;
                    _commandID = arg->field_4->_commandID;

                    AddSubject(arg->field_4);

                    while ( !arg->field_4->_kidList.empty() )
                    {
                        NC_STACK_ypabact *kid = arg->field_4->_kidList.front();
                        
                        AddSubject(kid);
                        kid->_aggr = arg->field_4->_aggr;

                        kid->CopyTargetOf(this);
                    }

                    _commandID = arg->field_4->_commandID;
                    sub_493480(this, 2);
                }
                else
                {
                    if ( _host_station != _parent && _host_station )
                    {
                        int a4 = _host_station->getROBO_commCount();

                        _commandID = a4;

                        _host_station->setROBO_commCount(a4 + 1);
                    }
                    sub_493480(this, 2);
                }
            }
        }
        break;

    case 3:
        if ( _status == BACT_STATUS_DEAD )
        {
            ypa_log_out("ORG_NEWCOMMAND: dead vehicle\n");
        }
        else if (_host_station)
        {

            if ( _host_station == _parent )
            {
                NC_STACK_ypabact *v14 = sb_0x493984(this, 0);

                if ( v14 )
                    sub_493480(v14, 13);
            }
            else
            {
                _host_station->AddSubject(this);
            }

            int a4 = _host_station->getROBO_commCount();
            _commandID = a4;

            if (_world->isNetGame)
                _commandID |= _owner << 24;

            _host_station->setROBO_commCount(a4 + 1);
            sub_493480(this, 3);
        }
        break;

    case 4:
        if ( arg->field_4->IsParentMyRobo() )
        {
            NC_STACK_ypabact *v19 = sb_0x493984(arg->field_4, 0);

            if ( v19 )
                sub_493480(v19, 14);
        }

        AddSubject(arg->field_4);

        arg->field_4->_commandID = _commandID;

        arg->field_4->CopyTargetOf(this);
        sub_493480(this, 4);
        break;

    case 6:
    {
        int a4 = getBACT_inputting();

        if ( !a4 )
        {
            NC_STACK_ypabact *v21 = sb_0x493984(this, 1);

            if ( v21 )
            {
                v21->AddSubject(this);
                v21->_commandID = _commandID;

                sub_493480(v21, 6);
            }
        }
    }
    break;

    default:
        break;
    }
}

void NC_STACK_ypabact::DoTargetWaypoint()
{
    if ( ( _position.XZ() - _primTpos.XZ() ).length() >= 300.0 )
        return;

    if ( !(_status_flg & BACT_STFLAG_WAYPOINTCCL) )
    {
        _current_waypoint++;

        setTarget_msg arg67;

        if ( _waypoints_count > 1 )
        {
            arg67.tgt_type = BACT_TGT_TYPE_CELL_IND;
            arg67.priority = 0;
            arg67.tgt_pos = _waypoints[ _current_waypoint ];

            SetTarget(&arg67);
        }

        if ( _current_waypoint >= _waypoints_count - 1 )
        {
            if ( _m_cmdID )
            {
                NC_STACK_ypabact *v9 = _world->FindBactByCmdOwn(_m_cmdID, _m_owner);

                if ( v9 )
                {
                    if ( (1 << _owner) & v9->_pSector->view_mask )
                    {
                        arg67.tgt.pbact = v9;
                        arg67.tgt_type = BACT_TGT_TYPE_UNIT;
                        arg67.priority = 0;

                        SetTarget(&arg67);
                    }
                }
            }

            _m_owner = 0;
            _m_cmdID = 0;
            _status_flg &= ~(BACT_STFLAG_WAYPOINT | BACT_STFLAG_WAYPOINTCCL);
        }
    }
    else
    {

        _current_waypoint++;

        int v5 = _current_waypoint;

        if ( _current_waypoint >= _waypoints_count )
        {
            _current_waypoint = 0;
            v5 = 0;
        }

        setTarget_msg arg67;

        arg67.tgt_type = BACT_TGT_TYPE_CELL_IND;
        arg67.priority = 0;
        arg67.tgt_pos = _waypoints[ v5 ];

        SetTarget(&arg67);
    }
}

size_t NC_STACK_ypabact::TargetAssess(bact_arg110 *arg)
{
    bool primTgtDone = false;
    bool primTgtNear = false;

    if ( arg->tgType == BACT_TGT_TYPE_FRMT 
         && 
        (_primTtype == BACT_TGT_TYPE_FRMT || _secndTtype == BACT_TGT_TYPE_FRMT) )
        return TA_MOVE;

    if ( arg->tgType == BACT_TGT_TYPE_NONE)
        return TA_IGNORE;

    if ( _primTtype == BACT_TGT_TYPE_CELL )
    {
        if ( (_position.XZ() - _primTpos.XZ()).length() < 1800.0 )
            primTgtNear = true;

        if ( _owner == _primT.pcell->owner )
            primTgtDone = true;
    }

    if ( _primTtype == BACT_TGT_TYPE_UNIT )
    {
        if ( (_position.XZ() - _primT.pbact->_position.XZ()).length() < 1800.0 )
            primTgtNear = true;

        if ( _owner == _primT.pbact->_owner )
            primTgtDone = true;
    }

    if ( arg->tgType == BACT_TGT_TYPE_UNIT )
    {
        NC_STACK_ypabact *enemy = NULL;
        bool isSecTgt = false;
        int aggr = 0;

        if ( arg->priority == 1 )
        {
            enemy = _secndT.pbact;
            isSecTgt = true;
            aggr = 50;
        }
        else if ( arg->priority == 0)
        {
            enemy = _primT.pbact;
            isSecTgt = false;
            aggr = 25;
        }

        if ( enemy )
        {
            float enemyDistance = (enemy->_position.XZ() - _position.XZ()).length();

            if ( !( (1 << _owner) & enemy->_pSector->view_mask) )
                return TA_CANCEL;

            if ( _aggr >= 100 )
            {
                if ( isSecTgt && enemyDistance > 2160.0 )
                    return TA_CANCEL;

                return TA_FIGHT;
            }

            if ( enemy->_owner == 0 || enemy->_owner == _owner )
            {
                if ( enemyDistance < 300.0 )
                    return TA_IGNORE;

                return TA_MOVE;
            }

            if ( _status_flg & BACT_STFLAG_ESCAPE )
            {
                if ( !primTgtNear )
                    return TA_CANCEL;

                return TA_FIGHT;
            }

            if ( _aggr < aggr )
            {
                if ( primTgtNear && primTgtDone )
                    return TA_FIGHT;

                return TA_CANCEL;
            }
            
            if ( !isSecTgt || _bact_type == BACT_TYPES_GUN )
                return TA_FIGHT;

            if ( enemyDistance > 2160.0 )
                return TA_CANCEL;

            vec3d tgtPos;

            if ( IsParentMyRobo() )
            {

                if ( _primTtype == BACT_TGT_TYPE_CELL )
                    tgtPos = _primTpos;
                else if ( _primTtype == BACT_TGT_TYPE_UNIT )
                    tgtPos = _primT.pbact->_position;
                else
                    tgtPos = _position;
            }
            else if ( _parent )
            {
                if ( _parent->_primTtype == BACT_TGT_TYPE_CELL )
                    tgtPos = _parent->_primTpos;
                else if ( _parent->_primTtype == BACT_TGT_TYPE_UNIT )
                    tgtPos = _parent->_primT.pbact->_position;
                else
                    tgtPos = _position;
            }

            if ( (tgtPos.XZ() - _position.XZ()).length() > 3600.0 )
            {
                int v28 = 0;

                for( NC_STACK_ypabact *v43 : _secndT.pbact->_attackersList )
                {
                    if ( v43->_secndTtype == BACT_TGT_TYPE_UNIT &&
                         v43->_secndT.pbact == _secndT.pbact &&
                         v43->_owner == _owner )
                        v28++;

                    if ( v28 > 2 )
                        break;
                }

                if ( v28 > 2 )
                    return TA_CANCEL;
            }
            return TA_FIGHT;
        }
    }
    else if ( arg->tgType == BACT_TGT_TYPE_CELL )
    {
        cellArea *pCell = NULL;
        vec2d cellPos;
        bool isSecTgt = false;
        int aggr = 0;

        if ( _secndTtype == BACT_TGT_TYPE_CELL )
        {
            pCell = _secndT.pcell;
            cellPos = _sencdTpos.XZ();

            aggr = 75;
            isSecTgt = true;
        }
        else if ( _primTtype == BACT_TGT_TYPE_CELL )
        {
            pCell = _primT.pcell;
            cellPos = _primTpos.XZ();

            aggr = 25;
            isSecTgt = false;
        }

        if ( (_status_flg & BACT_STFLAG_WAYPOINT) && !isSecTgt )
        {
            DoTargetWaypoint();
            return TA_MOVE;
        }

        if ( !pCell )
            return TA_IGNORE;

        int cellEnergy = pCell->GetEnergy();

        float cellDistance = (_position.XZ() - cellPos).length();

        if ( _aggr >= 100 )
        {
            if ( cellEnergy <= 0 && pCell->owner == _owner )
            {
                if ( cellDistance < 300.0 )
                    return TA_IGNORE;

                return TA_MOVE;
            }

            return TA_FIGHT;
        }

        if ( cellDistance >= 300.0 )
        {
            if ( _owner != pCell->owner )
            {
                if ( (_status_flg & BACT_STFLAG_ESCAPE) || _aggr < aggr )
                    return TA_CANCEL;

                return TA_FIGHT;
            }

            if ( isSecTgt )
                return TA_CANCEL;

            return TA_MOVE;
        }

        if ( _owner == pCell->owner )
        {
            if ( isSecTgt )
                return TA_CANCEL;

            return TA_IGNORE;
        }

        if ( (_status_flg & BACT_STFLAG_ESCAPE) || _aggr < aggr )
            return TA_CANCEL;

        return TA_FIGHT;
    }

    return TA_IGNORE;
}

size_t NC_STACK_ypabact::TestTargetSector(NC_STACK_ypabact *)
{
    return 1;
}

void NC_STACK_ypabact::BeamingTimeUpdate(update_msg *arg)
{
    float v14 = 0.66;

    if ( _scale_delay <= 0 )
    {
        if ( _scale_time >= 1980.0 )
        {
            if ( _scale_time >= 3000 )
            {
                _world->ypaworld_func168(this);

                _status_flg |= BACT_STFLAG_CLEAN;

                Die();

                if ( _oflags & BACT_OFLAG_USERINPT )
                    _status_flg |= BACT_STFLAG_NORENDER;
                else
                    Release();

                _status_flg &= ~BACT_STFLAG_SCALE;
            }
            else
            {
                _status_flg |= BACT_STFLAG_SCALE;

                _scale = vec3d(1.0, 30.0, 1.0) - vec3d::OY( (_scale_time - 1980.0) * 30.0 / 1020.0 );
            }
        }
        else
        {
            if ( _vp_genesis.base != _current_vp.base )
            {
                setState_msg arg78;
                arg78.newStatus = BACT_STATUS_BEAM;
                arg78.setFlags = 0;
                arg78.unsetFlags = 0;

                SetState(&arg78);
            }

            _status_flg |= BACT_STFLAG_SCALE;

            _scale = vec3d(1.0, 0.0, 1.0) + vec3d::OY( (30 * _scale_time)/ (v14 * 3000.0) );
        }

        _scale_time += arg->frameTime;
    }
    else
    {
        _scale_delay -= arg->frameTime;
    }
}

void NC_STACK_ypabact::StartDestFX__sub0(const destFX &fx)
{
    ypaworld_arg146 arg146;

    arg146.pos = _position;
    arg146.vehicle_id = fx.ModelID;

    if ( _radius > 31.0 )    // 31.0
    {
        float len = fx.pos.length();

        if ( len > 0.1 )
        {
            vec3d pos = fx.pos / len * _radius;

            arg146.pos += _rotation.Transform(pos);
        }
    }

    NC_STACK_ypabact *bah = _world->ypaworld_func146(&arg146);

    if ( bah )
    {
        _world->ypaworld_func134(bah);

        setState_msg v18;
        v18.newStatus = BACT_STATUS_DEAD;
        v18.setFlags = 0;
        v18.unsetFlags = 0;

        bah->SetStateInternal(&v18);

        bah->_fly_dir = _rotation.Transform(fx.pos);

        if ( fx.type_flag & 0x10 )
            bah->_fly_dir += _fly_dir * _fly_dir_length;

        float len = bah->_fly_dir.length();

        if ( len > 0.001 )
        {
            bah->_fly_dir /= len;
            bah->_fly_dir_length = len;
        }

    }
}

void NC_STACK_ypabact::StartDestFX(uint8_t arg)
{
    if ( _world->ypaworld_func145(this) )
    {
        int a4 = _world->getYW_destroyFX();

        if (a4 > 16)
            a4 = 16;

        for (int i = 0; i < a4; i++)
        {
            if ( _destroyFX[i].ModelID )
            {
                const destFX &fx = _destroyFX[i];

                if ( ((fx.type_flag & 2) && arg == 2) || ((fx.type_flag & 1) && arg == 1) || ((fx.type_flag & 4) && arg == 4) || ((fx.type_flag & 8) && arg == 8) )
                    StartDestFX__sub0(fx);
            }
        }
    }
}

void NC_STACK_ypabact::CorrectPositionOnLand()
{
    float radius;
    if ( _viewer_radius >= 32.0 )
        radius = _viewer_radius;
    else
        radius = 32.0;

    yw_137col coltmp[10];

    ypaworld_arg137 arg137;
    arg137.pos = _position;
    arg137.pos2 = _rotation.AxisX();
    arg137.coll_max = 10;
    arg137.radius = radius;
    arg137.field_30 = 0;
    arg137.collisions = coltmp;

    _world->ypaworld_func137(&arg137);

    vec3d tmp(0.0, 0.0, 0.0);

    float trad = 0.0;

    for (int i = arg137.coll_count - 1; i >= 0; i-- )
    {
        yw_137col *clsn = &arg137.collisions[i];

        if ( clsn->pos2.y < 0.6 )
        {
            vec3d tmp2 = _position - clsn->pos1;

            tmp += clsn->pos2;

            float v36 = radius - tmp2.length();

            if ( trad == 0.0 || trad < v36 )
                trad = v36;
        }
    }

    if ( _viewer_radius >= 32.0 )
        radius = _viewer_radius;
    else
        radius = 32.0;

    arg137.pos = _position;
    arg137.pos2 = -_rotation.AxisX();
    arg137.coll_max = 10;
    arg137.radius = radius;
    arg137.field_30 = 0;
    arg137.collisions = coltmp;

    _world->ypaworld_func137(&arg137);

    for (int i = arg137.coll_count - 1; i >= 0; i-- )
    {
        yw_137col *clsn = &arg137.collisions[i];

        if ( clsn->pos2.y < 0.6 )
        {
            vec3d tmp2 = _position - clsn->pos1;

            tmp += clsn->pos2;

            float v36 = radius - tmp2.length();

            if ( trad == 0.0 || trad < v36 )
                trad = v36;
        }
    }

    float v25 = tmp.length();

    if ( v25 > 0.0001 )
        tmp /= v25;

    _position -= tmp * trad;
}


void NC_STACK_ypabact::CorrectPositionInLevelBox(void *)
{
    int v4 = 0;

    if ( _position.x > _wrldX - 1210.0 )
    {
        v4 = 1;
        _position.x = _wrldX - 1210.0;
    }

    if ( _position.x < 1210.0 )
    {
        v4 = 1;
        _position.x = 1210.0;
    }

    if ( _position.z > -1210.0 )
    {
        v4 = 1;
        _position.z = -1210.0;
    }

    if ( _position.z < _wrldY + 1210.0 )
    {
        v4 = 1;
        _position.z = _wrldY + 1210.0;
    }

    if ( _oflags & BACT_OFLAG_VIEWER )
    {
        if ( v4 )
        {
            if ( _bact_type != BACT_TYPES_TANK && _bact_type != BACT_TYPES_CAR )
            {
                ypaworld_arg136 arg136;

                arg136.stPos = _position - vec3d::OY(100.0);

                arg136.vect = vec3d::OY(_viewer_overeof + 100.0);
                arg136.flags = 0;

                _world->ypaworld_func136(&arg136);

                if ( arg136.isect )
                    _position.y = arg136.isectPos.y - _viewer_overeof;
            }
        }
    }
}

void ypabact_NetUpdate_VPHACKS(NC_STACK_ypabact *bact, update_msg *upd)
{
    if ( bact->_vp_extra_mode == 1 )
    {
        int engy = bact->_energy_max * 0.7;

        if ( engy < 10000 )
            engy = 10000;

        if ( engy > 25000 )
            engy = 25000;

        sb_0x4874c4(bact, engy, upd->frameTime, 0.75);
        bact->_scale_time -= upd->frameTime;

        if ( bact->_scale_time < 0 )
        {
            bact->_vp_extra[0].vp.trigo = 0;
            bact->_vp_extra[0].vp.base = 0;
        }
    }

    if ( bact->_vp_extra_mode == 2 )
    {
        NC_STACK_yparobo *roboo = dynamic_cast<NC_STACK_yparobo *>(bact);

        if (roboo)
        {
            roboo->_roboBeamTimePre -= upd->frameTime;
            if ( roboo->_roboBeamTimePre <= 0 )
            {
                roboo->_roboBeamTimePre = 0;
                SFXEngine::SFXe.startSound(&bact->_soundcarrier, 10);

                roboo->_roboState &= ~NC_STACK_yparobo::ROBOSTATE_MOVE;
                bact->_vp_extra[0].flags = 0;
                bact->_vp_extra[1].flags = 0;
            }
            else
            {
                if ( roboo->_roboBeamFXTime <= 0 )
                {
                    if ( bact->_vp_extra[0].flags & EVPROTO_FLAG_ACTIVE )
                    {
                        roboo->_roboBeamFXTime = roboo->_roboBeamTimePre / 10;
                        bact->_vp_extra[0].flags &= ~EVPROTO_FLAG_ACTIVE;
                    }
                    else
                    {
                        roboo->_roboBeamFXTime = (1500 - roboo->_roboBeamTimePre) / 10;
                        bact->_vp_extra[0].pos = bact->_position;
                        bact->_vp_extra[0].rotate = bact->_rotation;;
                        bact->_vp_extra[0].flags = 3;
                        bact->_vp_extra[0].scale = 1.25;
                        bact->_vp_extra[0].vp.base = bact->_vp_genesis.base;
                        bact->_vp_extra[0].vp.trigo = bact->_vp_genesis.trigo;
                    }

                    if ( roboo->_vp_extra[1].flags & EVPROTO_FLAG_ACTIVE )
                    {
                        roboo->_roboBeamFXTime = roboo->_roboBeamTimePre / 10;
                        bact->_vp_extra[1].flags &= ~EVPROTO_FLAG_ACTIVE;
                    }
                    else
                    {
                        roboo->_roboBeamFXTime = (1500 - roboo->_roboBeamTimePre) / 10;
                        bact->_vp_extra[1].pos = roboo->_roboBeamPos;
                        bact->_vp_extra[1].rotate = bact->_rotation;
                        bact->_vp_extra[1].flags = 1;
                        bact->_vp_extra[1].vp.base = bact->_vp_genesis.base;
                        bact->_vp_extra[1].vp.trigo = bact->_vp_genesis.trigo;
                    }
                }
                roboo->_roboBeamFXTime -= upd->frameTime;
            }

        }
    }
}

void NC_STACK_ypabact::NetUpdate(update_msg *upd)
{
    ypabact_NetUpdate_VPHACKS(this, upd);

    yw_130arg arg130;
    arg130.pos_x = _position.x;
    arg130.pos_z = _position.z;
    if ( !_world->ypaworld_func130(&arg130) )
    {
        FixBeyondTheWorld();

        arg130.pos_x = _position.x;
        arg130.pos_z = _position.z;
        _world->ypaworld_func130(&arg130);
    }

    cellArea *oldSect = _pSector;

    _sectX = arg130.sec_x;
    _sectY = arg130.sec_z;
//  bact->pos_x_cntr = arg130.pos_x_cntr;
//  bact->pos_y_cntr = arg130.pos_y_cntr;
    _pSector = arg130.pcell;

    if ( oldSect != arg130.pcell )
    {
        _cellRef.Detach();
        _cellRef = _pSector->unitsList.push_back(this);
    }

    _clock += upd->frameTime;

    ypabact_func117(upd);

    for ( NC_STACK_ypamissile* &misl : _missiles_list )
    {
        misl->setMISS_launcher(this);
        misl->Update(upd);
    }

    sub_481F94(this);

    _tForm.locPos = _position;

    if ( _status_flg & BACT_STFLAG_SCALE )
        _tForm.locSclRot = _rotation.Transpose() * mat3x3::Scale(_scale);
    else
        _tForm.locSclRot = _rotation.Transpose();

    int units_cnt = upd->units_count;

    upd->units_count = 0;

    for (NC_STACK_ypabact* &bct : _kidList)
    {
        bct->NetUpdate(upd);
        upd->units_count++;
    }

    upd->units_count = units_cnt;

    _soundcarrier.field_0 = _position;
    _soundcarrier.field_C = _fly_dir * _fly_dir_length;

    SFXEngine::SFXe.sb_0x4242e0(&_soundcarrier);
}

void NC_STACK_ypabact::ypabact_func117(update_msg *upd)
{
    if (_world->netInterpolate)
        ypabact_func122(upd);
    else
        ypabact_func123(upd);
}

void NC_STACK_ypabact::Release()
{
    if ( _owner )
    {
        if ( _world->isNetGame )
        {
            if ( _bact_type != BACT_TYPES_MISSLE )
            {
                uamessage_destroyVhcl destrMsg;

                destrMsg.msgID = UAMSG_DESTROYVHCL;
                destrMsg.owner = _owner;
                destrMsg.id = _gid;
                destrMsg.type = _bact_type;

                yw_arg181 v6;
                v6.recvFlags = 2;
                v6.recvID = 0;
                v6.data = &destrMsg;
                v6.garant = 1;
                v6.dataSize = sizeof(destrMsg);

                _world->ypaworld_func181(&v6);
            }
        }
    }

    _world->ypaworld_func144(this);
}

size_t NC_STACK_ypabact::SetStateInternal(setState_msg *arg)
{
    int result = 0;

    if ( arg->newStatus )
        _status = arg->newStatus;

    if ( arg->setFlags )
        _status_flg |= arg->setFlags;

    if ( arg->unsetFlags )
        _status_flg &= ~arg->unsetFlags;

    if ( arg->newStatus == BACT_STATUS_DEAD && (_vp_active != 2 && _vp_active != 3) )
    {
        _energy = -10000;

        _current_vp.base = _vp_dead.base;
        _current_vp.trigo = _vp_dead.trigo;

        _vp_active = 2;

        if ( _soundFlags & 2 )
        {
            if ( _oflags & BACT_OFLAG_USERINPT )
            {
                yw_arg180 v43;
                v43.effects_type = 4;

                _world->ypaworld_func180(&v43);
            }

            SFXEngine::SFXe.sub_424000(&_soundcarrier, 1);
            _soundFlags &= ~2;
        }

        if ( _oflags & BACT_OFLAG_USERINPT )
            SFXEngine::SFXe.sub_424000(&_soundcarrier, 8);

        if ( _soundFlags & 1 )
        {
            _soundFlags &= ~1;
            SFXEngine::SFXe.sub_424000(&_soundcarrier, 0);
        }

        if ( _soundFlags & 8 )
        {
            _soundFlags &= ~8;
            SFXEngine::SFXe.sub_424000(&_soundcarrier, 3);
        }

        if ( _soundFlags & 4 )
        {
            _soundFlags &= ~4;
            SFXEngine::SFXe.sub_424000(&_soundcarrier, 2);
        }

        SFXEngine::SFXe.startSound(&_soundcarrier, 7);

        _soundFlags |= 0x80;

        StartDestFX(1);

        result = 1;
    }

    if ( arg->newStatus == BACT_STATUS_NORMAL && 1 != _vp_active )
    {
        _current_vp.base = _vp_normal.base;
        _current_vp.trigo = _vp_normal.trigo;

        _vp_active = 1;

        if ( _soundFlags & 8 )
        {
            _soundFlags &= ~8;
            SFXEngine::SFXe.sub_424000(&_soundcarrier, 3);
        }

        if ( _soundFlags & 4 )
        {
            _soundFlags &= ~4;
            SFXEngine::SFXe.sub_424000(&_soundcarrier, 2);
        }

        if ( _soundFlags & 0x80 )
        {
            _soundFlags &= ~0x80;
            SFXEngine::SFXe.sub_424000(&_soundcarrier, 7);
        }

        if ( !(_soundFlags & 1) )
        {
            _soundFlags |= 1;
            SFXEngine::SFXe.startSound(&_soundcarrier, 0);
        }

        result = 1;
    }

    if ( arg->newStatus == BACT_STATUS_BEAM && 5 != _vp_active )
    {
        _vp_active = 5;
        _current_vp.base = _vp_genesis.base;
        _current_vp.trigo = _vp_genesis.trigo;

        if ( _soundFlags & 8 )
        {
            _soundFlags &= ~8;
            SFXEngine::SFXe.sub_424000(&_soundcarrier, 3);
        }

        if ( _soundFlags & 4 )
        {
            _soundFlags &= ~4;
            SFXEngine::SFXe.sub_424000(&_soundcarrier, 2);
        }

        if ( _soundFlags & 0x80 )
        {
            _soundFlags &= ~0x80;
            SFXEngine::SFXe.sub_424000(&_soundcarrier, 7);
        }

        if ( !(_soundFlags & 0x200) )
        {
            _soundFlags |= 0x200;
            SFXEngine::SFXe.startSound(&_soundcarrier, 9);
        }

        StartDestFX(8);

        result = 1;
    }

    if ( arg->newStatus == BACT_STATUS_IDLE && _vp_active != 6 )
    {
        _current_vp.base = _vp_wait.base;
        _current_vp.trigo = _vp_wait.trigo;
        _vp_active = 6;

        if ( _soundFlags & 1 )
        {
            _soundFlags &= ~1;
            SFXEngine::SFXe.sub_424000(&_soundcarrier, 0);
        }

        if ( _soundFlags & 8 )
        {
            _soundFlags &= ~8;
            SFXEngine::SFXe.sub_424000(&_soundcarrier, 3);
        }

        if ( _soundFlags & 0x80 )
        {
            _soundFlags &= ~0x80;
            SFXEngine::SFXe.sub_424000(&_soundcarrier, 7);
        }

        if ( !(_soundFlags & 4) )
        {
            _soundFlags |= 4;
            SFXEngine::SFXe.startSound(&_soundcarrier, 2);
        }

        result = 1;
    }

    if ( arg->newStatus == BACT_STATUS_CREATE && 4 != _vp_active )
    {
        _vp_active = arg->newStatus;
        _current_vp.base = _vp_genesis.base;
        _current_vp.trigo = _vp_genesis.trigo;

        if ( _soundFlags & 2 )
        {
            if ( _oflags & BACT_OFLAG_USERINPT )
            {
                yw_arg180 v46;
                v46.effects_type = 4;

                _world->ypaworld_func180(&v46);
            }

            SFXEngine::SFXe.sub_424000(&_soundcarrier, 1);
            _soundFlags &= ~2;
        }

        if ( _soundFlags & 1 )
        {
            _soundFlags &= ~1;
            SFXEngine::SFXe.sub_424000(&_soundcarrier, 0);
        }

        if ( _soundFlags & 4 )
        {
            _soundFlags &= ~4;
            SFXEngine::SFXe.sub_424000(&_soundcarrier, 2);
        }

        if ( _soundFlags & 0x80 )
        {
            _soundFlags &= ~0x80;
            SFXEngine::SFXe.sub_424000(&_soundcarrier, 7);
        }

        if ( !(_soundFlags & 8) )
        {
            _soundFlags |= 8;
            SFXEngine::SFXe.startSound(&_soundcarrier, 3);
        }

        StartDestFX(4);

        result = 1;
    }

    if ( arg->unsetFlags == BACT_STFLAG_FIRE && _vp_active == 7 )
    {
        if ( _oflags & BACT_OFLAG_USERINPT )
        {
            yw_arg180 v45;
            v45.effects_type = 4;
            _world->ypaworld_func180(&v45);
        }

        _current_vp.base = _vp_normal.base;
        _current_vp.trigo = _vp_normal.trigo;
        _vp_active = 1;

        SFXEngine::SFXe.sub_424000(&_soundcarrier, 1);

        _soundFlags &= ~2;

        result = 1;
    }

    if ( arg->unsetFlags == BACT_STFLAG_DEATH2 && _vp_active == 3 )
    {
        _vp_active = 1;
        _current_vp.base = _vp_normal.base;
        _current_vp.trigo = _vp_normal.trigo;

        result = 1;
    }

    if ( arg->setFlags == BACT_STFLAG_FIRE && _vp_active != 7 )
    {
        _vp_active = 7;
        _current_vp.base = _vp_fire.base;
        _current_vp.trigo = _vp_fire.trigo;

        if ( !(_soundFlags & 2) )
        {
            if ( _oflags & BACT_OFLAG_USERINPT )
            {
                yw_arg180 v42;
                v42.effects_type = 3;
                _world->ypaworld_func180(&v42);
            }

            _soundFlags |= 2;
            SFXEngine::SFXe.startSound(&_soundcarrier, 1);
        }
        result = 1;
    }

    if ( arg->setFlags == BACT_STFLAG_DEATH2 )
    {
        _status = BACT_STATUS_DEAD;

        if ( _vp_active != 3 )
        {
            _current_vp.base = _vp_megadeth.base;
            _current_vp.trigo = _vp_megadeth.trigo;
            _vp_active = 3;

            if ( _soundFlags & 2 )
            {
                if ( _oflags & BACT_OFLAG_USERINPT )
                {
                    yw_arg180 v44;
                    v44.effects_type = 4;
                    _world->ypaworld_func180(&v44);
                }

                SFXEngine::SFXe.sub_424000(&_soundcarrier, 1);
                _soundFlags &= ~2;
            }

            if ( _oflags & BACT_OFLAG_USERINPT )
                SFXEngine::SFXe.sub_424000(&_soundcarrier, 8);

            if ( _soundFlags & 1 )
            {
                _soundFlags &= ~2;
                SFXEngine::SFXe.sub_424000(&_soundcarrier, 0);
            }

            if ( _soundFlags & 8 )
            {
                _soundFlags &= ~8;
                SFXEngine::SFXe.sub_424000(&_soundcarrier, 3);
            }

            if ( _soundFlags & 4 )
            {
                _soundFlags &= ~4;
                SFXEngine::SFXe.sub_424000(&_soundcarrier, 2);
            }

            if ( _soundFlags & 0x80 )
            {
                _soundFlags &= ~0x80;
                SFXEngine::SFXe.sub_424000(&_soundcarrier, 7);
            }

            SFXEngine::SFXe.startSound(&_soundcarrier, 4);

            StartDestFX(2);

            _fly_dir_length = 0;

            result = 1;
        }
    }
    return result;
}

void NC_STACK_ypabact::ChangeSectorEnergy(yw_arg129 *arg)
{
    arg->field_14 = 255;

    _world->ypaworld_func129(arg);

    yw_130arg arg130;
    arg130.pos_x = arg->pos.x;
    arg130.pos_z = arg->pos.z;

    int v5;

    if ( _world->ypaworld_func130(&arg130) )
        v5 = arg130.pcell->owner;
    else
        v5 = 0;

    if ( _world->isNetGame )
    {
        uamessage_sectorEnergy seMsg;
        seMsg.msgID = UAMSG_SECTORENERGY;
        seMsg.owner = _owner;
        seMsg.pos = arg->pos;
        seMsg.energy = arg->field_10;
        seMsg.sectOwner = v5;

        if ( arg->unit )
            seMsg.whoHit = arg->unit->_gid;
        else
            seMsg.whoHit = 0;

        yw_arg181 arg181;
        arg181.recvID = 0;
        arg181.recvFlags = 2;
        arg181.data = &seMsg;
        arg181.dataSize = sizeof(seMsg);
        arg181.garant = 1;

        _world->ypaworld_func181(&arg181);
    }
}

void sb_0x4874c4(NC_STACK_ypabact *bact, int a2, int a3, float a4)
{
    if (a2 == 0)
        a2 = 1;

    bact->_vp_extra[0].scale = sqrt( (float)bact->_scale_time / (float)a2 ) * a4;

    if ( bact->_vp_extra[0].scale < 0.0 )
        bact->_vp_extra[0].scale = 0;

    bact->_vp_extra[0].rotate = mat3x3::RotateY(bact->_maxrot * 2.0 * (float)a3 * 0.001) * bact->_vp_extra[0].rotate;
}

void NC_STACK_ypabact::DeadTimeUpdate(update_msg *arg)
{
    if ( _status_flg & BACT_STFLAG_LAND || (_clock - _dead_time > 5000 && _status_flg & BACT_STFLAG_DEATH1 ) )
    {
        if ( !(_status_flg & BACT_STFLAG_DEATH2) )
        {
            setState_msg arg78;
            arg78.newStatus = BACT_STATUS_NOPE;
            arg78.unsetFlags = 0;
            arg78.setFlags = BACT_STFLAG_DEATH2;

            SetState(&arg78);
        }

        _status_flg |= BACT_STFLAG_LAND;

        if ( _owner && _bact_type != BACT_TYPES_MISSLE && _vp_genesis.base )
        {
            int a2 = _energy_max * 0.7;

            if ( a2 < 10000 )
                a2 = 10000;

            if ( a2 > 25000 )
                a2 = 25000;

            if ( _vp_extra[0].flags & EVPROTO_FLAG_ACTIVE )
            {
                _scale_time -= arg->frameTime;

                if ( _scale_time <= 0 )
                {
                    _vp_extra[0].vp.trigo = NULL;
                    _vp_extra[0].vp.base = NULL;

                    if ( _yls_time <= 0 )
                    {

                        if ( _oflags & BACT_OFLAG_USERINPT )
                            _status_flg |= BACT_STFLAG_NORENDER;
                        else
                            Release();

                    }
                }
                else
                {
                    sb_0x4874c4(this, a2, arg->frameTime, 0.75);

                    if ( _yls_time <= 0 )
                        _status_flg |= BACT_STFLAG_NORENDER;
                }
            }
            else
            {
                _scale_time = a2;
                _vp_extra[0].scale = 0.75;
                _vp_extra[0].pos = _position;
                _vp_extra[0].rotate = _rotation;
                _vp_extra[0].vp = _vp_genesis;
                _vp_extra[0].flags |= (EVPROTO_FLAG_ACTIVE | EVPROTO_FLAG_SCALE);

                if ( _world->isNetGame )
                {
                    uamessage_startPlasma splMsg;
                    splMsg.msgID = UAMSG_STARTPLASMA;
                    splMsg.owner = _owner;
                    splMsg.scale = 0.75;
                    splMsg.time = a2;
                    splMsg.id = _gid;
                    splMsg.pos = _position;
                    splMsg.dir = _rotation;

                    yw_arg181 arg181;
                    arg181.data = &splMsg;
                    arg181.dataSize = sizeof(splMsg);
                    arg181.recvID = 0;
                    arg181.recvFlags = 2;
                    arg181.garant = 1;

                    _world->ypaworld_func181(&arg181);
                }
            }
        }
        else if ( _yls_time <= 0 )
        {
            if ( _oflags & BACT_OFLAG_USERINPT )
                _status_flg |= BACT_STFLAG_NORENDER;
            else
                Release();
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
    float ftime = upd->frameTime * 0.001;

    if ( 0.001 * (upd->gTime - _lastFrmStamp) > 0.0 )
    {
        // Interpolate rotation
        _rotation += _netDRot * ftime;

        vec3d axis = _rotation.AxisX();

        if (axis.normalise() > 0.0001)
            _rotation.SetX( axis );
        else
            _rotation.SetX( vec3d::OX(1.0) );

        axis = _rotation.AxisY();

        if (axis.normalise() > 0.0001)
            _rotation.SetY( axis );
        else
            _rotation.SetY( vec3d::OY(1.0) );

        // Get "90 - angle" between interpolated X and Y
        float as = C_PI_2 - clp_acos( _rotation.AxisX().dot( _rotation.AxisY() ));

        // Calculate correction axis
        vec3d axs = _rotation.AxisX() * _rotation.AxisY();

        // FIX MY MATH ?
        // axs must be 1.0 normalised?

        // Rotate Y axis for 90" between X and Y
        vec3d newY = mat3x3::AxisAngle(axs, -as).Transform( _rotation.AxisY() );

        _rotation.SetY( newY );

        _rotation.SetZ( _rotation.AxisX() * _rotation.AxisY() );

        _position += _fly_dir * (_fly_dir_length * ftime * 6.0);

        CorrectPositionInLevelBox(NULL);
    }
}

void NC_STACK_ypabact::ypabact_func123(update_msg *upd)
{
    float ftime = upd->frameTime * 0.001;
    float stupd = (upd->gTime - _lastFrmStamp) * 0.001;

    if ( stupd > 0.0 )
    {
        _rotation += _netDRot * ftime;

        vec3d axis = _rotation.AxisX();

        if (axis.normalise() > 0.0001)
            _rotation.SetX( axis );
        else
            _rotation.SetX( vec3d::OX(1.0) );

        axis = _rotation.AxisY();

        if (axis.normalise() > 0.0001)
            _rotation.SetY( axis );
        else
            _rotation.SetY( vec3d::OY(1.0) );

        axis = _rotation.AxisZ();

        if (axis.normalise() > 0.0001)
            _rotation.SetZ( axis );
        else
            _rotation.SetZ( vec3d::OZ(1.0) );

        vec3d spd = _fly_dir * _fly_dir_length + _netDSpeed * stupd;

        bool hgun = false;
        if (_bact_type == BACT_TYPES_GUN)
        {
            NC_STACK_ypagun *guno = dynamic_cast<NC_STACK_ypagun *>(this);
            if (guno)
            {
                hgun = guno->IsRoboGun();
            }
        }

        if (_bact_type != BACT_TYPES_GUN || hgun)
            _position = spd * ftime * 6.0;

        CorrectPositionInLevelBox(NULL);

        if ( _status_flg & BACT_STFLAG_LAND )
        {
            ypaworld_arg136 arg136;
            arg136.stPos = _position;
            arg136.vect = _rotation.AxisY() * 200.0;
            arg136.flags = 0;

            _world->ypaworld_func136(&arg136);

            if ( arg136.isect )
                _position = arg136.isectPos - _rotation.AxisY() * _overeof;
        }
    }
}

size_t NC_STACK_ypabact::PathFinder(bact_arg124 *arg)
{
    //path find for ground units (tank & car)
    int maxsteps = arg->steps_cnt;

    for (int xx = 0; xx < _world->_mapWidth; xx++)
    {
        for (int yy = 0; yy < _world->_mapHeight; yy++)
        {
            cellArea &cll = _world->_cells[xx + yy * _world->_mapWidth];

            cll.pf_flags = 0;
            cll.cost_to_this = 0;
            cll.cost_to_target = 0;
            cll.pf_treeup= NULL;
        }
    }

    int from_sec_x = arg->from.x / 1200.0;
    int from_sec_z = -arg->from.y / 1200.0;
    int to_sec_x = arg->to.x / 1200.0;
    int to_sec_z = -arg->to.y / 1200.0;

    cellArea *target_pcell = NULL;

    if ( to_sec_x >= 0 && to_sec_z >= 0 && to_sec_x < _world->_mapWidth && to_sec_z < _world->_mapHeight )
    {
        target_pcell = &_world->_cells[to_sec_x + to_sec_z * _world->_mapWidth];
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

    std::list<cellArea *> openList;

    cellArea *start_pcell = NULL;

    if ( from_sec_x >= 0 && from_sec_z >= 0 && from_sec_x < _world->_mapWidth && from_sec_z < _world->_mapHeight )
    {
        start_pcell = &_world->_cells[from_sec_x + from_sec_z * _world->_mapWidth];
        start_pcell->pos_x = from_sec_x;
        start_pcell->pos_y = from_sec_z;
    }

    start_pcell->pf_flags |= CELL_PFLAGS_IN_CLST;

    int current_sec_x = from_sec_x;
    int current_sec_z = from_sec_z;

    cellArea *current_pcell = start_pcell;

    start_pcell->cost_to_this = 0;

    int v23 = abs(to_sec_x - from_sec_x);
    int v24 = abs(to_sec_z - from_sec_z);

    float sq2 = sqrt(2.0);

    start_pcell->cost_to_target = MMIN(v23, v24) * sq2 + abs(v23 - v24);

    while ( 1 )
    {

        for(int dx = -1; dx <= 1; dx++)
        {
            for(int dz = -1; dz <= 1; dz++)
            {
                if ( dx == 0.0 && dz == 0.0 )
                    continue;

                int t_x = current_sec_x + dx;
                int t_z = current_sec_z + dz;

                if ( t_x > 0 && t_x < _secMaxX - 1 && t_z > 0 && t_z < _secMaxY - 1 )
                {
                    cellArea *cell_tzx = NULL;
                    if ( t_x >= 0 && t_z >= 0 && t_x < _world->_mapWidth && t_z < _world->_mapHeight )
                    {
                        cell_tzx = &_world->_cells[_world->_mapWidth * t_z + t_x];
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
                        subSec *v33 = _world->secTypes[ cell_tzx->type_id ].buildings[0][0];
                        int hlth = v33->health_models[ _world->build_hp_ref[ cell_tzx->buildings_health[0][0] ] ];

                        if (_world->legos[hlth].selected_sklt_intern != _world->legos[hlth].sklt_obj_intern)
                            continue;
                    }

                    if ( dx != 0 && dz != 0)
                    {
                        cellArea *cell_tz = NULL;

                        if (current_sec_x >= 0 && t_z >= 0 && current_sec_x < _world->_mapWidth && t_z < _world->_mapHeight)
                        {
                            cell_tz = &_world->_cells[current_sec_x + _world->_mapWidth * t_z];
                            cell_tz->pos_x = current_sec_x;
                            cell_tz->pos_y = t_z;
                        }

                        cellArea *cell_tx = NULL;

                        if (t_x >= 0 && current_sec_z >= 0 && t_x < _world->_mapWidth && current_sec_z < _world->_mapHeight)
                        {
                            cell_tx = &_world->_cells[current_sec_z * _world->_mapWidth + t_x];
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
                        openList.push_back(cell_tzx);

                    cell_tzx->pf_treeup = current_pcell;
                    cell_tzx->pf_flags |= CELL_PFLAGS_IN_OLST;
                }
            }
        }



        if ( openList.empty() )
        {
            arg->steps_cnt = 0;
            return 0;
        }

        std::list<cellArea *>::iterator it = openList.begin();
        
        std::list<cellArea *>::iterator selected = it;
        float selected_value = (*selected)->cost_to_this + (*selected)->cost_to_target;
        
        for(it++; it != openList.end(); it++)
        {
            float v49 = (*it)->cost_to_this + (*it)->cost_to_target;

            if ( v49 < selected_value )
            {
                selected = it;
                selected_value = v49;
            }
        }
        
        current_pcell = *selected;
        current_sec_x = current_pcell->pos_x;
        current_sec_z = current_pcell->pos_y;

        openList.erase(selected); // Remove OLIST

        current_pcell->pf_flags &= ~CELL_PFLAGS_IN_OLST;
        current_pcell->pf_flags |= CELL_PFLAGS_IN_CLST;

        if ( current_sec_x == to_sec_x && current_sec_z == to_sec_z )
            break;
    }

    std::stack<cellArea *> pathCells;

    cellArea *iter_cell = NULL;

    if ( to_sec_x >= 0 && to_sec_z >= 0 && to_sec_x < _world->_mapWidth && to_sec_z < _world->_mapHeight )
    {
        iter_cell = &_world->_cells[to_sec_x + _world->_mapWidth * to_sec_z];
        iter_cell->pos_x = to_sec_x;
        iter_cell->pos_y = to_sec_z;
    }

    while(iter_cell)
    {
        pathCells.push(iter_cell);
        iter_cell = iter_cell->pf_treeup;
    }

    cellArea *curcell = pathCells.top();
    pathCells.pop();
    
    cellArea *nextcell = pathCells.top();

    int v61 = nextcell->pos_x - curcell->pos_x;
    int v62 = nextcell->pos_y - curcell->pos_y;

    int step_id = 0;

    while ( !pathCells.empty() )
    {
        if ( maxsteps <= 1 || nextcell == target_pcell)
        {
            arg->waypoints[ step_id ].x = arg->to.x;
            arg->waypoints[ step_id ].z = arg->to.y;
            break;
        }

        curcell = nextcell;
        
        pathCells.pop();
        nextcell = pathCells.top();

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

    arg->steps_cnt = step_id + 1;
    return 1;
}

void SetPath__sub0(NC_STACK_ypabact *bact, int a2)
{
    for (NC_STACK_ypabact* &kidunit : bact->_kidList)
    {
        kidunit->_waypoints_count = bact->_waypoints_count;
        kidunit->_current_waypoint = a2;

        kidunit->_status_flg |= BACT_STFLAG_WAYPOINT;

        if ( bact->_status_flg & BACT_STFLAG_WAYPOINTCCL )
            kidunit->_status_flg |= BACT_STFLAG_WAYPOINTCCL;
        else
            kidunit->_status_flg &= ~BACT_STFLAG_WAYPOINTCCL;

        for (int i = 0; i < 32; i++)
        {
            kidunit->_waypoints[i] = bact->_waypoints[i];
        }
    }
}

size_t NC_STACK_ypabact::SetPath(bact_arg124 *arg)
{
    // path find caller for ground squads
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
            _waypoints[i] = arg->waypoints[i];
        }

        _status_flg |= BACT_STFLAG_WAYPOINT;

        _current_waypoint = 0;
        _waypoints_count = arg->steps_cnt;

        SetPath__sub0(this, 0);

        arg67.tgt_pos.x = arg->waypoints[0].x;
        arg67.tgt_pos.z = arg->waypoints[0].z;
    }

    arg67.tgt_type = BACT_TGT_TYPE_CELL;
    arg67.priority = 0;
    SetTarget(&arg67);

    for (NC_STACK_ypabact* &kidunit : _kidList)
    {
        if ( (kidunit->_bact_type == BACT_TYPES_CAR || kidunit->_bact_type == BACT_TYPES_TANK) && _pSector != kidunit->_pSector )
        {
            bact_arg124 arg125;
            arg125.steps_cnt = maxsteps;
            arg125.from = kidunit->_position.XZ();
            arg125.to = arg->to;
            arg125.field_12 = arg->field_12;

            kidunit->SetPath(&arg125);
        }
    }

    return 1;
}



void NC_STACK_ypabact::setBACT_viewer(int vwr)
{
    uamessage_viewer viewMsg;

    if ( vwr )
    {
        if (_world->current_bact)
        {
            if ( _world->current_bact->_bact_type != BACT_TYPES_MISSLE )
                _salve_counter = 0;
        }

        _world->ypaworld_func131(this); //Set current bact

        _oflags |= BACT_OFLAG_VIEWER;

        if ( _world->isNetGame )
            viewMsg.view = 1;

        if ( _bact_type == BACT_TYPES_BACT && !(_status_flg & BACT_STFLAG_LAND) && _status == BACT_STATUS_NORMAL )
            _thraction = _force;

        SFXEngine::SFXe.startSound(&_soundcarrier, 8);
    }
    else
    {
        _oflags &= ~BACT_OFLAG_VIEWER;

        if ( _world->isNetGame )
            viewMsg.view = 0;

        SFXEngine::SFXe.sub_424000(&_soundcarrier, 8);

        if ( _bact_type != BACT_TYPES_MISSLE && _bact_type != BACT_TYPES_ROBO && _status != BACT_STATUS_DEAD )
        {
            if ( _host_station == _parent )
            {
                if ( !(_status_flg & BACT_STFLAG_WAYPOINT) || !(_status_flg & BACT_STFLAG_WAYPOINTCCL) )
                {
                    for (NC_STACK_ypabact* &node : _kidList)
                        node->CopyTargetOf(this);
                }
            }
            else
            {
                if ( !(_status_flg & BACT_STFLAG_WAYPOINT) || !(_status_flg & BACT_STFLAG_WAYPOINTCCL) )
                    CopyTargetOf(_parent);
            }
        }
    }

    if ( _world->isNetGame ) // Network message send routine?
    {
        viewMsg.msgID = UAMSG_VIEWER;
        viewMsg.owner = _owner;
        viewMsg.classID = _bact_type;
        viewMsg.id = _gid;

        if ( viewMsg.classID == BACT_TYPES_MISSLE )
        {
            NC_STACK_ypamissile *miss = dynamic_cast<NC_STACK_ypamissile *>(this);
            viewMsg.launcher = miss->getMISS_launcher()->_gid;
        }

        yw_arg181 v13;
        v13.recvID = 0;
        v13.recvFlags = 2;
        v13.data = &viewMsg;
        v13.garant = 1;
        v13.dataSize = sizeof(viewMsg);

        _world->ypaworld_func181(&v13);
    }
}

void NC_STACK_ypabact::setBACT_inputting(int inpt)
{
    if ( inpt )
    {
        _oflags |= BACT_OFLAG_USERINPT;
        _world->setYW_userVehicle(this);

        if ( _bact_type != BACT_TYPES_GUN )
            CorrectPositionOnLand();
    }
    else
    {
        _oflags &= ~BACT_OFLAG_USERINPT;
    }
}

void NC_STACK_ypabact::setBACT_exactCollisions(int col)
{
    if ( col )
        _oflags |= BACT_OFLAG_EXACTCOLL;
    else
        _oflags &= ~BACT_OFLAG_EXACTCOLL;
}

void NC_STACK_ypabact::setBACT_bactCollisions(int col)
{
    if ( col )
        _oflags |= BACT_OFLAG_BACTCOLL;
    else
        _oflags &= ~BACT_OFLAG_BACTCOLL;
}

void NC_STACK_ypabact::setBACT_airconst(int air)
{
    _airconst = air;
    _airconst_static = air;
}

void NC_STACK_ypabact::setBACT_landingOnWait(int lnding)
{
    if ( lnding )
        _oflags |= BACT_OFLAG_LANDONWAIT;
    else
        _oflags &= ~BACT_OFLAG_LANDONWAIT;
}

void NC_STACK_ypabact::setBACT_yourLastSeconds(int ls)
{
    _yls_time = ls;
}

void NC_STACK_ypabact::setBACT_visProto(NC_STACK_base *vp)
{
    _current_vp.base = vp;
}

void NC_STACK_ypabact::setBACT_aggression(int aggr)
{
    _aggr = aggr;
    
    for (NC_STACK_ypabact* &nod : _kidList)
        nod->_aggr = aggr;
}

void NC_STACK_ypabact::setBACT_vpTransform(TFEngine::TForm3D *tr)
{
    _current_vp.trigo = tr;
}

void NC_STACK_ypabact::setBACT_extraViewer(int vwr)
{
    if ( vwr )
        _oflags |= BACT_OFLAG_EXTRAVIEW;
    else
        _oflags &= ~BACT_OFLAG_EXTRAVIEW;
}

void NC_STACK_ypabact::setBACT_alwaysRender(int rndr)
{
    if ( rndr )
        _oflags |= BACT_OFLAG_ALWAYSREND;
    else
        _oflags &= ~BACT_OFLAG_ALWAYSREND;
}



NC_STACK_ypaworld *NC_STACK_ypabact::getBACT_pWorld()
{
    return _world;
}

TFEngine::TForm3D *NC_STACK_ypabact::getBACT_pTransform()
{
    return &_tForm;
}

int NC_STACK_ypabact::getBACT_viewer()
{
    return (_oflags & BACT_OFLAG_VIEWER) != 0;
}

int NC_STACK_ypabact::getBACT_inputting()
{
    return (_oflags & BACT_OFLAG_USERINPT) != 0;
}

int NC_STACK_ypabact::getBACT_exactCollisions()
{
    return (_oflags & BACT_OFLAG_EXACTCOLL) != 0;
}

int NC_STACK_ypabact::getBACT_bactCollisions()
{
    return (_oflags & BACT_OFLAG_BACTCOLL) != 0;
}


int NC_STACK_ypabact::getBACT_landingOnWait()
{
    return (_oflags & BACT_OFLAG_LANDONWAIT) != 0;
}

int NC_STACK_ypabact::getBACT_yourLastSeconds()
{
    return _yls_time;
}

NC_STACK_base *NC_STACK_ypabact::getBACT_visProto()
{
    return _current_vp.base;
}

int NC_STACK_ypabact::getBACT_aggression()
{
    return _aggr;
}

rbcolls *NC_STACK_ypabact::getBACT_collNodes()
{
    return NULL;
}

TFEngine::TForm3D *NC_STACK_ypabact::getBACT_vpTransform()
{
    return _current_vp.trigo;
}

int NC_STACK_ypabact::getBACT_extraViewer()
{
    return (_oflags & BACT_OFLAG_EXTRAVIEW) != 0;
}

int NC_STACK_ypabact::getBACT_alwaysRender()
{
    return (_oflags & BACT_OFLAG_ALWAYSREND) != 0;
}


bool NC_STACK_ypabact::IsNeedsWaypoints( NC_STACK_ypabact *bact)
{
    if (bact->IsGroundUnit())
        return true;
    
    for (NC_STACK_ypabact* &unit : bact->_kidList)
    {
        if (unit->IsGroundUnit())
            return true;
    }
    
    return false;
}

void NC_STACK_ypabact::CleanAttackersTarget()
{
    for(World::BactList::iterator it = _attackersList.begin();
        it != _attackersList.end();
        it = _attackersList.erase(it))
    {
        NC_STACK_ypabact *attacker = *it;

        if ( attacker->_primTtype == BACT_TGT_TYPE_UNIT &&
             attacker->_primT.pbact == this )
        {
            attacker->_primT.pbact = NULL;
            attacker->_primTtype = BACT_TGT_TYPE_NONE;
            attacker->_assess_time = 0;
        }
        
        if ( attacker->_secndTtype == BACT_TGT_TYPE_UNIT && 
             attacker->_secndT.pbact == this )
        {
            attacker->_secndT.pbact = NULL;
            attacker->_secndTtype = BACT_TGT_TYPE_NONE;
            attacker->_assess_time = 0;
        }
    }
}

bool NC_STACK_ypabact::IsParentMyRobo() const
{
    return (_host_station) && (_parent) && (_host_station == _parent);
}
