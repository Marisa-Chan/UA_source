#ifndef BMPANM_H_INCLUDED
#define BMPANM_H_INCLUDED

#include "rsrc.h"
#include "engine_gfx.h"
#include "bitmap.h"

struct bmpAnim_t2
{
    tUtV *outline;
    ResBitmap *bitm;
    int frm_time;
    int16_t bitm_index;
    int16_t otl_index;
};

struct bmpAnim_t1_objs
{
    NC_STACK_bitmap *bitmObj;
    ResBitmap *bitm_intern;
    char *title;
};

struct bmpAnim_t1
{
    char *titles;
    tUtV *otl_buff;
    bmpAnim_t1_objs *bitm_buff;
    char *className;
    bmpAnim_t2 *start_frame;
    bmpAnim_t2 *end_frame;
    int16_t frame_cnt;
    int16_t titles_size;
    int16_t otl_buff_cnt;
    int16_t bitm_buff_cnt;
};

struct __NC_STACK_bmpanim
{
    bmpAnim_t1 *bmpanm_intern;
    bmpAnim_t2 *current_frame;
    int time_stmp;
    int time_ovr;
    int16_t anim_type;
    int16_t frm_adds;
};

struct __attribute__((packed)) bmpanm_loc
{
    int32_t frm_time;
    int16_t frame_id;
    int16_t uv_id;
};

class NC_STACK_bmpanim: public NC_STACK_bitmap
{
public:
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();
    virtual size_t func5(IFFile **file);
    virtual size_t func6(IFFile **file);
    virtual rsrc * rsrc_func64(IDVList &stak);
    virtual size_t rsrc_func65(rsrc *pres);
    virtual size_t rsrc_func66(rsrc_func66_arg *sv);
    virtual void bitmap_func130(bitmap_arg130 *arg);

    NC_STACK_bmpanim() {
        memset(&stack__bmpanim, 0, sizeof(stack__bmpanim));
    };
    virtual ~NC_STACK_bmpanim() {};
    
    virtual const std::string &GetClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_bmpanim();
    };

    enum BANM_ATT
    {
        BANM_ATT_NAME = 0x80003000,
        BANM_ATT_CLASSNAME = 0x80003001,
        BANM_ATT_FILE_TITLES = 0x80003002,
        BANM_ATT_OUTLINES = 0x80003003,
        BANM_ATT_ANIMTYPE = 0x80003004,
        BANM_ATT_FRAMECNT = 0x80003005,
        BANM_ATT_SEQDATA = 0x80003006
    };

    //Set
    virtual void setBANM_animType(int newType);

    //Get
    virtual ResBitmap * GetResBmp();
    virtual int getBMD_width();
    virtual int getBMD_height();
    virtual void *getBMD_buffer();
    virtual const char *getBANM_name();
    virtual const char *getBANM_classname();
    virtual int getBANM_framecnt();
    virtual int getBANM_animtype();
    
    
    virtual void PrepareTexture( bool force = false );

    //Data
    static const Nucleus::ClassDescr description;

    __NC_STACK_bmpanim stack__bmpanim;
};

#endif // BITMAP_H_INCLUDED
