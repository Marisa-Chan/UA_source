#include <inttypes.h>
#include <string.h>

#include "includes.h"
#include "yw.h"
#include "yw_internal.h"
#include "lstvw.h"
#include "font.h"
#include "env.h"

extern GuiList stru_5C91D0;

void ypaworld_func158__sub4__sub1__sub4__sub3(NC_STACK_ypaworld *yw, TInputState *inpt, TBriefengScreen *brf)
{
    SFXEngine::SFXe.StopMusicTrack();
    if ( yw->_GameShell->missiontrack )
    {
        SFXEngine::SFXe.SetMusicTrack(yw->_GameShell->missiontrack, yw->_GameShell->missiontrack__adv.min_delay, yw->_GameShell->missiontrack__adv.max_delay);
        SFXEngine::SFXe.PlayMusicTrack();
    }

    brf->StartTime = brf->CurrTime;
    brf->Stage = TBriefengScreen::STAGE_SCALING;

    brf->MapBlitParams.pbitm = brf->MbmapImg->GetBitmap();

    brf->MapBlitStart.left = -0.4515625;
    brf->MapBlitStart.top = -0.324999988079;
    brf->MapBlitStart.right = brf->MapBlitStart.left;
    brf->MapBlitStart.bottom = brf->MapBlitStart.top;

    float v17, v16;

    if ( yw->_mapSize.x == yw->_mapSize.y )
    {
        v17 = 1.0;
        v16 = 1.0;
    }
    else if (yw->_mapSize.x < yw->_mapSize.y)
    {
        v16 = 1.0;
        v17 = (float)yw->_mapSize.x / (float)yw->_mapSize.y;
    }
    else if (yw->_mapSize.x > yw->_mapSize.y)
    {
        v17 = 1.0;
        v16 = (float)yw->_mapSize.y / (float)yw->_mapSize.x;
    }

    brf->MapBlitParams.float4 = Common::FRect(-1.0, -1.0, 1.0, 1.0);

    brf->MapBlitEnd.left = -0.4515625 - v17 * 0.4828125;
    brf->MapBlitEnd.top = -0.325 - v16 * 0.6083333333333334;
    brf->MapBlitEnd.right = brf->MapBlitEnd.left + v17 * 0.965625;
    brf->MapBlitEnd.bottom = brf->MapBlitEnd.top + v16 * 1.216666666666667;
    brf->MapBlitParams.float14 = brf->MapBlitStart;
    brf->TextTime = 0;
    brf->PreTextTime = brf->CurrTime;
}

void ypaworld_func158__sub4__sub1__sub4__sub4(NC_STACK_ypaworld *yw, TInputState *inpt, TBriefengScreen *brf)
{
    int v5 = brf->CurrTime - brf->StartTime;

    if ( v5 >= 600 )
    {
        brf->MapBlitParams.float14 = brf->MapBlitEnd;
        brf->Stage = TBriefengScreen::STAGE_SCALEEND;

        GFX::displ_arg263 v4;
        if (yw->_mousePointers[5])
            v4.bitm = yw->_mousePointers[5]->GetBitmap();
        v4.pointer_id = 6;

        GFX::Engine.SetCursor(v4.pointer_id, 0);
    }
    else
    {
        float v3 = v5 * 0.0016666667;
        brf->MapBlitParams.float14.left = (brf->MapBlitEnd.left - brf->MapBlitStart.left) * v3 + brf->MapBlitStart.left;
        brf->MapBlitParams.float14.top = (brf->MapBlitEnd.top - brf->MapBlitStart.top) * v3 + brf->MapBlitStart.top;
        brf->MapBlitParams.float14.right = (brf->MapBlitEnd.right - brf->MapBlitStart.right) * v3 + brf->MapBlitStart.right;
        brf->MapBlitParams.float14.bottom = (brf->MapBlitEnd.bottom - brf->MapBlitStart.bottom) * v3 + brf->MapBlitStart.bottom;
    }
}

int yw_MBLoadSet(NC_STACK_ypaworld *yw, int setID)
{
    std::string oldRsrc = Common::Env.SetPrefix("rsrc", fmt::sprintf("data:set%d:", setID));

    if ( setID != yw->_setId && setID != 46 )
    {
        if ( yw->_setData )
        {
            ypa_log_out("yw_MBLoadSet(): killing set object %d\n", yw->_setId);
            yw->_setData->Delete();
            yw->_setData = NULL;
            yw->_setId = 0;
        }

        yw->_setData = load_set_base();
        if ( !yw->_setData )
        {
            ypa_log_out("yw_MBLoadSet(): loading set object %d failed\n", setID);
            Common::Env.SetPrefix("rsrc", oldRsrc);
            return 0;
        }
        
        yw->_setData->MakeVBO();

        yw->_setId = setID;
        ypa_log_out("yw_MBLoadSet(): loaded set object %d ok\n", setID);
    }

    if ( setID != 46 )
    {
        FSMgr::FileHandle *fil = uaOpenFileAlloc("rsrc:scripts/set.sdf", "r");
        if ( !fil )
        {
            ypa_log_out("Briefing: no set description file.\n");
            return 0;
        }

        int kid_id = 0;

        for( NC_STACK_base *& bs : yw->_setData->GetKidList() )
        {
            if ( kid_id == 0 )
            {
                if ( !sub_44A12C(yw, bs) )
                {
                    delete fil;
                    return 0;
                }
            }
            else if ( kid_id == 1 )
            {
                if ( !yw_parse_lego(yw, fil, bs) )
                {
                    delete fil;
                    return 0;
                }

                if ( !yw_parse_subSect(yw, fil) )
                {
                    delete fil;
                    return 0;
                }

                if ( !yw_parse_sektor(yw, fil) )
                {
                    delete fil;
                    return 0;
                }
            }
            else if ( kid_id == 2 )
            {
                if ( !sub_44A97C(yw, bs) )
                {
                    delete fil;
                    return 0;
                }
            }

            kid_id++;
        }

        delete fil;
        
        yw->LoadOverrideModels();
    }

    Common::Env.SetPrefix("rsrc", oldRsrc);

    return 1;
}

void ypaworld_func158__sub4__sub1__sub4__sub5(NC_STACK_ypaworld *yw, TInputState *inpt, TBriefengScreen *brf)
{
    brf->AddObjectsFlag = true;
    brf->TextTime = brf->CurrTime;
    brf->Objects.clear();

    if ( yw_MBLoadSet(yw, brf->Desc.SetID) )
    {
        brf->Stage = TBriefengScreen::STAGE_PLAYER_ST;

        GFX::displ_arg263 v6;
        if (yw->_mousePointers[0])
            v6.bitm = yw->_mousePointers[0]->GetBitmap();
        v6.pointer_id = 1;

        GFX::Engine.SetCursor(v6.pointer_id, 0);
    }
    else
    {
        brf->Stage = TBriefengScreen::STAGE_PLAYLEVEL;
    }
}

void ypaworld_func158__sub4__sub1__sub4__sub6(NC_STACK_ypaworld *yw, TInputState *inpt, TBriefengScreen *brf)
{
    brf->ViewingObject = TBriefObject();

    brf->ActiveElementID = -1;
    brf->ElementsCount = 1;

    brf->Stage = TBriefengScreen::STAGE_PLAYER_RN;
    brf->StartTime = brf->CurrTime;

    brf->SelectedObjID = -1;
    brf->ObjDescription = yw->GetLocaleString(150, "YOU ARE HERE");
}

void NC_STACK_ypaworld::BriefingSetObject(const TBriefObject &obj, bool doAdd)
{
    _briefScreen.ViewingObjectStartTime = _briefScreen.CurrTime;
    
    _briefScreen.ViewingObject = obj;

    if ( doAdd )
        _briefScreen.Objects.push_back( obj );

    if ( _GameShell )
        SFXEngine::SFXe.startSound(&_GameShell->samples1_info, 11);
}

void ypaworld_func158__sub4__sub1__sub4__sub7(NC_STACK_ypaworld *yw, TInputState *inpt, TBriefengScreen *brf)
{
    if ( brf->CurrTime - brf->StartTime >= 2500 )
    {
        brf->Stage = TBriefengScreen::STAGE_PLAYER_END;
    }
    else if ( brf->ActiveElementID )
    {
        MapRobo &robo = brf->Desc.Robos[0];

        brf->ActiveElementID = 0;
        yw->BriefingSetObject( TBriefObject(TBriefObject::TYPE_VEHICLE, robo.VhclID, robo.Pos.x, robo.Pos.z,
                                           26, 128 + robo.Owner, 25,
                                           yw->GetLocaleString(robo.VhclID + 1200, yw->_vhclProtos[ robo.VhclID ].name)), 
                               brf->AddObjectsFlag);
    }
}

void yw_BriefSetupKeySectors(NC_STACK_ypaworld *yw, TInputState *inpt, TBriefengScreen *brf)
{
    brf->ViewingObject = TBriefObject();

    brf->ActiveElementID = -1;
    brf->ElementsCount = 0;
    brf->StartTime = brf->CurrTime;

    brf->ObjDescription = yw->GetLocaleString(151, "PRIMARY TARGETS");

    for (const TMapGate &gate : yw->_levelInfo.Gates )
    {
        if ( gate.MbStatus != 1 )
            brf->ElementsCount += gate.KeySectors.size();
    }

    if ( brf->ElementsCount == 0 )
        brf->Stage = TBriefengScreen::STAGE_KEYS_END;
    else
        brf->Stage = TBriefengScreen::STAGE_KEYS_RN;
}

