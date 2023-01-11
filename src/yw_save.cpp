#include <inttypes.h>
#include <string.h>
#include "includes.h"
#include "yw_internal.h"
#include "yw.h"
#include "input.h"
#include "yparobo.h"


int yw_write_callSign(NC_STACK_ypaworld *yw, const char *filename, const char *callsign)
{
    UserData *usr = yw->_GameShell;

    if ( !usr )
        return 0;

    if ( usr->UserName.empty() )
        return 0;

    FSMgr::FileHandle *fil = uaOpenFileAlloc( fmt::sprintf("save:%s/%s", usr->UserName, filename), "w");

    if ( !fil )
        return 0;

    fil->printf("%s", callsign);

    delete fil;

    return 1;
}

int yw_write_user(FSMgr::FileHandle *fil, UserData *usr)
{
    NC_STACK_ypaworld *yw = usr->p_YW;

    if ( usr->netPlayerName[0] >= ' ' )
        yw_write_callSign(yw, "callsign.def", usr->netPlayerName.c_str());

    fil->printf("new_user\n");

    for (int i = 0; i < 8; i++)
    {
        fil->printf("    playerstatus = %d_%d_%d_%d_%d_%d_%d_%d\n",
                i,
                yw->_playersStats[i].DestroyedUnits,
                yw->_playersStats[i].DestroyedByUser,
                yw->_playersStats[i].ElapsedTime,
                yw->_playersStats[i].SectorsTaked,
                yw->_playersStats[i].Score,
                yw->_playersStats[i].Power,
                yw->_playersStats[i].Upgrades);
    }

    fil->printf("    maxroboenergy = %d\n", usr->p_YW->_maxRoboEnergy);
    fil->printf("    maxreloadconst = %d\n", usr->p_YW->_maxReloadConst);
    fil->printf("    numbuddies    = %d\n", 128);
    fil->printf("    beamenergy    = %d\n", usr->p_YW->_beamEnergyCapacity);

    std::string jodie = "    jodiefoster   = ";

    for (int i = 0; i < 8; i++)
    {
        jodie += fmt::sprintf("%d", usr->p_YW->_levelInfo.JodieFoster[i]);

        if ( i < 7 )
            jodie += "_";
    }
    
    fil->printf(jodie);

    fil->printf("      ; the contact flags\n");
    fil->printf("end\n\n");

    return 1;
}

int yw_write_input(FSMgr::FileHandle *fil, UserData *usr)
{
    fil->printf("new_input\n");

    if ( usr->p_YW->_preferences & World::PREF_JOYDISABLE )
        fil->printf("    joystick = no\n");
    else
        fil->printf("    joystick = yes\n");

    if ( usr->altJoystickEnabled )
        fil->printf("    altjoystick = yes\n");
    else
        fil->printf("    altjoystick = no\n");

    if ( usr->p_YW->_preferences & World::PREF_FFDISABLE )
        fil->printf("    forcefeedback = no\n");
    else
        fil->printf("    forcefeedback = yes\n");

    for (size_t i = 1; i < usr->InputConfig.size(); i++)
    {
        if ( usr->InputConfig[i].Type == World::INPUT_BIND_TYPE_BUTTON )
        {
            fil->printf("    input.button[%d] = ", usr->InputConfig[i].KeyID);
            fil->printf("$");

            if ( usr->InputConfig[i].PKeyCode )
                fil->printf(NC_STACK_input::KeyNamesTable.at(usr->InputConfig[i].PKeyCode).Name.c_str());
            else
                fil->printf("nop");
        }
        else if ( usr->InputConfig[i].Type == World::INPUT_BIND_TYPE_SLIDER )
        {
            fil->printf("    input.slider[%d] = ", usr->InputConfig[i].KeyID);

            fil->printf("~#");
            fil->printf("$");

            if ( usr->InputConfig[i].NKeyCode != Input::KC_NONE )
            {
                fil->printf(NC_STACK_input::KeyNamesTable.at(usr->InputConfig[i].NKeyCode).Name.c_str());
            }
            else
            {
                ypa_log_out("Slider(neg) %s is not declared!\n", usr->InputConfigTitle[i].c_str());
                ypa_log_out("Use space-key for it\n");

                fil->printf("space");
            }

            fil->printf("_#");
            fil->printf("$");

            if ( usr->InputConfig[i].PKeyCode != Input::KC_NONE )
            {
                fil->printf(NC_STACK_input::KeyNamesTable.at(usr->InputConfig[i].PKeyCode).Name.c_str());
            }
            else
            {
                ypa_log_out("Slider(pos) %s is not declared!\n", usr->InputConfigTitle[i].c_str());
                ypa_log_out("Use space-key for it\n");

                fil->printf("space");
            }
        }
        else if ( usr->InputConfig[i].Type == World::INPUT_BIND_TYPE_HOTKEY )
        {
            fil->printf("    input.hotkey[%d] = ", usr->InputConfig[i].KeyID);

            if ( usr->InputConfig[i].PKeyCode != Input::KC_NONE )
                fil->printf(NC_STACK_input::KeyNamesTable.at(usr->InputConfig[i].PKeyCode).Name.c_str());
            else
                fil->printf("nop");
        }
        else
            continue;

        fil->printf("         ; ");
        fil->printf(usr->InputConfigTitle[i]);
        fil->printf("\n");
    }

    fil->printf("end\n\n");

    return 1;
}

