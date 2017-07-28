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


const NewClassDescr NC_STACK_skeleton::description("skeleton.class", &newinstance);


size_t NC_STACK_skeleton::func0(stack_vals *stak)
{
    if ( !NC_STACK_rsrc::func0(stak) )
        return 0;

    __NC_STACK_skeleton *skelt = &stack__skeleton;
    skelt->data = (skeleton_64_stru *)getRsrc_pData();

    return 1;
}

size_t NC_STACK_skeleton::func3(stack_vals *stak)
{
    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == stack_vals::TAG_END)
            break;
        else if (stk->id == stack_vals::TAG_PTAGS)
        {
            stk = (stack_vals *)stk->value.p_data;
        }
        else if ( stk->id == stack_vals::TAG_SKIP_N )
        {
            stk += stk->value.i_data;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;

            case SKEL_ATT_PSKELET:
                *(skeleton_64_stru **)stk->value.p_data = getSKEL_pSkelet();
                break;
            case SKEL_ATT_POINTSCNT:
                *(int *)stk->value.p_data = getSKEL_pntCount();
                break;
            case SKEL_ATT_SENCNT:
                *(int *)stk->value.p_data = getSKEL_senCount();
                break;
            case SKEL_ATT_POLYCNT:
                *(int *)stk->value.p_data = getSKEL_polyCount();
                break;
            }
            stk++;
        }
    }
    return NC_STACK_rsrc::func3(stak);
}

// Create skeleton resource node and fill rsrc field data
rsrc * NC_STACK_skeleton::rsrc_func64(stack_vals *stak)
{
    rsrc *res = NC_STACK_rsrc::rsrc_func64(stak);

    if ( !res )
        return NULL;

    int elm_num = find_id_in_stack_def_val(SKEL_ATT_POINTSCNT, 0, stak);

    if (!elm_num)
    {
        rsrc_func65(&res);
        return NULL;
    }

    skeleton_64_stru *sklt = new skeleton_64_stru;

    if (!sklt)
    {
        rsrc_func65(&res);
        return NULL;
    }

    res->data = sklt;

    Vertex *POO = new Vertex[elm_num];

    sklt->POO = POO;

    if (!POO)
    {
        rsrc_func65(&res);
        return NULL;
    }

    Vertex *tformedVertex = new Vertex[elm_num];

    sklt->tformedVertex = tformedVertex;

    if (!tformedVertex)
    {
        rsrc_func65(&res);
        return NULL;
    }

    sklt->POO_NUM = elm_num;

    int sen_count = find_id_in_stack_def_val(SKEL_ATT_SENCNT, 0, stak);

    if (sen_count > 0)
    {
        skeleton_129_arg arg129;
        arg129.skeleton = sklt;
        arg129.sen_count = sen_count;

        if ( !skeleton_func129(&arg129) )
        {
            rsrc_func65(&res);
            return NULL;
        }
    }

    int pol_count = find_id_in_stack_def_val(SKEL_ATT_POLYCNT, 0, stak);
    int num_indexes = find_id_in_stack_def_val(SKEL_ATT_POLYPNTCNT, 0, stak);

    if (pol_count > 0)
    {
        if (num_indexes <= 0)
        {
            rsrc_func65(&res);
            return NULL;
        }

        skeleton_130_arg arg130;
        arg130.skeleton = sklt;
        arg130.num_indexes = num_indexes;
        arg130.pol_count = pol_count;

        if ( !skeleton_func130(&arg130) )
        {
            rsrc_func65(&res);
            return NULL;
        }
    }

    return res;
}

size_t NC_STACK_skeleton::rsrc_func65(rsrc **pres)
{
    rsrc *res = *pres;
    skeleton_64_stru *sklt = (skeleton_64_stru *)res->data;

    if ( sklt )
    {
        delete sklt;

        res->data = NULL;
    }
    return NC_STACK_rsrc::rsrc_func65(pres);
}

__NC_STACK_skeleton * NC_STACK_skeleton::skeleton_func128(stack_vals *)
{
    return &stack__skeleton;
}

size_t NC_STACK_skeleton::skeleton_func129(skeleton_129_arg *arg)
{
    skeleton_64_stru *sklt = arg->skeleton;

    sklt->SEN = new Vertex[arg->sen_count];

    if (!sklt->SEN)
        return 0;

    sklt->SEN_NUM = arg->sen_count;
    return 1;
}

size_t NC_STACK_skeleton::skeleton_func130(skeleton_130_arg *arg)
{
    skeleton_64_stru *sklt = arg->skeleton;

    sklt->polygons = new Polygon[arg->pol_count];

    if (!sklt->polygons)
        return 0;

    sklt->polygonsCount = arg->pol_count;

    return 1;
}

