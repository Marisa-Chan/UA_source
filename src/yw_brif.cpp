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

    brf->field_2F64.x1 = -0.4515625;
    brf->field_2F64.y1 = -0.324999988079;
    brf->field_2F64.x2 = brf->field_2F64.x1;
    brf->field_2F64.y2 = brf->field_2F64.y1;

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

    brf->field_2F74.x1 = -0.4515625 - v17 * 0.4828125;
    brf->field_2F74.y1 = -0.325 - v16 * 0.6083333333333334;
    brf->field_2F74.x2 = brf->field_2F74.x1 + v17 * 0.965625;
    brf->field_2F74.y2 = brf->field_2F74.y1 + v16 * 1.216666666666667;
    brf->field_2F40.float14 = brf->field_2F64.x1;
    brf->field_2F40.float18 = brf->field_2F64.y1;
    brf->field_2F40.float1C = brf->field_2F64.x2;
    brf->field_2F40.float20 = brf->field_2F64.y2;
    brf->field_2E80 = 0;
    brf->field_2E84 = brf->field_2E7C;
}

void ypaworld_func158__sub4__sub1__sub4__sub4(_NC_STACK_ypaworld *yw, struC5 *inpt, big_ypa_Brf *brf)
{
    int v5 = brf->field_2E7C - brf->field_2E78;

    if ( v5 >= 600 )
    {
        brf->field_2F40.float14 = brf->field_2F74.x1;
        brf->field_2F40.float18 = brf->field_2F74.y1;
        brf->field_2F40.float1C = brf->field_2F74.x2;
        brf->field_2F40.float20 = brf->field_2F74.y2;
        brf->field_2E68 = 6;

        displ_arg263 v4;
        v4.bitm = yw->pointers__bitm[5];
        v4.pointer_id = 6;

        yw->win3d->display_func263(&v4);
    }
    else
    {
        float v3 = v5 * 0.0016666667;
        brf->field_2F40.float14 = (brf->field_2F74.x1 - brf->field_2F64.x1) * v3 + brf->field_2F64.x1;
        brf->field_2F40.float18 = (brf->field_2F74.y1 - brf->field_2F64.y1) * v3 + brf->field_2F64.y1;
        brf->field_2F40.float1C = (brf->field_2F74.x2 - brf->field_2F64.x2) * v3 + brf->field_2F64.x2;
        brf->field_2F40.float20 = (brf->field_2F74.y2 - brf->field_2F64.y2) * v3 + brf->field_2F64.y2;
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

        yw->win3d->display_func263(&v6);
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

            float v13 = v10->xpos / yw->map_Width_meters * (brf->field_2F74.x2 - brf->field_2F74.x1) + brf->field_2F74.x1;
            float v15 = (v13 + 1.0) * 0.5;

            float v14 = -v10->ypos / yw->map_Height_meters * (brf->field_2F74.y2 - brf->field_2F74.y1) + brf->field_2F74.y1;
            float v23 = 0.5 * (v14 + 1.0);

            if ( v15 + -0.025 < mx && v15 + 0.025 > mx && v23 + -0.025 < my && v23 + 0.025 > my )
                v20 = i;
        }

        if ( v20 == -1 || v20 == brf->field_2F8C )
        {
            if ( v20 != brf->field_2F8C && brf->field_2F8C != -1 )
            {
                brf_obj *br_obj = &brf->brf_objs;
//                br_obj[ brf->field_2F90 ].field_0 = 0;
//                br_obj[ brf->field_2F90 ].object_id = 0;
                br_obj->field_0 = 0;      //Only one brf_obj!
                br_obj->object_id = 0;

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

            float v15 = (brf->field_2F74.x2 - brf->field_2F74.x1) * (v31->xpos / yw->map_Width_meters) + brf->field_2F74.x1;
            float v14 = (brf->field_2F74.y2 - brf->field_2F74.y1) * (-v31->ypos / yw->map_Height_meters) + brf->field_2F74.y1;

            fntcmd_set_center_xpos(&pos, (v15 * (yw->screen_width / 2)) - v38);
            fntcmd_set_center_ypos(&pos, (v14 * (yw->screen_height / 2)) - v38);

            fntcmd_store_u8(&pos, v31->field_D);
        }
    }

    fntcmd_set_end(&pos);

    w3d_a209 v28;

    v28.cmdbuf = v27;
    v28.includ = NULL;

    yw->win3d->raster_func209(&v28);
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

    yw->win3d->raster_func209(&v32);
}

void ypaworld_func158__sub4__sub1__sub4__sub2(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, struC5 *inpt, int obj_id, char a4)
{
    //brf_obj *br_obj = &brf->brf_objs + obj_id; //By first design it had more than 1 brf_obj
    brf_obj *br_obj = &brf->brf_objs;

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

                v21.x1 = (brf->field_2F74.x2 - brf->field_2F74.x1) * v27 + brf->field_2F74.x1;
                v21.x2 = v21.x1;
                v21.y1 = (brf->field_2F74.y2 - brf->field_2F74.y1) * v26 + brf->field_2F74.y1;
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

                yw->win3d->raster_func217(&v22);
                yw->win3d->raster_func198(&v20);

                if ( dword_514EFC )
                {
                    v22.dword4 = 0x7070;
                    v22.dword8 = 0xFFFFFFFF;
                    v22.dword0 = 0x4040;
                }

                yw->win3d->raster_func217(&v22);
                yw->win3d->raster_func198(&v21);
            }
        }

        if ( v8 > 50 && br_obj->title[0] )
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

                yw->win3d->raster_func209(&v23);
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

        yw->win3d->raster_func215(NULL);

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

            yw->win3d->raster_func204(&a4);
        }

        yw->win3d->raster_func204(&brf->field_2F40);

        yw->win3d->raster_func216(NULL);

        ypaworld_func158__DrawVehicle(yw, brf, inpt);

        yw->win3d->raster_func215(NULL);

        ypaworld_func158__sub4__sub1__sub4__sub1(yw, brf);
        ypaworld_func158__sub4__sub1__sub4__sub0(yw);

        w3d_a209 v10;
        v10 = stru_5C91D0.cmdstrm;

        yw->win3d->raster_func209(&v10);

        ypaworld_func158__sub4__sub1__sub4__sub2(yw, brf, inpt, 0, 2);

        yw->win3d->raster_func216(NULL);
    }
}




