#include "includes.h"
#include "nucleas.h"
#include "utils.h"
#include "system/inpt.h"

#include "rsrc.h"

#include "bitmap.h"

#include "skeleton.h"
#include "base.h"


#include <math.h>
#include <float.h>


const Nucleus::ClassDescr NC_STACK_skeleton::description("skeleton.class", &newinstance);

size_t NC_STACK_skeleton::Init(IDVList &stak)
{
    if ( !NC_STACK_rsrc::Init(stak) )
        return 0;

    _resData = (UAskeleton::Data *)getRsrc_pData();

    return 1;
}

// Create skeleton resource node and fill rsrc field data
rsrc * NC_STACK_skeleton::rsrc_func64(IDVList &stak)
{
    rsrc *res = NC_STACK_rsrc::rsrc_func64(stak);

    if ( !res )
        return NULL;

    int elm_num = stak.Get<int32_t>(SKEL_ATT_POINTSCNT, 0);

    if (!elm_num)
    {
        rsrc_func65(res);
        return NULL;
    }

    UAskeleton::Data *sklt = new UAskeleton::Data;

    if (!sklt)
    {
        rsrc_func65(res);
        return NULL;
    }

    res->data = sklt;

    
    sklt->POO.resize(elm_num);
    sklt->tformedVertex.resize(elm_num);

    int sen_count = stak.Get<int32_t>(SKEL_ATT_SENCNT, 0);

    if (sen_count > 0)
        sklt->SEN.resize(sen_count);

    int pol_count = stak.Get<int32_t>(SKEL_ATT_POLYCNT, 0);
    int num_indexes = stak.Get<int32_t>(SKEL_ATT_POLYPNTCNT, 0);

    if (pol_count > 0)
    {
        if (num_indexes <= 0)
        {
            rsrc_func65(res);
            return NULL;
        }

        sklt->polygons.resize(pol_count);
    }

    return res;
}

size_t NC_STACK_skeleton::rsrc_func65(rsrc *res)
{
    UAskeleton::Data *sklt = (UAskeleton::Data *)res->data;

    if ( sklt )
    {
        delete sklt;

        res->data = NULL;
    }
    return NC_STACK_rsrc::rsrc_func65(res);
}

size_t NC_STACK_skeleton::skeleton_func131(int *arg)
{
    int vtxid = *arg;

    if ( _resData )
    {
        UAskeleton::Polygon *pol = &_resData->polygons[vtxid];
        if ( pol->num_vertices >= 3 )
        {
            std::vector<UAskeleton::Vertex> &POO = _resData->POO;

            vec3d vec1 = POO[ pol->v[1] ] - POO[ pol->v[0] ];
            vec3d vec2 = POO[ pol->v[2] ] - POO[ pol->v[1] ];
            vec3d norm = vec1 * vec2;
            norm.normalise();

            _resData->polygons[vtxid].A = norm.x;
            _resData->polygons[vtxid].B = norm.y;
            _resData->polygons[vtxid].C = norm.z;
            _resData->polygons[vtxid].D = -(norm.x * POO[ pol->v[0] ].x +
                                        norm.y * POO[ pol->v[0] ].y +
                                        norm.z * POO[ pol->v[0] ].z);
        }
        else
        {
            _resData->polygons[vtxid].A = 0;
            _resData->polygons[vtxid].B = 0;
            _resData->polygons[vtxid].C = 0;
            _resData->polygons[vtxid].D = 0;
        }
    }

    return 1;
}


