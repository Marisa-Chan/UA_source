#include "3ds.h"

#include "image.h"
#include "env.h"
#include "system/inivals.h"


const Nucleus::ClassDescr NC_STACK_3ds::description("3ds.class", &newinstance);

NC_STACK_3ds::NC_STACK_3ds()
{}

NC_STACK_3ds::~NC_STACK_3ds()
{}

size_t NC_STACK_3ds::Init(IDVList &stak)
{
    if ( !NC_STACK_base::Init(stak) )
        return 0;

    return 1;
}

size_t NC_STACK_3ds::Deinit()
{
    return NC_STACK_base::Deinit();
}

bool NC_STACK_3ds::LoadFromFile(const std::string &filename)
{
    FSMgr::FileHandle fil = FSMgr::iDir::openFile(filename, "rb");

    if (!fil.OK())
        return false;

    return LoadFromFile(&fil);
}

bool NC_STACK_3ds::LoadFromFile(FSMgr::FileHandle *fil)
{
    if (fil->readU16L() != 0x4D4D)
        return false;

    size_t datSz = fil->readU32L() - 6;
    size_t readed = 0;

    while (readed < datSz)
    {
        uint16_t tag = fil->readU16L();
        uint32_t tagsz = fil->readU32L() - 6;

        datSz += 6;

        switch(tag)
        {
        case 0x3D3D: //Object
            readed += readChunkEditor(fil, tagsz);
            break;

        default:
            fil->seek(tagsz, SEEK_CUR);
            readed += tagsz;
            break;
        }
    }
    
    RecalcInternal();
    MakeCoordsCache();

    return true;
}

NC_STACK_3ds *NC_STACK_3ds::Load3DS(const std::string &filename)
{
    FSMgr::FileHandle fil = uaOpenFile(filename, "rb");
    if (!fil.OK())
        return NULL;
    
    NC_STACK_3ds *tmp = Nucleus::CInit<NC_STACK_3ds>();
    if(tmp->LoadFromFile(&fil))
        return tmp;
    
    tmp->Delete();
    return NULL;
}

size_t NC_STACK_3ds::readChunkEditor(FSMgr::FileHandle *fil, size_t sz)
{
    printf("3D3D\n");
    size_t readed = 0;

    while (readed < sz)
    {
        uint16_t tag = fil->readU16L();
        uint32_t tagsz = fil->readU32L() - 6;

        readed += 6;
        
        if (tagsz > 100 * 1024 * 1024)
            break;

        switch(tag)
        {
        case 0x4000: //Trimesh
            readed += readChunkObject(fil, tagsz);
            break;

        case 0xAFFF: //Material
            readed += readChunkMaterial(fil, tagsz);
            break;

        default:
            fil->seek(tagsz, SEEK_CUR);
            readed += tagsz;
            break;
        }
    }

    return readed;
}

size_t NC_STACK_3ds::readChunkObject(FSMgr::FileHandle *fil, size_t sz)
{
    size_t readed = 0;

    while (fil->readU8() != 0)
        readed++;

    readed++;

    while (readed < sz)
    {
        uint16_t tag = fil->readU16L();
        uint32_t tagsz = fil->readU32L() - 6;

        readed += 6;

        if (tagsz > 100 * 1024 * 1024)
            break;

        switch(tag)
        {
        case 0x4100: //Trimesh
            readed += readChunkTrimesh(fil, tagsz);
            break;

        default:
            fil->seek(tagsz, SEEK_CUR);
            readed += tagsz;
            break;
        }
    }

    return readed;
}

size_t NC_STACK_3ds::readChunkTrimesh(FSMgr::FileHandle *fil, size_t sz)
{
    size_t readed = 0;

    while (readed < sz)
    {
        uint16_t tag = fil->readU16L();
        uint32_t tagsz = fil->readU32L() - 6;

        readed += 6;

        if (tagsz > 100 * 1024 * 1024)
            break;

        switch(tag)
        {
        case 0x4110: //VertexL
            readed += readChunkVertex(fil, tagsz);
            break;

        case 0x4120: //Faces
            readed += readChunkFaces(fil, tagsz);
            break;

        case 0x4140: //Vertex coords
        {
            printf("tex coords\n");
            uint16_t coNum = fil->readU16L();
            readed += 2;
            
            texCoords.resize(coNum);

            for(tUtV &uv : texCoords)
            {
                uv.tu = fil->readFloatL();
                uv.tv = fil->readFloatL();
                readed += 8;
            }
        }
        break;

        default:
            fil->seek(tagsz, SEEK_CUR);
            readed += tagsz;
            break;
        }
    }

    return readed;
}

