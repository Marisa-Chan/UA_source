#include <inttypes.h>
#include <string.h>
#include "includes.h"
#include "yw_internal.h"
#include "yw.h"
#include "input.h"
#include "yparobo.h"


int yw_write_callSign(NC_STACK_ypaworld *yw, const char *filename, const char *callsign)
{
    UserData *usr = yw->GameShell;

    if ( !usr )
        return 0;

    if ( usr->user_name.empty() )
        return 0;

    char buf[260];
    sprintf(buf, "save:%s/%s", usr->user_name.c_str(), filename);

    FSMgr::FileHandle *fil = uaOpenFile(buf, "w");

    if ( !fil )
        return 0;

    fil->printf("%s", callsign);

    delete fil;

    return 1;
}

int yw_write_user(FSMgr::FileHandle *fil, UserData *usr)
{
    NC_STACK_ypaworld *yw = usr->p_ypaworld;

    if ( usr->callSIGN[0] >= ' ' )
        yw_write_callSign(yw, "callsign.def", usr->callSIGN.c_str());

    char buf[300];

    sprintf(buf, "new_user\n");
    fil->write(buf, strlen(buf));

    for (int i = 0; i < 8; i++)
    {
        sprintf(buf, "    playerstatus = %d_%d_%d_%d_%d_%d_%d_%d\n",
                i,
                yw->playerstatus[i].destroyed,
                yw->playerstatus[i].destroyedByUser,
                yw->playerstatus[i].elapsedTime,
                yw->playerstatus[i].sectorsTaked,
                yw->playerstatus[i].score,
                yw->playerstatus[i].power,
                yw->playerstatus[i].upgrades);

        fil->write(buf, strlen(buf));
    }

    sprintf(buf, "    maxroboenergy = %d\n", usr->p_ypaworld->_maxRoboEnergy);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    maxreloadconst = %d\n", usr->p_ypaworld->_maxReloadConst);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    numbuddies    = %d\n", 128);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    beamenergy    = %d\n", usr->p_ypaworld->beamenergy);
    fil->write(buf, strlen(buf));


    sprintf(buf, "    jodiefoster   = ");

    for (int i = 0; i < 8; i++)
    {
        char v29[12];

        sprintf(v29, "%d", usr->p_ypaworld->_levelInfo->JodieFoster[i]);
        strcat(buf, v29);

        if ( i < 7 )
        {
            strcat(buf, "_");
        }
    }

    strcat(buf, "      ; the contact flags\n");
    fil->write(buf, strlen(buf));

    sprintf(buf, "end\n\n");
    fil->write(buf, strlen(buf));

    return 1;
}

int yw_write_input(FSMgr::FileHandle *fil, UserData *usr)
{
    char buf[300];

    sprintf(buf, "new_input\n");
    fil->write(buf, strlen(buf));

    if ( usr->p_ypaworld->field_73CE & 4 )
        sprintf(buf, "    joystick = no\n");
    else
        sprintf(buf, "    joystick = yes\n");

    fil->write(buf, strlen(buf));

    if ( usr->inp_altjoystick )
        sprintf(buf, "    altjoystick = yes\n");
    else
        sprintf(buf, "    altjoystick = no\n");

    fil->write(buf, strlen(buf));

    if ( usr->p_ypaworld->field_73CE & 8 )
        sprintf(buf, "    forcefeedback = no\n");
    else
        sprintf(buf, "    forcefeedback = yes\n");

    fil->write(buf, strlen(buf));

    for (int i = 1; i < 46; i++)
    {
        if ( usr->keyConfig[i].inp_type == World::KEYC_TYPE_BUTTON )
        {
            sprintf(buf, "    input.button[%d] = ", usr->keyConfig[i].keyID);

            strcat(buf, "$");

            if ( usr->keyConfig[i].KeyCode )
            {
                strcat(buf, Input::KeysInfo[usr->keyConfig[i].KeyCode]._name.c_str());
            }
            else
            {
                strcat(buf, "nop");
            }
        }
        else if ( usr->keyConfig[i].inp_type == World::KEYC_TYPE_SLIDER )
        {
            sprintf(buf, "    input.slider[%d] = ", usr->keyConfig[i].keyID);

            strcat(buf, "~#");
            strcat(buf, "$");


            if ( usr->keyConfig[i].slider_neg )
            {
                strcat(buf, Input::KeysInfo[ usr->keyConfig[i].slider_neg ]._name.c_str());
            }
            else
            {
                ypa_log_out("Slider(neg) %s is not declared!\n", usr->keyConfig[i].slider_name);
                ypa_log_out("Use space-key for it\n");

                strcat(buf, "space");
            }

            strcat(buf, "_#");
            strcat(buf, "$");

            if ( usr->keyConfig[i].KeyCode )
            {
                strcat(buf, Input::KeysInfo[ usr->keyConfig[i].KeyCode ]._name.c_str());
            }
            else
            {
                ypa_log_out("Slider(pos) %s is not declared!\n", usr->keyConfig[i].slider_name);
                ypa_log_out("Use space-key for it\n");

                strcat(buf, "space");
            }
        }
        else if ( usr->keyConfig[i].inp_type == World::KEYC_TYPE_HOTKEY )
        {
            sprintf(buf, "    input.hotkey[%d] = ", usr->keyConfig[i].keyID);

            if ( usr->keyConfig[i].KeyCode )
            {
                strcat(buf, Input::KeysInfo[ usr->keyConfig[i].KeyCode ]._name.c_str());
            }
            else
            {
                strcat(buf, "nop");
            }
        }
        else
            continue;

        strcat(buf, "         ; ");
        strcat(buf, usr->keyConfig[i].slider_name);
        strcat(buf, "\n");
        fil->write(buf, strlen(buf));
    }

    sprintf(buf, "end\n\n");
    fil->write(buf, strlen(buf));

    return 1;
}