size_t NC_STACK_skeleton::skeleton_func131(int *arg)
{
    skeleton_64_stru *sklt = this->stack__skeleton.data;

    int vtxid = *arg;

    if ( sklt )
    {
        Polygon *pol = &sklt->polygons[vtxid];
        if ( pol->num_vertices >= 3 )
        {
            Vertex *POO = sklt->POO;

            xyz vec1 = POO[ pol->v[1] ] - POO[ pol->v[0] ];
            xyz vec2 = POO[ pol->v[2] ] - POO[ pol->v[1] ];
            xyz norm = vec1 * vec2;
            norm.normolize();

            sklt->polygons[vtxid].A = norm.sx;
            sklt->polygons[vtxid].B = norm.sy;
            sklt->polygons[vtxid].C = norm.sz;
            sklt->polygons[vtxid].D = -(norm.sx * POO[ pol->v[0] ].sx +
                                        norm.sy * POO[ pol->v[0] ].sy +
                                        norm.sz * POO[ pol->v[0] ].sz);
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


bool NC_STACK_skeleton::TransformVertexes(skeleton_arg_132 *arg, Vertex *in, Vertex *out, int num)
{
    if ( !num )
        return false;

    TForm3D *glob_1c = arg->glob_1c;
    TForm3D *base_1c = arg->base_1c;

    uint32_t andFlags = ~0;

    for(int i = 0; i < num; i++)
    {
        xyz v = in[i];

        if ( !(base_1c->flags & 2) )
            v = base_1c->globSclRot.Transform(v);

        xyz fv = glob_1c->globSclRot.Transform(v + base_1c->globPos - glob_1c->globPos);

        int flags = 0;

        if ( fv.sz < arg->minZ )
            flags = Vertex::CLIP_NEAR;
        else if ( fv.sz > arg->maxZ )
            flags = Vertex::CLIP_FAR;
        else
        {
            if ( fv.sx > fv.sz * 1.1 )
                flags |= Vertex::CLIP_RIGHT;
            else if ( fv.sx < -fv.sz * 1.1 )
                flags |= Vertex::CLIP_LEFT;

            if ( fv.sy > fv.sz * 1.1 )
                flags |= Vertex::CLIP_TOP;
            else if ( fv.sy < -fv.sz * 1.1 )
                flags |= Vertex::CLIP_BOTTOM;
        }

        andFlags &= flags;

        out[i].sx = fv.sx;
        out[i].sy = fv.sy;
        out[i].sz = fv.sz;
        out[i].flags = flags;
    }

    return andFlags == 0;
}

size_t NC_STACK_skeleton::skeleton_func132(skeleton_arg_132 *arg)
{
    skeleton_64_stru *sklt = this->stack__skeleton.data;
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

bool NC_STACK_skeleton::PolygonCheckInvisible(Vertex *in, xyz *out, Polygon *pol)
{
    uint16_t andFlags = ~0;

    for (int i = 0; i < pol->num_vertices; i++)
    {
        out[i] = in[ pol->v[i] ];
        andFlags &= in[ pol->v[i] ].flags;
    }

    if ( pol->num_vertices > 2 && !andFlags )
    {
        xyz vec1 = out[1] - out[0];
        xyz vec2 = out[2] - out[1];
        xyz norm = vec1 * vec2;

        if ( norm.sx * out[0].sx + norm.sy * out[0].sy + norm.sz * out[0].sz < 0.0 )
            return false;
    }

    return andFlags == 0;
}


bool NC_STACK_skeleton::skeleton_func133(skeleton_arg133 *arg)
{
    skeleton_64_stru *sklt = this->stack__skeleton.data;

    xyz PolyVertex[GFX_MAX_VERTEX];
    Polygon *pol = &sklt->polygons[arg->polyID];

    if ( !PolygonCheckInvisible(sklt->tformedVertex, PolyVertex, pol) )
        return false;

    arg->rndrArg->vertexCount = pol->num_vertices;
    int vtxCnt = pol->num_vertices;

    for (int i = 0; i < vtxCnt; i++ )
        arg->rndrArg->vertexes[i] = PolyVertex[i];

    if ( arg->field_4 & 1 )
    {
        for (int i = 0; i < vtxCnt; i++)
            arg->rndrArg->tu_tv[i] = arg->texCoords[i];
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



skeleton_64_stru *NC_STACK_skeleton::getSKEL_pSkelet()
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



size_t NC_STACK_skeleton::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (stack_vals *)data );
    case 3:
        return (size_t)func3( (stack_vals *)data );
    case 64:
        return (size_t)rsrc_func64( (stack_vals *)data );
    case 65:
        return (size_t)rsrc_func65( (rsrc **)data );
    case 128:
        return (size_t)skeleton_func128( (stack_vals *)data );
    case 129:
        return (size_t)skeleton_func129( (skeleton_129_arg *)data );
    case 130:
        return (size_t)skeleton_func130( (skeleton_130_arg *)data );
    case 131:
        return (size_t)skeleton_func131( (int *)data );
    case 132:
        return (size_t)skeleton_func132( (skeleton_arg_132 *)data );
    case 133:
        return (size_t)skeleton_func133( (skeleton_arg133 *)data );
    default:
        break;
    }
    return NC_STACK_rsrc::compatcall(method_id, data);
}
