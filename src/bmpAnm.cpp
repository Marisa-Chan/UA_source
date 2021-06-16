#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "bitmap.h"
#include "bmpAnm.h"
#include "utils.h"


const Nucleus::ClassDescr NC_STACK_bmpanim::description("bmpanim.class", &newinstance);


size_t NC_STACK_bmpanim::Init(IDVList &stak)
{
//    if (stak)
//    {
//        IDVList::iterator it = stak->find(BANM_ATT_NAME);
//        if ( it != stak.end() )
//            (*it).id = RSRC_ATT_NAME;
//    }

    if ( !NC_STACK_bitmap::Init(stak) )
        return 0;

    _pData = (ResBmpAnm *)getRsrc_pData();
    _frmAdd = 1;
    _animType = 0;
    _leftTime = 0;
    _curFrame = 0;

    _animType = stak.Get<int32_t>(BANM_ATT_ANIMTYPE, 0);

    return 1;
}

size_t NC_STACK_bmpanim::Deinit()
{
    return NC_STACK_bitmap::Deinit();
}


size_t NC_STACK_bmpanim::LoadingFromIFF(IFFile **file)
{
    char buf[250];

    IFFile *mfile = *file;

    int16_t animType = 0;
    int16_t version = 0;
    int16_t offset = 0;
    std::string anmName;

    while ( 1 )
    {
        int v8 = mfile->parse();
        if ( v8 == IFFile::IFF_ERR_EOC )
            break;
        if ( v8 )
            return 0;

        if ( mfile->GetCurrentChunk().Is(TAG_STRC) )
        {
            version = mfile->readS16B();
            offset = mfile->readS16B();
            animType = mfile->readS16B();
            mfile->read(buf, 250);

            if ( version >= 1 )
            {
                anmName.assign( buf + offset - 6 );
            }
            mfile->parse();
        }
        else
        {
            mfile->skipChunk();
        }
    }

    if ( !version )
        return 0;
    
    IDVList stak {
        {RSRC_ATT_NAME, anmName},
        {BANM_ATT_NAME, anmName},
        {BANM_ATT_ANIMTYPE, (int32_t)animType}};

    return Init( stak );
}

size_t NC_STACK_bmpanim::SavingIntoIFF(IFFile **file)
{
    IFFile *mfile = *file;

    const char *a3 = getRsrc_name();

    if ( !a3 )
    {
        return 0;
    }

    rsrc_func66_arg sv;
    sv.OpenedStream = 1;
    sv.filename = a3;
    sv.file = NULL;

    if ( rsrc_func66(&sv) != 1 )
    {
        ypa_log_out("bmpanim.class/OM_SAVETOIFF: couldn't save resource.\n");
        return 0;
    }

    if ( mfile->pushChunk(TAG_BANI, TAG_FORM, -1) )
    {
        return 0;
    }

    mfile->pushChunk(0, TAG_STRC, -1);

    mfile->writeS16B(1); //version
    mfile->writeS16B(6); //offset of name
    mfile->writeS16B(_animType);

    mfile->write(a3, strlen(a3) + 1);
    mfile->popChunk();
    return mfile->popChunk() == IFFile::IFF_ERR_OK;
}

bool NC_STACK_bmpanim::ReadClassName(FSMgr::iFileHandle *fil, ResBmpAnm *arg)
{
    int16_t sz = fil->readS16B();

    if ( fil->readErr() )
        return false;

    char *tmp = new char[sz]();
    
    if ( fil->read(tmp, sz) != (size_t)sz )
    {
        delete[] tmp;
        return false;
    }
    
    arg->ClassName.assign(tmp);
    delete[] tmp;
    
    return true;
}

bool NC_STACK_bmpanim::ReadBitmapNames(FSMgr::iFileHandle *fil, ResBmpAnm *arg)
{
    int16_t sz = fil->readS16B();

    if ( fil->readErr() )
        return false;

    char *tmp = new char[sz]();
    
    if ( fil->read(tmp, sz) != (size_t)sz )
    {
        delete[] tmp;
        return false;
    }
    
    std::string buf(tmp, sz);
    delete[] tmp;
    
    arg->BitmapNames = Stok::Split(buf, std::string("\0", 1));
    
    if (arg->BitmapNames.empty())
        return false;
    
    arg->Bitmaps.resize( arg->BitmapNames.size() );
    
    for (size_t i = 0; i < arg->BitmapNames.size(); ++i)
    {
        arg->Bitmaps[i] = Nucleus::CTFInit<NC_STACK_bitmap>(arg->ClassName, {
            {NC_STACK_rsrc::RSRC_ATT_NAME, arg->BitmapNames[i]},
            {NC_STACK_rsrc::RSRC_ATT_DONTCOPY, (int32_t)1}} );

        if ( !arg->Bitmaps[i] )
            return false;
    }
    
    return true;
}