void yw_BriefUpdateKeySectors(NC_STACK_ypaworld *yw, TInputState *inpt, TBriefengScreen *brf)
{
    int elmID = (brf->CurrTime - brf->StartTime) / 2500;

    if ( elmID >= brf->ElementsCount )
    {
        brf->Stage = TBriefengScreen::STAGE_KEYS_END;
    }
    else if ( elmID != brf->ActiveElementID )
    {
        brf->ActiveElementID = elmID;
        int ksID = 0; /* Current key sector */

        for ( const TMapGate &gate : yw->_levelInfo.Gates )
        {
            if ( gate.MbStatus != 1 )
            {
                for (const TMapKeySector &ks : gate.KeySectors)
                {
                    if ( elmID == ksID)
                    {
                        vec2d ps = World::SectorIDToCenterPos2( ks.CellId );

                        uint8_t v12 = yw->_lvlTypeMap(ks.CellId);

                        yw->BriefingSetObject( TBriefObject( TBriefObject::TYPE_SECTOR, v12, ps.x, ps.y, 26, 146, 25, 
                                                            yw->GetLocaleString(157, "KEY SECTOR")),
                                               brf->AddObjectsFlag);
                    }

                    ksID++;
                }
            }
        }
    }
}

void ypaworld_func158__sub4__sub1__sub4__sub10(NC_STACK_ypaworld *yw, TInputState *inpt, TBriefengScreen *brf)
{
    brf->ViewingObject = TBriefObject();

    brf->ActiveElementID = -1;
    brf->ElementsCount = 0;
    brf->StartTime = brf->CurrTime;
    brf->ObjDescription = yw->GetLocaleString(152, "TECHNOLOGY UPGRADES");

    for ( const TMapGem &gem : yw->_techUpgrades)
    {
        if ( gem.MbStatus != 1 )
            brf->ElementsCount++;
    }

    brf->Stage = (brf->ElementsCount == 0) + TBriefengScreen::STAGE_TECH_RN;
}

void ypaworld_func158__sub4__sub1__sub4__sub11(NC_STACK_ypaworld *yw, TInputState *inpt, TBriefengScreen *brf)
{
    int elmID = (brf->CurrTime - brf->StartTime) / 2500;
    if ( elmID >= brf->ElementsCount )
    {
        brf->Stage = TBriefengScreen::STAGE_TECH_END;
    }
    else if ( elmID != brf->ActiveElementID )
    {
        brf->ActiveElementID = elmID;

        const TMapGem * pGem = &yw->_techUpgrades[0];

        for (const TMapGem &gem : yw->_techUpgrades)
        {
            pGem = &gem;

            if (gem.MbStatus != 1)
            {
                if (elmID == 0)
                    break;

                elmID--;
            }
        }

        vec2d ps = World::SectorIDToCenterPos2( pGem->CellId );
        
        int v13 = yw->_buildProtos[pGem->BuildingID].SecType;

        yw->BriefingSetObject( TBriefObject( TBriefObject::TYPE_SECTOR, v13, ps.x, ps.y, 26, 144, 25, 
                                            yw->GetLocaleString(158, "TECH UPGRADE") ),  
                               brf->AddObjectsFlag);
    }
}

void ypaworld_func158__sub4__sub1__sub4__sub12(NC_STACK_ypaworld *yw, TInputState *inpt, TBriefengScreen *brf)
{
    brf->ViewingObject = TBriefObject();

    brf->ActiveElementID = -1;
    brf->StartTime = brf->CurrTime;

    brf->ObjDescription = yw->GetLocaleString(153, "ENEMY DEFENSE STATIONS");
    brf->ElementsCount = 0;

    for (size_t i = 1; i < brf->Desc.Robos.size(); i++)
    {
        if (brf->Desc.Robos[i].MbStatus != 1)
            brf->ElementsCount++;
    }

    brf->Stage = TBriefengScreen::STAGE_ENMHS_RN + (brf->ElementsCount == 0);
}

void ypaworld_func158__sub4__sub1__sub4__sub13(NC_STACK_ypaworld *yw, TInputState *inpt, TBriefengScreen *brf)
{
    int v5 = (brf->CurrTime - brf->StartTime) / 2500;

    if ( v5 >= brf->ElementsCount )
    {
        brf->Stage = TBriefengScreen::STAGE_ENMHS_END;
    }
    else if ( v5 != brf->ActiveElementID )
    {
        int v6 = v5;
        brf->ActiveElementID = v5;

        MapRobo *robo = &brf->Desc.Robos[1];

        for (size_t i = 1; i < brf->Desc.Robos.size(); i++)
        {
            robo = &brf->Desc.Robos[i];

            if (robo->MbStatus != 1)
            {
                if (v6 == 0)
                    break;

                v6--;
            }
        }

        yw->BriefingSetObject( TBriefObject( TBriefObject::TYPE_VEHICLE, robo->VhclID, robo->Pos.x, robo->Pos.z, 26, 128 + robo->Owner, 25, 
                                            yw->GetLocaleString(robo->VhclID + 1200, yw->_vhclProtos[ robo->VhclID ].name) ),  
                               brf->AddObjectsFlag);
    }
}

void ypaworld_func158__sub4__sub1__sub4__sub14(NC_STACK_ypaworld *yw, TInputState *inpt, TBriefengScreen *brf)
{
    brf->ViewingObject = TBriefObject();

    brf->ActiveElementID = -1;
    brf->ElementsCount = 0;
    brf->StartTime = brf->CurrTime;

    brf->ObjDescription = yw->GetLocaleString(154, "ENEMY FORCES");

    for ( const MapSquad &squad : brf->Desc.Squads )
    {
        if ( squad.Owner != brf->Desc.PlayerOwner && squad.MbStatus != 1)
            brf->ElementsCount++;
    }

    brf->Stage = (brf->ElementsCount == 0) + TBriefengScreen::STAGE_ENMFRC_RN;
}

void ypaworld_func158__sub4__sub1__sub4__sub15(NC_STACK_ypaworld *yw, TInputState *inpt, TBriefengScreen *brf)
{
    int v5 = (brf->CurrTime - brf->StartTime) / 2500;
    if ( v5 >= brf->ElementsCount )
    {
        brf->Stage = TBriefengScreen::STAGE_ENMFRC_END;
    }
    else if ( v5 != brf->ActiveElementID )
    {
        brf->ActiveElementID = v5;
        int v6 = v5;

        const MapSquad *squad = NULL;

        for ( const MapSquad &it : brf->Desc.Squads )
        {
            squad = &it;

            if ( it.Owner != brf->Desc.PlayerOwner && it.MbStatus != 1)
            {
                if ( v6 == 0 )
                    break;
                v6--;
            }
        }

        if ( squad )
        {
            std::string nm = fmt::sprintf("%d %s", squad->Count, yw->GetLocaleString(squad->VhclID + 1200, yw->_vhclProtos[squad->VhclID].name));

            yw->BriefingSetObject( TBriefObject( TBriefObject::TYPE_VEHICLE, squad->VhclID, squad->X, squad->Z, 26, 136 + squad->Owner, 36, 
                                                nm ),  
                                   brf->AddObjectsFlag);
        }
    }
}

void ypaworld_func158__sub4__sub1__sub4__sub16(NC_STACK_ypaworld *yw, TInputState *inpt, TBriefengScreen *brf)
{
    brf->ViewingObject = TBriefObject();
    
    brf->ActiveElementID = -1;
    brf->ElementsCount = 0;
    brf->Stage = TBriefengScreen::STAGE_BUDDY_RN;
    brf->ObjDescription = "FRIENDLY SUPPORT FORCES";

    brf->StartTime = brf->CurrTime;

    for ( const MapSquad &it : brf->Desc.Squads )
    {
        if ( it.Owner == brf->Desc.PlayerOwner && it.MbStatus != 1)
            brf->ElementsCount++;
    }

    brf->Stage = (brf->ElementsCount == 0) + TBriefengScreen::STAGE_BUDDY_RN;
}

void ypaworld_func158__sub4__sub1__sub4__sub17(NC_STACK_ypaworld *yw, TInputState *inpt, TBriefengScreen *brf)
{
    int v5 = (brf->CurrTime - brf->StartTime) / 2500;
    if ( v5 >= brf->ElementsCount )
    {
        brf->Stage = TBriefengScreen::STAGE_BUDDY_END;
    }
    else if ( v5 != brf->ActiveElementID )
    {
        int v6 = v5;
        brf->ActiveElementID = v5;

        const MapSquad *squad = NULL;

        for ( const MapSquad &it : brf->Desc.Squads )
        {
            squad = &it;

            if ( it.Owner == brf->Desc.PlayerOwner && it.MbStatus != 1)
            {
                if ( v6 == 0 )
                    break;

                v6--;
            }
        }

        if ( squad )
        {
            std::string nm = fmt::sprintf("%d %s", squad->Count, yw->GetLocaleString(squad->VhclID + 1200, yw->_vhclProtos[squad->VhclID].name));

            yw->BriefingSetObject( TBriefObject( TBriefObject::TYPE_VEHICLE, squad->VhclID, squad->X, squad->Z, 26, 136 + squad->Owner, 25, 
                                                nm ),  
                                   brf->AddObjectsFlag);
        }
    }
}

void ypaworld_func158__sub4__sub1__sub4__sub18(NC_STACK_ypaworld *yw, TInputState *inpt, TBriefengScreen *brf)
{
    brf->ViewingObject = TBriefObject();

    brf->ActiveElementID = -1;
    brf->ElementsCount = 0;

    brf->Stage = TBriefengScreen::STAGE_GATE_RN;
    brf->StartTime = brf->CurrTime;

    brf->ObjDescription = yw->GetLocaleString(156, "TRANSPORTER GATES");

    for ( const TMapGate &gate : yw->_levelInfo.Gates )
    {
        if (gate.MbStatus != 1)
            brf->ElementsCount++;
    }

    brf->Stage = (brf->ElementsCount == 0) + TBriefengScreen::STAGE_GATE_RN;
}

