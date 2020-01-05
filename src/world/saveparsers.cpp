#include "saveparsers.h"
#include "../fmtlib/core.h"
#include "../fmtlib/printf.h"
#include "../yw.h"
#include "../yparobo.h"
#include "../log.h"


extern int dword_5A7A80;
extern uint32_t bact_id;

namespace World
{
namespace Parsers
{
NC_STACK_yparobo *SaveBact::_lastRobo = NULL;
NC_STACK_ypabact *SaveBact::_lastCommander = NULL;


bool SaveRoboParser::RoboParser(const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "owner") )
    {
        _r->ypabact.owner = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "is_user_robo") )
    {
        if ( !StriCmp(p2, "yes") )
            _o.setYW_userHostStation(_r);
    }
    else if ( !StriCmp(p1, "robostate") )
    {
        _r->stack__yparobo.roboState = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "dockenergy") )
    {
        _r->stack__yparobo.dock_energ = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "dockcount") )
    {
        _r->stack__yparobo.dock_cnt = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "dockuser") )
    {
        _r->stack__yparobo.dock_user = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "docktime") )
    {
        _r->stack__yparobo.dock_time = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "docktargettype") )
    {
        _r->stack__yparobo.dock_tgType = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "dockaggr") )
    {
        _r->stack__yparobo.dock_aggr = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "docktargetpos") )
    {
        Stok stok(p2, " \t_\n");
        std::string tmp;
        if ( stok.GetNext(&tmp) )
        {
            _r->stack__yparobo.dock_tgt_pos.x = std::stod(tmp, 0);
            if ( stok.GetNext(&tmp) )
                _r->stack__yparobo.dock_tgt_pos.z = std::stod(tmp, 0);
        }
    }
    else if ( !StriCmp(p1, "docktargetID") )
    {
        _r->stack__yparobo.dock_tgt_comm_id = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "fillmodus") )
    {
        _r->stack__yparobo.field_501 = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "battbuilding") )
    {
    }
    else if ( !StriCmp(p1, "battvehicle") )
    {
        _r->stack__yparobo.field_4F5 = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "buildspare") )
    {
        _r->stack__yparobo.field_509 = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "battbeam") )
    {
        _r->stack__yparobo.field_4FD = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "vhoriz") )
    {
        _r->ypabact.viewer_horiz_angle = std::stod(p2, 0);
    }
    else if ( !StriCmp(p1, "vvert") )
    {
        _r->ypabact.viewer_vert_angle = std::stod(p2, 0);
    }
    else if ( !StriCmp(p1, "maximum") )
    {
        _r->ypabact.energy_max = std::stoi(p2);

        if ( _o.ypaworld.UserRobo == _r )
        {
            if ( _r->ypabact.energy_max < _o.ypaworld.maxroboenergy )
            {
                _r->ypabact.energy_max = _o.ypaworld.maxroboenergy;
                _r->ypabact.reload_const = _o.ypaworld.maxreloadconst;
            }
        }
    }
    else if ( !StriCmp(p1, "con_budget") )
    {
        _r->stack__yparobo.epConquer = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "def_budget") )
    {
        _r->stack__yparobo.epDefense = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "rec_budget") )
    {
        _r->stack__yparobo.epRecon = std::stoi(p2);
    }
    else  if ( !StriCmp(p1, "rob_budget") )
    {
        _r->stack__yparobo.epRobo = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "rad_budget") )
    {
        _r->stack__yparobo.epRadar = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "pow_budget") )
    {
        _r->stack__yparobo.epPower = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "saf_budget") )
    {
        _r->stack__yparobo.epSafety = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "cpl_budget") )
    {
        _r->stack__yparobo.epChangePlace = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "saf_delay") )
    {
        _r->stack__yparobo.safety_delay = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "pow_delay") )
    {
        _r->stack__yparobo.field_265 = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "rad_delay") )
    {
        _r->stack__yparobo.radar_delay = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "cpl_delay") )
    {
        _r->stack__yparobo.field_2B1 = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "def_delay") )
    {
        _r->stack__yparobo.field_281 = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "con_delay") )
    {
        _r->stack__yparobo.conq_delay = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "rec_delay") )
    {
        _r->stack__yparobo.field_2C9 = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "rob_delay") )
    {
        _r->stack__yparobo.field_2E1 = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "reload_const") )
    {
        if ( !_r->ypabact.reload_const )
            _r->ypabact.reload_const = std::stoi(p2);
    }
    else
        return false;

    return true;
}


