#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include "engine_gfx.h"

struct xyxyNNN
{
    int x1;
    int y1;
    int x2;
    int y2;
    int NNN;
};

struct ua_dRect
{
    int x1;
    int y1;
    int x2;
    int y2;
};

struct ua_fRect
{
    float x1;
    float y1;
    float x2;
    float y2;
};


struct __NC_STACK_display
{
    bitmap_intern *bitm_intern;
    DWORD field_4; // Color?
    DWORD field_8; // and Color2 ?
    DWORD field_c;
//  int field_10;
//  int field_14;
//  int field_18;
//  int field_1c;
//  int field_20;
    xyxyNNN field_24;
    xyxyNNN field_38;
//  int field_4c;
//  int field_50;
//  int field_54;
//  int field_58;
//  int field_5c;
//  int field_60;
//  int field_64;
//  int field_68;
//  int field_6c;
//  int field_70;
//  int field_74;
//  int field_78;
//  int field_7c;
//  int field_80;
//  int field_84;
//  int field_88;
//  int field_8c;
//  int field_90;
//  int field_94;
//  int field_98;
//  int field_9c;
//  int field_a0;
//  int field_a4;
//  int field_a8;
//  int field_ac;
//  int field_b0;
//  int field_b4;
//  int field_b8;
//  int field_bc;
//  int field_c0;
//  int field_c4;
//  int field_c8;
//  int field_cc;
//  int field_d0;
//  int field_d4;
//  int field_d8;
//  int field_dc;
//  int field_e0;
//  int field_e4;
//  int field_e8;
//  int field_ec;
//  int field_f0;
//  int field_f4;
//  int field_f8;
//  int field_fc;
//  int field_100;
//  int field_104;
//  int field_108;
//  int field_10c;
//  int field_110;
//  int field_114;
//  int field_118;
//  int field_11c;
//  int field_120;
//  int field_124;
//  int field_128;
//  int field_12c;
//  int field_130;
//  int field_134;
//  int field_138;
//  int field_13c;
//  int field_140;
//  int field_144;
//  int field_148;
    tiles_stru *tiles[256];
    int field_54c;
    int field_550;
    float field_554;
    float field_558;
//  int field_55c;
//  int field_560;
//  int field_564;
//  int field_568;
//  int field_56c;
//  int field_570;
//  int field_574;
//  int field_578;
//  int field_57c;
//  int field_580;
//  int field_584;
//  int field_588;
//  int field_58c;
//  int field_590;
//  int field_594;
//  int field_598;
//  int field_59c;


    UA_PALETTE palette;
    UA_PALETTE field_300[8];
    bitmap_intern *pointer_bitm;
    int field_1b04;
};




struct w3d_func198arg
{
    float x1;
    float y1;
    float x2;
    float y2;
};

struct w3d_func199arg
{
    int x1;
    int y1;
    int x2;
    int y2;
};

struct rstr_arg204
{
    bitmap_intern *pbitm;
    float float4;
    float float8;
    float floatC;
    float float10;
    float float14;
    float float18;
    float float1C;
    float float20;
};

struct rstr_loc204
{
    bitmap_intern *pbitm;
    int dword4;
    int dword8;
    int dwordC;
    int dword10;
    int dword14;
    int dword18;
    int dword1C;
    int dword20;
    int dword24;
};

struct rstr_arg217
{
    DWORD dword0;
    DWORD dword4;
    DWORD dword8;
};

struct rstr_218_arg
{
    bitmap_intern *bitm_intern;
    bitmap_intern *bitm_intern2;
    int flg;
    ua_fRect rect1;
    ua_fRect rect2;
};

struct rstr_261_arg
{
    int pal_id;
    int entrie_id;
    int pal_num;
    UA_PALETTE *palette;
};

struct rstr_262_arg
{
    int dword0;
    int *pdword4;
    int *pdword8;
};

struct displ_arg263
{
    bitmap_intern *bitm;
    int pointer_id;
};

struct windd_arg256
{
    int sort_id;
    int width;
    int height;
    char name[32];
};

