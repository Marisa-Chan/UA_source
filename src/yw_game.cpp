#include <inttypes.h>
#include <string.h>
#include <math.h>
#include "includes.h"
#include "def_parser.h"
#include "yw_internal.h"
#include "yw.h"
#include "input.h"

extern int bact_id;


NC_STACK_ilbm * loadDisk_screen(_NC_STACK_ypaworld *yw)
{
    CDAUDIO_t arg;
    arg.command = 1;
    sub_4444D4(&arg);

    const char *v3;

    if ( yw->screen_width <= 360 )
        v3 = "disk320.ilbm";
    else if ( yw->screen_width > 600 )
        v3 = "disk640.ilbm";
    else
        v3 = "disk512.ilbm";

    char rsrc_def[128];

    strcpy(rsrc_def, get_prefix_replacement("rsrc"));

    set_prefix_replacement("rsrc", "data:mc2res");

    NC_STACK_ilbm *disk = (NC_STACK_ilbm *)init_get_class("ilbm.class", 0x80001000, v3, 0x80002008, 1, 0x80002009, 1, 0);

    set_prefix_replacement("rsrc", rsrc_def);

    return disk;
}

void draw_splashScreen(_NC_STACK_ypaworld *yw, NC_STACK_ilbm *splashScreen)
{
    if ( splashScreen )
    {
        rstr_arg204 a4;

        call_vtbl(splashScreen, 3, 0x80002000, &a4.pbitm, 0); // bitmap_func3

        a4.float4  = -1.0;
        a4.float8  = -1.0;
        a4.floatC  =  1.0;
        a4.float10 =  1.0;
        a4.float14 = -1.0;
        a4.float18 = -1.0;
        a4.float1C =  1.0;
        a4.float20 =  1.0;

        NC_STACK_win3d *win3d;
        gfxEngine__getter(0x8000300D, &win3d, 0);

        if ( win3d )
        {
            displ_arg263 v4;
            v4.bitm = yw->pointers__bitm[5];
            v4.pointer_id = 6;

            call_method(win3d, 263, &v4);

            call_method(win3d, 257);
            call_method(win3d, 215);

            call_method(win3d, 202, &a4);

            call_method(win3d, 216);
            call_method(win3d, 258);

            call_method(win3d, 257);
            call_method(win3d, 215);

            call_method(win3d, 202, &a4);

            call_method(win3d, 216);
            call_method(win3d, 258);
        }
    }
}

void drawSplashScreenWithTOD(_NC_STACK_ypaworld *yw, NC_STACK_ilbm *splashScreen, const char *text)
{
    if ( splashScreen )
    {
        rstr_arg204 a4;

        call_vtbl(splashScreen, 3, 0x80002000, &a4.pbitm, 0); // bitmap_func3

        a4.float4  = -1.0;
        a4.float8  = -1.0;
        a4.floatC  =  1.0;
        a4.float10 =  1.0;
        a4.float14 = -1.0;
        a4.float18 = -1.0;
        a4.float1C =  1.0;
        a4.float20 =  1.0;

        NC_STACK_win3d *win3d;
        gfxEngine__getter(0x8000300D, &win3d, 0);

        if ( win3d )
        {
            displ_arg263 v4;
            v4.bitm = yw->pointers__bitm[5];
            v4.pointer_id = 6;

            call_method(win3d, 263, &v4);

            call_method(win3d, 257);
            call_method(win3d, 215);

            call_method(win3d, 202, &a4);

            splashScreen_OutText(yw, win3d, text, yw->screen_width / 7, yw->screen_height / 5);

            call_method(win3d, 216);
            call_method(win3d, 258);

            call_method(win3d, 257);
            call_method(win3d, 215);

            call_method(win3d, 202, &a4);

            splashScreen_OutText(yw, win3d, text, yw->screen_width / 7, yw->screen_height / 5);

            call_method(win3d, 216);
            call_method(win3d, 258);
        }
    }
}

void deleteSplashScreen(_NC_STACK_ypaworld *yw, NC_STACK_ilbm *splashScreen)
{
    if ( splashScreen )
        delete_class_obj(splashScreen);
}

void sb_0x44ca90__sub4(_NC_STACK_ypaworld *yw)
{
    printf("MAKE ME %s\n", "sb_0x44ca90__sub4");
}

int sb_0x44ca90__sub6(_NC_STACK_ypaworld *yw)
{
    printf("MAKE ME %s\n", "sb_0x44ca90__sub6");
    return 1;
}

int sb_0x44ca90__sub7(_NC_STACK_ypaworld *yw, int evnt)
{
    printf("MAKE ME %s\n", "sb_0x44ca90__sub7");
    return 1;
}

int sb_0x44ca90__sub3(_NC_STACK_ypaworld *yw)
{
    printf("MAKE ME %s\n", "sb_0x44ca90__sub3");
    return 1;
}

int sb_0x44ca90__sub5(_NC_STACK_ypaworld *yw)
{
    printf("MAKE ME %s\n", "sb_0x44ca90__sub5");
    return 1;
}

void sb_0x44ca90__sub2(_NC_STACK_ypaworld *yw, mapProto *mapp)
{
    NC_STACK_win3d *win3d;
    gfxEngine__getter(0x8000300D, &win3d, 0);

    for (int i = 0; i < 8; i++)
    {
        if (mapp->palettes[i][0])
        {
            NC_STACK_ilbm *ilbm = (NC_STACK_ilbm *)init_get_class("ilbm.class", 0x80001000, mapp->palettes[i], 0x80002006, 1, 0);

            if (ilbm)
            {
                rstr_261_arg v7;
                v7.pal_num = 256;
                v7.pal_id = i;
                v7.entrie_id = 0;

                call_vtbl(ilbm, 3, 0x80002007, &v7.pal_entries, 0);

                if ( i )
                    call_method(win3d, 261, &v7);
                else
                    call_vtbl(win3d, 2, 0x80002007, v7.pal_entries, 0);

                delete_class_obj(ilbm);
            }
            else
            {
                ypa_log_out("WARNING: slot #%d [%s] init failed!\n", i, mapp->palettes[i]);
            }
        }
    }
}

int sb_0x44ca90__sub8(_NC_STACK_ypaworld *yw)
{
    yw->samples = (yw_samples *)AllocVec(sizeof(yw_samples), 65537);
    if ( !yw->samples )
        return 0;

    yw->samples->field_0 = -1;
    sub_423DB0(&yw->samples->field_4);

    return 1;
}

