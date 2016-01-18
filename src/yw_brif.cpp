#include <inttypes.h>
#include <string.h>

#include "includes.h"
#include "def_parser.h"
#include "yw.h"
#include "yw_internal.h"
#include "lstvw.h"
#include "font.h"

extern listview stru_5C91D0;

void ypaworld_func158__sub4__sub1__sub4__sub3(_NC_STACK_ypaworld *yw, struC5 *inpt, big_ypa_Brf *brf)
{
    CDAUDIO_t cd_audio;
    cd_audio.command = 1;
    sub_4444D4(&cd_audio);

    if ( yw->GameShell->missiontrack )
    {
        cd_audio.command = 7;
        cd_audio.track_id = yw->GameShell->missiontrack;
        cd_audio.field_8 = yw->GameShell->missiontrack__adv.field_0;
        cd_audio.field_C = yw->GameShell->missiontrack__adv.field_4;
        sub_4444D4(&cd_audio);

        cd_audio.command = 2;
        sub_4444D4(&cd_audio);
    }

    brf->field_2E78 = brf->field_2E7C;
    brf->field_2E68 = 5;

    call_vtbl(brf->mbmap_img, 3, 0x80002000, &brf->field_2F40.pbitm, 0);

    brf->field_2F64 = -0.4515625;
    brf->field_2F68 = -0.324999988079;

    brf->field_2F6C = brf->field_2F64;
    brf->field_2F70 = brf->field_2F68;

    float v17, v16;

    if ( yw->sectors_maxX2 == yw->sectors_maxY2 )
    {
        v17 = 1.0;
        v16 = 1.0;
    }
    else if (yw->sectors_maxX2 < yw->sectors_maxY2)
    {
        v16 = 1.0;
        v17 = (float)yw->sectors_maxX2 / (float)yw->sectors_maxY2;
    }
    else if (yw->sectors_maxX2 > yw->sectors_maxY2)
    {
        v17 = 1.0;
        v16 = (float)yw->sectors_maxY2 / (float)yw->sectors_maxX2;
    }

    brf->field_2F40.float4 = -1.0;
    brf->field_2F40.float8 = -1.0;
    brf->field_2F40.floatC = 1.0;
    brf->field_2F40.float10 = 1.0;

    brf->field_2F74 = -0.4515625 - v17 * 0.4828125;
    brf->field_2F78 = -0.325 - v16 * 0.6083333333333334;
    brf->field_2F7C = brf->field_2F74 + v17 * 0.965625;
    brf->field_2F80 = brf->field_2F78 + v16 * 1.216666666666667;
    brf->field_2F40.float14 = brf->field_2F64;
    brf->field_2F40.float18 = brf->field_2F68;
    brf->field_2F40.float1C = brf->field_2F6C;
    brf->field_2F40.float20 = brf->field_2F70;
    brf->field_2E80 = 0;
    brf->field_2E84 = brf->field_2E7C;
}

void ypaworld_func158__sub4__sub1__sub4__sub4(_NC_STACK_ypaworld *yw, struC5 *inpt, big_ypa_Brf *brf)
{
    int v5 = brf->field_2E7C - brf->field_2E78;

    if ( v5 >= 600 )
    {
        brf->field_2F40.float14 = brf->field_2F74;
        brf->field_2F40.float18 = brf->field_2F78;
        brf->field_2F40.float1C = brf->field_2F7C;
        brf->field_2F40.float20 = brf->field_2F80;
        brf->field_2E68 = 6;

        displ_arg263 v4;
        v4.bitm = yw->pointers__bitm[5];
        v4.pointer_id = 6;

        call_method(yw->win3d, 263, &v4);
    }
    else
    {
        float v3 = v5 * 0.0016666667;
        brf->field_2F40.float14 = (brf->field_2F74 - brf->field_2F64) * v3 + brf->field_2F64;
        brf->field_2F40.float18 = (brf->field_2F78 - brf->field_2F68) * v3 + brf->field_2F68;
        brf->field_2F40.float1C = (brf->field_2F7C - brf->field_2F6C) * v3 + brf->field_2F6C;
        brf->field_2F40.float20 = (brf->field_2F80 - brf->field_2F70) * v3 + brf->field_2F70;
    }
}

