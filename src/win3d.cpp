#include <algorithm>

#include <inttypes.h>
#include <list>
#include "wrapSDL.h"
#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "bitmap.h"
#include "display.h"
#include "win3d.h"
#include "utils.h"

#include "font.h"

#include "gui/root.h"

struct gfxMode
{
    int w;
    int h;
    int bpp;
    SDL_DisplayMode mode;
    bool windowed;
    int sortid;
    std::string name;
};

std::list<gfxMode *> graphicsModes;
SDL_Cursor *cursors[11];

const Nucleus::ClassDescr NC_STACK_win3d::description("win3d.class", &newinstance);

Common::Ini::KeyList NC_STACK_win3d::win3d_keys
{
    Common::Ini::Key("gfx.dither",       Common::Ini::KT_BOOL),               //0
    Common::Ini::Key("gfx.filter",       Common::Ini::KT_BOOL),
    Common::Ini::Key("gfx.antialias",    Common::Ini::KT_BOOL),
    Common::Ini::Key("gfx.alpha",        Common::Ini::KT_DIGIT, (int32_t)192),
    Common::Ini::Key("gfx.zbuf_when_tracy", Common::Ini::KT_BOOL),
    Common::Ini::Key("gfx.colorkey",     Common::Ini::KT_BOOL),             //5
    Common::Ini::Key("gfx.force_emul",   Common::Ini::KT_BOOL),
    Common::Ini::Key("gfx.force_soft_cursor", Common::Ini::KT_BOOL),
    Common::Ini::Key("gfx.all_modes",    Common::Ini::KT_BOOL),
    Common::Ini::Key("gfx.movie_player", Common::Ini::KT_BOOL, true),
    Common::Ini::Key("gfx.force_alpha_textures", Common::Ini::KT_BOOL), //10
    Common::Ini::Key("gfx.use_draw_primitive", Common::Ini::KT_BOOL),
    Common::Ini::Key("gfx.disable_lowres", Common::Ini::KT_BOOL),
    Common::Ini::Key("gfx.export_window_mode", Common::Ini::KT_BOOL),
    Common::Ini::Key("gfx.blending",     Common::Ini::KT_DIGIT),
    Common::Ini::Key("gfx.solidfont",    Common::Ini::KT_BOOL),          //15
    Common::Ini::Key("gfx.vsync",        Common::Ini::KT_DIGIT, (int32_t)1),
    Common::Ini::Key("gfx.maxfps",       Common::Ini::KT_DIGIT, (int32_t)60),
    Common::Ini::Key("gfx.newsky",       Common::Ini::KT_BOOL),
    Common::Ini::Key("gfx.skydistance",  Common::Ini::KT_DIGIT, (int32_t)3000),
    Common::Ini::Key("gfx.skylength",    Common::Ini::KT_DIGIT, (int32_t)500)               //20
};



bool gfxComp(const gfxMode *a, const gfxMode *b)
{
    return (a->sortid > b->sortid);
}

void addGfxMode(gfxMode *md)
{
    for (std::list<gfxMode *>::iterator it = graphicsModes.begin(); it != graphicsModes.end(); it++)
    {
        if ( (*it)->sortid == md->sortid )
        {
            delete md;
            return;
        }
    }

    graphicsModes.push_back(md);
    log_d3dlog("Add display mode: %s%dx%d\n", (md->windowed ? "Windowed ": ""), md->w, md->h );
}


void NC_STACK_win3d::initfirst()
{
    SDL_DisplayMode deskMode;
    SDL_GetDesktopDisplayMode(0, &deskMode);

    int checkmodes[][2] =
    {
        {640, 480}, {800, 600}, {1024, 768}, {1280, 1024}, {1440, 1050}, {1600, 1200},
        {720, 480}, {852, 480}, {1280, 720}, {1366, 768}, {1600, 900}, {1920, 1080}, {2560, 1440},
        {-1, -1}
    };

    uint32_t corrected = SDLWRAP_CorrectFormat(deskMode.format);

    for(int i = 0; checkmodes[i][0] != -1; i++)
    {
        SDL_DisplayMode target, closest;

        target.w = checkmodes[i][0];
        target.h = checkmodes[i][1];
        target.format = corrected;
        target.refresh_rate = 0;
        target.driverdata   = 0;

        gfxMode *mode;
        if (SDL_GetClosestDisplayMode(0, &target, &closest) )
        {
            mode = new gfxMode;
            mode->w = closest.w;
            mode->h = closest.h;
            mode->mode = closest;
            mode->bpp = SDL_BYTESPERPIXEL(closest.format) * 8;
            mode->windowed = false;
            mode->name = fmt::sprintf("%d x %d", mode->w, mode->h);

            mode->sortid = (closest.w & 0x7FFF) << 7 | (closest.h & 0x7FFF);

            addGfxMode(mode);
        }

        if (checkmodes[i][0] <= deskMode.w && checkmodes[i][1] <= deskMode.h)
        {
            mode = new gfxMode;

            mode->w = checkmodes[i][0];
            mode->h = checkmodes[i][1];
            mode->mode = deskMode;
            mode->bpp = SDL_BYTESPERPIXEL(corrected) * 8;
            mode->windowed = true;
            mode->name = fmt::sprintf("Windowed %d x %d", mode->w, mode->h);

            mode->sortid = 0x40000000 | (checkmodes[i][0] & 0x7FFF) << 7 | (checkmodes[i][1] & 0x7FFF);

            addGfxMode(mode);
        }
    }

    graphicsModes.sort(gfxComp);

    cursors[0] = wrapLoadCursor("Pointer");
    cursors[1] = wrapLoadCursor("Cancel");
    cursors[2] = wrapLoadCursor("Select");
    cursors[3] = wrapLoadCursor("Attack");
    cursors[4] = wrapLoadCursor("Goto");
    cursors[5] = wrapLoadCursor("Disk");
    cursors[6] = wrapLoadCursor("New");
    cursors[7] = wrapLoadCursor("Add");
    cursors[8] = wrapLoadCursor("Control");
    cursors[9] = wrapLoadCursor("Beam");
    cursors[10] = wrapLoadCursor("Build");
}

ScreenFont::ScreenFont()
{
    ttfFont = NULL;
    height = 0;
    r = 0;
    g = 0;
    b = 0;
}

__NC_STACK_win3d::__NC_STACK_win3d()
{
    width = 0;
    height = 0;
    screenSurface = NULL;
    currentCursor = 0;
    forcesoftcursor = 0;
    movie_player = 0;
    field_38 = 0;
    txt16bit = 0;
    use_simple_d3d = 0;
    disable_lowres = 0;
    export_window_mode = 0;
    sort_id = 0;
    flags = 0;

    pending.clear();

    for(int i = 0; i < 9; i++)
    {
        gray_colors__[i][0] = 0.0;
        gray_colors__[i][1] = 0.0;
        gray_colors__[i][2] = 0.0;
    }

    dither = 0;
    filter = 0;
    antialias = 0;
    alpha = 0;
    zbuf_when_tracy = 0;
    colorkey = 0;

    pixfmt = NULL;
    glPixfmt = 0;
    glPixtype = 0;

    sceneBeginned = 0;

    corrIW = corrW = 1.0;
    corrIH = corrH = 1.0;

    solidFont = true;
}



int out_guid_to_file(const char *filename, const char *name)
{
    FSMgr::FileHandle *fil = uaOpenFile(filename, "w");
    if ( fil )
    {
        fil->puts(name);
        delete fil;
        return 1;
    }
    return 0;
}



void out_yes_no_status(const char *filename, int val)
{
    FSMgr::FileHandle *fil = uaOpenFile(filename, "w");
    if ( fil )
    {
        if ( val )
            fil->puts("yes");
        else
            fil->puts("no");
        delete fil;
    }
}


