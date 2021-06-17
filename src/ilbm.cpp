#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "ilbm.h"
#include "utils.h"


const Nucleus::ClassDescr NC_STACK_ilbm::description("ilbm.class", &newinstance);

size_t NC_STACK_ilbm::Init(IDVList &stak)
{
    if ( !NC_STACK_bitmap::Init(stak) )
        return 0;

    if ( stak.Get<bool>(ILBM_ATT_FMT, false) )
        _saveAsIlbm = true;

    return 1;
}


size_t NC_STACK_ilbm::ilbm_func5__sub0(NC_STACK_ilbm *obj, IFFile **pmfile)
{
    IFFile *mfile = *pmfile;

    int has_nam2 = 0;
    int has_opl = 0;

    char name[256];
    std::vector<tUtV> opls;

    while ( 1 )
    {
        int v6 = mfile->parse();

        if ( v6 == -2 )
            break;

        if ( v6 )
            return 0;

        const IFFile::Context &iff_chunk = mfile->GetCurrentChunk();

        if ( iff_chunk.Is(TAG_NAM2) )
        {
            mfile->read(name, 256);
            mfile->parse();

            has_nam2 = 1;
        }
        else if ( iff_chunk.Is(TAG_OTL2) )
        {
            uint8_t dst[128];

            mfile->read(&dst, 128);

            int opl_count = iff_chunk.TAG_SIZE / 2;
            opls.resize(opl_count);

            for (int i = 0; i < opl_count; ++i)
            {
                opls[i].tu = float(dst[0 + 2 * i]) / 256.0;
                opls[i].tv = float(dst[1 + 2 * i]) / 256.0;
            }

            mfile->parse();

            has_opl = 1;
        }
        else
        {
            mfile->skipChunk();
        }
    }

    if ( has_nam2 )
    {
        IDVList stk {
            {RSRC_ATT_NAME, std::string(name)},
            {RSRC_ATT_TRYSHARED, (int32_t)1},
            {BMD_ATT_CONVCOLOR, (int32_t)1}};

        if ( has_opl )
            stk.Add(BMD_ATT_OUTLINE, &opls);

        return NC_STACK_bitmap::Init(stk);
    }
    return 0;
}

size_t NC_STACK_ilbm::LoadingFromIFF(IFFile **file)
{
    if ( (*file)->GetCurrentChunk().TAG_EXTENSION == TAG_CIBO )
        return ilbm_func5__sub0(this, file);

    return 0;
}

size_t NC_STACK_ilbm::SavingIntoIFF(IFFile **pmfile)
{
    IFFile *mfile = *pmfile;

    std::string name = getRsrc_name();

    SetTime(1, 1);
    const std::vector<tUtV> &opl2 = GetOutline();

    if ( mfile->pushChunk(TAG_CIBO, TAG_FORM, -1) )
    {
        return 0;
    }
    else
    {

        mfile->pushChunk(0, TAG_NAM2, -1);
        mfile->write(name.c_str(), name.length() + 1);
        mfile->popChunk();

        if ( !opl2.empty() )
        {
            mfile->pushChunk(0, TAG_OTL2, opl2.size() * 2);
            
            for(tUtV uv : opl2)
            {
                mfile->writeU8(uv.tu * 256.0);
                mfile->writeU8(uv.tv * 256.0);
            }

            mfile->popChunk();
        }
        return mfile->popChunk() == IFFile::IFF_ERR_OK;
    }
    return 0;
}

void ILBM_BODY_READ__sub0(BMHD_type *bmhd, const std::vector<int8_t> &ilbm_data, ResBitmap *img)
{
    uint8_t masks[8] = {1, 2, 4, 8, 16, 32, 64, 128};

    SDL_LockSurface(img->swTex);
    uint8_t udp[1024];

    int pln_w = (bmhd->width + 7) / 8;
    pln_w += (pln_w & 1);
    
    int inByte = 0;

    for (int y = 0; y < bmhd->height; y++)
    {
        uint8_t *img_buffer = (uint8_t *)img->swTex->pixels + y * img->swTex->pitch;
        if ( bmhd->compression )
        {
            for (int plan = 0; plan < bmhd->nPlanes; plan++)
            {
                int planOff = plan << 7;

                int x = 0;

                while (x < pln_w)
                {
                    int n = ilbm_data[inByte];
                    inByte++;

                    if (n == -128)
                    {
                        ;
                    }
                    else if ( n < 0)
                    {
                        n = -n + 1;
                        int dat = ilbm_data[inByte];
                        inByte++;
                        for (int i = 0; i < n; i++)
                        {
                            udp[planOff + x] = dat;
                            x++;
                        }
                    }
                    else if (n >= 0)
                    {
                        int cnt = n + 1;
                        memcpy(udp + planOff + x, ilbm_data.data() + inByte, cnt);
                        inByte += cnt;
                        x += cnt;
                    }
                }
            }
        }
        else
        {
            for (int plan = 0; plan < bmhd->nPlanes; plan++)
            {
                int planOff = plan << 7;
                memcpy(udp + planOff, ilbm_data.data() + inByte, pln_w);
                inByte += pln_w;
            }
        }
        for (int x = 0; x < bmhd->width; x++)
        {
            int v16 = x / 8;

            uint8_t bt = 0;

            for (int plan = 0; plan < bmhd->nPlanes; plan++)
            {
                int planOff = plan << 7;

                if ( masks[7 - (x & 7)] & udp[planOff + v16] )
                    bt |= masks[plan];
            }
            *img_buffer = bt;
            img_buffer++;
        }
    }
    SDL_UnlockSurface(img->swTex);
}

