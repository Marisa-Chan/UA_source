#include <inttypes.h>
#include <string.h>

#include "includes.h"
#include "system/movie.h"
#include "yw.h"
#include "yw_internal.h"
#include "lstvw.h"
#include "font.h"
#include "button.h"
#include "windp.h"
#include "env.h"
#include "loaders.h"
#include "system/inivals.h"

extern GuiList stru_5C91D0;


TileMap * NC_STACK_ypaworld::yw_LoadFont(const std::string &fontname)
{
    FSMgr::FileHandle *fil = uaOpenFileAlloc("rsrc:hfonts/" + fontname, "r");
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

    tileset->img = Utils::ProxyLoadImage({
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

    tileset->img = Utils::ProxyLoadImage({
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
            ResBitmap *bitm = tileset->img->GetBitmap();

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
        _guiTiles[i] = yw_LoadFont(fontNames.at(i));
        if ( !_guiTiles[i] )
        {
            ypa_log_out("Could not load font (%s)", fontNames.at(i).c_str());
            return 0;
        }

        GFX::Engine.SetTileset(_guiTiles[i], i);
    }


    _guiTiles[40] = yw_LoadTileSet("lego16.ilbm", Common::Point(16, 16), Common::Point(16, 16), Common::Point(16, 16), Common::Point());
    if ( !_guiTiles[40] )
        return 0;
    GFX::Engine.SetTileset(_guiTiles[40], 40);

    _guiTiles[41] = yw_LoadTileSet("lego8.ilbm", Common::Point(8, 8), Common::Point(8, 8), Common::Point(16, 16), Common::Point());
    if ( !_guiTiles[41] )
        return 0;
    GFX::Engine.SetTileset(_guiTiles[41], 41);

    _guiTiles[42] = yw_LoadTileSet("lego4.ilbm", Common::Point(4, 4), Common::Point(4, 4), Common::Point(16, 16), Common::Point());
    if ( !_guiTiles[42] )
        return 0;
    GFX::Engine.SetTileset(_guiTiles[42], 42);

    _guiTiles[43] = yw_LoadTileSet("sec4.ilbm", Common::Point(4, 4), Common::Point(4, 4), Common::Point(16, 16), Common::Point());
    if ( !_guiTiles[43] )
        return 0;
    GFX::Engine.SetTileset(_guiTiles[43], 43);

    _guiTiles[44] = yw_LoadTileSet("sec8.ilbm", Common::Point(8, 8), Common::Point(8, 8), Common::Point(16, 16), Common::Point());
    if ( !_guiTiles[44] )
        return 0;
    GFX::Engine.SetTileset(_guiTiles[44], 44);
    
    
    
    _guiTiles[50] = yw_LoadTileSet("mapmisc.ilbm", Common::Point(4, 4), Common::Point(4, 0), Common::Point(9, 1), Common::Point(504, 64));
    if ( !_guiTiles[50] )
        return 0;
    GFX::Engine.SetTileset(_guiTiles[50], 50);
    
    _guiTiles[51] = yw_LoadTileSet("mapmisc.ilbm", Common::Point(8, 8), Common::Point(8, 0), Common::Point(9, 1), Common::Point(432, 64));
    if ( !_guiTiles[51] )
        return 0;
    GFX::Engine.SetTileset(_guiTiles[51], 51);
    
    _guiTiles[52] = yw_LoadTileSet("mapmisc.ilbm", Common::Point(16, 16), Common::Point(16, 0), Common::Point(9, 1), Common::Point(288, 64));
    if ( !_guiTiles[52] )
        return 0;
    GFX::Engine.SetTileset(_guiTiles[52], 52);
    
    _guiTiles[53] = yw_LoadTileSet("mapmisc.ilbm", Common::Point(32, 32), Common::Point(32, 0), Common::Point(9, 1), Common::Point(0, 64));
    if ( !_guiTiles[53] )
        return 0;
    GFX::Engine.SetTileset(_guiTiles[53], 53);
    
    _guiTiles[54] = yw_LoadTileSet("mapmisc.ilbm", Common::Point(64, 64), Common::Point(64, 0), Common::Point(9, 1), Common::Point(0, 0));
    if ( !_guiTiles[54] )
        return 0;
    GFX::Engine.SetTileset(_guiTiles[54], 54);

    for (int i = 50; i < 55; i++)
    {
        TileMap *t = _guiTiles[i];

        t->map[9] = t->map[8];
        t->map[8] = t->map[0];
    }
    
    

    _guiTiles[59] = yw_LoadFont("mapvhcl3.font");
    if ( !_guiTiles[59] )
        return 0;
    GFX::Engine.SetTileset(_guiTiles[59], 59);

    _guiTiles[60] = yw_LoadFont("mapvhcl5.font");
    if ( !_guiTiles[60] )
        return 0;
    GFX::Engine.SetTileset(_guiTiles[60], 60);

    _guiTiles[61] = yw_LoadFont("mapvhcl7.font");
    if ( !_guiTiles[61] )
        return 0;
    GFX::Engine.SetTileset(_guiTiles[61], 61);

    _guiTiles[62] = yw_LoadFont("mapvhcl9.font");
    if ( !_guiTiles[62] )
        return 0;
    GFX::Engine.SetTileset(_guiTiles[62], 62);

    _fontH = _guiTiles[0]->h;
    _fontDefCloseW = _guiTiles[0]->map[97].w; // a
    _fontBorderW = _guiTiles[0]->map[98].w; // b

    _fontVBScrollW = _guiTiles[12]->map[66].w; // B (Y-Scroller, Background-Inners)
    _fontHScrollH = _guiTiles[11]->h;

    _fontBorderH = 2;

    _fontVScrollH = _guiTiles[12]->h;

    _iconOrderW = _guiTiles[21]->map[65].w; // A (Mode:      ORDER)
    _iconOrderH = _guiTiles[21]->h;

    _iconHelpW = _guiTiles[24]->map[65].w; // A (NEU: DER HILFEBUTTTON)
    _iconHelpH = _guiTiles[24]->h;

    _upScreenBorder = _guiTiles[30]->h; // ENERGIE height
    _downScreenBorder = _iconOrderH;
    
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

bool NC_STACK_ypaworld::InitMapRegionsNet()
{
    _iniColors.fill( GFX::Engine.Color(255, 255, 0) );

    if ( !yw_ParseWorldIni("env:world.ini") && !yw_ParseWorldIni("data:world.ini"))
    {
        ypa_log_out("yw_ParseWorldIni() failed.\n");
        return false;
    }

    if ( !yw_ScanLevels() )
    {
        ypa_log_out("yw_ScanLevels() failed.\n");
        return false;
    }

    _levelInfo.JodieFoster.fill(0);

    return true;
}

int yw_InitNetwork(NC_STACK_ypaworld *yw)
{

    NC_STACK_windp *windp = Nucleus::CInit<NC_STACK_windp>();

    yw->_netDriver = windp;

    if ( !windp )
    {
        ypa_log_out("Unable to create network-Object\n");
        return 0;
    }

    yw->_netInterpolate = true;

    yw->_netTcpAddress.clear();

    yw->_netDriver->SetVersion(yw->_buildDate);

    return 1;
}


bool NC_STACK_ypaworld::ParseColorString(int color_id, const std::string &color_string)
{
    Stok stok(color_string, "_");
    std::string sr, sg, sb, si;

    if ( stok.GetNext(&sr) && stok.GetNext(&sg) && stok.GetNext(&sb) /*&& stok.GetNext(&si)*/)
    {
        int r = std::stoi(sr);
        int g = std::stoi(sg);
        int b = std::stoi(sb);
        //int i = std::stoi(si);
        _iniColors.at(color_id) = GFX::Engine.Color(r, g, b);
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





bool NC_STACK_ypaworld::sb_0x4e1a88__sub0__sub0(TLevelDescription *mapp, const std::string &fname)
{
    *mapp = TLevelDescription();
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

    TLevelDescription mapp;
    sb_0x4e1a88__sub0__sub0(&mapp, buf);

    TMapRegionInfo &minf = _globalMapRegions.MapRegions[levelnmb];

    if ( multiplayer )
        minf.Status = TMapRegionInfo::STATUS_NETWORK;
    else
        minf.Status = TMapRegionInfo::STATUS_ENABLED;

    minf.MapDirectory = buf;

    minf.MapName = _levelInfo.MapName;

    minf.Rect = Common::FRect();

    minf.FractionsBits = 0;
    minf.RoboCount = mapp.Robos.size();

    for ( const MapRobo &robo : mapp.Robos )
        minf.FractionsBits |= 1 << robo.Owner;

    minf.MapSize = mapp.MapSize;
    minf.SlowConnection = mapp.SlowConnection;
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

NC_STACK_base * sub_44AD8C(const std::string &fname)
{
    NC_STACK_base *obj = Nucleus::CInit<NC_STACK_base>();
    if ( obj )
    {
        FSMgr::FileHandle *fil = uaOpenFileAlloc(fname, "r");
        if ( !fil )
        {
            obj->Delete();
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

            NC_STACK_base *kid = Utils::ProxyLoadBase(basName);

            if ( !kid )
            {
                ypa_log_out("init: Could not load %s.\n", basName.c_str());
                delete fil;
                obj->Delete();
                return NULL;
            }

            obj->AddKid(kid); //Add to kid list
        }
        delete fil;
    }
    return obj;
}

NC_STACK_base *load_set_base()
{
    NC_STACK_base *base = NC_STACK_base::LoadBaseFromFile("rsrc:objects/set.base");
    if ( !base )
    {
        ypa_log_out("init: no set.base, trying fragment load.\n");

        base = Nucleus::CInit<NC_STACK_base>();
        if ( base )
        {
            NC_STACK_base *visproto = NC_STACK_base::LoadBaseFromFile("rsrc:objects/visproto.base");
            if ( !visproto )
            {
                ypa_log_out("init: no visproto.base, trying single load.\n");
                visproto = sub_44AD8C("rsrc:scripts/visproto.lst");
            }
            if ( !visproto )
            {
                base->Delete();
                return NULL;
            }
            base->AddKid(visproto);

            NC_STACK_base *lego = NC_STACK_base::LoadBaseFromFile("rsrc:objects/lego.base");
            if ( !lego )
            {
                ypa_log_out("init: no lego.base, trying single load.\n");
                lego = sub_44AD8C("rsrc:scripts/set.sdf");
            }
            if ( !lego )
            {
                base->Delete();
                return NULL;
            }
            base->AddKid(lego);

            NC_STACK_base *slurp = NC_STACK_base::LoadBaseFromFile("rsrc:objects/slurp.base");
            if ( !slurp )
            {
                ypa_log_out("init: no slurp.base, trying single load.\n");
                slurp = sub_44AD8C("rsrc:scripts/slurps.lst");
            }
            if ( !slurp )
            {
                base->Delete();
                return NULL;
            }
            base->AddKid(slurp);
        }
    }
    return base;
}

int sub_44A12C(NC_STACK_ypaworld *yw, NC_STACK_base *base)
{
    for( NC_STACK_base * kd : base->GetKidList() )
    {
        yw->_vhclModels.push_back(kd);
        kd->SetVizLimit(yw->_normalVizLimit);
        kd->SetFadeLength(yw->_normalFadeLength);
    }

    return 1;
}

int yw_parse_lego(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil, NC_STACK_base *base)
{
    /* TODO:
     * Make base cache by names and search of set.sdf base by name, not by index
     */
    
    int id = 0;
    for( NC_STACK_base *& bs : base->GetKidList() )
    {
        yw->_legoArray[id].Base = bs;
        bs->SetVizLimit(yw->_normalVizLimit);
        bs->SetFadeLength(yw->_normalFadeLength);
        bs->SetStatic(true);
        id++;
    }
    
    id = 0;
    std::string line;
    while ( fil->ReadLine(&line) && !line.empty() && line[0] != '>' )
    {
        TLego &lego = yw->_legoArray[id];
        
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

            lego.CollisionSkelet = skelet;

            if ( !parse.GetNext(&token) )
                return 0;

            // Not used
            // int fontPage = std::stol(token, NULL, 0);

            if ( !parse.GetNext(&token) )
                return 0;

            lego.GUIElementID = std::stol(token, NULL, 0);

            if ( !parse.GetNext(&token) )
                return 0;

            lego.Shield = std::stol(token, NULL, 0);

            if ( !parse.GetNext(&token) )
                return 0;

            switch( std::stol(token, NULL, 0) )
            {
            default:
            case 0:
                lego.UseCollisionSkelet = lego.CollisionSkelet;
                break;

            case 1:
                lego.UseCollisionSkelet = yw->_colsubSkeleton;
                break;

            case 2:
                lego.UseCollisionSkelet = yw->_colcompSkeleton;
                break;
            }
            
            lego.Explosions.clear();

            int fxindex = 0;
            while ( 1 )
            {
                if ( !parse.GetNext(&token) )
                    break;

                lego.Explosions.emplace_back();
                TLego::ExFX &fx = lego.Explosions.back();
                
                fx.Index = fxindex;
                fx.ObjectID = std::stol(token, NULL, 0);

                if ( parse.GetNext(&token) )
                    fx.Position.x = std::stof(token, 0);

                if ( parse.GetNext(&token) )
                    fx.Position.z = -std::stof(token, 0);

                if ( parse.GetNext(&token) )
                    fx.Position.y = -std::stof(token, 0);

                fxindex++;
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
            TSubSectorDesc *ssec = &yw->_subSectorArray[id];

            ssec->HPModels[0] = std::stol(token, NULL, 0);

            if ( !parse.GetNext(&token) )
                return 0;

            ssec->HPModels[1] = std::stol(token, NULL, 0);

            if ( !parse.GetNext(&token) )
                return 0;

            ssec->HPModels[2] = std::stol(token, NULL, 0);

            if ( !parse.GetNext(&token) )
                return 0;

            ssec->HPModels[3] = std::stol(token, NULL, 0);

            if ( !parse.GetNext(&token) )
                return 0;

            if ( token.at(0) > '0' && token.at(0) == '1' )
                ssec->StartHealth = 255;
            else
                ssec->StartHealth = 0;
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
            TSectorDesc *sektp = &yw->_secTypeArray[ std::stol(token, NULL, 0) ];

            if ( !parse.GetNext(&token) )
            {
                ypa_log_out("Error reading '%s', line '%s'.\n", "set.sdf", line.c_str());
                return 0;
            }

            sektp->SectorType = std::stol(token, NULL, 0);

            if ( !parse.GetNext(&token) )
            {
                ypa_log_out("Error reading '%s', line '%s'.\n", "set.sdf", line.c_str());
                return 0;
            }

            sektp->SurfaceType = std::stol(token, NULL, 0);

            if ( !parse.GetNext(&token) )
            {
                ypa_log_out("Error reading '%s', line '%s'.\n", "set.sdf", line.c_str());
                return 0;
            }

            sektp->GUIElementID = std::stol(token, NULL, 0);

            sektp->SubSectors.fill(NULL);

            if ( sektp->SectorType == 1 )
            {
                if ( !parse.GetNext(&token) )
                {
                    ypa_log_out("Error reading '%s', line '%s'.\n", "set.sdf", line.c_str());
                    return 0;
                }
                sektp->SubSectors.At(0, 0) = &yw->_subSectorArray[ std::stol(token, NULL, 0) ];
            }
            else
            {
                for (int yy = 0; yy < 3; yy++)
                {
                    for (int xx = 0; xx < 3; xx++)
                    {
                        if ( !parse.GetNext(&token) )
                        {
                            ypa_log_out("Error reading '%s', line '%s'.\n", "set.sdf", line.c_str());
                            return 0;
                        }

                        sektp->SubSectors.At(xx, 2 - yy) = &yw->_subSectorArray[ std::stol(token, NULL, 0) ];
                    }
                }
            }
        }
    }
    return 1;
}

int sub_44A97C(NC_STACK_ypaworld *yw, NC_STACK_base *base)
{
    BaseList &kid_list = base->GetKidList();
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
                (*it)->SetVizLimit(yw->_normalVizLimit);
                (*it)->SetFadeLength(yw->_normalFadeLength);
                (*it)->SetStatic(true);

                if (i == 0)
                    yw->_fillersHorizontal(j, k) = *it;
                else if (i == 1)
                    yw->_fillersVertical(j, k) = *it;

                it++;
            }
        }
    }

    std::string oldRsrc = Common::Env.SetPrefix("rsrc", "data:mc2res");

    NC_STACK_skeleton *tmp_skel = Nucleus::CInit<NC_STACK_sklt>( {{NC_STACK_rsrc::RSRC_ATT_NAME, std::string("Skeleton/ColSide.sklt")}} );
    if ( !tmp_skel )
    {
        ypa_log_out("Couldn't create side collision sklt.\n");
        return 0;
    }

    yw->_fillerSide = tmp_skel;

    tmp_skel = Nucleus::CInit<NC_STACK_sklt>( {{NC_STACK_rsrc::RSRC_ATT_NAME, std::string("Skeleton/ColCross.sklt")}} );
    if ( !tmp_skel )
    {
        ypa_log_out("Couldn't create cross collision sklt.\n");
        return 0;
    }

    yw->_fillerCross = tmp_skel;

    Common::Env.SetPrefix("rsrc", oldRsrc);

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
    
    std::string oldRsrc = Common::Env.SetPrefix("rsrc", "data:mc2res");

    for (int i = 0; i < 11; i++)
    {
        if (yw->_mousePointers[i])
            yw->_mousePointers[i]->Delete();
        
        yw->_mousePointers[i] = Utils::ProxyLoadImage({
            {NC_STACK_rsrc::RSRC_ATT_NAME, pointerFiles.at(i)},
            {NC_STACK_bitmap::BMD_ATT_CONVCOLOR, (int32_t)1}});
        if ( !yw->_mousePointers[i] )
        {
            ypa_log_out("yw_select.c/yw_InitMouseStuff()\n");
            Common::Env.SetPrefix("rsrc", oldRsrc);
            return 0;
        }
    }

    GFX::displ_arg263 arg_263;

    arg_263.bitm = 0;
    arg_263.pointer_id = 0;
    GFX::Engine.SetCursor(arg_263.pointer_id, 0);

    if (yw->_mousePointers[0])
        arg_263.bitm = yw->_mousePointers[0]->GetBitmap();
    arg_263.pointer_id = 1;
    GFX::Engine.SetCursor(arg_263.pointer_id, 0);

    Common::Env.SetPrefix("rsrc", oldRsrc);
    return 1;
}

int NC_STACK_ypaworld::yw_LoadSet(int setID)
{
    _mouseGrabbed = false;

    std::string oldRsrc = Common::Env.SetPrefix("rsrc", "data:mc2res");

    GFX::Engine.display_func271(NULL);

    _colsubSkeleton = Nucleus::CInit<NC_STACK_sklt>({{NC_STACK_rsrc::RSRC_ATT_NAME, std::string("skeleton/colsub.sklt")}});
    if ( !_colsubSkeleton )
    {
        ypa_log_out("Couldn't load <skeleton/colsub.sklt>, set %d.\n", setID);
        return 0;
    }

    _colcompSkeleton = Nucleus::CInit<NC_STACK_sklt>({{NC_STACK_rsrc::RSRC_ATT_NAME, std::string("skeleton/colcomp.sklt")}});
    if ( !_colcompSkeleton )
    {
        ypa_log_out("Couldn't load <skeleton/colcomp.sklt>, set %d.\n", setID);
        return 0;
    }

    Common::Env.SetPrefix("rsrc", fmt::sprintf("data:set%d", setID));

    if ( !GFX::Engine.LoadPalette("palette/standard.pal") )
        ypa_log_out("WARNING: Could not load set default palette!\n");

    if ( setID != _setId && setID != 46 )
    {
        if ( _setData )
        {
            ypa_log_out("yw_LoadSet(): killing set object %d\n", _setId);
            _setData->Delete();
            _setData = NULL;
            _setId = 0;
        }

        _setData = load_set_base();
        if ( !_setData )
        {
            _setId = 0;
            ypa_log_out("yw_LoadSet(): loading set object %d failed\n", setID);
            Common::Env.SetPrefix("rsrc", oldRsrc);
            return 0;
        }
        
        _setData->MakeVBO();
        
        _setId = setID;
        ypa_log_out("yw_LoadSet(): loaded set object %d ok\n", setID);
    }


    if ( setID != 46 )
    {
        FSMgr::FileHandle* fil = uaOpenFileAlloc("rsrc:scripts/set.sdf", "r");
        if ( !fil )
        {
            ypa_log_out("Couldn't open set description file, set %d!\n", setID);
            return 0;
        }

        int kid_id = 0;

        for( NC_STACK_base *& bs : _setData->GetKidList() )
        {
            if ( kid_id == 0 )
            {
                if ( !sub_44A12C(this, bs) )
                {
                    delete fil;
                    return 0;
                }
            }
            else if ( kid_id == 1 )
            {
                if ( !yw_parse_lego(this, fil, bs) )
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
                if ( !sub_44A97C(this, bs) )
                {
                    delete fil;
                    return 0;
                }
            }

            kid_id++;
        }

        delete fil;
        
        LoadOverrideModels();
    }

    _tracyBitmap = Utils::ProxyLoadImage( {{NC_STACK_rsrc::RSRC_ATT_NAME, std::string("remap/tracyrmp.ilbm")}} );
    if ( !_tracyBitmap )
    {
        ypa_log_out("Couldn't load tracy remap table, set %d.\n", setID);
        return 0;
    }

    _shadermpBitmap = Utils::ProxyLoadImage( {{NC_STACK_rsrc::RSRC_ATT_NAME, std::string("remap/shadermp.ilbm")}} );
    if ( !_shadermpBitmap )
    {
        ypa_log_out("Couldn't load shade remap table, set %d.\n", setID);
        return 0;
    }

    GFX::Engine.SetTracyRmp( _tracyBitmap->GetBitmap() );
    GFX::Engine.SetShadeRmp( _shadermpBitmap->GetBitmap() );

    _beeBox = NC_STACK_base::LoadBaseFromFile("rsrc:objects/beebox.base");
    if ( !_beeBox )
    {
        ypa_log_out("Couldn't load bbox object, set %d.\n", setID);
        return 0;
    }

    _beeBox->SetStatic(true);

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
    if ( yw->_GameShell )
    {
        FSMgr::FileHandle *fil = uaOpenFileAlloc( fmt::sprintf("save:%s/%s", yw->_GameShell->UserName, fname), "r");

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
    if ( yw->_GameShell )
    {
        FSMgr::FileHandle *fil = uaOpenFileAlloc( fmt::sprintf("save:%s/%s", yw->_GameShell->UserName, fname), "w");

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
    GFX::rstr_arg204 a4;
    a4.pbitm = bitm->GetBitmap();

    a4.float14 = Common::FRect(-1.0, -1.0, 1.0, 1.0);
    a4.float4 = Common::FRect(-1.0, -1.0, 1.0, 1.0);

    GFX::Engine.raster_func202(&a4);
}

void NC_STACK_ypaworld::sub_4491A0(const std::string &movie_fname)
{
    std::string buf = correctSeparatorAndExt( Common::Env.ApplyPrefix( movie_fname ) );

    if ( System::IniConf::GfxMoviePlayer.Get<bool>() )
    {
        GFX::Engine.EndFrame();
        
        if ( _preferences & World::PREF_CDMUSICDISABLE )
                SFXEngine::SFXe.StopMusicTrack(false); // Stop music without reset track ID
        
        System::Movie.PlayMovie(buf, _GameShell->soundVolume);
        
        if ( _preferences & World::PREF_CDMUSICDISABLE )
            SFXEngine::SFXe.PlayMusicTrack();
        
        GFX::Engine.BeginFrame();
    }
    
    Input::Engine.QueryInput(&input_states);

    input_states.KbdLastHit = Input::KC_NONE;
    input_states.KbdLastDown = Input::KC_NONE;
    input_states.HotKeyID = -1;
}

void NC_STACK_ypaworld::FreeFillers()
{
    _fillersHorizontal.fill(NULL);
    _fillersVertical.fill(NULL);

    if ( _fillerSide )
    {
        _fillerSide->Delete();
        _fillerSide = NULL;
    }

    if ( _fillerCross )
    {
        _fillerCross->Delete();
        _fillerCross = NULL;
    }
}

void NC_STACK_ypaworld::FreeLegos()
{
    for (TLego &lego : _legoArray)
    {
        if (lego.CollisionSkelet)
            lego.CollisionSkelet->Delete();
        
        lego = TLego();
    }
}

void NC_STACK_ypaworld::FreeBriefDataSet()
{
    _vhclModels.clear();
    FreeLegos();
    FreeFillers();
    ClearOverrideModels();
    
    for (TSubSectorDesc &ssec : _subSectorArray)
        ssec = TSubSectorDesc();
    
    for (TSectorDesc &styp : _secTypeArray)
        styp = TSectorDesc();

    if ( _setData )
    {
        _setData->Delete();
        _setData = NULL;
        _setId = 0;
    }
}

void NC_STACK_ypaworld::FreeBriefing()
{
    FreeBriefDataSet();

    _lvlTypeMap.Clear();

    _briefScreen.UnloadRes();
    _briefScreen.Stage = TBriefengScreen::STAGE_NONE;
    
    GuiWinClose(&stru_5C91D0);
    stru_5C91D0.Free();
}

void NC_STACK_ypaworld::FreeDebrief()
{
    _briefScreen.Clear();
    _briefScreen.Stage = TBriefengScreen::STAGE_NONE;

    _levelInfo.State = TLevelInfo::STATE_MENU;
}

// Select map
void ypaworld_func158__sub4__sub1__sub0(NC_STACK_ypaworld *yw, TInputState *inpt)
{
    float v3 = (float)inpt->ClickInf.move.ScreenPos.x / (float)yw->_screenSize.x;
    float v4 = (float)inpt->ClickInf.move.ScreenPos.y / (float)yw->_screenSize.y;

    if (v3 < 0.0)
        v3 = 0.0;
    if (v3 > 1.0)
        v3 = 1.0;

    if (v4 < 0.0)
        v4 = 0.0;
    if (v4 > 1.0)
        v4 = 1.0;

    int v6 = 0;
    size_t v7 = 0;

    if ( yw->_globalMapRegions.MaskImage )
    {
        if ( inpt->ClickInf.selected_btnID == -1 )
        {
            ResBitmap *a4 = yw->_globalMapRegions.MaskImage->GetBitmap();

            int xpos = a4->width * v3;
            int ypos = a4->height * v4;

            SDL_LockSurface(a4->swTex);
            v7 = ((uint8_t *)a4->swTex->pixels)[a4->swTex->pitch * ypos + xpos];
            SDL_UnlockSurface(a4->swTex);
            
            if ( v7 > 0 && v7 < 256 )
            {
                int v15 = yw->_globalMapRegions.MapRegions[v7].Status;
                if ( v15 != TMapRegionInfo::STATUS_NONE && v15 != TMapRegionInfo::STATUS_DISABLED && v15 != TMapRegionInfo::STATUS_NETWORK )
                    v6 = 1;
            }
        }
    }

    if ( v6 )
    {
        if ( v7 != yw->_globalMapRegions.SelectedRegion )
        {
            if ( yw->_GameShell )
                SFXEngine::SFXe.startSound(&yw->_GameShell->samples1_info, 8);
        }
        yw->_globalMapRegions.SelectedRegion = v7;
    }
    else
    {
        yw->_globalMapRegions.SelectedRegion = 0;
    }
}


void splashScreen_OutText(NC_STACK_ypaworld *yw, const std::string &txt, int x, int y)
{
    CmdStream cmdbuf;
    cmdbuf.reserve(2048);

    if ( !txt.empty() )
    {
        FontUA::select_tileset(&cmdbuf, 15);
        FontUA::set_xpos(&cmdbuf, x);
        FontUA::set_ypos(&cmdbuf, y);

        FontUA::set_txtColor(&cmdbuf, yw->_iniColors[13].r, yw->_iniColors[13].g, yw->_iniColors[13].b);

        Stok parse(txt, "\n\r");
        std::string line;
        while( parse.GetNext( &line ) )
        {
            FontUA::FormateClippedText(yw->_guiTiles[15], &cmdbuf, line, yw->_screenSize.x - x, ' ');

            FontUA::next_line(&cmdbuf);
        }

        FontUA::set_end(&cmdbuf);

        GFX::Engine.ProcessDrawSeq(cmdbuf);
    }
}

void NC_STACK_ypaworld::DrawMapRegionsTutorial()
{
    if ( _globalMapRegions.MenuImage && _globalMapRegions.MaskImage && _globalMapRegions.RolloverImage )
    {
        GFX::rstr_arg204 a4;
        a4.pbitm = _globalMapRegions.MenuImage->GetBitmap();

        a4.float4 = Common::FRect(-1.0, -1.0, 1.0, 1.0);
        a4.float14 = Common::FRect(-1.0, -1.0, 1.0, 1.0);

        GFX::Engine.raster_func202(&a4);

        for (size_t i = 0; i < _globalMapRegions.MapRegions.size(); i++)
        {
            TMapRegionInfo &region = _globalMapRegions.MapRegions[i];

            if ( !region.Rect.IsEmpty() )
            {
                ResBitmap *v20 = NULL;

                if ( region.Status == TMapRegionInfo::STATUS_ENABLED ||  region.Status == TMapRegionInfo::STATUS_COMPLETED)
                {
                    if ( i == _globalMapRegions.SelectedRegion )
                    {
                        v20 = _globalMapRegions.RolloverImage->GetBitmap();
                    }
                }

                if ( v20 )
                {
                    GFX::rstr_218_arg v17;
                    v17.bitm_intern = v20;
                    v17.bitm_intern2 = _globalMapRegions.MaskImage->GetBitmap();

                    v17.flg = i;
                    v17.rect2 = region.Rect;
                    v17.rect1 = v17.rect2;

                    GFX::Engine.raster_func218(&v17);
                }
            }
        }

        if ( _globalMapRegions.SelectedRegion )
        {
            CmdStream cmdBuff;
            cmdBuff.reserve(256);

            if ( _globalMapRegions.MapRegions[ _globalMapRegions.SelectedRegion ].Status != TMapRegionInfo::STATUS_NONE )
            {
                if ( !_globalMapRegions.MapRegions[ _globalMapRegions.SelectedRegion ].MapName.empty() )
                {
                    const std::string v12 = GetLevelName(_globalMapRegions.SelectedRegion);

                    FontUA::select_tileset(&cmdBuff, 15);
                    FontUA::set_xpos(&cmdBuff, 0);
                    FontUA::set_ypos(&cmdBuff, -(_fontH + _iconOrderH + 4) );
                    FontUA::set_txtColor(&cmdBuff, _iniColors[63].r, _iniColors[63].g, _iniColors[63].b);

                    FontUA::FormateCenteredSkipableItem(_guiTiles[15], &cmdBuff, v12, _screenSize.x);
                }
            }
            FontUA::set_end(&cmdBuff);

            GFX::Engine.ProcessDrawSeq(cmdBuff);
        }
    }

}

void NC_STACK_ypaworld::DrawMapRegions()
{
    if ( _globalMapRegions.MenuImage && _globalMapRegions.MaskImage && _globalMapRegions.RolloverImage && _globalMapRegions.FinishedImage && _globalMapRegions.EnabledImage )
    {
        GFX::rstr_arg204 a4;
        a4.pbitm = _globalMapRegions.MenuImage->GetBitmap();

        a4.float4 = Common::FRect(-1.0, -1.0, 1.0, 1.0);
        a4.float14 = Common::FRect(-1.0, -1.0, 1.0, 1.0);

        GFX::Engine.raster_func202(&a4);
        //printf("field_BE38 %d \n",yw->LevelNet->field_BE38);

        for (size_t i = 0; i < _globalMapRegions.MapRegions.size(); i++)
        {
            TMapRegionInfo &region = _globalMapRegions.MapRegions[i];

            if ( !region.Rect.IsEmpty() )
            {
                ResBitmap *v20 = NULL;

                if ( region.Status == TMapRegionInfo::STATUS_ENABLED )
                {
                    if ( i == _globalMapRegions.SelectedRegion )
                    {
                        v20 = _globalMapRegions.RolloverImage->GetBitmap();
                    }
                    else
                    {
                        v20 = _globalMapRegions.EnabledImage->GetBitmap();
                    }
                }
                else if ( region.Status == TMapRegionInfo::STATUS_COMPLETED )
                {
                    v20 = _globalMapRegions.FinishedImage->GetBitmap();
                }

                if ( v20 )
                {
                    GFX::rstr_218_arg v17;
                    v17.bitm_intern = v20;
                    v17.bitm_intern2 = _globalMapRegions.MaskImage->GetBitmap();

                    v17.flg = i;
                    v17.rect2 = region.Rect;
                    v17.rect1 = v17.rect2;

                    GFX::Engine.raster_func218(&v17);
                }
            }
        }

        if ( _globalMapRegions.SelectedRegion )
        {
            CmdStream cmdBuff;
            cmdBuff.reserve(256);

            if ( _globalMapRegions.MapRegions[ _globalMapRegions.SelectedRegion ].Status != TMapRegionInfo::STATUS_NONE )
            {
                if ( !_globalMapRegions.MapRegions[ _globalMapRegions.SelectedRegion ].MapName.empty() )
                {
                    const std::string v12 = GetLevelName(_globalMapRegions.SelectedRegion);

                    FontUA::select_tileset(&cmdBuff, 15);
                    FontUA::set_xpos(&cmdBuff, 0);
                    FontUA::set_ypos(&cmdBuff, -(_fontH + _iconOrderH + 4) );
                    FontUA::set_txtColor(&cmdBuff, _iniColors[63].r, _iniColors[63].g, _iniColors[63].b);

                    FontUA::FormateCenteredSkipableItem(_guiTiles[15], &cmdBuff, v12, _screenSize.x);
                }
            }
            FontUA::set_end(&cmdBuff);

            GFX::Engine.ProcessDrawSeq(cmdBuff);
        }
        const std::string &v13 = Locale::Text::ToD(_tipOfDayId, " ");
        splashScreen_OutText(this, v13, _screenSize.x / 20, _screenSize.x / 20);
    }
}


int NC_STACK_ypaworld::ypaworld_func158__sub4__sub1__sub3__sub0()
{
    int lft = _screenSize.x * 0.575;
    int rght = _screenSize.x * 0.9734375;
    int top = _screenSize.y * 0.08958333333333329;
    int bottom = _screenSize.y * 0.9083333333333334;

    int v5 = (bottom - top) / _fontH;

    stru_5C91D0 = GuiList();

    GuiList::tInit args;
    args.numEntries = 1;
    args.shownEntries = v5;
    args.minShownEntries = v5;
    args.maxShownEntries = v5;
    args.withIcon = false;
    args.entryHeight = _fontH;
    args.entryWidth = (rght - lft);
    args.enabled = true;
    args.vborder = _fontBorderH;

    int result = stru_5C91D0.Init(this, args);

    if ( !result )
        return 0;

    stru_5C91D0.SetRect(this, lft, top);
    GuiWinOpen( &stru_5C91D0 );

    return 1;
}

bool NC_STACK_ypaworld::InitBriefing(int lvlid)
{
    _briefScreen.Clear();

    _briefScreen.SelectedObjID = -1;
    
    _particles.Clear();

    SFXEngine::SFXe.StopMusicTrack();

    if ( !ypaworld_func158__sub4__sub1__sub3__sub0() )
        return false;

    GFX::Engine.display_func271(NULL);
    //_win3d->raster_func192(NULL);

    TLevelInfo backup = _levelInfo;

    _briefScreen.Stage = TBriefengScreen::STAGE_NONE;
    _briefScreen.TimerStatus = TBriefengScreen::TIMER_NORMAL;
    _briefScreen.CurrTime = 0;
    _briefScreen.TextTime = 0;

    _levelInfo.LevelID = lvlid;
    _levelInfo.State = TLevelInfo::STATE_BRIEFING;

    _techUpgrades.clear();

    _levelInfo.Gates.clear();

    Common::Env.SetPrefix("rsrc", "data:");

    TLevelDescription &mproto = _briefScreen.Desc;

    if ( !sub_4DA41C(&mproto, _globalMapRegions.MapRegions[ lvlid ].MapDirectory) )
        goto ON_ERR;

    if ( !_briefScreen.Desc.IsOk() )
        goto ON_ERR;

    _briefScreen.BriefingText = Locale::Text::MissionText(_levelInfo.LevelID, "<NO INFO AVAILABLE>");

    if ( mproto.Mbmaps.empty() )
        goto ON_ERR;

    if ( _levelInfo.MovieStr.empty() || _briefScreen.Stage != TBriefengScreen::STAGE_NONE )
    {
        for ( const MapRobo &robo : mproto.Robos)
        {
            if ( !_levelInfo.JodieFoster[ robo.Owner ] )
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

                if ( v19 != -1 && !_movies[v19].empty() )
                {
                    _briefScreen.MovieStr = _movies[v19];

                    _firstContactFaction = robo.Owner;
                    _briefScreen.Stage = TBriefengScreen::STAGE_MOVIE;
                    break;
                }
            }
        }
    }
    else
    {
        _briefScreen.MovieStr = _levelInfo.MovieStr;
        _briefScreen.Stage = TBriefengScreen::STAGE_MOVIE;
    }

    Common::Env.SetPrefix("rsrc", "levels:");

    if ( _globalMapRegions.brief_map[0].PicName[0] )
        _briefScreen.BriefingMapImg = Utils::ProxyLoadImage( {
            {NC_STACK_rsrc::RSRC_ATT_NAME, _globalMapRegions.brief_map[0].PicName},
            {NC_STACK_bitmap::BMD_ATT_CONVCOLOR, (int32_t)1}} );

    Common::Env.SetPrefix("rsrc", "mbpix:");

    _briefScreen.MbmapImg = Utils::ProxyLoadImage( {
        {NC_STACK_rsrc::RSRC_ATT_NAME, mproto.Mbmaps[0].PicName},
        {NC_STACK_bitmap::BMD_ATT_CONVCOLOR, (int32_t)1}} );

    if ( !_briefScreen.MbmapImg )
        goto ON_ERR;

    if ( _lvlTypeMap.IsNull() )
        _lvlTypeMap = World::LoadMapDataFromImage(mproto.TypStr);

    if ( _lvlTypeMap.IsNull() )
    {
        Common::DeleteAndNull( &_briefScreen.MbmapImg );
        goto ON_ERR;
    }

    SetMapSize(_lvlTypeMap.Size());

    //Set transitions
    _briefScreen.ViewingObjectRect = Common::FRect(-0.98750001, 0.34999999, -0.003125, 0.85416669);

    if ( _briefScreen.Stage == TBriefengScreen::STAGE_NONE )
        _briefScreen.Stage = TBriefengScreen::STAGE_LOADED;

    FreeBriefDataSet();
    return true;

ON_ERR:
    GuiWinClose( &stru_5C91D0 );
    stru_5C91D0.Free();

    _levelInfo = backup;

    return false;
}

int NC_STACK_ypaworld::ypaworld_func158__sub4__sub1__sub5__sub0(TLevelDescription *mapproto, const std::string &filename)
{
    *mapproto = TLevelDescription();

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
    const std::array<std::string, 4> vGfxName
    {
        "wireless/db_genes.sklt",
        "wireless/db_death.sklt",
        "wireless/db_death.sklt",
        "wireless/db_sec.sklt"
    };
    TBkgPicInfo *mapImg = NULL;
    
    _briefScreen.Clear();
    
    _particles.Clear();

    if ( _lvlPrimevalOwnMap.IsNull() || _lvlPrimevalTypeMap.IsNull() )
        goto ON_ERR;

    _briefScreen.ZoomFromGate = _levelInfo.State == TLevelInfo::STATE_COMPLETED;

    GFX::Engine.raster_func210( Common::FRect(-1.0, -1.0, 1.0, 1.0) );

    _briefScreen.Stage = TBriefengScreen::STAGE_NONE;
    _briefScreen.TimerStatus = TBriefengScreen::TIMER_NORMAL;
    _briefScreen.CurrTime = 0;
    //brief._owner = playerOwner;

    _levelInfo.State = TLevelInfo::STATE_DEBRIEFING;

    Common::Env.SetPrefix("rsrc", "data:");

    for (size_t i = 0; i < vGfxName.size(); i++)
        _briefScreen.VectorGfx[i] = Nucleus::CInit<NC_STACK_sklt>({{NC_STACK_rsrc::RSRC_ATT_NAME, vGfxName[i]}});

    
    _briefScreen.OwnMap = _lvlPrimevalOwnMap;
    _briefScreen.TypMap = _lvlPrimevalTypeMap;

    if ( !ypaworld_func158__sub4__sub1__sub5__sub0(&_briefScreen.Desc, _globalMapRegions.MapRegions[_levelInfo.LevelID].MapDirectory) )
        goto ON_ERR;

    if (_briefScreen.Desc.Dbmaps.empty())
    {
        if (!_briefScreen.Desc.Mbmaps.empty())
            mapImg = &_briefScreen.Desc.Mbmaps[0];
    }
    else
    {
        if (!_briefScreen.Desc.Dbmaps.empty())
            mapImg = &_briefScreen.Desc.Dbmaps[0];
    }

    if ( !mapImg || mapImg->PicName.empty() )
        goto ON_ERR;

    Common::Env.SetPrefix("rsrc", "levels:");

    if ( _globalMapRegions.debrief_map[0].PicName[0] )
        _briefScreen.BriefingMapImg = Utils::ProxyLoadImage( {
            {NC_STACK_rsrc::RSRC_ATT_NAME, _globalMapRegions.debrief_map[0].PicName},
            {NC_STACK_bitmap::BMD_ATT_CONVCOLOR, (int32_t)1}});

    Common::Env.SetPrefix("rsrc", "mbpix:");

    _briefScreen.MbmapImg = Utils::ProxyLoadImage( {
        {NC_STACK_rsrc::RSRC_ATT_NAME, mapImg->PicName},
        {NC_STACK_bitmap::BMD_ATT_CONVCOLOR, (int32_t)1}});
    if ( !_briefScreen.MbmapImg )
        goto ON_ERR;

    SetMapSize(_lvlTypeMap.Size());

    _briefScreen.Stage = TBriefengScreen::STAGE_LOADED;

    return true;
    
ON_ERR:
    FreeDebrief();
    return false;
}


void yw_calcPlayerScore(NC_STACK_ypaworld *yw)
{
    if ( !yw->_gameWasNetGame )
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
                    decoder->AddScore(&yw->_playersStats);
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
            yw->_playersStats[i].ElapsedTime += last_time;

    }
}

void NC_STACK_ypaworld::ypaworld_func158__sub4__sub1()
{
    if ( _levelInfo.State != TLevelInfo::STATE_COMPLETED && _levelInfo.State != TLevelInfo::STATE_ABORTED )
    {
        if ( _briefScreen.Stage )
        {
            if ( _levelInfo.State == TLevelInfo::STATE_BRIEFING )
            {
                if ( _GameShell->Input->KbdLastHit == Input::KC_RETURN )
                    _briefScreen.Stage = TBriefengScreen::STAGE_PLAYLEVEL;

                if ( _briefScreen.Stage == TBriefengScreen::STAGE_PLAYLEVEL )
                {
                    _GameShell->envAction.action = EnvAction::ACTION_PLAY;
                    _GameShell->envAction.params[0] = _levelInfo.LevelID;
                    _GameShell->envAction.params[1] = _levelInfo.LevelID;
                    FreeBriefing();
                }
                else if ( _briefScreen.Stage == TBriefengScreen::STAGE_CANCEL )
                {
                    FreeBriefing();
                    _levelInfo.State = TLevelInfo::STATE_MENU;
                }
                else
                {
                    ypaworld_func158__sub4__sub1__sub4(this, _GameShell, _GameShell->Input);
                }
            }
            else if ( _levelInfo.State == TLevelInfo::STATE_DEBRIEFING )
            {
                if ( _briefScreen.Stage == TBriefengScreen::STAGE_CANCEL )
                {
                    yw_calcPlayerScore(this);
                    FreeDebrief();
                }
                else
                {
                    yw_debriefUpdate(this, _GameShell->Input);
                }
            }
        }
        else
        {
            ypaworld_func158__sub4__sub1__sub0(this, _GameShell->Input);

            if ( _GameShell->EnvMode == ENVMODE_TUTORIAL )
                DrawMapRegionsTutorial();
            else
                DrawMapRegions();


            if ( _globalMapRegions.SelectedRegion )
            {
                if ( _GameShell->Input->ClickInf.flag & TClickBoxInf::FLAG_LM_DOWN )
                {
                    if ( _globalMapRegions.MapRegions[ _globalMapRegions.SelectedRegion ].Status == TMapRegionInfo::STATUS_COMPLETED )
                    {
//            v14 = get_lang_string(ypaworld.string_pointers_p2, 2466, "2466 == THE YPA CD IS REQUIRED.");
//            v15 = get_lang_string(ypaworld.string_pointers_p2, 21, "21 == YOUR PERSONAL AMOK");
//            if ( make_CD_CHECK(0, 1, v15, v14) )
//            {
                        _GameShell->envAction.action = EnvAction::ACTION_PLAY;
                        _GameShell->envAction.params[0] = _globalMapRegions.SelectedRegion;
                        _GameShell->envAction.params[1] = _globalMapRegions.SelectedRegion;
//            }
                    }
                    else
                    {
//            v16 = get_lang_string(ypaworld.string_pointers_p2, 2466, "2466 == THE YPA CD IS REQUIRED.");
//            v17 = get_lang_string(ypaworld.string_pointers_p2, 21, "21 == YOUR PERSONAL AMOK");
//            if ( make_CD_CHECK(0, 1, v17, v16) )
//            {
                        if ( !InitBriefing(_globalMapRegions.SelectedRegion) )// LOAD LEVEL INFO
                        {
                            _GameShell->envAction.action = EnvAction::ACTION_PLAY;
                            _GameShell->envAction.params[0] = _globalMapRegions.SelectedRegion;
                            _GameShell->envAction.params[1] = _globalMapRegions.SelectedRegion;
                        }
//            }
                    }
                }
                else if ( _GameShell->Input->ClickInf.flag & TClickBoxInf::FLAG_RM_DOWN )
                {
//          v18 = get_lang_string(ypaworld.string_pointers_p2, 2466, "2466 == THE YPA CD IS REQUIRED.");
//          v19 = get_lang_string(ypaworld.string_pointers_p2, 21, "21 == YOUR PERSONAL AMOK");
//          if ( make_CD_CHECK(0, 1, v19, v18) )
//          {
                    _GameShell->envAction.action = EnvAction::ACTION_PLAY;
                    _GameShell->envAction.params[0] = _globalMapRegions.SelectedRegion;
                    _GameShell->envAction.params[1] = _globalMapRegions.SelectedRegion;
//          }
                }
            }
        }
    }
    else
    {
        if ( _levelInfo.State == TLevelInfo::STATE_COMPLETED )
        {
            if ( _levelInfo.MovieWinStr[0] )
                sub_4491A0(_levelInfo.MovieWinStr);
        }

        if ( _levelInfo.State == TLevelInfo::STATE_ABORTED )
        {
            if ( _levelInfo.MovieLoseStr[0] )
            {
                if ( _playerHSDestroyed )
                    sub_4491A0(_levelInfo.MovieLoseStr);
            }
        }

        _briefScreen.Stage = TBriefengScreen::STAGE_NONE;
        _levelInfo.State = TLevelInfo::STATE_MENU;

        if ( _prepareDebrief )
            InitDebrief();
    }
}


int NC_STACK_ypaworld::sub_4DA41C(TLevelDescription *mapp, const std::string &fname)
{
    *mapp = TLevelDescription();
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


void sub_4C4284(NC_STACK_ypaworld *yw, GuiList *lstvw, CmdStream *out, const std::string &txt)
{
    FontUA::store_u8(out, 123);
    FontUA::select_tileset(out, 9);

    FontUA::store_u8(out, 98);

    FontUA::set_txtColor(out, yw->_iniColors[62].r, yw->_iniColors[62].g, yw->_iniColors[62].b);

    FontUA::FormateClippedText(yw->_guiTiles[9], out, txt, lstvw->entryWidth - 4 * yw->_fontBorderW, 99);

    FontUA::store_u8(out, 100);

    FontUA::select_tileset(out, 0);

    FontUA::store_u8(out, 125);
    FontUA::next_line(out);
}

void sub_4C41DC(NC_STACK_ypaworld *yw, GuiList *lstvw, CmdStream *out, const std::string &txt)
{
    FontUA::store_u8(out, 123);

    FontUA::set_txtColor(out, yw->_iniColors[61].r, yw->_iniColors[61].g, yw->_iniColors[61].b);

    FontUA::FormateClippedText(yw->_guiTiles[0], out, txt, lstvw->entryWidth - 2 * yw->_fontBorderW, 32);

    FontUA::store_u8(out, 125);
    FontUA::next_line(out);
}

void ypaworld_func158__video_list_draw(NC_STACK_ypaworld *yw, UserData *usr)
{
    usr->video_listvw.itemBlock.clear();

    usr->video_listvw.SetRect(yw, -2, -2);

    usr->video_listvw.ItemsPreLayout(yw, &usr->video_listvw.itemBlock, 0, "uvw");

    int index = 0;
    for (const GFX::GfxMode &nod : GFX::GFXEngine::Instance.GetAvailableModes())
    {
        if ( index >= usr->video_listvw.firstShownEntries && index < usr->video_listvw.firstShownEntries + usr->video_listvw.shownEntries)
        {
            if ( usr->video_listvw.selectedEntry == index )
                sub_4C4284(yw, &usr->video_listvw, &usr->video_listvw.itemBlock, nod.name);
            else
                sub_4C41DC(yw, &usr->video_listvw, &usr->video_listvw.itemBlock, nod.name);
        }
        index++;
    }

    if ( index >= 4 )
        index = 4;

    usr->video_listvw.shownEntries = index;
    usr->video_listvw.ItemsPostLayout(yw, &usr->video_listvw.itemBlock, 0, "xyz");
    FontUA::set_end(&usr->video_listvw.itemBlock);

    GFX::Engine.ProcessDrawSeq(usr->video_listvw.cmdCommands, &usr->video_listvw.cmdInclude);
}

void ypaworld_func158__d3d_list_draw(NC_STACK_ypaworld *yw, UserData *usr)
{
    usr->d3d_listvw.itemBlock.clear();

    usr->d3d_listvw.SetRect(yw, -2, -2);

    usr->d3d_listvw.ItemsPreLayout(yw, &usr->d3d_listvw.itemBlock, 0, "uvw");

    const std::vector<GFX::TGFXDeviceInfo> &devices = GFX::Engine.GetDevices();
    
    usr->d3d_listvw.numEntries = devices.size();
    usr->d3d_listvw.shownEntries = MMIN(4, devices.size());
    int j = MMIN(usr->d3d_listvw.firstShownEntries + usr->d3d_listvw.shownEntries, usr->d3d_listvw.numEntries);
    
    for (int i = usr->d3d_listvw.firstShownEntries; i < j; i++)
    {
        const GFX::TGFXDeviceInfo &dev = devices.at(i);
        
        std::string name;

        if ( !StriCmp(dev.name, "software") )
            name = Locale::Text::Advanced(Locale::ADV_SOFTWARE);
        else
            name = dev.name;

        if ( usr->d3d_listvw.selectedEntry == i )
            sub_4C4284(yw, &usr->d3d_listvw, &usr->d3d_listvw.itemBlock, name);
        else
            sub_4C41DC(yw, &usr->d3d_listvw, &usr->d3d_listvw.itemBlock, name);
    }

    
    usr->d3d_listvw.ItemsPostLayout(yw, &usr->d3d_listvw.itemBlock, 0, "xyz");

    FontUA::set_end(&usr->d3d_listvw.itemBlock);

    GFX::Engine.ProcessDrawSeq(usr->d3d_listvw.cmdCommands, &usr->d3d_listvw.cmdInclude);
}

void sub_4DDF78(NC_STACK_ypaworld *yw, GuiList *lstvw, CmdStream *pos, int a3)
{
    for ( int i = 0; i < a3; i++ )
        sub_4C41DC(yw, lstvw, pos, " ");
}

void ypaworld_func158__network_list_draw(NC_STACK_ypaworld *yw, UserData *usr)
{
    const std::array<std::string, 2> connNames
    { "Host the game"
    , "Connect to game"};
    
    bool slct = false;

    usr->network_listvw.itemBlock.clear();
    
    usr->network_listvw.SetRect(yw, -2, -2);

    usr->network_listvw.ItemsPreLayout(yw, &usr->network_listvw.itemBlock, 0, "uvw");

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
                    usr->network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXTBOX, usr->netName);
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
            if ( yw->_netDriver->GetSessionName(&msg) )
            {
                size_t j;
                for (j = 0; (uint8_t)msg.name[j] >= ' ' && msg.name[j] != '|'; j++ )
                    str1 += msg.name[j];

                int lvlid = std::stoi(str1);

                str1 = yw->GetLevelName(lvlid);

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

                    usr->netName = yw->_globalMapRegions.MapRegions[ lvlid ].MapName;

                    usr->network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXTBOX, usr->netName);

                    usr->netSel = 0;
                }
            }
            else
                str1 = "----";
        }
        break;

        case UserData::NETSCREEN_CHOOSE_MAP: //level
            if ( i < (int)usr->mapDescriptions.size() )
            {
                TMapRegionInfo &lvl = yw->_globalMapRegions.MapRegions[ usr->mapDescriptions[i].id ];

                if ( lvl.RoboCount < yw->_netDriver->GetPlayerCount() )
                    continue;

                str1 = usr->mapDescriptions[i].pstring;

                brk = false;

                str2 = fmt::sprintf("%d X %d", lvl.MapSize.x, lvl.MapSize.y);

                cnt++;

                if ( usr->netSel == cnt )
                {
                    slct = true;

                    if ( lvl.IsFraction(World::OWNER_RESIST) )
                        str3 = "1";

                    if ( lvl.IsFraction(World::OWNER_GHOR) )
                        str3 += "2";

                    if ( lvl.IsFraction(World::OWNER_MYKO) )
                        str3 += "3";

                    if ( lvl.IsFraction(World::OWNER_TAER) )
                        str3 += "4";

                    if ( lvl.SlowConnection )
                        connTp = "Y";
                    else
                        connTp = "j";
                }
                else
                {
                    slct = false;

                    if ( lvl.IsFraction(World::OWNER_RESIST) )
                        str3 = "5";

                    if ( lvl.IsFraction(World::OWNER_GHOR) )
                        str3 += "6";

                    if ( lvl.IsFraction(World::OWNER_MYKO) )
                        str3 += "7";

                    if ( lvl.IsFraction(World::OWNER_TAER) )
                        str3 += "8";

                    if ( lvl.SlowConnection )
                        connTp = "X";
                    else
                        connTp = "k";
                }

                if ( usr->netSel == -1 && cnt == 0)
                {
                    usr->netName = usr->mapDescriptions[ i ].pstring;

                    usr->network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXTBOX, usr->netName);

                    usr->netLevelName = usr->mapDescriptions[ i ].pstring;
                    usr->netLevelID = usr->mapDescriptions[ i ].id;
                    usr->netSel = 0;
                }
            }
            break;

        case 2:
        case 4:
            if ( i < (int)usr->msgBuffers.size() )
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

        if (usr->netSelMode == UserData::NETSCREEN_CHOOSE_MAP)
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

            if ( usr->netSel != i || usr->netSelMode == UserData::NETSCREEN_INSESSION || usr->netSelMode == UserData::NETSCREEN_ENTER_NAME )
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

            int wdth = usr->network_listvw.entryWidth - 2 * yw->_fontBorderW;
            int itemsCount = 0;

            switch(usr->netSelMode)
            {
            case UserData::NETSCREEN_MODE_SELECT:
                items[0].fontID = fontID;
                items[0].spaceChar = spaceChar;
                items[0].prefixChar = prefixChar;
                items[0].postfixChar = postfixChar;
                items[0].txt = str1;
                items[0].flags = 0x27;
                items[0].width = wdth;
                itemsCount = 1;
                break;

            case UserData::NETSCREEN_SESSION_SELECT:
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

            case UserData::NETSCREEN_CHOOSE_MAP:
            {
                TileMap *tiles = GFX::Engine.GetTileset(8);

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

            case UserData::NETSCREEN_ENTER_NAME:
            case UserData::NETSCREEN_INSESSION:
                items[0].width = usr->network_listvw.entryWidth - 2 * usr->p_YW->_fontBorderW;
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

            FontUA::select_tileset(&usr->network_listvw.itemBlock, 0);
            FontUA::store_u8(&usr->network_listvw.itemBlock, 123);

            if ( usr->netSel != i || usr->netSelMode == UserData::NETSCREEN_INSESSION || usr->netSelMode == UserData::NETSCREEN_ENTER_NAME )
                FontUA::set_txtColor(&usr->network_listvw.itemBlock, yw->_iniColors[61].r, yw->_iniColors[61].g, yw->_iniColors[61].b );
            else
                FontUA::set_txtColor(&usr->network_listvw.itemBlock, yw->_iniColors[62].r, yw->_iniColors[62].g, yw->_iniColors[62].b );

            FontUA::FormateColumnItem(yw, &usr->network_listvw.itemBlock, itemsCount, items);

            FontUA::select_tileset(&usr->network_listvw.itemBlock, 0);
            FontUA::store_u8(&usr->network_listvw.itemBlock, 125);
            FontUA::next_line(&usr->network_listvw.itemBlock);
        }

        if (usr->netSelMode == UserData::NETSCREEN_CHOOSE_MAP)
        {
            int tmp = i;
            i = cnt;
            cnt = tmp;
        }
    }

    if ( usr->netSelMode == UserData::NETSCREEN_CHOOSE_MAP)
        i = cnt + 1;

    if (usr->network_listvw.maxShownEntries < i)
        usr->network_listvw.shownEntries = usr->network_listvw.maxShownEntries;
    else
        usr->network_listvw.shownEntries = i;

    if ( usr->network_listvw.maxShownEntries > usr->network_listvw.shownEntries )
        sub_4DDF78(yw, &usr->network_listvw, &usr->network_listvw.itemBlock, usr->network_listvw.maxShownEntries - usr->network_listvw.shownEntries);

    usr->network_listvw.numEntries = i;

    usr->network_listvw.ItemsPostLayout(yw, &usr->network_listvw.itemBlock, 0, "xyz");
    FontUA::set_end(&usr->network_listvw.itemBlock);

    GFX::Engine.ProcessDrawSeq(usr->network_listvw.cmdCommands, &usr->network_listvw.cmdInclude);
}

