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

    skeleton_64_stru *sklt = (skeleton_64_stru *)AllocVec(sizeof(skeleton_64_stru), 65537);

    if (!sklt)
    {
        rsrc_func65(&res);
        return NULL;
    }

    res->data = sklt;

    skeleton_type1 *POO = (skeleton_type1 *)AllocVec(sizeof(skeleton_type1) * elm_num, 65537);

    sklt->POO = POO;

    if (!POO)
    {
        rsrc_func65(&res);
        return NULL;
    }

    skeleton_type1 *POO2 = (skeleton_type1 *)AllocVec(sizeof(skeleton_type1) * elm_num, 65537);

    sklt->type2 = POO2;

    if (!POO2)
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
        if ( sklt->POO )
            nc_FreeMem(sklt->POO);

        if ( sklt->pol_entries )
            delete sklt->pol_entries;

        if ( sklt->sen_entries )
            nc_FreeMem(sklt->sen_entries);

        if ( sklt->type2 )
            nc_FreeMem(sklt->type2);

        nc_FreeMem(sklt);

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

    sklt->sen_entries = (skeleton_type1 *)AllocVec(sizeof(skeleton_type1) * arg->sen_count, 65537);

    if (!sklt->sen_entries)
        return 0;

    sklt->sen_count = arg->sen_count;
    return 1;
}

size_t NC_STACK_skeleton::skeleton_func130(skeleton_130_arg *arg)
{
    skeleton_64_stru *sklt = arg->skeleton;

    sklt->pol_entries = new Polygon[arg->pol_count];

    if (!sklt->pol_entries)
        return 0;

    sklt->pol_count = arg->pol_count;

    return 1;
}

size_t NC_STACK_skeleton::skeleton_func131(int *arg)
{
    skeleton_64_stru *sklt = this->stack__skeleton.data;

    int vtxid = *arg;

    if ( sklt )
    {
        Polygon *pol = &sklt->pol_entries[vtxid];
        if ( pol->num_vertices >= 3 )
        {
            skeleton_type1 *POO = sklt->POO;

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

            sklt->pol_entries[vtxid].A = dzy;
            sklt->pol_entries[vtxid].B = dzx;
            sklt->pol_entries[vtxid].C = dxy;
            sklt->pol_entries[vtxid].D = -(dzy * POO[pol->v[0]].sx + dzx * POO[pol->v[0]].sy + dxy * POO[pol->v[0]].sz);
        }
        else
        {
            sklt->pol_entries[vtxid].A = 0;
            sklt->pol_entries[vtxid].B = 0;
            sklt->pol_entries[vtxid].C = 0;
            sklt->pol_entries[vtxid].D = 0;
        }
    }

    return 1;
}


int sub_428D5C(skeleton_arg_132 *arg, skeleton_type1 *in, skeleton_type1 *out, int num)
{
    TForm3D *glob_1c = arg->glob_1c;
    TForm3D *base_1c = arg->base_1c;

    uint32_t flag = 0xFFFFFFFF;

    if ( num )
    {
        skeleton_type1 *vtx_in = in;
        skeleton_type1 *out_vertex = out;

        for(int i = 0; i < num; i++)
        {
            float v_x = vtx_in->sx;
            float v_y = vtx_in->sy;
            float v_z = vtx_in->sz;

            float _v_x, _v_y, _v_z;

            if ( base_1c->flags & 2 )
            {
                _v_y = v_y;
                _v_x = v_x;
                _v_z = v_z;
            }
            else
            {
                _v_x = base_1c->globSclRot.m00 * v_x + base_1c->globSclRot.m01 * v_y + base_1c->globSclRot.m02 * v_z;
                _v_y = base_1c->globSclRot.m10 * v_x + base_1c->globSclRot.m11 * v_y + base_1c->globSclRot.m12 * v_z;
                _v_z = base_1c->globSclRot.m20 * v_x + base_1c->globSclRot.m21 * v_y + base_1c->globSclRot.m22 * v_z;
            }

            float tx = _v_x + base_1c->globPos.sx - glob_1c->globPos.sx;
            float ty = _v_y + base_1c->globPos.sy - glob_1c->globPos.sy;
            float tz = _v_z + base_1c->globPos.sz - glob_1c->globPos.sz;

            int tflag = 0;

            float sx = glob_1c->globSclRot.m00 * tx + glob_1c->globSclRot.m01 * ty + glob_1c->globSclRot.m02 * tz;
            float sy = glob_1c->globSclRot.m10 * tx + glob_1c->globSclRot.m11 * ty + glob_1c->globSclRot.m12 * tz;
            float sz = glob_1c->globSclRot.m20 * tx + glob_1c->globSclRot.m21 * ty + glob_1c->globSclRot.m22 * tz;

            if ( sz >= arg->field_8 )
            {
                if ( sz > arg->field_C )
                    tflag = 0x20;
            }
            else
                tflag = 0x10;

            if ( sx > sz )
                tflag |= 2;
            if ( -sz > sx )
                tflag |= 1;
            if ( sy > sz )
                tflag |= 8;
            if ( -sz > sy )
                tflag |= 4;

            flag &= tflag;

            out_vertex->flags = tflag;
            out_vertex->sx = sx;
            out_vertex->sy = sy;
            out_vertex->sz = sz;

            vtx_in++;
            out_vertex++;
        }
    }
    return flag == 0;
}