bool NC_STACK_skeleton::TransformVertexes(skeleton_arg_132 *arg, const std::vector<UAskeleton::Vertex> &in, std::vector<UAskeleton::Vertex> *out)
{
    if ( in.empty() )
        return false;
    
    if ( in.size() != out->size() )
        out->resize( in.size() );

    uint32_t andFlags = ~0;

    for(size_t i = 0; i < in.size(); ++i)
    {
        vec3d fv = arg->tform.Transform( in[i] );

        int clipFlags = 0;

        if ( fv.z < arg->minZ )
            clipFlags = UAskeleton::Vertex::CLIP_NEAR;
        else if ( fv.z > arg->maxZ )
            clipFlags = UAskeleton::Vertex::CLIP_FAR;
        else
        {
            if ( fv.x > fv.z * 1.1 )
                clipFlags |= UAskeleton::Vertex::CLIP_RIGHT;
            else if ( fv.x < -fv.z * 1.1 )
                clipFlags |= UAskeleton::Vertex::CLIP_LEFT;

            if ( fv.y > fv.z * 1.1 )
                clipFlags |= UAskeleton::Vertex::CLIP_TOP;
            else if ( fv.y < -fv.z * 1.1 )
                clipFlags |= UAskeleton::Vertex::CLIP_BOTTOM;
        }

        andFlags &= clipFlags;

        out->at(i) = UAskeleton::Vertex(fv, clipFlags);
    }

    return andFlags == 0;
}

size_t NC_STACK_skeleton::skeleton_func132(skeleton_arg_132 *arg)
{
    int result = 1;

    if ( !_resData->SEN.empty() )
    {
        if ( _resData->SEN.size() < (_resData->POO.size() / 4) )
            result = TransformVertexes(arg, _resData->SEN, &_resData->tformedVertex);
    }

    if ( result )
        result = TransformVertexes(arg, _resData->POO, &_resData->tformedVertex);

    return result;
}

bool NC_STACK_skeleton::PolygonCheckInvisible(const std::vector<UAskeleton::Vertex> &in, const UAskeleton::Polygon &pol, std::vector<vec3d> *out)
{
    uint16_t andFlags = ~0;

    out->resize(pol.num_vertices);
    for (int16_t i = 0; i < pol.num_vertices; i++)
    {
        out->at(i) = in[ pol.v[i] ];
        andFlags &= in[ pol.v[i] ].flags;
    }

    if ( pol.num_vertices > 2 && !andFlags )
    {
        vec3d vec1 = out->at(1) - out->at(0);
        vec3d vec2 = out->at(2) - out->at(1);
        vec3d norm = vec1 * vec2;

        if ( norm.dot(out->at(0)) < 0.0 )
            return false;
    }

    return andFlags == 0;
}


bool NC_STACK_skeleton::skeleton_func133(skeleton_arg133 *arg)
{
    std::vector<vec3d> PolyVertex;
    UAskeleton::Polygon &pol = _resData->polygons[arg->polyID];

    if ( !PolygonCheckInvisible(_resData->tformedVertex, pol, &PolyVertex) )
        return false;

    arg->rndrArg->vertexCount = pol.num_vertices;
    int vtxCnt = pol.num_vertices;

    for (int i = 0; i < vtxCnt; i++ )
        arg->rndrArg->vertexes[i] = PolyVertex[i];

    if ( arg->field_4 & 1 )
    {
        if (arg->texCoords)
        {
            for (int i = 0; i < vtxCnt; i++)
                arg->rndrArg->tu_tv[i] = arg->texCoords[i];
        }
    }

    if ( arg->field_4 & 2 )
    {
        if ( arg->field_4 & 4 )
        {
            for (int i = 0; i < vtxCnt; i++ )
            {
                float tsq = (PolyVertex[i].length() - arg->fadeStart) / arg->fadeLength;

                if ( tsq < 0.0 )
                    tsq = 0.0;

                tsq += arg->shadeVal;

                if ( tsq > 1.0 )
                    tsq = 1.0;

                arg->rndrArg->color[i] = tsq;
            }
        }
        else
        {
            for (int i = 0; i < vtxCnt; i++ )
                arg->rndrArg->color[i] = arg->shadeVal;
        }
    }

    return true;
}



UAskeleton::Data *NC_STACK_skeleton::GetSkelet()
{
    return _resData;
}

int NC_STACK_skeleton::getSKEL_pntCount()
{
    if (_resData)
        return _resData->POO.size();
    return 0;
}

int NC_STACK_skeleton::getSKEL_senCount()
{
    if (_resData)
        return _resData->SEN.size();
    return 0;
}

int NC_STACK_skeleton::getSKEL_polyCount()
{
    if (_resData)
        return _resData->polygons.size();
    return 0;
}