void NC_STACK_win3d::DrawTextEntry(const ScreenText *txt)
{
    ScreenFont *font = &stack__win3d.font;

    if ( font->ttfFont )
    {
        if ( txt->flag & 0x20 )
        {
            font->r = txt->p1;
            font->g = txt->p2;
            font->b = txt->p3;
        }
        else
        {
            int len = strlen(txt->string);

            if (len)
            {

                int cx = 0, cy = 0;

                if ( txt->flag & 0xE )
                {
                    TTF_SizeUTF8(font->ttfFont, txt->string, &cx, &cy);
                }

                int p1 = txt->p1;
                int p2 = txt->p2;
                int p3 = txt->p3;
                int p4 = txt->p4;

                if ( txt->flag & 8 )
                    p3 = cx * p3 / 100;

                SDL_Rect clipRect;

                clipRect.x = p1;
                clipRect.w = p3 + 4;
                clipRect.h = p4 + 1;
                clipRect.y = p2;

                if ( txt->flag & 2 )
                {
                    if ( txt->flag & 8 )
                    {
                        p1 -= cx;
                        clipRect.x = p1;
                        clipRect.w = p3 + 4;
                    }
                    else
                    {
                        p1 += (p3 - cx);
                    }
                }
                else if ( txt->flag & 4 )
                {
                    if ( txt->flag & 8 )
                    {
                        p1 -= cx / 2;
                        clipRect.x = p1;
                        clipRect.w = p3 + 4;
                    }
                    else
                    {
                        p1 += (p3 - cx) / 2;
                    }
                }

                SDL_SetClipRect(stack__win3d.screenSurface, &clipRect);


                int v10 = ((p4 - font->height) / 2) - 2 + p2;
                if ( txt->flag & 0x10 )
                {
                    v10++;
                    p1++;
                }

                SDL_Color clr;
                clr.a = 255;
                clr.r = 0;
                clr.g = 0;
                clr.b = 0;

                SDL_Surface *tmp;

                if (stack__win3d.solidFont)
                {
                    tmp = TTF_RenderUTF8_Solid(font->ttfFont, txt->string, clr);
                    SDL_SetSurfaceBlendMode(tmp, SDL_BLENDMODE_NONE);
                }
                else
                {
                    tmp = TTF_RenderUTF8_Blended(font->ttfFont, txt->string, clr);
                    SDL_SetSurfaceBlendMode(tmp, SDL_BLENDMODE_BLEND);
                }

                SDL_Rect want;
                want.w = tmp->w;
                want.h = tmp->h;
                want.x = p1 + 2;
                want.y = v10 + 1;

                SDL_BlitSurface(tmp, NULL, stack__win3d.screenSurface, &want);

                clr.a = 255;
                clr.r = font->r;
                clr.g = font->g;
                clr.b = font->b;

                if (stack__win3d.solidFont)
                {
                    SDL_SetPaletteColors(tmp->format->palette, &clr, 1, 1);
                }
                else
                {
                    SDL_FreeSurface(tmp);
                    tmp = TTF_RenderUTF8_Blended(font->ttfFont, txt->string, clr);
                    SDL_SetSurfaceBlendMode(tmp, SDL_BLENDMODE_BLEND);
                }

                want.w = tmp->w;
                want.h = tmp->h;
                want.x = p1 + 1;
                want.y = v10;

                SDL_BlitSurface(tmp, NULL, stack__win3d.screenSurface, &want);
                SDL_FreeSurface(tmp);


                SDL_SetClipRect(stack__win3d.screenSurface, NULL);
            }
        }
    }
}

void NC_STACK_win3d::AddScreenText(const char *string, int p1, int p2, int p3, int p4, int flag)
{
    ScreenText *v8 = new ScreenText;
    v8->string = string;
    v8->p1 = p1;
    v8->p2 = p2;
    v8->p3 = p3;
    v8->p4 = p4;
    v8->flag = flag;

    stack__win3d.font.entries.push_back(v8);
}

void NC_STACK_win3d::DrawScreenText()
{
    ScreenFont *font = &stack__win3d.font;

    font->r = 255;
    font->g = 255;
    font->b = 0;

    for ( std::list<ScreenText *>::iterator it = font->entries.begin(); it != font->entries.end(); it++ )
    {
        DrawTextEntry(*it);
        delete (*it);
    }

    font->entries.clear();
}

int win3dInitialisation(__NC_STACK_win3d *w3d)
{
    w3d->pending.clear();

    for (int i = 0; i < 8; i++)
    {
        uint32_t color = 0;
        switch(i)
        {
        case 0:
        case 4:
        case 5:
        case 6:
            color = 0xFFFFFF;
            break;
        case 1:
            color = 0xFF0000;
            break;
        case 2:
            color = 0x0000FF;
            break;
        case 3:
            color = 0x00FF80;
            break;
        case 7:
            color = 0x00FFC0;
            break;
        default:
            break;
        }

        w3d->gray_colors__[i][0] = ((color >> 16) & 0xFF) / 255.0;
        w3d->gray_colors__[i][1] = ((color >> 8) & 0xFF) / 255.0;
        w3d->gray_colors__[i][2] = (color & 0xFF) / 255.0;
    }

    w3d->gray_colors__[8][0] = 1.0;
    w3d->gray_colors__[8][1] = 1.0;
    w3d->gray_colors__[8][2] = 1.0;

    return 1;
}

int NC_STACK_win3d::initPixelFormats()
{
    __NC_STACK_win3d *w3d = &stack__win3d;

    if (w3d->pixfmt)
        SDL_FreeFormat(w3d->pixfmt);

    SDL_DisplayMode curr;
    SDL_GetCurrentDisplayMode(0, &curr);
    curr.format = SDLWRAP_CorrectFormat(curr.format);

    w3d->pixfmt = SDL_AllocFormat( curr.format );

    SDLWRAP_GL_mapFormat(curr.format, &w3d->glPixfmt, &w3d->glPixtype);

    return 1;
}

int NC_STACK_win3d::initPolyEngine()
{
    __NC_STACK_win3d *w3d = &stack__win3d;

    w3d->rendStates[TEXTUREHANDLE] = 0;
    w3d->rendStates[SHADEMODE] = 1; //Smooth
    w3d->rendStates[STIPPLEENABLE] = 0;
    w3d->rendStates[SRCBLEND] = 1;//D3DBLEND_ONE;
    w3d->rendStates[DESTBLEND] = 0;//D3DBLEND_ZERO;
    w3d->rendStates[TEXTUREMAPBLEND] = 2;//D3DTBLEND_MODULATE;
    w3d->rendStates[ALPHABLENDENABLE] = 0; /* TRUE to enable alpha blending */
    w3d->rendStates[ZWRITEENABLE] = 1; /* TRUE to enable z writes */
    w3d->rendStates[TEXTUREMAG] = (w3d->filter != 0); // D3DFILTER_NEAREST Or D3DFILTER_LINEAR
    w3d->rendStates[TEXTUREMIN] = (w3d->filter != 0); // D3DFILTER_NEAREST Or D3DFILTER_LINEAR

    for (int i = 0; i < W3D_STATES_MAX; i++)
        w3d->rendStates2[i] = w3d->rendStates[i];

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);

    glDisable(GL_CULL_FACE);

    if (w3d->dither)
        glEnable(GL_DITHER);
    else
        glDisable(GL_DITHER);

    glDisable(GL_BLEND);
    glDisable(GL_FOG);

    //win3d__SetRenderState(w3d, D3DRENDERSTATE_TEXTUREMAG, (w3d->filter != 0) + D3DFILTER_NEAREST); // Or D3DFILTER_LINEAR

    //win3d__SetRenderState(w3d, D3DRENDERSTATE_SUBPIXEL, 1); /* TRUE to enable subpixel correction */
    //win3d__SetRenderState(w3d, D3DRENDERSTATE_STIPPLEDALPHA, can_stippling); /* TRUE to enable stippled alpha */
    //win3d__SetRenderState(w3d, D3DRENDERSTATE_STIPPLEENABLE, 0); /* TRUE to enable stippling */
    //win3d__SetRenderState(w3d, D3DRENDERSTATE_COLORKEYENABLE, 1); /* TRUE to enable source colorkeyed textures */

    w3d->pending.clear();

    //glAlphaFunc ( GL_GREATER, 0.1 ) ;
    //glEnable ( GL_ALPHA_TEST );



//	for(int y = 0; y < 32; y++)
//    {
//        for(int x = 0; x < 4; x++)
//        {
//            if (y & 1)
//                stpl[x + y * 4] = 0x55;
//            else
//                stpl[x + y * 4] = 0xAA;
//        }
//    }

//	if (can_stippling)
//        glPolygonStipple(stpl);

    return 1;
}


gfxMode *sub_41F68C()
{
    for (std::list<gfxMode *>::iterator it = graphicsModes.begin(); it != graphicsModes.end(); it++)
    {
        if ((*it)->w == 640 && (*it)->h == 480)
            return *it;
    }

    return graphicsModes.front();
}

gfxMode *windd_func0__sub0(const std::string &file)
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

            for (std::list<gfxMode *>::iterator it = graphicsModes.begin(); it != graphicsModes.end(); it++)
            {
                if ( StriCmp((*it)->name, line) == 0 )
                    return *it;
            }
        }
        delete fil;
    }

    return sub_41F68C();
}

int sub_42AC78(__NC_STACK_win3d *a1)
{
    return a1->forcesoftcursor;
}

uint32_t cursPix(uint8_t *data, int ofs, int bpp)
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

