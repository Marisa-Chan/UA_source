#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "ilbm.h"
#include "utils.h"


const NewClassDescr NC_STACK_ilbm::description("ilbm.class", &newinstance);

NC_STACK_ilbm *NC_STACK_ilbm::CInit(stack_vals *stak)
{
    NC_STACK_ilbm *tmp = new NC_STACK_ilbm();
    if (!tmp)
        return NULL;

    if (!tmp->func0(stak))
    {
        delete tmp;
        return NULL;
    }

    return tmp;
}

size_t NC_STACK_ilbm::func0(stack_vals *stak)
{
    if ( !NC_STACK_bitmap::func0(stak) )
        return 0;

    __NC_STACK_ilbm *ilbm = &stack__ilbm;

    if ( find_id_in_stack_def_val(ILBM_ATT_FMT, 0, stak) )
        ilbm->flags |= 1;

    return 1;
}

size_t NC_STACK_ilbm::func2(stack_vals *stak)
{
    stack_vals *v6 = find_id_in_stack2(ILBM_ATT_FMT, stak);

    if ( v6 )
        setILBM_saveFmt( v6->value.i_data );

    return NC_STACK_bitmap::func2(stak);
}

size_t NC_STACK_ilbm::func3(stack_vals *stak)
{
    int *val = (int *)find_id_in_stack_def_val(ILBM_ATT_FMT, 0, stak);

    if ( val )
        *val = getILBM_saveFmt();

    return NC_STACK_bitmap::func3(stak);
}

