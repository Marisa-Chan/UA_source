#include "includes.h"
#include "nucleas.h"
#include "utils.h"
#include "system/inpt.h"

#include "rsrc.h"

#include "bitmap.h"

#include "skeleton.h"
#include "sklt.h"
#include "base.h"

#include "math.h"
#include "inttypes.h"



const Nucleus::ClassDescr NC_STACK_sklt::description("sklt.class", &newinstance);



size_t NC_STACK_sklt::LoadingFromIFF(IFFile **file)
{
    char name[256];

    IFFile *mfile = *file;
    int getted = 0;

    while ( 1 )
    {
        int iff_result = mfile->parse();
        if ( iff_result == -2 )
            break;

        if ( iff_result )
            return 0;

        if ( mfile->GetCurrentChunk().Is(TAG_NAME) )
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
    
    IDVList stak {
        {RSRC_ATT_NAME, std::string(name)},
        {RSRC_ATT_TRYSHARED, (int32_t)1} };

    if ( !NC_STACK_skeleton::Init(stak) 
        )
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

rsrc * skeleton_read_pooX(NC_STACK_sklt *obj, IDVList &stak, IFFile *mfile, int version)
{
    const IFFile::Context &chunk = mfile->GetCurrentChunk();

    if ( version < 1 )
        return NULL;

    int num;

    if ( version == 1 )
    {
        num = chunk.TAG_SIZE / 6;
    }
    else if (version == 2)
    {
        num = chunk.TAG_SIZE / 12;
    }
    else
        return NULL;

    stak.Add(NC_STACK_skeleton::SKEL_ATT_POINTSCNT, (int32_t)num);

    rsrc *res = obj->NC_STACK_skeleton::rsrc_func64(stak);
    if ( res )
    {
        UAskeleton::Data *sklt = (UAskeleton::Data *)res->data;

        if ( version == 1 )                      // POOL
        {
            for (int i = 0; i < num; i++)
            {
                int16_t tmp[3];
                tmp[0] = mfile->readS16B();
                tmp[1] = mfile->readS16B();
                tmp[2] = mfile->readS16B();

                sklt->POO[i].flags = 0;
                sklt->POO[i].x = tmp[0];
                sklt->POO[i].y = tmp[1];
                sklt->POO[i].z = tmp[2];
            }
        }
        else if ( version == 2 )                  // POO2
        {
            for (int i = 0; i < num; i++)
            {
                sklt->POO[i].flags = 0;
                sklt->POO[i].x = mfile->readFloatB();
                sklt->POO[i].y = mfile->readFloatB();
                sklt->POO[i].z = mfile->readFloatB();
            }
        }
    }
    return res;
}

int skeleton_read_poly(NC_STACK_sklt *obj, IFFile *mfile, UAskeleton::Data *sklt)
{
    int32_t entrycnt = mfile->GetCurrentChunk().TAG_SIZE / 2;
    int16_t *entries = new int16_t[entrycnt];

    if ( !entries )
        return 0;

    int polyCount = 0;

    for (int i = 0; i < entrycnt ; i++)
    {
        entries[i] = mfile->readS16B();
        if (entries[i] == -1) // Meet end of vertex indexes for this polygon
            polyCount++;
    }

    sklt->polygons.resize(polyCount);

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

int skeleton_read_pol2(NC_STACK_sklt *obj, IFFile *mfile, UAskeleton::Data *sklt)
{
    //IFFile::Context *chunk = mfile->GetCurrentChunk();
    int pol_count = 0;

    pol_count = mfile->readS32B();

    sklt->polygons.resize(pol_count);

    for (int i = 0; i < pol_count; i++)
    {
        uint16_t numVertex;
        numVertex = mfile->readU16B();

        sklt->polygons[i].num_vertices = numVertex;
        for(int j = 0; j < numVertex; j++)
        {
            uint16_t v;
            v = mfile->readU16B();
            sklt->polygons[i].v[j] = v;
        }
    }
    return 1;
}

int skeleton_read_senX(NC_STACK_sklt *obj, IFFile *mfile, UAskeleton::Data *sklt, int version)
{
    const IFFile::Context &chunk = mfile->GetCurrentChunk();
    int sen_count;

    if ( version < 1 )
        return 0;

    if ( version == 1 )
    {
        sen_count = chunk.TAG_SIZE / 6;
    }
    else if (version == 2)
    {
        sen_count = chunk.TAG_SIZE / 12;
    }
    else
        return 0;

    sklt->SEN.resize(sen_count);
    
    if ( version == 1 )
    {
        for (int i = 0; i < sen_count; i++)
        {
            int16_t tmp[3];
            tmp[0] = mfile->readS16B();
            tmp[1] = mfile->readS16B();
            tmp[2] = mfile->readS16B();

            sklt->SEN[i].flags = 0;
            sklt->SEN[i].x = tmp[0];
            sklt->SEN[i].y = tmp[1];
            sklt->SEN[i].z = tmp[2];
        }
    }
    else if ( version == 2 )
    {
        for (int i = 0; i < sen_count; i++)
        {
            sklt->SEN[i].flags = 0;
            sklt->SEN[i].x = mfile->readFloatB();
            sklt->SEN[i].y = mfile->readFloatB();
            sklt->SEN[i].z = mfile->readFloatB();
        }
    }
    return 1;
}

void sklt_func64__sub0__sub0(UAskeleton::Data *sklt, int id)
{
    if ( sklt )
    {
        UAskeleton::Polygon *pol = &sklt->polygons[id];
        if ( pol->num_vertices >= 3 )
        {
            std::vector<UAskeleton::Vertex> &POO = sklt->POO;

            float dx1 = POO[pol->v[1]].x - POO[pol->v[0]].x;
            float dy1 = POO[pol->v[1]].y - POO[pol->v[0]].y;
            float dz1 = POO[pol->v[1]].z - POO[pol->v[0]].z;


            float dx2 = POO[pol->v[2]].x - POO[pol->v[1]].x;
            float dy2 = POO[pol->v[2]].y - POO[pol->v[1]].y;
            float dz2 = POO[pol->v[2]].z - POO[pol->v[1]].z;

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
            sklt->polygons[id].D = -(dzy * POO[pol->v[0]].x + dzx * POO[pol->v[0]].y + dxy * POO[pol->v[0]].z);
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

rsrc * sklt_func64__sub0(NC_STACK_sklt *obj, IDVList &stak, IFFile *mfile)
{
    UAskeleton::Data *sklt = NULL;
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
                obj->rsrc_func65(res);

            return NULL;
        }

        int tag = mfile->GetCurrentChunk().TAG;
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
                    sklt = (UAskeleton::Data *)res->data;
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
                    sklt = (UAskeleton::Data *)res->data;
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
            obj->rsrc_func65(res);

        return NULL;
    }

    for (size_t i = 0; i < sklt->polygons.size(); i++)
        sklt_func64__sub0__sub0(sklt, i);

    return res;
}

// Create sklt resource node and fill rsrc field data
rsrc * NC_STACK_sklt::rsrc_func64(IDVList &stak)
{
    const std::string filename = stak.Get<std::string>(RSRC_ATT_NAME, "");
    if ( !filename.empty() )
    {
        IFFile *mfile = stak.Get<IFFile *>(RSRC_ATT_PIFFFILE, NULL);

        int selfopened = 0;

        if ( !mfile )
        {
            mfile = IFFile::RsrcOpenIFFile(filename, "rb");

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
    UAskeleton::Data *sklt = (UAskeleton::Data *)getRsrc_pData();

    if ( !sklt )
        return 0;

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

    if ( mfile->pushChunk(TAG_SKLT, TAG_FORM, -1) )
        return 0;

    if ( !sklt->POO.empty() )
    {
        mfile->pushChunk(0, TAG_POO2, 12 * sklt->POO.size());
        for (size_t i = 0; i < sklt->POO.size(); i++)
        {
            mfile->writeFloatB(sklt->POO[i].x);
            mfile->writeFloatB(sklt->POO[i].y);
            mfile->writeFloatB(sklt->POO[i].z);
        }
        mfile->popChunk();
    }

    if ( !sklt->SEN.empty() )
    {
        mfile->pushChunk(0, TAG_SEN2, 12 * sklt->SEN.size());

        for (size_t i = 0; i < sklt->SEN.size(); i++)
        {
            mfile->writeFloatB(sklt->SEN[i].x);
            mfile->writeFloatB(sklt->SEN[i].y);
            mfile->writeFloatB(sklt->SEN[i].z);
        }
        mfile->popChunk();
    }

    if ( !sklt->polygons.empty() )
    {
        mfile->pushChunk(0, TAG_POL2, -1);

        mfile->writeS32B(sklt->polygons.size());

        for (size_t i = 0; i < sklt->polygons.size(); i++)
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