int sb_0x44ca90(_NC_STACK_ypaworld *yw, mapProto *mapp, int levelID, int a5)
{
    int ok = 0;

    memset(mapp, 0, sizeof(mapProto));

    memset(yw->field_7796, 0, sizeof(player_status) * 8);

    yw->field_1614 = 0;
    yw->field_1a04 = 0;
    yw->field_1a08 = 0;
    yw->field_1a00 = 0;
    yw->field_1a1c = 0;
    yw->field_161c = 0;

    yw->field_2d90->levelID = levelID;
    yw->field_2d90->field_48 = a5;
    yw->field_2d90->field_40 = 0;
    yw->field_2d90->gate_count = 0;
    yw->field_2d90->supetItems_count = 0;
    yw->field_2d90->ownerMap__has_vehicles = 0;
    yw->field_2d90->field_60 = 0;

    yw->field_1a60 = 0;
    yw->field_1a98 = 0;
    yw->field_1aac = 0;
    yw->current_bact = NULL;
    yw->field_1b78 = NULL;
    yw->field_1b7c = NULL;
    yw->field_1b80 = NULL;
    yw->field_1b84 = NULL;
    yw->field_1b88 = NULL;
    yw->field_1ab0 = 0;
    yw->field_160c = 0;
    yw->field_1610 = 0;
    yw->field_7882 = 1;
    yw->field_7886 = 1;
    yw->field_788A = 0;
    yw->field_81AF = 0;
    yw->field_241c = 0;
    yw->field_1628 = 0;
    yw->field_162A = 0;
    yw->field_1624 = 0;
    yw->vehicle_sector_ratio_1 = 0;
    yw->field_8283 = 0;
    yw->field_1a20 = 0;

    memset(yw->field_2d90->gates, 0, sizeof(gateProto) * 8);
    memset(yw->field_2d90->supetItems, 0, sizeof(supetItemProto) * 8);
    memset(yw->gems, 0, sizeof(gemProto) * 8);
    memset(&yw->field_81CB, 0, sizeof(yw_81cb));
    memset(yw->field_1bac, 0, sizeof(int) * 8);

    yw->field_753A = 0;
    yw->field_7542 = 0;
    yw->field_754A = 0;
    yw->field_755A = 0;
    yw->field_7552 = 0;

    if ( !yw->one_game_res )
    {
        if ( yw->game_default_res != yw->shell_default_res )
        {
            inputEngine__setter(0x80001007, 0, 0);
            gfxEngine__setter(0x80003007, yw->game_default_res, 0);

            windd__window_params *a2;
            gfxEngine__getter(0x80003007, &a2, 0);
            inputEngine__setter(0x80001007, a2, 0);

            int width, height;

            gfxEngine__getter(0x80003003, &width, 0x80003004, &height, 0x8000300D, &yw->win3d, 0);
            yw->screen_width = width;
            yw->screen_height = height;

            call_vtbl(yw->win3d, 2, 0x80005000, (yw->field_73CE & 0x40) != 0, 0);

            if ( yw->screen_width >= 512 )
            {
                load_font( get_lang_string(yw->string_pointers_p2, 15, "MS Sans Serif,12,400,0") );
            }
            else
            {
                load_font( get_lang_string(yw->string_pointers_p2, 16, "Arial,8,400,0") );
            }
        }
    }

    NC_STACK_ilbm *diskScreenImage = loadDisk_screen(yw);

    if ( diskScreenImage )
        draw_splashScreen(yw, diskScreenImage);


    char rsr[256];

    strcpy(rsr, get_prefix_replacement("rsrc"));

    set_prefix_replacement("rsrc", "data:fonts");

    int v19 = load_fonts_and_icons(yw);

    set_prefix_replacement("rsrc", rsr);

    if ( !v19 )
        return 0;

    int tod = loadTOD(yw, "tod.def");

    const char *text = get_lang_string(yw->string_pointers_p2, tod + 2490, " ");

    tod++;
    if ( tod + 2490 > 2510 )
        tod = 0;

    writeTOD(yw, "tod.def", tod);

    if ( diskScreenImage )
    {
        drawSplashScreenWithTOD(yw, diskScreenImage, text);
        deleteSplashScreen(yw, diskScreenImage);
    }

    yw->p_1_grp_cnt = 0;
    for (int i = 0; i < 8; i++)
    {
        yw->p_1_grp_p1[i] = 0;
        yw->p_1_grp_p2[i] = 0;
        yw->p_1_grp_p3[i] = 100000;
        yw->p_1_grp_p4[i] = 0;
    }

    sb_0x44ca90__sub4(yw);

    milesEngine__getter(0x80004003, &yw->audio_volume, 0);

    sb_0x44ca90__sub8(yw);
    sb_0x44ca90__sub6(yw);

    set_prefix_replacement("rsrc", "data:");

    if ( sub_4DA41C(yw, mapp, yw->LevelNet->mapInfos[yw->field_2d90->levelID].mapPath) && (mapp->flags & 0x7F) == 0x7F )
    {
        sb_0x44ca90__sub7(yw, mapp->event_loop);

        if ( !sb_0x44ca90__sub3(yw) )
            return 0;

        sb_0x44ca90__sub5(yw);

        char a1[32];
        sprintf(a1, "data:set%d", mapp->setNumber);

        set_prefix_replacement("rsrc", a1);

        sb_0x44ca90__sub2(yw, mapp);

        if ( yw_LoadSet(yw, mapp->setNumber) )
        {
            if ( yw_loadSky(yw, mapp->sky) )
                ok = 1;
        }
    }

    return ok;
}

int cells_mark_type(_NC_STACK_ypaworld *yw, const char *a2)
{
    if ( !yw->typ_map )
        return 0;

    bitmap_intern *bitm;
    call_vtbl(yw->typ_map, 3, 0x80002000, &bitm, 0);

    uint8_t *typMap = (uint8_t *)bitm->buffer;

    yw->sectors_maxX2 = bitm->width;
    yw->sectors_maxY2 = bitm->height;

    yw->map_Width_meters = yw->sectors_maxX2 * 1200.0;
    yw->map_Height_meters = yw->sectors_maxY2 * 1200.0;

    int sectors_count = yw->sectors_maxY2 * yw->sectors_maxX2;

    memset(yw->cells, 0, sizeof(cellArea) * sectors_count);

    for (int i = 0; i < sectors_count; i++)
    {
        cellArea *cell = &yw->cells[i];
        secType *sectp = &yw->secTypes[ typMap[i] ];

        init_list(&cell->field_3C);

        cell->sec_type = typMap[i];
        cell->field_2E = sectp->field_0;
        cell->field_2F = 0;

        if ( sectp->field_0 == 1)
        {
            cell->buildings_health[0][0] = sectp->buildings[0][0]->build_health;
        }
        else
        {
            for (int j = 0; j < 3; j++)
                for (int k = 0; k < 3; k++)
                    cell->buildings_health[j][k] = sectp->buildings[j][k]->build_health;
        }
    }
    return 1;
}

int cells_mark_owner(_NC_STACK_ypaworld *yw, const char *a2)
{
    memset(yw->sectors_count_by_owner, 0, sizeof(int) * 8);

    if ( !yw->own_map )
        return 0;

    bitmap_intern *bitm;
    call_vtbl(yw->own_map, 3, 0x80002000, &bitm, 0);       // bitmap_func3

    uint8_t *ownmap = (uint8_t *)bitm->buffer;

    if ( bitm->width != yw->sectors_maxX2 || bitm->height != yw->sectors_maxY2 )
    {
        ypa_log_out("Mapsize mismatch %s: is [%d,%d], should be [%d,%d].\n", a2, bitm->width, bitm->height, yw->sectors_maxX2, yw->sectors_maxY2);
        delete_class_obj(yw->own_map);
        yw->own_map = NULL;
        return 0;
    }


    for (int yy = 0; yy < yw->sectors_maxY2; yy++)
    {
        uint8_t *ownmapp = ownmap + yy * yw->sectors_maxX2;
        cellArea *cells = yw->cells + yy * yw->sectors_maxX2;

        for (int xx = 0; xx < yw->sectors_maxX2; xx++)
        {
            if ( xx > 0 && yy > 0 && xx != yw->sectors_maxX2 - 1 && yy != yw->sectors_maxY2 - 1 )
                cells[xx].owner = ownmapp[xx];
            else
                cells[xx].owner = 0;

            yw->sectors_count_by_owner[ (int)cells[xx].owner ]++;
        }
    }

    return 1;
}

