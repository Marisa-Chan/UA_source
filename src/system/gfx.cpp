#include "../fmtlib/printf.h"
#include "gfx.h"
#include "../utils.h"
#include "../common.h"
#include "../ini.h"
#include "../gui/root.h"
#include "../bitmap.h"
#include "../ilbm.h"
#include "../display.h"
#include "../win3d.h"
#include "../log.h"
#include "../gui/oldcompat.h"

namespace GFX
{
    GFXEngine GFXEngine::Instance;
}

Common::Ini::KeyList gfx_keys
{
    Common::Ini::Key("gfx.mode",     Common::Ini::KT_DIGIT),
    Common::Ini::Key("gfx.xres",     Common::Ini::KT_DIGIT),
    Common::Ini::Key("gfx.yres",     Common::Ini::KT_DIGIT),
    Common::Ini::Key("gfx.palette",  Common::Ini::KT_WORD),
    Common::Ini::Key("gfx.display",  Common::Ini::KT_WORD),
    Common::Ini::Key("gfx.display2", Common::Ini::KT_WORD)
};


TileMap::TileMap()
{
    img = NULL;
    h = 0;
}
    
TileMap::~TileMap()
{
    if (img)
        Nucleus::Delete(img);
}

void TileMap::Draw(SDL_Surface *surface, const Common::Point &pos, uint8_t c)
{
    SDL_Rect src = map[c];
    SDL_Rect dst = pos;
    SDL_BlitSurface(img->GetSwTex(), &src, surface, &dst);
}

void TileMap::Draw(SDL_Surface *surface, const Common::PointRect &pos, uint8_t c)
{
    SDL_Rect src = map[c];
    if (src.w > pos.w)
        src.w = pos.w;
    if (src.h > pos.h)
        src.h = pos.h;    
    SDL_Rect dst = pos;
    SDL_BlitSurface(img->GetSwTex(), &src, surface, &dst);
}

void TileMap::Draw(SDL_Surface *surface, const Common::Rect &pos, uint8_t c)
{
    SDL_Rect src = map[c];
    if (src.w > pos.Width())
        src.w = pos.Width();
    if (src.h > pos.Height())
        src.h = pos.Height();    
    SDL_Rect dst = pos;
    SDL_BlitSurface(img->GetSwTex(), &src, surface, &dst);
}

void TileMap::Fill(SDL_Surface *surface, const Common::Rect &rect, uint8_t c)
{
    GFX::Engine.DrawFill(img->GetSwTex(), map[c], surface, rect);
}

void TileMap::Fill(SDL_Surface *surface, const Common::PointRect &rect, uint8_t c)
{
    GFX::Engine.DrawFill(img->GetSwTex(), map[c], surface, rect);
}

void TileMap::FillColumn(SDL_Surface *surface, const Common::Rect &rect, uint8_t c)
{
    Common::PointRect sPRect = map[c];
    sPRect.w = 1;
    GFX::Engine.DrawFill(img->GetSwTex(), sPRect, surface, rect);
}

void TileMap::FillColumn(SDL_Surface *surface, const Common::PointRect &rect, uint8_t c)
{
    Common::PointRect sPRect = map[c];
    sPRect.w = 1;
    GFX::Engine.DrawFill(img->GetSwTex(), sPRect, surface, rect);
}

int TileMap::GetWidth(uint8_t c) const
{
    return map[c].w;
}

int TileMap::GetWidth(const std::string &str) const
{
    int wdth = 0;
    for (uint8_t c : str)
        wdth += map[c].w;
    return wdth;
}

Common::Point TileMap::GetSize(uint8_t c) const
{
    return Common::Point(map[c].w, h);
}




TFEngine TFEngine::Engine;

Common::Ini::KeyList tform_keys
{
    Common::Ini::Key("tform.backplane",  Common::Ini::KT_DIGIT, (int32_t)4096),
    Common::Ini::Key("tform.frontplane", Common::Ini::KT_DIGIT, (int32_t)16),
    Common::Ini::Key("tform.zoomx",      Common::Ini::KT_DIGIT, (int32_t)320),
    Common::Ini::Key("tform.zoomy",      Common::Ini::KT_DIGIT, (int32_t)200)
};

int TFEngine::init()
{
    Common::Ini::ParseIniFile(NC_STACK_nucleus::DefaultIniFile, &tform_keys);

    SinCos_table = new SinCos[361];

    if ( !SinCos_table)
        return 0;

    for (int i = 0; i < 361; i++)
    {
        float rad = i * C_PI_180;

        SinCos_table[i].sin = sin(rad);
        SinCos_table[i].cos = cos(rad);
    }

    return 1;
}

void TFEngine::deinit()
{
    if (SinCos_table)
        delete[] SinCos_table;

    SinCos_table = NULL;
}



// rot_x == 0   rot_y == 0   rot_z == 0
void TFEngine::TForm3D::scale_rot_0()
{
    locSclRot.m00 = scale.x;
    locSclRot.m01 = 0;
    locSclRot.m02 = 0;

    locSclRot.m10 = 0;
    locSclRot.m11 = scale.y;
    locSclRot.m12 = 0;

    locSclRot.m20 = 0;
    locSclRot.m21 = 0;
    locSclRot.m22 = scale.z;
}

