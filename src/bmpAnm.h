#ifndef BMPANM_H_INCLUDED
#define BMPANM_H_INCLUDED

#include "rsrc.h"
#include "system/gfx.h"
#include "bitmap.h"


struct BAnmFrame
{
    int32_t FrameTime = 0;
    int16_t FrameID = 0;
    int16_t TexCoordsID = 0;
};

struct BAnmFrameCache : BAnmFrame
{   
    std::vector<tUtV> *pTexCoords = NULL;
    NC_STACK_bitmap *pBitmap = NULL;
};

struct ResBmpAnm
{
    std::vector< std::string > BitmapNames;
    std::vector< std::vector<tUtV> > TexCoords;
    std::vector<NC_STACK_bitmap *> Bitmaps;
    std::string ClassName;
    std::vector<BAnmFrameCache> FrameData;
    
    ~ResBmpAnm()
    {
        for (NC_STACK_bitmap *bitm : Bitmaps)
        {
            Nucleus::Delete(bitm);
        }
    }
};



class NC_STACK_bmpanim: public NC_STACK_bitmap
{
public:
    virtual size_t Init(IDVList &stak);
    virtual size_t Deinit();
    virtual size_t LoadingFromIFF(IFFile **file);
    virtual size_t SavingIntoIFF(IFFile **file);
    virtual rsrc * rsrc_func64(IDVList &stak);
    virtual size_t rsrc_func65(rsrc *pres);
    virtual size_t rsrc_func66(rsrc_func66_arg *sv);
    virtual void SetTime(int32_t timeStamp, int32_t frameTime);

    NC_STACK_bmpanim() {};
    virtual ~NC_STACK_bmpanim() {};
    
    virtual const std::string &ClassName() const {
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
    virtual ResBitmap * GetBitmap();
    virtual std::vector<tUtV> &GetOutline() override;
    virtual int getBMD_width();
    virtual int getBMD_height();
    virtual int getBANM_framecnt();
    virtual int getBANM_animtype();
    
    
    virtual void PrepareTexture( bool force = false );
    
    virtual bool IsDynamic() const override { return true; };
    
    static ResBmpAnm *LoadFromFile(const std::string &name, IFFile *iff);
    
    static bool ReadClassName(FSMgr::iFileHandle *fil, ResBmpAnm *arg);
    static bool ReadBitmapNames(FSMgr::iFileHandle *fil, ResBmpAnm *arg);
    static bool ReadTexCoords(FSMgr::iFileHandle *fil, ResBmpAnm *arg);
    static bool ReadFrameData(FSMgr::iFileHandle *fil, ResBmpAnm *arg);
    
    static ResBmpAnm * ConstructBAnm(const std::string &className, std::vector<std::string> *a2, std::vector< std::vector<tUtV> >*a3, int a4, BAnmFrame *a5);
    
    static bool WriteToFile(ResBmpAnm *banm, const std::string &name, IFFile *iff);
    
    static bool WriteClassName(FSMgr::iFileHandle *fil, ResBmpAnm *arg);
    static bool WriteBitmapNames(FSMgr::iFileHandle *fil, ResBmpAnm *arg);
    static bool WriteTexCoords(FSMgr::iFileHandle *fil, ResBmpAnm *arg);
    static bool WriteFrameData(FSMgr::iFileHandle *fil, ResBmpAnm *arg);

    //Data
    static const Nucleus::ClassDescr description;

public:
    ResBmpAnm *_pData = NULL;
    int32_t _curFrame = 0;
    int32_t _timeStamp = 0;
    int32_t _leftTime = 0;
    int8_t _animType = 0;
    int8_t _frmAdd = 0;
};

#endif // BITMAP_H_INCLUDED
