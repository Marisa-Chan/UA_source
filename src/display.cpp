#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "bitmap.h"
#include "display.h"
#include "utils.h"


const Nucleus::ClassDescr NC_STACK_display::description("display.class", &newinstance);

NC_STACK_display::NC_STACK_display() 
{
    for(TileMap *&t : _tiles)
        t = NULL;
    
    for(SDL_Color &c : _palette)
        c = {0, 0, 0, 0};
    
    for(UA_PALETTE &p : _field_300)
    {
        for(SDL_Color &c : p)
            c = {0, 0, 0, 0};
    }
};

size_t NC_STACK_display::func0(IDVList &stak)
{
    if ( !NC_STACK_nucleus::func0(stak) )
        return 0;

    dprintf("MAKE ME %s\n","raster_func0");
    
    SetResolution( Common::Point(stak.Get<int32_t>(ATT_WIDTH, DEFAULT_WIDTH), stak.Get<int32_t>(ATT_HEIGHT, DEFAULT_HEIGHT)) );

    engines.display___win3d = this;

    return 1;
}

bool NC_STACK_display::SetResolution(Common::Point res)
{
    _width = res.x;   //stak.Get<int32_t>(ATT_WIDTH, 0);
    _height = res.y; //stak.Get<int32_t>(ATT_HEIGHT, 0);
    
    _clip = Common::Rect(_width - 1, _height - 1);

    _field_54c = _width / 2;
    _field_550 = _height / 2;

    _field_554 = _width / 2;
    _field_558 = _height / 2;
    
    return true;
}

bool NC_STACK_display::ChangeResolution(Common::Point res, bool windowed)
{
    return SetResolution(res);
}

size_t NC_STACK_display::func1()
{
    engines.display___win3d = NULL;
    return NC_STACK_nucleus::func1();
}

size_t NC_STACK_display::raster_func192(IDVPair *)
{
//    __NC_STACK_display *rstr = &stack__display;
//    bitmap_intern *bitm = rstr->bitm_intern;
//
//    memset(bitm->buffer, rstr->BG_Color, bitm->width * bitm->height);
    return 0;
}

//size_t NC_STACK_display::raster_func193(bitmap_intern **out)
//{
//    __NC_STACK_display *rstr = &stack__display;
//    bitmap_intern *bitm = rstr->bitm_intern;
//
//    memcpy((*out)->buffer, bitm->buffer, bitm->width * bitm->height);
//    return 0;
//}

size_t NC_STACK_display::raster_func198(w3d_func198arg *)
{
    dprintf("MAKE ME %s\n","raster_func198");
    return 0;
}

size_t NC_STACK_display::raster_func199(w3d_func199arg *)
{
    dprintf("MAKE ME %s\n","raster_func199");
    return 0;
}

size_t NC_STACK_display::raster_func200(w3d_func198arg *)
{
    dprintf("MAKE ME %s\n","raster_func200");
    return 0;
}

size_t NC_STACK_display::raster_func201(w3d_func199arg *)
{
    dprintf("MAKE ME %s\n","raster_func201");
    return 0;
}

size_t NC_STACK_display::raster_func202(rstr_arg204 *)
{
    dprintf("MAKE ME %s\n","raster_func202");
    return 0;
}

size_t NC_STACK_display::raster_func203(IDVPair *)
{
    dprintf("MAKE ME %s\n","raster_func203");
    return 0;
}

size_t NC_STACK_display::raster_func204(rstr_arg204 *)
{
    dprintf("MAKE ME %s\n","raster_func204");
    return 0;
}

size_t NC_STACK_display::raster_func205(IDVPair *)
{
    dprintf("MAKE ME %s\n","raster_func205");
    return 0;
}

size_t NC_STACK_display::raster_func206(polysDat *)
{
    dprintf("MAKE ME %s\n","raster_func206");
    return 0;
}

void NC_STACK_display::raster_func207(int id, TileMap *t)
{
    _tiles[id] = t;
}

TileMap *NC_STACK_display::raster_func208(int id)
{
    return _tiles[id];
}

int NC_STACK_display::raster_func208(TileMap *t)
{
    if ( t )
    {
        for (int i = 0; i < 256; i++)
        {
            if (_tiles[i] == t)
                return i;
        }
    }
    return -1;
}

void NC_STACK_display::raster_func209(w3d_a209 *)
{
    dprintf("MAKE ME %s\n","raster_func209");
}

void NC_STACK_display::raster_func210(ua_fRect *arg)
{
    _clip = Common::Rect(   (arg->x1 + 1.0) * (_field_554 + -1.0),
                            (arg->y1 + 1.0) * (_field_558 + -1.0),
                            (arg->x2 + 1.0) * (_field_554 + -1.0),
                            (arg->y2 + 1.0) * (_field_558 + -1.0) );
    }

void NC_STACK_display::raster_func211(ua_dRect *arg)
{
    _clip = Common::Rect(   _field_54c + arg->x1,
                            _field_550 + arg->y1,
                            _field_54c + arg->x2,
                            _field_550 + arg->y2 );
}

