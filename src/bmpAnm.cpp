#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "bitmap.h"
#include "bmpAnm.h"
#include "utils.h"


const NewClassDescr NC_STACK_bmpanim::description("bmpanim.class", &newinstance);


static int dword_515200 = 0;
static int dword_515204 = 0;
static int dword_5B2410;
static tUtV *dword_5A11A0[256];


size_t NC_STACK_bmpanim::func0(stack_vals *stak)
{
    stack_vals *val = find_id_in_stack2(0x80003000, stak);
    if ( val )
        val->id = 0x80001000;

    if ( !NC_STACK_bitmap::func0(stak) )
        return 0;

    __NC_STACK_bmpanim *bmpanm = &this->stack__bmpanim;
    call_vtbl(this, 3, 0x80001002, bmpanm, 0);

    bmpanm->field_12 = 1;
    bmpanm->field_10 = find_id_in_stack_def_val(0x80003004, 0, stak);
    bmpanm->field_C = 0;
    bmpanm->t2_ = bmpanm->bmpanm_intern->t2;

    return 1;
}

size_t NC_STACK_bmpanim::func1(stack_vals *)
{
    return NC_STACK_bitmap::func1(NULL);
}

size_t NC_STACK_bmpanim::func2(stack_vals *stak)
{
    __NC_STACK_bmpanim *bmpAnm = &this->stack__bmpanim;

    stack_vals *val = find_id_in_stack2(0x80003004, stak);
    if ( val )
        bmpAnm->field_10 = val->value;

    return NC_STACK_bitmap::func2(stak);
}

size_t NC_STACK_bmpanim::func3(stack_vals *stak)
{
    __NC_STACK_bmpanim *bmpAnm = &this->stack__bmpanim;

    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == 0)
            break;
        else if (stk->id == 2)
        {
            stk = (stack_vals *)stk->value;
        }
        else if ( stk->id == 3 )
        {
            stk += stk->value;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;

            case 0x80002000:
                *(bitmap_intern **)stk->value = bmpAnm->t2_->bitm;
                break;
            case 0x80002001:
            case 0x80002002:
            case 0x80002003:
            case 0x80002004:
            case 0x80002005:
                *(int *)stk->value = 0;
                break;

            case 0x80003000:
                call_vtbl(this, 3, 0x80001000, stk->value, 0);
                break;
            case 0x80003001:
                *(char **)stk->value = bmpAnm->bmpanm_intern->className;
                break;
            case 0x80003005:
                *(int *)stk->value = bmpAnm->bmpanm_intern->t2_cnt;
                break;
            case 0x80003004:
                *(int *)stk->value = bmpAnm->field_10;
                break;
            }
            stk++;
        }
    }

    return NC_STACK_bitmap::func3(stak);
}

size_t NC_STACK_bmpanim::func5(MFILE **file)
{
    int16_t buf[128];

    MFILE *mfile = *file;

    int v7 = 0;
    int v4;
    char *v3;

    while ( 1 )
    {
        int v8 = read_next_IFF(mfile, 2);
        if ( v8 == -2 )
            break;
        if ( v8 )
            return 0;

        if ( GET_FORM_INFO_OR_NULL(mfile)->TAG == TAG_STRC )
        {

            mfread(mfile, buf, sizeof(int16_t) * 128);
            buf[0] = SWAP16(buf[0]);
            buf[1] = SWAP16(buf[1]);
            buf[2] = SWAP16(buf[2]);

            v7 = buf[0];

            if ( buf[0] >= 1 )
            {
                v4 = buf[2];
                v3 = (char *)buf + buf[1];
            }
            read_next_IFF(mfile, 2);
        }
        else
        {
            read_default(mfile);
        }
    }

    if ( !v7 )
        return 0;

    stack_vals stk[4];

    stk[0].id = 0x80003000;
    stk[0].value = (size_t)v3;
    stk[1].id = 0x80001004;
    stk[1].value = 1;
    stk[2].id = 0x80003004;
    stk[2].value = v4;
    stk[3].id = 0;

    return func0(stk);
}