int ILBM_BODY_READ(IFFile *mfile, BMHD_type *bmhd, ResBitmap *bitm)
{
    if ( !bitm->swTex )
        return false;

    const IFFile::Context &chunk = mfile->GetCurrentChunk();

    std::vector<int8_t> buffer;
    buffer.resize(chunk.TAG_SIZE);

    mfile->read(buffer.data(), chunk.TAG_SIZE);

    ILBM_BODY_READ__sub0(bmhd, buffer, bitm);

    return true;
}

rsrc * NC_STACK_ilbm::READ_ILBM(IDVList &stak, IFFile *mfil, int transp)
{
    int ILBM__OR__VBMP;

    BMHD_type bmhd;
    VBMP_type vbmp;
    
    rsrc *res = NC_STACK_rsrc::rsrc_func64(stak);
    int convertColor = stak.Get<int32_t>(BMD_ATT_CONVCOLOR, 0);
    int alphaPalette = stak.Get<int32_t>(ATT_ALPHAPALETTE, 1);
    
    if (!res)
        return NULL;

    ResBitmap *bitm = NULL;

    if ( mfil->parse() )
    {
        ypa_log_out("ilbm.class: Not an IFF file!\n");
        return NULL;
    }

    const IFFile::Context &formChunk = mfil->GetCurrentChunk();
    if ( !formChunk.Is(TAG_FORM) )
    {
        ypa_log_out("ilbm.class: Not an IFF FORM chunk!\n");
        return NULL;
    }

    if ( formChunk.TAG_EXTENSION == TAG_ILBM )
        ILBM__OR__VBMP = 1;
    else if ( formChunk.TAG_EXTENSION == TAG_VBMP )
        ILBM__OR__VBMP = 0;
    else
    {
        ypa_log_out("ilbm.class: Not an ILBM or VBMP file!\n");
        return NULL;
    }

    while ( true )
    {
        int v10 = mfil->parse();
        if ( v10 == IFFile::IFF_ERR_EOC )
            break;
        if ( v10 )
        {
            if ( res )
                rsrc_func65(res);

            return NULL;
        }

        const IFFile::Context &chunk = mfil->GetCurrentChunk();

        if ( chunk.Is(TAG_BMHD) )
        {
            bmhd.width = mfil->readU16B();
            bmhd.height = mfil->readU16B();
            bmhd.x = mfil->readU16B();
            bmhd.y = mfil->readU16B();
            bmhd.nPlanes = mfil->readS8();
            bmhd.masking = mfil->readS8();
            bmhd.compression = mfil->readS8();
            bmhd.flags = mfil->readS8();
            bmhd.transparentColor = mfil->readU16B();
            bmhd.xAspect = mfil->readS8();
            bmhd.yAspect = mfil->readS8();
            bmhd.pageWidth = mfil->readU16B();
            bmhd.pageHeight = mfil->readU16B();

            bitm = new ResBitmap;
            bitm->width = bmhd.width;
            bitm->height = bmhd.height;
            
            res->data = bitm;
            
            mfil->parse();
        }
        else if ( chunk.Is(TAG_HEAD) )
        {
            vbmp.width = mfil->readU16B();
            vbmp.height = mfil->readU16B();
            vbmp.flags = mfil->readU16B();
            
            bitm = new ResBitmap;
            bitm->width = vbmp.width;
            bitm->height = vbmp.height;
            
            res->data = bitm;

            mfil->parse();
        }
        else if ( chunk.Is(TAG_CMAP) )
        {
            if ( bitm )
            {
                if ( !bitm->palette )
                    bitm->palette = new UA_PALETTE;
                
                if ( bitm->palette )
                {
                    for (int i = 0; i < 256; i++)
                    {
                        bitm->palette->at(i).r = mfil->readU8();
                        bitm->palette->at(i).g = mfil->readU8();
                        bitm->palette->at(i).b = mfil->readU8();
                        bitm->palette->at(i).a = 255;
                    }
                }
            }
            mfil->parse();
        }
        else if ( chunk.Is(TAG_BODY) )
        {
            bool success = false;

            if ( bitm )
            {
                bitm->swTex = SDL_CreateRGBSurface(0, bitm->width, bitm->height, 8, 0, 0, 0, 0);
                
                if ( ILBM__OR__VBMP )
                {
                    success = ILBM_BODY_READ(mfil, &bmhd, bitm);
                }
                else if ( bitm->swTex )              // VBMP READ
                {
                    SDL_LockSurface(bitm->swTex);
                    for(int y = 0; y < bitm->height; y++)
                        mfil->read((uint8_t *)bitm->swTex->pixels + y * bitm->swTex->pitch, bitm->width);                   
                    SDL_UnlockSurface(bitm->swTex);
                    success = true;
                }
                else
                {
                    success = false;
                }
            }
            if ( !success )
            {
                rsrc_func65(res);
                return NULL;
            }
            mfil->parse();
        }
        else
        {
            mfil->skipChunk();
        }
    }
    
    if (bitm && bitm->swTex)
    {
        if (bitm->swTex->format->palette)
        {
            UA_PALETTE *pal = bitm->palette;
            if (!pal)
                pal = GFX::Engine.GetPalette();
            
            //if (alphaPalette && convertColor)
            if (alphaPalette)
            {
                UA_PALETTE tmp;
                GFX::Engine.ConvAlphaPalette(&tmp, *pal, transp);
                SDL_SetPaletteColors(bitm->swTex->format->palette, tmp.data(), 0, 256);
            }
            else
                SDL_SetPaletteColors(bitm->swTex->format->palette, pal->data(), 0, 256);
        }

        if ( convertColor )
        {
            SDL_Surface *screenFmt = GFX::Engine.ConvertToScreenFormat(bitm->swTex);
            if (screenFmt)
            {
                SDL_FreeSurface(bitm->swTex);
                bitm->swTex = screenFmt;

                if (bitm->palette)
                {
                    delete bitm->palette;
                    bitm->palette = NULL;
                }
            }
        }
    }
    return res;
}