bool NC_STACK_bmpanim::ReadTexCoords(FSMgr::iFileHandle *fil, ResBmpAnm *arg)
{
    int16_t cnt = fil->readS16B();
    
    if ( fil->readErr() )
        return false;

    if (arg->Bitmaps.size())
        arg->TexCoords.reserve( arg->Bitmaps.size() );

    for (int i = 0; i < cnt; i++)
    {
        arg->TexCoords.emplace_back();
        std::vector<tUtV> &coords = arg->TexCoords.back();

        uint16_t ttmp = fil->readS16B();
        
        if ( fil->readErr() )
            return false;
        
        coords.resize(ttmp);
        for (size_t j = 0; j < ttmp; j++)
        {
            uint8_t uv[2];
            if (fil->read(uv, 2) != 2)
                return false;

            coords[j].tu = (float)uv[0] / 256.0;
            coords[j].tv = (float)uv[1] / 256.0;
        }
        i += ttmp;
    }

    return true;
}

bool NC_STACK_bmpanim::ReadFrameData(FSMgr::iFileHandle *fil, ResBmpAnm *arg)
{
    int16_t cnt = fil->readS16B();
    
    if ( fil->readErr() )
        return false;
    
    arg->FrameData.resize(cnt);
    
    for (int16_t i = 0; i < cnt; ++i)
    {
        BAnmFrameCache &frm = arg->FrameData[i];
        
        frm.FrameTime = fil->readS32B();
        frm.FrameID = fil->readS16B();
        frm.TexCoordsID = fil->readS16B();
        
        frm.pTexCoords = &arg->TexCoords.at( frm.TexCoordsID );
        frm.pBitmap = arg->Bitmaps.at(frm.FrameID);
    }
    return true;
}

ResBmpAnm *NC_STACK_bmpanim::LoadFromFile(const std::string &name, IFFile *iff)
{
    IFFile iffOpened;
    FSMgr::FileHandle fil;
    FSMgr::iFileHandle *pfile = iff;

    if ( !iff )
    {                
        fil = uaOpenFile( std::string("rsrc:rsrcpool/") + name, "rb" );
        if (!fil.OK())
            return NULL;
        
        uint32_t tmp = fil.readU32B();
        fil.seek(0, SEEK_SET);

        if ( tmp != TAG_FORM )
            pfile = &fil;
        else
        {
            iffOpened = IFFile(fil);
            pfile = &iffOpened;
            
            iff = &iffOpened;
        }
    }
    
    if (iff)
    {
        if ( (iff->parse() | iff->parse()) != IFFile::IFF_ERR_OK )
            return NULL;
    }

    if ( !pfile->OK() )
        return NULL;

    ResBmpAnm *pdata = new ResBmpAnm();
    if ( pdata )
    {
        bool ReadOK = false;
        
        if ( ReadClassName(pfile, pdata) )
        {
            if ( ReadBitmapNames(pfile, pdata) )
            {
                if ( ReadTexCoords(pfile, pdata) )
                {
                    if ( ReadFrameData(pfile, pdata) )
                        ReadOK = true;
                }
            }
        }
        
        if ( !ReadOK )
        {
            delete pdata;
            pdata = NULL;
        }
    }
    
    if (iff)
    {
        iff->parse();
        iff->parse();
    }

    return pdata;
}

