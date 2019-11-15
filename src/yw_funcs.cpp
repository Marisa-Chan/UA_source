#include <inttypes.h>
#include <string.h>

#include "includes.h"
#include "def_parser.h"
#include "yw.h"
#include "yw_internal.h"
#include "lstvw.h"
#include "font.h"
#include "button.h"
#include "windp.h"

extern GuiList stru_5C91D0;


void delete_tile_stuct(tiles_stru *tileset)
{
    if ( tileset )
    {
        if ( tileset->font_image )
            delete_class_obj(tileset->font_image);
        if ( tileset->chars )
            nc_FreeMem(tileset->chars);
        nc_FreeMem(tileset);
    }
}

tiles_stru * yw_LoadFont(_NC_STACK_ypaworld *yw, const char *fontname)
{
    char filename[256];

    strcpy(filename, "rsrc:");

    if ( yw->screen_width >= 512 )
        strcat(filename, "hfonts/");
    else
        strcat(filename, "fonts/");

    strcat(filename, fontname);

    FSMgr::FileHandle *fil = uaOpenFile(filename, "r");
    if ( !fil )
        return NULL;

    tiles_stru *tileset = (tiles_stru *)AllocVec(sizeof(tiles_stru), 65537);

    if ( tileset )
    {
        tile_xy *v12 = (tile_xy *)AllocVec(sizeof(tile_xy) * 256, 65537);
        tileset->chars = v12;
        if ( !v12 )
        {
            delete_tile_stuct(tileset);
            tileset = NULL;
        }
    }

    if ( tileset )
    {
        char buf[128];

        if ( !fil->gets(buf, 128) )
        {
            ypa_log_out("yw_LoadFont(): font %s, font definition file corrupt.\n", fontname);
            if ( tileset )
            {
                if ( tileset->font_image )
                    delete_class_obj(tileset->font_image);
                if ( tileset->chars )
                    nc_FreeMem(tileset->chars);
                nc_FreeMem(tileset);
            }
            delete fil;
            return NULL;
        }

        char *chrpos = buf;

        while (*chrpos)
        {
            if (*chrpos == '\n')
            {
                *chrpos = 0;
                break;
            }
            chrpos++;
        }

        char *bitmap_name = strtok(buf, " \t");

        if ( !bitmap_name )
        {
            if ( tileset )
            {
                if ( tileset->font_image )
                    delete_class_obj(tileset->font_image);
                if ( tileset->chars )
                    nc_FreeMem(tileset->chars);
                nc_FreeMem(tileset);
            }
            ypa_log_out("yw_LoadFont(): font %s, font definition file corrupt.\n", fontname);
            delete fil;
            return NULL;
        }

        IDVList init_vals;
        init_vals.Add(NC_STACK_rsrc::RSRC_ATT_NAME, bitmap_name);
        init_vals.Add(NC_STACK_bitmap::BMD_ATT_TEXTURE, 1);
        init_vals.Add(NC_STACK_bitmap::BMD_ATT_TEXTURE_SYS, 1);

        tileset->font_image = Nucleus::CInit<NC_STACK_ilbm>(init_vals);
        if ( !tileset->font_image )
        {
            if ( tileset->chars )
                nc_FreeMem(tileset->chars);
            nc_FreeMem(tileset);
            ypa_log_out("yw_LoadFont(): font %s, couldn't load fontpage %s.\n", fontname, bitmap_name);
            delete fil;
            return NULL;
        }

        tileset->field_4 = tileset->font_image->getBMD_pBitmap();
        tileset->field_8 = tileset->field_4->buffer;

        char *fntHeight = strtok(0, " \t");
        if ( !fntHeight )
        {
            ypa_log_out("yw_LoadFont(): font %s, font definition file corrupt.\n", fontname);
            if ( tileset )
            {
                if ( tileset->font_image )
                    delete_class_obj(tileset->font_image);
                if ( tileset->chars )
                    nc_FreeMem(tileset->chars);
                nc_FreeMem(tileset);
            }
            delete fil;
            return NULL;
        }

        tileset->font_height = strtol(fntHeight, NULL, 0);

        while ( fil->gets(buf, 128) )
        {
            chrpos = buf;

            while (*chrpos)
            {
                if (*chrpos == '\n')
                {
                    *chrpos = 0;
                    break;
                }
                chrpos++;
            }

            int go = 0;
            uint8_t chr;
            char *nxt;

            if ( buf[0] == ' ' )
            {
                chr = (uint8_t)' ';
                nxt = buf + 1;
                go = 1;
            }
            else
            {
                char *chrname = strtok(buf, " \t");
                if (chrname)
                {
                    if ( *chrname == '#' && chrname[1] )
                        chr = strtol(chrname + 1, NULL, 0);
                    else
                        chr = *(uint8_t *)chrname;

                    nxt = 0;
                    go = 1;
                }
            }

            if ( go )
            {
                char *str_x = strtok(nxt, " \t");
                if ( str_x )
                {
                    int xpos = strtol(str_x, NULL, 0);

                    char *str_y = strtok(0, " \t");
                    if ( str_y )
                    {
                        int ypos = strtol(str_y, NULL, 0);

                        char *str_w = strtok(0, " \t");
                        if ( str_w )
                        {
                            tileset->chars[chr].byteoff = tileset->field_4->width * ypos + xpos;
                            tileset->chars[chr].width = strtol(str_w, NULL, 0);
                        }
                    }
                }
            }
        }

        int v33 = 0;

        for (int i = 0; i < 256; i++)
        {
            if (tileset->chars[i].width)
            {
                v33 = i;
                break;
            }
        }

        for (int i = 0; i < 256; i++)
        {
            if (tileset->chars[i].width == 0)
            {
                tileset->chars[i] = tileset->chars[v33];
            }
        }
    }
    delete fil;
    return tileset;
}


tiles_stru * yw_LoadTileSet(const char *filename, int chr_width, int font_height, int tile_width, int tile_height, int columns, int rows, int x_off, int y_off)
{
    tiles_stru *tileset = (tiles_stru *)AllocVec(sizeof(tiles_stru), 65537);

    if ( tileset )
    {
        tile_xy *v13 = (tile_xy *)AllocVec(sizeof(tile_xy) * 256, 65537);
        tileset->chars = v13;
        if ( !v13 )
        {
            delete_tile_stuct(tileset);
            tileset = NULL;
        }
    }

    if ( tileset )
    {
        IDVList init_vals;
        init_vals.Add(NC_STACK_rsrc::RSRC_ATT_NAME, filename);
        init_vals.Add(NC_STACK_bitmap::BMD_ATT_TEXTURE, 1);
        init_vals.Add(NC_STACK_bitmap::BMD_ATT_TEXTURE_SYS, 1);

        tileset->font_image = Nucleus::CInit<NC_STACK_ilbm>(init_vals);
        if ( tileset->font_image )
        {
            tileset->field_4 = tileset->font_image->getBMD_pBitmap();

            tileset->field_8 = tileset->field_4->buffer;
            tileset->font_height = font_height;

            int id = 0;
            int y = y_off;
            for (int j = 0; j < rows; j++)
            {
                for (int i = 0; i < columns; i++ )
                {
                    bitmap_intern *bitm = tileset->field_4;

                    int x_pos = x_off + i * tile_width;
                    int y_pos = y;

                    if ( chr_width + (x_off + i * tile_width) > bitm->width )
                        x_pos = bitm->width - chr_width;

                    if ( font_height + y > bitm->height )
                        y_pos = bitm->height - font_height;

                    tileset->chars[id].byteoff = tileset->field_4->width * y_pos + x_pos;
                    tileset->chars[id].width = chr_width;

                    id++;
                }
                y += tile_height;
            }
        }
        else
        {
            if ( tileset->chars )
                nc_FreeMem(tileset->chars);
            nc_FreeMem(tileset);
            tileset = NULL;
        }
    }
    return tileset;
}

int load_fonts_and_icons(_NC_STACK_ypaworld *yw)
{
    const char *font_names[32] =
    {
        "default.font", //0
        "maprobo.font",
        "icondown.font",
        "defblue.font",
        "defwhite.font",
        "menublue.font", //5
        "menugray.font",
        "menuw.font",
        "gadget.font",
        "menudown.font",
        "mapbtns.font", //10
        "maphorz.font",
        "mapvert.font",
        "mapvert1.font",
        "gauge4.font",
        "tracy.font", //15
        "mapcur4.font",
        "mapcur8.font",
        "mapcur16.font",
        "mapcur32.font",
        "mapcur64.font", //20
        "icon_nb.font",
        "icon_pb.font",
        "icon_db.font",
        "icon_ns.font",
        "icon_ps.font", //25
        "type_nb.font",
        "type_pb.font",
        "type_ns.font",
        "type_ps.font",
        "energy.font", //30
        "ltracy.font"
    };

    for (int i = 0; i < 32; i++)
    {
        yw->tiles[i] = yw_LoadFont(yw, font_names[i]);
        if ( !yw->tiles[i] )
        {
            ypa_log_out("Could not load font (%s)", font_names[i]);
            return 0;
        }

        GFXEngine::GFXe.setTileset(yw->tiles[i], i);
    }


    yw->tiles[40] = yw_LoadTileSet("lego16.ilbm", 16, 16, 16, 16, 16, 16, 0, 0);
    if ( !yw->tiles[40] )
        return 0;
    GFXEngine::GFXe.setTileset(yw->tiles[40], 40);

    yw->tiles[41] = yw_LoadTileSet("lego8.ilbm", 8, 8, 8, 8, 16, 16, 0, 0);
    if ( !yw->tiles[41] )
        return 0;
    GFXEngine::GFXe.setTileset(yw->tiles[41], 41);

    yw->tiles[42] = yw_LoadTileSet("lego4.ilbm", 4, 4, 4, 4, 16, 16, 0, 0);
    if ( !yw->tiles[42] )
        return 0;
    GFXEngine::GFXe.setTileset(yw->tiles[42], 42);

    yw->tiles[43] = yw_LoadTileSet("sec4.ilbm", 4, 4, 4, 4, 16, 16, 0, 0);
    if ( !yw->tiles[43] )
        return 0;
    GFXEngine::GFXe.setTileset(yw->tiles[43], 43);

    yw->tiles[44] = yw_LoadTileSet("sec8.ilbm", 8, 8, 8, 8, 16, 16, 0, 0);
    if ( !yw->tiles[44] )
        return 0;
    GFXEngine::GFXe.setTileset(yw->tiles[44], 44);

    for (int i = 0; i < 5; i++)
    {
        int id = 50 + i;

        int v12 = 4 << i;
        int v13 = 4 << i;
        int v1, v2, a10;

        switch ( i )
        {
        case 0:
            v1 = 4;
            v2 = 504;
            a10 = 64;
            break;
        case 1:
            v1 = 8;
            v2 = 432;
            a10 = 64;
            break;
        case 2:
            v1 = 16;
            v2 = 288;
            a10 = 64;
            break;
        case 3:
            v1 = 32;
            v2 = 0;
            a10 = 64;
            break;
        case 4:
            v1 = 64;
            v2 = 0;
            a10 = 0;
            break;

        default:
            break;
        }

        tiles_stru *v14 = yw_LoadTileSet("mapmisc.ilbm",v12, v13, v1, 0, 9, 1, v2, a10);

        if ( !v14 )
            return 0;

        v14->chars[9].byteoff = v14->chars[8].byteoff;
        v14->chars[9].width = v14->chars[8].width;
        v14->chars[8].byteoff = v14->chars[0].byteoff;
        v14->chars[8].width = v14->chars[0].width;

        yw->tiles[id] = v14;
        GFXEngine::GFXe.setTileset(v14, id);


    }

    yw->tiles[59] = yw_LoadFont(yw, "mapvhcl3.font");
    if ( !yw->tiles[59] )
        return 0;
    GFXEngine::GFXe.setTileset(yw->tiles[59], 59);

    yw->tiles[60] = yw_LoadFont(yw, "mapvhcl5.font");
    if ( !yw->tiles[60] )
        return 0;
    GFXEngine::GFXe.setTileset(yw->tiles[60], 60);

    yw->tiles[61] = yw_LoadFont(yw, "mapvhcl7.font");
    if ( !yw->tiles[61] )
        return 0;
    GFXEngine::GFXe.setTileset(yw->tiles[61], 61);

    yw->tiles[62] = yw_LoadFont(yw, "mapvhcl9.font");
    if ( !yw->tiles[62] )
        return 0;
    GFXEngine::GFXe.setTileset(yw->tiles[62], 62);

    yw->font_default_h = yw->tiles[0]->font_height;
    yw->font_default_w__a = yw->tiles[0]->chars[97].width; // a
    yw->font_default_w__b = yw->tiles[0]->chars[98].width; // b

    yw->font_yscrl_bkg_w = yw->tiles[12]->chars[66].width; // B (Y-Scroller, Background-Inners)
    yw->font_xscrl_h = yw->tiles[11]->font_height;

    yw->field_1a38 = 2;

    yw->font_yscrl_h = yw->tiles[12]->font_height;

    yw->icon_order__w = yw->tiles[21]->chars[65].width; // A (Mode:      ORDER)
    yw->icon_order__h = yw->tiles[21]->font_height;

    yw->icon_help__w = yw->tiles[24]->chars[65].width; // A (NEU: DER HILFEBUTTTON)
    yw->icon_help__h = yw->tiles[24]->font_height;

    yw->icon_energy__h = yw->tiles[30]->font_height; // ENERGIE height
    yw->icon0___h = yw->icon_order__h;

    return 1;
}



void yw_InitLevelNet__sub0(_NC_STACK_ypaworld *yw)
{
    for(int i = 0; i < 70; i++)
    {
        yw->iniColors[i].r = 255;
        yw->iniColors[i].g = 255;
        yw->iniColors[i].b = 0;
        yw->iniColors[i].i = 10;
        yw->iniColors[i].color = 0xFFFF00;
    }
}

int yw_ParseWorldIni(_NC_STACK_ypaworld *yw, const char *filename)
{
    scrCallBack parsers[5];

    memset(parsers, 0, sizeof(parsers));

    parsers[0].world = yw;
    parsers[0].func = BkgParser;
    parsers[1].world = yw;
    parsers[1].func = Colors_Parser;
    parsers[2].world = yw;
    parsers[2].func = MoviesParser;
    parsers[3].world = yw;
    parsers[3].func = SuperItem_Parse;
    parsers[4].world = yw;
    parsers[4].func = Misc_Parse;

    return def_parseFile(filename, 5, parsers, 2);
}