size_t NC_STACK_3ds::readChunkVertex(FSMgr::FileHandle *fil, size_t sz)
{
    size_t readed = 0;

    uint16_t numvertex = fil->readU16L();
    readed += 2;

    printf("vtx %d\n", numvertex);

    _skeleton = Nucleus::CInit<NC_STACK_skeleton>( {
        {NC_STACK_rsrc::RSRC_ATT_NAME, std::string("3ds_sklt")},
        {NC_STACK_rsrc::RSRC_ATT_TRYSHARED, (int32_t)2},
        {NC_STACK_skeleton::SKEL_ATT_POINTSCNT, (int32_t)numvertex}});
        
    UAskeleton::Data *dat = _skeleton->GetSkelet();

    for (int i = 0; i < numvertex; i++)
    {
        dat->POO[i].x = fil->readFloatL();
        dat->POO[i].z = fil->readFloatL();
        dat->POO[i].y = -fil->readFloatL();
        dat->POO[i].flags = 0;

        readed += 12;
    }

    return readed;
}

size_t NC_STACK_3ds::readChunkFaces(FSMgr::FileHandle *fil, size_t sz)
{
    size_t readed = 0;

    uint16_t numfaces = fil->readU16L();
    readed += 2;
    printf("faces %d\n", numfaces);

    UAskeleton::Data *dat = _skeleton->GetSkelet();

    dat->polygons.resize(numfaces);

    faceMaterial.resize(numfaces);

    for (int i = 0; i < numfaces; i++)
    {
        dat->polygons[i].num_vertices = 3;
        dat->polygons[i].v[2] = fil->readU16L();
        dat->polygons[i].v[1] = fil->readU16L();
        dat->polygons[i].v[0] = fil->readU16L();

        //uint16_t diff = fil->readU16L();
        fil->readU16L();

        dat->polygons[i].A = 0;
        dat->polygons[i].B = 0;
        dat->polygons[i].C = 0;
        dat->polygons[i].D = 0;

        faceMaterial[i] = NULL;

        readed += 8;
    }

    while (readed < sz)
    {
        uint16_t tag = fil->readU16L();
        uint32_t tagsz = fil->readU32L() - 6;

        readed += 6;

        if (tagsz > 100 * 1024 * 1024)
            break;

        switch(tag)
        {
        case 0x4130: //FaceMaterial
        {
            std::string matName;
            readed += readName(fil, &matName, 64);

            d3dsMaterial *mat = findMaterial(matName);

            uint16_t n = fil->readU16L();
            readed += 2;

            for (int i = 0; i < n; i++)
            {
                uint16_t idx = fil->readU16L();
                readed += 2;

                faceMaterial[idx] = mat;
            }
        }
        break;

        default:
            fil->seek(tagsz, SEEK_CUR);
            readed += tagsz;
            break;
        }
    }

    faceNum = numfaces;

    return readed;
}

size_t NC_STACK_3ds::readChunkMaterial(FSMgr::FileHandle *fil, size_t sz)
{
    size_t readed = 0;

    materials.emplace_back();
    d3dsMaterial &mat = materials.back();

    while (readed < sz)
    {
        uint16_t tag = fil->readU16L();
        uint32_t tagsz = fil->readU32L() - 6;

        readed += 6;

        if (tagsz > 100 * 1024 * 1024)
            break;

        switch(tag)
        {
        case 0xA000: //Mat name
        {
            std::string matName;
            readed += readName(fil, &matName, 64);

            mat.name = matName;
        }
        break;

        case 0xA020: //Diffuse color
            readed += readChunkColor(mat.diffuse, fil, tagsz);
            break;

        case 0xA200: //Tex map
            readed += readChunkTexMap(mat.texture1_map, fil, tagsz);
            break;

        default:
            fil->seek(tagsz, SEEK_CUR);
            readed += tagsz;
            break;
        }
    }

    return readed;
}

size_t NC_STACK_3ds::readChunkTexMap(d3dsTextureMap &texmap, FSMgr::FileHandle *fil, size_t sz)
{
    size_t readed = 0;

    while (readed < sz)
    {
        uint16_t tag = fil->readU16L();
        uint32_t tagsz = fil->readU32L() - 6;

        readed += 6;

        if (tagsz > 100 * 1024 * 1024)
            break;

        switch(tag)
        {
        case 0xA300: //Mat name
        {
            std::string texName;
            readed += readName(fil, &texName, 64);

            texmap.name = texName;

            if (!texmap.tex)
            {
                std::string oldprefix = Common::Env.SetPrefix("rsrc", "");

                texmap.tex = Nucleus::CInit<NC_STACK_image>( {
                    {NC_STACK_rsrc::RSRC_ATT_NAME, std::string(texName)},
                    {NC_STACK_rsrc::RSRC_ATT_TRYSHARED, (int32_t)2},
                    {NC_STACK_bitmap::BMD_ATT_CONVCOLOR, (int32_t)1}} );

                Common::Env.SetPrefix("rsrc", oldprefix);
                printf("%s\n", texName.c_str());
            }
        }
        break;

        default:
            fil->seek(tagsz, SEEK_CUR);
            readed += tagsz;
            break;
        }
    }

    return readed;
}