int yw_write_sound(FSMgr::FileHandle *fil, UserData *usr)
{
    fil->printf("new_sound\n");
    fil->printf("    volume = %d\n", usr->soundVolume);
    fil->printf("    cdvolume = %d\n", usr->musicVolume);

    if ( usr->soundFlags & World::SF_INVERTLR )
        fil->printf("    invertlr = yes\n");
    else
        fil->printf("    invertlr = no\n");

    if ( usr->soundFlags & World::SF_CDSOUND )
        fil->printf("    cdsound = yes\n");
    else
        fil->printf("    cdsound = no\n");

    fil->printf("end\n\n");

    return 1;
}

int yw_write_video(FSMgr::FileHandle *fil, UserData *usr)
{
    fil->printf("new_video\n");
    fil->printf("    videomode = %d\n", ((usr->p_YW->_gameDefaultRes.x << 12) | usr->p_YW->_gameDefaultRes.y) );

    if ( usr->GFXFlags & World::GFX_FLAG_FARVIEW )
        fil->printf("    farview = yes\n");
    else
        fil->printf("    farview = no\n");

    if ( usr->GFXFlags & World::GFX_FLAG_16BITTEXTURE )
        fil->printf("    16bittexture = yes\n");
    else
        fil->printf("    16bittexture = no\n");

    if ( usr->GFXFlags & World::GFX_FLAG_DRAWPRIMITIVES )
        fil->printf("    drawprimitive = yes\n");
    else
        fil->printf("    drawprimitive = no\n");

    if ( usr->GFXFlags & World::GFX_FLAG_SKYRENDER )
        fil->printf("    heaven = yes\n");
    else
        fil->printf("    heaven = no\n");

    if ( usr->GFXFlags & World::GFX_FLAG_SOFTMOUSE )
        fil->printf("    softmouse = yes\n");
    else
        fil->printf("    softmouse = no\n");

    if ( usr->enemyIndicator )
        fil->printf("    enemyindicator = yes\n");
    else
        fil->printf("    enemyindicator = no\n");

    fil->printf("    fxnumber = %d\n", usr->fxnumber);
    
    if (usr->IsWindowedFlag())
        fil->printf("    ;#!gfxmode = %d_%d_1\n", usr->p_YW->_gfxMode.w, usr->p_YW->_gfxMode.h);
    else
        fil->printf("    ;#!gfxmode = %d_%d_0\n", usr->p_YW->_gfxMode.w, usr->p_YW->_gfxMode.h);
    
    fil->printf("end\n\n");

    return 1;
}

int yw_write_level_status(FSMgr::FileHandle *fil, NC_STACK_ypaworld *yw, int lvlid)
{
    //sprintf(buf, "\nbegin_levelstatus %ld\n", lvlid);
    fil->printf("\nbegin_levelstatus %d\n", lvlid);

    //sprintf(buf, "    status = %ld\n", yw->LevelNet->mapInfos[lvlid].field_0);
    fil->printf("    status = %d\n", yw->_globalMapRegions.MapRegions[lvlid].Status);

    fil->printf("end\n\n");

    return 1;
}

int yw_write_levels_statuses(FSMgr::FileHandle *fil, NC_STACK_ypaworld *yw)
{
    for (size_t i = 0; i < yw->_globalMapRegions.MapRegions.size(); i++)
    {
        if ( yw->_globalMapRegions.MapRegions[ i ].Status != TMapRegionInfo::STATUS_NONE )
        {
            if ( !yw_write_level_status(fil, yw, i))
                return 0;
        }
    }
    return 1;
}