int yw_MBLoadSet(_NC_STACK_ypaworld *yw, int setID)
{
    char rsr[256];
    strcpy(rsr, get_prefix_replacement("rsrc"));

    char a1a[256];
    sprintf(a1a, "data:set%d:", setID);

    set_prefix_replacement("rsrc", a1a);

    if ( setID != yw->set_number && setID != 46 )
    {
        if ( yw->additionalSet )
        {
            ypa_log_out("yw_MBLoadSet(): killing set object %d\n", yw->set_number);
            delete_class_obj(yw->additionalSet);
            yw->additionalSet = NULL;
            yw->set_number = 0;
        }

        yw->additionalSet = load_set_base();
        if ( !yw->additionalSet )
        {
            ypa_log_out("yw_MBLoadSet(): loading set object %d failed\n", setID);
            set_prefix_replacement("rsrc", rsr);
            return 0;
        }

        yw->set_number = setID;
        ypa_log_out("yw_MBLoadSet(): loaded set object %d ok\n", setID);
    }

    if ( setID != 46 )
    {
        FILE *fil = FOpen("rsrc:scripts/set.sdf", "r");
        if ( !fil )
        {
            ypa_log_out("Briefing: no set description file.\n");
            return 0;
        }

        nlist *kid_list;

        call_vtbl(yw->additionalSet, 3, 0x8000101A, &kid_list, 0);// Get KIDS

        int kid_id = 0;

        for (base_node *bnode = (base_node *)kid_list->head; bnode->next; bnode = (base_node *)bnode->next)
        {
            if ( kid_id == 0 )
            {
                if ( !sub_44A12C(yw, bnode->self_full) )
                {
                    FClose(fil);
                    return 0;
                }
            }
            else if ( kid_id == 1 )
            {
                if ( !yw_parse_lego(yw, fil, bnode->self_full) )
                {
                    FClose(fil);
                    return 0;
                }

                if ( !yw_parse_subSect(yw, fil) )
                {
                    FClose(fil);
                    return 0;
                }

                if ( !yw_parse_sektor(yw, fil) )
                {
                    FClose(fil);
                    return 0;
                }
            }
            else if ( kid_id == 2 )
            {
                if ( !sub_44A97C(yw, bnode->self_full) )
                {
                    FClose(fil);
                    return 0;
                }
            }

            kid_id++;
        }

        FClose(fil);
    }

    set_prefix_replacement("rsrc", rsr);

    return 1;
}

void ypaworld_func158__sub4__sub1__sub4__sub5(_NC_STACK_ypaworld *yw, struC5 *inpt, big_ypa_Brf *brf)
{
    brf->field_2F84 = 1;
    brf->field_2E80 = brf->field_2E7C;
    brf->tp2_count = 0;

    if ( yw_MBLoadSet(yw, brf->map_prototype.setNumber) )
    {
        brf->field_2E68 = 7;

        displ_arg263 v6;
        v6.bitm = yw->pointers__bitm[0];
        v6.pointer_id = 1;

        call_method(yw->win3d, 263, &v6);
    }
    else
    {
        brf->field_2E68 = 1;
    }
}

void ypaworld_func158__sub4__sub1__sub4__sub6(_NC_STACK_ypaworld *yw, struC5 *inpt, big_ypa_Brf *brf)
{
    brf_obj *brobj = &brf->brf_objs;
    brobj->field_0 = 0;
    brobj->object_id = 0;

    brf->field_2E70 = -1;
    brf->field_2E74 = 1;

    brf->field_2E68 = 8;
    brf->field_2E78 = brf->field_2E7C;

    brf->field_2F8C = -1;
    brf->field_2F90 = -1;
    brf->field_2E88 = get_lang_string(yw->string_pointers_p2, 150, "YOU ARE HERE");
}

int sub_4EBBA8(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, float xpos, float ypos, int a5, const char *title, int a7, int a8, int a9, int vehicle_id, int a11)
{
    brf_obj *brobj = &brf->brf_objs;

    brf_t2 *brtp2 = &brf->tp2[brf->tp2_count];

    brobj->field_C = brf->field_2E7C;
    brobj->xpos = xpos;
    brobj->ypos = ypos;
    brobj->field_2C = a5;
    brobj->field_0 = a9;

    brobj->object_id = vehicle_id;
    memset(brobj->title, 0, 128);
    strcpy(brobj->title, title);

    if ( a11 )
    {
        brtp2->xpos = xpos;
        brtp2->ypos = ypos;
        brtp2->field_C = a7;
        brtp2->field_D = a8;
        brtp2->field_E = a5;

        brtp2->field_8 = a9;
        brtp2->vehicle_id = vehicle_id;
        memset(brtp2->title, 0, 128);
        strcpy(brtp2->title, title);

        brf->tp2_count++;

        if ( brf->tp2_count >= 32 )
            brf->tp2_count = 31;
    }

    if ( yw->GameShell )
        sub_423F74(&yw->GameShell->samples1_info, 11);

    return 0;
}

void ypaworld_func158__sub4__sub1__sub4__sub7(_NC_STACK_ypaworld *yw, struC5 *inpt, big_ypa_Brf *brf)
{
    if ( brf->field_2E7C - brf->field_2E78 >= 2500 )
    {
        brf->field_2E68 = 9;
    }
    else if ( brf->field_2E70 )
    {
        mapRobo *robo = brf->map_prototype.mapRobos;
        const char *v6 = get_lang_string(yw->string_pointers_p2, robo[0].vehicle + 1200, yw->VhclProtos[ robo[0].vehicle ].name);

        brf->field_2E70 = 0;
        sub_4EBBA8(yw, brf, robo[0].pos_x, robo[0].pos_z, 25, v6, 26, robo[0].owner + 128, 2, robo[0].vehicle, brf->field_2F84);
    }
}