int cells_mark_hight(_NC_STACK_ypaworld *yw, const char *a2)
{
    if ( !yw->hgt_map )
        return 0;

    bitmap_intern *bitm;
    call_vtbl(yw->hgt_map, 3, 0x80002000, &bitm, 0);

    uint8_t *hgtMap = (uint8_t *)bitm->buffer;

    if ( bitm->width != yw->sectors_maxX2 || bitm->height != yw->sectors_maxY2 )
    {
        ypa_log_out("Mapsize mismatch %s: is [%d,%d], should be [%d,%d].\n", a2, bitm->width, bitm->height, yw->sectors_maxX2, yw->sectors_maxY2);
        delete_class_obj(yw->hgt_map);
        yw->hgt_map = NULL;
        return 0;
    }

    int sectors_cnt = yw->sectors_maxY2 * yw->sectors_maxX2;

    for (int i = 0; i < sectors_cnt; i++)
    {
        yw->cells[i].sector_height_meters = hgtMap[i] * -100.0;
    }

    for (int y = 1; y < yw->sectors_maxY2; y++)
    {
        for (int x = 1; x < yw->sectors_maxX2; x++)
        {
            int yy = y - 1;
            int xx = x - 1;

            int cur_sector_id = x + y * yw->sectors_maxX2;
            int left_sector = xx + y * yw->sectors_maxX2;
            int left_up_sector = xx + yy * yw->sectors_maxX2;
            int up_sector = x + yy * yw->sectors_maxX2;

            yw->cells[cur_sector_id].smooth_height = (yw->cells[cur_sector_id].sector_height_meters +
                    yw->cells[left_sector].sector_height_meters +
                    yw->cells[left_up_sector].sector_height_meters +
                    yw->cells[up_sector].sector_height_meters ) / 4.0;
        }
    }

    return 1;
}

int yw_createRobos(NC_STACK_ypaworld *ywo, _NC_STACK_ypaworld *yw, int robos_count, mapRobo *robo)
{
    stru_2d90 *f2d90 = yw->field_2d90;

    if ( f2d90->field_48 != 1 )
    {
        f2d90->ownerMap__has_vehicles = 0;
        f2d90->field_60 = 2;

        for (int i = 0; i < robos_count; i++)
        {
            mapRobo *v8 = robo + i;

            ypaworld_arg136 v14;
            v14.pos_x = v8->pos_x;
            v14.pos_y = -30000.0;
            v14.pos_z = v8->pos_z;

            v14.field_14 = 0;
            v14.field_18 = 50000.0;
            v14.field_1C = 0;
            v14.field_40 = 0;

            ypaworld_arg146 v15;
            v15.vehicle_id = v8->vehicle;
            v15.pos.sx = v8->pos_x;
            v15.pos.sy = v8->pos_y;
            v15.pos.sz = v8->pos_z;

            call_method(ywo, 136, &v14);

            if ( v14.field_20 )
                v15.pos.sy = v14.field_30 + v8->pos_y;

            NC_STACK_ypabact *bact = (NC_STACK_ypabact *)call_method(ywo, 146, &v15);

            if ( bact )
            {
                int v20 = 0;

                call_method(ywo, 134, bact);

                __NC_STACK_ypabact *bact_int;
                call_vtbl(bact, 3, 0x80001003, &bact_int, 0); // ypabact_func3

                int v12;

                if ( i )
                {
                    v12 = v8->energy;
                }
                else
                {
                    v12 = v8->energy / 4;

                    if ( v12 < yw->maxroboenergy )
                    {
                        v12 = yw->maxroboenergy;
                        v20 = yw->maxreloadconst;
                    }

                }

                bact_int->owner = v8->owner;
                bact_int->energy = v12;
                bact_int->energy_2 = v12;

                if ( !v20 )
                {
                    if ( v8->reload_const )
                        v20 = v8->reload_const;
                    else
                        v20 = bact_int->energy_2;
                }

                bact_int->reload_const_or_energy2 = v20;

                call_vtbl(bact, 2, 0x80001007, 1, 0);
                call_vtbl(bact, 2, 0x8000200B, 15, 0);
                call_vtbl(bact, 2, 0x80002008, v12, 0);
                call_vtbl(bact, 2, 0x8000200A, v12, 0);

                yw->field_2d90->ownerMap__has_vehicles |= 1 << v8->owner;

                call_vtbl(bact, 2, 0x80002002, v8->con_budget, 0);
                call_vtbl(bact, 2, 0x80002003, v8->def_budget, 0);
                call_vtbl(bact, 2, 0x80002004, v8->rad_budget, 0);
                call_vtbl(bact, 2, 0x80002005, v8->pow_budget, 0);
                call_vtbl(bact, 2, 0x80002006, v8->saf_budget, 0);
                call_vtbl(bact, 2, 0x8000200F, v8->cpl_budget, 0);
                call_vtbl(bact, 2, 0x80002011, v8->rob_budget, 0);
                call_vtbl(bact, 2, 0x80002010, v8->rec_budget, 0);
                call_vtbl(bact, 2, 0x80002013, v8->viewangle, 0);
                call_vtbl(bact, 2, 0x80002014, v8->saf_delay, 0);
                call_vtbl(bact, 2, 0x80002015, v8->pow_delay, 0);
                call_vtbl(bact, 2, 0x80002017, v8->cpl_delay, 0);
                call_vtbl(bact, 2, 0x80002016, v8->rad_delay, 0);
                call_vtbl(bact, 2, 0x80002018, v8->def_delay, 0);
                call_vtbl(bact, 2, 0x80002019, v8->con_delay, 0);
                call_vtbl(bact, 2, 0x8000201B, v8->rec_delay, 0);
                call_vtbl(bact, 2, 0x8000201A, v8->rob_delay, 0);

                if ( !i )
                {
                    call_vtbl(bact, 2, 0x80001004, 1, 0);
                    call_vtbl(bact, 2, 0x80001005, 1, 0);
                }
            }
        }
    }
    return 1;
}

int sub_44B9B8(NC_STACK_ypaworld *ywo, _NC_STACK_ypaworld *yw, const char *mapp)
{
    if ( !yw->blg_map )
        return 0;

    bitmap_intern *bitmp;
    call_vtbl(yw->blg_map, 3, 0x80002000, &bitmp, 0);

    uint8_t *blgMap = (uint8_t *)bitmp->buffer;

    if ( bitmp->width != yw->sectors_maxX2 || bitmp->height != yw->sectors_maxY2 )
    {
        ypa_log_out("Mapsize mismatch %s: is [%d,%d], should be [%d,%d].\n", mapp, bitmp->width, bitmp->height, yw->sectors_maxX2, yw->sectors_maxY2);
        delete_class_obj(yw->blg_map);
        yw->blg_map = NULL;
        return 0;
    }

    for ( int y = 0; y < yw->sectors_maxY2; y++)
    {
        uint8_t *blgln = blgMap + y * yw->sectors_maxX2;
        cellArea *cellln = yw->cells + y * yw->sectors_maxX2;

        for ( int x = 0; x < yw->sectors_maxX2; x++)
        {
            if (blgln[x] && cellln[x].owner)
            {
                ypaworld_arg148 arg148;

                arg148.ownerID = cellln[x].owner;
                arg148.ownerID2 = cellln[x].owner;
                arg148.blg_ID = blgln[x];
                arg148.field_C = 1;
                arg148.field_18 = 0;
                arg148.x = x;
                arg148.y = y;

                call_method(ywo, 148, &arg148);
            }
        }
    }

    return 1;
}