size_t NC_STACK_3ds::readChunkColor(float colors[3], FSMgr::FileHandle *fil, size_t sz)
{
    size_t readed = 0;

    bool hasLin = false;

    while (readed < sz)
    {
        uint16_t tag = fil->readU16L();
        uint32_t tagsz = fil->readU32L() - 6;

        readed += 6;

        if (tagsz > 100 * 1024 * 1024)
            break;

        switch(tag)
        {
        case 0x0012: //LIN_COLOR_24
        {
            for (int i = 0; i < 3; i++)
                colors[i] = fil->readU8() / 255.0;

            readed += 3;
            hasLin = true;
        }
        break;

        case 0x0011: //COLOR_24
            if (!hasLin)
            {
                for (int i = 0; i < 3; i++)
                    colors[i] = fil->readU8() / 255.0;
                readed += 3;
            }
            else
            {
                fil->seek(tagsz, SEEK_CUR);
                readed += tagsz;
            }
            break;

        case 0x0013: //LIN_COLOR_F
        {
            for (int i = 0; i < 3; i++)
                colors[i] = fil->readFloatL();

            readed += 3 * 4;
            hasLin = true;
        }
        break;

        case 0x0010: //COLOR_F
            if (!hasLin)
            {
                for (int i = 0; i < 3; i++)
                    colors[i] = fil->readFloatL();
                readed += 3 * 4;
            }
            else
            {
                fil->seek(tagsz, SEEK_CUR);
                readed += tagsz;
            }
            break;

        default:
            fil->seek(tagsz, SEEK_CUR);
            readed += tagsz;
            break;
        }
    }

    return readed;
}

size_t NC_STACK_3ds::readName(FSMgr::FileHandle *fil, std::string *dst, size_t maxn)
{
    *dst = "";

    for(size_t i = 0; i < maxn; ++i)
    {
        char c = fil->readS8();
        if (!c)
            break;
        
        *dst += c;
    }

    return dst->size();
}

d3dsMaterial * NC_STACK_3ds::findMaterial(const std::string &matName)
{
    for (d3dsMaterial &mat : materials)
    {
        if ( StriCmp(mat.name, matName) == 0 )
            return &mat;
    }
    return NULL;
}

void NC_STACK_3ds::RecalcInternal(bool kids)
{
    NC_STACK_base::RecalcInternal(kids);
    
    if ( _skeleton )
    {
        UAskeleton::Data *skeldat = _skeleton->GetSkelet();
        
        for(int32_t i = 0; i < faceNum; ++i)
        {
            GFX::TRenderParams mat;
            if (faceMaterial[i])
                mat = GenRenderParams(faceMaterial[i]);
            
            GFX::TMesh *msh = NC_STACK_base::FindMeshByRenderParams(&Meshes, mat);
            
            if (!msh)
            {
                Meshes.emplace_back();
                msh = &Meshes.back();
                msh->Mat = mat;
            }
            
            UAskeleton::Polygon &pol = skeldat->polygons[ i ];

            uint32_t fid = msh->Vertexes.size();

            if (pol.num_vertices >= 3)
            {
                for(int j = 0; j < pol.num_vertices; ++j)
                {
                    msh->Vertexes.emplace_back();
                    msh->Vertexes.back().Pos = skeldat->POO[ pol.v[j] ];
                    msh->Vertexes.back().TexCoordId = j;
                    msh->Vertexes.back().Color = mat.Color;

                    msh->BoundBox.Add( skeldat->POO[ pol.v[j] ] );
                    
                    if (!texCoords.empty())
                        msh->Vertexes.back().TexCoord = texCoords.at(pol.v[j]);
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
}

GFX::TRenderParams NC_STACK_3ds::GenRenderParams(d3dsMaterial *mat)
{
    GFX::TRenderParams tmp;
    
    if (!mat)
        return tmp;
    
    tmp.Flags |= GFX::RFLAGS_SHADED;
    
    if (mat->texture1_map.tex)
    {
        tmp.Flags |= GFX::RFLAGS_TEXTURED;
        tmp.TexSource = mat->texture1_map.tex;
        
        if (mat->texture1_map.tex->IsDynamic())
            tmp.Flags |= GFX::RFLAGS_DYNAMIC_TEXTURE;
        else
            tmp.Tex = mat->texture1_map.tex->GetBitmap();
    }
    
    tmp.Color = GFX::TGLColor(mat->diffuse[0], mat->diffuse[1], mat->diffuse[2], 1.0 - mat->transparency);
    return tmp;
}
