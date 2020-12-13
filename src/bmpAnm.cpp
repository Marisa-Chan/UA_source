#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "bitmap.h"
#include "bmpAnm.h"
#include "utils.h"


const Nucleus::ClassDescr NC_STACK_bmpanim::description("bmpanim.class", &newinstance);


static int dword_515200 = 0;
static int dword_515204 = 0;
static int dword_5B2410;
static tUtV *dword_5A11A0[256];


size_t NC_STACK_bmpanim::func0(IDVList &stak)
{
//    if (stak)
//    {
//        IDVList::iterator it = stak->find(BANM_ATT_NAME);
//        if ( it != stak.end() )
//            (*it).id = RSRC_ATT_NAME;
//    }

    if ( !NC_STACK_bitmap::func0(stak) )
        return 0;

    stack__bmpanim.bmpanm_intern = (bmpAnim_t1 *)getRsrc_pData();
    stack__bmpanim.frm_adds = 1;
    stack__bmpanim.anim_type = 0;
    stack__bmpanim.time_ovr = 0;
    stack__bmpanim.current_frame = stack__bmpanim.bmpanm_intern->start_frame;

    stack__bmpanim.anim_type = stak.Get<int32_t>(BANM_ATT_ANIMTYPE, 0);

    return 1;
}

size_t NC_STACK_bmpanim::func1()
{
    return NC_STACK_bitmap::func1();
}


