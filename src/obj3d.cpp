#include "obj3d.h"
#include "image.h"
#include "env.h"
#include "system/inivals.h"
#include "loaders.h"
#include "fmtlib/printf.h"


NC_STACK_Obj3D::NC_STACK_Obj3D()
{}

NC_STACK_Obj3D::~NC_STACK_Obj3D()
{}

size_t NC_STACK_Obj3D::Init(IDVList &stak)
{
    if ( !NC_STACK_base::Init(stak) )
        return 0;

    return 1;
}

size_t NC_STACK_Obj3D::Deinit()
{
    return NC_STACK_base::Deinit();
}

bool NC_STACK_Obj3D::LoadFromFile(const std::string &filename)
{
    FSMgr::FileHandle fil = FSMgr::iDir::openFile(filename, "r");

    if (!fil.OK())
        return false;
    
    fname = filename;

    return LoadFromFile(&fil);
}

bool NC_STACK_Obj3D::LoadFromFile(FSMgr::FileHandle *fil)
{
    if (!ParseObj(fil))
        return false;

    MakeCoordsCache();
        
    _skeleton = Nucleus::CInit<NC_STACK_skeleton>( {
        {NC_STACK_rsrc::RSRC_ATT_NAME, std::string("Obj3D_sklt")},
        {NC_STACK_rsrc::RSRC_ATT_TRYSHARED, (int32_t)2},
        {NC_STACK_skeleton::SKEL_ATT_POINTSCNT, (int32_t)_v.size()}});  
    
    UAskeleton::Data *dat = _skeleton->GetSkelet();

    for (int32_t i = 0; i < (int32_t)_v.size(); ++i)
        dat->POO[i] = _v[i];
    
    dat->polygons.resize(_f.size());

    for (int32_t i = 0; i < (int32_t)_f.size(); i++)
    {
        UAskeleton::Polygon &pol = dat->polygons[i];
        
        for (int j = 0; j < GFX_MAX_VERTEX; ++j )
        {
            FaceVT &f = _f[i].at(j);
            if (f.V == 0)
            {
                break;
            }
            
            pol.v[j] = f.V - 1;
            pol.num_vertices = j + 1;
        }
    }
    
    return true;
}

NC_STACK_Obj3D *NC_STACK_Obj3D::LoadObj3D(const std::string &filename)
{
    FSMgr::FileHandle fil = uaOpenFile(filename, "r");
    if (!fil.OK())
        return NULL;
    
    NC_STACK_Obj3D *tmp = Nucleus::CInit<NC_STACK_Obj3D>();
    tmp->fname = filename;
    if(tmp->LoadFromFile(&fil))
        return tmp;
    
    tmp->Delete();
    return NULL;
}

bool NC_STACK_Obj3D::ParseObj(FSMgr::FileHandle *fil)
{
    if (!fil->OK())
        return false;
    
    std::string line;
    while ( fil->ReadLine(&line) && !line.empty() )
    {
        size_t pos = line.find_first_of("#\n\r");
        if ( pos != std::string::npos )
            line.erase(pos);

        if (line.empty())
            continue;
        
        std::vector<std::string> tokens = Stok::Split(line, " \t");
        
        if (tokens.empty())
            continue;
        
        if (!StriCmp(tokens[0], "v"))
        {
            _v.emplace_back( std::stof(tokens[1]), -std::stof(tokens[2]), std::stof(tokens[3]) );
        }
        else if (!StriCmp(tokens[0], "vt"))
        {
            if (tokens.size() == 3)
                _vt.emplace_back( std::stof(tokens[1]), std::stof(tokens[2]), 0.0 );
            else 
                _vt.emplace_back( std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]) );
        }
        else if (!StriCmp(tokens[0], "vn"))
        {
            _vn.emplace_back( std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]) );
        }
        else if (!StriCmp(tokens[0], "f"))
        {
            _f.emplace_back();            
            auto &f = _f.back();
            
            int numvtx = tokens.size() - 1;
            
            for (int32_t i = 1; i < (int32_t)tokens.size(); ++i )
                f[i - 1] = ParseFaceVT(tokens[i]);
            
            if (numvtx >= 3)
            {
                GFX::TRenderParams prm;
                if (_currentMtl)
                    prm = _currentMtl->GenParams();
                else
                    prm.Flags |= GFX::RFLAGS_FOG | GFX::RFLAGS_SHADED;

                GFX::TMesh *msh = FindMeshByRenderParams(&Meshes, prm);
                if (!msh)
                {
                    Meshes.emplace_back();
                    msh = &Meshes.back();
                    msh->Mat = prm;
                }
                
                uint32_t fid = msh->Vertexes.size();
                
                for(int j = 0; j < numvtx; ++j)
                {
                    FaceVT &ftx = f[j];
                    msh->Vertexes.emplace_back();
                    msh->Vertexes.back().Pos = _v.at(ftx.V - 1);
                    msh->Vertexes.back().TexCoordId = j;
                    msh->Vertexes.back().Color = prm.Color;

                    msh->BoundBox.Add( _v.at(ftx.V - 1) );

                    if (ftx.T > 0)
                        msh->Vertexes.back().TexCoord = tUtV(_vt.at(ftx.T - 1).x, 1.0 - _vt.at(ftx.T - 1).y);
                }

                for(int j = 2; j < numvtx; ++j)
                {
                    msh->Indixes.push_back(fid + 0);
                    msh->Indixes.push_back(fid + j);
                    msh->Indixes.push_back(fid + j - 1);
                }
            }
        }
        else if (!StriCmp(tokens[0], "mtllib"))
        {
            ParseMtl(tokens[1]);
        }
        else if (!StriCmp(tokens[0], "usemtl"))
        {
            _currentMtl = FindMtl(tokens[1]);
        }
    }
    return true;
}