void ypaworld_func158__locale_list_draw(NC_STACK_ypaworld *yw, UserData *usr)
{
    usr->local_listvw.itemBlock.clear();

    usr->local_listvw.SetRect(yw, -2, -2);

    usr->local_listvw.ItemsPreLayout(yw, &usr->local_listvw.itemBlock, 0, "uvw");

    int v5 = 0;
    for( const auto &x : usr->lang_dlls )
    {
        if ( v5 >= usr->local_listvw.firstShownEntries && v5 < usr->local_listvw.shownEntries + usr->local_listvw.firstShownEntries)
        {
            if ( v5 == usr->local_listvw.selectedEntry )
                sub_4C4284(yw, &usr->local_listvw, &usr->local_listvw.itemBlock, x);
            else
                sub_4C41DC(yw, &usr->local_listvw, &usr->local_listvw.itemBlock, x);
        }

        v5++;
    }

    if ( v5 > 10 )
        usr->local_listvw.shownEntries = 10;
    else
        usr->local_listvw.shownEntries = v5;

    usr->local_listvw.numEntries = usr->local_listvw.shownEntries;

    if ( usr->local_listvw.shownEntries < 10 )
        sub_4DDF78(yw, &usr->local_listvw, &usr->local_listvw.itemBlock, 10 - usr->local_listvw.shownEntries);

    usr->local_listvw.ItemsPostLayout(yw, &usr->local_listvw.itemBlock, 0, "xyz");

    FontUA::set_end(&usr->local_listvw.itemBlock);

    GFX::Engine.ProcessDrawSeq(usr->local_listvw.cmdCommands, &usr->local_listvw.cmdInclude);
}

