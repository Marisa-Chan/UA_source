#include "nparticle.h"
#include "../system/inivals.h"
#include "../skeleton.h"
#include "../ade.h"
#include "../particle.h"
#include "../base.h"

namespace World
{

ParticleSystem::ParticleSystem()
{
}
    
void ParticleSystem::AddParticle(NC_STACK_particle *base, const vec3d& pos, const vec3d& vec, int32_t age)
{
    if (!_disableAdd && (int32_t)_particles.size() < System::IniConf::ParticlesLimit.Get<int32_t>())
        _particles.emplace_back( base, pos, vec, age );
}

void ParticleSystem::UpdateRender(area_arg_65 *rndrParams, int32_t delta)
{
    //Prevent spawn new particles from particle render
    _disableAdd = true;
    
    float fsec = (float)delta * 0.001;
    
    for(auto it = _particles.begin(); it != _particles.end();)
    {
        Frak &f = *it;
        if (!f.pParticleGen)
        {
            it = _particles.erase(it);
            continue;
        }
        
        f.Age += delta;
        
        if (f.Age >= f.pParticleGen->_lifeTime)
        {
            it = _particles.erase(it);
            continue;
        }
        
        f.Vec += f.pParticleGen->_accelStart + f.pParticleGen->_accelDelta * f.Age;
        f.Pos += f.Vec * fsec + NC_STACK_particle::RandVec() * f.pParticleGen->_noisePower;
        float scl = f.pParticleGen->_scaleStart + f.pParticleGen->_scaleDelta * f.Age;
        
        Render(&f, scl, rndrParams);
        
        it++;
    }
    
    _disableAdd = false;
}


void ParticleSystem::Render(Frak *p, float scale, area_arg_65 *rndrParams)
{
    TF::TForm3D *view = rndrParams->ViewTForm;

    int v27 = -1;

    vec3d sprPos = p->Pos - ((view->CalcSclRot.AxisX() + view->CalcSclRot.AxisY()) * scale * 0.5);

    vec3d v37;//, v36, v37;

    for (int i = 0; i < 5; i++)
    {
        switch ( i )
        {
        case 0:
            v37 = p->Pos;
            break;
        case 1:
            v37 = sprPos;
            break;
        case 2:
            v37 = sprPos + view->CalcSclRot.AxisX() * scale;
            break;
        case 3:
            v37 = sprPos + (view->CalcSclRot.AxisX() + view->CalcSclRot.AxisY()) * scale;
            break;
        case 4:
            v37 = sprPos + view->CalcSclRot.AxisY() * scale;
            break;
        }

        int flags = 0;

        vec3d v30 = view->CalcSclRot.Transform(v37 - view->CalcPos);

        if (v30.z < rndrParams->minZ)
            flags = 16;
        else if (v30.z > rndrParams->maxZ)
            flags = 32;

        if ( v30.x > v30.z )
            flags |= 2;

        if ( -v30.z > v30.x )
            flags |= 1;

        if ( v30.y > v30.z )
            flags |= 8;

        if ( -v30.z > v30.y )
            flags |= 4;

        //_skltData->tformedVertex[i] = UAskeleton::Vertex(v30, flags);

        v27 &= flags;
    }

    if ( !v27 )
    {
        //rndrParams->sklt = _skltData;

        if ( p->pParticleGen->_lifePerAde )
        {
            size_t id = p->Age / p->pParticleGen->_lifePerAde;
            
            if (id == p->pParticleGen->_meshCache.size()) // fix little overlap
                id = p->pParticleGen->_meshCache.size() - 1;
            
            if ( id < p->pParticleGen->_meshCache.size() )
            {
                GFX::TMesh &mesh = p->pParticleGen->_meshCache.at(id);
                
                GFX::TRenderNode& rend = GFX::Engine.AllocRenderNode();
                rend = GFX::TRenderNode( GFX::TRenderNode::TYPE_PARTICLE );

                vec3d pos = view->CalcSclRot.Transform(p->Pos - view->CalcPos);

                rend.Distance = pos.length();
                rend.Flags = mesh.Mat.Flags | rndrParams->flags;
                rend.Color = mesh.Mat.Color;

                /*if (newsky)
                {
                    if ( distance >= transDist ||
                         skel132.tform.Transform(msh.BoundBox.Min).XZ().length() >= transDist ||
                         skel132.tform.Transform(msh.BoundBox.Max).XZ().length() >= transDist )
                        rend.Mat.Flags |= GFX::RFLAGS_FALLOFF;
                }*/
                
                if ((mesh.Mat.Flags & GFX::RFLAGS_DYNAMIC_TEXTURE) && mesh.Mat.TexSource)
                {
                    mesh.Mat.TexSource->SetTime(rndrParams->timeStamp, rndrParams->frameTime);
                    uint32_t frameid = mesh.Mat.TexSource->GetCurrentFrameID();

                    if (frameid < mesh.CoordsCache.size())
                    {
                        rend.Tex = mesh.CoordsCache.at(frameid).Tex;
                        rend.pCoords = &mesh.CoordsCache.at(frameid).Coords;                    
                    }
                }
                else
                    rend.Tex = mesh.Mat.Tex;

                rend.Mesh = &mesh;            

                rend.TForm = mat4x4(view->CalcSclRot) * mat4x4(p->Pos - view->CalcPos);
                rend.TimeStamp = rndrParams->timeStamp;
                rend.FrameTime = rndrParams->frameTime;
                rend.FogStart = rndrParams->fadeStart;
                rend.FogLength = rndrParams->fadeLength;
                rend.ParticleSize = scale;

                GFX::GFXEngine::Instance.QueueRenderMesh(&rend);
            }
        }
    }
}

void ParticleSystem::Clear()
{
    _particles.clear();
}

}