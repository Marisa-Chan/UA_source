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

    FILE *fil = FOpen(buf, "w");

    if ( !fil )
        return 0;

    fprintf(fil, "%s", callsign);

    FClose(fil);

    return 1;
}

int yw_write_user(FILE *fil, UserData *usr)
{
    _NC_STACK_ypaworld *yw = usr->p_ypaworld;

    if ( usr->callSIGN[0] >= ' ' )
        yw_write_callSign(yw, "callsign.def", usr->callSIGN);

    char buf[300];

    sprintf(buf, "new_user\n");
    fwrite(buf, strlen(buf), 1, fil);

    for (int i = 0; i < 8; i++)
    {
        sprintf(buf, "    playerstatus = %d_%d_%d_%d_%d_%d_%d_%d\n",
                i,
                yw->playerstatus[i].p1,
                yw->playerstatus[i].p2,
                yw->playerstatus[i].p3,
                yw->playerstatus[i].p4,
                yw->playerstatus[i].p5,
                yw->playerstatus[i].p6,
                yw->playerstatus[i].p7);

        fwrite(buf, strlen(buf), 1, fil);
    }

    sprintf(buf, "    maxroboenergy = %d\n", usr->p_ypaworld->maxroboenergy);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    maxreloadconst = %d\n", usr->p_ypaworld->maxreloadconst);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    numbuddies    = %d\n", usr->p_ypaworld->field_2d90->field_74);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    beamenergy    = %d\n", usr->p_ypaworld->beamenergy);
    fwrite(buf, strlen(buf), 1, fil);


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
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "end\n\n");
    fwrite(buf, strlen(buf), 1, fil);

    return 1;
}

int yw_write_input(FILE *fil, UserData *usr)
{
    char buf[300];

    sprintf(buf, "new_input\n");
    fwrite(buf, strlen(buf), 1, fil);

    if ( usr->p_ypaworld->field_73CE & 4 )
        sprintf(buf, "    joystick = no\n");
    else
        sprintf(buf, "    joystick = yes\n");

    fwrite(buf, strlen(buf), 1, fil);

    if ( usr->inp_altjoystick )
        sprintf(buf, "    altjoystick = yes\n");
    else
        sprintf(buf, "    altjoystick = no\n");

    fwrite(buf, strlen(buf), 1, fil);

    if ( usr->p_ypaworld->field_73CE & 8 )
        sprintf(buf, "    forcefeedback = no\n");
    else
        sprintf(buf, "    forcefeedback = yes\n");

    fwrite(buf, strlen(buf), 1, fil);

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
        fwrite(buf, strlen(buf), 1, fil);
    }

    sprintf(buf, "end\n\n");
    fwrite(buf, strlen(buf), 1, fil);

    return 1;
}

int yw_write_sound(FILE *fil, UserData *usr)
{
    char buf[300];

    sprintf(buf, "new_sound\n");
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    volume = %d\n", usr->snd__volume);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    cdvolume = %d\n", usr->snd__cdvolume);
    fwrite(buf, strlen(buf), 1, fil);

    if ( usr->snd__flags2 & 1 )
        sprintf(buf, "    invertlr = yes\n");
    else
        sprintf(buf, "    invertlr = no\n");

    fwrite(buf, strlen(buf), 1, fil);

    if ( usr->snd__flags2 & 0x10 )
        sprintf(buf, "    cdsound = yes\n");
    else
        sprintf(buf, "    cdsound = no\n");

    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "end\n\n");
    fwrite(buf, strlen(buf), 1, fil);

    return 1;
}

