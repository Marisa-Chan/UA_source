#include <inttypes.h>
#include <string.h>
#include "includes.h"
#include "def_parser.h"
#include "yw_internal.h"
#include "yw.h"
#include "input.h"
#include "yparobo.h"

extern Key_stru keySS[256];

int yw_write_callSign(_NC_STACK_ypaworld *yw, const char *filename, const char *callsign)
{
    UserData *usr = yw->GameShell;

    if ( !usr )
        return 0;

    if ( !usr->user_name[0] )
        return 0;

    char buf[260];
    sprintf(buf, "save:%s/%s", usr->user_name, filename);

    FSMgr::FileHandle *fil = uaOpenFile(buf, "w");

    if ( !fil )
        return 0;

    fil->printf("%s", callsign);

    delete fil;

    return 1;
}

int yw_write_user(FSMgr::FileHandle *fil, UserData *usr)
{
    _NC_STACK_ypaworld *yw = usr->p_ypaworld;

    if ( usr->callSIGN[0] >= ' ' )
        yw_write_callSign(yw, "callsign.def", usr->callSIGN);

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

    sprintf(buf, "    maxroboenergy = %d\n", usr->p_ypaworld->maxroboenergy);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    maxreloadconst = %d\n", usr->p_ypaworld->maxreloadconst);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    numbuddies    = %d\n", usr->p_ypaworld->field_2d90->field_74);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    beamenergy    = %d\n", usr->p_ypaworld->beamenergy);
    fil->write(buf, strlen(buf));


    sprintf(buf, "    jodiefoster   = ");

    for (int i = 0; i < 8; i++)
    {
        char v29[12];

        sprintf(v29, "%d", usr->p_ypaworld->field_2d90->jodiefoster[i]);
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
        if ( usr->keyConfig[i].inp_type == 1 )
        {
            sprintf(buf, "    input.button[%d] = ", usr->keyConfig[i].keyID);

            strcat(buf, "$");

            if ( usr->keyConfig[i].KeyCode )
            {
                strcat(buf, keySS[usr->keyConfig[i].KeyCode].short_name);
            }
            else
            {
                strcat(buf, "nop");
            }
        }
        else if ( usr->keyConfig[i].inp_type == 2 )
        {
            sprintf(buf, "    input.slider[%d] = ", usr->keyConfig[i].keyID);

            strcat(buf, "~#");
            strcat(buf, "$");


            if ( usr->keyConfig[i].slider_neg )
            {
                strcat(buf, keySS[ usr->keyConfig[i].slider_neg ].short_name);
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
                strcat(buf, keySS[ usr->keyConfig[i].KeyCode ].short_name);
            }
            else
            {
                ypa_log_out("Slider(pos) %s is not declared!\n", usr->keyConfig[i].slider_name);
                ypa_log_out("Use space-key for it\n");

                strcat(buf, "space");
            }
        }
        else if ( usr->keyConfig[i].inp_type == 3 )
        {
            sprintf(buf, "    input.hotkey[%d] = ", usr->keyConfig[i].keyID);

            if ( usr->keyConfig[i].KeyCode )
            {
                strcat(buf, keySS[ usr->keyConfig[i].KeyCode ].short_name);
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

int yw_write_level_status(FSMgr::FileHandle *fil, _NC_STACK_ypaworld *yw, int lvlid)
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

int yw_write_levels_statuses(FSMgr::FileHandle *fil, _NC_STACK_ypaworld *yw)
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

int yw_write_buddy(FSMgr::FileHandle *fil, _NC_STACK_ypaworld *yw, int buddy_id)
{
    char buf[300];

    sprintf(buf, "\nbegin_buddy\n");
    fil->write(buf, strlen(buf));

    //sprintf(buf, "    commandid = %ld\n", yw->field_2d90->buddies[buddy_id].commandid);
    sprintf(buf, "    commandid = %d\n", yw->field_2d90->buddies[buddy_id].commandid);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    type      = %d\n", yw->field_2d90->buddies[buddy_id].type);
    fil->write(buf, strlen(buf));

    //sprintf(buf, "    energy    = %ld\n", yw->field_2d90->buddies[buddy_id].energy);
    sprintf(buf, "    energy    = %d\n", yw->field_2d90->buddies[buddy_id].energy);
    fil->write(buf, strlen(buf));

    sprintf(buf, "end\n\n");
    fil->write(buf, strlen(buf));

    return 1;
}

int yw_write_buddies(FSMgr::FileHandle *fil, _NC_STACK_ypaworld *yw)
{
    for (int i = 0; i < yw->field_2d90->buddies_count; i++ )
    {
        if ( !yw_write_buddy(fil, yw, i) )
            return 0;
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

    _NC_STACK_ypaworld *yw = usr->p_ypaworld;

    sprintf(buf, "new_shell\n");
    fil->write(buf, strlen(buf));

    if ( usr->default_lang_dll )
    {
        sprintf(buf, "    language = %s\n", usr->default_lang_dll->langDllName);
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

int yw_write_item_modifers(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
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

int yw_write_levelnum(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    fil->printf("\nbegin_levelnum\n");
    fil->printf("    levelnum = %d\n", yw->field_2d90->levelID);
    fil->printf("end\n");

    return 1;
}

void yw_write_map(_NC_STACK_ypaworld *yw, NC_STACK_bitmap *bitmap, const char *padding, FSMgr::FileHandle *fil)
{
    bitmap_intern *bitm = bitmap->getBMD_pBitmap();

    if ( padding )
        fil->printf(padding);

    fil->printf("%d %d\n", bitm->width, bitm->height);

    uint8_t *v6 = (uint8_t *)bitm->buffer;

    for (int y = 0; y < bitm->height; y++)
    {
        if ( padding )
            fil->printf(padding);

        for (int x = 0; x < bitm->width; x++ )
        {
            fil->printf("%02x ", *v6);
            v6++;
        }

        fil->printf("\n");
    }
}

void yw_write_ownermap(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    stack_vals init_vals[4];
    init_vals[0].set(NC_STACK_rsrc::RSRC_ATT_NAME, "temp_owner_map");
    init_vals[1].set(NC_STACK_bitmap::BMD_ATT_WIDTH, yw->sectors_maxX2);
    init_vals[2].set(NC_STACK_bitmap::BMD_ATT_HEIGHT, yw->sectors_maxY2);
    init_vals[3].end();

    NC_STACK_bitmap *bitmap = NC_STACK_bitmap::CInit(init_vals);

    if ( bitmap )
    {
        bitmap_intern *bitm = bitmap->getBMD_pBitmap();

        int sz = yw->sectors_maxY2 * yw->sectors_maxX2;
        uint8_t *outbf = (uint8_t *)bitm->buffer;

        for (int i = 0; i < sz; i++)
            outbf[i] = yw->cells[i].owner;

        fil->printf("\nbegin_ownermap\n");
        yw_write_map(yw, bitmap, "        ", fil);
        fil->printf("end\n");

        delete_class_obj(bitmap);
    }
}

void yw_write_buildmap(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    fil->printf("\nbegin_buildingmap\n");
    yw_write_map(yw, yw->blg_map, "        ", fil);
    fil->printf("end\n");
}

void yw_write_energymap(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    stack_vals init_vals[4];
    init_vals[0].set(NC_STACK_rsrc::RSRC_ATT_NAME, "ActualEnergyMap");
    init_vals[1].set(NC_STACK_bitmap::BMD_ATT_WIDTH, 3 * yw->sectors_maxX2);
    init_vals[2].set(NC_STACK_bitmap::BMD_ATT_HEIGHT, 3 * yw->sectors_maxY2);
    init_vals[3].end();

    NC_STACK_bitmap *bitmap = NC_STACK_bitmap::CInit(init_vals);

    if ( bitmap )
    {
        bitmap_intern *bitm = bitmap->getBMD_pBitmap();

        int sz = yw->sectors_maxY2 * yw->sectors_maxX2;
        uint8_t *outbf = (uint8_t *)bitm->buffer;

        for (int i = 0; i < sz; i++)
        {
            cellArea *cell = &yw->cells[i];

            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    *outbf = cell->buildings_health[j][k];
                    outbf++;
                }
            }
        }

        fil->printf("\nbegin_energymap\n");
        yw_write_map(yw, bitmap, "        ", fil);
        fil->printf("end\n");
    }
    else
    {
        ypa_log_out("game save error: Unable to create bmo for saving owner\n");
    }

    if ( bitmap )
        delete_class_obj(bitmap);
}

int yw_write_bact(bact_node *bct, FSMgr::FileHandle *fil)
{
    char buf[300];

    int a4 = bct->bacto->getBACT_viewer();

    if ( a4 )
        sprintf(buf, "    viewer         = yes\n");
    else
        sprintf(buf, "    viewer         = no\n");

    fil->write(buf, strlen(buf));

    a4 = bct->bacto->getBACT_inputting();

    if ( a4 )
        sprintf(buf, "    user           = yes\n");
    else
        sprintf(buf, "    user           = no\n");

    fil->write(buf, strlen(buf));

    a4 = bct->bacto->getBACT_bactCollisions();

    if ( a4 )
        sprintf(buf, "    collision      = yes\n");
    else
        sprintf(buf, "    collision      = no\n");

    fil->write(buf, strlen(buf));
    sprintf(buf, "    energy         = %d\n", bct->bact->energy);

    fil->write(buf, strlen(buf));
    sprintf(buf, "    speed          = %6.5f_%6.5f_%6.5f_%6.5f\n", bct->bact->fly_dir.sx, bct->bact->fly_dir.sy, bct->bact->fly_dir.sz, bct->bact->fly_dir_length);

    fil->write(buf, strlen(buf));

    sprintf(
        buf,
        "    matrix         = %6.5f_%6.5f_%6.5f_%6.5f_%6.5f_%6.5f_%6.5f_%6.5f_%6.5f\n",
        bct->bact->rotation.m00,
        bct->bact->rotation.m01,
        bct->bact->rotation.m02,
        bct->bact->rotation.m10,
        bct->bact->rotation.m11,
        bct->bact->rotation.m12,
        bct->bact->rotation.m20,
        bct->bact->rotation.m21,
        bct->bact->rotation.m22);

    fil->write(buf, strlen(buf));

    if ( bct->bact->bact_type == BACT_TYPES_ROBO )
    {
        NC_STACK_yparobo *roboo = dynamic_cast<NC_STACK_yparobo *>(bct->bacto);
        sprintf(
            buf,
            "    pos            = %2.2f_%2.2f_%2.2f\n",
            bct->bact->position.sx,
            roboo->stack__yparobo.field_1D5,
            bct->bact->position.sz);
    }
    else
        sprintf(
            buf,
            "    pos            = %2.2f_%2.2f_%2.2f\n",
            bct->bact->position.sx,
            bct->bact->position.sy,
            bct->bact->position.sz);

    fil->write(buf, strlen(buf));

    sprintf(buf, "    force          = %2.2f\n", bct->bact->thraction);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    gunangle       = %5.4f\n", bct->bact->gun_angle_user);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    commandID      = %d\n", bct->bact->commandID);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    aggression     = %d\n", bct->bact->aggr);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    mainstate      = %d\n", bct->bact->status);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    extrastate     = %d\n", bct->bact->status_flg);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    ident          = %d\n", bct->bact->gid);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    killerowner    = %d\n", bct->bact->killer_owner);
    fil->write(buf, strlen(buf));

    if ( bct->bact->primTtype == BACT_TGT_TYPE_UNIT )
        sprintf(buf, "    primary        = %d_%d_%2.2f_%2.2f_%d\n", bct->bact->primTtype, bct->bact->primT.pbact->gid, bct->bact->primTpos.sx, bct->bact->primTpos.sz, bct->bact->primT_cmdID);
    else
        sprintf(buf, "    primary        = %d_0_%2.2f_%2.2f_%d\n", bct->bact->primTtype, bct->bact->primTpos.sx, bct->bact->primTpos.sz, bct->bact->primT_cmdID);

    fil->write(buf, strlen(buf));

    for (int i = 0; i < bct->bact->waypoints_count; i++)
    {
        sprintf(buf, "    waypoint       = %d_%2.2f_%2.2f\n", i, bct->bact->waypoints[i].sx, bct->bact->waypoints[i].sz);
        fil->write(buf, strlen(buf));
    }

    sprintf(buf, "    num_wp         = %d\n", bct->bact->waypoints_count);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    count_wp       = %d\n", bct->bact->current_waypoint);
    fil->write(buf, strlen(buf));

    return 1;
}

int yw_write_robo(_NC_STACK_ypaworld *yw, bact_node *bct, FSMgr::FileHandle *fil)
{
    char buf[300];

    sprintf(buf, "\nbegin_robo %d\n", bct->bact->vehicleID);
    fil->write(buf, strlen(buf));

    const char *isuser = "no";

    if (bct->bacto == yw->field_1b78)
        isuser = "yes";

    sprintf(buf, "    is_user_robo   = %s\n", isuser);
    fil->write(buf, strlen(buf));

    if ( !yw_write_bact(bct, fil) )
        return 0;

    sprintf(buf, "    owner          = %d\n", bct->bact->owner);
    fil->write(buf, strlen(buf));

    NC_STACK_yparobo *roboo = dynamic_cast<NC_STACK_yparobo *>(bct->bacto);
    __NC_STACK_yparobo *robo = &roboo->stack__yparobo;

    sprintf(buf, "    robostate      = %d\n", robo->roboState & 0xC00F );
    fil->write(buf, strlen(buf));

    sprintf(buf, "    dockenergy     = %d\n", robo->dock_energ);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    dockcount      = %d\n", robo->dock_cnt);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    dockuser       = %d\n", robo->dock_user);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    docktime       = %d\n", robo->dock_time);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    docktargetpos  = %2.2f_%2.2f\n", robo->dock_tgt_pos.sx, robo->dock_tgt_pos.sz);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    docktargetID   = %d\n", robo->dock_tgt_comm_id);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    docktargettype = %d\n", robo->dock_tgType);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    dockaggr       = %d\n", robo->dock_aggr);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    battvehicle    = %d\n", robo->field_4F5);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    battbeam       = %d\n", robo->field_4FD);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    fillmodus      = %d\n", robo->field_501);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    maximum        = %d\n", robo->bact_internal->energy_max);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    buildspare     = %d\n", robo->field_509);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    vhoriz         = %7.5f\n", robo->bact_internal->viewer_horiz_angle);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    vvert          = %7.5f\n", robo->bact_internal->viewer_vert_angle);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    con_budget     = %d\n", robo->epConquer);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    def_budget     = %d\n", robo->epDefense);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    rec_budget     = %d\n", robo->epRecon);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    rob_budget     = %d\n", robo->epRobo);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    rad_budget     = %d\n", robo->epRadar);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    pow_budget     = %d\n", robo->epPower);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    saf_budget     = %d\n", robo->epSafety);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    cpl_budget     = %d\n", robo->epChangePlace);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    saf_delay     = %d\n", robo->safety_delay);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    pow_delay     = %d\n", robo->field_265);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    rad_delay     = %d\n", robo->radar_delay);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    cpl_delay     = %d\n", robo->field_2B1);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    def_delay     = %d\n", robo->field_281);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    con_delay     = %d\n", robo->conq_delay);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    rec_delay     = %d\n", robo->field_2C9);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    rob_delay     = %d\n", robo->field_2E1);
    fil->write(buf, strlen(buf));

    sprintf(buf, "    reload_const  = %d\n", robo->bact_internal->reload_const);
    fil->write(buf, strlen(buf));

    sprintf(buf, "end\n\n");
    fil->write(buf, strlen(buf));

    return 1;
}