int yw_write_buddies(FSMgr::FileHandle *fil, NC_STACK_ypaworld *yw)
{
    for (const TMapBuddy &buddy : yw->_levelInfo.Buddies )
    {
        fil->printf("\nbegin_buddy\n");

        //sprintf(buf, "    commandid = %ld\n", yw->field_2d90->buddies[buddy_id].commandid);
        fil->printf("    commandid = %d\n", buddy.CommandID);

        fil->printf("    type      = %d\n", buddy.Type);

        //sprintf(buf, "    energy    = %ld\n", yw->field_2d90->buddies[buddy_id].energy);
        fil->printf("    energy    = %d\n", buddy.Energy);

        fil->printf("end\n\n");
    }

    return 1;
}

int yw_write_status(TMFWinStatus *status, const char *field, FSMgr::FileHandle *file)
{
    file->printf("    %s = %d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d\n",
            field,
            (status->Valid ? 1 : 0),
            (status->IsOpen ? 1 : 0),
            status->Rect.x,
            status->Rect.y,
            status->Rect.w,
            status->Rect.h,
            status->Data[0],
            status->Data[1],
            status->Data[2],
            status->Data[3],
            status->Data[4],
            status->Data[5],
            status->Data[6],
            status->Data[7]);

    return 1;
}

int yw_write_shell(FSMgr::FileHandle *fil, UserData *usr)
{
    NC_STACK_ypaworld *yw = usr->p_YW;

    fil->printf("new_shell\n");

    if ( usr->default_lang_dll )
        fil->printf("    language = %s\n", usr->default_lang_dll->c_str());

    if ( yw->_shellConfIsParsed )
    {
        fil->printf("    finder = na_0_0_0_0\n");
        fil->printf("    log    = na_0_0_0_0\n");
        fil->printf("    energy = na_0_0_0_0\n");
        fil->printf("    message = na_0_0_0_0\n");
        fil->printf("    map    = na_0_0_0_0_0_0\n");

        yw_write_status(&yw->_roboMapStatus, "robo_map_status", fil);
        yw_write_status(&yw->_roboFinderStatus, "robo_finder_status", fil);
        yw_write_status(&yw->_vhclMapStatus, "vhcl_map_status", fil);
        yw_write_status(&yw->_vhclFinderStatus, "vhcl_finder_status", fil);
    }

    fil->printf("end\n\n");

    return 1;
}

int yw_write_item_modifers(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    int i = 0;
    for (const World::TVhclProto &proto : yw->_vhclProtos)
    {
        if ( proto.model_id != BACT_TYPES_NOPE )
        {
            fil->printf("modify_vehicle %d\n", i);

            for (int j = 1; j < 8; j++)
            {
                if ( proto.disable_enable_bitmask & (1 << j) )
                    fil->printf("    enable         = %d\n", j);
                else
                    fil->printf("    disable        = %d\n", j);
            }

            fil->printf("    shield         = %d\n", proto.shield);
            fil->printf("    energy         = %d\n", proto.energy);
            fil->printf("    num_weapons    = %d\n", proto.num_weapons);
            fil->printf("    weapon         = %d\n", proto.weapon);
            fil->printf("    radar          = %d\n", proto.radar);
            fil->printf("    fire_x         = %4.2f\n", proto.fire_x);
            fil->printf("    fire_y         = %4.2f\n", proto.fire_y);
            fil->printf("    fire_z         = %4.2f\n", proto.fire_z);
            fil->printf("end\n\n");
        }
        i++;
    }

    i = 0;
    for (const World::TWeapProto &proto : yw->_weaponProtos)
    {
        if ( proto.unitID )
        {
            fil->printf("modify_weapon %d\n", i);

            for (int j = 1; j < 8; j++)
            {
                if ( proto.enable_mask & (1 << j) )
                    fil->printf("    enable         = %d\n", j);
                else
                    fil->printf("    disable        = %d\n", j);
            }

            fil->printf("    shot_time      = %d\n", proto.shot_time);
            fil->printf("    shot_time_user = %d\n", proto.shot_time_user);
            fil->printf("    energy         = %d\n", proto.energy);
            fil->printf("end\n\n");
        }
        
        i++;
    }

    i = 0;
    for (const World::TBuildingProto &proto : yw->_buildProtos )
    {
        if ( proto.TypeIcon )
        {
            fil->printf("modify_building %d\n", i);

            for (int j = 1; j < 8; j++)
            {
                if ( proto.EnableMask & (1 << j) )
                    fil->printf("    enable         = %d\n", j);
                else
                    fil->printf("    disable        = %d\n", j);
            }

            fil->printf("end\n\n");
        }
        i++;
    }

    return 1;
}