int yw_InitLevelNet(_NC_STACK_ypaworld *yw)
{
    yw->field_2d90 = (stru_2d90 *)AllocVec(sizeof(stru_2d90), 65537);
    if ( !yw->field_2d90 )
    {
        if ( yw->LevelNet )
            nc_FreeMem(yw->LevelNet);

        if ( yw->field_2d90 )
            nc_FreeMem(yw->field_2d90);

        yw->field_2d90 = NULL;
        yw->LevelNet = NULL;
        return 0;
    }

    yw->LevelNet = (stru_LevelNet *)AllocVec(sizeof(stru_LevelNet), 65537);
    if ( !yw->LevelNet )
    {
        if ( yw->LevelNet )
            nc_FreeMem(yw->LevelNet);

        if ( yw->field_2d90 )
            nc_FreeMem(yw->field_2d90);

        yw->field_2d90 = NULL;
        yw->LevelNet = NULL;
        return 0;
    }

    yw_InitLevelNet__sub0(yw);
    if ( !yw_ParseWorldIni(yw, "env:world.ini") && !yw_ParseWorldIni(yw, "data:world.ini"))
    {
        ypa_log_out("yw_ParseWorldIni() failed.\n");
        if ( yw->LevelNet )
            nc_FreeMem(yw->LevelNet);

        if ( yw->field_2d90 )
            nc_FreeMem(yw->field_2d90);

        yw->field_2d90 = NULL;
        yw->LevelNet = NULL;
        return 0;
    }

    if ( !yw_ScanLevels(yw) )
    {
        ypa_log_out("yw_ScanLevels() failed.\n");
        if ( yw->LevelNet )
            nc_FreeMem(yw->LevelNet);

        if ( yw->field_2d90 )
            nc_FreeMem(yw->field_2d90);

        yw->field_2d90 = NULL;
        yw->LevelNet = NULL;
        return 0;
    }
    yw->field_2d90->field_74 = 128;
    memset(yw->field_2d90->jodiefoster, 0, 32);
    return 1;
}

int yw_InitNetwork(_NC_STACK_ypaworld *yw)
{

    NC_STACK_windp *windp = Nucleus::CInit<NC_STACK_windp>();

    yw->windp = windp;

    if ( !windp )
    {
        ypa_log_out("Unable to create network-Object\n");
        return 0;
    }

    yw->netInterpolate = 1;

    memset(&yw->field_759E, 0, 4);

    yw->field_75A2 = 0;
    yw->field_75E2[0] = 0;

    yw->windp->SetVersion(yw->buildDate);

    return 1;
}



void yw_setIniColor(_NC_STACK_ypaworld *yw, int color_id, int r, int g, int b, int i)
{
    yw->iniColors[color_id].r = r;
    yw->iniColors[color_id].g = g;
    yw->iniColors[color_id].b = b;
    yw->iniColors[color_id].i = i;
    yw->iniColors[color_id].color = ((r << 16) & 0xFF0000) | ((g << 8) & 0xFF00) | (b & 0xFF) ;
}

int yw_parse_color(_NC_STACK_ypaworld *yw, int color_number, char *color_string)
{
    char *pr = strtok(color_string, "_");
    char *pg = strtok(0, "_");
    char *pb = strtok(0, "_");
    char *pi = strtok(0, "_");

    if ( pr && pg && pb && pi )
    {
        int r = atoi(pr);
        int g = atoi(pg);
        int b = atoi(pb);
        int i = atoi(pi);
        yw_setIniColor(yw, color_number, r, g, b, i);
        return 1;
    }

    return 0;
}

int get_level_numb(const char *filename)
{
    char buf[260];
    strcpy(buf, filename);
    char *tmp = strpbrk(buf, ".");
    if (tmp)
        *tmp = 0;

    int v9 = strlen(buf);

    if ( v9 == 5 )
    {
        buf[3] = 0;
    }
    else if ( v9 == 7 )
    {
        buf[4] = 0;
    }

    int v10 = atoi(&buf[1]);
    if ( v10 < 1 || v10 >= 256 )
    {
        v10 = -1;
        ypa_log_out("Invalid level num [valid: 0..127] for %s.\n", filename);
    }
    return v10;
}





int sb_0x4e1a88__sub0__sub0(_NC_STACK_ypaworld *yw, mapProto *mapp, const char *fname)
{
    scrCallBack v7[3];

    memset(v7, 0, sizeof(v7));
    memset(mapp, 0, sizeof(mapProto));

    v7[0].dataForStore = mapp;
    v7[0].world = yw;
    v7[0].func = LevelDataParser;
    v7[1].dataForStore = mapp;
    v7[1].func = parse_map_robos;
    v7[2].world = yw;
    v7[2].dataForStore = mapp;
    v7[2].func = MapParseSizes;

    return def_parseFile(fname, 3, v7, 6);
}



int sb_0x4e1a88__sub0(_NC_STACK_ypaworld *yw, int multiplayer, const char *fname)
{
    mapProto mapp;
    char buf[256];

    int levelnmb = get_level_numb(fname);

    if ( levelnmb != -1 )
    {
        mapINFO *minf = &yw->LevelNet->mapInfos[levelnmb];

        if ( multiplayer )
        {
            strcpy(buf, "levels:multi/");
        }
        else
        {
            strcpy(buf, "levels:single/");
        }

        strcat(buf, fname);

        sb_0x4e1a88__sub0__sub0(yw, &mapp, buf);

        if ( multiplayer )
            minf->field_0 = 4;
        else
            minf->field_0 = 2;

        minf->field_10 = 0;
        minf->field_C = minf->field_10;
        minf->field_8 = minf->field_10;
        minf->field_4 = minf->field_10;

        memset(minf->field_14, 0, sizeof(minf->field_14));

        strcpy(minf->mapPath, buf);

        strcpy(minf->map_name, yw->field_2d90->map_name);

        memset(&minf->field_9C, 0, sizeof(ua_fRect));

        minf->fractions_mask = 0;
        minf->robos_count = mapp.mapRobos_count;

        for (int i = 0; i < mapp.mapRobos_count; i++)
            minf->fractions_mask |= 1 << mapp.mapRobos[i].owner;

        minf->secXsize = mapp.secXsize & 0xFF;
        minf->secYsize = mapp.secYsize & 0xFF;
        minf->slow_connection = mapp.slow_connection;
        return 1;
    }
    return 0;
}

int sb_0x4e1a88(_NC_STACK_ypaworld *yw, int multiplayer)
{
    int v10 = 1;

    const char *v4;

    if ( multiplayer )
        v4 = "levels:multi/";
    else
        v4 = "levels:single/";

    FSMgr::DirIter dir = uaOpenDir(v4);
    if ( dir )
    {
        FSMgr::iNode *fsnode;
        while ( dir.getNext(&fsnode) )
        {
            if ( strcmp(fsnode->getName(), ".") && strcmp(fsnode->getName(), "..") )
            {
                if ( sb_0x4e1a88__sub0(yw, multiplayer, fsnode->getName()) )
                {
                    ypa_log_out("Scanning [%s%s] .. ok.\n", v4, fsnode->getName());
                }
                else
                {
                    ypa_log_out("Scanning [%s%s] .. FAILED.\n", v4, fsnode->getName());
                    v10 = 0;
                }
            }
        }
    }

    return v10;
}

int yw_ScanLevels(_NC_STACK_ypaworld *yw)
{
    int v2 = 1;
    if ( !sb_0x4e1a88(yw, 0) )
        v2 = 0;
    if ( !sb_0x4e1a88(yw, 1) )
        v2 = 0;
    return v2;
}

int yw_InitTooltips(_NC_STACK_ypaworld *yw)
{
    const char **tips = (const char **)AllocVec(sizeof(char *) * 199, 65537);
    yw->tooltips = tips;
    if ( tips )
    {
        tips[0] = NULL;
        tips[1] = "GIVE ORDERS TO SELECTED SQUADRON";
        tips[2] = "SWITCH INTO DEFENSE GUNS";
        tips[3] = "CREATE A NEW SQUADRON LEADER";
        tips[4] = "ADD A DRONE TO SELECTED SQUADRON";
        tips[5] = "TAKE OVER CONTROL OF A DRONE";
        tips[6] = "CREATE A NEW BUILDING";
        tips[7] = "BEAM TO SOME OTHER PLACE";
        tips[8] = "OPEN/CLOSE THE MAP WINDOW";
        tips[9] = "OPEN/CLOSE THE FINDER WINDOW";
        tips[10] = "OPEN/CLOSE THE MESSAGE LOG WINDOW";
        tips[11] = "OPEN/CLOSE THE ENERGY WINDOW";
        tips[12] = "AGGR SETTING: ESCAPE NOW";
        tips[13] = "AGGR SETTING: ATTACK PRIMARY TARGET ONLY";
        tips[14] = "AGGR SETTING: SEARCH AND DESTROY NEARBY ENEMY DRONES";
        tips[15] = "AGGR SETTING: ATTACK ALL ENEMY TARGETS ON YOUR WAY";
        tips[16] = "AGGR SETTING: GO AMOK";
        tips[17] = "SWITCH BACK INTO HOST STATION";
        tips[18] = "SWITCH INTO COMMANDER OF SQUAD";
        tips[19] = "SWITCH INTO NEXT UNIT OF SQUAD";
        tips[20] = "SWITCH INTO NEXT SQUADRON COMMANDER";
        tips[21] = "PAUSE OR CANCEL CURRENT MISSION";
        tips[22] = "SHOW/HIDE SECTOR DETAIL LAYER";
        tips[23] = "SHOW/HIDE SECTOR OWNER LAYER";
        tips[24] = "SHOW/HIDE EXTENDED INFO LAYER";
        tips[25] = "FREE MAP SCROLLING";
        tips[26] = "MAP SCROLLING LOCKED ON CURRENT VIEWER";
        tips[27] = "MAP SCROLLING LOCKED ON SELECTED SQUADRON";
        tips[28] = "ZOOM OUT";
        tips[29] = "ZOOM IN";
        tips[30] = "TOGGLE MAP SIZE";
        tips[31] = "SQUADRON ACTION: AWAITING ORDER";
        tips[32] = "SQUADRON ACTION: FIGHT ENEMY";
        tips[33] = "SQUADRON ACTION: ON THE WAY";
        tips[34] = "SQUADRON ACTION: ESCAPING";
        tips[35] = "ENERGY RELOAD LEVEL";
        tips[36] = "MAIN SYSTEM BATTERY LEVEL";
        tips[37] = "DRONE CREATION BATTERY LEVEL";
        tips[38] = "BUILDING CREATION BATTERY LEVEL";
        tips[39] = "BEAM TRANSPORTER BATTERY LEVEL";
        tips[40] = "SELECT ANOTHER ACTION MODE";
        tips[41] = "SELECT A DEFENSE GUN";
        tips[42] = "SELECT A DRONE TYPE";
        tips[43] = "SELECT A BUILDING TYPE";
        tips[44] = "SHOW/HIDE HEAD UP DISPLAY";
        tips[45] = "CONTROL TO LAST MESSAGE SENDER";
        tips[46] = "NUMBER OF VEHICLES IN SELECTED SQUAD";
        tips[47] = "SITUATION ANALYZER";
        tips[48] = "ONLINE HELP";
        tips[64] = "MAKE SQUADRON THE SELECTED ONE";
        tips[65] = "ADVANCE TO THIS LOCATION";
        tips[66] = "ATTACK THIS SECTOR";
        tips[67] = "ATTACK THIS DRONE";
        tips[68] = "CREATE A NEW SQUADRON LEADER";
        tips[69] = "ADD A NEW DRONE TO SELECTED SQUAD";
        tips[70] = "TAKE OVER HAND CONTROL OF THIS DRONE";
        tips[71] = "CREATE A NEW BUILDING HERE";
        tips[72] = "BEAM HOST STATION TO THIS LOCATION";
        tips[73] = "EXIT DEBRIEFING";
        tips[74] = "GOTO LOAD/SAVE SCREEN";
        tips[75] = "SELECT 3D DEVICE";
        tips[76] = "SEND THIS MESSAGE";
        tips[77] = "USE ALTERNATE JOYSTICK MODEL";
        tips[78] = "USE 16 BIT TEXTURES";
        tips[79] = "USE DRAW PRIMITIVE D3D MODEL";
        tips[80] = "880 == MOUSE CONTROL MODE ACTIVE (PRESS RMB TO DEACTIVATE)";
        tips[81] = "881 == UNIT LIMIT REACHED";
        tips[96] = "ERROR: NO VALID TARGET GIVEN!";
        tips[97] = "ERROR: NOT ENOUGH ROOM HERE!";
        tips[98] = "ERROR: CURRENTLY NOT ENOUGH ENERGY!";
        tips[99] = "ERROR: SECTOR MUST BE CONQUERED!";
        tips[100] = "ERROR: LOCATION TOO FAR AWAY!";
        tips[101] = "ERROR: LOCATION TOO CLOSE TO HOST STATION!";
        tips[102] = "ERROR: BUILDING CREATION SYSTEM BUSY!";
        tips[103] = "ERROR: MUST BE NO DRONES IN TARGET SECTOR!";
        tips[104] = "ERROR: TARGET LOCATION TOO JAGGY!";
        tips[105] = "ERROR: LOCATION NOT IN SENSOR AREA!";
        tips[106] = "GO TO LEVEL SELECTION MAP";
        tips[107] = "GO TO TRAINING AREA";
        tips[108] = "USE SELECTED LANGUAGE";
        tips[109] = "CLOSE LOCALE REQUESTER";
        tips[110] = "CHOOSE A NEW LEVEL";
        tips[111] = "SEND MESSAGE MTO ALL PLAYERS";
        tips[112] = "SELECT RESISTANCE AS RACE";
        tips[113] = "SELECT GHORKOV AS RACE";
        tips[114] = "SELECT MYKONIANS AS RACE";
        tips[115] = "SELECT TAERKASTEN AS RACE";
        tips[116] = "MARK GAME AS STARTABLE";
        tips[117] = "MARK GAME AS NOT STARTBALE";
        tips[118] = "LOAD SELECTED PLAYER";
        tips[119] = "SAVE ACTUAL PLAYER";
        tips[120] = "OPEN/CLOSE INPUTREQUESTER";
        tips[121] = "OPEN/CLOSE VIDEOREQUESTER";
        tips[122] = "OPEN/CLOSE SOUNDREQUESTER";
        tips[123] = "OPEN/CLOSE IN-OUT-REQUESTER";
        tips[124] = "OPEN/CLOSE LANGUAGEREQUESTER";
        tips[125] = "PAUSE/CONTINUE MISSION BRIEFING";
        tips[126] = "STOP MISSION BRIEFING";
        tips[127] = "LOAD LAST SAVEGAME";
        tips[128] = "WIND FORWARD MISSION BRIEFING";
        tips[129] = "RESET MISSION BRIEFING";
        tips[130] = "PLAY THE LEVEL";
        tips[131] = "UHHHMMM... QUIT? YEAH RIGHT, DUDE, KILL IT!";
        tips[132] = "PRESS FOR ADDITIONAL SELECTION";
        tips[133] = "ENTER NEW KEY IF BUTTON IS PRESSED";
        tips[134] = "YOU HEAR THE SOUND AT LEFT";
        tips[135] = "YOU HEAR THE SOUND AT RIGHT";
        tips[136] = "OPEN/CLOSE NETWORKREQUESTER";
        tips[137] = "GIVE ME MORE INFORMATION";
        tips[138] = "GO BACK TO START PAGE";
        tips[139] = "GO BACK TO LEVELSELECTION";
        tips[140] = "CLOSE NETWORK REQUESTER";
        tips[141] = "???";
        tips[142] = "APPLY THIS PROVIDER";
        tips[143] = "APPLY THIS PLAYERNAME";
        tips[144] = "CREATE GAME WITH THIS LEVEL";
        tips[145] = "JOIN THIS SESSION";
        tips[146] = "START GAME";
        tips[147] = "GO BACK TO PROVIDER SELECTION";
        tips[148] = "GO BACK TO ENTER A NEW NAME";
        tips[149] = "GO BACK TO SESSION SELECTION";
        tips[151] = "EXIT DEBRIEFING";
        tips[152] = "REWIND DEBRIEFING";
        tips[153] = "PLAYING TIME - THIS LEVEL";
        tips[154] = "PLAYING TIME - OVERALL";
        tips[155] = "KILLS - THIS LEVEL";
        tips[156] = "KILLS - OVERALL";
        tips[157] = "LOSSES - THIS LEVEL";
        tips[158] = "LOSSES - OVERALL";
        tips[159] = "SECTORS CONQUERED - THIS LEVEL";
        tips[160] = "SECTORS CONQUERED - OVERALL";
        tips[161] = "SCORE - THIS LEVEL";
        tips[162] = "SCORE - OVERALL";
        tips[163] = "POWERSTATIONS CAPTURED - THIS LEVEL";
        tips[164] = "POWERSTATIONS CAPTURED - OVERALL";
        tips[165] = "TECH UPGRADES CAPTURED - THIS LEVEL";
        tips[166] = "TECH UPGRADES CAPTURED - OVERALL";
        tips[167] = "SHOW STATISTICS FOR THIS RACE";
        tips[168] = "DELETE SELECTED PLAYER";
        tips[169] = "CREATE A NEW PLAYER";
        tips[170] = "LOAD PLAYER WITH THIS NAME";
        tips[171] = "SAVE ACTUAL PLAYER UNDER THIS NAME";
        tips[172] = "DELETE PLAYER WITH THIS NAME";
        tips[173] = "CREATE PLAYER WITH THIS NAME";
        tips[174] = "CLOSE PLAYER REQUESTER";
        tips[175] = "STOP LOADING PLAYER";
        tips[176] = "STOP SAVING PLAYER";
        tips[177] = "STOP DELETING PLAYER";
        tips[178] = "STOP CREATING PLAYER";
        tips[179] = "CLOSE SETTINGS REQUESTER";
        tips[180] = "APPLY NEW SETTINGS";
        tips[181] = "PRESS TO CHANGE SCREEN RESOLUTION";
        tips[182] = "SWITCH FX-SOUND";
        tips[183] = "TOGGLE CHANNEL ASSIGNMENT";
        tips[184] = "CHANGE HORIZON DEPTH";
        tips[185] = "SWITCH ON/OFF SKY";
        tips[186] = "SWITCH ON/OFF FILTERING";
        tips[187] = "ENABLE/DISABLE CD SOUND";
        tips[188] = "USE SOFTWARE OR HARDWARE MOUSEPOINTER";
        tips[189] = "ALLOW/FORBID ENEMYINDICATORS";
        tips[190] = "CHANGE FX VOLUME";
        tips[191] = "CHANGE CD VOLUME";
        tips[192] = "CHANGE NUMBER OF EXPLODE EFFECTS";
        tips[193] = "APPLY INPUT CHANGES";
        tips[194] = "IGNORE INPUT CHANGES";
        tips[195] = "RESET TO DEFAULT KEYS";
        tips[196] = "USE JOYSTICK";
        tips[197] = "USE FORCEFEEDBACK FOR JOSTICK";
        tips[198] = "REMOVE KEY FROM ACTION";
    }
    else
        return 0;
    return 1;
}