// rot_x != 0   rot_y == 0   rot_z == 0
void TFEngine::TForm3D::scale_rot_1()
{
    SinCos &angl = Engine.Angle(ax >> 16);

    locSclRot.m00 = scale.x;
    locSclRot.m01 = 0;
    locSclRot.m02 = 0;

    locSclRot.m10 = 0;
    locSclRot.m11 = scale.y *  angl.cos;
    locSclRot.m12 = scale.y * -angl.sin;

    locSclRot.m20 = 0;
    locSclRot.m21 = scale.z * angl.sin;
    locSclRot.m22 = scale.z * angl.cos;

}

// rot_x == 0   rot_y != 0   rot_z == 0
void TFEngine::TForm3D::scale_rot_2()
{
    SinCos &angl = Engine.Angle(ay >> 16);

    locSclRot.m00 = scale.x * angl.cos;
    locSclRot.m01 = 0;
    locSclRot.m02 = scale.x * angl.sin;

    locSclRot.m10 = 0;
    locSclRot.m11 = scale.y;
    locSclRot.m12 = 0;

    locSclRot.m20 = -angl.sin * scale.z;
    locSclRot.m21 = 0;
    locSclRot.m22 =  angl.cos * scale.z;
}

// rot_x != 0   rot_y != 0   rot_z == 0
void TFEngine::TForm3D::scale_rot_3()
{
    SinCos &angl_x = Engine.Angle(ax >> 16);
    SinCos &angl_y = Engine.Angle(ay >> 16);

    locSclRot.m00 = scale.x * angl_y.cos;
    locSclRot.m01 = 0;
    locSclRot.m02 = scale.x * angl_y.sin;

    locSclRot.m10 =  angl_x.sin * angl_y.sin * scale.y;
    locSclRot.m11 =   scale.y * angl_x.cos;
    locSclRot.m12 = -angl_x.sin * angl_y.cos * scale.y;

    locSclRot.m20 = -angl_x.cos * angl_y.sin * scale.z;
    locSclRot.m21 =   scale.z * angl_x.sin;
    locSclRot.m22 =  angl_x.cos * angl_y.cos * scale.z;
}

// rot_x == 0   rot_y == 0   rot_z != 0
void TFEngine::TForm3D::scale_rot_4()
{
    TFEngine::SinCos &angl = TFEngine::Engine.Angle(az >> 16);

    locSclRot.m00 =  angl.cos * scale.x;
    locSclRot.m01 = -angl.sin * scale.x;
    locSclRot.m02 = 0;

    locSclRot.m10 = scale.y * angl.sin;
    locSclRot.m11 = scale.y * angl.cos;
    locSclRot.m12 = 0;

    locSclRot.m20 = 0;
    locSclRot.m21 = 0;
    locSclRot.m22 = scale.z;
}

// rot_x != 0   rot_y == 0   rot_z != 0
void TFEngine::TForm3D::scale_rot_5()
{
    SinCos &angl_x = Engine.Angle(ax >> 16);
    SinCos &angl_z = Engine.Angle(az >> 16);

    locSclRot.m00 =   scale.x * angl_z.cos;
    locSclRot.m01 = -angl_z.sin * angl_x.cos * scale.x;
    locSclRot.m02 =  angl_z.sin * angl_x.sin * scale.x;

    locSclRot.m10 =   scale.y * angl_z.sin;
    locSclRot.m11 =  angl_z.cos * angl_x.cos * scale.y;
    locSclRot.m12 = -angl_z.cos * angl_x.sin * scale.y;

    locSclRot.m20 = 0;
    locSclRot.m21 = scale.z * angl_x.sin;
    locSclRot.m22 = scale.z * angl_x.cos;
}

// rot_x == 0   rot_y != 0   rot_z != 0
void TFEngine::TForm3D::scale_rot_6()
{
    SinCos &angl_y = Engine.Angle(ay >> 16);
    SinCos &angl_z = Engine.Angle(az >> 16);

    locSclRot.m00 =  angl_z.cos * angl_y.cos * scale.x;
    locSclRot.m01 = -angl_z.sin * scale.x;
    locSclRot.m02 =  angl_z.cos * angl_y.sin * scale.x;

    locSclRot.m10 =  angl_z.sin * angl_y.cos * scale.y;
    locSclRot.m11 =  angl_z.cos * scale.y;
    locSclRot.m12 =  angl_z.sin * angl_y.sin * scale.y;

    locSclRot.m20 = -angl_y.sin * scale.z;
    locSclRot.m21 = 0;
    locSclRot.m22 =  angl_y.cos * scale.z;

}

// rot_x != 0   rot_y != 0   rot_z != 0
void TFEngine::TForm3D::scale_rot_7()
{
    SinCos &angl_x = Engine.Angle(ax >> 16);
    SinCos &angl_y = Engine.Angle(ay >> 16);
    SinCos &angl_z = Engine.Angle(az >> 16);


    locSclRot.m00 = (angl_z.cos * angl_y.cos - angl_z.sin * angl_x.sin * angl_y.sin) * scale.x;
    locSclRot.m01 = -angl_z.sin * angl_x.cos * scale.x;
    locSclRot.m02 = (angl_z.cos * angl_y.sin + angl_z.sin * angl_x.sin * angl_y.cos) * scale.x;

    locSclRot.m10 = (angl_z.sin * angl_y.cos + angl_z.cos * angl_x.sin * angl_y.sin) * scale.y;
    locSclRot.m11 =  angl_z.cos * angl_x.cos * scale.y;
    locSclRot.m12 = (angl_z.sin * angl_y.sin - angl_z.cos * angl_x.sin * angl_y.cos) * scale.y;

    locSclRot.m20 = -angl_x.cos * angl_y.sin * scale.z;
    locSclRot.m21 =  angl_x.sin * scale.z;
    locSclRot.m22 =  angl_x.cos * angl_y.cos * scale.z;
}