void ypaworld_func158__sub4__sub1__sub4__sub8(_NC_STACK_ypaworld *yw, struC5 *inpt, big_ypa_Brf *brf)
{
    if ( brf->field_2F90 )
    {
        brf->brf_objs.object_id = 0;
        brf->brf_objs.field_0 = 0;
    }

    brf->field_2E70 = -1;
    brf->field_2E74 = 0;
    brf->field_2E78 = brf->field_2E7C;

    brf->field_2E88 = get_lang_string(yw->string_pointers_p2, 151, "PRIMARY TARGETS");

    for (int i = 0; i < yw->field_2d90->gate_count; i++ )
    {
        gateProto *v9 = &yw->field_2d90->gates[i];

        if ( v9->mb_status != 1 )
            brf->field_2E74 += v9->keySectors_count;
    }

    brf->field_2E68 = (brf->field_2E74 == 0) + 11;
}

void ypaworld_func158__sub4__sub1__sub4__sub9(_NC_STACK_ypaworld *yw, struC5 *inpt, big_ypa_Brf *brf)
{
    int v5 = (brf->field_2E7C - brf->field_2E78) / 2500;

    if ( v5 >= brf->field_2E74 )
    {
        brf->field_2E68 = 12;
    }
    else if ( v5 != brf->field_2E70 )
    {
        brf->field_2E70 = v5;

        for (int i = 0; i <  yw->field_2d90->gate_count ; i++ )
        {

            if ( yw->field_2d90->gates[i].mb_status != 1 )
            {
                for (int j = 0; j < yw->field_2d90->gates[i].keySectors_count; j++)
                {
                    keysec *v8 = &yw->field_2d90->gates[i].keySectors[j];

                    if ( v5 == j)
                    {
                        float xpos = v8->x * 1200.0 + 600.0;
                        float ypos = -(v8->y * 1200.0 + 600.0);
                        uint8_t v12 = *( (uint8_t *)brf->typ_map_bitm->buffer + v8->x + v8->y * brf->typ_map_bitm->width );
                        const char *v13 = get_lang_string(yw->string_pointers_p2, 157, "KEY SECTOR");

                        sub_4EBBA8(yw, brf, xpos, ypos, 25, v13, 26, 146, 1, v12, brf->field_2F84);
                    }
                }
            }
        }
    }
}

void ypaworld_func158__sub4__sub1__sub4__sub10(_NC_STACK_ypaworld *yw, struC5 *inpt, big_ypa_Brf *brf)
{
    if ( brf->field_2F90 )
    {
        brf->brf_objs.object_id = 0;
        brf->brf_objs.field_0 = 0;
    }

    brf->field_2E70 = -1;
    brf->field_2E74 = 0;
    brf->field_2E78 = brf->field_2E7C;
    brf->field_2E88 = get_lang_string(yw->string_pointers_p2, 152, "TECHNOLOGY UPGRADES");

    for (int i = 0; i < 8; i++)
    {
        if (yw->gems[i].field_0)
        {
            if ( yw->gems[i].mb_status != 1 )
                brf->field_2E74++;
        }
    }

    brf->field_2E68 = (brf->field_2E74 == 0) + 14;
}

void ypaworld_func158__sub4__sub1__sub4__sub11(_NC_STACK_ypaworld *yw, struC5 *inpt, big_ypa_Brf *brf)
{
    int v5 = (brf->field_2E7C - brf->field_2E78) / 2500;
    if ( v5 >= brf->field_2E74 )
    {
        brf->field_2E68 = 15;
    }
    else if ( v5 != brf->field_2E70 )
    {
        brf->field_2E70 = v5;
        int v6 = v5;

        gemProto * v9 = yw->gems;

        for (int i = 0; i < 8; i++)
        {
            v9 = &yw->gems[i];

            if (yw->gems[i].field_0)
            {
                if (yw->gems[i].mb_status != 1)
                {
                    if (v6 == 0)
                        break;

                    v6--;
                }
            }
        }

        float xpos = v9->sec_x * 1200.0 + 600.0;
        float ypos = -(v9->sec_y * 1200.0 + 600.0);
        int v13 = yw->BuildProtos[v9->building].sec_type;
        const char *v14 = get_lang_string(yw->string_pointers_p2, 158, "TECH UPGRADE");

        sub_4EBBA8(yw, brf, xpos, ypos, 25, v14, 26, 144, 1, v13, brf->field_2F84);
    }
}

void ypaworld_func158__sub4__sub1__sub4__sub12(_NC_STACK_ypaworld *yw, struC5 *inpt, big_ypa_Brf *brf)
{
    if ( brf->field_2F90 )
    {
        brf->brf_objs.object_id = 0;
        brf->brf_objs.field_0 = 0;
    }

    brf->field_2E70 = -1;
    brf->field_2E78 = brf->field_2E7C;

    brf->field_2E88 = get_lang_string(yw->string_pointers_p2, 153, "ENEMY DEFENSE STATIONS");
    brf->field_2E74 = 0;

    for (int i = 1; i < brf->map_prototype.mapRobos_count; i++)
    {
        if (brf->map_prototype.mapRobos[i].mb_status != 1)
            brf->field_2E74++;
    }

    brf->field_2E68 = (brf->field_2E74 == 0) + 17;
}

