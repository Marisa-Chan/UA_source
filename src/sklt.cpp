#include "includes.h"
#include "nucleas.h"
#include "utils.h"
#include "engine_input.h"

#include "rsrc.h"

#include "bitmap.h"
#include "raster.h"
#include "display.h"
#include "windd.h"
#include "win3d.h"

#include "skeleton.h"
#include "sklt.h"
#include "base.h"

#include "math.h"
#include "inttypes.h"


stored_functions *classvtbl_get_sklt();
class_return * class_set_sklt(int, ...);

stored_functions sklt_class_vtbl(class_set_sklt);


class_stored sklt_class_off (NULL, NULL, "MC2classes:sklt.class", classvtbl_get_sklt);


stored_functions *classvtbl_get_sklt()
{
    return &sklt_class_vtbl;
}

CLASSFUNC sklt_funcs[1024];


NC_STACK_sklt * sklt_func5(class_stru *clss, class_stru *zis, MFILE **file)
{
    char name[256];
    stack_vals stk[3];

    MFILE *mfile = *file;
    NC_STACK_sklt *obj = NULL;
    int getted = 0;

    while ( 1 )
    {
        int iff_result = read_next_IFF(mfile, 2);
        if ( iff_result == -2 )
            break;

        if ( iff_result )
            return NULL;

        if ( GET_FORM_INFO_OR_NULL(mfile)->TAG == TAG_NAME )
        {
            mfread(mfile, name, 255);
            read_next_IFF(mfile, 2);
            getted = 1;
        }
        else
        {
            read_default(mfile);
        }
    }

    if ( getted )
    {
        stk[0].id = 0x80001000;
        stk[0].value = (size_t)name;
        stk[1].id = 0x80001001;
        stk[1].value = 1;
        stk[2].id = 0;
        stk[2].value = 0;

        obj = (NC_STACK_sklt *)call_parent(zis, clss, 0, stk);
    }
    return obj;
}

size_t sklt_func6(NC_STACK_sklt *obj, class_stru *, MFILE **file)
{
    MFILE *mfile = *file;

    if ( sub_412FC0(mfile, TAG_SKLC, TAG_FORM, -1) )
        return 0;
    else
    {
        char name[256];

        sub_412FC0(mfile, 0, TAG_NAME, -1);
        call_vtbl(obj, 3, 0x80001000, name, 0);
        sub_413564(mfile, strlen(name) + 1, name);
        sub_413290(mfile);
        return sub_413290(mfile) == 0;
    }
}

rsrc * skeleton_read_pooX(NC_STACK_sklt *obj, class_stru *zis, stack_vals *stak, MFILE *mfile, int version)
{
    MFILE_S1 *chunk = GET_FORM_INFO_OR_NULL(mfile);

    if ( version < 1 )
        return NULL;

    int num;

    if ( version == 1 )
    {
        num = chunk->TAG_SIZE / 6;
    }
    else if (version == 2)
    {
        num = chunk->TAG_SIZE / 12;
    }
    else
        return NULL;

    stack_vals stk[2];

    stk[0].id = 0x80002001;
    stk[0].value = num;
    stk[1].id = 2;
    stk[1].value = (size_t)stak;

    rsrc *res = (rsrc *)call_parent(zis, obj, 64, stk);
    if ( res )
    {
        skeleton_64_stru *sklt = (skeleton_64_stru *)res->data;

        if ( version == 1 )                      // POOL
        {
            for (int i = 0; i < num; i++)
            {
                int16_t tmp[3];

                mfread(mfile, tmp, 2 * 3);
                tmp[0] = SWAP16(tmp[0]);
                tmp[1] = SWAP16(tmp[1]);
                tmp[2] = SWAP16(tmp[2]);

                sklt->POO[i].field_0 = 0;
                sklt->POO[i].pos3f.sx = tmp[0];
                sklt->POO[i].pos3f.sy = tmp[1];
                sklt->POO[i].pos3f.sz = tmp[2];
            }
        }
        else if ( version == 2 )                  // POO2
        {
            for (int i = 0; i < num; i++)
            {
                uint32_t tmp[3];

                mfread(mfile, tmp, 4 * 3);
                tmp[0] = SWAP32(tmp[0]);
                tmp[1] = SWAP32(tmp[1]);
                tmp[2] = SWAP32(tmp[2]);

                sklt->POO[i].field_0 = 0;
                sklt->POO[i].pos3f.sx = *(float *)&tmp[0];
                sklt->POO[i].pos3f.sy = *(float *)&tmp[1];
                sklt->POO[i].pos3f.sz = *(float *)&tmp[2];
            }
        }
    }
    return res;
}

