#ifndef ILBM_H_INCLUDED
#define ILBM_H_INCLUDED

#include "bitmap.h"

struct __NC_STACK_ilbm
{
    uint32_t flags;
};


struct __attribute__((packed)) BMHD_type
{
    uint16_t width;
    uint16_t height;
    uint16_t x;
    uint16_t y;
    char nPlanes;
    char masking;
    char compression;
    char flags;
    uint16_t transparentColor;
    char xAspect;
    char yAspect;
    uint16_t pageWidth;
    uint16_t pageHeight;
};

struct __attribute__((packed)) VBMP_type
{
    uint16_t width;
    uint16_t height;
    uint16_t flags;
};

class NC_STACK_ilbm: public NC_STACK_bitmap
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func2(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
    virtual size_t func5(IFFile **file);
    virtual size_t func6(IFFile **pmfile);
    virtual rsrc * rsrc_func64(stack_vals *stak);
    virtual size_t rsrc_func66(rsrc_func66_arg *arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_ilbm() {
        memset(&stack__ilbm, 0, sizeof(stack__ilbm));
    };
    virtual ~NC_STACK_ilbm() {};

    virtual const char * getClassName() {
        return "ilbm.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_ilbm();
    };

    enum ILBM_ATT
    {
        ILBM_ATT_FMT = 0x80003000
    };

    virtual void setILBM_saveFmt(int);

    virtual int getILBM_saveFmt();

    size_t ilbm_func5__sub0(NC_STACK_ilbm *obj, IFFile **pmfile);
    rsrc * READ_ILBM(stack_vals *stak, IFFile *mfil, int val5);

    //Data
    static const NewClassDescr description;

    __NC_STACK_ilbm stack__ilbm;
};

#endif // ILBM_H_INCLUDED
