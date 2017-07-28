#include "includes.h"
#include "nucleas.h"
#include "utils.h"
#include "engine_input.h"

#include "rsrc.h"

#include "bitmap.h"
#include "display.h"
#include "win3d.h"

#include "skeleton.h"
#include "sklt.h"
#include "base.h"

#include "math.h"
#include "inttypes.h"



const NewClassDescr NC_STACK_sklt::description("sklt.class", &newinstance);



size_t NC_STACK_sklt::func5(IFFile **file)
{
    char name[256];
    stack_vals stk[3];

    IFFile *mfile = *file;
    int getted = 0;

    while ( 1 )
    {
        int iff_result = mfile->parse();
        if ( iff_result == -2 )
            break;

        if ( iff_result )
            return 0;

        if ( mfile->getCurrentChunk()->TAG == TAG_NAME )
        {
            mfile->read(name, 255);
            mfile->parse();
            getted = 1;
        }
        else
        {
            mfile->skipChunk();
        }
    }

    if ( !getted )
        return 0;

    stk[0].set(RSRC_ATT_NAME, name);
    stk[1].set(RSRC_ATT_TRYSHARED, 1);
    stk[2].end();

    if ( !NC_STACK_skeleton::func0(stk) )
        return 0;

    return 1;
}

size_t NC_STACK_sklt::func6(IFFile **file)
{
    IFFile *mfile = *file;

    if ( mfile->pushChunk(TAG_SKLC, TAG_FORM, -1) )
        return 0;
    else
    {
        mfile->pushChunk(0, TAG_NAME, -1);
        const char *name = getRsrc_name();
        mfile->write(name, strlen(name) + 1);
        mfile->popChunk();
        return mfile->popChunk() == IFFile::IFF_ERR_OK;
    }
}

rsrc * skeleton_read_pooX(NC_STACK_sklt *obj, stack_vals *stak, IFFile *mfile, int version)
{
    IFFile::Context *chunk = mfile->getCurrentChunk();

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

    stk[0].set(NC_STACK_skeleton::SKEL_ATT_POINTSCNT, num);
    stk[1].nextStack(stak);

    rsrc *res = obj->NC_STACK_skeleton::rsrc_func64(stk);
    if ( res )
    {
        skeleton_64_stru *sklt = (skeleton_64_stru *)res->data;

        if ( version == 1 )                      // POOL
        {
            for (int i = 0; i < num; i++)
            {
                int16_t tmp[3];
                mfile->readS16B(tmp[0]);
                mfile->readS16B(tmp[1]);
                mfile->readS16B(tmp[2]);

                sklt->POO[i].flags = 0;
                sklt->POO[i].sx = tmp[0];
                sklt->POO[i].sy = tmp[1];
                sklt->POO[i].sz = tmp[2];
            }
        }
        else if ( version == 2 )                  // POO2
        {
            for (int i = 0; i < num; i++)
            {
                sklt->POO[i].flags = 0;
                mfile->readFloatB(sklt->POO[i].sx);
                mfile->readFloatB(sklt->POO[i].sy);
                mfile->readFloatB(sklt->POO[i].sz);
            }
        }
    }
    return res;
}

int skeleton_read_poly(NC_STACK_sklt *obj, IFFile *mfile, skeleton_64_stru *sklt)
{
    IFFile::Context *chunk = mfile->getCurrentChunk();
    int32_t entrycnt = chunk->TAG_SIZE / 2;
    int16_t *entries = new int16_t[entrycnt];

    if ( !entries )
        return 0;

    int polyCount = 0;

    for (int i = 0; i < entrycnt ; i++)
    {
        mfile->readS16B(entries[i]);
        if (entries[i] == -1) // Meet end of vertex indexes for this polygon
            polyCount++;
    }

    skeleton_130_arg arg130;

    arg130.pol_count = polyCount;
    arg130.skeleton = sklt;
    arg130.num_indexes = entrycnt - polyCount;

    if ( !obj->skeleton_func130(&arg130) )
    {
        delete[] entries;
        return 0;
    }

    int16_t *v14 = entries;

    for (int i = 0; i < polyCount; i++)
    {
        int vert_count = 0;

        while( *v14 != -1)
        {
            sklt->polygons[i].v[vert_count] = *v14;
            vert_count++;
            v14++;
        }
        v14++;

        sklt->polygons[i].num_vertices = vert_count;
    }
    delete[] entries;
    return 1;
}