int yw_write_sound(FSMgr::FileHandle *fil, UserData *usr)
{
    char buf[300];

    sprintf(buf, "new_sound\n");
    fil->write(buf, strlen(buf));

    sprintf(buf, "    volume = %d\n", usr->snd__volume);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    cdvolume = %d\n", usr->snd__cdvolume);
    fil->write(buf, strlen(buf));

    if ( usr->snd__flags2 & 1 )
        sprintf(buf, "    invertlr = yes\n");
    else
        sprintf(buf, "    invertlr = no\n");

    fil->write(buf, strlen(buf));

    if ( usr->snd__flags2 & 0x10 )
        sprintf(buf, "    cdsound = yes\n");
    else
        sprintf(buf, "    cdsound = no\n");

    fil->write(buf, strlen(buf));

    sprintf(buf, "end\n\n");
    fil->write(buf, strlen(buf));

    return 1;
}

int yw_write_video(FSMgr::FileHandle *fil, UserData *usr)
{
    char buf[300];

    sprintf(buf, "new_video\n");
    fil->write(buf, strlen(buf));

    sprintf(buf, "    videomode = %d\n", usr->p_ypaworld->game_default_res);
    fil->write(buf, strlen(buf));

    if ( usr->GFX_flags & 1 )
        sprintf(buf, "    farview = yes\n");
    else
        sprintf(buf, "    farview = no\n");

    fil->write(buf, strlen(buf));

    if ( usr->GFX_flags & 0x10 )
        sprintf(buf, "    16bittexture = yes\n");
    else
        sprintf(buf, "    16bittexture = no\n");

    fil->write(buf, strlen(buf));

    if ( usr->GFX_flags & 8 )
        sprintf(buf, "    drawprimitive = yes\n");
    else
        sprintf(buf, "    drawprimitive = no\n");

    fil->write(buf, strlen(buf));

    if ( usr->GFX_flags & 2 )
        sprintf(buf, "    heaven = yes\n");
    else
        sprintf(buf, "    heaven = no\n");

    fil->write(buf, strlen(buf));

    if ( usr->GFX_flags & 4 )
        sprintf(buf, "    softmouse = yes\n");
    else
        sprintf(buf, "    softmouse = no\n");

    fil->write(buf, strlen(buf));

    if ( usr->enemyindicator )
        sprintf(buf, "    enemyindicator = yes\n");
    else
        sprintf(buf, "    enemyindicator = no\n");

    fil->write(buf, strlen(buf));

    sprintf(buf, "    fxnumber = %d\n", usr->fxnumber);
    fil->write(buf, strlen(buf));

    sprintf(buf, "end\n\n");
    fil->write(buf, strlen(buf));

    return 1;
}