int yw_write_levelnum(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    fil->printf("\nbegin_levelnum\n");
    fil->printf("    levelnum = %d\n", yw->_levelInfo.LevelID);
    fil->printf("end\n");

    return 1;
}

void yw_write_map(NC_STACK_ypaworld *yw, const Common::PlaneBytes &map, const std::string &padding, FSMgr::FileHandle *fil)
{
    if ( !padding.empty() )
        fil->printf(padding.c_str());

    fil->printf("%d %d\n", map.Width(), map.Height());

    for (uint32_t y = 0; y < map.Height(); y++)
    {
        if ( !padding.empty() )
            fil->printf(padding.c_str());

        const uint8_t * v6 = map.Line(y);
        
        for (uint32_t x = 0; x < map.Width(); x++ )
        {
            fil->printf("%02x ", *v6);
            v6++;
        }

        fil->printf("\n");
    }
}

void yw_write_ownermap(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    Common::PlaneBytes ownermap = Common::PlaneBytes(yw->_mapSize);

    if ( ownermap.IsOk() )
    {
        for (int y = 0; y < yw->_mapSize.y; y++)
        {
            for (int x = 0; x < yw->_mapSize.x; x++)
                ownermap(x, y) = yw->_cells(x, y).owner;
        }

        fil->printf("\nbegin_ownermap\n");
        yw_write_map(yw, ownermap, "        ", fil);
        fil->printf("end\n");
    }
}

void yw_write_buildmap(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    fil->printf("\nbegin_buildingmap\n");
    yw_write_map(yw, yw->_lvlBuildingsMap, "        ", fil);
    fil->printf("end\n");
}

void yw_write_energymap(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    Common::PlaneBytes energymap = Common::PlaneBytes(yw->_mapSize.x * 3, yw->_mapSize.y * 3);

    if ( energymap.IsOk() )
    {
        for (size_t i = 0; i < yw->_cells.size(); ++i)
        {
            cellArea &cell = yw->_cells.At(i);

            for(size_t j = 0; j < 9; j++)
                energymap( i * 9  + j ) = cell.buildings_health.At(j / 3, j % 3);
        }

        fil->printf("\nbegin_energymap\n");
        yw_write_map(yw, energymap, "        ", fil);
        fil->printf("end\n");
    }
    else
    {
        ypa_log_out("game save error: Unable to create bmo for saving owner\n");
    }
}