NC_STACK_base * sub_44AD8C(const char *fname)
{
    NC_STACK_base *obj = Nucleus::CInit<NC_STACK_base>();
    if ( obj )
    {
        FSMgr::FileHandle *fil = uaOpenFile(fname, "r");
        if ( !fil )
        {
            delete_class_obj(obj);
            return NULL;
        }

        char buf[512];
        char basfl[300];

        while ( fil->gets(buf, 512) )
        {
            char *v4 = strtok(buf, " #;\t\n");

            if ( !v4 || *v4 == '>' )
                break;

            strcpy(basfl, "rsrc:objects/");
            strcat(basfl, v4);

            NC_STACK_base *kid = NC_STACK_base::READ_BAS_FILE(basfl);

            if ( !kid )
            {
                ypa_log_out("init: Could not load %s.\n", basfl);
                delete fil;
                delete_class_obj(obj);
                return NULL;
            }

            obj->base_func65(kid); //Add to kid list
        }
        delete fil;
    }
    return obj;
}

NC_STACK_base *load_set_base()
{
    NC_STACK_base *base = NC_STACK_base::READ_BAS_FILE("rsrc:objects/set.base");
    if ( !base )
    {
        ypa_log_out("init: no set.base, trying fragment load.\n");

        base = Nucleus::CInit<NC_STACK_base>();
        if ( base )
        {
            NC_STACK_base *visproto = NC_STACK_base::READ_BAS_FILE("rsrc:objects/visproto.base");
            if ( !visproto )
            {
                ypa_log_out("init: no visproto.base, trying single load.\n");
                visproto = sub_44AD8C("rsrc:scripts/visproto.lst");
            }
            if ( !visproto )
            {
                delete_class_obj(base);
                return NULL;
            }
            base->base_func65(visproto);

            NC_STACK_base *lego = NC_STACK_base::READ_BAS_FILE("rsrc:objects/lego.base");
            if ( !lego )
            {
                ypa_log_out("init: no lego.base, trying single load.\n");
                lego = sub_44AD8C("rsrc:scripts/set.sdf");
            }
            if ( !lego )
            {
                delete_class_obj(base);
                return NULL;
            }
            base->base_func65(lego);

            NC_STACK_base *slurp = NC_STACK_base::READ_BAS_FILE("rsrc:objects/slurp.base");
            if ( !slurp )
            {
                ypa_log_out("init: no slurp.base, trying single load.\n");
                slurp = sub_44AD8C("rsrc:scripts/slurps.lst");
            }
            if ( !slurp )
            {
                delete_class_obj(base);
                return NULL;
            }
            base->base_func65(slurp);
        }
    }
    return base;
}

int sub_44A12C(_NC_STACK_ypaworld *yw, NC_STACK_base *base)
{
    int id = 0;
    BaseList &kid_list = base->getBASE_kidList();

    for(BaseList::iterator it = kid_list.begin(); it != kid_list.end(); it++)
    {
        yw->vhcls_models[id].base = *it;
        (*it)->setBASE_visLimit(yw->field_15e4);
        (*it)->setBASE_fadeLength(yw->field_15e8);
        yw->vhcls_models[id].trigo = (*it)->getBASE_pTransform();
        id++;
    }

    return 1;
}

int yw_parse_lego(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil, NC_STACK_base *base)
{
    int id = 0;
    BaseList &kid_list = base->getBASE_kidList();

    for(BaseList::iterator it = kid_list.begin(); it != kid_list.end(); it++)
    {
        yw->legos[id].base = *it;
        (*it)->setBASE_visLimit(yw->field_15e4);
        (*it)->setBASE_fadeLength(yw->field_15e8);
        (*it)->setBASE_static(1);
        id++;
    }

    id = 0;
    char line_buf[1024];

    while ( fil->gets(line_buf, 1024) && line_buf[0] != '>' )
    {
        cityBases *lego = &yw->legos[id];

        if ( id >= 256 )
        {
            ypa_log_out("Too many legos!\n");
            return 0;
        }

        char *v10 = strpbrk(line_buf, ";#\n\r");
        if ( v10 )
            *v10 = 0;

        if ( strtok(line_buf, " \t") ) // ignore base file name
        {
            char *v11 = strtok(0, " \t");
            if ( !v11 )
                return 0;

            IDVList init_vals;
            init_vals.Add(NC_STACK_rsrc::RSRC_ATT_NAME, v11);

            NC_STACK_sklt *skelet = Nucleus::CInit<NC_STACK_sklt>(init_vals);
            if ( !skelet )
            {
                ypa_log_out("Couldn't load sklt (%s)\n", v11);
                return 0;
            }

            lego->sklt_obj = skelet;
            lego->sklt_obj_intern = skelet->getSKEL_pSkelet();

            v11 = strtok(0, " \t");
            if ( !v11 )
                return 0;

            lego->field_11 = strtol(v11, NULL, 0);

            v11 = strtok(0, " \t");
            if ( !v11 )
                return 0;

            lego->field_12 = strtol(v11, NULL, 0);

            v11 = strtok(0, " \t");
            if ( !v11 )
                return 0;

            lego->field_10 = strtol(v11, NULL, 0);

            v11 = strtok(0, " \t");
            if ( !v11 )
                return 0;

            switch( strtol(v11, NULL, 0) )
            {
            default:
            case 0:
                lego->selected_sklt_intern = lego->sklt_obj_intern;
                break;

            case 1:
                lego->selected_sklt_intern = yw->colsub_sklt_intrn;
                break;

            case 2:
                lego->selected_sklt_intern = yw->colcomp_sklt_intrn;
                break;
            }

            int fxnumb = 0;
            while ( 1 )
            {
                v11 = strtok(0, " \t");
                if ( !v11 || fxnumb >= yw->fxnumber )
                    break;

                float x = 0.0;
                float y = 0.0;
                float z = 0.0;

                lego->field_14[fxnumb] = strtol(v11, NULL, 0);

                v11 = strtok(0, " \t");
                if ( v11 )
                    x = strtof(v11, 0);

                v11 = strtok(0, " \t");
                if ( v11 )
                    z = strtof(v11, 0);

                v11 = strtok(0, " \t");
                if ( v11 )
                    y = strtof(v11, 0);

                lego->field_24[fxnumb].pos_x = x;
                lego->field_24[fxnumb].pos_y = -y;
                lego->field_24[fxnumb].pos_z = -z;

                fxnumb++;
            }

            id++;
        }
    }
    return 1;
}

int yw_parse_subSect(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    char buf[256];

    int id = 0;
    while ( fil->gets(buf, 255) && buf[0] != '>' )
    {
        if ( id >= 256 )
        {
            ypa_log_out("Too many sub sector defs!\n");
            return 0;
        }

        char *endln = strpbrk(buf, ";#\n\r");
        if ( endln )
            *endln = 0;

        char *pp = strtok(buf, " \t");
        if ( pp )
        {
            subSec *ssec = &yw->subSectors[id];

            ssec->health_models[0] = strtol(pp, NULL, 0);

            pp = strtok(0, " \t");
            if ( !pp )
                return 0;

            ssec->health_models[1] = strtol(pp, NULL, 0);

            pp = strtok(0, " \t");
            if ( !pp )
                return 0;

            ssec->health_models[2] = strtol(pp, NULL, 0);

            pp = strtok(0, " \t");
            if ( !pp )
                return 0;

            ssec->health_models[3] = strtol(pp, NULL, 0);

            pp = strtok(0, " \t");
            if ( !pp )
                return 0;

            if ( *pp > '0' && *pp == '1' )
                ssec->build_health = 255;
            else
                ssec->build_health = 0;

            pp = strtok(0, " \t");
            if ( pp )
                ssec->field_8 = strtol(pp, NULL, 0);

            id++;
        }
    }
    return 1;
}

int yw_parse_sektor(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    char line_buf[512];
    char buf[512];

    while ( fil->gets(buf, 512) && buf[0] != '>' )
    {
        strcpy(line_buf, buf);

        char *lnend = strpbrk(buf, ";#\n\r");
        if ( lnend )
            *lnend = 0;

        char *pp = strtok(buf, " \t");
        if ( pp )
        {
            secType *sektp = &yw->secTypes[ strtol(pp, NULL, 0) ];

            pp = strtok(0, " \t");
            if ( !pp )
            {
                ypa_log_out("Error reading '%s', line '%s'.\n", "set.sdf", line_buf);
                return 0;
            }

            sektp->field_0 = strtol(pp, NULL, 0);

            pp = strtok(0, " \t");
            if ( !pp )
            {
                ypa_log_out("Error reading '%s', line '%s'.\n", "set.sdf", line_buf);
                return 0;
            }

            sektp->field_1 = strtol(pp, NULL, 0);

            pp = strtok(0, " \t");
            if ( !pp )
            {
                ypa_log_out("Error reading '%s', line '%s'.\n", "set.sdf", line_buf);
                return 0;
            }

            sektp->field_3 = strtol(pp, NULL, 0);

            memset(sektp->buildings, 0, sizeof(sektp->buildings));

            if ( sektp->field_0 == 1 )
            {
                pp = strtok(0, " \t");
                if ( !pp )
                {
                    ypa_log_out("Error reading '%s', line '%s'.\n", "set.sdf", line_buf);
                    return 0;
                }
                sektp->buildings[0][0] = &yw->subSectors[ strtol(pp, NULL, 0) ];
            }
            else
            {
                for (int j = 0; j < 3; j++)
                {
                    for (int i = 0; i < 3; i++)
                    {
                        pp = strtok(0, " \t");
                        if ( !pp )
                        {
                            ypa_log_out("Error reading '%s', line '%s'.\n", "set.sdf", line_buf);
                            return 0;
                        }

                        sektp->buildings[i][2 - j] = &yw->subSectors[ strtol(pp, NULL, 0) ];
                    }
                }
            }
        }
    }
    return 1;
}

int sub_44A97C(_NC_STACK_ypaworld *yw, NC_STACK_base *base)
{
    BaseList &kid_list = base->getBASE_kidList();
    BaseList::iterator it = kid_list.begin();

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            for (int k = 0; k < 6; k++)
            {
                if ( it == kid_list.end() )
                {
                    ypa_log_out("Too few slurps in slurp child.\n");
                    return 0;
                }
                (*it)->setBASE_visLimit(yw->field_15e4);
                (*it)->setBASE_fadeLength(yw->field_15e8);
                (*it)->setBASE_static(1);

                NC_STACK_skeleton *skeleton = (*it)->getBASE_skeleton();

                UAskeleton::Data *skeleton_internal = skeleton->getSKEL_pSkelet();
                if (i == 0)
                {
                    yw->slurps1[j][k].skeletons_bas = *it;
                    yw->slurps1[j][k].skeleton_internal = skeleton_internal;
                }
                else if (i == 1)
                {
                    yw->slurps2[j][k].skeletons_bas = *it;
                    yw->slurps2[j][k].skeleton_internal = skeleton_internal;
                }

                it++;
            }
        }
    }

    char rsr[256];
    strcpy(rsr, get_prefix_replacement("rsrc"));

    set_prefix_replacement("rsrc", "data:mc2res");

    IDVList init_vals;
    init_vals.Add(NC_STACK_rsrc::RSRC_ATT_NAME, "Skeleton/ColSide.sklt");

    NC_STACK_sklt *tmp_skel = Nucleus::CInit<NC_STACK_sklt>(init_vals);
    if ( !tmp_skel )
    {
        ypa_log_out("Couldn't create side collision sklt.\n");
        return 0;
    }

    yw->ColSide.skeleton = tmp_skel;
    yw->ColSide.skeleton_internal = yw->ColSide.skeleton->getSKEL_pSkelet();

    init_vals.clear();
    init_vals.Add(NC_STACK_rsrc::RSRC_ATT_NAME, "Skeleton/ColCross.sklt");

    tmp_skel = Nucleus::CInit<NC_STACK_sklt>(init_vals);
    if ( !tmp_skel )
    {
        ypa_log_out("Couldn't create cross collision sklt.\n");
        return 0;
    }

    yw->ColCross.skeleton = tmp_skel;
    yw->ColCross.skeleton_internal = yw->ColCross.skeleton->getSKEL_pSkelet();

    set_prefix_replacement("rsrc", rsr);

    return 1;
}