SDL_Cursor *NC_STACK_win3d::wrapLoadCursor(const char *name)
{
    std::string cur = "res/";
    cur += name;
    cur += ".cur";

    FSMgr::FileHandle *fil = uaOpenFile(cur.c_str(), "rb");

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
                int idx = cursPix(data, invY * width + x, bitcount);
                int alph = cursPix(mask, invY * width + x, 1);
                row[x * 4 + 0] = pal[ idx ].r;
                row[x * 4 + 1] = pal[ idx ].g;
                row[x * 4 + 2] = pal[ idx ].b;
                row[x * 4 + 3] = (1 - alph) * 255;
            }
            else
            {
                uint32_t clr = cursPix(data, invY * width + x, bitcount);
                int alph = cursPix(mask, invY * width + x, 1);
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

void sub_42D410(__NC_STACK_win3d *obj, int curID, int force)
{
    int sett = 0;

    if ( force )
        sett = 1;
    else if ( curID != obj->currentCursor )
        sett = 1;

    if ( sett )
    {
        if ( curID == 0 )
            SDL_ShowCursor(SDL_DISABLE);
        else if ( curID <= 11 )
        {
            if ( cursors[curID - 1] )
                SDL_SetCursor( cursors[curID - 1] );

            if (!obj->currentCursor)
                SDL_ShowCursor(SDL_ENABLE);
        }

    }

    obj->currentCursor = curID;
}


int NC_STACK_win3d::LoadFontByDescr(const std::string &fontname)
{
    std::vector<std::string> splt = Stok::Split(fontname, ",");

    std::string facename;
    std::string s_height;
    
    if (splt.size() > 0)
        facename = splt[0];
    
    if (splt.size() > 1)
        s_height = splt[1];
    //const char *s_weight = strtok(0, ",");
    //const char *s_charset = strtok(0, ",");

    int height;//, weight, charset;
    if ( !facename.empty() && !s_height.empty() )//&& s_weight && s_charset )
    {
        height = std::stoi(s_height);
        //weight = atoi(s_weight);
        //charset = atoi(s_charset);
    }
    else
    {
        height = 12;
        //charset = 0;
        facename = "MS Sans Serif";
        //weight = 400;
    }

    if ( stack__win3d.font.ttfFont )
    {
        TTF_CloseFont(stack__win3d.font.ttfFont);
        stack__win3d.font.ttfFont = NULL;
    }

    stack__win3d.font.height = height;
    stack__win3d.font.ttfFont = SDLWRAP_loadFont(facename, height);

    if ( stack__win3d.font.ttfFont )
    {
        if (!stack__win3d.solidFont)
            TTF_SetFontHinting(stack__win3d.font.ttfFont, TTF_HINTING_LIGHT);
        else
            TTF_SetFontHinting(stack__win3d.font.ttfFont, TTF_HINTING_MONO);

        return 1;
    }

    printf("Can't load font\n");

    return 0;
}

size_t NC_STACK_win3d::windd_func0(IDVList &stak)
{
    int txt16bit_def = read_yes_no_status("env/txt16bit.def", 1);
    int drawprim_def = read_yes_no_status("env/drawprim.def", 0);
    int export_window_mode = win3d_keys[13].Get<bool>();     // gfx.export_window_mode

    switch(win3d_keys[14].Get<int>())
    {
        case 0:
        {
            can_srcblend = 1;
            can_destblend = 0;
            can_stippling = 0;
        }
        break;
        
        default:
        case 1:
        {
            can_srcblend = 1;
            can_destblend = 1;
            can_stippling = 0;
        }
        break;
    
        case 2:
        {
            can_srcblend = 0;
            can_destblend = 0;
            can_stippling = 1;
        }
        break;
    }


    int v7 = stak.Get<int32_t>(ATT_DISPLAY_ID, 0);

    gfxMode *picked = NULL;
    if ( v7 )
    {
        for (std::list<gfxMode *>::iterator it = graphicsModes.begin(); it != graphicsModes.end(); it++)
        {
            if ( (*it)->sortid == v7 )
            {
                picked = *it;
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

    log_d3dlog(" picked mode %s\n", picked->name.c_str());


    stak.Add(ATT_WIDTH, (int32_t)picked->w);
    stak.Add(ATT_HEIGHT, (int32_t)picked->h);

    if ( !NC_STACK_display::func0(stak) )
        return 0;

    __NC_STACK_win3d *win3d = &stack__win3d;

    win3d->forcesoftcursor = 0;
    win3d->sort_id = picked->sortid;
    win3d->movie_player = win3d_keys[9].Get<bool>();
    win3d->disable_lowres = win3d_keys[12].Get<bool>();
    win3d->txt16bit = txt16bit_def;
    win3d->use_simple_d3d = drawprim_def;
    win3d->export_window_mode = export_window_mode;

    win3d->solidFont = win3d_keys[15].Get<bool>();

    win3d->windowed = picked->windowed; ////HACK

    if ( picked->windowed )
        win3d->flags |= 1;


    win3d->mode = picked->mode;

    win3d->currentCursor = -1;
    win3d->width = picked->w;
    win3d->height = picked->h;

    if (!picked->windowed)
        SDLWRAP_setFullscreen(SDL_WINDOW_FULLSCREEN_DESKTOP, &picked->mode);
    else
        SDLWRAP_setFullscreen(0, NULL);

    SDL_Delay(250);

    SDLWRAP_resizeWindow(picked->w, picked->h);
    
    win3d->screenSurface = SDLWRAP::Screen();

    switch( win3d_keys[16].Get<int>() )
    {
        case 0:
            SDL_GL_SetSwapInterval(0);
            break;
        
        default:
        case 1:
            SDL_GL_SetSwapInterval(1);
            break;
        
        case 2:
            {
                if ( SDL_GL_SetSwapInterval(-1) == -1)
                    SDL_GL_SetSwapInterval(1);
            }
            break;
    }

    fpsLimitter(win3d_keys[17].Get<int>());

    LoadFontByDescr("MS Sans Serif,12,400,0");

    FSMgr::FileHandle *fil = uaOpenFile("env/vid.def", "w");
    if ( fil )
    {
        fil->printf("%s\n", picked->name.c_str());
        delete fil;
    }

    if ( (float)win3d->width / (float)win3d->height >= 1.4 )
    {
        int half = (win3d->width + win3d->height) / 2;
        win3d->corrW = (float)half * 1.1429 / (float)win3d->width;
        win3d->corrH = (float)half * 0.85715 / (float)win3d->height;
        win3d->corrIW = 1.0 / win3d->corrW;
        win3d->corrIH = 1.0 / win3d->corrH;
    }
    else //No correction
    {
        win3d->corrIW = win3d->corrW = 1.0;
        win3d->corrIH = win3d->corrH = 1.0;
    }


    //win3d->field_54______rsrc_field4 = (bitmap_intern *)getRsrc_pData();
    return 1;
}









size_t NC_STACK_win3d::func0(IDVList &stak)
{
    Common::Ini::ParseIniFile(DefaultIniFile, &win3d_keys);

    if ( !windd_func0(stak) )
        return 0;

    __NC_STACK_win3d *w3d = &stack__win3d;

    w3d->dither = win3d_keys[0].Get<bool>();
    w3d->filter = win3d_keys[1].Get<bool>();
    w3d->antialias = win3d_keys[2].Get<bool>();
    w3d->zbuf_when_tracy = win3d_keys[4].Get<bool>();
    w3d->colorkey = win3d_keys[5].Get<bool>();

    if ( can_srcblend )
        w3d->alpha = 192;
    else
        w3d->alpha = 128;

    if ( !win3dInitialisation(w3d) )
    {
        ypa_log_out("win3d.class: Initialization failed.\n");
        func1();
        return 0;
    }

    if ( !initPixelFormats() )
    {
        ypa_log_out("win3d.class: Pixelformat problems.\n");
        func1();
        return 0;
    }

    /*	if ( !initTextureCache(w3d) )
    	{
    		ypa_log_out("win3d.class: Failed to initialize texture cache.\n");
    		func1();
    		return 0;
    	}*/

    if ( !initPolyEngine() )
    {
        ypa_log_out("win3d.class: Failed to initialize polygon engine.\n");
        func1();
        return 0;
    }

    return 1;
}

size_t NC_STACK_win3d::func1()
{
    __NC_STACK_win3d *w3d = &stack__win3d;

    if (w3d->pixfmt)
    {
        SDL_FreeFormat(w3d->pixfmt);
        w3d->pixfmt = NULL;
    }

    if ( w3d->font.ttfFont )
    {
        TTF_CloseFont(w3d->font.ttfFont);
        w3d->font.ttfFont = NULL;
    }

    if (w3d->windowed)
        SDLWRAP_restoreWindow();

    return NC_STACK_display::func1();
}


size_t NC_STACK_win3d::raster_func192(IDVPair *)
{
    return 1;
}

size_t NC_STACK_win3d::raster_func198(w3d_func198arg *arg)
{
    __NC_STACK_display *rstr = &stack__display;

    float tX = rstr->field_554 - 1.0;
    float tY = rstr->field_558 - 1.0;

    int y1 = (arg->y1 + 1.0) * tY;
    int y2 = (arg->y2 + 1.0) * tY;
    int x1 = (arg->x1 + 1.0) * tX;
    int x2 = (arg->x2 + 1.0) * tX;

    SDLWRAP::DrawLine(stack__win3d.screenSurface,
                      Common::Rect(x1, y1, x2, y2),
                      (rstr->field_4 >> 16) & 0xFF,
                      (rstr->field_4 >> 8) & 0xFF,
                      rstr->field_4 & 0xFF);
    return 1;
}


size_t NC_STACK_win3d::raster_func199(w3d_func199arg *arg)
{
    __NC_STACK_display *rstr = &stack__display;

    SDLWRAP::DrawLine(stack__win3d.screenSurface,
                      Common::Rect(rstr->field_54c + arg->x1, rstr->field_550 + arg->y1,
                                   rstr->field_54c + arg->x2, rstr->field_550 + arg->y2),
                      (rstr->field_4 >> 16) & 0xFF,
                      (rstr->field_4 >> 8) & 0xFF,
                      rstr->field_4 & 0xFF);

    return 1;
}

void NC_STACK_win3d::sub_420EDC(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b)
{
    Common::Rect tmp1(x1, y1, x2, y2);

    if ( Common::ClipLine(stack__display._clip, &tmp1) )
    {
        Common::Rect tmp2 = tmp1;
        

        if ( stack__display._inverseClip.IsEmpty() || !Common::ClipLine(stack__display._inverseClip, &tmp2) )
        {
            SDLWRAP::DrawLine(stack__win3d.screenSurface, tmp1, r, g, b);
        }
        else
        {
            if ( tmp2.right != tmp1.right || tmp2.bottom != tmp1.bottom )
            {
                if ( tmp2.left != tmp1.left || tmp2.top != tmp1.top )
                {
                    SDLWRAP::DrawLine(stack__win3d.screenSurface, Common::Rect(tmp1.left, tmp1.top, tmp2.left, tmp2.top), r, g, b);
                    SDLWRAP::DrawLine(stack__win3d.screenSurface, Common::Rect(tmp2.right, tmp2.bottom, tmp1.right, tmp1.bottom), r, g, b);
                }
                else
                {
                    SDLWRAP::DrawLine(stack__win3d.screenSurface, Common::Rect(tmp2.right, tmp2.bottom, tmp1.right, tmp1.bottom), r, g, b);
                }
            }
            else
            {
                SDLWRAP::DrawLine(stack__win3d.screenSurface, Common::Rect(tmp1.left, tmp1.top, tmp2.left, tmp2.top), r, g, b);
            }
        }
    }
}

size_t NC_STACK_win3d::raster_func200(w3d_func198arg *arg)
{
    __NC_STACK_display *rstr = &stack__display;

    float tX = rstr->field_554 - 1.0;
    float tY = rstr->field_558 - 1.0;

    int y1 = (arg->y1 + 1.0) * tY;
    int y2 = (arg->y2 + 1.0) * tY;
    int x1 = (arg->x1 + 1.0) * tX;
    int x2 = (arg->x2 + 1.0) * tX;

    sub_420EDC (x1, y1, x2, y2,
               (rstr->field_4 >> 16) & 0xFF,
               (rstr->field_4 >> 8) & 0xFF,
               rstr->field_4 & 0xFF);

    return 1;
}

size_t NC_STACK_win3d::raster_func201(w3d_func199arg *arg)
{
    __NC_STACK_display *rstr = &stack__display;

    sub_420EDC(rstr->field_54c + arg->x1,
               rstr->field_550 + arg->y1,
               rstr->field_54c + arg->x2,
               rstr->field_550 + arg->y2,
               (rstr->field_4 >> 16) & 0xFF,
               (rstr->field_4 >> 8) & 0xFF,
               rstr->field_4 & 0xFF);

    return 1;
}

size_t NC_STACK_win3d::raster_func202(rstr_arg204 *arg)
{
    __NC_STACK_display *rstr = &stack__display;

    ResBitmap *pbitm = arg->pbitm;

    int a1 = (arg->float4 + 1.0) * (arg->pbitm->width / 2);
    int a2 = (arg->float8 + 1.0) * (arg->pbitm->height / 2);
    int a3 = (arg->floatC + 1.0) * (arg->pbitm->width / 2);
    int a4 = (arg->float10 + 1.0) * (arg->pbitm->height / 2);

    int a5 = rstr->field_554 * (arg->float14 + 1.0);
    int a6 = rstr->field_558 * (arg->float18 + 1.0);
    int a7 = rstr->field_554 * (arg->float1C + 1.0);
    int a8 = rstr->field_558 * (arg->float20 + 1.0);
    
    SDL_Rect src;
    src.x = a1;
    src.y = a2;
    src.w = a3 - a1;
    src.h = a4 - a2;
    
    SDL_Rect dst;
    dst.x = a5;
    dst.y = a6;
    dst.w = a7 - a5;
    dst.h = a8 - a6;
    
    SDL_BlitScaled(pbitm->swTex, &src, stack__win3d.screenSurface, &dst);
    
    return 1;
}


int win3d_func204__sub0(__NC_STACK_display *rstr, rstr_loc204 *arg)
{
    if ( rstr->_clip.left > arg->dword20 || rstr->_clip.right < arg->dword18 || rstr->_clip.top > arg->dword24 || rstr->_clip.bottom < arg->dword1C )
        return 0;

    int d04 = arg->dword4;
    int d08 = arg->dword8;
    int d0C = arg->dwordC;
    int d10 = arg->dword10;

    int d18 = arg->dword18;
    int d1C = arg->dword1C;
    int d20 = arg->dword20;
    int d24 = arg->dword24;


    if ( d18 < rstr->_clip.left )
    {
        d04 += (rstr->_clip.left - d18) * (d0C - d04) / (d20 - d18);
        d18 = rstr->_clip.left;
    }

    if ( d20 > rstr->_clip.right )
    {
        d0C += (rstr->_clip.right - d20) * (d0C - d04) / (d20 - d18);
        d20 = rstr->_clip.right;
    }

    if ( d1C < rstr->_clip.top )
    {
        d08 += (rstr->_clip.top - d1C) * (d10 - d08) / (d24 - d1C);
        d1C = rstr->_clip.top;
    }

    if ( d24 > rstr->_clip.bottom )
    {
        d10 += (rstr->_clip.bottom - d24) * (d10 - d08) / (d24 - d1C);
        d24 = rstr->_clip.bottom;
    }

    arg->dword4 = d04;
    arg->dword8 = d08;
    arg->dwordC = d0C;
    arg->dword10 = d10;
    arg->dword18 = d18;
    arg->dword1C = d1C;
    arg->dword20 = d20;
    arg->dword24 = d24;

    return 1;
}


size_t NC_STACK_win3d::raster_func204(rstr_arg204 *arg)
{

    __NC_STACK_display *rstr = &stack__display;
    __NC_STACK_win3d *w3d = &stack__win3d;

    rstr_loc204 loc;

    loc.pbitm = arg->pbitm;
    loc.dword4 = (arg->float4 + 1.0) * (arg->pbitm->width / 2);
    loc.dword8 = (arg->float8 + 1.0) * (arg->pbitm->height / 2);
    loc.dwordC = (arg->floatC + 1.0) * (arg->pbitm->width / 2);
    loc.dword10 = (arg->float10 + 1.0) * (arg->pbitm->height / 2);

    loc.dword18 = (arg->float14 + 1.0) * rstr->field_554;
    loc.dword1C = (arg->float18 + 1.0) * rstr->field_558;
    loc.dword20 = (arg->float1C + 1.0) * rstr->field_554;
    loc.dword24 = (arg->float20 + 1.0) * rstr->field_558;

    if ( win3d_func204__sub0(rstr, &loc) )
    {
        SDL_Rect src;
        src.x = loc.dword4;
        src.y = loc.dword8;
        src.w = loc.dwordC - loc.dword4;
        src.h = loc.dword10 - loc.dword8;

        SDL_Rect dst;
        dst.x = loc.dword18;
        dst.y = loc.dword1C;
        dst.w = loc.dword20 - loc.dword18;
        dst.h = loc.dword24 - loc.dword1C;

        SDL_BlitScaled(loc.pbitm->swTex, &src, w3d->screenSurface, &dst);
    }


    return 1;
}

void NC_STACK_win3d::SetRenderStates(int setAll)
{
    __NC_STACK_win3d *w3d = &stack__win3d;

    uint32_t changeStates = 0;

    if (setAll)
        changeStates = 0xFFFFFFFF;

    for (int i = 0; i < W3D_STATES_MAX; i++)
    {
        if (w3d->rendStates2[i] != w3d->rendStates[i])
            changeStates |= 1 << i;

        w3d->rendStates[i] = w3d->rendStates2[i];
    }

    if ( changeStates & MSK(TEXTUREHANDLE))
    {
        if (w3d->rendStates[TEXTUREHANDLE])
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, w3d->rendStates[TEXTUREHANDLE]);
        }
        else
        {
            glDisable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    if ( changeStates & ( MSK(TEXTUREHANDLE) | MSK(TEXTUREMAG) ) )
    {
        if (w3d->rendStates[TEXTUREMAG])
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        else
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    if ( changeStates & ( MSK(TEXTUREHANDLE) | MSK(TEXTUREMIN) ) )
    {
        if (w3d->rendStates[TEXTUREMIN])
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        else
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }


    if ( changeStates & MSK(SHADEMODE) )
    {
        if (w3d->rendStates[SHADEMODE])
            glShadeModel(GL_SMOOTH);
        else
            glShadeModel(GL_FLAT);
    }

    if ( changeStates & MSK(STIPPLEENABLE) )
    {
    }

    if ( changeStates & (MSK(SRCBLEND) | MSK(DESTBLEND)) )
    {
        GLint src, dst;
        switch (w3d->rendStates[SRCBLEND])
        {
        case 0:
            src = GL_ZERO;
            break;

        case 1:
            src = GL_ONE;
            break;

        default:
        case 2:
            src = GL_SRC_ALPHA;
            break;

        case 3:
            src = GL_ONE_MINUS_SRC_ALPHA;
            break;
        }

        switch (w3d->rendStates[DESTBLEND])
        {
        case 0:
            dst = GL_ZERO;
            break;

        case 1:
            dst = GL_ONE;
            break;

        case 2:
            dst = GL_SRC_ALPHA;
            break;

        default:
        case 3:
            dst = GL_ONE_MINUS_SRC_ALPHA;
            break;
        }

        glBlendFunc(src, dst);
    }

    if ( changeStates & MSK(TEXTUREMAPBLEND) )
    {
        if (w3d->rendStates[TEXTUREMAPBLEND] == 0)
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        else if (w3d->rendStates[TEXTUREMAPBLEND] == 1)
        {
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
            glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
            glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
        }
        else if (w3d->rendStates[TEXTUREMAPBLEND] == 2)
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }

    if ( changeStates & MSK(ALPHABLENDENABLE) )
    {
        if (w3d->rendStates[ALPHABLENDENABLE])
            glEnable(GL_BLEND);
        else
            glDisable(GL_BLEND);
    }

    if ( changeStates & MSK(ZWRITEENABLE) )
    {
        if (w3d->rendStates[ZWRITEENABLE])
            glDepthMask(GL_TRUE);
        else
            glDepthMask(GL_FALSE);
    }
}


void NC_STACK_win3d::sb_0x43b518(polysDat *in, int a5, int a6)
{
    polysDatSub *polysDat = &in->datSub;

    struct vtxOut
    {
        float x, y, z;
        float tu, tv;
        float r, g, b, a;
    };

    __NC_STACK_win3d *w3d = &stack__win3d;

    if ( !w3d->sceneBeginned )
        return;

    if ( polysDat->vertexCount < 3 || polysDat->vertexCount > 12 )
        return;

    //Store for rendering later ( from 214 method )
    if ( !(polysDat->renderFlags & RFLAGS_FALLOFF) )
        if ( polysDat->renderFlags & 0x30 && !a6 )
        {
            w3d->pending.push_back( in );
            return;
        }

    vtxOut vtx[24];

    for (int i = 0; i < polysDat->vertexCount; i++)
    {
        vtx[i].x = polysDat->vertexes[i].x;
        vtx[i].y = polysDat->vertexes[i].y;
        vtx[i].z = polysDat->vertexes[i].z;
        vtx[i].tu = 0.0;
        vtx[i].tv = 0.0;
        vtx[i].r = polysDat->r;
        vtx[i].g = polysDat->g;
        vtx[i].b = polysDat->b;
        vtx[i].a = 1.0;
    }

    w3d->rendStates2[SHADEMODE] = 0;//D3DSHADE_FLAT;
    w3d->rendStates2[STIPPLEENABLE] = 0;
    w3d->rendStates2[SRCBLEND] = 1;//D3DBLEND_ONE;
    w3d->rendStates2[DESTBLEND] = 0;//D3DBLEND_ZERO;
    w3d->rendStates2[TEXTUREMAPBLEND] = 0;//D3DTBLEND_COPY;
    w3d->rendStates2[ALPHABLENDENABLE] = 0;
    w3d->rendStates2[ZWRITEENABLE] = 1;
    w3d->rendStates2[TEXTUREHANDLE] = 0;

    w3d->rendStates2[TEXTUREMAG] = (w3d->filter != 0);
    w3d->rendStates2[TEXTUREMIN] = (w3d->filter != 0);

    if ( polysDat->renderFlags & (RFLAGS_LINMAP | RFLAGS_PERSPMAP) )
    {
        if (in->datSub.pbitm)
            w3d->rendStates2[TEXTUREHANDLE] = in->datSub.pbitm->hwTex;

        for (int i = 0; i < polysDat->vertexCount; i++)
        {
            vtx[i].tu = polysDat->tu_tv[i].tu;
            vtx[i].tv = polysDat->tu_tv[i].tv;
        }
    }
    else
    {
        for (int i = 0; i < polysDat->vertexCount; i++)
        {
            vtx[i].r = 0;
            vtx[i].g = 0;
            vtx[i].b = 0;
            vtx[i].a = 1.0;
        }

    }

    if ( polysDat->renderFlags & (RFLAGS_FLATSHD | RFLAGS_GRADSHD) )
    {
        w3d->rendStates2[TEXTUREMAPBLEND] = 2;//D3DTBLEND_MODULATE;
        w3d->rendStates2[SHADEMODE] = 1;//D3DSHADE_GOURAUD;

        for (int i = 0; i < polysDat->vertexCount; i++)
        {
            float comp = (1.0 - polysDat->color[i]);
            vtx[i].r *= comp;
            vtx[i].g *= comp;
            vtx[i].b *= comp;
            vtx[i].a = 1.0;
        }
    }

    if ( polysDat->renderFlags & RFLAGS_LUMTRACY )
    {
        if ( !w3d->zbuf_when_tracy )
            w3d->rendStates2[ZWRITEENABLE] = 0;

        if ( can_destblend )
        {
            w3d->rendStates2[ALPHABLENDENABLE] = 1;
            w3d->rendStates2[TEXTUREMAPBLEND] = 1;//D3DTBLEND_MODULATEALPHA;
            w3d->rendStates2[SRCBLEND] = 1;//D3DBLEND_ONE;
            w3d->rendStates2[DESTBLEND] = 1;//D3DBLEND_ONE;
            w3d->rendStates2[SHADEMODE] = 0;//D3DSHADE_FLAT;
        }
        else if ( can_srcblend )
        {
            w3d->rendStates2[ALPHABLENDENABLE] = 1;
            w3d->rendStates2[TEXTUREMAPBLEND] = 1;//D3DTBLEND_MODULATEALPHA;
            w3d->rendStates2[SRCBLEND] = 2;//D3DBLEND_SRCALPHA;
            w3d->rendStates2[DESTBLEND] = 3;//D3DBLEND_INVSRCALPHA;
            w3d->rendStates2[SHADEMODE] = 0;//D3DSHADE_FLAT;
        }
        else if ( can_stippling )
        {
            w3d->rendStates2[ALPHABLENDENABLE] = 1;
            w3d->rendStates2[TEXTUREMAPBLEND] = 1;//D3DTBLEND_MODULATEALPHA;
            w3d->rendStates2[SRCBLEND] = 2;//D3DBLEND_SRCALPHA;
            w3d->rendStates2[DESTBLEND] = 3;//D3DBLEND_INVSRCALPHA;
            w3d->rendStates2[STIPPLEENABLE] = 1;
            w3d->rendStates2[SHADEMODE] = 0;//D3DSHADE_FLAT;
        }

        for (int i = 0; i < polysDat->vertexCount; i++)
            vtx[i].a = w3d->alpha;
    }
    else if ( polysDat->renderFlags & RFLAGS_ZEROTRACY )
    {
        if ( !w3d->zbuf_when_tracy )
            w3d->rendStates2[ZWRITEENABLE] = 0;

        if ( w3d->pixfmt->BytesPerPixel != 1 )
        {
            w3d->rendStates2[DESTBLEND] = 3;//D3DBLEND_INVSRCALPHA;
            w3d->rendStates2[SRCBLEND] = 2;//D3DBLEND_SRCALPHA;
        }

        w3d->rendStates2[ALPHABLENDENABLE] = 1;
        w3d->rendStates2[TEXTUREMAG] = 0;//D3DFILTER_NEAREST;
        w3d->rendStates2[TEXTUREMIN] = 0;//D3DFILTER_NEAREST;
        w3d->rendStates2[TEXTUREMAPBLEND] = 2;//D3DTBLEND_MODULATE;
    }

    if (win3d_keys[18].Get<bool>())
    {
        if (polysDat->renderFlags & RFLAGS_SKY)
        {
            w3d->rendStates2[ZWRITEENABLE] = 0;
        }
        else if (polysDat->renderFlags & RFLAGS_FALLOFF)
        {
            w3d->rendStates2[ZWRITEENABLE] = 1;
            w3d->rendStates2[ALPHABLENDENABLE] = 1;
            w3d->rendStates2[TEXTUREMAPBLEND] = 1;//D3DTBLEND_MODULATEALPHA;
            w3d->rendStates2[SRCBLEND] = 2;//D3DBLEND_SRCALPHA;
            w3d->rendStates2[DESTBLEND] = 3;//D3DBLEND_INVSRCALPHA;
            w3d->rendStates2[SHADEMODE] = 1;//D3DSHADE_FLAT;
            w3d->rendStates2[STIPPLEENABLE] = 0;

            float transDist = win3d_keys[19].Get<int>();
            float transLen = win3d_keys[20].Get<int>();

            for (int i = 0; i < polysDat->vertexCount; i++)
            {
                if (polysDat->distance[i] > transDist)
                {
                    float prc = (polysDat->distance[i] - transDist) / transLen;
                    if (prc > 1.0)
                        prc = 1.0;
                    vtx[i].a = (1.0 - prc);
                }
            }
        }
    }

    SetRenderStates(0);

    glBegin(GL_TRIANGLE_FAN);
    {
        for (int i = 0; i < polysDat->vertexCount; i++)
        {
            glColor4f(vtx[i].r, vtx[i].g, vtx[i].b, vtx[i].a);
            glTexCoord2f(vtx[i].tu, vtx[i].tv);
            glVertex3d(vtx[i].x, -vtx[i].y, vtx[i].z);
        }
    }
    glEnd();
}

size_t NC_STACK_win3d::raster_func206(polysDat *arg)
{
    sb_0x43b518(arg, 0, 0);

    return 1;
}

void NC_STACK_win3d::win3d_func209__sub0(TileMap **tiles, char *cmdline, char **arr)
{
    __NC_STACK_win3d *w3d = &stack__win3d;

    int v11;

    int bytesPerColor = w3d->screenSurface->format->BytesPerPixel;

    char *curpos = cmdline;
    int w_pixels = w3d->screenSurface->pitch / bytesPerColor;
    TileMap *tile = NULL;

    int x_out = 0;
    int y_out = 0;

    int x_out_txt = 0;
    int y_out_txt = 0;
    int txt_flag = 0;

    int y_pos_line = 0;
    int x_pos_line = 0;

    int rilHeight, rilWidth;

    if ( w3d->flags & 8 )
    {
        rilHeight = w3d->height / 2;
        rilWidth = w3d->width / 2;
    }
    else
    {
        rilHeight = w3d->height;
        rilWidth = w3d->width;
    }

    int halfWidth = rilWidth / 2;
    int halfHeight = rilHeight / 2;

    int line_width = 0;
    int line_height = 0;

    //if v11 = 0 - clone first column of tile  (count = line_width)
    //if v11 = 1 - normal copy of tile
    v11 = 1;


    int x_off = 0;
    int y_off = 0;


    char *positions[64];
    int position_idx = 0;

    positions[position_idx] = NULL;
    position_idx++;

    while ( 1 )
    {
        int v13 = FontUA::get_u8(&curpos);

        if ( v13 )
        {
            Common::PointRect &chrr = tile->map[v13];

            int cpy_width;

            if ( line_width )
                cpy_width = line_width - x_off;
            else
                cpy_width = chrr.w - x_off;

            int cpy_height = line_height - y_off;

            SDL_Rect srcR, dstR;
            dstR.x = x_out;
            dstR.y = y_out;
            dstR.w = cpy_width;
            dstR.h = cpy_height;
            
            srcR.x = chrr.x + x_off;
            srcR.y = chrr.y + y_off;
            srcR.h = cpy_height;
            
            if (v11)
                srcR.w = cpy_width;
            else
                srcR.w = 1;
            
            for(int i = 0; i < cpy_width; i += srcR.w)
            {
                SDL_BlitSurface(tile->img->GetSwTex(), &srcR, w3d->screenSurface, &dstR);
                dstR.x += srcR.w;
            }                

            line_width = 0;
            x_off = 0;
            x_out += cpy_width;
            v11 = 1;
        }
        else // 0
        {
            int opcode = FontUA::get_u8(&curpos);

            switch ( opcode )
            {
            case 0: // End
                position_idx--;

                curpos = positions[position_idx];
                if ( curpos )
                    break;

                DrawScreenText();
                return;

            case 1: // x pos from center
                x_out = halfWidth + FontUA::get_s16(&curpos);
                x_pos_line = x_out;

                y_pos_line = y_out;
                y_off = 0;

                line_height = tile->h;
                break;

            case 2: // y pos from center
                y_out = halfHeight + FontUA::get_s16(&curpos);
                x_pos_line = x_out;

                y_pos_line = y_out;
                y_off = 0;

                line_height = tile->h;
                break;

            case 3: //xpos
                x_out = FontUA::get_s16(&curpos);
                if ( x_out < 0 )
                    x_out += w_pixels;

                x_pos_line = x_out;
                y_pos_line = y_out;

                line_height = tile->h;
                y_off = 0;
                break;

            case 4: //ypos
                y_out = FontUA::get_s16(&curpos);
                if ( y_out < 0 )
                    y_out += rilHeight;

                x_pos_line = x_out;
                y_pos_line = y_out;

                line_height = tile->h;
                y_off = 0;
                break;

            case 5: //add to x pos
                x_out += FontUA::get_s16(&curpos);
                break;

            case 6: //add to y pos
                y_out += FontUA::get_s16(&curpos);
                break;

            case 7: //next line
                y_out = (line_height - y_off) + y_pos_line;
                y_pos_line = y_out;
                x_out = x_pos_line;

                y_off = 0;
                line_height = tile->h;
                break;

            case 8: // Select tileset
                tile = tiles[FontUA::get_u8(&curpos)];
                break;

            case 9: // Include another cmdlist source
            {
                int azaza = FontUA::get_u8(&curpos);
                positions[position_idx] = curpos;
                position_idx++;
                curpos = arr[azaza];
            }
            break;

            case 10:
                line_width = FontUA::get_u8(&curpos);

                v11 = 0;
                x_off = 0;

                break;

            case 11:

                line_width = FontUA::get_u8(&curpos);

                v11 = 0;
                x_off = 0;

                line_width -= (x_out - x_pos_line);
                break;

            case 12: // Set x offset
                x_off = FontUA::get_u8(&curpos);
                break;

            case 13: // Set x width
                line_width = FontUA::get_u8(&curpos);
                break;

            case 14: // Set y offset
                y_off = FontUA::get_u8(&curpos);
                break;

            case 15: // Set y height
                line_height = FontUA::get_u8(&curpos);
                break;

            case 16: // Full reset tileset
                tile = tiles[FontUA::get_u8(&curpos)];
                line_height = tile->h;
                y_off = 0;
                break;

            case 17:
                line_width = FontUA::get_s16(&curpos);
                v11 = 0;
                x_off = 0;
                line_width -= (x_out - x_pos_line);
                break;

            case 18: // Add text
            {
                int block_width = FontUA::get_s16(&curpos);
                int flag = txt_flag | FontUA::get_u16(&curpos);

                char *txtpos = (char *)curpos;

                curpos += strlen(txtpos) + 1;
                AddScreenText(txtpos, x_out_txt, y_out_txt, block_width, tile->h, flag);
            }
            break;

            case 19: // Copy current x/y pos for text output
                x_out_txt = x_out;
                y_out_txt = y_out;
                break;

            case 20: // Add txtout flag
                txt_flag |= FontUA::get_u16(&curpos);
                break;

            case 21: // Delete txtout flag
                txt_flag &= ~(FontUA::get_u16(&curpos));
                break;

            case 22: // set color for font
            {
                int r = FontUA::get_u16(&curpos);

                int g = FontUA::get_u16(&curpos);

                int b = FontUA::get_u16(&curpos);

                AddScreenText(0, r, g, b, 0, 0x20);
            }
            break;
            }
        }
    }
}

void NC_STACK_win3d::raster_func209(w3d_a209 *arg)
{
    win3d_func209__sub0(stack__display.tiles.data(), arg->cmdbuf, arg->includ);
}


void NC_STACK_win3d::BeginScene()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixd(frustum);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    stack__win3d.sceneBeginned = 1;
}

bool NC_STACK_win3d::compare(polysDat *a, polysDat *b)
{
    return a->range > b->range;
}

void NC_STACK_win3d::RenderTransparent()
{
    if ( stack__win3d.sceneBeginned )
    {
        if ( stack__win3d.pending.size() )
        {
            std::stable_sort(stack__win3d.pending.begin(), stack__win3d.pending.end(), compare);

            for (std::deque<polysDat *>::iterator it = stack__win3d.pending.begin(); it != stack__win3d.pending.end(); it++)
            {
                sb_0x43b518((*it), 1, 1);
            }


            stack__win3d.pending.clear();
        }
    }
}

// Draw transparent
void NC_STACK_win3d::EndScene()
{
    RenderTransparent();

    stack__win3d.sceneBeginned = 0;
}

void NC_STACK_win3d::raster_func218(rstr_218_arg *arg)
{
    __NC_STACK_display *rstr = &stack__display;

    Common::Rect sRect( (arg->rect1.x1 + 1.0) * (arg->bitm_intern->width / 2),
                        (arg->rect1.y1 + 1.0) * (arg->bitm_intern->height / 2),
                        (arg->rect1.x2 + 1.0) * (arg->bitm_intern->width / 2),
                        (arg->rect1.y2 + 1.0) * (arg->bitm_intern->height / 2) );

    Common::Rect dRect( (arg->rect2.x1 + 1.0) * rstr->field_554,
                        (arg->rect2.y1 + 1.0) * rstr->field_558,
                        (arg->rect2.x2 + 1.0) * rstr->field_554,
                        (arg->rect2.y2 + 1.0) * rstr->field_558 );

    SDLWRAP::BlitScaleMasked(arg->bitm_intern->swTex, sRect, arg->bitm_intern2->swTex, arg->flg, stack__win3d.screenSurface, dRect);
}

size_t NC_STACK_win3d::display_func256(windd_arg256 *inout)
{
    gfxMode *nod = NULL;

    if ( inout->sort_id )
    {
        for (std::list<gfxMode *>::iterator it = graphicsModes.begin(); it != graphicsModes.end(); it++)
        {
            if ( (*it)->sortid == inout->sort_id )
            {
                nod = *it;
                break;
            }
        }

    }
    else
        nod = graphicsModes.front();

    if ( !nod )
        return 0;

    inout->sort_id = nod->sortid;
    inout->width = nod->w;
    inout->height = nod->h;

    strncpy(inout->name, nod->name.c_str(), 32);

    for (std::list<gfxMode *>::iterator it = graphicsModes.begin(); it != graphicsModes.end(); it++)
    {
        if ( *it == nod )
        {
            it++;
            if (it != graphicsModes.end())
                return (*it)->sortid;

            break;
        }
    }

    return 0;
}


void NC_STACK_win3d::BeginFrame()
{
    __NC_STACK_win3d *w3d = &stack__win3d;

    SDL_FillRect(w3d->screenSurface, NULL, SDL_MapRGBA(w3d->screenSurface->format, 0, 0, 0, 0) );

    glPushAttrib(GL_DEPTH_WRITEMASK);
    glDepthMask(GL_TRUE);

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPopAttrib();
}

void NC_STACK_win3d::EndFrame()
{
    Gui::Root::Instance.Draw(SDLWRAP::Screen());
    SDLWRAP_drawScreen();
    Gui::Root::Instance.HwCompose();
    
    SDLWRAP_flipWindow();
}




void win3d_func262__sub0(__NC_STACK_win3d *w3d, int a2, int *a3, int *a4)
{
    float cl1 = 0.0;
    float cl2 = 0.0;
    float cl3 = 0.0;

    if ( a2 )
    {
        float tclr1 = 0.0;
        float tclr2 = 0.0;
        float tclr3 = 0.0;

        for(int i = 0; i < a2; i++)
        {
            float v15 = a4[i] / 255.0;
            int v14 = a3[i];

            tclr1 += w3d->gray_colors__[v14][0] * v15;
            tclr2 += w3d->gray_colors__[v14][1] * v15;
            tclr3 += w3d->gray_colors__[v14][2] * v15;
        }

        cl1 = tclr1;
        cl2 = tclr2;
        cl3 = tclr3;
    }
    if ( cl1 > 1.0 )
        cl1 = 1.0;
    if ( cl2 > 1.0 )
        cl2 = 1.0;
    if ( cl3 > 1.0 )
        cl3 = 1.0;

    w3d->gray_colors__[8][0] = cl1;
    w3d->gray_colors__[8][1] = cl2;
    w3d->gray_colors__[8][2] = cl3;
}

void NC_STACK_win3d::display_func262(rstr_262_arg *arg)
{
    __NC_STACK_win3d *w3d = &stack__win3d;

    win3d_func262__sub0(w3d, arg->cnt, arg->slot, arg->weight);
    NC_STACK_display::display_func262(arg);
}


void NC_STACK_win3d::display_func263(displ_arg263 *arg)
{
    __NC_STACK_win3d *w3d = &stack__win3d;

    sub_42D410(w3d, arg->pointer_id, 0);
    NC_STACK_display::display_func263(arg);
}

bool NC_STACK_win3d::AllocTexture(ResBitmap *bitm)
{
    if (bitm->swTex && !bitm->hwTex)
    {
        glPushAttrib(GL_TEXTURE_2D | GL_TEXTURE_BINDING_2D);

        glGenTextures(1, &bitm->hwTex);

        if (!bitm->hwTex)
        {
            glPopAttrib();
            return false;
        }

        glBindTexture(GL_TEXTURE_2D, bitm->hwTex);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        if (bitm->swTex->format->format == stack__win3d.pixfmt->format)
        {
            SDL_LockSurface(bitm->swTex);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitm->width, bitm->height, 0, stack__win3d.glPixfmt, stack__win3d.glPixtype, bitm->swTex->pixels);
            SDL_UnlockSurface(bitm->swTex);
        }
        else
        {
            SDL_Surface *conv = ConvertSDLSurface(bitm->swTex, stack__win3d.pixfmt);
            
            SDL_LockSurface(conv);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitm->width, bitm->height, 0, stack__win3d.glPixfmt, stack__win3d.glPixtype, conv->pixels);
            SDL_UnlockSurface(conv);
            
            SDL_FreeSurface(conv);
        }
        
        glPopAttrib();
    }

    return true;
}

void NC_STACK_win3d::FreeTexture(ResBitmap *bitm)
{
    if ( bitm->hwTex )
        glDeleteTextures(1, &bitm->hwTex);

    bitm->hwTex = 0;
}

void NC_STACK_win3d::display_func271(IDVPair *stak)
{

}

void NC_STACK_win3d::display_func272(IDVPair *)
{
}




void win3d_func274__sub0(FSMgr::FileHandle *fil)
{
    int bf_w = 0, bf_h = 0;

    uint8_t *buf = SDLWRAP_makeScreenCopy(bf_w, bf_h);

    if (buf && bf_w && bf_h)
    {
        fil->printf("P6\n#YPA screenshot\n%d %d\n255\n", bf_w, bf_h);

        int lwidth = 3 * bf_w;

        for (int j = 0; j < bf_h; j++)
        {
            uint8_t *ln = buf + lwidth * (bf_h - 1 - j);

            for (int i = 0; i < bf_w; i++)
            {
                uint8_t *px = ln + i * 3;

                uint8_t r = px[0];
                uint8_t g = px[1];
                uint8_t b = px[2];

                fil->writeU8(r);
                fil->writeU8(g);
                fil->writeU8(b);
            }
        }

        free(buf);
    }
}

void NC_STACK_win3d::SaveScreenshot(const std::string & screenName)
{
    FSMgr::FileHandle *fil = uaOpenFile(fmt::sprintf("%s.ppm", screenName), "wb");
    if ( fil )
    {
        win3d_func274__sub0(fil);
        delete fil;
    }
}


void NC_STACK_win3d::windd_func320(IDVPair *)
{
}

void NC_STACK_win3d::windd_func321(IDVPair *)
{
}





char * windd_func322__sub0(__NC_STACK_win3d *wdd, const char *box_title, const char *box_ok, const char *box_cancel, const char *box_startText, uint32_t timer_time, void (*timer_func)(int, int, int), void *timer_context, int replace, int maxLen)
{
    dprintf("MAKE ME %s\n","windd_func322__sub0");
    return NULL;
}

//Show DLGBox with edit field and get entered value
void NC_STACK_win3d::windd_func322(windd_dlgBox *dlgBox)
{
    __NC_STACK_win3d *w3d = &stack__win3d;

    windd_func320(NULL);

    dlgBox->result = windd_func322__sub0(
                         w3d,
                         dlgBox->title,
                         dlgBox->ok,
                         dlgBox->cancel,
                         dlgBox->startText,
                         dlgBox->time,
                         dlgBox->timer_func,
                         dlgBox->timer_context,
                         dlgBox->replace,
                         dlgBox->maxLen);

    windd_func321(NULL);
}





void windd_func323__sub0__sub0(const char *filename)
{
    printf("MAKE MOVIE PLAYER %s, file %s\n","windd_func323__sub0__sub0", filename);
}


void windd_func323__sub0(__NC_STACK_win3d *w3d, const char *filename)
{
////    unk_514F20 = 1;
    if ( w3d->movie_player )
        windd_func323__sub0__sub0(filename);
////    unk_514F20 = 0;
}

//Play movie file
void NC_STACK_win3d::windd_func323(const char **filename)
{
    __NC_STACK_win3d *w3d = &stack__win3d;

    windd_func323__sub0(w3d, *filename);
}

void NC_STACK_win3d::windd_func324(wdd_func324arg *inout)
{
    if ( inout->guid )
    {
        inout->name = NULL;
        inout->guid = NULL;
        inout->currr = 0;
    }
    else
    {
        inout->name = "OpenGL";
        inout->guid = "<primary>";
        inout->currr = 1;
    }
}

void NC_STACK_win3d::windd_func325(wdd_func324arg *arg)
{
    const char *v4 = arg->guid;
    const char *v5 = "<error>";

    if ( v4 )
    {
        if ( !strcmp(v4, "<primary>") )
        {
            v5 = v4;
            v4 = NULL;
        }
        if ( !strcmp(v4, "<software>") )
        {
            v5 = v4;
            v4 = NULL;
        }
    }

    out_guid_to_file("env/guid3d.def", v5);
}



void NC_STACK_win3d::setWDD_cursor(int mode)
{
}

void NC_STACK_win3d::setWDD_disLowRes(int arg)
{
}

void NC_STACK_win3d::setWDD_16bitTex(int arg)
{
    out_yes_no_status("env/txt16bit.def", arg);
}

void NC_STACK_win3d::setWDD_drawPrim(int arg)
{
    out_yes_no_status("env/drawprim.def", arg);
}



int NC_STACK_win3d::getDISP_displID()
{
    return stack__win3d.sort_id;
}

int NC_STACK_win3d::getWDD_16bitTex()
{
    return stack__win3d.txt16bit;
}

int NC_STACK_win3d::getWDD_drawPrim()
{
    return stack__win3d.use_simple_d3d;
}

void NC_STACK_win3d::setW3D_texFilt(int arg)
{
    stack__win3d.filter = arg;
}



void NC_STACK_win3d::draw2DandFlush()
{
    Gui::Root::Instance.Draw(SDLWRAP::Screen());
    SDLWRAP_drawScreen();
    Gui::Root::Instance.HwCompose();

    SDL_FillRect(stack__win3d.screenSurface, NULL, SDL_MapRGBA(stack__win3d.screenSurface->format, 0, 0, 0, 0) );
}

void NC_STACK_win3d::matrixAspectCorrection(mat3x3 &inout, bool invert)
{
    if (invert)
    {
        inout.m00 *= stack__win3d.corrIW;
        inout.m01 *= stack__win3d.corrIW;
        inout.m02 *= stack__win3d.corrIW;
        inout.m10 *= stack__win3d.corrIH;
        inout.m11 *= stack__win3d.corrIH;
        inout.m12 *= stack__win3d.corrIH;
    }
    else
    {
        inout.m00 *= stack__win3d.corrW;
        inout.m01 *= stack__win3d.corrW;
        inout.m02 *= stack__win3d.corrW;
        inout.m10 *= stack__win3d.corrH;
        inout.m11 *= stack__win3d.corrH;
        inout.m12 *= stack__win3d.corrH;
    }
}

void NC_STACK_win3d::getAspectCorrection(float &cW, float &cH, bool invert)
{
    if (invert)
    {
        cW = stack__win3d.corrIW;
        cH = stack__win3d.corrIH;
    }
    else
    {
        cW = stack__win3d.corrW;
        cH = stack__win3d.corrH;
    }
}

void NC_STACK_win3d::setFrustumClip(float _near, float _far)
{
    if (_near != frustumNear || _far != frustumFar)
        _setFrustumClip(_near, _far);
}

void NC_STACK_win3d::_setFrustumClip(float _near, float _far)
{
    //-z * frustum
    frustumNear = _near;
    frustumFar = _far;
    frustum[0] = 1.0;
    frustum[1] = 0.0;
    frustum[2] = 0.0;
    frustum[3] = 0.0;

    frustum[4] = 0.0;
    frustum[5] = 1.0;
    frustum[6] = 0.0;
    frustum[7] = 0.0;

    frustum[8] = 0.0;
    frustum[9] = 0.0;
    frustum[10] = (_far + _near) / (_far - _near);
    frustum[11] = 1.0;

    frustum[12] = 0.0;
    frustum[13] = 0.0;
    frustum[14] = -2.0 * (_far * _near) / (_far - _near);
    frustum[15] = 0.0;
}

void NC_STACK_win3d::ConvAlphaPalette(UA_PALETTE *dst, const UA_PALETTE &src, bool transp)
{
    for (uint16_t i = 0; i < dst->size(); i++)
    {
        SDL_Color &c = (*dst)[i];
        c = src[i];
        c.a = 255;

        if (c.a == 255 && c.g == 255 && c.b == 0)
        {
            c.a = 0;
            c.r = 0;
            c.g = 0;
            c.b = 0;
        }
        else
        {
            if (!can_destblend && can_srcblend && transp)
            {
                int mx = (c.r >= c.g) ? (c.r > c.b ? c.r: c.b) : (c.g > c.b ? c.g : c.b);

                if (mx <= 8)
                {
                    c.r = 0;
                    c.g = 0;
                    c.b = 0;
                    c.a = 0;
                }
                else
                {
                    float prm = mx;
                    c.r = 255.0 * (c.r / prm);
                    c.g = 255.0 * (c.g / prm);
                    c.b = 255.0 * (c.b / prm);
                    c.a = mx;
                }
            }
            else
            {
                c.a = 255;
            }
        }
    }
}

SDL_PixelFormat *NC_STACK_win3d::GetScreenFormat()
{
    return stack__win3d.screenSurface->format;
}

SDL_Surface *NC_STACK_win3d::CreateSurfaceScreenFormat(int width, int height)
{
    SDL_PixelFormat *fmt = engines.display___win3d->GetScreenFormat();
#if SDL_VERSION_ATLEAST(2,0,5)
    return SDL_CreateRGBSurfaceWithFormat(0, width, height, fmt->BitsPerPixel, fmt->format);
#else
    return SDL_CreateRGBSurface(0, width, height, fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask );
#endif
}

SDL_Surface *NC_STACK_win3d::ConvertToScreenFormat(SDL_Surface *src)
{
    return ConvertSDLSurface(src, stack__win3d.screenSurface->format);
}

SDL_Surface * NC_STACK_win3d::ConvertSDLSurface(SDL_Surface *src, const SDL_PixelFormat * fmt)
{
#if (SDL_COMPILEDVERSION == SDL_VERSIONNUM(2, 0, 12))
    /***
     * Workaround for bug with convertation of surface with palette introduced
     * in SDL2 2.0.12 and fixed soon but after release.
     ***/
    if (src->format->BytesPerPixel == 1)
    {
#if SDL_VERSION_ATLEAST(2,0,5)
        SDL_Surface *tmp = SDL_CreateRGBSurfaceWithFormat(0, src->w, src->h, fmt->BitsPerPixel, fmt->format);
#else
        SDL_Surface *tmp = SDL_CreateRGBSurface(0, src->w, src->h, fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask );
#endif
        SDL_BlendMode blend = SDL_BLENDMODE_NONE;
        SDL_GetSurfaceBlendMode(src, &blend);
        SDL_SetSurfaceBlendMode(src, SDL_BLENDMODE_NONE);
        SDL_BlitSurface(src, NULL, tmp, NULL);
        SDL_SetSurfaceBlendMode(src, blend);
        return tmp;
    }
    else
        return SDL_ConvertSurface(src, fmt, 0);
#else
    return SDL_ConvertSurface(src, fmt, 0);
#endif
}

