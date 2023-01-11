#ifndef ILBM_H_INCLUDED
#define ILBM_H_INCLUDED

#include "bitmap.h"

struct BMHD_type
{
    uint16_t width;
    uint16_t height;
    uint16_t x;
    uint16_t y;
    int8_t nPlanes;
    int8_t masking;
    int8_t compression;
    int8_t flags;
    uint16_t transparentColor;
    int8_t xAspect;
    int8_t yAspect;
    uint16_t pageWidth;
    uint16_t pageHeight;
};

struct VBMP_type
{
    uint16_t width;
    uint16_t height;
    uint16_t flags;
};

class NC_STACK_ilbm: public NC_STACK_bitmap
{
public:
    virtual size_t Init(IDVList &stak);
    virtual size_t LoadingFromIFF(IFFile **file);
    virtual size_t SavingIntoIFF(IFFile **pmfile);
    virtual rsrc * rsrc_func64(IDVList &stak);
    virtual size_t rsrc_func66(rsrc_func66_arg *arg);

    NC_STACK_ilbm() {};
    virtual ~NC_STACK_ilbm() {};
    
    virtual const std::string ClassName() const {
        return __ClassName;
    };

    enum ILBM_ATT
    {
        ILBM_ATT_FMT = 0x80003000,
        ATT_ALPHAPALETTE = 0x80003001, // Special mode for compatibility
    };

    virtual void setILBM_saveFmt(int);

    virtual int getILBM_saveFmt();

    size_t ilbm_func5__sub0(NC_STACK_ilbm *obj, IFFile **pmfile);
    rsrc * READ_ILBM(IDVList &stak, IFFile *mfil, int val5);

    //Data
    static constexpr const char * __ClassName = "ilbm.class";

public:
    bool _saveAsIlbm = false;
};

#endif // ILBM_H_INCLUDED