void ypaworld_func158__saveload_list_draw(NC_STACK_ypaworld *yw, UserData *usr)
{
    usr->disk_listvw.itemBlock.clear();

    usr->disk_listvw.SetRect(yw, -2, -2);

    usr->disk_listvw.ItemsPreLayout(yw, &usr->disk_listvw.itemBlock, 0, "uvw");

    int v54 = 0;

    for ( ProfileList::iterator it = usr->profiles.begin(); it != usr->profiles.end(); it++ )
    {
        if ( v54 >= usr->disk_listvw.firstShownEntries && v54 < usr->disk_listvw.shownEntries + usr->disk_listvw.firstShownEntries )
        {
            FontUA::ColumnItem v31[2];

            int v8, v9, v45, v44;

            if ( v54 + 1 == usr->diskListActiveElement )
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

            int v37 = usr->disk_listvw.entryWidth - 2 * usr->p_YW->_fontBorderW;

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

            FontUA::select_tileset(&usr->disk_listvw.itemBlock, 0);

            FontUA::store_u8(&usr->disk_listvw.itemBlock, 123);

            if ( !StriCmp(it->name, usr->UserName) )
            {
                FontUA::set_txtColor(&usr->disk_listvw.itemBlock, yw->_iniColors[2].r, yw->_iniColors[2].g, yw->_iniColors[2].b);
            }
            else
            {
                if ( v54 + 1 == usr->diskListActiveElement )
                {
                    FontUA::set_txtColor(&usr->disk_listvw.itemBlock, yw->_iniColors[62].r, yw->_iniColors[62].g, yw->_iniColors[62].b);
                }
                else
                {
                    FontUA::set_txtColor(&usr->disk_listvw.itemBlock, yw->_iniColors[61].r, yw->_iniColors[61].g, yw->_iniColors[61].b);
                }
            }

            FormateColumnItem(yw, &usr->disk_listvw.itemBlock, 2, v31);

            FontUA::select_tileset(&usr->disk_listvw.itemBlock, 0);

            FontUA::store_u8(&usr->disk_listvw.itemBlock, 125);

            FontUA::next_line(&usr->disk_listvw.itemBlock);
        }

        v54++;
    }

    if ( v54 <= 10 )
    {
        usr->disk_listvw.shownEntries = v54;

        for (int i = 0; i < 10 - v54; i++)
        {
            sub_4C41DC(yw, &usr->disk_listvw, &usr->disk_listvw.itemBlock, " ");
        }
    }

    usr->disk_listvw.numEntries = v54;
    usr->disk_listvw.ItemsPostLayout(yw, &usr->disk_listvw.itemBlock, 0, "xyz");

    FontUA::set_end(&usr->disk_listvw.itemBlock);

    GFX::Engine.ProcessDrawSeq(usr->disk_listvw.cmdCommands, &usr->disk_listvw.cmdInclude);
}

