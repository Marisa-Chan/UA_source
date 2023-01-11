#include "includes.h"
#include "nucleas.h"
#include "area.h"
#include "utils.h"
#include "system/inpt.h"

#include "skeleton.h"
#include "base.h"

#include "system/gfx.h"
#include "system/inivals.h"


size_t NC_STACK_area::Init(IDVList &stak)
{
    if ( !NC_STACK_ade::Init(stak) )
        return 0;

    _colorVal = 1;
    _tracyVal = 0;
    _shadeVal = 0;
    _polflags = 0;

    for( auto& it : stak )
    {
        IDVPair &val = it.second;

        if ( !val.Skip )
        {
            switch (val.ID)
            {
            case AREA_ATT_TEXBITM:
                setAREA_bitm ( val.Get<NC_STACK_bitmap *>() );
                break;
            case AREA_ATT_COLORVAL:
                setAREA_colorVal( val.Get<int32_t>() );
                break;
            case AREA_ATT_MAP:
                setAREA_map( val.Get<int32_t>() );
                break;

            case AREA_ATT_TEX:
                setAREA_tex( val.Get<int32_t>() );
                break;

            case AREA_ATT_SHADE:
                setAREA_shade ( val.Get<int32_t>() );
                break;

            case AREA_ATT_TRACY:
                setAREA_tracy ( val.Get<int32_t>() );
                break;

            case AREA_ATT_TRACYMODE:
                setAREA_tracymode( val.Get<int32_t>() );
                break;

            case AREA_ATT_TRACYBITM:
                setAREA_tracybitm ( val.Get<NC_STACK_bitmap *>() );
                break;

            case AREA_ATT_SHADEVAL:
                setAREA_shadeVal ( val.Get<int32_t>() );
                break;

            case AREA_ATT_TRACYVAL:
                setAREA_tracyVal ( val.Get<int32_t>() );
                break;

            default:
                break;
            }
        }
    }

    return 1;
}

size_t NC_STACK_area::Deinit()
{
    if ( _texImg )
        _texImg->Delete();
    if ( _tracyImg )
        _tracyImg->Delete();
    return NC_STACK_ade::Deinit();
}


int NC_STACK_area::area_func5__sub0(IFFile *mfile)
{
    struct AREA_STRC
    {
        int16_t version;
        uint16_t flags;
        uint16_t polFlags;
        uint8_t _un1;
        uint8_t clrVal;
        uint8_t trcVal;
        uint8_t shdVal;
    } tmp;

    tmp.version = mfile->readS16B();
    tmp.flags = mfile->readU16B();
    tmp.polFlags = mfile->readU16B();
    tmp._un1 = mfile->readU8();
    tmp.clrVal = mfile->readU8();
    tmp.trcVal = mfile->readU8();
    tmp.shdVal = mfile->readU8();

    if ( tmp.version >= 1 )
    {
        _polflags = tmp.polFlags;
        _colorVal = tmp.clrVal;
        _tracyVal = tmp.trcVal;
        _shadeVal = tmp.shdVal;
        
        setADE_bkCheck( (tmp.flags & AREA_FLAG_BKCHECK) != 0 );
        setADE_depthFade( (tmp.flags & AREA_FLAG_DPTHFADE) != 0 );        
    }

    return 1;
}



int NC_STACK_area::area_func5__sub1(IFFile *mfile)
{
    int v8 = _polflags & AREA_POL_FLAG_TEXUTRED;

    if ( (_polflags & AREA_POL_FLAG_TRACYMAPPED) == AREA_POL_FLAG_TRACYMAPPED )
        v8 |= AREA_POL_FLAG_TRACYMAPPED;

    NC_STACK_bitmap *objt = dynamic_cast<NC_STACK_bitmap *>( LoadObjectFromIFF(mfile) );
    if ( objt )
    {
        if (v8 == AREA_POL_FLAG_TEXUTRED )
        {
            setAREA_bitm(objt);
        }
        else if ( v8 == AREA_POL_FLAG_TRACYMAPPED )
        {
            setAREA_tracybitm(objt);
        }
        else if ( v8 == (AREA_POL_FLAG_TRACYMAPPED | AREA_POL_FLAG_TEXUTRED) )
        {
            if ( _texImg )
            {
                setAREA_tracybitm(objt);
            }
            else
            {
                setAREA_bitm(objt);
            }
        }
        return 1;
    }

    return 0;
}


size_t NC_STACK_area::LoadingFromIFF(IFFile **file)
{
    IFFile *mfile = *file;
    int obj_ok = 0;

    while ( 1 )
    {
        int iff_res = mfile->parse();

        if ( iff_res == -2 )
            break;

        if ( iff_res )
        {
            if ( obj_ok )
                Deinit();
            return 0;
        }

        const IFFile::Context &chunk = mfile->GetCurrentChunk();

        if ( chunk.Is(TAG_FORM, TAG_ADE) )
        {
            obj_ok = NC_STACK_ade::LoadingFromIFF(file);

            if ( !obj_ok )
                return 0;
        }
        else if ( chunk.Is(TAG_STRC) )
        {
            if ( obj_ok && !area_func5__sub0(mfile) )
            {
                Deinit();
                return 0;
            }
            mfile->parse();
        }
        else if ( chunk.Is(TAG_FORM, TAG_OBJT) )
        {
            if ( obj_ok && !area_func5__sub1(mfile) )
            {
                Deinit();
                return 0;
            }
        }
        else
        {
            mfile->skipChunk();
        }
    }

    return obj_ok;
}