NC_STACK_Obj3D::FaceVT NC_STACK_Obj3D::ParseFaceVT(const std::string &l)
{
    int s = 0;
    std::string t[3];
    for(auto c : l)
    {
        if (c == '/')
            s++;
        else
            t[s] += c;
    }
    
    int32_t v[3] {0, 0, 0};

    for(int i = 0; i < 3; ++i)
    {
        if (!t[i].empty())
            v[i] = std::stoi(t[i]);
    }
    
    return FaceVT( v[0], v[1], v[2] );
}
        

bool NC_STACK_Obj3D::ParseMtl(const std::string &filename)
{
    std::string fl = filename;
    if (!uaFileExist(fl))
    {
        if (fl.size() > 2 && fl[0] == '.' && fl[1] == '/')
            fl = fl.substr(2);
        
        if (!uaFileExist(fl))
        {
            size_t pos = fname.find_last_of("/\\:");
            if (pos != std::string::npos)
                fl = fname.substr(0, pos + 1) + fl;
            else
                fl = fname + fl;
        }
    }
    
    FSMgr::FileHandle fil = uaOpenFile(fl, "r");
    if (!fil.OK())
        return false;
    
    Mtl *current = NULL;
    
    std::string line;
    while ( fil.ReadLine(&line) && !line.empty() )
    {
        size_t pos = line.find_first_of("#\n\r");
        if ( pos != std::string::npos )
            line.erase(pos);

        if (line.empty())
            continue;
        
        std::vector<std::string> tokens = Stok::Split(line, " \t");
        
        if (tokens.empty())
            continue;
        
        if (!StriCmp(tokens[0], "newmtl"))
        {
            _mtls.emplace_back();
            current = &_mtls.back();
            current->name = tokens[1];
        }
        else if (!StriCmp(tokens[0], "Ns"))
        {
            
        }
        else if (!StriCmp(tokens[0], "Ka"))
        {
            
        }
        else if (!StriCmp(tokens[0], "Kd"))
        {
            if (current && tokens.size() >= 4)
            {
                current->diffuse[0] = std::stof(tokens[1]);
                current->diffuse[1] = std::stof(tokens[2]);
                current->diffuse[2] = std::stof(tokens[3]);
            }
        }
        else if (!StriCmp(tokens[0], "Ks"))
        {

        }
        else if (!StriCmp(tokens[0], "Ke"))
        {

        }
        else if (!StriCmp(tokens[0], "Ni"))
        {

        }
        else if (!StriCmp(tokens[0], "Ns"))
        {

        }
        else if (!StriCmp(tokens[0], "d"))
        {
            if (current)
                current->d = std::stof(tokens[1]);
        }
        else if (!StriCmp(tokens[0], "illum"))
        {

        }
        else if (!StriCmp(tokens[0], "map_Kd"))
        {
            if (current)
            {
                std::string txname = tokens[1];
                if (!uaFileExist(std::string("rsrc:") + txname))
                {
                    txname = "objects/" + tokens[1];
                    
                    if (!uaFileExist(std::string("rsrc:") + txname))
                        txname = "rsrcpool/" + tokens[1];
                }
                
                current->diffuseMap = Utils::ProxyLoadImage( {
                    {NC_STACK_rsrc::RSRC_ATT_NAME, txname},
                    {NC_STACK_rsrc::RSRC_ATT_TRYSHARED, (int32_t)2},
                    {NC_STACK_bitmap::BMD_ATT_CONVCOLOR, (int32_t)1}} );
                    
                if (current->diffuseMap)
                    current->diffuseMap->PrepareTexture();
                    
            }
        }
        else if (!StriCmp(tokens[0], "map_Ks"))
        {

        }
        else if (!StriCmp(tokens[0], "map_Ke"))
        {

        }
        else if (!StriCmp(tokens[0], "norm"))
        {

        }
        else if (!StriCmp(tokens[0], "norm"))
        {

        }
        else if (!StriCmp(tokens[0], "map_bump") || !StriCmp(tokens[0], "bump"))
        {

        }
        else if (!StriCmp(tokens[0], "map_d"))
        {

        }
        else if (!StriCmp(tokens[0], "Tr"))
        {

        }
    }
    
    return true;
}

