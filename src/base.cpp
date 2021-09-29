#include <algorithm>

#include "includes.h"

#include "nucleas.h"

#include "utils.h"
#include "system/inpt.h"

#include "bitmap.h"
#include "base.h"
#include "skeleton.h"
#include "ade.h"
#include "embed.h"

#include "system/gfx.h"
#include "system/inivals.h"
#include "particle.h"

const Nucleus::ClassDescr NC_STACK_base::description("base.class", &NewInstance);

size_t NC_STACK_base::Init(IDVList &stak)
{
    if ( !NC_STACK_nucleus::Init(stak) )
        return 0;

    _ADES.clear();
    _KIDS.clear();

    _transform.Scale = vec3d(1.0, 1.0, 1.0);

    _transform.MakeScaleRotationMatrix();

    _visLimit = 4096;

    _renderMsg.fadeStart = 3496.0;
    _renderMsg.fadeLength = 600.0;
    _renderMsg.ambientLight = 255;

    ___svdFlags |= (FLAG_RENDERALL | FLAG_FOLLOWPARENT);
    _transform.flags |= TF::TForm3D::FLAG_FOLLOW_PARENT;

    return 1;
}

size_t NC_STACK_base::Deinit()
{
    if (_skeleton)
        Nucleus::Delete(_skeleton);

    while (!_ADES.empty())
    {
        NC_STACK_ade * ade = _ADES.front();
        _ADES.pop_front();
        
        ade->AttachedTo = NULL; //Clear ade attached, because we do erase in this list
        Nucleus::Delete(ade);
    }

    if ( _parent )
        _parent->_KIDS.remove(this);

    while (!_KIDS.empty())
    {
        NC_STACK_base * kid = _KIDS.front();
        _KIDS.pop_front();
        
        kid->_parent = NULL; //Clear kid parent field, because we do erase in this list
        Nucleus::Delete(kid);
    }

    if ( _embed )
        Nucleus::Delete(_embed);

    return NC_STACK_nucleus::Deinit();
}

int NC_STACK_base::ReadIFFTagSTRC(IFFile *mfile)
{
    int16_t readVersion;
    vec3d pos;
    vec3d scale;
    int16_t ax;
    int16_t ay;
    int16_t az;
    int32_t visLimit;
    int32_t ambientLight;

    readVersion = mfile->readS16B();

    pos.x = mfile->readFloatB();
    pos.y = mfile->readFloatB();
    pos.z = mfile->readFloatB();

    ___svdMove.x = mfile->readFloatB();
    ___svdMove.y = mfile->readFloatB();
    ___svdMove.z = mfile->readFloatB();

    scale.x = mfile->readFloatB();
    scale.y = mfile->readFloatB();
    scale.z = mfile->readFloatB();

    ax = mfile->readS16B();
    ay = mfile->readS16B();
    az = mfile->readS16B();

    ___svdRx = mfile->readS16B();
    ___svdRy = mfile->readS16B();
    ___svdRz = mfile->readS16B();

    ___svdFlags = mfile->readS16B();
    mfile->readS16B();

    visLimit = mfile->readS32B();
    ambientLight = mfile->readS32B();

    if (mfile->readErr())
        return 0;

    if ( readVersion >= 1 )
    {
        SetPosition(pos);
        SetScale(scale);
        SetEulerRotation(ax, ay, az);
        
        SetParentFollow( (___svdFlags & FLAG_FOLLOWPARENT) != 0 );

        SetVizLimit( visLimit );
        SetAmbientLight( ambientLight );
    }
    return 1;
}

int NC_STACK_base::ReadIFFTagADES(IFFile *mfile)
{
    while ( 1 )
    {
        int v5 = mfile->parse();
        if ( v5 == -2 )
            break;
        if ( v5 )
            return 0;

        const IFFile::Context &chunk = mfile->GetCurrentChunk();

        if ( chunk.Is(TAG_FORM, TAG_OBJT) )
        {
            NC_STACK_ade *ade = dynamic_cast<NC_STACK_ade *>(LoadObjectFromIFF(mfile));
            if ( !ade )
                return 0;

            SetAde(ade);
        }
        else
        {
            mfile->skipChunk();
        }
    }
    return 1;
}