void TFEngine::TForm3D::MakeScaleRotationMatrix()
{
    typedef void (TFEngine::TForm3D::*matrfunc)();
    const static matrfunc pTFormFunc[8] = {&TFEngine::TForm3D::scale_rot_0,
                                           &TFEngine::TForm3D::scale_rot_1,
                                           &TFEngine::TForm3D::scale_rot_2,
                                           &TFEngine::TForm3D::scale_rot_3,
                                           &TFEngine::TForm3D::scale_rot_4,
                                           &TFEngine::TForm3D::scale_rot_5,
                                           &TFEngine::TForm3D::scale_rot_6,
                                           &TFEngine::TForm3D::scale_rot_7
                                          };

    uint8_t func_id = 0;

    if ( ax )
        func_id |= 1;

    if ( ay )
        func_id |= 2;

    if ( az )
        func_id |= 4;

    (this->*(pTFormFunc[func_id]))();
}


void TFEngine::TForm3D::CalcGlobal()
{
    if ( parent_1c && (flags & TFEngine::TForm3D::FLAG_FOLLOW_PARENT) )
    {
        globPos = parent_1c->globSclRot.Transform( locPos );
        globSclRot = parent_1c->globSclRot * locSclRot;

        mat4x4 loc = locSclRot;
        loc += locPos;
        tform = loc * tform;
    }
    else
    {
        globPos = locPos;
        globSclRot = locSclRot;

        tform = locSclRot;
        tform += locPos;
    }
}