size_t NC_STACK_skeleton::skeleton_func132(skeleton_arg_132 *arg)
{
    skeleton_64_stru *sklt = this->stack__skeleton.data;
    int result = 1;

    if ( sklt->sen_count )
    {
        if ( sklt->sen_count < (sklt->POO_NUM / 4) )
            result = sub_428D5C(arg, sklt->sen_entries, sklt->type2, sklt->sen_count);
    }

    if ( result )
        result = sub_428D5C(arg, sklt->POO, sklt->type2, sklt->POO_NUM);

    return result;
}



int dword_5151C4;


void sub_4BE1C4(skeleton_intern133 *arg, skeleton_type1 *skt1, skeleton_type1 *skt2, tUtV *uv1, tUtV *uv2, skeleton_type1 *outskt, tUtV *outuv)
{
    int flag = arg->field_10;
    float sy = skt2->sy - skt1->sy;
    float sz = skt2->sz - skt1->sz;
    float sx = skt2->sx - skt1->sx;

    float tmp = 1.0;

    if (flag == 1)
    {
        tmp = (skt1->sx + skt1->sz) / (-sx - sz);
        outskt->sx = sx * tmp + skt1->sx;
        outskt->sy = sy * tmp + skt1->sy;
        outskt->sz = -outskt->sx;
    }
    else if (flag == 2)
    {
        tmp = (skt1->sx - skt1->sz) / (sz - sx);
        outskt->sx = sx * tmp + skt1->sx;
        outskt->sz = sx * tmp + skt1->sx;
        outskt->sy = sy * tmp + skt1->sy;
    }
    else if (flag == 4)
    {
        tmp = (skt1->sy + skt1->sz) / (-sy - sz);
        outskt->sx = tmp * sx + skt1->sx;
        outskt->sy = sy * tmp + skt1->sy;
        outskt->sz = -(sy * tmp + skt1->sy);
    }
    else if (flag == 8)
    {
        tmp = (skt1->sy - skt1->sz) / (sz - sy);
        outskt->sx = tmp * sx + skt1->sx;
        outskt->sy = sy * tmp + skt1->sy;
        outskt->sz = sy * tmp + skt1->sy;
    }
    else if (flag == 16)
    {
        tmp = (arg->field_14 - skt1->sz) / sz;
        outskt->sx = tmp * sx + skt1->sx;
        outskt->sy = tmp * sy + skt1->sy;
        outskt->sz = arg->field_14;
    }
    else if (flag == 32)
    {
        tmp = (arg->field_18 - skt1->sz) / sz;
        outskt->sx = tmp * sx + skt1->sx;
        outskt->sy = tmp * sy + skt1->sy;
        outskt->sz = arg->field_18;
    }

    int tmpflag = 0;

    if ( outskt->sz > arg->field_18 )
        tmpflag = 32;
    else if ( outskt->sz < arg->field_18 )
        tmpflag = 16;

    if ( outskt->sx > outskt->sz )
        tmpflag |= 2;
    if ( outskt->sx < -outskt->sz )
        tmpflag |= 1;
    if ( outskt->sy > outskt->sz )
        tmpflag |= 8;
    if ( outskt->sy < -outskt->sz )
        tmpflag |= 4;

    outskt->flags = tmpflag;

    if ( dword_5151C4 )
    {
        outuv->tu = (uv2->tu - uv1->tu) * tmp + uv1->tu;
        outuv->tv = (uv2->tv - uv1->tv) * tmp + uv1->tv;
    }
}