// Debrif


void sub_449310(ua_fRect *rect)
{
    if ( rect->x1 > 1.0 )
        rect->x1 = 1.0;
    else if ( rect->x1 < -1.0 )
        rect->x1 = -1.0;

    if ( rect->y1 > 1.0 )
        rect->y1 = 1.0;
    else if ( rect->y1 < -1.0 )
        rect->y1 = -1.0;

    if ( rect->x2 > 1.0 )
        rect->x2 = 1.0;
    else if ( rect->x2 < -1.0 )
        rect->x2 = -1.0;

    if ( rect->y2 > 1.0 )
        rect->y2 = 1.0;
    else if ( rect->y2 < -1.0 )
        rect->y2 = -1.0;
}

void ypaworld_func158__sub4__sub1__sub6__sub0(_NC_STACK_ypaworld *yw, struC5 *struc, big_ypa_Brf *brf)
{
    brf->field_2E78 = brf->field_2E7C;
    brf->field_2E68 = 5;

    call_vtbl(brf->mbmap_img, 3, 0x80002000, &brf->field_2F40.pbitm, 0);

    float v21, v22;

    if ( yw->sectors_maxX2 > yw->sectors_maxY2 )
    {
        v21 = 1.0;
        v22 = (float)yw->sectors_maxY2 / (float)yw->sectors_maxX2;
    }
    else if ( yw->sectors_maxY2 > yw->sectors_maxX2 )
    {
        v22 = 1.0;
        v21 = (float)yw->sectors_maxX2 / (float)yw->sectors_maxY2;
    }
    else
    {
        v21 = 1.0;
        v22 = 1.0;
    }

    float v19 = 1.0 / (float)yw->sectors_maxY2;
    float v20 = 1.0 / (float)yw->sectors_maxX2;

    int v8, v9;
    if ( brf->field_41A0 )
    {
        v8 = yw->field_2d90->gates[ yw->field_2d90->field_4C ].sec_x;
        v9 = yw->field_2d90->gates[ yw->field_2d90->field_4C ].sec_y;
    }
    else
    {
        v9 = yw->sectors_maxY2 / 2;
        v8 = yw->sectors_maxX2 / 2;
    }

    float v11 = 2.0 * ((float)v9 / (float)yw->sectors_maxY2) - 1.0;
    float v12 = 2.0 * ((float)v8 / (float)yw->sectors_maxX2) - 1.0;

    brf->field_2F64.x1 = v12 - v20;
    brf->field_2F64.y1 = v11 - v19;
    brf->field_2F64.x2 = v20 + v12;
    brf->field_2F64.y2 = v11 + v19;
    sub_449310(&brf->field_2F64);

    brf->field_2F74.x1 = -0.4515625 - v21 * 0.4828125;
    brf->field_2F74.y1 = -0.325 - v22 * 0.6083333333333334;
    brf->field_2F74.x2 = brf->field_2F74.x1 + v21 * 0.965625;
    brf->field_2F74.y2 = brf->field_2F74.y1 + v22 * 1.216666666666667;
    sub_449310(&brf->field_2F74);

    brf->field_2F40.float4 = brf->field_2F64.x1;
    brf->field_2F40.float8 = brf->field_2F64.y1;
    brf->field_2F40.floatC = brf->field_2F64.x2;
    brf->field_2F40.float10 = brf->field_2F64.y2;

    brf->field_2F40.float14 = brf->field_2F74.x1;
    brf->field_2F40.float18 = brf->field_2F74.y1;
    brf->field_2F40.float1C = brf->field_2F74.x2;
    brf->field_2F40.float20 = brf->field_2F74.y2;

    if ( yw->GameShell )
        sub_423F74(&yw->GameShell->samples1_info, 11);
}

void ypaworld_func158__sub4__sub1__sub6__sub1(_NC_STACK_ypaworld *yw, struC5 *struc, big_ypa_Brf *brf)
{
    int v4 = brf->field_2E7C - brf->field_2E78;

    if ( v4 >= 600 )
    {
        brf->field_2F40.float4 = -1.0;
        brf->field_2F40.float8 = -1.0;
        brf->field_2F40.floatC = 1.0;
        brf->field_2F40.float10 = 1.0;

        brf->field_2F40.float14 = brf->field_2F74.x1;
        brf->field_2F40.float18 = brf->field_2F74.y1;
        brf->field_2F40.float1C = brf->field_2F74.x2;
        brf->field_2F40.float20 = brf->field_2F74.y2;
        brf->field_2E68 = 6;
    }
    else
    {
        float v3 = v4 / 600.0;
        brf->field_2F40.float4 = brf->field_2F64.x1 + (-1.0 - brf->field_2F64.x1) * v3;
        brf->field_2F40.float8 = brf->field_2F64.y1 + (-1.0 - brf->field_2F64.y1) * v3;
        brf->field_2F40.floatC = brf->field_2F64.x2 + (1.0 - brf->field_2F64.x2) * v3;
        brf->field_2F40.float10 = brf->field_2F64.y2 + (1.0 - brf->field_2F64.y2) * v3;
        brf->field_2F40.float14 = brf->field_2F74.x1;
        brf->field_2F40.float18 = brf->field_2F74.y1;
        brf->field_2F40.float1C = brf->field_2F74.x2;
        brf->field_2F40.float20 = brf->field_2F74.y2;
    }
}

