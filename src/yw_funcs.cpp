#include <inttypes.h>
#include <string.h>

#include "includes.h"
#include "yw.h"
#include "yw_internal.h"
#include "lstvw.h"
#include "font.h"
#include "button.h"
#include "windp.h"

extern GuiList stru_5C91D0;


TileMap * NC_STACK_ypaworld::yw_LoadFont(const std::string &fontname)
{
    FSMgr::FileHandle *fil = uaOpenFile("rsrc:hfonts/" + fontname, "r");
    if ( !fil )
        return NULL;

    TileMap *tileset = new TileMap;
    if ( !tileset )
    {
        delete fil;
        return NULL;
    }

    std::string buf;
    if ( !fil->ReadLine(&buf) )
    {
        ypa_log_out("yw_LoadFont(): font %s, font definition file corrupt.\n", fontname.c_str());
        delete tileset;
        delete fil;
        return NULL;
    }

    Utils::StringSetEnd(&buf, ";\n\r");

    std::string bitmap_name;
    std::string fntHeight;
    Stok tk(buf, " \t");

    if ( !tk.GetNext(&bitmap_name) || !tk.GetNext(&fntHeight) )
    {
        delete tileset;
        ypa_log_out("yw_LoadFont(): font %s, font definition file corrupt.\n", fontname.c_str());
        delete fil;
        return NULL;
    }

    tileset->img = Nucleus::CInit<NC_STACK_ilbm>({
        {NC_STACK_rsrc::RSRC_ATT_NAME, std::string(bitmap_name)},
        {NC_STACK_rsrc::RSRC_ATT_TRYSHARED, (int32_t)0},
        {NC_STACK_bitmap::BMD_ATT_CONVCOLOR, (int32_t)1},
        {NC_STACK_ilbm::ATT_ALPHAPALETTE, (int32_t)0} });
    if ( !tileset->img )
    {
        delete tileset;
        ypa_log_out("yw_LoadFont(): font %s, couldn't load fontpage %s.\n", fontname.c_str(), bitmap_name.c_str());
        delete fil;
        return NULL;
    }

    SDL_SetColorKey(tileset->img->GetSwTex(), SDL_TRUE, SDL_MapRGB(tileset->img->GetSwTex()->format, 255, 255, 0));

    tileset->h = std::stol(fntHeight, NULL, 0);

    while ( fil->ReadLine(&buf) )
    {
        Utils::StringSetEnd(&buf, "\n\r");
        Stok tkc(buf, " \t");

        int chr = -1;          

        if ( buf[0] == ' ' )
            chr = (uint8_t)' ';
        else
        {
            std::string sChar;
            if (tkc.GetNext(&sChar))
            {
                if ( sChar[0] == '#' && sChar.length() > 1 )
                    chr = std::stol(sChar.substr(1), NULL, 0);
                else
                    chr = (uint8_t)sChar[0];
            }
        }

        if ( chr >= 0 )
        {
            std::string sX, sY, sW;
            if ( tkc.GetNext(&sX) && tkc.GetNext(&sY) && tkc.GetNext(&sW) )
            {
                tileset->map[chr].x = std::stol(sX, NULL, 0);
                tileset->map[chr].y = std::stol(sY, NULL, 0);
                tileset->map[chr].w = std::stol(sW, NULL, 0);
                tileset->map[chr].h = tileset->h;
            }
        }
    }

    // All empty to first non-empty
    for ( auto &i : tileset->map)
    {
        if ( !i.IsEmpty() )
        {
            for ( auto &j : tileset->map)
            {
                if ( j.IsEmpty() )
                    j = i;
            }
            break;
        }
    }
    delete fil;
    return tileset;
}


TileMap * NC_STACK_ypaworld::yw_LoadTileSet(const std::string &bitmap, Common::Point chrSz, Common::Point delta, Common::Point cr, Common::Point offset)
{
    TileMap *tileset = new TileMap;

    if ( !tileset )
        return NULL;

    tileset->img = Nucleus::CInit<NC_STACK_ilbm>({
        {NC_STACK_rsrc::RSRC_ATT_NAME, std::string(bitmap)},
        {NC_STACK_bitmap::BMD_ATT_CONVCOLOR, (int32_t)1}, // Speed up painting
        {NC_STACK_ilbm::ATT_ALPHAPALETTE, (int32_t)0} });
    if ( !tileset->img )
    {
        delete tileset;
        return NULL;
    }

    SDL_SetColorKey(tileset->img->GetSwTex(), SDL_TRUE, SDL_MapRGB(tileset->img->GetSwTex()->format, 255, 255, 0));

    tileset->h = chrSz.y;

    int id = 0;
    int y_pos = offset.y;
    for (int j = 0; j < cr.y; j++)
    {
        int x_pos = offset.x;
        
        for (int i = 0; i < cr.x; i++ )
        {
            ResBitmap *bitm = tileset->img->GetResBmp();

            if ( x_pos + chrSz.x > bitm->width )
                x_pos = bitm->width - chrSz.x;

            if ( y_pos + chrSz.y > bitm->height )
                y_pos = bitm->height - chrSz.y;

            tileset->map[id].w = chrSz.x;
            tileset->map[id].h = chrSz.y;
            tileset->map[id].x = x_pos;
            tileset->map[id].y = y_pos;

            id++;
            x_pos += delta.x;
        }
        y_pos += delta.y;
    }
    return tileset;
}

int NC_STACK_ypaworld::load_fonts_and_icons()
{
    const std::array<std::string, 32> fontNames
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
        tiles[i] = yw_LoadFont(fontNames.at(i));
        if ( !tiles[i] )
        {
            ypa_log_out("Could not load font (%s)", fontNames.at(i).c_str());
            return 0;
        }

        GFXEngine::GFXe.setTileset(tiles[i], i);
    }


    tiles[40] = yw_LoadTileSet("lego16.ilbm", Common::Point(16, 16), Common::Point(16, 16), Common::Point(16, 16), Common::Point());
    if ( !tiles[40] )
        return 0;
    GFXEngine::GFXe.setTileset(tiles[40], 40);

    tiles[41] = yw_LoadTileSet("lego8.ilbm", Common::Point(8, 8), Common::Point(8, 8), Common::Point(16, 16), Common::Point());
    if ( !tiles[41] )
        return 0;
    GFXEngine::GFXe.setTileset(tiles[41], 41);

    tiles[42] = yw_LoadTileSet("lego4.ilbm", Common::Point(4, 4), Common::Point(4, 4), Common::Point(16, 16), Common::Point());
    if ( !tiles[42] )
        return 0;
    GFXEngine::GFXe.setTileset(tiles[42], 42);

    tiles[43] = yw_LoadTileSet("sec4.ilbm", Common::Point(4, 4), Common::Point(4, 4), Common::Point(16, 16), Common::Point());
    if ( !tiles[43] )
        return 0;
    GFXEngine::GFXe.setTileset(tiles[43], 43);

    tiles[44] = yw_LoadTileSet("sec8.ilbm", Common::Point(8, 8), Common::Point(8, 8), Common::Point(16, 16), Common::Point());
    if ( !tiles[44] )
        return 0;
    GFXEngine::GFXe.setTileset(tiles[44], 44);
    
    
    
    tiles[50] = yw_LoadTileSet("mapmisc.ilbm", Common::Point(4, 4), Common::Point(4, 0), Common::Point(9, 1), Common::Point(504, 64));
    if ( !tiles[50] )
        return 0;
    GFXEngine::GFXe.setTileset(tiles[50], 50);
    
    tiles[51] = yw_LoadTileSet("mapmisc.ilbm", Common::Point(8, 8), Common::Point(8, 0), Common::Point(9, 1), Common::Point(432, 64));
    if ( !tiles[51] )
        return 0;
    GFXEngine::GFXe.setTileset(tiles[51], 51);
    
    tiles[52] = yw_LoadTileSet("mapmisc.ilbm", Common::Point(16, 16), Common::Point(16, 0), Common::Point(9, 1), Common::Point(288, 64));
    if ( !tiles[52] )
        return 0;
    GFXEngine::GFXe.setTileset(tiles[52], 52);
    
    tiles[53] = yw_LoadTileSet("mapmisc.ilbm", Common::Point(32, 32), Common::Point(32, 0), Common::Point(9, 1), Common::Point(0, 64));
    if ( !tiles[53] )
        return 0;
    GFXEngine::GFXe.setTileset(tiles[53], 53);
    
    tiles[54] = yw_LoadTileSet("mapmisc.ilbm", Common::Point(64, 64), Common::Point(64, 0), Common::Point(9, 1), Common::Point(0, 0));
    if ( !tiles[54] )
        return 0;
    GFXEngine::GFXe.setTileset(tiles[54], 54);

    for (int i = 50; i < 55; i++)
    {
        TileMap *t = tiles[i];

        t->map[9] = t->map[8];
        t->map[8] = t->map[0];
    }
    
    

    tiles[59] = yw_LoadFont("mapvhcl3.font");
    if ( !tiles[59] )
        return 0;
    GFXEngine::GFXe.setTileset(tiles[59], 59);

    tiles[60] = yw_LoadFont("mapvhcl5.font");
    if ( !tiles[60] )
        return 0;
    GFXEngine::GFXe.setTileset(tiles[60], 60);

    tiles[61] = yw_LoadFont("mapvhcl7.font");
    if ( !tiles[61] )
        return 0;
    GFXEngine::GFXe.setTileset(tiles[61], 61);

    tiles[62] = yw_LoadFont("mapvhcl9.font");
    if ( !tiles[62] )
        return 0;
    GFXEngine::GFXe.setTileset(tiles[62], 62);

    font_default_h = tiles[0]->h;
    font_default_w__a = tiles[0]->map[97].w; // a
    font_default_w__b = tiles[0]->map[98].w; // b

    font_yscrl_bkg_w = tiles[12]->map[66].w; // B (Y-Scroller, Background-Inners)
    font_xscrl_h = tiles[11]->h;

    field_1a38 = 2;

    font_yscrl_h = tiles[12]->h;

    icon_order__w = tiles[21]->map[65].w; // A (Mode:      ORDER)
    icon_order__h = tiles[21]->h;

    icon_help__w = tiles[24]->map[65].w; // A (NEU: DER HILFEBUTTTON)
    icon_help__h = tiles[24]->h;

    icon_energy__h = tiles[30]->h; // ENERGIE height
    icon0___h = icon_order__h;
    
    UpdateGuiSettings();

    return 1;
}