int yw_write_level_status(FSMgr::FileHandle *fil, NC_STACK_ypaworld *yw, int lvlid)
{
    char buf[300];

    //sprintf(buf, "\nbegin_levelstatus %ld\n", lvlid);
    sprintf(buf, "\nbegin_levelstatus %d\n", lvlid);
    fil->write(buf, strlen(buf));

    //sprintf(buf, "    status = %ld\n", yw->LevelNet->mapInfos[lvlid].field_0);
    sprintf(buf, "    status = %d\n", yw->LevelNet->mapInfos[lvlid].field_0);
    fil->write(buf, strlen(buf));

    sprintf(buf, "end\n\n");
    fil->write(buf, strlen(buf));

    return 1;
}

int yw_write_levels_statuses(FSMgr::FileHandle *fil, NC_STACK_ypaworld *yw)
{
    for (int i = 0; i < 256; i++)
    {
        if ( yw->LevelNet->mapInfos[ i ].field_0 )
        {
            if ( !yw_write_level_status(fil, yw, i))
                return 0;
        }
    }
    return 1;
}

int yw_write_buddies(FSMgr::FileHandle *fil, NC_STACK_ypaworld *yw)
{
    for (const MapBuddy &buddy : yw->_levelInfo->Buddies )
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

int yw_write_status(save_status *status, const char *field, FSMgr::FileHandle *file)
{
    char buf[256];

    sprintf(buf, "    %s = %d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d\n",
            field,
            status->p1,
            status->p2,
            status->p3,
            status->p4,
            status->p5,
            status->p6,
            status->pX[0],
            status->pX[1],
            status->pX[2],
            status->pX[3],
            status->pX[4],
            status->pX[5],
            status->pX[6],
            status->pX[7]);

    return file->write(buf, strlen(buf));
}

int yw_write_shell(FSMgr::FileHandle *fil, UserData *usr)
{
    char buf[300];

    NC_STACK_ypaworld *yw = usr->p_ypaworld;

    sprintf(buf, "new_shell\n");
    fil->write(buf, strlen(buf));

    if ( usr->default_lang_dll )
    {
        sprintf(buf, "    language = %s\n", usr->default_lang_dll->c_str());
        fil->write(buf, strlen(buf));
    }

    if ( yw->field_739A )
    {
        sprintf(buf, "    finder = na_0_0_0_0\n");
        fil->write(buf, strlen(buf));

        sprintf(buf, "    log    = na_0_0_0_0\n");
        fil->write(buf, strlen(buf));

        sprintf(buf, "    energy = na_0_0_0_0\n");
        fil->write(buf, strlen(buf));

        sprintf(buf, "    message = na_0_0_0_0\n");
        fil->write(buf, strlen(buf));

        sprintf(buf, "    map    = na_0_0_0_0_0_0\n");
        fil->write(buf, strlen(buf));

        yw_write_status(&yw->robo_map_status, "robo_map_status", fil);
        yw_write_status(&yw->robo_finder_status, "robo_finder_status", fil);
        yw_write_status(&yw->vhcl_map_status, "vhcl_map_status", fil);
        yw_write_status(&yw->vhcl_finder_status, "vhcl_finder_status", fil);
    }

    sprintf(buf, "end\n\n");
    fil->write(buf, strlen(buf));

    return 1;
}

int yw_write_item_modifers(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    char buf[300];

    for (int i = 0; i < 256; i++)
    {
        if ( yw->VhclProtos[i].model_id )
        {
            sprintf(buf, "modify_vehicle %d\n", i);
            fil->write(buf, strlen(buf));

            for (int j = 1; j < 8; j++)
            {
                if ( yw->VhclProtos[i].disable_enable_bitmask & (1 << j) )
                    sprintf(buf, "    enable         = %d\n", j);
                else
                    sprintf(buf, "    disable        = %d\n", j);

                fil->write(buf, strlen(buf));
            }

            sprintf(buf, "    shield         = %d\n", yw->VhclProtos[i].shield);
            fil->write(buf, strlen(buf));

            sprintf(buf, "    energy         = %d\n", yw->VhclProtos[i].energy);
            fil->write(buf, strlen(buf));

            sprintf(buf, "    num_weapons    = %d\n", yw->VhclProtos[i].num_weapons);
            fil->write(buf, strlen(buf));

            sprintf(buf, "    weapon         = %d\n", yw->VhclProtos[i].weapon);
            fil->write(buf, strlen(buf));

            sprintf(buf, "    radar          = %d\n", yw->VhclProtos[i].radar);
            fil->write(buf, strlen(buf));

            sprintf(buf, "    fire_x         = %4.2f\n", yw->VhclProtos[i].fire_x);
            fil->write(buf, strlen(buf));

            sprintf(buf, "    fire_y         = %4.2f\n", yw->VhclProtos[i].fire_y);
            fil->write(buf, strlen(buf));

            sprintf(buf, "    fire_z         = %4.2f\n", yw->VhclProtos[i].fire_z);
            fil->write(buf, strlen(buf));

            sprintf(buf, "end\n\n");
            fil->write(buf, strlen(buf));
        }
    }

    for (int i = 0; i < 128; i++)
    {
        if ( yw->WeaponProtos[i].field_0 )
        {
            sprintf(buf, "modify_weapon %d\n", i);
            fil->write(buf, strlen(buf));

            for (int j = 1; j < 8; j++)
            {
                if ( yw->WeaponProtos[i].enable_mask & (1 << j) )
                    sprintf(buf, "    enable         = %d\n", j);
                else
                    sprintf(buf, "    disable        = %d\n", j);

                fil->write(buf, strlen(buf));
            }

            sprintf(buf, "    shot_time      = %d\n", yw->WeaponProtos[i].shot_time);
            fil->write(buf, strlen(buf));

            sprintf(buf, "    shot_time_user = %d\n", yw->WeaponProtos[i].shot_time_user);
            fil->write(buf, strlen(buf));

            sprintf(buf, "    energy         = %d\n", yw->WeaponProtos[i].energy);
            fil->write(buf, strlen(buf));

            sprintf(buf, "end\n\n");
            fil->write(buf, strlen(buf));
        }
    }

    for (int i = 0; i < 128; i++)
    {
        if ( yw->BuildProtos[i].type_icon )
        {
            sprintf(buf, "modify_building %d\n", i);
            fil->write(buf, strlen(buf));

            for (int j = 1; j < 8; j++)
            {
                if ( yw->BuildProtos[i].enable_mask & (1 << j) )
                    sprintf(buf, "    enable         = %d\n", j);
                else
                    sprintf(buf, "    disable        = %d\n", j);

                fil->write(buf, strlen(buf));
            }

            sprintf(buf, "end\n\n");
            fil->write(buf, strlen(buf));
        }
    }

    return 1;
}

int yw_write_levelnum(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    fil->printf("\nbegin_levelnum\n");
    fil->printf("    levelnum = %d\n", yw->_levelInfo->LevelID);
    fil->printf("end\n");

    return 1;
}

void yw_write_map(NC_STACK_ypaworld *yw, Common::PlaneBytes *map, const std::string &padding, FSMgr::FileHandle *fil)
{
    if ( !padding.empty() )
        fil->printf(padding.c_str());

    fil->printf("%d %d\n", map->Width(), map->Height());

    for (uint32_t y = 0; y < map->Height(); y++)
    {
        if ( !padding.empty() )
            fil->printf(padding.c_str());

        uint8_t *v6 = map->Line(y);
        
        for (uint32_t x = 0; x < map->Width(); x++ )
        {
            fil->printf("%02x ", *v6);
            v6++;
        }

        fil->printf("\n");
    }
}

void yw_write_ownermap(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    Common::PlaneBytes *ownermap = new Common::PlaneBytes(yw->sectors_maxX2, yw->sectors_maxY2);

    if ( ownermap )
    {
        for (uint32_t i = 0; i < ownermap->size(); i++)
            (*ownermap)[i] = yw->cells[i].owner;

        fil->printf("\nbegin_ownermap\n");
        yw_write_map(yw, ownermap, "        ", fil);
        fil->printf("end\n");

        delete ownermap;
    }
}

void yw_write_buildmap(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    fil->printf("\nbegin_buildingmap\n");
    yw_write_map(yw, yw->blg_map, "        ", fil);
    fil->printf("end\n");
}

void yw_write_energymap(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    Common::PlaneBytes *energymap = new Common::PlaneBytes(yw->sectors_maxX2 * 3, yw->sectors_maxY2 * 3);

    if ( energymap )
    {
        int sz = yw->sectors_maxY2 * yw->sectors_maxX2;
        uint8_t *outbf = energymap->data();

        for (int i = 0; i < sz; i++)
        {
            cellArea &cell = yw->cells[i];

            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    *outbf = cell.buildings_health[j][k];
                    outbf++;
                }
            }
        }

        fil->printf("\nbegin_energymap\n");
        yw_write_map(yw, energymap, "        ", fil);
        fil->printf("end\n");
        
        delete energymap;
    }
    else
    {
        ypa_log_out("game save error: Unable to create bmo for saving owner\n");
    }
}

