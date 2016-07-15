#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "ilbm.h"
#include "utils.h"


const NewClassDescr NC_STACK_ilbm::description("ilbm.class", &newinstance);

size_t NC_STACK_ilbm::func0(stack_vals *stak)
{
    if ( !NC_STACK_bitmap::func0(stak) )
        return 0;

    __NC_STACK_ilbm *ilbm = &this->stack__ilbm;

    if ( find_id_in_stack_def_val(ILBM_ATT_FMT, 0, stak) )
        ilbm->flags |= 1;

    return 1;
}

size_t NC_STACK_ilbm::func2(stack_vals *stak)
{
    stack_vals *v6 = find_id_in_stack2(ILBM_ATT_FMT, stak);

    if ( v6 )
        setILBM_saveFmt( v6->value );

    return NC_STACK_bitmap::func2(stak);
}

size_t NC_STACK_ilbm::func3(stack_vals *stak)
{
    int *val = (int *)find_id_in_stack_def_val(ILBM_ATT_FMT, 0, stak);

    if ( val )
        *val = getILBM_saveFmt();

    return NC_STACK_bitmap::func3(stak);
}

size_t NC_STACK_ilbm::ilbm_func5__sub0(NC_STACK_ilbm *obj, MFILE **pmfile)
{
    MFILE *mfile = *pmfile;

    int has_nam2 = 0;
    int has_opl = 0;

    char name[256];
    pixel_2d opls[64];

    while ( 1 )
    {
        int v6 = read_next_IFF(mfile, 2);

        if ( v6 == -2 )
            break;

        if ( v6 )
            return 0;

        MFILE_S1 *iff_chunk = GET_FORM_INFO_OR_NULL(mfile);

        if ( iff_chunk->TAG == TAG_NAM2 )
        {
            mfread(mfile, name, 256);
            read_next_IFF(mfile, 2);

            has_nam2 = 1;
        }
        else if ( iff_chunk->TAG == TAG_OTL2 )
        {
            BYTE dst[128];

            mfread(mfile, &dst, 128);

            int opl_count = iff_chunk->TAG_SIZE / 2;

            for (int i = 0; i < opl_count; i++)
            {
                memset(&opls[i], 0, sizeof(pixel_2d));

                opls[i].x = dst[2 * i];
                opls[i].y = dst[1 + 2 * i];
            }

            memset(&opls[opl_count], 0, sizeof(pixel_2d));
            opls[opl_count].flags = MINSHORT;

            read_next_IFF(mfile, 2);

            has_opl = 1;
        }
        else
        {
            read_default(mfile);
        }
    }

    if ( has_nam2 )
    {
        stack_vals stk[6];

        stk[0].id = RSRC_ATT_NAME;
        stk[0].value = (size_t)name;

        stk[1].id = RSRC_ATT_TRYSHARED;
        stk[1].value = 1;

        stk[2].id = BMD_ATT_TEXTURE;
        stk[2].value = 1;

        stk[3].id = 0;

        if ( has_opl )
        {
            stk[3].id = BMD_ATT_OUTLINE;
            stk[3].value = (size_t)&opls;

            stk[4].id = 0;
        }

        return NC_STACK_bitmap::func0(stk);
    }
    return 0;
}

size_t NC_STACK_ilbm::func5(MFILE **file)
{
    DWORD TAG = GET_FORM_INFO_OR_NULL(*file)->TAG_EXTENSION;

    if ( TAG == TAG_CIBO )
        return ilbm_func5__sub0(this, file);

    return 0;
}

size_t NC_STACK_ilbm::func6(MFILE **pmfile)
{
    MFILE *mfile = *pmfile;

    const char *name = getRsrc_name();

    bitmap_arg130 bitmap_info;
    bitmap_info.time_stmp = 1;
    bitmap_info.frame_time = 1;

    bitmap_func130(&bitmap_info);

    tUtV *opl2 = bitmap_info.outline;

    if ( sub_412FC0(mfile, TAG_CIBO, TAG_FORM, -1) )
    {
        return 0;
    }
    else
    {

        sub_412FC0(mfile, 0, TAG_NAM2, -1);
        sub_413564(mfile, strlen(name) + 1, name);
        sub_413290(mfile);

        if ( opl2 )
        {
            BYTE buf[128];
            int opl_count = 0;

            tUtV *tmp = opl2;

            while(tmp->tu >= 0.0)
            {
                buf[opl_count * 2] = tmp->tu * 256.0;
                buf[1 + opl_count * 2] = tmp->tv * 256.0;
                tmp++;
                opl_count++;
            }

            sub_412FC0(mfile, 0, TAG_OTL2, opl_count * 2);
            sub_413564(mfile, opl_count * 2, buf);
            sub_413290(mfile);
        }
        return sub_413290(mfile) == 0;
    }
    return 0;
}