void ypaworld_func158__sub4__sub1__sub6__sub2(_NC_STACK_ypaworld *yw, struC5 *struc, big_ypa_Brf *brf)
{
    brf->field_2E78 = brf->field_2E7C;
    brf->field_2E68 = 8;
    brf->field_41D8 = 0;

    bitmap_intern *v5;
    bitmap_intern *a4;

    call_vtbl(yw->copyof_ownermap, 3, 0x80002000, &a4, 0);
    call_vtbl(brf->copy2_of_ownmap, 3, 0x80002000, &v5, 0);

    memcpy(v5->buffer, a4->buffer, a4->width * a4->height);

    call_vtbl(yw->copyof_typemap, 3, 0x80002000, &a4, 0);
    call_vtbl(brf->copy2_of_typmap, 3, 0x80002000, &v5, 0);

    memcpy(v5->buffer, a4->buffer, a4->width * a4->height);

    for (int i = 0; i < 8; i++)
        brf->copy_of_playerstatus[i] = yw->playerstatus[i];

    memset(brf->field_42BC, 0, sizeof(brf->field_42BC));

    brf->tp1_count = 0;
    memset(brf->tp1, 0, sizeof(brf->tp1));
}

void ypaworld_func158__sub4__sub1__sub6__sub3__sub1(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, yw_arg184 *arg, int a4, int a5)
{
    int sx = arg->t26.secX;
    int sy = arg->t26.secY;
    int own = arg->t26.owner;
    int v17 = a4 - a5;

    *( (uint8_t *)brf->copy2_of_ownmap_bitm->buffer + sx + sy * brf->copy2_of_ownmap_bitm->width) = own;

    if ( a5 == brf->field_41D8 )
    {
        sub_4EE710(yw, arg, brf->field_42BC);

        if ( yw->GameShell )
            sub_423F74(&yw->GameShell->samples1_info, 12);
    }

    if ( v17 < 30000 )
    {
        if ( own )
        {
            if ( brf->wireless_db_skels[3] )
            {
                float v20 = (brf->field_2F74.x2 - brf->field_2F74.x1) / (float)yw->sectors_maxX2;
                float v21 = (brf->field_2F74.y2 - brf->field_2F74.y1) / (float)yw->sectors_maxY2;

                float a3a = (float)sx * v20 + brf->field_2F74.x1 + v20 * 0.5;
                float a4a = (float)sy * v21 + brf->field_2F74.y1 + v21 * 0.5;

                float v19 = 1.0 - (float )v17 / 30000.0;

                if ( v19 > 1.0 )
                    v19 = 1.0;
                else if ( v19 < 0.0)
                    v19 = 0.0;

                float a9 = v20 * v19;
                float a10 = v21 * v19;

                sub_4E332C(yw, brf->wireless_db_skels[3], a3a, a4a, 1.0, 0.0, 0.0, 1.0, a9, a10,  yw_GetColor(yw, own), NULL, NULL);
            }
        }
    }
}

void ypaworld_func158__sub4__sub1__sub6__sub3__sub2(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, yw_arg184 *arg, int a4, int a5)
{
    int own = arg->t34.field_1 & 7;
    int v22 = a4 - a5;

    if ( a5 == brf->field_41D8 )
    {
        sub_4EE710(yw, arg, brf->field_42BC);

        if ( yw->GameShell )
            sub_423F74(&yw->GameShell->samples1_info, 13);
    }

    if ( v22 < 120000 )
    {
        if ( brf->wireless_db_skels[1] )
        {
            float v12 = brf->field_2F74.x2 - brf->field_2F74.x1;
            float v13 = brf->field_2F74.y2 - brf->field_2F74.y1;

            float a3a = v12 * (arg->t34.field_4 / 256.0) + brf->field_2F74.x1;
            float a4a = v13 * (arg->t34.field_5 / 256.0) + brf->field_2F74.y1;

            float v25;

            if ( v22 >= 30000 )
            {
                v25 = 0.1;
            }
            else
            {
                v25 = 1.0 - v22 / 30000.0;

                if ( v25 < 0.1 )
                    v25 = 0.1;
                else if ( v25 > 1.0 )
                    v25 = 1.0;
            }

            float a9 = ((v12 / (float)yw->sectors_maxX2) / 2.0) * v25;
            float a10 = ((v13 / (float)yw->sectors_maxY2) / 2.0) * v25;

            sub_4E332C(yw, brf->wireless_db_skels[1], a3a, a4a, 1.0, 0.0, 0.0, 1.0, a9, a10, yw_GetColor(yw, own), NULL, NULL);
        }
    }
}

void ypaworld_func158__sub4__sub1__sub6__sub3__sub3(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, yw_arg184 *arg, int a4, int a5)
{
    int v9 = a4 - a5;
    if ( v9 < 45000 )
    {
        if ( brf->wireless_db_skels[1] )
        {
            float v13 = brf->field_2F74.x2 - brf->field_2F74.x1;
            float v14 = brf->field_2F74.y2 - brf->field_2F74.y1;

            float a3a = v13 * (arg->t34.field_4 / 256.0) + brf->field_2F74.x1;
            float a4a = v14 * (arg->t34.field_5 / 256.0) + brf->field_2F74.y1;

            float v22 = v9 / 45000.0;

            if ( v22 > 1.0 )
                v22 = 1.0;
            else if ( v22 < 0.0 )
                v22 = 0.0;

            float a9 = ((v13 / (float)yw->sectors_maxX2) / 8.0) * v22;
            float a10 = ((v14 / (float)yw->sectors_maxY2) / 8.0) * v22;

            sub_4E332C(yw, brf->wireless_db_skels[1], a3a, a4a, 1.0, 0.0, 0.0, 1.0, a9, a10, yw_GetColor(yw, arg->t34.field_1), NULL, NULL);
        }
    }
}

