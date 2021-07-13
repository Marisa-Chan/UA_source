#include "includes.h"
#include "inttypes.h"
#include "nucleas.h"
#include "amesh.h"
#include "utils.h"
#include "system/inpt.h"

#include "skeleton.h"
#include "base.h"

#include "system/gfx.h"
#include "system/inivals.h"


const Nucleus::ClassDescr NC_STACK_amesh::description("amesh.class", &newinstance);

size_t NC_STACK_amesh::Init(IDVList &stak)
{
    if ( !NC_STACK_area::Init(stak) )
        return 0;

    for( auto& it : stak )
    {
        IDVPair &val = it.second;

        if ( !val.Skip )
        {
            switch (val.ID)
            {
            case AMESH_ATT_NUMPOLY:
                setAMESH_numpoly(val.Get<int32_t>());
                break;

            case AMESH_ATT_ATTPOLYS:
                if ( !setAMESH_polys( val.Get<ATTS *>() ) )
                {
                    Deinit();
                    return 0;
                }
                break;

            case AMESH_ATT_OTLPOOL:
                if ( !setAMESH_otls(val.Get<std::vector< std::vector<tUtV> >*>()) )
                {
                    Deinit();
                    return 0;
                }
                break;

            default:
                break;
            }
        }
    }


    return 1;
}


size_t NC_STACK_amesh::LoadingFromIFF(IFFile **file)
{
    IFFile *mfile = *file;
    int obj_ok = 0;

    while ( 1 )
    {
        int iff_res = mfile->parse();

        if ( iff_res == IFFile::IFF_ERR_EOC )
            break;

        if ( iff_res )
        {
            if ( obj_ok )
                Deinit();
            return 0;
        }

        const IFFile::Context &chunk = mfile->GetCurrentChunk();

        if ( chunk.Is(TAG_FORM, TAG_AREA) )
        {
            obj_ok = NC_STACK_area::LoadingFromIFF(file);

            if ( !obj_ok )
                return 0;
        }
        else if ( chunk.Is(TAG_ATTS) )
        {
            if ( obj_ok )
            {
                polyCnt = chunk.TAG_SIZE / 6;
                
                atts.resize(polyCnt);
                
                for (int i = 0; i < polyCnt; i++)
                {
                    ATTS &att = atts.at(i);
                    att.polyID = mfile->readS16B();
                    att.colorVal = mfile->readU8();
                    att.shadeVal = mfile->readU8();
                    att.tracyVal = mfile->readU8();
                    att.pad = mfile->readU8();
                }
            }
            mfile->parse();
        }
        else if ( chunk.Is(TAG_OLPL) )
        {
            if ( obj_ok )
            {
                texCoords.resize(polyCnt);
                //texCoordsData.resize( chunk.TAG_SIZE / 2 );

                //tUtV *uv = texCoordsData.data();

                for (auto &n : texCoords)
                {
                    int16_t cnt = mfile->readS16B();
                    n.resize(cnt);

                    for (tUtV &uv : n)
                    {
                        //If you cross refernce the VBMP these are the X,Y coordinates
                        // as it is stored on disk
                        uv.tu = mfile->readU8() / 256.0;
                        uv.tv = mfile->readU8() / 256.0;
                    }
                }
            }
            mfile->parse();
        }
        else
        {
            mfile->skipChunk();
        }
    }

    return obj_ok;
}

size_t NC_STACK_amesh::SaveIntoIFF(IFFile **file)
{
    IFFile *mfile = *file;

    if ( mfile->pushChunk(TAG_AMSH, TAG_FORM, -1) )
        return 0;

    if ( !NC_STACK_area::SaveIntoIFF(file) )
        return 0;

    mfile->pushChunk(0, TAG_ATTS, -1);

    for (int i = 0; i < polyCnt; i++)
    {
        ATTS &att = atts.at(i);
        mfile->writeS16B(att.polyID);
        mfile->writeU8(att.colorVal);
        mfile->writeU8(att.shadeVal);
        mfile->writeU8(att.tracyVal);
        mfile->writeU8(att.pad);
    }

    mfile->popChunk();

    if ( !texCoords.empty() )
    {
        mfile->pushChunk(0, TAG_OLPL, -1);

        for (const std::vector<tUtV> &arr : texCoords)
        {
            mfile->writeS16B(arr.size());

            for (tUtV uv : arr)
            {
                mfile->writeU8( uv.tu * 256.0 );
                mfile->writeU8( uv.tv * 256.0 );
            }
        }
        mfile->popChunk();
    }
    mfile->popChunk();
    return 1;
}