void skeleton_func133__sub1__sub0(skeleton_intern133_sb *arg)
{
    skeleton_type1 *skt1 = arg->intern133.skt1;
    skeleton_type1 *skt2 = arg->intern133.skt1 + 1;
    skeleton_type1 *skt3 = arg->intern133.skt2;

    tUtV *uv1 = arg->intern133.uv1;
    tUtV *uv2 = uv1 + 1;
    tUtV *uv3 = arg->intern133.uv2;

    while ( 1 )
    {
        int flg1 = skt1->flags;

        if ( skt1->flags < 0 )
            break;

        int flg2 = skt2->flags;
        if ( skt2->flags < 0 )
        {
            skt2 = arg->intern133.skt1;
            uv2 = arg->intern133.uv1;
            flg2 = arg->intern133.skt1->flags;
        }

        int flg3 = arg->intern133.field_10;
        if ( flg3 & flg2 )
        {
            if ( !(flg3 & flg1) )
            {
                sub_4BE1C4(&arg->intern133, skt2, skt1, uv2, uv1, skt3, uv3);
                uv3++;
                skt3++;
            }
        }
        else if ( flg3 & flg1 )
        {
            sub_4BE1C4(&arg->intern133, skt1, skt2, uv1, uv2, skt3, uv3);
            uv3++;

            skeleton_type1 *skt4 = skt3 + 1;
            *skt4 = *skt2;
            skt3 += 2;

            if ( dword_5151C4 )
            {
                *uv3 = *uv2;
                uv3++;
            }
        }
        else
        {
            *skt3 = *skt2;
            skt3++;
            if ( dword_5151C4 )
            {
                *uv3 = *uv2;
                uv3++;
            }
        }

        uv1++;
        uv2++;
        skt1++;
        skt2++;
    }
    skt3->flags = 0x80000000;
}

skeleton_type1 stru_5B1210[6][32];
tUtV stru_5B1E10[6][32];

void skeleton_func133__sub1(skeleton_intern133 *arg)
{
    skeleton_intern133_sb tmp;

    int flag = arg->field_10;
    int i = 5;

    dword_5151C4 = arg->uv1 != 0;

    tmp.intern133 = *arg;

    while ( 1 )
    {
        if ( ((1 << i) & flag) )
        {
            flag &= ~(1 << i);
            if ( flag )
            {
                tmp.intern133.skt2 = stru_5B1210[i];
                tmp.intern133.uv2 = stru_5B1E10[i];
            }
            else
            {
                tmp.intern133.skt2 = arg->skt2;
                tmp.intern133.uv2 = arg->uv2;
            }
            tmp.intern133.field_10 = 1 << i;
            skeleton_func133__sub1__sub0(&tmp);

            if ( !flag )
                break;

            tmp.intern133.skt1 = tmp.intern133.skt2;
            tmp.intern133.uv1 = tmp.intern133.uv2;
        }
        i--;
    }
}