int NC_STACK_base::ReadIFFTagKIDS(IFFile *mfile)
{
    int kidid = 0;

    while ( 1 )
    {
        int v5 = mfile->parse();
        if ( v5 == -2 )
            break;
        if ( v5 )
            return 0;

        const IFFile::Context &chunk = mfile->GetCurrentChunk();

        if ( chunk.Is(TAG_FORM, TAG_OBJT) )
        {
            NC_STACK_base *objt = dynamic_cast<NC_STACK_base *>( LoadObjectFromIFF(mfile) );
            if ( !objt )
            {
                return 0;
            }
            kidid++;

            AddKid(objt);
        }
        else
        {
            mfile->skipChunk();
        }
    }
    return 1;
}


size_t NC_STACK_base::LoadingFromIFF(IFFile **file)
{
    IFFile *mfile = *file;
    int obj_ok = 0;
    int STRC_readed = 0;

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



        if ( chunk.Is(TAG_FORM, TAG_ROOT) )
        {
            obj_ok = NC_STACK_nucleus::LoadingFromIFF(file);

            if ( !obj_ok )
                return 0;

            _ADES.clear();
            _KIDS.clear();

            _transform.Scale = vec3d(1.0, 1.0, 1.0);

//            kid_node.self_full = this;

            _transform.MakeScaleRotationMatrix();

            _visLimit = 4096;
            _renderMsg.fadeStart = 3496.0;
            _renderMsg.fadeLength = 600.0;
        }
        else if ( chunk.Is(TAG_STRC) )
        {
            STRC_readed = 1;

            if ( !ReadIFFTagSTRC(mfile) )
            {
                Deinit();
                return 0;
            }
            mfile->parse();
        }
        else if ( chunk.Is(TAG_FORM, TAG_OBJT) )
        {
            if ( obj_ok )
            {
                if ( STRC_readed )
                {
                    NC_STACK_skeleton *skel = dynamic_cast<NC_STACK_skeleton *>(LoadObjectFromIFF(mfile));
                    if ( !skel )
                    {
                        Deinit();
                        return 0;
                    }
                    SetSkeleton(skel);
                }
                else
                {
                    _embed = LoadObjectFromIFF(mfile);
                }
            }
        }
        else if ( chunk.Is(TAG_FORM, TAG_ADES) )
        {
            if ( !ReadIFFTagADES(mfile) )
            {
                Deinit();
                return 0;
            }
        }
        else if ( chunk.Is(TAG_FORM, TAG_KIDS) )
        {
            if ( !ReadIFFTagKIDS(mfile) )
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
    
    RecalcInternal();
    MakeCoordsCache();
    
    return obj_ok;
}

size_t NC_STACK_base::SavingIntoIFF(IFFile **file)
{
    IFFile *mfile = *file;

    if ( mfile->pushChunk(TAG_BASE, TAG_FORM, -1) )
        return 0;

    if ( !NC_STACK_nucleus::SavingIntoIFF(file) )
        return 0;

    mfile->pushChunk(0, TAG_STRC, -1);

    mfile->writeS16B(1); // version
    mfile->writeFloatB(_transform.Pos.x);
    mfile->writeFloatB(_transform.Pos.y);
    mfile->writeFloatB(_transform.Pos.z);

    mfile->writeFloatB(___svdMove.x);
    mfile->writeFloatB(___svdMove.y);
    mfile->writeFloatB(___svdMove.z);

    mfile->writeFloatB(_transform.Scale.x);
    mfile->writeFloatB(_transform.Scale.y);
    mfile->writeFloatB(_transform.Scale.z);

    mfile->writeS16B(_transform.ax);
    mfile->writeS16B(_transform.ay);
    mfile->writeS16B(_transform.az);

    mfile->writeS16B(___svdRx);
    mfile->writeS16B(___svdRy);
    mfile->writeS16B(___svdRz);

    mfile->writeS16B(___svdFlags);

    mfile->writeS16B(0); // _un1

    mfile->writeS32B(_renderMsg.fadeStart + _renderMsg.fadeLength); // visLimit
    mfile->writeS32B(_renderMsg.ambientLight); // ambientLight

    mfile->popChunk();

    if ( _skeleton )
    {
        if ( !_skeleton->SaveObjectIntoIFF(mfile) )
            return 0;

        if ( !_ADES.empty() )
        {
            mfile->pushChunk(TAG_ADES, TAG_FORM, -1);
            for (NC_STACK_ade *ade : _ADES)
            {
                if ( !ade->SaveObjectIntoIFF(mfile) )
                    return 0;
            }
            mfile->popChunk();
        }
    }

    if ( !_KIDS.empty() )
    {
        mfile->pushChunk(TAG_KIDS, TAG_FORM, -1);

        for (NC_STACK_base *kid : _KIDS)
        {
            if ( !kid->SaveObjectIntoIFF(mfile) )
                return 0;
        }
        mfile->popChunk();
    }

    return mfile->popChunk() == IFFile::IFF_ERR_OK;
}

// Push parent info to kid
void NC_STACK_base::AddKid(NC_STACK_base *kid)
{
    if ( _skeleton )
        kid->ChangeParentTo(this, &_transform);
    else
        kid->ChangeParentTo(this, NULL);
}

// Add object to parent kids list
void NC_STACK_base::ChangeParentTo(NC_STACK_base *parent, TF::TForm3D *parentTForm)
{
    if ( _parent )
        _parent->_KIDS.remove(this);

    _parent = parent;
    
    if ( _parent )
        _parent->_KIDS.push_back(this);

    _transform.Parent = parentTForm;
}

void NC_STACK_base::SetPosition(const vec3d &v, int flag)
{
    if (flag)
    {
        if ( flag & UF_X )
            _transform.Pos.x = v.x;
        if ( flag & UF_Y )
            _transform.Pos.y = v.y;
        if ( flag & UF_Z )
            _transform.Pos.z = v.z;
    }
}

void NC_STACK_base::ChangePosition(const vec3d &v, int flag)
{
    if (flag)
    {
        if ( flag & UF_X )
            _transform.Pos.x += v.x;
        if ( flag & UF_Y )
            _transform.Pos.y += v.y;
        if ( flag & UF_Z )
            _transform.Pos.z += v.z;
    }
}

void NC_STACK_base::SetEulerRotation(int32_t x, int32_t y, int32_t z, int flag)
{
    if (flag)
    {
        if ( flag & UF_X )
            _transform.ax = x;
        if ( flag & UF_Y )
            _transform.ay = y;
        if ( flag & UF_Z )
            _transform.az = z;
        
        _transform.MakeScaleRotationMatrix();
    }
}

void NC_STACK_base::ChangeEulerRotation(int32_t x, int32_t y, int32_t z, int flag)
{
    if (flag)
    {
        if ( flag & UF_X )
            _transform.ax += x;
        if ( flag & UF_Y )
            _transform.ay += y;
        if ( flag & UF_Z )
            _transform.az += z;
        
        _transform.MakeScaleRotationMatrix();
    }
}


void NC_STACK_base::SetScale(const vec3d &v, int flag)
{
    if ( flag )
    {
        if ( flag & UF_X )
            _transform.Scale.x = v.x;
        if ( flag & UF_Y )
            _transform.Scale.y = v.y;
        if ( flag & UF_Z )
            _transform.Scale.z = v.z;
        
        _transform.MakeScaleRotationMatrix();
    }
}

void NC_STACK_base::ChangeScale(const vec3d &v, int flag)
{
    if ( flag )
    {
        if ( flag & UF_X )
            _transform.Scale.x *= v.x;
        if ( flag & UF_Y )
            _transform.Scale.y *= v.y;
        if ( flag & UF_Z )
            _transform.Scale.z *= v.z;
        
        _transform.MakeScaleRotationMatrix();
    }
}

size_t NC_STACK_base::Render(baseRender_msg *arg, Instance * inst, bool doCopy /* = false */)
{
    int isVisible = 0;

    if ( _skeleton )
    {
        _transform.CalcGlobal();

        TF::TForm3D *view = TF::Engine.GetViewPoint();

        skeleton_arg_132 skel132;
        skel132.minZ = arg->minZ;
        skel132.maxZ = arg->maxZ;
        skel132.tform = view->CalcSclRot;

        if ( !(_transform.flags & TF::TForm3D::FLAG_NO_WRLD_ROT) )
            skel132.tform *= (_transform.TForm - view->CalcPos);
        else
            skel132.tform *= mat4x4(_transform.CalcPos - view->CalcPos);
        
        
        _renderMsg.timeStamp = arg->globTime;
        _renderMsg.frameTime = arg->frameTime;
        _renderMsg.minZ = arg->minZ;
        _renderMsg.maxZ = arg->maxZ;
        _renderMsg.ViewTForm = view;
        _renderMsg.OwnerTForm = &_transform;
        _renderMsg.flags = arg->flags;
        
        if (!inst)
            isVisible = _skeleton->skeleton_func132(&skel132);
        else
            isVisible = 1;
        
        float distance = skel132.tform.getTranslate().length();
        
        bool newsky = System::IniConf::GfxNewSky.Get<bool>();
        float transDist = System::IniConf::GfxSkyDistance.Get<int>();
        
        for(GFX::TMesh &msh : Meshes)
        {
            GFX::TRenderNode& rend = GFX::Engine.AllocRenderNode();
            rend = GFX::TRenderNode( GFX::TRenderNode::TYPE_MESH );

            rend.Distance = distance;
            rend.Color = msh.Mat.Color;
            rend.Flags = msh.Mat.Flags;
            
            if ((msh.Mat.Flags & GFX::RFLAGS_DYNAMIC_TEXTURE) && msh.Mat.TexSource)
            {
                msh.Mat.TexSource->SetTime(arg->globTime, arg->frameTime);
                uint32_t frameid = msh.Mat.TexSource->GetCurrentFrameID();
                
                if (frameid < msh.CoordsCache.size())
                {
                    rend.Tex = msh.CoordsCache.at(frameid).Tex;
                    rend.pCoords = &msh.CoordsCache.at(frameid).Coords;                    
                }
            }
            else
                rend.Tex = msh.Mat.Tex;
            
            rend.Flags |= arg->flags;
            
            if (newsky && !(rend.Flags & GFX::RFLAGS_IGNORE_FALLOFF))
            {
                if ( distance >= transDist ||
                     skel132.tform.Transform(msh.BoundBox.Min).XZ().length() >= transDist ||
                     skel132.tform.Transform(msh.BoundBox.Max).XZ().length() >= transDist )
                    rend.Flags |= GFX::RFLAGS_FALLOFF;
            }

            if (doCopy)
            {
                rend.LocalMesh = msh;
                rend.Mesh = &rend.LocalMesh;
            }
            else
                rend.Mesh = &msh;            
            
            rend.TForm = skel132.tform;
            rend.TimeStamp = arg->globTime;
            rend.FrameTime = arg->frameTime;
            rend.FogStart = _renderMsg.fadeStart;
            rend.FogLength = _renderMsg.fadeLength;

            GFX::GFXEngine::Instance.QueueRenderMesh(&rend);
        }

        if (inst)
        {
            for (NC_STACK_ade::InstanceOpts *ade : inst->Particles)
            {
                if (ade)
                    (static_cast<NC_STACK_particle*>(ade->Ade))->Emit(&_renderMsg, ade);
            }
        }
    }

    if (!inst || inst->Bas != this)
    {
        for (NC_STACK_base *kd : _KIDS)
        {
            if ( kd->Render(arg) )
                isVisible = 1;
        }
    }
    else
    {
        for (NC_STACK_base::Instance *kd : inst->KidsOpts)
        {
            if ( kd->Bas->Render(arg, kd) )
                isVisible = 1;
        }
    }

    return isVisible;
}

size_t NC_STACK_base::RenderImmediately(baseRender_msg *arg, Instance * inst)
{
    int v12 = 0;

    if ( _skeleton )
    {
        _transform.CalcGlobal();

        TF::TForm3D *view = TF::Engine.GetViewPoint();

        skeleton_arg_132 skel132;
        skel132.minZ = arg->minZ;
        skel132.maxZ = arg->maxZ;
        skel132.tform = view->CalcSclRot;

        if ( !(_transform.flags & TF::TForm3D::FLAG_NO_WRLD_ROT) )
            skel132.tform *= (_transform.TForm - view->CalcPos);
        else
            skel132.tform *= mat4x4(_transform.CalcPos - view->CalcPos);
        
        _renderMsg.timeStamp = arg->globTime;
        _renderMsg.frameTime = arg->frameTime;
        _renderMsg.minZ = arg->minZ;
        _renderMsg.maxZ = arg->maxZ;
        _renderMsg.ViewTForm = view;
        _renderMsg.OwnerTForm = &_transform;
        _renderMsg.flags = arg->flags;
        
        float distance = skel132.tform.Transform( _transform.Pos ).length();
        
        bool newsky = System::IniConf::GfxNewSky.Get<bool>();
        float transDist = System::IniConf::GfxSkyDistance.Get<int>();
        
        for(GFX::TMesh &msh : Meshes)
        {
            GFX::TRenderNode rend( GFX::TRenderNode::TYPE_MESH );
            rend.Distance = distance;
            rend.Color = msh.Mat.Color;
            rend.Flags = msh.Mat.Flags | arg->flags;
            
            if (newsky)
            {
                if ( skel132.tform.Transform(msh.BoundBox.Min + _transform.Pos).XZ().length() >= transDist ||
                     skel132.tform.Transform(msh.BoundBox.Max + _transform.Pos).XZ().length() >= transDist )
                    rend.Flags |= GFX::RFLAGS_FALLOFF;
            }

            rend.Mesh = &msh;            
            rend.TForm = skel132.tform;
            rend.TimeStamp = arg->globTime;
            rend.FrameTime = arg->frameTime;
            rend.FogStart = _renderMsg.fadeStart;
            rend.FogLength = _renderMsg.fadeLength;

            GFX::GFXEngine::Instance.RenderingMesh(&rend);
        }
        
        if (inst)
        {
            for (NC_STACK_ade::InstanceOpts *ade : inst->Particles)
            {
                if (ade)
                    (static_cast<NC_STACK_particle*>(ade->Ade))->Emit(&_renderMsg, ade);
            }
        }
    }

    if (!inst || inst->Bas != this)
    {
        for (NC_STACK_base *kd : _KIDS)
        {
            if ( kd->RenderImmediately(arg) )
                v12 = 1;
        }
    }
    else
    {
        for (NC_STACK_base::Instance *kd : inst->KidsOpts)
        {
            if ( kd->Bas->RenderImmediately(arg, kd) )
                v12 = 1;
        }
    }

    return v12;
}


void NC_STACK_base::SetSkeleton(NC_STACK_skeleton *skel)
{
    if (skel)
    {
        if ( _skeleton )
            Nucleus::Delete(_skeleton);

        _skeleton = skel;
        _renderMsg.sklt = skel->GetSkelet();
    }
}

void NC_STACK_base::SetAde(NC_STACK_ade *ade)
{
    if (ade)
        ade->ade_func64(_ADES);
}

void NC_STACK_base::SetParentFollow(bool follow)
{
    if ( follow )
    {
        ___svdFlags |= FLAG_FOLLOWPARENT;
        _transform.flags |= TF::TForm3D::FLAG_FOLLOW_PARENT;
    }
    else
    {
        ___svdFlags &= ~FLAG_FOLLOWPARENT;
        _transform.flags &= ~TF::TForm3D::FLAG_FOLLOW_PARENT;
    }
}

void NC_STACK_base::SetVizLimit(int32_t val)
{
    _visLimit = val;
    _renderMsg.fadeStart = val - _renderMsg.fadeLength;
}

void NC_STACK_base::SetAmbientLight(int32_t val)
{
    _renderMsg.ambientLight = val;
}

void NC_STACK_base::SetFadeLength(int32_t len)
{
    _renderMsg.fadeLength = len;
    _renderMsg.fadeStart = _visLimit - len;
}

void NC_STACK_base::SetStatic(bool stic)
{
    if (stic)
        _transform.flags |= TF::TForm3D::FLAG_NO_WRLD_ROT;
    else
        _transform.flags &= ~TF::TForm3D::FLAG_NO_WRLD_ROT;
}


NC_STACK_skeleton *NC_STACK_base::GetSkeleton()
{
    return _skeleton;
}

bool NC_STACK_base::IsParentFollow()
{
    return (_transform.flags & TF::TForm3D::FLAG_FOLLOW_PARENT) != 0;
}

int32_t NC_STACK_base::GetVisLimit()
{
    return _visLimit;
}

int32_t NC_STACK_base::GetAmbientLight()
{
    return _renderMsg.ambientLight;
}

vec3d NC_STACK_base::GetPos()
{
    return _transform.Pos;
}

vec3d NC_STACK_base::GetScale()
{
    return _transform.Scale;
}

vec3d NC_STACK_base::GetEulerAngles()
{
    return vec3d( (double)_transform.ax, (double)_transform.ay, (double)_transform.az );
}

vec3i NC_STACK_base::GetIntEulerAngles()
{
    return vec3i( _transform.ax, _transform.ay, _transform.az );
}

AdeList *NC_STACK_base::GetAdeList()
{
    return &_ADES;
}

TF::TForm3D &NC_STACK_base::TForm()
{
    return _transform;
}

BaseList &NC_STACK_base::GetKidList()
{
    return _KIDS;
}

area_arg_65 *NC_STACK_base::GetRenderParams()
{
    return &_renderMsg;
}

int32_t NC_STACK_base::GetFadeLength()
{
    return _renderMsg.fadeLength;
}

bool NC_STACK_base::IsStatic()
{
    return (_transform.flags & TF::TForm3D::FLAG_NO_WRLD_ROT) != 0;
}


NC_STACK_base *NC_STACK_base::LoadBaseFromFile(const std::string &fname)
{
    NC_STACK_base *result = NULL;

    IFFile mfile = IFFile::UAOpenIFFile(fname, "rb");
    if ( !mfile.OK() )
        return NULL;

    if ( !mfile.parse() )
    {
        if ( mfile.GetCurrentChunk().Is(TAG_FORM, TAG_MC2) && !mfile.parse() )
        {
            if ( mfile.GetCurrentChunk().Is(TAG_FORM, TAG_OBJT) )
                result = dynamic_cast<NC_STACK_base *>(LoadObjectFromIFF(&mfile));
        }
    }

    return result;
}


NC_STACK_base::Instance *NC_STACK_base::GenRenderInstance()
{
    Instance *tmp = new Instance(this);
    
    tmp->Particles.reserve(_ADES.size());
    tmp->KidsOpts.reserve(_KIDS.size());
    
    for(NC_STACK_base *bs : _KIDS)
        tmp->KidsOpts.emplace_back(bs->GenRenderInstance());
    
    for(NC_STACK_ade *ade : _ADES)
    {
        if (ade->IsParticle())
            tmp->Particles.emplace_back(ade->GenRenderInstance());
    }
        
    return tmp;
}

void NC_STACK_base::CheckOpts(Instance **vpOpts, NC_STACK_base *bas)
{
    if (vpOpts && bas)
    {
        if (!(*vpOpts))
        {
            *vpOpts = bas->GenRenderInstance();
            return;
        }
        
        if ((*vpOpts)->Bas != bas)
        {
            Common::DeleteAndNull(vpOpts);
            *vpOpts = bas->GenRenderInstance();
        }
    }
}

void NC_STACK_base::RecalcInternal(bool kids)
{
    Meshes.clear();
    
    if (_skeleton)
    {
        for(NC_STACK_ade *ade : _ADES)
        {
            if (!ade->IsParticle())
                ade->GenMesh(&Meshes, _skeleton);
        }
        
        if (kids)
        {
            for(NC_STACK_base *bs : _KIDS)
                bs->RecalcInternal(true);
        }
    }
}

void NC_STACK_base::ComputeStaticFog()
{
    const float fend = _visLimit;
    const float fstrt = _renderMsg.fadeStart;
    
    for (GFX::TMesh &m : Meshes)
    {
        if (m.Mat.Flags & GFX::RFLAGS_FOG)
        {
            for (GFX::TVertex &v : m.Vertexes)
            {
                float dist = v.Pos.length();
                float f = (fend - dist) / (fend - fstrt);
                if (f > 1.0) f = 1.0;
                if (f < 0.0) f = 0.0;

                v.ComputedColor.a = v.Color.a;
                v.ComputedColor.r = v.Color.r * f;
                v.ComputedColor.g = v.Color.g * f;
                v.ComputedColor.b = v.Color.b * f;        
            }
        }
        else
        {
            for (GFX::TVertex &v : m.Vertexes)
                v.ComputedColor = v.Color;
        }
    }
}


NC_STACK_base::Instance::~Instance()
{
    for(Instance *kd : KidsOpts)
        delete kd;
    
    for(NC_STACK_ade::InstanceOpts *opt : Particles)
        delete opt;
}

GFX::TMesh *NC_STACK_base::FindMeshByRenderParams(std::list<GFX::TMesh> *list, const GFX::TRenderParams &p)
{
    for(GFX::TMesh &m : *list)
    {
        if (m.Mat == p)
            return &m;
    }
    
    return NULL;
}

void NC_STACK_base::GenerateMeshCoordsCache(GFX::TMesh *mesh)
{
    if (!mesh)
        return;
    
    mesh->CoordsCache.clear();
 
    if ( !(mesh->Mat.Flags & GFX::RFLAGS_DYNAMIC_TEXTURE) || !mesh->Mat.TexSource )
        return;
    
    mesh->CoordsCache.resize( mesh->Mat.TexSource->GetFramesCount() );
    
    for (uint32_t i = 0; i < mesh->CoordsCache.size(); ++i)
    {
        mesh->CoordsCache[i].Tex = mesh->Mat.TexSource->GetBitmap(i);
        std::vector<tUtV> &srcCoords = mesh->Mat.TexSource->GetOutline(i);
        
        std::vector<tUtV> &cache = mesh->CoordsCache[i].Coords;
        cache.resize( mesh->Vertexes.size() );
        
        for(uint32_t j = 0; j < cache.size(); ++j)
            cache[j] = srcCoords.at( mesh->Vertexes[j].TexCoordId );
    }
}

void NC_STACK_base::MakeCoordsCache()
{
    for(GFX::TMesh &m : Meshes)
        GenerateMeshCoordsCache( &m );
}

TObjectCache *NC_STACK_base::MakeCache()
{
    TObjectCache *cache = new TObjectCache();
    MakeCache(cache);
    return cache;
}

void NC_STACK_base::MakeCache(TObjectCache *cache)
{
    cache->Meshes = Meshes;
    cache->Transform = _transform;
    cache->Transform.Parent = NULL;
    
    cache->fadeStart = _renderMsg.fadeStart;
    cache->fadeLength = _renderMsg.fadeLength;
}


void TObjectCache::Render(baseRender_msg *arg)
{
    if (Meshes.empty())
        return;
    
    Transform.CalcGlobal();
    
    TF::TForm3D *view = TF::Engine.GetViewPoint();
    
    mat4x4 tf = view->CalcSclRot;
    
    if ( !(Transform.flags & TF::TForm3D::FLAG_NO_WRLD_ROT) )
        tf *= (Transform.TForm - view->CalcPos);
    else
        tf *= mat4x4(Transform.CalcPos - view->CalcPos);

    float distance = tf.Transform( Transform.Pos ).length();

    bool newsky = System::IniConf::GfxNewSky.Get<bool>();
    float transDist = System::IniConf::GfxSkyDistance.Get<int>();
        
    for(GFX::TMesh &msh : Meshes)
    {
        GFX::TRenderNode& rend = GFX::Engine.AllocRenderNode();
        rend = GFX::TRenderNode( GFX::TRenderNode::TYPE_MESH );

        rend.Distance = distance;
        rend.Color = msh.Mat.Color;
        rend.Flags = msh.Mat.Flags;

        if ((msh.Mat.Flags & GFX::RFLAGS_DYNAMIC_TEXTURE) && msh.Mat.TexSource)
        {
            msh.Mat.TexSource->SetTime(arg->globTime, arg->frameTime);
            uint32_t frameid = msh.Mat.TexSource->GetCurrentFrameID();

            if (frameid < msh.CoordsCache.size())
            {
                rend.Tex = msh.CoordsCache.at(frameid).Tex;
                rend.pCoords = &msh.CoordsCache.at(frameid).Coords;                    
            }
        }
        else
            rend.Tex = msh.Mat.Tex;

        rend.Flags |= arg->flags;

        if (newsky && !(rend.Flags & GFX::RFLAGS_IGNORE_FALLOFF))
        {
            if ( distance >= transDist ||
                 tf.Transform(msh.BoundBox.Min).XZ().length() >= transDist ||
                 tf.Transform(msh.BoundBox.Max).XZ().length() >= transDist )
                rend.Flags |= GFX::RFLAGS_FALLOFF;
        }

        rend.Mesh = &msh;
        rend.TForm = tf;
        rend.TimeStamp = arg->globTime;
        rend.FrameTime = arg->frameTime;
        rend.FogStart = fadeStart;
        rend.FogLength = fadeLength;

        GFX::GFXEngine::Instance.QueueRenderMesh(&rend);
    }
}