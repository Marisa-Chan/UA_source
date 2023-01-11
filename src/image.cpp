#include <SDL2/SDL.h>
#include <SDL2/SDL_rwops.h>
#include <SDL2/SDL_image.h>
#include "nucleas.h"
#include "rsrc.h"
#include "image.h"
#include "utils.h"
#include "includes.h"

#include "glob_funcs.h"


NC_STACK_image::NC_STACK_image()
{
}

NC_STACK_image::~NC_STACK_image()
{
}

size_t NC_STACK_image::Init(IDVList &stak)
{
    if ( !NC_STACK_bitmap::Init(stak) )
        return 0;

    return 1;
}



static Sint64 mysizefunc(SDL_RWops * context)
{
    FSMgr::FileHandle *fil = (FSMgr::FileHandle *)context->hidden.unknown.data2;

    size_t curr = fil->tell();
    fil->seek(0, SEEK_END);
    size_t sz = fil->tell();
    fil->seek(curr, SEEK_SET);
    return sz;
}

static Sint64 myseekfunc(SDL_RWops *context, Sint64 offset, int whence)
{
    FSMgr::FileHandle *fil = (FSMgr::FileHandle *)context->hidden.unknown.data2;
    switch (whence)
    {
    case RW_SEEK_SET:
        if ( fil->seek(offset, SEEK_SET) != 0 )
            return -1;
        break;
    case RW_SEEK_CUR:
        if ( fil->seek(offset, SEEK_CUR) != 0 )
            return -1;
        break;
    case RW_SEEK_END:
        if ( fil->seek(offset, SEEK_END) != 0 )
            return -1;
        break;
    default:
        SDL_SetError("Can't seek of this kind of whence");
        return -1;
    }
    return fil->tell();
}

static size_t myreadfunc(SDL_RWops *context, void *ptr, size_t size, size_t maxnum)
{
    FSMgr::FileHandle *fil = (FSMgr::FileHandle *)context->hidden.unknown.data2;
    return fil->read(ptr, size * maxnum) / size;
}

static size_t mywritefunc(SDL_RWops *context, const void *ptr, size_t size, size_t num)
{
    FSMgr::FileHandle *fil = (FSMgr::FileHandle *)context->hidden.unknown.data2;
    return fil->write(ptr, size * num) / size;
}

static int myclosefunc(SDL_RWops *context)
{
    FSMgr::FileHandle *fil = (FSMgr::FileHandle *)context->hidden.unknown.data2;
    delete fil;

    SDL_FreeRW(context);
    return 0;
}

static SDL_RWops * MyCustomRWop(FSMgr::FileHandle *fil)
{
    SDL_RWops *c = SDL_AllocRW();

    if (c == NULL)
        return NULL;

    c->size = mysizefunc;
    c->seek = myseekfunc;
    c->read = myreadfunc;
    c->write = mywritefunc;
    c->close = myclosefunc;
    c->type = 0xF00BA91;
    c->hidden.unknown.data2 = fil;
    return c;
}




rsrc * NC_STACK_image::rsrc_func64(IDVList &stak)
{
    const std::string resName = stak.Get<std::string>(RSRC_ATT_NAME, "");
    int convertColor = stak.Get<int32_t>(BMD_ATT_CONVCOLOR, 0);

    if ( resName.empty() )
        return NULL;

    std::string tmpBuf = "rsrc:";
    tmpBuf += resName;

    FSMgr::FileHandle *fil = uaOpenFileAlloc(tmpBuf, "rb");
    if (!fil)
        return NULL;

    SDL_RWops *rwops = MyCustomRWop(fil);
    if (!rwops)
    {
        delete fil;
        return NULL;
    }

    SDL_Surface *loaded = IMG_Load_RW(rwops, 1);

    if (!loaded)
    {
        printf("%s\n", SDL_GetError());
        return NULL;
    }

    rsrc *res = NC_STACK_rsrc::rsrc_func64(stak); // bitmap_func64

    if ( !res )
    {
        SDL_FreeSurface(loaded);
        return NULL;
    }

    ResBitmap *bitm = new ResBitmap;   
    if ( !bitm )
    {
        SDL_FreeSurface(loaded);
        rsrc_func65(res);
        return NULL;
    }
    
    res->data = bitm;
    
    bitm->width = loaded->w;
    bitm->height = loaded->h;
    bitm->swTex = loaded;
    
    if (convertColor)
    {
        SDL_Surface *screenFmt = GFX::Engine.ConvertToScreenFormat(loaded);
        if (screenFmt)
        {
            SDL_FreeSurface(bitm->swTex);
            bitm->swTex = screenFmt;
        }
    } 
    else if (loaded->format->palette)
    {
        if ( !bitm->palette )
            bitm->palette = new UA_PALETTE;

        if ( bitm->palette )
        {
            for (int i = 0; i < loaded->format->palette->ncolors; i++)
                (*bitm->palette)[i] = loaded->format->palette->colors[i];
        }
    }

    return res;
}

bool NC_STACK_image::SavePng(ResBitmap *bitm, const std::string &fname)
{
    if (!bitm)
        return false;
    
    if (!bitm->swTex)
        return false;
    
    FSMgr::FileHandle *fil = uaOpenFileAlloc(fname, "wb");
    if (!fil || !fil->OK())
        return false;

    SDL_RWops *rwops = MyCustomRWop(fil);
    if (!rwops)
        return false;

    return IMG_SavePNG_RW(bitm->swTex, rwops, 1) == 0;
}