size_t NC_STACK_bmpanim::func5(IFFile **file)
{
    char buf[250];

    IFFile *mfile = *file;

    int16_t animType = 0;
    int16_t version = 0;
    int16_t offset = 0;
    char *anmName = NULL;

    while ( 1 )
    {
        int v8 = mfile->parse();
        if ( v8 == IFFile::IFF_ERR_EOC )
            break;
        if ( v8 )
            return 0;

        if ( mfile->getCurrentChunk()->TAG == TAG_STRC )
        {
            mfile->readS16B(version);
            mfile->readS16B(offset);
            mfile->readS16B(animType);
            mfile->read(buf, 250);

            if ( version >= 1 )
            {
                anmName = buf + offset - 6;
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
        {RSRC_ATT_NAME, std::string(anmName)},
        {BANM_ATT_NAME, std::string(anmName)},
        {BANM_ATT_ANIMTYPE, (int32_t)animType}};

    return func0( stak );
}

size_t NC_STACK_bmpanim::func6(IFFile **file)
{
    __NC_STACK_bmpanim *bmpAnm = &stack__bmpanim;

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
    mfile->writeS16B(bmpAnm->anim_type);

    mfile->write(a3, strlen(a3) + 1);
    mfile->popChunk();
    return mfile->popChunk() == IFFile::IFF_ERR_OK;
}

void *sub_4BFB60(void *mfl, const char *mode)
{
    while ( *mode != 'r' )
    {
        if (! *mode)
        {
            mode = NULL;
            break;
        }
        mode++;
    }

    dword_5B2410 = mode == NULL;

    if ( dword_515200 )
    {
        IFFile *mfile = (IFFile *)mfl;
        dword_515204 = 1;

        if ( mode == NULL )
        {
            if ( (mfile->pushChunk(TAG_VANM, TAG_FORM, -1) | mfile->pushChunk(0, TAG_DATA, -1)) == IFFile::IFF_ERR_OK )
                return mfile;
        }
        else
        {
            if ( (mfile->parse() | mfile->parse()) == IFFile::IFF_ERR_OK )
                return mfile;
        }
        return NULL;
    }
    else
    {
        dword_515204 = dword_515200;
        const char* fname = (const char *)mfl;

        FSMgr::FileHandle *fil;

        if ( mode == NULL )
        {
            dword_515200 = 1;
            fil = uaOpenFile(fname, mode);
            if ( !fil )
                return NULL;
        }
        else
        {
            fil = uaOpenFile(fname, mode);
            if ( !fil )
                return NULL;

            uint32_t tmp = fil->readU32B();

            if ( tmp != TAG_FORM )
            {
                dword_515200 = 0;
                fil->seek(0, SEEK_SET);
                return fil;
            }
            else
            {
                dword_515200 = 1;
                fil->seek(0, SEEK_SET);
            }
        }

        if ( dword_515200 )
        {
            IFFile *mfile = new IFFile(fil, dword_5B2410, true);
            if ( mfile )
            {
                if ( dword_5B2410 )
                {
                    if ( (mfile->pushChunk(TAG_VANM, TAG_FORM, -1) | mfile->pushChunk(0, TAG_DATA, -1)) == IFFile::IFF_ERR_OK )
                        return mfile;
                }
                else
                {
                    if ( (mfile->parse() | mfile->parse()) == IFFile::IFF_ERR_OK )
                        return mfile;
                }
                delete mfile;
            }
            else
                delete fil;

        }
        else if (fil)
            delete fil;
    }
    return NULL;
}

int fread_bmp(void *dst, int size, int count, void *file)
{
    int v5 = 0;

    if ( dword_515200 )
        v5 = ((IFFile *)file)->read(dst, size * count);
    else
        v5 = ((FSMgr::FileHandle *)file)->read(dst, size * count);

    if ( v5 >= 0 )
        return v5 / size;
    else
        return 0;
}

int fwrite_bmp(void *dst, int size, int count, void *file)
{
    int v5 = 0;

    if ( dword_515200 )
        v5 = ((IFFile *)file)->write(dst, size * count);
    else
        v5 = ((FSMgr::FileHandle *)file)->write(dst, size * count);

    if ( v5 >= 0 )
        return v5 / size;
    else
        return 0;
}

int bmpanim_func64__readClassName(void *fil, bmpAnim_t1 *arg)
{
    int16_t sz;

    if ( fread_bmp(&sz, 2, 1, fil) == 1 )
    {
        sz = SWAP16(sz);

        arg->className = (char *)AllocVec(sz, 1);
        if ( arg->className )
        {
            if ( fread_bmp(arg->className, sz, 1, fil) == 1 )
                return 1;
        }
    }
    return 0;
}

int bmpanim_func64__sub1__sub2(void *fil, bmpAnim_t1 *arg)
{
    int16_t sz;
    char *pbmpAnm_titles[256];

    if ( fread_bmp(&sz, 2, 1, fil) == 1 )
    {
        sz = SWAP16(sz);
        arg->titles_size = sz;

        char *buf = (char *)AllocVec(sz, 1);
        if ( buf )
        {
            arg->titles = buf;
            if ( fread_bmp(buf, sz, 1, fil) == 1 )
            {
                char *v8 = buf;

                int nm_cnt = -1;

                for (int i = 0; i < 256; i++)
                {
                    if (v8 >= (buf + sz) || *v8 == 0 )
                    {
                        pbmpAnm_titles[i] = NULL;
                        nm_cnt = i;
                        break;
                    }
                    else
                    {
                        pbmpAnm_titles[i] = v8;
                        v8 += strlen(v8) + 1;
                    }
                }

                if (nm_cnt == -1)
                    return 0;

                arg->bitm_buff_cnt = nm_cnt;

                bmpAnim_t1_objs *frames = (bmpAnim_t1_objs *)AllocVec(sizeof(bmpAnim_t1_objs) * nm_cnt, 65537);
                if ( frames )
                {
                    arg->bitm_buff = frames;

                    for (int i = 0; i < nm_cnt; i++)
                    {
                        frames[i].bitmObj = Nucleus::CTFInit<NC_STACK_bitmap>(arg->className, {
                            {NC_STACK_rsrc::RSRC_ATT_NAME, std::string(pbmpAnm_titles[i])},
                            {NC_STACK_rsrc::RSRC_ATT_DONTCOPY, (int32_t)1}} );

                        if ( !frames[i].bitmObj )
                            return 0;

                        frames[i].bitm_intern = frames[i].bitmObj->GetResBmp();
                        frames[i].title = pbmpAnm_titles[i];
                    }
                    return 1;
                }
            }
        }
    }
    return 0;
}

int bmpanim_func64__sub1__sub1(void *fil, bmpAnim_t1 *arg)
{
    int16_t cnt;
    if ( fread_bmp(&cnt, 2, 1, fil) != 1 )
        return 0;

    cnt = SWAP16(cnt);

    arg->otl_buff_cnt = cnt;

    arg->otl_buff = (tUtV *)AllocVec(sizeof(tUtV) * (int)cnt, 1);
    if ( !arg->otl_buff )
        return 0;

    tUtV *v5 = arg->otl_buff;

    int k = 0;

    for (int i = 0; i < cnt; i++)
    {
        dword_5A11A0[k] = v5;

        uint16_t ttmp;
        if ( fread_bmp(&ttmp, 2, 1, fil) != 1 )
            return 0;

        ttmp = SWAP16(ttmp);

        for (int j = 0; j < ttmp; j++)
        {
            uint8_t uv[2];
            if (fread_bmp(uv, 2, 1, fil) != 1)
                return 0;

            v5->tu = (float)uv[0] / 256.0;
            v5->tv = (float)uv[1] / 256.0;
            v5++;
        }

        v5->tu = -1.0;
        v5->tv = -1.0;
        v5++;

        i += ttmp;
        k++;
    }

    return 1;
}

int bmpanim_func64__sub1__sub0(void *fil, bmpAnim_t1 *arg)
{
    int16_t cnt;
    if ( fread_bmp(&cnt, 2, 1, fil) != 1 )
        return 0;
    cnt = SWAP16(cnt);

    bmpAnim_t2 *t2 = (bmpAnim_t2 *)AllocVec(sizeof(bmpAnim_t2) * (int)cnt, 1);

    if ( !t2 )
        return 0;

    arg->frame_cnt = cnt;
    arg->start_frame = t2;
    arg->end_frame = &t2[cnt];

    for (int i = 0; i < cnt; i++)
    {
        bmpanm_loc v14;
        if (fread_bmp(&v14, 8, 1, fil) != 1)
            return 0;

        v14.frm_time = SWAP32(v14.frm_time);
        v14.frame_id = SWAP16(v14.frame_id);
        v14.uv_id = SWAP16(v14.uv_id);

        t2[i].outline = dword_5A11A0[v14.uv_id];
        t2[i].bitm = arg->bitm_buff[v14.frame_id].bitm_intern;
        t2[i].frm_time = v14.frm_time;
        t2[i].bitm_index = v14.frame_id;
        t2[i].otl_index = v14.uv_id;
    }
    return 1;
}

void sub_431608(bmpAnim_t1 *bmpAnm)
{
    if ( bmpAnm )
    {
        if ( bmpAnm->titles )
            nc_FreeMem(bmpAnm->titles);

        if ( bmpAnm->otl_buff )
            nc_FreeMem(bmpAnm->otl_buff);

        if ( bmpAnm->className )
            nc_FreeMem(bmpAnm->className);

        if ( bmpAnm->start_frame )
            nc_FreeMem(bmpAnm->start_frame);

        if ( bmpAnm->bitm_buff )
        {
            for (int i = 0; i < bmpAnm->bitm_buff_cnt; i++)
            {
                if (bmpAnm->bitm_buff[i].bitmObj)
                    delete_class_obj(bmpAnm->bitm_buff[i].bitmObj);
            }
            nc_FreeMem(bmpAnm->bitm_buff);
        }
        nc_FreeMem(bmpAnm);
    }
}

int sub_4BFD74(void *fil)
{
    if ( fil )
    {
        if ( dword_515200 )
        {
            if ( dword_5B2410 )
            {
                ((IFFile *)fil)->popChunk();
                ((IFFile *)fil)->popChunk();
            }
            else
            {
                ((IFFile *)fil)->parse();
                ((IFFile *)fil)->parse();
            }

            if ( !dword_515204 )
            {
                delete ((IFFile *)fil);
            }
        }
        else
        {
            delete ((FSMgr::FileHandle *)fil);
        }
    }
    return 0;
}

bmpAnim_t1 *bmpanim_func64__sub1(const std::string &name, IFFile *a2)
{
    char buf[256];
    void *ldfrom = a2;

    bmpAnim_t1 *v19 = NULL;
    int v23 = 0;

    if ( ldfrom )
    {
        dword_515200 = 1;
    }
    else
    {
        strcpy(buf, "rsrc:");
        strcat(buf, "rsrcpool/");
        strcat(buf, name.c_str());

        ldfrom = buf;
        dword_515200 = 0;
    }

    void *fil = sub_4BFB60(ldfrom, "rb");
    if ( fil )
    {
        v19 = (bmpAnim_t1 *)AllocVec(sizeof(bmpAnim_t1), 65537);
        if ( v19 )
        {
            if ( bmpanim_func64__readClassName(fil, v19) )
            {
                if ( bmpanim_func64__sub1__sub2(fil, v19) )
                {
                    if ( bmpanim_func64__sub1__sub1(fil, v19) )
                    {
                        if ( bmpanim_func64__sub1__sub0(fil, v19) )
                            v23 = 1;
                    }
                }
            }
            if ( !v23 )
                sub_431608(v19);
        }
        sub_4BFD74(fil);
    }
    if ( v23 && v19 )
        return v19;

    return NULL;
}

int bmpanim_func64__sub0__sub0(bmpAnim_t1 *t1, char **a2, const std::string &className)
{
    t1->className = (char *)AllocVec(className.size() + 1, 1);
    if ( !t1->className )
        return 0;

    strcpy(t1->className, className.c_str());

    int bfsz = 0;
    int sz = 0;

    char **pt = a2;

    while(*pt)
    {
        bfsz += strlen(*pt) + 1;
        sz++;
        pt++;
    }

    t1->bitm_buff_cnt = sz;
    t1->titles_size = bfsz + 1;

    t1->titles = (char *)AllocVec(t1->titles_size, 1);
    if ( !t1->titles )
        return 0;

    t1->bitm_buff = (bmpAnim_t1_objs *)AllocVec(sizeof(bmpAnim_t1_objs) * sz, 65537);
    if (!t1->bitm_buff)
        return 0;

    pt = a2;

    char *out = t1->titles;
    for (int i = 0; i < sz; i++)
    {
        strcpy(out, *pt);

        t1->bitm_buff[i].bitmObj = Nucleus::CTFInit<NC_STACK_bitmap>(className,
           {{NC_STACK_rsrc::RSRC_ATT_NAME, std::string(out)},
            {NC_STACK_rsrc::RSRC_ATT_DONTCOPY, (int32_t)1}} );
            
        if ( !t1->bitm_buff[i].bitmObj )
            return 0;

        t1->bitm_buff[i].bitm_intern = t1->bitm_buff[i].bitmObj->GetResBmp();

        t1->bitm_buff[i].title = out;

        out += strlen(out) + 1;
        pt++;
    }
    *out = 0;

    return 1;
}

int bmpanim_func64__sub0__sub1(bmpAnim_t1 *t1, pixel_2d **opls)
{
    pixel_2d **v3 = opls;

    int nm = 0;
    while (*v3)
    {
        int v5 = 1;

        pixel_2d *v6 = *v3;
        while ( v6->flags >= 0 )
        {
            v6++;
            v5++;
        }

        nm += v5;
        v3++;
    }

    t1->otl_buff_cnt = nm;

    t1->otl_buff = (tUtV *)AllocVec(sizeof(tUtV) * nm, 1);

    if ( !t1->otl_buff )
        return 0;

    v3 = opls;
    tUtV *out = t1->otl_buff;
    int j = 0;

    while (*v3)
    {
        int v5 = 1;

        pixel_2d *v6 = *v3;
        while ( v6->flags >= 0 )
        {
            v6++;
            v5++;
        }

        dword_5A11A0[j] = out;

        for(int i = 0; i < (v5 - 1); i++)
        {
            out->tu = (*v3)[i].x / 256.0;
            out->tv = (*v3)[i].y / 256.0;
            out++;
        }

        out->tu = -1.0;
        out->tv = -1.0;
        out++;

        nm += v5;
        v3++;
        j++;
    }

    return 1;
}

int bmpanim_func64__sub0__sub2(bmpAnim_t1 *t1, int num, bmpanm_loc *arg)
{
    t1->frame_cnt = num;

    bmpAnim_t2 *t2 = (bmpAnim_t2 *)AllocVec(sizeof(bmpAnim_t2) * num, 1);
    if ( !t2 )
        return 0;

    t1->start_frame = t2;

    for (int i = 0; i < num; i++)
    {
        t2[i].outline = dword_5A11A0[ arg[i].uv_id ];
        t2[i].bitm = t1->bitm_buff[ arg[i].frame_id ].bitm_intern;
        t2[i].frm_time = arg[i].frm_time;
        t2[i].bitm_index = arg[i].frame_id;
        t2[i].otl_index = arg[i].uv_id;
    }
    t1->end_frame = &t2[num];
    return 1;
}

bmpAnim_t1 * bmpanim_func64__sub0(const std::string &className, char **a2, pixel_2d **a3, int a4, bmpanm_loc *a5)
{
    bmpAnim_t1 *t1 = (bmpAnim_t1 *)AllocVec(sizeof(bmpAnim_t1), 65537);

    if ( !t1 )
        return NULL;

    if ( !bmpanim_func64__sub0__sub0(t1, a2, className) )
    {
        sub_431608(t1);
        return NULL;
    }

    if ( !bmpanim_func64__sub0__sub1(t1, a3) )
    {
        sub_431608(t1);
        return NULL;
    }

    if ( !bmpanim_func64__sub0__sub2(t1, a4, a5) )
    {
        sub_431608(t1);
        return NULL;
    }


    return t1;
}

// Create bmpanim resource node and fill rsrc field data
rsrc * NC_STACK_bmpanim::rsrc_func64(IDVList &stak)
{
    stak.Add(RSRC_ATT_LISTYPE, (int32_t)1);

    rsrc *res = NC_STACK_bitmap::rsrc_func64(stak);// rsrc_func64
    if ( res )
    {

        const std::string a1 = stak.Get<std::string>(BANM_ATT_CLASSNAME, "");
        if ( !a1.empty() )
        {
            char **titles = stak.Get<char **>(BANM_ATT_FILE_TITLES, NULL);
            pixel_2d **opls = stak.Get<pixel_2d **>(BANM_ATT_OUTLINES, NULL);
            int num = stak.Get<int32_t>(BANM_ATT_FRAMECNT, 0);
            bmpanm_loc *v7 = stak.Get<bmpanm_loc *>(BANM_ATT_SEQDATA, NULL);

            if ( titles && opls && num && v7 )
                res->data = bmpanim_func64__sub0(a1, titles, opls, num, v7);
        }
        else
        {
            const std::string v9 = stak.Get<std::string>(RSRC_ATT_NAME, "");
            IFFile *v10 = stak.Get<IFFile *>(RSRC_ATT_PIFFFILE, NULL);
            if ( !v9.empty() )
                res->data = bmpanim_func64__sub1(v9, v10);
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
    bmpAnim_t1 *v5 = (bmpAnim_t1 *)res->data;

    if ( v5 )
    {
        sub_431608(v5);
        res->data = NULL;
    }
    return NC_STACK_bitmap::rsrc_func65(res);
}

int bmpanim_func66__sub0__sub3(void *fil, bmpAnim_t1 *t1)
{
    int16_t v5 = strlen(t1->className) + 1;
    v5 = SWAP16(v5);

    if ( fwrite_bmp(&v5, 2, 1, fil) != 1 )
        return 0;

    v5 = SWAP16(v5);

    if ( fwrite_bmp(t1->className, v5, 1, fil) != 1 )
        return 0;
    return 1;
}

int bmpanim_func66__sub0__sub2(void *fil, bmpAnim_t1 *t1)
{
    int16_t v7 = t1->titles_size;
    v7 = SWAP16(v7);

    if ( fwrite_bmp(&v7, 2, 1, fil) != 1)
        return 0;

    if ( fwrite_bmp(t1->titles, t1->titles_size, 1, fil) != 1 )
        return 0;

    return 1;
}

int bmpanim_func66__sub0__sub1(void *fil, bmpAnim_t1 *t1)
{
    tUtV *uv_end = &t1->otl_buff[t1->otl_buff_cnt];

    int16_t v17 = SWAP16(t1->otl_buff_cnt);

    if ( fwrite_bmp(&v17, 2, 1, fil) != 1 )
        return 0;

    tUtV *v6 = t1->otl_buff;

    while( v6 != uv_end )
    {
        int v7 = 1;

        while ( v6[v7 - 1].tu >= 0.0 )
            v7++;

        v17 = SWAP16(v7);

        if ( fwrite_bmp(&v17, 2, 1, fil) != 1 )
            return 0;


        while ( v6->tu >= 0.0)
        {
            uint8_t uv[2];
            uv[0] = v6->tu * 256.0;
            uv[1] = v6->tv * 256.0;

            if ( fwrite_bmp(uv, 2, 1, fil) != 1 )
                return 0;
            v6++;
        }
        v6++;
    }
    return 1;
}

int bmpanim_func66__sub0__sub0(void *fil, bmpAnim_t1 *t1)
{
    int16_t cnt = SWAP16(t1->frame_cnt);

    if ( fwrite_bmp(&cnt, 2, 1, fil) != 1 )
        return 0;

    for(int i = 0; i < t1->frame_cnt; i++)
    {
        bmpanm_loc lc;

        bmpAnim_t2 *v6 = &t1->start_frame[i];

        lc.frm_time = SWAP32(v6->frm_time);
        lc.frame_id = SWAP16(v6->bitm_index);
        lc.uv_id = SWAP16(v6->otl_index);

        if ( fwrite_bmp(&lc, 8, 1, fil) != 1 )
            return 0;
    }
    return 1;
}

int bmpanim_func66__sub0(bmpAnim_t1 *t1, const char *resName, IFFile *a3)
{
    int v22 = 0;

    char buf[256];
    void *saveto = NULL;

    if ( a3 )
    {
        saveto = a3;
        dword_515200 = 1;
    }
    else
    {
        strcpy(buf, "rsrc:");
        strcat(buf, "rsrcpool/");
        strcat(buf, resName);
        saveto = buf;
        dword_515200 = 0;
    }

    void *fil = sub_4BFB60(saveto, "wb");
    if ( fil )
    {
        if ( bmpanim_func66__sub0__sub3(fil, t1) )
        {
            if ( bmpanim_func66__sub0__sub2(fil, t1) )
            {
                if ( bmpanim_func66__sub0__sub1(fil, t1) )
                {
                    if ( bmpanim_func66__sub0__sub0(fil, t1) )
                        v22 = 1;
                }
            }
        }
        sub_4BFD74(fil);
    }
    return v22;
}

size_t NC_STACK_bmpanim::rsrc_func66(rsrc_func66_arg *sv)
{
    __NC_STACK_bmpanim *bmpanm = &stack__bmpanim;

    if ( !bmpanm->bmpanm_intern )
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
    if ( bmpanim_func66__sub0(bmpanm->bmpanm_intern, resName, mfile) )
        return sv->OpenedStream;
    return 0;
}

void NC_STACK_bmpanim::bitmap_func130(bitmap_arg130 *arg)
{
    __NC_STACK_bmpanim *bmpAnm = &stack__bmpanim;

    if ( arg->frame_time == -1 )
    {
        bmpAnim_t2 *t2 = &bmpAnm->current_frame[ bmpAnm->frm_adds ];

        if ( t2 == bmpAnm->bmpanm_intern->end_frame )
        {
            if ( bmpAnm->anim_type )
            {
                t2--;
                bmpAnm->frm_adds = -1;
            }
            else
            {
                t2 = bmpAnm->bmpanm_intern->start_frame;
            }
        }
        else if ( t2 < bmpAnm->bmpanm_intern->start_frame )
        {
            t2++;
            bmpAnm->frm_adds = 1;
        }

        bmpAnm->current_frame = t2;
        arg->pbitm = t2->bitm;
        arg->outline = t2->outline;
    }
    else
    {
        if ( arg->time_stmp != bmpAnm->time_stmp )
        {
            bmpAnm->time_stmp = arg->time_stmp;

            bmpAnim_t2 *t2 = bmpAnm->current_frame;
            int v8 = arg->frame_time + bmpAnm->time_ovr;

            while ( v8 - t2->frm_time >= 0 )
            {
                v8 = v8 - t2->frm_time;

                t2 += bmpAnm->frm_adds;

                if ( t2 == bmpAnm->bmpanm_intern->end_frame )
                {
                    if ( bmpAnm->anim_type )
                    {
                        t2--;
                        bmpAnm->frm_adds = -1;
                    }
                    else
                    {
                        t2 = bmpAnm->bmpanm_intern->start_frame;
                    }
                }
                else if ( t2 < bmpAnm->bmpanm_intern->start_frame )
                {
                    t2++;
                    bmpAnm->frm_adds = 1;
                }
            }

            bmpAnm->time_ovr = v8;
            bmpAnm->current_frame = t2;
            arg->pbitm = t2->bitm;
            arg->outline = t2->outline;
        }
        else
        {
            arg->pbitm = bmpAnm->current_frame->bitm;
            arg->outline = bmpAnm->current_frame->outline;
        }
    }
}



void NC_STACK_bmpanim::setBANM_animType(int newType)
{
    stack__bmpanim.anim_type = newType;
}

ResBitmap * NC_STACK_bmpanim::GetResBmp()
{
    return stack__bmpanim.current_frame->bitm;
}

int NC_STACK_bmpanim::getBMD_width()
{
    return 0;
}

int NC_STACK_bmpanim::getBMD_height()
{
    return 0;
}

void *NC_STACK_bmpanim::getBMD_buffer()
{
    return NULL;
}

const char *NC_STACK_bmpanim::getBANM_name()
{
    return getRsrc_name();
}

const char *NC_STACK_bmpanim::getBANM_classname()
{
    return stack__bmpanim.bmpanm_intern->className;
}

int NC_STACK_bmpanim::getBANM_framecnt()
{
    return stack__bmpanim.bmpanm_intern->frame_cnt;
}

int NC_STACK_bmpanim::getBANM_animtype()
{
    return stack__bmpanim.anim_type;
}

void NC_STACK_bmpanim::PrepareTexture( bool force )
{
    if (!stack__bmpanim.bmpanm_intern)
        return;
    
    for(int i = 0; i < stack__bmpanim.bmpanm_intern->bitm_buff_cnt; i++)
        stack__bmpanim.bmpanm_intern->bitm_buff[i].bitmObj->PrepareTexture(force);
}