void ILBM_BODY_READ__sub0(BMHD_type *bmhd, BYTE *ilbm_data, void *_img_buffer)
{
    BYTE masks[8] = {1, 2, 4, 8, 16, 32, 64, 128};

    BYTE *img_buffer = (BYTE *)_img_buffer;
    BYTE udp[1024];

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

            BYTE bt = 0;

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

int ILBM_BODY_READ(MFILE *mfile, BMHD_type *bmhd, bitmap_intern *bitm)
{
    if ( bitm->buffer )
    {
        MFILE_S1 *chunk = GET_FORM_INFO_OR_NULL(mfile);

        BYTE *buffer = (BYTE *)AllocVec(chunk->TAG_SIZE, 1);

        if ( buffer )
        {
            mfread(mfile, buffer, chunk->TAG_SIZE);

            ILBM_BODY_READ__sub0(bmhd, buffer, bitm->buffer);

            nc_FreeMem(buffer);

            return 1;
        }
    }
    return 0;
}

rsrc * NC_STACK_ilbm::READ_ILBM(stack_vals *stak, MFILE *mfil, int val5)
{
    int ILBM__OR__VBMP;

    BMHD_type bmhd;
    VBMP_type vbmp;
    stack_vals stk[3];
    rsrc *res = NULL;

    bitmap_intern *bitm = NULL;

    if ( read_next_IFF(mfil, 2) )
    {
        ypa_log_out("ilbm.class: Not an IFF file!\n");
        return NULL;
    }

    MFILE_S1 *chunk = GET_FORM_INFO_OR_NULL(mfil);
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
        int v10 = read_next_IFF(mfil, 2);
        if ( v10 == -2 )
            break;
        if ( v10 )
        {
            if ( res )
                rsrc_func65(&res);

            return NULL;
        }

        DWORD tag = GET_FORM_INFO_OR_NULL(mfil)->TAG;

        if ( tag == TAG_BMHD )
        {
            mfread(mfil, &bmhd, sizeof(BMHD_type));
            bmhd.width = SWAP16(bmhd.width);
            bmhd.height = SWAP16(bmhd.height);

            stk[0].id = BMD_ATT_WIDTH;
            stk[0].value = bmhd.width;
            stk[1].id = BMD_ATT_HEIGHT;
            stk[1].value = bmhd.height;
            stk[2].id = 2;
            stk[2].value = (size_t)stak;

            res = NC_STACK_bitmap::rsrc_func64(stk); // bitmap_func64
            if ( res )
            {
                bitm = (bitmap_intern *)res->data;
                if ( !bitm )
                {
                    rsrc_func65(&res);
                    return NULL;
                }
            }
            read_next_IFF(mfil, 2);
        }
        else if ( tag == TAG_HEAD )
        {
            mfread(mfil, &vbmp, sizeof(VBMP_type));
            vbmp.width = SWAP16(vbmp.width);
            vbmp.height = SWAP16(vbmp.height);

            stk[0].id = BMD_ATT_WIDTH;
            stk[0].value = vbmp.width;
            stk[1].id = BMD_ATT_HEIGHT;
            stk[1].value = vbmp.height;
            stk[2].id = 2;
            stk[2].value = (size_t)stak;

            res = NC_STACK_bitmap::rsrc_func64(stk); // bitmap_func64
            // creation of bitmap internal structure and allocation buffer, creation of surface, texture and palette
            if ( res )
            {
                bitm = (bitmap_intern *)res->data;
                if ( !bitm )
                {
                    rsrc_func65(&res);
                    return NULL;
                }
            }
            read_next_IFF(mfil, 2);
        }
        else if ( tag == TAG_CMAP )
        {
            if ( bitm )
            {
                if ( !bitm->pallete )
                    bitm->pallete = (UA_PALETTE *)AllocVec(sizeof(UA_PALETTE), 65537);

                if ( bitm->pallete )
                {
                    mfread(mfil, bitm->pallete, sizeof(UA_PALETTE));
                    bitm->flags |= BITMAP_FLAG_HAS_PALETTE;
                }
            }
            read_next_IFF(mfil, 2);
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
                    if ( ! w3d->display_func269(&bitm) )
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
                        mfread(mfil, bitm->buffer, bitm->width * bitm->height);
                        success = 1;
                    }
                    else
                    {
                        success = 0;
                    }

                    if ( w3d && bitm->flags & BITMAP_FLAG_TEXTURE )
                    {
                        w3d->display_func270(&bitm); // win3d_func270
                        if ( val5 )
                            bitm->flags |= BITMAP_FLAG_TRANSP;

                        w3d->display_func267(&bitm); // win3d_func267
                    }
                }
            }
            if ( !success )
            {
                rsrc_func65(&res);
                return NULL;
            }
            read_next_IFF(mfil, 2);
        }
        else
        {
            read_default(mfil);
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

    stk[0].id = 2;
    stk[0].value = (size_t)stak;

    if ( dword_514EFC )
    {
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
    }

    MFILE *mfile = (MFILE *)find_id_in_stack_def_val(RSRC_ATT_PIFFFILE, 0, stak);

    int selfOpened = 0;

    if ( reassignName )
    {
        if ( mfile )
        {

            stk[0].id = BMD_ATT_TEXTURE;
            stk[0].value = 1;
            stk[1].id = 2;
            stk[1].value = (size_t)stak;

            read_next_IFF(mfile, 2);
            read_default(mfile);
        }

        mfile = open_mfile(reassignName, 0);

        if ( !mfile )
            return NULL;

        selfOpened = 1;
    }
    else
    {
        if ( mfile )
        {
            stk[0].id = BMD_ATT_TEXTURE;
            stk[0].value = 1;
            stk[1].id = 2;
            stk[1].value = (size_t)stak;
        }
        else
        {
            mfile = open_mfile(resName, 0);
            if ( !mfile )
                return NULL;

            selfOpened = 1;
        }
    }

    rsrc *res = READ_ILBM(stk, mfile, reassignName != 0);

    if ( selfOpened )
        close_mfile(mfile);

    return res;
}