namespace GFX
{
    
GfxMode::GfxMode()
{
    w = 0;
    h = 0;
    bpp = 0;
    mode = {0};
    windowed = false;
    sortid = 0;
    name = "";
}

GfxMode::GfxMode(GfxMode &&g)
{
    w = std::move(g.w);
    h = std::move(g.h);
    bpp = std::move(g.bpp);
    mode = std::move(g.mode);
    windowed = std::move(g.windowed);
    sortid = std::move(g.sortid);
    name = std::move(g.name);
}

GfxMode::GfxMode(const GfxMode &g)
{
    w = g.w;
    h = g.h;
    bpp = g.bpp;
    mode = g.mode;
    windowed = g.windowed;
    sortid = g.sortid;
    name = g.name;
}

GfxMode& GfxMode::operator=(const GfxMode &g)
{
    w = g.w;
    h = g.h;
    bpp = g.bpp;
    mode = g.mode;
    windowed = g.windowed;
    sortid = g.sortid;
    name = g.name;
    return *this;
}

GfxMode::operator bool() const
{
    if (w == 0 || h == 0 || bpp == 0)
        return false;
    return true;
}

bool GfxMode::Compare(const GfxMode &a, const GfxMode &b)
{
    return (a.sortid > b.sortid);
}




void GFXEngine::AddGfxMode(const GfxMode &md)
{
    for (const GfxMode &m : graphicsModes)
    {
        if ( m.sortid == md.sortid )
            return;
    }

    graphicsModes.push_back(md);
    log_d3dlog("Add display mode: %s%dx%d\n", (md.windowed ? "Windowed ": ""), md.w, md.h );
}


uint32_t GFXEngine::CursPix(uint8_t *data, int ofs, int bpp)
{
    switch (bpp)
    {
    case 1:
        return (data[ofs / 8] >> (7 - ofs % 8)) & 1;
    case 2:
        return (data[ofs / 4] >> ((3 - ofs % 4) << 1)) & 3;
    case 4:
        return (data[ofs / 2] >> ((1 - ofs % 2) << 2)) & 15;
    case 8:
        return data[ofs];
    case 16:
        return data[2 * ofs] | data[2 * ofs + 1] << 8;
    case 24:
        return data[3 * ofs] | data[3 * ofs + 1] << 8 | data[ 3 * ofs + 2] << 16;
    case 32:
        return data[4 * ofs] | data[4 * ofs + 1] << 8 | data[4 * ofs + 2] << 16 | data[4 * ofs + 3] << 24;
    }

    return 0;
}

SDL_Cursor *GFXEngine::LoadCursor(const std::string &name)
{
    FSMgr::FileHandle *fil = uaOpenFile( fmt::sprintf("res/%s.cur", name) , "rb");

    UA_PALETTE pal;

    if (!fil)
        return NULL;

    fil->readU16L();
    if (fil->readU16L() != 2)
    {
        delete fil;
        return NULL;
    }

    fil->readU16L(); //count

    //Only first entry
    fil->readU8(); //w
    fil->readU8(); //h
    fil->readU8(); //color count
    fil->readU8(); //reserved
    int hotX = fil->readU16L();
    int hotY = fil->readU16L();
    fil->readU32L(); //size
    int off = fil->readU32L();

    //seek to cursor
    fil->seek(off, SEEK_SET);

    //read InfoHeader
    fil->readU32L(); //header size
    int bmpw = fil->readS32L();
    int bmph = fil->readS32L();
    fil->readU16L(); //planes
    int bitcount = fil->readU16L();
    fil->readU32L(); //compression
    fil->readU32L(); //imagesize
    fil->readU32L(); //XpixelsPerM
    fil->readU32L(); //YpixelsPerM
    int clrused = fil->readU32L(); //ColorsUsed
    fil->readU32L(); //ColorsImportant

    // read pallete
    int palcnt = 0;
    if (clrused == 0 || bitcount < 16)
    {
        palcnt = clrused != 0 ? clrused : 1 << bitcount;

        for (int i = 0; i < palcnt; i++)
        {
            pal[i].r = fil->readU8();
            pal[i].g = fil->readU8();
            pal[i].b = fil->readU8();
            fil->readU8(); //reserved
        }
    }

    int width = bmpw;
    int height = abs(bmph)/2;

    int imgsz = height * width * bitcount / 8;
    int mask_size = height * width / 8;

    uint8_t *data = (uint8_t *)malloc(imgsz);
    uint8_t *mask = (uint8_t *)malloc(mask_size);

    fil->read(data, imgsz);
    fil->read(mask, mask_size);

    delete fil;


    SDL_Surface *cursr = SDL_CreateRGBSurface(0, width, height, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);

    for (int y = 0; y < height; y++)
    {
        int invY = height - 1 - y;
        uint8_t *row = (uint8_t *)cursr->pixels + y * cursr->pitch;

        for (int x = 0; x < width; x++)
        {
            if (palcnt > 0)
            {
                int idx = CursPix(data, invY * width + x, bitcount);
                int alph = CursPix(mask, invY * width + x, 1);
                row[x * 4 + 0] = pal[ idx ].r;
                row[x * 4 + 1] = pal[ idx ].g;
                row[x * 4 + 2] = pal[ idx ].b;
                row[x * 4 + 3] = (1 - alph) * 255;
            }
            else
            {
                uint32_t clr = CursPix(data, invY * width + x, bitcount);
                int alph = CursPix(mask, invY * width + x, 1);
                row[x * 4 + 0] = clr & 0xFF;
                row[x * 4 + 1] = (clr >> 8) & 0xFF;
                row[x * 4 + 2] = (clr >> 16) & 0xFF;
                row[x * 4 + 3] = (1 - alph) * 255;
            }
        }
    }

    free(data);
    free(mask);

    SDL_Cursor* cursor = SDL_CreateColorCursor(cursr, hotX, hotY);

    SDL_FreeSurface(cursr);

    return cursor;
}

void GFXEngine::Init()
{
    RecreateScreenSurface();
    System::EventsAddHandler(EventsWatcher);
    
    Common::Ini::ParseIniFile(NC_STACK_nucleus::DefaultIniFile, &gfx_keys);

    
    SDL_DisplayMode deskMode;
    SDL_GetDesktopDisplayMode(0, &deskMode);

    int checkmodes[][2] =
    {
        {640, 480}, {800, 600}, {1024, 768}, {1280, 1024}, {1440, 1050}, {1600, 1200},
        {720, 480}, {852, 480}, {1280, 720}, {1366, 768}, {1600, 900}, {1920, 1080}, {2560, 1440},
        {-1, -1}
    };

    uint32_t corrected = CorrectSurfaceFormat(deskMode.format);

    for(int i = 0; checkmodes[i][0] != -1; i++)
    {
        SDL_DisplayMode target, closest;

        target.w = checkmodes[i][0];
        target.h = checkmodes[i][1];
        target.format = corrected;
        target.refresh_rate = 0;
        target.driverdata   = 0;

        if (SDL_GetClosestDisplayMode(0, &target, &closest) )
        {
            GfxMode mode;
            mode.w = closest.w;
            mode.h = closest.h;
            mode.mode = closest;
            mode.bpp = SDL_BYTESPERPIXEL(closest.format) * 8;
            mode.windowed = false;
            mode.name = fmt::sprintf("%d x %d", mode.w, mode.h);

            mode.sortid = (closest.w & 0x7FFF) << 7 | (closest.h & 0x7FFF);

            AddGfxMode(mode);
        }

        if (checkmodes[i][0] <= deskMode.w && checkmodes[i][1] <= deskMode.h)
        {
            GfxMode mode;
            mode.w = checkmodes[i][0];
            mode.h = checkmodes[i][1];
            mode.mode = deskMode;
            mode.bpp = SDL_BYTESPERPIXEL(corrected) * 8;
            mode.windowed = true;
            mode.name = fmt::sprintf("Windowed %d x %d", mode.w, mode.h);

            mode.sortid = 0x40000000 | (checkmodes[i][0] & 0x7FFF) << 7 | (checkmodes[i][1] & 0x7FFF);

            AddGfxMode(mode);
        }
    }

    graphicsModes.sort(GfxMode::Compare);
    
    cursors[0] = LoadCursor("Pointer");
    cursors[1] = LoadCursor("Cancel");
    cursors[2] = LoadCursor("Select");
    cursors[3] = LoadCursor("Attack");
    cursors[4] = LoadCursor("Goto");
    cursors[5] = LoadCursor("Disk");
    cursors[6] = LoadCursor("New");
    cursors[7] = LoadCursor("Add");
    cursors[8] = LoadCursor("Control");
    cursors[9] = LoadCursor("Beam");
    cursors[10] = LoadCursor("Build");

    
    win3dObj = Nucleus::CInit<NC_STACK_win3d>( { {NC_STACK_display::ATT_WIDTH, (int32_t)640},
                                                 {NC_STACK_display::ATT_HEIGHT, (int32_t)480} } );

    Gui::Instance.SetScreenSize(win3dObj->GetSize());
    portalID = Gui::Instance.AddPortal(Common::Point(640, 480), Common::Rect(0, 0, 640, 480));
    scrCompat = new Gui::OldCompat(Common::Point(640, 480));
    Gui::Instance.AddWidgetPortal(portalID, scrCompat);
    
    LoadPalette(gfx_keys[3].Get<std::string>());
}

void GFXEngine::Deinit()
{
    if (win3dObj)
        Nucleus::Delete(win3dObj);
    
    win3dObj = NULL;
    
    if (ScreenSurface)
        SDL_FreeSurface(ScreenSurface);
    
    if (scrCompat)
        delete scrCompat;
    
    ScreenSurface = NULL;
}

int GFXEngine::EventsWatcher(void *, SDL_Event *event)
{
    switch(event->type)
    {
    case SDL_WINDOWEVENT:
    {
        switch(event->window.event)
        {
        case SDL_WINDOWEVENT_RESIZED:
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            Instance.RecreateScreenSurface();
            Gui::Instance.SetScreenSize(System::GetResolution());
            Gui::Instance.SetPortal(Instance.portalID, Common::Rect(System::GetResolution()));
            break;
        default:
            break;
        }

    }
    break;
    }
    
    return 1; // This event can be passed to another event watcher
}

void GFXEngine::SetCursor(int curID, int force)
{
    int sett = 0;

    if ( force )
        sett = 1;
    else if ( curID != CurrentCursorID )
        sett = 1;

    if ( sett )
    {
        if ( curID == 0 )
            SDL_ShowCursor(SDL_DISABLE);
        else if ( curID <= 11 )
        {
            if ( cursors[curID - 1] )
                SDL_SetCursor( cursors[curID - 1] );

            if (!CurrentCursorID)
                SDL_ShowCursor(SDL_ENABLE);
        }

    }

    CurrentCursorID = curID;
}

GfxMode GFXEngine::sub_41F68C()
{
    for (const GfxMode &m : graphicsModes)
    {
        if (m.w == 640 && m.h == 480)
            return m;
    }

    return graphicsModes.front();
}

int GFXEngine::GetScreenH()
{
    return win3dObj->GetHeight();
}

int GFXEngine::GetScreenW()
{
    return win3dObj->GetWidth();
}


GfxMode GFXEngine::windd_func0__sub0(const std::string &file)
{
    FSMgr::FileHandle *fil = uaOpenFile(file, "r");

    if ( fil )
    {
        std::string line;
        if ( fil->ReadLine(&line) )
        {
            size_t pos = line.find_first_of("\n\r");
            
            if (pos != std::string::npos)
                line.erase(pos);

            for (const GfxMode &m : graphicsModes)
            {
                if ( StriCmp(m.name, line) == 0 )
                    return m;
            }
        }
        delete fil;
    }

    return sub_41F68C();
}

void GFXEngine::SetResolution(int res)
{
    UA_PALETTE *screen_palette = win3dObj->GetPalette();

    UA_PALETTE palette_copy;

    if ( screen_palette )
        palette_copy = *screen_palette; // Copy palette
    else
    {
        for(auto &x : palette_copy)
        {
            x.r = 0;
            x.g = 0;
            x.b = 0;
            x.a = 0;
        }
    }

    win3dObj->EndFrame();

    //delete_class_obj(cls3D);
    
    GfxMode picked;
    if ( res )
    {
        for (const GfxMode &m : graphicsModes)
        {
            if ( m.sortid == res )
            {
                picked = m;
                break;
            }
        }

        if ( !picked )
            picked = sub_41F68C();
    }
    else
    {
        picked = windd_func0__sub0("env/vid.def");
    }
    
    if (!picked.windowed)
        System::SetFullScreen(SDL_WINDOW_FULLSCREEN_DESKTOP, &picked.mode);
    else
        System::SetFullScreen(0, NULL);
    
    if (picked.windowed)
    {
        SDL_Delay(250);
    
        System::ResizeWindow(picked.w, picked.h);
    }
    RecreateScreenSurface();
    
    GfxSelectedMode = picked.sortid;

    if ( win3dObj->ChangeResolution(Common::Point(picked.w, picked.h), picked.windowed) )
    {
        win3dObj->BeginFrame();

        win3dObj->SetPalette(palette_copy);
    }
    
    FSMgr::FileHandle *fil = uaOpenFile("env/vid.def", "w");
    if ( fil )
    {
        fil->printf("%s\n", picked.name.c_str());
        delete fil;
    }
    
    Gui::Instance.ResizePortal(portalID, Common::Point(picked.w, picked.h));
    Gui::Instance.SetScreenSize(System::GetResolution());
    Gui::Instance.SetPortal(portalID, Common::Rect(Common::Point(picked.w, picked.h)));
}



size_t GFXEngine::display_func256(windd_arg256 *inout)
{
    auto it = graphicsModes.begin();

    if ( inout->sort_id )
    {
        while (it != graphicsModes.end())
        {
            if ( it->sortid == inout->sort_id )
                break;
            
            it++;
        }

    }

    if ( it == graphicsModes.end() )
        return 0;

    inout->sort_id = it->sortid;
    inout->width = it->w;
    inout->height = it->h;

    strncpy(inout->name, it->name.c_str(), 32);

    it++;
    
    if (it != graphicsModes.end())
        return it->sortid;
    
    return 0;
}










void GFXEngine::SetTracyRmp(ResBitmap *rmp)
{
    win3dObj->setRSTR_trcRmp(rmp);
}

void GFXEngine::SetShadeRmp(ResBitmap *rmp)
{
    win3dObj->setRSTR_shdRmp(rmp);
}

int32_t GFXEngine::GetGfxMode()
{
    return GfxSelectedMode;
}

TileMap * GFXEngine::GetTileset(int id)
{
    return win3dObj->raster_func208(id);
}

void GFXEngine::DrawText(w3d_a209 *arg)
{
    w3d_a209 arg209;
    arg209 = *arg;
    win3dObj->raster_func209(&arg209);
}

void GFXEngine::SetTileset(TileMap *tileset, int id)
{
    win3dObj->raster_func207(id, tileset);
}


bool GFXEngine::LoadPalette(const std::string &palette_ilbm)
{
    NC_STACK_bitmap *ilbm = Nucleus::CInit<NC_STACK_ilbm>( {
        {NC_STACK_rsrc::RSRC_ATT_NAME, std::string(palette_ilbm)},
        {NC_STACK_bitmap::BMD_ATT_HAS_COLORMAP, (int32_t)1}} );

    if (!ilbm)
        return false;

    win3dObj->SetPalette( *ilbm->getBMD_palette() );

    delete_class_obj(ilbm);

    return true;
}

NC_STACK_win3d *GFXEngine::C3D()
{
    return win3dObj;
}
    
uint8_t *GFXEngine::MakeScreenCopy(int *ow, int *oh)
{
    Common::Point res = System::GetResolution();

    // power of 2
    res.x &= ~1;
    res.y &= ~1;

    uint8_t *buf = (uint8_t *)malloc(res.x * res.y * 3);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, res.x, res.y, GL_RGB, GL_UNSIGNED_BYTE, buf);