void ypaworld_func161__sub1(_NC_STACK_ypaworld *yw, unsigned int a2, squadProto *a3)
{
    printf("MAKE ME %s\n", "ypaworld_func161__sub1");
}

void sub_471CA0(_NC_STACK_ypaworld *yw)
{
    printf("MAKE ME %s\n", "sub_471CA0");
}

void sub_44C248(NC_STACK_ypaworld *yw, _NC_STACK_ypaworld *a2)
{
    printf("MAKE ME %s\n", "sub_44C248");
}

void sub_472130(_NC_STACK_ypaworld *yw)
{
    printf("MAKE ME %s\n", "sub_472130");
}

void sub_4D1084(_NC_STACK_ypaworld *yw)
{
    printf("MAKE ME %s\n", "sub_4D1084");
}

void sub_44F748(_NC_STACK_ypaworld *yw)
{
    printf("MAKE ME %s\n", "sub_44F748");
}


void sb_0x451034__sub1(_NC_STACK_ypaworld *yw)
{
    inputEngine__getter(0x80001009, &yw->input_class, 0);

    yw->field_7562 = 0;
    yw->field_7566 = -1;
    yw->field_756A = 0;
    yw->field_756E = 0;
}

int sb_0x451034(NC_STACK_ypaworld *obj, _NC_STACK_ypaworld *yw)
{
    if ( yw->field_162c )
        return yw->field_162c;

    yw->field_17b0 = 0;
    yw->field_17b4 = 0;
    yw->field_17bc = 0;
    yw->field_1b1c = 0;
    yw->field_1aa0 = 0;
    yw->field_1aa4 = 0;
    yw->field_1aa8 = 0;
    yw->field_1aac = 0;

    sb_0x451034__sub1(yw);

    printf("COMPLETE ME %s\n", "sb_0x451034");

    /*sb_0x451034__sub9(yw);
    sb_0x451034__sub8(yw);
    sb_0x451034__sub2(obj, yw);
    create_squad_man(obj, yw);
    create_info_log(obj, yw);
    create_exit_menu(yw);
    sb_0x451034__sub5(yw);
    sb_0x451034__sub6(yw);

    call_method(obj, 139, &robo_map);
    call_method(obj, 139, &squadron_manager);
    call_method(obj, 139, &info_log);
    call_method(obj, 139, &exit_menu);
    call_method(obj, 139, &lstvw);
    call_method(obj, 139, &node);

    if ( !sb_0x451034__sub3(obj, yw) )
        {
    call_method(obj, 140, &lstvw);
    call_method(obj, 140, &exit_menu);
    call_method(obj, 140, &info_log);
    call_method(obj, 140, &squadron_manager);
    call_method(obj, 140, &robo_map);
    call_method(obj, 140, &node);

      sub_4DA874(yw);
      sub_46E16C(obj, yw);
      sub_4C706C(obj, yw);
      return 0;
    }*/

    //call_vtbl(yw->tracyrmp_ilbm, 3, 0x80002000, &a4, 0);
    //call_vtbl(yw->shadermp_ilbm, 3, 0x80002000, &v8, 0);
    yw->field_162c = 1;

    if ( yw->snd__cdsound & 1 )
    {
        CDAUDIO_t v6;

        v6.command = 7;
        v6.track_id = yw->field_2d90->amb_track_p0;
        v6.field_8 = yw->field_2d90->amb_track_p1;
        v6.field_C = yw->field_2d90->amb_track_p2;

        sub_4444D4(&v6);

        v6.command = 2;
        sub_4444D4(&v6);
    }

    return 1;
}



void sb_0x44fc60(_NC_STACK_ypaworld *yw, cellArea *cell, int secX, int secY, int a5, int a6)
{

}



void sub_44DBF8(_NC_STACK_ypaworld *yw, int _dx, int _dz, int _dxx, int _dzz, struct_44dbf8 *a6, int flags)
{
    int v8 = flags;

    a6->sklt = NULL;
    a6->field_1E = 0;
    a6->field_1C = 0;

    if ( _dxx >= 1  &&  _dxx < 4 * yw->sectors_maxX2 - 1  &&  _dzz >= 1  &&  _dzz < 4 * yw->sectors_maxY2 - 1 )
    {
        a6->sec_x = _dxx / 4;
        a6->sec_y = _dzz / 4;

        a6->p_cell = &yw->cells[a6->sec_x + yw->sectors_maxX2 * (_dzz / 4)];

        if ( _dxx % 4 && _dzz % 4 )
        {
            a6->field_1C = 1;

            int v14, v16;

            if ( a6->p_cell->field_2E == 1 )
            {
                v14 = 0;
                v16 = 0;

                if ( (_dxx / 4) == (_dx / 4)  &&  (_dz / 4) == (_dzz / 4) )
                    v8 = flags & 0xFFFE;

                a6->pos_x =   1200.0 * a6->sec_x + 600.0;
                a6->pos_y = a6->p_cell->sector_height_meters;
                a6->pos_z = -(1200.0 * a6->sec_y + 600.0);
            }
            else
            {
                v16 = (_dxx % 4) - 1;
                v14 = 2 - ((_dzz % 4) - 1);

                if ( _dxx == _dx && _dzz == _dz )
                    v8 = flags & 0xFFFE;

                a6->pos_z = -(_dzz * 300.0);
                a6->pos_x = _dxx * 300.0;
                a6->pos_y = a6->p_cell->sector_height_meters;
            }

            a6->field_1E = v8;

            int model_id = yw->secTypes[a6->p_cell->sec_type].buildings[v16][v14]->health_models [   yw->build_hp_ref[    a6->p_cell->buildings_health[v16][v14]    ]    ];

            if ( v8 & 1 )
                a6->sklt = yw->legos[model_id].selected_sklt_intern;
            else
                a6->sklt = yw->legos[model_id].sklt_obj_intern;
        }
        else
        {
            a6->pos_y = 0;
            a6->pos_x = _dxx * 300.0;
            a6->pos_z = -(_dzz * 300.0);

            if ( _dxx == _dx && _dzz == _dz )
                v8 = flags & 0xFE;

            a6->field_1E = v8;

            if ( _dxx % 4 == 0 && _dzz % 4 == 0)
            {
                a6->sklt = yw->ColCross.skeleton_internal;
                a6->field_1C = 4;
            }
            else if ( _dxx % 4 == 0 && _dzz % 4 != 0 )
            {
                a6->sklt = yw->ColSide.skeleton_internal;
                a6->field_1C = 2;
            }
            else if ( _dxx % 4 != 0 && _dzz % 4 == 0 )
            {
                a6->sklt = yw->ColSide.skeleton_internal;
                a6->field_1C = 3;
            }
        }

        if ( a6->field_1C && !a6->sklt )
        {
            ypa_log_out("yw_GetSklt: WARNING, not CZT_INVALID, but Sklt NULL!\n");

            const char *v17 = "UNKNOWN";

            switch ( a6->field_1C )
            {
            case 4:
                v17 = "czt_cross_slurp";
                break;
            case 2:
                v17 = "czt_vside_slurp";
                break;
            case 3:
                v17 = "czt_hside_slurp";
                break;
            case 1:
                v17 = "czt_lego";
                break;
            default:
                break;
            }
            ypa_log_out("    Type=%s, sec_x=%d, sec_y=%d.\n", v17, a6->sec_x, a6->sec_y);

            a6->field_1C = 0;
        }
    }
}