void ypaworld_func158__sub4__sub1__sub4__sub13(_NC_STACK_ypaworld *yw, struC5 *inpt, big_ypa_Brf *brf)
{
    int v5 = (brf->field_2E7C - brf->field_2E78) / 2500;

    if ( v5 >= brf->field_2E74 )
    {
        brf->field_2E68 = 18;
    }
    else if ( v5 != brf->field_2E70 )
    {
        int v6 = v5;
        brf->field_2E70 = v5;

        mapRobo *v9 = &brf->map_prototype.mapRobos[1];

        for (int i = 1; i < brf->map_prototype.mapRobos_count; i++)
        {
            v9 = &brf->map_prototype.mapRobos[i];

            if (brf->map_prototype.mapRobos[i].mb_status != 1)
            {
                if (v6 == 0)
                    break;

                v6--;
            }
        }

        const char *v11 = get_lang_string(yw->string_pointers_p2, v9->vehicle + 1200, yw->VhclProtos[ v9->vehicle ].name);
        sub_4EBBA8(yw, brf, v9->pos_x, v9->pos_z, 25, v11, 26, v9->owner + 128, 2, v9->vehicle, brf->field_2F84);
    }
}

void ypaworld_func158__sub4__sub1__sub4__sub14(_NC_STACK_ypaworld *yw, struC5 *inpt, big_ypa_Brf *brf)
{
    if ( brf->field_2F90 )
    {
        brf->brf_objs.object_id = 0;
        brf->brf_objs.field_0 = 0;
    }

    brf->field_2E70 = -1;
    brf->field_2E74 = 0;
    brf->field_2E78 = brf->field_2E7C;

    brf->field_2E88 = get_lang_string(yw->string_pointers_p2, 154, "ENEMY FORCES");

    for (int i = 0; i < brf->map_prototype.squad_count; i++)
    {
        if ( brf->map_prototype.squads[i].owner != 1 && brf->map_prototype.squads[i].mb_status != 1)
            brf->field_2E74++;
    }

    brf->field_2E68 = (brf->field_2E74 == 0) + 20;
}

void ypaworld_func158__sub4__sub1__sub4__sub15(_NC_STACK_ypaworld *yw, struC5 *inpt, big_ypa_Brf *brf)
{
    int v5 = (brf->field_2E7C - brf->field_2E78) / 2500;
    if ( v5 >= brf->field_2E74 )
    {
        brf->field_2E68 = 21;
    }
    else if ( v5 != brf->field_2E70 )
    {
        brf->field_2E70 = v5;
        int v6 = v5;

        squadProto *v9 = NULL;

        for (int i = 0; i < brf->map_prototype.squad_count; i++)
        {
            v9 = &brf->map_prototype.squads[i];

            if ( brf->map_prototype.squads[i].owner != 1 && brf->map_prototype.squads[i].mb_status != 1)
            {
                if ( v6 == 0 )
                    break;
                v6--;
            }
        }

        if ( v9 )
        {
            const char *v11 = get_lang_string(yw->string_pointers_p2, v9->vehicle + 1200, yw->VhclProtos[v9->vehicle].name);

            char a1[128];
            sprintf(a1, "%d %s", v9->num, v11);

            sub_4EBBA8(yw, brf, v9->pos_x, v9->pos_z, 36, a1, 26, v9->owner + 136, 2, v9->vehicle, brf->field_2F84);
        }
    }
}

void ypaworld_func158__sub4__sub1__sub4__sub16(_NC_STACK_ypaworld *yw, struC5 *inpt, big_ypa_Brf *brf)
{
    if ( brf->field_2F90 )
    {
        brf->brf_objs.object_id = 0;
        brf->brf_objs.field_0 = 0;
    }
    brf->field_2E70 = -1;
    brf->field_2E74 = 0;
    brf->field_2E68 = 23;
    brf->field_2E88 = "FRIENDLY SUPPORT FORCES";

    brf->field_2E78 = brf->field_2E7C;

    for (int i = 0; i < brf->map_prototype.squad_count; i++)
    {
        squadProto *v5 = &brf->map_prototype.squads[i];

        if ( v5->owner == 1 && v5->mb_status != 1)
            brf->field_2E74++;
    }

    brf->field_2E68 = (brf->field_2E74 == 0) + 23;
}