void sub_4EF010(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, yw_arg184 *arg)
{
    int v4 = arg->t7.field_4;
    int last_vhcl = arg->t7.last_vhcl;
    int last_weapon = arg->t7.last_weapon;
    int last_build = arg->t7.last_build;

    int v5 = 0;

    if ( !yw->field_727c || arg->t7.owner == yw->field_7280 )
    {
        for (int i = 0; i < brf->tp1_count; i++)
        {
            brf_t1 *v8 = &brf->tp1[i];

            if ( v8->field_0 == v4 && v8->last_vhcl == last_vhcl && v8->last_weapon == last_weapon && v8->last_build == last_build)
            {
                v5 = 1;
                break;
            }
        }

        if ( !v5 )
        {
            if ( brf->tp1_count >= 7 )
                brf->tp1_count = 6;

            brf_t1 *v9 = &brf->tp1[brf->tp1_count];

            v9->field_0 = v4;
            v9->last_build = last_build;
            v9->last_vhcl = last_vhcl;
            v9->last_weapon = last_weapon;

            brf->tp1_count++;
        }
    }
}

void ypaworld_func158__sub4__sub1__sub6__sub3__sub0(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf)
{
    uint8_t *ownmap = (uint8_t *)brf->copy2_of_ownmap_bitm->buffer;

    yw->win3d->raster_func215(NULL);

    float v3 = (brf->field_2F74.x2 - brf->field_2F74.x1) / (float)yw->sectors_maxX2;
    float v4 = (brf->field_2F74.y2 - brf->field_2F74.y1) / (float)yw->sectors_maxY2;

    float v19 = v3 / 10.0;
    float v16 = v4 / 10.0;

    float v21 = brf->field_2F74.y1 + v4 * 0.5;

    for (int yy = 0; yy < yw->sectors_maxY2; yy++)
    {
        float v23 = brf->field_2F74.x1 + v3 * 0.5;

        for (int xx = 0; xx < yw->sectors_maxX2; xx++)
        {
            int owner = *ownmap;

            if ( owner != 0 )
            {
                if ( xx > 0 && xx < yw->sectors_maxX2 - 1
                        && yy > 0 && yy < yw->sectors_maxY2 - 1 )
                {
                    w3d_func198arg arg198;
                    w3d_func198arg arg198_1;

                    arg198.x1 = v23 - v19;
                    arg198.y1 = v21;
                    arg198.x2 = v19 + v23;
                    arg198.y2 = v21;

                    arg198_1.x1 = v23;
                    arg198_1.y1 = v21 - v16;
                    arg198_1.x2 = v23;
                    arg198_1.y2 = v21 + v16;

                    rstr_arg217 arg217;
                    arg217.dword0 = yw_GetColor(yw, owner);
                    arg217.dword4 = arg217.dword0;
                    arg217.dword8 = -1;

                    yw->win3d->raster_func217(&arg217);
                    yw->win3d->raster_func198(&arg198);
                    yw->win3d->raster_func198(&arg198_1);
                }
            }

            v23 += v3;
            ownmap++;
        }

        v21 += v4;
    }

    yw->win3d->raster_func216(NULL);
}


void ypaworld_func158__sub4__sub1__sub6__sub3__sub6(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf)
{
    int v14 = (yw->screen_width / 2) * -0.934375;
    int v13 = (yw->screen_width / 2) * 0.03125;
    int v16 = (yw->screen_height / 2) * -0.9333333333333333;

    const char *v7 = get_lang_string(yw->string_pointers_p2, yw->field_2d90->levelID + 1800, yw->field_2d90->map_name);

    char cmdBuff[264];
    char *cur = cmdBuff;

    fntcmd_select_tileset(&cur, 16);
    fntcmd_set_center_xpos(&cur, v14);
    fntcmd_set_center_ypos(&cur, v16);

    fntcmd_set_txtColor(&cur, yw->iniColors[66].r, yw->iniColors[66].g, yw->iniColors[66].b);

    cur = sub_45148C(yw->tiles[16], cur, v7, v13 - v14);

    fntcmd_set_end(&cur);

    w3d_a209 arg209;
    arg209.cmdbuf = cmdBuff;
    arg209.includ = 0;

    yw->win3d->raster_func209(&arg209);
}

char * ypaworld_func158__sub4__sub1__sub6__sub3__sub4__sub0(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, char *in, int a4)
{
    char *cur = in;

    fntcmd_set_txtColor(&cur, yw->iniColors[67].r, yw->iniColors[67].g, yw->iniColors[67].b);
    listview_t1 elms[3];

    elms[0].txt = get_lang_string(yw->string_pointers_p2, 2450, "KILLS");
    elms[0].bkg_tile = 32;
    elms[0].left_tile = 0;
    elms[0].right_tile = 0;
    elms[0].field_width = a4 * 0.4;
    elms[0].flags = 36;
    elms[0].tileset_id = 15;

    elms[1].txt = get_lang_string(yw->string_pointers_p2, 2451, "BY PLAYER");
    elms[1].tileset_id = 15;
    elms[1].field_width = a4 * 0.3;
    elms[1].bkg_tile = 32;
    elms[1].flags = 36;
    elms[1].left_tile = 0;
    elms[1].right_tile = 0;

    elms[2].txt = get_lang_string(yw->string_pointers_p2, 2452, "ALL");
    elms[2].field_width = a4 * 0.3;
    elms[2].tileset_id = 15;
    elms[2].bkg_tile = 32;
    elms[2].flags = 36;
    elms[2].left_tile = 0;
    elms[2].right_tile = 0;

    return lstvw_txt_line(yw, cur, 3, elms);
}

int sub_4EF2A8(const void *a1, const void *a2)
{
    return ((debrif_t1 *)a2)->status - ((debrif_t1 *)a1)->status;
}