void sub_44DF60(skeleton_64_stru *arg, int id)
{
    int16_t *vtx = &arg->pol_entries[id]->v1;

    pol_entries2 *triangle = &arg->triangles[id];

    int vtx1 = vtx[0];
    int vtx2 = vtx[1];
    int vtx3 = vtx[2];

    float sy1 = arg->POO[vtx2].pos3f.sy - arg->POO[vtx1].pos3f.sy;
    float sz1 = arg->POO[vtx2].pos3f.sz - arg->POO[vtx1].pos3f.sz;
    float sx1 = arg->POO[vtx2].pos3f.sx - arg->POO[vtx1].pos3f.sx;
    float sz2 = arg->POO[vtx3].pos3f.sz - arg->POO[vtx2].pos3f.sz;
    float sx2 = arg->POO[vtx3].pos3f.sx - arg->POO[vtx2].pos3f.sx;
    float sy2 = arg->POO[vtx3].pos3f.sy - arg->POO[vtx2].pos3f.sy;

    triangle->field_4 = sx2 * sz1 - sx1 * sz2;
    triangle->field_0 = sy1 * sz2 - sy2 * sz1;
    triangle->field_8 = sx1 * sy2 - sx2 * sy1;

    float v28 = (triangle->field_4 * triangle->field_4) + (triangle->field_0 * triangle->field_0) + (triangle->field_8 * triangle->field_8);

    if ( v28 < 0.0 )
        v28 = 0.0;

    float v25 = 1.0 / sqrt(v28);

    triangle->field_0 = triangle->field_0 * v25;
    triangle->field_4 = triangle->field_4 * v25;
    triangle->field_8 = triangle->field_4 * v25 * triangle->field_8;
    triangle->field_C = -(triangle->field_4 * arg->POO[vtx1].pos3f.sy + triangle->field_0 * arg->POO[vtx1].pos3f.sx + triangle->field_8 * arg->POO[vtx1].pos3f.sz);
}

void sub_44E07C(_NC_STACK_ypaworld *yw, struct_44dbf8 *arg)
{
    if ( arg->field_1C == 2 )
    {
        cellArea *cur = arg->p_cell;
        cellArea *left = arg->p_cell - 1;

        if ( !(arg->field_1E & 1) || abs(cur->sector_height_meters - left->sector_height_meters) < 500.0 )
        {

            arg->sklt->POO[0].pos3f.sy = left->sector_height_meters;
            arg->sklt->POO[1].pos3f.sy = cur->sector_height_meters;
            arg->sklt->POO[2].pos3f.sy = cur->sector_height_meters;
            arg->sklt->POO[3].pos3f.sy = left->sector_height_meters;

            sub_44DF60(arg->sklt, 0);
        }
        else
        {
            arg->sklt = yw->colsub_sklt_intrn;

            if ( cur->sector_height_meters > left->sector_height_meters )
                arg->pos_y = cur->sector_height_meters;
            else
                arg->pos_y = left->sector_height_meters;
        }
    }
    else if ( arg->field_1C == 3 )
    {
        cellArea *cur = arg->p_cell;
        cellArea *up = arg->p_cell - yw->sectors_maxX2;

        if ( !(arg->field_1E & 1) || abs(cur->sector_height_meters - up->sector_height_meters) < 500.0 )
        {
            arg->sklt->POO[0].pos3f.sy = up->sector_height_meters;
            arg->sklt->POO[1].pos3f.sy = up->sector_height_meters;
            arg->sklt->POO[2].pos3f.sy = cur->sector_height_meters;
            arg->sklt->POO[3].pos3f.sy = cur->sector_height_meters;

            sub_44DF60(arg->sklt, 0);
        }
        else
        {
            arg->sklt = yw->colsub_sklt_intrn;

            if ( cur->sector_height_meters > up->sector_height_meters )
                arg->pos_y = cur->sector_height_meters;
            else
                arg->pos_y = up->sector_height_meters;
        }
    }
    else if ( arg->field_1C == 4 )
    {

        int kk = 0;

        cellArea *cur = arg->p_cell;
        cellArea *left = arg->p_cell - 1;
        cellArea *up = arg->p_cell - yw->sectors_maxX2;
        cellArea *leftup = arg->p_cell - yw->sectors_maxX2 - 1;

        if ( arg->field_1E & 1 )
        {
            float cs = cur->sector_height_meters;
            float ls = left->sector_height_meters;
            float us = up->sector_height_meters;
            float lus = leftup->sector_height_meters;

            float v15, v16, v17, v18;

            if ( cs >= ls )
                v15 = ls;
            else
                v15 = cs;

            if ( us >= lus )
                v16 = lus;
            else
                v16 = us;

            if ( v15 < v16 )
                v16 = v15;

            if ( cs <= ls )
                v17 = ls;
            else
                v17 = cs;

            if ( us <= lus )
                v18 = lus;
            else
                v18 = us;

            if ( v17 > v18 )
                v18 = v17;

            if ( abs( (int)(v18 - v16) ) > 300.0 )
            {
                arg->sklt = yw->colsub_sklt_intrn;
                arg->pos_y = v18;
                kk = 1;
            }
        }
        if ( !kk )
        {
            arg->sklt->POO[0].pos3f.sy = leftup->sector_height_meters;
            arg->sklt->POO[1].pos3f.sy = up->sector_height_meters;
            arg->sklt->POO[2].pos3f.sy = cur->sector_height_meters;
            arg->sklt->POO[3].pos3f.sy = left->sector_height_meters;
            arg->sklt->POO[4].pos3f.sy = cur->smooth_height;

            sub_44DF60(arg->sklt, 0);
            sub_44DF60(arg->sklt, 1);
            sub_44DF60(arg->sklt, 2);
            sub_44DF60(arg->sklt, 3);
        }
    }
}

int sub_44D36C(float dx, float dy, float dz, int id, skeleton_64_stru *sklt)
{
    pol_entries2 *triangle = &sklt->triangles[id];

    int v7 = 0;

    float v24 = abs(triangle->field_0);
    float v23 = abs(triangle->field_4);
    float v27 = abs(triangle->field_8);

    float v9 = (v24 <= v23 ? v23 : v24 );
    v9 = (v9 <= v27 ? v27 : v9);

    if ( v9 == v24 )
    {
        int prev = sklt->pol_entries[id]->num_vertices - 1;

        for (int i = 0; i < sklt->pol_entries[id]->num_vertices; i++)
        {
            int16_t *vtx = &sklt->pol_entries[id]->v1;

            skeleton_type1 *v12 = &sklt->POO[ vtx[i] ];
            skeleton_type1 *v13 = &sklt->POO[ vtx[prev] ];

            if ( ( (v13->pos3f.sz <= dz && dz < v12->pos3f.sz) ||
                    (v12->pos3f.sz <= dz && dz < v13->pos3f.sz) ) &&
                    v13->pos3f.sy + (v12->pos3f.sy - v13->pos3f.sy) * (dz - v13->pos3f.sz) / (v12->pos3f.sz - v13->pos3f.sz) > dy )
            {
                v7 = v7 == 0;
            }

            prev = i;
        }
    }
    else if ( v9 == v23 )
    {
        int prev = sklt->pol_entries[id]->num_vertices - 1;

        for (int i = 0; i < sklt->pol_entries[id]->num_vertices; i++)
        {
            int16_t *vtx = &sklt->pol_entries[id]->v1;

            skeleton_type1 *v12 = &sklt->POO[ vtx[i] ];
            skeleton_type1 *v13 = &sklt->POO[ vtx[prev] ];

            if ( ( (v13->pos3f.sz <= dz && dz < v12->pos3f.sz) ||
                    (v12->pos3f.sz <= dz && dz < v13->pos3f.sz) ) &&
                    v13->pos3f.sx + (v12->pos3f.sx - v13->pos3f.sx) * (dz - v13->pos3f.sz) / (v12->pos3f.sz - v13->pos3f.sz) > dx )
            {
                v7 = v7 == 0;
            }

            prev = i;
        }
    }
    else if ( v9 == v27 )
    {
        int prev = sklt->pol_entries[id]->num_vertices - 1;

        for (int i = 0; i < sklt->pol_entries[id]->num_vertices; i++)
        {
            int16_t *vtx = &sklt->pol_entries[id]->v1;

            skeleton_type1 *v12 = &sklt->POO[ vtx[i] ];
            skeleton_type1 *v13 = &sklt->POO[ vtx[prev] ];

            if ( ( (v13->pos3f.sy <= dy && dy < v12->pos3f.sy) ||
                    (v12->pos3f.sy <= dy && dy < v13->pos3f.sy) ) &&
                    v13->pos3f.sx + (v12->pos3f.sx - v13->pos3f.sx) * (dy - v13->pos3f.sy) / (v12->pos3f.sy - v13->pos3f.sy) > dx )
            {
                v7 = v7 == 0;
            }

            prev = i;
        }
    }
    return v7;
}