    *ow = res.x;
    *oh = res.y;
    return buf;
}

uint8_t *GFXEngine::MakeDepthScreenCopy(int *ow, int *oh)
{
    Common::Point res = System::GetResolution();

    // power of 2
    res.x &= ~1;
    res.y &= ~1;

    uint8_t *buf = (uint8_t *)malloc(res.x * res.y);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, res.x, res.y, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, buf);

    *ow = res.x;
    *oh = res.y;
    return buf;
}






SDL_Surface *GFXEngine::RealScreen()
{
    return ScreenSurface;
}

SDL_Surface *GFXEngine::Screen()
{
    return scrCompat->GetSurface();
}



// Draw line Bresenham's algorithm
void GFXEngine::DrawLine(SDL_Surface *surface, const Common::Line &line, uint8_t cr, uint8_t cg, uint8_t cb )
{
    if ((line.Width() == 0 && line.Height() == 0) || 
         !Common::Rect(surface->w, surface->h).IsIn(line.P1()) ||
         !Common::Rect(surface->w, surface->h).IsIn(line.P2()) )
        return;
    
    int rilWidth = surface->pitch / surface->format->BytesPerPixel;

    int xCount = line.Width();
    int yCount = line.Height();

    uint32_t color = SDL_MapRGBA(surface->format, cr, cg, cb, 255);

    int stepAdd, stepOdd;
    int steps, subSteps;

    if ( xCount <= yCount )
    {
        if ( line.y2 <= line.y1 )
            stepAdd = -rilWidth;
        else
            stepAdd = rilWidth;

        if ( line.x2 <= line.x1 )
            stepOdd = -1;
        else
            stepOdd = 1;

        steps = yCount;
        subSteps = xCount;
    }
    else
    {
        if ( line.x2 <= line.x1 )
            stepAdd = -1;
        else
            stepAdd = 1;

        if ( line.y2 <= line.y1 )
            stepOdd = -rilWidth;
        else
            stepOdd = rilWidth;
        
        steps = xCount;
        subSteps = yCount;
    }

    int incr1 = 2 * subSteps;
    int t = 2 * subSteps - steps;
    int incr2 = 2 * (subSteps - steps);
    
    SDL_LockSurface(surface);
    
    void *surfPos = (void *) ((uint8_t *) surface->pixels 
                    + line.y1 * surface->pitch 
                    + line.x1 * surface->format->BytesPerPixel );

    switch(surface->format->BytesPerPixel)
    {
        case 1:
        {
            uint8_t *surf = (uint8_t *)surfPos;

            for (int i = 0; i <= steps; i++) // Verify i bound
            {
                *surf = color;
                if ( t > 0 )
                {
                    t += incr2;
                    surf += stepOdd;
                }
                else
                    t += incr1;

                surf += stepAdd;
            }
        }
        break;
        
        case 2:
        {
            uint16_t *surf = (uint16_t *)surfPos;

            for (int i = 0; i <= steps; i++) // Verify i bound
            {
                *surf = color;
                if ( t > 0 )
                {
                    t += incr2;
                    surf += stepOdd;
                }
                else
                    t += incr1;

                surf += stepAdd;
            }
        }
        break;
        
        case 4:
        {
            uint32_t *surf = (uint32_t *)surfPos;

            for (int i = 0; i <= steps; i++) // Verify i bound
            {
                *surf = color;
                if ( t > 0 )
                {
                    t += incr2;
                    surf += stepOdd;
                }
                else
                    t += incr1;

                surf += stepAdd;
            }
        }
        break;
        
        default:
        break;
    }
    
    SDL_UnlockSurface(surface);
}