char * ypaworld_func158__sub4__sub1__sub6__sub3__sub4__sub1(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, char *in, int a4)
{
    char *cur = in;
    int a2 = 0;

    debrif_t1 v28[8];

    for (int i = 0; i < 8; i++)
    {
        if ( (1 << i) & yw->field_2d90->ownerMap__has_vehicles )
        {
            v28[a2].owner = i;
            v28[a2].status = brf->field_42BC[i].p1;
            a2++;
        }
    }

    qsort(v28, a2, sizeof(debrif_t1), sub_4EF2A8);


    for (int i = 0; i < a2; i++)
    {
        int clr_id;
        const char *who;

        switch ( v28[i].owner )
        {
        case 1:
            who = get_lang_string(yw->string_pointers_p2, 2411, "RESISTANCE");
            clr_id = 1;
            break;

        case 2:
            who = get_lang_string(yw->string_pointers_p2, 2412, "SULGOGARS");
            clr_id = 2;
            break;

        case 3:
            who = get_lang_string(yw->string_pointers_p2, 2413, "MYKONIANS");
            clr_id = 3;
            break;

        case 4:
            who = get_lang_string(yw->string_pointers_p2, 2414, "TAERKASTEN");
            clr_id = 4;
            break;

        case 5:
            who = get_lang_string(yw->string_pointers_p2, 2415, "BLACK SECT");
            clr_id = 5;
            break;

        case 6:
            who = get_lang_string(yw->string_pointers_p2, 2416, "GHORKOV");
            clr_id = 6;
            break;

        default:
            who = get_lang_string(yw->string_pointers_p2, 2417, "NEUTRAL");
            clr_id = 7;
            break;
        }

        listview_t1 elms[3];

        fntcmd_set_txtColor(&cur, yw->iniColors[clr_id].r, yw->iniColors[clr_id].g, yw->iniColors[clr_id].b);

        elms[0].txt = who;
        elms[0].bkg_tile = 32;
        elms[0].tileset_id = 15;
        elms[0].field_width = a4 * 0.4;
        elms[0].left_tile = 0;
        elms[0].right_tile = 0;
        elms[0].flags = 36;

        char a1[32];

        if ( yw->field_727c || v28[i].owner == 1 )
            sprintf(a1, "%d", brf->field_42BC[ v28[i].owner ].p2);
        else
            sprintf(a1, "-");

        elms[1].txt = a1;
        elms[1].bkg_tile = 32;
        elms[1].tileset_id = 15;
        elms[1].left_tile = 0;
        elms[1].right_tile = 0;
        elms[1].flags = 36;
        elms[1].field_width = a4 * 0.3;

        char v30[32];
        sprintf(v30, "%d", brf->field_42BC[ v28[i].owner ].p1);

        elms[2].txt = v30;
        elms[2].field_width = a4 * 0.3;
        elms[2].tileset_id = 15;
        elms[2].bkg_tile = 32;
        elms[2].flags = 36;
        elms[2].left_tile = 0;
        elms[2].right_tile = 0;

        cur = lstvw_txt_line(yw, cur, 3, elms);

        fntcmd_next_line(&cur);
    }

    return cur;
}

char * ypaworld_func158__sub4__sub1__sub6__sub3__sub4__sub2__sub0(_NC_STACK_ypaworld *yw, char *in, int a4)
{
    char *cur = in;

    fntcmd_set_txtColor(&cur, yw->iniColors[67].r, yw->iniColors[67].g, yw->iniColors[67].b);

    listview_t1 elm;

    elm.txt = get_lang_string(yw->string_pointers_p2, 2453, "SCORE");
    elm.field_width = a4;
    elm.bkg_tile = 32;
    elm.tileset_id = 15;
    elm.left_tile = 0;
    elm.right_tile = 0;
    elm.flags = 36;

    return lstvw_txt_line(yw, cur, 1, &elm);
}

char *ypaworld_func158__sub4__sub1__sub6__sub3__sub4__sub2(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, char *in, int a4)
{
    char *cur = in;

    if ( yw->field_727c )
    {
        cur = ypaworld_func158__sub4__sub1__sub6__sub3__sub4__sub2__sub0(yw, cur, a4);

        fntcmd_next_line(&cur);

        debrif_t1 v32[8];
        int a2 = 0;

        for (int i = 0; i < 8; i++)
        {
            if ( (1 << i) & yw->field_2d90->ownerMap__has_vehicles )
            {
                v32[a2].owner = i;
                v32[a2].status = brf->field_42BC[i].p5;
                a2++;
            }
        }

        qsort(v32, a2, sizeof(debrif_t1), sub_4EF2A8);

        for (int i = 0; i < a2; i++)
        {
            int clr_id;
            const char *who;

            switch ( v32[i].owner )
            {
            case 1:
                who = get_lang_string(yw->string_pointers_p2, 2411, "RESISTANCE");
                clr_id = 1;
                break;

            case 2:
                who = get_lang_string(yw->string_pointers_p2, 2412, "SULGOGARS");
                clr_id = 2;
                break;

            case 3:
                who = get_lang_string(yw->string_pointers_p2, 2413, "MYKONIANS");
                clr_id = 3;
                break;

            case 4:
                who = get_lang_string(yw->string_pointers_p2, 2414, "TAERKASTEN");
                clr_id = 4;
                break;

            case 5:
                who = get_lang_string(yw->string_pointers_p2, 2415, "BLACK SECT");
                clr_id = 5;
                break;

            case 6:
                who = get_lang_string(yw->string_pointers_p2, 2416, "GHORKOV");
                clr_id = 6;
                break;

            default:
                who = get_lang_string(yw->string_pointers_p2, 2417, "NEUTRAL");
                clr_id = 7;
                break;
            }

            fntcmd_set_txtColor(&cur, yw->iniColors[clr_id].r, yw->iniColors[clr_id].g, yw->iniColors[clr_id].b);

            listview_t1 a4a[2];

            a4a[0].txt = who;
            a4a[0].bkg_tile = 32;
            a4a[0].tileset_id = 15;
            a4a[0].left_tile = 0;
            a4a[0].right_tile = 0;
            a4a[0].flags = 36;
            a4a[0].field_width = a4 * 0.5;

            char a1[32];
            sprintf(a1, "%d", brf->field_42BC[ v32[i].owner ].p5);

            a4a[1].txt = a1;
            a4a[1].field_width = a4 * 0.5;
            a4a[1].tileset_id = 15;
            a4a[1].bkg_tile = 32;
            a4a[1].flags = 36;
            a4a[1].left_tile = 0;
            a4a[1].right_tile = 0;

            cur = lstvw_txt_line(yw, cur, 2, a4a);

            fntcmd_next_line(&cur);
        }
    }
    else
    {
        fntcmd_set_txtColor(&cur, yw->iniColors[67].r, yw->iniColors[67].g, yw->iniColors[67].b);

        listview_t1 v35[2];

        v35[0].txt = get_lang_string(yw->string_pointers_p2, 2457, "SCORE THIS MISSION:");
        v35[0].bkg_tile = 32;
        v35[0].left_tile = 0;
        v35[0].right_tile = 0;
        v35[0].field_width = a4 * 0.7;
        v35[0].tileset_id = 15;
        v35[0].flags = 36;

        char v34[32];
        sprintf(v34, "%d", brf->field_42BC[1].p5);

        v35[1].txt = v34;
        v35[1].tileset_id = 15;
        v35[1].bkg_tile = 32;
        v35[1].left_tile = 0;
        v35[1].field_width = a4 * 0.3;
        v35[1].right_tile = 0;
        v35[1].flags = 36;

        cur = lstvw_txt_line(yw, cur, 2, v35);

        fntcmd_next_line(&cur);

        v35[0].txt = get_lang_string(yw->string_pointers_p2, 2458, "SCORE OVERALL:");
        v35[0].flags = 36;
        v35[0].bkg_tile = 32;
        v35[0].field_width = a4 * 0.7;
        v35[0].left_tile = 0;
        v35[0].right_tile = 0;
        v35[0].tileset_id = 15;

        sprintf(v34, "%d", brf->field_42BC[1].p5 + brf->copy_of_playerstatus[1].p5);

        v35[1].txt = v34;
        v35[1].tileset_id = 15;
        v35[1].bkg_tile = 32;
        v35[1].right_tile = 0;
        v35[1].field_width = a4 * 0.3;
        v35[1].left_tile = 0;
        v35[1].flags = 36;

        cur = lstvw_txt_line(yw, cur, 2, v35);

        fntcmd_next_line(&cur);
    }
    return cur;
}