int yw_write_video(FILE *fil, UserData *usr)
{
    char buf[300];

    sprintf(buf, "new_video\n");
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    videomode = %d\n", usr->p_ypaworld->game_default_res);
    fwrite(buf, strlen(buf), 1, fil);

    if ( usr->GFX_flags & 1 )
        sprintf(buf, "    farview = yes\n");
    else
        sprintf(buf, "    farview = no\n");

    fwrite(buf, strlen(buf), 1, fil);

    if ( usr->GFX_flags & 0x10 )
        sprintf(buf, "    16bittexture = yes\n");
    else
        sprintf(buf, "    16bittexture = no\n");

    fwrite(buf, strlen(buf), 1, fil);

    if ( usr->GFX_flags & 8 )
        sprintf(buf, "    drawprimitive = yes\n");
    else
        sprintf(buf, "    drawprimitive = no\n");

    fwrite(buf, strlen(buf), 1, fil);

    if ( usr->GFX_flags & 2 )
        sprintf(buf, "    heaven = yes\n");
    else
        sprintf(buf, "    heaven = no\n");

    fwrite(buf, strlen(buf), 1, fil);

    if ( usr->GFX_flags & 4 )
        sprintf(buf, "    softmouse = yes\n");
    else
        sprintf(buf, "    softmouse = no\n");

    fwrite(buf, strlen(buf), 1, fil);

    if ( usr->enemyindicator )
        sprintf(buf, "    enemyindicator = yes\n");
    else
        sprintf(buf, "    enemyindicator = no\n");

    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    fxnumber = %d\n", usr->fxnumber);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "end\n\n");
    fwrite(buf, strlen(buf), 1, fil);

    return 1;
}

int yw_write_level_status(FILE *fil, _NC_STACK_ypaworld *yw, int lvlid)
{
    char buf[300];

    //sprintf(buf, "\nbegin_levelstatus %ld\n", lvlid);
    sprintf(buf, "\nbegin_levelstatus %d\n", lvlid);
    fwrite(buf, strlen(buf), 1, fil);

    //sprintf(buf, "    status = %ld\n", yw->LevelNet->mapInfos[lvlid].field_0);
    sprintf(buf, "    status = %d\n", yw->LevelNet->mapInfos[lvlid].field_0);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "end\n\n");
    fwrite(buf, strlen(buf), 1, fil);

    return 1;
}

int yw_write_levels_statuses(FILE *fil, _NC_STACK_ypaworld *yw)
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

int yw_write_buddy(FILE *fil, _NC_STACK_ypaworld *yw, int buddy_id)
{
    char buf[300];

    sprintf(buf, "\nbegin_buddy\n");
    fwrite(buf, strlen(buf), 1, fil);

    //sprintf(buf, "    commandid = %ld\n", yw->field_2d90->buddies[buddy_id].commandid);
    sprintf(buf, "    commandid = %d\n", yw->field_2d90->buddies[buddy_id].commandid);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    type      = %d\n", yw->field_2d90->buddies[buddy_id].type);
    fwrite(buf, strlen(buf), 1, fil);

    //sprintf(buf, "    energy    = %ld\n", yw->field_2d90->buddies[buddy_id].energy);
    sprintf(buf, "    energy    = %d\n", yw->field_2d90->buddies[buddy_id].energy);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "end\n\n");
    fwrite(buf, strlen(buf), 1, fil);

    return 1;
}

int yw_write_buddies(FILE *fil, _NC_STACK_ypaworld *yw)
{
    for (int i = 0; i < yw->field_2d90->buddies_count; i++ )
    {
        if ( !yw_write_buddy(fil, yw, i) )
            return 0;
    }

    return 1;
}

int yw_write_status(save_status *status, const char *field, FILE *file)
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

    return fwrite(buf, strlen(buf), 1, file);
}

int yw_write_shell(FILE *fil, UserData *usr)
{
    char buf[300];

    _NC_STACK_ypaworld *yw = usr->p_ypaworld;

    sprintf(buf, "new_shell\n");
    fwrite(buf, strlen(buf), 1, fil);

    if ( usr->default_lang_dll )
    {
        sprintf(buf, "    language = %s\n", usr->default_lang_dll->langDllName);
        fwrite(buf, strlen(buf), 1, fil);
    }

    if ( yw->field_739A )
    {
        sprintf(buf, "    finder = na_0_0_0_0\n");
        fwrite(buf, strlen(buf), 1, fil);

        sprintf(buf, "    log    = na_0_0_0_0\n");
        fwrite(buf, strlen(buf), 1, fil);

        sprintf(buf, "    energy = na_0_0_0_0\n");
        fwrite(buf, strlen(buf), 1, fil);

        sprintf(buf, "    message = na_0_0_0_0\n");
        fwrite(buf, strlen(buf), 1, fil);

        sprintf(buf, "    map    = na_0_0_0_0_0_0\n");
        fwrite(buf, strlen(buf), 1, fil);

        yw_write_status(&yw->robo_map_status, "robo_map_status", fil);
        yw_write_status(&yw->robo_finder_status, "robo_finder_status", fil);
        yw_write_status(&yw->vhcl_map_status, "vhcl_map_status", fil);
        yw_write_status(&yw->vhcl_finder_status, "vhcl_finder_status", fil);
    }

    sprintf(buf, "end\n\n");
    fwrite(buf, strlen(buf), 1, fil);

    return 1;
}

