#include <inttypes.h>
#include <string.h>
#include "includes.h"
#include "def_parser.h"
#include "yw_internal.h"
#include "yw.h"
#include "input.h"

#include "yparobo.h"

extern Key_stru keySS[256];

NC_STACK_yparobo *current_robo;
NC_STACK_ypabact *current_bact;
NC_STACK_ypabact *current_commander;
__NC_STACK_ypabact *dword_5A7A88;
extern NC_STACK_yparobo *dword_5A7A84;
extern int dword_5A7A80;
extern int bact_id;
extern int dword_5A7A8C;
extern int dword_5A7A78;

int dword_5A7A70; // for super items


int get_snd_type(const char *snd_typ_name)
{
    if ( !strcasecmp(snd_typ_name, "normal") )
        return 0 ;
    else  if ( !strcasecmp(snd_typ_name, "fire") )
        return 1;
    else if ( !strcasecmp(snd_typ_name, "wait") )
        return 2;
    else if ( !strcasecmp(snd_typ_name, "genesis") )
        return 3;
    else if ( !strcasecmp(snd_typ_name, "explode") )
        return 4;
    else if ( !strcasecmp(snd_typ_name, "crashland") )
        return 5;
    else if ( !strcasecmp(snd_typ_name, "crashvhcl") )
        return 6;
    else if ( !strcasecmp(snd_typ_name, "goingdown") )
        return 7;
    else if ( !strcasecmp(snd_typ_name, "cockpit") )
        return 8;
    else if ( !strcasecmp(snd_typ_name, "beamin") )
        return 9;
    else if ( !strcasecmp(snd_typ_name, "beamout") )
        return 10;
    else if ( !strcasecmp(snd_typ_name, "build") )
        return 11;

    return -1;
}


int ParseExtSampleDef(vhclSndFX *sndfx, char *p2)
{
    char *pp1 = strtok(p2, "_");
    char *pp2 = strtok(0, "_");
    char *pp3 = strtok(0, "_");
    char *pp4 = strtok(0, "_");
    char *pp5 = strtok(0, "_");
    char *pname = strtok(0, "_");

    if ( !pp1 || !pp2 || !pp3 || !pp4 || !pp5 || !pname )
        return 0;

    if ( sndfx->extS.cnt < 8 )
    {
        int curid = sndfx->extS.cnt;
        sndfx->extS.cnt++;

        sndExt *sndEx = &sndfx->extS.sndExts[curid];
        sndEx->sample = NULL;
        sndEx->field_4 = strtol(pp1, NULL, 0);
        sndEx->field_6 = strtol(pp2, NULL, 0);
        sndEx->field_8 = strtol(pp3, NULL, 0);
        sndEx->field_C = strtol(pp4, NULL, 0);
        sndEx->field_10 = strtol(pp5, NULL, 0);

        if ( strlen(pname) >= 31 )
        {
            ypa_log_out("ParseExtSampleDef(): Name too long!\n");
            return 0;
        }

        strcpy(sndfx->extSampleNames[curid], pname);
    }
    return 1;
}


int ParseVhclSndFX(VhclProto *vhcl, const char *p1, char *p2)
{
    char buf[32];

    strcpy(buf, p1);

    char *psnd = strtok(buf, "_");

    if ( strcasecmp(psnd, "snd") )
        return 3;

    char *snd_type = strtok(0, "_");
    int snd_id = get_snd_type(snd_type);

    if ( snd_id == -1 )
        return 3;

    vhclSndFX *sndfx = &vhcl->sndFX[snd_id];

    char *tp = strtok(0, "_");
    if ( !strcasecmp(tp, "sample") )
    {
        if ( strlen(p2) >= 31 )
        {
            ypa_log_out("ParseVhclSndFX(): Sample name too long!\n");
            return 4;
        }
        strcpy(sndfx->sample_name, p2);
    }
    else if ( !strcasecmp(tp, "volume") )
        sndfx->volume = strtol(p2, NULL, 0);
    else if ( !strcasecmp(tp, "pitch") )
        sndfx->pitch = strtol(p2, NULL, 0);
    else if ( !strcasecmp(tp, "ext") )
    {
        if ( !ParseExtSampleDef(sndfx, p2) )
            return 4;
    }
    else
        return 3;

    return 0;
}

int ParseVhclPal(VhclProto *vhcl, const char *p1, const char *p2)
{
    char buf[32];
    strcpy(buf, p1);

    char *ppal = strtok(buf, "_");

    if ( strcasecmp(ppal, "pal") )
        return 3;

    char *ptype = strtok(0, "_");
    int type_id = get_snd_type(ptype);

    if ( type_id == -1 )
        return 3;

    vhclSndFX *sndfx = &vhcl->sndFX[type_id];

    char *tp = strtok(0, "_");
    if ( !strcasecmp(tp, "slot") )
        sndfx->sndPrm.slot = strtol(p2, NULL, 0);
    else if ( !strcasecmp(tp, "mag0") )
        sndfx->sndPrm.mag0 = strtof(p2, 0);
    else if ( !strcasecmp(tp, "mag1") )
        sndfx->sndPrm.mag1 = strtof(p2, 0);
    else if ( !strcasecmp(tp, "time") )
        sndfx->sndPrm.time = strtol(p2, NULL, 0);
    else
        return 3;

    return 0;
}

int ParseVhclShk(VhclProto *vhcl, const char *p1, const char *p2)
{
    char buf[32];
    strcpy(buf, p1);

    char *ppal = strtok(buf, "_");

    if ( strcasecmp(ppal, "shk") )
        return 3;

    char *ptype = strtok(0, "_");
    int type_id = get_snd_type(ptype);

    if ( type_id == -1 )
        return 3;

    vhclSndFX *sndfx = &vhcl->sndFX[type_id];

    char *tp = strtok(0, "_");
    if ( !strcasecmp(tp, "slot") )
        sndfx->sndPrm_shk.slot = strtol(p2, NULL, 0);
    else if ( !strcasecmp(tp, "mag0") )
        sndfx->sndPrm_shk.mag0 = strtof(p2, 0);
    else if ( !strcasecmp(tp, "mag1") )
        sndfx->sndPrm_shk.mag1 = strtof(p2, 0);
    else if ( !strcasecmp(tp, "time") )
        sndfx->sndPrm_shk.time = strtol(p2, NULL, 0);
    else if ( !strcasecmp(tp, "mute") )
        sndfx->sndPrm_shk.mute = strtof(p2, 0);
    else if ( !strcasecmp(tp, "x") )
        sndfx->sndPrm_shk.pos.sx = strtof(p2, 0);
    else if ( !strcasecmp(tp, "y") )
        sndfx->sndPrm_shk.pos.sy = strtof(p2, 0);
    else if ( !strcasecmp(tp, "z") )
        sndfx->sndPrm_shk.pos.sz = strtof(p2, 0);
    else
        return 3;

    return 0;
}