size_t NC_STACK_display::raster_func212(IDVPair *)
{
    dprintf("MAKE ME %s\n","raster_func212");
    return 0;
}

void NC_STACK_display::BeginScene()
{
}

void NC_STACK_display::EndScene()
{
}


size_t NC_STACK_display::raster_func217(uint32_t color)
{
    if ( color != 0xFFFFFFFF )
        _field_4 = color;

    return 0;
}

void NC_STACK_display::raster_func218(rstr_218_arg *)
{
    dprintf("MAKE ME %s\n","raster_func218");
}

size_t NC_STACK_display::raster_func219(IDVPair *)
{
    dprintf("MAKE ME %s\n","raster_func219");
    return 0;
}

size_t NC_STACK_display::raster_func220(IDVPair *)
{
    dprintf("MAKE ME %s\n","raster_func220");
    return 0;
}

void NC_STACK_display::raster_func221(ua_dRect *arg)
{
    _inverseClip.left = _field_54c + arg->x1;
    _inverseClip.top = _field_550 + arg->y1;
    _inverseClip.right = _field_54c + arg->x2;
    _inverseClip.bottom = _field_550 + arg->y2;
}




void NC_STACK_display::display_func261(int ID, UA_PALETTE &pal, int from, int num)
{
    for (int i = 0; i < num; i++)
        _field_300[ID][from + i] = pal[i];
}

void NC_STACK_display::display_func261(int ID, UA_PALETTE &pal)
{
    _field_300[ID] = pal;
}

void NC_STACK_display::display_func262(rstr_262_arg *arg)
{
    for (int i = 0; i < 256; i++)
    {
        int tmpr = 0;
        int tmpg = 0;
        int tmpb = 0;

        for (int j = 0; j < arg->cnt; j++)
        {
            UA_PALETTE &pal = _field_300[ arg->slot[j] ];
            tmpr += arg->weight[j] * pal[i].r;
            tmpg += arg->weight[j] * pal[i].g;
            tmpb += arg->weight[j] * pal[i].b;
        }

        tmpr >>= 8;
        tmpg >>= 8;
        tmpb >>= 8;

        if (tmpr > 255)
            tmpr = 255;
        if (tmpg > 255)
            tmpg = 255;
        if (tmpb > 255)
            tmpb = 255;

        _palette[i].r = tmpr;
        _palette[i].g = tmpg;
        _palette[i].b = tmpb;
    }
}

void NC_STACK_display::display_func263(displ_arg263 *arg)
{
//    __NC_STACK_display *displ = &stack__display;

//    display_func265(NULL);
//    displ->pointer_bitm = arg->bitm;
//    display_func264(NULL);
}

//void NC_STACK_display::display_func264(void *)
//{
//    __NC_STACK_display *displ = &stack__display;
//    displ->field_1b04 &= 0xFFFFFFFE;
//}

//void NC_STACK_display::display_func265(void *)
//{
//    __NC_STACK_display *displ = &stack__display;
//    displ->field_1b04 |= 1;
//}


bool NC_STACK_display::AllocTexture(ResBitmap *pbitm)
{
    return false;
}

void NC_STACK_display::FreeTexture(ResBitmap *pbitm)
{
}

UA_PALETTE * NC_STACK_display::display_func273(int paletteId)
{
    return &_field_300[paletteId];
}

void NC_STACK_display::SaveScreenshot(const std::string & screenName)
{
    dprintf("MAKE ME %s(Save pcx screenshot)\n","display_func274");
}



void NC_STACK_display::SetPalette(UA_PALETTE &newPal)
{
    display_func261(0, newPal);

    rstr_262_arg arg_262;
    int v11 = 0;
    int v12 = 256;
    arg_262.slot = &v11;
    arg_262.cnt = 1;
    arg_262.weight = &v12;

    display_func262(&arg_262);
}


void NC_STACK_display::SetPen(uint32_t pen)
{
    _field_4 = pen;
}

void NC_STACK_display::setRSTR_BGpen(uint32_t pen)
{
//    stack__display.BG_Color = pen;
}

void NC_STACK_display::setRSTR_shdRmp(ResBitmap *rmp)
{
//	rstr->field_10 = rmp;
//	if ( rmp )
//		rstr->field_14 = rmp->buffer;
}

void NC_STACK_display::setRSTR_trcRmp(ResBitmap *rmp)
{
//	rstr->field_18 = rmp;
//	if ( rmp )
//		rstr->field_1c = rmp->buffer;
}

void NC_STACK_display::setRSTR_FGApen(uint32_t pen)
{
    //stack__display.field_8 = pen;
}


UA_PALETTE *NC_STACK_display::GetPalette()
{
    return &_palette;
}

int16_t NC_STACK_display::GetWidth()
{
    return _width;
}

int16_t NC_STACK_display::GetHeight()
{
    return _height;
}