int yw_write_item_modifers(_NC_STACK_ypaworld *yw, FILE *fil)
{
    char buf[300];

    for (int i = 0; i < 256; i++)
    {
        if ( yw->VhclProtos[i].model_id )
        {
            sprintf(buf, "modify_vehicle %d\n", i);
            fwrite(buf, strlen(buf), 1, fil);

            for (int j = 1; j < 8; j++)
            {
                if ( yw->VhclProtos[i].disable_enable_bitmask & (1 << j) )
                    sprintf(buf, "    enable         = %d\n", j);
                else
                    sprintf(buf, "    disable        = %d\n", j);

                fwrite(buf, strlen(buf), 1, fil);
            }

            sprintf(buf, "    shield         = %d\n", yw->VhclProtos[i].shield);
            fwrite(buf, strlen(buf), 1, fil);

            sprintf(buf, "    energy         = %d\n", yw->VhclProtos[i].energy);
            fwrite(buf, strlen(buf), 1, fil);

            sprintf(buf, "    num_weapons    = %d\n", yw->VhclProtos[i].num_weapons);
            fwrite(buf, strlen(buf), 1, fil);

            sprintf(buf, "    weapon         = %d\n", yw->VhclProtos[i].weapon);
            fwrite(buf, strlen(buf), 1, fil);

            sprintf(buf, "    radar          = %d\n", yw->VhclProtos[i].radar);
            fwrite(buf, strlen(buf), 1, fil);

            sprintf(buf, "    fire_x         = %4.2f\n", yw->VhclProtos[i].fire_x);
            fwrite(buf, strlen(buf), 1, fil);

            sprintf(buf, "    fire_y         = %4.2f\n", yw->VhclProtos[i].fire_y);
            fwrite(buf, strlen(buf), 1, fil);

            sprintf(buf, "    fire_z         = %4.2f\n", yw->VhclProtos[i].fire_z);
            fwrite(buf, strlen(buf), 1, fil);

            sprintf(buf, "end\n\n");
            fwrite(buf, strlen(buf), 1, fil);
        }
    }

    for (int i = 0; i < 128; i++)
    {
        if ( yw->WeaponProtos[i].field_0 )
        {
            sprintf(buf, "modify_weapon %d\n", i);
            fwrite(buf, strlen(buf), 1, fil);

            for (int j = 1; j < 8; j++)
            {
                if ( yw->WeaponProtos[i].enable_mask & (1 << j) )
                    sprintf(buf, "    enable         = %d\n", j);
                else
                    sprintf(buf, "    disable        = %d\n", j);

                fwrite(buf, strlen(buf), 1, fil);
            }

            sprintf(buf, "    shot_time      = %d\n", yw->WeaponProtos[i].shot_time);
            fwrite(buf, strlen(buf), 1, fil);

            sprintf(buf, "    shot_time_user = %d\n", yw->WeaponProtos[i].shot_time_user);
            fwrite(buf, strlen(buf), 1, fil);

            sprintf(buf, "    energy         = %d\n", yw->WeaponProtos[i].energy);
            fwrite(buf, strlen(buf), 1, fil);

            sprintf(buf, "end\n\n");
            fwrite(buf, strlen(buf), 1, fil);
        }
    }

    for (int i = 0; i < 128; i++)
    {
        if ( yw->BuildProtos[i].type_icon )
        {
            sprintf(buf, "modify_building %d\n", i);
            fwrite(buf, strlen(buf), 1, fil);

            for (int j = 1; j < 8; j++)
            {
                if ( yw->BuildProtos[i].enable_mask & (1 << j) )
                    sprintf(buf, "    enable         = %d\n", j);
                else
                    sprintf(buf, "    disable        = %d\n", j);

                fwrite(buf, strlen(buf), 1, fil);
            }

            sprintf(buf, "end\n\n");
            fwrite(buf, strlen(buf), 1, fil);
        }
    }

    return 1;
}

int yw_write_levelnum(_NC_STACK_ypaworld *yw, FILE *fil)
{
    fprintf(fil, "\nbegin_levelnum\n");
    fprintf(fil, "    levelnum = %d\n", yw->field_2d90->levelID);
    fprintf(fil, "end\n");

    return 1;
}