void sb_0x4dee74__sub0(UserData *usr, int x1, int y1, int w, int h)
{
    CmdStream buf;
    buf.reserve(300);

    FontUA::select_tileset(&buf, 0);

    FontUA::set_center_xpos(&buf, x1 - usr->p_YW->_screenSize.x / 2);
    FontUA::set_center_ypos(&buf, y1 - usr->p_YW->_screenSize.y / 2);

    int v9 = 1;

    int v6 = h;

    TileMap *v7 = GFX::Engine.GetTileset(0);

    while ( v6 > v7->h )
    {
        if ( v9 )
            FontUA::store_u8(&buf, 117);
        else
            FontUA::store_u8(&buf, 123);

        FontUA::op17(&buf, w);

        if ( v9 )
            FontUA::store_u8(&buf, 118);
        else
            FontUA::store_u8(&buf, 123);

        if ( v9 )
            FontUA::store_u8(&buf, 119);
        else
            FontUA::store_u8(&buf, 125);

        FontUA::next_line(&buf);

        v9 = 0;

        v6 -= v7->h;
    }

    FontUA::set_yoff(&buf, v7->h - 1);

    FontUA::store_u8(&buf, 120);

    FontUA::op17(&buf, w);

    FontUA::store_u8(&buf, 121);
    FontUA::store_u8(&buf, 122);

    FontUA::set_end(&buf);

    GFX::Engine.ProcessDrawSeq(buf);
}