int VhclProtoParser(scrCallBack *arg)
{
    char *_p1 = arg->p1;
    char *_p2 = arg->p2;
    _NC_STACK_ypaworld *yw = arg->world;

    int result;

    if ( !arg->field_18 )
    {
        arg->dataForStore = NULL;

        if ( !strcasecmp(_p1, "new_vehicle") )
        {
            int vhcl_id = strtol(_p2, NULL, 0);
            if ( vhcl_id >= 256 )
                return 4;

            VhclProto *vhcl = &yw->VhclProtos[vhcl_id];

            if ( vhcl->wireframe )
            {
                delete_class_obj(vhcl->wireframe);
                vhcl->wireframe = NULL;
            }

            if ( vhcl->hud_wireframe )
            {
                delete_class_obj(vhcl->hud_wireframe);
                vhcl->hud_wireframe = NULL;
            }

            if ( vhcl->mg_wireframe )
            {
                delete_class_obj(vhcl->mg_wireframe);
                vhcl->mg_wireframe = NULL;
            }

            if ( vhcl->wpn_wireframe_1 )
            {
                delete_class_obj(vhcl->wpn_wireframe_1);
                vhcl->wpn_wireframe_1 = NULL;
            }

            if ( vhcl->wpn_wireframe_2 )
            {
                delete_class_obj(vhcl->wpn_wireframe_2);
                vhcl->wpn_wireframe_2 = NULL;
            }

            memset(vhcl, 0, sizeof(VhclProto));

            vhcl->model_id = 2;
            vhcl->weapon = -1;
            vhcl->mgun = -1;
            vhcl->type_icon = 65;
            vhcl->vp_normal = 0;
            vhcl->vp_fire = 1;
            vhcl->vp_megadeth = 2;
            vhcl->vp_wait = 3;
            vhcl->vp_dead = 4;
            vhcl->vp_genesis = 5;
            vhcl->shield = 50;
            vhcl->energy = 10000;
            vhcl->adist_sector = 800.0;
            vhcl->adist_bact = 650.0;
            vhcl->sdist_sector = 200.0;
            vhcl->sdist_bact = 100.0;
            vhcl->radar = 1;
            vhcl->kill_after_shot = 0;
            vhcl->mass = 400.0;
            vhcl->force = 5000.0;
            vhcl->airconst = 80.0;
            vhcl->maxrot = 0.8;
            vhcl->height = 150.0;
            vhcl->radius = 25.0;
            vhcl->overeof = 25.0;
            vhcl->vwr_radius = 30.0;
            vhcl->vwr_overeof = 30.0;
            vhcl->gun_power = 4000.0;
            vhcl->gun_radius = 5.0;
            vhcl->max_pitch = -1.0;
            vhcl->job_fightflyer = 0;
            vhcl->job_fighthelicopter = 0;
            vhcl->job_fightrobo = 0;
            vhcl->job_fighttank = 0;
            vhcl->job_reconnoitre = 0;
            vhcl->job_conquer = 0;
            for (int i = 0; i < 12; i++)
            {
                vhclSndFX *v9 = &vhcl->sndFX[i];
                v9->sndPrm.mag0 = 1.0;
                v9->sndPrm_shk.mag0 = 1.0;
                v9->sndPrm_shk.mute = 0.02;
                v9->sndPrm_shk.pos.sx = 0.2;
                v9->sndPrm_shk.pos.sy = 0.2;
                v9->sndPrm_shk.pos.sz = 0.2;
                v9->volume = 120;
                v9->sndPrm.time = 1000;
                v9->sndPrm_shk.time = 1000;
            }
            vhcl->stack_pointer__position = vhcl->stak;
            vhcl->stak[0].id = 0;

            arg->field_18 = 1;
            arg->world2 = 0;
            arg->field_24 = 0;
            arg->roboID = vhcl_id & 0xF;
            arg->dataForStore = vhcl;
        }
        else if ( !strcasecmp(_p1, "modify_vehicle") )
        {
            int vhcl_id = strtol(_p2, NULL, 0);
            if ( vhcl_id >= 256 )
                return 4;

            yw->last_modify_vhcl = vhcl_id;
            VhclProto *vhcl = &yw->VhclProtos[vhcl_id];
            arg->field_18 = 1;
            arg->world2 = 0;
            arg->field_24 = 0;
            arg->roboID = vhcl_id & 0xF;
            arg->dataForStore = vhcl;
        }
        else
            return 3;
        return 1;
    }
    else
    {
        VhclProto * vhcl = (VhclProto *)arg->dataForStore;
        int v76 = (int)arg->world2;
        int v77 = arg->field_24;
        roboProto *robo = &yw->RoboProtos[arg->roboID];

        if ( !strcasecmp(_p1, "end") )
        {
            if ( vhcl->model_id == 3 )
            {
                stack_vals *val = find_id_in_stack2(0x80002001, vhcl->stak);
                if ( val )
                {
                    val->value = (size_t)robo;
                }
                else
                {
                    vhcl->stack_pointer__position->id = 0x80002001;
                    vhcl->stack_pointer__position->value = (size_t)robo;
                    vhcl->stack_pointer__position++;
                    vhcl->stack_pointer__position->id = 0;
                }
            }

            if ( vhcl->model_id == 1 )
                vhcl->field_1D6F = (vhcl->force * 0.6) / vhcl->airconst;
            else
                vhcl->field_1D6F = (vhcl->force) / vhcl->airconst;

            vhcl->field_1D6D = (vhcl->field_1D6F / 10) * 1200.0;

            arg->field_18 = 0;
            arg->dataForStore = NULL;
            return 2;
        }
        else if ( !strcasecmp(_p1, "model") )
        {
            if ( !strcasecmp(_p2, "heli") )
            {
                vhcl->model_id = 1;
            }
            else if ( !strcasecmp(_p2, "tank") )
            {
                vhcl->model_id = 2;
            }
            else if ( !strcasecmp(_p2, "robo") )
            {
                vhcl->model_id = 3;
                memset(robo, 0, sizeof(roboProto));
                robo->matrix.m00 = 1.0;
                robo->matrix.m11 = 1.0;
                robo->matrix.m22 = 1.0;
            }
            else if ( !strcasecmp(_p2, "ufo") )
            {
                vhcl->model_id = 7;
            }
            else if ( !strcasecmp(_p2, "car") )
            {
                vhcl->model_id = 8;
            }
            else if ( !strcasecmp(_p2, "gun") )
            {
                vhcl->model_id = 9;
            }
            else if ( !strcasecmp(_p2, "hover") )
            {
                vhcl->model_id = 10;
            }
            else if ( !strcasecmp(_p2, "plane") )
            {
                vhcl->model_id = 6;
                stack_vals *val = find_id_in_stack2(0x80002000, vhcl->stak);

                if ( val )
                {
                    val->value = 3;
                }
                else
                {
                    vhcl->stack_pointer__position->id = 0x80002000;
                    vhcl->stack_pointer__position->value = 3;
                    vhcl->stack_pointer__position++;
                    vhcl->stack_pointer__position->id = 0;
                }
            }
            else if ( !strcasecmp(_p2, "glider") )
            {
                vhcl->model_id = 6;
                stack_vals *val = find_id_in_stack2(0x80002000, vhcl->stak);
                if ( val )
                {
                    val->value = 2;
                }
                else
                {
                    vhcl->stack_pointer__position->id = 0x80002000;
                    vhcl->stack_pointer__position->value = 2;
                    vhcl->stack_pointer__position++;
                    vhcl->stack_pointer__position->id = 0;
                }
            }
            else if ( !strcasecmp(_p2, "zeppelin") )
            {
                vhcl->model_id = 6;
                stack_vals *val = find_id_in_stack2(0x80002000, vhcl->stak);
                if ( val )
                {
                    val->value = 0;
                }
                else
                {
                    vhcl->stack_pointer__position->id = 0x80002000;
                    vhcl->stack_pointer__position->value = 0;
                    vhcl->stack_pointer__position++;
                    vhcl->stack_pointer__position->id = 0;
                }
            }
            else
            {
                return 4;
            }
        }
        else if ( !strcasecmp(_p1, "enable") )
        {
            vhcl->disable_enable_bitmask |= 1 << strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "disable") )
        {
            vhcl->disable_enable_bitmask &= ~(1 << strtol(_p2, NULL, 0));
        }
        else if ( !strcasecmp(_p1, "name") )
        {
            if ( strlen(_p2) >= 126 )
            {
                ypa_log_out("VhclProtoParser(): Name too long!\n");
                return 4;
            }
            strcpy(vhcl->name, _p2);

            char *tmp = strpbrk(vhcl->name, "_");
            while ( tmp )
            {
                *tmp = ' ';
                tmp = strpbrk(tmp + 1, "_");
            }
        }
        else if ( !strcasecmp(_p1, "energy") )
        {
            vhcl->energy = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "shield") )
        {
            vhcl->shield = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "mass") )
        {
            vhcl->mass = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "force") )
        {
            vhcl->force = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "maxrot") )
        {
            vhcl->maxrot = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "airconst") )
        {
            vhcl->airconst = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "height") )
        {
            vhcl->height = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "radius") )
        {
            vhcl->radius = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "overeof") )
        {
            vhcl->overeof = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "vwr_radius") )
        {
            vhcl->vwr_radius = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "vwr_overeof") )
        {
            vhcl->vwr_overeof = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "adist_sector") )
        {
            vhcl->adist_sector = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "adist_bact") )
        {
            vhcl->adist_bact = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "sdist_sector") )
        {
            vhcl->sdist_sector = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "sdist_bact") )
        {
            vhcl->sdist_bact = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "radar") )
        {
            vhcl->radar = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "add_energy") )
        {
            vhcl->energy += strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "add_shield") )
        {
            vhcl->shield += strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "add_radar") )
        {
            vhcl->radar += strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "vp_normal") )
        {
            vhcl->vp_normal = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "vp_fire") )
        {
            vhcl->vp_fire = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "vp_megadeth") )
        {
            vhcl->vp_megadeth = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "vp_wait") )
        {
            vhcl->vp_wait = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "vp_dead") )
        {
            vhcl->vp_dead = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "vp_genesis") )
        {
            vhcl->vp_genesis = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "type_icon") )
        {
            vhcl->type_icon = *_p2;
        }
        else if ( !strcasecmp(_p1, "dest_fx") )
        {
            char *fx_type = strtok(_p2, " _");
            char *pp1 = strtok(0, " _");
            char *pp2 = strtok(0, " _");
            char *pp3 = strtok(0, " _");
            char *pp4 = strtok(0, " _");

            if ( fx_type && pp1 && pp2 && pp3 && pp4 )
            {
                destFX *dfx = &vhcl->dest_fx[vhcl->destFxCount];
                dfx->type_flag = 0;

                if ( !strcasecmp(fx_type, "death") )
                {
                    dfx->type_flag |= 1;
                }
                else if ( !strcasecmp(fx_type, "megadeth") )
                {
                    dfx->type_flag |= 2;
                }
                else if ( !strcasecmp(fx_type, "create") )
                {
                    dfx->type_flag |= 4;
                }
                else if ( !strcasecmp(fx_type, "beam") )
                {
                    dfx->type_flag |= 8;
                }
                else
                {
                    return 4;
                }
                dfx->p1 = strtol(pp1, NULL, 0);
                dfx->p2 = strtof(pp2, 0);
                dfx->p3 = strtof(pp3, 0);
                dfx->p4 = strtof(pp4, 0);

                vhcl->destFxCount++;
                if ( vhcl->destFxCount >= 16 )
                    vhcl->destFxCount = 15;
            }
            else
            {
                return 4;
            }
        }
        else if ( !strcasecmp(_p1, "weapon") )
        {
            vhcl->field_3 = -1;
            vhcl->weapon = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "mgun") )
        {
            vhcl->mgun = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "fire_x") )
        {
            vhcl->fire_x = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "fire_y") )
        {
            vhcl->fire_y = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "fire_z") )
        {
            vhcl->fire_z = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "gun_radius") )
        {
            vhcl->gun_radius = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "gun_power") )
        {
            vhcl->gun_power = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "gun_angle") )
        {
            vhcl->gun_angle = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "num_weapons") )
        {
            vhcl->num_weapons = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "kill_after_shot") )
        {
            vhcl->kill_after_shot = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "job_fighthelicopter") )
        {
            vhcl->job_fighthelicopter = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "job_fightflyer") )
        {
            vhcl->job_fightflyer = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "job_fighttank") )
        {
            vhcl->job_fighttank = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "job_fightrobo") )
        {
            vhcl->job_fightrobo = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "job_reconnoitre") )
        {
            vhcl->job_reconnoitre = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "job_conquer") )
        {
            vhcl->job_conquer = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "gun_side_angle") )
        {
            stack_vals *val = find_id_in_stack2(0x80002000, vhcl->stak);
            if ( val )
            {
                val->value = strtol(_p2, NULL, 0);
            }
            else
            {
                vhcl->stack_pointer__position->id = 0x80002000;
                vhcl->stack_pointer__position->value = strtol(_p2, NULL, 0);
                vhcl->stack_pointer__position++;
                vhcl->stack_pointer__position->id = 0;
            }
        }
        else if ( !strcasecmp(_p1, "gun_up_angle") )
        {
            stack_vals *val = find_id_in_stack2(0x80002001, vhcl->stak);
            if ( val )
            {
                val->value = strtol(_p2, NULL, 0);
            }
            else
            {
                vhcl->stack_pointer__position->id = 0x80002001;
                vhcl->stack_pointer__position->value = strtol(_p2, NULL, 0);
                vhcl->stack_pointer__position++;
                vhcl->stack_pointer__position->id = 0;
            }
        }
        else if ( !strcasecmp(_p1, "gun_down_angle") )
        {
            stack_vals *val = find_id_in_stack2(0x80002002, vhcl->stak);
            if ( val )
            {
                val->value = strtol(_p2, NULL, 0);
            }
            else
            {
                vhcl->stack_pointer__position->id = 0x80002002;
                vhcl->stack_pointer__position->value = strtol(_p2, NULL, 0);
                vhcl->stack_pointer__position++;
                vhcl->stack_pointer__position->id = 0;
            }
        }
        else if ( !strcasecmp(_p1, "gun_type") )
        {
            int gun_type = 0;
            if ( !strcasecmp(_p2, "flak") )
            {
                gun_type = 1;
            }
            else if ( !strcasecmp(_p2, "mg") )
            {
                gun_type = 2;
            }
            else
            {
                //strcasecmp(_p2, "dummy");
            }
            if ( gun_type )
            {
                stack_vals *val = find_id_in_stack2(0x80002003, vhcl->stak);
                if ( val )
                {
                    val->value = gun_type;
                }
                else
                {
                    vhcl->stack_pointer__position->id = 0x80002003;
                    vhcl->stack_pointer__position->value = gun_type;
                    vhcl->stack_pointer__position++;
                    vhcl->stack_pointer__position->id = 0;
                }
            }
        }
        else if ( !strcasecmp(_p1, "kamikaze") )
        {
            stack_vals *val = find_id_in_stack2(0x80003000, vhcl->stak);
            if ( val )
            {
                val->value = 1;
            }
            else
            {
                vhcl->stack_pointer__position->id = 0x80003000;
                vhcl->stack_pointer__position->value = 1;
                vhcl->stack_pointer__position++;
                vhcl->stack_pointer__position->id = 0;
            }

            val = find_id_in_stack2(0x80003001, vhcl->stak);
            if ( val )
            {
                val->value = strtol(_p2, NULL, 0);
            }
            else
            {
                vhcl->stack_pointer__position->id = 0x80003001;
                vhcl->stack_pointer__position->value = strtol(_p2, NULL, 0);
                vhcl->stack_pointer__position++;
                vhcl->stack_pointer__position->id = 0;
            }
        }
        else if ( !strcasecmp(_p1, "wireframe") )
        {
            if ( vhcl->wireframe )
            {
                delete_class_obj(vhcl->wireframe);
                vhcl->wireframe = NULL;
            }
            vhcl->wireframe = (NC_STACK_sklt *)init_get_class("sklt.class", 0x80001000, _p2, 0);
        }
        else if ( !strcasecmp(_p1, "hud_wireframe") )
        {
            if ( vhcl->hud_wireframe )
            {
                delete_class_obj(vhcl->hud_wireframe);
                vhcl->hud_wireframe = NULL;
            }
            vhcl->hud_wireframe = (NC_STACK_sklt *)init_get_class("sklt.class", 0x80001000, _p2, 0);
        }
        else if ( !strcasecmp(_p1, "mg_wireframe") )
        {
            if ( vhcl->mg_wireframe )
            {
                delete_class_obj(vhcl->mg_wireframe);
                vhcl->mg_wireframe = NULL;
            }
            vhcl->mg_wireframe = (NC_STACK_sklt *)init_get_class("sklt.class", 0x80001000, _p2, 0);
        }
        else if ( !strcasecmp(_p1, "wpn_wireframe_1") )
        {
            if ( vhcl->wpn_wireframe_1 )
            {
                delete_class_obj(vhcl->wpn_wireframe_1);
                vhcl->wpn_wireframe_1 = NULL;
            }
            vhcl->wpn_wireframe_1 = (NC_STACK_sklt *)init_get_class("sklt.class", 0x80001000, _p2, 0);
        }
        else if ( !strcasecmp(_p1, "wpn_wireframe_2") )
        {
            if ( vhcl->wpn_wireframe_2 )
            {
                delete_class_obj(vhcl->wpn_wireframe_2);
                vhcl->wpn_wireframe_2 = NULL;
            }
            vhcl->wpn_wireframe_2 = (NC_STACK_sklt *)init_get_class("sklt.class", 0x80001000, _p2, 0);
        }
        else if ( !strcasecmp(_p1, "vo_type") )
        {
            vhcl->vo_type = strtol(_p2, NULL, 16);
        }
        else if ( !strcasecmp(_p1, "max_pitch") )
        {
            vhcl->max_pitch = strtof(_p2, 0);
        }
        else if ( (result = ParseVhclSndFX(vhcl, _p1, _p2)) != 3)
        {
            if (result == 4)
                return 4;
        }
        else if ( (result = ParseVhclPal(vhcl, _p1, _p2)) != 3)
        {
            if (result == 4)
                return 4;
        }
        else if ( (result = ParseVhclShk(vhcl, _p1, _p2)) != 3)
        {
            if (result == 4)
                return 4;
        }
        else if ( !strcasecmp(_p1, "scale_fx") )
        {
            char *pp0 = strtok(_p2, "_");
            char *pp1 = strtok(0, "_");
            char *pp2 = strtok(0, "_");
            char *pp3 = strtok(0, "_");

            if ( pp0 && pp1 && pp2 && pp3 )
            {
                vhcl->scale_fx_p0 = strtof(pp0, 0);
                vhcl->scale_fx_p1 = strtof(pp1, 0);
                vhcl->scale_fx_p2 = strtof(pp2, 0);
                vhcl->scale_fx_p3 = strtol(pp3, NULL, 0);

                int tmp = 0;

                while ( 1 )
                {
                    char *v65 = strtok(0, "_");
                    if ( !v65 )
                        break;

                    vhcl->scale_fx_pXX[tmp] = strtol(v65, NULL, 0);
                    tmp++;
                }
            }
        }
        else if ( !strcasecmp(_p1, "robo_data_slot") )
        {
        }
        else if ( !strcasecmp(_p1, "robo_num_guns") )
        {
            robo->robo_num_guns = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "robo_act_gun") )
        {
            v76 = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "robo_gun_pos_x") )
        {
            robo->guns[v76].pos.sx = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_gun_pos_y") )
        {
            robo->guns[v76].pos.sy = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_gun_pos_z") )
        {
            robo->guns[v76].pos.sz = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_gun_dir_x") )
        {
            robo->guns[v76].dir.sx = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_gun_dir_y") )
        {
            robo->guns[v76].dir.sy = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_gun_dir_z") )
        {
            robo->guns[v76].dir.sz = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_gun_type") )
        {
            robo->guns[v76].robo_gun_type = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "robo_gun_name") )
        {
            strcpy(robo->guns[v76].robo_gun_name, _p2);
        }
        else if ( !strcasecmp(_p1, "robo_dock_x") )
        {
            robo->dock.sx = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_dock_y") )
        {
            robo->dock.sy = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_dock_z") )
        {
            robo->dock.sz = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_coll_num") )
        {
            robo->coll.robo_coll_num = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "robo_coll_radius") )
        {
            robo->coll.roboColls[v77].robo_coll_radius = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_coll_act") )
        {
            v77 = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "robo_coll_x") )
        {
            robo->coll.roboColls[v77].robo_coll_x = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_coll_y") )
        {
            robo->coll.roboColls[v77].robo_coll_y = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_coll_z") )
        {
            robo->coll.roboColls[v77].robo_coll_z = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_viewer_x") )
        {
            robo->viewer.sx = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_viewer_y") )
        {
            robo->viewer.sy = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_viewer_z") )
        {
            robo->viewer.sz = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_viewer_max_up") )
        {
            robo->robo_viewer_max_up = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_viewer_max_down") )
        {
            robo->robo_viewer_max_down = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_viewer_max_side") )
        {
            robo->robo_viewer_max_side = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_does_twist") )
        {
            stack_vals *val = find_id_in_stack2(0x8000200D, vhcl->stak);
            if ( val )
            {
                val->value = 1;
            }
            else
            {
                vhcl->stack_pointer__position->id = 0x8000200D;
                vhcl->stack_pointer__position->value = 1;
                vhcl->stack_pointer__position++;
                vhcl->stack_pointer__position->id = 0;
            }
        }
        else if ( !strcasecmp(_p1, "robo_does_flux") )
        {
            stack_vals *val = find_id_in_stack2(0x8000200C, vhcl->stak);
            if ( val )
            {
                val->value = 1;
            }
            else
            {
                vhcl->stack_pointer__position->id = 0x8000200C;
                vhcl->stack_pointer__position->value = 1;
                vhcl->stack_pointer__position++;
                vhcl->stack_pointer__position->id = 0;
            }
        }
        else
            return 3;

        arg->world2 = (_NC_STACK_ypaworld *)v76;
        arg->field_24 = v77;
        return 0;
    }
}





int wpn_get_snd_type(const char *snd_typ_name)
{
    if ( !strcasecmp(snd_typ_name, "normal") )
        return 0 ;
    else if ( !strcasecmp(snd_typ_name, "launch") )
        return 1;
    else if ( !strcasecmp(snd_typ_name, "hit") )
        return 2;

    return -1;
}


int ParseWeaponSndFX(WeapProto *wpn, const char *p1, char *p2)
{
    char buf[32];

    strcpy(buf, p1);

    char *psnd = strtok(buf, "_");

    if ( strcasecmp(psnd, "snd") )
        return 3;

    char *snd_type = strtok(0, "_");
    int snd_id = wpn_get_snd_type(snd_type);

    if ( snd_id == -1 )
        return 3;

    vhclSndFX *sndfx = &wpn->sndFXes[snd_id];

    char *tp = strtok(0, "_");
    if ( !strcasecmp(tp, "sample") )
    {
        if ( strlen(p2) >= 31 )
        {
            ypa_log_out("ParseWeaponSndFX(): Sample name too long!\n");
            return 4;
        }
        strcpy(sndfx->sample_name, p2);
    }
    else if ( !strcasecmp(tp, "volume") )
        sndfx->volume = strtol(p2, NULL, 0);
    else if ( !strcasecmp(tp, "pitch") )
        sndfx->pitch = strtol(p2, NULL, 0);
    else if ( !strcasecmp(tp, "ext") )
    {
        if ( !ParseExtSampleDef(sndfx, p2) )
            return 4;
    }
    else
        return 3;

    return 0;
}

int ParseWeaponPal(WeapProto *wpn, const char *p1, const char *p2)
{
    char buf[32];
    strcpy(buf, p1);

    char *ppal = strtok(buf, "_");

    if ( strcasecmp(ppal, "pal") )
        return 3;

    char *ptype = strtok(0, "_");
    int type_id = wpn_get_snd_type(ptype);

    if ( type_id == -1 )
        return 3;

    vhclSndFX *sndfx = &wpn->sndFXes[type_id];

    char *tp = strtok(0, "_");
    if ( !strcasecmp(tp, "slot") )
        sndfx->sndPrm.slot = strtol(p2, NULL, 0);
    else if ( !strcasecmp(tp, "mag0") )
        sndfx->sndPrm.mag0 = strtof(p2, 0);
    else if ( !strcasecmp(tp, "mag1") )
        sndfx->sndPrm.mag1 = strtof(p2, 0);
    else if ( !strcasecmp(tp, "time") )
        sndfx->sndPrm.time = strtol(p2, NULL, 0);
    else
        return 3;

    return 0;
}

int ParseWeaponShk(WeapProto *wpn, const char *p1, const char *p2)
{
    char buf[32];
    strcpy(buf, p1);

    char *ppal = strtok(buf, "_");

    if ( strcasecmp(ppal, "shk") )
        return 3;

    char *ptype = strtok(0, "_");
    int type_id = wpn_get_snd_type(ptype);

    if ( type_id == -1 )
        return 3;

    vhclSndFX *sndfx = &wpn->sndFXes[type_id];

    char *tp = strtok(0, "_");

    if ( !strcasecmp(tp, "slot") )
        sndfx->sndPrm_shk.slot = strtol(p2, NULL, 0);
    else if ( !strcasecmp(tp, "mag0") )
        sndfx->sndPrm_shk.mag0 = strtof(p2, 0);
    else if ( !strcasecmp(tp, "mag1") )
        sndfx->sndPrm_shk.mag1 = strtof(p2, 0);
    else if ( !strcasecmp(tp, "time") )
        sndfx->sndPrm_shk.time = strtol(p2, NULL, 0);
    else if ( !strcasecmp(tp, "mute") )
        sndfx->sndPrm_shk.mute = strtof(p2, 0);
    else if ( !strcasecmp(tp, "x") )
        sndfx->sndPrm_shk.pos.sx = strtof(p2, 0);
    else if ( !strcasecmp(tp, "y") )
        sndfx->sndPrm_shk.pos.sy = strtof(p2, 0);
    else if ( !strcasecmp(tp, "z") )
        sndfx->sndPrm_shk.pos.sz = strtof(p2, 0);
    else
        return 3;

    return 0;
}


int WeaponProtoParser(scrCallBack *arg)
{
    char *_p1 = arg->p1;
    char *_p2 = arg->p2;
    _NC_STACK_ypaworld *yw = arg->world;

    if ( !arg->field_18 )
    {
        arg->dataForStore = NULL;

        WeapProto *wpn;

        if ( !strcasecmp(_p1, "new_weapon") )
        {
            int wpn_id = strtol(_p2, NULL, 0);

            if ( wpn_id >= 128 )
                return 4;

            wpn = &yw->WeaponProtos[wpn_id];

            if ( wpn->wireframe )
            {
                delete_class_obj(wpn->wireframe);
                wpn->wireframe = NULL;
            }

            memset(wpn, 0, sizeof(WeapProto));

            wpn->field_0 = 4;

            memset(wpn->name, 0, sizeof(wpn->name));

            wpn->energy = 10000;
            wpn->mass = 50.0;
            wpn->force = 5000.0;
            wpn->airconst = 50.0;
            wpn->maxrot = 2.0;
            wpn->radius = 20.0;
            wpn->overeof = 10.0;
            wpn->vwr_radius = 20.0;
            wpn->vwr_overeof = 20.0;
            wpn->energy_heli = 1.0;
            wpn->energy_tank = 1.0;
            wpn->energy_flyer = 1.0;
            wpn->energy_robo = 1.0;
            wpn->radius_heli = 0;
            wpn->radius_tank = 0;
            wpn->radius_flyer = 0;
            wpn->radius_robo = 0;
            wpn->start_speed = 70.0;
            wpn->life_time = 20000;
            wpn->life_time_nt = 0;
            wpn->drive_time = 7000;
            wpn->shot_time = 3000;
            wpn->shot_time_user = 1000;
            wpn->salve_delay = 0;
            wpn->salve_shots = 0;
            wpn->vp_normal = 0;
            wpn->vp_fire = 1;
            wpn->vp_megadeth = 2;
            wpn->vp_wait = 3;
            wpn->vp_dead = 4;
            wpn->vp_genesis = 5;
            wpn->type_icon = 65;

            for (int i = 0; i < 3; i++)
            {
                vhclSndFX *v9 = &wpn->sndFXes[i];
                v9->sndPrm.mag0 = 1.0;
                v9->sndPrm_shk.mag0 = 1.0;
                v9->sndPrm_shk.mute = 0.02;
                v9->sndPrm_shk.pos.sx = 0.2;
                v9->sndPrm_shk.pos.sy = 0.2;
                v9->sndPrm_shk.pos.sz = 0.2;
                v9->volume = 120;
                v9->sndPrm.time = 1000;
                v9->sndPrm_shk.time = 1000;
            }

            wpn->pointer = wpn->stack;
            wpn->stack[0].id = 0;
        }
        else if ( !strcasecmp(_p1, "modify_weapon") )
        {
            int wpn_id = strtol(_p2, NULL, 0);

            if ( wpn_id >= 128 )
                return 4;

            wpn = &yw->WeaponProtos[wpn_id];
            yw->last_modify_weapon = wpn_id;
        }
        else
            return 3;

        arg->field_18 = 1;
        arg->dataForStore = wpn;

        return 1;
    }
    else
    {
        WeapProto *wpn = (WeapProto *)arg->dataForStore;
        if ( !strcasecmp(arg->p1, "end") )
        {
            arg->dataForStore = 0;
            arg->field_18 = 0;
            return 2;
        }
        else if ( !strcasecmp(_p1, "model") )
        {
            if ( !strcasecmp(_p2, "grenade") )
                wpn->model_id = 17;
            else if ( !strcasecmp(_p2, "rocket") )
                wpn->model_id = 3;
            else if ( !strcasecmp(_p2, "missile") )
                wpn->model_id = 7;
            else if ( !strcasecmp(_p2, "bomb") || !strcasecmp(_p2, "special") )
                wpn->model_id = 1;
            else
                return 4;
        }
        else if ( !strcasecmp(_p1, "enable") )
        {
            wpn->enable_mask |= 1 << strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "disable") )
        {
            wpn->enable_mask &= ~(1 << strtol(_p2, NULL, 0));
        }
        else if ( !strcasecmp(_p1, "name") )
        {
            if ( strlen(_p2) >= 126 )
            {
                ypa_log_out("WeaponProtoParser(): Name too long!\n");
                return 4;
            }
            strcpy(wpn->name, _p2);

            char *tmp = strpbrk(wpn->name, "_");
            while ( tmp )
            {
                *tmp = ' ';
                tmp = strpbrk(tmp + 1, "_");
            }
        }
        else if ( !strcasecmp(_p1, "energy") )
        {
            wpn->energy = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "energy_heli") )
        {
            wpn->energy_heli = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "energy_tank") )
        {
            wpn->energy_tank = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "energy_flyer") )
        {
            wpn->energy_flyer = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "energy_robo") )
        {
            wpn->energy_robo = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "mass") )
        {
            wpn->mass = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "force") )
        {
            wpn->force = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "maxrot") )
        {
            wpn->maxrot = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "airconst") )
        {
            wpn->airconst = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "radius") )
        {
            wpn->radius = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "radius_heli") )
        {
            wpn->radius_heli = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "radius_tank") )
        {
            wpn->radius_tank = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "radius_flyer") )
        {
            wpn->radius_flyer = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "radius_robo") )
        {
            wpn->radius_robo = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "overeof") )
        {
            wpn->overeof = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "vwr_radius") )
        {
            wpn->vwr_radius = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "vwr_overeof") )
        {
            wpn->vwr_overeof = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "start_speed") )
        {
            wpn->start_speed = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "life_time") )
        {
            wpn->life_time = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "life_time_nt") )
        {
            wpn->life_time_nt = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "drive_time") )
        {
            wpn->drive_time = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "delay_time") )
        {
            wpn->delay_time = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "shot_time") )
        {
            wpn->shot_time = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "shot_time_user") )
        {
            wpn->shot_time_user = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "salve_shots") )
        {
            wpn->salve_shots = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "salve_delay") )
        {
            wpn->salve_delay = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "add_energy") )
        {
            wpn->energy += strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "add_energy_heli") )
        {
            wpn->energy_heli += strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "add_energy_tank") )
        {
            wpn->energy_tank += strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "add_energy_flyer") )
        {
            wpn->energy_flyer += strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "add_energy_Robo") )
        {
            wpn->energy_robo += strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "add_shot_time") )
        {
            wpn->shot_time += strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "add_shot_time_user") )
        {
            wpn->shot_time_user += strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "vp_normal") )
        {
            wpn->vp_normal = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "vp_fire") )
        {
            wpn->vp_fire = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "vp_megadeth") )
        {
            wpn->vp_megadeth = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "vp_wait") )
        {
            wpn->vp_wait = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "vp_dead") )
        {
            wpn->vp_dead = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "vp_genesis") )
        {
            wpn->vp_genesis = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "type_icon") )
        {
            wpn->type_icon = *_p2;
        }
        else if ( !strcasecmp(_p1, "wireframe") )
        {
            if ( wpn->wireframe )
            {
                delete_class_obj(wpn->wireframe);
                wpn->wireframe = NULL;
            }
            wpn->wireframe = (NC_STACK_sklt *)init_get_class("sklt.class", 0x80001000, _p2, 0);
        }
        else if ( !strcasecmp(_p1, "dest_fx") )
        {
            char *fx_type = strtok(_p2, " _");
            char *pp1 = strtok(0, " _");
            char *pp2 = strtok(0, " _");
            char *pp3 = strtok(0, " _");
            char *pp4 = strtok(0, " _");

            if ( fx_type && pp1 && pp2 && pp3 && pp4 )
            {
                destFX *dfx = &wpn->dfx[wpn->destFxCount];
                dfx->type_flag = 0;

                if ( !strcasecmp(fx_type, "death") )
                {
                    dfx->type_flag |= 1;
                }
                else if ( !strcasecmp(fx_type, "megadeth") )
                {
                    dfx->type_flag |= 2;
                }
                else if ( !strcasecmp(fx_type, "create") )
                {
                    dfx->type_flag |= 4;
                }
                else if ( !strcasecmp(fx_type, "beam") )
                {
                    dfx->type_flag |= 8;
                }
                else
                {
                    return 4;
                }
                dfx->p1 = strtol(pp1, NULL, 0);
                dfx->p2 = strtof(pp2, 0);
                dfx->p3 = strtof(pp3, 0);
                dfx->p4 = strtof(pp4, 0);

                wpn->destFxCount++;
                if ( wpn->destFxCount >= 16 )
                    wpn->destFxCount = 15;
            }
            else
            {
                return 4;
            }
        }
        else
        {
            int result = ParseWeaponSndFX(wpn, _p1, _p2);

            if ( result == 3 )
            {
                result = ParseWeaponPal(wpn, _p1, _p2);

                if ( result == 3 )
                {
                    result = ParseWeaponShk(wpn, _p1, _p2);

                    if ( result == 3 || result == 4 )
                        return result;
                }
                else if ( result == 4 )
                {
                    return result;
                }
            }
            else if ( result == 4 )
            {
                return result;
            }
        }
    }
    return 0;
}