int yw_write_gun(bact_node *bct, FSMgr::FileHandle *fil)
{
    char buf[300];

    if ( bct->bact->bact_type == BACT_TYPES_GUN )
    {
        NC_STACK_ypagun *guno = (NC_STACK_ypagun *)bct->bacto;

        sprintf(
            buf,
            "    gunbasis = %7.6f_%7.6f_%7.6f\n",
            guno->stack__ypagun.dir.sx,
            guno->stack__ypagun.dir.sy,
            guno->stack__ypagun.dir.sz);

        fil->write(buf, strlen(buf));
    }
    return 1;
}

int yw_write_commander(bact_node *bct, FSMgr::FileHandle *fil)
{
    char buf[300];

    sprintf(buf, "\nbegin_commander %d\n", bct->bact->vehicleID);
    fil->write(buf, strlen(buf));

    if ( !yw_write_bact(bct, fil) )
        return 0;

    if ( !yw_write_gun(bct, fil) )
        return 0;

    sprintf(buf, "end\n\n");
    fil->write(buf, strlen(buf));

    return 1;
}

int yw_write_slave(bact_node *bct, FSMgr::FileHandle *fil)
{
    char buf[300];

    sprintf(buf, "\nbegin_slave %d\n", bct->bact->vehicleID);
    fil->write(buf, strlen(buf));

    if ( !yw_write_bact(bct, fil) )
        return 0;

    if ( !yw_write_gun(bct, fil) )
        return 0;

    sprintf(buf, "end\n\n");
    fil->write(buf, strlen(buf));

    return 1;
}