void yw_write_map(_NC_STACK_ypaworld *yw, NC_STACK_bitmap *bitmap, const char *padding, FILE *fil)
{
    bitmap_intern *bitm = bitmap->getBMD_pBitmap();

    if ( padding )
        fprintf(fil, padding);

    fprintf(fil, "%d %d\n", bitm->width, bitm->height);

    uint8_t *v6 = (uint8_t *)bitm->buffer;

    for (int y = 0; y < bitm->height; y++)
    {
        if ( padding )
            fprintf(fil, padding);

        for (int x = 0; x < bitm->width; x++ )
        {
            fprintf(fil, "%02x ", *v6);
            v6++;
        }

        fprintf(fil, "\n");
    }
}

void yw_write_ownermap(_NC_STACK_ypaworld *yw, FILE *fil)
{
    NC_STACK_bitmap *bitmap = (NC_STACK_bitmap *)init_get_class("bitmap.class", 0x80001000, "temp_owner_map", 0x80002002, yw->sectors_maxX2, 0x80002003, yw->sectors_maxY2, 0);

    if ( bitmap )
    {
        bitmap_intern *bitm = bitmap->getBMD_pBitmap();

        int sz = yw->sectors_maxY2 * yw->sectors_maxX2;
        uint8_t *outbf = (uint8_t *)bitm->buffer;

        for (int i = 0; i < sz; i++)
            outbf[i] = yw->cells[i].owner;

        fprintf(fil, "\nbegin_ownermap\n");
        yw_write_map(yw, bitmap, "        ", fil);
        fprintf(fil, "end\n");

        delete_class_obj(bitmap);
    }
}

void yw_write_buildmap(_NC_STACK_ypaworld *yw, FILE *fil)
{
    fprintf(fil, "\nbegin_buildingmap\n");
    yw_write_map(yw, yw->blg_map, "        ", fil);
    fprintf(fil, "end\n");
}

void yw_write_energymap(_NC_STACK_ypaworld *yw, FILE *fil)
{
    NC_STACK_bitmap *bitmap = (NC_STACK_bitmap *)init_get_class("bitmap.class", 0x80001000, "ActualEnergyMap", 0x80002002, 3 * yw->sectors_maxX2, 0x80002003, 3 * yw->sectors_maxY2, 0);

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

        fprintf(fil, "\nbegin_energymap\n");
        yw_write_map(yw, bitmap, "        ", fil);
        fprintf(fil, "end\n");
    }
    else
    {
        ypa_log_out("game save error: Unable to create bmo for saving owner\n");
    }

    if ( bitmap )
        delete_class_obj(bitmap);
}