void ypaworld_func158__sub4__sub1__sub4__sub19(NC_STACK_ypaworld *yw, TInputState *inpt, TBriefengScreen *brf)
{
    int elmID = (brf->CurrTime - brf->StartTime) / 2500;

    if ( elmID >= brf->ElementsCount )
    {
        brf->Stage = TBriefengScreen::STAGE_GATE_END;
    }
    else if ( elmID != brf->ActiveElementID )
    {
        brf->ActiveElementID = elmID;

        const TMapGate *pGate = &yw->_levelInfo.Gates[0];

        for ( const TMapGate &gate : yw->_levelInfo.Gates )
        {
            pGate = &gate;

            if (gate.MbStatus != 1)
            {
                if ( elmID == 0 )
                    break;

                elmID--;
            }
        }
        
        vec2d ps = World::SectorIDToCenterPos2( pGate->CellId );

        int v13 = yw->_buildProtos[ pGate->ClosedBldID ].SecType;

        yw->BriefingSetObject( TBriefObject( TBriefObject::TYPE_SECTOR, v13, ps.x, ps.y, 26, 145, 25, 
                                            yw->GetLocaleString(159, "BEAM GATE") ),  
                               brf->AddObjectsFlag);
    }
}

void ypaworld_func158__sub4__sub1__sub4__sub21(NC_STACK_ypaworld *yw, TInputState *inpt, TBriefengScreen *brf)
{
    int v20 = -1;

    if ( !inpt->ClickInf.selected_btn )
    {
        float mx = (float)inpt->ClickInf.move.ScreenPos.x / (float)yw->_screenSize.x;
        float my = (float)inpt->ClickInf.move.ScreenPos.y / (float)yw->_screenSize.y;

        for (size_t i = 0; i < brf->Objects.size(); i++)
        {
            TBriefObject &obj = brf->Objects[i];

            float v13 = obj.Pos.x / yw->_mapLength.x * brf->MapBlitEnd.Width() + brf->MapBlitEnd.left;
            float v15 = (v13 + 1.0) * 0.5;

            float v14 = -obj.Pos.y / yw->_mapLength.y * brf->MapBlitEnd.Height() + brf->MapBlitEnd.top;
            float v23 = 0.5 * (v14 + 1.0);

            if ( v15 + -0.025 < mx && v15 + 0.025 > mx && v23 + -0.025 < my && v23 + 0.025 > my )
                v20 = i;
        }

        if ( v20 == -1 || v20 == brf->SelectedObjID )
        {
            if ( v20 != brf->SelectedObjID && brf->SelectedObjID != -1 )
            {
                brf->ViewingObject = TBriefObject();
                brf->SelectedObjID = -1;
            }
        }
        else
        {
            brf->SelectedObjID = v20;
            yw->BriefingSetObject( brf->Objects[v20], false);
        }
    }
}

void sub_4ED434(NC_STACK_ypaworld *yw, TBriefengScreen *brf)
{
    char *v3 = stru_5C91D0.itemBlock;

    if ( brf->TextTime > 0 )
    {
        int numLines = 1;
		size_t pos = brf->BriefingText.find_first_of("\n\r");
        while( pos != std::string::npos )
        {
        	pos = brf->BriefingText.find_first_of("\n\r", pos + 1);
            numLines++;
        }

        int v34 = brf->CurrTime - brf->TextTime;
        int v33 = 100 * v34;

        stru_5C91D0.numEntries = numLines;

        v3 = stru_5C91D0.ItemsPreLayout(yw, v3, 16, "   ");

        int v37 = 0;

        pos = 0;

        while ( 1 )
        {
            bool endline = false;

            size_t stpos = pos;
            pos = brf->BriefingText.find_first_of("\n\r", stpos);

            std::string line;

            if (pos == std::string::npos)
            {
                endline = true;
                line = brf->BriefingText.substr(stpos);
            }
            else
            {
                line = brf->BriefingText.substr(stpos, pos - stpos);
                pos++;
            }

            int v12 = 0;

            if ( v34 >= 500 )
            {
                v12 = 100;
            }
            else
            {
                v12 = v33 / 500;

                if ( v12 < 0 )
                {
                    v12 = 0;
                }
                else if ( v12 > 100 )
                {
                    v12 = 100;
                }
            }

            v33 -= 50000;
            v34 -= 500;

            if ( stru_5C91D0.firstShownEntries <= v37 && stru_5C91D0.firstShownEntries + stru_5C91D0.shownEntries > v37	&& v12 > 0 )
            {
                FontUA::set_txtColor(&v3, yw->_iniColors[63].r, yw->_iniColors[63].g, yw->_iniColors[63].b);

                v3 = FontUA::TextRelWidthItem(yw->_guiTiles[16], v3, line.c_str(), v12, 4);

                FontUA::next_line(&v3);
            }

            v37++;

            if ( endline )
            {
                v3 = stru_5C91D0.ItemsPostLayout(yw, v3, 16, "   ");
                break;
            }
        }
    }
    else
    {
        int v20 = brf->CurrTime - brf->PreTextTime;

        const std::string v21 = yw->GetLocaleString(2467, "LOADING MISSION OBJECTIVES...");

        int v24;

        if ( v20 >= 600 )
            v24 = 100;
        else
        {
            v24 = 100 * v20 / 600;

            if ( v24 < 0 )
                v24 = 0;
            else if ( v24 > 100 )
                v24 = 100;
        }

        if ( v24 > 0 )
        {
            v3 = stru_5C91D0.ItemsPreLayout(yw, v3, 16, "   ");

            FontUA::set_txtColor(&v3, yw->_iniColors[63].r, yw->_iniColors[63].g, yw->_iniColors[63].b);

            v3 = FontUA::TextRelWidthItem(yw->_guiTiles[16], v3, v21, v24, 4);

            v3 = stru_5C91D0.ItemsPostLayout(yw, v3, 16, "   ");
        }
    }

    FontUA::set_end(&v3);
}

void ypaworld_func158__sub4__sub1__sub4__sub1(NC_STACK_ypaworld *yw, TBriefengScreen *brf)
{
    char v27[1024];
    char *pos = v27;

    int v39 = -1;

    for ( TBriefObject &obj : brf->Objects)
    {
        int v29 = 1;

        if ( brf->ViewingObject && brf->ViewingObject == obj )
        {
            if ( (brf->CurrTime / 200) & 1 )
            {
                v29 = 0;
            }
        }

        if ( v29 )
        {
            if ( v39 != obj.TileSet)
            {
                v39 = obj.TileSet;
                FontUA::select_tileset(&pos, obj.TileSet);
            }

            int v38 = yw->_guiTiles[(int)obj.TileSet]->h / 2;

            float v15 = brf->MapBlitEnd.Width() * (obj.Pos.x / yw->_mapLength.x) + brf->MapBlitEnd.left;
            float v14 = brf->MapBlitEnd.Height() * (-obj.Pos.y / yw->_mapLength.y) + brf->MapBlitEnd.top;

            FontUA::set_center_xpos(&pos, (v15 * (yw->_screenSize.x / 2)) - v38);
            FontUA::set_center_ypos(&pos, (v14 * (yw->_screenSize.y / 2)) - v38);

            FontUA::store_u8(&pos, obj.TileID);
        }
    }

    FontUA::set_end(&pos);

    w3d_a209 v28;

    v28.cmdbuf = v27;
    v28.includ = NULL;

    GFX::Engine.raster_func209(&v28);
}

void ypaworld_func158__sub4__sub1__sub4__sub0(NC_STACK_ypaworld *yw)
{
    int w = yw->_screenSize.x / 2;
    int h = yw->_screenSize.y / 2;

    int v34 = w * 0.03125;
    int v35 = w * -0.934375;
    int v37 = h * -0.9333333333333333;

    char buf[260];

    char *pos = buf;

    FontUA::select_tileset(&pos, 16);
    FontUA::set_center_xpos(&pos, v35);
    FontUA::set_center_ypos(&pos, 4 + v37);
    FontUA::set_txtColor(&pos, yw->_iniColors[66].r, yw->_iniColors[66].g, yw->_iniColors[66].b);

    const std::string v7 = yw->GetLocaleString(yw->_levelInfo.LevelID + 1800, yw->_levelInfo.MapName);

    pos = FontUA::FormateCenteredSkipableItem(yw->_guiTiles[16], pos, v7, v34 - v35);
    FontUA::set_end(&pos);

    w3d_a209 v32;
    v32.cmdbuf = buf;
    v32.includ = NULL;

    GFX::Engine.raster_func209(&v32);
}