int yw_write_bact(NC_STACK_ypabact *bct, FSMgr::FileHandle *fil)
{
    if ( bct->getBACT_viewer() )
        fil->printf("    viewer         = yes\n");
    else
        fil->printf("    viewer         = no\n");
    
    if ( bct->getBACT_inputting() )
        fil->printf("    user           = yes\n");
    else
        fil->printf("    user           = no\n");

    if ( bct->getBACT_bactCollisions() )
        fil->printf("    collision      = yes\n");
    else
        fil->printf("    collision      = no\n");

    fil->printf("    energy         = %d\n", bct->_energy);
    fil->printf("    speed          = %6.5f_%6.5f_%6.5f_%6.5f\n", bct->_fly_dir.x, bct->_fly_dir.y, bct->_fly_dir.z, bct->_fly_dir_length);

    fil->printf(
        "    matrix         = %6.5f_%6.5f_%6.5f_%6.5f_%6.5f_%6.5f_%6.5f_%6.5f_%6.5f\n",
        bct->_rotation.m00,
        bct->_rotation.m01,
        bct->_rotation.m02,
        bct->_rotation.m10,
        bct->_rotation.m11,
        bct->_rotation.m12,
        bct->_rotation.m20,
        bct->_rotation.m21,
        bct->_rotation.m22);

    if ( bct->_bact_type == BACT_TYPES_ROBO )
    {
        NC_STACK_yparobo *roboo = dynamic_cast<NC_STACK_yparobo *>(bct);
        fil->printf(
            "    pos            = %2.2f_%2.2f_%2.2f\n",
            bct->_position.x,
            roboo->_roboYPos,
            bct->_position.z);
    }
    else
        fil->printf(
            "    pos            = %2.2f_%2.2f_%2.2f\n",
            bct->_position.x,
            bct->_position.y,
            bct->_position.z);

    fil->printf("    force          = %2.2f\n", bct->_thraction);
    fil->printf("    gunangle       = %5.4f\n", bct->_gun_angle_user);
    fil->printf("    commandID      = %d\n", (bct->_commandID & 0xFFFFFF));
    fil->printf("    aggression     = %d\n", bct->_aggr);
    fil->printf("    mainstate      = %d\n", bct->_status);
    fil->printf("    extrastate     = %d\n", bct->_status_flg);
    fil->printf("    ident          = %d\n", bct->_gid);
    fil->printf("    killerowner    = %d\n", bct->_killer_owner);

    if ( bct->_primTtype == BACT_TGT_TYPE_UNIT )
        fil->printf("    primary        = %d_%d_%2.2f_%2.2f_%d\n", bct->_primTtype, bct->_primT.pbact->_gid, bct->_primTpos.x, bct->_primTpos.z, bct->_primT_cmdID);
    else
        fil->printf("    primary        = %d_0_%2.2f_%2.2f_%d\n", bct->_primTtype, bct->_primTpos.x, bct->_primTpos.z, bct->_primT_cmdID);

    for (int i = 0; i < bct->_waypoints_count; i++)
        fil->printf("    waypoint       = %d_%2.2f_%2.2f\n", i, bct->_waypoints[i].x, bct->_waypoints[i].z);

    fil->printf("    num_wp         = %d\n", bct->_waypoints_count);
    fil->printf("    count_wp       = %d\n", bct->_current_waypoint);

    return 1;
}

bool NC_STACK_ypaworld::yw_write_robo(NC_STACK_yparobo *robo, FSMgr::FileHandle *fil)
{
    fil->printf("\nbegin_robo %d\n", robo->_vehicleID);

    const char *isuser = "no";

    if (robo == _userRobo)
        isuser = "yes";

    fil->printf("    is_user_robo   = %s\n", isuser);

    if ( !yw_write_bact(robo, fil) )
        return false;

    fil->printf("    owner          = %d\n", robo->_owner);
    fil->printf("    robostate      = %d\n", robo->_roboState & 0xC00F );
    fil->printf("    dockenergy     = %d\n", robo->_roboDockEnerg);
    fil->printf("    dockcount      = %d\n", robo->_roboDockCnt);
    fil->printf("    dockuser       = %d\n", robo->_roboDockUser);
    fil->printf("    docktime       = %d\n", robo->_roboDockTime);
    fil->printf("    docktargetpos  = %2.2f_%2.2f\n", robo->_roboDockTargetPos.x, robo->_roboDockTargetPos.z);
    fil->printf("    docktargetID   = %d\n", robo->_roboDockTargetCommandID);
    fil->printf("    docktargettype = %d\n", robo->_roboDockTargetType);
    fil->printf("    dockaggr       = %d\n", robo->_roboDockAggr);
    fil->printf("    battvehicle    = %d\n", robo->_roboEnergyLife);
    fil->printf("    battbeam       = %d\n", robo->_roboEnergyMove);
    fil->printf("    fillmodus      = %d\n", robo->_roboFillMode);
    fil->printf("    maximum        = %d\n", robo->_energy_max);
    fil->printf("    buildspare     = %d\n", robo->_roboBuildSpare);
    fil->printf("    vhoriz         = %7.5f\n", robo->_viewer_horiz_angle);
    fil->printf("    vvert          = %7.5f\n", robo->_viewer_vert_angle);
    fil->printf("    con_budget     = %d\n", robo->_roboEpConquer);
    fil->printf("    def_budget     = %d\n", robo->_roboEpDefense);
    fil->printf("    rec_budget     = %d\n", robo->_roboEpRecon);
    fil->printf("    rob_budget     = %d\n", robo->_roboEpRobo);
    fil->printf("    rad_budget     = %d\n", robo->_roboEpRadar);
    fil->printf("    pow_budget     = %d\n", robo->_roboEpPower);
    fil->printf("    saf_budget     = %d\n", robo->_roboEpSafety);
    fil->printf("    cpl_budget     = %d\n", robo->_roboEpChangePlace);
    fil->printf("    saf_delay     = %d\n", robo->_roboSafetyDelay);
    fil->printf("    pow_delay     = %d\n", robo->_roboPowerDelay);
    fil->printf("    rad_delay     = %d\n", robo->_roboRadarDelay);
    fil->printf("    cpl_delay     = %d\n", robo->_roboPositionDelay);
    fil->printf("    def_delay     = %d\n", robo->_roboEnemyDelay);
    fil->printf("    con_delay     = %d\n", robo->_roboConqDelay);
    fil->printf("    rec_delay     = %d\n", robo->_roboExploreDelay);
    fil->printf("    rob_delay     = %d\n", robo->_roboDangerDelay);
    fil->printf("    reload_const  = %d\n", robo->_reload_const);
    fil->printf("end\n\n");
    return true;
}