void GFXEngine::BlitScaleMasked(SDL_Surface *src, Common::Rect sRect, SDL_Surface *mask, uint8_t index, SDL_Surface *dst, Common::Rect dRect)
{
    if (mask->format->BitsPerPixel != 8)
        return;
        
    if (src->w != mask->w || src->h != mask->h)
        return;
    
    if (sRect.IsEmpty() || !sRect.IsValid())
        sRect = Common::Rect(src->w, src->h);
    else if (!Common::Rect(src->w, src->h).IsIn(sRect))
        return;
    
    if (dRect.IsEmpty() || !dRect.IsValid())
        dRect = Common::Rect(dst->w, dst->h);
    else if (!Common::Rect(dst->w, dst->h).IsIn(dRect))
        return;
    
    // Try fast
    if (src->format->format == dst->format->format)
    {
        switch(src->format->BytesPerPixel)
        {
            case 2:
            {
                SDL_LockSurface(src);
                SDL_LockSurface(mask);
                SDL_LockSurface(dst);
                
                int32_t dY = (sRect.Height() << 16) / dRect.Height();
                int32_t dX = (sRect.Width()  << 16) / dRect.Width();
                
                int32_t srcY  = sRect.top << 16;
                for (int y = dRect.top; y < dRect.bottom; y++)
                {
                    uint16_t *dBuf = (uint16_t *)((uint8_t *)dst->pixels + y * dst->pitch) + dRect.left;
                    uint16_t *sBuf = (uint16_t *)((uint8_t *)src->pixels + (srcY >> 16) * src->pitch) + sRect.left;
                    uint8_t  *mBuf = (uint8_t *)mask->pixels + (srcY >> 16) * mask->pitch + sRect.left;
                    
                    int32_t xx = 0;
                    for (int x = dRect.left; x < dRect.right; x++)
                    {
                        if (mBuf[xx >> 16] == index)
                            *dBuf = sBuf[xx >> 16];
                        dBuf++;
                        xx += dX;
                    }
                    srcY += dY;
                }
                
                SDL_UnlockSurface(dst);
                SDL_UnlockSurface(mask);
                SDL_UnlockSurface(src);
            }
            break;
            
            case 4:
            {
                SDL_LockSurface(src);
                SDL_LockSurface(mask);
                SDL_LockSurface(dst);
                
                int32_t dY = (sRect.Height() << 16) / dRect.Height();
                int32_t dX = (sRect.Width()  << 16) / dRect.Width();
                
                int32_t srcY  = sRect.top << 16;
                for (int y = dRect.top; y < dRect.bottom; y++)
                {
                    uint32_t *dBuf = (uint32_t *)((uint8_t *)dst->pixels + y * dst->pitch) + dRect.left;
                    uint32_t *sBuf = (uint32_t *)((uint8_t *)src->pixels + (srcY >> 16) * src->pitch) + sRect.left;
                    uint8_t  *mBuf = (uint8_t *)mask->pixels + (srcY >> 16) * mask->pitch + sRect.left;
                    
                    int32_t xx = 0;
                    for (int x = dRect.left; x < dRect.right; x++)
                    {
                        if (mBuf[xx >> 16] == index)
                            *dBuf = sBuf[xx >> 16];
                        dBuf++;
                        xx += dX;
                    }
                    srcY += dY;
                }
                
                SDL_UnlockSurface(dst);
                SDL_UnlockSurface(mask);
                SDL_UnlockSurface(src);
            }
            break;
            
            default:
            break;
        }
    }
    else // Slow
    {
        SDL_LockSurface(src);
        SDL_LockSurface(mask);
        SDL_LockSurface(dst);

        uint8_t sbpp = src->format->BytesPerPixel;
        uint8_t dbpp = dst->format->BytesPerPixel;
        
        int32_t dY = (sRect.Height() << 16) / dRect.Height();
        int32_t dX = (sRect.Width()  << 16) / dRect.Width();

        int32_t srcY  = sRect.top << 16;
        for (int y = dRect.top; y < dRect.bottom; y++)
        {
            uint8_t *dBuf = (uint8_t *)dst->pixels + y * dst->pitch + dRect.left * dbpp;
            uint8_t *sBuf = (uint8_t *)src->pixels + (srcY >> 16) * src->pitch + sRect.left * sbpp;
            uint8_t  *mBuf = (uint8_t *)mask->pixels + (srcY >> 16) * mask->pitch + sRect.left;

            int32_t xx = 0;
            for (int x = dRect.left; x < dRect.right; x++)
            {
                if (mBuf[xx >> 16] == index)
                {
                    uint8_t r,g,b;
                    uint32_t clr = 0;
                    
                    uint8_t *spix = sBuf + (xx >> 16) * sbpp;
                    for(int i = 0; i < sbpp; i++)
                        clr |= spix[i] << (i * 8);
                    
                    SDL_GetRGB(clr, src->format, &r, &g, &b);
                    clr = SDL_MapRGB(dst->format, r, g, b);
                    
                    for(int i = 0; i < dbpp; i++)
                        dBuf[i] = (clr >> (i * 8)) & 0xFF;
                }
                dBuf += dbpp;
                xx += dX;
            }
            srcY += dY;
        }

        SDL_UnlockSurface(dst);
        SDL_UnlockSurface(mask);
        SDL_UnlockSurface(src);
    }
}