int skeleton_func133__sub0(skeleton_type1 *skt, skeleton_type1 *out, Polygon *pol)
{
    uint16_t v4 = 0xFFFF;
    int16_t *p_cur_vertex = pol->v;
    int v7 = 0;

    skeleton_type1 *skt_out = out;

    for (int i = 0; i < pol->num_vertices; i++)
    {
        *skt_out = skt[ pol->v[i] ];

        v7 |= skt_out->flags;
        v4 &= skt_out->flags;
        skt_out++;
        p_cur_vertex++;
    }

    skt_out->flags = 0x80000000;

    if ( pol->num_vertices > 2 && !v4 )
    {
        float sy  = out[1].sy - out[0].sy;
        float sz2 = out[2].sz - out[1].sz;
        float sz  = out[1].sz - out[0].sz;
        float sx2 = out[2].sx - out[1].sx;
        float sx  = out[1].sx - out[0].sx;
        float sy2 = out[2].sy - out[1].sy;

        if ( (sy * sz2 - sy2 * sz) * out->sx +
                (sz * sx2 - sz2 * sx) * out->sy +
                (sx * sy2 - sx2 * sy) * out->sz < 0.0 )
            v4 |= 0x8000;
    }
    return v7 | (v4 << 16);
}


void * NC_STACK_skeleton::skeleton_func133(skeleton_arg133 *arg)
{
    void *result;
    skeleton_64_stru *sklt = this->stack__skeleton.data;

    skeleton_type1 tmp_sklt[12];
    skeleton_type1 tmp_sklt2[12];
    tUtV tmp_uv[12];

    int flg = skeleton_func133__sub0(sklt->type2, tmp_sklt, &sklt->pol_entries[arg->polyID]);

    tUtV *puv;
    skeleton_type1 *pskt;

    if ( (flg >> 16) )
        return NULL;

    if ( flg & 0x3F )
    {
        skeleton_intern133 prms;

        prms.skt1 = tmp_sklt;
        prms.skt2 = tmp_sklt2;
        prms.uv1 = arg->texCoords;
        prms.field_10 = flg & 0x3F;
        prms.uv2 = tmp_uv;
        prms.field_14 = arg->field_10;
        prms.field_18 = arg->field_14;

        skeleton_func133__sub1(&prms);

        pskt = tmp_sklt2;
        puv = tmp_uv;
    }
    else
    {
        pskt = tmp_sklt;
        puv = arg->texCoords;
    }

    skeleton_type1 *tpskt = pskt;

    int i = 0;
    for ( i = 0; tpskt->flags >= 0; i++ )
    {
        arg->rndrArg->vertexes[i].sx = tpskt->sx / tpskt->sz;
        arg->rndrArg->vertexes[i].sy = tpskt->sy / tpskt->sz;
        arg->rndrArg->vertexes[i].sz = tpskt->sz;
        tpskt++;
    }

    arg->rndrArg->vertexCount = i;

    tUtV *tmpuv = (tUtV *)&arg->rndrArg->vertexes[i]; // tex coordinates going after vertex data

    result = tmpuv;

    if ( arg->field_4 & 1 )
    {
        arg->rndrArg->tu_tv = tmpuv;

        int j = 0;

        for ( j = 0; j < arg->rndrArg->vertexCount; j++)
            arg->rndrArg->tu_tv[j] = puv[j];

        result = &arg->rndrArg->tu_tv[j];
    }

    if ( arg->field_4 & 2 )
    {
        arg->rndrArg->color = (float *)result;
        int k = 0;

        if ( arg->field_4 & 4 )
        {
            tpskt = pskt;
            for ( k = 0; k < arg->rndrArg->vertexCount; k++ )
            {
                float sq = tpskt->sx * tpskt->sx + tpskt->sy * tpskt->sy + tpskt->sz * tpskt->sz;

                if ( sq < 0.0 )
                    sq = 0.0;

                float tsq = (sqrt(sq) - arg->field_1C) / arg->field_20;

                if ( tsq < 0.0 )
                    tsq = 0.0;

                tsq += arg->field_18;

                if ( tsq > 1.0 )
                    tsq = 1.0;

                arg->rndrArg->color[k] = tsq;
                tpskt++;
            }
        }
        else
        {
            for ( k = 0; k < arg->rndrArg->vertexCount; k++ )
                arg->rndrArg->color[k] = arg->field_18;
        }
        result = &arg->rndrArg->color[k];
    }

    return result; //return pointer to data after vertexes + uv coords + color
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
        return stack__skeleton.data->sen_count;
    return 0;
}

int NC_STACK_skeleton::getSKEL_polyCount()
{
    if (stack__skeleton.data)
        return stack__skeleton.data->pol_count;
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
