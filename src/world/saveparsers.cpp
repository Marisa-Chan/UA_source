#include "saveparsers.h"
#include "../fmtlib/core.h"
#include "../fmtlib/printf.h"
#include "../yw.h"
#include "../yparobo.h"
#include "../log.h"
#include "history.h"


extern uint32_t dword_5A7A80;
extern uint32_t bact_id;

namespace World
{
namespace Parsers
{
NC_STACK_yparobo *SaveBact::_lastRobo = NULL;
NC_STACK_ypabact *SaveBact::_lastCommander = NULL;


bool SaveRoboParser::RoboParser(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "owner") )
    {
        _r->_owner = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "is_user_robo") )
    {
        if ( !StriCmp(p2, "yes") )
            _o.setYW_userHostStation(_r);
    }
    else if ( !StriCmp(p1, "robostate") )
    {
        _r->_roboState = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "dockenergy") )
    {
        _r->_roboDockEnerg = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "dockcount") )
    {
        _r->_roboDockCnt = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "dockuser") )
    {
        _r->_roboDockUser = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "docktime") )
    {
        _r->_roboDockTime = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "docktargettype") )
    {
        _r->_roboDockTargetType = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "dockaggr") )
    {
        _r->_roboDockAggr = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "docktargetpos") )
    {
        Stok stok(p2, " \t_\n");
        std::string tmp;
        if ( stok.GetNext(&tmp) )
        {
            _r->_roboDockTargetPos.x = parser.stod(tmp, 0);
            if ( stok.GetNext(&tmp) )
                _r->_roboDockTargetPos.z = parser.stod(tmp, 0);
        }
    }
    else if ( !StriCmp(p1, "docktargetID") )
    {
        _r->_roboDockTargetCommandID = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "fillmodus") )
    {
        _r->_roboFillMode = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "battbuilding") )
    {
    }
    else if ( !StriCmp(p1, "battvehicle") )
    {
        _r->_roboEnergyLife = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "buildspare") )
    {
        _r->_roboBuildSpare = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "battbeam") )
    {
        _r->_roboEnergyMove = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "vhoriz") )
    {
        _r->_viewer_horiz_angle = parser.stod(p2, 0);
    }
    else if ( !StriCmp(p1, "vvert") )
    {
        _r->_viewer_vert_angle = parser.stod(p2, 0);
    }
    else if ( !StriCmp(p1, "maximum") )
    {
        _r->_energy_max = parser.stoi(p2);

        if ( _o._userRobo == _r )
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
        _r->_roboEpConquer = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "def_budget") )
    {
        _r->_roboEpDefense = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "rec_budget") )
    {
        _r->_roboEpRecon = parser.stoi(p2);
    }
    else  if ( !StriCmp(p1, "rob_budget") )
    {
        _r->_roboEpRobo = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "rad_budget") )
    {
        _r->_roboEpRadar = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "pow_budget") )
    {
        _r->_roboEpPower = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "saf_budget") )
    {
        _r->_roboEpSafety = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "cpl_budget") )
    {
        _r->_roboEpChangePlace = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "saf_delay") )
    {
        _r->_roboSafetyDelay = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "pow_delay") )
    {
        _r->_roboPowerDelay = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "rad_delay") )
    {
        _r->_roboRadarDelay = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "cpl_delay") )
    {
        _r->_roboPositionDelay = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "def_delay") )
    {
        _r->_roboEnemyDelay = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "con_delay") )
    {
        _r->_roboConqDelay = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "rec_delay") )
    {
        _r->_roboExploreDelay = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "rob_delay") )
    {
        _r->_roboDangerDelay = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "reload_const") )
    {
        if ( !_r->_reload_const )
            _r->_reload_const = parser.stoi(p2);
    }
    else
        return false;

    return true;
}