int yw_write_bact(bact_node *bct, FILE *fil)
{
    char buf[300];

    int a4 = bct->bacto->getBACT_viewer();

    if ( a4 )
        sprintf(buf, "    viewer         = yes\n");
    else
        sprintf(buf, "    viewer         = no\n");

    fwrite(buf, strlen(buf), 1, fil);

    a4 = bct->bacto->getBACT_inputting();

    if ( a4 )
        sprintf(buf, "    user           = yes\n");
    else
        sprintf(buf, "    user           = no\n");

    fwrite(buf, strlen(buf), 1, fil);

    a4 = bct->bacto->getBACT_bactCollisions();

    if ( a4 )
        sprintf(buf, "    collision      = yes\n");
    else
        sprintf(buf, "    collision      = no\n");

    fwrite(buf, strlen(buf), 1, fil);
    sprintf(buf, "    energy         = %d\n", bct->bact->energy);

    fwrite(buf, strlen(buf), 1, fil);
    sprintf(buf, "    speed          = %6.5f_%6.5f_%6.5f_%6.5f\n", bct->bact->field_605.sx, bct->bact->field_605.sy, bct->bact->field_605.sz, bct->bact->field_611);

    fwrite(buf, strlen(buf), 1, fil);

    sprintf(
        buf,
        "    matrix         = %6.5f_%6.5f_%6.5f_%6.5f_%6.5f_%6.5f_%6.5f_%6.5f_%6.5f\n",
        bct->bact->field_651.m00,
        bct->bact->field_651.m01,
        bct->bact->field_651.m02,
        bct->bact->field_651.m10,
        bct->bact->field_651.m11,
        bct->bact->field_651.m12,
        bct->bact->field_651.m20,
        bct->bact->field_651.m21,
        bct->bact->field_651.m22);

    fwrite(buf, strlen(buf), 1, fil);

    if ( bct->bact->field_24 == 3 )
    {
        NC_STACK_yparobo *roboo = dynamic_cast<NC_STACK_yparobo *>(bct->bacto);
        sprintf(
            buf,
            "    pos            = %2.2f_%2.2f_%2.2f\n",
            bct->bact->field_621.sx,
            roboo->stack__yparobo.field_1D5,
            bct->bact->field_621.sz);
    }
    else
        sprintf(
            buf,
            "    pos            = %2.2f_%2.2f_%2.2f\n",
            bct->bact->field_621.sx,
            bct->bact->field_621.sy,
            bct->bact->field_621.sz);

    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    force          = %2.2f\n", bct->bact->field_601);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    gunangle       = %5.4f\n", bct->bact->gun_angle2);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    commandID      = %d\n", bct->bact->field_2E);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    aggression     = %d\n", bct->bact->field_3D4);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    mainstate      = %d\n", bct->bact->field_3D5);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    extrastate     = %d\n", bct->bact->field_3D6);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    ident          = %d\n", bct->bact->ypabact__id);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    killerowner    = %d\n", bct->bact->field_9B5);
    fwrite(buf, strlen(buf), 1, fil);

    if ( bct->bact->primTtype == BACT_TGT_TYPE_UNIT )
        sprintf(buf, "    primary        = %d_%d_%2.2f_%2.2f_%d\n", bct->bact->primTtype, bct->bact->primT.pbact->ypabact__id, bct->bact->primTpos.sx, bct->bact->primTpos.sz, bct->bact->primT_cmd_id);
    else
        sprintf(buf, "    primary        = %d_0_%2.2f_%2.2f_%d\n", bct->bact->primTtype, bct->bact->primTpos.sx, bct->bact->primTpos.sz, bct->bact->primT_cmd_id);

    fwrite(buf, strlen(buf), 1, fil);

    for (int i = 0; i < bct->bact->field_59A; i++)
    {
        sprintf(buf, "    waypoint       = %d_%2.2f_%2.2f\n", i, bct->bact->field_418[i].sx, bct->bact->field_418[i].sz);
        fwrite(buf, strlen(buf), 1, fil);
    }

    sprintf(buf, "    num_wp         = %d\n", bct->bact->field_59A);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    count_wp       = %d\n", bct->bact->field_598);
    fwrite(buf, strlen(buf), 1, fil);

    return 1;
}

int yw_write_robo(bact_node *bct, FILE *fil)
{
    char buf[300];

    sprintf(buf, "\nbegin_robo %d\n", bct->bact->id);
    fwrite(buf, strlen(buf), 1, fil);

    if ( !yw_write_bact(bct, fil) )
        return 0;

    sprintf(buf, "    owner          = %d\n", bct->bact->owner);
    fwrite(buf, strlen(buf), 1, fil);

    NC_STACK_yparobo *roboo = dynamic_cast<NC_STACK_yparobo *>(bct->bacto);
    __NC_STACK_yparobo *robo = &roboo->stack__yparobo;

    sprintf(buf, "    robostate      = %d\n", robo->roboState & 0xC00F );
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    dockenergy     = %d\n", robo->dock_energ);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    dockcount      = %d\n", robo->dock_cnt);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    dockuser       = %d\n", robo->dock_user);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    docktime       = %d\n", robo->dock_time);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    docktargetpos  = %2.2f_%2.2f\n", robo->dock_tgt_pos.sx, robo->dock_tgt_pos.sz);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    docktargetID   = %d\n", robo->dock_tgt_comm_id);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    docktargettype = %d\n", robo->dock_tgType);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    dockaggr       = %d\n", robo->dock_aggr);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    battvehicle    = %d\n", robo->field_4F5);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    battbeam       = %d\n", robo->field_4FD);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    fillmodus      = %d\n", robo->field_501);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    maximum        = %d\n", robo->bact_internal->energy_2);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    buildspare     = %d\n", robo->field_509);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    vhoriz         = %7.5f\n", robo->bact_internal->field_5ED);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    vvert          = %7.5f\n", robo->bact_internal->field_5F1);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    con_budget     = %d\n", robo->epConquer);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    def_budget     = %d\n", robo->epDefense);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    rec_budget     = %d\n", robo->epRecon);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    rob_budget     = %d\n", robo->epRobo);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    rad_budget     = %d\n", robo->epRadar);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    pow_budget     = %d\n", robo->epPower);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    saf_budget     = %d\n", robo->epSafety);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    cpl_budget     = %d\n", robo->epChangePlace);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    saf_delay     = %d\n", robo->safety_delay);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    pow_delay     = %d\n", robo->field_265);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    rad_delay     = %d\n", robo->radar_delay);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    cpl_delay     = %d\n", robo->field_2B1);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    def_delay     = %d\n", robo->field_281);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    con_delay     = %d\n", robo->conq_delay);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    rec_delay     = %d\n", robo->field_2C9);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    rob_delay     = %d\n", robo->field_2E1);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    reload_const  = %d\n", robo->bact_internal->reload_const_or_energy2);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "end\n\n");
    fwrite(buf, strlen(buf), 1, fil);

    return 1;
}