int yw_InitMouseStuff(_NC_STACK_ypaworld *yw)
{
    char rsr[256];

    strcpy(rsr, get_prefix_replacement("rsrc"));
    set_prefix_replacement("rsrc", "data:mc2res");

    for (int i = 0; i < 11; i++)
    {
        const char *v5;
        switch ( i )
        {
        case 1:
            v5 = (const char *)"pointers/cancel.ilbm";
            break;
        case 2:
            v5 = "pointers/select.ilbm";
            break;
        case 3:
            v5 = "pointers/attack.ilbm";
            break;
        case 4:
            v5 = "pointers/goto.ilbm";
            break;
        case 5:
            v5 = "pointers/disk.ilbm";
            break;
        case 6:
            v5 = "pointers/new.ilbm";
            break;
        case 7:
            v5 = "pointers/add.ilbm";
            break;
        case 8:
            v5 = "pointers/control.ilbm";
            break;
        case 9:
            v5 = "pointers/beam.ilbm";
            break;
        case 10:
            v5 = "pointers/build.ilbm";
            break;
        default:
            v5 = "pointers/pointer.ilbm";
            break;
        }

        IDVList init_vals;
        init_vals.Add(NC_STACK_rsrc::RSRC_ATT_NAME, v5);
        init_vals.Add(NC_STACK_bitmap::BMD_ATT_TEXTURE, 1);
        init_vals.Add(NC_STACK_bitmap::BMD_ATT_TEXTURE_SYS, 1);

        yw->pointers[i] = Nucleus::CInit<NC_STACK_ilbm>(init_vals);
        if ( !yw->pointers[i] )
        {
            ypa_log_out("yw_select.c/yw_InitMouseStuff()\n");
            set_prefix_replacement("rsrc", rsr);
            return 0;
        }

        yw->pointers__bitm[i] = yw->pointers[i]->getBMD_pBitmap();
    }

    displ_arg263 arg_263;

    arg_263.bitm = 0;
    arg_263.pointer_id = 0;
    yw->win3d->display_func263(&arg_263);

    arg_263.bitm = yw->pointers__bitm[0];
    arg_263.pointer_id = 1;
    yw->win3d->display_func263(&arg_263);

    set_prefix_replacement("rsrc", rsr);
    return 1;
}

int yw_LoadSet(_NC_STACK_ypaworld *yw, int setID)
{
    yw->win3d = GFXEngine::GFXe.getC3D();
    yw->field_17c0 = 0;

    char buf[1024];
    sprintf(buf, "data:set%d", setID);

    char rsr[256];
    strcpy(rsr,  get_prefix_replacement("rsrc"));

    yw->win3d->display_func271(NULL);


    set_prefix_replacement("rsrc", "data:mc2res");

    IDVList init_vals;
    init_vals.Add(NC_STACK_rsrc::RSRC_ATT_NAME, "skeleton/colsub.sklt");

    yw->colsub_sklt = Nucleus::CInit<NC_STACK_sklt>(init_vals);
    if ( !yw->colsub_sklt )
    {
        ypa_log_out("Couldn't load <skeleton/colsub.sklt>, set %d.\n", setID);
        return 0;
    }
    yw->colsub_sklt_intrn = yw->colsub_sklt->getSKEL_pSkelet();


    init_vals.clear();
    init_vals.Add(NC_STACK_rsrc::RSRC_ATT_NAME, "skeleton/colcomp.sklt");

    yw->colcomp_sklt = Nucleus::CInit<NC_STACK_sklt>(init_vals);
    if ( !yw->colcomp_sklt )
    {
        ypa_log_out("Couldn't load <skeleton/colcomp.sklt>, set %d.\n", setID);
        return 0;
    }
    yw->colcomp_sklt_intrn = yw->colcomp_sklt->getSKEL_pSkelet();


    set_prefix_replacement("rsrc", buf);

    if ( !GFXEngine::GFXe.loadPal("palette/standard.pal") )
        ypa_log_out("WARNING: Could not load set default palette!\n");

    if ( setID != yw->set_number && setID != 46 )
    {
        if ( yw->additionalSet )
        {
            ypa_log_out("yw_LoadSet(): killing set object %d\n", yw->set_number);
            delete_class_obj(yw->additionalSet);
            yw->additionalSet = NULL;
            yw->set_number = 0;
        }

        yw->additionalSet = load_set_base();
        if ( !yw->additionalSet )
        {
            yw->set_number = 0;
            ypa_log_out("yw_LoadSet(): loading set object %d failed\n", setID);
            set_prefix_replacement("rsrc", rsr);
            return 0;
        }
        yw->set_number = setID;
        ypa_log_out("yw_LoadSet(): loaded set object %d ok\n", setID);
    }


    if ( setID != 46 )
    {
        FSMgr::FileHandle* fil = uaOpenFile("rsrc:scripts/set.sdf", "r");
        if ( !fil )
        {
            ypa_log_out("Couldn't open set description file, set %d!\n", setID);
            return 0;
        }

        BaseList &kid_list = yw->additionalSet->getBASE_kidList();

        int kid_id = 0;

        for(BaseList::iterator it = kid_list.begin(); it != kid_list.end(); it++)
        {
            if ( kid_id == 0 )
            {
                if ( !sub_44A12C(yw, *it) )
                {
                    delete fil;
                    return 0;
                }
            }
            else if ( kid_id == 1 )
            {
                if ( !yw_parse_lego(yw, fil, *it) )
                {
                    delete fil;
                    return 0;
                }

                if ( !yw_parse_subSect(yw, fil) )
                {
                    delete fil;
                    return 0;
                }

                if ( !yw_parse_sektor(yw, fil) )
                {
                    delete fil;
                    return 0;
                }
            }
            else if ( kid_id == 2 )
            {
                if ( !sub_44A97C(yw, *it) )
                {
                    delete fil;
                    return 0;
                }
            }

            kid_id++;
        }

        delete fil;
    }

    init_vals.clear();
    init_vals.Add(NC_STACK_rsrc::RSRC_ATT_NAME, "remap/tracyrmp.ilbm");

    yw->tracyrmp_ilbm = Nucleus::CInit<NC_STACK_ilbm>(init_vals);
    if ( !yw->tracyrmp_ilbm )
    {
        ypa_log_out("Couldn't load tracy remap table, set %d.\n", setID);
        return 0;
    }

    init_vals.clear();
    init_vals.Add(NC_STACK_rsrc::RSRC_ATT_NAME, "remap/shadermp.ilbm");

    yw->shadermp_ilbm = Nucleus::CInit<NC_STACK_ilbm>(init_vals);
    if ( !yw->shadermp_ilbm )
    {
        ypa_log_out("Couldn't load shade remap table, set %d.\n", setID);
        return 0;
    }

    GFXEngine::GFXe.setTracyRmp( yw->tracyrmp_ilbm->getBMD_pBitmap() );
    GFXEngine::GFXe.setShadeRmp( yw->shadermp_ilbm->getBMD_pBitmap() );

    yw->additionalBeeBox = NC_STACK_base::READ_BAS_FILE("rsrc:objects/beebox.base");
    if ( !yw->additionalBeeBox )
    {
        ypa_log_out("Couldn't load bbox object, set %d.\n", setID);
        return 0;
    }

    yw->additionalBeeBox->setBASE_static(1);

    if ( setID == 46 || setID == 42 )
    {
        if ( load_fonts_and_icons(yw) == 0 )
            return 0;
    }

    yw_InitMouseStuff(yw);
    return 1;
}


int loadTOD(_NC_STACK_ypaworld *yw, const char *fname)
{
    int tod = 0;
    if ( yw->GameShell )
    {
        char buf[256];
        sprintf(buf, "save:%s/%s", yw->GameShell->user_name, fname);
        FSMgr::FileHandle *fil = uaOpenFile(buf, "r");

        if ( fil )
        {
            char lnbuf[128];
            if ( fil->gets(lnbuf, 128) )
            {
                char *lnbrk = strpbrk(lnbuf, "\n\r;");
                if ( lnbrk )
                    *lnbrk = 0;

                tod = strtol(lnbuf, NULL, 0);
            }
            delete fil;
        }
    }
    return tod;
}

int writeTOD(_NC_STACK_ypaworld *yw, const char *fname, int tod)
{
    int v6 = 0;
    if ( yw->GameShell )
    {
        char buf[256];
        sprintf(buf, "save:%s/%s", yw->GameShell->user_name, fname);
        FSMgr::FileHandle *fil = uaOpenFile(buf, "w");

        if ( fil )
        {
            fil->printf("%d", tod);
            delete fil;
            v6 = 1;
        }
    }
    return v6;
}


void NC_STACK_ypaworld::GameShellBlitBkg(NC_STACK_bitmap *bitm)
{
    ypaworld.win3d->LockSurface();

    rstr_arg204 a4;
    a4.pbitm = bitm->getBMD_pBitmap();

    a4.float14 = -1.0;
    a4.float4 = -1.0;
    a4.float18 = -1.0;
    a4.float8 = -1.0;
    a4.float1C = 1.0;
    a4.floatC = 1.0;
    a4.float20 = 1.0;
    a4.float10 = 1.0;

    ypaworld.win3d->raster_func202(&a4);

    ypaworld.win3d->UnlockSurface();
}

void sub_4491A0(_NC_STACK_ypaworld *yw, const char *movie_fname)
{
    std::string buf;
    sub_412810(movie_fname, buf);

    const char *v6;
    v6 = buf.c_str();

    yw->win3d->windd_func323(&v6);

    INPe.sub_412D28(&input_states);

    input_states.downed_key = 0;
    input_states.downed_key_2 = 0;
    input_states.dword8 = 0;
}

void sub_44A908(_NC_STACK_ypaworld *yw)
{
    memset(yw->slurps1, 0, sizeof(slurp) * 6 * 6);
    memset(yw->slurps2, 0, sizeof(slurp) * 6 * 6);

    if ( yw->ColSide.skeleton )
    {
        delete_class_obj(yw->ColSide.skeleton);
        memset(&yw->ColSide, 0, sizeof(slurp2));
    }

    if ( yw->ColCross.skeleton )
    {
        delete_class_obj(yw->ColCross.skeleton);
        memset(&yw->ColCross, 0, sizeof(slurp2));
    }
}

void sb_0x4ea37c__sub1(_NC_STACK_ypaworld *yw)
{
    for (int i = 0 ; i < 256; i++)
    {
        if (yw->legos[i].sklt_obj)
            delete_class_obj(yw->legos[i].sklt_obj);
    }

    memset(yw->legos, 0, sizeof(cityBases) * 256);
}

void sb_0x4ea37c(_NC_STACK_ypaworld *yw)
{
    memset(yw->vhcls_models, 0, sizeof(vhclBases) * 512);
    sb_0x4ea37c__sub1(yw);
    sub_44A908(yw);
    memset(yw->subSectors, 0, sizeof(subSec) * 256);
    memset(yw->secTypes, 0, sizeof(secType) * 256);

    if ( yw->additionalSet )
    {
        delete_class_obj(yw->additionalSet);
        yw->additionalSet = NULL;
        yw->set_number = 0;
    }

    NC_STACK_display *win3d = GFXEngine::GFXe.getC3D();

    win3d->display_func272(NULL);
}

void sub_4EAC80(_NC_STACK_ypaworld *yw)
{
    big_ypa_Brf *brf = &yw->brief;

    sb_0x4ea37c(yw);

    if ( yw->typ_map )
    {
        delete_class_obj(yw->typ_map);
        yw->typ_map = NULL;
    }

    if ( brf->mbmap_img )
    {
        delete_class_obj(brf->mbmap_img);
        brf->mbmap_img = NULL;
    }

    if ( brf->briefing_map )
    {
        delete_class_obj(brf->briefing_map);
        brf->briefing_map = NULL;
    }

    brf->briefStage = 0;
    yw->self_full->GuiWinClose(&stru_5C91D0);
    stru_5C91D0.Free();
}

void yw_freeDebrief(_NC_STACK_ypaworld *yw)
{
    big_ypa_Brf *brf = &yw->brief;

    if ( yw->brief.copy2_of_ownmap )
    {
        delete_class_obj(yw->brief.copy2_of_ownmap);
        brf->copy2_of_ownmap = NULL;
    }

    if ( brf->copy2_of_typmap )
    {
        delete_class_obj(brf->copy2_of_typmap);
        brf->copy2_of_typmap = NULL;
    }

    if ( brf->mbmap_img )
    {
        delete_class_obj(brf->mbmap_img);
        brf->mbmap_img = 0;
    }

    if ( brf->briefing_map )
    {
        delete_class_obj(brf->briefing_map);
        brf->briefing_map = NULL;
    }

    for (int i = 0; i < 4; i++)
    {
        if (brf->wireless_db[i])
        {
            delete_class_obj(brf->wireless_db[i]);

            brf->wireless_db[i] = NULL;
            brf->wireless_db_skels[i] = NULL;
        }
    }
    brf->briefStage = 0;

    yw->field_2d90->field_40 = 8;
}

// Select map
void ypaworld_func158__sub4__sub1__sub0(_NC_STACK_ypaworld *yw, struC5 *inpt)
{
    float v3 = (float)inpt->ClickInf.move.screenPos.x / (float)yw->screen_width;
    float v4 = (float)inpt->ClickInf.move.screenPos.y / (float)yw->screen_height;

    if (v3 < 0.0)
        v3 = 0.0;
    if (v3 > 1.0)
        v3 = 1.0;

    if (v4 < 0.0)
        v4 = 0.0;
    if (v4 > 1.0)
        v4 = 1.0;

    int v6 = 0;
    int v7 = 0;

    if ( yw->LevelNet->ilbm_mask_map )
    {
        if ( inpt->ClickInf.selected_btnID == -1 )
        {
            bitmap_intern *a4 = yw->LevelNet->ilbm_mask_map->getBMD_pBitmap();

            int xpos = a4->width * v3;
            int ypos = a4->height * v4;

            uint8_t *pos = (uint8_t *)a4->buffer + a4->pitch * ypos + xpos;

            v7 = *pos;
            if ( v7 > 0 && v7 < 256 )
            {
                int v15 = yw->LevelNet->mapInfos[v7].field_0;
                if ( v15 != 0 && v15 != 1 && v15 != 4 )
                    v6 = 1;
            }
        }
    }

    if ( v6 )
    {
        if ( v7 != yw->LevelNet->field_BE38 )
        {
            if ( yw->GameShell )
                SFXEngine::SFXe.startSound(&yw->GameShell->samples1_info, 8);
        }
        yw->LevelNet->field_BE38 = v7;
    }
    else
    {
        yw->LevelNet->field_BE38 = 0;
    }
}


void splashScreen_OutText(_NC_STACK_ypaworld *yw, NC_STACK_display *w3d, const char *txt, int x, int y)
{
    char cmdbuf[2048];
    char txtbuf[256];

    char *cur = cmdbuf;

    if ( txt )
    {
        FontUA::select_tileset(&cur, 15);
        FontUA::set_xpos(&cur, x);
        FontUA::set_ypos(&cur, y);

        FontUA::set_txtColor(&cur, yw->iniColors[13].r, yw->iniColors[13].g, yw->iniColors[13].b);

        const char *txtpos = txt;

        int lastline = 0;
        while ( !lastline )
        {
            const char *en = strpbrk(txtpos, "\n\r");

            if (en)
            {
                int num = en - txtpos;

                memcpy(txtbuf, txtpos, num);
                txtbuf[num] = 0;

                txtpos = en + 1;
            }
            else
            {
                strcpy(txtbuf, txtpos);
                lastline = 1;
            }

            cur = FontUA::FormateClippedText(yw->tiles[15], cur, txtbuf, yw->screen_width - x, ' ');

            FontUA::next_line(&cur);

        }

        FontUA::set_end(&cur);

        w3d_a209 v15;
        v15.cmdbuf = cmdbuf;
        v15.includ = NULL;

        yw->win3d->raster_func209(&v15);
    }
}