void sub_44D8B8(ypaworld_arg136 *arg, struct_44dbf8 *loc)
{
    for ( int i = 0; i < loc->sklt->pol_count; i++)
    {
        pol_entries2 *triangle = &loc->sklt->triangles[i];

        float v11 = triangle->field_4 * arg->field_18 + triangle->field_0 * arg->field_14 + triangle->field_8 * arg->field_1C;

        if ( v11 > 0.0 )
        {
            float v19 = -(triangle->field_4 * arg->pos_y + triangle->field_0 * arg->pos_x + triangle->field_8 * arg->pos_z + triangle->field_C) / v11;

            if ( v19 > 0.0 && v19 <= 1.0 && v19 < arg->field_24 )
            {
                float pz = arg->field_1C * v19 + arg->pos_z;
                float py = arg->field_18 * v19 + arg->pos_y;
                float px = arg->field_14 * v19 + arg->pos_x;

                if ( sub_44D36C(px, py, pz, i, loc->sklt) )
                {
                    arg->field_20 = 1;
                    arg->field_24 = v19;
                    arg->field_2C = loc->pos_x + px;
                    arg->field_30 = loc->pos_y + py;
                    arg->field_34 = loc->pos_z + pz;
                    arg->field_38 = i;
                    arg->field_3C = loc->sklt;
                }
            }
        }
    }
}


void ypaworld_func2__sub0__sub0(_NC_STACK_ypaworld *yw)
{
    if ( !yw->field_739A || (!(yw->field_73CE & 4) && !(yw->field_73CE & 8)) )
    {
        if ( yw->field_1b84 )
        {
            yw->field_7562 = yw->field_1614;

            winp_71arg winp71;
            winp71.effID = 0;
            winp71.state = 1;
            winp71.p4 = 0;
            winp71.p3 = 0;
            winp71.p2 = 0;
            winp71.p1 = 0;

            input__func66__params arg66;

            if ( yw->input_class )
            {
                arg66.field_0 = 1;
                arg66.funcID = 71;
                arg66.field_4 = 0;
                arg66.vals = &winp71;

                call_method(yw->input_class, 66, &arg66);
            }

            int v2;
            float v13;
            float v14;
            float v15;
            float v16;
            float v17;
            float v18;
            float v19;
            float v22;
            float v24;
            float v25;


            switch ( yw->field_1b84->field_24 )
            {
            case 1:
                v2 = 3;
                v16 = 300.0;
                v13 = 800.0;
                v15 = 1.0;
                v14 = 2.0;
                v25 = 1.0;
                v18 = 0.0;
                v17 = 0.7;
                v24 = 0.3;
                v22 = 1.0;
                v19 = 0.5;
                break;

            case 2:
            case 8:
                v2 = 1;
                v16 = 200.0;
                v13 = 500.0;
                v15 = 0.6;
                v14 = 1.0;
                v25 = 1.0;
                v22 = 1.0;
                v17 = 0.1;
                v24 = 0.3;
                v19 = 0.4;
                v18 = 0.0;
                break;

            case 6:
                v2 = 2;
                v16 = 200.0;
                v13 = 500.0;
                v15 = 1.0;
                v14 = 2.0;
                v25 = 1.0;
                v18 = 0.0;
                v17 = 0.1;
                v24 = 0.3;
                v22 = 1.0;
                v19 = 0.75;
                break;

            default:
                v2 = -1;
                break;
            }

            if ( v2 != -1 )
            {
                float v4 = (yw->field_1b84->mass - v16) / (v13 - v16);
                float v5 = (yw->field_1b84->maxrot - v15) / (v14 - v15);

                float v21 = (v19 - v24) * v4 + v24;
                float v23 = (v18 - v22) * v4 + v22;

                float v20 = (v17 - v25) * v5 + v25;

                if ( v21 < v24)
                    v21 = v24;
                else if (v21 > v19)
                    v21 = v19;

                if ( v23 < v18 )
                    v23 = v18;
                else if ( v23 > v22)
                    v23 = v22;

                if ( v20 < v17)
                    v20 = v17;
                else if ( v20 > v25)
                    v20 = v25;

                yw->field_7566 = v2;
                yw->field_756A = v23;
                yw->field_756E = v21;

                winp71.effID = v2;
                winp71.p1 = v21;
                winp71.state = 0;
                winp71.p2 = v23;

                winp71.p4 = 0;
                winp71.p3 = 0;

                if ( yw->input_class )
                {
                    arg66.field_4 = 0;
                    arg66.field_0 = 1;
                    arg66.vals = &winp71;
                    arg66.funcID = 71;

                    call_method(yw->input_class, 66, &arg66);
                }

                winp71.state = 0;
                winp71.p1 = v20;
                winp71.p4 = 0;
                winp71.p3 = 0;
                winp71.p2 = 0;
                winp71.effID = 4;

                if ( yw->input_class )
                {
                    arg66.field_4 = 0;
                    arg66.field_0 = 1;
                    arg66.funcID = 71;
                    arg66.vals = &winp71;
                    call_method(yw->input_class, 66, &arg66);
                }
            }
        }
    }
}


void ypaworld_func2__sub0__sub1(_NC_STACK_ypaworld *yw, __NC_STACK_ypabact *bact1, __NC_STACK_ypabact *bact2)
{
    printf("MAKE ME %s\n", "ypaworld_func2__sub0__sub1");
}