int yw_write_bact(NC_STACK_ypabact *bct, FSMgr::FileHandle *fil)
{
    char buf[300];

    int a4 = bct->getBACT_viewer();

    if ( a4 )
        sprintf(buf, "    viewer         = yes\n");
    else
        sprintf(buf, "    viewer         = no\n");

    fil->write(buf, strlen(buf));

    a4 = bct->getBACT_inputting();

    if ( a4 )
        sprintf(buf, "    user           = yes\n");
    else
        sprintf(buf, "    user           = no\n");

    fil->write(buf, strlen(buf));

    a4 = bct->getBACT_bactCollisions();

    if ( a4 )
        sprintf(buf, "    collision      = yes\n");
    else
        sprintf(buf, "    collision      = no\n");

    fil->write(buf, strlen(buf));
    sprintf(buf, "    energy         = %d\n", bct->_energy);

    fil->write(buf, strlen(buf));
    sprintf(buf, "    speed          = %6.5f_%6.5f_%6.5f_%6.5f\n", bct->_fly_dir.x, bct->_fly_dir.y, bct->_fly_dir.z, bct->_fly_dir_length);

    fil->write(buf, strlen(buf));

    sprintf(
        buf,
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

    fil->write(buf, strlen(buf));

    if ( bct->_bact_type == BACT_TYPES_ROBO )
    {
        NC_STACK_yparobo *roboo = dynamic_cast<NC_STACK_yparobo *>(bct);
        sprintf(
            buf,
            "    pos            = %2.2f_%2.2f_%2.2f\n",
            bct->_position.x,
            roboo->_roboYPos,
            bct->_position.z);
    }
    else
        sprintf(
            buf,
            "    pos            = %2.2f_%2.2f_%2.2f\n",
            bct->_position.x,
            bct->_position.y,
            bct->_position.z);

    fil->write(buf, strlen(buf));

    sprintf(buf, "    force          = %2.2f\n", bct->_thraction);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    gunangle       = %5.4f\n", bct->_gun_angle_user);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    commandID      = %d\n", (bct->_commandID & 0xFFFFFF));
    fil->write(buf, strlen(buf));

    sprintf(buf, "    aggression     = %d\n", bct->_aggr);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    mainstate      = %d\n", bct->_status);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    extrastate     = %d\n", bct->_status_flg);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    ident          = %d\n", bct->_gid);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    killerowner    = %d\n", bct->_killer_owner);
    fil->write(buf, strlen(buf));

    if ( bct->_primTtype == BACT_TGT_TYPE_UNIT )
        sprintf(buf, "    primary        = %d_%d_%2.2f_%2.2f_%d\n", bct->_primTtype, bct->_primT.pbact->_gid, bct->_primTpos.x, bct->_primTpos.z, bct->_primT_cmdID);
    else
        sprintf(buf, "    primary        = %d_0_%2.2f_%2.2f_%d\n", bct->_primTtype, bct->_primTpos.x, bct->_primTpos.z, bct->_primT_cmdID);

    fil->write(buf, strlen(buf));

    for (int i = 0; i < bct->_waypoints_count; i++)
    {
        sprintf(buf, "    waypoint       = %d_%2.2f_%2.2f\n", i, bct->_waypoints[i].x, bct->_waypoints[i].z);
        fil->write(buf, strlen(buf));
    }

    sprintf(buf, "    num_wp         = %d\n", bct->_waypoints_count);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    count_wp       = %d\n", bct->_current_waypoint);
    fil->write(buf, strlen(buf));

    return 1;
}

