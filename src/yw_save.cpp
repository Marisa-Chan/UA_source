#include <inttypes.h>
#include <string.h>
#include "includes.h"
#include "def_parser.h"
#include "yw_internal.h"
#include "yw.h"
#include "input.h"

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