void ypaworld_func158__confirm_draw(UserData *usr)
{
    NC_STACK_ypaworld *yw = usr->p_YW;

    if ( usr->confirmMode )
    {
        int v4 = yw->_screenSize.y * 0.21875;
        int v6 = yw->_screenSize.x * 0.53125;
        int v7 = yw->_screenSize.y * 0.3854166666666667;
        int v8 = yw->_screenSize.x * 0.234375;

        sb_0x4dee74__sub0(usr, v8, v7, v6, v4);

        usr->confirm_button->Draw();
    }
}

void ypaworld_func158__sub3(NC_STACK_ypaworld *yw, UserData *usr)
{
    switch ( usr->EnvMode )
    {
    case ENVMODE_TITLE:
        usr->titel_button->Draw();
        break;

    case ENVMODE_INPUT:
        usr->button_input_button->Draw();
        yw_draw_input_list(yw, usr);
        break;

    case ENVMODE_SETTINGS:
        usr->video_button->Draw();

        if ( usr->video_listvw.IsOpen() )
            ypaworld_func158__video_list_draw(yw, usr);

        if ( usr->d3d_listvw.IsOpen() )
            ypaworld_func158__d3d_list_draw(yw, usr);
        break;

    case ENVMODE_TUTORIAL:
    case ENVMODE_SINGLEPLAY:
        usr->sub_bar_button->Draw();
        break;

    case ENVMODE_NETPLAY:
        usr->network_button->Draw();

        if ( usr->netSelMode != UserData::NETSCREEN_ENTER_NAME && usr->netSelMode != UserData::NETSCREEN_ENTER_IP)
            ypaworld_func158__network_list_draw(yw, usr);
        break;

    case ENVMODE_SELLOCALE:
        usr->locale_button->Draw();
        ypaworld_func158__locale_list_draw(yw, usr);
        break;

    case ENVMODE_ABOUT:
        usr->about_button->Draw();
        break;

    case ENVMODE_SELPLAYER:
        usr->disk_button->Draw();
        ypaworld_func158__saveload_list_draw(yw, usr);
        break;

    default:
        break;
    }
    ypaworld_func158__confirm_draw(usr);
}