int yw_write_gun(NC_STACK_ypabact *bct, FSMgr::FileHandle *fil)
{
    if ( bct->_bact_type == BACT_TYPES_GUN )
    {
        NC_STACK_ypagun *guno = (NC_STACK_ypagun *)bct;

        fil->printf("    gunbasis = %7.6f_%7.6f_%7.6f\n",
                    guno->_gunBasis.x,
                    guno->_gunBasis.y,
                    guno->_gunBasis.z);
    }
    return 1;
}

int yw_write_commander(NC_STACK_ypabact *bct, FSMgr::FileHandle *fil)
{
    fil->printf("\nbegin_commander %d\n", bct->_vehicleID);

    if ( !yw_write_bact(bct, fil) )
        return 0;

    if ( !yw_write_gun(bct, fil) )
        return 0;

    fil->printf("end\n\n");

    return 1;
}

int yw_write_slave(NC_STACK_ypabact *bct, FSMgr::FileHandle *fil)
{
    fil->printf("\nbegin_slave %d\n", bct->_vehicleID);

    if ( !yw_write_bact(bct, fil) )
        return 0;

    if ( !yw_write_gun(bct, fil) )
        return 0;

    fil->printf("end\n\n");

    return 1;
}

int yw_write_extraviewer(NC_STACK_ypabact *bct, FSMgr::FileHandle *fil)
{
    if ( bct->_bact_type == BACT_TYPES_GUN )
    {
        int v7 = -1;
        
        fil->printf("\nbegin_extraviewer\n");

        int i = 0;
        for (World::TRoboGun &gun : bct->_host_station->_roboGuns)
        {
            if ( gun.gun_obj == bct )
            {
                v7 = i;
                break;
            }
            
            i++;
        }

        if ( v7 >= 0 )
        {
            fil->printf("    kind = robogun\n");
            fil->printf("    number = %d\n", v7);
            fil->printf("end\n\n");
            return 1;
        }

        ypa_log_out("Error: Gun not found in yrd!\n");
    }
    return 0;
}

bool NC_STACK_ypaworld::yw_write_units(FSMgr::FileHandle *fil)
{
    for ( World::RefBactList::reverse_iterator staIt = _unitsList.rbegin(); staIt != _unitsList.rend(); staIt++ )
    {
        NC_STACK_ypabact *station = *staIt;
        
        if ( station->_status != BACT_STATUS_DEAD )
        {
            if ( !yw_write_robo((NC_STACK_yparobo *)station, fil) )
                return false;

            for ( World::RefBactList::reverse_iterator comIt = station->_kidList.rbegin(); comIt != station->_kidList.rend(); comIt++ )
            {
                NC_STACK_ypabact *commander = *comIt;
                bool a4 = false;

                if ( commander->_bact_type == BACT_TYPES_GUN )
                {
                    NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( commander );
                    a4 = gun->IsRoboGun();
                }

                if ( !a4 )
                {
                    if ( !yw_write_commander(commander, fil) )
                        return false;
                }
                else
                {
                    if ( commander->getBACT_viewer() )
                    {
                        if ( !yw_write_extraviewer(commander, fil) )
                            return false;
                    }
                }
                
                for ( World::RefBactList::reverse_iterator slvIt = commander->_kidList.rbegin(); slvIt != commander->_kidList.rend(); slvIt++ )
                {
                    NC_STACK_ypabact *slave = *slvIt;
                    bool v9 = false;
                    if ( slave->_bact_type == BACT_TYPES_GUN )
                    {
                        NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( slave );
                        v9 = gun->IsRoboGun();
                    }

                    if ( !v9 )
                    {
                        if ( !yw_write_slave(slave, fil) )
                            return false;
                    }
                    else if ( slave->getBACT_viewer() )
                    {
                        if ( !yw_write_extraviewer(slave, fil) )
                            return false;
                    }
                }
            }
        }
    }

    return true;
}