void ypaworld_func158__sub4__sub1__sub4__sub17(_NC_STACK_ypaworld *yw, struC5 *inpt, big_ypa_Brf *brf)
{
    int v5 = (brf->field_2E7C - brf->field_2E78) / 2500;
    if ( v5 >= brf->field_2E74 )
    {
        brf->field_2E68 = 24;
    }
    else if ( v5 != brf->field_2E70 )
    {
        int v6 = v5;
        brf->field_2E70 = v5;

        squadProto *v9 = NULL;

        for (int i = 0; i < brf->map_prototype.squad_count; i++)
        {
            v9 = &brf->map_prototype.squads[i];

            if ( v9->owner == 1 && v9->mb_status != 1)
            {
                if ( v6 == 0 )
                    break;

                v6--;
            }
        }

        if ( v9 )
        {
            const char *v11 = get_lang_string(yw->string_pointers_p2, v9->vehicle + 1200, yw->VhclProtos[v9->vehicle].name);

            char title[128];
            sprintf(title, "%d %s", v9->num, v11);

            sub_4EBBA8(yw, brf, v9->pos_x, v9->pos_z, 25, title, 26, v9->owner + 136, 2, v9->vehicle, brf->field_2F84);
        }
    }
}

void ypaworld_func158__sub4__sub1__sub4__sub18(_NC_STACK_ypaworld *yw, struC5 *inpt, big_ypa_Brf *brf)
{
    if ( brf->field_2F90 )
    {
        brf->brf_objs.object_id = 0;
        brf->brf_objs.field_0 = 0;
    }

    brf->field_2E70 = -1;
    brf->field_2E74 = 0;

    brf->field_2E68 = 26;
    brf->field_2E78 = brf->field_2E7C;

    brf->field_2E88 = get_lang_string(yw->string_pointers_p2, 156, "TRANSPORTER GATES");

    for (int i = 0; i < yw->field_2d90->gate_count; i++)
    {
        if (yw->field_2d90->gates[i].mb_status != 1)
            brf->field_2E74++;
    }

    brf->field_2E68 = (brf->field_2E74 == 0) + 26;
}

void ypaworld_func158__sub4__sub1__sub4__sub19(_NC_STACK_ypaworld *yw, struC5 *inpt, big_ypa_Brf *brf)
{
    int v6 = (brf->field_2E7C - brf->field_2E78) / 2500;

    if ( v6 >= brf->field_2E74 )
    {
        brf->field_2E68 = 27;
    }
    else if ( v6 != brf->field_2E70 )
    {
        brf->field_2E70 = v6;

        int v66 = v6;

        gateProto *v3 = yw->field_2d90->gates;

        for ( int i = 0; i < yw->field_2d90->gate_count; i++)
        {
            v3 = &yw->field_2d90->gates[i];

            if (yw->field_2d90->gates[i].mb_status != 1)
            {
                if ( v66 == 0 )
                    break;

                v66--;
            }
        }

        float xpos = v3->sec_x * 1200.0 + 600.0;
        float ypos = -(v3->sec_y * 1200.0 + 600.0);
        int v13 = yw->BuildProtos[ v3->closed_bp ].sec_type;
        const char *v14 = get_lang_string(yw->string_pointers_p2, 159, "BEAM GATE");

        sub_4EBBA8(yw, brf, xpos, ypos, 25, v14, 26, 145, 1, v13, brf->field_2F84);
    }
}

void ypaworld_func158__sub4__sub1__sub4__sub21(_NC_STACK_ypaworld *yw, struC5 *inpt, big_ypa_Brf *brf)
{
    int v20 = -1;

    if ( !inpt->winp131arg.selected_btn )
    {
        float mx = (float)inpt->winp131arg.move[0].x / (float)yw->screen_width;
        float my = (float)inpt->winp131arg.move[0].y / (float)yw->screen_height;

        for (int i = 0; i < brf->tp2_count; i++)
        {
            brf_t2 *v10 = &brf->tp2[i];

            float v13 = v10->xpos / yw->map_Width_meters * (brf->field_2F7C - brf->field_2F74) + brf->field_2F74;
            float v15 = (v13 + 1.0) * 0.5;

            float v14 = -v10->ypos / yw->map_Height_meters * (brf->field_2F80 - brf->field_2F78) + brf->field_2F78;
            float v23 = 0.5 * (v14 + 1.0);

            if ( v15 + -0.025 < mx && v15 + 0.025 > mx && v23 + -0.025 < my && v23 + 0.025 > my )
                v20 = i;
        }

        if ( v20 == -1 || v20 == brf->field_2F8C )
        {
            if ( v20 != brf->field_2F8C && brf->field_2F8C != -1 )
            {
                brf_obj *br_obj = &brf->brf_objs;
                br_obj[ brf->field_2F90 ].field_0 = 0;
                br_obj[ brf->field_2F90 ].object_id = 0;

                brf->field_2F8C = -1;
                brf->field_2F90 = -1;
            }
        }
        else
        {
            brf_t2 *v16 = &brf->tp2[v20];
            brf->field_2F8C = v20;
            brf->field_2F90 = sub_4EBBA8(yw, brf, v16->xpos, v16->ypos, v16->field_E, v16->title, v16->field_C, v16->field_D, v16->field_8, v16->vehicle_id, 0);
        }
    }
}