NC_STACK_ypabact *yw_createUnit(NC_STACK_ypaworld *ywo, _NC_STACK_ypaworld *yw, int model_id)
{
    const char *unit_classes_names [11] =
    {
        "dummy.class",      // 0
        "ypabact.class",    // 1
        "ypatank.class",    // 2
        "yparobo.class",    // 3
        "ypamissile.class", // 4
        "ypazepp.class",    // 5
        "ypaflyer.class",   // 6
        "ypaufo.class",     // 7
        "ypacar.class",     // 8
        "ypagun.class",     // 9
        "ypahovercraft.class" //10
    };

    NC_STACK_ypabact *bacto = NULL;
    __NC_STACK_ypabact *bact = NULL;

    // Find dead units
    bact_node *node = (bact_node *)yw->dead_cache.head;

    while (node->next)
    {
        if (node->bact->field_24 == model_id)
        {
            bacto = node->bact->self;
            bact = node->bact;
            break;
        }

        node = (bact_node *)node->next;
    }

    if ( !bacto )
    {
        bacto = (NC_STACK_ypabact *)init_get_class(unit_classes_names[model_id], 0x80001001, ywo, 0);

        if ( !bacto )
            return NULL;

        call_vtbl(bacto, 3, 0x80001003, &bact, 0);
    }

    call_method(bacto, 96, 0); // Reset bact

    bact->ypabact__id = bact_id;
    bact->owner = 0;

    bact->field_651.m00 = 1.0;    // 1.0
    bact->field_651.m01 = 0;
    bact->field_651.m02 = 0;
    bact->field_651.m10 = 0;
    bact->field_651.m11 = 1.0;    // 1.0
    bact->field_651.m12 = 0;
    bact->field_651.m20 = 0;
    bact->field_651.m21 = 0;
    bact->field_651.m22 = 1.0;    // 1.0

    bact_id++;

    return bacto;
}


void sub_44BF34(vhclSndFX *sndfx)
{
    char rsr[256];

    if ( !sndfx->wavs[0] && !sndfx->single_sample )
    {
        strcpy(rsr, get_prefix_replacement("rsrc"));

        set_prefix_replacement("rsrc", "data:");

        if ( sndfx->extCount )
        {
            for (int i = 0; i < sndfx->extCount; i++)
            {
                sndfx->wavs[i] = (NC_STACK_wav *)init_get_class("wav.class", 0x80001000, sndfx->extSampleNames[i], 0);

                if ( sndfx->wavs[i] )
                {
                    sampl *sample;
                    call_vtbl(sndfx->wavs[i], 3, 0x80002000, &sample, 0);

                    sndfx->sndExts[i].sample = sample;
                    sndfx->sndExts[i].field_14 = sample->SampleRate * sndfx->sndExts[i].field_C / 11000;
                    sndfx->sndExts[i].field_18 = sample->SampleRate * sndfx->sndExts[i].field_10 / 11000;

                    if ( sndfx->sndExts[i].field_14 > sample->bufsz )
                        sndfx->sndExts[i].field_14 = sample->bufsz;

                    if ( !sndfx->sndExts[i].field_18 )
                        sndfx->sndExts[i].field_18 = sample->bufsz;

                    if ( sndfx->sndExts[i].field_18 + sndfx->sndExts[i].field_14 > sample->bufsz )
                        sndfx->sndExts[i].field_18 = sample->bufsz - sndfx->sndExts[i].field_14;
                }
                else
                {
                    ypa_log_out("Warning: Could not load sample %s.\n", sndfx->extSampleNames[i]);
                }
            }
        }
        else if ( sndfx->sample_name[0] )
        {
            sndfx->single_sample = (NC_STACK_wav *)init_get_class("wav.class", 0x80001000, sndfx->sample_name, 0);

            if ( !sndfx->single_sample )
                ypa_log_out("Warning: Could not load sample %s.\n", sndfx->sample_name);
        }

        set_prefix_replacement("rsrc", rsr);
    }
}


void sub_4D7F60(_NC_STACK_ypaworld *yw, int x, int y, stru_a3 *sct, base77Func *bs77)
{
    sct->dword8 = 0;
    sct->dword4 = 0;

    if ( x >= 0  &&  x < yw->sectors_maxX2  &&  y >= 0  &&  y < yw->sectors_maxY2 )
    {
        sct->dword4 = 1;
        sct->p_cell = yw->cells + x + yw->sectors_maxX2 * y;
        sct->smooth_height = sct->p_cell->smooth_height;

        flag_xyz grp_1;
        grp_1.flag = 7;
        grp_1.x = x * 1200.0 + 600.0;
        grp_1.z = -(y * 1200.0 + 600.0);
        grp_1.y = sct->p_cell->sector_height_meters;

        sct->x = grp_1.x;
        sct->y = grp_1.y;
        sct->z = grp_1.z;

        call_method(yw->additionalBeeBox, 68, &grp_1);

        if ( call_method(yw->additionalBeeBox, 77, bs77) )
        {
            sct->dword8 = 1;
        }
    }
}

void sub_4D806C(_NC_STACK_ypaworld *yw, stru_a3 *sct, base77Func *bs77)
{
    if ( sct->dword8 )
    {
        cellArea *pcell = sct->p_cell;

        flag_xyz grp_1;
        grp_1.flag = 7;
        grp_1.x = sct->x;
        grp_1.y = sct->y;
        grp_1.z = sct->z;

        int v22 = 0;

        flag_xyz scel;

        if ( pcell->field_3A == 1 )
        {
            yw_f80 *v5 = &yw->field_80[ pcell->field_3B ];

            scel.flag = 2;
            scel.y = (float)v5->field_4 / (float)v5->field_8;

            pcell->sec_type = yw->BuildProtos[ v5->blg_ID ].sec_type;
            pcell->field_2E = yw->secTypes[ pcell->sec_type ].field_0;

            v22 = 1;
        }

        int v17, v20;

        if ( pcell->field_2E == 1 )
        {
            v17 = 0;
            v20 = 1;
        }
        else
        {
            v17 = -1;
            v20 = 3;
        }

        for (int zz = 0; zz < v20; zz++)
        {
            for (int xx = 0; xx < v20; xx++)
            {
                grp_1.x = (v17 + xx) * 300.0 + sct->x;
                grp_1.z = (v17 + zz) * 300.0 + sct->z;

                if ( v22 )
                {
                    NC_STACK_base *bld = yw->legos[ yw->secTypes[ pcell->sec_type ].buildings[xx][zz]->health_models[0] ].base;

                    call_vtbl(bld, 2, 0x80001024, 0, 0);

                    call_method(bld, 72, &scel);
                    call_method(bld, 68, &grp_1);
                    call_method(bld, 77, bs77);

                    call_vtbl(bld, 2, 0x80001024, 1, 0);
                }
                else
                {
                    NC_STACK_base *bld = yw->legos[ yw->secTypes[ pcell->sec_type ].buildings[xx][zz]->health_models[ yw->build_hp_ref[ pcell->buildings_health[xx][zz] ] ] ].base;

                    call_method(bld, 68, &grp_1);
                    call_method(bld, 77, bs77);
                }
            }
        }
    }

    __NC_STACK_ypabact *bact = (__NC_STACK_ypabact *)sct->p_cell->field_3C.head;

    while ( bact->next )
    {
        if ( sct->dword8 || bact->field_24 == 3)
            call_method(bact->self, 66, bs77);

        bact = (__NC_STACK_ypabact *)bact->next;
    }
}

void yw_renderSky(_NC_STACK_ypaworld *yw, base77Func *rndr_params)
{
    if ( yw->sky_loaded_base )
    {
        float v6 = rndr_params->field_24;
        flag_xyz v5;
        v5.x = yw->current_bact->field_621.sx;
        v5.y = yw->field_15f4 + yw->current_bact->field_621.sy;
        v5.z = yw->current_bact->field_621.sz;
        v5.flag = 7;

        call_method(yw->sky_loaded_base, 68, &v5);

        rndr_params->field_24 = 32000.0;

        call_method(yw->sky_loaded_base, 77, rndr_params);

        rndr_params->field_24 = v6;
    }
}