size_t NC_STACK_area::SaveIntoIFF(IFFile **file)
{
    IFFile *mfile = *file;

    if ( mfile->pushChunk(TAG_AREA, TAG_FORM, -1) )
        return 0;


    if ( !NC_STACK_ade::SavingIntoIFF(file) )
        return 0;

    mfile->pushChunk(0, TAG_STRC, -1);
    
    uint16_t tmpflg = 0;
    if (flags & ADE_FLAG_BKCHECK)
        tmpflg |= AREA_FLAG_BKCHECK;
    if (flags & ADE_FLAG_DPTHFADE)
        tmpflg |= AREA_FLAG_DPTHFADE;

    mfile->writeS16B(1); // version
    mfile->writeU16B(tmpflg);
    mfile->writeU16B(_polflags);
    mfile->writeU8(0);
    mfile->writeU8(_colorVal);
    mfile->writeU8(_tracyVal);
    mfile->writeU8(_shadeVal);

    mfile->popChunk();

    if ( (_polflags & AREA_POL_FLAG_TEXUTRED) )
    {
        if ( !_texImg )
            return 0;
        if ( !_texImg->SaveObjectIntoIFF(mfile) )
            return 0;
    }
    if ( (_polflags & AREA_POL_FLAG_TRACYMAPPED) == AREA_POL_FLAG_TRACYMAPPED )
    {
        if ( !_tracyImg )
            return 0;
        if ( !_tracyImg->SaveObjectIntoIFF(mfile) )
            return 0;
    }

    return mfile->popChunk() == IFFile::IFF_ERR_OK;
}

void NC_STACK_area::GenMesh(std::list<GFX::TMesh> *meshList, NC_STACK_skeleton * skelet)
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
    
    float alpha = 1.0;
        
    if (renderFlags & GFX::RFLAGS_LUMTRACY)
        alpha = GFX::Engine.GetAlpha();
    
    float clr = 1.0;
    if (renderFlags & GFX::RFLAGS_SHADED)
    {
        clr = (1.0 - _shadeVal / 256.0);
        if (clr < 0.0)
            clr = 0.0;
        if (clr > 1.0)
            clr = 1.0;
    }
    
    if (!_texImg)
        clr = 0.0;
    
    GFX::TRenderParams mat(renderFlags);
    
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
    
    UAskeleton::Data *dat = skelet->GetSkelet();
    UAskeleton::Polygon &pol = dat->polygons[_polyID];

    uint32_t fid = msh->Vertexes.size();
    
    std::vector<tUtV> *coords = NULL;
    if (_texImg && !_texImg->IsDynamic())
        coords = &(_texImg->GetOutline());
    
    if (pol.num_vertices >= 3)
    {
        for(int i = 0; i < pol.num_vertices; ++i)
        {
            msh->Vertexes.emplace_back();
            msh->Vertexes.back().Pos = dat->POO[ pol.v[i] ];
            msh->Vertexes.back().TexCoordId = i;
            msh->Vertexes.back().Color = GFX::TGLColor(clr, clr, clr, alpha);
            
            if (coords)
                msh->Vertexes.back().TexCoord = coords->at(i);
            
            msh->BoundBox.Add( dat->POO[ pol.v[i] ] );
        }
        
        for(int j = 2; j < pol.num_vertices; ++j)
            {
                msh->Indixes.push_back(fid + 0);
                msh->Indixes.push_back(fid + j);
                msh->Indixes.push_back(fid + j - 1);
            }
    }
}

GFX::TRenderParams NC_STACK_area::GetRenderParams( size_t ) 
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
    
    float clr = 1.0;
    if (renderFlags & GFX::RFLAGS_SHADED)
    {
        clr = (1.0 - _shadeVal / 256.0);
        if (clr < 0.0)
            clr = 0.0;
        if (clr > 1.0)
            clr = 1.0;
    }
    
    GFX::TRenderParams params;
    
    params.Flags = renderFlags;
    
    if (_texImg)
    {
        params.Color = GFX::TGLColor(clr, clr, clr, 1.0);
        params.TexSource = _texImg;
        
        if (_texImg->IsDynamic())
            params.Flags |= GFX::RFLAGS_DYNAMIC_TEXTURE;
        else
            params.Tex = _texImg->GetBitmap();
    }
    else
    {
        params.Color = GFX::TGLColor(0.0, 0.0, 0.0, 1.0);
    }
    
    return params; 
};

void NC_STACK_area::setAREA_bitm(NC_STACK_bitmap *bitm)
{
    if ( bitm )
    {
        if ( _texImg != NULL )
            _texImg->Delete();

        _texImg = bitm;
        bitm->PrepareTexture();
    }
}

void NC_STACK_area::setAREA_colorVal(int val)
{
    _colorVal = val;
}