size_t NC_STACK_bmpanim::func6(MFILE **file)
{
    __NC_STACK_bmpanim *bmpAnm = &this->stack__bmpanim;

    MFILE *mfile = *file;

    const char *a3;
    call_vtbl(this, 3, 0x80001000, &a3, 0);

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

    if ( sub_412FC0(mfile, TAG_BANI, TAG_FORM, -1) )
    {
        return 0;
    }

    sub_412FC0(mfile, 0, TAG_STRC, -1);

    int16_t tmp[3];
    tmp[0] = SWAP16(1);
    tmp[1] = SWAP16(6); //sizeof tmp
    tmp[2] = SWAP16(bmpAnm->field_10);

    sub_413564(mfile, 6, &tmp);
    sub_413564(mfile, strlen(a3) + 1, a3);
    sub_413290(mfile);

    return sub_413290(mfile) == 0;
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
        MFILE *mfile = (MFILE *)mfl;
        dword_515204 = 1;

        if ( mode == NULL )
        {
            if ( (sub_412FC0(mfile, TAG_VANM, TAG_FORM, -1) | sub_412FC0(mfile, 0, TAG_DATA, -1)) == 0 )
                return mfile;
        }
        else
        {
            if ( (read_next_IFF(mfile, 2) | read_next_IFF(mfile, 2)) == 0 )
                return mfile;
        }
        return NULL;
    }
    else
    {
        dword_515204 = dword_515200;
        const char* fname = (const char *)mfl;

        FILE *v7;

        if ( mode == NULL )
        {
            dword_515200 = 1;
            v7 = FOpen(fname, mode);
            if ( !v7 )
                return NULL;
        }
        else
        {
            v7 = FOpen(fname, mode);
            if ( !v7 )
                return NULL;

            uint32_t tmp;
            fread(&tmp, 4, 1, v7);
            tmp = SWAP32(tmp);

            if ( tmp != TAG_FORM )
            {
                dword_515200 = 0;
                fseek(v7, 0, 0);
                return v7;
            }
            else
            {
                dword_515200 = 1;
                fseek(v7, 0, 0);
            }
        }

        if ( dword_515200 )
        {
            MFILE *v9 = new_MFILE();
            if ( v9 )
            {
                v9->file_handle = v7;
                if ( !sub_412F98(v9, dword_5B2410) )
                {
                    if ( dword_5B2410 )
                    {
                        if ( (sub_412FC0(v9, TAG_VANM, TAG_FORM, -1) | sub_412FC0(v9, 0, TAG_DATA, -1)) == 0 )
                            return v9;
                    }
                    else
                    {
                        if ( (read_next_IFF(v9, 2) | read_next_IFF(v9, 2)) == 0 )
                            return v9;
                    }
                }
                FClose(v7);
                del_MFILE(v9);
            }
        }
        else if (v7)
            FClose(v7);
    }
    return NULL;
}

int fread_bmp(void *dst, int size, int count, void *file)
{
    if ( dword_515200 )
    {
        int v5 = mfread((MFILE *)file, dst, size * count);
        if ( v5 >= 0 )
            return v5 / size;
        else
            return 0;
    }
    else
    {
        return fread(dst, size, count, (FILE *)file);
    }
}