int yw_write_wunderinfo(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    fil->printf("\nbegin_wunderinfo\n");

    size_t i = 0;
    for ( const TMapGem &gem : yw->_techUpgrades )
    {
        if ( yw->GetSector(gem.CellId)->PurposeType != cellArea::PT_TECHUPGRADE )
            fil->printf("    disablegem %d\n", i);

        i++;
    }

    fil->printf("end\n\n");
    return 1;
}

int yw_write_kwfactor(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    fil->printf("\nbegin_kwfactor\n");

    for (const auto &kw : yw->_powerStations)
    {
        if ( kw.second.pCell )
            fil->printf("    kw = %d_%d_%d\n", kw.second.CellId.x, kw.second.CellId.y, kw.second.EffectivePower);
    }
    
    fil->printf("end\n\n");
    return 1;
}

int yw_write_globals(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    fil->printf("\nbegin_globals\n");
    fil->printf("    time = %d\n", yw->_timeStamp);
    fil->printf("end\n\n");
    return 1;
}

int yw_write_superbomb(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    int i = 0;
    for ( const TMapSuperItem &sitem : yw->_levelInfo.SuperItems)
    {
        fil->printf("\nbegin_superbomb\n");
        fil->printf("    num               = %d\n", i);
        fil->printf("    status            = %d\n", sitem.State);
        fil->printf("    active_timestamp  = %d\n", sitem.ActiveTime);
        fil->printf("    trigger_timestamp = %d\n", sitem.TriggerTime);
        fil->printf("    activated_by      = %d\n", sitem.ActivateOwner);
        fil->printf("    countdown         = %d\n", sitem.CountDown);
        fil->printf("    last_ten_sec      = %d\n", sitem.LastTenSec);
        fil->printf("    last_sec          = %d\n", sitem.LastSec);
        fil->printf("    radius            = %d\n", sitem.CurrentRadius);
        fil->printf("    last_radius       = %d\n", sitem.LastRadius);
        fil->printf("end\n");
        
        i++;
    }

    return 1;
}

int yw_write_histbuffer(const std::vector<uint8_t> &buff, FSMgr::FileHandle *fil)
{
    fil->printf("    history_buffer = \n");
    fil->printf("    64 64\n");

    size_t k = 0;
    for (int i = 0; i < 64; i++)
    {
        fil->printf("    ");

        for (int j = 0; j < 64; j++)
        {
            fil->printf("%02x ", buff[k]);
            k++;
        }

        fil->printf("\n");
    }

    fil->printf("\n");
    return 1;
}

int yw_write_history(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    fil->printf(";------------------------------------------------------------\n");
    fil->printf("; History Buffers\n");
    fil->printf(";------------------------------------------------------------\n");
    fil->printf("begin_history\n");

    std::vector<uint8_t> buf;
    const size_t BuffSize = 64 * 64;

    World::History::Instance HistDecoders;

    auto reader = yw->_history.GetReader();

    bool run = true;
    while ( run && !reader.Eof() )
    {
        World::History::Record *decoder = HistDecoders[ reader.ReadU8() ];

        if (decoder)
        {
            if ( buf.size() + 1 + decoder->dataSize > BuffSize ) // Write buffer
            {
                if (BuffSize - buf.size() > 0)
                    Common::Append(&buf, std::vector<uint8_t>(BuffSize - buf.size(), 0));

                yw_write_histbuffer(buf, fil);

                buf.clear();
            }

            buf.push_back(decoder->type);
            Common::Append(&buf, reader.Read(decoder->dataSize));
        }
        else
        {
            run = false;
        }
    }

    if (buf.size())
    {
        if (BuffSize - buf.size() > 0)
            Common::Append(&buf, std::vector<uint8_t>(BuffSize - buf.size(), 0));

        yw_write_histbuffer(buf, fil);
    }

    fil->printf("end");

    return 1;
}

int yw_write_masks(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    fil->printf("\nbegin_masks\n");
    fil->printf("    ownermask = %d\n", yw->_levelInfo.OwnerMask);
    fil->printf("    usermask  = %d\n", yw->_levelInfo.UserMask);
    fil->printf("end\n");
    return 1;
}