ResBmpAnm * NC_STACK_bmpanim::ConstructBAnm(const std::string &className, std::vector<std::string> *a2, std::vector< std::vector<tUtV> >*a3, int num, BAnmFrame *frames)
{
    ResBmpAnm *banm = new ResBmpAnm();

    if ( !banm )
        return NULL;

    banm->ClassName = className;
    banm->BitmapNames = *a2;
    
    banm->Bitmaps.resize( banm->BitmapNames.size() );

    for (size_t i = 0; i < banm->BitmapNames.size(); i++)
    {
        banm->Bitmaps[i] = Nucleus::CTFInit<NC_STACK_bitmap>(className,
           {{NC_STACK_rsrc::RSRC_ATT_NAME, banm->BitmapNames[i]},
            {NC_STACK_rsrc::RSRC_ATT_DONTCOPY, (int32_t)1}} );
            
        if ( !banm->Bitmaps[i] )
        {
            delete banm;
            return NULL;
        }
    }
    
    banm->TexCoords = *a3;

    banm->FrameData.resize(num);

    for (int i = 0; i < num; i++)
    {
        BAnmFrameCache &out = banm->FrameData.at(i);
        BAnmFrame &in = frames[i];
                
        out.FrameTime = in.FrameTime;
        out.FrameID = in.FrameID;
        out.TexCoordsID = in.TexCoordsID;
        
        out.pTexCoords = &banm->TexCoords[ out.TexCoordsID ];
        out.pBitmap = banm->Bitmaps[ out.FrameID ];
        
    }

    return banm;
}

// Create bmpanim resource node and fill rsrc field data
rsrc * NC_STACK_bmpanim::rsrc_func64(IDVList &stak)
{
    stak.Add(RSRC_ATT_TOTAIL, (int32_t)1);

    rsrc *res = NC_STACK_bitmap::rsrc_func64(stak);// rsrc_func64
    if ( res )
    {

        const std::string a1 = stak.Get<std::string>(BANM_ATT_CLASSNAME, "");
        if ( !a1.empty() )
        {
            std::vector<std::string> *titles = stak.Get<std::vector<std::string> *>(BANM_ATT_FILE_TITLES, NULL);
            std::vector< std::vector<tUtV> > *opls = stak.Get< std::vector< std::vector<tUtV> >* >(BANM_ATT_OUTLINES, NULL);
            int num = stak.Get<int32_t>(BANM_ATT_FRAMECNT, 0);
            BAnmFrame *v7 = stak.Get<BAnmFrame *>(BANM_ATT_SEQDATA, NULL);

            if ( titles && opls && num && v7 )
                res->data = ConstructBAnm(a1, titles, opls, num, v7);
        }
        else
        {
            const std::string v9 = stak.Get<std::string>(RSRC_ATT_NAME, "");
            IFFile *iff = stak.Get<IFFile *>(RSRC_ATT_PIFFFILE, NULL);
            if ( !v9.empty() )
                res->data = LoadFromFile(v9, iff);
        }

        if ( !res->data )
        {
            rsrc_func65(res);
            return NULL;
        }

    }
    return res;
}

size_t NC_STACK_bmpanim::rsrc_func65(rsrc *res)
{
    ResBmpAnm *v5 = (ResBmpAnm *)res->data;

    if ( v5 )
    {
        delete v5;
        res->data = NULL;
    }
    return NC_STACK_bitmap::rsrc_func65(res);
}

bool NC_STACK_bmpanim::WriteClassName(FSMgr::iFileHandle *fil, ResBmpAnm *t1)
{
    if ( !fil->writeS16B( t1->ClassName.size() + 1 ) )
        return false;

    if ( fil->write( t1->ClassName.c_str(),  t1->ClassName.size() + 1) != t1->ClassName.size() + 1 )
        return false;
    return true;
}

bool NC_STACK_bmpanim::WriteBitmapNames(FSMgr::iFileHandle *fil, ResBmpAnm *t1)
{
    size_t sz = 0;
    for (std::string &n : t1->BitmapNames)
        sz += n.size() + 1;

    if ( !fil->writeS16B(sz) )
        return false;

    for (std::string &n : t1->BitmapNames)
    {
        sz = n.size() + 1;
        if ( fil->write(n.c_str(), sz) != sz )
            return false;
    }

    return true;
}

bool NC_STACK_bmpanim::WriteTexCoords(FSMgr::iFileHandle *fil, ResBmpAnm *t1)
{
    size_t num = 0;
    for(auto &n : t1->TexCoords)
        num += n.size();

    if ( !fil->writeS16B(num) )
        return false;
    
    for(auto &n : t1->TexCoords)
    {
        if ( !fil->writeS16B(n.size()) )
            return false;
        
        for (tUtV uv : n)
        {
            if ( !(fil->writeU8(uv.tu * 256.0)
                 && fil->writeU8(uv.tv * 256.0)) )
                return false;
        }
    }
    
    return true;
}