NC_STACK_Obj3D::Mtl *NC_STACK_Obj3D::FindMtl(const std::string &mtlname)
{
    for(Mtl &m : _mtls)
    {
        if (!StriCmp(m.name, mtlname))
            return &m;
    }
    return NULL;
}




GFX::TRenderParams NC_STACK_Obj3D::Mtl::GenParams() const
{
    GFX::TRenderParams tmp;
    tmp.Flags = GFX::RFLAGS_SHADED | GFX::RFLAGS_FOG;
    
    if (diffuseMap)
    {
        tmp.Flags |= GFX::RFLAGS_TEXTURED;
        
        if (diffuseMap->IsDynamic())
        {
            tmp.Flags |= GFX::RFLAGS_DYNAMIC_TEXTURE;
            tmp.TexSource = diffuseMap;
        }
        else
            tmp.Tex = diffuseMap->GetBitmap();
    }
    
    tmp.Color = GFX::TGLColor(diffuse[0], diffuse[1], diffuse[2], d);
    
    if (d < 1.0)
        tmp.Flags |= GFX::RFLAGS_LUMTRACY;
    
    return tmp;
}


bool NC_STACK_Obj3D::ExportObj(NC_STACK_base *base, const std::string &fname)
{
    if (!base)
        return false;
    
    if (base->Meshes.empty())
        return false;
    
    FSMgr::FileHandle mtl = uaOpenFile(fname + ".mtl", "w");
    
    
    FSMgr::FileHandle obj = uaOpenFile(fname, "w");
    
    obj.puts(std::string("mtllib ") + fname + ".mtl\n\n");
    
    int vid = 1;
    int tvid = 1;
    
    int matID = 0;
    for( GFX::TMesh &m : base->Meshes )
    {
        mtl.printf("newmtl Mat%d\n", matID);
        mtl.printf("Kd %f %f %f\n", m.Mat.Color.r, m.Mat.Color.g, m.Mat.Color.b);
        mtl.puts("\
Ka 1.000000 1.000000 1.000000\n\
Ks 0.500000 0.500000 0.500000\n\
Ke 0.000000 0.000000 0.000000\n\
Ni 1.450000\n");
        mtl.printf("d %f\n", m.Mat.Color.a);
        mtl.puts("illum 2\n");
        
        if (m.Mat.TexSource || m.Mat.Tex)
        {
            std::string texName = fmt::sprintf("Tex%d.png", matID);
            if (m.Mat.Tex)
                NC_STACK_image::SavePng(m.Mat.Tex, texName);
            else
                NC_STACK_image::SavePng(m.Mat.TexSource->GetBitmap(), texName);
            
            mtl.printf("map_Kd %s\n\n", texName.c_str());
        }
        else
            mtl.puts("\n");
        
        for(GFX::TVertex &v : m.Vertexes)
            obj.printf("v %f %f %f\n", v.Pos.x, -v.Pos.y, v.Pos.z);
        
        if (m.Mat.Flags & GFX::RFLAGS_TEXTURED)
        {
            for(GFX::TVertex &v : m.Vertexes)
                obj.printf("vt %f %f\n", v.TexCoord.tu, 1.0 - v.TexCoord.tv);
        }
        
        obj.printf("usemtl Mat%d\n", matID);
        
        for (uint32_t i = 0; i < m.Indixes.size(); i += 3)
        {
            if (m.Mat.Flags & GFX::RFLAGS_TEXTURED)
                obj.printf("f %d/%d %d/%d %d/%d\n", vid + m.Indixes[i], tvid + m.Indixes[i],
                        vid + m.Indixes[i + 2], tvid + m.Indixes[i + 2],
                        vid + m.Indixes[i + 1], tvid + m.Indixes[i + 1]);
            else
                obj.printf("f %d %d %d\n", vid + m.Indixes[i],
                        vid + m.Indixes[i + 2],
                        vid + m.Indixes[i + 1]);
        }
        
        vid += m.Vertexes.size();  
        if (m.Mat.Flags & GFX::RFLAGS_TEXTURED)
            tvid += m.Vertexes.size();  
        ++matID;
    }
    return true;
}