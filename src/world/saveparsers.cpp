#include "saveparsers.h"
#include "../fmtlib/core.h"
#include "../fmtlib/printf.h"
#include "../yw.h"
#include "../yparobo.h"
#include "../log.h"
#include "history.h"


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
        _r->_owner = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "is_user_robo") )
    {
        if ( !StriCmp(p2, "yes") )
            _o.setYW_userHostStation(_r);
    }
    else if ( !StriCmp(p1, "robostate") )
    {
        _r->_roboState = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "dockenergy") )
    {
        _r->_roboDockEnerg = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "dockcount") )
    {
        _r->_roboDockCnt = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "dockuser") )
    {
        _r->_roboDockUser = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "docktime") )
    {
        _r->_roboDockTime = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "docktargettype") )
    {
        _r->_roboDockTargetType = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "dockaggr") )
    {
        _r->_roboDockAggr = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "docktargetpos") )
    {
        Stok stok(p2, " \t_\n");
        std::string tmp;
        if ( stok.GetNext(&tmp) )
        {
            _r->_roboDockTargetPos.x = std::stod(tmp, 0);
            if ( stok.GetNext(&tmp) )
                _r->_roboDockTargetPos.z = std::stod(tmp, 0);
        }
    }
    else if ( !StriCmp(p1, "docktargetID") )
    {
        _r->_roboDockTargetCommandID = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "fillmodus") )
    {
        _r->_roboFillMode = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "battbuilding") )
    {
    }
    else if ( !StriCmp(p1, "battvehicle") )
    {
        _r->_roboEnergyLife = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "buildspare") )
    {
        _r->_roboBuildSpare = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "battbeam") )
    {
        _r->_roboEnergyMove = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "vhoriz") )
    {
        _r->_viewer_horiz_angle = std::stod(p2, 0);
    }
    else if ( !StriCmp(p1, "vvert") )
    {
        _r->_viewer_vert_angle = std::stod(p2, 0);
    }
    else if ( !StriCmp(p1, "maximum") )
    {
        _r->_energy_max = std::stoi(p2);

        if ( _o.UserRobo == _r )
        {
            if ( _r->_energy_max < _o._maxRoboEnergy )
            {
                _r->_energy_max = _o._maxRoboEnergy;
                _r->_reload_const = _o._maxReloadConst;
            }
        }
    }
    else if ( !StriCmp(p1, "con_budget") )
    {
        _r->_roboEpConquer = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "def_budget") )
    {
        _r->_roboEpDefense = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "rec_budget") )
    {
        _r->_roboEpRecon = std::stoi(p2);
    }
    else  if ( !StriCmp(p1, "rob_budget") )
    {
        _r->_roboEpRobo = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "rad_budget") )
    {
        _r->_roboEpRadar = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "pow_budget") )
    {
        _r->_roboEpPower = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "saf_budget") )
    {
        _r->_roboEpSafety = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "cpl_budget") )
    {
        _r->_roboEpChangePlace = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "saf_delay") )
    {
        _r->_roboSafetyDelay = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "pow_delay") )
    {
        _r->_roboPowerDelay = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "rad_delay") )
    {
        _r->_roboRadarDelay = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "cpl_delay") )
    {
        _r->_roboPositionDelay = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "def_delay") )
    {
        _r->_roboEnemyDelay = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "con_delay") )
    {
        _r->_roboConqDelay = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "rec_delay") )
    {
        _r->_roboExploreDelay = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "rob_delay") )
    {
        _r->_roboDangerDelay = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "reload_const") )
    {
        if ( !_r->_reload_const )
            _r->_reload_const = std::stoi(p2);
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
        b->_commandID = (std::stoi(p2) & 0xFFFFFF);

        if ( dword_5A7A80 < b->_commandID )
            dword_5A7A80 = b->_commandID;
    }
    else if ( !StriCmp(p1, "aggression") )
    {
        b->_aggr = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "mainstate") )
    {
        b->_status = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "extrastate") )
    {
        b->_status_flg = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "killerowner") )
    {
        b->_killer_owner = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "ident") )
    {
        b->_gid = std::stoi(p2);

        if ( bact_id < b->_gid )
        {
            bact_id = b->_gid;
        }
    }
    else if ( !StriCmp(p1, "primary") )
    {
        Stok stok(p2, " \t_\n");
        std::string tmp;
        if ( stok.GetNext(&tmp) )
        {
            b->_primTtype = std::stoi(tmp);
            if ( stok.GetNext(&tmp) )
            {
                b->_primT.pbact = (NC_STACK_ypabact *)(size_t)std::stoi(tmp);
                if ( stok.GetNext(&tmp) )
                {
                    b->_primTpos.x = std::stod(tmp, 0);
                    if ( stok.GetNext(&tmp) )
                    {
                        b->_primTpos.z = std::stod(tmp, 0);
                        if ( stok.GetNext(&tmp) )
                            b->_primT_cmdID = std::stoi(tmp);
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
            b->_fly_dir.x = std::stod(tmp, 0);
            if ( stok.GetNext(&tmp) )
            {
                b->_fly_dir.y = std::stod(tmp, 0);
                if ( stok.GetNext(&tmp) )
                {
                    b->_fly_dir.z = std::stod(tmp, 0);
                    if ( stok.GetNext(&tmp) )
                        b->_fly_dir_length = std::stod(tmp, 0);
                }
            }
        }
    }
    else if ( !StriCmp(p1, "energy") )
    {
        b->_energy = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "matrix") )
    {
        Stok stok(p2, " \t_\n");
        std::string tmp;
        if ( stok.GetNext(&tmp) )
        {
            b->_rotation.m00 = std::stod(tmp, 0);
            if ( stok.GetNext(&tmp) )
            {
                b->_rotation.m01 = std::stod(tmp, 0);
                if ( stok.GetNext(&tmp) )
                {
                    b->_rotation.m02 = std::stod(tmp, 0);
                    if ( stok.GetNext(&tmp) )
                    {
                        b->_rotation.m10 = std::stod(tmp, 0);
                        if ( stok.GetNext(&tmp) )
                        {
                            b->_rotation.m11 = std::stod(tmp, 0);
                            if ( stok.GetNext(&tmp) )
                            {
                                b->_rotation.m12 = std::stod(tmp, 0);
                                if ( stok.GetNext(&tmp) )
                                {
                                    b->_rotation.m20 = std::stod(tmp, 0);
                                    if ( stok.GetNext(&tmp) )
                                    {
                                        b->_rotation.m21 = std::stod(tmp, 0);
                                        if ( stok.GetNext(&tmp) )
                                            b->_rotation.m22 = std::stod(tmp, 0);
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
            b->_position.x = std::stod(tmp, 0);
            if ( stok.GetNext(&tmp) )
            {
                b->_position.y = std::stod(tmp, 0);
                if ( stok.GetNext(&tmp) )
                    b->_position.z = std::stod(tmp, 0);
            }
        }
    }
    else if ( !StriCmp(p1, "force") )
    {
        b->_thraction = std::stod(p2, 0);
    }
    else if ( !StriCmp(p1, "gunangle") )
    {
        b->_gun_angle_user = std::stod(p2, 0);
    }
    else if ( !StriCmp(p1, "gunbasis") )
    {
        NC_STACK_ypagun *guno = dynamic_cast<NC_STACK_ypagun *>(b);

        Stok stok(p2, " \t_\n");
        std::string tmp;
        if ( stok.GetNext(&tmp) )
        {
            guno->_gunBasis.x = std::stod(tmp, 0);
            if ( stok.GetNext(&tmp) )
            {
                guno->_gunBasis.y = std::stod(tmp, 0);
                if ( stok.GetNext(&tmp) )
                    guno->_gunBasis.z = std::stod(tmp, 0);
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
                b->_waypoints[id].x = std::stod(tmp, 0);
                if ( stok.GetNext(&tmp) )
                    b->_waypoints[id].z = std::stod(tmp, 0);
            }
        }
    }
    else if ( !StriCmp(p1, "num_wp") )
    {
        b->_waypoints_count = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "count_wp") )
    {
        b->_current_waypoint = std::stoi(p2);
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
        if ( !_r->_reload_const )
            _r->_reload_const = _r->_energy_max;

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

        if ( gemId >= 0 && gemId < (int)_o._Gems.size() )
            _o._cells[_o._mapWidth * _o._Gems[gemId].SecY + _o._Gems[gemId].SecX].w_type = 0;
        else
            printf("SaveGemParser::Handle : gemId = %d but _Gems.size() = %d\n", gemId, (int)_o._Gems.size());
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
                        yw_field34 &kw = _o._powerStations[i];
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
        _o.timeStamp = std::stoi(p2);
    else
        return ScriptParser::RESULT_UNKNOWN;
    return ScriptParser::RESULT_OK;
}

bool SaveOwnerMapParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_ownermap") )
        return false;

    for (int i = 0; i < 8; i++)
        _o.sectors_count_by_owner[i] = 0;

    if ( _o.own_map )
        delete _o.own_map;

    _o.own_map = ReadMapAsPlaneBytes(parser);

    if ( _o.own_map )
    {
        uint8_t *pbuf = _o.own_map->data();
        cellArea *v6 = _o._cells;

        for (int i = 0; i < (int)_o.own_map->size(); i++)
        {
            int v8 = pbuf[i];
            v6->owner = v8;
            _o.sectors_count_by_owner[v8]++;
            
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

    if ( _o.blg_map )
        delete _o.blg_map;

    _o.blg_map = ReadMapAsPlaneBytes(parser);
    if ( _o.blg_map )
    {
        uint8_t *pbuf = _o.blg_map->data();
        cellArea *v6 = _o._cells;

        for (int i = 0; i < (int)_o.blg_map->size(); i++)
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
                arg148.x = i % _o._mapWidth;
                arg148.y = i / _o._mapWidth;

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
        int smax = _o._mapHeight * _o._mapWidth;
        cellArea *v6 = _o._cells;
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

        std::vector<uint8_t> hh( w * h );
        size_t k = 0;
        for (int i = 0; i < h; i++)
        {
            parser.ReadLine(&buf);
            stok = buf;

            for (int j = 0; j < w; j++)
            {
                stok.GetNext(&tmp);

                hh[k] = std::stol(tmp, 0, 16);
                k++;
            }
        }
        
        History::Instance decoders;

        k = 0;
        while(decoders[ hh[k] ] != NULL)
        {
            size_t sz = decoders[ hh[k] ]->dataSize + 1;
            _o._history.Write(&hh[k], sz);
            k += sz;            
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
        _o._levelInfo->OwnerMask = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "usermask") )
    {
        _o._levelInfo->UserMask = std::stoi(p2);
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
        _o._levelInfo->SuperItems[_id].ActivateOwner = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "active_timestamp") )
    {
        _o._levelInfo->SuperItems[_id].ActiveTime = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "trigger_timestamp") )
    {
        _o._levelInfo->SuperItems[_id].TriggerTime = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "countdown") )
    {
        _o._levelInfo->SuperItems[_id].CountDown = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "last_ten_sec") )
    {
        _o._levelInfo->SuperItems[_id].LastTenSec = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "last_sec") )
    {
        _o._levelInfo->SuperItems[_id].LastSec = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "radius") )
    {
        _o._levelInfo->SuperItems[_id].CurrentRadius = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "last_radius") )
    {
        _o._levelInfo->SuperItems[_id].LastRadius = std::stoi(p2);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;
    return ScriptParser::RESULT_OK;
}

}
}