int yw_write_robo(NC_STACK_ypaworld *yw, NC_STACK_yparobo *robo, FSMgr::FileHandle *fil)
{
    char buf[300];

    sprintf(buf, "\nbegin_robo %d\n", robo->_vehicleID);
    fil->write(buf, strlen(buf));

    const char *isuser = "no";

    if (robo == yw->UserRobo)
        isuser = "yes";

    sprintf(buf, "    is_user_robo   = %s\n", isuser);
    fil->write(buf, strlen(buf));

    if ( !yw_write_bact(robo, fil) )
        return 0;

    sprintf(buf, "    owner          = %d\n", robo->_owner);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    robostate      = %d\n", robo->_roboState & 0xC00F );
    fil->write(buf, strlen(buf));

    sprintf(buf, "    dockenergy     = %d\n", robo->_roboDockEnerg);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    dockcount      = %d\n", robo->_roboDockCnt);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    dockuser       = %d\n", robo->_roboDockUser);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    docktime       = %d\n", robo->_roboDockTime);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    docktargetpos  = %2.2f_%2.2f\n", robo->_roboDockTargetPos.x, robo->_roboDockTargetPos.z);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    docktargetID   = %d\n", robo->_roboDockTargetCommandID);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    docktargettype = %d\n", robo->_roboDockTargetType);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    dockaggr       = %d\n", robo->_roboDockAggr);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    battvehicle    = %d\n", robo->_roboEnergyLife);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    battbeam       = %d\n", robo->_roboEnergyMove);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    fillmodus      = %d\n", robo->_roboFillMode);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    maximum        = %d\n", robo->_energy_max);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    buildspare     = %d\n", robo->_roboBuildSpare);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    vhoriz         = %7.5f\n", robo->_viewer_horiz_angle);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    vvert          = %7.5f\n", robo->_viewer_vert_angle);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    con_budget     = %d\n", robo->_roboEpConquer);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    def_budget     = %d\n", robo->_roboEpDefense);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    rec_budget     = %d\n", robo->_roboEpRecon);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    rob_budget     = %d\n", robo->_roboEpRobo);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    rad_budget     = %d\n", robo->_roboEpRadar);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    pow_budget     = %d\n", robo->_roboEpPower);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    saf_budget     = %d\n", robo->_roboEpSafety);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    cpl_budget     = %d\n", robo->_roboEpChangePlace);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    saf_delay     = %d\n", robo->_roboSafetyDelay);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    pow_delay     = %d\n", robo->_roboPowerDelay);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    rad_delay     = %d\n", robo->_roboRadarDelay);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    cpl_delay     = %d\n", robo->_roboPositionDelay);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    def_delay     = %d\n", robo->_roboEnemyDelay);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    con_delay     = %d\n", robo->_roboConqDelay);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    rec_delay     = %d\n", robo->_roboExploreDelay);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    rob_delay     = %d\n", robo->_roboDangerDelay);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    reload_const  = %d\n", robo->_reload_const);
    fil->write(buf, strlen(buf));

    sprintf(buf, "end\n\n");
    fil->write(buf, strlen(buf));

    return 1;
}