void NC_STACK_area::setAREA_map(int mode)
{
    _polflags &= ~(AREA_POL_FLAG_MAPBIT1 | AREA_POL_FLAG_MAPBIT2);

    if ( mode == 1 )
        _polflags |= AREA_POL_FLAG_LINEARMAPPED;
    else if ( mode == 2 )
        _polflags |= AREA_POL_FLAG_DEPTHMAPPED;
}

void NC_STACK_area::setAREA_tex(int mode)
{
    _polflags &= ~(AREA_POL_FLAG_TEXBIT);

    if ( mode == 1 )
        _polflags |= AREA_POL_FLAG_TEXUTRED;
}

void NC_STACK_area::setAREA_shade(int mode)
{
    _polflags &= ~(AREA_POL_FLAG_SHADEBIT1 | AREA_POL_FLAG_SHADEBIT2);

    if ( mode == 1 )
        _polflags |= AREA_POL_FLAG_FLATSHADE;
    else if ( mode == 2 )
        _polflags |= AREA_POL_FLAG_LINESHADE;
    else if ( mode == 3 )
        _polflags |= AREA_POL_FLAG_GRADIENTSHADE;
}

void NC_STACK_area::setAREA_tracy(int mode)
{
    _polflags &= ~(AREA_POL_FLAG_TRACYBIT1 | AREA_POL_FLAG_TRACYBIT2);

    if ( mode == 1 )
        _polflags |= AREA_POL_FLAG_CLEARTRACY;
    else if ( mode == 2 )
        _polflags |= AREA_POL_FLAG_FLATTRACY;
    else if ( mode == 3 )
        _polflags |= AREA_POL_FLAG_TRACYMAPPED;
}

void NC_STACK_area::setAREA_tracymode(int mode)
{
    _polflags &= ~(AREA_POL_FLAG_TRACYBIT3);

    if ( mode == 1 )
        _polflags |= AREA_POL_FLAG_LIGHT;
}

void NC_STACK_area::setAREA_tracybitm(NC_STACK_bitmap *bitm)
{
    if ( bitm )
    {
        if ( _tracyImg != NULL )
            _tracyImg->Delete();

        _tracyImg = bitm;
        bitm->PrepareTexture();
    }
}

void NC_STACK_area::setAREA_shadeVal(int val)
{
    _shadeVal = val;
}

void NC_STACK_area::setAREA_tracyVal(int val)
{
    _tracyVal = val;
}

NC_STACK_bitmap *NC_STACK_area::getAREA_bitm()
{
    return _texImg;
}

int NC_STACK_area::getAREA_colorVal()
{
    return _colorVal;
}

int NC_STACK_area::getAREA_map()
{
    int v9 = _polflags & (AREA_POL_FLAG_MAPBIT1 | AREA_POL_FLAG_MAPBIT2);

    if ( v9 == AREA_POL_FLAG_NONMAPPED )
        return 0;
    else if ( v9 == AREA_POL_FLAG_LINEARMAPPED )
        return 1;
    else if ( v9 == AREA_POL_FLAG_DEPTHMAPPED )
        return 2;

    return 0;
}

int NC_STACK_area::getAREA_tex()
{
    int v6 = _polflags & AREA_POL_FLAG_TEXBIT;

    if ( v6 == AREA_POL_FLAG_TEXUTRED )
        return 1;

    return 0;
}

int NC_STACK_area::getAREA_shade()
{
    int v4 = _polflags & (AREA_POL_FLAG_SHADEBIT1 | AREA_POL_FLAG_SHADEBIT2);

    if ( v4 == AREA_POL_FLAG_NOSHADE )
        return 0;
    else if ( v4 == AREA_POL_FLAG_FLATSHADE )
        return 1;
    else if ( v4 == AREA_POL_FLAG_LINESHADE )
        return 2;
    else if ( v4 == AREA_POL_FLAG_GRADIENTSHADE )
        return 3;
    return 0;
}

int NC_STACK_area::getAREA_tracy()
{
    int v5 = _polflags & (AREA_POL_FLAG_TRACYBIT1 | AREA_POL_FLAG_TRACYBIT2);

    if ( v5 == AREA_POL_FLAG_NOTRACY )
        return 0;
    else if ( v5 == AREA_POL_FLAG_CLEARTRACY )
        return 1;
    else if ( v5 == AREA_POL_FLAG_FLATTRACY )
        return 2;
    else if ( v5 == AREA_POL_FLAG_TRACYMAPPED )
        return 3;
    return 0;
}

int NC_STACK_area::getAREA_tracymode()
{
    int v10 = _polflags & AREA_POL_FLAG_TRACYBIT3;

    if ( v10 == AREA_POL_FLAG_DARK )
        return 0;
    else if ( v10 == AREA_POL_FLAG_LIGHT )
        return 1;

    return 0;
}

NC_STACK_bitmap *NC_STACK_area::getAREA_tracybitm()
{
    return _tracyImg;
}

int NC_STACK_area::getAREA_shadeVal()
{
    return _shadeVal;
}

int NC_STACK_area::getAREA_tracyVal()
{
    return _tracyVal;
}