int BuildProtoParser(scrCallBack *arg)
{
    char *_p1 = arg->p1;
    char *_p2 = arg->p2;
    _NC_STACK_ypaworld *yw = arg->world2;

    if ( !arg->field_18 )
    {
        arg->dataForStore = NULL;
        BuildProto *bld;

        if ( !strcasecmp(_p1, "new_building") )
        {
            int bld_id = strtol(_p2, NULL, 0);
            if ( bld_id >= 128 )
                return 4;

            bld = &yw->BuildProtos[bld_id];
            memset(bld, 0, sizeof(BuildProto));
            bld->energy = 50000;
            bld->type_icon = 65;
            bld->sndfx.volume = 120;
        }
        else if ( !strcasecmp(_p1, "modify_building") )
        {
            int bld_id = strtol(_p2, NULL, 0);
            if ( bld_id >= 128 )
                return 4;

            yw->last_modify_build = bld_id;
            bld = &yw->BuildProtos[bld_id];
        }
        else
            return 3;

        arg->field_18 = 1;
        arg->world = 0;
        arg->dataForStore = bld;
        return 1;
    }
    else
    {
        BuildProto *bld = (BuildProto *)arg->dataForStore;
        int v20 = (int)arg->world;

        if ( !strcasecmp(_p1, "end") )
        {
            arg->dataForStore = 0;
            arg->field_18 = 0;
            return 2;
        }

        if ( !strcasecmp(_p1, "model") )
        {
            if ( !strcasecmp(_p2, "building") )
            {
                bld->model_id = 0;
            }
            else if ( !strcasecmp(_p2, "kraftwerk") )
            {
                bld->model_id = 1;
            }
            else if ( !strcasecmp(_p2, "radar") )
            {
                bld->model_id = 2;
            }
            else if ( !strcasecmp(_p2, "defcenter") )
            {
                bld->model_id = 3;
            }
            else
                return 4;
        }
        else if ( !strcasecmp(_p1, "enable") )
        {
            bld->enable_mask |= 1 << strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "disable") )
        {
            bld->enable_mask &= ~(1 << strtol(_p2, NULL, 0));
        }
        else if ( !strcasecmp(_p1, "name") )
        {
            if ( strlen(_p2) >= 126 )
            {
                ypa_log_out("BuildProtoParser(): Name too long!\n");
                return 4;
            }
            strcpy(bld->name, _p2);

            char *tmp = strpbrk(bld->name, "_");
            while ( tmp )
            {
                *tmp = ' ';
                tmp = strpbrk(tmp + 1, "_");
            }
        }
        else if ( !strcasecmp(_p1, "power") )
        {
            bld->power = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "energy") )
        {
            bld->energy = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "sec_type") )
        {
            bld->sec_type = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "type_icon") )
        {
            bld->type_icon = *_p2;
        }
        else if ( !strcasecmp(_p1, "snd_normal_sample") )
        {
            if ( strlen(_p2) >= 31 )
            {
                ypa_log_out("BuildProtoParser(): Sample name too long!\n");
                return 4;
            }
            strcpy(bld->sndfx.sample_name, _p2);
        }
        else if ( !strcasecmp(_p1, "snd_normal_volume") )
        {
            bld->sndfx.volume = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "snd_normal_pitch") )
        {
            bld->sndfx.pitch = strtol(_p2, NULL, 0);
        }
        else if ( !strcasecmp(_p1, "sbact_act") )
        {
            v20 = strtol(_p2, NULL, 0);
        }
        else
        {
            buildSbact *v19 = &bld->sbacts[v20];
            if ( !strcasecmp(_p1, "sbact_vehicle") )
            {
                v19->sbact_vehicle = strtol(_p2, NULL, 0);
            }
            else if ( !strcasecmp(_p1, "sbact_pos_x") )
            {
                v19->sbact_pos_x = strtof(_p2, 0);
            }
            else if ( !strcasecmp(_p1, "sbact_pos_y") )
            {
                v19->sbact_pos_y = strtof(_p2, 0);
            }
            else if ( !strcasecmp(_p1, "sbact_pos_z") )
            {
                v19->sbact_pos_z = strtof(_p2, 0);
            }
            else if ( !strcasecmp(_p1, "sbact_dir_x") )
            {
                v19->sbact_dir_x = strtof(_p2, 0);
            }
            else if ( !strcasecmp(_p1, "sbact_dir_y") )
            {
                v19->sbact_dir_y = strtof(_p2, 0);
            }
            else if ( !strcasecmp(_p1, "sbact_dir_z") )
            {
                v19->sbact_dir_z = strtof(_p2, 0);
            }
            else
                return 3;
        }
        arg->world = (_NC_STACK_ypaworld *)v20;
        return 0;
    }
}


int MoviesParser(scrCallBack *arg)
{
    char *_p1 = arg->p1;
    char *_p2 = arg->p2;

    if ( arg->field_18 )
    {
        _NC_STACK_ypaworld *yw = arg->world;
        if ( !strcasecmp(arg->p1, "end") )
        {
            arg->field_18 = 0;
            return 2;
        }
        else if ( !strcasecmp(_p1, "game_intro") )
        {
            strcpy(yw->movies.game_intro, _p2);
            yw->movies.movies_names_present[0] = 1;
        }
        else if ( !strcasecmp(_p1, "win_extro") )
        {
            strcpy(yw->movies.win_extro, _p2);
            yw->movies.movies_names_present[2] = 1;
        }
        else if ( !strcasecmp(_p1, "lose_extro") )
        {
            strcpy(yw->movies.lose_extro, _p2);
            yw->movies.movies_names_present[1] = 1;
        }
        else if ( !strcasecmp(_p1, "user_intro") )
        {
            strcpy(yw->movies.user_intro, _p2);
            yw->movies.movies_names_present[3] = 1;
        }
        else if ( !strcasecmp(_p1, "kyt_intro") )
        {
            strcpy(yw->movies.kyt_intro, _p2);
            yw->movies.movies_names_present[4] = 1;
        }
        else if ( !strcasecmp(_p1, "taer_intro") )
        {
            strcpy(yw->movies.taer_intro, _p2);
            yw->movies.movies_names_present[5] = 1;
        }
        else if ( !strcasecmp(_p1, "myk_intro") )
        {
            strcpy(yw->movies.myk_intro, _p2);
            yw->movies.movies_names_present[6] = 1;
        }
        else if ( !strcasecmp(_p1, "sulg_intro") )
        {
            strcpy(yw->movies.sulg_intro, _p2);
            yw->movies.movies_names_present[7] = 1;
        }
        else if ( !strcasecmp(_p1, "black_intro") )
        {
            strcpy(yw->movies.black_intro, _p2);
            yw->movies.movies_names_present[8] = 1;
        }
        else
            return 3;
        return 0;
    }
    else if ( !strcasecmp(arg->p1, "begin_movies") )
    {
        memset(&arg->world->movies, 0, sizeof(yw_movies));
        arg->field_18 = 1;
        return 1;
    }
    else
        return 3;

    return 3;
}


int BkgParser(scrCallBack *arg)
{
    char *_p1 = arg->p1;
    char *_p2 = arg->p2;

    if ( !arg->field_18 )
    {
        if ( !strcasecmp(arg->p1, "begin_bg") )
        {
            arg->field_18 = 1;
            return 1;
        }
        return 3;
    }

    stru_LevelNet *lvlnet = arg->world->LevelNet;

    if ( lvlnet->bg_n >= 4 )
    {
        ypa_log_out("world.ini: Too many BG pics (max=%d)!\n", 4);
        return 4;
    }

    if ( !strcasecmp(arg->p1, "end") )
    {
        lvlnet->bg_n++;
        arg->field_18 = 0;
        return 2;
    }

    if ( !strcasecmp(_p1, "background_map") )
    {
        strcpy(lvlnet->background_map[lvlnet->bg_n].map_name, _p2);
    }
    else if ( !strcasecmp(_p1, "rollover_map") )
    {
        strcpy(lvlnet->rollover_map[lvlnet->bg_n].map_name, _p2);
    }
    else if ( !strcasecmp(_p1, "finished_map") )
    {
        strcpy(lvlnet->finished_map[lvlnet->bg_n].map_name, _p2);
    }
    else if ( !strcasecmp(_p1, "enabled_map") )
    {
        strcpy(lvlnet->enabled_map[lvlnet->bg_n].map_name, _p2);
    }
    else if ( !strcasecmp(_p1, "mask_map") )
    {
        strcpy(lvlnet->mask_map[lvlnet->bg_n].map_name, _p2);
    }
    else if ( !strcasecmp(_p1, "tut_background_map") )
    {
        strcpy(lvlnet->tut_background_map[lvlnet->bg_n].map_name, _p2);
    }
    else if ( !strcasecmp(_p1, "tut_rollover_map") )
    {
        strcpy(lvlnet->tut_rollover_map[lvlnet->bg_n].map_name, _p2);
    }
    else if ( !strcasecmp(_p1, "tut_mask_map") )
    {
        strcpy(lvlnet->tut_mask_map[lvlnet->bg_n].map_name, _p2);
    }
    else if ( !strcasecmp(_p1, "menu_map") )
    {
        strcpy(lvlnet->menu_map[lvlnet->bg_n].map_name, _p2);
    }
    else if ( !strcasecmp(_p1, "input_map") )
    {
        strcpy(lvlnet->input_map[lvlnet->bg_n].map_name, _p2);
    }
    else if ( !strcasecmp(_p1, "settings_map") )
    {
        strcpy(lvlnet->settings_map[lvlnet->bg_n].map_name, _p2);
    }
    else if ( !strcasecmp(_p1, "network_map") )
    {
        strcpy(lvlnet->network_map[lvlnet->bg_n].map_name, _p2);
    }
    else if ( !strcasecmp(_p1, "locale_map") )
    {
        strcpy(lvlnet->locale_map[lvlnet->bg_n].map_name, _p2);
    }
    else if ( !strcasecmp(_p1, "save_map") )
    {
        strcpy(lvlnet->save_map[lvlnet->bg_n].map_name, _p2);
    }
    else if ( !strcasecmp(_p1, "about_map") )
    {
        strcpy(lvlnet->about_map[lvlnet->bg_n].map_name, _p2);
    }
    else if ( !strcasecmp(_p1, "help_map") )
    {
        strcpy(lvlnet->help_map[lvlnet->bg_n].map_name, _p2);
    }
    else if ( !strcasecmp(_p1, "brief_map") )
    {
        strcpy(lvlnet->brief_map[lvlnet->bg_n].map_name, _p2);
    }
    else if ( !strcasecmp(_p1, "debrief_map") )
    {
        strcpy(lvlnet->debrief_map[lvlnet->bg_n].map_name, _p2);
    }
    else if ( !strcasecmp(_p1, "size_x") )
    {
        lvlnet->background_map[lvlnet->bg_n].size_x = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "size_y") )
    {
        lvlnet->background_map[lvlnet->bg_n].size_y = strtol(_p2, NULL, 0);
    }
    else
        return 3;

    return 0;
}

int Colors_Parser(scrCallBack *arg)
{
    char *_p2 = arg->p2;
    char *_p1 = arg->p1;

    if ( !arg->field_18 )
    {
        if ( !strcasecmp(arg->p1, "begin_colors") )
        {
            arg->field_18 = 1;
            return 1;
        }
        return 3;
    }

    _NC_STACK_ypaworld *yw = arg->world;

    if ( !strcasecmp(arg->p1, "end") )
    {
        arg->field_18 = 0;
        return 2;
    }

    if ( !strcasecmp(_p1, "owner_0") )
    {
        yw_parse_color(yw, 0, _p2);
    }
    else if ( !strcasecmp(_p1, "owner_1") )
    {
        yw_parse_color(yw, 1, _p2);
    }
    else if ( !strcasecmp(_p1, "owner_2") )
    {
        yw_parse_color(yw, 2, _p2);
    }
    else if ( !strcasecmp(_p1, "owner_3") )
    {
        yw_parse_color(yw, 3, _p2);
    }
    else if ( !strcasecmp(_p1, "owner_4") )
    {
        yw_parse_color(yw, 4, _p2);
    }
    else if ( !strcasecmp(_p1, "owner_5") )
    {
        yw_parse_color(yw, 5, _p2);
    }
    else if ( !strcasecmp(_p1, "owner_6") )
    {
        yw_parse_color(yw, 6, _p2);
    }
    else if ( !strcasecmp(_p1, "owner_7") )
    {
        yw_parse_color(yw, 7, _p2);
    }
    else if ( !strcasecmp(_p1, "map_direction") )
    {
        yw_parse_color(yw, 8, _p2);
    }
    else if ( !strcasecmp(_p1, "map_primtarget") )
    {
        yw_parse_color(yw, 9, _p2);
    }
    else if ( !strcasecmp(_p1, "map_sectarget") )
    {
        yw_parse_color(yw, 10, _p2);
    }
    else if ( !strcasecmp(_p1, "map_commander") )
    {
        yw_parse_color(yw, 11, _p2);
    }
    else if ( !strcasecmp(_p1, "map_dragbox") )
    {
        yw_parse_color(yw, 12, _p2);
    }
    else if ( !strcasecmp(_p1, "map_viewer") )
    {
        yw_parse_color(yw, 13, _p2);
    }
    else if ( !strcasecmp(_p1, "hud_weapon") )
    {
        yw_parse_color(yw, 20, _p2);
    }
    else if ( !strcasecmp(_p1, "hud_weapon_1") )
    {
        yw_parse_color(yw, 29, _p2);
    }
    else if ( !strcasecmp(_p1, "hud_compass_commandvec") )
    {
        yw_parse_color(yw, 21, _p2);
    }
    else if ( !strcasecmp(_p1, "hud_compass_commandvec_1") )
    {
        yw_parse_color(yw, 30, _p2);
    }
    else if ( !strcasecmp(_p1, "hud_compass_primtarget") )
    {
        yw_parse_color(yw, 22, _p2);
    }
    else if ( !strcasecmp(_p1, "hud_compass_primtarget_1") )
    {
        yw_parse_color(yw, 31, _p2);
    }
    else if ( !strcasecmp(_p1, "hud_compass_locktarget") )
    {
        yw_parse_color(yw, 23, _p2);
    }
    else if ( !strcasecmp(_p1, "hud_compass_locktarget_1") )
    {
        yw_parse_color(yw, 32, _p2);
    }
    else if ( !strcasecmp(_p1, "hud_compass_compass") )
    {
        yw_parse_color(yw, 24, _p2);
    }
    else if ( !strcasecmp(_p1, "hud_compass_compass_1") )
    {
        yw_parse_color(yw, 33, _p2);
    }
    else if ( !strcasecmp(_p1, "hud_vehicle") )
    {
        yw_parse_color(yw, 25, _p2);
    }
    else if ( !strcasecmp(_p1, "hud_vehicle_1") )
    {
        yw_parse_color(yw, 34, _p2);
    }
    else if ( !strcasecmp(_p1, "hud_visor_mg") )
    {
        yw_parse_color(yw, 26, _p2);
    }
    else if ( !strcasecmp(_p1, "hud_visor_mg_1") )
    {
        yw_parse_color(yw, 35, _p2);
    }
    else if ( !strcasecmp(_p1, "hud_visor_locked") )
    {
        yw_parse_color(yw, 27, _p2);
    }
    else if ( !strcasecmp(_p1, "hud_visor_locked_1") )
    {
        yw_parse_color(yw, 36, _p2);
    }
    else if ( !strcasecmp(_p1, "hud_visor_autonom") )
    {
        yw_parse_color(yw, 28, _p2);
    }
    else if ( !strcasecmp(_p1, "hud_visor_autonom_1") )
    {
        yw_parse_color(yw, 37, _p2);
    }
    else if ( !strcasecmp(_p1, "brief_lines") )
    {
        yw_parse_color(yw, 50, _p2);
    }
    else if ( !strcasecmp(_p1, "text_default") )
    {
        yw_parse_color(yw, 60, _p2);
    }
    else if ( !strcasecmp(_p1, "text_list") )
    {
        yw_parse_color(yw, 61, _p2);
    }
    else if ( !strcasecmp(_p1, "text_list_sel") )
    {
        yw_parse_color(yw, 62, _p2);
    }
    else if ( !strcasecmp(_p1, "text_tooltip") )
    {
        yw_parse_color(yw, 63, _p2);
    }
    else if ( !strcasecmp(_p1, "text_message") )
    {
        yw_parse_color(yw, 64, _p2);
    }
    else if ( !strcasecmp(_p1, "text_hud") )
    {
        yw_parse_color(yw, 65, _p2);
    }
    else if ( !strcasecmp(_p1, "text_briefing") )
    {
        yw_parse_color(yw, 66, _p2);
    }
    else if ( !strcasecmp(_p1, "text_debriefing") )
    {
        yw_parse_color(yw, 67, _p2);
    }
    else if ( !strcasecmp(_p1, "text_button") )
    {
        yw_parse_color(yw, 68, _p2);
    }
    else
        return 3;

    return 0;
}


int Misc_Parse(scrCallBack *arg)
{
    char *_p2 = arg->p2;
    char *_p1 = arg->p1;

    _NC_STACK_ypaworld *yw = arg->world;

    if ( !arg->field_18 )
    {
        if ( !strcasecmp(arg->p1, "begin_misc") )
        {
            yw->beam_energy_start = 500;
            yw->beam_energy_add = 100;
            yw->unit_limit = 512;
            yw->unit_limit_type = 0;
            yw->unit_limit_arg = 0;
            yw->easy_cheat_keys = 0;
            arg->field_18 = 1;
            return 1;
        }
        return 3;
    }

    if ( !strcasecmp(arg->p1, "end") )
    {
        arg->field_18 = 0;
        return 2;
    }

    if ( !strcasecmp(_p1, "one_game_res") )
    {
        yw->one_game_res = !strcasecmp(_p2, "yes") || !strcasecmp(_p2, "true") || !strcasecmp(_p2, "on");
    }
    else if ( !strcasecmp(_p1, "shell_default_res") )
    {
        char *pp1 = strtok(_p2, "_");
        char *pp2 = strtok(0, " \t");

        if ( pp1 && pp2 )
        {
            yw->shell_default_res = strtol(pp2, NULL, 0) | (strtol(pp1, NULL, 0) << 12);
        }
    }
    else if ( !strcasecmp(_p1, "game_default_res") )
    {
        char *pp1 = strtok(_p2, "_");
        char *pp2 = strtok(0, " \t");

        if ( pp1 && pp2 )
        {
            yw->game_default_res = strtol(pp2, NULL, 0) | (strtol(pp1, NULL, 0) << 12);
        }
    }
    else if ( !strcasecmp(_p1, "max_impulse") )
    {
        yw->max_impulse = strtof(_p2, 0);
    }
    else if ( !strcasecmp(_p1, "unit_limit") )
    {
        yw->unit_limit = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "unit_limit_type") )
    {
        yw->unit_limit_type = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "unit_limit_arg") )
    {
        yw->unit_limit_arg = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "beam_energy_start") )
    {
        yw->beam_energy_start = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "beam_energy_add") )
    {
        yw->beam_energy_add = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "easy_cheat_keys") )
    {
        yw->easy_cheat_keys = strtol(_p2, NULL, 0);
    }
    else
        return 3;

    return 0;
}

int SuperItem_Parse(scrCallBack *arg)
{
    char *_p2 = arg->p2;
    char *_p1 = arg->p1;
    _NC_STACK_ypaworld *yw = arg->world;

    if ( !arg->field_18 )
    {
        if ( !strcasecmp(arg->p1, "begin_superitem") )
        {
            yw->superbomb_wall_vproto = 0;
            yw->superbomb_center_vproto = 0;
            arg->field_18 = 1;
            return 1;
        }
        return 3;
    }

    if ( !strcasecmp(arg->p1, "end") )
    {
        arg->field_18 = 0;
        return 2;
    }

    if ( !strcasecmp(_p1, "superbomb_center_vproto") )
    {
        yw->superbomb_center_vproto = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "superbomb_wall_vproto") )
    {
        yw->superbomb_wall_vproto = strtol(_p2, NULL, 0);
    }
    else
        return 3;
    return 0;
}

void parse_map_sizes(FILE *fil, int *sec_x, int *sec_y)
{
    char buf[1024];
    fgets(buf, 1024, fil);

    char *tmp = strtok(buf, " \n");
    int xsz = strtol(tmp, NULL, 0);

    tmp = strtok(0, " \n");
    int ysz = strtol(tmp, NULL, 0);

    for (int i = 0; i < ysz; i++)
        fgets(buf, 1024, fil);

    if ( sec_x )
        *sec_x = xsz;

    if ( sec_y )
        *sec_y = ysz;
}