void sub_4ED434(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf)
{
    char *v3 = stru_5C91D0.data_cmdbuf;

    if ( brf->field_2E80 > 0 )
    {
        const char *ln = brf->LEVEL_BRIEF_INFO;

        int v5 = 1;

        while( 1 )
        {
            ln = strpbrk(ln, "\n");
            if (!ln)
                break;

            ln++;
            v5++;
        }

        int v34 = brf->field_2E7C - brf->field_2E80;
        int v33 = 100 * v34;

        stru_5C91D0.elements_for_scroll_size = v5;

        v3 = lstvw_up_border(yw, &stru_5C91D0, v3, 16, "   ");

        int v37 = 0;

        ln = brf->LEVEL_BRIEF_INFO;

        while ( 1 )
        {
            int en = 0;

            char a4[512];

            const char *epos = strpbrk(ln, "\n");

            if (epos)
            {
                int len = epos - ln;
                memcpy(a4, ln, len);
                a4[len] = 0;

                ln = epos + 1;
            }
            else
            {
                en = 1;
                strcpy(a4, ln);
            }

            int v12 = 0;

            if ( v34 >= 500 )
            {
                v12 = 100;
            }
            else
            {
                v12 = v33 / 500;

                if ( v12 < 0 )
                {
                    v12 = 0;
                }
                else if ( v12 > 100 )
                {
                    v12 = 100;
                }
            }

            v33 -= 50000;
            v34 -= 500;

            if ( stru_5C91D0.scroll_pos <= v37 && stru_5C91D0.scroll_pos + stru_5C91D0.element_count > v37	&& v12 > 0 )
            {
                fntcmd_set_txtColor(&v3, yw->iniColors[63].r, yw->iniColors[63].g, yw->iniColors[63].b);

                v3 = sub_451E64(yw->tiles[16], v3, a4, v12, 4);

                fntcmd_next_line(&v3);
            }

            v37++;

            if ( en )
            {
                v3 = lstvw_down_border(yw, &stru_5C91D0, v3, 16, "   ");
                break;
            }
        }
    }
    else
    {
        int v20 = brf->field_2E7C - brf->field_2E84;

        const char *v21 = get_lang_string(yw->string_pointers_p2, 2467, "LOADING MISSION OBJECTIVES...");

        int v24;

        if ( v20 >= 600 )
            v24 = 100;
        else
        {
            v24 = 100 * v20 / 600;

            if ( v24 < 0 )
                v24 = 0;
            else if ( v24 > 100 )
                v24 = 100;
        }

        if ( v24 > 0 )
        {
            v3 = lstvw_up_border(yw, &stru_5C91D0, v3, 16, "   ");

            fntcmd_set_txtColor(&v3, yw->iniColors[63].r, yw->iniColors[63].g, yw->iniColors[63].b);

            v3 = sub_451E64(yw->tiles[16], v3, v21, v24, 4);

            v3 = lstvw_down_border(yw, &stru_5C91D0, v3, 16, "   ");
        }
    }

    fntcmd_set_end(&v3);
}

void ypaworld_func158__sub4__sub1__sub4__sub1(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf)
{
    char v27[1024];
    char *pos = v27;

    int v39 = -1;

    for (int i = 0; i < brf->tp2_count; i++)
    {
        brf_t2 *v31 = &brf->tp2[i];
        int v29 = 1;

        for (int j = 0; j < 1; j++)
        {
            brf_obj *brobj = &brf->brf_objs + j;

            if ( brobj->field_0 )
            {
                if ( v31->field_8 == brobj->field_0 )
                {
                    if ( v31->vehicle_id == brobj->object_id && brobj->xpos == v31->xpos && brobj->ypos == v31->ypos )
                    {
                        if ( (brf->field_2E7C / 200) & 1 )
                        {
                            v29 = 0;
                            break;
                        }
                    }
                }
            }
        }

        if ( v29 )
        {
            if ( v39 != v31->field_C)
            {
                v39 = v31->field_C;
                fntcmd_select_tileset(&pos, v31->field_C);
            }

            int v38 = yw->tiles[(int)v31->field_C]->font_height / 2;

            float v15 = (brf->field_2F7C - brf->field_2F74) * (v31->xpos / yw->map_Width_meters) + brf->field_2F74;
            float v14 = (brf->field_2F80 - brf->field_2F78) * (-v31->ypos / yw->map_Height_meters) + brf->field_2F78;

            fntcmd_set_center_xpos(&pos, (v15 * (yw->screen_width / 2)) - v38);
            fntcmd_set_center_ypos(&pos, (v14 * (yw->screen_height / 2)) - v38);

            fntcmd_store_u8(&pos, v31->field_D);
        }
    }

    fntcmd_set_end(&pos);

    w3d_a209 v28;

    v28.cmdbuf = v27;
    v28.includ = NULL;

    call_method(yw->win3d, 209, &v28);
}