int skeleton_read_pol2(NC_STACK_sklt *obj, IFFile *mfile, skeleton_64_stru *sklt)
{
    IFFile::Context *chunk = mfile->getCurrentChunk();
    int pol_count = 0;

    mfile->readS32B(pol_count);

    skeleton_130_arg arg130;
    arg130.skeleton = sklt;
    arg130.pol_count = pol_count;
    arg130.num_indexes = ((chunk->TAG_SIZE - 4) / 2) - pol_count;

    if ( !obj->skeleton_func130(&arg130) )
        return 0;

    for (int i = 0; i < pol_count; i++)
    {
        uint16_t numVertex;
        mfile->readU16B(numVertex);

        sklt->polygons[i].num_vertices = numVertex;
        for(int j = 0; j < numVertex; j++)
        {
            uint16_t v;
            mfile->readU16B(v);
            sklt->polygons[i].v[j] = v;
        }
    }
    return 1;
}

int skeleton_read_senX(NC_STACK_sklt *obj, IFFile *mfile, skeleton_64_stru *sklt, int version)
{
    IFFile::Context *chunk = mfile->getCurrentChunk();
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

    if ( obj->skeleton_func129(&arg129) )
    {
        if ( version == 1 )
        {
            for (int i = 0; i < sen_count; i++)
            {
                int16_t tmp[3];
                mfile->readS16B(tmp[0]);
                mfile->readS16B(tmp[1]);
                mfile->readS16B(tmp[2]);

                sklt->SEN[i].flags = 0;
                sklt->SEN[i].sx = tmp[0];
                sklt->SEN[i].sy = tmp[1];
                sklt->SEN[i].sz = tmp[2];
            }
        }
        else if ( version == 2 )
        {
            for (int i = 0; i < sen_count; i++)
            {
                sklt->SEN[i].flags = 0;
                mfile->readFloatB(sklt->SEN[i].sx);
                mfile->readFloatB(sklt->SEN[i].sy);
                mfile->readFloatB(sklt->SEN[i].sz);
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
        Polygon *pol = &sklt->polygons[id];
        if ( pol->num_vertices >= 3 )
        {
            Vertex *POO = sklt->POO;

            float dx1 = POO[pol->v[1]].sx - POO[pol->v[0]].sx;
            float dy1 = POO[pol->v[1]].sy - POO[pol->v[0]].sy;
            float dz1 = POO[pol->v[1]].sz - POO[pol->v[0]].sz;


            float dx2 = POO[pol->v[2]].sx - POO[pol->v[1]].sx;
            float dy2 = POO[pol->v[2]].sy - POO[pol->v[1]].sy;
            float dz2 = POO[pol->v[2]].sz - POO[pol->v[1]].sz;

            float zx = dz1 * dx2 - dx1 * dz2;
            float zy = dy1 * dz2 - dy2 * dz1;
            float xy = dx1 * dy2 - dx2 * dy1;

            float sqwr = sqrt(zx * zx + zy * zy + xy * xy);

            float dzx = 0.0;
            float dzy = 0.0;
            float dxy = 0.0;

            if ( sqwr != 0.0 )
            {
                dzy = zy / sqwr;
                dzx = zx / sqwr;
                dxy = xy / sqwr;
            }

            sklt->polygons[id].A = dzy;
            sklt->polygons[id].B = dzx;
            sklt->polygons[id].C = dxy;
            sklt->polygons[id].D = -(dzy * POO[pol->v[0]].sx + dzx * POO[pol->v[0]].sy + dxy * POO[pol->v[0]].sz);
        }
        else
        {
            sklt->polygons[id].A = 0;
            sklt->polygons[id].B = 0;
            sklt->polygons[id].C = 0;
            sklt->polygons[id].D = 0;
        }
    }
}

rsrc * sklt_func64__sub0(NC_STACK_sklt *obj, stack_vals *stak, IFFile *mfile)
{
    skeleton_64_stru *sklt = NULL;
    rsrc *res = NULL;
    int read_ok = 1;

    while ( 1 )
    {
        int iff_flag = mfile->parse();

        if ( iff_flag == -2 )
            break;

        if ( iff_flag )
        {
            if ( res )
                obj->rsrc_func65(&res);

            return NULL;
        }

        int tag = mfile->getCurrentChunk()->TAG;
        if ( tag != TAG_FORM )
        {
            switch ( tag )
            {
            case TAG_TYPE:
                mfile->parse();
                break;

            case TAG_POOL:
                res = skeleton_read_pooX(obj, stak, mfile, 1);
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
                mfile->parse();
                break;

            case TAG_POO2:
                res = skeleton_read_pooX(obj, stak, mfile, 2);
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
                mfile->parse();
                break;

            case TAG_POLY:
                if ( sklt )
                {
                    if ( !skeleton_read_poly(obj, mfile, sklt) )
                        read_ok = 0;
                }
                mfile->parse();
                break;

            case TAG_POL2:
                if ( sklt )
                {
                    if ( !skeleton_read_pol2(obj, mfile, sklt) )
                        read_ok = 0;
                }
                mfile->parse();
                break;

            case TAG_SENS:
                if ( sklt )
                {
                    if ( !skeleton_read_senX(obj, mfile, sklt, 1) )
                        read_ok = 0;
                }
                mfile->parse();
                break;

            case TAG_SEN2:
                if ( sklt )
                {
                    if ( !skeleton_read_senX(obj, mfile, sklt, 2) )
                        read_ok = 0;
                }
                mfile->parse();
                break;

            default:
                mfile->skipChunk();
                break;
            }
        }
    }

    if ( !read_ok || !sklt )
    {
        if ( res )
            obj->rsrc_func65(&res);

        return NULL;
    }

    for (int i = 0; i < sklt->polygonsCount; i++)
        sklt_func64__sub0__sub0(sklt, i);

    return res;
}

// Create sklt resource node and fill rsrc field data
rsrc * NC_STACK_sklt::rsrc_func64(stack_vals *stak)
{
    const char *filename = (const char *)find_id_in_stack_def_val(RSRC_ATT_NAME, 0, stak);
    if ( filename )
    {
        IFFile *mfile = (IFFile *)find_id_in_stack_def_val(RSRC_ATT_PIFFFILE, 0, stak);

        int selfopened = 0;

        if ( !mfile )
        {
            mfile = IFFile::openIFFile(filename, false);

            if ( !mfile )
                return NULL;

            selfopened = 1;
        }

        rsrc *res = sklt_func64__sub0(this, stak, mfile);

        if ( selfopened )
            delete mfile;

        return res;
    }
    else
        return NULL;
}

size_t NC_STACK_sklt::rsrc_func66(rsrc_func66_arg *arg)
{
    skeleton_64_stru *sklt = (skeleton_64_stru *)getRsrc_pData();

    if ( !sklt )
        return 0;

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

    if ( mfile->pushChunk(TAG_SKLT, TAG_FORM, -1) )
        return 0;

    if ( sklt->POO )
    {
        mfile->pushChunk(0, TAG_POO2, 12 * sklt->POO_NUM);
        for (int i = 0; i < sklt->POO_NUM; i++)
        {
            mfile->writeFloatB(sklt->POO[i].sx);
            mfile->writeFloatB(sklt->POO[i].sy);
            mfile->writeFloatB(sklt->POO[i].sz);
        }
        mfile->popChunk();
    }

    if ( sklt->SEN )
    {
        mfile->pushChunk(0, TAG_SEN2, 12 * sklt->SEN_NUM);

        for (int i = 0; i < sklt->SEN_NUM; i++)
        {
            mfile->writeFloatB(sklt->SEN[i].sx);
            mfile->writeFloatB(sklt->SEN[i].sy);
            mfile->writeFloatB(sklt->SEN[i].sz);
        }
        mfile->popChunk();
    }

    if ( sklt->polygons )
    {
        mfile->pushChunk(0, TAG_POL2, -1);

        mfile->writeS32B(sklt->polygonsCount);

        for (int i = 0; i < sklt->polygonsCount; i++)
        {
            mfile->writeU16B(sklt->polygons[i].num_vertices);

            for (int j = 0; j < sklt->polygons[i].num_vertices; j++)
                mfile->writeU16B(sklt->polygons[i].v[j]);
        }
        mfile->popChunk();
    }

    if ( mfile->popChunk() )
        return 0;

    if ( arg->OpenedStream == 1 )
        delete mfile;
    return arg->OpenedStream;
}


size_t NC_STACK_sklt::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
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
    return NC_STACK_skeleton::compatcall(method_id, data);
}