void GFXEngine::DrawFill(SDL_Surface *src, const Common::Rect &sRect, SDL_Surface *dst, const Common::Rect &dRect)
{
    if (sRect.IsEmpty() || dRect.IsEmpty())
        return;
    
    SDL_Rect lsrc = sRect;
    SDL_Rect ldst;
    
    for(ldst.y = dRect.top; ldst.y < dRect.bottom; ldst.y += lsrc.h)
    {
        if (dRect.bottom - ldst.y < lsrc.h)
            lsrc.h = dRect.bottom - ldst.y;
        
        lsrc.w = sRect.Width();
        
        for(ldst.x = dRect.left; ldst.x < dRect.right; ldst.x += lsrc.w)
        {
            if (dRect.right - ldst.x < lsrc.w)
                lsrc.w = dRect.right - ldst.x;
            
            SDL_BlitSurface(src, &lsrc, dst, &ldst);
        }
    }
}

void GFXEngine::Draw(SDL_Surface *src, const Common::Rect &sRect, SDL_Surface *dst, Common::Point dPoint)
{
    SDL_Rect Ssrc = sRect;
    SDL_Rect Sdst = dPoint;
    SDL_BlitSurface(src, &Ssrc, dst, &Sdst);
}

void GFXEngine::Draw(SDL_Surface *src, const Common::Rect &sRect, SDL_Surface *dst, Common::PointRect dRect)
{
    SDL_Rect Ssrc = sRect;
    if (Ssrc.w > dRect.w)
        Ssrc.w = dRect.w;
    if (Ssrc.h > dRect.h)
        Ssrc.h = dRect.h;
    SDL_Rect Sdst = dRect;
    SDL_BlitSurface(src, &Ssrc, dst, &Sdst);
}