void ypaworld_func158__sub4__sub1__sub4__sub2(NC_STACK_ypaworld *yw, TBriefengScreen *brf, TInputState *inpt, int obj_id, char a4)
{
    if ( brf->ViewingObject.ObjType )
    {
        int v8 = brf->CurrTime - brf->ViewingObjectStartTime;
        float v26 = -brf->ViewingObject.Pos.y / yw->_mapLength.y;
        float v27 = brf->ViewingObject.Pos.x / yw->_mapLength.x;

        if ( v8 > 50 )
        {
            if ( a4 & 1 )
            {
                Common::FLine v20;
                Common::FLine v21;

                v21.x1 = brf->MapBlitEnd.Width() * v27 + brf->MapBlitEnd.left;
                v21.x2 = v21.x1;
                v21.y1 = brf->MapBlitEnd.Height() * v26 + brf->MapBlitEnd.top;
                v21.y2 = brf->ViewingObjectRect.bottom;

                v20.y1 = v21.y2;
                v20.y2 = v21.y2;

                if ( obj_id >= 3 )
                {
                    v20.x1 = v21.x1;
                    v20.x2 = brf->ViewingObjectRect.right;
                }
                else
                {
                    v20.x1 = brf->ViewingObjectRect.left;
                    v20.x2 = v21.x1;
                }

                GFX::Engine.raster_func217(GFX::Engine.Color(0, 160, 160));
                GFX::Engine.raster_func198(v20);

                GFX::Engine.raster_func217(GFX::Engine.Color(0, 64, 64));
                GFX::Engine.raster_func198(v21);
            }
        }

        if ( v8 > 50 && !brf->ViewingObject.Title.empty() )
        {
            if ( a4 & 2 )
            {
                int v11 = v8 - 50;
                int v13;

                if ( v11 >= 500 )
                    v13 = 100;
                else
                    v13 = 100 * v11 / 500;

                int xpos = ((brf->ViewingObjectRect.left + brf->ViewingObjectRect.right) / 2.0) * (yw->_screenSize.x / 2);
                int ypos = ((yw->_screenSize.y / 2) * brf->ViewingObjectRect.bottom - yw->_guiTiles[16]->h + -1.0);

                char cmdbuf[128];
                char *pos = cmdbuf;

                FontUA::select_tileset(&pos, 16);
                FontUA::set_center_xpos(&pos, xpos);
                FontUA::set_center_ypos(&pos, ypos);
                FontUA::set_txtColor(&pos, yw->_iniColors[66].r, yw->_iniColors[66].g, yw->_iniColors[66].b);

                pos = FontUA::TextRelWidthItem(yw->_guiTiles[16], pos, brf->ViewingObject.Title.c_str(), v13, 16);

                FontUA::set_end(&pos);

                w3d_a209 v23;
                v23.cmdbuf = cmdbuf;
                v23.includ = NULL;

                GFX::Engine.raster_func209(&v23);
            }
        }
    }
}

void ypaworld_func158__sub4__sub1__sub4(NC_STACK_ypaworld *yw, UserData *usr, TInputState *inpt)
{
    TBriefengScreen *brf = &yw->_briefScreen;

    if ( yw->_briefScreen.Stage == TBriefengScreen::STAGE_MOVIE )
    {
        yw->sub_4491A0(yw->_briefScreen.MovieStr);
        brf->Stage = TBriefengScreen::STAGE_LOADED;
    }
    else
    {

        switch ( yw->_briefScreen.TimerStatus )
        {
        case TBriefengScreen::TIMER_NORMAL:
            yw->_briefScreen.CurrTime += inpt->Period;
            break;

        case TBriefengScreen::TIMER_STOP:
            inpt->Period = 1;
            yw->_briefScreen.CurrTime += inpt->Period;
            break;

        case TBriefengScreen::TIMER_FAST:
            yw->_briefScreen.CurrTime += inpt->Period;

            if ( yw->_briefScreen.Stage == 30 )
            {
                yw->_briefScreen.Stage = 31;
                yw->_briefScreen.TimerStatus = TBriefengScreen::TIMER_NORMAL;
            }
            else if ( yw->_briefScreen.ActiveElementID == -1 )
            {
                yw->_briefScreen.TimerStatus = TBriefengScreen::TIMER_NORMAL;
            }
            else
            {
                switch (yw->_briefScreen.Stage)
                {
                case TBriefengScreen::STAGE_PLAYER_RN:
                case TBriefengScreen::STAGE_KEYS_RN:
                case TBriefengScreen::STAGE_TECH_RN:
                case TBriefengScreen::STAGE_ENMHS_RN:
                case TBriefengScreen::STAGE_ENMFRC_RN:
                case TBriefengScreen::STAGE_BUDDY_RN:
                case TBriefengScreen::STAGE_GATE_RN:
                    yw->_briefScreen.CurrTime = 2500 * (yw->_briefScreen.ActiveElementID + 1) + yw->_briefScreen.StartTime;
                    break;
                default:
                    break;
                }

                yw->_briefScreen.TimerStatus = TBriefengScreen::TIMER_NORMAL;
            }
            break;

        case TBriefengScreen::TIMER_RESTART:
            yw->_briefScreen.TimerStatus = TBriefengScreen::TIMER_NORMAL;
            yw->_briefScreen.Stage = 29;
            break;

        default:
            break;
        }

        switch ( yw->_briefScreen.Stage )
        {
        case TBriefengScreen::STAGE_LOADED:
            ypaworld_func158__sub4__sub1__sub4__sub3(yw, inpt, brf);
            break;
        case TBriefengScreen::STAGE_SCALING:
            ypaworld_func158__sub4__sub1__sub4__sub4(yw, inpt, brf);
            break;
        case TBriefengScreen::STAGE_SCALEEND:
            ypaworld_func158__sub4__sub1__sub4__sub5(yw, inpt, brf);
            break;
        case TBriefengScreen::STAGE_PLAYER_ST:
            ypaworld_func158__sub4__sub1__sub4__sub6(yw, inpt, brf);
            break;
        case TBriefengScreen::STAGE_PLAYER_RN:
            if ( yw->_briefScreen.SelectedObjID == -1 )
                ypaworld_func158__sub4__sub1__sub4__sub7(yw, inpt, brf);
            break;
        case TBriefengScreen::STAGE_PLAYER_END:
            yw->_briefScreen.Stage = TBriefengScreen::STAGE_GATE_ST;
            break;
        case TBriefengScreen::STAGE_KEYS_ST:
            yw_BriefSetupKeySectors(yw, inpt, brf);
            break;
        case TBriefengScreen::STAGE_KEYS_RN:
            if ( yw->_briefScreen.SelectedObjID == -1 )
                yw_BriefUpdateKeySectors(yw, inpt, brf);
            break;
        case TBriefengScreen::STAGE_KEYS_END:
            yw->_briefScreen.Stage = TBriefengScreen::STAGE_TECH_ST;
            break;
        case TBriefengScreen::STAGE_TECH_ST:
            ypaworld_func158__sub4__sub1__sub4__sub10(yw, inpt, brf);
            break;
        case TBriefengScreen::STAGE_TECH_RN:
            if ( yw->_briefScreen.SelectedObjID == -1 )
                ypaworld_func158__sub4__sub1__sub4__sub11(yw, inpt, brf);
            break;
        case TBriefengScreen::STAGE_TECH_END:
            yw->_briefScreen.Stage = TBriefengScreen::STAGE_ENMHS_ST;
            break;
        case TBriefengScreen::STAGE_ENMHS_ST:
            ypaworld_func158__sub4__sub1__sub4__sub12(yw, inpt, brf);
            break;
        case TBriefengScreen::STAGE_ENMHS_RN:
            if ( yw->_briefScreen.SelectedObjID == -1 )
                ypaworld_func158__sub4__sub1__sub4__sub13(yw, inpt, brf);
            break;
        case TBriefengScreen::STAGE_ENMHS_END:
            yw->_briefScreen.Stage = TBriefengScreen::STAGE_ENMFRC_ST;
            break;
        case TBriefengScreen::STAGE_ENMFRC_ST:
            ypaworld_func158__sub4__sub1__sub4__sub14(yw, inpt, brf);
            break;
        case TBriefengScreen::STAGE_ENMFRC_RN:
            if ( yw->_briefScreen.SelectedObjID == -1 )
                ypaworld_func158__sub4__sub1__sub4__sub15(yw, inpt, brf);
            break;
        case TBriefengScreen::STAGE_ENMFRC_END:
            yw->_briefScreen.Stage = TBriefengScreen::STAGE_BUDDY_ST;
            break;
        case TBriefengScreen::STAGE_BUDDY_ST:
            ypaworld_func158__sub4__sub1__sub4__sub16(yw, inpt, brf);
            break;
        case TBriefengScreen::STAGE_BUDDY_RN:
            if ( yw->_briefScreen.SelectedObjID == -1 )
                ypaworld_func158__sub4__sub1__sub4__sub17(yw, inpt, brf);
            break;
        case TBriefengScreen::STAGE_BUDDY_END:
            yw->_briefScreen.AddObjectsFlag = false;
            yw->_briefScreen.Stage = TBriefengScreen::STAGE_PLAYER_ST;
            break;
        case TBriefengScreen::STAGE_GATE_ST:
            ypaworld_func158__sub4__sub1__sub4__sub18(yw, inpt, brf);
            break;
        case TBriefengScreen::STAGE_GATE_RN:
            if ( yw->_briefScreen.SelectedObjID == -1 )
                ypaworld_func158__sub4__sub1__sub4__sub19(yw, inpt, brf);
            break;
        case TBriefengScreen::STAGE_GATE_END:
            yw->_briefScreen.Stage = TBriefengScreen::STAGE_KEYS_ST;
            break;
        default:
            break;
        }

        stru_5C91D0.InputHandle(yw, inpt);
        stru_5C91D0.Formate(yw);

        sub_4ED434(yw, brf);

        ypaworld_func158__sub4__sub1__sub4__sub21(yw, inpt, brf);

        if ( brf->BriefingMapImg )
        {
            GFX::rstr_arg204 a4;

            a4.pbitm = brf->BriefingMapImg->GetBitmap();

            a4.float4 = Common::FRect(-1.0, -1.0, 1.0, 1.0);
            a4.float14 = Common::FRect(-1.0, -1.0, 1.0, 1.0);

            GFX::Engine.raster_func204(&a4);
        }

        GFX::Engine.raster_func204(&brf->MapBlitParams);

        GFX::Engine.draw2DandFlush();

        ypaworld_func158__DrawVehicle(yw, brf, inpt);

        ypaworld_func158__sub4__sub1__sub4__sub1(yw, brf);
        ypaworld_func158__sub4__sub1__sub4__sub0(yw);

        w3d_a209 v10;
        v10 = stru_5C91D0.cmdstrm;

        GFX::Engine.raster_func209(&v10);

        ypaworld_func158__sub4__sub1__sub4__sub2(yw, brf, inpt, 0, 2);
    }
}




