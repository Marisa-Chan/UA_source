#include "obj3d.h"
#include "image.h"
#include "env.h"
#include "system/inivals.h"



const Nucleus::ClassDescr NC_STACK_Obj3D::description("Obj3D.class", &newinstance);

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

    return LoadFromFile(&fil);
}

bool NC_STACK_Obj3D::LoadFromFile(FSMgr::FileHandle *fil)
{
    if (!ParseObj(fil))
        return false;
    
    _skeleton = Nucleus::CInit<NC_STACK_skeleton>( {
        {NC_STACK_rsrc::RSRC_ATT_NAME, std::string("Obj3D_sklt")},
        {NC_STACK_rsrc::RSRC_ATT_TRYSHARED, (int32_t)2},
        {NC_STACK_skeleton::SKEL_ATT_POINTSCNT, (int32_t)_v.size()}});
        
    Meshes.emplace_back();
    GFX::TMesh *msh = &Meshes.back();
    
    for (int32_t i = 0; i < (int32_t)_f.size(); i++)
    {
        int num_vertices = GFX_MAX_VERTEX;
        
        for (int j = 0; j < GFX_MAX_VERTEX; ++j )
        {
            FaceVT &f = _f[i].at(j);
            if (f.V == 0)
            {
                num_vertices = j;
                break;
            }
        }
        
        uint32_t fid = msh->Vertexes.size();
        
        if (num_vertices >= 3)
        {
            for(int j = 0; j < num_vertices; ++j)
            {
                FaceVT &f = _f[i].at(j);
                
                msh->Vertexes.emplace_back();
                msh->Vertexes.back().Pos = _v[ f.V - 1 ];
                msh->Vertexes.back().TexCoordId = j;
                msh->Vertexes.back().Color = msh->Mat.Color;

                msh->BoundBox.Add( _v[ f.V - 1 ] );

                /*if (!texCoords.empty())
                    msh->Vertexes.back().TexCoord = texCoords.at(pol.v[j]);*/
            }

            for(int j = 2; j < num_vertices; ++j)
            {
                msh->Indixes.push_back(fid + 0);
                msh->Indixes.push_back(fid + j);
                msh->Indixes.push_back(fid + j - 1);
            }
        }
        
    }
    
    
    
    
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
    if(tmp->LoadFromFile(&fil))
        return tmp;
    
    Nucleus::Delete(tmp);
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
            
            for (int32_t i = 1; i < (int32_t)tokens.size(); ++i )
                f[i - 1] = ParseFaceVT(tokens[i]);
        }
        else
        {
            printf("unk line %s\n", line.c_str());
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
    
    for(std::string &str : t)
    {
        if (str.empty())
            str = "0";
    }
    
    return FaceVT( std::stoi(t[0]), std::stoi(t[1]), std::stoi(t[2]) );
}
        