void yw_freeTileSets(NC_STACK_ypaworld *yw)
{
    for ( auto &x : yw->_guiTiles )
    {
        delete x;
        x = NULL;
    }
}

void NC_STACK_ypaworld::FreeGameDataCursors()
{
    GFX::displ_arg263 v5;
    v5.bitm = NULL;
    v5.pointer_id = 0;

    GFX::Engine.SetCursor(v5.pointer_id, 0);

    for (NC_STACK_bitmap* &point : _mousePointers)
    {
        if ( point )
        {
            point->Delete();
            point = NULL;
        }
    }
}

void sb_0x44ac24(NC_STACK_ypaworld *yw)
{
    yw->_vhclModels.clear();

    yw->FreeLegos();

    yw->FreeFillers();
    
    yw->ClearOverrideModels();
    

    for (TSubSectorDesc &ssec : yw->_subSectorArray)
        ssec = TSubSectorDesc();
    
    for (TSectorDesc &styp : yw->_secTypeArray)
        styp = TSectorDesc();

    if ( yw->_beeBox )
    {
        yw->_beeBox->Delete();
        yw->_beeBox = NULL;
    }

    if ( yw->_colsubSkeleton )
        yw->_colsubSkeleton->Delete();

    if ( yw->_colcompSkeleton )
        yw->_colcompSkeleton->Delete();

    if ( yw->_tracyBitmap )
    {
        yw->_tracyBitmap->Delete();
        yw->_tracyBitmap = NULL;
    }

    if ( yw->_shadermpBitmap )
    {
        yw->_shadermpBitmap->Delete();
        yw->_shadermpBitmap = NULL;
    }

    yw_freeTileSets(yw);
    yw->UpdateGuiSettings();

    if ( yw->_setData )
    {
        yw->_setData->Delete();
        yw->_setData = NULL;
        yw->_setId = 0;
    }
}

