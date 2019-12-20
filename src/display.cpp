#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "bitmap.h"
#include "display.h"
#include "utils.h"


const Nucleus::ClassDescr NC_STACK_display::description("display.class", &newinstance);


size_t NC_STACK_display::func0(IDVList &stak)
{
    if ( !NC_STACK_nucleus::func0(stak) )
        return 0;

    dprintf("MAKE ME %s\n","raster_func0");

    _width = stak.Get(ATT_WIDTH, 0);
    _height = stak.Get(ATT_HEIGHT, 0);

//    rstr->bitm_intern = (bitmap_intern *)getRsrc_pData();

    stack__display.field_24.x1 = 0;
    stack__display.field_24.y1 = 0;
    stack__display.field_24.x2 = _width - 1;
    stack__display.field_24.y2 = _height - 1;

    stack__display.field_54c = _width / 2;
    stack__display.field_550 = _height / 2;

    stack__display.field_554 = _width / 2;
    stack__display.field_558 = _height / 2;

    engines.display___win3d = this;

    return 1;
}

size_t NC_STACK_display::func1()
{
    engines.display___win3d = NULL;
    return NC_STACK_nucleus::func1();
}

size_t NC_STACK_display::func2(IDVList &stak)
{
    for(IDVList::iterator it = stak.begin(); it != stak.end(); it++)
    {
        IDVPair &val = it->second;

        if ( !val.skip() )
        {
            switch (val.id)
            {
            case ATT_PALETTE:
                SetPalette(*(UA_PALETTE *)val.value.p_data);
                break;

            case ATT_FGPEN:
                setRSTR_FGpen(val.value.u_data);
                break;

            case ATT_BGPEN:
                setRSTR_BGpen(val.value.u_data);
                break;

            case ATT_SHADE_RMP:
                setRSTR_shdRmp((bitmap_intern *)val.value.p_data);
                break;

            case ATT_TRACY_RMP:
                setRSTR_trcRmp((bitmap_intern *)val.value.p_data);
                break;

            case ATT_FGAPEN:
                setRSTR_FGApen(val.value.i_data);
                break;

            default:
                break;
            }
        }
    }

    return NC_STACK_nucleus::func2(stak);
}