char * ypaworld_func158__sub4__sub1__sub6__sub3__sub4__sub3(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, char *in, int a4)
{
    char *cur = in;
    fntcmd_set_txtColor(&cur, yw->iniColors[67].r, yw->iniColors[67].g, yw->iniColors[67].b);

    if ( yw->field_727c )
    {
        listview_t1 v30[2];
        v30[0].txt = get_lang_string(yw->string_pointers_p2, 2456, "PLAYING TIME:");
        v30[0].bkg_tile = 32;
        v30[0].left_tile = 0;
        v30[0].right_tile = 0;
        v30[0].field_width = a4 * 0.7;
        v30[0].flags = 36;
        v30[0].tileset_id = 15;

        int v12 = brf->field_41D8 / 1024;

        char a1[30];
        sprintf(a1, "%02d:%02d:%02d", v12 / 60 / 60, v12 / 60 % 60, v12 % 60);

        v30[1].txt = a1;
        v30[1].tileset_id = 15;
        v30[1].bkg_tile = 32;
        v30[1].left_tile = 0;
        v30[1].right_tile = 0;
        v30[1].flags = 36;
        v30[1].field_width = a4 * 0.3;

        cur = lstvw_txt_line(yw, cur, 2, v30);
    }
    else
    {

        listview_t1 a4a[2];
        a4a[0].txt = get_lang_string(yw->string_pointers_p2, 2454, "PLAYING TIME THIS MISSION:");
        a4a[0].bkg_tile = 32;
        a4a[0].left_tile = 0;
        a4a[0].right_tile = 0;
        a4a[0].field_width = a4 * 0.7;
        a4a[0].flags = 36;
        a4a[0].tileset_id = 15;

        int v19 = brf->field_41D8 / 1024;

        char v28[32];
        sprintf(v28, "%02d:%02d:%02d", v19 / 60 / 60, v19 / 60 % 60, v19 % 60);

        a4a[1].txt = v28;
        a4a[1].field_width = a4 * 0.3;
        a4a[1].tileset_id = 15;
        a4a[1].bkg_tile = 32;
        a4a[1].left_tile = 0;
        a4a[1].right_tile = 0;
        a4a[1].flags = 36;

        cur = lstvw_txt_line(yw, cur, 2, a4a);

        fntcmd_next_line(&cur);

        a4a[0].txt = get_lang_string(yw->string_pointers_p2, 2455, "PLAYING TIME OVERALL:");
        a4a[0].flags = 36;
        a4a[0].bkg_tile = 32;
        a4a[0].field_width = a4 * 0.7;
        a4a[0].left_tile = 0;
        a4a[0].right_tile = 0;
        a4a[0].tileset_id = 15;

        v19 = (brf->field_41D8 + brf->copy_of_playerstatus[1].p3) / 1024;
        sprintf(v28, "%02d:%02d:%02d", v19 / 60 / 60, v19 / 60 % 60, v19 % 60);

        a4a[1].txt = v28;
        a4a[1].tileset_id = 15;
        a4a[1].bkg_tile = 32;
        a4a[1].field_width = a4 * 0.3;
        a4a[1].left_tile = 0;
        a4a[1].right_tile = 0;
        a4a[1].flags = 36;

        cur = lstvw_txt_line(yw, cur, 2, a4a);
    }

    fntcmd_next_line(&cur);

    return cur;
}