void UserData::clear()
{

    returnToTitle = false;

    for (NC_STACK_sample * &smpl : samples1)
        smpl = NULL;

    sub_bar_button = NULL;
    
    titel_button = NULL;
    button_input_button = NULL;

    inpListActiveElement = 0;
    confFirstKey = false;
    joystickEnabled = false;
    confJoystickEnabled = false;
    altJoystickEnabled = false;
    confAltJoystickEnabled = false;
    confFFEnabled = false;
    keyCatchMode = false;

    inputChangedParts = 0;
    video_button = NULL;
    //GuiList video_listvw;
    game_default_res = 0;
    //nlist video_mode_list;
    _gfxModeIndex = 0;
    //GuiList d3d_listvw;
    win3d_guid.clear();
    win3d_name.clear();

    conf3DGuid.clear();
    conf3DName.clear();
    
    fxnumber = 0;
    confFxNumber = 0;
    GFXFlags = 0;
    confGFXFlags = 0;

    soundFlags = 0;

    confSoundFlags = 0;
    soundVolume = 0;
    confSoundVolume = 0;
    musicVolume = 0;
    confMusicVolume = 0;
    enemyIndicator = false;
    confEnemyIndicator = false;
    _settingsChangeOptions = 0;
    disk_button = NULL;
    //GuiList disk_listvw;
    diskListActiveElement = 0;
    userNameDir.clear();
    diskScreenMode = 0;
    //opened_dir = NULL;
    //nlist files_list;

    savedDataFlags = 0;

    diskEnterFromMapSelect = false;
    locale_button = NULL;
    //nlist lang_dlls;
    //GuiList local_listvw;
    default_lang_dll = NULL;
    prev_lang = NULL;

    about_button = NULL;
    aboutDlgLastKeyTime = 0;
    aboutDlgKeyCount = 0;

    network_button = NULL;
    //GuiList network_listvw;

    netListY = 0;

    netSelMode = NETSCREEN_MODE_SELECT;
    netSel = 0;
    nInputMode = 0;
    netName.clear();

    netNameCurPos = 0;
    netLevelID = 0;
    netLevelName.clear();

    netPlayerName.clear();

    netCRC = 0;
    takTime = 0;
    netPlayerOwner = 0;
    FreeFraction = 0;
    SelectedFraction = 0;
    netGameCanStart = false;
    isHost = false;
    modemAskSession = 0;

    msgcount = 0;
    isWelcmd = false;
    rdyStart = false;
    remoteMode = false;
    disconnected = false;
    blocked = false;
    problemTimer = 0;

    msgBuffers.clear();
    lastSender.clear();
    
    mapDescriptions.clear();
    
    noSent = false;
    sentAQ = false;
    netProblemOwner = 0;
    netProblemName.clear();
    netProblem = 0;
    netAllOk = 0;
    update_time_norm = 0;
    flush_time_norm = 0;
    kickTime = 0;
    latencyCheck = 0;
    netProblemCountDown = 0;
    netAllOkCountDown = 0;
    deadCheckTime = 0;
    sendScoreCountDown = 0;
    
    netPlayers.fill(TNetPlayerData());
    
    lobbyPlayers.fill(TNetPlayerLobbyData());
    
    confirm_button = NULL;
    confirmMode = 0;

    for(std::string &str : InputConfigTitle)
        str.clear();
    
    for(TInputConf &cfg : InputConfig)
        cfg = TInputConf();

    sgmSaveExist = 0;
    shelltrack = 0;
    missiontrack = 0;
    loadingtrack = 0;
    debriefingtrack = 0;
//     audiotrack_adv shelltrack__adv;
//     audiotrack_adv missiontrack__adv;
//     audiotrack_adv loadingtrack__adv;
//     audiotrack_adv debriefingtrack__adv;
    
    lastInputEvent = 0;

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


void NC_STACK_ypaworld::ClearOverrideModels()
{
    while(!_overrideModels.empty())
    {
        _overrideModels.front()->Delete();
        _overrideModels.pop_front();
    }
}

void NC_STACK_ypaworld::LoadOverrideModels()
{
    FSMgr::FileHandle fil = uaOpenFile("rsrc:scripts/override.lst", "r");
    if ( !fil.OK() )
        return;

    std::string line;
    while ( fil.ReadLine(&line) && !line.empty() )
    {
        size_t pos = line.find_first_of(";#\n\r");
        if ( pos != std::string::npos )
            line.erase(pos);

        if (line.empty())
            continue;
        
        std::vector<std::string> tokens = Stok::Split(line, " \t");
        
        if (tokens.size() < 3)
            continue;
        
        std::string basName = fmt::sprintf("rsrc:objects/%s", tokens[2]);

        NC_STACK_base *model = Utils::ProxyLoadBase(basName);
        
        if ( !model )
            continue;
        
        model->SetVizLimit(_normalVizLimit);
        model->SetFadeLength(_normalFadeLength);
        model->MakeVBO();
        
        uint32_t id = std::stoi(tokens[1]);
        
        if (!StriCmp(tokens[0], "vhcl"))
        {
            if (id < _vhclModels.size())
                _vhclModels[id] = model;
            else if (id == _vhclModels.size())
                _vhclModels.push_back(model);
        }
        else if (!StriCmp(tokens[0], "lego"))
        {
            if (id < _legoArray.size())
                _legoArray[id].Base = model;
        }
        else if (!StriCmp(tokens[0], "slurp"))
        {
            if (id < 6 * 6 * 2)
            {
                int32_t i = id / (6 * 6);
                id = id % (6 * 6);
                
                if (i == 0)
                    _fillersHorizontal(id / 6 , id % 6) = model;
                else if (i == 1)
                    _fillersVertical(id / 6 , id % 6) = model;
                
            }
        }
        else
            model->Delete();
    }
}