int NC_STACK_ypaworld::yw_ParseWorldIni(const std::string &filename)
{
	ScriptParser::HandlersList hndls {
		new World::Parsers::BkgParser(this),
		new World::Parsers::ColorParser(this),
		new World::Parsers::MovieParser(this),
		new World::Parsers::SuperItemParser(this),
		new World::Parsers::MiscParser(this)
	};

	return ScriptParser::ParseFile(filename, hndls, 0);
}

int NC_STACK_ypaworld::yw_InitLevelNet()
{
    _levelInfo = new LevelInfo;
    if ( !_levelInfo )
    {
        if ( LevelNet )
            delete LevelNet;

        if ( _levelInfo )
            delete _levelInfo;

        _levelInfo = NULL;
        LevelNet = NULL;
        return 0;
    }

    LevelNet = new stru_LevelNet;
    if ( !LevelNet )
    {
        if ( LevelNet )
            delete LevelNet;

        if ( _levelInfo )
            delete _levelInfo;

        _levelInfo = NULL;
        LevelNet = NULL;
        return 0;
    }

    for(auto &x : iniColors)
    {
        x.r = 255;
        x.g = 255;
        x.b = 0;
        x.i = 10;
        x.color = 0xFFFF00;
    }

    if ( !yw_ParseWorldIni("env:world.ini") && !yw_ParseWorldIni("data:world.ini"))
    {
        ypa_log_out("yw_ParseWorldIni() failed.\n");
        if ( LevelNet )
            delete LevelNet;

        if ( _levelInfo )
            delete _levelInfo;

        _levelInfo = NULL;
        LevelNet = NULL;
        return 0;
    }

    if ( !yw_ScanLevels() )
    {
        ypa_log_out("yw_ScanLevels() failed.\n");
        if ( LevelNet )
            delete LevelNet;

        if ( _levelInfo )
            delete _levelInfo;

        _levelInfo = NULL;
        LevelNet = NULL;
        return 0;
    }

    for(auto &x : _levelInfo->JodieFoster)
		x = 0;

    return 1;
}

int yw_InitNetwork(NC_STACK_ypaworld *yw)
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



void NC_STACK_ypaworld::setIniColor(int color_id, int r, int g, int b, int i)
{
    iniColors[color_id].r = r;
    iniColors[color_id].g = g;
    iniColors[color_id].b = b;
    iniColors[color_id].i = i;
    iniColors[color_id].color = ((r << 16) & 0xFF0000) | ((g << 8) & 0xFF00) | (b & 0xFF) ;
}

bool NC_STACK_ypaworld::ParseColorString(int color_id, const std::string &color_string)
{
	Stok stok(color_string, "_");
	std::string sr, sg, sb, si;

	if ( stok.GetNext(&sr) && stok.GetNext(&sg) && stok.GetNext(&sb) && stok.GetNext(&si))
    {
        int r = std::stoi(sr);
        int g = std::stoi(sg);
        int b = std::stoi(sb);
        int i = std::stoi(si);
        setIniColor(color_id, r, g, b, i);
        return true;
    }

    return false;
}

int get_level_numb(const std::string &filename)
{
    std::string buf = filename.substr(1);

    size_t pos = buf.find('.');
    if (pos != std::string::npos)
        buf.resize(pos);

    if ( buf.size() == 4 )
        buf.resize(2);
    else if ( buf.size() == 6 )
        buf.resize(3);

    int v10 = std::stoi(buf);
    if ( v10 < 1 || v10 >= 256 )
    {
        v10 = -1;
        ypa_log_out("Invalid level num [valid: 0..127] for %s.\n", filename.c_str());
    }
    return v10;
}





bool NC_STACK_ypaworld::sb_0x4e1a88__sub0__sub0(LevelDesc *mapp, const std::string &fname)
{
    mapp->clear();
    ScriptParser::HandlersList handlers {
        new World::Parsers::LevelDataParser(this, mapp),
        new World::Parsers::MapRobosParser(mapp),
        new World::Parsers::MapSizesParser(mapp)
    };

    return ScriptParser::ParseFile(fname, handlers, ScriptParser::FLAG_NO_INCLUDE);
}



bool NC_STACK_ypaworld::sb_0x4e1a88__sub0(const std::string &fname, bool multiplayer)
{
    int levelnmb = get_level_numb(fname);
    if ( levelnmb == -1 )
        return false;

    std::string buf;
    if ( multiplayer )
        buf = std::string("levels:multi/") + fname;
    else
        buf = std::string("levels:single/") + fname;

    LevelDesc mapp;
    sb_0x4e1a88__sub0__sub0(&mapp, buf);

    mapINFO &minf = LevelNet->mapInfos[levelnmb];

    if ( multiplayer )
        minf.field_0 = 4;
    else
        minf.field_0 = 2;

    minf.mapPath = buf;

    minf.map_name = _levelInfo->MapName;

    minf.field_9C = ua_fRect();

    minf.fractions_mask = 0;
    minf.robos_count = mapp.Robos.size();

    for ( const MapRobo &robo : mapp.Robos )
        minf.fractions_mask |= 1 << robo.Owner;

    minf.secXsize = mapp.MapXSize & 0xFF;
    minf.secYsize = mapp.MapYSize & 0xFF;
    minf.slow_connection = mapp.SlowConnection;
    return true;
}

int NC_STACK_ypaworld::sb_0x4e1a88(bool multiplayer)
{
    int v10 = 1;

    std::string path;

    if ( multiplayer )
        path = "levels:multi/";
    else
        path = "levels:single/";

    FSMgr::DirIter dir = uaOpenDir(path);
    if ( dir )
    {
        FSMgr::iNode *fsnode;
        while ( dir.getNext(&fsnode) )
        {
            if ( strcmp(fsnode->getName().c_str(), ".") && strcmp(fsnode->getName().c_str(), "..") )
            {
                if ( sb_0x4e1a88__sub0(fsnode->getName(), multiplayer) )
                {
                    ypa_log_out("Scanning [%s%s] .. ok.\n", path.c_str(), fsnode->getName().c_str());
                }
                else
                {
                    ypa_log_out("Scanning [%s%s] .. FAILED.\n", path.c_str(), fsnode->getName().c_str());
                    v10 = 0;
                }
            }
        }
    }

    return v10;
}