void ILBM__WRITE_TO_FILE_BMHD(MFILE *mfile, bitmap_intern *bitm)
{
    BMHD_type bmhd;

    bmhd.width = SWAP16(bitm->width);
    bmhd.height = SWAP16(bitm->height);
    bmhd.y = 0;
    bmhd.x = 0;
    bmhd.flags = 0x80;
    bmhd.transparentColor = 0;
    bmhd.nPlanes = 8;
    bmhd.masking = 0;
    bmhd.compression = 0;
    bmhd.pageWidth = SWAP16(bitm->width);
    bmhd.pageHeight = SWAP16(bitm->height);
    bmhd.xAspect = 22;
    bmhd.yAspect = 22;

    sub_412FC0(mfile, 0, TAG_BMHD, sizeof(BMHD_type));
    sub_413564(mfile, sizeof(BMHD_type), &bmhd);
    sub_413290(mfile);
}

int ILBM__WRITE_TO_FILE_BODY(MFILE *mfile, bitmap_intern *bitm)
{
    int planeSz = 2 * ((bitm->width + 15) / 16);
    BYTE *buf = (BYTE *)AllocVec(planeSz, 1);

    if (!buf)
        return 0;

    sub_412FC0(mfile, 0, TAG_BODY, 8 * bitm->height * planeSz);

    BYTE *bfline = (BYTE *)bitm->buffer;

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

            sub_413564(mfile, planeSz, (const void *)buf);
        }

        bfline += bitm->width;
    }

    sub_413290(mfile);
    nc_FreeMem(buf);
    return 1;
}

int ILBM__WRITE_TO_FILE(MFILE *mfile, bitmap_intern *bitm)
{
    if ( sub_412FC0(mfile, TAG_ILBM, TAG_FORM, -1) )
        return 0;


    ILBM__WRITE_TO_FILE_BMHD(mfile, bitm);

    if ( bitm->pallete )
    {
        sub_412FC0(mfile, 0, TAG_CMAP, 256 * 3);
        sub_413564(mfile, 256 * 3, bitm->pallete);
        sub_413290(mfile);
    }

    if ( !ILBM__WRITE_TO_FILE_BODY(mfile, bitm) )
        return 0;

    return sub_413290(mfile) == 0;
}

int VBMP__WRITE_TO_FILE(MFILE *mfile, bitmap_intern *bitm)
{
    int pixelCount = bitm->height * bitm->width;
    if ( sub_412FC0(mfile, TAG_VBMP, TAG_FORM, -1) )
        return 0;

    sub_412FC0(mfile, 0, TAG_HEAD, sizeof(VBMP_type));

    VBMP_type vbmp;
    vbmp.width = SWAP16(bitm->width);
    vbmp.height = SWAP16(bitm->height);
    vbmp.flags = 0;

    sub_413564(mfile, sizeof(VBMP_type), &vbmp);
    sub_413290(mfile);

    if ( bitm->pallete )
    {
        sub_412FC0(mfile, 0, TAG_CMAP, 256 * 3);
        sub_413564(mfile, 256 * 3, bitm->pallete);
        sub_413290(mfile);
    }

    sub_412FC0(mfile, 0, TAG_BODY, pixelCount);
    sub_413564(mfile, pixelCount, bitm->buffer);
    sub_413290(mfile);

    return sub_413290(mfile) == 0;
}

size_t NC_STACK_ilbm::rsrc_func66(rsrc_func66_arg *arg)
{
    __NC_STACK_ilbm *ilbm = &this->stack__ilbm;

    MFILE *mfile;

    if ( arg->OpenedStream == 1 )
    {
        if ( !arg->filename )
            return 0;

        mfile = open_mfile(arg->filename, 1);
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
            close_mfile(mfile);
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
        return (size_t)func5( (MFILE **)data );
    case 6:
        return (size_t)func6( (MFILE **)data );
    case 64:
        return (size_t)rsrc_func64( (stack_vals *)data );
    case 66:
        return (size_t)rsrc_func66( (rsrc_func66_arg *)data );
    default:
        break;
    }
    return NC_STACK_bitmap::compatcall(method_id, data);
}