class NC_STACK_display: public NC_STACK_bitmap
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func1(stack_vals *stak);
    virtual size_t func2(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
    virtual size_t raster_func192(stack_vals *);
    virtual size_t raster_func193(bitmap_intern **out);
    virtual size_t raster_func198(w3d_func198arg *);
    virtual size_t raster_func199(w3d_func199arg *);
    virtual size_t raster_func200(w3d_func198arg *);
    virtual size_t raster_func201(w3d_func199arg *);
    virtual size_t raster_func202(rstr_arg204 *);
    virtual size_t raster_func203(stack_vals *);
    virtual size_t raster_func204(rstr_arg204 *);
    virtual size_t raster_func205(stack_vals *);
    virtual size_t raster_func206(polysDatSub *);
    virtual void raster_func207(rstr_207_arg *arg);
    virtual void raster_func208(rstr_207_arg *arg);
    virtual void raster_func209(w3d_a209 *);
    virtual void raster_func210(ua_fRect *arg);
    virtual void raster_func211(ua_dRect *arg);
    virtual size_t raster_func212(stack_vals *);
    virtual void raster_func213(polysDatSub *);
    virtual size_t raster_func214(void *);
    virtual void raster_func215(void *);
    virtual void raster_func216(void *);
    virtual size_t raster_func217(rstr_arg217 *arg);
    virtual void raster_func218(rstr_218_arg *arg);
    virtual size_t raster_func219(stack_vals *);
    virtual size_t raster_func220(stack_vals *);
    virtual void raster_func221(ua_dRect *arg);
    virtual size_t display_func256(windd_arg256 *inout) {
        return 0;
    };
    virtual void display_func257(stack_vals *) {};
    virtual void display_func258(stack_vals *) {};
    virtual void display_func259(stack_vals *) {};
    virtual void display_func260(stack_vals *) {};
    virtual void display_func261(rstr_261_arg *arg);
    virtual void display_func262(rstr_262_arg *arg);
    virtual void display_func263(displ_arg263 *arg);
    virtual void display_func264(void *);
    virtual void display_func265(void *);
    virtual size_t display_func266(bitmap_intern **pbitm);
    virtual void display_func267(bitmap_intern **);
    virtual void display_func268(bitmap_intern **pbitm);
    virtual size_t display_func269(bitmap_intern **);
    virtual void display_func270(bitmap_intern **);
    virtual void display_func271(stack_vals *stak) {};
    virtual void display_func272(stack_vals *) {};
    virtual UA_PALETTE * display_func273(rstr_261_arg *arg);
    virtual void display_func274(const char **);


    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_display() {
        memset(&stack__display, 0, sizeof(stack__display));
    };
    virtual ~NC_STACK_display() {};

    virtual const char * getClassName() {
        return "display.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_display();
    };

    enum DISP_ATT
    {
        DISP_ATT_DISPLAY_ID = 0x80004000,
        DISP_ATT_DISPLAY_INF = 0x80004001,
    };


    enum RSTR_RFLAGS
    {
        RSTR_RFLAGS_LINMAP = 0x1,
        RSTR_RFLAGS_PERSPMAP = 0x2,
        RSTR_RFLAGS_FLATSHD = 0x4,
        RSTR_RFLAGS_GRADSHD = 0x8,
        RSTR_RFLAGS_ZEROTRACY = 0x10,
        RSTR_RFLAGS_LUMTRACY = 0x20
    };

    enum RSTR_ATT
    {
        RSTR_ATT_FGPEN = 0x80003000,
        RSTR_ATT_BGPEN = 0x80003001,
        RSTR_ATT_SHADE_RMP = 0x80003002,
        RSTR_ATT_TRACY_RMP = 0x80003003,
        RSTR_ATT_FGAPEN = 0x80003004
    };



    //Set
    virtual void setBMD_palette(UA_PALETTE *);

    virtual void setRSTR_FGpen(uint32_t pen);
    virtual void setRSTR_BGpen(uint32_t pen);
    virtual void setRSTR_shdRmp(bitmap_intern *rmp);
    virtual void setRSTR_trcRmp(bitmap_intern *rmp);
    virtual void setRSTR_FGApen(uint32_t pen);

    //Get
    virtual UA_PALETTE *getBMD_palette();

    //Data
    static const NewClassDescr description;

    __NC_STACK_display stack__display;
};

#endif // DISPLAY_H_INCLUDED