int NC_STACK_ypaworld::yw_ScanLevels()
{
    int v2 = 1;
    if ( !sb_0x4e1a88(false) )
        v2 = 0;
    if ( !sb_0x4e1a88(true) )
        v2 = 0;
    return v2;
}

int yw_InitTooltips(NC_STACK_ypaworld *yw)
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

NC_STACK_base * sub_44AD8C(const std::string &fname)
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

        std::string line;
        while ( fil->ReadLine(&line) && !line.empty() && line[0] != '>' )
        {
            size_t pos = line.find_first_of(" ;#\n\r");
            if ( pos != std::string::npos )
                line.erase(pos);
            
            if (line.empty())
                continue;

            std::string basName = fmt::sprintf("rsrc:objects/%s", line);

            NC_STACK_base *kid = NC_STACK_base::READ_BAS_FILE(basName);

            if ( !kid )
            {
                ypa_log_out("init: Could not load %s.\n", basName.c_str());
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

int sub_44A12C(NC_STACK_ypaworld *yw, NC_STACK_base *base)
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

int yw_parse_lego(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil, NC_STACK_base *base)
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
    std::string line;
    while ( fil->ReadLine(&line) && !line.empty() && line[0] != '>' )
    {
        cityBases *lego = &yw->legos[id];

        if ( id >= 256 )
        {
            ypa_log_out("Too many legos!\n");
            return 0;
        }

        size_t pos = line.find_first_of(";#\n\r");
        if ( pos != std::string::npos )
            line.erase(pos);

        Stok parse(line, " \t");
        std::string token;
        if ( parse.GetNext(&token) ) // ignore base file name
        {
            if ( !parse.GetNext(&token) )
                return 0;

            NC_STACK_sklt *skelet = Nucleus::CInit<NC_STACK_sklt>({{NC_STACK_rsrc::RSRC_ATT_NAME, token}});
            if ( !skelet )
            {
                ypa_log_out("Couldn't load sklt (%s)\n", token.c_str());
                return 0;
            }

            lego->sklt_obj = skelet;
            lego->sklt_obj_intern = skelet->GetSkelet();

            if ( !parse.GetNext(&token) )
                return 0;

            lego->field_11 = std::stol(token, NULL, 0);

            if ( !parse.GetNext(&token) )
                return 0;

            lego->field_12 = std::stol(token, NULL, 0);

            if ( !parse.GetNext(&token) )
                return 0;

            lego->field_10 = std::stol(token, NULL, 0);

            if ( !parse.GetNext(&token) )
                return 0;

            switch( std::stol(token, NULL, 0) )
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
                if ( !parse.GetNext(&token) || fxnumb >= yw->fxnumber )
                    break;

                float x = 0.0;
                float y = 0.0;
                float z = 0.0;

                lego->field_14[fxnumb] = std::stol(token, NULL, 0);

                if ( parse.GetNext(&token) )
                    x = std::stof(token, 0);

                if ( parse.GetNext(&token) )
                    z = std::stof(token, 0);

                if ( parse.GetNext(&token) )
                    y = std::stof(token, 0);

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

int yw_parse_subSect(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    int id = 0;
    std::string line;
    while ( fil->ReadLine(&line) && !line.empty() && line[0] != '>' )
    {
        if ( id >= 256 )
        {
            ypa_log_out("Too many sub sector defs!\n");
            return 0;
        }

        size_t pos = line.find_first_of(";#\n\r");
        if ( pos != std::string::npos )
            line.erase(pos);

        Stok parse(line, " \t");
        std::string token;
        if ( parse.GetNext(&token) )
        {
            subSec *ssec = &yw->subSectors[id];

            ssec->health_models[0] = std::stol(token, NULL, 0);

            if ( !parse.GetNext(&token) )
                return 0;

            ssec->health_models[1] = std::stol(token, NULL, 0);

            if ( !parse.GetNext(&token) )
                return 0;

            ssec->health_models[2] = std::stol(token, NULL, 0);

            if ( !parse.GetNext(&token) )
                return 0;

            ssec->health_models[3] = std::stol(token, NULL, 0);

            if ( !parse.GetNext(&token) )
                return 0;

            if ( token.at(0) > '0' && token.at(0) == '1' )
                ssec->build_health = 255;
            else
                ssec->build_health = 0;

            if ( parse.GetNext(&token) )
                ssec->field_8 = std::stol(token, NULL, 0);

            id++;
        }
    }
    return 1;
}

int yw_parse_sektor(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil)
{
    std::string line;
    while ( fil->ReadLine(&line) && !line.empty() && line[0] != '>' )
    {
        std::string tmp = line;
        
        size_t pos = tmp.find_first_of(";#\n\r");
        if ( pos != std::string::npos )
            tmp.erase(pos);

        Stok parse(tmp, " \t");
        std::string token;
        if ( parse.GetNext(&token) )
        {
            secType *sektp = &yw->secTypes[ std::stol(token, NULL, 0) ];

            if ( !parse.GetNext(&token) )
            {
                ypa_log_out("Error reading '%s', line '%s'.\n", "set.sdf", line.c_str());
                return 0;
            }

            sektp->field_0 = std::stol(token, NULL, 0);

            if ( !parse.GetNext(&token) )
            {
                ypa_log_out("Error reading '%s', line '%s'.\n", "set.sdf", line.c_str());
                return 0;
            }

            sektp->field_1 = std::stol(token, NULL, 0);

            if ( !parse.GetNext(&token) )
            {
                ypa_log_out("Error reading '%s', line '%s'.\n", "set.sdf", line.c_str());
                return 0;
            }

            sektp->field_3 = std::stol(token, NULL, 0);

            memset(sektp->buildings, 0, sizeof(sektp->buildings));

            if ( sektp->field_0 == 1 )
            {
                if ( !parse.GetNext(&token) )
                {
                    ypa_log_out("Error reading '%s', line '%s'.\n", "set.sdf", line.c_str());
                    return 0;
                }
                sektp->buildings[0][0] = &yw->subSectors[ std::stol(token, NULL, 0) ];
            }
            else
            {
                for (int j = 0; j < 3; j++)
                {
                    for (int i = 0; i < 3; i++)
                    {
                        if ( !parse.GetNext(&token) )
                        {
                            ypa_log_out("Error reading '%s', line '%s'.\n", "set.sdf", line.c_str());
                            return 0;
                        }

                        sektp->buildings[i][2 - j] = &yw->subSectors[ std::stol(token, NULL, 0) ];
                    }
                }
            }
        }
    }
    return 1;
}

int sub_44A97C(NC_STACK_ypaworld *yw, NC_STACK_base *base)
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

                UAskeleton::Data *skeleton_internal = skeleton->GetSkelet();
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

    std::string oldRsrc = get_prefix_replacement("rsrc");

    set_prefix_replacement("rsrc", "data:mc2res");

    NC_STACK_sklt *tmp_skel = Nucleus::CInit<NC_STACK_sklt>( {{NC_STACK_rsrc::RSRC_ATT_NAME, std::string("Skeleton/ColSide.sklt")}} );
    if ( !tmp_skel )
    {
        ypa_log_out("Couldn't create side collision sklt.\n");
        return 0;
    }

    yw->ColSide.skeleton = tmp_skel;
    yw->ColSide.skeleton_internal = yw->ColSide.skeleton->GetSkelet();

    tmp_skel = Nucleus::CInit<NC_STACK_sklt>( {{NC_STACK_rsrc::RSRC_ATT_NAME, std::string("Skeleton/ColCross.sklt")}} );
    if ( !tmp_skel )
    {
        ypa_log_out("Couldn't create cross collision sklt.\n");
        return 0;
    }

    yw->ColCross.skeleton = tmp_skel;
    yw->ColCross.skeleton_internal = yw->ColCross.skeleton->GetSkelet();

    set_prefix_replacement("rsrc", oldRsrc);

    return 1;
}

int yw_InitMouseStuff(NC_STACK_ypaworld *yw)
{
    const std::array<std::string, 11> pointerFiles
    {
        "pointers/pointer.ilbm",
        "pointers/cancel.ilbm",
        "pointers/select.ilbm",
        "pointers/attack.ilbm",
        "pointers/goto.ilbm",
        "pointers/disk.ilbm",
        "pointers/new.ilbm",
        "pointers/add.ilbm",
        "pointers/control.ilbm",
        "pointers/beam.ilbm",
        "pointers/build.ilbm"
    };
    
    std::string oldRsrc = get_prefix_replacement("rsrc");
    set_prefix_replacement("rsrc", "data:mc2res");

    for (int i = 0; i < 11; i++)
    {
        yw->pointers[i] = Nucleus::CInit<NC_STACK_ilbm>({
            {NC_STACK_rsrc::RSRC_ATT_NAME, pointerFiles.at(i)},
            {NC_STACK_bitmap::BMD_ATT_CONVCOLOR, (int32_t)1}});
        if ( !yw->pointers[i] )
        {
            ypa_log_out("yw_select.c/yw_InitMouseStuff()\n");
            set_prefix_replacement("rsrc", oldRsrc);
            return 0;
        }

        yw->pointers__bitm[i] = yw->pointers[i]->GetResBmp();
    }

    displ_arg263 arg_263;

    arg_263.bitm = 0;
    arg_263.pointer_id = 0;
    yw->_win3d->display_func263(&arg_263);

    arg_263.bitm = yw->pointers__bitm[0];
    arg_263.pointer_id = 1;
    yw->_win3d->display_func263(&arg_263);

    set_prefix_replacement("rsrc", oldRsrc);
    return 1;
}

int NC_STACK_ypaworld::yw_LoadSet(int setID)
{
    _win3d = GFXEngine::GFXe.getC3D();
    _mouseGrabbed = 0;

    std::string oldRsrc = get_prefix_replacement("rsrc");
    std::string newRsrc = fmt::sprintf("data:set%d", setID);

    _win3d->display_func271(NULL);


    set_prefix_replacement("rsrc", "data:mc2res");

    colsub_sklt = Nucleus::CInit<NC_STACK_sklt>({{NC_STACK_rsrc::RSRC_ATT_NAME, std::string("skeleton/colsub.sklt")}});
    if ( !colsub_sklt )
    {
        ypa_log_out("Couldn't load <skeleton/colsub.sklt>, set %d.\n", setID);
        return 0;
    }
    colsub_sklt_intrn = colsub_sklt->GetSkelet();

    colcomp_sklt = Nucleus::CInit<NC_STACK_sklt>({{NC_STACK_rsrc::RSRC_ATT_NAME, std::string("skeleton/colcomp.sklt")}});
    if ( !colcomp_sklt )
    {
        ypa_log_out("Couldn't load <skeleton/colcomp.sklt>, set %d.\n", setID);
        return 0;
    }
    colcomp_sklt_intrn = colcomp_sklt->GetSkelet();


    set_prefix_replacement("rsrc", newRsrc);

    if ( !GFXEngine::GFXe.loadPal("palette/standard.pal") )
        ypa_log_out("WARNING: Could not load set default palette!\n");

    if ( setID != set_number && setID != 46 )
    {
        if ( additionalSet )
        {
            ypa_log_out("yw_LoadSet(): killing set object %d\n", set_number);
            delete_class_obj(additionalSet);
            additionalSet = NULL;
            set_number = 0;
        }

        additionalSet = load_set_base();
        if ( !additionalSet )
        {
            set_number = 0;
            ypa_log_out("yw_LoadSet(): loading set object %d failed\n", setID);
            set_prefix_replacement("rsrc", oldRsrc);
            return 0;
        }
        set_number = setID;
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

        BaseList &kid_list = additionalSet->getBASE_kidList();

        int kid_id = 0;

        for(BaseList::iterator it = kid_list.begin(); it != kid_list.end(); it++)
        {
            if ( kid_id == 0 )
            {
                if ( !sub_44A12C(this, *it) )
                {
                    delete fil;
                    return 0;
                }
            }
            else if ( kid_id == 1 )
            {
                if ( !yw_parse_lego(this, fil, *it) )
                {
                    delete fil;
                    return 0;
                }

                if ( !yw_parse_subSect(this, fil) )
                {
                    delete fil;
                    return 0;
                }

                if ( !yw_parse_sektor(this, fil) )
                {
                    delete fil;
                    return 0;
                }
            }
            else if ( kid_id == 2 )
            {
                if ( !sub_44A97C(this, *it) )
                {
                    delete fil;
                    return 0;
                }
            }

            kid_id++;
        }

        delete fil;
    }

    tracyrmp_ilbm = Nucleus::CInit<NC_STACK_ilbm>( {{NC_STACK_rsrc::RSRC_ATT_NAME, std::string("remap/tracyrmp.ilbm")}} );
    if ( !tracyrmp_ilbm )
    {
        ypa_log_out("Couldn't load tracy remap table, set %d.\n", setID);
        return 0;
    }

    shadermp_ilbm = Nucleus::CInit<NC_STACK_ilbm>( {{NC_STACK_rsrc::RSRC_ATT_NAME, std::string("remap/shadermp.ilbm")}} );
    if ( !shadermp_ilbm )
    {
        ypa_log_out("Couldn't load shade remap table, set %d.\n", setID);
        return 0;
    }

    GFXEngine::GFXe.setTracyRmp( tracyrmp_ilbm->GetResBmp() );
    GFXEngine::GFXe.setShadeRmp( shadermp_ilbm->GetResBmp() );

    additionalBeeBox = NC_STACK_base::READ_BAS_FILE("rsrc:objects/beebox.base");
    if ( !additionalBeeBox )
    {
        ypa_log_out("Couldn't load bbox object, set %d.\n", setID);
        return 0;
    }

    additionalBeeBox->setBASE_static(1);

    if ( setID == 46 || setID == 42 )
    {
        if ( load_fonts_and_icons() == 0 )
            return 0;
    }

    yw_InitMouseStuff(this);
    return 1;
}


int loadTOD(NC_STACK_ypaworld *yw, const char *fname)
{
    int tod = 0;
    if ( yw->GameShell )
    {
        FSMgr::FileHandle *fil = uaOpenFile( fmt::sprintf("save:%s/%s", yw->GameShell->user_name, fname), "r");

        if ( fil )
        {
            std::string lnbuf;
            if ( fil->ReadLine(&lnbuf) )
            {
                size_t pos = lnbuf.find_first_of("\n\r;");
                if (pos != std::string::npos)
                    lnbuf.erase(pos);

                tod = std::stol(lnbuf, 0, 0);
            }
            delete fil;
        }
    }
    return tod;
}

int writeTOD(NC_STACK_ypaworld *yw, const char *fname, int tod)
{
    int v6 = 0;
    if ( yw->GameShell )
    {
        FSMgr::FileHandle *fil = uaOpenFile( fmt::sprintf("save:%s/%s", yw->GameShell->user_name, fname), "w");

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
    rstr_arg204 a4;
    a4.pbitm = bitm->GetResBmp();

    a4.float14 = -1.0;
    a4.float4 = -1.0;
    a4.float18 = -1.0;
    a4.float8 = -1.0;
    a4.float1C = 1.0;
    a4.floatC = 1.0;
    a4.float20 = 1.0;
    a4.float10 = 1.0;

    _win3d->raster_func202(&a4);
}

void NC_STACK_ypaworld::sub_4491A0(const std::string &movie_fname)
{
    std::string buf;
    sub_412810(movie_fname, buf);

    const char *v6;
    v6 = buf.c_str();

    _win3d->windd_func323(&v6);

    INPe.QueryInput(&input_states);

    input_states.KbdLastHit = Input::KC_NONE;
    input_states.KbdLastDown = Input::KC_NONE;
    input_states.HotKeyID = -1;
}

void sub_44A908(NC_STACK_ypaworld *yw)
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

void sb_0x4ea37c__sub1(NC_STACK_ypaworld *yw)
{
    for (int i = 0 ; i < 256; i++)
    {
        if (yw->legos[i].sklt_obj)
            delete_class_obj(yw->legos[i].sklt_obj);
    }

    memset(yw->legos, 0, sizeof(cityBases) * 256);
}

void sb_0x4ea37c(NC_STACK_ypaworld *yw)
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

void sub_4EAC80(NC_STACK_ypaworld *yw)
{
    BriefengScreen *brf = &yw->brief;

    sb_0x4ea37c(yw);

    if ( yw->typ_map )
    {
        delete yw->typ_map;
        yw->typ_map = NULL;
    }

    if ( brf->MbmapImg )
    {
        delete brf->MbmapImg;
        brf->MbmapImg = NULL;
    }

    if ( brf->BriefingMapImg )
    {
        delete brf->BriefingMapImg;
        brf->BriefingMapImg = NULL;
    }

    brf->Stage = 0;
    yw->GuiWinClose(&stru_5C91D0);
    stru_5C91D0.Free();
}

void yw_freeDebrief(NC_STACK_ypaworld *yw)
{
    BriefengScreen *brf = &yw->brief;

    brf->OwnMap.Clear();

    brf->TypMap.Clear();

    if ( brf->MbmapImg )
    {
        delete_class_obj(brf->MbmapImg);
        brf->MbmapImg = 0;
    }

    if ( brf->BriefingMapImg )
    {
        delete_class_obj(brf->BriefingMapImg);
        brf->BriefingMapImg = NULL;
    }

    for ( NC_STACK_sklt* &gfx : brf->VectorGfx )
    {
        if (gfx)
        {
            Nucleus::Delete(gfx);
            gfx = NULL;
        }
    }
    brf->Stage = 0;

    yw->_levelInfo->State = 8;
}

// Select map
void ypaworld_func158__sub4__sub1__sub0(NC_STACK_ypaworld *yw, InputState *inpt)
{
    float v3 = (float)inpt->ClickInf.move.ScreenPos.x / (float)yw->screen_width;
    float v4 = (float)inpt->ClickInf.move.ScreenPos.y / (float)yw->screen_height;

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
            ResBitmap *a4 = yw->LevelNet->ilbm_mask_map->GetResBmp();

            int xpos = a4->width * v3;
            int ypos = a4->height * v4;

            SDL_LockSurface(a4->swTex);
            v7 = ((uint8_t *)a4->swTex->pixels)[a4->swTex->pitch * ypos + xpos];
            SDL_UnlockSurface(a4->swTex);
            
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


void splashScreen_OutText(NC_STACK_ypaworld *yw, NC_STACK_display *w3d, const std::string &txt, int x, int y)
{
    char cmdbuf[2048];

    char *cur = cmdbuf;

    if ( !txt.empty() )
    {
        FontUA::select_tileset(&cur, 15);
        FontUA::set_xpos(&cur, x);
        FontUA::set_ypos(&cur, y);

        FontUA::set_txtColor(&cur, yw->iniColors[13].r, yw->iniColors[13].g, yw->iniColors[13].b);

        Stok parse(txt, "\n\r");
        std::string line;
        while( parse.GetNext( &line ) )
        {
            cur = FontUA::FormateClippedText(yw->tiles[15], cur, line, yw->screen_width - x, ' ');

            FontUA::next_line(&cur);
        }

        FontUA::set_end(&cur);

        w3d_a209 v15;
        v15.cmdbuf = cmdbuf;
        v15.includ = NULL;

        yw->_win3d->raster_func209(&v15);
    }
}

void ypaworld_func158__sub4__sub1__sub2(NC_STACK_ypaworld *yw)
{
    stru_LevelNet *lvlnet = yw->LevelNet;

    if ( lvlnet->ilbm_menu_map && lvlnet->ilbm_mask_map && lvlnet->ilbm_rollover_map )
    {
        rstr_arg204 a4;
        a4.pbitm = yw->LevelNet->ilbm_menu_map->GetResBmp();

        a4.float4 = -1.0;
        a4.float8 = -1.0;
        a4.floatC = 1.0;
        a4.float10 = 1.0;

        a4.float14 = -1.0;
        a4.float18 = -1.0;
        a4.float1C = 1.0;
        a4.float20 = 1.0;

        yw->_win3d->raster_func202(&a4);

        for (int i = 0; i < 256; i++)
        {
            mapINFO *v5 = &yw->LevelNet->mapInfos[i];

            if ( v5->field_9C.x1 != v5->field_9C.x2 )
            {
                ResBitmap *v20 = NULL;

                if ( v5->field_0 == 2 ||  v5->field_0 == 3)
                {
                    if ( i == yw->LevelNet->field_BE38 )
                    {
                        v20 = yw->LevelNet->ilbm_rollover_map->GetResBmp();
                    }
                }

                if ( v20 )
                {
                    rstr_218_arg v17;
                    v17.bitm_intern = v20;
                    v17.bitm_intern2 = yw->LevelNet->ilbm_mask_map->GetResBmp();

                    v17.flg = i;
                    v17.rect2.x1 = v5->field_9C.x1;
                    v17.rect1.x1 = v17.rect2.x1;
                    v17.rect2.y1 = v5->field_9C.y1;
                    v17.rect1.y1 = v17.rect2.y1;
                    v17.rect2.x2 = v5->field_9C.x2;
                    v17.rect1.x2 = v17.rect2.x2;
                    v17.rect2.y2 = v5->field_9C.y2;
                    v17.rect1.y2 = v17.rect2.y2;

                    yw->_win3d->raster_func218(&v17);
                }
            }
        }

        if ( yw->LevelNet->field_BE38 )
        {
            char cmdBuff[256];
            char *v11 = cmdBuff;

            if ( yw->LevelNet->mapInfos[ yw->LevelNet->field_BE38 ].field_0 )
            {
                if ( !yw->LevelNet->mapInfos[ yw->LevelNet->field_BE38 ].map_name.empty() )
                {
                    const char *v12 = get_lang_string(yw->string_pointers_p2, yw->LevelNet->field_BE38 + 1800, yw->LevelNet->mapInfos[ yw->LevelNet->field_BE38 ].map_name.c_str());

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

            yw->_win3d->raster_func209(&v19);
        }
    }

}

void ypaworld_func158__sub4__sub1__sub1(NC_STACK_ypaworld *yw)
{
    stru_LevelNet *lvlnet = yw->LevelNet;

    if ( lvlnet->ilbm_menu_map && lvlnet->ilbm_mask_map && lvlnet->ilbm_rollover_map && lvlnet->ilbm_finished_map && lvlnet->ilbm_enabled_map )
    {
        rstr_arg204 a4;
        a4.pbitm = yw->LevelNet->ilbm_menu_map->GetResBmp();

        a4.float4 = -1.0;
        a4.float8 = -1.0;
        a4.floatC = 1.0;
        a4.float10 = 1.0;

        a4.float14 = -1.0;
        a4.float18 = -1.0;
        a4.float1C = 1.0;
        a4.float20 = 1.0;

        yw->_win3d->raster_func202(&a4);
        //printf("field_BE38 %d \n",yw->LevelNet->field_BE38);

        for (int i = 0; i < 256; i++)
        {
            mapINFO *v5 = &yw->LevelNet->mapInfos[i];

            if ( v5->field_9C.x1 != v5->field_9C.x2 )
            {
                ResBitmap *v20 = NULL;

                if ( v5->field_0 == 2 )
                {
                    if ( i == yw->LevelNet->field_BE38 )
                    {
                        v20 = yw->LevelNet->ilbm_rollover_map->GetResBmp();
                    }
                    else
                    {
                        v20 = yw->LevelNet->ilbm_enabled_map->GetResBmp();
                    }
                }
                else if ( v5->field_0 == 3 )
                {
                    v20 = yw->LevelNet->ilbm_finished_map->GetResBmp();
                }

                if ( v20 )
                {
                    rstr_218_arg v17;
                    v17.bitm_intern = v20;
                    v17.bitm_intern2 = yw->LevelNet->ilbm_mask_map->GetResBmp();

                    v17.flg = i;
                    v17.rect2.x1 = v5->field_9C.x1;
                    v17.rect1.x1 = v17.rect2.x1;
                    v17.rect2.y1 = v5->field_9C.y1;
                    v17.rect1.y1 = v17.rect2.y1;
                    v17.rect2.x2 = v5->field_9C.x2;
                    v17.rect1.x2 = v17.rect2.x2;
                    v17.rect2.y2 = v5->field_9C.y2;
                    v17.rect1.y2 = v17.rect2.y2;

                    yw->_win3d->raster_func218(&v17);
                }
            }
        }

        if ( yw->LevelNet->field_BE38 )
        {
            char cmdBuff[256];
            char *v11 = cmdBuff;

            if ( yw->LevelNet->mapInfos[ yw->LevelNet->field_BE38 ].field_0 )
            {
                if ( !yw->LevelNet->mapInfos[ yw->LevelNet->field_BE38 ].map_name.empty() )
                {
                    const char *v12 = get_lang_string(yw->string_pointers_p2, yw->LevelNet->field_BE38 + 1800, yw->LevelNet->mapInfos[ yw->LevelNet->field_BE38 ].map_name.c_str());

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

            yw->_win3d->raster_func209(&v19);
        }
        const char *v13 = get_lang_string(yw->string_pointers_p2, yw->TOD_ID + 2490, " ");
        splashScreen_OutText(yw, yw->_win3d, v13, yw->screen_width / 20, yw->screen_width / 20);
    }
}


int NC_STACK_ypaworld::ypaworld_func158__sub4__sub1__sub3__sub0()
{
    int lft = screen_width * 0.575;
    int rght = screen_width * 0.9734375;
    int top = screen_height * 0.08958333333333329;
    int bottom = screen_height * 0.9083333333333334;

    int v5 = (bottom - top) / font_default_h;

    memset(&stru_5C91D0, 0, sizeof(GuiList));

    GuiList::tInit args;
    args.numEntries = 1;
    args.shownEntries = v5;
    args.minShownEntries = v5;
    args.maxShownEntries = v5;
    args.withIcon = false;
    args.entryHeight = font_default_h;
    args.entryWidth = (rght - lft);
    args.enabled = true;
    args.vborder = field_1a38;

    int result = stru_5C91D0.Init(this, args);

    if ( !result )
        return 0;

    stru_5C91D0.SetRect(this, lft, top);
    GuiWinOpen( &stru_5C91D0 );

    return 1;
}

int NC_STACK_ypaworld::ypaworld_func158__sub4__sub1__sub3(int lvlid)
{
    brief.clear();

    brief.SelectedObjID = -1;

    SFXEngine::SFXe.StopMusicTrack();

    if ( !ypaworld_func158__sub4__sub1__sub3__sub0() )
        return 0; // May be HACK

    _win3d->display_func271(NULL);
    _win3d->raster_func192(NULL);

    LevelInfo *backup = new LevelInfo();
    *backup = *_levelInfo;

    brief.Stage = 0;
    brief.TimerStatus = 0;
    brief.CurrTime = 0;
    brief.TextTime = 0;

    _levelInfo->LevelID = lvlid;
    _levelInfo->State = 5;

    _Gems.clear();

    _levelInfo->Gates.clear();

    set_prefix_replacement("rsrc", "data:");

    LevelDesc *mproto = &brief.Desc;

    if ( sub_4DA41C(mproto, LevelNet->mapInfos[ lvlid ].mapPath) )
    {
        if ( (brief.Desc.Flags & 0x7F) == 127 )
        {
            brief.BriefingText = get_lang_string(string_pointers_p2, _levelInfo->LevelID + 2100, "<NO INFO AVAILABLE>");

            if ( !mproto->Mbmaps.empty() )
            {
                if ( !_levelInfo->MovieStr[0] || brief.Stage )
                {
                    for ( const MapRobo &robo : mproto->Robos)
                    {
                        if ( !_levelInfo->JodieFoster[ robo.Owner ] )
                        {
                            int v19;

                            switch ( robo.Owner )
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

                            if ( v19 != -1 && !movies[v19].empty() )
                            {
                                brief.MovieStr = movies[v19];

                                field_81AB = robo.Owner;
                                brief.Stage = 28;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    brief.MovieStr = _levelInfo->MovieStr;
                    brief.Stage = 28;
                }

                set_prefix_replacement("rsrc", "levels:");

                if ( LevelNet->brief_map[0].map_name[0] )
                    brief.BriefingMapImg = Nucleus::CInit<NC_STACK_ilbm>( {
                        {NC_STACK_rsrc::RSRC_ATT_NAME, LevelNet->brief_map[0].map_name},
                        {NC_STACK_bitmap::BMD_ATT_CONVCOLOR, (int32_t)1}} );

                set_prefix_replacement("rsrc", "mbpix:");

                brief.MbmapImg = Nucleus::CInit<NC_STACK_ilbm>( {
                    {NC_STACK_rsrc::RSRC_ATT_NAME, mproto->Mbmaps[0].Name},
                    {NC_STACK_bitmap::BMD_ATT_CONVCOLOR, (int32_t)1}} );

                if ( brief.MbmapImg )
                {
                    if ( !typ_map )
                        typ_map = new Common::PlaneBytes;

                    if ( typ_map )
                    {
                        _mapWidth = typ_map->Width();
                        _mapHeight = typ_map->Height();

                        map_Width_meters = _mapWidth * 1200.0;
                        map_Height_meters = _mapHeight * 1200.0;

                        //Set transitions
                        brief.ViewingObjectRect = ua_fRect(-0.98750001, 0.34999999, -0.003125, 0.85416669);

                        if ( !brief.Stage )
                            brief.Stage = 4;

                        sb_0x4ea37c(this);
                        return 1;
                    }

                    delete_class_obj(brief.MbmapImg);
                    brief.MbmapImg = NULL;
                }
            }
        }
    }

    GuiWinClose( &stru_5C91D0 );
    stru_5C91D0.Free();

    *_levelInfo = *backup;
    delete backup;

    return 0;
}

int NC_STACK_ypaworld::ypaworld_func158__sub4__sub1__sub5__sub0(LevelDesc *mapproto, const std::string &filename)
{
    mapproto->clear();

    ScriptParser::HandlersList parsers
    {
        new World::Parsers::LevelDataParser(this, mapproto),
        new World::Parsers::LevelGatesParser(this),
        new World::Parsers::LevelMbMapParser(mapproto),
        new World::Parsers::LevelMapsParser(this, mapproto),
        new World::Parsers::LevelDebMapParser(mapproto),
        new World::Parsers::LevelSuperItemsParser(this),
    };

    return ScriptParser::ParseFile(filename, parsers, ScriptParser::FLAG_NO_INCLUDE);
}

bool NC_STACK_ypaworld::InitDebrief()
{
    brief.clear();

    if ( !copyof_ownermap || !copyof_typemap )
    {
        yw_freeDebrief(this);
        return false;
    }

    brief.ZoomFromGate = _levelInfo->State == 1;

    ua_fRect v17;
    v17.x1 = -1.0;
    v17.y1 = -1.0;
    v17.x2 = 1.0;
    v17.y2 = 1.0;

    _win3d->raster_func210(&v17);

    brief.Stage = 0;
    brief.TimerStatus = 0;
    brief.CurrTime = 0;
    //brief._owner = playerOwner;

    _levelInfo->State = 9;

    set_prefix_replacement("rsrc", "data:");

    const std::array<std::string, 4> vGfxName
    {
        "wireless/db_genes.sklt",
        "wireless/db_death.sklt",
        "wireless/db_death.sklt",
        "wireless/db_sec.sklt"
    };

    for (size_t i = 0; i < vGfxName.size(); i++)
        brief.VectorGfx[i] = Nucleus::CInit<NC_STACK_sklt>({{NC_STACK_rsrc::RSRC_ATT_NAME, vGfxName[i]}});

    if ( copyof_ownermap )
        brief.OwnMap = *copyof_ownermap;

    if ( copyof_typemap )
        brief.TypMap = *copyof_typemap;

    if ( !ypaworld_func158__sub4__sub1__sub5__sub0(&brief.Desc, LevelNet->mapInfos[_levelInfo->LevelID].mapPath) )
    {
        yw_freeDebrief(this);
        return false;
    }

    dbmapProto &v8 = !brief.Desc.Dbmaps.empty() ? brief.Desc.Dbmaps[0] : brief.Desc.Mbmaps[0];

    if ( v8.Name.empty() )
    {
        yw_freeDebrief(this);
        return false;
    }

    set_prefix_replacement("rsrc", "levels:");

    if ( LevelNet->debrief_map[0].map_name[0] )
        brief.BriefingMapImg = Nucleus::CInit<NC_STACK_ilbm>( {
            {NC_STACK_rsrc::RSRC_ATT_NAME, LevelNet->debrief_map[0].map_name},
            {NC_STACK_bitmap::BMD_ATT_CONVCOLOR, (int32_t)1}});

    set_prefix_replacement("rsrc", "mbpix:");

    brief.MbmapImg = Nucleus::CInit<NC_STACK_ilbm>( {
        {NC_STACK_rsrc::RSRC_ATT_NAME, v8.Name},
        {NC_STACK_bitmap::BMD_ATT_CONVCOLOR, (int32_t)1}});
    if ( !brief.MbmapImg )
    {
        yw_freeDebrief(this);
        return false;
    }

    _mapWidth = typ_map->Width();
    _mapHeight = typ_map->Height();
    map_Width_meters = _mapWidth * 1200.0;
    map_Height_meters = _mapHeight * 1200.0;

    brief.Stage = 4;

    return true;
}


void yw_calcPlayerScore(NC_STACK_ypaworld *yw)
{
    if ( !yw->field_727c )
    {
        int last_time = 0;

        //yw_f726c_nod *hnode = (yw_f726c_nod *)yw->history->lst.head;
        auto reader = yw->_history.GetReader();
        
        bool run = true;
        //int tlen = 0;
        //uint8_t *bf = hnode->bufStart;
        
        World::History::Instance HistDecoders;

        while (run && !reader.Eof())
        {
            World::History::Record *decoder = HistDecoders[ reader.ReadU8() ];
            if (decoder)
            {
                World::History::FillDecoderBStrm(decoder, &reader);
                
                switch(decoder->type)
                {
                case World::History::TYPE_FRAME:
                {
                    World::History::Frame *frm = static_cast<World::History::Frame *>(decoder);
                    last_time = frm->TimeStamp;
                }
                break;

                case World::History::TYPE_CONQ:
                case World::History::TYPE_VHCLKILL:
                case World::History::TYPE_VHCLCREATE:
                case World::History::TYPE_POWERST:
                case World::History::TYPE_UPGRADE:
                    decoder->AddScore(&yw->playerstatus);
                    break;

                default:
                    run = 0;
                    break;
                }
            }
            else
            {
                run = 0;
            }
        }

        for (int i = 0; i < 8; i++)
            yw->playerstatus[i].elapsedTime += last_time;

    }
}

void NC_STACK_ypaworld::ypaworld_func158__sub4__sub1()
{
    if ( _levelInfo->State != 1 && _levelInfo->State != 2 )
    {
        if ( brief.Stage )
        {
            if ( _levelInfo->State == 5 )
            {
                if ( GameShell->_input->KbdLastHit == Input::KC_RETURN )
                    brief.Stage = 1;

                if ( brief.Stage == 1 )
                {
                    GameShell->envAction.action = EnvAction::ACTION_PLAY;
                    GameShell->envAction.params[0] = _levelInfo->LevelID;
                    GameShell->envAction.params[1] = _levelInfo->LevelID;
                    sub_4EAC80(this);
                }
                else if ( brief.Stage == 2 )
                {
                    sub_4EAC80(this);
                    _levelInfo->State = 8;
                }
                else
                {
                    ypaworld_func158__sub4__sub1__sub4(this, GameShell, GameShell->_input);
                }
            }
            else if ( _levelInfo->State == 9 )
            {
                if ( brief.Stage == 2 )
                {
                    yw_calcPlayerScore(this);
                    yw_freeDebrief(this);
                }
                else
                {
                    yw_debriefUpdate(this, GameShell->_input);
                }
            }
        }
        else
        {
            ypaworld_func158__sub4__sub1__sub0(this, GameShell->_input);

            if ( GameShell->envMode == ENVMODE_TUTORIAL )
                ypaworld_func158__sub4__sub1__sub2(this);
            else
                ypaworld_func158__sub4__sub1__sub1(this);


            if ( LevelNet->field_BE38 )
            {
                if ( GameShell->_input->ClickInf.flag & ClickBoxInf::FLAG_LM_DOWN )
                {
                    if ( LevelNet->mapInfos[ LevelNet->field_BE38 ].field_0 == 3 )
                    {
//            v14 = get_lang_string(ypaworld.string_pointers_p2, 2466, "2466 == THE YPA CD IS REQUIRED.");
//            v15 = get_lang_string(ypaworld.string_pointers_p2, 21, "21 == YOUR PERSONAL AMOK");
//            if ( make_CD_CHECK(0, 1, v15, v14) )
//            {
                        GameShell->envAction.action = EnvAction::ACTION_PLAY;
                        GameShell->envAction.params[0] = LevelNet->field_BE38;
                        GameShell->envAction.params[1] = LevelNet->field_BE38;
//            }
                    }
                    else
                    {
//            v16 = get_lang_string(ypaworld.string_pointers_p2, 2466, "2466 == THE YPA CD IS REQUIRED.");
//            v17 = get_lang_string(ypaworld.string_pointers_p2, 21, "21 == YOUR PERSONAL AMOK");
//            if ( make_CD_CHECK(0, 1, v17, v16) )
//            {
                        if ( !ypaworld_func158__sub4__sub1__sub3(LevelNet->field_BE38) )// LOAD LEVEL INFO
                        {
                            GameShell->envAction.action = EnvAction::ACTION_PLAY;
                            GameShell->envAction.params[0] = LevelNet->field_BE38;
                            GameShell->envAction.params[1] = LevelNet->field_BE38;
                        }
//            }
                    }
                }
                else if ( GameShell->_input->ClickInf.flag & ClickBoxInf::FLAG_RM_DOWN )
                {
//          v18 = get_lang_string(ypaworld.string_pointers_p2, 2466, "2466 == THE YPA CD IS REQUIRED.");
//          v19 = get_lang_string(ypaworld.string_pointers_p2, 21, "21 == YOUR PERSONAL AMOK");
//          if ( make_CD_CHECK(0, 1, v19, v18) )
//          {
                    GameShell->envAction.action = EnvAction::ACTION_PLAY;
                    GameShell->envAction.params[0] = LevelNet->field_BE38;
                    GameShell->envAction.params[1] = LevelNet->field_BE38;
//          }
                }
            }
        }
    }
    else
    {
        if ( _levelInfo->State == 1 )
        {
            if ( _levelInfo->MovieWinStr[0] )
                sub_4491A0(_levelInfo->MovieWinStr);
        }

        if ( _levelInfo->State == 2 )
        {
            if ( _levelInfo->MovieLoseStr[0] )
            {
                if ( field_1624 )
                    sub_4491A0(_levelInfo->MovieLoseStr);
            }
        }

        brief.Stage = 0;
        _levelInfo->State = 8;

        if ( field_7278 )
            InitDebrief();
    }
}


int NC_STACK_ypaworld::sub_4DA41C(LevelDesc *mapp, const std::string &fname)
{
    mapp->clear();
    ScriptParser::HandlersList parsers
    {
        new World::Parsers::LevelDataParser(this, mapp),
        new World::Parsers::MapRobosParser(mapp),
        new World::Parsers::LevelGemParser(this),
        new World::Parsers::LevelSquadParser(mapp),
        new World::Parsers::LevelGatesParser(this),
        new World::Parsers::LevelMbMapParser(mapp),
        new World::Parsers::VhclProtoParser(this),
        new World::Parsers::WeaponProtoParser(this),
        new World::Parsers::BuildProtoParser(this),
        new World::Parsers::LevelEnableParser(this),
        new World::Parsers::LevelMapsParser(this, mapp),
        new World::Parsers::LevelDebMapParser(mapp),
        new World::Parsers::LevelSuperItemsParser(this),
    };

    return ScriptParser::ParseFile(fname, parsers, ScriptParser::FLAG_NO_SCOPE_SKIP);
}


char * sub_4C4284(NC_STACK_ypaworld *yw, GuiList *lstvw, char *out, const char *txt)
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

char * sub_4C41DC(NC_STACK_ypaworld *yw, GuiList *lstvw, char *out, const char *txt)
{

    char * tmp = out;

    FontUA::store_u8(&tmp, 123);

    FontUA::set_txtColor(&tmp, yw->iniColors[61].r, yw->iniColors[61].g, yw->iniColors[61].b);

    tmp = FontUA::FormateClippedText(yw->tiles[0], tmp, txt, lstvw->entryWidth - 2 * yw->font_default_w__b, 32);

    FontUA::store_u8(&tmp, 125);
    FontUA::next_line(&tmp);

    return tmp;
}

void ypaworld_func158__video_list_draw(NC_STACK_ypaworld *yw, UserData *usr)
{
    char *v3 = usr->video_listvw.itemBlock;

    usr->video_listvw.SetRect(yw, -2, -2);

    v3 = usr->video_listvw.ItemsPreLayout(yw, v3, 0, "uvw");

    int index = 0;
    for (const auto &nod : usr->video_mode_list)
    {
        if ( index >= usr->video_listvw.firstShownEntries && index < usr->video_listvw.firstShownEntries + usr->video_listvw.shownEntries)
        {
            if ( usr->video_listvw.selectedEntry == index )
                v3 = sub_4C4284(yw, &usr->video_listvw, v3, nod.name.c_str());
            else
                v3 = sub_4C41DC(yw, &usr->video_listvw, v3, nod.name.c_str());
        }
        index++;
    }

    if ( index >= 4 )
        index = 4;

    usr->video_listvw.shownEntries = index;
    v3 = usr->video_listvw.ItemsPostLayout(yw, v3, 0, "xyz");
    FontUA::set_end(&v3);

    w3d_a209 v16;
    v16 = usr->video_listvw.cmdstrm;

    GFXEngine::GFXe.drawText(&v16);
}

void ypaworld_func158__d3d_list_draw(NC_STACK_ypaworld *yw, UserData *usr)
{
    char *v3 = usr->d3d_listvw.itemBlock;

    usr->d3d_listvw.SetRect(yw, -2, -2);

    v3 = usr->d3d_listvw.ItemsPreLayout(yw, v3, 0, "uvw");

    int v5 = 0;

    wdd_func324arg v15;
    v15.name = NULL;
    v15.guid = NULL;
    v15.currr = 0;

    NC_STACK_win3d *windd = dynamic_cast<NC_STACK_win3d *>(yw->_win3d);
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

char * sub_4DDF78(NC_STACK_ypaworld *yw, GuiList *lstvw, char *pos, int a3)
{
    char *v3 = pos;

    for ( int i = 0; i < a3; i++ )
        v3 = sub_4C41DC(yw, lstvw, v3, " ");

    return v3;
}

void ypaworld_func158__network_list_draw(NC_STACK_ypaworld *yw, UserData *usr)
{
    const std::array<std::string, 2> connNames
    { "Host the game"
    , "Connect to game"};
    
    bool slct = false;

    char *cmd = usr->network_listvw.itemBlock;
    usr->network_listvw.SetRect(yw, -2, -2);

    cmd = usr->network_listvw.ItemsPreLayout(yw, cmd, 0, "uvw");

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
            if (i < 2)
            {
                str1 = connNames[i];
                brk = false;
                
                if (usr->netSel == -1 && i == 0)
                {
                    usr->netName = str1;
                    usr->network_button->button_func71(1200, usr->netName);
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

                int lvlid = std::stoi(str1);

                str1 = get_lang_string( yw->string_pointers_p2, lvlid + 1800, yw->LevelNet->mapInfos[ lvlid ].map_name.c_str() );

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

                    lvlid = std::stoi(str1);

                    usr->netName = yw->LevelNet->mapInfos[ lvlid ].map_name;

                    usr->network_button->button_func71(1200, usr->netName);

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

                str2 = fmt::sprintf("%d X %d", lvl->secXsize, lvl->secYsize);

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
                    usr->netName = usr->map_descriptions[ i ].pstring;

                    usr->network_button->button_func71(1200, usr->netName);

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
                items[0].txt = str1;
                items[0].flags = 0x27;
                items[0].width = wdth;
                itemsCount = 1;
                break;

            case 1:
                items[0].fontID = fontID;
                items[0].spaceChar = spaceChar;
                items[0].prefixChar = prefixChar;
                items[0].postfixChar = postfixChar;
                items[0].txt = str4;
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
                items[2].txt = str1;
                items[2].flags = 0x26;
                items[2].width = wdth - (items[0].width + items[1].width);

                itemsCount = 3;
                break;

            case 3:
            {
                TileMap *tiles = GFXEngine::GFXe.getTileset(8);

                items[0].txt = connTp;
                items[0].width = tiles->map[80].w;
                items[0].fontID = 8;
                items[0].spaceChar = (slct == false) + 106;
                items[0].prefixChar = prefixChar;
                items[0].postfixChar = postfixChar;
                items[0].flags = 4;

                items[1].txt = str1;
                items[1].fontID = fontID;
                items[1].spaceChar = spaceChar;
                items[1].prefixChar = prefixChar;
                items[1].postfixChar = postfixChar;
                items[1].width = floor(0.6 * wdth);
                items[1].flags = 0x25;

                items[2].fontID = fontID;
                items[2].spaceChar = spaceChar;
                items[2].prefixChar = prefixChar;
                items[2].txt = str2;
                items[2].postfixChar = postfixChar;
                items[2].width = floor(0.15 * wdth);
                items[2].flags = 0x24;

                items[3].txt = str3;
                items[3].width = 4 * tiles->map[80].w + 6;
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
                items[0].txt = str1;
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

void ypaworld_func158__locale_list_draw(NC_STACK_ypaworld *yw, UserData *usr)
{
    char *v3 = usr->local_listvw.itemBlock;

    usr->local_listvw.SetRect(yw, -2, -2);

    v3 = usr->local_listvw.ItemsPreLayout(yw, v3, 0, "uvw");

    int v5 = 0;
    for( const auto &x : usr->lang_dlls )
    {
        if ( v5 >= usr->local_listvw.firstShownEntries && v5 < usr->local_listvw.shownEntries + usr->local_listvw.firstShownEntries)
        {
            if ( v5 == usr->local_listvw.selectedEntry )
                v3 = sub_4C4284(yw, &usr->local_listvw, v3, x.c_str());
            else
                v3 = sub_4C41DC(yw, &usr->local_listvw, v3, x.c_str());
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

void ypaworld_func158__saveload_list_draw(NC_STACK_ypaworld *yw, UserData *usr)
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

            v31[0].fontID = v8;
            v31[0].spaceChar = v9;
            v31[0].flags = 37;
            v31[0].width = (v37 * 0.75);
            v31[0].prefixChar = v45;
            v31[0].postfixChar = 0;
            v31[0].txt = it->name;

            v31[1].fontID = v8;
            v31[1].spaceChar = v9;
            v31[1].txt = fmt::sprintf("%02d:%02d:%02d", (it->totalElapsedTime / 1000) / 3600, (it->totalElapsedTime / 1000) % 3600 / 60, (it->totalElapsedTime / 1000) % 3600 % 60 );
            v31[1].width = v37 - (v37 * 0.75);
            v31[1].prefixChar = 0;
            v31[1].flags = 38;
            v31[1].postfixChar = v44;

            FontUA::select_tileset(&v4, 0);

            FontUA::store_u8(&v4, 123);

            if ( !StriCmp(it->name, usr->user_name) )
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

    TileMap *v7 = GFXEngine::GFXe.getTileset(0);

    while ( v6 > v7->h )
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

        v6 -= v7->h;
    }

    FontUA::set_yoff(&tmp, v7->h - 1);

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
    NC_STACK_ypaworld *yw = usr->p_ypaworld;

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

void ypaworld_func158__sub3(NC_STACK_ypaworld *yw, UserData *usr)
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

void yw_freeTileSets(NC_STACK_ypaworld *yw)
{
    for ( auto &x : yw->tiles )
    {
        delete x;
        x = NULL;
    }
}

void sb_0x44ac24__sub0(NC_STACK_ypaworld *yw)
{
    if ( yw->_win3d )
    {
        displ_arg263 v5;
        v5.bitm = 0;
        v5.pointer_id = 0;
        yw->_win3d->display_func263(&v5);
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

void sb_0x44ac24(NC_STACK_ypaworld *yw)
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
    yw->UpdateGuiSettings();

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

    user_name.clear();

    p_YW = NULL;
    p_ypaworld = NULL;
    _input = NULL;
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
    
    titel_button = NULL;
    button_input_button = NULL;

    field_D36 = 0;
    field_D3A = 0;
    inp_joystick = false;
    field_D42 = 0;
    inp_altjoystick = false;
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
    usernamedir.clear();
    field_0x1744 = 0;
    //opened_dir = NULL;
    //nlist files_list;
    field_1756 = 0;
    _saveDataFlags = 0;
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
    netName.clear();

    netNameCurPos = 0;
    netLevelID = 0;
    netLevelName = NULL;

    callSIGN.clear();

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

    for(std::string &str : InputConfigTitle)
        str.clear();
    
    for(TInputConf &cfg : InputConfig)
        cfg = TInputConf();

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