bool SaveBact::SaveBactParser(NC_STACK_ypabact *b, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "viewer") )
    {
        if ( StrGetBool(p2) )
            b->setBACT_viewer(true);
    }
    else if ( !StriCmp(p1, "user") )
    {
        if ( StrGetBool(p2) )
            b->setBACT_inputting(true);
    }
    else if ( !StriCmp(p1, "collision") )
    {
        if ( StrGetBool(p2) )
            b->setBACT_bactCollisions(true);
    }
    else if ( !StriCmp(p1, "commandid") )
    {
        b->ypabact.commandID = (std::stoi(p2) & 0xFFFFFF);

        if ( dword_5A7A80 < b->ypabact.commandID )
            dword_5A7A80 = b->ypabact.commandID;
    }
    else if ( !StriCmp(p1, "aggression") )
    {
        b->ypabact.aggr = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "mainstate") )
    {
        b->ypabact.status = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "extrastate") )
    {
        b->ypabact.status_flg = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "killerowner") )
    {
        b->ypabact.killer_owner = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "ident") )
    {
        b->ypabact.gid = std::stoi(p2);

        if ( bact_id < b->ypabact.gid )
        {
            bact_id = b->ypabact.gid;
        }
    }
    else if ( !StriCmp(p1, "primary") )
    {
        Stok stok(p2, " \t_\n");
        std::string tmp;
        if ( stok.GetNext(&tmp) )
        {
            b->ypabact.primTtype = std::stoi(tmp);
            if ( stok.GetNext(&tmp) )
            {
                b->ypabact.primT.pbact = (__NC_STACK_ypabact *)(size_t)std::stoi(tmp);
                if ( stok.GetNext(&tmp) )
                {
                    b->ypabact.primTpos.x = std::stod(tmp, 0);
                    if ( stok.GetNext(&tmp) )
                    {
                        b->ypabact.primTpos.z = std::stod(tmp, 0);
                        if ( stok.GetNext(&tmp) )
                            b->ypabact.primT_cmdID = std::stoi(tmp);
                    }
                }
            }
        }
    }
    else if ( !StriCmp(p1, "speed") )
    {
        Stok stok(p2, " \t_\n");
        std::string tmp;
        if ( stok.GetNext(&tmp) )
        {
            b->ypabact.fly_dir.x = std::stod(tmp, 0);
            if ( stok.GetNext(&tmp) )
            {
                b->ypabact.fly_dir.y = std::stod(tmp, 0);
                if ( stok.GetNext(&tmp) )
                {
                    b->ypabact.fly_dir.z = std::stod(tmp, 0);
                    if ( stok.GetNext(&tmp) )
                        b->ypabact.fly_dir_length = std::stod(tmp, 0);
                }
            }
        }
    }
    else if ( !StriCmp(p1, "energy") )
    {
        b->ypabact.energy = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "matrix") )
    {
        Stok stok(p2, " \t_\n");
        std::string tmp;
        if ( stok.GetNext(&tmp) )
        {
            b->ypabact.rotation.m00 = std::stod(tmp, 0);
            if ( stok.GetNext(&tmp) )
            {
                b->ypabact.rotation.m01 = std::stod(tmp, 0);
                if ( stok.GetNext(&tmp) )
                {
                    b->ypabact.rotation.m02 = std::stod(tmp, 0);
                    if ( stok.GetNext(&tmp) )
                    {
                        b->ypabact.rotation.m10 = std::stod(tmp, 0);
                        if ( stok.GetNext(&tmp) )
                        {
                            b->ypabact.rotation.m11 = std::stod(tmp, 0);
                            if ( stok.GetNext(&tmp) )
                            {
                                b->ypabact.rotation.m12 = std::stod(tmp, 0);
                                if ( stok.GetNext(&tmp) )
                                {
                                    b->ypabact.rotation.m20 = std::stod(tmp, 0);
                                    if ( stok.GetNext(&tmp) )
                                    {
                                        b->ypabact.rotation.m21 = std::stod(tmp, 0);
                                        if ( stok.GetNext(&tmp) )
                                            b->ypabact.rotation.m22 = std::stod(tmp, 0);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if ( !StriCmp(p1, "pos") )
    {
        Stok stok(p2, " \t_\n");
        std::string tmp;
        if ( stok.GetNext(&tmp) )
        {
            b->ypabact.position.x = std::stod(tmp, 0);
            if ( stok.GetNext(&tmp) )
            {
                b->ypabact.position.y = std::stod(tmp, 0);
                if ( stok.GetNext(&tmp) )
                    b->ypabact.position.z = std::stod(tmp, 0);
            }
        }
    }
    else if ( !StriCmp(p1, "force") )
    {
        b->ypabact.thraction = std::stod(p2, 0);
    }
    else if ( !StriCmp(p1, "gunangle") )
    {
        b->ypabact.gun_angle_user = std::stod(p2, 0);
    }
    else if ( !StriCmp(p1, "gunbasis") )
    {
        NC_STACK_ypagun *guno = dynamic_cast<NC_STACK_ypagun *>(b);

        Stok stok(p2, " \t_\n");
        std::string tmp;
        if ( stok.GetNext(&tmp) )
        {
            guno->ypagun.basis.x = std::stod(tmp, 0);
            if ( stok.GetNext(&tmp) )
            {
                guno->ypagun.basis.y = std::stod(tmp, 0);
                if ( stok.GetNext(&tmp) )
                    guno->ypagun.basis.z = std::stod(tmp, 0);
            }
        }
    }
    else if ( !StriCmp(p1, "waypoint") )
    {
        Stok stok(p2, " \t_\n");
        std::string tmp;
        if ( stok.GetNext(&tmp) )
        {
            int id = std::stoi(tmp);
            if ( stok.GetNext(&tmp) )
            {
                b->ypabact.waypoints[id].x = std::stod(tmp, 0);
                if ( stok.GetNext(&tmp) )
                    b->ypabact.waypoints[id].z = std::stod(tmp, 0);
            }
        }
    }
    else if ( !StriCmp(p1, "num_wp") )
    {
        b->ypabact.waypoints_count = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "count_wp") )
    {
        b->ypabact.current_waypoint = std::stoi(p2);
    }
    else
        return false;

    return true;
}


bool SaveRoboParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_robo") )
        return false;

    ypaworld_arg146 v5;
    v5.pos.x = 600;
    v5.pos.z = -600.0;
    v5.pos.y = 0;
    v5.vehicle_id = std::stoi(opt);

    _r = dynamic_cast<NC_STACK_yparobo *> ( _o.ypaworld_func146(&v5) );

    if ( !_r )
        return false;

    _lastRobo = _r;

    _o.ypaworld_func134(_r);

    return true;
}


int SaveRoboParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        if ( !_r->ypabact.reload_const )
            _r->ypabact.reload_const = _r->ypabact.energy_max;

        return ScriptParser::RESULT_SCOPE_END;
    }

    if ( SaveBactParser(_r, p1, p2) || RoboParser(p1, p2) )
        return ScriptParser::RESULT_OK;

    return ScriptParser::RESULT_UNKNOWN;
}

bool SaveSquadParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( !StriCmp(word, "begin_commander") )
    {
        ypaworld_arg146 v5;
        v5.pos.x = 600.0;
        v5.pos.z = -600.0;
        v5.pos.y = 0;
        v5.vehicle_id = std::stoi(opt);

        _c = _o.ypaworld_func146(&v5);

        if ( !_c )
            return false;

        _lastCommander = _c;
        _lastRobo->AddSubject(_c);
    }
    else if ( !StriCmp(word, "begin_slave") )
    {
        ypaworld_arg146 v5;
        v5.pos.x = 600.0;
        v5.pos.z = -600.0;
        v5.pos.y = 0;
        v5.vehicle_id = std::stoi(opt);

        _c = _o.ypaworld_func146(&v5);

        if ( !_c )
            return false;

        _lastCommander->AddSubject(_c);
    }
    else
        return false;

    return true;
}

int SaveSquadParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    if ( SaveBactParser(_c, p1, p2) )
        return ScriptParser::RESULT_OK;

    return ScriptParser::RESULT_UNKNOWN;
}

bool SaveGemParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_wunderinfo") )
        return false;

    _o.yw_InitTechUpgradeBuildings();
    return true;
}


int SaveGemParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    if ( !StriCmp(p1, "disablegem") )
    {
        int gemId = std::stoi(p2);

        if ( gemId >= 0 && gemId < 8 )
            _o.ypaworld.cells[_o.ypaworld.sectors_maxX2 * _o.ypaworld.gems[gemId].sec_y + _o.ypaworld.gems[gemId].sec_x].w_type = 0;
    }
    else
        return ScriptParser::RESULT_UNKNOWN;
    return ScriptParser::RESULT_OK;
}

int SaveExtraViewParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    if ( !StriCmp(p1, "kind") )
    {
        if ( !StriCmp(p2, "robogun") )
            _o._extraViewEnable = true;
    }
    else if ( !StriCmp(p1, "number") )
        _o._extraViewNumber = std::stoi(p2);
    else
        return ScriptParser::RESULT_UNKNOWN;
    return ScriptParser::RESULT_OK;
}


int SaveKwFactorParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    if ( !StriCmp(p1, "kw") )
    {
        Stok stok(p2, "_ \r\n");
        std::string tmp;
        if ( stok.GetNext(&tmp) )
        {
            int secX = std::stoi(tmp);
            if ( stok.GetNext(&tmp) )
            {
                int secY = std::stoi(tmp);
                if ( stok.GetNext(&tmp) )
                {
                    int pwr = std::stoi(tmp);

                    for (int i = 0; i < 256; i++)
                    {
                        yw_field34 &kw = _o.ypaworld.field_34[i];
                        if ( kw.p_cell )
                        {
                            if ( secX == kw.x && secY == kw.y )
                                kw.power_2 = pwr;
                        }
                    }
                }
            }
        }
    }
    else
        return ScriptParser::RESULT_UNKNOWN;
    return ScriptParser::RESULT_OK;
}

int SaveGlobalsParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    if ( !StriCmp(p1, "time") )
        _o.ypaworld.timeStamp = std::stoi(p2);
    else
        return ScriptParser::RESULT_UNKNOWN;
    return ScriptParser::RESULT_OK;
}

bool SaveOwnerMapParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_ownermap") )
        return false;

    for (int i = 0; i < 8; i++)
        _o.ypaworld.sectors_count_by_owner[i] = 0;

    if ( _o.ypaworld.own_map )
        delete _o.ypaworld.own_map;

    _o.ypaworld.own_map = ReadMapAsPlaneBytes(parser);

    if ( _o.ypaworld.own_map )
    {
        uint8_t *pbuf = _o.ypaworld.own_map->data();
        cellArea *v6 = _o.ypaworld.cells;

        for (int i = 0; i < (int)_o.ypaworld.own_map->size(); i++)
        {
            int v8 = pbuf[i];
            v6->owner = v8;
            _o.ypaworld.sectors_count_by_owner[v8]++;
            
            v6++;
        }
    }
    return true;
}

int SaveOwnerMapParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;
    return ScriptParser::RESULT_UNKNOWN;
}


bool SaveBuildingMapParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_buildingmap") )
        return false;

    if ( _o.ypaworld.blg_map )
        delete _o.ypaworld.blg_map;

    _o.ypaworld.blg_map = ReadMapAsPlaneBytes(parser);
    if ( _o.ypaworld.blg_map )
    {
        uint8_t *pbuf = _o.ypaworld.blg_map->data();
        cellArea *v6 = _o.ypaworld.cells;

        for (int i = 0; i < (int)_o.ypaworld.blg_map->size(); i++)
        {
            int v8 = *pbuf;

            if ( v8 && v6->owner )
            {
                ypaworld_arg148 arg148;
                arg148.field_18 = 1;
                arg148.ownerID = v6->owner;
                arg148.ownerID2 = v6->owner;
                arg148.blg_ID = v8;
                arg148.field_C = 1;
                arg148.x = i % _o.ypaworld.sectors_maxX2;
                arg148.y = i / _o.ypaworld.sectors_maxX2;

                _o.ypaworld_func148(&arg148);
            }

            v6++;
            pbuf++;
        }
    }
    return true;
}

int SaveBuildingMapParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;
    return ScriptParser::RESULT_UNKNOWN;
}

bool SaveEnergyMapParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_energymap") )
        return false;

    Common::PlaneBytes *nrgmap = ReadMapAsPlaneBytes(parser);

    if ( nrgmap )
    {
        int smax = _o.ypaworld.sectors_maxY2 * _o.ypaworld.sectors_maxX2;
        cellArea *v6 = _o.ypaworld.cells;
        int n = 0;

        for (int i = 0; i < smax; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    v6->buildings_health[j][k] = (*nrgmap)[n];
                    n++;
                }
            }

            v6++;

        }

        delete nrgmap;
    }
    return true;
}

int SaveEnergyMapParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;
    return ScriptParser::RESULT_UNKNOWN;
}

int SaveLevelNumParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    if ( !StriCmp(p1, "levelnum") )
        _levelNum = std::stoi(p2);
    else
        return ScriptParser::RESULT_UNKNOWN;
    return ScriptParser::RESULT_OK;
}

int SaveHistoryParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    if ( !StriCmp(p1, "history_buffer") )
    {
        std::string buf;
        parser.ReadLine(&buf);

        Stok stok(buf, " \r\n");
        std::string tmp;

        stok.GetNext(&tmp);
        int w = std::stol(tmp, NULL, 0);

        stok.GetNext(&tmp);
        int h = std::stol(tmp, NULL, 0);

        if ( _o.ypaworld.history->sub_47EDDC(h * w) )
        {
            uint8_t *cur = _o.ypaworld.history->last_bufStart;
            for (int i = 0; i < h; i++)
            {
                parser.ReadLine(&buf);
                stok = buf;

                for (int j = 0; j < w; j++)
                {
                    stok.GetNext(&tmp);

                    *cur = std::stol(tmp, 0, 16);
                    cur++;
                }
            }

            cur = _o.ypaworld.history->last_bufStart;
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

            _o.ypaworld.history->field_1C = NULL;
            _o.ypaworld.history->last_bufStart = cur;
        }
    }
    else
        return ScriptParser::RESULT_UNKNOWN;
    return ScriptParser::RESULT_OK;
}

int SaveMasksParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    if ( !StriCmp(p1, "ownermask") )
    {
        _o.ypaworld.field_2d90->ownerMap__has_vehicles = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "usermask") )
    {
        _o.ypaworld.field_2d90->field_60 = std::stoi(p2);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;
    return ScriptParser::RESULT_OK;
}

int SaveSuperBombParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    if ( !StriCmp(p1, "status") )
    {
        switch( std::stoi(p2) )
        {
        case 1:
            _o.sub_4D12D8(_id, 0);
            break;
        case 2:
            _o.sub_4D1594(_id);
            break;
        case 3:
            _o.sub_4D1444(_id);
            break;
        default:
            break;
        }
    }
    else if ( !StriCmp(p1, "num") )
    {
        _id = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "activated_by") )
    {
        _o.ypaworld.field_2d90->supetItems[_id].field_F4 = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "active_timestamp") )
    {
        _o.ypaworld.field_2d90->supetItems[_id].field_EC = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "trigger_timestamp") )
    {
        _o.ypaworld.field_2d90->supetItems[_id].field_F0 = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "countdown") )
    {
        _o.ypaworld.field_2d90->supetItems[_id].field_F8 = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "last_ten_sec") )
    {
        _o.ypaworld.field_2d90->supetItems[_id].field_FC = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "last_sec") )
    {
        _o.ypaworld.field_2d90->supetItems[_id].field_100 = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "radius") )
    {
        _o.ypaworld.field_2d90->supetItems[_id].field_104 = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "last_radius") )
    {
        _o.ypaworld.field_2d90->supetItems[_id].field_108 = std::stoi(p2);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;
    return ScriptParser::RESULT_OK;
}

}
}