// Debrif


void sub_449310(Common::FRect *rect)
{
    if ( rect->left > 1.0 )
        rect->left = 1.0;
    else if ( rect->left < -1.0 )
        rect->left = -1.0;

    if ( rect->top > 1.0 )
        rect->top = 1.0;
    else if ( rect->top < -1.0 )
        rect->top = -1.0;

    if ( rect->right > 1.0 )
        rect->right = 1.0;
    else if ( rect->right < -1.0 )
        rect->right = -1.0;

    if ( rect->bottom > 1.0 )
        rect->bottom = 1.0;
    else if ( rect->bottom < -1.0 )
        rect->bottom = -1.0;
}

void ypaworld_func158__sub4__sub1__sub6__sub0(NC_STACK_ypaworld *yw, TInputState *struc, TBriefengScreen *brf)
{
    brf->StartTime = brf->CurrTime;
    brf->Stage = TBriefengScreen::STAGE_SCALING;

    brf->MapBlitParams.pbitm = brf->MbmapImg->GetBitmap();

    float v21, v22;

    if ( yw->_mapSize.x > yw->_mapSize.y )
    {
        v21 = 1.0;
        v22 = (float)yw->_mapSize.y / (float)yw->_mapSize.x;
    }
    else if ( yw->_mapSize.y > yw->_mapSize.x )
    {
        v22 = 1.0;
        v21 = (float)yw->_mapSize.x / (float)yw->_mapSize.y;
    }
    else
    {
        v21 = 1.0;
        v22 = 1.0;
    }

    float v19 = 1.0 / (float)yw->_mapSize.y;
    float v20 = 1.0 / (float)yw->_mapSize.x;

    Common::Point tmp;
    if ( brf->ZoomFromGate )
    {
        tmp = yw->_levelInfo.Gates[ yw->_levelInfo.GateCompleteID ].CellId;
    }
    else
    {
        tmp = {yw->_mapSize.x / 2, yw->_mapSize.y / 2};
    }

    float v11 = 2.0 * ((float)tmp.y / (float)yw->_mapSize.y) - 1.0;
    float v12 = 2.0 * ((float)tmp.x / (float)yw->_mapSize.x) - 1.0;

    brf->MapBlitStart.left = v12 - v20;
    brf->MapBlitStart.top = v11 - v19;
    brf->MapBlitStart.right = v20 + v12;
    brf->MapBlitStart.bottom = v11 + v19;
    sub_449310(&brf->MapBlitStart);

    brf->MapBlitEnd.left = -0.4515625 - v21 * 0.4828125;
    brf->MapBlitEnd.top = -0.325 - v22 * 0.6083333333333334;
    brf->MapBlitEnd.right = brf->MapBlitEnd.left + v21 * 0.965625;
    brf->MapBlitEnd.bottom = brf->MapBlitEnd.top + v22 * 1.216666666666667;
    sub_449310(&brf->MapBlitEnd);

    brf->MapBlitParams.float4 = brf->MapBlitStart;
    brf->MapBlitParams.float14 = brf->MapBlitEnd;

    if ( yw->_GameShell )
        SFXEngine::SFXe.startSound(&yw->_GameShell->samples1_info, 11);
}

void ypaworld_func158__sub4__sub1__sub6__sub1(NC_STACK_ypaworld *yw, TInputState *struc, TBriefengScreen *brf)
{
    int v4 = brf->CurrTime - brf->StartTime;

    if ( v4 >= 600 )
    {
        brf->MapBlitParams.float4 = Common::FRect(-1.0, -1.0, 1.0, 1.0);
        brf->MapBlitParams.float14 = brf->MapBlitEnd;
        brf->Stage = TBriefengScreen::STAGE_SCALEEND;
    }
    else
    {
        float v3 = v4 / 600.0;
        brf->MapBlitParams.float4.left = brf->MapBlitStart.left + (-1.0 - brf->MapBlitStart.left) * v3;
        brf->MapBlitParams.float4.top = brf->MapBlitStart.top + (-1.0 - brf->MapBlitStart.top) * v3;
        brf->MapBlitParams.float4.right = brf->MapBlitStart.right + (1.0 - brf->MapBlitStart.right) * v3;
        brf->MapBlitParams.float4.bottom = brf->MapBlitStart.bottom + (1.0 - brf->MapBlitStart.bottom) * v3;
        brf->MapBlitParams.float14 = brf->MapBlitEnd;
    }
}

void ypaworld_func158__sub4__sub1__sub6__sub2(NC_STACK_ypaworld *yw, TInputState *struc, TBriefengScreen *brf)
{
    brf->StartTime = brf->CurrTime;
    brf->Stage = TBriefengScreen::STAGE_PLAYER_RN;
    brf->LastFrameTimeStamp = 0;

    brf->OwnMap = yw->_lvlPrimevalOwnMap;
    brf->TypMap = yw->_lvlPrimevalTypeMap;

    for (int i = 0; i < 8; i++)
        brf->StatsGlobal[i] = yw->_playersStats[i];

    brf->StatsIngame.fill(World::TPlayerStatus());
    
    brf->Upgrades.clear();
}

void yw_DebriefConqSector(NC_STACK_ypaworld *yw, TBriefengScreen *brf, World::History::Conq *arg, uint32_t time, uint32_t stime)
{
    uint32_t dtime = time - stime;

    brf->OwnMap(arg->secX, arg->secY) = arg->owner;

    if ( stime == brf->LastFrameTimeStamp )
    {
        arg->AddScore(&brf->StatsIngame);

        if ( yw->_GameShell )
            SFXEngine::SFXe.startSound(&yw->_GameShell->samples1_info, 12);
    }

    if ( dtime < 30000 )
    {
        if ( arg->owner )
        {
            if ( brf->VectorGfx[3] )
            {
                float v20 = brf->MapBlitEnd.Width() / (float)yw->_mapSize.x;
                float v21 = brf->MapBlitEnd.Height() / (float)yw->_mapSize.y;

                float a3a = (float)arg->secX * v20 + brf->MapBlitEnd.left + v20 * 0.5;
                float a4a = (float)arg->secY * v21 + brf->MapBlitEnd.top + v21 * 0.5;

                float v19 = 1.0 - (float )dtime / 30000.0;

                if ( v19 > 1.0 )
                    v19 = 1.0;
                else if ( v19 < 0.0)
                    v19 = 0.0;

                float a9 = v20 * v19;
                float a10 = v21 * v19;

                yw_RenderVector2D(yw, brf->VectorGfx[3]->GetSkelet(), a3a, a4a, 1.0, 0.0, 0.0, 1.0, a9, a10,  yw->GetColor(arg->owner), NULL, NULL);
            }
        }
    }
}

void yw_DebriefVhclKill(NC_STACK_ypaworld *yw, TBriefengScreen *brf, World::History::VhclKill *arg, uint32_t time, uint32_t stime)
{
    int own = arg->owners & 7;
    uint32_t dtime = time - stime;

    if ( stime == brf->LastFrameTimeStamp )
    {
        arg->AddScore(&brf->StatsIngame);

        if ( yw->_GameShell )
            SFXEngine::SFXe.startSound(&yw->_GameShell->samples1_info, 13);
    }

    if ( dtime < 120000 )
    {
        if ( brf->VectorGfx[1] )
        {
            float v12 = brf->MapBlitEnd.Width();
            float v13 = brf->MapBlitEnd.Height();

            float a3a = v12 * (arg->posX / 256.0) + brf->MapBlitEnd.left;
            float a4a = v13 * (arg->posY / 256.0) + brf->MapBlitEnd.top;

            float v25;

            if ( dtime >= 30000 )
            {
                v25 = 0.1;
            }
            else
            {
                v25 = 1.0 - dtime / 30000.0;

                if ( v25 < 0.1 )
                    v25 = 0.1;
                else if ( v25 > 1.0 )
                    v25 = 1.0;
            }

            float a9 = ((v12 / (float)yw->_mapSize.x) / 2.0) * v25;
            float a10 = ((v13 / (float)yw->_mapSize.y) / 2.0) * v25;

            yw_RenderVector2D(yw, brf->VectorGfx[1]->GetSkelet(), a3a, a4a, 1.0, 0.0, 0.0, 1.0, a9, a10, yw->GetColor(own), NULL, NULL);
        }
    }
}

void yw_DebriefVhclCreate(NC_STACK_ypaworld *yw, TBriefengScreen *brf, World::History::VhclCreate *arg, int time, int stime)
{
    int dtime = time - stime;
    if ( dtime < 45000 )
    {
        if ( brf->VectorGfx[1] )
        {
            float v13 = brf->MapBlitEnd.Width();
            float v14 = brf->MapBlitEnd.Height();

            float a3a = v13 * (arg->posX / 256.0) + brf->MapBlitEnd.left;
            float a4a = v14 * (arg->posY / 256.0) + brf->MapBlitEnd.top;

            float v22 = dtime / 45000.0;

            if ( v22 > 1.0 )
                v22 = 1.0;
            else if ( v22 < 0.0 )
                v22 = 0.0;

            float a9 = ((v13 / (float)yw->_mapSize.x) / 8.0) * v22;
            float a10 = ((v14 / (float)yw->_mapSize.y) / 8.0) * v22;

            yw_RenderVector2D(yw, brf->VectorGfx[1]->GetSkelet(), a3a, a4a, 1.0, 0.0, 0.0, 1.0, a9, a10, yw->GetColor(arg->owner), NULL, NULL);
        }
    }
}