char * ypaworld_func158__sub4__sub1__sub6__sub3__sub4(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, char *in)
{
    char *cur = in;

    fntcmd_select_tileset(&cur, 15);
    fntcmd_set_center_xpos(&cur, ((yw->screen_width / 2) * 0.15) );
    fntcmd_set_center_ypos(&cur, ((yw->screen_height / 2) * -0.821) );

    int v14 = (yw->screen_width / 2) * 0.796875;

    cur = ypaworld_func158__sub4__sub1__sub6__sub3__sub4__sub0(yw, brf, cur, v14);

    fntcmd_next_line(&cur);

    cur = ypaworld_func158__sub4__sub1__sub6__sub3__sub4__sub1(yw, brf, cur, v14);

    fntcmd_next_line(&cur);

    cur = ypaworld_func158__sub4__sub1__sub6__sub3__sub4__sub2(yw, brf, cur, v14);

    fntcmd_next_line(&cur);

    return ypaworld_func158__sub4__sub1__sub6__sub3__sub4__sub3(yw, brf, cur, v14);
}

char * ypaworld_func158__sub4__sub1__sub6__sub3__sub5__sub0(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, brf_t1 *tp1, char *in, int a5)
{
    char *cur = in;

    int last_weapon = tp1->last_weapon;
    int last_vhcl   = tp1->last_vhcl;
    int last_build  = tp1->last_build;

    if ( !last_vhcl && last_weapon )
    {
        for (int i = 0; i < 256; i++)
        {
            if (yw->VhclProtos[i].weapon == last_weapon)
            {
                last_vhcl = i;
                break;
            }
        }

        if ( !last_vhcl )
            return cur;
    }

    if ( !last_weapon && last_vhcl )
    {
        last_weapon = yw->VhclProtos[ last_vhcl ].weapon;

        if ( last_weapon == -1 )
            last_weapon = 0;
    }

    VhclProto *vhcl = NULL;
    WeapProto *wpn  = NULL;
    BuildProto *bld = NULL;

    if ( last_vhcl )
        vhcl = &yw->VhclProtos[last_vhcl];

    if ( last_build )
        bld = &yw->BuildProtos[last_build];

    if ( last_weapon )
        wpn = &yw->WeaponProtos[last_weapon];

    const char *v13 = " ";
    const char *v14 = " ";
    const char *v33 = " ";

    char a1[256];

    if ( vhcl )
    {
        v33 = get_lang_string(yw->string_pointers_p2, last_vhcl + 1200, vhcl->name);
    }
    else if ( bld )
    {
        if ( yw->field_727c )
            v33 = get_lang_string(yw->string_pointers_p2, last_build + 1700, bld->name);
        else
            v33 = get_lang_string(yw->string_pointers_p2, last_build + 1500, bld->name);
    }

    switch ( tp1->field_0 )
    {
    case 1:
        if ( wpn )
        {
            if ( vhcl )
            {
                v13 = get_lang_string(yw->string_pointers_p2, 2459, "WEAPON UPGRADE:");

                if ( vhcl->num_weapons > 1 )
                    sprintf(a1, "(%d x%d)", wpn->energy / 100, vhcl->num_weapons);
                else
                    sprintf(a1, "(%d)", wpn->energy / 100);


                v14 = a1;
            }
        }
        break;

    case 2:
        if ( vhcl )
        {
            v13 = get_lang_string(yw->string_pointers_p2, 2460, "ARMOR UPGRADE:");

            sprintf(a1, "(%d%%)", vhcl->shield);
            v14 = a1;
        }
        break;

    case 3:
        if ( vhcl )
        {
            v13 = get_lang_string(yw->string_pointers_p2, 2461, "NEW VEHICLE TECH:");
        }
        break;

    case 4:
        if ( bld )
        {
            v13 = get_lang_string(yw->string_pointers_p2, 2462, "NEW BUILDING TECH:");
        }
        break;

    case 5:
        if ( vhcl )
        {
            v13 = get_lang_string(yw->string_pointers_p2, 2463, "RADAR UPGRADE:");
        }
        break;

    case 6:
        if ( vhcl )
        {
            if ( bld )
            {
                if ( yw->field_727c )
                    strcpy(a1, get_lang_string(yw->string_pointers_p2, last_build + 1700, bld->name));
                else
                    strcpy(a1, get_lang_string(yw->string_pointers_p2, last_build + 1500, bld->name));

                v13 = get_lang_string(yw->string_pointers_p2, 2464, "COMBINED UPGRADE:");
                v14 = a1;
            }
        }
        break;

    case 7:
        v13 = get_lang_string(yw->string_pointers_p2, 2465, "GENERIC TECH UPGRADE");
        break;

    default:
        break;
    }

    if ( v13 && v33 && v14 )
    {

        listview_t1 elm[3];

        elm[0].txt = v13;
        elm[0].bkg_tile = 32;
        elm[0].flags = 36;
        elm[0].left_tile = 0;
        elm[0].right_tile = 0;
        elm[0].field_width = a5 * 0.48;
        elm[0].tileset_id = 15;

        elm[1].flags = 36;
        elm[1].tileset_id = 15;
        elm[1].bkg_tile = 32;
        elm[1].left_tile = 0;
        elm[1].right_tile = 0;
        elm[1].txt = v33;
        elm[1].field_width = a5 * 0.3;

        elm[2].flags = 36;
        elm[2].tileset_id = 15;
        elm[2].bkg_tile = 32;
        elm[2].left_tile = 0;
        elm[2].right_tile = 0;
        elm[2].field_width = a5 * 0.22;
        elm[2].txt = v14;

        cur = lstvw_txt_line(yw, cur, 3, elm);

        fntcmd_next_line(&cur);
    }

    return cur;
}

char * ypaworld_func158__sub4__sub1__sub6__sub3__sub5(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, char *in)
{
    char *cur = in;

    fntcmd_select_tileset(&cur, 15);
    fntcmd_set_center_xpos(&cur, ((yw->screen_width / 2) * -0.9875) );
    fntcmd_set_center_ypos(&cur, ((yw->screen_height / 2) * 0.35) );

    fntcmd_set_txtColor(&cur, yw->iniColors[67].r, yw->iniColors[67].g, yw->iniColors[67].b);

    for (int i = 0; i < brf->tp1_count; i++)
        cur = ypaworld_func158__sub4__sub1__sub6__sub3__sub5__sub0(yw, brf, &brf->tp1[i], cur,  (yw->screen_width / 2) * 0.984375 );

    return cur;
}

