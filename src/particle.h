#ifndef PARTICLE_H_INCLUDED
#define PARTICLE_H_INCLUDED

#include "nucleas.h"
#include "area.h"
#include "ade.h"

class NC_STACK_particle;

struct __attribute__((packed)) particle_t_loc
{
    float fl0;
    float fl4;
    float fl8;
    float flC;
    float fl10;
    float fl14;
};

struct __attribute__((packed)) prtcl_att
{
    int16_t field_0;
    particle_t_loc field_2;
    particle_t_loc field_1A;
    int field_32;
    int field_36;
    int field_3A;
    int field_3E;
    int field_42;
    int field_46;
    int field_4A;
    int field_4E;
    int field_52;
    int field_56;
    int field_5A;
};

struct prtcl_tp2
{
    float field_0;
    float field_4;
    float field_8;
    float field_C;
    float field_10;
    float field_14;
};

struct prtcl_tp
{
    prtcl_tp2 *tp2;
    prtcl_tp2 *tp2_end;
    prtcl_tp2 *field_8;
    prtcl_tp2 *field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
};

struct __NC_STACK_particle
{
    NC_STACK_skeleton *particle_sklt;
    skeleton_64_stru *particle_sklt_intern;
    int field_8;
    int field_c;
    int ADEs_count;
    int field_14;
    NC_STACK_ade *ADEs[12];
    particle_t_loc field_3c;
    float field_54;
    float field_58;
    float field_5c;
    particle_t_loc field_60;
    float field_78;
    float field_7c;
    float field_80;
    int field_84;
    int field_88;
    int field_8c;
    float field_90;
    float field_94;
    float field_98;
    float field_9c;
    float field_a0;
    int tp1_cnt;
    int field_a8;
    int field_ac;
    int field_b0;
    prtcl_tp *tp1;
    prtcl_tp *tp1_end;
    prtcl_tp *tp1_st;
};

class NC_STACK_particle: public NC_STACK_ade
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func1(stack_vals *stak);
    virtual size_t func2(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
    virtual size_t func5(MFILE **file);
    virtual size_t func6(MFILE **file);
    virtual size_t ade_func65(area_arg_65 *arg);
    virtual void particle_func128(particle_t_loc *arg);
    virtual void particle_func129(particle_t_loc *arg);
    virtual void particle_func130(particle_t_loc *out);
    virtual void particle_func131(particle_t_loc *out);
    virtual size_t particle_func132(NC_STACK_area **ade);
    virtual NC_STACK_ade * particle_func133(int *id);
    virtual size_t particle_func134(int *iid);
    virtual size_t particle_func135(int *iid);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_particle() {
        memset(&stack__particle, 0, sizeof(stack__particle));
    };
    virtual ~NC_STACK_particle() {};

    virtual const char * getClassName() {
        return "particle.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_particle();
    };

    //Data
    static const NewClassDescr description;

    __NC_STACK_particle stack__particle;
};

#endif // PARTICLE_H_INCLUDED