uint32_t GFXEngine::CorrectSurfaceFormat(uint32_t format)
{
    if (format == SDL_PIXELFORMAT_BGR888)
        return SDL_PIXELFORMAT_ABGR8888;
    else if (format == SDL_PIXELFORMAT_RGB888)
        return SDL_PIXELFORMAT_ARGB8888;
    return format;
}

void GFXEngine::GLMapFormat(uint32_t pixelFormat, GLint *format, GLint *type)
{
    switch(pixelFormat)
    {
    case SDL_PIXELFORMAT_ARGB8888:
        *format = GL_BGRA;
        *type = GL_UNSIGNED_INT_8_8_8_8_REV;
        break;

    case SDL_PIXELFORMAT_ABGR8888:
        *format = GL_RGBA;
        *type = GL_UNSIGNED_INT_8_8_8_8_REV;
        break;

    case SDL_PIXELFORMAT_BGRA8888:
        *format = GL_BGRA;
        *type = GL_UNSIGNED_BYTE;
        break;

    case SDL_PIXELFORMAT_RGBA8888:
        *format = GL_RGBA;
        *type = GL_UNSIGNED_BYTE;
        break;

    default:
        *format = GL_RGBA;
        *type = GL_UNSIGNED_BYTE;
        break;
    }
}

void GFXEngine::RecreateScreenSurface()
{
    if ( ScreenSurface )
        SDL_FreeSurface(ScreenSurface);

    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(0, &mode);
    mode.format = CorrectSurfaceFormat(mode.format);

    int bpp;
    uint32_t rm, gm, bm, am;
    SDL_PixelFormatEnumToMasks(mode.format, &bpp, &rm, &gm, &bm, &am);

    GLMapFormat(mode.format, &pixfmt, &pixtype);
    
    Common::Point res = System::GetResolution();
    ScreenSurface = SDL_CreateRGBSurface(0, res.x, res.y, bpp, rm, gm, bm, am);

    if ( !screenTex )
        glGenTextures(1, &screenTex);

    glPushAttrib(GL_TEXTURE_BIT);

    glBindTexture(GL_TEXTURE_2D, screenTex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ScreenSurface->w, ScreenSurface->h, 0, pixfmt, pixtype, NULL);

    glPopAttrib();
}

void GFXEngine::DrawScreenSurface()
{
    Common::Point scrSz = System::GetResolution();
    glViewport(0, 0, scrSz.x, scrSz.y);
    
    glPushAttrib(GL_LIGHTING | GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_TRANSFORM_BIT | GL_TEXTURE_BIT | GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_LIGHTING);

    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

    glDisable(GL_LIGHTING);

    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //GL_ONE_MINUS_SRC_ALPHA

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, screenTex);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ScreenSurface->w, ScreenSurface->h, pixfmt, pixtype, ScreenSurface->pixels);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glColor3f(1,1,1);
    
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0, 0);
        glVertex3f(-1.0, 1.0, 0);
        glTexCoord2f(0, 1);
        glVertex3f(-1.0, -1.0, 0);
        glTexCoord2f(1, 1);
        glVertex3f(1.0, -1.0, 0);
        glTexCoord2f(1, 0);
        glVertex3f(1.0, 1.0, 0);
    }
    glEnd();

    glPopAttrib();
}

SDL_Color GFXEngine::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SDL_Color tmp;
    tmp.a = a;
    tmp.r = r;
    tmp.g = g;
    tmp.b = b;
    return tmp;
}

}