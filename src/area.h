#ifndef AREA_H_INCLUDED
#define AREA_H_INCLUDED

#include <inttypes.h>
#include "ade.h"
#include "bitmap.h"


class NC_STACK_area;

struct __NC_STACK_area
{
    NC_STACK_bitmap *texImg;
    NC_STACK_bitmap *tracyImg;
    int flags;
    char colorVal;
    char tracyVal;
    char shadeVal;
//    char field_F;
//    int field_10;
    int16_t polnum;
    int16_t polflags;
//    int field_18;
//    int field_1c;
//    int field_20;
//    int field_24;
};


struct AREA_STRC
{
    int16_t version;
    uint16_t flags;
    uint16_t polFlags;
    uint8_t _un1;
    uint8_t clrVal;
    uint8_t trcVal;
    uint8_t shdVal;
};


class NC_STACK_area: public NC_STACK_ade
{
public:
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();
    virtual size_t func5(IFFile **file);
    virtual size_t func6(IFFile **file);
    virtual size_t ade_func65(area_arg_65 *arg);

    NC_STACK_area() {
        memset(&stack__area, 0, sizeof(stack__area));
    };
    virtual ~NC_STACK_area() {};
    
    virtual const std::string &GetClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_area();
    };

    enum AREA_FLAG
    {
        AREA_FLAG_DPTHFADE = 1,
        AREA_FLAG_BKCHECK = 2
    };

    enum AREA_POL_FLAG
    {
        AREA_POL_FLAG_SCANLN = 0x1,
        AREA_POL_FLAG_HORIZ = (AREA_POL_FLAG_SCANLN),
        AREA_POL_FLAG_VERT = 0x0, // AREA_POL_FLAG_SCANLN BIT

        AREA_POL_FLAG_MAPBIT1 = 0x2,
        AREA_POL_FLAG_MAPBIT2 = 0x4,
        AREA_POL_FLAG_NONMAPPED = 0x0, // AREA_POL_FLAG_MAPBIT(1|2) BITS
        AREA_POL_FLAG_LINEARMAPPED = (AREA_POL_FLAG_MAPBIT1),
        AREA_POL_FLAG_DEPTHMAPPED = (AREA_POL_FLAG_MAPBIT1 | AREA_POL_FLAG_MAPBIT2),

        AREA_POL_FLAG_TEXBIT = 0x8,
        AREA_POL_FLAG_NOTEX = 0x0,
        AREA_POL_FLAG_TEXUTRED = (AREA_POL_FLAG_TEXBIT),

        AREA_POL_FLAG_SHADEBIT1 = 0x10,
        AREA_POL_FLAG_SHADEBIT2 = 0x20,
        AREA_POL_FLAG_NOSHADE = 0x0,
        AREA_POL_FLAG_FLATSHADE = (AREA_POL_FLAG_SHADEBIT1),
        AREA_POL_FLAG_LINESHADE = (AREA_POL_FLAG_SHADEBIT2),
        AREA_POL_FLAG_GRADIENTSHADE = (AREA_POL_FLAG_SHADEBIT1 | AREA_POL_FLAG_SHADEBIT2),

        AREA_POL_FLAG_TRACYBIT1 = 0x40,
        AREA_POL_FLAG_TRACYBIT2 = 0x80,
        AREA_POL_FLAG_NOTRACY = 0x0,
        AREA_POL_FLAG_CLEARTRACY = (AREA_POL_FLAG_TRACYBIT1),
        AREA_POL_FLAG_FLATTRACY = (AREA_POL_FLAG_TRACYBIT2),
        AREA_POL_FLAG_TRACYMAPPED = (AREA_POL_FLAG_TRACYBIT1 | AREA_POL_FLAG_TRACYBIT2),


        AREA_POL_FLAG_TRACYBIT3 = 0x100,
        AREA_POL_FLAG_DARK = 0x0,
        AREA_POL_FLAG_LIGHT = (AREA_POL_FLAG_TRACYBIT3)
    };

    enum AREA_ATT
    {
        AREA_ATT_TEXBITM = 0x80002000,
        AREA_ATT_COLORVAL = 0x80002001,
        AREA_ATT_MAP = 0x80002004,
        AREA_ATT_TEX = 0x80002005,
        AREA_ATT_SHADE = 0x80002006,
        AREA_ATT_TRACY = 0x80002007,
        AREA_ATT_TRACYMODE = 0x80002008,
        AREA_ATT_TRACYBITM = 0x80002009,
        AREA_ATT_SHADEVAL = 0x8000200A,
        AREA_ATT_TRACYVAL = 0x8000200B,
        AREA_ATT_BLOB1 = 0x8000200C,
        AREA_ATT_BLOB2 = 0x8000200D
//        AREA_ATT_PolInfo = 0x8000200E
    };

    virtual void setADE_depthFade(int);
    virtual void setADE_poly(int);

    virtual void setAREA_bitm(NC_STACK_bitmap *);
    virtual void setAREA_colorVal(int);
    virtual void setAREA_map(int);
    virtual void setAREA_tex(int);
    virtual void setAREA_shade(int);
    virtual void setAREA_tracy(int);
    virtual void setAREA_tracymode(int);
    virtual void setAREA_tracybitm(NC_STACK_bitmap *);
    virtual void setAREA_shadeVal(int);
    virtual void setAREA_tracyVal(int);
    virtual void setAREA_blob1(uint32_t);
    virtual void setAREA_blob2(uint32_t);

    virtual NC_STACK_bitmap *getAREA_bitm();
    virtual int getAREA_colorVal();
    virtual int getAREA_map();
    virtual int getAREA_tex();
    virtual int getAREA_shade();
    virtual int getAREA_tracy();
    virtual int getAREA_tracymode();
    virtual NC_STACK_bitmap *getAREA_tracybitm();
    virtual int getAREA_shadeVal();
    virtual int getAREA_tracyVal();


    int area_func5__sub0(IFFile *mfile);
    int area_func5__sub1(IFFile *mfile);


    //Data
    static const Nucleus::ClassDescr description;

    __NC_STACK_area stack__area;
};

#endif // AREA_H_INCLUDED