void yw_DebriefAddTechUpgrade(NC_STACK_ypaworld *yw, TBriefengScreen *brf, World::History::Upgrade *arg)
{
    if ( !yw->_gameWasNetGame || arg->owner == yw->_userOwnerIdWasInNetGame )
    {
        for (auto &u : brf->Upgrades)
        {
            if ( u.upgradeType == arg->upgradeType && u.lastVhcl == arg->lastVhcl && u.lastWeapon == arg->lastWeapon && u.lastBuild == arg->lastBuild)
                return;
        }

        //CHECKME if it needed to limit
        /*if ( brf->tp1_count >= 7 ) 
            brf->tp1_count = 6;*/
        
        brf->Upgrades.push_back(*arg);
    }
}

void yw_DebriefRenderSectorsOwners(NC_STACK_ypaworld *yw, TBriefengScreen *brf)
{
    float v3 = brf->MapBlitEnd.Width() / (float)yw->_mapSize.x;
    float v4 = brf->MapBlitEnd.Height() / (float)yw->_mapSize.y;

    float v19 = v3 / 10.0;
    float v16 = v4 / 10.0;

    float v21 = brf->MapBlitEnd.top + v4 * 0.5;

    for (int yy = 0; yy < yw->_mapSize.y; yy++)
    {
        float v23 = brf->MapBlitEnd.left + v3 * 0.5;
        
        uint8_t *ownmap = brf->OwnMap.Line(yy);

        for (int xx = 0; xx < yw->_mapSize.x; xx++)
        {
            int owner = *ownmap;

            if ( owner != 0 )
            {
                if ( yw->IsGamePlaySector( {xx, yy} ) )
                {
                    Common::FLine arg198;
                    Common::FLine arg198_1;

                    arg198.x1 = v23 - v19;
                    arg198.y1 = v21;
                    arg198.x2 = v19 + v23;
                    arg198.y2 = v21;

                    arg198_1.x1 = v23;
                    arg198_1.y1 = v21 - v16;
                    arg198_1.x2 = v23;
                    arg198_1.y2 = v21 + v16;

                    GFX::Engine.raster_func217( yw->GetColor(owner) );
                    GFX::Engine.raster_func198(arg198);
                    GFX::Engine.raster_func198(arg198_1);
                }
            }

            v23 += v3;
            ownmap++;
        }

        v21 += v4;
    }
}


void ypaworld_func158__sub4__sub1__sub6__sub3__sub6(NC_STACK_ypaworld *yw, TBriefengScreen *brf)
{
    int v14 = (yw->_screenSize.x / 2) * -0.934375;
    int v13 = (yw->_screenSize.x / 2) * 0.03125;
    int v16 = (yw->_screenSize.y / 2) * -0.9333333333333333;

    const std::string v7 = yw->GetLocaleString(yw->_levelInfo.LevelID + 1800, yw->_levelInfo.MapName);

    char cmdBuff[264];
    char *cur = cmdBuff;

    FontUA::select_tileset(&cur, 16);
    FontUA::set_center_xpos(&cur, v14);
    FontUA::set_center_ypos(&cur, v16);

    FontUA::set_txtColor(&cur, yw->_iniColors[66].r, yw->_iniColors[66].g, yw->_iniColors[66].b);

    cur = FontUA::FormateCenteredSkipableItem(yw->_guiTiles[16], cur, v7, v13 - v14);

    FontUA::set_end(&cur);

    w3d_a209 arg209;
    arg209.cmdbuf = cmdBuff;
    arg209.includ = 0;

    GFX::Engine.raster_func209(&arg209);
}

char * yw_DebriefKillsTitleLine(NC_STACK_ypaworld *yw, TBriefengScreen *brf, char *in, int a4)
{
    char *cur = in;

    FontUA::set_txtColor(&cur, yw->_iniColors[67].r, yw->_iniColors[67].g, yw->_iniColors[67].b);
    FontUA::ColumnItem elms[3];

    elms[0].txt = yw->GetLocaleString(2450, "KILLS");
    elms[0].spaceChar = 32;
    elms[0].prefixChar = 0;
    elms[0].postfixChar = 0;
    elms[0].width = a4 * 0.4;
    elms[0].flags = 36;
    elms[0].fontID = 15;

    elms[1].txt = yw->GetLocaleString(2451, "BY PLAYER");
    elms[1].fontID = 15;
    elms[1].width = a4 * 0.3;
    elms[1].spaceChar = 32;
    elms[1].flags = 36;
    elms[1].prefixChar = 0;
    elms[1].postfixChar = 0;

    elms[2].txt = yw->GetLocaleString(2452, "ALL");
    elms[2].width = a4 * 0.3;
    elms[2].fontID = 15;
    elms[2].spaceChar = 32;
    elms[2].flags = 36;
    elms[2].prefixChar = 0;
    elms[2].postfixChar = 0;

    return FormateColumnItem(yw, cur, 3, elms);
}

int sub_4EF2A8(const void *a1, const void *a2)
{
    return ((debrif_t1 *)a2)->status - ((debrif_t1 *)a1)->status;
}

char * yw_DebriefKillsScore(NC_STACK_ypaworld *yw, TBriefengScreen *brf, char *in, int a4)
{
    char *cur = in;
    int a2 = 0;

    debrif_t1 v28[8];

    for (int i = 0; i < 8; i++)
    {
        if ( (1 << i) & yw->_levelInfo.OwnerMask )
        {
            v28[a2].owner = i;
            v28[a2].status = brf->StatsIngame[i].DestroyedUnits;
            a2++;
        }
    }

    qsort(v28, a2, sizeof(debrif_t1), sub_4EF2A8);


    for (int i = 0; i < a2; i++)
    {
        int clr_id;
        std::string who;

        switch ( v28[i].owner )
        {
        case 1:
            who = yw->GetLocaleString(2411, "RESISTANCE");
            clr_id = 1;
            break;

        case 2:
            who = yw->GetLocaleString(2412, "SULGOGARS");
            clr_id = 2;
            break;

        case 3:
            who = yw->GetLocaleString(2413, "MYKONIANS");
            clr_id = 3;
            break;

        case 4:
            who = yw->GetLocaleString(2414, "TAERKASTEN");
            clr_id = 4;
            break;

        case 5:
            who = yw->GetLocaleString(2415, "BLACK SECT");
            clr_id = 5;
            break;

        case 6:
            who = yw->GetLocaleString(2416, "GHORKOV");
            clr_id = 6;
            break;

        default:
            who = yw->GetLocaleString(2417, "NEUTRAL");
            clr_id = 7;
            break;
        }

        FontUA::ColumnItem elms[3];

        FontUA::set_txtColor(&cur, yw->_iniColors[clr_id].r, yw->_iniColors[clr_id].g, yw->_iniColors[clr_id].b);

        elms[0].txt = who;
        elms[0].spaceChar = 32;
        elms[0].fontID = 15;
        elms[0].width = a4 * 0.4;
        elms[0].prefixChar = 0;
        elms[0].postfixChar = 0;
        elms[0].flags = 36;

        if ( yw->_gameWasNetGame || v28[i].owner == yw->_playerOwner )
            elms[1].txt = fmt::sprintf("%d", brf->StatsIngame[ v28[i].owner ].DestroyedByUser);
        else
            elms[1].txt = "-";
        elms[1].spaceChar = 32;
        elms[1].fontID = 15;
        elms[1].prefixChar = 0;
        elms[1].postfixChar = 0;
        elms[1].flags = 36;
        elms[1].width = a4 * 0.3;

        elms[2].txt = fmt::sprintf("%d", brf->StatsIngame[ v28[i].owner ].DestroyedUnits);
        elms[2].width = a4 * 0.3;
        elms[2].fontID = 15;
        elms[2].spaceChar = 32;
        elms[2].flags = 36;
        elms[2].prefixChar = 0;
        elms[2].postfixChar = 0;

        cur = FormateColumnItem(yw, cur, 3, elms);

        FontUA::next_line(&cur);
    }

    return cur;
}

char * yw_DebriefMPlayScoreTitle(NC_STACK_ypaworld *yw, char *in, int a4)
{
    char *cur = in;

    FontUA::set_txtColor(&cur, yw->_iniColors[67].r, yw->_iniColors[67].g, yw->_iniColors[67].b);

    FontUA::ColumnItem elm;

    elm.txt = yw->GetLocaleString(2453, "SCORE");
    elm.width = a4;
    elm.spaceChar = 32;
    elm.fontID = 15;
    elm.prefixChar = 0;
    elm.postfixChar = 0;
    elm.flags = 36;

    return FormateColumnItem(yw, cur, 1, &elm);
}