void ypaworld_func158__sub4__sub1__sub6__sub3(_NC_STACK_ypaworld *yw, struC5 *struc, big_ypa_Brf *brf)
{
    char cmdbuf[2048];
    char *cur = cmdbuf;

    int a4 = brf->field_2E7C - brf->field_2E78;

    ypaworld_func158__sub4__sub1__sub6__sub3__sub0(yw, brf);

    cur = ypaworld_func158__sub4__sub1__sub6__sub3__sub4(yw, brf, cur);
    cur = ypaworld_func158__sub4__sub1__sub6__sub3__sub5(yw, brf, cur);

    fntcmd_set_end(&cur);

    int v6 = 0;

    w3d_a209 v24;
    v24.includ = 0;
    v24.cmdbuf = cmdbuf;

    yw->win3d->raster_func215(NULL);
    yw->win3d->raster_func209(&v24);

    int v26 = 0;

    if ( brf->field_2E68 == 8 )
    {
        yw_f726c_nod *hist_nod = (yw_f726c_nod *)yw->history->lst.head;

        while ( hist_nod->next )
        {
            if ( v26 )
                break;

            int tlen = 0;
            int v28 = 1;

            uint8_t *v9 = hist_nod->bufStart;

            yw_arg184 arg184;

            while (v28)
            {
                yw_histbf_read_evnt(v9, &arg184);

                switch ( arg184.type )
                {
                case 0:
                    tlen = 0;
                    v28 = 0;
                    break;

                case 1:
                    tlen = 5;

                    v6 = arg184.t15.field_1;

                    if ( v6 >= brf->field_41D8 )
                        brf->field_41D8 = v6;

                    if ( v6 >= a4 )
                    {
                        v26 = 1;
                        v28 = 0;
                    }
                    break;

                case 2:
                    tlen = 4;
                    ypaworld_func158__sub4__sub1__sub6__sub3__sub1(yw, brf, &arg184, a4, v6);
                    break;

                case 3:
                    tlen = 6;
                    ypaworld_func158__sub4__sub1__sub6__sub3__sub2(yw, brf, &arg184, a4, v6);
                    break;

                case 4:
                    tlen = 6;
                    ypaworld_func158__sub4__sub1__sub6__sub3__sub3(yw, brf, &arg184, a4, v6);
                    break;

                case 6:
                    tlen = 4;
                    if ( v6 == brf->field_41D8 )
                    {
                        sub_4EE710(yw, &arg184, brf->field_42BC);

                        if ( yw->GameShell )
                            sub_423F74(&yw->GameShell->samples1_info, 14);
                    }
                    break;

                case 7:
                    tlen = 12;
                    if ( v6 == brf->field_41D8 )
                    {
                        sub_4EE710(yw, &arg184, brf->field_42BC);
                        sub_4EF010(yw, brf, &arg184);

                        if ( yw->GameShell )
                            sub_423F74(&yw->GameShell->samples1_info, 14);

                    }
                    break;

                case 5:
                    break;

                default:
                    v28 = 0;
                    break;
                }

                v9 += tlen;
            }

            hist_nod = (yw_f726c_nod *)hist_nod->next;
        }

        if ( !v26 )
        {
            brf->field_2E68 = 9;
            if ( yw->field_727c )
            {
                for (int i = 0; i < 8; i ++)
                    brf->field_42BC[i] = yw->field_7796[i];
            }
        }
    }

    ypaworld_func158__sub4__sub1__sub6__sub3__sub6(yw, brf);

    yw->win3d->raster_func216(NULL);
}

void ypaworld_func158__sub4__sub1__sub6(_NC_STACK_ypaworld *yw, UserData *usr, struC5 *inpt)
{
    big_ypa_Brf *brf = &yw->brief;

    if ( yw->history )
    {
        if ( yw->brief.field_2E6C == 0 )
        {
            if ( brf->field_2E68 == 8 )
            {
                brf->field_2E7C += 60 * inpt->period;
            }
            else if ( brf->field_2E68 != 9 )
            {
                brf->field_2E7C += inpt->period;
            }
        }
        else if ( yw->brief.field_2E6C == 1 )
        {
            inpt->period = 1;
        }
        else if ( yw->brief.field_2E6C == 3 )
        {
            brf->field_2E6C = 0;
            brf->field_2E68 = 7;
        }

        if ( brf->field_2E68 != 4 )
        {
            yw->win3d->raster_func215(NULL);

            if ( brf->briefing_map )
            {
                rstr_arg204 arg204;
                call_vtbl(brf->briefing_map, 3, 0x80002000, &arg204.pbitm, 0);

                arg204.float4 = -1.0;
                arg204.floatC = 1.0;
                arg204.float8 = -1.0;
                arg204.float10 = 1.0;
                arg204.float14 = -1.0;
                arg204.float1C = 1.0;
                arg204.float18 = -1.0;
                arg204.float20 = 1.0;

                yw->win3d->raster_func204(&arg204);
            }

            yw->win3d->raster_func204(&brf->field_2F40);
            yw->win3d->raster_func216(NULL);
        }

        switch ( brf->field_2E68 )
        {
        case 4:
            ypaworld_func158__sub4__sub1__sub6__sub0(yw, inpt, brf);
            break;

        case 5:
            ypaworld_func158__sub4__sub1__sub6__sub1(yw, inpt, brf);
            break;

        case 6:
            brf->field_2E68 = 7;
            break;

        case 7:
            ypaworld_func158__sub4__sub1__sub6__sub2(yw, inpt, brf);
            break;

        case 8:
        case 9:
            ypaworld_func158__sub4__sub1__sub6__sub3(yw, inpt, brf);
            break;

        default:
            break;
        }
    }
    else
    {
        yw->brief.field_2E68 = 2;
    }
}