void NC_STACK_amesh::GenMesh(std::list<GFX::TMesh> *meshList, NC_STACK_skeleton * skelet)
{
    uint32_t renderFlags = _polflags & ~(AREA_POL_FLAG_SCANLN | AREA_POL_FLAG_TEXBIT | AREA_POL_FLAG_TRACYBIT3);

    if (renderFlags == 0)
        renderFlags = 0;
    else if (renderFlags == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_NOTRACY) )
        renderFlags = GFX::RFLAGS_TEXTURED;

    else if (renderFlags == (AREA_POL_FLAG_DEPTHMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_NOTRACY) )
        renderFlags = GFX::RFLAGS_TEXTURED;

    else if (renderFlags == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_GRADIENTSHADE | AREA_POL_FLAG_NOTRACY) )
        renderFlags = GFX::RFLAGS_TEXTURED | GFX::RFLAGS_SHADED;

    else if (renderFlags == (AREA_POL_FLAG_DEPTHMAPPED | AREA_POL_FLAG_GRADIENTSHADE | AREA_POL_FLAG_NOTRACY) )
        renderFlags = GFX::RFLAGS_TEXTURED | GFX::RFLAGS_SHADED;

    else if (renderFlags == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_CLEARTRACY) )
        renderFlags = GFX::RFLAGS_TEXTURED | GFX::RFLAGS_ZEROTRACY;

    else if (renderFlags == (AREA_POL_FLAG_DEPTHMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_CLEARTRACY) )
        renderFlags = GFX::RFLAGS_TEXTURED | GFX::RFLAGS_ZEROTRACY;

    else if (renderFlags == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_GRADIENTSHADE | AREA_POL_FLAG_CLEARTRACY) )
        renderFlags = GFX::RFLAGS_TEXTURED | GFX::RFLAGS_SHADED | GFX::RFLAGS_ZEROTRACY;

    else if (renderFlags == (AREA_POL_FLAG_DEPTHMAPPED | AREA_POL_FLAG_GRADIENTSHADE | AREA_POL_FLAG_CLEARTRACY) )
        renderFlags = GFX::RFLAGS_TEXTURED | GFX::RFLAGS_SHADED | GFX::RFLAGS_ZEROTRACY;

    else if (renderFlags == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_FLATTRACY) )
        renderFlags = GFX::RFLAGS_TEXTURED | GFX::RFLAGS_LUMTRACY;
    
    if (flags & ADE_FLAG_DPTHFADE)
        renderFlags |= GFX::RFLAGS_FOG;
    
    UAskeleton::Data *dat = skelet->GetSkelet();
    
    float alpha = 1.0;
        
    if (renderFlags & GFX::RFLAGS_LUMTRACY)
        alpha = GFX::Engine.GetAlpha();
    
    for (int i = 0; i < polyCnt; i++)
    {
        ATTS &att = atts.at(i);
        
        float clr = 1.0;
        if (renderFlags & GFX::RFLAGS_SHADED)
        {
            clr = (1.0 - att.shadeVal / 256.0);
            if (clr < 0.0)
                clr = 0.0;
            if (clr > 1.0)
                clr = 1.0;
        }
        
        if (!_texImg)
            clr = 0.0;

        GFX::TRenderParams mat = GFX::TRenderParams(renderFlags);
        
        if (_texImg)
        {
            mat.TexSource = _texImg;
            
            if ( _texImg->IsDynamic() )
                mat.Flags |= GFX::RFLAGS_DYNAMIC_TEXTURE;
            else
                mat.Tex = _texImg->GetBitmap();
        }

        GFX::TMesh *msh = NC_STACK_base::FindMeshByRenderParams(meshList, mat);
        
        if (!msh)
        {
            meshList->emplace_back();
            msh = &(meshList->back());

            msh->Mat = mat;
        }
        
        
        UAskeleton::Polygon &pol = dat->polygons[att.polyID];

        uint32_t fid = msh->Vertexes.size();

        if (pol.num_vertices >= 3)
        {
            for(int j = 0; j < pol.num_vertices; ++j)
            {
                msh->Vertexes.emplace_back();
                msh->Vertexes.back().Pos = dat->POO[ pol.v[j] ];
                msh->Vertexes.back().TexCoordId = j;
                msh->Vertexes.back().Color = GFX::TGLColor(clr, clr, clr, alpha);
                
                msh->BoundBox.Add( dat->POO[ pol.v[j] ] );
                
                if (!texCoords.empty())
                    msh->Vertexes.back().TexCoord = texCoords.at(i).at(j);
            }
            
            for(int j = 2; j < pol.num_vertices; ++j)
            {
                msh->Indixes.push_back(fid + 0);
                msh->Indixes.push_back(fid + j);
                msh->Indixes.push_back(fid + j - 1);
            }
        }
    }
}

void NC_STACK_amesh::setAMESH_numpoly(int num)
{
    polyCnt = num;
}

int NC_STACK_amesh::setAMESH_polys(ATTS *patts)
{
    atts.resize(polyCnt);

    for (int i = 0; i < polyCnt; i++)
        atts.at(i) = patts[i];

    return 1;
}

int NC_STACK_amesh::setAMESH_otls(std::vector< std::vector<tUtV> >*uv)
{
    texCoords = *uv;
    return 1;
}


int NC_STACK_amesh::getAMESH_numpoly()
{
    return polyCnt;
}