size_t NC_STACK_ilbm::ilbm_func5__sub0(NC_STACK_ilbm *obj, IFFile **pmfile)
{
    IFFile *mfile = *pmfile;

    int has_nam2 = 0;
    int has_opl = 0;

    char name[256];
    pixel_2d opls[64];

    while ( 1 )
    {
        int v6 = mfile->parse();

        if ( v6 == -2 )
            break;

        if ( v6 )
            return 0;

        IFFile::Context *iff_chunk = mfile->getCurrentChunk();

        if ( iff_chunk->TAG == TAG_NAM2 )
        {
            mfile->read(name, 256);
            mfile->parse();

            has_nam2 = 1;
        }
        else if ( iff_chunk->TAG == TAG_OTL2 )
        {
            uint8_t dst[128];

            mfile->read(&dst, 128);

            int opl_count = iff_chunk->TAG_SIZE / 2;

            for (int i = 0; i < opl_count; i++)
            {
                memset(&opls[i], 0, sizeof(pixel_2d));

                opls[i].x = dst[0 + 2 * i];
                opls[i].y = dst[1 + 2 * i];
            }

            memset(&opls[opl_count], 0, sizeof(pixel_2d));
            opls[opl_count].flags = 0xFFFF;

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
        stack_vals stk[6];

        stk[0].set(RSRC_ATT_NAME, name);
        stk[1].set(RSRC_ATT_TRYSHARED, 1);
        stk[2].set(BMD_ATT_TEXTURE, 1);
        stk[3].end();

        if ( has_opl )
        {
            stk[3].set(BMD_ATT_OUTLINE, &opls);
            stk[4].end();
        }

        return NC_STACK_bitmap::func0(stk);
    }
    return 0;
}

size_t NC_STACK_ilbm::func5(IFFile **file)
{
    uint32_t TAG = (*file)->getCurrentChunk()->TAG_EXTENSION;

    if ( TAG == TAG_CIBO )
        return ilbm_func5__sub0(this, file);

    return 0;
}

size_t NC_STACK_ilbm::func6(IFFile **pmfile)
{
    IFFile *mfile = *pmfile;

    const char *name = getRsrc_name();

    bitmap_arg130 bitmap_info;
    bitmap_info.time_stmp = 1;
    bitmap_info.frame_time = 1;

    bitmap_func130(&bitmap_info);

    tUtV *opl2 = bitmap_info.outline;

    if ( mfile->pushChunk(TAG_CIBO, TAG_FORM, -1) )
    {
        return 0;
    }
    else
    {

        mfile->pushChunk(0, TAG_NAM2, -1);
        mfile->write(name, strlen(name) + 1);
        mfile->popChunk();

        if ( opl2 )
        {
            uint8_t buf[128];
            int opl_count = 0;

            tUtV *tmp = opl2;

            while(tmp->tu >= 0.0)
            {
                buf[0 + opl_count * 2] = tmp->tu * 256.0;
                buf[1 + opl_count * 2] = tmp->tv * 256.0;
                tmp++;
                opl_count++;
            }

            mfile->pushChunk(0, TAG_OTL2, opl_count * 2);
            mfile->write(buf, opl_count * 2);
            mfile->popChunk();
        }
        return mfile->popChunk() == IFFile::IFF_ERR_OK;
    }
    return 0;
}

void ILBM_BODY_READ__sub0(BMHD_type *bmhd, uint8_t *ilbm_data, void *_img_buffer)
{
    uint8_t masks[8] = {1, 2, 4, 8, 16, 32, 64, 128};

    uint8_t *img_buffer = (uint8_t *)_img_buffer;
    uint8_t udp[1024];

    int pln_w = (bmhd->width + 7) / 8;
    pln_w += (pln_w & 1);

    for (int y = 0; y < bmhd->height; y++)
    {
        if ( bmhd->compression )
        {
            for (int plan = 0; plan < bmhd->nPlanes; plan++)
            {
                int planOff = plan << 7;

                int x = 0;

                while (x < pln_w)
                {
                    int n = *(signed char *)ilbm_data;
                    ilbm_data++;

                    if (n == -128)
                    {
                        ;
                    }
                    else if ( n < 0)
                    {
                        n = -n + 1;
                        int dat = *ilbm_data;
                        ilbm_data++;
                        for (int i = 0; i < n; i++)
                        {
                            udp[planOff + x] = dat;
                            x++;
                        }
                    }
                    else if (n >= 0)
                    {
                        int cnt = n + 1;
                        memcpy(udp + planOff + x, ilbm_data, cnt);
                        ilbm_data += cnt;
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
                memcpy(udp + planOff, ilbm_data, pln_w);
                ilbm_data += pln_w;
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
}

int ILBM_BODY_READ(IFFile *mfile, BMHD_type *bmhd, bitmap_intern *bitm)
{
    if ( bitm->buffer )
    {
        IFFile::Context *chunk = mfile->getCurrentChunk();

        uint8_t *buffer = (uint8_t *)AllocVec(chunk->TAG_SIZE, 1);

        if ( buffer )
        {
            mfile->read(buffer, chunk->TAG_SIZE);

            ILBM_BODY_READ__sub0(bmhd, buffer, bitm->buffer);

            nc_FreeMem(buffer);

            return 1;
        }
    }
    return 0;
}

rsrc * NC_STACK_ilbm::READ_ILBM(stack_vals *stak, IFFile *mfil, int val5)
{
    int ILBM__OR__VBMP;

    BMHD_type bmhd;
    VBMP_type vbmp;
    stack_vals stk[3];
    rsrc *res = NULL;

    bitmap_intern *bitm = NULL;

    if ( mfil->parse() )
    {
        ypa_log_out("ilbm.class: Not an IFF file!\n");
        return NULL;
    }

    IFFile::Context *chunk = mfil->getCurrentChunk();
    if ( chunk->TAG != TAG_FORM )
    {
        ypa_log_out("ilbm.class: Not an IFF FORM chunk!\n");
        return NULL;
    }

    if ( chunk->TAG_EXTENSION == TAG_ILBM )
        ILBM__OR__VBMP = 1;
    else if ( chunk->TAG_EXTENSION == TAG_VBMP )
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

        uint32_t tag = mfil->getCurrentChunk()->TAG;

        if ( tag == TAG_BMHD )
        {
            mfil->readU16B(bmhd.width);
            mfil->readU16B(bmhd.height);
            mfil->readU16B(bmhd.x);
            mfil->readU16B(bmhd.y);
            mfil->readS8(bmhd.nPlanes);
            mfil->readS8(bmhd.masking);
            mfil->readS8(bmhd.compression);
            mfil->readS8(bmhd.flags);
            mfil->readU16B(bmhd.transparentColor);
            mfil->readS8(bmhd.xAspect);
            mfil->readS8(bmhd.yAspect);
            mfil->readU16B(bmhd.pageWidth);
            mfil->readU16B(bmhd.pageHeight);

            stk[0].set(BMD_ATT_WIDTH, bmhd.width);
            stk[1].set(BMD_ATT_HEIGHT, bmhd.height);
            stk[2].nextStack(stak);

            res = NC_STACK_bitmap::rsrc_func64(stk); // bitmap_func64
            if ( res )
            {
                bitm = (bitmap_intern *)res->data;
                if ( !bitm )
                {
                    rsrc_func65(res);
                    return NULL;
                }
            }
            mfil->parse();
        }
        else if ( tag == TAG_HEAD )
        {
            mfil->readU16B(vbmp.width);
            mfil->readU16B(vbmp.height);
            mfil->readU16B(vbmp.flags);

            stk[0].set(BMD_ATT_WIDTH, vbmp.width);
            stk[1].set(BMD_ATT_HEIGHT, vbmp.height);
            stk[2].nextStack(stak);

            res = NC_STACK_bitmap::rsrc_func64(stk); // bitmap_func64
            // creation of bitmap internal structure and allocation buffer, creation of surface, texture and palette
            if ( res )
            {
                bitm = (bitmap_intern *)res->data;
                if ( !bitm )
                {
                    rsrc_func65(res);
                    return NULL;
                }
            }
            mfil->parse();
        }
        else if ( tag == TAG_CMAP )
        {
            if ( bitm )
            {
                if ( !bitm->pallete )
                {
                    bitm->pallete = new UA_PALETTE;
                    bitm->flags |= BITMAP_FLAG_HAS_PALETTE;
                }

                if ( bitm->pallete )
                {
                    for (int i = 0; i < 256; i++)
                    {
                        uint8_t r,g,b;
                        mfil->readU8(r);
                        mfil->readU8(g);
                        mfil->readU8(b);
                        bitm->pallete->pal_entries[i].r = r;
                        bitm->pallete->pal_entries[i].g = g;
                        bitm->pallete->pal_entries[i].b = b;
                        bitm->pallete->pal_entries[i].a = 255;
                    }
                }
            }
            mfil->parse();
        }
        else if ( tag == TAG_BODY )
        {
            int success = 0;

            if ( bitm )
            {
                NC_STACK_display *w3d = engines.display___win3d;
                int locked = 1;

                if ( w3d && bitm->flags & BITMAP_FLAG_TEXTURE )
                {
                    if ( ! w3d->LockTexture(bitm) )
                        locked = 0;
                }

                if ( locked )
                {
                    if ( ILBM__OR__VBMP )
                    {
                        success = ILBM_BODY_READ(mfil, &bmhd, bitm);
                    }
                    else if ( bitm->buffer )              // VBMP READ
                    {
                        mfil->read(bitm->buffer, bitm->width * bitm->height);
                        success = 1;
                    }
                    else
                    {
                        success = 0;
                    }

                    if ( w3d && bitm->flags & BITMAP_FLAG_TEXTURE )
                    {
                        w3d->UnlockTexture(bitm); // win3d_func270
                        if ( val5 )
                            bitm->flags |= BITMAP_FLAG_TRANSP;

                        w3d->TextureApplyPalette(bitm); // win3d_func267
                    }
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
    return res;
}


// Create ilbm resource node and fill rsrc field data
rsrc * NC_STACK_ilbm::rsrc_func64(stack_vals *stak)
{
    const char *resName = (const char *)find_id_in_stack_def_val(RSRC_ATT_NAME, 0, stak);
    const char *reassignName = NULL;

    if ( !resName )
        return NULL;

    stack_vals stk[2];

    stk[0].nextStack(stak);

    if ( can_destblend )
    {
        if ( !strcasecmp(resName, "fx1.ilbm") )
            reassignName = "hi/alpha/fx1.ilbm";
        else if ( !strcasecmp(resName, "fx2.ilbm") )
            reassignName = "hi/alpha/fx2.ilbm";
        else if ( !strcasecmp(resName, "fx3.ilbm") )
            reassignName = "hi/alpha/fx3.ilbm";
    }
    else if ( can_stippling )
    {
        if ( !strcasecmp(resName, "fx1.ilbm") )
            reassignName = "hi/beta/fx1.ilbm";
        else if ( !strcasecmp(resName, "fx2.ilbm") )
            reassignName = "hi/beta/fx2.ilbm";
        else if ( !strcasecmp(resName, "fx3.ilbm") )
            reassignName = "hi/beta/fx3.ilbm";
    }
    else if ( can_srcblend )
    {
        if ( !strcasecmp(resName, "fx1.ilbm") )
            reassignName = "hi/gamma/fx1.ilbm";
        else if ( !strcasecmp(resName, "fx2.ilbm") )
            reassignName = "hi/gamma/fx2.ilbm";
        else if ( !strcasecmp(resName, "fx3.ilbm") )
            reassignName = "hi/gamma/fx3.ilbm";
    }

    IFFile *mfile = (IFFile *)find_id_in_stack_def_val(RSRC_ATT_PIFFFILE, 0, stak);

    int selfOpened = 0;

    if ( reassignName )
    {
        if ( mfile )
        {

            stk[0].set(BMD_ATT_TEXTURE, 1);
            stk[1].nextStack(stak);

            mfile->parse();
            mfile->skipChunk();
        }

        mfile = IFFile::openIFFile(reassignName, 0);

        if ( !mfile )
            return NULL;

        selfOpened = 1;
    }
    else
    {
        if ( mfile )
        {
            stk[0].set(BMD_ATT_TEXTURE, 1);
            stk[1].nextStack(stak);
        }
        else
        {
            mfile = IFFile::openIFFile(resName, 0);
            if ( !mfile )
                return NULL;

            selfOpened = 1;
        }
    }

    rsrc *res = READ_ILBM(stk, mfile, reassignName != 0);

    if ( selfOpened )
        delete mfile;

    return res;
}

void ILBM__WRITE_TO_FILE_BMHD(IFFile *mfile, bitmap_intern *bitm)
{
    mfile->pushChunk(0, TAG_BMHD, -1); //20 bytes

    mfile->writeU16B(bitm->width);
    mfile->writeU16B(bitm->height);
    mfile->writeU16B(0); // x
    mfile->writeU16B(0); // y
    mfile->writeS8(8); // nPlanes
    mfile->writeS8(0); // masking
    mfile->writeS8(0); // compression
    mfile->writeS8(0x80); //flags
    mfile->writeU16B(0); // transparentColor
    mfile->writeS8(22); // xAspect
    mfile->writeS8(22); // yAspect
    mfile->writeU16B(bitm->width); // pageWidth
    mfile->writeU16B(bitm->height); // pageHeight

    mfile->popChunk();
}

int ILBM__WRITE_TO_FILE_BODY(IFFile *mfile, bitmap_intern *bitm)
{
    int planeSz = 2 * ((bitm->width + 15) / 16);
    uint8_t *buf = (uint8_t *)AllocVec(planeSz, 1);

    if (!buf)
        return 0;

    mfile->pushChunk(0, TAG_BODY, 8 * bitm->height * planeSz);

    uint8_t *bfline = (uint8_t *)bitm->buffer;

    for (int i = bitm->height; i > 0; i-- )
    {
        for (int plane = 0; plane < 8; plane++)
        {
            memset((void *)buf, 0, planeSz);

            for (int x = 0; x < bitm->width; x++)
            {
                if ( (1 << plane) & bfline[x] )
                    buf[ x / 8 ] |= ( 1 << ( (x & 7) ^ 7) );
            }

            mfile->write((const void *)buf, planeSz);
        }

        bfline += bitm->width;
    }

    mfile->popChunk();
    nc_FreeMem(buf);
    return 1;
}

int ILBM__WRITE_TO_FILE(IFFile *mfile, bitmap_intern *bitm)
{
    if ( mfile->pushChunk(TAG_ILBM, TAG_FORM, -1) )
        return 0;


    ILBM__WRITE_TO_FILE_BMHD(mfile, bitm);

    if ( bitm->pallete )
    {
        mfile->pushChunk(0, TAG_CMAP, 256 * 3);
        mfile->write(bitm->pallete, 256 * 3);
        mfile->popChunk();
    }

    if ( !ILBM__WRITE_TO_FILE_BODY(mfile, bitm) )
        return 0;

    return mfile->popChunk() == IFFile::IFF_ERR_OK;
}

int VBMP__WRITE_TO_FILE(IFFile *mfile, bitmap_intern *bitm)
{
    int pixelCount = bitm->height * bitm->width;
    if ( mfile->pushChunk(TAG_VBMP, TAG_FORM, -1) )
        return 0;

    mfile->pushChunk(0, TAG_HEAD, -1); // 6 bytes

    mfile->writeU16B(bitm->width);
    mfile->writeU16B(bitm->height);
    mfile->writeU16B(0); // flags

    mfile->popChunk();

    if ( bitm->pallete )
    {
        mfile->pushChunk(0, TAG_CMAP, 256 * 3);
        mfile->write(bitm->pallete, 256 * 3);
        mfile->popChunk();
    }

    mfile->pushChunk(0, TAG_BODY, pixelCount);
    mfile->write(bitm->buffer, pixelCount);
    mfile->popChunk();

    return mfile->popChunk() == IFFile::IFF_ERR_OK;
}

size_t NC_STACK_ilbm::rsrc_func66(rsrc_func66_arg *arg)
{
    __NC_STACK_ilbm *ilbm = &stack__ilbm;

    IFFile *mfile;

    if ( arg->OpenedStream == 1 )
    {
        if ( !arg->filename )
            return 0;

        mfile = IFFile::openIFFile(arg->filename, true);
    }
    else
        mfile = arg->file;

    if ( !mfile )
        return 0;

    bitmap_arg130 v6;
    v6.time_stmp = 1;
    v6.frame_time = 1;

    bitmap_func130(&v6);

    if ( !v6.pbitm || !v6.pbitm->buffer )
        return 0;

    int res;

    if ( ilbm->flags & 1 )
        res = ILBM__WRITE_TO_FILE(mfile, v6.pbitm);
    else
        res = VBMP__WRITE_TO_FILE(mfile, v6.pbitm);

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
        stack__ilbm.flags |= 1;
    else
        stack__ilbm.flags &= ~1;
}

int NC_STACK_ilbm::getILBM_saveFmt()
{
    return (stack__ilbm.flags & 1) != 0;
}

size_t NC_STACK_ilbm::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (stack_vals *)data );
    case 2:
        return func2( (stack_vals *)data );
    case 3:
        return func3( (stack_vals *)data );
    case 5:
        return (size_t)func5( (IFFile **)data );
    case 6:
        return (size_t)func6( (IFFile **)data );
    case 64:
        return (size_t)rsrc_func64( (stack_vals *)data );
    case 66:
        return (size_t)rsrc_func66( (rsrc_func66_arg *)data );
    default:
        break;
    }
    return NC_STACK_bitmap::compatcall(method_id, data);
}