void ypaworld_func158__sub4__sub1__sub2(_NC_STACK_ypaworld *yw)
{
    stru_LevelNet *lvlnet = yw->LevelNet;

    if ( lvlnet->ilbm_menu_map && lvlnet->ilbm_mask_map && lvlnet->ilbm_rollover_map )
    {
        yw->win3d->LockSurface();

        rstr_arg204 a4;
        a4.pbitm = yw->LevelNet->ilbm_menu_map->getBMD_pBitmap();

        a4.float4 = -1.0;
        a4.float8 = -1.0;
        a4.floatC = 1.0;
        a4.float10 = 1.0;

        a4.float14 = -1.0;
        a4.float18 = -1.0;
        a4.float1C = 1.0;
        a4.float20 = 1.0;

        yw->win3d->raster_func202(&a4);

        for (int i = 0; i < 256; i++)
        {
            mapINFO *v5 = &yw->LevelNet->mapInfos[i];

            if ( v5->field_9C.x1 != v5->field_9C.x2 )
            {
                bitmap_intern *v20 = NULL;

                if ( v5->field_0 == 2 ||  v5->field_0 == 3)
                {
                    if ( i == yw->LevelNet->field_BE38 )
                    {
                        v20 = yw->LevelNet->ilbm_rollover_map->getBMD_pBitmap();
                    }
                }

                if ( v20 )
                {
                    rstr_218_arg v17;
                    v17.bitm_intern = v20;
                    v17.bitm_intern2 = yw->LevelNet->ilbm_mask_map->getBMD_pBitmap();

                    v17.flg = i;
                    v17.rect2.x1 = v5->field_9C.x1;
                    v17.rect1.x1 = v17.rect2.x1;
                    v17.rect2.y1 = v5->field_9C.y1;
                    v17.rect1.y1 = v17.rect2.y1;
                    v17.rect2.x2 = v5->field_9C.x2;
                    v17.rect1.x2 = v17.rect2.x2;
                    v17.rect2.y2 = v5->field_9C.y2;
                    v17.rect1.y2 = v17.rect2.y2;

                    yw->win3d->raster_func218(&v17);
                }
            }
        }

        if ( yw->LevelNet->field_BE38 )
        {
            char cmdBuff[256];
            char *v11 = cmdBuff;

            if ( yw->LevelNet->mapInfos[ yw->LevelNet->field_BE38 ].field_0 )
            {
                if ( yw->LevelNet->mapInfos[ yw->LevelNet->field_BE38 ].map_name[0] )
                {
                    const char *v12 = get_lang_string(yw->string_pointers_p2, yw->LevelNet->field_BE38 + 1800, yw->LevelNet->mapInfos[ yw->LevelNet->field_BE38 ].map_name);

                    FontUA::select_tileset(&v11, 15);
                    FontUA::set_xpos(&v11, 0);
                    FontUA::set_ypos(&v11, -(yw->font_default_h + yw->icon_order__h + 4) );
                    FontUA::set_txtColor(&v11, yw->iniColors[63].r, yw->iniColors[63].g, yw->iniColors[63].b);

                    v11 = FontUA::FormateCenteredSkipableItem(yw->tiles[15], v11, v12, yw->screen_width);
                }
            }
            FontUA::set_end(&v11);

            w3d_a209 v19;

            v19.cmdbuf = cmdBuff;
            v19.includ = NULL;

            yw->win3d->raster_func209(&v19);
        }

        yw->win3d->UnlockSurface();
    }

}

void ypaworld_func158__sub4__sub1__sub1(_NC_STACK_ypaworld *yw)
{
    stru_LevelNet *lvlnet = yw->LevelNet;

    if ( lvlnet->ilbm_menu_map && lvlnet->ilbm_mask_map && lvlnet->ilbm_rollover_map && lvlnet->ilbm_finished_map && lvlnet->ilbm_enabled_map )
    {
        yw->win3d->LockSurface();

        rstr_arg204 a4;
        a4.pbitm = yw->LevelNet->ilbm_menu_map->getBMD_pBitmap();

        a4.float4 = -1.0;
        a4.float8 = -1.0;
        a4.floatC = 1.0;
        a4.float10 = 1.0;

        a4.float14 = -1.0;
        a4.float18 = -1.0;
        a4.float1C = 1.0;
        a4.float20 = 1.0;

        yw->win3d->raster_func202(&a4);
        //printf("field_BE38 %d \n",yw->LevelNet->field_BE38);

        for (int i = 0; i < 256; i++)
        {
            mapINFO *v5 = &yw->LevelNet->mapInfos[i];

            if ( v5->field_9C.x1 != v5->field_9C.x2 )
            {
                bitmap_intern *v20 = NULL;

                if ( v5->field_0 == 2 )
                {
                    if ( i == yw->LevelNet->field_BE38 )
                    {
                        v20 = yw->LevelNet->ilbm_rollover_map->getBMD_pBitmap();
                    }
                    else
                    {
                        v20 = yw->LevelNet->ilbm_enabled_map->getBMD_pBitmap();
                    }
                }
                else if ( v5->field_0 == 3 )
                {
                    v20 = yw->LevelNet->ilbm_finished_map->getBMD_pBitmap();
                }

                if ( v20 )
                {
                    rstr_218_arg v17;
                    v17.bitm_intern = v20;
                    v17.bitm_intern2 = yw->LevelNet->ilbm_mask_map->getBMD_pBitmap();

                    v17.flg = i;
                    v17.rect2.x1 = v5->field_9C.x1;
                    v17.rect1.x1 = v17.rect2.x1;
                    v17.rect2.y1 = v5->field_9C.y1;
                    v17.rect1.y1 = v17.rect2.y1;
                    v17.rect2.x2 = v5->field_9C.x2;
                    v17.rect1.x2 = v17.rect2.x2;
                    v17.rect2.y2 = v5->field_9C.y2;
                    v17.rect1.y2 = v17.rect2.y2;

                    yw->win3d->raster_func218(&v17);
                }
            }
        }

        if ( yw->LevelNet->field_BE38 )
        {
            char cmdBuff[256];
            char *v11 = cmdBuff;

            if ( yw->LevelNet->mapInfos[ yw->LevelNet->field_BE38 ].field_0 )
            {
                if ( yw->LevelNet->mapInfos[ yw->LevelNet->field_BE38 ].map_name[0] )
                {
                    const char *v12 = get_lang_string(yw->string_pointers_p2, yw->LevelNet->field_BE38 + 1800, yw->LevelNet->mapInfos[ yw->LevelNet->field_BE38 ].map_name);

                    FontUA::select_tileset(&v11, 15);
                    FontUA::set_xpos(&v11, 0);
                    FontUA::set_ypos(&v11, -(yw->font_default_h + yw->icon_order__h + 4) );
                    FontUA::set_txtColor(&v11, yw->iniColors[63].r, yw->iniColors[63].g, yw->iniColors[63].b);

                    v11 = FontUA::FormateCenteredSkipableItem(yw->tiles[15], v11, v12, yw->screen_width);
                }
            }
            FontUA::set_end(&v11);

            w3d_a209 v19;

            v19.cmdbuf = cmdBuff;
            v19.includ = NULL;

            yw->win3d->raster_func209(&v19);
        }
        const char *v13 = get_lang_string(yw->string_pointers_p2, yw->TOD_ID + 2490, " ");
        splashScreen_OutText(yw, yw->win3d, v13, yw->screen_width / 20, yw->screen_width / 20);

        yw->win3d->UnlockSurface();
    }
}


int ypaworld_func158__sub4__sub1__sub3__sub0(_NC_STACK_ypaworld *yw)
{
    int lft = yw->screen_width * 0.575;
    int rght = yw->screen_width * 0.9734375;
    int top = yw->screen_height * 0.08958333333333329;
    int bottom = yw->screen_height * 0.9083333333333334;

    int v5 = (bottom - top) / yw->font_default_h;

    memset(&stru_5C91D0, 0, sizeof(GuiList));

    GuiList::tInit args;
    args.numEntries = 1;
    args.shownEntries = v5;
    args.minShownEntries = v5;
    args.maxShownEntries = v5;
    args.withIcon = false;
    args.entryHeight = yw->font_default_h;
    args.entryWidth = (rght - lft);
    args.enabled = true;
    args.vborder = yw->field_1a38;

    int result = stru_5C91D0.Init(yw, args);

    if ( !result )
        return 0;

    stru_5C91D0.SetRect(yw, lft, top);
    yw->self_full->GuiWinOpen( &stru_5C91D0 );

    return 1;
}