int yw_write_gun(bact_node *bct, FILE *fil)
{
    char buf[300];

    if ( bct->bact->field_24 == 9 )
    {
        NC_STACK_ypagun *guno = (NC_STACK_ypagun *)bct->bacto;

        sprintf(
            buf,
            "    gunbasis = %7.6f_%7.6f_%7.6f\n",
            guno->stack__ypagun.dir.sx,
            guno->stack__ypagun.dir.sy,
            guno->stack__ypagun.dir.sz);

        fwrite(buf, strlen(buf), 1, fil);
    }
    return 1;
}

int yw_write_commander(bact_node *bct, FILE *fil)
{
    char buf[300];

    sprintf(buf, "\nbegin_commander %d\n", bct->bact->id);
    fwrite(buf, strlen(buf), 1, fil);

    if ( !yw_write_bact(bct, fil) )
        return 0;

    if ( !yw_write_gun(bct, fil) )
        return 0;

    sprintf(buf, "end\n\n");
    fwrite(buf, strlen(buf), 1, fil);

    return 1;
}

int yw_write_slave(bact_node *bct, FILE *fil)
{
    char buf[300];

    sprintf(buf, "\nbegin_slave %d\n", bct->bact->id);
    fwrite(buf, strlen(buf), 1, fil);

    if ( !yw_write_bact(bct, fil) )
        return 0;

    if ( !yw_write_gun(bct, fil) )
        return 0;

    sprintf(buf, "end\n\n");
    fwrite(buf, strlen(buf), 1, fil);

    return 1;
}

int yw_write_extraviewer(bact_node *bct, FILE *fil)
{
    char buf[300];

    if ( bct->bact->field_24 == 9 )
    {
        int v7 = -1;

        sprintf(buf, "\nbegin_extraviewer\n");
        fwrite(buf, strlen(buf), 1, fil);

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
            fwrite(buf, strlen(buf), 1, fil);

            sprintf(buf, "    number = %d\n", v7);
            fwrite(buf, strlen(buf), 1, fil);

            sprintf(buf, "end\n\n");
            fwrite(buf, strlen(buf), 1, fil);
            return 1;
        }

        ypa_log_out("Error: Gun not found in yrd!\n");
    }
    return 0;
}