int fwrite_bmp(void *dst, int size, int count, void *file)
{
    if ( dword_515200 )
    {
        int v5 = sub_413564((MFILE *)file, size * count, dst);
        if ( v5 >= 0 )
            return v5 / size;
        else
            return 0;
    }
    else
    {
        return fwrite(dst, size, count, (FILE *)file);
    }
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
        arg->field_1A = sz;

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

                arg->frame_count = nm_cnt;

                bmpAnim_t1_objs *frames = (bmpAnim_t1_objs *)AllocVec(sizeof(bmpAnim_t1_objs) * nm_cnt, 65537);
                if ( frames )
                {
                    arg->frames = frames;

                    for (int i = 0; i < nm_cnt; i++)
                    {
                        frames[i].clsObj = (NC_STACK_bitmap *)init_get_class(arg->className, 0x80001000, pbmpAnm_titles[i], 0x80001003, 1, 0);

                        if ( !frames[i].clsObj )
                            return 0;

                        call_vtbl(frames[i].clsObj, 3, 0x80002000, &frames[i].bitm_intern, 0);
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

    arg->field_1C = cnt;

    arg->field_4 = (tUtV *)AllocVec(sizeof(tUtV) * (int)cnt, 1);
    if ( !arg->field_4 )
        return 0;

    tUtV *v5 = arg->field_4;

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
            BYTE uv[2];
            if (fread_bmp(uv, 2, 1, fil) != 1)
                return 0;

            v5->tu = uv[0] / 256.0;
            v5->tv = uv[1] / 256.0;
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

    arg->t2_cnt = cnt;
    arg->t2 = t2;
    arg->t2_end = &t2[cnt];

    for (int i = 0; i < cnt; i++)
    {
        bmpanm_loc v14;
        if (fread_bmp(&v14, 8, 1, fil) != 1)
            return 0;

        v14.a = SWAP32(v14.a);
        v14.frame_id = SWAP16(v14.frame_id);
        v14.uv_id = SWAP16(v14.uv_id);

        t2[i].opls = dword_5A11A0[v14.uv_id];
        t2[i].bitm = arg->frames[v14.frame_id].bitm_intern;
        t2[i].field_8 = v14.a;
        t2[i].field_C = v14.frame_id;
        t2[i].field_E = v14.uv_id;
    }
    return 1;
}

void sub_431608(bmpAnim_t1 *bmpAnm)
{
    if ( bmpAnm )
    {
        if ( bmpAnm->titles )
            nc_FreeMem(bmpAnm->titles);

        if ( bmpAnm->field_4 )
            nc_FreeMem(bmpAnm->field_4);

        if ( bmpAnm->className )
            nc_FreeMem(bmpAnm->className);

        if ( bmpAnm->t2 )
            nc_FreeMem(bmpAnm->t2);

        if ( bmpAnm->frames )
        {
            for (int i = 0; i < bmpAnm->frame_count; i++)
            {
                if (bmpAnm->frames[i].clsObj)
                    delete_class_obj(bmpAnm->frames[i].clsObj);
            }
            nc_FreeMem(bmpAnm->frames);
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
                sub_413290((MFILE *)fil);
                sub_413290((MFILE *)fil);
            }
            else
            {
                read_next_IFF((MFILE *)fil, 2);
                read_next_IFF((MFILE *)fil, 2);
            }

            if ( !dword_515204 )
            {
                FClose(((MFILE *)fil)->file_handle);
                del_MFILE((MFILE *)fil);
            }
        }
        else
        {
            FClose((FILE *)fil);
        }
    }
    return 0;
}

bmpAnim_t1 *bmpanim_func64__sub1(char *name, MFILE *a2)
{
    char buf[256];
    const char *mode;
    void *ldfrom = a2;

    bmpAnim_t1 *v19 = NULL;
    int v23 = 0;

    if ( ldfrom )
    {
        mode = "rb";
        dword_515200 = 1;
    }
    else
    {
        strcpy(buf, get_MC_str(MC_TYPE_RES));
        strcat(buf, "rsrcpool/");
        strcat(buf, name);

        ldfrom = buf;
        dword_515200 = 0;
        mode = "rb";
    }

    void *fil = sub_4BFB60(ldfrom, mode);
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

int bmpanim_func64__sub0__sub0(bmpAnim_t1 *t1, char **a2, const char *className)
{
    t1->className = (char *)AllocVec(strlen(className) + 1, 1);
    if ( !t1->className )
        return 0;

    strcpy(t1->className, className);

    int bfsz = 0;
    int sz = 0;

    char **pt = a2;

    while(*pt)
    {
        bfsz += strlen(*pt) + 1;
        sz++;
        pt++;
    }

    t1->frame_count = sz;
    t1->field_1A = bfsz + 1;

    t1->titles = (char *)AllocVec(t1->field_1A, 1);
    if ( !t1->titles )
        return 0;

    t1->frames = (bmpAnim_t1_objs *)AllocVec(sizeof(bmpAnim_t1_objs) * sz, 65537);
    if (!t1->frames)
        return 0;

    pt = a2;

    char *out = t1->titles;
    for (int i = 0; i < sz; i++)
    {
        strcpy(out, *pt);

        t1->frames[i].clsObj = dynamic_cast<NC_STACK_bitmap *>( init_get_class(className, 0x80001000, out, 0x80001003, 1, 0) );
        if ( !t1->frames[i].clsObj )
            return 0;

        call_vtbl(t1->frames[i].clsObj, 3, 0x80002000, &t1->frames[i].bitm_intern, 0);

        t1->frames[i].title = out;

        out += strlen(out) + 1;
        pt++;
    }
    *out = 0;

    return 1;
}

int bmpanim_func64__sub0__sub1(bmpAnim_t1 *t1, bitmap__opl **opls)
{
    bitmap__opl **v3 = opls;

    int nm = 0;
    while (*v3)
    {
        int v5 = 1;

        bitmap__opl *v6 = *v3;
        while ( v6->field_E >= 0 )
        {
            v6++;
            v5++;
        }

        nm += v5;
        v3++;
    }

    t1->field_1C = nm;

    t1->field_4 = (tUtV *)AllocVec(sizeof(tUtV) * nm, 1);

    if ( !t1->field_4 )
        return 0;

    v3 = opls;
    tUtV *out = t1->field_4;
    int j = 0;

    while (*v3)
    {
        int v5 = 1;

        bitmap__opl *v6 = *v3;
        while ( v6->field_E >= 0 )
        {
            v6++;
            v5++;
        }

        dword_5A11A0[j] = out;

        for(int i = 0; i < (v5 - 1); i++)
        {
            out->tu = (*v3)[i].field_0 / 256.0;
            out->tv = (*v3)[i].field_2 / 256.0;
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
    t1->t2_cnt = num;

    bmpAnim_t2 *t2 = (bmpAnim_t2 *)AllocVec(sizeof(bmpAnim_t2) * num, 1);
    if ( !t2 )
        return 0;

    t1->t2 = t2;

    for (int i = 0; i < num; i++)
    {
        t2[i].opls = dword_5A11A0[ arg[i].uv_id ];
        t2[i].bitm = t1->frames[ arg[i].frame_id ].bitm_intern;
        t2[i].field_8 = arg[i].a;
        t2[i].field_C = arg[i].frame_id;
        t2[i].field_E = arg[i].uv_id;
    }
    t1->t2_end = &t2[num];
    return 1;
}

bmpAnim_t1 * bmpanim_func64__sub0(const char *className, char **a2, bitmap__opl **a3, int a4, bmpanm_loc *a5)
{
    bmpAnim_t1 *t1 = (bmpAnim_t1 *)AllocVec(32, 65537);

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
rsrc * NC_STACK_bmpanim::rsrc_func64(stack_vals *stak)
{
    stack_vals stk[2];

    stk[0].id = 0x80001008;
    stk[0].value = 1;
    stk[1].id = 2;
    stk[1].value = (size_t)stak;

    rsrc *res = NC_STACK_bitmap::rsrc_func64(stk);// rsrc_func64
    if ( res )
    {

        const char *a1 = (const char *)find_id_in_stack_def_val(0x80003001, 0, stak);
        if ( a1 )
        {
            char **titles = (char **)find_id_in_stack_def_val(0x80003002, 0, stak);
            bitmap__opl **opls = (bitmap__opl **)find_id_in_stack_def_val(0x80003003, 0, stak);
            int num = find_id_in_stack_def_val(0x80003005, 0, stak);
            bmpanm_loc *v7 = (bmpanm_loc *)find_id_in_stack_def_val(0x80003006, 0, stak);

            if ( titles && opls && num && v7 )
                res->data = bmpanim_func64__sub0(a1, titles, opls, num, v7);
        }
        else
        {
            char *v9 = (char *)find_id_in_stack_def_val(0x80001000, 0, stak);
            MFILE *v10 = (MFILE *)find_id_in_stack_def_val(0x80001005, 0, stak);
            if ( v9 )
                res->data = bmpanim_func64__sub1(v9, v10);
        }

        if ( !res->data )
        {
            //call_vtbl(obj, 65, res);
            rsrc_func65(&res);
            return NULL;
        }

    }
    return res;
}

size_t NC_STACK_bmpanim::rsrc_func65(rsrc **pres)
{
    rsrc *res = *pres;

    bmpAnim_t1 *v5 = (bmpAnim_t1 *)res->data;

    if ( v5 )
    {
        sub_431608(v5);
        res->data = NULL;
    }
    return NC_STACK_bitmap::rsrc_func65(pres);
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
    int16_t v7 = t1->field_1A;
    v7 = SWAP16(v7);

    if ( fwrite_bmp(&v7, 2, 1, fil) != 1)
        return 0;

    if ( fwrite_bmp(t1->titles, t1->field_1A, 1, fil) != 1 )
        return 0;

    return 1;
}

int bmpanim_func66__sub0__sub1(void *fil, bmpAnim_t1 *t1)
{
    tUtV *uv_end = &t1->field_4[t1->field_1C];

    int16_t v17 = SWAP16(t1->field_1C);

    if ( fwrite_bmp(&v17, 2, 1, fil) != 1 )
        return 0;

    tUtV *v6 = t1->field_4;

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
            BYTE uv[2];
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
    int16_t cnt = SWAP16(t1->t2_cnt);

    if ( fwrite_bmp(&cnt, 2, 1, fil) != 1 )
        return 0;

    for(int i = 0; i < t1->t2_cnt; i++)
    {
        bmpanm_loc lc;

        bmpAnim_t2 *v6 = &t1->t2[i];

        lc.a = SWAP32(v6->field_8);
        lc.frame_id = SWAP16(v6->field_C);
        lc.uv_id = SWAP16(v6->field_E);

        if ( fwrite_bmp(&lc, 8, 1, fil) != 1 )
            return 0;
    }
    return 1;
}

int bmpanim_func66__sub0(bmpAnim_t1 *t1, const char *resName, MFILE *a3)
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
        strcpy(buf, get_MC_str(MC_TYPE_RES));
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
    __NC_STACK_bmpanim *bmpanm = &this->stack__bmpanim;

    if ( !bmpanm->bmpanm_intern )
        return 0;

    MFILE *mfile = NULL;
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
    __NC_STACK_bmpanim *bmpAnm = &this->stack__bmpanim;

    if ( arg->field_4 == -1 )
    {
        bmpAnim_t2 *t2 = &bmpAnm->t2_[ bmpAnm->field_12 ];

        if ( t2 == bmpAnm->bmpanm_intern->t2_end )
        {
            if ( bmpAnm->field_10 )
            {
                t2--;
                bmpAnm->field_12 = -1;
            }
            else
            {
                t2 = bmpAnm->bmpanm_intern->t2;
            }
        }
        else if ( t2 < bmpAnm->bmpanm_intern->t2 )
        {
            t2++;
            bmpAnm->field_12 = 1;
        }

        bmpAnm->t2_ = t2;
        arg->pbitm = t2->bitm;
        arg->opl2 = t2->opls;
    }
    else
    {
        if ( arg->field_0 != bmpAnm->field_8 )
        {
            bmpAnm->field_8 = arg->field_0;

            bmpAnim_t2 *t2 = bmpAnm->t2_;
            int v8 = arg->field_4 + bmpAnm->field_C;

            while ( v8 - t2->field_8 >= 0 )
            {
                v8 = v8 - t2->field_8;

                t2 += bmpAnm->field_12;

                if ( t2 == bmpAnm->bmpanm_intern->t2_end )
                {
                    if ( bmpAnm->field_10 )
                    {
                        t2--;
                        bmpAnm->field_12 = -1;
                    }
                    else
                    {
                        t2 = bmpAnm->bmpanm_intern->t2;
                    }
                }
                else if ( t2 < bmpAnm->bmpanm_intern->t2 )
                {
                    t2++;
                    bmpAnm->field_12 = 1;
                }
            }

            bmpAnm->field_C = v8;
            bmpAnm->t2_ = t2;
            arg->pbitm = t2->bitm;
            arg->opl2 = t2->opls;
        }
        else
        {
            arg->pbitm = bmpAnm->t2_->bitm;
            arg->opl2 = bmpAnm->t2_->opls;
        }
    }
}


size_t NC_STACK_bmpanim::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (stack_vals *)data );
    case 1:
        return (size_t)func1( (stack_vals *)data );
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
    case 65:
        return rsrc_func65( (rsrc **)data );
    case 66:
        return (size_t)rsrc_func66( (rsrc_func66_arg *)data );
    case 130:
        bitmap_func130( (bitmap_arg130 *)data );
        return 1;
    default:
        break;
    }
    return NC_STACK_bitmap::compatcall(method_id, data);
}