int yw_write_gun(NC_STACK_ypabact *bct, FSMgr::FileHandle *fil)
{
    char buf[300];

    if ( bct->_bact_type == BACT_TYPES_GUN )
    {
        NC_STACK_ypagun *guno = (NC_STACK_ypagun *)bct;

        sprintf(
            buf,
            "    gunbasis = %7.6f_%7.6f_%7.6f\n",
            guno->_gunBasis.x,
            guno->_gunBasis.y,
            guno->_gunBasis.z);

        fil->write(buf, strlen(buf));
    }
    return 1;
}

int yw_write_commander(NC_STACK_ypabact *bct, FSMgr::FileHandle *fil)
{
    char buf[300];

    sprintf(buf, "\nbegin_commander %d\n", bct->_vehicleID);
    fil->write(buf, strlen(buf));

    if ( !yw_write_bact(bct, fil) )
        return 0;

    if ( !yw_write_gun(bct, fil) )
        return 0;

    sprintf(buf, "end\n\n");
    fil->write(buf, strlen(buf));

    return 1;
}

int yw_write_slave(NC_STACK_ypabact *bct, FSMgr::FileHandle *fil)
{
    char buf[300];

    sprintf(buf, "\nbegin_slave %d\n", bct->_vehicleID);
    fil->write(buf, strlen(buf));

    if ( !yw_write_bact(bct, fil) )
        return 0;

    if ( !yw_write_gun(bct, fil) )
        return 0;

    sprintf(buf, "end\n\n");
    fil->write(buf, strlen(buf));

    return 1;
}