int yw_write_units(_NC_STACK_ypaworld *yw, FILE *fil)
{
    bact_node *station = (bact_node *)yw->bact_list.tailpred;
    while (station->prev)
    {
        if ( station->bact->field_3D5 != 2 )
        {
            if ( !yw_write_robo(station, fil) )
                return 0;

            bact_node *commander = (bact_node *)station->bact->list2.tailpred;

            while ( commander->prev )
            {
                int a4 = 0;

                if ( commander->bact->field_24 == 9 )
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

                bact_node *slave = (bact_node *)commander->bact->list2.tailpred;

                while ( slave->prev )
                {
                    int v9 = 0;
                    if ( slave->bact->field_24 == 9 )
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

int yw_write_wunderinfo(_NC_STACK_ypaworld *yw, FILE *fil)
{
    char buf[300];

    sprintf(buf, "\nbegin_wunderinfo\n");
    fwrite(buf, strlen(buf), 1, fil);

    for (int i = 0; i < 8; i++)
    {
        gemProto *gem = &yw->gems[i];

        if ( gem->field_0 )
        {
            if ( yw->cells[gem->sec_x + gem->sec_y * yw->sectors_maxX2].w_type != 4 )
            {
                sprintf(buf, "    disablegem %d\n", i);
                fwrite(buf, strlen(buf), 1, fil);
            }
        }
    }

    sprintf(buf, "end\n\n");
    fwrite(buf, strlen(buf), 1, fil);
    return 1;
}

int yw_write_kwfactor(_NC_STACK_ypaworld *yw, FILE *fil)
{
    char buf[300];

    sprintf(buf, "\nbegin_kwfactor\n");
    fwrite(buf, strlen(buf), 1, fil);

    for (int i = 0; i < 256; i++)
    {
        yw_field34 *kw = &yw->field_34[i];
        if ( kw->p_cell )
        {
            sprintf(buf, "    kw = %d_%d_%d\n", kw->x, kw->y, kw->power_2);
            fwrite(buf, strlen(buf), 1, fil);
        }
    }

    sprintf(buf, "end\n\n");
    fwrite(buf, strlen(buf), 1, fil);
    return 1;
}

int yw_write_globals(_NC_STACK_ypaworld *yw, FILE *fil)
{
    char buf[300];

    sprintf(buf, "\nbegin_globals\n");
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "    time = %d\n", yw->field_1614);
    fwrite(buf, strlen(buf), 1, fil);

    sprintf(buf, "end\n\n");
    fwrite(buf, strlen(buf), 1, fil);
    return 1;
}

int yw_write_superbomb(_NC_STACK_ypaworld *yw, FILE *fil)
{
    for (int i = 0; i < yw->field_2d90->supetItems_count; i++)
    {
        fprintf(fil, "\nbegin_superbomb\n");
        fprintf(fil, "    num               = %d\n", i);
        fprintf(fil, "    status            = %d\n", yw->field_2d90->supetItems[i].field_4);
        fprintf(fil, "    active_timestamp  = %d\n", yw->field_2d90->supetItems[i].field_EC);
        fprintf(fil, "    trigger_timestamp = %d\n", yw->field_2d90->supetItems[i].field_F0);
        fprintf(fil, "    activated_by      = %d\n", yw->field_2d90->supetItems[i].field_F4);
        fprintf(fil, "    countdown         = %d\n", yw->field_2d90->supetItems[i].field_F8);
        fprintf(fil, "    last_ten_sec      = %d\n", yw->field_2d90->supetItems[i].field_FC);
        fprintf(fil, "    last_sec          = %d\n", yw->field_2d90->supetItems[i].field_100);
        fprintf(fil, "    radius            = %d\n", yw->field_2d90->supetItems[i].field_104);
        fprintf(fil, "    last_radius       = %d\n", yw->field_2d90->supetItems[i].field_108);
        fprintf(fil, "end\n");
    }

    return 1;
}

int yw_write_histbuffer(yw_f726c_nod *hist_node, FILE *fil)
{
    uint8_t *bufpos = hist_node->bufStart;
    int lines = (hist_node->bufEnd - hist_node->bufStart) / 64;

    fprintf(fil, "    history_buffer = \n");
    fprintf(fil, "    %d %d\n", 64, lines);

    for (int i = 0; i < lines; i++)
    {
        fprintf(fil, "    ");

        for (int j = 0; j < 64; j++)
        {
            fprintf(fil, "%02x ", *bufpos);
            bufpos++;
        }

        fprintf(fil, "\n");
    }

    fprintf(fil, "\n");
    return 1;
}

int yw_write_history(_NC_STACK_ypaworld *yw, FILE *fil)
{
    if ( yw->history )
    {
        fprintf(fil, ";------------------------------------------------------------\n");
        fprintf(fil, "; History Buffers\n");
        fprintf(fil, ";------------------------------------------------------------\n");
        fprintf(fil, "begin_history\n");

        yw_f726c_nod *histnode = (yw_f726c_nod *)yw->history->lst.head;

        while ( histnode->next )
        {
            yw_write_histbuffer(histnode, fil);

            histnode = (yw_f726c_nod *)histnode->next;
        }

        fprintf(fil, "end");
    }

    return 1;
}

int yw_write_masks(_NC_STACK_ypaworld *yw, FILE *fil)
{
    fprintf(fil, "\nbegin_masks\n");
    fprintf(fil, "    ownermask = %d\n", yw->field_2d90->ownerMap__has_vehicles);
    fprintf(fil, "    usermask  = %d\n", yw->field_2d90->field_60);
    fprintf(fil, "end\n");
    return 1;
}