void ypaworld_func158__sub4__sub1__sub4__sub0(_NC_STACK_ypaworld *yw)
{
    int w = yw->screen_width / 2;
    int h = yw->screen_height / 2;

    int v34 = w * 0.03125;
    int v35 = w * -0.934375;
    int v37 = h * -0.9333333333333333;

    char buf[260];

    char *pos = buf;

    fntcmd_select_tileset(&pos, 16);
    fntcmd_set_center_xpos(&pos, v35);
    fntcmd_set_center_ypos(&pos, 4 + v37);
    fntcmd_set_txtColor(&pos, yw->iniColors[66].r, yw->iniColors[66].g, yw->iniColors[66].b);

    const char *v7 = get_lang_string(yw->string_pointers_p2, yw->field_2d90->levelID + 1800, yw->field_2d90->map_name);

    pos = sub_45148C(yw->tiles[16], pos, v7, v34 - v35);
    fntcmd_set_end(&pos);

    w3d_a209 v32;
    v32.cmdbuf = buf;
    v32.includ = NULL;

    call_method(yw->win3d, 209, &v32);
}

void ypaworld_func158__sub4__sub1__sub4__sub2(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, struC5 *inpt, int obj_id, char a4)
{
    brf_obj *br_obj = &brf->brf_objs + obj_id;

    if ( br_obj->field_0 )
    {
        int v8 = brf->field_2E7C - br_obj->field_C;
        float v26 = -br_obj->ypos / yw->map_Height_meters;
        float v27 = br_obj->xpos / yw->map_Width_meters;

        if ( v8 > 50 )
        {
            if ( a4 & 1 )
            {
                w3d_func198arg v20;
                w3d_func198arg v21;

                v21.x1 = (brf->field_2F7C - brf->field_2F74) * v27 + brf->field_2F74;
                v21.x2 = v21.x1;
                v21.y1 = (brf->field_2F80 - brf->field_2F78) * v26 + brf->field_2F78;
                v21.y2 = br_obj->field_1C;

                v20.y1 = v21.y2;
                v20.y2 = v21.y2;

                if ( obj_id >= 3 )
                {
                    v20.x1 = v21.x1;
                    v20.x2 = br_obj->field_18;
                }
                else
                {
                    v20.x1 = br_obj->field_10;
                    v20.x2 = v21.x1;
                }

                rstr_arg217 v22;

                if ( dword_514EFC )
                {
                    v22.dword4 = 0x7070;
                    v22.dword8 = 0xFFFFFFFF;
                    v22.dword0 = 0xA0A0;
                }
                else
                {
                    v22.dword4 = br_obj->field_2C;
                    v22.dword8 = 0xFFFFFFFF;
                    v22.dword0 = v22.dword4;
                }

                call_method(yw->win3d, 217, &v22);
                call_method(yw->win3d, 198, &v20);

                if ( dword_514EFC )
                {
                    v22.dword4 = 0x7070;
                    v22.dword8 = 0xFFFFFFFF;
                    v22.dword0 = 0x4040;
                }

                call_method(yw->win3d, 217, &v22);
                call_method(yw->win3d, 198, &v21);
            }
        }

        if ( v8 > 50 && br_obj->title )
        {
            if ( a4 & 2 )
            {
                int v11 = v8 - 50;
                int v13;

                if ( v11 >= 500 )
                    v13 = 100;
                else
                    v13 = 100 * v11 / 500;

                int xpos = ((br_obj->field_10 + br_obj->field_18) * 0.5) * (yw->screen_width / 2);
                int ypos = ((yw->screen_height / 2) * br_obj->field_1C - yw->tiles[16]->font_height + -1.0);

                char cmdbuf[128];
                char *pos = cmdbuf;

                fntcmd_select_tileset(&pos, 16);
                fntcmd_set_center_xpos(&pos, xpos);
                fntcmd_set_center_ypos(&pos, ypos);
                fntcmd_set_txtColor(&pos, yw->iniColors[66].r, yw->iniColors[66].g, yw->iniColors[66].b);

                pos = sub_451E64(yw->tiles[16], pos, br_obj->title, v13, 16);

                fntcmd_set_end(&pos);

                w3d_a209 v23;
                v23.cmdbuf = cmdbuf;
                v23.includ = NULL;

                call_method(yw->win3d, 209, &v23);
            }
        }
    }
}