bool NC_STACK_bmpanim::WriteFrameData(FSMgr::iFileHandle *fil, ResBmpAnm *t1)
{
    if ( !fil->writeS16B(t1->FrameData.size()) )
        return false;

    for(const BAnmFrameCache &f : t1->FrameData)
    {
        if ( !(fil->writeS32B(f.FrameTime)
            && fil->writeS16B(f.FrameID)
            && fil->writeS16B(f.TexCoordsID) ) )
            return false;
    }
    return true;
}

bool NC_STACK_bmpanim::WriteToFile(ResBmpAnm *banm, const std::string &name, IFFile *iff)
{
    IFFile iffOpened;
    FSMgr::iFileHandle *pfile = iff;

    if ( !iff )
    {                
        iffOpened = IFFile::UAOpenIFFile( std::string("rsrc:rsrcpool/") + name, "wb" );
        if (!iffOpened.OK())
            return false;

        pfile = &iffOpened;
        iff = &iffOpened;
    }
    
    if (iff)
    {
        if ( (iff->pushChunk(TAG_VANM, TAG_FORM, -1) | iff->pushChunk(0, TAG_DATA, -1)) != IFFile::IFF_ERR_OK )
            return false;
    }
    
    
    bool wok = false;

    if ( WriteClassName(pfile, banm) )
    {
        if ( WriteBitmapNames(pfile, banm) )
        {
            if ( WriteTexCoords(pfile, banm) )
            {
                if ( WriteFrameData(pfile, banm) )
                    wok = true;
            }
        }
    }
    
    if (iff)
    {
        iff->popChunk();
        iff->popChunk();
    }
    
    return wok;
}

size_t NC_STACK_bmpanim::rsrc_func66(rsrc_func66_arg *sv)
{
    if ( !_pData )
        return 0;

    IFFile *mfile = NULL;
    const char *resName = NULL;

    if ( sv->OpenedStream == 1 )
    {
        resName = sv->filename;

        if ( !sv->filename )
            return 0;
    }
    else
    {
        mfile = sv->file;
        if ( !mfile )
            return 0;
    }
    if ( WriteToFile(_pData, resName, mfile) )
        return sv->OpenedStream;
    return 0;
}

void NC_STACK_bmpanim::SetTime(int32_t timeStamp, int32_t frameTime)
{
    if ( frameTime == -1 )
    {
        _curFrame += _frmAdd;
        
        if (_curFrame >= (int32_t)_pData->FrameData.size())
        {
            if ( _animType )
            {
                _curFrame = _pData->FrameData.size() - 1;
                _frmAdd = -1;
            }
            else
            {
                _curFrame = 0;
            }
        }
        else if (_curFrame < 0)
        {
            _curFrame = 0;
            _frmAdd = 1;
        }
    }
    else if ( timeStamp != _timeStamp )
    {
        _timeStamp = timeStamp;
        _leftTime += frameTime;

        while ( true )
        {
            BAnmFrameCache &frm = _pData->FrameData[_curFrame];
            if (_leftTime < frm.FrameTime)
                break;
            
            _leftTime -= frm.FrameTime;

            _curFrame += _frmAdd;

            if (_curFrame >= (int32_t)_pData->FrameData.size())
            {
                if ( _animType )
                {
                    _curFrame = _pData->FrameData.size() - 1;
                    _frmAdd = -1;
                }
                else
                {
                    _curFrame = 0;
                }
            }
            else if (_curFrame < 0)
            {
                _curFrame = 0;
                _frmAdd = 1;
            }
        }
    }
}



void NC_STACK_bmpanim::setBANM_animType(int newType)
{
    _animType = newType;
}

ResBitmap * NC_STACK_bmpanim::GetBitmap()
{
    return _pData->FrameData[_curFrame].pBitmap->GetBitmap();
}

tUtV * NC_STACK_bmpanim::GetOutline()
{
    return _pData->FrameData[_curFrame].pTexCoords->data();
}

int NC_STACK_bmpanim::getBMD_width()
{
    return 0;
}

int NC_STACK_bmpanim::getBMD_height()
{
    return 0;
}


int NC_STACK_bmpanim::getBANM_framecnt()
{
    return _pData->FrameData.size();
}

int NC_STACK_bmpanim::getBANM_animtype()
{
    return _animType;
}

void NC_STACK_bmpanim::PrepareTexture( bool force )
{
    if (!_pData)
        return;
    
    for(NC_STACK_bitmap *bitm : _pData->Bitmaps)
        bitm->PrepareTexture(force);
}
