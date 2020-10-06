#include "3ds.h"

#include "image.h"
#include "display.h"
#include "win3d.h"
#include "glob_funcs.h"


const Nucleus::ClassDescr NC_STACK_3ds::description("3ds.class", &newinstance);

NC_STACK_3ds::NC_STACK_3ds()
{
    texCoords = NULL;
    faceMaterial = NULL;
    faceNum = 0;
}

NC_STACK_3ds::~NC_STACK_3ds()
{
    if (texCoords)
        delete [] texCoords;

    if (faceMaterial)
        delete [] faceMaterial;

    for (std::list<d3dsMaterial *>::iterator it = materials.begin(); it != materials.end(); it++)
        delete *it;
}

size_t NC_STACK_3ds::func0(IDVList &stak)
{
    if ( !NC_STACK_base::func0(stak) )
        return 0;

    return 1;
}

size_t NC_STACK_3ds::func1()
{
    return NC_STACK_base::func1();
}

bool NC_STACK_3ds::readFile(const char *filename)
{
    FSMgr::FileHandle *fil = FSMgr::iDir::openFile(filename, "rb");

    if (!fil)
        return false;

    if (fil->readU16L() != 0x4D4D)
    {
        delete fil;
        return false;
    }

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


    return true;
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
            if (texCoords)
                delete [] texCoords;

            texCoords = new tUtV[coNum];

            for(int i = 0; i < coNum; i++)
            {
                texCoords[i].tu = fil->readFloatL();
                texCoords[i].tv = fil->readFloatL();
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

    OBJ_SKELETON = Nucleus::CInit<NC_STACK_skeleton>( {
        {NC_STACK_rsrc::RSRC_ATT_NAME, std::string("3ds_sklt")},
        {NC_STACK_rsrc::RSRC_ATT_TRYSHARED, (int32_t)2},
        {NC_STACK_skeleton::SKEL_ATT_POINTSCNT, (int32_t)numvertex}});
        
    UAskeleton::Data *dat = OBJ_SKELETON->GetSkelet();

    for (int i = 0; i < numvertex; i++)
    {
        dat->POO[i].x = fil->readFloatL() * 0.1;
        dat->POO[i].z = fil->readFloatL() * 0.1;
        dat->POO[i].y = -fil->readFloatL() * 0.1;
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

    UAskeleton::Data *dat = OBJ_SKELETON->GetSkelet();

    skeleton_130_arg arg;
    arg.skeleton = dat;
    arg.pol_count = numfaces;

    OBJ_SKELETON->skeleton_func130(&arg);

    if (faceMaterial)
        delete [] faceMaterial;

    faceMaterial = new d3dsMaterial *[numfaces];

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
            char matName[64];
            readed += readName(fil, matName, 64);

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

    d3dsMaterial *mat = new d3dsMaterial;
    materials.push_back(mat);

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
            char matName[64];
            readed += readName(fil, matName, 64);

            mat->name = matName;
        }
        break;

        case 0xA020: //Diffuse color
            readed += readChunkColor(mat->diffuse, fil, tagsz);
            break;

        case 0xA200: //Tex map
            readed += readChunkTexMap(mat->texture1_map, fil, tagsz);
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
            char texName[64];
            readed += readName(fil, texName, 64);

            texmap.name = texName;

            if (!texmap.tex)
            {
                std::string oldprefix = get_prefix_replacement("rsrc");
                set_prefix_replacement("rsrc", "");

                texmap.tex = Nucleus::CInit<NC_STACK_image>( {
                    {NC_STACK_rsrc::RSRC_ATT_NAME, std::string(texName)},
                    {NC_STACK_rsrc::RSRC_ATT_TRYSHARED, (int32_t)2},
                    {NC_STACK_bitmap::BMD_ATT_CONVCOLOR, (int32_t)1}} );

                set_prefix_replacement("rsrc", oldprefix.c_str());
                printf("%s\n", texName);
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

size_t NC_STACK_3ds::readName(FSMgr::FileHandle *fil, char *dst, size_t maxn)
{
    size_t rd = 0;

    while(rd < maxn)
    {
        char c = fil->readS8();
        dst[rd] = c;
        rd++;

        if (!c)
            break;
    }

    return rd;
}

d3dsMaterial * NC_STACK_3ds::findMaterial(const char *name)
{
    for (std::list<d3dsMaterial *>::iterator it = materials.begin(); it != materials.end(); it++)
    {
        if ( strcmp( (*it)->name.c_str(), name ) == 0 )
            return *it;
    }
    return NULL;
}

size_t NC_STACK_3ds::base_func77(baseRender_msg *arg)
{
    int v12 = 0;

    if ( OBJ_SKELETON )
    {
        UAskeleton::Data *skeldat = OBJ_SKELETON->GetSkelet();

        if ( !(flags & FLAG_MAINOBJT) )
        {
            transform.CalcGlobal();

            TFEngine::TForm3D *view = TFEngine::Engine.GetViewPoint();

            skeleton_arg_132 skel132;
            skel132.minZ = arg->minZ;
            skel132.maxZ = arg->maxZ;
            skel132.tform = view->globSclRot;

            if ( !(transform.flags & TFEngine::TForm3D::FLAG_NO_WRLD_ROT) )
                skel132.tform *= (transform.tform - view->globPos);
            else
                skel132.tform *= mat4x4(transform.globPos - view->globPos);

            v12 = OBJ_SKELETON->skeleton_func132(&skel132);

            if ( v12 )
            {
                renderMsg.ownerID = arg->ownerID;
                renderMsg.timeStamp = arg->globTime;
                renderMsg.frameTime = arg->frameTime;
                renderMsg.minZ = arg->minZ;
                renderMsg.maxZ = arg->maxZ;
                renderMsg.rndrStack = arg->rndrStack;
                renderMsg.view = view;
                renderMsg.owner = &transform;
                renderMsg.flags = arg->flags;

                renderMsg.OBJ_SKELETON = OBJ_SKELETON;
                renderMsg.adeCount = 0;

                for (int32_t i = 0; i < faceNum; i++)
                {
                    int renderFlags = NC_STACK_display::RFLAGS_LINMAP | NC_STACK_display::RFLAGS_GRADSHD;
                    //int renderFlags = NC_STACK_display::RFLAGS_GRADSHD;

                    skeleton_arg133 skel133;

                    skel133.field_4 = 1 | 2;
                    skel133.minZ = arg->minZ;
                    skel133.maxZ = arg->maxZ;
                    skel133.fadeStart = renderMsg.fadeStart;
                    skel133.fadeLength = renderMsg.fadeLength;

                    ResBitmap *bitm = NULL;

                    if ( faceMaterial[i] )
                    {
                        if (faceMaterial[i]->texture1_map.tex)
                        {
                            bitmap_arg130 bitm130;

                            bitm130.time_stmp = arg->globTime;
                            bitm130.frame_time = arg->frameTime;
                            faceMaterial[i]->texture1_map.tex->bitmap_func130(&bitm130);
                            bitm = bitm130.pbitm;
                        }
                    }

                    polysDat *data = arg->rndrStack->get();
                    polysDatSub *datSub = &data->datSub;

                    datSub->renderFlags = renderFlags;

                    if (faceMaterial[i])
                    {
                        datSub->r = faceMaterial[i]->diffuse[0];
                        datSub->g = faceMaterial[i]->diffuse[1];
                        datSub->b = faceMaterial[i]->diffuse[2];
                    }
                    else
                    {
                        datSub->r = 1.0;
                        datSub->g = 1.0;
                        datSub->b = 1.0;
                    }
                    datSub->pbitm = bitm;

                    skel133.polyID = i;
                    skel133.rndrArg = datSub;
                    skel133.shadeVal = 0;

                    tUtV coo[3];
                    for (int j = 0; j < 3; j++)
                        if (texCoords)
                            coo[j] = texCoords[ skeldat->polygons[i].v[j] ];
                        else
                        {
                            coo[j].tu = 0.0;
                            coo[j].tv = 0.0;
                        }

                    skel133.texCoords = coo;

                    if (OBJ_SKELETON->skeleton_func133(&skel133))
                    {
                        arg->adeCount++;

                        if ( datSub->renderFlags & ( NC_STACK_display::RFLAGS_FLATSHD | NC_STACK_display::RFLAGS_GRADSHD ) )
                        {
                            int v6 = 0;
                            int v8 = 0;

                            for (int j = 0; j < datSub->vertexCount; j++)
                            {
                                float clr = datSub->color[j];
                                if (clr < 0.01)
                                    v6++;
                                else if (clr > 0.99)
                                    v8++;
                            }

                            if ( v6 == datSub->vertexCount )
                            {
                                datSub->renderFlags &= ~( NC_STACK_display::RFLAGS_FLATSHD | NC_STACK_display::RFLAGS_GRADSHD );
                            }
                            else if ( v8 == datSub->vertexCount )
                            {
                                datSub->renderFlags = 0;
                            }
                        }

                        datSub->renderFlags |= (arg->flags & NC_STACK_display::RFLAGS_SKY);

                        float maxz = 0.0;

                        for (int j = 0; j < datSub->vertexCount; j++)
                            if (datSub->vertexes[j].z > maxz)
                                maxz = datSub->vertexes[j].z;

                        if ( !(arg->flags & NC_STACK_display::RFLAGS_IGNORE_FALLOFF) && NC_STACK_win3d::win3d_keys[18].Get<bool>() )
                        {
                            float maxln = 0.0;

                            for (int j = 0; j < datSub->vertexCount; j++)
                            {
                                datSub->distance[j] = sqrt(POW2(datSub->vertexes[j].x) + POW2(datSub->vertexes[j].z));
                                if (datSub->distance[j] > maxln)
                                    maxln = datSub->distance[j];
                            }

                            if (maxln > NC_STACK_win3d::win3d_keys[19].Get<int>())
                                datSub->renderFlags |= NC_STACK_display::RFLAGS_FALLOFF;
                        }

                        data->render_func = GFXEngine::defRenderFunc;
                        data->range = maxz;

                        arg->rndrStack->commit();
                    }
                }
            }
        }
    }

    return v12;
}