char *yw_DebriefMPlayScore(NC_STACK_ypaworld *yw, TBriefengScreen *brf, char *in, int a4)
{
    char *cur = in;

    if ( yw->_gameWasNetGame )
    {
        cur = yw_DebriefMPlayScoreTitle(yw, cur, a4);

        FontUA::next_line(&cur);

        debrif_t1 v32[8];
        int a2 = 0;

        for (int i = 0; i < 8; i++)
        {
            if ( (1 << i) & yw->_levelInfo.OwnerMask )
            {
                v32[a2].owner = i;
                v32[a2].status = brf->StatsIngame[i].Score;
                a2++;
            }
        }

        qsort(v32, a2, sizeof(debrif_t1), sub_4EF2A8);

        for (int i = 0; i < a2; i++)
        {
            int clr_id;
            std::string who;

            switch ( v32[i].owner )
            {
            case 1:
                who = yw->GetLocaleString(2411, "RESISTANCE");
                clr_id = 1;
                break;

            case 2:
                who = yw->GetLocaleString(2412, "SULGOGARS");
                clr_id = 2;
                break;

            case 3:
                who = yw->GetLocaleString(2413, "MYKONIANS");
                clr_id = 3;
                break;

            case 4:
                who = yw->GetLocaleString(2414, "TAERKASTEN");
                clr_id = 4;
                break;

            case 5:
                who = yw->GetLocaleString(2415, "BLACK SECT");
                clr_id = 5;
                break;

            case 6:
                who = yw->GetLocaleString(2416, "GHORKOV");
                clr_id = 6;
                break;

            default:
                who = yw->GetLocaleString(2417, "NEUTRAL");
                clr_id = 7;
                break;
            }

            FontUA::set_txtColor(&cur, yw->_iniColors[clr_id].r, yw->_iniColors[clr_id].g, yw->_iniColors[clr_id].b);

            FontUA::ColumnItem a4a[2];

            a4a[0].txt = who;
            a4a[0].spaceChar = 32;
            a4a[0].fontID = 15;
            a4a[0].prefixChar = 0;
            a4a[0].postfixChar = 0;
            a4a[0].flags = 36;
            a4a[0].width = a4 * 0.5;

            a4a[1].txt = fmt::sprintf("%d", brf->StatsIngame[ v32[i].owner ].Score);
            a4a[1].width = a4 * 0.5;
            a4a[1].fontID = 15;
            a4a[1].spaceChar = 32;
            a4a[1].flags = 36;
            a4a[1].prefixChar = 0;
            a4a[1].postfixChar = 0;

            cur = FormateColumnItem(yw, cur, 2, a4a);

            FontUA::next_line(&cur);
        }
    }
    else
    {
        FontUA::set_txtColor(&cur, yw->_iniColors[67].r, yw->_iniColors[67].g, yw->_iniColors[67].b);

        FontUA::ColumnItem v35[2];

        v35[0].txt = yw->GetLocaleString(2457, "SCORE THIS MISSION:");
        v35[0].spaceChar = 32;
        v35[0].prefixChar = 0;
        v35[0].postfixChar = 0;
        v35[0].width = a4 * 0.7;
        v35[0].fontID = 15;
        v35[0].flags = 36;

        v35[1].txt = fmt::sprintf("%d", brf->StatsIngame[yw->_playerOwner].Score);
        v35[1].fontID = 15;
        v35[1].spaceChar = 32;
        v35[1].prefixChar = 0;
        v35[1].width = a4 * 0.3;
        v35[1].postfixChar = 0;
        v35[1].flags = 36;

        cur = FormateColumnItem(yw, cur, 2, v35);

        FontUA::next_line(&cur);

        v35[0].txt = yw->GetLocaleString(2458, "SCORE OVERALL:");
        v35[0].flags = 36;
        v35[0].spaceChar = 32;
        v35[0].width = a4 * 0.7;
        v35[0].prefixChar = 0;
        v35[0].postfixChar = 0;
        v35[0].fontID = 15;

        

        v35[1].txt = fmt::sprintf("%d", brf->StatsIngame[yw->_playerOwner].Score + brf->StatsGlobal[yw->_playerOwner].Score);
        v35[1].fontID = 15;
        v35[1].spaceChar = 32;
        v35[1].postfixChar = 0;
        v35[1].width = a4 * 0.3;
        v35[1].prefixChar = 0;
        v35[1].flags = 36;

        cur = FormateColumnItem(yw, cur, 2, v35);

        FontUA::next_line(&cur);
    }
    return cur;
}

char * yw_DebriefRenderTime(NC_STACK_ypaworld *yw, TBriefengScreen *brf, char *in, int a4)
{
    char *cur = in;
    FontUA::set_txtColor(&cur, yw->_iniColors[67].r, yw->_iniColors[67].g, yw->_iniColors[67].b);

    if ( yw->_gameWasNetGame )
    {
        FontUA::ColumnItem v30[2];
        v30[0].txt = yw->GetLocaleString(2456, "PLAYING TIME:");
        v30[0].spaceChar = 32;
        v30[0].prefixChar = 0;
        v30[0].postfixChar = 0;
        v30[0].width = a4 * 0.7;
        v30[0].flags = 36;
        v30[0].fontID = 15;

        int v12 = brf->LastFrameTimeStamp / 1024;

        v30[1].txt = fmt::sprintf("%02d:%02d:%02d", v12 / 60 / 60, v12 / 60 % 60, v12 % 60);
        v30[1].fontID = 15;
        v30[1].spaceChar = 32;
        v30[1].prefixChar = 0;
        v30[1].postfixChar = 0;
        v30[1].flags = 36;
        v30[1].width = a4 * 0.3;

        cur = FormateColumnItem(yw, cur, 2, v30);
    }
    else
    {

        FontUA::ColumnItem a4a[2];
        a4a[0].txt = yw->GetLocaleString(2454, "PLAYING TIME THIS MISSION:");
        a4a[0].spaceChar = 32;
        a4a[0].prefixChar = 0;
        a4a[0].postfixChar = 0;
        a4a[0].width = a4 * 0.7;
        a4a[0].flags = 36;
        a4a[0].fontID = 15;

        int v19 = brf->LastFrameTimeStamp / 1024;

        a4a[1].txt = fmt::sprintf("%02d:%02d:%02d", v19 / 60 / 60, v19 / 60 % 60, v19 % 60);
        a4a[1].width = a4 * 0.3;
        a4a[1].fontID = 15;
        a4a[1].spaceChar = 32;
        a4a[1].prefixChar = 0;
        a4a[1].postfixChar = 0;
        a4a[1].flags = 36;

        cur = FormateColumnItem(yw, cur, 2, a4a);

        FontUA::next_line(&cur);

        a4a[0].txt = yw->GetLocaleString(2455, "PLAYING TIME OVERALL:");
        a4a[0].flags = 36;
        a4a[0].spaceChar = 32;
        a4a[0].width = a4 * 0.7;
        a4a[0].prefixChar = 0;
        a4a[0].postfixChar = 0;
        a4a[0].fontID = 15;

        v19 = (brf->LastFrameTimeStamp + brf->StatsGlobal[yw->_playerOwner].ElapsedTime) / 1024;

        a4a[1].txt = fmt::sprintf("%02d:%02d:%02d", v19 / 60 / 60, v19 / 60 % 60, v19 % 60);
        a4a[1].fontID = 15;
        a4a[1].spaceChar = 32;
        a4a[1].width = a4 * 0.3;
        a4a[1].prefixChar = 0;
        a4a[1].postfixChar = 0;
        a4a[1].flags = 36;

        cur = FormateColumnItem(yw, cur, 2, a4a);
    }

    FontUA::next_line(&cur);

    return cur;
}

char * yw_DebriefScoreTable(NC_STACK_ypaworld *yw, TBriefengScreen *brf, char *in)
{
    char *cur = in;

    FontUA::select_tileset(&cur, 15);
    FontUA::set_center_xpos(&cur, ((yw->_screenSize.x / 2) * 0.15) );
    FontUA::set_center_ypos(&cur, ((yw->_screenSize.y / 2) * -0.821) );

    int v14 = (yw->_screenSize.x / 2) * 0.796875;

    cur = yw_DebriefKillsTitleLine(yw, brf, cur, v14);

    FontUA::next_line(&cur);

    cur = yw_DebriefKillsScore(yw, brf, cur, v14);

    FontUA::next_line(&cur);

    cur = yw_DebriefMPlayScore(yw, brf, cur, v14);

    FontUA::next_line(&cur);

    return yw_DebriefRenderTime(yw, brf, cur, v14);
}