void sb_0x4d7c08__sub1(_NC_STACK_ypaworld *yw, base77Func *arg)
{
    //Render superItems
    printf("MAKE ME %s\n", "sb_0x4d7c08__sub1");
}

void sb_0x4d7c08__sub0(_NC_STACK_ypaworld *yw)
{
    //Draw UI
    printf("MAKE ME %s\n", "sb_0x4d7c08__sub0");
}


NC_STACK_base * sb_0x4d7c08__sub3__sub0(_NC_STACK_ypaworld *yw, stru_a3 *sct, stru_a3 *sct2, float a4, float a5)
{
    if ( sct->dword4 != 1 || sct2->dword4 != 1 || (sct->dword8 != 1 && sct2->dword8 != 1) )
        return 0;

    int i = yw->secTypes[ sct->p_cell->sec_type ].field_1;
    int j = yw->secTypes[ sct2->p_cell->sec_type ].field_1;

    NC_STACK_base *bs = yw->slurps2[i][j].skeletons_bas;
    skeleton_64_stru *skel = yw->slurps2[i][j].skeleton_internal;

    flag_xyz grp_1;
    grp_1.flag = 5;
    grp_1.x = sct2->x;
    grp_1.z = sct2->z;

    call_method(bs, 68, &grp_1);

    for (int i = 0; i < 4; i++)
        skel->POO[i].pos3f.sy = sct->y;

    for (int i = 4; i < 8; i++)
        skel->POO[i].pos3f.sy = sct2->y;

    skel->POO[8].pos3f.sy = a5;
    skel->POO[9].pos3f.sy = a4;

    return bs;
}

NC_STACK_base * sb_0x4d7c08__sub3__sub1(_NC_STACK_ypaworld *yw, stru_a3 *sct, stru_a3 *sct2, float a4, float a5)
{
    if ( sct->dword4 != 1 || sct2->dword4 != 1 || (sct->dword8 != 1 && sct2->dword8 != 1) )
        return NULL;

    int i = yw->secTypes[ sct->p_cell->sec_type ].field_1;
    int j = yw->secTypes[ sct2->p_cell->sec_type ].field_1;

    NC_STACK_base *bs = yw->slurps1[i][j].skeletons_bas;
    skeleton_64_stru *skel = yw->slurps1[i][j].skeleton_internal;

    flag_xyz grp_1;
    grp_1.flag = 5;
    grp_1.x = sct2->x;
    grp_1.z = sct2->z;

    call_method(bs, 68, &grp_1);

    for (int i = 0; i < 4; i++)
        skel->POO[i].pos3f.sy = sct->y;

    for (int i = 4; i < 8; i++)
        skel->POO[i].pos3f.sy = sct2->y;

    skel->POO[8].pos3f.sy = a5;
    skel->POO[9].pos3f.sy = a4;

    return bs;
}




stru_a3 rendering_sectors[10][10];

void sb_0x4d7c08__sub3(_NC_STACK_ypaworld *yw, base77Func *arg)
{
    //Render empty sectors and modify landscape linking parts
    for (int i = 0; i < yw->field_1368; i++)
    {
        for (int j = 0; j < yw->field_1368 - 1; j++)
        {
            stru_a3 *sct = &rendering_sectors[j][i];
            stru_a3 *sct2 = &rendering_sectors[j + 1][i];

            float h = rendering_sectors[j + 1][i].smooth_height;
            float h2 = rendering_sectors[j + 1][i + 1].smooth_height;

            NC_STACK_base *bs = sb_0x4d7c08__sub3__sub0(yw, sct, sct2, h, h2);
            if ( bs )
                call_method(bs, 77, arg);
        }
    }

    for (int i = 0; i < yw->field_1368 - 1; i++)
    {
        for (int j = 0; j < yw->field_1368; j++)
        {
            stru_a3 *sct = &rendering_sectors[j][i];
            stru_a3 *sct2 = &rendering_sectors[j][i + 1];

            float h = rendering_sectors[j][i + 1].smooth_height;
            float h2 = rendering_sectors[j + 1][i + 1].smooth_height;

            NC_STACK_base *bs = sb_0x4d7c08__sub3__sub1(yw, sct, sct2, h, h2);
            if ( bs )
                call_method(bs, 77, arg);

        }
    }
}

void sb_0x4d7c08(NC_STACK_ypaworld *ywo, _NC_STACK_ypaworld *yw, base_64arg *bs64, int a2)
{
    if ( yw->current_bact )
    {
        base_1c_struct *v5 = sub_430A28();

        if ( v5 )
            sub_430A38(v5);

        base77Func rndrs;

        rndrs.field_0 = bs64->field_4;
        rndrs.field_4 = bs64->field_0;
        rndrs.field_14 = 0;
        rndrs.curOutPoly = p_outPolys;
        rndrs.field_18 = 2000;
        rndrs.polysData = p_polysdata;
        rndrs.field_1C = 1;
        rndrs.polysData_end = p_polysdata_end;

        rndrs.field_20 = 17.0;

        if ( dword_514EFC )
            rndrs.field_20 = 1.0;

        if ( yw->field_1368 == 5 )
            rndrs.field_24 = 1500.0;
        else
            rndrs.field_24 = 3500.0;

        int v6 = yw->field_1368 - 1;

        for (int j = 0; j < v6; j++)
        {
            for (int i = 0; i < v6; i++)
            {
                rendering_sectors[j][i].dword4 = 0;
                rendering_sectors[j][i].dword8 = 0;
            }
        }

        int v29 = v6 / 2;
        for (int i = 0; i <= v29; i++)
        {
            int v28 = v29 - i;

            for (int j = -i; j <= i; j++)
            {
                stru_a3 *sct = &rendering_sectors[v29 + j][v29 - v28];

                sub_4D7F60(yw, j + yw->current_bact->field_c, -v28 + yw->current_bact->field_10, sct, &rndrs);

                if ( sct->dword4 )
                    sub_4D806C(yw, sct, &rndrs);

            }

            if ( -v28 != v28 )
            {
                for (int j = -i; j <= i; j++)
                {
                    stru_a3 *sct = &rendering_sectors[v29 + j][v29 + v28];

                    sub_4D7F60(yw, j + yw->current_bact->field_c, v28 + yw->current_bact->field_10, sct, &rndrs);

                    if ( sct->dword4 )
                        sub_4D806C(yw, sct, &rndrs);
                }
            }
        }

        sb_0x4d7c08__sub3(yw, &rndrs);
        sb_0x4d7c08__sub1(yw, &rndrs);

        if ( yw->field_15f8 )
            yw_renderSky(yw, &rndrs);

        bs64->field_C = rndrs.field_14;

        yw->field_1B6A = rndrs.field_14;
        yw->field_1b6c = rndrs.curOutPoly - p_outPolys;

        if ( yw->field_1b6c > 1 && !dword_514EFC )
            qsort(p_outPolys, yw->field_1b6c, sizeof(polys), sub_4D7BFC);

        call_method(yw->win3d, 213, 0);

        for (int i = 0; i < yw->field_1b6c; i++)
        {
            polysDat *pol = p_outPolys[i].data;
            pol->render_func(&pol->datSub);
        }

        call_method(yw->win3d, 214, 0);

        if ( a2 )
        {
            call_method(yw->win3d, 215, 0);
            sb_0x4d7c08__sub0(yw);
            call_method(yw->win3d, 216, 0);
        }
    }
}
