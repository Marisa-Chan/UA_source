#include <inttypes.h>
#include <string.h>
#include "includes.h"
#include "def_parser.h"
#include "yw_internal.h"
#include "yw.h"



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

    if ( sndfx->extCount < 8 )
    {
        int curid = sndfx->extCount;
        sndfx->extCount++;

        sndExt *sndEx = &sndfx->sndExts[curid];
        sndEx->field_0 = 0;
        sndEx->field_4 = atoi(pp1);
        sndEx->field_6 = atoi(pp2);
        sndEx->field_8 = atoi(pp3);
        sndEx->field_C = atoi(pp4);
        sndEx->field_10 = atoi(pp5);

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
        sndfx->volume = atoi(p2);
    else if ( !strcasecmp(tp, "pitch") )
        sndfx->pitch = atoi(p2);
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
        sndfx->slot = atoi(p2);
    else if ( !strcasecmp(tp, "mag0") )
        sndfx->mag0 = strtof(p2, 0);
    else if ( !strcasecmp(tp, "mag1") )
        sndfx->mag1 = strtof(p2, 0);
    else if ( !strcasecmp(tp, "time") )
        sndfx->time = atoi(p2);
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
        sndfx->shk_slot = atoi(p2);
    else if ( !strcasecmp(tp, "mag0") )
        sndfx->shk_mag0 = strtof(p2, 0);
    else if ( !strcasecmp(tp, "mag1") )
        sndfx->shk_mag1 = strtof(p2, 0);
    else if ( !strcasecmp(tp, "time") )
        sndfx->shk_time = atoi(p2);
    else if ( !strcasecmp(tp, "mute") )
        sndfx->mute = strtof(p2, 0);
    else if ( !strcasecmp(tp, "x") )
        sndfx->x = strtof(p2, 0);
    else if ( !strcasecmp(tp, "y") )
        sndfx->y = strtof(p2, 0);
    else if ( !strcasecmp(tp, "z") )
        sndfx->z = strtof(p2, 0);
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
            int vhcl_id = atoi(_p2);
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
                v9->mag0 = 1.0;
                v9->shk_mag0 = 1.0;
                v9->mute = 0.02;
                v9->x = 0.2;
                v9->y = 0.2;
                v9->z = 0.2;
                v9->volume = 120;
                v9->time = 1000;
                v9->shk_time = 1000;
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
            int vhcl_id = atoi(_p2);
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
            vhcl->disable_enable_bitmask |= 1 << atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "disable") )
        {
            vhcl->disable_enable_bitmask &= ~(1 << atoi(_p2));
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
            vhcl->energy = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "shield") )
        {
            vhcl->shield = atoi(_p2);
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
            vhcl->radar = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "add_energy") )
        {
            vhcl->energy += atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "add_shield") )
        {
            vhcl->shield += atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "add_radar") )
        {
            vhcl->radar += atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "vp_normal") )
        {
            vhcl->vp_normal = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "vp_fire") )
        {
            vhcl->vp_fire = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "vp_megadeth") )
        {
            vhcl->vp_megadeth = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "vp_wait") )
        {
            vhcl->vp_wait = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "vp_dead") )
        {
            vhcl->vp_dead = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "vp_genesis") )
        {
            vhcl->vp_genesis = atoi(_p2);
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
                dfx->p1 = atoi(pp1);
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
            vhcl->weapon = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "mgun") )
        {
            vhcl->mgun = atoi(_p2);
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
            vhcl->num_weapons = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "kill_after_shot") )
        {
            vhcl->kill_after_shot = atoi(_p2);
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
                val->value = atoi(_p2);
            }
            else
            {
                vhcl->stack_pointer__position->id = 0x80002000;
                vhcl->stack_pointer__position->value = atoi(_p2);
                vhcl->stack_pointer__position++;
                vhcl->stack_pointer__position->id = 0;
            }
        }
        else if ( !strcasecmp(_p1, "gun_up_angle") )
        {
            stack_vals *val = find_id_in_stack2(0x80002001, vhcl->stak);
            if ( val )
            {
                val->value = atoi(_p2);
            }
            else
            {
                vhcl->stack_pointer__position->id = 0x80002001;
                vhcl->stack_pointer__position->value = atoi(_p2);
                vhcl->stack_pointer__position++;
                vhcl->stack_pointer__position->id = 0;
            }
        }
        else if ( !strcasecmp(_p1, "gun_down_angle") )
        {
            stack_vals *val = find_id_in_stack2(0x80002002, vhcl->stak);
            if ( val )
            {
                val->value = atoi(_p2);
            }
            else
            {
                vhcl->stack_pointer__position->id = 0x80002002;
                vhcl->stack_pointer__position->value = atoi(_p2);
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
                val->value = atoi(_p2);
            }
            else
            {
                vhcl->stack_pointer__position->id = 0x80003001;
                vhcl->stack_pointer__position->value = atoi(_p2);
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
            vhcl->vo_type = atoi(_p2);
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
                vhcl->scale_fx_p3 = atoi(pp3);
                int16_t * tmp = &vhcl->scale_fx_pXX;
                while ( 1 )
                {
                    char *v65 = strtok(0, "_");
                    if ( !v65 )
                        break;

                    *tmp = atoi(v65);
                    tmp++;
                }
            }
        }
        else if ( !strcasecmp(_p1, "robo_data_slot") )
        {
        }
        else if ( !strcasecmp(_p1, "robo_num_guns") )
        {
            robo->robo_num_guns = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "robo_act_gun") )
        {
            v76 = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "robo_gun_pos_x") )
        {
            robo->guns[v76].robo_gun_pos_x = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_gun_pos_y") )
        {
            robo->guns[v76].robo_gun_pos_y = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_gun_pos_z") )
        {
            robo->guns[v76].robo_gun_pos_z = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_gun_dir_x") )
        {
            robo->guns[v76].robo_gun_dir_x = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_gun_dir_y") )
        {
            robo->guns[v76].robo_gun_dir_y = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_gun_dir_z") )
        {
            robo->guns[v76].robo_gun_dir_z = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_gun_type") )
        {
            robo->guns[v76].robo_gun_type = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "robo_gun_name") )
        {
            strcpy(robo->guns[v76].robo_gun_name, _p2);
        }
        else if ( !strcasecmp(_p1, "robo_dock_x") )
        {
            robo->robo_dock_x = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_dock_y") )
        {
            robo->robo_dock_y = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_dock_z") )
        {
            robo->robo_dock_z = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_coll_num") )
        {
            robo->robo_coll_num = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "robo_coll_radius") )
        {
            robo->roboColls[v77].robo_coll_radius = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_coll_act") )
        {
            v77 = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "robo_coll_x") )
        {
            robo->roboColls[v77].robo_coll_x = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_coll_y") )
        {
            robo->roboColls[v77].robo_coll_y = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_coll_z") )
        {
            robo->roboColls[v77].robo_coll_z = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_viewer_x") )
        {
            robo->robo_viewer_x = strtof(_p2, 0);
        }
        else if ( !strcasecmp(_p1, "robo_viewer_y") )
        {
            robo->robo_viewer_y = strtof(_p2, 0);

        }
        else if ( !strcasecmp(_p1, "robo_viewer_z") )
        {
            robo->robo_viewer_z = strtof(_p2, 0);
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
        sndfx->volume = atoi(p2);
    else if ( !strcasecmp(tp, "pitch") )
        sndfx->pitch = atoi(p2);
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
        sndfx->slot = atoi(p2);
    else if ( !strcasecmp(tp, "mag0") )
        sndfx->mag0 = strtof(p2, 0);
    else if ( !strcasecmp(tp, "mag1") )
        sndfx->mag1 = strtof(p2, 0);
    else if ( !strcasecmp(tp, "time") )
        sndfx->time = atoi(p2);
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
        sndfx->shk_slot = atoi(p2);
    else if ( !strcasecmp(tp, "mag0") )
        sndfx->shk_mag0 = strtof(p2, 0);
    else if ( !strcasecmp(tp, "mag1") )
        sndfx->shk_mag1 = strtof(p2, 0);
    else if ( !strcasecmp(tp, "time") )
        sndfx->shk_time = atoi(p2);
    else if ( !strcasecmp(tp, "mute") )
        sndfx->mute = strtof(p2, 0);
    else if ( !strcasecmp(tp, "x") )
        sndfx->x = strtof(p2, 0);
    else if ( !strcasecmp(tp, "y") )
        sndfx->y = strtof(p2, 0);
    else if ( !strcasecmp(tp, "z") )
        sndfx->z = strtof(p2, 0);
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
            int wpn_id = atoi(_p2);
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
                v9->mag0 = 1.0;
                v9->shk_mag0 = 1.0;
                v9->mute = 0.02;
                v9->x = 0.2;
                v9->y = 0.2;
                v9->z = 0.2;
                v9->volume = 120;
                v9->time = 1000;
                v9->shk_time = 1000;
            }

            wpn->pointer = wpn->stack;
            wpn->stack[0].id = 0;
        }
        else if ( !strcasecmp(_p1, "modify_weapon") )
        {
            int wpn_id = atoi(_p2);

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
            wpn->enable_mask |= 1 << atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "disable") )
        {
            wpn->enable_mask &= ~(1 << atoi(_p2));
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
            wpn->energy = atoi(_p2);
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
            wpn->life_time = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "life_time_nt") )
        {
            wpn->life_time_nt = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "drive_time") )
        {
            wpn->drive_time = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "delay_time") )
        {
            wpn->delay_time = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "shot_time") )
        {
            wpn->shot_time = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "shot_time_user") )
        {
            wpn->shot_time_user = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "salve_shots") )
        {
            wpn->salve_shots = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "salve_delay") )
        {
            wpn->salve_delay = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "add_energy") )
        {
            wpn->energy += atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "add_energy_heli") )
        {
            wpn->energy_heli += atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "add_energy_tank") )
        {
            wpn->energy_tank += atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "add_energy_flyer") )
        {
            wpn->energy_flyer += atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "add_energy_Robo") )
        {
            wpn->energy_robo += atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "add_shot_time") )
        {
            wpn->shot_time += atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "add_shot_time_user") )
        {
            wpn->shot_time_user += atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "vp_normal") )
        {
            wpn->vp_normal = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "vp_fire") )
        {
            wpn->vp_fire = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "vp_megadeth") )
        {
            wpn->vp_megadeth = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "vp_wait") )
        {
            wpn->vp_wait = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "vp_dead") )
        {
            wpn->vp_dead = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "vp_genesis") )
        {
            wpn->vp_genesis = atoi(_p2);
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
                dfx->p1 = atoi(pp1);
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
            int bld_id = atoi(_p2);
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
            int bld_id = atoi(_p2);
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
            bld->enable_mask |= 1 << atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "disable") )
        {
            bld->enable_mask &= ~(1 << atoi(_p2));
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
            bld->power = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "energy") )
        {
            bld->energy = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "sec_type") )
        {
            bld->sec_type = atoi(_p2);
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
            bld->sndfx.volume = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "snd_normal_pitch") )
        {
            bld->sndfx.pitch = atoi(_p2);
        }
        else if ( !strcasecmp(_p1, "sbact_act") )
        {
            v20 = atoi(_p2);
        }
        else
        {
            buildSbact *v19 = &bld->sbacts[v20];
            if ( !strcasecmp(_p1, "sbact_vehicle") )
            {
                v19->sbact_vehicle = atoi(_p2);
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
        lvlnet->background_map[lvlnet->bg_n].size_x = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "size_y") )
    {
        lvlnet->background_map[lvlnet->bg_n].size_y = atoi(_p2);
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
            yw->shell_default_res = atoi(pp2) | (atoi(pp1) << 12);
        }
    }
    else if ( !strcasecmp(_p1, "game_default_res") )
    {
        char *pp1 = strtok(_p2, "_");
        char *pp2 = strtok(0, " \t");

        if ( pp1 && pp2 )
        {
            yw->game_default_res = atoi(pp2) | (atoi(pp1) << 12);
        }
    }
    else if ( !strcasecmp(_p1, "max_impulse") )
    {
        yw->max_impulse = strtof(_p2, 0);
    }
    else if ( !strcasecmp(_p1, "unit_limit") )
    {
        yw->unit_limit = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "unit_limit_type") )
    {
        yw->unit_limit_type = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "unit_limit_arg") )
    {
        yw->unit_limit_arg = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "beam_energy_start") )
    {
        yw->beam_energy_start = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "beam_energy_add") )
    {
        yw->beam_energy_add = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "easy_cheat_keys") )
    {
        yw->easy_cheat_keys = atoi(_p2);
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
        yw->superbomb_center_vproto = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "superbomb_wall_vproto") )
    {
        yw->superbomb_wall_vproto = atoi(_p2);
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
    int xsz = atoi(tmp);

    tmp = strtok(0, " \n");
    int ysz = atoi(tmp);

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
        mapp->setNumber = atoi(_p2);
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

        yw->field_2d90->amb_track_p0 = atoi(tmp);

        tmp = strtok(0, " \t_\n");
        if ( tmp )
        {
            yw->field_2d90->amb_track_p1 = atoi(tmp);

            tmp = strtok(0, " \t_\n");
            if ( tmp )
            {
                yw->field_2d90->amb_track_p2 = atoi(tmp);
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
        mapp->event_loop = atoi(_p2);
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
        yw->unit_limit_1 = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "unit_limit_type") )
    {
        yw->unit_limit_type_1 = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "unit_limit_arg") )
    {
        yw->unit_limit_arg_1 = atoi(_p2);
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
        robo->owner = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "vehicle") )
    {
        robo->vehicle = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "sec_x") )
    {
        int tmp = atoi(_p2);
        robo->pos_y = -300;
        robo->pos_x = tmp * 1200.0 + 600.0;
    }
    else if ( !strcasecmp(_p1, "sec_y") )
    {
        int tmp = atoi(_p2);
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
        robo->energy = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "con_budget") )
    {
        robo->con_budget = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "rad_budget") )
    {
        robo->rad_budget = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "pow_budget") )
    {
        robo->pow_budget = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "def_budget") )
    {
        robo->def_budget = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "saf_budget") )
    {
        robo->saf_budget = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "rec_budget") )
    {
        robo->rec_budget = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "cpl_budget") )
    {
        robo->cpl_budget = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "rob_budget") )
    {
        robo->rob_budget = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "viewangle") )
    {
        robo->viewangle = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "saf_delay") )
    {
        robo->saf_delay = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "pow_delay") )
    {
        robo->pow_delay = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "rad_delay") )
    {
        robo->rad_delay = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "cpl_delay") )
    {
        robo->cpl_delay = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "def_delay") )
    {
        robo->def_delay = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "con_delay") )
    {
        robo->con_delay = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "rec_delay") )
    {
        robo->rec_delay = atoi(_p2);
    }
    else if ( !strcasecmp(_p1, "rob_delay") )
    {
        robo->rob_delay = atoi(_p2);
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
        robo->reload_const = atoi(_p2);
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
                    strcpy(yw->GameShell->callSIGN , get_lang_string(yw->string_pointers_p2, 366, "UNNAMED") );
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
        else if ( strcasecmp(arg->p1, "netname") )
        {
        }
        else if ( !strcasecmp(arg->p1, "maxroboenergy") )
        {
            yw->maxroboenergy = atoi(arg->p2);
        }
        else if ( !strcasecmp(arg->p1, "maxreloadconst") )
        {
            yw->maxreloadconst = atoi(arg->p2);
        }
        else if ( !strcasecmp(arg->p1, "numbuddies") )
        {
        }
        else if ( !strcasecmp(arg->p1, "beamenergy") )
        {
            yw->beamenergy = atoi(arg->p2);
        }
        else if ( !strcasecmp(arg->p1, "playerstatus") )
        {
            char *v10 = strtok(arg->p2, "_ \t");
            if ( v10 )
            {
                int plid = atoi(v10);
                v10 = strtok(0, "_ \t");
                if ( v10 )
                {
                    yw->playerstatus[plid].p1 = atoi(v10);
                    v10 = strtok(0, "_ \t");
                    if ( v10 )
                    {
                        yw->playerstatus[plid].p2 = atoi(v10);
                        v10 = strtok(0, "_ \t");
                        if ( v10 )
                        {
                            yw->playerstatus[plid].p3 = atoi(v10);
                            v10 = strtok(0, "_ \t");
                            if ( v10 )
                            {
                                yw->playerstatus[plid].p4 = atoi(v10);
                                v10 = strtok(0, "_ \t");
                                if ( v10 )
                                {
                                    yw->playerstatus[plid].p5 = atoi(v10);
                                    v10 = strtok(0, "_ \t");
                                    if ( v10 )
                                    {
                                        yw->playerstatus[plid].p6 = atoi(v10);
                                        v10 = strtok(0, "_ \t");
                                        if ( v10 )
                                        {
                                            yw->playerstatus[plid].p7 = atoi(v10);
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
                yw->field_2d90->jodiefoster[0] = atoi(v20);
                v20 = strtok(0, "_ \t");
                if ( v20 )
                {
                    yw->field_2d90->jodiefoster[1] = atoi(v20);
                    v20 = strtok(0, "_ \t");
                    if ( v20 )
                    {
                        yw->field_2d90->jodiefoster[2] = atoi(v20);
                        v20 = strtok(0, "_ \t");
                        if ( v20 )
                        {
                            yw->field_2d90->jodiefoster[3] = atoi(v20);
                            v20 = strtok(0, "_ \t");
                            if ( v20 )
                            {
                                yw->field_2d90->jodiefoster[4] = atoi(v20);
                                v20 = strtok(0, "_ \t");
                                if ( v20 )
                                {
                                    yw->field_2d90->jodiefoster[5] = atoi(v20);
                                    v20 = strtok(0, "_ \t");
                                    if ( v20 )
                                    {
                                        yw->field_2d90->jodiefoster[6] = atoi(v20);
                                        v20 = strtok(0, "_ \t");
                                        if ( v20 )
                                            yw->field_2d90->jodiefoster[7] = atoi(v20);
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

    call_vtbl(v3[v4], 3, 0x80002000, &v13->samples_data[v4], 0);

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
                usr->shelltrack = atoi(v8);
                v8 = strtok(0, " \t_\n");
                if ( v8 )
                {
                    usr->shelltrack__adv.field_0 = atoi(v8);
                    v8 = strtok(0, " \t_\n");
                    if ( v8 )
                        usr->shelltrack__adv.field_4 = atoi(v8);
                }
            }
            else if ( !strcasecmp(arg->p1, "missiontrack") )
            {
                usr->missiontrack__adv.field_0 = 0;
                usr->missiontrack__adv.field_4 = 0;
                char *v8 = strtok(buf, " \t_\n");
                usr->missiontrack = atoi(v8);
                v8 = strtok(0, " \t_\n");
                if ( v8 )
                {
                    usr->missiontrack__adv.field_0 = atoi(v8);
                    v8 = strtok(0, " \t_\n");
                    if ( v8 )
                        usr->missiontrack__adv.field_4 = atoi(v8);
                }
            }
            else if ( !strcasecmp(arg->p1, "debriefingtrack") )
            {
                usr->debriefingtrack__adv.field_0 = 0;
                usr->debriefingtrack__adv.field_4 = 0;
                char *v8 = strtok(buf, " \t_\n");
                usr->debriefingtrack = atoi(v8);
                v8 = strtok(0, " \t_\n");
                if ( v8 )
                {
                    usr->debriefingtrack__adv.field_0 = atoi(v8);
                    v8 = strtok(0, " \t_\n");
                    if ( v8 )
                        usr->debriefingtrack__adv.field_4 = atoi(v8);
                }
            }
            else if ( !strcasecmp(arg->p1, "loadingtrack") )
            {
                usr->loadingtrack__adv.field_0 = 0;
                usr->loadingtrack__adv.field_4 = 0;
                char *v8 = strtok(buf, " \t_\n");
                usr->loadingtrack = atoi(v8);
                v8 = strtok(0, " \t_\n");
                if ( v8 )
                {
                    usr->loadingtrack__adv.field_0 = atoi(v8);
                    v8 = strtok(0, " \t_\n");
                    if ( v8 )
                        usr->loadingtrack__adv.field_4 = atoi(v8);
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