// Create ilbm resource node and fill rsrc field data
rsrc * NC_STACK_ilbm::rsrc_func64(IDVList &stak)
{
    const std::string resName = stak.Get<std::string>(RSRC_ATT_NAME, "");
    const char *reassignName = NULL;

    if ( resName.empty() )
        return NULL;

    if ( GFX::Engine.can_destblend )
    {
        if ( !StriCmp(resName, "fx1.ilbm") )
            reassignName = "hi/alpha/fx1.ilbm";
        else if ( !StriCmp(resName, "fx2.ilbm") )
            reassignName = "hi/alpha/fx2.ilbm";
        else if ( !StriCmp(resName, "fx3.ilbm") )
            reassignName = "hi/alpha/fx3.ilbm";
    }
    else if ( GFX::Engine.can_stippling )
    {
        if ( !StriCmp(resName, "fx1.ilbm") )
            reassignName = "hi/beta/fx1.ilbm";
        else if ( !StriCmp(resName, "fx2.ilbm") )
            reassignName = "hi/beta/fx2.ilbm";
        else if ( !StriCmp(resName, "fx3.ilbm") )
            reassignName = "hi/beta/fx3.ilbm";
    }
    else if ( GFX::Engine.can_srcblend )
    {
        if ( !StriCmp(resName, "fx1.ilbm") )
            reassignName = "hi/gamma/fx1.ilbm";
        else if ( !StriCmp(resName, "fx2.ilbm") )
            reassignName = "hi/gamma/fx2.ilbm";
        else if ( !StriCmp(resName, "fx3.ilbm") )
            reassignName = "hi/gamma/fx3.ilbm";
    }

    IFFile *mfile = stak.Get<IFFile *>(RSRC_ATT_PIFFFILE, NULL);

    int selfOpened = 0;

    if ( reassignName )
    {
        if ( mfile )
        {
            stak.Add(BMD_ATT_CONVCOLOR, (int32_t)1);

            mfile->parse();
            mfile->skipChunk();
        }

        mfile = IFFile::RsrcOpenIFFile(reassignName, "rb");

        if ( !mfile )
            return NULL;

        selfOpened = 1;
    }
    else
    {
        if ( mfile )
        {
            stak.Add(BMD_ATT_CONVCOLOR, (int32_t)1);
        }
        else
        {
            mfile = IFFile::RsrcOpenIFFile(resName, "rb");
            if ( !mfile )
                return NULL;

            selfOpened = 1;
        }
    }

    rsrc *res = READ_ILBM(stak, mfile, reassignName != 0);

    if ( selfOpened )
        delete mfile;

    return res;
}