int skeleton_read_poly(NC_STACK_sklt *obj, MFILE *mfile, skeleton_64_stru *sklt)
{
    MFILE_S1 *chunk = GET_FORM_INFO_OR_NULL(mfile);
    int16_t *polys = (int16_t *)AllocVec(chunk->TAG_SIZE, 1);

    if ( polys )
    {
        int v9 = 0;
        mfread(mfile, polys, chunk->TAG_SIZE);

        for (int i = 0; i < chunk->TAG_SIZE / 2 ; i++)
        {
            polys[i] = SWAP16(polys[i]);
            if (polys[i] == -1)
                v9++;
        }

        skeleton_130_arg arg130;

        arg130.pol_count = v9;
        arg130.skeleton = sklt;
        arg130.num_indexes = (chunk->TAG_SIZE / 2) - v9;

        if ( call_method(obj, 130, &arg130) )
        {
            int16_t *v14 = polys;
            pol_indixes *v15 = sklt->pol_entries[0];

            for (int i = 0; i < v9; i++)
            {
                int vert_count = 0;

                sklt->pol_entries[i] = v15;

                while( *v14 != -1)
                {
                    int16_t *vtxs = &v15->v1;
                    vtxs[vert_count] = *v14;

                    vert_count++;
                    v14++;
                }
                v14++;
                v15->num_vertices = vert_count;

                int16_t *vtx = &v15->v1;
                v15 = (pol_indixes *)&vtx[vert_count];
            }
            nc_FreeMem(polys);
            return 1;
        }
        else
        {
            nc_FreeMem(polys);
            return 0;
        }
    }
    return 0;
}

int skeleton_read_pol2(NC_STACK_sklt *obj, MFILE *mfile, skeleton_64_stru *sklt)
{
    MFILE_S1 *chunk = GET_FORM_INFO_OR_NULL(mfile);
    int pol_count = 0;

    mfread(mfile, &pol_count, 4);

    pol_count = SWAP32(pol_count);

    skeleton_130_arg arg130;
    arg130.skeleton = sklt;
    arg130.pol_count = pol_count;
    arg130.num_indexes = ((chunk->TAG_SIZE - 4) / 2) - pol_count;

    if ( call_method(obj, 130, &arg130) )
    {
        pol_indixes *indixes = sklt->pol_entries[0];
        mfread(mfile, sklt->pol_entries[0], chunk->TAG_SIZE - 4);

        for (int i = 0; i < pol_count; i++)
        {
            sklt->pol_entries[i] = indixes;

            indixes->num_vertices = SWAP16(indixes->num_vertices);

            int16_t *vtx = &indixes->v1;

            for(int j = 0; j < indixes->num_vertices; j++)
                vtx[j] = SWAP16(vtx[j]);

            indixes = (pol_indixes *)&vtx[indixes->num_vertices];
        }
        return 1;
    }
    return 0;
}

int skeleton_read_senX(NC_STACK_sklt *obj, MFILE *mfile, skeleton_64_stru *sklt, int version)
{
    MFILE_S1 *chunk = GET_FORM_INFO_OR_NULL(mfile);
    int sen_count;

    if ( version < 1 )
        return 0;

    if ( version == 1 )
    {
        sen_count = chunk->TAG_SIZE / 6;
    }
    else if (version == 2)
    {
        sen_count = chunk->TAG_SIZE / 12;
    }
    else
        return 0;

    skeleton_129_arg arg129;
    arg129.skeleton = sklt;
    arg129.sen_count = sen_count;

    if ( call_method(obj, 129, &arg129) )
    {
        if ( version == 1 )
        {
            for (int i = 0; i < sen_count; i++)
            {
                int16_t tmp[3];

                mfread(mfile, tmp, 2 * 3);
                tmp[0] = SWAP16(tmp[0]);
                tmp[1] = SWAP16(tmp[1]);
                tmp[2] = SWAP16(tmp[2]);

                sklt->sen_entries[i].field_0 = 0;
                sklt->sen_entries[i].pos3f.sx = tmp[0];
                sklt->sen_entries[i].pos3f.sy = tmp[1];
                sklt->sen_entries[i].pos3f.sz = tmp[2];
            }
        }
        else if ( version == 2 )
        {
            for (int i = 0; i < sen_count; i++)
            {
                uint32_t tmp[3];

                mfread(mfile, tmp, 4 * 3);
                tmp[0] = SWAP32(tmp[0]);
                tmp[1] = SWAP32(tmp[1]);
                tmp[2] = SWAP32(tmp[2]);

                sklt->sen_entries[i].field_0 = 0;
                sklt->sen_entries[i].pos3f.sx = *(float *)&tmp[0];
                sklt->sen_entries[i].pos3f.sy = *(float *)&tmp[1];
                sklt->sen_entries[i].pos3f.sz = *(float *)&tmp[2];
            }
        }
        return 1;
    }
    return 0;
}