size_t NC_STACK_display::func3(IDVList &stak)
{
    IDVList::iterator it = stak.find(ATT_PALETTE);
    if ( it != stak.end() )
    {
        *(UA_PALETTE **)it->second.value.p_data = GetPalette();
    }

    return NC_STACK_nucleus::func3(stak);
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

size_t NC_STACK_display::raster_func206(polysDatSub *)
{
    dprintf("MAKE ME %s\n","raster_func206");
    return 0;
}

void NC_STACK_display::raster_func207(rstr_207_arg *arg)
{
    __NC_STACK_display *rstr = &stack__display;
    rstr->tiles[arg->id] = arg->tiles;
}

void NC_STACK_display::raster_func208(rstr_207_arg *arg)
{
    __NC_STACK_display *rstr = &stack__display;

    if ( arg->tiles )
    {
        arg->id = -1;

        for (int i = 0; i < 256; i++)
        {
            if (rstr->tiles[i] == arg->tiles)
            {
                arg->id = i;
                break;
            }
        }
    }
    else
    {
        arg->tiles = rstr->tiles[arg->id];
    }
}

void NC_STACK_display::raster_func209(w3d_a209 *)
{
    dprintf("MAKE ME %s\n","raster_func209");
}

void NC_STACK_display::raster_func210(ua_fRect *arg)
{
    __NC_STACK_display *rstr = &stack__display;

    rstr->field_24.x1 = (arg->x1 + 1.0) * (rstr->field_554 + -1.0);
    rstr->field_24.y1 = (arg->y1 + 1.0) * (rstr->field_558 + -1.0);
    rstr->field_24.x2 = (arg->x2 + 1.0) * (rstr->field_554 + -1.0);
    rstr->field_24.y2 = (arg->y2 + 1.0) * (rstr->field_558 + -1.0);
}

void NC_STACK_display::raster_func211(ua_dRect *arg)
{
    __NC_STACK_display *rstr = &stack__display;

    rstr->field_24.x1 = rstr->field_54c + arg->x1;
    rstr->field_24.y1 = rstr->field_550 + arg->y1;
    rstr->field_24.x2 = rstr->field_54c + arg->x2;
    rstr->field_24.y2 = rstr->field_550 + arg->y2;
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

void NC_STACK_display::LockSurface()
{
}

void NC_STACK_display::UnlockSurface()
{
}

size_t NC_STACK_display::raster_func217(rstr_arg217 *arg)
{
    __NC_STACK_display *rstr = &stack__display;

    if ( arg->dword0 != 0xFFFFFFFF )
        rstr->field_4 = arg->dword0;

    if ( arg->dword4 != 0xFFFFFFFF )
        rstr->field_8 = arg->dword4;

//    if ( arg->dword8 != 0xFFFFFFFF )
//        rstr->BG_Color = arg->dword8;

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
    __NC_STACK_display *rstr = &stack__display;

    rstr->field_38.x1 = rstr->field_54c + arg->x1;
    rstr->field_38.y1 = rstr->field_550 + arg->y1;
    rstr->field_38.x2 = rstr->field_54c + arg->x2;
    rstr->field_38.y2 = rstr->field_550 + arg->y2;
}




void NC_STACK_display::display_func261(int ID, UA_PALETTE &pal, int from, int num)
{
    for (int i = 0; i < num; i++)
        stack__display.field_300[ID][from + i] = pal[i];
}

void NC_STACK_display::display_func261(int ID, UA_PALETTE &pal)
{
    stack__display.field_300[ID] = pal;
}

void NC_STACK_display::display_func262(rstr_262_arg *arg)
{
    __NC_STACK_display *displ = &stack__display;

    for (int i = 0; i < 256; i++)
    {
        int tmpr = 0;
        int tmpg = 0;
        int tmpb = 0;

        for (int j = 0; j < arg->cnt; j++)
        {
            UA_PALETTE &pal = displ->field_300[ arg->slot[j] ];
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

        displ->palette[i].r = tmpr;
        displ->palette[i].g = tmpg;
        displ->palette[i].b = tmpb;
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


bool NC_STACK_display::AllocTexture(bitmap_intern *pbitm)
{
    bitmap_intern *bitm = pbitm;

    bitm->pitch = bitm->width;
    bitm->buffer = AllocVec(bitm->width * bitm->height, 65537);
    bitm->flags |= BITMAP_FLAG_TEXTURE;
    return bitm->buffer != NULL;
}

void NC_STACK_display::TextureApplyPalette(bitmap_intern *)
{
}

void NC_STACK_display::FreeTexture(bitmap_intern *pbitm)
{
    bitmap_intern *bitm = pbitm;
    if (bitm->buffer)
    {
        nc_FreeMem(bitm->buffer);
        bitm->buffer = NULL;
    }
}

size_t NC_STACK_display::LockTexture(bitmap_intern *)
{
    return 1;
}

void NC_STACK_display::UnlockTexture(bitmap_intern *)
{
}

UA_PALETTE * NC_STACK_display::display_func273(int paletteId)
{
    return &stack__display.field_300[paletteId];
}

void NC_STACK_display::display_func274(const char **)
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
};


void NC_STACK_display::setRSTR_FGpen(uint32_t pen)
{
    stack__display.field_4 = pen;
}

void NC_STACK_display::setRSTR_BGpen(uint32_t pen)
{
//    stack__display.BG_Color = pen;
}

void NC_STACK_display::setRSTR_shdRmp(bitmap_intern *rmp)
{
//	rstr->field_10 = rmp;
//	if ( rmp )
//		rstr->field_14 = rmp->buffer;
}

void NC_STACK_display::setRSTR_trcRmp(bitmap_intern *rmp)
{
//	rstr->field_18 = rmp;
//	if ( rmp )
//		rstr->field_1c = rmp->buffer;
}

void NC_STACK_display::setRSTR_FGApen(uint32_t pen)
{
    stack__display.field_8 = pen;
}


UA_PALETTE *NC_STACK_display::GetPalette()
{
    return &stack__display.palette;
}

int16_t NC_STACK_display::GetWidth()
{
    return _width;
}

int16_t NC_STACK_display::GetHeight()
{
    return _height;
}


size_t NC_STACK_display::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( *(IDVList *)data );
    case 1:
        return (size_t)func1();
    case 2:
        return func2( *(IDVList *)data );
    case 3:
        return func3( *(IDVList *)data );
    case 192:
        return (size_t)raster_func192( (IDVPair *)data );
//    case 193:
//        return (size_t)raster_func193( (bitmap_intern **)data );
    case 198:
        return (size_t)raster_func198( (w3d_func198arg *)data );
    case 199:
        return (size_t)raster_func199( (w3d_func199arg *)data );
    case 200:
        return (size_t)raster_func200( (w3d_func198arg *)data );
    case 201:
        return (size_t)raster_func201( (w3d_func199arg *)data );
    case 202:
        return (size_t)raster_func202( (rstr_arg204 *)data );
    case 203:
        return (size_t)raster_func203( (IDVPair *)data );
    case 204:
        return (size_t)raster_func204( (rstr_arg204 *)data );
    case 205:
        return (size_t)raster_func205( (IDVPair *)data );
    case 206:
        return (size_t)raster_func206( (polysDatSub *)data );
    case 207:
        raster_func207( (rstr_207_arg *)data );
        return 1;
    case 208:
        raster_func208( (rstr_207_arg *)data );
        return 1;
    case 209:
        raster_func209( (w3d_a209 *)data );
        return 1;
    case 210:
        raster_func210( (ua_fRect *)data );
        return 1;
    case 211:
        raster_func211( (ua_dRect *)data );
        return 1;
    case 212:
        return (size_t)raster_func212( (IDVPair *)data );
    case 213:
        BeginScene();
        return 1;
    case 214:
        EndScene();
        return 1;
    case 215:
        LockSurface();
        return 1;
    case 216:
        UnlockSurface();
        return 1;
    case 217:
        return (size_t)raster_func217( (rstr_arg217 *)data );
    case 218:
        raster_func218( (rstr_218_arg *)data );
        return 1;
    case 219:
        return (size_t)raster_func219( (IDVPair *)data );
    case 220:
        return (size_t)raster_func220( (IDVPair *)data );
    case 221:
        raster_func221( (ua_dRect *)data );
        return 1;
    case 261:
        //display_func261( (rstr_261_arg *)data );
        return 1;
    case 262:
        display_func262( (rstr_262_arg *)data );
        return 1;
    case 263:
        display_func263( (displ_arg263 *)data );
        return 1;
//    case 264:
//        display_func264( (void *)data );
//        return 1;
//    case 265:
//        display_func265( (void *)data );
//        return 1;
    case 266:
        return (size_t)AllocTexture( (bitmap_intern *)data );
    case 267:
        TextureApplyPalette( (bitmap_intern *)data );
        return 1;
    case 268:
        FreeTexture( (bitmap_intern *)data );
        return 1;
    case 269:
        return (size_t)LockTexture( (bitmap_intern *)data );
    case 270:
        UnlockTexture( (bitmap_intern *)data );
        return 1;
    case 273:
        return (size_t)display_func273( (size_t)data );
    case 274:
        display_func274( (const char **)data );
        return 1;
    default:
        break;
    }
    return NC_STACK_nucleus::compatcall(method_id, data);
}
