#include "includes.h"
#include "nucleas.h"
#include "utils.h"
#include "engine_input.h"

#include "rsrc.h"

#include "bitmap.h"
#include "display.h"
#include "win3d.h"

#include "skeleton.h"
#include "base.h"


#include <math.h>
#include <float.h>


const Nucleus::ClassDescr NC_STACK_skeleton::description("skeleton.class", &newinstance);

size_t NC_STACK_skeleton::func0(IDVList &stak)
{
    if ( !NC_STACK_rsrc::func0(stak) )
        return 0;

    stack__skeleton.data = (UAskeleton::Data *)getRsrc_pData();

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

    UAskeleton::Vertex *POO = new UAskeleton::Vertex[elm_num];

    sklt->POO = POO;

    if (!POO)
    {
        rsrc_func65(res);
        return NULL;
    }

    UAskeleton::Vertex *tformedVertex = new UAskeleton::Vertex[elm_num];

    sklt->tformedVertex = tformedVertex;

    if (!tformedVertex)
    {
        rsrc_func65(res);
        return NULL;
    }

    sklt->POO_NUM = elm_num;

    int sen_count = stak.Get<int32_t>(SKEL_ATT_SENCNT, 0);

    if (sen_count > 0)
    {
        skeleton_129_arg arg129;
        arg129.skeleton = sklt;
        arg129.sen_count = sen_count;

        if ( !skeleton_func129(&arg129) )
        {
            rsrc_func65(res);
            return NULL;
        }
    }

    int pol_count = stak.Get<int32_t>(SKEL_ATT_POLYCNT, 0);
    int num_indexes = stak.Get<int32_t>(SKEL_ATT_POLYPNTCNT, 0);

    if (pol_count > 0)
    {
        if (num_indexes <= 0)
        {
            rsrc_func65(res);
            return NULL;
        }

        skeleton_130_arg arg130;
        arg130.skeleton = sklt;
        arg130.num_indexes = num_indexes;
        arg130.pol_count = pol_count;

        if ( !skeleton_func130(&arg130) )
        {
            rsrc_func65(res);
            return NULL;
        }
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

__NC_STACK_skeleton * NC_STACK_skeleton::skeleton_func128(IDVPair *)
{
    return &stack__skeleton;
}

size_t NC_STACK_skeleton::skeleton_func129(skeleton_129_arg *arg)
{
    UAskeleton::Data *sklt = arg->skeleton;

    sklt->SEN = new UAskeleton::Vertex[arg->sen_count];

    if (!sklt->SEN)
        return 0;

    sklt->SEN_NUM = arg->sen_count;
    return 1;
}

size_t NC_STACK_skeleton::skeleton_func130(skeleton_130_arg *arg)
{
    UAskeleton::Data *sklt = arg->skeleton;

    sklt->polygons = new UAskeleton::Polygon[arg->pol_count];

    if (!sklt->polygons)
        return 0;

    sklt->polygonsCount = arg->pol_count;

    return 1;
}

size_t NC_STACK_skeleton::skeleton_func131(int *arg)
{
    UAskeleton::Data *sklt = this->stack__skeleton.data;

    int vtxid = *arg;

    if ( sklt )
    {
        UAskeleton::Polygon *pol = &sklt->polygons[vtxid];
        if ( pol->num_vertices >= 3 )
        {
            UAskeleton::Vertex *POO = sklt->POO;

            vec3d vec1 = POO[ pol->v[1] ] - POO[ pol->v[0] ];
            vec3d vec2 = POO[ pol->v[2] ] - POO[ pol->v[1] ];
            vec3d norm = vec1 * vec2;
            norm.normalise();

            sklt->polygons[vtxid].A = norm.x;
            sklt->polygons[vtxid].B = norm.y;
            sklt->polygons[vtxid].C = norm.z;
            sklt->polygons[vtxid].D = -(norm.x * POO[ pol->v[0] ].x +
                                        norm.y * POO[ pol->v[0] ].y +
                                        norm.z * POO[ pol->v[0] ].z);
        }
        else
        {
            sklt->polygons[vtxid].A = 0;
            sklt->polygons[vtxid].B = 0;
            sklt->polygons[vtxid].C = 0;
            sklt->polygons[vtxid].D = 0;
        }
    }

    return 1;
}


bool NC_STACK_skeleton::TransformVertexes(skeleton_arg_132 *arg, UAskeleton::Vertex *in, UAskeleton::Vertex *out, int num)
{
    if ( !num )
        return false;

    uint32_t andFlags = ~0;

    for(int i = 0; i < num; i++)
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

        out[i].x = fv.x;
        out[i].y = fv.y;
        out[i].z = fv.z;
        out[i].flags = clipFlags;
    }

    return andFlags == 0;
}

size_t NC_STACK_skeleton::skeleton_func132(skeleton_arg_132 *arg)
{
    UAskeleton::Data *sklt = this->stack__skeleton.data;
    int result = 1;

    if ( sklt->SEN_NUM )
    {
        if ( sklt->SEN_NUM < (sklt->POO_NUM / 4) )
            result = TransformVertexes(arg, sklt->SEN, sklt->tformedVertex, sklt->SEN_NUM);
    }

    if ( result )
        result = TransformVertexes(arg, sklt->POO, sklt->tformedVertex, sklt->POO_NUM);

    return result;
}

bool NC_STACK_skeleton::PolygonCheckInvisible(UAskeleton::Vertex *in, vec3d *out, UAskeleton::Polygon *pol)
{
    uint16_t andFlags = ~0;

    for (int i = 0; i < pol->num_vertices; i++)
    {
        out[i] = in[ pol->v[i] ];
        andFlags &= in[ pol->v[i] ].flags;
    }

    if ( pol->num_vertices > 2 && !andFlags )
    {
        vec3d vec1 = out[1] - out[0];
        vec3d vec2 = out[2] - out[1];
        vec3d norm = vec1 * vec2;

        if ( norm.x * out[0].x + norm.y * out[0].y + norm.z * out[0].z < 0.0 )
            return false;
    }

    return andFlags == 0;
}


bool NC_STACK_skeleton::skeleton_func133(skeleton_arg133 *arg)
{
    UAskeleton::Data *sklt = this->stack__skeleton.data;

    vec3d PolyVertex[GFX_MAX_VERTEX];
    UAskeleton::Polygon *pol = &sklt->polygons[arg->polyID];

    if ( !PolygonCheckInvisible(sklt->tformedVertex, PolyVertex, pol) )
        return false;

    arg->rndrArg->vertexCount = pol->num_vertices;
    int vtxCnt = pol->num_vertices;

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
    return stack__skeleton.data;
}

int NC_STACK_skeleton::getSKEL_pntCount()
{
    if (stack__skeleton.data)
        return stack__skeleton.data->POO_NUM;
    return 0;
}

int NC_STACK_skeleton::getSKEL_senCount()
{
    if (stack__skeleton.data)
        return stack__skeleton.data->SEN_NUM;
    return 0;
}

int NC_STACK_skeleton::getSKEL_polyCount()
{
    if (stack__skeleton.data)
        return stack__skeleton.data->polygonsCount;
    return 0;
}