int MapParseSizes(scrCallBack *arg)
{
    char *_p1 = arg->p1;

    if ( !arg->field_18 )
    {
        if ( !strcasecmp(arg->p1, "begin_maps") )
        {
            arg->field_18 = 1;
            return 1;
        }
        return 3;
    }

    mapProto *mapp = (mapProto *)arg->dataForStore;

    if ( !strcasecmp(arg->p1, "end") )
    {
        arg->field_18 = 0;
        return 2;
    }

    if ( !strcasecmp(_p1, "typ_map") )
    {
        int y_size;
        int x_size;

        parse_map_sizes(arg->file, &x_size, &y_size);
        mapp->secXsize = x_size;
        mapp->secYsize = y_size;
    }
    else if ( !strcasecmp(_p1, "own_map") || !strcasecmp(_p1, "hgt_map") || !strcasecmp(_p1, "blg_map") )
    {
        parse_map_sizes(arg->file, 0, 0);
    }
    else
        return 3;
    return 0;
}


int LevelDataParser(scrCallBack *arg)
{
    char *_p2 = arg->p2;
    char *_p1 = arg->p1;
    _NC_STACK_ypaworld *yw = arg->world;

    if ( !arg->field_18 )
    {
        if ( !strcasecmp(_p1, "begin_level") )
        {
            strcpy(yw->field_2d90->map_name, "<NO NAME>");
            yw->field_2d90->movie[0] = 0;
            yw->field_2d90->win_movie[0] = 0;
            yw->field_2d90->lose_movie[0] = 0;
            yw->vehicle_sector_ratio_1 = 0;
            yw->unit_limit_1 = yw->unit_limit;
            yw->unit_limit_type_1 = yw->unit_limit_type;
            yw->unit_limit_arg_1 = yw->unit_limit_arg;
            arg->field_18 = 1;
            return 1;
        }
        return 3;
    }

    mapProto *mapp = (mapProto *)arg->dataForStore;

    if ( !strcasecmp(_p1, "end") )
    {
        arg->field_18 = 0;
        return 2;
    }

    if ( strstr(_p1, "title_") )
    {

        char title_lang[32];
        strcpy(title_lang, "title_");
        strcat(title_lang, yw->lang_name);

        if ( !strcasecmp(_p1, "title_default") || !strcasecmp(_p1, title_lang) )
        {
            if ( strlen(_p2) < 63 )
            {
                strcpy(yw->field_2d90->map_name, _p2);
            }
            else
            {
                ypa_log_out("LevelDataParser(): Level titel too long!");
                return 4;
            }
        }
    }
    else if ( !strcasecmp(_p1, "set") )
    {
        mapp->setNumber = strtol(_p2, NULL, 0);
        mapp->flags |= 1;
    }
    else if ( !strcasecmp(_p1, "sky") )
    {
        if ( strlen(_p2) < 63 )
        {
            strcpy(mapp->sky, _p2);
            mapp->flags |= 2;
        }
        else
        {
            ypa_log_out("LevelDataParser(): Sky name too long!");
            return 4;
        }
    }
    else if ( !strcasecmp(_p1, "typ") )
    {
        if ( strlen(_p2) < 63 )
        {
            strcpy(mapp->typ, _p2);

            mapp->flags |= 4;
        }
        else
        {
            ypa_log_out("LevelDataParser(): Type map name too long!");
            return 4;
        }
    }
    else if ( !strcasecmp(_p1, "own") )
    {
        if ( strlen(_p2) < 63 )
        {
            strcpy(mapp->own, _p2);

            mapp->flags |= 8;
        }
        else
        {
            ypa_log_out("LevelDataParser(): Owner map name too long!");
            return 4;
        }
    }
    else if ( !strcasecmp(_p1, "hgt") )
    {
        if ( strlen(_p2) < 63 )
        {
            strcpy(mapp->hgt, _p2);

            mapp->flags |= 0x10;
        }
        else
        {
            ypa_log_out("LevelDataParser(): Height map name too long!");
            return 4;
        }
    }
    else if ( !strcasecmp(_p1, "blg") )
    {
        if ( strlen(_p2) < 63 )
        {
            strcpy(mapp->blg, _p2);

            mapp->flags |= 0x20;
        }
        else
        {
            ypa_log_out("LevelDataParser(): Building map name too long!");
            return 4;
        }
    }
    else if ( !strcasecmp(_p1, "palette") )
    {
        strcpy(mapp->palettes[0], _p2);
    }
    else if ( !strcasecmp(_p1, "slot0") )
    {
        strcpy(mapp->palettes[0], _p2);
    }
    else if ( !strcasecmp(_p1, "slot1") )
    {
        strcpy(mapp->palettes[1], _p2);
    }
    else if ( !strcasecmp(_p1, "slot2") )
    {
        strcpy(mapp->palettes[2], _p2);
    }
    else if ( !strcasecmp(_p1, "slot3") )
    {
        strcpy(mapp->palettes[3], _p2);
    }
    else if ( !strcasecmp(_p1, "slot4") )
    {
        strcpy(mapp->palettes[4], _p2);
    }
    else if ( !strcasecmp(_p1, "slot5") )
    {
        strcpy(mapp->palettes[5], _p2);
    }
    else if ( !strcasecmp(_p1, "slot6") )
    {
        strcpy(mapp->palettes[6], _p2);
    }
    else if ( !strcasecmp(_p1, "slot7") )
    {
        strcpy(mapp->palettes[7], _p2);
    }
    else if ( !strcasecmp(_p1, "script") )
    {
        if ( !sub_4DA354(arg->world, _p2) )
            return 4;
        return 0;
    }
    else if ( !strcasecmp(_p1, "ambiencetrack") )
    {
        yw->field_2d90->amb_track_p1 = 0;
        yw->field_2d90->amb_track_p2 = 0;

        char *tmp = strtok(_p2, " \t_\n");

        yw->field_2d90->amb_track_p0 = strtol(tmp, NULL, 0);

        tmp = strtok(0, " \t_\n");
        if ( tmp )
        {
            yw->field_2d90->amb_track_p1 = strtol(tmp, NULL, 0);

            tmp = strtok(0, " \t_\n");
            if ( tmp )
            {
                yw->field_2d90->amb_track_p2 = strtol(tmp, NULL, 0);
            }
        }
    }
    else if ( !strcasecmp(_p1, "movie") )
    {
        strcpy(yw->field_2d90->movie, _p2);
    }
    else if ( !strcasecmp(_p1, "win_movie") )
    {
        strcpy(yw->field_2d90->win_movie, _p2);
    }
    else if ( !strcasecmp(_p1, "lose_movie") )
    {
        strcpy(yw->field_2d90->lose_movie, _p2);
    }
    else if ( !strcasecmp(_p1, "event_loop") )
    {
        mapp->event_loop = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "slow_connection") )
    {
        if ( !strcasecmp(_p2, "yes") || !strcasecmp(_p2, "on") || !strcasecmp(_p2, "true") )
        {
            mapp->slow_connection = 1;
        }
        else
        {
            mapp->slow_connection = 0;
        }
    }
    else if ( !strcasecmp(_p1, "vehicle_sector_ratio") )
    {
        yw->vehicle_sector_ratio_1 = strtof(_p2, 0);
    }
    else if ( !strcasecmp(_p1, "unit_limit") )
    {
        yw->unit_limit_1 = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "unit_limit_type") )
    {
        yw->unit_limit_type_1 = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "unit_limit_arg") )
    {
        yw->unit_limit_arg_1 = strtol(_p2, NULL, 0);
    }
    else
        return 3;

    return 0;
}

int parse_map_robos(scrCallBack *arg)
{
    mapProto *mapp = (mapProto *)arg->dataForStore;
    char *_p2 = arg->p2;
    char *_p1 = arg->p1;
    mapRobo *robo = &mapp->mapRobos[mapp->mapRobos_count];

    if ( !arg->field_18 )
    {
        if ( !strcasecmp(_p1, "begin_robo") )
        {
            robo->mb_status = 0;
            arg->field_18 = 1;
            return 1;
        }
        return 3;
    }

    if ( !strcasecmp(_p1, "end") )
    {
        arg->field_18 = 0;
        mapp->mapRobos_count++;
        mapp->flags |= 0x40;
        return 2;
    }

    if ( !strcasecmp(_p1, "owner") )
    {
        robo->owner = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "vehicle") )
    {
        robo->vehicle = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "sec_x") )
    {
        int tmp = strtol(_p2, NULL, 0);
        robo->pos_y = -300;
        robo->pos_x = tmp * 1200.0 + 600.0;
    }
    else if ( !strcasecmp(_p1, "sec_y") )
    {
        int tmp = strtol(_p2, NULL, 0);
        robo->pos_y = -300;
        robo->pos_z = -(tmp * 1200.0 + 600.0);
    }
    else if ( !strcasecmp(_p1, "pos_x") )
    {
        robo->pos_x = strtof(_p2, 0) + 0.3;
    }
    else if ( !strcasecmp(_p1, "pos_y") )
    {
        robo->pos_y = strtof(_p2, 0) + 0.3;
    }
    else if ( !strcasecmp(_p1, "pos_z") )
    {
        robo->pos_z = strtof(_p2, 0) + 0.3;
    }
    else if ( !strcasecmp(_p1, "energy") )
    {
        robo->energy = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "con_budget") )
    {
        robo->con_budget = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "rad_budget") )
    {
        robo->rad_budget = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "pow_budget") )
    {
        robo->pow_budget = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "def_budget") )
    {
        robo->def_budget = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "saf_budget") )
    {
        robo->saf_budget = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "rec_budget") )
    {
        robo->rec_budget = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "cpl_budget") )
    {
        robo->cpl_budget = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "rob_budget") )
    {
        robo->rob_budget = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "viewangle") )
    {
        robo->viewangle = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "saf_delay") )
    {
        robo->saf_delay = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "pow_delay") )
    {
        robo->pow_delay = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "rad_delay") )
    {
        robo->rad_delay = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "cpl_delay") )
    {
        robo->cpl_delay = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "def_delay") )
    {
        robo->def_delay = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "con_delay") )
    {
        robo->con_delay = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "rec_delay") )
    {
        robo->rec_delay = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "rob_delay") )
    {
        robo->rob_delay = strtol(_p2, NULL, 0);
    }
    else if ( !strcasecmp(_p1, "mb_status") )
    {
        if ( !strcasecmp(_p2, "known") )
        {
            robo->mb_status = 0;
        }
        else if ( !strcasecmp(_p2, "unknown") )
        {
            robo->mb_status = 1;
        }
        else if ( !strcasecmp(_p2, "hidden") )
        {
            robo->mb_status = 2;
        }
        else
            return 4;
    }
    else if ( !strcasecmp(_p1, "reload_const") )
    {
        robo->reload_const = strtol(_p2, NULL, 0);
    }
    else
        return 3;

    return 0;
}



int sb_0x4739e8__sub0(_NC_STACK_ypaworld *yw, const char *a2, char *a4, int buf_size)
{
    int v7 = 0;
    char buf[256];

    memset(a4, 0, buf_size);

    UserData *usr = yw->GameShell;

    if ( usr )
    {
        if ( usr->user_name[0] != 0 )
        {
            sprintf(buf, "save:%s/%s", usr->user_name, a2);
            FILE *v8 = FOpen(buf, "r");

            if ( v8 )
            {
                if ( fgets(a4, buf_size - 1, v8) )
                    v7 = 1;
                FClose(v8);
            }
        }
    }
    return v7;
}