int yw_write_extraviewer(bact_node *bct, FSMgr::FileHandle *fil)
{
    char buf[300];

    if ( bct->bact->bact_type == BACT_TYPES_GUN )
    {
        int v7 = -1;

        sprintf(buf, "\nbegin_extraviewer\n");
        fil->write(buf, strlen(buf));

        NC_STACK_yparobo *roboo = bct->bact->host_station;

        for (int i = 0; i < 8; i++)
        {
            if ( roboo->stack__yparobo.guns[i].gun_obj == bct->bacto )
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

int yw_write_units(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    bact_node *station = (bact_node *)yw->bact_list.tailpred;
    while (station->prev)
    {
        if ( station->bact->status != BACT_STATUS_DEAD )
        {
            if ( !yw_write_robo(yw, station, fil) )
                return 0;

            bact_node *commander = (bact_node *)station->bact->subjects_list.tailpred;

            while ( commander->prev )
            {
                int a4 = 0;

                if ( commander->bact->bact_type == BACT_TYPES_GUN )
                {
                    NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( commander->bacto );
                    a4 = gun->getGUN_roboGun();
                }

                if ( !a4 )
                {
                    if ( !yw_write_commander(commander, fil) )
                        return 0;
                }
                else
                {
                    int v8 = commander->bacto->getBACT_viewer();

                    if ( v8 )
                    {
                        if ( !yw_write_extraviewer(commander, fil) )
                            return 0;
                    }
                }

                bact_node *slave = (bact_node *)commander->bact->subjects_list.tailpred;

                while ( slave->prev )
                {
                    int v9 = 0;
                    if ( slave->bact->bact_type == BACT_TYPES_GUN )
                    {
                        NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( slave->bacto );
                        v9 = gun->getGUN_roboGun();
                    }

                    if ( !v9 )
                    {
                        if ( !yw_write_slave(slave, fil) )
                            return 0;
                    }
                    else
                    {
                        int v10 = slave->bacto->getBACT_viewer();

                        if ( v10 )
                        {
                            if ( !yw_write_extraviewer(slave, fil) )
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

int yw_write_wunderinfo(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    char buf[300];

    sprintf(buf, "\nbegin_wunderinfo\n");
    fil->write(buf, strlen(buf));

    for (int i = 0; i < 8; i++)
    {
        gemProto *gem = &yw->gems[i];

        if ( gem->field_0 )
        {
            if ( yw->cells[gem->sec_x + gem->sec_y * yw->sectors_maxX2].w_type != 4 )
            {
                sprintf(buf, "    disablegem %d\n", i);
                fil->write(buf, strlen(buf));
            }
        }
    }

    sprintf(buf, "end\n\n");
    fil->write(buf, strlen(buf));
    return 1;
}

int yw_write_kwfactor(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
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

int yw_write_globals(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
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

int yw_write_superbomb(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    for (int i = 0; i < yw->field_2d90->supetItems_count; i++)
    {
        fil->printf("\nbegin_superbomb\n");
        fil->printf("    num               = %d\n", i);
        fil->printf("    status            = %d\n", yw->field_2d90->supetItems[i].field_4);
        fil->printf("    active_timestamp  = %d\n", yw->field_2d90->supetItems[i].field_EC);
        fil->printf("    trigger_timestamp = %d\n", yw->field_2d90->supetItems[i].field_F0);
        fil->printf("    activated_by      = %d\n", yw->field_2d90->supetItems[i].field_F4);
        fil->printf("    countdown         = %d\n", yw->field_2d90->supetItems[i].field_F8);
        fil->printf("    last_ten_sec      = %d\n", yw->field_2d90->supetItems[i].field_FC);
        fil->printf("    last_sec          = %d\n", yw->field_2d90->supetItems[i].field_100);
        fil->printf("    radius            = %d\n", yw->field_2d90->supetItems[i].field_104);
        fil->printf("    last_radius       = %d\n", yw->field_2d90->supetItems[i].field_108);
        fil->printf("end\n");
    }

    return 1;
}

int yw_write_histbuffer(yw_f726c_nod *hist_node, FSMgr::FileHandle *fil)
{
    uint8_t *bufpos = hist_node->bufStart;
    int lines = (hist_node->bufEnd - hist_node->bufStart) / 64;

    fil->printf("    history_buffer = \n");
    fil->printf("    %d %d\n", 64, lines);

    for (int i = 0; i < lines; i++)
    {
        fil->printf("    ");

        for (int j = 0; j < 64; j++)
        {
            fil->printf("%02x ", *bufpos);
            bufpos++;
        }

        fil->printf("\n");
    }

    fil->printf("\n");
    return 1;
}

int yw_write_history(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    if ( yw->history )
    {
        fil->printf(";------------------------------------------------------------\n");
        fil->printf("; History Buffers\n");
        fil->printf(";------------------------------------------------------------\n");
        fil->printf("begin_history\n");

        yw_f726c_nod *histnode = (yw_f726c_nod *)yw->history->lst.head;

        while ( histnode->next )
        {
            yw_write_histbuffer(histnode, fil);

            histnode = (yw_f726c_nod *)histnode->next;
        }

        fil->printf("end");
    }

    return 1;
}

int yw_write_masks(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    fil->printf("\nbegin_masks\n");
    fil->printf("    ownermask = %d\n", yw->field_2d90->ownerMap__has_vehicles);
    fil->printf("    usermask  = %d\n", yw->field_2d90->field_60);
    fil->printf("end\n");
    return 1;
}