int ypaworld_func158__sub4__sub1__sub3(_NC_STACK_ypaworld *yw, int lvlid)
{
    big_ypa_Brf *brf = &yw->brief;

    memset(brf, 0, sizeof(big_ypa_Brf));

    brf->mouseSelectedElement = -1;
    brf->field_2F90 = -1;

    SFXEngine::SFXe.StopMusicTrack();

    if ( !ypaworld_func158__sub4__sub1__sub3__sub0(yw) )
        return 0; // May be HACK

    yw->win3d->display_func271(NULL);
    yw->win3d->LockSurface();
    yw->win3d->raster_func192(NULL);
    yw->win3d->UnlockSurface();

    memcpy(&brf->s2d90, yw->field_2d90, sizeof(brf->s2d90));

    brf->briefStage = 0;
    brf->field_2E6C = 0;
    brf->currTime = 0;
    brf->field_2E80 = 0;

    yw->field_2d90->levelID = lvlid;
    yw->field_2d90->field_40 = 5;
    yw->field_2d90->gate_count = 0;

    for (int i = 0; i < 8; i++)
    {
        yw->gems[i].init();
    }
    memset(yw->field_2d90->gates, 0, sizeof(gateProto) * 8);

    set_prefix_replacement("rsrc", "data:");

    mapProto *mproto = &brf->map_prototype;

    if ( sub_4DA41C(yw, mproto, yw->LevelNet->mapInfos[ lvlid ].mapPath) )
    {
        if ( (brf->map_prototype.flags & 0x7F) == 127 )
        {
            brf->LEVEL_BRIEF_INFO = get_lang_string(yw->string_pointers_p2, yw->field_2d90->levelID + 2100, "<NO INFO AVAILABLE>");

            if ( mproto->mbmaps_count )
            {
                if ( !yw->field_2d90->movie[0] || brf->briefStage )
                {
                    for (int i = 0; i < mproto->mapRobos_count; i++)
                    {
                        int owner = mproto->mapRobos[i].owner;
                        if ( !yw->field_2d90->jodiefoster[ owner ] )
                        {
                            int v19;

                            switch ( owner )
                            {
                            case 2:
                                v19 = 7;
                                break;
                            case 3:
                                v19 = 6;
                                break;
                            case 4:
                                v19 = 5;
                                break;
                            case 5:
                                v19 = 8;
                                break;
                            case 6:
                                v19 = 4;
                                break;
                            default:
                                v19 = -1;
                                break;
                            }

                            if ( v19 != -1 && yw->movies.movies_names_present[v19] )
                            {
                                char *from;
                                switch (v19)
                                {
                                default:
                                case 0:
                                    from = yw->movies.game_intro;
                                    break;

                                case 1:
                                    from = yw->movies.lose_extro;
                                    break;

                                case 2:
                                    from = yw->movies.win_extro;
                                    break;

                                case 3:
                                    from = yw->movies.user_intro;
                                    break;

                                case 4:
                                    from = yw->movies.kyt_intro;
                                    break;

                                case 5:
                                    from = yw->movies.taer_intro;
                                    break;

                                case 6:
                                    from = yw->movies.myk_intro;
                                    break;

                                case 7:
                                    from = yw->movies.sulg_intro;
                                    break;

                                case 8:
                                    from = yw->movies.black_intro;
                                    break;
                                }

                                strcpy(brf->movie, from);

                                yw->field_81AB = owner;
                                brf->briefStage = 28;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    strcpy(brf->movie, yw->field_2d90->movie);
                    brf->briefStage = 28;
                }

                set_prefix_replacement("rsrc", "levels:");

                IDVList init_vals;
                init_vals.Add(NC_STACK_rsrc::RSRC_ATT_NAME, yw->LevelNet->brief_map[0].map_name);
                init_vals.Add(NC_STACK_bitmap::BMD_ATT_TEXTURE, 1);
                init_vals.Add(NC_STACK_bitmap::BMD_ATT_TEXTURE_SYS, 1);

                if ( yw->LevelNet->brief_map[0].map_name[0] )
                    brf->briefing_map = Nucleus::CInit<NC_STACK_ilbm>(init_vals);

                set_prefix_replacement("rsrc", "mbpix:");

                init_vals.clear();
                init_vals.Add(NC_STACK_rsrc::RSRC_ATT_NAME, mproto->mbmaps[0].name);
                init_vals.Add(NC_STACK_bitmap::BMD_ATT_TEXTURE, 1);
                init_vals.Add(NC_STACK_bitmap::BMD_ATT_TEXTURE_SYS, 1);

                brf->mbmap_img = Nucleus::CInit<NC_STACK_ilbm>(init_vals);

                if ( brf->mbmap_img )
                {
                    init_vals.clear();
                    init_vals.Add(NC_STACK_rsrc::RSRC_ATT_NAME, mproto->typ);

                    if ( !yw->typ_map )
                        yw->typ_map = Nucleus::CInit<NC_STACK_ilbm>(init_vals);

                    if ( yw->typ_map )
                    {
                        brf->typ_map_bitm = yw->typ_map->getBMD_pBitmap();

                        yw->sectors_maxX2 = brf->typ_map_bitm->width;
                        yw->sectors_maxY2 = brf->typ_map_bitm->height;

                        yw->map_Width_meters = yw->sectors_maxX2 * 1200.0;
                        yw->map_Height_meters = yw->sectors_maxY2 * 1200.0;

                        //Set transitions
                        brf->brf_objs.field_10 = -0.98750001;
                        brf->brf_objs.field_14 = 0.34999999;
                        brf->brf_objs.field_18 = -0.003125;
                        brf->brf_objs.field_1C = 0.85416669;

                        if ( !brf->briefStage )
                            brf->briefStage = 4;

                        sb_0x4ea37c(yw);
                        return 1;
                    }

                    delete_class_obj(brf->mbmap_img);
                    brf->mbmap_img = NULL;
                }
            }
        }
    }

    yw->self_full->GuiWinClose( &stru_5C91D0 );
    stru_5C91D0.Free();

    memcpy(yw->field_2d90, &brf->s2d90, sizeof(stru_2d90));

    return 0;
}

int ypaworld_func158__sub4__sub1__sub5__sub0(_NC_STACK_ypaworld *yw, mapProto *mapproto, const char *filename)
{
    scrCallBack parsers[6];

    memset(parsers, 0, sizeof(parsers));
    memset(mapproto, 0, sizeof(mapProto));

    parsers[0].dataForStore = mapproto;
    parsers[0].world = yw;
    parsers[0].func = LevelDataParser;

    parsers[1].dataForStore = mapproto;
    parsers[1].world = yw;
    parsers[1].func = LevelGatesParser;

    parsers[2].func = LevelMbMapParser;
    parsers[2].dataForStore = mapproto;

    parsers[3].dataForStore = mapproto;
    parsers[3].world = yw;
    parsers[3].func = LevelMapsParser;

    parsers[4].dataForStore = mapproto;
    parsers[4].func = LeveldbMapParser;

    parsers[5].dataForStore = mapproto;
    parsers[5].func = LevelSuperItemsParser;
    parsers[5].world = yw;

    return def_parseFile(filename, 6, parsers, 6);
}

NC_STACK_bitmap * sub_44816C(NC_STACK_bitmap *src, const char *name)
{
    bitmap_intern *a4 = src->getBMD_pBitmap();

    IDVList init_vals;
    init_vals.Add(NC_STACK_rsrc::RSRC_ATT_NAME, name);
    init_vals.Add(NC_STACK_bitmap::BMD_ATT_WIDTH, a4->width);
    init_vals.Add(NC_STACK_bitmap::BMD_ATT_HEIGHT, a4->height);

    NC_STACK_bitmap *v3 = Nucleus::CInit<NC_STACK_bitmap>(init_vals);

    if ( v3 )
    {
        bitmap_intern *v6 = v3->getBMD_pBitmap();

        memcpy(v6->buffer, a4->buffer, a4->width * a4->height);
    }

    return v3;
}

size_t ypaworld_func158__sub4__sub1__sub5(_NC_STACK_ypaworld *yw)
{
    big_ypa_Brf *brf = &yw->brief;

    memset(&yw->brief, 0, sizeof(big_ypa_Brf));

    if ( !yw->copyof_ownermap || !yw->copyof_typemap )
    {
        yw_freeDebrief(yw);
        return 0;
    }

    brf->field_41A0 = yw->field_2d90->field_40 == 1;

    ua_fRect v17;
    v17.x1 = -1.0;
    v17.y1 = -1.0;
    v17.x2 = 1.0;
    v17.y2 = 1.0;

    yw->win3d->raster_func210(&v17);

    brf->briefStage = 0;
    brf->field_2E6C = 0;
    brf->currTime = 0;
    brf->field_41D4 = 1;

    yw->field_2d90->field_40 = 9;

    set_prefix_replacement("rsrc", "data:");

    const char *wireless_db[4] =
    {
        "wireless/db_genes.sklt",
        "wireless/db_death.sklt",
        "wireless/db_death.sklt",
        "wireless/db_sec.sklt"
    };

    for (int i = 0; i < 4; i++)
    {
        IDVList init_vals;
        init_vals.Add(NC_STACK_rsrc::RSRC_ATT_NAME, wireless_db[i]);

        brf->wireless_db[i] = Nucleus::CInit<NC_STACK_sklt>(init_vals);
        if (brf->wireless_db[i])
            brf->wireless_db_skels[i] = brf->wireless_db[i]->getSKEL_pSkelet();
    }

    if ( yw->copyof_ownermap )
    {
        brf->copy2_of_ownmap = sub_44816C(yw->copyof_ownermap, "copy2_of_ownmap");
        brf->copy2_of_ownmap_bitm = brf->copy2_of_ownmap->getBMD_pBitmap();
    }

    if ( yw->copyof_typemap )
    {
        brf->copy2_of_typmap = sub_44816C(yw->copyof_typemap, "copy2_of_typmap");
        brf->copy2_of_typmap_bitm = brf->copy2_of_typmap->getBMD_pBitmap();
    }

    brf->field_419C = 0;

    brf->field_419C = 1;

    if ( !ypaworld_func158__sub4__sub1__sub5__sub0(yw, &brf->map_prototype, yw->LevelNet->mapInfos[yw->field_2d90->levelID].mapPath) )
    {
        yw_freeDebrief(yw);
        return 0;
    }

    dbmapProto *v8 = brf->map_prototype.dbmap_count ? brf->map_prototype.dbmaps : brf->map_prototype.mbmaps;

    if ( !v8->name[0] )
    {
        yw_freeDebrief(yw);
        return 0;
    }

    set_prefix_replacement("rsrc", "levels:");


    IDVList init_vals;
    init_vals.Add(NC_STACK_rsrc::RSRC_ATT_NAME, yw->LevelNet->debrief_map[0].map_name);
    init_vals.Add(NC_STACK_bitmap::BMD_ATT_TEXTURE, 1);
    init_vals.Add(NC_STACK_bitmap::BMD_ATT_TEXTURE_SYS, 1);

    if ( yw->LevelNet->debrief_map[0].map_name[0] )
        brf->briefing_map = Nucleus::CInit<NC_STACK_ilbm>(init_vals);

    set_prefix_replacement("rsrc", "mbpix:");

    init_vals.clear();
    init_vals.Add(NC_STACK_rsrc::RSRC_ATT_NAME, v8->name);
    init_vals.Add(NC_STACK_bitmap::BMD_ATT_TEXTURE, 1);
    init_vals.Add(NC_STACK_bitmap::BMD_ATT_TEXTURE_SYS, 1);

    brf->mbmap_img = Nucleus::CInit<NC_STACK_ilbm>(init_vals);
    if ( !brf->mbmap_img )
    {
        yw_freeDebrief(yw);
        return 0;
    }

    brf->typ_map_bitm = yw->copyof_typemap->getBMD_pBitmap();

    yw->sectors_maxX2 = brf->typ_map_bitm->width;
    yw->sectors_maxY2 = brf->typ_map_bitm->height;
    yw->map_Width_meters = yw->sectors_maxX2 * 1200.0;
    yw->map_Height_meters = yw->sectors_maxY2 * 1200.0;

    brf->briefStage = 4;

    return 1;
}


void yw_calcPlayerScore(_NC_STACK_ypaworld *yw)
{
    if ( !yw->field_727c )
    {
        int last_time = 0;

        yw_f726c_nod *hnode = (yw_f726c_nod *)yw->history->lst.head;
        while (hnode->next)
        {
            int v14 = 1;
            int tlen = 0;
            uint8_t *bf = hnode->bufStart;

            while (v14)
            {
                yw_arg184 arg184;

                yw_histbf_read_evnt(bf, &arg184);

                switch(arg184.type)
                {
                case 0:
                    tlen = 0;
                    v14 = 0;
                    break;

                case 1:
                    tlen = 5;
                    last_time = arg184.t15.field_1;
                    break;

                case 2:
                case 6:
                    tlen = 4;
                    yw_score(yw, &arg184, yw->playerstatus);
                    break;

                case 3:
                case 4:
                    tlen = 6;
                    yw_score(yw, &arg184, yw->playerstatus);
                    break;

                case 7:
                    tlen = 12;
                    yw_score(yw, &arg184, yw->playerstatus);
                    break;

                case 5:
                default:
                    break;
                }

                bf += tlen;

            }

            hnode = (yw_f726c_nod *)hnode->next;
        }

        for (int i = 0; i < 8; i++)
            yw->playerstatus[i].elapsedTime += last_time;

    }
}

void ypaworld_func158__sub4__sub1(_NC_STACK_ypaworld *yw, UserData *usr, struC5 *inpt)
{
    if ( yw->field_2d90->field_40 != 1 && yw->field_2d90->field_40 != 2 )
    {
        if ( yw->brief.briefStage )
        {
            if ( yw->field_2d90->field_40 == 5 )
            {
                if ( inpt->downed_key == UAVK_RETURN )
                    yw->brief.briefStage = 1;

                if ( yw->brief.briefStage == 1 )
                {
                    usr->envAction.action = EnvAction::ACTION_PLAY;
                    usr->envAction.params[0] = yw->field_2d90->levelID;
                    usr->envAction.params[1] = yw->field_2d90->levelID;
                    sub_4EAC80(yw);
                }
                else if ( yw->brief.briefStage == 2 )
                {
                    sub_4EAC80(yw);
                    yw->field_2d90->field_40 = 8;
                }
                else
                {
                    ypaworld_func158__sub4__sub1__sub4(yw, usr, inpt);
                }
            }
            else if ( yw->field_2d90->field_40 == 9 )
            {
                if ( yw->brief.briefStage == 2 )
                {
                    yw_calcPlayerScore(yw);
                    yw_freeDebrief(yw);
                }
                else
                {
                    yw_debriefUpdate(yw, inpt);
                }
            }
        }
        else
        {
            ypaworld_func158__sub4__sub1__sub0(yw, inpt);

            if ( yw->GameShell->envMode == ENVMODE_TUTORIAL )
                ypaworld_func158__sub4__sub1__sub2(yw);
            else
                ypaworld_func158__sub4__sub1__sub1(yw);


            if ( yw->LevelNet->field_BE38 )
            {
                if ( inpt->ClickInf.flag & ClickBoxInf::FLAG_LM_DOWN )
                {
                    if ( yw->LevelNet->mapInfos[ yw->LevelNet->field_BE38 ].field_0 == 3 )
                    {
//            v14 = get_lang_string(yw->string_pointers_p2, 2466, "2466 == THE YPA CD IS REQUIRED.");
//            v15 = get_lang_string(yw->string_pointers_p2, 21, "21 == YOUR PERSONAL AMOK");
//            if ( make_CD_CHECK(0, 1, v15, v14) )
//            {
                        usr->envAction.action = EnvAction::ACTION_PLAY;
                        usr->envAction.params[0] = yw->LevelNet->field_BE38;
                        usr->envAction.params[1] = yw->LevelNet->field_BE38;
//            }
                    }
                    else
                    {
//            v16 = get_lang_string(yw->string_pointers_p2, 2466, "2466 == THE YPA CD IS REQUIRED.");
//            v17 = get_lang_string(yw->string_pointers_p2, 21, "21 == YOUR PERSONAL AMOK");
//            if ( make_CD_CHECK(0, 1, v17, v16) )
//            {
                        if ( !ypaworld_func158__sub4__sub1__sub3(yw, yw->LevelNet->field_BE38) )// LOAD LEVEL INFO
                        {
                            usr->envAction.action = EnvAction::ACTION_PLAY;
                            usr->envAction.params[0] = yw->LevelNet->field_BE38;
                            usr->envAction.params[1] = yw->LevelNet->field_BE38;
                        }
//            }
                    }
                }
                else if ( inpt->ClickInf.flag & ClickBoxInf::FLAG_RM_DOWN )
                {
//          v18 = get_lang_string(yw->string_pointers_p2, 2466, "2466 == THE YPA CD IS REQUIRED.");
//          v19 = get_lang_string(yw->string_pointers_p2, 21, "21 == YOUR PERSONAL AMOK");
//          if ( make_CD_CHECK(0, 1, v19, v18) )
//          {
                    usr->envAction.action = EnvAction::ACTION_PLAY;
                    usr->envAction.params[0] = yw->LevelNet->field_BE38;
                    usr->envAction.params[1] = yw->LevelNet->field_BE38;
//          }
                }
            }
        }
    }
    else
    {
        if ( yw->field_2d90->field_40 == 1 )
        {
            if ( yw->field_2d90->win_movie[0] )
                sub_4491A0(yw, yw->field_2d90->win_movie);
        }

        if ( yw->field_2d90->field_40 == 2 )
        {
            if ( yw->field_2d90->lose_movie[0] )
            {
                if ( yw->field_1624 )
                    sub_4491A0(yw, yw->field_2d90->lose_movie);
            }
        }

        yw->brief.briefStage = 0;
        yw->field_2d90->field_40 = 8;

        if ( yw->field_7278 )
            ypaworld_func158__sub4__sub1__sub5(yw);
    }
}


int sub_4DA41C(_NC_STACK_ypaworld *yw, mapProto *mapp, const char *fname)
{
    scrCallBack parsers[13];

    memset(parsers, 0, sizeof(parsers));
    memset(mapp, 0, sizeof(mapProto));

    parsers[0].dataForStore = mapp;
    parsers[0].world = yw;
    parsers[0].func = LevelDataParser;

    parsers[1].dataForStore = mapp;
    parsers[1].func = parse_map_robos;

    parsers[2].func = LevelGemParser;
    parsers[2].dataForStore = mapp;
    parsers[2].world = yw;

    parsers[3].dataForStore = mapp;
    parsers[3].func = LevelSquadParser;

    parsers[4].dataForStore = mapp;
    parsers[4].world = yw;
    parsers[4].func = LevelGatesParser;

    parsers[5].dataForStore = mapp;
    parsers[5].func = LevelMbMapParser;

    parsers[6].world = yw;
    parsers[6].func = VhclProtoParser;

    parsers[7].world = yw;
    parsers[7].func = WeaponProtoParser;

    parsers[8].world2 = yw;
    parsers[8].func = BuildProtoParser;

    parsers[9].world = yw;
    parsers[9].func = LevelEnableParser;

    parsers[10].dataForStore = mapp;
    parsers[10].world = yw;
    parsers[10].func = LevelMapsParser;

    parsers[11].dataForStore = mapp;
    parsers[11].func = LeveldbMapParser;

    parsers[12].dataForStore = mapp;
    parsers[12].world = yw;
    parsers[12].func = LevelSuperItemsParser;

    return def_parseFile(fname, 13, parsers, 1);
}


char * sub_4C4284(_NC_STACK_ypaworld *yw, GuiList *lstvw, char *out, const char *txt)
{
    char * tmp = out;

    FontUA::store_u8(&tmp, 123);
    FontUA::select_tileset(&tmp, 9);

    FontUA::store_u8(&tmp, 98);

    FontUA::set_txtColor(&tmp, yw->iniColors[62].r, yw->iniColors[62].g, yw->iniColors[62].b);

    tmp = FontUA::FormateClippedText(yw->tiles[9], tmp, txt, lstvw->entryWidth - 4 * yw->font_default_w__b, 99);

    FontUA::store_u8(&tmp, 100);

    FontUA::select_tileset(&tmp, 0);

    FontUA::store_u8(&tmp, 125);
    FontUA::next_line(&tmp);

    return tmp;
}

char * sub_4C41DC(_NC_STACK_ypaworld *yw, GuiList *lstvw, char *out, const char *txt)
{

    char * tmp = out;

    FontUA::store_u8(&tmp, 123);

    FontUA::set_txtColor(&tmp, yw->iniColors[61].r, yw->iniColors[61].g, yw->iniColors[61].b);

    tmp = FontUA::FormateClippedText(yw->tiles[0], tmp, txt, lstvw->entryWidth - 2 * yw->font_default_w__b, 32);

    FontUA::store_u8(&tmp, 125);
    FontUA::next_line(&tmp);

    return tmp;
}

void ypaworld_func158__video_list_draw(_NC_STACK_ypaworld *yw, UserData *usr)
{
    char *v3 = usr->video_listvw.itemBlock;

    usr->video_listvw.SetRect(yw, -2, -2);

    v3 = usr->video_listvw.ItemsPreLayout(yw, v3, 0, "uvw");

    int v5 = 0;

    video_mode_node *v20 = (video_mode_node *)usr->video_mode_list.head;

    while (v20->next)
    {
        if ( v5 >= usr->video_listvw.firstShownEntries && v5 < usr->video_listvw.firstShownEntries + usr->video_listvw.shownEntries)
        {
            if ( usr->video_listvw.selectedEntry == v5 )
                v3 = sub_4C4284(yw, &usr->video_listvw, v3, v20->name);
            else
                v3 = sub_4C41DC(yw, &usr->video_listvw, v3, v20->name);
        }
        v5++;
        v20 = (video_mode_node *)v20->next;
    }

    if ( v5 >= 4 )
        v5 = 4;

    usr->video_listvw.shownEntries = v5;
    v3 = usr->video_listvw.ItemsPostLayout(yw, v3, 0, "xyz");
    FontUA::set_end(&v3);

    w3d_a209 v16;
    v16 = usr->video_listvw.cmdstrm;

    GFXEngine::GFXe.drawText(&v16);
}

void ypaworld_func158__d3d_list_draw(_NC_STACK_ypaworld *yw, UserData *usr)
{
    char *v3 = usr->d3d_listvw.itemBlock;

    usr->d3d_listvw.SetRect(yw, -2, -2);

    v3 = usr->d3d_listvw.ItemsPreLayout(yw, v3, 0, "uvw");

    int v5 = 0;

    wdd_func324arg v15;
    v15.name = NULL;
    v15.guid = NULL;
    v15.currr = 0;

    NC_STACK_win3d *windd = dynamic_cast<NC_STACK_win3d *>(yw->win3d);
    windd->windd_func324(&v15);

    while (v15.name)
    {
        if ( v15.name )
        {
            const char *v6;

            if ( !strcmp(v15.name, "software") )
                v6 = get_lang_string(yw->string_pointers_p2, 2472, "2472 = Software");
            else
                v6 = v15.name;

            if ( v5 >= usr->d3d_listvw.firstShownEntries && v5 < usr->d3d_listvw.firstShownEntries + usr->d3d_listvw.shownEntries )
            {
                if ( usr->d3d_listvw.selectedEntry == v5 )
                    v3 = sub_4C4284(yw, &usr->d3d_listvw, v3, v6);
                else
                    v3 = sub_4C41DC(yw, &usr->d3d_listvw, v3, v6);
            }
            v5++;
        }
        windd->windd_func324(&v15);
    }


    if ( v5 >= 4)
        usr->d3d_listvw.shownEntries = 4;
    else
        usr->d3d_listvw.shownEntries = v5;

    usr->d3d_listvw.numEntries = v5;
    v3 = usr->d3d_listvw.ItemsPostLayout(yw, v3, 0, "xyz");

    FontUA::set_end(&v3);

    w3d_a209 v16;
    v16 = usr->d3d_listvw.cmdstrm;

    GFXEngine::GFXe.drawText(&v16);
}

char * sub_4DDF78(_NC_STACK_ypaworld *yw, GuiList *lstvw, char *pos, int a3)
{
    char *v3 = pos;

    for ( int i = 0; i < a3; i++ )
        v3 = sub_4C41DC(yw, lstvw, v3, " ");

    return v3;
}

void ypaworld_func158__network_list_draw(_NC_STACK_ypaworld *yw, UserData *usr)
{
    bool slct = false;

    char *cmd = usr->network_listvw.itemBlock;
    usr->network_listvw.SetRect(yw, -2, -2);

    cmd = usr->network_listvw.ItemsPreLayout(yw, cmd, 0, "uvw");

    NC_STACK_button::button_71arg setStr;
    setStr.butID = 1200;
    setStr.field_8 = usr->netName;

    int cnt = -1;

    std::string str1;
    std::string str2;
    std::string str3;
    std::string str4;

    std::string connTp;

    int i = 0;

    for(;; i++)
    {
        bool brk = true;
        str1 = "";
        str2 = "";
        str3 = "";
        str4 = "";

        switch(usr->netSelMode)
        {
        case 0: //provider
        {
            windp_getNameMsg msg;
            msg.id = i;
            if ( yw->windp->GetProviderName(&msg) )
            {
                str1 = msg.name;
                brk = false;

                if (usr->netSel == -1 && i == 0)
                {
                    strcpy(usr->netName, msg.name);

                    setStr.field_4 = usr->netName;
                    usr->network_button->button_func71(&setStr);
                    usr->netSel = 0;
                }

            }
            else
                str1 = "----";
        }
        break;

        case 1: //sessions
        {
            windp_getNameMsg msg;
            msg.id = i;
            if ( yw->windp->GetSessionName(&msg) )
            {
                size_t j;
                for (j = 0; (uint8_t)msg.name[j] >= ' ' && msg.name[j] != '|'; j++ )
                    str1 += msg.name[j];

                int lvlid = atoi(str1.c_str());

                str1 = get_lang_string( yw->string_pointers_p2, lvlid + 1800, yw->LevelNet->mapInfos[ lvlid ].map_name );

                if (msg.name[j] == 0)
                    str4 = "";
                else
                {
                    for (j = j + 1; (uint8_t)msg.name[j] >= ' '; j++ )
                        str4 += msg.name[j];
                }

                brk = false;

                if (usr->netSel == -1 && i == 0)
                {
                    str1 = msg.name;

                    for (j = 0; j < str1.length(); j++)
                    {
                        if (str1[j] == '|')
                        {
                            str1[j] = 0;
                            break;
                        }
                    }

                    lvlid = atoi(str1.c_str());

                    strcpy(usr->netName, yw->LevelNet->mapInfos[ lvlid ].map_name);

                    setStr.field_4 = usr->netName;
                    usr->network_button->button_func71(&setStr);

                    usr->netSel = 0;
                }
            }
            else
                str1 = "----";
        }
        break;

        case 3: //level
            if ( i < usr->map_descriptions_count )
            {
                mapINFO *lvl = &yw->LevelNet->mapInfos[ usr->map_descriptions[i].id ];

                if ( lvl->robos_count < (int)yw->windp->CountPlayers(NULL) )
                    continue;

                str1 = usr->map_descriptions[i].pstring;

                brk = false;

                char buf[64];
                sprintf(buf, "%d X %d", lvl->secXsize, lvl->secYsize);
                str2 = buf;

                cnt++;

                if ( usr->netSel == cnt )
                {
                    slct = true;

                    if ( lvl->fractions_mask & 2 )
                        str3 = "1";

                    if ( lvl->fractions_mask & 0x40 )
                        str3 += "2";

                    if ( lvl->fractions_mask & 8 )
                        str3 += "3";

                    if ( lvl->fractions_mask & 0x10 )
                        str3 += "4";

                    if ( lvl->slow_connection )
                        connTp = "Y";
                    else
                        connTp = "j";
                }
                else
                {
                    slct = false;

                    if ( lvl->fractions_mask & 2 )
                        str3 = "5";

                    if ( lvl->fractions_mask & 0x40 )
                        str3 += "6";

                    if ( lvl->fractions_mask & 8 )
                        str3 += "7";

                    if ( lvl->fractions_mask & 0x10 )
                        str3 += "8";

                    if ( lvl->slow_connection )
                        connTp = "X";
                    else
                        connTp = "k";
                }

                if ( usr->netSel == -1 && cnt == 0)
                {
                    strcpy(usr->netName, usr->map_descriptions[ i ].pstring);

                    setStr.field_4 = usr->netName;
                    usr->network_button->button_func71(&setStr);

                    usr->netLevelName = usr->map_descriptions[ i ].pstring;
                    usr->netLevelID = usr->map_descriptions[ i ].id;
                    usr->netSel = 0;
                }
            }
            break;

        case 2:
        case 4:
            if ( i < usr->msgBuffLine )
            {
                str1 = usr->msgBuffers[ i ];
                brk = false;
            }
            break;

        default:
            break;
        }

        if (brk)
            break;

        if (usr->netSelMode == 3)
        {
            int tmp = i;
            i = cnt;
            cnt = tmp;
        }

        if (usr->network_listvw.firstShownEntries <= i && (usr->network_listvw.firstShownEntries + usr->network_listvw.shownEntries) > i)
        {
            FontUA::ColumnItem items[6];

            uint8_t fontID;
            uint8_t spaceChar;
            uint8_t prefixChar;
            uint8_t postfixChar;

            if ( usr->netSel != i || usr->netSelMode == 4 || usr->netSelMode == 2 )
            {
                fontID = 0;
                spaceChar = 102;
                prefixChar = 102;
                postfixChar = 102;
            }
            else
            {
                fontID = 9;
                prefixChar = 98;
                spaceChar = 99;
                postfixChar = 100;
            }

            int wdth = usr->network_listvw.entryWidth - 2 * yw->font_default_w__b;
            int itemsCount = 0;

            switch(usr->netSelMode)
            {
            case 0:
                items[0].fontID = fontID;
                items[0].spaceChar = spaceChar;
                items[0].prefixChar = prefixChar;
                items[0].postfixChar = postfixChar;
                items[0].txt = str1.c_str();
                items[0].flags = 0x27;
                items[0].width = wdth;
                itemsCount = 1;
                break;

            case 1:
                items[0].fontID = fontID;
                items[0].spaceChar = spaceChar;
                items[0].prefixChar = prefixChar;
                items[0].postfixChar = postfixChar;
                items[0].txt = str4.c_str();
                items[0].width = wdth * 0.5;
                items[0].flags = 0x25;

                items[1].fontID = fontID;
                items[1].spaceChar = spaceChar;
                items[1].prefixChar = prefixChar;
                items[1].postfixChar = postfixChar;
                items[1].txt = " ";
                items[1].width = 10;
                items[1].flags = 0x24;

                items[2].fontID = fontID;
                items[2].spaceChar = spaceChar;
                items[2].prefixChar = prefixChar;
                items[2].postfixChar = postfixChar;
                items[2].txt = str1.c_str();
                items[2].flags = 0x26;
                items[2].width = wdth - (items[0].width + items[1].width);

                itemsCount = 3;
                break;

            case 3:
            {
                tiles_stru *tiles = GFXEngine::GFXe.getTileset(8);

                items[0].txt = connTp.c_str();
                items[0].width = tiles->chars[80].width;
                items[0].fontID = 8;
                items[0].spaceChar = (slct == false) + 106;
                items[0].prefixChar = prefixChar;
                items[0].postfixChar = postfixChar;
                items[0].flags = 4;

                items[1].txt = str1.c_str();
                items[1].fontID = fontID;
                items[1].spaceChar = spaceChar;
                items[1].prefixChar = prefixChar;
                items[1].postfixChar = postfixChar;
                items[1].width = floor(0.6 * wdth);
                items[1].flags = 0x25;

                items[2].fontID = fontID;
                items[2].spaceChar = spaceChar;
                items[2].prefixChar = prefixChar;
                items[2].txt = str2.c_str();
                items[2].postfixChar = postfixChar;
                items[2].width = floor(0.15 * wdth);
                items[2].flags = 0x24;

                items[3].txt = str3.c_str();
                items[3].width = 4 * tiles->chars[80].width + 6;
                items[3].fontID = 9;

                if ( slct )
                    items[3].spaceChar = spaceChar;
                else
                    items[3].spaceChar = 57;

                items[3].postfixChar = postfixChar;
                items[3].flags = 4;
                items[3].prefixChar = prefixChar;

                items[4].fontID = fontID;
                items[4].txt = " ";
                items[4].spaceChar = spaceChar;
                items[4].prefixChar = prefixChar;
                items[4].postfixChar = postfixChar;
                items[4].width = (wdth - items[0].width - items[1].width - items[2].width - items[3].width);
                items[4].flags = 6;

                itemsCount = 5;
            }
            break;

            case 2:
            case 4:
                items[0].width = usr->network_listvw.entryWidth - 2 * usr->p_ypaworld->font_default_w__b;
                items[0].fontID = fontID;
                items[0].spaceChar = spaceChar;
                items[0].prefixChar = prefixChar;
                items[0].postfixChar = postfixChar;
                items[0].txt = str1.c_str();
                items[0].flags = 0x27;
                itemsCount = 1;
                break;

            default:
                break;
            }

            FontUA::select_tileset(&cmd, 0);
            FontUA::store_u8(&cmd, 123);

            if ( usr->netSel != i || usr->netSelMode == 4 || usr->netSelMode == 2 )
                FontUA::set_txtColor(&cmd, yw->iniColors[61].r, yw->iniColors[61].g, yw->iniColors[61].b );
            else
                FontUA::set_txtColor(&cmd, yw->iniColors[62].r, yw->iniColors[62].g, yw->iniColors[62].b );

            cmd = FontUA::FormateColumnItem(yw, cmd, itemsCount, items);

            FontUA::select_tileset(&cmd, 0);
            FontUA::store_u8(&cmd, 125);
            FontUA::next_line(&cmd);
        }

        if (usr->netSelMode == 3)
        {
            int tmp = i;
            i = cnt;
            cnt = tmp;
        }
    }

    if ( usr->netSelMode == 3)
        i = cnt + 1;

    if (usr->network_listvw.maxShownEntries < i)
        usr->network_listvw.shownEntries = usr->network_listvw.maxShownEntries;
    else
        usr->network_listvw.shownEntries = i;

    if ( usr->network_listvw.maxShownEntries > usr->network_listvw.shownEntries )
        cmd = sub_4DDF78(yw, &usr->network_listvw, cmd, usr->network_listvw.maxShownEntries - usr->network_listvw.shownEntries);

    usr->network_listvw.numEntries = i;

    cmd = usr->network_listvw.ItemsPostLayout(yw, cmd, 0, "xyz");
    FontUA::set_end(&cmd);

    w3d_a209 txt = usr->network_listvw.cmdstrm;
    GFXEngine::GFXe.drawText(&txt);
}

void ypaworld_func158__locale_list_draw(_NC_STACK_ypaworld *yw, UserData *usr)
{
    char *v3 = usr->local_listvw.itemBlock;

    usr->local_listvw.SetRect(yw, -2, -2);

    v3 = usr->local_listvw.ItemsPreLayout(yw, v3, 0, "uvw");

    int v5 = 0;
    for(StringList::iterator it = usr->lang_dlls.begin(); it != usr->lang_dlls.end(); it++)
    {
        if ( v5 >= usr->local_listvw.firstShownEntries && v5 < usr->local_listvw.shownEntries + usr->local_listvw.firstShownEntries)
        {
            if ( v5 == usr->local_listvw.selectedEntry )
                v3 = sub_4C4284(yw, &usr->local_listvw, v3, it->c_str());
            else
                v3 = sub_4C41DC(yw, &usr->local_listvw, v3, it->c_str());
        }

        v5++;
    }

    if ( v5 > 10 )
        usr->local_listvw.shownEntries = 10;
    else
        usr->local_listvw.shownEntries = v5;

    usr->local_listvw.numEntries = usr->local_listvw.shownEntries;

    if ( usr->local_listvw.shownEntries < 10 )
        v3 = sub_4DDF78(yw, &usr->local_listvw, v3, 10 - usr->local_listvw.shownEntries);

    v3 = usr->local_listvw.ItemsPostLayout(yw, v3, 0, "xyz");

    FontUA::set_end(&v3);

    w3d_a209 v13;
    v13 = usr->local_listvw.cmdstrm;

    GFXEngine::GFXe.drawText(&v13);
}

void ypaworld_func158__saveload_list_draw(_NC_STACK_ypaworld *yw, UserData *usr)
{
    char *v4 = usr->disk_listvw.itemBlock;

    usr->disk_listvw.SetRect(yw, -2, -2);

    v4 = usr->disk_listvw.ItemsPreLayout(yw, v4, 0, "uvw");

    int v54 = 0;

    for ( ProfileList::iterator it = usr->profiles.begin(); it != usr->profiles.end(); it++ )
    {
        if ( v54 >= usr->disk_listvw.firstShownEntries && v54 < usr->disk_listvw.shownEntries + usr->disk_listvw.firstShownEntries )
        {
            FontUA::ColumnItem v31[2];
            memset(&v31, 0, sizeof(v31));

            int v8, v9, v45, v44;

            if ( v54 + 1 == usr->field_1612 )
            {
                v8 = 9;
                v9 = 99;
                v45 = 98;
                v44 = 100;
            }
            else
            {
                v8 = 0;
                v9 = 102;
                v45 = 102;
                v44 = 102;
            }

            int v37 = usr->disk_listvw.entryWidth - 2 * usr->p_ypaworld->font_default_w__b;

            char a1a[20];
            sprintf(a1a, "%02d:%02d:%02d", (it->totalElapsedTime / 1000) / 3600, (it->totalElapsedTime / 1000) % 3600 / 60, (it->totalElapsedTime / 1000) % 3600 % 60 );

            v31[0].fontID = v8;
            v31[0].spaceChar = v9;
            v31[0].flags = 37;
            v31[0].width = (v37 * 0.75);
            v31[0].prefixChar = v45;
            v31[0].postfixChar = 0;
            v31[0].txt = it->name.c_str();

            v31[1].fontID = v8;
            v31[1].spaceChar = v9;
            v31[1].txt = a1a;
            v31[1].width = v37 - (v37 * 0.75);
            v31[1].prefixChar = 0;
            v31[1].flags = 38;
            v31[1].postfixChar = v44;

            FontUA::select_tileset(&v4, 0);

            FontUA::store_u8(&v4, 123);

            if ( !strcasecmp(it->name.c_str(), usr->user_name) )
            {
                FontUA::set_txtColor(&v4, yw->iniColors[2].r, yw->iniColors[2].g, yw->iniColors[2].b);
            }
            else
            {
                if ( v54 + 1 == usr->field_1612 )
                {
                    FontUA::set_txtColor(&v4, yw->iniColors[62].r, yw->iniColors[62].g, yw->iniColors[62].b);
                }
                else
                {
                    FontUA::set_txtColor(&v4, yw->iniColors[61].r, yw->iniColors[61].g, yw->iniColors[61].b);
                }
            }

            v4 = FormateColumnItem(yw, v4, 2, v31);

            FontUA::select_tileset(&v4, 0);

            FontUA::store_u8(&v4, 125);

            FontUA::next_line(&v4);
        }

        v54++;
    }

    if ( v54 <= 10 )
    {
        usr->disk_listvw.shownEntries = v54;

        for (int i = 0; i < 10 - v54; i++)
        {
            v4 = sub_4C41DC(yw, &usr->disk_listvw, v4, " ");
        }
    }

    usr->disk_listvw.numEntries = v54;
    v4 = usr->disk_listvw.ItemsPostLayout(yw, v4, 0, "xyz");

    FontUA::set_end(&v4);

    w3d_a209 arg;
    arg = usr->disk_listvw.cmdstrm;

    GFXEngine::GFXe.drawText(&arg);
}

void sb_0x4dee74__sub0(UserData *usr, int x1, int y1, int w, int h)
{
    char buf[300];

    char *tmp = buf;

    FontUA::select_tileset(&tmp, 0);

    FontUA::set_center_xpos(&tmp, x1 - usr->p_ypaworld->screen_width / 2);
    FontUA::set_center_ypos(&tmp, y1 - usr->p_ypaworld->screen_height / 2);

    int v9 = 1;

    int v6 = h;

    tiles_stru *v7 = GFXEngine::GFXe.getTileset(0);

    while ( v6 > v7->font_height )
    {
        if ( v9 )
            FontUA::store_u8(&tmp, 117);
        else
            FontUA::store_u8(&tmp, 123);

        FontUA::op17(&tmp, w);

        if ( v9 )
            FontUA::store_u8(&tmp, 118);
        else
            FontUA::store_u8(&tmp, 123);

        if ( v9 )
            FontUA::store_u8(&tmp, 119);
        else
            FontUA::store_u8(&tmp, 125);

        FontUA::next_line(&tmp);

        v9 = 0;

        v6 -= v7->font_height;
    }

    FontUA::set_yoff(&tmp, v7->font_height - 1);

    FontUA::store_u8(&tmp, 120);

    FontUA::op17(&tmp, w);

    FontUA::store_u8(&tmp, 121);
    FontUA::store_u8(&tmp, 122);

    FontUA::set_end(&tmp);

    w3d_a209 a1a;
    a1a.cmdbuf = buf;
    a1a.includ = NULL;

    GFXEngine::GFXe.drawText(&a1a);
}

void ypaworld_func158__confirm_draw(UserData *usr)
{
    _NC_STACK_ypaworld *yw = usr->p_ypaworld;

    if ( usr->field_0x2fb4 )
    {
        int v4 = yw->screen_height * 0.21875;
        int v6 = yw->screen_width * 0.53125;
        int v7 = yw->screen_height * 0.3854166666666667;
        int v8 = yw->screen_width * 0.234375;

        sb_0x4dee74__sub0(usr, v8, v7, v6, v4);

        usr->confirm_button->button_func70(0);
    }
}

void ypaworld_func158__sub3(_NC_STACK_ypaworld *yw, UserData *usr)
{
    switch ( usr->envMode )
    {
    case ENVMODE_TITLE:
        usr->titel_button->button_func70(0);
        break;

    case ENVMODE_INPUT:
        usr->button_input_button->button_func70(0);
        yw_draw_input_list(yw, usr);
        break;

    case ENVMODE_SETTINGS:
        usr->video_button->button_func70(0);

        if ( usr->video_listvw.IsOpen() )
            ypaworld_func158__video_list_draw(yw, usr);

        if ( usr->d3d_listvw.IsOpen() )
            ypaworld_func158__d3d_list_draw(yw, usr);
        break;

    case ENVMODE_TUTORIAL:
    case ENVMODE_SINGLEPLAY:
        usr->sub_bar_button->button_func70(0);
        break;

    case ENVMODE_NETPLAY:
        usr->network_button->button_func70(0);

        if ( usr->netSelMode != 2 )
            ypaworld_func158__network_list_draw(yw, usr);
        break;

    case ENVMODE_SELLOCALE:
        usr->locale_button->button_func70(0);
        ypaworld_func158__locale_list_draw(yw, usr);
        break;

    case ENVMODE_ABOUT:
        usr->about_button->button_func70(0);
        break;

    case ENVMODE_SELPLAYER:
        usr->disk_button->button_func70(0);
        ypaworld_func158__saveload_list_draw(yw, usr);
        break;

    default:
        break;
    }
    ypaworld_func158__confirm_draw(usr);
}

void yw_freeTileSets(_NC_STACK_ypaworld *yw)
{
    for (int i = 0; i < 92; i++)
    {
        tiles_stru *tile = yw->tiles[i];

        if (tile != NULL)
        {
            if (tile->font_image)
                delete_class_obj(tile->font_image);

            if (tile->chars)
                nc_FreeMem(tile->chars);

            nc_FreeMem(tile);
        }

    }

    memset(yw->tiles, 0, 0x170);
}

void sb_0x44ac24__sub0(_NC_STACK_ypaworld *yw)
{
    if ( yw->win3d )
    {
        displ_arg263 v5;
        v5.bitm = 0;
        v5.pointer_id = 0;
        yw->win3d->display_func263(&v5);
    }

    for (int i = 0; i < 11; i++)
    {
        if ( yw->pointers[i] )
        {
            delete_class_obj(yw->pointers[i]);
            yw->pointers[i] = NULL;
            yw->pointers__bitm[i] = NULL;
        }
    }
}

void sb_0x44ac24(_NC_STACK_ypaworld *yw)
{
    memset(yw->vhcls_models, 0, sizeof(vhclBases) * 512);

    for (int i = 0; i < 256; i++)
    {
        if (yw->legos[i].sklt_obj)
            delete_class_obj(yw->legos[i].sklt_obj);
    }
    memset(yw->legos, 0, sizeof(cityBases) * 256);

    sub_44A908(yw);

    memset(yw->subSectors, 0, sizeof(subSec) * 256);
    memset(yw->secTypes, 0, sizeof(secType) * 256);

    if ( yw->additionalBeeBox )
    {
        delete_class_obj(yw->additionalBeeBox);
        yw->additionalBeeBox = NULL;
    }

    if ( yw->colsub_sklt )
    {
        delete_class_obj(yw->colsub_sklt);
        yw->colsub_sklt = NULL;
        yw->colsub_sklt_intrn = NULL;
    }

    if ( yw->colcomp_sklt )
    {
        delete_class_obj(yw->colcomp_sklt);
        yw->colcomp_sklt = NULL;
        yw->colcomp_sklt_intrn = NULL;
    }

    if ( yw->tracyrmp_ilbm )
    {
        delete_class_obj(yw->tracyrmp_ilbm);
        yw->tracyrmp_ilbm = NULL;
    }

    if ( yw->shadermp_ilbm )
    {
        delete_class_obj(yw->shadermp_ilbm);
        yw->shadermp_ilbm = NULL;
    }

    yw_freeTileSets(yw);

    if ( yw->additionalSet )
    {
        delete_class_obj(yw->additionalSet);
        yw->additionalSet = NULL;
        yw->set_number = 0;
    }

    NC_STACK_display *win3d = GFXEngine::GFXe.getC3D();

    win3d->display_func272(NULL);

    sb_0x44ac24__sub0(yw);
}

void UserData::clear()
{
    field_0x0 = 0;
    field_0x4 = 0;
    field_0x8 = 0;
    field_0xc = 0;
    field_0x10 = 0;
    memset(user_name, 0, sizeof(user_name));

    p_YW = NULL;
    p_ypaworld = NULL;
    field_3A = NULL;
    frameTime = 0;
    glblTime = 0;

    envMode = 0;
    envModeChanged = false;

    returnToTitle = false;

    for(int i = 0; i < 16; i++)
        samples1[i] = NULL;
    for(int i = 0; i < 16; i++)
        samples2[i] = NULL;

    field_ADA = NULL;
    sub_bar_button = NULL;
    field_AE2 = 0;
    titel_button = NULL;
    button_input_button = NULL;

    field_D36 = 0;
    field_D3A = 0;
    inp_joystick = 0;
    field_D42 = 0;
    inp_altjoystick = 0;
    field_D4A = 0;
    field_D4E = 0;
    field_D52 = 0;

    field_D5A = 0;
    field_0xd5c = 0;

    field_D5E = 0;
    video_button = NULL;
    //GuiList video_listvw;
    game_default_res = 0;
    //nlist video_mode_list;
    field_FBE = 0;
    //GuiList d3d_listvw;
    memset(win3d_guid, 0, sizeof(win3d_guid));
    memset(win3d_name, 0, sizeof(win3d_name));

    field_139A = NULL;
    field_139E = NULL;
    fxnumber = 0;
    field_0x13a4 = 0;
    GFX_flags = 0;
    field_0x13a8 = 0;
    field_13AA = 0;
    field_0x13ac = 0;
    snd__flags2 = 0;

    field_0x13b0 = 0;
    snd__volume = 0;
    field_0x13b4 = 0;
    snd__cdvolume = 0;
    field_0x13b8 = 0;
    enemyindicator = 0;
    field_13BE = 0;
    field_13C2 = 0;
    disk_button = NULL;
    //GuiList disk_listvw;
    field_1612 = 0;
    memset(usernamedir, 0, sizeof(usernamedir));
    usernamedir_len = 0;
    field_0x1744 = 0;
    opened_dir = NULL;
    //nlist files_list;
    field_1756 = 0;
    snd__flags1 = 0;
    field_0x1758 = 0;
    field_175A = 0;
    field_0x175c = 0;
    field_175E = 0;
    field_0x1760 = 0;
    locale_button = NULL;
    //nlist lang_dlls;
    //GuiList local_listvw;
    default_lang_dll = NULL;
    prev_lang = NULL;

    field_19C6 = 0;
    field_0x19c8 = 0;
    field_19CA = 0;
    lang_dlls_count = 0;
    about_button = NULL;
    field_19D6 = 0;
    field_19DA = 0;
    field_19DE = 0;
    field_0x19e0 = 0;
    network_button = NULL;
    //GuiList network_listvw;
    field_1C2E = 0;
    field_0x1c30 = 0;
    field_1C32 = 0;
    field_0x1c34 = 0;

    field_1C36 = 0;
    netSelMode = 0;
    netSel = 0;
    nInputMode = 0;
    memset(netName, 0, sizeof(netName));

    netNameCurPos = 0;
    netLevelID = 0;
    netLevelName = NULL;

    memset(callSIGN, 0, sizeof(callSIGN));
    netCRC = 0;
    takTime = 0;
    netPlayerOwner = 0;
    FreeFraction = 0;
    SelectedFraction = 0;
    field_1CD7 = 0;
    isHost = 0;
    modemAskSession = 0;

    msgcount = 0;
    field_1CE8 = 0;
    rdyStart = 0;
    remoteMode = 0;
    disconnected = 0;
    blocked = 0;
    problemCnt = 0;
    msgBuffLine = 0;
    memset(msgBuffers, 0, sizeof(msgBuffers));
    memset(lastSender, 0, sizeof(lastSender));
    //usr_str map_descriptions[256];
    map_descriptions_count = 0;
    noSent = 0;
    sentAQ = 0;
    netProblemOwner = 0;
    memset(netProblemName, 0, sizeof(netProblemName));
    netProblem = 0;
    netAllOk = 0;
    update_time_norm = 0;
    flush_time_norm = 0;
    kickTime = 0;
    latencyCheck = 0;
    netProblemCount = 0;
    netAllOkCount = 0;
    deadCheck = 0;
    sendScore = 0;
    memset(players, 0, sizeof(players));
    memset(players2, 0, sizeof(players2));
    confirm_button = NULL;
    field_0x2fb4 = 0;

    memset(keyConfig, 0, sizeof(keyConfig));

    field_3426 = 0;
    shelltrack = 0;
    missiontrack = 0;
    loadingtrack = 0;
    debriefingtrack = 0;
//     audiotrack_adv shelltrack__adv;
//     audiotrack_adv missiontrack__adv;
//     audiotrack_adv loadingtrack__adv;
//     audiotrack_adv debriefingtrack__adv;
    memset(snaps, 0, sizeof(snaps));
    snap_count = 0;
    cd = 0;
    last_cdchk = 0;
    lastInputEvent = 0;
    WaitForDemo = 0;
    netsend_count = 0;
    netrecv_count = 0;
    netrecv_time = 0;
    netsend_speed = 0;
    netrecv_speed = 0;
    netrecv_bpsMin = 0;
    netrecv_bpsMax = 0;
    netrecv_bpsAvr = 0;
    netsend_bpsMin = 0;
    netsend_bpsMax = 0;
    netsend_bpsAvr = 0;
    net_gcnt = 0;
    net_packet_size = 0;
    net_packet_min = 0;
    net_packet_max = 0;
    net_packet_cnt = 0;
    net_packet_avr = 0;
}