bool SaveBact::SaveBactParser(ScriptParser::Parser &parser, NC_STACK_ypabact *b, const std::string &p1, const std::string &p2)
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
        b->_commandID = (parser.stoi(p2) & 0xFFFFFF);

        if ( dword_5A7A80 < b->_commandID )
            dword_5A7A80 = b->_commandID;
    }
    else if ( !StriCmp(p1, "aggression") )
    {
        b->_aggr = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "mainstate") )
    {
        b->_status = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "extrastate") )
    {
        b->_status_flg = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "killerowner") )
    {
        b->_killer_owner = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "ident") )
    {
        b->_gid = parser.stoi(p2);

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
            b->_primTtype = parser.stoi(tmp);
            if ( stok.GetNext(&tmp) )
            {
                b->_primT.pbact = (NC_STACK_ypabact *)(size_t)parser.stoi(tmp);
                if ( stok.GetNext(&tmp) )
                {
                    b->_primTpos.x = parser.stod(tmp, 0);
                    if ( stok.GetNext(&tmp) )
                    {
                        b->_primTpos.z = parser.stod(tmp, 0);
                        if ( stok.GetNext(&tmp) )
                            b->_primT_cmdID = parser.stoi(tmp);
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
            b->_fly_dir.x = parser.stod(tmp, 0);
            if ( stok.GetNext(&tmp) )
            {
                b->_fly_dir.y = parser.stod(tmp, 0);
                if ( stok.GetNext(&tmp) )
                {
                    b->_fly_dir.z = parser.stod(tmp, 0);
                    if ( stok.GetNext(&tmp) )
                        b->_fly_dir_length = parser.stod(tmp, 0);
                }
            }
        }
    }
    else if ( !StriCmp(p1, "energy") )
    {
        b->_energy = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "matrix") )
    {
        Stok stok(p2, " \t_\n");
        std::string tmp;
        if ( stok.GetNext(&tmp) )
        {
            b->_rotation.m00 = parser.stod(tmp, 0);
            if ( stok.GetNext(&tmp) )
            {
                b->_rotation.m01 = parser.stod(tmp, 0);
                if ( stok.GetNext(&tmp) )
                {
                    b->_rotation.m02 = parser.stod(tmp, 0);
                    if ( stok.GetNext(&tmp) )
                    {
                        b->_rotation.m10 = parser.stod(tmp, 0);
                        if ( stok.GetNext(&tmp) )
                        {
                            b->_rotation.m11 = parser.stod(tmp, 0);
                            if ( stok.GetNext(&tmp) )
                            {
                                b->_rotation.m12 = parser.stod(tmp, 0);
                                if ( stok.GetNext(&tmp) )
                                {
                                    b->_rotation.m20 = parser.stod(tmp, 0);
                                    if ( stok.GetNext(&tmp) )
                                    {
                                        b->_rotation.m21 = parser.stod(tmp, 0);
                                        if ( stok.GetNext(&tmp) )
                                            b->_rotation.m22 = parser.stod(tmp, 0);
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
            b->_position.x = parser.stod(tmp, 0);
            if ( stok.GetNext(&tmp) )
            {
                b->_position.y = parser.stod(tmp, 0);
                if ( stok.GetNext(&tmp) )
                    b->_position.z = parser.stod(tmp, 0);
            }
        }
    }
    else if ( !StriCmp(p1, "force") )
    {
        b->_thraction = parser.stod(p2, 0);
    }
    else if ( !StriCmp(p1, "gunangle") )
    {
        b->_gun_angle_user = parser.stod(p2, 0);
    }
    else if ( !StriCmp(p1, "gunbasis") )
    {
        NC_STACK_ypagun *guno = dynamic_cast<NC_STACK_ypagun *>(b);

        Stok stok(p2, " \t_\n");
        std::string tmp;
        if ( stok.GetNext(&tmp) )
        {
            guno->_gunBasis.x = parser.stod(tmp, 0);
            if ( stok.GetNext(&tmp) )
            {
                guno->_gunBasis.y = parser.stod(tmp, 0);
                if ( stok.GetNext(&tmp) )
                    guno->_gunBasis.z = parser.stod(tmp, 0);
            }
        }
    }
    else if ( !StriCmp(p1, "waypoint") )
    {
        Stok stok(p2, " \t_\n");
        std::string tmp;
        if ( stok.GetNext(&tmp) )
        {
            int id = parser.stoi(tmp);
            if ( stok.GetNext(&tmp) )
            {
                b->_waypoints[id].x = parser.stod(tmp, 0);
                if ( stok.GetNext(&tmp) )
                    b->_waypoints[id].z = parser.stod(tmp, 0);
            }
        }
    }
    else if ( !StriCmp(p1, "num_wp") )
    {
        b->_waypoints_count = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "count_wp") )
    {
        b->_current_waypoint = parser.stoi(p2);
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
    v5.vehicle_id = parser.stoi(opt);

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

    if ( SaveBactParser(parser, _r, p1, p2) || RoboParser(parser, p1, p2) )
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
        v5.vehicle_id = parser.stoi(opt);

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
        v5.vehicle_id = parser.stoi(opt);

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

    if ( SaveBactParser(parser, _c, p1, p2) )
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
        int gemId = parser.stoi(p2);

        if ( gemId >= 0 && gemId < (int)_o._techUpgrades.size() )
            _o.GetSector(_o._techUpgrades[gemId].CellId)->PurposeType = cellArea::PT_NONE;
        else
            printf("SaveGemParser::Handle : gemId = %d but _Gems.size() = %d\n", gemId, (int)_o._techUpgrades.size());
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
        _o._extraViewNumber = parser.stoi(p2);
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
            int secX = parser.stoi(tmp);
            if ( stok.GetNext(&tmp) )
            {
                int secY = parser.stoi(tmp);
                if ( stok.GetNext(&tmp) )
                {
                    int pwr = parser.stoi(tmp);

                    for (auto kw : _o._powerStations)
                    {
                        if ( kw.second.pCell )
                        {
                            if ( secX == kw.second.CellId.x && secY == kw.second.CellId.y )
                            {
                                kw.second.EffectivePower = pwr;
                                break;
                            }
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
        _o._timeStamp = parser.stoi(p2);
    else
        return ScriptParser::RESULT_UNKNOWN;
    return ScriptParser::RESULT_OK;
}

bool SaveOwnerMapParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_ownermap") )
        return false;

    _o._countSectorsPerOwner.fill(0);

    _o._lvlOwnMap = ReadMapAsPlaneBytes(parser);

    if ( _o._lvlOwnMap.IsNotNull() )
    {
        for (int y = 0; y < _o._mapSize.y; y++)
        {
            for (int x = 0; x < _o._mapSize.x; x++)
            {
                int own = _o._lvlOwnMap(x, y);
                _o._cells(x, y).owner = own;
                _o._countSectorsPerOwner[own]++;
            }
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

    _o._lvlBuildingsMap = ReadMapAsPlaneBytes(parser);
    
    if ( _o._lvlBuildingsMap.IsNotNull() )
    {
        for (int y = 0; y < _o._mapSize.y; y++)
        {
            for (int x = 0; x < _o._mapSize.x; x++)
            {
                cellArea &cell = _o._cells(x, y);
                int blg = _o._lvlBuildingsMap(x, y);
                
                if (blg > 0 && cell.owner != 0)
                {
                    ypaworld_arg148 arg148;
                    arg148.field_18 = 1;
                    arg148.owner = cell.owner;
                    arg148.blg_ID = blg;
                    arg148.field_C = 1;
                    arg148.CellId = cell.CellId;

                    _o.ypaworld_func148(&arg148);
                }
            }
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

    Common::PlaneBytes nrgmap = ReadMapAsPlaneBytes(parser);

    if ( nrgmap.IsOk() )
    {
        for (size_t i = 0; i < _o._cells.size(); ++i)
        {
            cellArea &cell = _o._cells.At(i);

            for(size_t j = 0; j < 9; j++)
                cell.buildings_health.At(j / 3, j % 3) = nrgmap( i * 9 + j );
        }
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
        _levelNum = parser.stoi(p2);
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
        int w = parser.stol(tmp, NULL, 0);

        stok.GetNext(&tmp);
        int h = parser.stol(tmp, NULL, 0);

        std::vector<uint8_t> hh( w * h );
        size_t k = 0;
        for (int i = 0; i < h; i++)
        {
            parser.ReadLine(&buf);
            stok = buf;

            for (int j = 0; j < w; j++)
            {
                stok.GetNext(&tmp);

                hh[k] = parser.stol(tmp, 0, 16);
                k++;
            }
        }
        
        History::Instance decoders;

        k = 0;
        while(k < hh.size() && decoders[ hh.at(k) ] != NULL)
        {
            size_t sz = decoders[ hh.at(k) ]->dataSize + 1;
            _o._history.Write(&hh.at(k), sz);
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
        _o._levelInfo.OwnerMask = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "usermask") )
    {
        _o._levelInfo.UserMask = parser.stoi(p2);
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
        switch( parser.stoi(p2) )
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
        _id = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "activated_by") )
    {
        _o._levelInfo.SuperItems[_id].ActivateOwner = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "active_timestamp") )
    {
        _o._levelInfo.SuperItems[_id].ActiveTime = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "trigger_timestamp") )
    {
        _o._levelInfo.SuperItems[_id].TriggerTime = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "countdown") )
    {
        _o._levelInfo.SuperItems[_id].CountDown = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "last_ten_sec") )
    {
        _o._levelInfo.SuperItems[_id].LastTenSec = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "last_sec") )
    {
        _o._levelInfo.SuperItems[_id].LastSec = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "radius") )
    {
        _o._levelInfo.SuperItems[_id].CurrentRadius = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "last_radius") )
    {
        _o._levelInfo.SuperItems[_id].LastRadius = parser.stoi(p2);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;
    return ScriptParser::RESULT_OK;
}


bool SaveLuaScriptParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_luascript") )
        return false;

    std::string buf;
    std::string script;
    std::regex re(".*EOF.*");
    while ( parser.ReadLine(&buf) )
    {
        if ( std::regex_search(buf, re) )
        {
            if (_o._script)
                _o._script->RunBuffer(script);
                
            break;
        }
        
        script += buf;
    }
    
    return true;
}

int SaveLuaScriptParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;
    return ScriptParser::RESULT_OK;
}


}
}