void sklt_func64__sub0__sub0(skeleton_64_stru *sklt, int id)
{
    if ( sklt )
    {
        pol_indixes *pol = sklt->pol_entries[id];
        if ( pol->num_vertices >= 3 )
        {
            skeleton_type1 *POO = sklt->POO;

            float dx1 = POO[pol->v2].pos3f.sx - POO[pol->v1].pos3f.sx;
            float dy1 = POO[pol->v2].pos3f.sy - POO[pol->v1].pos3f.sy;
            float dz1 = POO[pol->v2].pos3f.sz - POO[pol->v1].pos3f.sz;


            float dx2 = POO[pol->v3].pos3f.sx - POO[pol->v2].pos3f.sx;
            float dy2 = POO[pol->v3].pos3f.sy - POO[pol->v2].pos3f.sy;
            float dz2 = POO[pol->v3].pos3f.sz - POO[pol->v2].pos3f.sz;

            float zx = dz1 * dx2 - dx1 * dz2;
            float zy = dy1 * dz2 - dy2 * dz1;
            float xy = dx1 * dy2 - dx2 * dy1;

            float sqwr = sqrt(zx * zx + zy * zy + xy * xy);

            float dzx = 0.0;
            float dzy = 0.0;
            float dxy = 0.0;

            if ( sqwr != 0.0 )
            {
                float param = 1.0 / sqwr;
                dzy = zy * param;
                dzx = zx * param;
                dxy = xy * param;
            }

            sklt->triangles[id].field_0 = dzy;
            sklt->triangles[id].field_4 = dzx;
            sklt->triangles[id].field_8 = dxy;
            sklt->triangles[id].field_C = -(dzy * POO[pol->v1].pos3f.sx + dzx * POO[pol->v1].pos3f.sy + dxy * POO[pol->v1].pos3f.sz);
        }
        else
        {
            sklt->triangles[id].field_0 = 0;
            sklt->triangles[id].field_4 = 0;
            sklt->triangles[id].field_8 = 0;
            sklt->triangles[id].field_C = 0;
        }
    }
}

rsrc * sklt_func64__sub0(NC_STACK_sklt *obj, class_stru *zis, stack_vals *stak, MFILE *mfile)
{
    skeleton_64_stru *sklt = NULL;
    rsrc *res = NULL;
    int read_ok = 1;

    while ( 1 )
    {
        int iff_flag = read_next_IFF(mfile, 2);

        if ( iff_flag == -2 )
            break;

        if ( iff_flag )
        {
            if ( res )
            {
                //call_vtbl(obj, 65, res);
                call_method(obj, 65, &res);
            }

            return NULL;
        }

        int tag = GET_FORM_INFO_OR_NULL(mfile)->TAG;
        if ( tag != TAG_FORM )
        {
            switch ( tag )
            {
            case TAG_TYPE:
                read_next_IFF(mfile, 2);
                break;

            case TAG_POOL:
                res = skeleton_read_pooX(obj, zis, stak, mfile, 1);
                if ( res )
                {
                    sklt = (skeleton_64_stru *)res->data;
                    if ( !sklt )
                        read_ok = 0;
                }
                else
                {
                    read_ok = 0;
                }
                read_next_IFF(mfile, 2);
                break;

            case TAG_POO2:
                res = skeleton_read_pooX(obj, zis, stak, mfile, 2);
                if ( res )
                {
                    sklt = (skeleton_64_stru *)res->data;
                    if ( !sklt )
                        read_ok = 0;
                }
                else
                {
                    read_ok = 0;
                }
                read_next_IFF(mfile, 2);
                break;

            case TAG_POLY:
                if ( sklt )
                {
                    if ( !skeleton_read_poly(obj, mfile, sklt) )
                        read_ok = 0;
                }
                read_next_IFF(mfile, 2);
                break;

            case TAG_POL2:
                if ( sklt )
                {
                    if ( !skeleton_read_pol2(obj, mfile, sklt) )
                        read_ok = 0;
                }
                read_next_IFF(mfile, 2);
                break;

            case TAG_SENS:
                if ( sklt )
                {
                    if ( !skeleton_read_senX(obj, mfile, sklt, 1) )
                        read_ok = 0;
                }
                read_next_IFF(mfile, 2);
                break;

            case TAG_SEN2:
                if ( sklt )
                {
                    if ( !skeleton_read_senX(obj, mfile, sklt, 2) )
                        read_ok = 0;
                }
                read_next_IFF(mfile, 2);
                break;

            default:
                read_default(mfile);
                break;
            }
        }
    }

    if ( !read_ok || !sklt )
    {
        if ( res )
        {
            //call_vtbl(obj, 65, res);
            call_method(obj, 65, &res);
        }

        return NULL;
    }

    for (int i = 0; i < sklt->pol_count; i++)
        sklt_func64__sub0__sub0(sklt, i);

    return res;
}