int yw_write_extraviewer(NC_STACK_ypabact *bct, FSMgr::FileHandle *fil)
{
    char buf[300];

    if ( bct->_bact_type == BACT_TYPES_GUN )
    {
        int v7 = -1;

        sprintf(buf, "\nbegin_extraviewer\n");
        fil->write(buf, strlen(buf));

        NC_STACK_yparobo *roboo = bct->_host_station;

        for (int i = 0; i < 8; i++)
        {
            if ( roboo->_roboGuns[i].gun_obj == bct )
            {
                v7 = i;
                break;
            }
        }

        if ( v7 >= 0 )
        {
            sprintf(buf, "    kind = robogun\n");
            fil->write(buf, strlen(buf));

            sprintf(buf, "    number = %d\n", v7);
            fil->write(buf, strlen(buf));

            sprintf(buf, "end\n\n");
            fil->write(buf, strlen(buf));
            return 1;
        }

        ypa_log_out("Error: Gun not found in yrd!\n");
    }
    return 0;
}

int yw_write_units(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    bact_node *station = (bact_node *)yw->bact_list.tailpred;
    while (station->prev)
    {
        if ( station->bact->_status != BACT_STATUS_DEAD )
        {
            if ( !yw_write_robo(yw, (NC_STACK_yparobo *)station->bact, fil) )
                return 0;

            bact_node *commander = (bact_node *)station->bact->_subjects_list.tailpred;

            while ( commander->prev )
            {
                int a4 = 0;

                if ( commander->bact->_bact_type == BACT_TYPES_GUN )
                {
                    NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( commander->bact );
                    a4 = gun->IsRoboGun();
                }

                if ( !a4 )
                {
                    if ( !yw_write_commander(commander->bact, fil) )
                        return 0;
                }
                else
                {
                    int v8 = commander->bact->getBACT_viewer();

                    if ( v8 )
                    {
                        if ( !yw_write_extraviewer(commander->bact, fil) )
                            return 0;
                    }
                }

                bact_node *slave = (bact_node *)commander->bact->_subjects_list.tailpred;

                while ( slave->prev )
                {
                    int v9 = 0;
                    if ( slave->bact->_bact_type == BACT_TYPES_GUN )
                    {
                        NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( slave->bact );
                        v9 = gun->IsRoboGun();
                    }

                    if ( !v9 )
                    {
                        if ( !yw_write_slave(slave->bact, fil) )
                            return 0;
                    }
                    else
                    {
                        int v10 = slave->bact->getBACT_viewer();

                        if ( v10 )
                        {
                            if ( !yw_write_extraviewer(slave->bact, fil) )
                                return 0;
                        }
                    }

                    slave = (bact_node *)slave->prev;
                }

                commander = (bact_node *)commander->prev;
            }
        }

        station = (bact_node *)station->prev;
    }

    return 1;
}

int yw_write_wunderinfo(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    fil->printf("\nbegin_wunderinfo\n");

    size_t i = 0;
    for ( const MapGem &gem : yw->_Gems )
    {
        if ( yw->cells[gem.SecX + gem.SecY * yw->sectors_maxX2].w_type != 4 )
            fil->printf("    disablegem %d\n", i);

        i++;
    }

    fil->printf("end\n\n");
    return 1;
}

int yw_write_kwfactor(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    char buf[300];

    sprintf(buf, "\nbegin_kwfactor\n");
    fil->write(buf, strlen(buf));

    for (int i = 0; i < 256; i++)
    {
        yw_field34 *kw = &yw->field_34[i];
        if ( kw->p_cell )
        {
            sprintf(buf, "    kw = %d_%d_%d\n", kw->x, kw->y, kw->power_2);
            fil->write(buf, strlen(buf));
        }
    }

    sprintf(buf, "end\n\n");
    fil->write(buf, strlen(buf));
    return 1;
}

int yw_write_globals(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    char buf[300];

    sprintf(buf, "\nbegin_globals\n");
    fil->write(buf, strlen(buf));

    sprintf(buf, "    time = %d\n", yw->timeStamp);
    fil->write(buf, strlen(buf));

    sprintf(buf, "end\n\n");
    fil->write(buf, strlen(buf));
    return 1;
}

int yw_write_superbomb(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    int i = 0;
    for ( const MapSuperItem &sitem : yw->_levelInfo->SuperItems)
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
    if ( yw->_history.Size() )
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
    }

    return 1;
}

int yw_write_masks(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    fil->printf("\nbegin_masks\n");
    fil->printf("    ownermask = %d\n", yw->_levelInfo->OwnerMask);
    fil->printf("    usermask  = %d\n", yw->_levelInfo->UserMask);
    fil->printf("end\n");
    return 1;
}