void ypaworld_func158__sub4__sub1__sub4(_NC_STACK_ypaworld *yw, UserData *usr, struC5 *inpt)
{
    big_ypa_Brf *brf = &yw->brief;

    if ( yw->brief.field_2E68 == 28 )
    {
        sub_4491A0(yw, yw->brief.movie);
        brf->field_2E68 = 4;
    }
    else
    {

        switch ( yw->brief.field_2E6C )
        {
        case 0:
            yw->brief.field_2E7C += inpt->period;
            break;

        case 1:
            inpt->period = 1;
            yw->brief.field_2E7C += inpt->period;
            break;

        case 2:
            yw->brief.field_2E7C += inpt->period;

            if ( yw->brief.field_2E68 == 30 )
            {
                yw->brief.field_2E68 = 31;
                yw->brief.field_2E6C = 0;
            }
            else if ( yw->brief.field_2E70 == -1 )
            {
                yw->brief.field_2E6C = 0;
            }
            else
            {
                switch (yw->brief.field_2E68)
                {
                case 8:
                case 11:
                case 14:
                case 17:
                case 20:
                case 23:
                case 26:
                    yw->brief.field_2E7C = 2500 * (yw->brief.field_2E70 + 1) + yw->brief.field_2E78;
                    break;
                default:
                    break;
                }

                yw->brief.field_2E6C = 0;
            }
            break;

        case 3:
            yw->brief.field_2E6C = 0;
            yw->brief.field_2E68 = 29;
            break;

        default:
            break;
        }

        switch ( yw->brief.field_2E68 )
        {
        case 4:
            ypaworld_func158__sub4__sub1__sub4__sub3(yw, inpt, brf);
            break;
        case 5:
            ypaworld_func158__sub4__sub1__sub4__sub4(yw, inpt, brf);
            break;
        case 6:
            ypaworld_func158__sub4__sub1__sub4__sub5(yw, inpt, brf);
            break;
        case 7:
            ypaworld_func158__sub4__sub1__sub4__sub6(yw, inpt, brf);
            break;
        case 8:
            if ( yw->brief.field_2F8C == -1 )
                ypaworld_func158__sub4__sub1__sub4__sub7(yw, inpt, brf);
            break;
        case 9:
            yw->brief.field_2E68 = 25;
            break;
        case 10:
            ypaworld_func158__sub4__sub1__sub4__sub8(yw, inpt, brf);
            break;
        case 11:
            if ( yw->brief.field_2F8C == -1 )
                ypaworld_func158__sub4__sub1__sub4__sub9(yw, inpt, brf);
            break;
        case 12:
            yw->brief.field_2E68 = 13;
            break;
        case 13:
            ypaworld_func158__sub4__sub1__sub4__sub10(yw, inpt, brf);
            break;
        case 14:
            if ( yw->brief.field_2F8C == -1 )
                ypaworld_func158__sub4__sub1__sub4__sub11(yw, inpt, brf);
            break;
        case 15:
            yw->brief.field_2E68 = 16;
            break;
        case 16:
            ypaworld_func158__sub4__sub1__sub4__sub12(yw, inpt, brf);
            break;
        case 17:
            if ( yw->brief.field_2F8C == -1 )
                ypaworld_func158__sub4__sub1__sub4__sub13(yw, inpt, brf);
            break;
        case 18:
            yw->brief.field_2E68 = 19;
            break;
        case 19:
            ypaworld_func158__sub4__sub1__sub4__sub14(yw, inpt, brf);
            break;
        case 20:
            if ( yw->brief.field_2F8C == -1 )
                ypaworld_func158__sub4__sub1__sub4__sub15(yw, inpt, brf);
            break;
        case 21:
            yw->brief.field_2E68 = 22;
            break;
        case 22:
            ypaworld_func158__sub4__sub1__sub4__sub16(yw, inpt, brf);
            break;
        case 23:
            if ( yw->brief.field_2F8C == -1 )
                ypaworld_func158__sub4__sub1__sub4__sub17(yw, inpt, brf);
            break;
        case 24:
            yw->brief.field_2F84 = 0;
            yw->brief.field_2E68 = 7;
            break;
        case 25:
            ypaworld_func158__sub4__sub1__sub4__sub18(yw, inpt, brf);
            break;
        case 26:
            if ( yw->brief.field_2F8C == -1 )
                ypaworld_func158__sub4__sub1__sub4__sub19(yw, inpt, brf);
            break;
        case 27:
            yw->brief.field_2E68 = 10;
            break;
        default:
            break;
        }

        lstvw_update_input(yw, &stru_5C91D0, inpt);
        lstvw_update(yw, &stru_5C91D0);

        sub_4ED434(yw, brf);

        ypaworld_func158__sub4__sub1__sub4__sub21(yw, inpt, brf);

        call_method(yw->win3d, 215, 0);

        if ( brf->briefing_map )
        {
            rstr_arg204 a4;

            call_vtbl(brf->briefing_map, 3, 0x80002000, &a4.pbitm, 0);

            a4.float4 = -1.0;
            a4.floatC = 1.0;
            a4.float8 = -1.0;
            a4.float10 = 1.0;
            a4.float14 = -1.0;
            a4.float1C = 1.0;
            a4.float18 = -1.0;
            a4.float20 = 1.0;

            call_method(yw->win3d, 204, &a4);
        }

        call_method(yw->win3d, 204, &brf->field_2F40);

        call_method(yw->win3d, 216, 0);

        ypaworld_func158__DrawVehicle(yw, brf, inpt);

        call_method(yw->win3d, 215, 0);

        ypaworld_func158__sub4__sub1__sub4__sub1(yw, brf);
        ypaworld_func158__sub4__sub1__sub4__sub0(yw);

        w3d_a209 v10;
        v10.cmdbuf = stru_5C91D0.draw_cmd;
        v10.includ = stru_5C91D0.field_1C4;

        call_method(yw->win3d, 209, &v10);

        ypaworld_func158__sub4__sub1__sub4__sub2(yw, brf, inpt, 0, 2);

        call_method(yw->win3d, 216, 0);
    }
}