void ILBM__WRITE_TO_FILE_BMHD(IFFile *mfile, ResBitmap *bitm)
{
    mfile->pushChunk(0, TAG_BMHD, -1); //20 bytes

    mfile->writeU16B(bitm->width);
    mfile->writeU16B(bitm->height);
    mfile->writeU16B(0); // x
    mfile->writeU16B(0); // y
    mfile->writeU8(8); // nPlanes
    mfile->writeU8(0); // masking
    mfile->writeU8(0); // compression
    mfile->writeU8(0x80); //flags
    mfile->writeU16B(0); // transparentColor
    mfile->writeS8(22); // xAspect
    mfile->writeS8(22); // yAspect
    mfile->writeU16B(bitm->width); // pageWidth
    mfile->writeU16B(bitm->height); // pageHeight

    mfile->popChunk();
}

int ILBM__WRITE_TO_FILE_BODY(IFFile *mfile, ResBitmap *bitm)
{
    int planeSz = 2 * ((bitm->width + 15) / 16);
    std::vector<uint8_t> buf(planeSz);

    mfile->pushChunk(0, TAG_BODY, 8 * bitm->height * planeSz);

    SDL_LockSurface(bitm->swTex);
    const uint8_t *bfline = (uint8_t *)bitm->swTex->pixels;

    for (int i = bitm->height; i > 0; i-- )
    {
        for (int plane = 0; plane < 8; plane++)
        {
            std::fill(buf.begin(), buf.end(), 0);

            for (int x = 0; x < bitm->width; x++)
            {
                if ( (1 << plane) & bfline[x] )
                    buf[ x / 8 ] |= ( 1 << ( (x & 7) ^ 7) );
            }

            mfile->write((const void *)buf.data(), planeSz);
        }

        bfline += bitm->width;
    }
    SDL_UnlockSurface(bitm->swTex);

    mfile->popChunk();
    return 1;
}

int ILBM__WRITE_TO_FILE(IFFile *mfile, ResBitmap *bitm)
{
    if ( mfile->pushChunk(TAG_ILBM, TAG_FORM, -1) )
        return 0;


    ILBM__WRITE_TO_FILE_BMHD(mfile, bitm);

    if ( bitm->palette )
    {
        mfile->pushChunk(0, TAG_CMAP, 256 * 3);
        mfile->write(bitm->palette, 256 * 3);
        mfile->popChunk();
    }

    if ( !ILBM__WRITE_TO_FILE_BODY(mfile, bitm) )
        return 0;

    return mfile->popChunk() == IFFile::IFF_ERR_OK;
}

int VBMP__WRITE_TO_FILE(IFFile *mfile, ResBitmap *bitm)
{
    int pixelCount = bitm->height * bitm->width;
    if ( mfile->pushChunk(TAG_VBMP, TAG_FORM, -1) )
        return 0;

    mfile->pushChunk(0, TAG_HEAD, -1); // 6 bytes

    mfile->writeU16B(bitm->width);
    mfile->writeU16B(bitm->height);
    mfile->writeU16B(0); // flags

    mfile->popChunk();

    if ( bitm->palette )
    {
        mfile->pushChunk(0, TAG_CMAP, 256 * 3);
        mfile->write(bitm->palette, 256 * 3);
        mfile->popChunk();
    }

    mfile->pushChunk(0, TAG_BODY, pixelCount);
    
    SDL_LockSurface(bitm->swTex);
    mfile->write(bitm->swTex->pixels, pixelCount);
    SDL_UnlockSurface(bitm->swTex);
    
    mfile->popChunk();

    return mfile->popChunk() == IFFile::IFF_ERR_OK;
}

size_t NC_STACK_ilbm::rsrc_func66(rsrc_func66_arg *arg)
{
    IFFile *mfile;

    if ( arg->OpenedStream == 1 )
    {
        if ( !arg->filename )
            return 0;

        mfile = IFFile::RsrcOpenIFFile(arg->filename, "wb");
    }
    else
        mfile = arg->file;

    if ( !mfile )
        return 0;

    SetTime(1, 1);
    
    ResBitmap *bitm = GetBitmap();

    if ( !bitm || !bitm->swTex )
        return 0;

    int res;

    if ( _saveAsIlbm )
        res = ILBM__WRITE_TO_FILE(mfile, bitm);
    else
        res = VBMP__WRITE_TO_FILE(mfile, bitm);

    //// CHECK THIS---
    if ( res )
    {
        if ( arg->OpenedStream == 1 )
            delete mfile;
        res = arg->OpenedStream;
    }
    return res;
    //// CHECK THIS+++
}

void NC_STACK_ilbm::setILBM_saveFmt(int fmt)
{
    if ( fmt )
        _saveAsIlbm = true;
    else
        _saveAsIlbm = false;
}

int NC_STACK_ilbm::getILBM_saveFmt()
{
    return _saveAsIlbm;
}