// Create sklt resource node and fill rsrc field data
rsrc * sklt_func64(NC_STACK_sklt *obj, class_stru *zis, stack_vals *stak)
{
    const char *filename = (const char *)find_id_in_stack_def_val(0x80001000, 0, stak);
    if ( filename )
    {
        MFILE *mfile = (MFILE *)find_id_in_stack_def_val(0x80001005, 0, stak);

        int selfopened = 0;

        if ( !mfile )
        {
            mfile = open_mfile(filename, 0);

            if ( !mfile )
                return NULL;

            selfopened = 1;
        }

        rsrc *res = sklt_func64__sub0(obj, zis, stak, mfile);

        if ( selfopened )
            close_mfile(mfile);

        return res;
    }
    else
        return NULL;
}

size_t sklt_func66(NC_STACK_sklt *obj, class_stru *, rsrc_func66_arg *arg)
{
    skeleton_64_stru *sklt = NULL;

    call_vtbl(obj, 3, 0x80001002, &sklt, 0);

    if ( !sklt )
        return 0;

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

    if ( sub_412FC0(mfile, TAG_SKLT, TAG_FORM, -1) )
        return 0;

    if ( sklt->POO )
    {
        sub_412FC0(mfile, 0, TAG_POO2, 3 * 4 * sklt->POO_NUM);
        for (int i = 0; i < sklt->POO_NUM; i++)
        {
            uint32_t tmp[3];
            tmp[0] = *(uint32_t *)&sklt->POO[i].pos3f.sx;
            tmp[1] = *(uint32_t *)&sklt->POO[i].pos3f.sy;
            tmp[2] = *(uint32_t *)&sklt->POO[i].pos3f.sz;

            tmp[0] = SWAP32(tmp[0]);
            tmp[1] = SWAP32(tmp[1]);
            tmp[2] = SWAP32(tmp[2]);

            sub_413564(mfile, 3 * 4, tmp);
        }
        sub_413290(mfile);
    }

    if ( sklt->sen_entries )
    {
        sub_412FC0(mfile, 0, TAG_SEN2, 3 * 4 * sklt->sen_count);

        for (int i = 0; i < sklt->sen_count; i++)
        {
            uint32_t tmp[3];
            tmp[0] = *(uint32_t *)&sklt->sen_entries[i].pos3f.sx;
            tmp[1] = *(uint32_t *)&sklt->sen_entries[i].pos3f.sy;
            tmp[2] = *(uint32_t *)&sklt->sen_entries[i].pos3f.sz;

            tmp[0] = SWAP32(tmp[0]);
            tmp[1] = SWAP32(tmp[1]);
            tmp[2] = SWAP32(tmp[2]);

            sub_413564(mfile, 3 * 4, tmp);
        }
        sub_413290(mfile);
    }

    if ( sklt->pol_entries )
    {
        sub_412FC0(mfile, 0, TAG_POL2, -1);

        int32_t tmp = SWAP32(sklt->pol_count);
        sub_413564(mfile, 4, &tmp);

        for (int i = 0; i < sklt->pol_count; i++)
        {
            int16_t vtmp[64];

            int16_t *vtx = &sklt->pol_entries[i]->v1;

            vtmp[0] = SWAP16(sklt->pol_entries[i]->num_vertices);

            for (int j = 0; j < sklt->pol_entries[i]->num_vertices; j++)
                vtmp[1 + j] = SWAP16(vtx[j]);

            sub_413564(mfile, 2 * sklt->pol_entries[i]->num_vertices + 2, vtmp);
        }
        sub_413290(mfile);
    }

    if ( sub_413290(mfile) )
        return 0;

    if ( arg->OpenedStream == 1 )
        close_mfile(mfile);
    return arg->OpenedStream;
}


class_return sklt_class_descr;

class_return * class_set_sklt(int, ...)
{
    memset(sklt_funcs, 0, sizeof(CLASSFUNC) * 1024);

    sklt_funcs[5] = (CLASSFUNC)sklt_func5;
    sklt_funcs[6] = (CLASSFUNC)sklt_func6;
    sklt_funcs[64] = (CLASSFUNC)sklt_func64;
    sklt_funcs[66] = (CLASSFUNC)sklt_func66;

    sklt_class_descr.parent = "skeleton.class";

    sklt_class_descr.vtbl = sklt_funcs;
    ////sklt_class_descr.varSize = sizeof(__NC_STACK_sklt);
    sklt_class_descr.varSize = sizeof(NC_STACK_sklt) - offsetof(NC_STACK_sklt, stack__sklt); //// HACK
    sklt_class_descr.field_A = 0;
    return &sklt_class_descr;
}