char * yw_DebriefTechUpgradeLine(NC_STACK_ypaworld *yw, TBriefengScreen *brf, const World::History::Upgrade &upg, char *in, int a5)
{
    char *cur = in;

    int lastWeapon = upg.lastWeapon;
    int lastVhcl   = upg.lastVhcl;
    int lastBuild  = upg.lastBuild;

    if ( lastVhcl == 0 && lastWeapon != 0 )
    {
        int i = 0;
        for (const World::TVhclProto &vhcl : yw->_vhclProtos)
        {
            if (vhcl.weapon == lastWeapon)
            {
                lastVhcl = i;
                break;
            }
            
            i++;
        }

        if ( !lastVhcl )
            return cur;
    }

    if ( lastWeapon == 0 && lastVhcl != 0 )
    {
        lastWeapon = yw->_vhclProtos[ lastVhcl ].weapon;

        if ( lastWeapon == -1 )
            lastWeapon = 0;
    }

    World::TVhclProto *vhcl = NULL;
    World::TWeapProto *wpn  = NULL;
    World::TBuildingProto *bld = NULL;

    if ( lastVhcl && lastVhcl < yw->_vhclProtos.size() )
        vhcl = &yw->_vhclProtos.at(lastVhcl);

    if ( lastBuild && lastBuild < yw->_buildProtos.size() )
        bld = &yw->_buildProtos.at(lastBuild);

    if ( lastWeapon && lastWeapon < yw->_weaponProtos.size() )
        wpn = &yw->_weaponProtos.at(lastWeapon);

    std::string v13 = " ";
    std::string v14 = " ";
    std::string v33 = " ";

    if ( vhcl )
    {
        v33 = yw->GetLocaleString(lastVhcl + 1200, vhcl->name);
    }
    else if ( bld )
    {
        if ( yw->_gameWasNetGame )
            v33 = yw->GetLocaleString(lastBuild + 1700, bld->Name);
        else
            v33 = yw->GetLocaleString(lastBuild + 1500, bld->Name);
    }

    switch ( upg.upgradeType )
    {
    case World::UPGRADE_WEAPON:
        if ( wpn )
        {
            if ( vhcl )
            {
                v13 = yw->GetLocaleString(2459, "WEAPON UPGRADE:");

                if ( vhcl->num_weapons > 1 )
                    v14 = fmt::sprintf("(%d x%d)", wpn->energy / 100, vhcl->num_weapons);
                else
                    v14 = fmt::sprintf("(%d)", wpn->energy / 100);
            }
        }
        break;

    case World::UPGRADE_ARMOR:
        if ( vhcl )
        {
            v13 = yw->GetLocaleString(2460, "ARMOR UPGRADE:");

            v14 = fmt::sprintf("(%d%%)", vhcl->shield);
        }
        break;

    case World::UPGRADE_VEHICLE:
        if ( vhcl )
        {
            v13 = yw->GetLocaleString(2461, "NEW VEHICLE TECH:");
        }
        break;

    case World::UPGRADE_BUILDING:
        if ( bld )
        {
            v13 = yw->GetLocaleString(2462, "NEW BUILDING TECH:");
        }
        break;

    case World::UPGRADE_RADARE:
        if ( vhcl )
        {
            v13 = yw->GetLocaleString(2463, "RADAR UPGRADE:");
        }
        break;

    case World::UPGRADE_BLDVHCL:
        if ( vhcl )
        {
            if ( bld )
            {
                if ( yw->_gameWasNetGame )
                    v14 = yw->GetLocaleString(lastBuild + 1700, bld->Name);
                else
                    v14 = yw->GetLocaleString(lastBuild + 1500, bld->Name);

                v13 = yw->GetLocaleString(2464, "COMBINED UPGRADE:");
            }
        }
        break;

    case World::UPGRADE_GENERIC:
        v13 = yw->GetLocaleString(2465, "GENERIC TECH UPGRADE");
        break;

    default:
        break;
    }

    FontUA::ColumnItem elm[3];

    elm[0].txt = v13;
    elm[0].spaceChar = 32;
    elm[0].flags = 36;
    elm[0].prefixChar = 0;
    elm[0].postfixChar = 0;
    elm[0].width = a5 * 0.48;
    elm[0].fontID = 15;

    elm[1].flags = 36;
    elm[1].fontID = 15;
    elm[1].spaceChar = 32;
    elm[1].prefixChar = 0;
    elm[1].postfixChar = 0;
    elm[1].txt = v33;
    elm[1].width = a5 * 0.3;

    elm[2].flags = 36;
    elm[2].fontID = 15;
    elm[2].spaceChar = 32;
    elm[2].prefixChar = 0;
    elm[2].postfixChar = 0;
    elm[2].width = a5 * 0.22;
    elm[2].txt = v14;

    cur = FontUA::FormateColumnItem(yw, cur, 3, elm);

    FontUA::next_line(&cur);

    return cur;
}

char * yw_DebriefTechUpgradesTable(NC_STACK_ypaworld *yw, TBriefengScreen *brf, char *in)
{
    char *cur = in;

    FontUA::select_tileset(&cur, 15);
    FontUA::set_center_xpos(&cur, ((yw->_screenSize.x / 2) * -0.9875) );
    FontUA::set_center_ypos(&cur, ((yw->_screenSize.y / 2) * 0.35) );

    FontUA::set_txtColor(&cur, yw->_iniColors[67].r, yw->_iniColors[67].g, yw->_iniColors[67].b);

    for ( const auto &upg : brf->Upgrades )
        cur = yw_DebriefTechUpgradeLine(yw, brf, upg, cur,  (yw->_screenSize.x / 2) * 0.984375 );

    return cur;
}

void yw_DebriefRunDebrief(NC_STACK_ypaworld *yw, TInputState *struc, TBriefengScreen *brf)
{
    char cmdbuf[2048];
    char *cur = cmdbuf;

    uint32_t dtime = brf->CurrTime - brf->StartTime;

    yw_DebriefRenderSectorsOwners(yw, brf);

    cur = yw_DebriefScoreTable(yw, brf, cur);
    cur = yw_DebriefTechUpgradesTable(yw, brf, cur);

    FontUA::set_end(&cur);

    uint32_t lastFrameTimeStamp = 0;

    w3d_a209 v24;
    v24.includ = 0;
    v24.cmdbuf = cmdbuf;

    GFX::Engine.raster_func209(&v24);

    int v26 = 0;
    
    

    if ( brf->Stage == TBriefengScreen::STAGE_PLAYER_RN )
    {
        bool readLoop = true;
        auto reader = yw->_history.GetReader();
        
        World::History::Instance HistDecoders;

        while (readLoop && !reader.Eof())
        {
            World::History::Record *decoder = HistDecoders[ reader.ReadU8() ];
            if (decoder)
            {
                World::History::FillDecoderBStrm(decoder, &reader);
                
                switch(decoder->type)
                {
                case World::History::TYPE_FRAME:
                {
                    World::History::Frame *frm = static_cast<World::History::Frame *>(decoder);
                    
                    lastFrameTimeStamp = frm->TimeStamp;

                    if ( lastFrameTimeStamp >= brf->LastFrameTimeStamp )
                        brf->LastFrameTimeStamp = lastFrameTimeStamp;

                    if ( lastFrameTimeStamp >= dtime )
                    {
                        v26 = 1;
                        readLoop = false;
                    }
                }
                break;

                case World::History::TYPE_CONQ:
                    yw_DebriefConqSector(yw, brf, static_cast<World::History::Conq *>(decoder), dtime, lastFrameTimeStamp);
                    break;

                case World::History::TYPE_VHCLKILL:
                    yw_DebriefVhclKill(yw, brf, static_cast<World::History::VhclKill *>(decoder), dtime, lastFrameTimeStamp);
                    break;

                case World::History::TYPE_VHCLCREATE:
                    yw_DebriefVhclCreate(yw, brf, static_cast<World::History::VhclCreate *>(decoder), dtime, lastFrameTimeStamp);
                    break;

                case World::History::TYPE_POWERST:
                    if ( lastFrameTimeStamp == brf->LastFrameTimeStamp )
                    {
                        decoder->AddScore(&brf->StatsIngame);

                        if ( yw->_GameShell )
                            SFXEngine::SFXe.startSound(&yw->_GameShell->samples1_info, 14);
                    }
                    break;

                case World::History::TYPE_UPGRADE:
                    if ( lastFrameTimeStamp == brf->LastFrameTimeStamp )
                    {
                        decoder->AddScore(&brf->StatsIngame);
                        yw_DebriefAddTechUpgrade(yw, brf, static_cast<World::History::Upgrade *>(decoder));

                        if ( yw->_GameShell )
                            SFXEngine::SFXe.startSound(&yw->_GameShell->samples1_info, 14);

                    }
                    break;

                default:
                    readLoop = false;
                    break;
                }
            }
            else
            {
                readLoop = false;
            }
        }

        if ( !v26 )
        {
            brf->Stage = TBriefengScreen::STAGE_PLAYER_END;
            if ( yw->_gameWasNetGame )
            {
                brf->StatsIngame = yw->_gameplayStats;
            }
        }
    }

    ypaworld_func158__sub4__sub1__sub6__sub3__sub6(yw, brf);
}

void yw_debriefUpdate(NC_STACK_ypaworld *yw, TInputState *inpt)
{
    TBriefengScreen *brf = &yw->_briefScreen;

    if ( yw->_history.Size() ) //FIXME
    {
        if ( yw->_briefScreen.TimerStatus == TBriefengScreen::TIMER_NORMAL )
        {
            if ( brf->Stage == TBriefengScreen::STAGE_PLAYER_RN )
            {
                brf->CurrTime += 60 * inpt->Period;
            }
            else if ( brf->Stage != TBriefengScreen::STAGE_PLAYER_END )
            {
                brf->CurrTime += inpt->Period;
            }
        }
        else if ( yw->_briefScreen.TimerStatus == TBriefengScreen::TIMER_STOP )
        {
            inpt->Period = 1;
        }
        else if ( yw->_briefScreen.TimerStatus == TBriefengScreen::TIMER_RESTART )
        {
            brf->TimerStatus = TBriefengScreen::TIMER_NORMAL;
            brf->Stage = TBriefengScreen::STAGE_PLAYER_ST;
        }

        if ( brf->Stage != TBriefengScreen::STAGE_LOADED )
        {
            if ( brf->BriefingMapImg )
            {
                GFX::rstr_arg204 arg204;
                arg204.pbitm = brf->BriefingMapImg->GetBitmap();

                arg204.float4 = Common::FRect(-1.0, -1.0, 1.0, 1.0);
                arg204.float14 = Common::FRect(-1.0, -1.0, 1.0, 1.0);

                GFX::Engine.raster_func204(&arg204);
            }

            GFX::Engine.raster_func204(&brf->MapBlitParams);
        }

        switch ( brf->Stage )
        {
        case TBriefengScreen::STAGE_LOADED:
            ypaworld_func158__sub4__sub1__sub6__sub0(yw, inpt, brf);
            break;

        case TBriefengScreen::STAGE_SCALING:
            ypaworld_func158__sub4__sub1__sub6__sub1(yw, inpt, brf);
            break;

        case TBriefengScreen::STAGE_SCALEEND:
            brf->Stage = TBriefengScreen::STAGE_PLAYER_ST;
            break;

        case TBriefengScreen::STAGE_PLAYER_ST:
            ypaworld_func158__sub4__sub1__sub6__sub2(yw, inpt, brf);
            break;

        case TBriefengScreen::STAGE_PLAYER_RN:
        case TBriefengScreen::STAGE_PLAYER_END:
            yw_DebriefRunDebrief(yw, inpt, brf);
            break;

        default:
            break;
        }
    }
    else
    {
        yw->_briefScreen.Stage = TBriefengScreen::STAGE_CANCEL;
    }
}
