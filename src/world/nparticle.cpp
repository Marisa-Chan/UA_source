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
    _sklt = Nucleus::CInit<NC_STACK_skeleton>( {
        {NC_STACK_rsrc::RSRC_ATT_NAME, std::string("ParticleSystemSklt")},
        {NC_STACK_rsrc::RSRC_ATT_TRYSHARED, (int32_t)2},
        {NC_STACK_skeleton::SKEL_ATT_POINTSCNT, (int32_t)5},
        {NC_STACK_skeleton::SKEL_ATT_POLYCNT, (int32_t)1},
        {NC_STACK_skeleton::SKEL_ATT_POLYPNTCNT, (int32_t)4} } );

    _skltData = _sklt->GetSkelet();

    UAskeleton::Polygon &v3 = _skltData->polygons[0];

    v3.num_vertices = 4;
    v3.v[0] = 1;
    v3.v[1] = 2;
    v3.v[2] = 3;
    v3.v[3] = 4;
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
    TF::TForm3D *view = rndrParams->view;

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

        _skltData->tformedVertex[i] = UAskeleton::Vertex(v30, flags);

        v27 &= flags;
    }

    if ( !v27 )
    {
        rndrParams->sklt = _skltData;
        rndrParams->OBJ_SKELETON = _sklt;

        if ( p->pParticleGen->_lifePerAde )
        {
            size_t id = p->Age / p->pParticleGen->_lifePerAde;
            
            if (id == p->pParticleGen->_lifeStagesAdes.size()) // fix little overlap
                id = p->pParticleGen->_lifeStagesAdes.size() - 1;
            
            if ( id < p->pParticleGen->_lifeStagesAdes.size() )
            {
                NC_STACK_ade *ade = p->pParticleGen->_lifeStagesAdes.at(id);
                if ( ade )
                    ade->ade_func65(rndrParams);
            }
        }
    }
}

void ParticleSystem::Clear()
{
    _particles.clear();
}

}