int parseSaveUser(scrCallBack *arg)
{
    _NC_STACK_ypaworld *yw = arg->world2;

    if ( !arg->field_18 )
    {
        if ( !strcasecmp(arg->p1, "new_user") )
        {
            if ( !yw->GameShell->field_1CEA )
            {
                if ( !sb_0x4739e8__sub0(yw, "callsign.def", yw->GameShell->callSIGN, 64) )
                {
                    strcpy(yw->GameShell->callSIGN, get_lang_string(yw->string_pointers_p2, 366, "UNNAMED") );
                }
            }
            arg->field_18 = 1;
            return 1;
        }
        else
            return 3;
    }
    else
    {
        if ( !strcasecmp(arg->p1, "end") )
        {
            arg->field_18 = 0;
            return 2;
        }

        if ( !arg->world || !arg->world2 )
            return 3;

        if ( !strcasecmp(arg->p1, "username") )
        {
        }
        else if ( !strcasecmp(arg->p1, "netname") )
        {
        }
        else if ( !strcasecmp(arg->p1, "maxroboenergy") )
        {
            yw->maxroboenergy = strtol(arg->p2, NULL, 0);
        }
        else if ( !strcasecmp(arg->p1, "maxreloadconst") )
        {
            yw->maxreloadconst = strtol(arg->p2, NULL, 0);
        }
        else if ( !strcasecmp(arg->p1, "numbuddies") )
        {
        }
        else if ( !strcasecmp(arg->p1, "beamenergy") )
        {
            yw->beamenergy = strtol(arg->p2, NULL, 0);
        }
        else if ( !strcasecmp(arg->p1, "playerstatus") )
        {
            char *v10 = strtok(arg->p2, "_ \t");
            if ( v10 )
            {
                int plid = strtol(v10, NULL, 0);
                v10 = strtok(0, "_ \t");
                if ( v10 )
                {
                    yw->playerstatus[plid].p1 = strtol(v10, NULL, 0);
                    v10 = strtok(0, "_ \t");
                    if ( v10 )
                    {
                        yw->playerstatus[plid].p2 = strtol(v10, NULL, 0);
                        v10 = strtok(0, "_ \t");
                        if ( v10 )
                        {
                            yw->playerstatus[plid].p3 = strtol(v10, NULL, 0);
                            v10 = strtok(0, "_ \t");
                            if ( v10 )
                            {
                                yw->playerstatus[plid].p4 = strtol(v10, NULL, 0);
                                v10 = strtok(0, "_ \t");
                                if ( v10 )
                                {
                                    yw->playerstatus[plid].p5 = strtol(v10, NULL, 0);
                                    v10 = strtok(0, "_ \t");
                                    if ( v10 )
                                    {
                                        yw->playerstatus[plid].p6 = strtol(v10, NULL, 0);
                                        v10 = strtok(0, "_ \t");
                                        if ( v10 )
                                        {
                                            yw->playerstatus[plid].p7 = strtol(v10, NULL, 0);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else if ( !strcasecmp(arg->p1, "jodiefoster") )
        {
            char *v20 = strtok(arg->p2, "_ \t");
            if ( v20 )
            {
                yw->field_2d90->jodiefoster[0] = strtol(v20, NULL, 0);
                v20 = strtok(0, "_ \t");
                if ( v20 )
                {
                    yw->field_2d90->jodiefoster[1] = strtol(v20, NULL, 0);
                    v20 = strtok(0, "_ \t");
                    if ( v20 )
                    {
                        yw->field_2d90->jodiefoster[2] = strtol(v20, NULL, 0);
                        v20 = strtok(0, "_ \t");
                        if ( v20 )
                        {
                            yw->field_2d90->jodiefoster[3] = strtol(v20, NULL, 0);
                            v20 = strtok(0, "_ \t");
                            if ( v20 )
                            {
                                yw->field_2d90->jodiefoster[4] = strtol(v20, NULL, 0);
                                v20 = strtok(0, "_ \t");
                                if ( v20 )
                                {
                                    yw->field_2d90->jodiefoster[5] = strtol(v20, NULL, 0);
                                    v20 = strtok(0, "_ \t");
                                    if ( v20 )
                                    {
                                        yw->field_2d90->jodiefoster[6] = strtol(v20, NULL, 0);
                                        v20 = strtok(0, "_ \t");
                                        if ( v20 )
                                            yw->field_2d90->jodiefoster[7] = strtol(v20, NULL, 0);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else
            return 3;

        return 0;
    }
    return 0;
}


int ShellSoundsParse_sample(UserData *usr, scrCallBack *arg)
{
    NC_STACK_wav **v3;
    int v4;
    samples_collection1 *v13;

    if ( !strcasecmp(arg->p1, "quit_sample") )
    {
        v3 = usr->samples1;
        v4 = 4;
        v13 = &usr->samples1_info;
    }
    else if ( !strcasecmp(arg->p1, "volume_sample") )
    {
        v3 = usr->samples1;
        v4 = 0;
        v13 = &usr->samples1_info;
    }
    else if ( !strcasecmp(arg->p1, "button_sample") )
    {
        v3 = usr->samples1;
        v4 = 3;
        v13 = &usr->samples1_info;
    }
    else if ( !strcasecmp(arg->p1, "left_sample") )
    {
        v3 = usr->samples1;
        v4 = 2;
        v13 = &usr->samples1_info;
    }
    else if ( !strcasecmp(arg->p1, "right_sample") )
    {
        v3 = usr->samples1;
        v4 = 1;
        v13 = &usr->samples1_info;
    }
    else if ( !strcasecmp(arg->p1, "slider_sample") )
    {
        v3 = usr->samples1;
        v4 = 5;
        v13 = &usr->samples1_info;
    }
    else if ( !strcasecmp(arg->p1, "welcome_sample") )
    {
        v3 = usr->samples1;
        v4 = 6;
        v13 = &usr->samples1_info;
    }
    else if ( !strcasecmp(arg->p1, "menuopen_sample") )
    {
        v3 = usr->samples1;
        v4 = 7;
        v13 = &usr->samples1_info;
    }
    else if ( !strcasecmp(arg->p1, "overlevel_sample") )
    {
        v3 = usr->samples1;
        v4 = 8;
        v13 = &usr->samples1_info;
    }
    else if ( !strcasecmp(arg->p1, "levelselect_sample") )
    {
        v3 = usr->samples1;
        v4 = 9;
        v13 = &usr->samples1_info;
    }
    else if ( !strcasecmp(arg->p1, "textappear_sample") )
    {
        v3 = usr->samples1;
        v4 = 10;
        v13 = &usr->samples1_info;
    }
    else if ( !strcasecmp(arg->p1, "objectappear_sample") )
    {
        v3 = usr->samples1;
        v4 = 11;
        v13 = &usr->samples1_info;
    }
    else if ( !strcasecmp(arg->p1, "sectorconquered_sample") )
    {
        v3 = usr->samples1;
        v13 = &usr->samples1_info;
        v4 = 12;
    }
    else if ( !strcasecmp(arg->p1, "vhcldestroyed_sample") )
    {
        v3 = usr->samples1;
        v13 = &usr->samples1_info;
        v4 = 13;
    }
    else if ( !strcasecmp(arg->p1, "bldgconquered_sample") )
    {
        v3 = usr->samples1;
        v13 = &usr->samples1_info;
        v4 = 14;
    }
    else if ( !strcasecmp(arg->p1, "timercount_sample") )
    {
        v3 = usr->samples1;
        v13 = &usr->samples1_info;
        v4 = 15;
    }
    else if ( !strcasecmp(arg->p1, "select_sample") )
    {
        v13 = &usr->samples2_info;
        v4 = 0;
        v3 = usr->samples2;
    }
    else if ( !strcasecmp(arg->p1, "error_sample") )
    {
        v13 = &usr->samples2_info;
        v4 = 1;
        v3 = usr->samples2;
    }
    else if ( !strcasecmp(arg->p1, "attention_sample") )
    {
        v13 = &usr->samples2_info;
        v4 = 2;
        v3 = usr->samples2;
    }
    else if ( !strcasecmp(arg->p1, "secret_sample") )
    {
        v13 = &usr->samples2_info;
        v4 = 3;
        v3 = usr->samples2;
    }
    else if ( !strcasecmp(arg->p1, "plasma_sample") )
    {
        v13 = &usr->samples2_info;
        v4 = 4;
        v3 = usr->samples2;
    }
    else
        return 3;

    char buf[300];

    strcpy(buf, get_prefix_replacement("rsrc"));

    set_prefix_replacement("rsrc", "data:");

    v3[v4] = (NC_STACK_wav *)init_get_class("wav.class", 0x80001000, arg->p2, 0);
    if ( !v3[v4] )
        return 4;

    call_vtbl(v3[v4], 3, 0x80002000, &v13->samples_data[v4].psampl, 0);

    if ( v3 == usr->samples1 && (v4 == 0 || v4 == 5 || v4 == 10 || v4 == 15 ) )
        v13->samples_data[v4].field_12 |= 1;

    set_prefix_replacement("rsrc", buf);
    return 0;
}

signed int __fastcall ShellSoundsParse_volume(UserData *usr, scrCallBack *arg)
{
    samples_collection1 *v6;
    int v7;

    if ( !strcasecmp(arg->p1, "quit_volume") )
    {
        v6 = &usr->samples1_info;
        v7 = 4;
    }
    else if ( !strcasecmp(arg->p1, "volume_volume") )
    {
        v6 = &usr->samples1_info;
        v7 = 0;
    }
    else if ( !strcasecmp(arg->p1, "button_volume") )
    {
        v6 = &usr->samples1_info;
        v7 = 3;
    }
    else if ( !strcasecmp(arg->p1, "left_volume") )
    {
        v6 = &usr->samples1_info;
        v7 = 2;
    }
    else if ( !strcasecmp(arg->p1, "right_volume") )
    {
        v6 = &usr->samples1_info;
        v7 = 1;
    }
    else if ( !strcasecmp(arg->p1, "slider_volume") )
    {
        v6 = &usr->samples1_info;
        v7 = 5;
    }
    else if ( !strcasecmp(arg->p1, "welcome_volume") )
    {
        v6 = &usr->samples1_info;
        v7 = 6;
    }
    else if ( !strcasecmp(arg->p1, "menuopen_volume") )
    {
        v6 = &usr->samples1_info;
        v7 = 7;
    }
    else if ( !strcasecmp(arg->p1, "overlevel_volume") )
    {
        v6 = &usr->samples1_info;
        v7 = 8;
    }
    else if ( !strcasecmp(arg->p1, "levelselect_volume") )
    {
        v6 = &usr->samples1_info;
        v7 = 9;
    }
    else if ( !strcasecmp(arg->p1, "textappear_volume") )
    {
        v6 = &usr->samples1_info;
        v7 = 10;
    }
    else if ( !strcasecmp(arg->p1, "objectappear_volume") )
    {
        v6 = &usr->samples1_info;
        v7 = 11;
    }
    else if ( !strcasecmp(arg->p1, "sectorconquered_volume") )
    {
        v7 = 12;
        v6 = &usr->samples1_info;
    }
    else if ( !strcasecmp(arg->p1, "vhcldestroyed_volume") )
    {
        v7 = 13;
        v6 = &usr->samples1_info;
    }
    else if ( !strcasecmp(arg->p1, "bldgconquered_volume") )
    {
        v7 = 14;
        v6 = &usr->samples1_info;
    }
    else if ( !strcasecmp(arg->p1, "timercount_volume") )
    {
        v7 = 15;
        v6 = &usr->samples1_info;
    }
    else if ( !strcasecmp(arg->p1, "select_volume") )
    {
        v6 = &usr->samples2_info;
        v7 = 0;
    }
    else if ( !strcasecmp(arg->p1, "error_volume") )
    {
        v6 = &usr->samples2_info;
        v7 = 1;
    }
    else if ( !strcasecmp(arg->p1, "attention_volume") )
    {
        v6 = &usr->samples2_info;
        v7 = 2;
    }
    else if ( !strcasecmp(arg->p1, "secret_volume") )
    {
        v6 = &usr->samples2_info;
        v7 = 3;
    }
    else if ( !strcasecmp(arg->p1, "plasma_volume") )
    {
        v6 = &usr->samples2_info;
        v7 = 4;
    }
    else
        return 3;

    v6->samples_data[v7].volume = atoi(arg->p2);
    return 0;
}

int ShellSoundsParse_pitch(UserData *usr, scrCallBack *arg)
{
    samples_collection1 *v4;
    int v5;

    if ( !strcasecmp(arg->p1, "quit_pitch") )
    {
        v4 = &usr->samples1_info;
        v5 = 4;

    }
    else if ( !strcasecmp(arg->p1, "volume_pitch") )
    {
        v4 = &usr->samples1_info;
        v5 = 0;
    }
    else if ( !strcasecmp(arg->p1, "button_pitch") )
    {
        v4 = &usr->samples1_info;
        v5 = 3;

    }
    else if ( !strcasecmp(arg->p1, "left_pitch") )
    {
        v4 = &usr->samples1_info;
        v5 = 2;

    }
    else if ( !strcasecmp(arg->p1, "right_pitch") )
    {
        v4 = &usr->samples1_info;
        v5 = 1;

    }
    else if ( !strcasecmp(arg->p1, "slider_pitch") )
    {
        v4 = &usr->samples1_info;
        v5 = 5;

    }
    else if ( !strcasecmp(arg->p1, "welcome_pitch") )
    {
        v4 = &usr->samples1_info;
        v5 = 6;

    }
    else if ( !strcasecmp(arg->p1, "menuopen_pitch") )
    {
        v4 = &usr->samples1_info;
        v5 = 7;

    }
    else if ( !strcasecmp(arg->p1, "overlevel_pitch") )
    {
        v4 = &usr->samples1_info;
        v5 = 8;

    }
    else if ( !strcasecmp(arg->p1, "levelselect_pitch") )
    {
        v4 = &usr->samples1_info;
        v5 = 9;

    }
    else if ( !strcasecmp(arg->p1, "textappear_pitch") )
    {
        v4 = &usr->samples1_info;
        v5 = 10;

    }
    else if ( !strcasecmp(arg->p1, "objectappear_pitch") )
    {
        v4 = &usr->samples1_info;
        v5 = 11;

    }
    else if ( !strcasecmp(arg->p1, "sectorconquered_pitch") )
    {
        v5 = 12;
        v4 = &usr->samples1_info;

    }
    else if ( !strcasecmp(arg->p1, "vhcldestroyed_pitch") )
    {
        v5 = 13;
        v4 = &usr->samples1_info;

    }
    else if ( !strcasecmp(arg->p1, "bldgconquered_pitch") )
    {
        v5 = 14;
        v4 = &usr->samples1_info;

    }
    else if ( !strcasecmp(arg->p1, "timercount_pitch") )
    {
        v5 = 15;
        v4 = &usr->samples1_info;

    }
    else if ( !strcasecmp(arg->p1, "select_pitch") )
    {
        v4 = &usr->samples2_info;
        v5 = 0;
    }
    else if ( !strcasecmp(arg->p1, "error_pitch") )
    {
        v4 = &usr->samples2_info;
        v5 = 1;
    }
    else if ( !strcasecmp(arg->p1, "attention_pitch") )
    {
        v4 = &usr->samples2_info;
        v5 = 2;
    }
    else if ( !strcasecmp(arg->p1, "secret_pitch") )
    {
        v4 = &usr->samples2_info;
        v5 = 3;
    }
    else if ( !strcasecmp(arg->p1, "plasma_pitch") )
    {
        v4 = &usr->samples2_info;
        v5 = 4;
    }
    else
        return 3;

    v4->samples_data[v5].pitch = atoi(arg->p2);
    return 0;
}


int ShellSoundsParse(scrCallBack *arg)
{
    UserData *usr = NULL;
    if ( arg->dataForStore )
        usr = (UserData *)arg->dataForStore;

    if ( !arg->field_18 )
    {
        if ( !strcasecmp(arg->p1, "begin_shellsounds") )
        {
            arg->field_18 = 1;
            return 1;
        }
        else
            return 3;
    }
    else
    {
        if ( !strcasecmp(arg->p1, "end") )
        {
            arg->field_18 = 0;
            return 2;
        }
        else if ( usr )
        {
            int result = ShellSoundsParse_sample(usr, arg);
            if ( result == 3 )
            {
                result = ShellSoundsParse_volume(usr, arg);
                if ( result == 3 )
                    result = ShellSoundsParse_pitch(usr, arg);
            }
            return result;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

int ShellTracksParse(scrCallBack *arg)
{
    UserData *usr = NULL;
    if ( arg->dataForStore )
        usr = (UserData *)arg->dataForStore;

    if ( !arg->field_18 )
    {
        if ( !strcasecmp(arg->p1, "begin_shelltracks") )
        {
            arg->field_18 = 1;
            return 1;
        }
        else
            return 3;
    }
    else
    {
        if ( !strcasecmp(arg->p1, "end") )
        {
            arg->field_18 = 0;
            return 2;
        }

        if ( usr )
        {
            char buf[300];
            strcpy(buf, arg->p2);

            if ( !strcasecmp(arg->p1, "shelltrack") )
            {
                usr->shelltrack__adv.field_0 = 0;
                usr->shelltrack__adv.field_4 = 0;
                char *v8 = strtok(buf, " \t_\n");
                usr->shelltrack = strtol(v8, NULL, 0);
                v8 = strtok(0, " \t_\n");
                if ( v8 )
                {
                    usr->shelltrack__adv.field_0 = strtol(v8, NULL, 0);
                    v8 = strtok(0, " \t_\n");
                    if ( v8 )
                        usr->shelltrack__adv.field_4 = strtol(v8, NULL, 0);
                }
            }
            else if ( !strcasecmp(arg->p1, "missiontrack") )
            {
                usr->missiontrack__adv.field_0 = 0;
                usr->missiontrack__adv.field_4 = 0;
                char *v8 = strtok(buf, " \t_\n");
                usr->missiontrack = strtol(v8, NULL, 0);
                v8 = strtok(0, " \t_\n");
                if ( v8 )
                {
                    usr->missiontrack__adv.field_0 = strtol(v8, NULL, 0);
                    v8 = strtok(0, " \t_\n");
                    if ( v8 )
                        usr->missiontrack__adv.field_4 = strtol(v8, NULL, 0);
                }
            }
            else if ( !strcasecmp(arg->p1, "debriefingtrack") )
            {
                usr->debriefingtrack__adv.field_0 = 0;
                usr->debriefingtrack__adv.field_4 = 0;
                char *v8 = strtok(buf, " \t_\n");
                usr->debriefingtrack = strtol(v8, NULL, 0);
                v8 = strtok(0, " \t_\n");
                if ( v8 )
                {
                    usr->debriefingtrack__adv.field_0 = strtol(v8, NULL, 0);
                    v8 = strtok(0, " \t_\n");
                    if ( v8 )
                        usr->debriefingtrack__adv.field_4 = strtol(v8, NULL, 0);
                }
            }
            else if ( !strcasecmp(arg->p1, "loadingtrack") )
            {
                usr->loadingtrack__adv.field_0 = 0;
                usr->loadingtrack__adv.field_4 = 0;
                char *v8 = strtok(buf, " \t_\n");
                usr->loadingtrack = strtol(v8, NULL, 0);
                v8 = strtok(0, " \t_\n");
                if ( v8 )
                {
                    usr->loadingtrack__adv.field_0 = strtol(v8, NULL, 0);
                    v8 = strtok(0, " \t_\n");
                    if ( v8 )
                        usr->loadingtrack__adv.field_4 = strtol(v8, NULL, 0);
                }
            }
            else
                return 3;
        }
    }
    return 0;
}


int  ShellSoundsLoad(UserData *usr)
{
    scrCallBack v4[2];

    v4[0].dataForStore = usr;
    v4[0].func = ShellSoundsParse;
    v4[1].dataForStore = usr;
    v4[1].func = ShellTracksParse;

    return def_parseFile("data:world.ini", 2, v4, 2) != 0;
}


int sub_476074(int tp, int id)
{
    int result = -1;

    if ( tp == 1 )
    {
        switch ( id )
        {
        case 0:
            result = 10;
            break;
        case 1:
            result = 11;
            break;
        case 2:
            result = 12;
            break;
        case 3:
            result = 9;
            break;
        case 4:
            result = 26;
            break;
        default:
            break;
        }
    }
    else if ( tp == 2 )
    {
        switch ( id )
        {
        case 0:
            result = 8;
            break;
        case 1:
            result = 6;
            break;
        case 2:
            result = 7;
            break;
        case 3:
            result = 3;
            break;
        case 4:
            result = 4;
            break;
        case 5:
            result = 5;
            break;
        default:
            break;
        }
    }
    else if ( tp == 3 )
    {
        switch ( id )
        {
        case 0:
            result = 16;
            break;
        case 1:
            result = 37;
            break;
        case 2:
            result = 17;
            break;
        case 3:
            result = 18;
            break;
        case 4:
            result = 35;
            break;
        case 7:
            result = 15;
            break;
        case 8:
            result = 25;
            break;
        case 9:
            result = 19;
            break;
        case 10:
            result = 27;
            break;
        case 11:
            result = 28;
            break;
        case 12:
            result = 29;
            break;
        case 14:
            result = 31;
            break;
        case 16:
            result = 32;
            break;
        case 17:
            result = 33;
            break;
        case 18:
            result = 30;
            break;
        case 20:
            result = 41;
            break;
        case 21:
            result = 38;
            break;
        case 22:
            result = 40;
            break;
        case 23:
            result = 39;
            break;
        case 24:
            result = 2;
            break;
        case 25:
            result = 14;
            break;
        case 27:
            result = 34;
            break;
        case 31:
            result = 42;
            break;
        case 32:
            result = 1;
            break;
        case 37:
            result = 43;
            break;
        case 38:
            result = 20;
            break;
        case 39:
            result = 21;
            break;
        case 40:
            result = 22;
            break;
        case 41:
            result = 23;
            break;
        case 42:
            result = 24;
            break;
        case 43:
            result = 44;
            break;
        case 44:
            result = 36;
            break;
        case 45:
            result = 13;
            break;
        case 46:
            result = 45;
            break;
        default:
            break;
        }
    }
    return result;
}

int sub_476204(const char *a4)
{
    for (int i = 0; i < 256; i++)
    {
        if (keySS[i].short_name && !strcasecmp(keySS[i].short_name, a4) )
            return i;
    }
    return -1;
}

int parseSaveInput(scrCallBack *arg)
{
    NC_STACK_input *inpt;
    inputEngine__getter(0x80001009, &inpt, 0);

    UserData *usr = (UserData *)arg->dataForStore;

    if ( !arg->field_18 )
    {
        if ( !strcasecmp(arg->p1, "new_input") || !strcasecmp(arg->p1, "modify_input") )
        {
            if ( !strcasecmp(arg->p1, "new_input") )
            {
                if ( usr )
                {
                    for (int i = 0; i < 46; i++)
                        usr->keyConfig[i].KeyCode = 0;
                }
            }
            arg->field_18 = 1;
            return 1;
        }
        else
        {
            return 3;
        }
    }
    else
    {
        if ( !strcasecmp(arg->p1, "end") )
        {
            arg->field_18 = 0;
            return 2;
        }

        if ( !usr )
            return 0;

        usr->snd__flags1 |= 2;

        if ( !strcasecmp(arg->p1, "qualmode") )
        {

        }
        else if ( !strcasecmp(arg->p1, "joystick") )
        {
            if ( !strcasecmp(arg->p2, "yes") || !strcasecmp(arg->p2, "on") )
            {
                usr->inp_joystick = 1;
                usr->p_ypaworld->field_73CE &= 0xFB;
            }
            else
            {
                usr->inp_joystick = 0;
                usr->p_ypaworld->field_73CE |= 4;
            }
        }
        else if ( !strcasecmp(arg->p1, "altjoystick") )
        {
            if ( !strcasecmp(arg->p2, "yes") || !strcasecmp(arg->p2, "on") )
            {
                usr->inp_altjoystick = 1;
                usr->p_ypaworld->snd__cdsound |= 2;
            }
            else
            {
                usr->inp_altjoystick = 0;
                usr->p_ypaworld->snd__cdsound &= 0xFD;
            }
        }
        else if ( !strcasecmp(arg->p1, "forcefeedback") )
        {
            if ( !strcasecmp(arg->p2, "yes") || !strcasecmp(arg->p2, "on") )
            {
                usr->p_ypaworld->field_73CE &= 0xF7;
            }
            else
            {
                usr->p_ypaworld->field_73CE |= 8;
            }
        }
        else
        {
            char *v11 = arg->p2;
            while ( 1 )
            {
                v11 = strpbrk(v11, "_");
                if ( !v11 )
                    break;
                *v11 = ' ';
            }

            char v39[500];

            char *v13 = v39;
            int ln = strlen(arg->p2);

            for (int i = 0; i < ln ; i++ )
            {
                if ( (arg->p2[i] == '$') )
                {
                    strcpy(v13, "winp:");
                    v13 += strlen("winp:");
                }
                else
                {
                    *v13 = arg->p2[i];
                    v13++;
                }
            }
            *v13 = 0;

            if ( !strnicmp(arg->p1, "input.slider[", 13) )
            {
                int v18 = 0;

                char *tmp = strtok(arg->p1 + 13, "] \t=\n");
                int v16 = atoi(tmp);

                input__func64__params v44;
                v44.value = v39;
                v44.type_id = 5;
                v44.item_number = v16;

                if ( inpt->input_func64(&v44) )
                {
                    int v19 = sub_476074(2, v16);
                    if ( v19 == -1 )
                    {
                        ypa_log_out("Unknown number in slider-declaration (%d)\n", v16);
                        return 4;
                    }
                    usr->keyConfig[ v19 ].inp_type = 2;
                    usr->keyConfig[ v19 ].keyID = v16;

                    if ( strtok(v39, ":") )
                    {
                        tmp = strtok(0, " \t");

                        if ( tmp )
                        {
                            usr->keyConfig[ v19 ].slider_neg = sub_476204(tmp);

                            if ( usr->keyConfig[ v19 ].slider_neg == -1 )
                            {
                                ypa_log_out("Unknown keyword for slider %s\n", tmp);
                                return 4;
                            }

                            if ( strtok(0, ":") )
                            {
                                tmp = strtok(0, " \t\n");

                                if ( tmp )
                                {
                                    usr->keyConfig[ v19 ].KeyCode = sub_476204(tmp);

                                    if ( usr->keyConfig[ v19 ].KeyCode == -1 )
                                    {
                                        ypa_log_out("Unknown keyword for slider %s\n", tmp);
                                        return 4;
                                    }
                                    v18 = 1;
                                }
                            }
                        }
                    }
                }
                else
                {
                    ypa_log_out("WARNING: cannot set slider %d with %s\n", v16, v39);
                }

                if ( !v18 )
                {
                    ypa_log_out("Wrong input expression for slider %d\n", v16);
                    return 4;
                }
            }
            else if ( !strnicmp(arg->p1, "input.button[", 13) )
            {
                int v18 = 0;

                char *tmp = strtok(arg->p1 + 13, "] \t=\n");
                int v16 = atoi(tmp);

                input__func64__params v44;
                v44.value = v39;
                v44.type_id = 4;
                v44.item_number = v16;

                if ( inpt->input_func64(&v44) )
                {
                    int v19 = sub_476074(1, v16);
                    if ( v19 == -1 )
                    {
                        ypa_log_out("Unknown number in button-declaration (%d)\n", v16);
                        return 4;
                    }
                    usr->keyConfig[ v19 ].inp_type = 1;
                    usr->keyConfig[ v19 ].keyID = v16;

                    if ( strtok(v39, ":") )
                    {
                        tmp = strtok(0, " \t");
                        if ( tmp )
                        {
                            usr->keyConfig[ v19 ].KeyCode = sub_476204(tmp);

                            if ( usr->keyConfig[ v19 ].KeyCode == -1 )
                            {
                                ypa_log_out("Unknown keyword for button %s\n", tmp);
                                return 4;
                            }
                            v18 = 1;
                        }
                    }
                }
                else
                {
                    ypa_log_out("WARNING: cannot set button %d with %s\n", v16, v39);
                }

                if ( !v18 )
                {
                    ypa_log_out("Wrong input expression for button %d\n", v16);
                    return 4;
                }
            }
            else if ( !strnicmp(arg->p1, "input.hotkey[", 13) )
            {
                int v18 = 0;

                char *tmp = strtok(arg->p1 + 13, "] \t=\n");
                int v16 = atoi(tmp);

                winp_68arg zz;
                zz.keyname = v39;
                zz.id = v16;

                input__func66__params v44;
                v44.funcID = 68;
                v44.field_4 = 0;
                v44.field_0 = 3;
                v44.vals = &zz;

                if ( inpt->input_func66(&v44) )
                {
                    int v19 = sub_476074(3, v16);
                    if ( v19 == -1 )
                    {
                        ypa_log_out("Unknown number in hotkey-declaration (%d)\n", v16);
                        return 0;
                    }

                    usr->keyConfig[ v19 ].inp_type = 3;
                    usr->keyConfig[ v19 ].keyID = v16;

                    tmp = strtok(v39, " \t\n");
                    if ( tmp )
                    {
                        usr->keyConfig[ v19 ].KeyCode = sub_476204(tmp);
                        if ( usr->keyConfig[ v19 ].KeyCode == -1 )
                        {
                            ypa_log_out("Unknown keyword for hotkey: %s\n", tmp);
                            return 4;
                        }
                        v18 = 1;
                    }
                }
                else
                {
                    ypa_log_out("WARNING: cannot set hotkey %d with %s\n", v16, v39);
                }
                if ( !v18 )
                {
                    ypa_log_out("Wrong input expression for hotkey %d\n", v16);
                    return 4;
                }
            }
            else
            {
                ypa_log_out("Unknown keyword %s in InputExpression\n", arg->p1);
                return 3;
            }
        }
    }
    return 0;
}



int LevelSquadParser(scrCallBack *arg)
{
    mapProto *mapp = (mapProto *)arg->dataForStore;

    squadProto *squad = &mapp->squads[ mapp->squad_count ];

    if ( !arg->field_18 )
    {
        if ( !strcasecmp(arg->p1, "begin_squad") )
        {
            squad->mb_status = 0;
            arg->field_18 = 1;
            return 1;
        }
        else
            return 3;
    }
    else
    {
        if ( mapp->squad_count >= 96 )
            return 4;

        if ( !strcasecmp(arg->p1, "end") )
        {
            if ( !squad->vehicle )
            {
                ypa_log_out("Squad init: squad[%d]аno vehicle defined!\n", mapp->squad_count);
                return 4;
            }

            if ( !squad->num )
            {
                ypa_log_out("Squad init: squad[%d] num of vehicles is 0!\n", mapp->squad_count);
                return 4;
            }

            if ( squad->pos_x == 0.0 || squad->pos_z == 0.0 )
            {
                ypa_log_out("Squad init: squad[%d] no pos given!\n", mapp->squad_count);
                return 4;
            }

            arg->field_18 = 0;
            squad->field_0 = 1;
            mapp->squad_count++;
            return 2;
        }
        else if ( !strcasecmp(arg->p1, "owner") )
        {
            squad->owner = strtol(arg->p2, NULL, 0);
        }
        else if ( !strcasecmp(arg->p1, "useable") )
        {
            squad->useable = 1;
        }
        else if ( !strcasecmp(arg->p1, "vehicle") )
        {
            squad->vehicle = strtol(arg->p2, NULL, 0);
        }
        else if ( !strcasecmp(arg->p1, "num") )
        {
            squad->num = strtol(arg->p2, NULL, 0);
        }
        else if ( !strcasecmp(arg->p1, "pos_x") )
        {
            squad->pos_x = strtod(arg->p2, 0) + 0.3;
        }
        else if ( !strcasecmp(arg->p1, "pos_z") )
        {
            squad->pos_z = strtod(arg->p2, 0) + 0.3;
        }
        else if ( !strcasecmp(arg->p1, "mb_status") )
        {
            if ( !strcasecmp(arg->p2, "known") )
            {
                squad->mb_status = 0;
            }
            else if ( !strcasecmp(arg->p2, "unknown") )
            {
                squad->mb_status = 1;
            }
            else if ( !strcasecmp(arg->p2, "hidden") )
            {
                squad->mb_status = 2;
            }
            else
                return 4;
        }
        else
            return 3;

        return 0;
    }
}

int LevelGatesParser(scrCallBack *arg)
{
    _NC_STACK_ypaworld *yw = arg->world;
    gateProto *gate = &yw->field_2d90->gates[ yw->field_2d90->gate_count ];

    if ( !arg->field_18 )
    {
        if ( !strcasecmp(arg->p1, "begin_gate") )
        {
            gate->mb_status = 0;
            arg->field_18 = 1;
            return 1;
        }
        else
            return 3;
    }
    else
    {

        if ( yw->field_2d90->gate_count > 8 )
            return 4;

        if ( !strcasecmp(arg->p1, "end") )
        {
            if ( !gate->closed_bp )
            {
                ypa_log_out("Gate init: gate[%d] no closed building defined!\n", yw->field_2d90->gate_count);
                return 4;
            }

            if ( !gate->opened_bp )
            {
                ypa_log_out("Gate init: gate[%d] no opened building defined!\n", yw->field_2d90->gate_count);
                return 4;
            }

            if ( gate->sec_x == 0 || gate->sec_y == 0)
            {
                ypa_log_out("Gate init: gate[%d] no sector coords!\n", yw->field_2d90->gate_count);
                return 4;
            }

            if ( !gate->target_level_count )
            {
                ypa_log_out("Gate init: gate[%d] no target levels defined!\n", yw->field_2d90->gate_count);
                return 4;
            }

            yw->field_2d90->gate_count++;
            arg->field_18 = 0;
            return 2;
        }
        else if ( !strcasecmp(arg->p1, "sec_x") )
        {
            gate->sec_x = strtol(arg->p2, NULL, 0);
        }
        else if ( !strcasecmp(arg->p1, "sec_y") )
        {
            gate->sec_y = strtol(arg->p2, NULL, 0);
        }
        else if ( !strcasecmp(arg->p1, "closed_bp") )
        {
            gate->closed_bp = strtol(arg->p2, NULL, 0);
        }
        else if ( !strcasecmp(arg->p1, "opened_bp") )
        {
            gate->opened_bp = strtol(arg->p2, NULL, 0);
        }
        else if ( !strcasecmp(arg->p1, "target_level") )
        {
            if ( gate->target_level_count < 8 )
            {
                gate->target_levels[ gate->target_level_count ] = strtol(arg->p2, NULL, 0);
                gate->target_level_count++;
            }
        }
        else if ( !strcasecmp(arg->p1, "keysec_x") )
        {
            if ( gate->keySectors_count < 16 )
            {
                gate->keySectors[gate->keySectors_count].x = strtol(arg->p2, NULL, 0);
            }
        }
        else if ( !strcasecmp(arg->p1, "keysec_y") )
        {
            if ( gate->keySectors_count < 16 )
            {
                gate->keySectors[ gate->keySectors_count ].y = strtol(arg->p2, NULL, 0);
                gate->keySectors_count++;
            }
        }
        else if ( !strcasecmp(arg->p1, "mb_status") )
        {
            if ( !strcasecmp(arg->p2, "known") )
            {
                gate->mb_status = 0;
            }
            else if ( !strcasecmp(arg->p2, "unknown") )
            {
                gate->mb_status = 1;
            }
            else if ( !strcasecmp(arg->p2, "hidden") )
            {
                gate->mb_status = 2;
            }
            else
                return 4;
        }
        else
            return 3;
        return 0;
    }
}


int LevelMbMapParser(scrCallBack *arg)
{
    mapProto *mapp = (mapProto *)arg->dataForStore;
    dbmapProto *mbmap = (dbmapProto *)&mapp->mbmaps[ mapp->mbmaps_count ];

    if ( !arg->field_18 )
    {
        if ( !strcasecmp(arg->p1, "begin_mbmap") )
        {
            arg->field_18 = 1;
            return 1;
        }
        else
            return 3;
    }
    else
    {

        if ( mapp->mbmaps_count >= 4 )
        {
            ypa_log_out("Mission Briefing Init: Too many bg maps (max=%d)!\n", 4);
            return 4;
        }

        if ( !strcasecmp(arg->p1, "end") )
        {
            mapp->mbmaps_count++;
            arg->field_18 = 0;
            return 2;
        }

        if ( !strcasecmp(arg->p1, "name") )
        {
            strcpy(mbmap->name, arg->p2);
        }
        else if ( !strcasecmp(arg->p1, "size_x") )
        {
            mbmap->size_x = strtol(arg->p2, NULL, 0);
        }
        else if ( !strcasecmp(arg->p1, "size_y") )
        {
            mbmap->size_y = strtol(arg->p2, NULL, 0);
        }
        else
            return 3;

        return 0;
    }
}


int LevelGemParser(scrCallBack *arg)
{
    int v33 = (int)arg->world2;

    _NC_STACK_ypaworld *yw = arg->world;
    gemProto *v3 = &yw->gems[v33];

    if ( !arg->field_18 )
    {
        if ( !strcasecmp(arg->p1, "begin_gem") )
        {
            v3->mb_status = 0;
            arg->field_18 = 1;
            arg->field_24 = 0;
            return 1;
        }
        else
            return 3;
    }
    else
    {

        if ( v33 > 7 )
            return 4;

        char v29[32];

        sprintf(v29, "msg_%s", yw->lang_name);

        if ( arg->field_24 )
        {
            if ( !strcasecmp(arg->p1, "end_action") )
            {
                v3->end_action__line = arg->line_number;
                arg->field_24 = 0;
            }
            return 0;
        }

        if ( !strcasecmp(arg->p1, "end") )
        {
            if ( !v3->building )
            {
                ypa_log_out("WStein init: gem[%d] no building defined!\n", v33);
                return 4;
            }

            if ( v3->sec_x == 0 || v3->sec_y == 0 )
            {
                ypa_log_out("WStein init: gem[%d] sector pos wonky tonk!\n", v33);
                return 4;
            }

            v3->field_0 = 1;
            arg->field_18 = 0;
            arg->world2 = (_NC_STACK_ypaworld *)(v33 + 1);
            return 2;
        }

        if ( strstr(arg->p1, "msg_") )
        {
            if ( strcasecmp(arg->p1, "msg_default") && strcasecmp(arg->p1, v29) )
                return 0;

            if ( strlen(arg->p2) >= 127 )
            {
                ypa_log_out("LevelGemParser(): Msg too long!");
                return 4;
            }

            strcpy(v3->msg_default, arg->p2);
        }
        else if ( !strcasecmp(arg->p1, "sec_x") )
        {
            v3->sec_x = strtol(arg->p2, NULL, 0);
        }
        else if ( !strcasecmp(arg->p1, "sec_y") )
        {
            v3->sec_y = strtol(arg->p2, NULL, 0);
        }
        else if ( !strcasecmp(arg->p1, "building") )
        {
            v3->building = strtol(arg->p2, NULL, 0);
        }
        else if ( !strcasecmp(arg->p1, "type") )
        {
            switch ( strtol(arg->p2, NULL, 0) )
            {
            case 1:
                v3->type = 25;
                break;
            case 2:
                v3->type = 26;
                break;
            case 3:
                v3->type = 27;
                break;
            case 4:
                v3->type = 28;
                break;
            case 5:
                v3->type = 78;
                break;
            case 6:
                v3->type = 79;
                break;
            default:
                v3->type = 80;
                break;
            }
        }
        else if ( !strcasecmp(arg->p1, "script") )
        {
            if ( strlen(arg->p2) >= 63 )
            {
                ypa_log_out("LevelGemParser(): Script name too long!");
                return 4;
            }

            strcpy(v3->script, arg->p2);

            FILE *v20 = FOpen(v3->script, "r");

            if ( v20 )
            {
                FClose(v20);
            }
            else
            {
                return 4;
            }
        }
        else if ( !strcasecmp(arg->p1, "mb_status") )
        {
            if ( !strcasecmp(arg->p2, "known") )
            {
                v3->mb_status = 0;
            }
            else if ( !strcasecmp(arg->p2, "unknown") )
            {
                v3->mb_status = 1;
            }
            else if ( !strcasecmp(arg->p2, "hidden") )
            {
                v3->mb_status = 2;
            }
            else
            {
                return 4;
            }
        }
        else if ( !strcasecmp(arg->p1, "nw_vproto_num") )
        {
            char *v21 = strtok(arg->p2, "_ \t");
            if ( v21 )
            {
                v3->nw_vproto_num_1 = strtol(v21, NULL, 0);
                v21 = strtok(0, "_ \t");
                if ( v21 )
                {
                    v3->nw_vproto_num_2 = strtol(v21, NULL, 0);
                    v21 = strtok(0, "_ \t");
                    if ( v21 )
                    {
                        v3->nw_vproto_num_3 = strtol(v21, NULL, 0);
                        v21 = strtok(0, "_ \t");
                        if ( v21 )
                        {
                            v3->nw_vproto_num_4 = strtol(v21, NULL, 0);
                        }
                    }
                }
            }
        }
        else if ( !strcasecmp(arg->p1, "nw_bproto_num") )
        {
            char *v25 = strtok(arg->p2, "_ \t");
            if ( v25 )
            {
                v3->nw_bproto_num_1 = strtol(v25, NULL, 0);
                v25 = strtok(0, "_ \t");
                if ( v25 )
                {
                    v3->nw_bproto_num_2 = strtol(v25, NULL, 0);
                    v25 = strtok(0, "_ \t");
                    if ( v25 )
                    {
                        v3->nw_bproto_num_3 = strtol(v25, NULL, 0);
                        v25 = strtok(0, "_ \t");
                        if ( v25 )
                        {
                            v3->nw_bproto_num_4 = strtol(v25, NULL, 0);
                        }
                    }
                }
            }
        }
        else if ( !strcasecmp(arg->p1, "begin_action") )
        {
            v3->begin_action__line = arg->line_number;
            arg->field_24 = 1;
            return 0;
        }
        else if ( !arg->field_24 )
            return 3;

        return 0;
    }
}



int LevelEnableParser(scrCallBack *arg)
{
    if ( arg->field_18 )
    {
        int v15 = (int)arg->world2;

        if ( !strcasecmp(arg->p1, "end") )
        {
            arg->field_18 = 0;
            return 2;
        }

        if ( !strcasecmp(arg->p1, "vehicle") )
        {

            int v17 = strtol(arg->p2, NULL, 0);
            if ( v17 >= 0 && v17 < 256 )
            {
                arg->world->VhclProtos[v17].disable_enable_bitmask |= (1 << v15);
            }
            else
                return 4;
        }
        else if ( !strcasecmp(arg->p1, "building") )
        {
            int v20 = strtol(arg->p2, NULL, 0);
            if ( v20 >= 0 && v20 < 128 )
            {
                arg->world->BuildProtos[v20].enable_mask |= (1 << v15);
            }
            else
                return 4;
        }
        else
            return 3;

        return 0;
    }
    else
    {
        if ( !strcasecmp(arg->p1, "begin_enable") )
        {
            int v5 = strtol(arg->p2, NULL, 0);

            for (int i = 0; i < 256; i++)
            {
                arg->world->VhclProtos[i].disable_enable_bitmask &= ~(1 << v5);
            }

            for (int i = 0; i < 128; i++)
            {
                arg->world->BuildProtos[i].enable_mask &= ~(1 << v5);
            }

            arg->field_18 = 1;
            arg->world2 = (_NC_STACK_ypaworld *)v5;

            return 1;
        }
        else
            return 3;
    }
}

int LeveldbMapParser(scrCallBack *arg)
{
    mapProto *mapp = (mapProto *)arg->dataForStore;
    dbmapProto *dbmap = (dbmapProto *)&mapp->dbmaps[ mapp->dbmap_count ];

    if ( !arg->field_18 )
    {
        if ( !strcasecmp(arg->p1, "begin_dbmap") )
        {
            arg->field_18 = 1;
            return 1;
        }
        else
            return 3;
    }
    else
    {

        if ( mapp->mbmaps_count >= 4 )
        {
            ypa_log_out("Debriefing Init: Too many bg maps (max=%d)!\n", 4);
            return 4;
        }

        if ( !strcasecmp(arg->p1, "end") )
        {
            mapp->mbmaps_count++;
            arg->field_18 = 0;
            return 2;
        }

        if ( !strcasecmp(arg->p1, "name") )
        {
            strcpy(dbmap->name, arg->p2);
        }
        else if ( !strcasecmp(arg->p1, "size_x") )
        {
            dbmap->size_x = strtol(arg->p2, NULL, 0);
        }
        else if ( !strcasecmp(arg->p1, "size_y") )
        {
            dbmap->size_y = strtol(arg->p2, NULL, 0);
        }
        else
            return 3;

        return 0;
    }
}

int LevelSuperItemsParser(scrCallBack *arg)
{
    _NC_STACK_ypaworld *yw = arg->world;

    supetItemProto *sitem = &yw->field_2d90->supetItems[yw->field_2d90->supetItems_count];

    if ( !arg->field_18 )
    {
        if ( !strcasecmp(arg->p1, "begin_item") )
        {
            memset(sitem, 0, sizeof(supetItemProto));
            sitem->type = 0;
            sitem->countdown = 60000; //1hour
            sitem->field_4 = 0;
            sitem->mb_status = 0;
            arg->field_18 = 1;
            return 1;
        }
        return 3;
    }
    else
    {
        if ( yw->field_2d90->supetItems_count > 8 )
            return 4;

        if ( !strcasecmp(arg->p1, "end") )
        {
            if ( sitem->sec_x == 0 || sitem->sec_y == 0)
            {
                ypa_log_out("Super item #%d: invalid sector coordinates!\n", yw->field_2d90->supetItems_count);
                return 4;
            }

            if ( !sitem->inactive_bp )
            {
                ypa_log_out("Super item #%d: no <inactive_bp> defined!\n", yw->field_2d90->supetItems_count);
                return 4;
            }

            if ( !sitem->active_bp )
            {
                ypa_log_out("Super item #%d: no <active_bp> defined!\n", yw->field_2d90->supetItems_count);
                return 4;
            }

            if ( !sitem->trigger_bp )
            {
                ypa_log_out("Super item #%d: no <trigger_bp> defined!\n", yw->field_2d90->supetItems_count);
                return 4;
            }

            if ( sitem->type != 1 && sitem->type != 2 )
            {
                ypa_log_out("Super item #%d: no valid <type> defined!\n", yw->field_2d90->supetItems_count);
                return 4;
            }

            yw->field_2d90->supetItems_count++;
            arg->field_18 = 0;
            return 2;
        }

        if ( !strcasecmp(arg->p1, "sec_x") )
        {
            sitem->sec_x = strtol(arg->p2, NULL, 0);
        }
        else if ( !strcasecmp(arg->p1, "sec_y") )
        {
            sitem->sec_y = strtol(arg->p2, NULL, 0);
        }
        else if ( !strcasecmp(arg->p1, "inactive_bp") )
        {
            sitem->inactive_bp = strtol(arg->p2, NULL, 0);
        }
        else if ( !strcasecmp(arg->p1, "active_bp") )
        {
            sitem->active_bp = strtol(arg->p2, NULL, 0);
        }
        else if ( !strcasecmp(arg->p1, "trigger_bp") )
        {
            sitem->trigger_bp = strtol(arg->p2, NULL, 0);
        }
        else if ( !strcasecmp(arg->p1, "keysec_x") )
        {
            if ( sitem->keySectors_count < 16 )
            {
                sitem->keySectors[sitem->keySectors_count].x = strtol(arg->p2, NULL, 0);
            }
        }
        else if ( !strcasecmp(arg->p1, "keysec_y") )
        {
            if ( sitem->keySectors_count < 16 )
            {
                sitem->keySectors[sitem->keySectors_count].y = strtol(arg->p2, NULL, 0);
                sitem->keySectors_count++;
            }
        }
        else if ( !strcasecmp(arg->p1, "mb_status") )
        {
            if ( !strcasecmp(arg->p2, "known") )
            {
                sitem->mb_status = 0;
            }
            else if ( !strcasecmp(arg->p2, "unknown") )
            {
                sitem->mb_status = 1;
            }
            else if ( !strcasecmp(arg->p2, "hidden") )
            {
                sitem->mb_status = 2;
            }
            else
                return 4;
        }
        else if ( !strcasecmp(arg->p1, "type") )
        {
            sitem->type = strtol(arg->p2, NULL, 0);
        }
        else if ( !strcasecmp(arg->p1, "countdown") )
        {
            sitem->countdown = strtol(arg->p2, NULL, 0);
        }
        else
            return 3;

        return 0;
    }
}


NC_STACK_ilbm * readMapDumpAsILBM(_NC_STACK_ypaworld *yw, const char *mapName, FILE *scrFile)
{
    char v15[1024];

    fgets(v15, 1024, scrFile);

    char *v3 = strtok(v15, " \n");
    int w = strtol(v3, NULL, 0);

    v3 = strtok(0, " \n");
    int h = strtol(v3, NULL, 0);

    NC_STACK_bitmap *v7 = (NC_STACK_bitmap *)init_get_class("bitmap.class", 0x80001000, mapName, 0x80002002, w, 0x80002003, h, 0x80002006, 1, 0);
    if ( !v7 )
        return NULL;

    bitmap_intern *a4;
    call_vtbl(v7, 3, 0x80002000, &a4, 0);       // bitmap_func3

    uint8_t *bf = (uint8_t *)a4->buffer;

    for (int j = 0; j < a4->height; j++)
    {
        fgets(v15, 1024, scrFile);

        uint8_t *ln = bf + j * a4->width;

        char *pos = v15;

        for (int i = 0; i < a4->width; i++)
        {
            char * v12 = strtok(pos, " \n");

            if (pos)
                pos = NULL;

            ln[i] = strtol(v12, NULL, 16);

        }
    }

    NC_STACK_ilbm *tmp = (NC_STACK_ilbm *)init_get_class("ilbm.class", 0x80001000, mapName, 0x80003000, 1, 0);
    delete_class_obj(v7);

    return tmp;
}


int LevelMapsParser(scrCallBack *arg)
{
    _NC_STACK_ypaworld *yw = arg->world;

    if ( !arg->field_18 )
    {
        if ( !strcasecmp(arg->p1, "begin_maps") )
        {
            if ( yw->typ_map )
            {
                delete_class_obj(yw->typ_map);
                yw->typ_map = NULL;
            }

            if ( yw->own_map )
            {
                delete_class_obj(yw->own_map);
                yw->own_map = NULL;
            }

            if ( yw->hgt_map )
            {
                delete_class_obj(yw->hgt_map);
                yw->hgt_map = NULL;
            }

            if ( yw->blg_map )
            {
                delete_class_obj(yw->blg_map);
                yw->blg_map = NULL;
            }

            arg->field_18 = 1;
            return 1;
        }
        else
            return 3;
    }
    else
    {
        mapProto *mapp = (mapProto *)arg->dataForStore;

        if ( !strcasecmp(arg->p1, "end") )
        {
            arg->field_18 = 0;
            return 2;
        }

        if ( !strcasecmp(arg->p1, "typ_map") )
        {
            yw->typ_map = readMapDumpAsILBM(yw, "typmap", arg->file);

            if ( !yw->typ_map )
                return 4;

            call_vtbl(yw->typ_map, 3, 0x80002002, &mapp->secXsize, 0);
            call_vtbl(yw->typ_map, 3, 0x80002003, &mapp->secYsize, 0);

            mapp->flags |= 4;
        }
        else if ( !strcasecmp(arg->p1, "own_map") )
        {
            yw->own_map = readMapDumpAsILBM(yw, "ownmap", arg->file);
            if ( !yw->own_map )
                return 4;

            mapp->flags |= 8;
        }
        else if ( !strcasecmp(arg->p1, "hgt_map") )
        {
            yw->hgt_map = readMapDumpAsILBM(yw, "hgtmap", arg->file);
            if ( !yw->hgt_map )
                return 4;

            mapp->flags |= 0x10;
        }
        else if ( !strcasecmp(arg->p1, "blg_map") )
        {
            yw->blg_map = readMapDumpAsILBM(yw, "blgmap", arg->file);
            if ( !yw->blg_map )
                return 4;

            mapp->flags |= 0x20;
        }
        else
            return 3;

        return 0;
    }
}



int parseSaveVideo(scrCallBack *arg)
{
    NC_STACK_ypaworld *ywo = (NC_STACK_ypaworld *)arg->world;

    UserData *usr = (UserData *)arg->dataForStore;

    if ( !arg->field_18 )
    {
        if ( !strcasecmp(arg->p1, "new_video") )
        {
            arg->field_18 = 1;
            return 1;
        }
        else
            return 3;
    }
    else
    {
        if ( !strcasecmp(arg->p1, "end") )
        {
            NC_STACK_win3d *win3d = NULL;
            gfxEngine__getter(0x8000300D, &win3d, 0);

            if ( win3d )
            {
                int txt16bit;
                int simple_d3d;

                call_vtbl(win3d, 3, 0x80005003, &txt16bit, 0);
                call_vtbl(win3d, 3, 0x80005004, &simple_d3d, 0);

                if ( simple_d3d )
                    usr->GFX_flags |= 8;
                else
                    usr->GFX_flags &= 0xF7;

                if ( txt16bit )
                    usr->GFX_flags |= 0x10;
                else
                    usr->GFX_flags &= 0xEF;
            }

            arg->field_18 = 0;

            return 2;
        }

        if ( !arg->dataForStore )
            return 0;

        usr->snd__flags1 |= 4;

        if ( !strcasecmp(arg->p1, "videomode") )
        {
            int v5 = atoi(arg->p2);

            video_mode_node *nod = (video_mode_node *)usr->video_mode_list.head;
            video_mode_node *vdmd = NULL;
            int v7 = 0;

            while (nod->next)
            {
                if (v5 == nod->sort_id)
                {
                    vdmd = nod;
                    break;
                }

                v7++;
                nod = (video_mode_node *)nod->next;
            }

            if ( !vdmd )
            {
                ypa_log_out("Warning: This machine doesn't support mode %d\n", v5);

                v7 = 0;
                nod = (video_mode_node *)usr->video_mode_list.head;

                while (nod->next)
                {
                    if (nod->sort_id == 0x2801E0)
                    {
                        vdmd = nod;
                        break;
                    }

                    v7++;
                    nod = (video_mode_node *)nod->next;
                }
            }

            usr->field_FBE = v7;
            usr->p_ypaworld->game_default_res = vdmd->sort_id;
            usr->game_default_res = vdmd->sort_id;
        }
        else if ( !strcasecmp(arg->p1, "farview") )
        {
            if ( !strcasecmp(arg->p2, "yes") )
            {
                usr->GFX_flags |= 1;
                sub_46D370(ywo, 1);
            }
            else
            {
                usr->GFX_flags &= 0xFE;
                sub_46D370(ywo, 0);
            }
        }
        else if ( !strcasecmp(arg->p1, "filtering") )
        {
        }
        else if ( !strcasecmp(arg->p1, "drawprimitive") )
        {
            if ( !strcasecmp(arg->p2, "yes") )
            {
                usr->GFX_flags |= 8;
            }
            else
            {
                usr->GFX_flags &= 0xF7;
            }
        }
        else if ( !strcasecmp(arg->p1, "16bittexture") )
        {
            if ( !strcasecmp(arg->p2, "yes") )
            {
                usr->GFX_flags |= 0x10;
            }
            else
            {
                usr->GFX_flags &= 0xEF;
            }
        }
        else if ( !strcasecmp(arg->p1, "softmouse") )
        {
            if ( !strcasecmp(arg->p2, "yes") )
            {
                usr->GFX_flags |= 4;
                usr->p_ypaworld->field_73CE |= 0x40;

                call_vtbl(usr->p_ypaworld->win3d, 2, 0x80005000, 1, 0);
            }
            else
            {
                usr->GFX_flags &= 0xFB;
                usr->p_ypaworld->field_73CE &= 0xBF;

                call_vtbl(usr->p_ypaworld->win3d, 2, 0x80005000, 0, 0);
            }
        }
        else if ( !strcasecmp(arg->p1, "palettefx") )
        {
        }
        else if ( !strcasecmp(arg->p1, "heaven") )
        {
            if ( !strcasecmp(arg->p2, "yes") )
            {
                usr->GFX_flags |= 2;
                call_vtbl(ywo, 2, 0x8000200C, 1, 0);
            }
            else
            {
                usr->GFX_flags &= 0xFD;
                call_vtbl(ywo, 2, 0x8000200C, 0, 0);
            }
        }
        else if ( !strcasecmp(arg->p1, "fxnumber") )
        {
            usr->fxnumber = atoi(arg->p2);
            usr->p_ypaworld->fxnumber = usr->fxnumber;
        }
        else if ( !strcasecmp(arg->p1, "enemyindicator") )
        {
            if ( !strcasecmp(arg->p2, "yes") )
            {
                usr->p_ypaworld->field_73CE |= 0x20;
                usr->enemyindicator = 1;
            }
            else
            {
                usr->p_ypaworld->field_73CE &= 0xDF;
                usr->enemyindicator = 0;
            }
        }
        else
            return 3;
        return 0;
    }
}

int parseSaveSound(scrCallBack *arg)
{
    if ( !arg->field_18 )
    {
        if ( !strcasecmp(arg->p1, "new_sound") )
        {
            arg->field_18 = 1;
            return 1;
        }
        else
            return 3;
    }
    else
    {
        if ( !strcasecmp(arg->p1, "end") )
        {
            arg->field_18 = 0;
            return 2;
        }

        UserData *usr = (UserData *)arg->dataForStore;

        if ( !usr )
            return 0;

        usr->snd__flags1 |= 8;

        if ( !strcasecmp(arg->p1, "channels") )
        {
        }
        else if ( !strcasecmp(arg->p1, "volume") )
        {
            usr->snd__volume = atoi(arg->p2);
            milesEngine__setter(0x80004003, usr->snd__volume, 0);
        }
        else if ( !strcasecmp(arg->p1, "cdvolume") )
        {
            int v4 = atoi(arg->p2);

            usr->snd__cdvolume = v4;

            sub_4448C0(&v4);
        }
        else if ( !strcasecmp(arg->p1, "invertlr") )
        {
            if ( !strcasecmp(arg->p2, "yes") )
            {
                usr->snd__flags2 |= 1;
                milesEngine__setter(0x80004005, 1, 0);
            }
            else
            {
                usr->snd__flags2 &= 0xFE;
                milesEngine__setter(0x80004005, 0, 0);
            }
        }
        else if ( !strcasecmp(arg->p1, "sound") )
        {
        }
        else if ( !strcasecmp(arg->p1, "cdsound") )
        {
            if ( !strcasecmp(arg->p2, "yes") )
            {
                usr->snd__flags2 |= 0x10;
                usr->p_ypaworld->snd__cdsound |= 1;

                CDAUDIO_t v10;
                v10.command = 8;
                v10.track_id = 1;

                sub_4444D4(&v10);
            }
            else
            {
                usr->snd__flags2 &= 0xEF;
                usr->p_ypaworld->snd__cdsound &= 0xFE;

                CDAUDIO_t v10;
                v10.command = 8;
                v10.track_id = 0;

                sub_4444D4(&v10);
            }
        }
        else
            return 3;

        return 0;
    }
}


int level_id;

int parseSaveLevelStatus(scrCallBack *arg)
{
    _NC_STACK_ypaworld *yw = NULL;

    if ( arg->dataForStore )
        yw = (_NC_STACK_ypaworld *)arg->dataForStore;

    if ( !arg->field_18 )
    {
        if ( !strcasecmp(arg->p1, "begin_levelstatus") )
        {
            arg->field_18 = 1;
            level_id = atoi(arg->p2);
            return 1;
        }
        else
            return 3;
    }
    else
    {
        if ( !strcasecmp(arg->p1, "end") )
        {
            arg->field_18 = 0;
            return 2;
        }
        else
        {
            if ( arg->dataForStore )
            {
                uint8_t *v5 = (BYTE *)arg->world;

                if ( v5 )
                    *v5 |= 0x10;

                if ( !strcasecmp(arg->p1, "status") )
                {
                    if ( yw->LevelNet->mapInfos[level_id].field_0 )
                        yw->LevelNet->mapInfos[level_id].field_0 = atoi(arg->p2);
                }
                else
                    return 3;
            }
            return 0;
        }
    }
}


int parseSaveBuddy(scrCallBack *arg)
{
    _NC_STACK_ypaworld *yw = NULL;

    if ( arg->dataForStore )
        yw = (_NC_STACK_ypaworld *)arg->dataForStore;

    if ( !arg->field_18 )
    {
        if ( !strcasecmp(arg->p1, "begin_buddy") )
        {
            arg->field_18 = 1;
            return 1;
        }
        else
            return 3;
    }
    else
    {
        if ( !strcasecmp(arg->p1, "end") )
        {
            arg->field_18 = 0;

            if ( arg->dataForStore )
                yw->field_2d90->buddies_count++;

            return 2;
        }

        if ( arg->dataForStore )
        {
            if ( !strcasecmp(arg->p1, "commandid") )
            {
                yw->field_2d90->buddies[ yw->field_2d90->buddies_count ].commandid = atoi(arg->p2);
            }
            else if ( !strcasecmp(arg->p1, "type") )
            {
                yw->field_2d90->buddies[ yw->field_2d90->buddies_count ].type = atoi(arg->p2);
            }
            else if ( !strcasecmp(arg->p1, "energy") )
            {
                yw->field_2d90->buddies[ yw->field_2d90->buddies_count ].energy = atoi(arg->p2);
            }
            else
                return 3;
        }

        return 0;
    }
}

void yw_parse_status(save_status *status, char *txt)
{
    memset(status, 0, sizeof(save_status));

    char *v5 = strtok(txt, " _");
    if ( v5 )
        status->p1 = atoi(v5);

    v5 = strtok(0, " _");
    if ( v5 )
        status->p2 = atoi(v5);

    v5 = strtok(0, " _");
    if ( v5 )
        status->p3 = atoi(v5);

    v5 = strtok(0, " _");
    if ( v5 )
        status->p4 = atoi(v5);

    v5 = strtok(0, " _");
    if ( v5 )
        status->p5 = atoi(v5);

    v5 = strtok(0, " _");
    if ( v5 )
        status->p6 = atoi(v5);

    for (int i = 0; i < 8; i++)
    {
        v5 = strtok(0, " _");
        if ( v5 )
            status->pX[i] = atoi(v5);
    }
}

int parseSaveShell(scrCallBack *arg)
{
    UserData *usr = (UserData *)arg->dataForStore;
    _NC_STACK_ypaworld *yw = usr->p_ypaworld;
    NC_STACK_ypaworld *ywo = (NC_STACK_ypaworld *)&arg->world->self_full;

    if ( !arg->field_18 )
    {
        if ( !strcasecmp(arg->p1, "new_shell") )
        {
            arg->field_18 = 1;
            return 1;
        }
        else
            return 3;
    }
    else
    {
        if ( !strcasecmp(arg->p1, "end") )
        {
            yw->field_739A = 1;
            arg->field_18 = 0;
            return 2;
        }

        usr->snd__flags1 |= 0x20;

        char v12[300];
        strncpy(v12, arg->p2, 299);

        if ( !strcasecmp(arg->p1, "LANGUAGE") )
        {
            langDll_node *nod = (langDll_node *)usr->lang_dlls.head;

            langDll_node * deflt = NULL;
            langDll_node * slct = NULL;

            while ( nod->next )
            {
                if ( !strcasecmp(nod->langDllName, arg->p2) )
                    slct = nod;
                if ( !strcasecmp(nod->langDllName, "language") )
                    deflt = nod;

                nod = (langDll_node *)nod->next;
            }

            if ( slct )
                usr->default_lang_dll = slct;
            else
                usr->default_lang_dll = deflt;

            usr->prev_lang = usr->default_lang_dll;

            if ( !ywo->ypaworld_func175(usr) )
            {
                ypa_log_out("Unable to set new language\n");
            }
        }
        else if ( !strcasecmp(arg->p1, "SOUND") || !strcasecmp(arg->p1, "VIDEO") ||
                  !strcasecmp(arg->p1, "INPUT") || !strcasecmp(arg->p1, "DISK") ||
                  !strcasecmp(arg->p1, "LOCALE") || !strcasecmp(arg->p1, "NET") ||
                  !strcasecmp(arg->p1, "FINDER") || !strcasecmp(arg->p1, "LOG") ||
                  !strcasecmp(arg->p1, "ENERGY") || !strcasecmp(arg->p1, "MESSAGE") ||
                  !strcasecmp(arg->p1, "MAP") )
        {

        }
        else if ( !strcasecmp(arg->p1, "robo_map_status") )
        {
            yw_parse_status(&yw->robo_map_status, arg->p2);
        }
        else if ( !strcasecmp(arg->p1, "robo_finder_status") )
        {
            yw_parse_status(&yw->robo_finder_status, arg->p2);
        }
        else if ( !strcasecmp(arg->p1, "vhcl_map_status") )
        {
            yw_parse_status(&yw->vhcl_map_status, arg->p2);
        }
        else if ( !strcasecmp(arg->p1, "vhcl_finder_status") )
        {
            yw_parse_status(&yw->vhcl_finder_status, arg->p2);
        }
        else
            return 3;
        return 0;
    }
}



int sb_0x479f4c__sub0(scrCallBack *scr)
{
    __NC_STACK_yparobo *robo = &current_robo->stack__yparobo;
    _NC_STACK_ypaworld *yw = &robo->wrld->stack__ypaworld;

    if ( !strcasecmp(scr->p1, "owner") )
    {
        dword_5A7A88->owner = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "robostate") )
    {
        robo->field_1DB = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "dockenergy") )
    {
        robo->dock_energ = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "dockcount") )
    {
        robo->dock_cnt = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "dockuser") )
    {
        robo->dock_user = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "docktime") )
    {
        robo->dock_time = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "docktargettype") )
    {
        robo->dock_tgType = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "dockaggr") )
    {
        robo->dock_aggr = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "docktargetpos") )
    {
        char *v5 = strtok(scr->p2, " \t_\n");
        if ( v5 )
        {
            robo->dock_tgt_pos.sx = strtod(v5, 0);
            v5 = strtok(0, " \t_\n");

            if ( v5 )
            {
                robo->dock_tgt_pos.sz = strtod(v5, 0);
            }
        }
    }
    else if ( !strcasecmp(scr->p1, "docktargetID") )
    {
        robo->dock_tgt_comm_id = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "fillmodus") )
    {
        robo->field_501 = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "battbuilding") )
    {
    }
    else if ( !strcasecmp(scr->p1, "battvehicle") )
    {
        robo->field_4F5 = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "buildspare") )
    {
        robo->field_509 = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "battbeam") )
    {
        robo->field_4FD = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "vhoriz") )
    {
        robo->bact_internal->field_5ED = strtod(scr->p2, 0);
    }
    else if ( !strcasecmp(scr->p1, "vvert") )
    {
        robo->bact_internal->field_5F1 = strtod(scr->p2, 0);
    }
    else if ( !strcasecmp(scr->p1, "maximum") )
    {
        robo->bact_internal->energy_2 = atoi(scr->p2);

        if ( robo->bact_internal->owner == 1 )
        {
            if ( robo->bact_internal->energy_2 < yw->maxroboenergy )
            {
                robo->bact_internal->energy_2 = yw->maxroboenergy;
                robo->bact_internal->reload_const_or_energy2 = yw->maxreloadconst;
            }
        }
    }
    else if ( !strcasecmp(scr->p1, "con_budget") )
    {
        robo->field_1E7 = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "def_budget") )
    {
        robo->field_1EA = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "rec_budget") )
    {
        robo->field_1ec = atoi(scr->p2);
    }
    else  if ( !strcasecmp(scr->p1, "rob_budget") )
    {
        robo->field_1EE = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "rad_budget") )
    {
        robo->field_1e8 = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "pow_budget") )
    {
        robo->field_1E9 = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "saf_budget") )
    {
        robo->field_1EB = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "cpl_budget") )
    {
        robo->field_1ED = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "saf_delay") )
    {
        robo->field_24D = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "pow_delay") )
    {
        robo->field_265 = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "rad_delay") )
    {
        robo->field_235 = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "cpl_delay") )
    {
        robo->field_2B1 = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "def_delay") )
    {
        robo->field_281 = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "con_delay") )
    {
        robo->field_299 = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "rec_delay") )
    {
        robo->field_2C9 = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "rob_delay") )
    {
        robo->field_2E1 = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "reload_const") )
    {
        if ( !robo->bact_internal->reload_const_or_energy2 )
            robo->bact_internal->reload_const_or_energy2 = atoi(scr->p2);

    }
    else
        return 0;

    return 1;
}

int sub_47A0C0(scrCallBack *scr)
{
    if ( !strcasecmp(scr->p1, "viewer") )
    {
        if ( !strcasecmp(scr->p2, "yes") )
            call_vtbl(current_bact, 2, 0x80001004, 1, 0);
    }
    else if ( !strcasecmp(scr->p1, "user") )
    {
        if ( !strcasecmp(scr->p2, "yes") )
        {
            call_vtbl(current_bact, 2, 0x80001005, 1, 0);
        }
    }
    else if ( !strcasecmp(scr->p1, "collision") )
    {
        if ( !strcasecmp(scr->p2, "yes") )
        {
            call_vtbl(current_bact, 2, 0x80001007, 1, 0);
        }
    }
    else if ( !strcasecmp(scr->p1, "commandid") )
    {
        dword_5A7A88->field_2E = atoi(scr->p2);

        if ( dword_5A7A80 < dword_5A7A88->field_2E )
        {
            dword_5A7A80 = dword_5A7A88->field_2E;
        }
    }
    else if ( !strcasecmp(scr->p1, "aggression") )
    {
        dword_5A7A88->field_3D4 = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "mainstate") )
    {
        dword_5A7A88->field_3D5 = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "extrastate") )
    {
        dword_5A7A88->field_3D6 = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "killerowner") )
    {
        dword_5A7A88->field_9B5 = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "ident") )
    {
        dword_5A7A88->ypabact__id = atoi(scr->p2);

        if ( bact_id < dword_5A7A88->ypabact__id )
        {
            bact_id = dword_5A7A88->ypabact__id;
        }
    }
    else if ( !strcasecmp(scr->p1, "primary") )
    {
        char *v7 = strtok(scr->p2, " \t_\n");
        if ( v7 )
        {
            dword_5A7A88->primTtype = atoi(v7);

            char *v9 = strtok(0, " \t_\n");
            if ( v9 )
            {
                dword_5A7A88->primT.pbact = (__NC_STACK_ypabact *)atoi(v9);

                v9 = strtok(0, " \t_\n");
                if ( v9 )
                {
                    dword_5A7A88->primTpos.sx = strtod(v9, 0);

                    v9 = strtok(0, " \t_\n");
                    if ( v9 )
                    {
                        dword_5A7A88->primTpos.sz = strtod(v9, 0);

                        v9 = strtok(0, " \t_\n");
                        if ( v9 )
                        {
                            dword_5A7A88->primT_cmd_id = atoi(v9);
                        }
                    }
                }
            }
        }
    }
    else if ( !strcasecmp(scr->p1, "speed") )
    {
        if ( strtok(scr->p2, " \t_\n") )
        {
            dword_5A7A88->field_605.sx = strtod(scr->p2, 0);
            if ( strtok(0, " \t_\n") )
            {
                dword_5A7A88->field_605.sy = strtod(scr->p2, 0);
                if ( strtok(0, " \t_\n") )
                {
                    dword_5A7A88->field_605.sz = strtod(scr->p2, 0);
                    if ( strtok(0, " \t_\n") )
                    {
                        dword_5A7A88->field_611 = strtod(scr->p2, 0);
                    }
                }
            }
        }
    }
    else if ( !strcasecmp(scr->p1, "energy") )
    {
        dword_5A7A88->energy = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "matrix") )
    {
        char *v13 = strtok(scr->p2, " \t_\n");

        if ( v13 )
        {
            dword_5A7A88->field_651.m00 = strtod(v13, 0);

            v13 = strtok(0, " \t_\n");
            if ( v13 )
            {
                dword_5A7A88->field_651.m01 = strtod(v13, 0);

                v13 = strtok(0, " \t_\n");
                if ( v13 )
                {
                    dword_5A7A88->field_651.m02 = strtod(v13, 0);

                    v13 = strtok(0, " \t_\n");
                    if ( v13 )
                    {
                        dword_5A7A88->field_651.m10 = strtod(v13, 0);

                        v13 = strtok(0, " \t_\n");
                        if ( v13 )
                        {
                            dword_5A7A88->field_651.m11 = strtod(v13, 0);

                            v13 = strtok(0, " \t_\n");
                            if ( v13 )
                            {
                                dword_5A7A88->field_651.m12 = strtod(v13, 0);

                                v13 = strtok(0, " \t_\n");
                                if ( v13 )
                                {
                                    dword_5A7A88->field_651.m20 = strtod(v13, 0);

                                    v13 = strtok(0, " \t_\n");
                                    if ( v13 )
                                    {
                                        dword_5A7A88->field_651.m21 = strtod(v13, 0);

                                        v13 = strtok(0, " \t_\n");
                                        if ( v13 )
                                        {
                                            dword_5A7A88->field_651.m22 = strtod(v13, 0);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if ( !strcasecmp(scr->p1, "pos") )
    {
        char *v22 = strtok(scr->p2, " \t_\n");
        if ( v22 )
        {
            dword_5A7A88->field_621.sx = strtod(v22, 0);

            v22 = strtok(0, " \t_\n");
            if ( v22 )
            {

                dword_5A7A88->field_621.sy = strtod(v22, 0);

                v22 = strtok(0, " \t_\n");
                if ( v22 )
                {
                    dword_5A7A88->field_621.sz = strtod(v22, 0);
                }
            }
        }
    }
    else if ( !strcasecmp(scr->p1, "force") )
    {
        dword_5A7A88->field_601 = strtod(scr->p2, 0);
    }
    else if ( !strcasecmp(scr->p1, "gunangle") )
    {
        dword_5A7A88->gun_angle2 = strtod(scr->p2, 0);
    }
    else if ( !strcasecmp(scr->p1, "gunbasis") )
    {
        NC_STACK_ypagun *guno = (NC_STACK_ypagun *)current_bact;
        __NC_STACK_ypagun *gun = &guno->stack__ypagun;

        char *v26 = strtok(scr->p2, " \t_\n");
        if ( v26 )
        {
            gun->dir.sx = strtod(v26, 0);

            v26 = strtok(0, " \t_\n");
            if ( v26 )
            {
                gun->dir.sy = strtod(v26, 0);

                v26 = strtok(0, " \t_\n");
                if ( v26 )
                {
                    gun->dir.sz = strtod(v26, 0);
                }
            }
        }
    }
    else if ( !strcasecmp(scr->p1, "waypoint") )
    {
        char *v29 = strtok(scr->p2, " \t_\n");
        if ( v29 )
        {
            int v32 = atoi(v29);

            v29 = strtok(0, " \t_\n");
            if ( v29 )
            {
                dword_5A7A88->field_418[v32].sx = strtod(v29, 0);

                v29 = strtok(0, " \t_\n");
                if ( v29 )
                {
                    dword_5A7A88->field_418[v32].sz = strtod(v29, 0);
                }
            }
        }
    }
    else if ( !strcasecmp(scr->p1, "num_wp") )
    {
        dword_5A7A88->field_59A = atoi(scr->p2);
    }
    else if ( !strcasecmp(scr->p1, "count_wp") )
    {
        dword_5A7A88->field_598 = atoi(scr->p2);
    }
    else
        return 0;

    return 1;
}

int sb_0x479f4c(scrCallBack *scr)
{
    _NC_STACK_ypaworld *yw = NULL;

    if ( scr->dataForStore )
        yw = (_NC_STACK_ypaworld *)scr->dataForStore;

    if ( !scr->field_18 )
    {
        if ( !strcasecmp(scr->p1, "begin_robo") )
        {
            scr->field_18 = 1;

            ypaworld_arg146 v5;
            v5.pos.sx = 600;
            v5.pos.sz = -600.0;
            v5.pos.sy = 0;
            v5.vehicle_id = atoi(scr->p2);

            current_bact = yw->self_full->ypaworld_func146(&v5);

            if ( current_bact )
            {
                current_robo = dynamic_cast<NC_STACK_yparobo *>(current_bact);

                //call_vtbl(v3, 3, 0x80001003, &dword_5A7A88, 0);

                dword_5A7A88 = &current_robo->stack__ypabact;

                yw->self_full->ypaworld_func134(current_bact);
                return 1;
            }

            return 4;
        }
        return 3;
    }
    else
    {
        if ( !strcasecmp(scr->p1, "end") )
        {
            if ( dword_5A7A88->owner == 1 )
            {
                dword_5A7A84 = dynamic_cast<NC_STACK_yparobo *>(current_bact);
                call_vtbl(yw->self_full, 2, 0x80002010, current_bact, 0);
            }

            if ( !dword_5A7A88->reload_const_or_energy2 )
                dword_5A7A88->reload_const_or_energy2 = dword_5A7A88->energy_2;

            scr->field_18 = 0;
            return 2;
        }
        else
        {
            if ( !scr->dataForStore || sub_47A0C0(scr) || sb_0x479f4c__sub0(scr) )
                return 0;
            return 3;
        }

    }
}


int sub_479E30(scrCallBack *scr)
{
    _NC_STACK_ypaworld *yw = NULL;

    if ( scr->dataForStore )
        yw = (_NC_STACK_ypaworld *)scr->dataForStore;

    if ( scr->field_18 )
    {
        if ( !strcasecmp(scr->p1, "end") )
        {
            scr->field_18 = 0;
            return 2;
        }

        if ( !scr->dataForStore || sub_47A0C0(scr) )
            return 0;

        return 3;
    }
    else
    {
        if ( !strcasecmp(scr->p1, "begin_commander") )
        {
            scr->field_18 = 1;

            ypaworld_arg146 v5;
            v5.pos.sx = 600.0;
            v5.pos.sz = -600.0;
            v5.pos.sy = 0;
            v5.vehicle_id = atoi(scr->p2);

            current_bact = yw->self_full->ypaworld_func146(&v5);

            if ( current_bact )
            {
                dword_5A7A88 = &current_bact->stack__ypabact;

                current_commander = current_bact;

                current_robo->ypabact_func72(current_bact);

                return 1;
            }
            else
                return 4;
        }
        else
            return 3;
    }
}

int sub_479D20(scrCallBack *scr)
{
    _NC_STACK_ypaworld *yw = NULL;

    if ( scr->dataForStore )
        yw = (_NC_STACK_ypaworld *)scr->dataForStore;

    if ( scr->field_18 )
    {
        if ( !strcasecmp(scr->p1, "end") )
        {
            scr->field_18 = 0;
            return 2;
        }

        if ( !scr->dataForStore || sub_47A0C0(scr) )
            return 0;

        return 3;
    }
    else
    {
        if ( !strcasecmp(scr->p1, "begin_slave") )
        {
            scr->field_18 = 1;

            ypaworld_arg146 v5;
            v5.pos.sx = 600.0;
            v5.pos.sz = -600.0;
            v5.pos.sy = 0;
            v5.vehicle_id = atoi(scr->p2);

            current_bact = yw->self_full->ypaworld_func146(&v5);

            if ( current_bact )
            {
                dword_5A7A88 = &current_bact->stack__ypabact;

                current_commander->ypabact_func72(current_bact);

                return 1;
            }
            else
                return 4;
        }
        else
            return 3;
    }
}

int sub_479C40(scrCallBack *scr)
{
    _NC_STACK_ypaworld *yw = NULL;

    if ( scr->dataForStore )
        yw = (_NC_STACK_ypaworld *)scr->dataForStore;

    if ( scr->field_18 )
    {
        if ( !strcasecmp(scr->p1, "end") )
        {
            scr->field_18 = 0;
            return 2;
        }

        if ( yw )
        {
            if ( strcasecmp(scr->p1, "disablegem") )
                return 3;

            int v5 = atoi(scr->p2);

            if ( v5 >= 0 && v5 < 8 )
                yw->cells[yw->sectors_maxX2 * yw->gems[v5].sec_y + yw->gems[v5].sec_x].w_type = 0;
        }
        return 0;
    }
    else
    {
        if ( !strcasecmp(scr->p1, "begin_wunderinfo") )
        {
            yw_InitTechUpgradeBuildings(yw->self_full, yw);
            scr->field_18 = 1;
            return 1;
        }
        else
            return 3;
    }
}

int sub_479B98(scrCallBack *scr)
{
    if ( !scr->field_18 )
    {
        if ( !strcasecmp(scr->p1, "begin_extraviewer") )
        {
            scr->field_18 = 1;
            return 1;
        }

        return 3;
    }
    else
    {
        if ( !strcasecmp(scr->p1, "end") )
        {
            scr->field_18 = 0;
            return 2;
        }

        if ( !strcasecmp(scr->p1, "kind") )
        {
            if ( !strcasecmp(scr->p2, "robogun") )
            {
                dword_5A7A8C = 1;
                return 0;
            }
        }
        else
        {
            if ( strcasecmp(scr->p1, "number") )
                return 3;
            dword_5A7A78 = atoi(scr->p2);
        }
        return 0;
    }
}

int sub_479AB0(scrCallBack *scr)
{
    _NC_STACK_ypaworld *yw = NULL;

    if ( scr->dataForStore )
        yw = (_NC_STACK_ypaworld *)scr->dataForStore;

    if ( !scr->field_18 )
    {
        if ( !strcasecmp(scr->p1, "begin_kwfactor") )
        {
            scr->field_18 = 1;
            return 1;
        }
        return 3;
    }
    else
    {
        if ( !strcasecmp(scr->p1, "end") )
        {
            scr->field_18 = 0;
            return 2;
        }
        else
        {
            if ( !strcasecmp(scr->p1, "kw") )
            {
                char *v4 = strtok(scr->p2, "_ \n");
                if ( v4 )
                {
                    int v5 = atoi(v4);

                    v4 = strtok(0, "_ \n");
                    if ( v4 )
                    {
                        int v7 = atoi(v4);

                        v4 = strtok(0, "_ \n");
                        if ( v4 )
                        {
                            int v9 = atoi(v4);

                            for (int i = 0; i < 256; i++)
                            {
                                yw_field34 *v11 = &yw->field_34[i];

                                if ( v11->p_cell )
                                {
                                    if ( v5 == v11->x && v7 == v11->y )
                                        v11->power_2 = v9;
                                }
                            }
                        }
                    }
                }
                return 0;
            }
            else
                return 3;
        }
    }
}

int sub_479A30(scrCallBack *scr)
{
    _NC_STACK_ypaworld *yw = NULL;

    if ( scr->dataForStore )
        yw = (_NC_STACK_ypaworld *)scr->dataForStore;

    if ( !scr->field_18 )
    {
        if ( !strcasecmp(scr->p1, "begin_globals") )
        {
            scr->field_18 = 1;
            return 1;
        }
        return 3;
    }
    else
    {

        if ( !strcasecmp(scr->p1, "end") )
        {
            scr->field_18 = 0;
            return 2;
        }
        else
        {
            if ( !strcasecmp(scr->p1, "time") )
            {
                yw->field_1614 = atoi(scr->p2);
                return 0;
            }
            else
                return 3;
        }
    }
}

int sub_47965C(scrCallBack *scr)
{
    _NC_STACK_ypaworld *yw;

    if ( scr->dataForStore )
        yw = (_NC_STACK_ypaworld *)scr->dataForStore;

    if ( scr->field_18 )
    {
        if ( !strcasecmp(scr->p1, "end") )
        {
            scr->field_18 = 0;
            return 2;
        }

        return 3;
    }
    else
    {
        if ( !strcasecmp(scr->p1, "begin_ownermap") )
        {
            scr->field_18 = 1;

            for (int i = 0; i < 8; i++)
                yw->sectors_count_by_owner[i] = 0;

            if ( yw->own_map )
                delete_class_obj(yw->own_map);

            yw->own_map = readMapDumpAsILBM(yw, "ownmap", scr->file);

            if ( yw->own_map )
            {
                bitmap_intern *bitm;
                call_vtbl(yw->own_map, 3, 0x80002000, &bitm, 0);

                int smax = yw->sectors_maxY2 * yw->sectors_maxX2;
                uint8_t *pbuf = (uint8_t *)bitm->buffer;
                cellArea *v6 = yw->cells;

                for (int i = 0; i < smax; i++)
                {
                    int v8 = *pbuf;
                    v6->owner = v8;
                    yw->sectors_count_by_owner[v8]++;

                    v6++;
                    pbuf++;
                }
                return 1;
            }
            else
            {
                return 4;
            }
        }
        else
            return 3;
    }
}

int sub_479770(scrCallBack *scr)
{
    _NC_STACK_ypaworld *yw;

    if ( scr->dataForStore )
        yw = (_NC_STACK_ypaworld *)scr->dataForStore;

    if ( scr->field_18 )
    {
        if ( !strcasecmp(scr->p1, "end") )
        {
            scr->field_18 = 0;
            return 2;
        }
        return 3;
    }
    else
    {
        if ( !strcasecmp(scr->p1, "begin_buildingmap") )
        {
            scr->field_18 = 1;

            if ( yw->blg_map )
                delete_class_obj(yw->blg_map);

            yw->blg_map = readMapDumpAsILBM(yw, "blgmap", scr->file);
            if ( yw->blg_map )
            {
                bitmap_intern *bitm;
                call_vtbl(yw->blg_map, 3, 0x80002000, &bitm, 0);

                int smax = yw->sectors_maxY2 * yw->sectors_maxX2;
                uint8_t *pbuf = (uint8_t *)bitm->buffer;
                cellArea *v6 = yw->cells;

                for (int i = 0; i < smax; i++)
                {
                    int v8 = *pbuf;

                    if ( v8 )
                    {
                        if ( v6->owner )
                        {
                            ypaworld_arg148 arg148;
                            arg148.field_18 = 1;
                            arg148.ownerID = v6->owner;
                            arg148.ownerID2 = v6->owner;
                            arg148.blg_ID = v8;
                            arg148.field_C = 1;
                            arg148.x = i % yw->sectors_maxX2;
                            arg148.y = i / yw->sectors_maxX2;

                            yw->self_full->ypaworld_func148(&arg148);
                        }
                    }

                    v6++;
                    pbuf++;
                }

                return 1;
            }
            else
            {
                return 4;
            }
        }
        else
            return 3;
    }
}

int sub_4798D0(scrCallBack *scr)
{
    _NC_STACK_ypaworld *yw;

    if ( scr->dataForStore )
        yw = (_NC_STACK_ypaworld *)scr->dataForStore;

    if ( scr->field_18 )
    {
        if ( !strcasecmp(scr->p1, "end") )
        {
            scr->field_18 = 0;
            return 2;
        }
        return 3;
    }
    else
    {
        if ( !strcasecmp(scr->p1, "begin_energymap") )
        {
            scr->field_18 = 1;

            NC_STACK_ilbm *v4 = readMapDumpAsILBM(yw, "nrgmap", scr->file);

            if ( v4 )
            {
                bitmap_intern *bitm;
                call_vtbl(v4, 3, 0x80002000, &bitm, 0);

                int smax = yw->sectors_maxY2 * yw->sectors_maxX2;
                cellArea *v6 = yw->cells;
                uint8_t *pbuf = (uint8_t *)bitm->buffer;

                for (int i = 0; i < smax; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        for (int k = 0; k < 3; k++)
                        {
                            v6->buildings_health[j][k] = *pbuf;
                            pbuf++;
                        }
                    }

                    v6++;

                }

                delete_class_obj(v4);
                return 1;
            }
            else
            {
                ypa_log_out("Error while reading energy map!\n");
                return 4;
            }
        }
        else
            return 3;
    }
}


int sub_47925C(scrCallBack *scr)
{
    if ( scr->field_18 )
    {
        if ( !strcasecmp(scr->p1, "end") )
        {
            scr->field_18 = 0;
            return 2;
        }

        if ( !strcasecmp(scr->p1, "levelnum") )
        {
            *(int *)scr->dataForStore = atoi(scr->p2);
            return 0;
        }
    }
    else if ( !strcasecmp(scr->p1, "begin_levelnum") )
    {
        scr->field_18 = 1;
        return 1;
    }

    return 3;
}



int sb_0x47f2d8__sub0(yw_f726c *hist_list, FILE *fil)
{
    char v18[1024];
    fgets(v18, 1024, fil);

    char *v2 = strtok(v18, " \n");
    int v3 = strtol(v2, 0, 0);

    v2 = strtok(0, " \n");
    int v7 = strtol(v2, 0, 0);

    if ( sub_47EDDC(hist_list, v7 * v3) )
    {
        uint8_t *cur = hist_list->last_bufStart;
        for (int i = 0; i < v7; i++)
        {
            fgets(v18, 1024, fil);

            char *ln = v18;
            for (int j = 0; j < v3; j++)
            {
                char *vl = strtok(ln, " \n");
                if (ln)
                    ln = NULL;

                *cur = strtol(vl, 0, 16);
                cur++;
            }
        }

        cur = hist_list->last_bufStart;
        while(*cur)
        {
            int wdth = 0;

            switch (*cur)
            {
            case 1:
                wdth = 5;
                break;

            case 2:
            case 6:
                wdth = 4;
                break;

            case 7:
                wdth = 12;
                break;

            case 3:
            case 4:
                wdth = 6;
                break;

            case 5:
                wdth = 0;
                *cur = 0;
                break;
            }

            cur += wdth;
        }

        hist_list->field_1C = NULL;
        hist_list->last_bufStart = cur;

        return 1;
    }
    return 0;
}

int sb_0x47f2d8(scrCallBack *scr)
{
    _NC_STACK_ypaworld *yw = (_NC_STACK_ypaworld *)scr->dataForStore;

    if ( !scr->field_18 )
    {
        if ( !strcasecmp(scr->p1, "begin_history") )
        {
            scr->field_18 = 1;
            return 1;
        }
        return 3;
    }
    else
    {
        if ( !strcasecmp(scr->p1, "end") )
        {
            scr->field_18 = 0;
            return 2;
        }
        else
        {
            if ( !strcasecmp(scr->p1, "history_buffer") )
            {
                if ( yw->history )
                    sb_0x47f2d8__sub0(yw->history, scr->file);
                return 0;
            }
            else
                return 3;
        }
    }
}

int sub_4795B0(scrCallBack *scr)
{
    _NC_STACK_ypaworld *yw = (_NC_STACK_ypaworld *)scr->dataForStore;

    if ( scr->field_18 )
    {
        if ( !strcasecmp(scr->p1, "end") )
        {
            scr->field_18 = 0;
            return 2;
        }

        if ( !strcasecmp(scr->p1, "ownermask") )
        {
            yw->field_2d90->ownerMap__has_vehicles = atoi(scr->p2);
            return 0;
        }

        if ( !strcasecmp(scr->p1, "usermask") )
        {
            yw->field_2d90->field_60 = atoi(scr->p2);
            return 0;
        }
    }
    else if ( !strcasecmp(scr->p1, "begin_masks") )
    {
        scr->field_18 = 1;
        return 1;
    }

    return 3;
}


int sub_4792D0(scrCallBack *scr)
{
    _NC_STACK_ypaworld *yw = (_NC_STACK_ypaworld *)scr->dataForStore;

    if ( !scr->field_18 )
    {
        if ( !strcasecmp(scr->p1, "begin_superbomb") )
        {
            scr->field_18 = 1;
            return 1;
        }
        return 3;
    }
    else
    {
        if ( !strcasecmp(scr->p1, "end") )
        {
            scr->field_18 = 0;
            return 2;
        }
        else if ( !strcasecmp(scr->p1, "status") )
        {
            int v4 = atoi(scr->p2);

            if ( v4 == 1 )
                sub_4D12D8(yw, dword_5A7A70, 0);
            else if ( v4 == 2 )
                sub_4D1594(yw, dword_5A7A70);
            else if ( v4 == 3 )
                sub_4D1444(yw, dword_5A7A70);

        }
        else if ( !strcasecmp(scr->p1, "num") )
        {
            dword_5A7A70 = atoi(scr->p2);
        }
        else if ( !strcasecmp(scr->p1, "activated_by") )
        {
            yw->field_2d90->supetItems[dword_5A7A70].field_F4 = atoi(scr->p2);
        }
        else if ( !strcasecmp(scr->p1, "active_timestamp") )
        {
            yw->field_2d90->supetItems[dword_5A7A70].field_EC = atoi(scr->p2);
        }
        else if ( !strcasecmp(scr->p1, "trigger_timestamp") )
        {
            yw->field_2d90->supetItems[dword_5A7A70].field_F0 = atoi(scr->p2);
        }
        else if ( !strcasecmp(scr->p1, "countdown") )
        {
            yw->field_2d90->supetItems[dword_5A7A70].field_F8 = atoi(scr->p2);
        }
        else if ( !strcasecmp(scr->p1, "last_ten_sec") )
        {
            yw->field_2d90->supetItems[dword_5A7A70].field_FC = atoi(scr->p2);
        }
        else if ( !strcasecmp(scr->p1, "last_sec") )
        {
            yw->field_2d90->supetItems[dword_5A7A70].field_100 = atoi(scr->p2);
        }
        else if ( !strcasecmp(scr->p1, "radius") )
        {
            yw->field_2d90->supetItems[dword_5A7A70].field_104 = atoi(scr->p2);
        }
        else if ( !strcasecmp(scr->p1, "last_radius") )
        {
            yw->field_2d90->supetItems[dword_5A7A70].field_108 = atoi(scr->p2);
        }
        else
            return 3;

        return 0;
    }
}
