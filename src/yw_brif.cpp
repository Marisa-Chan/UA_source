#include <inttypes.h>
#include <string.h>

#include "includes.h"
#include "yw.h"
#include "yw_internal.h"
#include "lstvw.h"
#include "font.h"

extern GuiList stru_5C91D0;

void ypaworld_func158__sub4__sub1__sub4__sub3(NC_STACK_ypaworld *yw, InputState *inpt, BriefengScreen *brf)
{
    SFXEngine::SFXe.StopMusicTrack();
    if ( yw->GameShell->missiontrack )
    {
        SFXEngine::SFXe.SetMusicTrack(yw->GameShell->missiontrack, yw->GameShell->missiontrack__adv.min_delay, yw->GameShell->missiontrack__adv.max_delay);
        SFXEngine::SFXe.PlayMusicTrack();
    }

    brf->StartTime = brf->CurrTime;
    brf->Stage = 5;

    brf->MapBlitParams.pbitm = brf->MbmapImg->GetResBmp();

    brf->MapBlitStart.x1 = -0.4515625;
    brf->MapBlitStart.y1 = -0.324999988079;
    brf->MapBlitStart.x2 = brf->MapBlitStart.x1;
    brf->MapBlitStart.y2 = brf->MapBlitStart.y1;

    float v17, v16;

    if ( yw->_mapWidth == yw->_mapHeight )
    {
        v17 = 1.0;
        v16 = 1.0;
    }
    else if (yw->_mapWidth < yw->_mapHeight)
    {
        v16 = 1.0;
        v17 = (float)yw->_mapWidth / (float)yw->_mapHeight;
    }
    else if (yw->_mapWidth > yw->_mapHeight)
    {
        v17 = 1.0;
        v16 = (float)yw->_mapHeight / (float)yw->_mapWidth;
    }

    brf->MapBlitParams.float4 = -1.0;
    brf->MapBlitParams.float8 = -1.0;
    brf->MapBlitParams.floatC = 1.0;
    brf->MapBlitParams.float10 = 1.0;

    brf->MapBlitEnd.x1 = -0.4515625 - v17 * 0.4828125;
    brf->MapBlitEnd.y1 = -0.325 - v16 * 0.6083333333333334;
    brf->MapBlitEnd.x2 = brf->MapBlitEnd.x1 + v17 * 0.965625;
    brf->MapBlitEnd.y2 = brf->MapBlitEnd.y1 + v16 * 1.216666666666667;
    brf->MapBlitParams.float14 = brf->MapBlitStart.x1;
    brf->MapBlitParams.float18 = brf->MapBlitStart.y1;
    brf->MapBlitParams.float1C = brf->MapBlitStart.x2;
    brf->MapBlitParams.float20 = brf->MapBlitStart.y2;
    brf->TextTime = 0;
    brf->PreTextTime = brf->CurrTime;
}

void ypaworld_func158__sub4__sub1__sub4__sub4(NC_STACK_ypaworld *yw, InputState *inpt, BriefengScreen *brf)
{
    int v5 = brf->CurrTime - brf->StartTime;

    if ( v5 >= 600 )
    {
        brf->MapBlitParams.float14 = brf->MapBlitEnd.x1;
        brf->MapBlitParams.float18 = brf->MapBlitEnd.y1;
        brf->MapBlitParams.float1C = brf->MapBlitEnd.x2;
        brf->MapBlitParams.float20 = brf->MapBlitEnd.y2;
        brf->Stage = 6;

        displ_arg263 v4;
        v4.bitm = yw->pointers__bitm[5];
        v4.pointer_id = 6;

        yw->_win3d->display_func263(&v4);
    }
    else
    {
        float v3 = v5 * 0.0016666667;
        brf->MapBlitParams.float14 = (brf->MapBlitEnd.x1 - brf->MapBlitStart.x1) * v3 + brf->MapBlitStart.x1;
        brf->MapBlitParams.float18 = (brf->MapBlitEnd.y1 - brf->MapBlitStart.y1) * v3 + brf->MapBlitStart.y1;
        brf->MapBlitParams.float1C = (brf->MapBlitEnd.x2 - brf->MapBlitStart.x2) * v3 + brf->MapBlitStart.x2;
        brf->MapBlitParams.float20 = (brf->MapBlitEnd.y2 - brf->MapBlitStart.y2) * v3 + brf->MapBlitStart.y2;
    }
}

int yw_MBLoadSet(NC_STACK_ypaworld *yw, int setID)
{
    std::string oldRsrc = get_prefix_replacement("rsrc");

    set_prefix_replacement("rsrc", fmt::sprintf("data:set%d:", setID));

    if ( setID != yw->set_number && setID != 46 )
    {
        if ( yw->additionalSet )
        {
            ypa_log_out("yw_MBLoadSet(): killing set object %d\n", yw->set_number);
            delete_class_obj(yw->additionalSet);
            yw->additionalSet = NULL;
            yw->set_number = 0;
        }

        yw->additionalSet = load_set_base();
        if ( !yw->additionalSet )
        {
            ypa_log_out("yw_MBLoadSet(): loading set object %d failed\n", setID);
            set_prefix_replacement("rsrc", oldRsrc);
            return 0;
        }

        yw->set_number = setID;
        ypa_log_out("yw_MBLoadSet(): loaded set object %d ok\n", setID);
    }

    if ( setID != 46 )
    {
        FSMgr::FileHandle *fil = uaOpenFile("rsrc:scripts/set.sdf", "r");
        if ( !fil )
        {
            ypa_log_out("Briefing: no set description file.\n");
            return 0;
        }

        int kid_id = 0;
        BaseList &kids_list = yw->additionalSet->getBASE_kidList();

        for(BaseList::iterator it = kids_list.begin(); it != kids_list.end(); it++)
        {
            if ( kid_id == 0 )
            {
                if ( !sub_44A12C(yw, *it) )
                {
                    delete fil;
                    return 0;
                }
            }
            else if ( kid_id == 1 )
            {
                if ( !yw_parse_lego(yw, fil, *it) )
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
                if ( !sub_44A97C(yw, *it) )
                {
                    delete fil;
                    return 0;
                }
            }

            kid_id++;
        }

        delete fil;
    }

    set_prefix_replacement("rsrc", oldRsrc);

    return 1;
}

void ypaworld_func158__sub4__sub1__sub4__sub5(NC_STACK_ypaworld *yw, InputState *inpt, BriefengScreen *brf)
{
    brf->AddObjectsFlag = true;
    brf->TextTime = brf->CurrTime;
    brf->Objects.clear();

    if ( yw_MBLoadSet(yw, brf->Desc.SetID) )
    {
        brf->Stage = 7;

        displ_arg263 v6;
        v6.bitm = yw->pointers__bitm[0];
        v6.pointer_id = 1;

        yw->_win3d->display_func263(&v6);
    }
    else
    {
        brf->Stage = 1;
    }
}

void ypaworld_func158__sub4__sub1__sub4__sub6(NC_STACK_ypaworld *yw, InputState *inpt, BriefengScreen *brf)
{
    brf->ViewingObject = BriefObject();

    brf->ActiveElementID = -1;
    brf->ElementsCount = 1;

    brf->Stage = 8;
    brf->StartTime = brf->CurrTime;

    brf->SelectedObjID = -1;
    brf->ObjDescription = get_lang_string(yw->string_pointers_p2, 150, "YOU ARE HERE");
}

void NC_STACK_ypaworld::BriefingSetObject(const BriefObject &obj, bool doAdd)
{
    brief.ViewingObjectStartTime = brief.CurrTime;
    
    brief.ViewingObject = obj;

    if ( doAdd )
        brief.Objects.push_back( obj );

    if ( GameShell )
        SFXEngine::SFXe.startSound(&GameShell->samples1_info, 11);
}

void ypaworld_func158__sub4__sub1__sub4__sub7(NC_STACK_ypaworld *yw, InputState *inpt, BriefengScreen *brf)
{
    if ( brf->CurrTime - brf->StartTime >= 2500 )
    {
        brf->Stage = 9;
    }
    else if ( brf->ActiveElementID )
    {
        MapRobo &robo = brf->Desc.Robos[0];
        const char *v6 = get_lang_string(yw->string_pointers_p2, robo.VhclID + 1200, yw->VhclProtos[ robo.VhclID ].name.c_str());

        brf->ActiveElementID = 0;
        yw->BriefingSetObject( BriefObject(BriefObject::TYPE_VEHICLE, robo.VhclID, robo.Pos.x, robo.Pos.z,
                                           26, 128 + robo.Owner, 25,
                                           v6), 
                               brf->AddObjectsFlag);
    }
}

void yw_BriefSetupKeySectors(NC_STACK_ypaworld *yw, InputState *inpt, BriefengScreen *brf)
{
    brf->ViewingObject = BriefObject();

    brf->ActiveElementID = -1;
    brf->ElementsCount = 0;
    brf->StartTime = brf->CurrTime;

    brf->ObjDescription = get_lang_string(yw->string_pointers_p2, 151, "PRIMARY TARGETS");

    for (const MapGate &gate : yw->_levelInfo->Gates )
    {
        if ( gate.MbStatus != 1 )
            brf->ElementsCount += gate.KeySectors.size();
    }

    if ( brf->ElementsCount == 0 )
        brf->Stage = 12;
    else
        brf->Stage = 11;
}

void yw_BriefUpdateKeySectors(NC_STACK_ypaworld *yw, InputState *inpt, BriefengScreen *brf)
{
    int elmID = (brf->CurrTime - brf->StartTime) / 2500;

    if ( elmID >= brf->ElementsCount )
    {
        brf->Stage = 12;
    }
    else if ( elmID != brf->ActiveElementID )
    {
        brf->ActiveElementID = elmID;
        int ksID = 0; /* Current key sector */

        for ( const MapGate &gate : yw->_levelInfo->Gates )
        {
            if ( gate.MbStatus != 1 )
            {
                for (const MapKeySector &ks : gate.KeySectors)
                {
                    if ( elmID == ksID)
                    {
                        float xpos = ks.x * 1200.0 + 600.0;
                        float ypos = -(ks.y * 1200.0 + 600.0);
                        uint8_t v12 = (*yw->typ_map)(ks.x, ks.y);
                        const char *v13 = get_lang_string(yw->string_pointers_p2, 157, "KEY SECTOR");

                        yw->BriefingSetObject( BriefObject( BriefObject::TYPE_SECTOR, v12, xpos, ypos, 26, 146, 25, 
                                                            v13),
                                               brf->AddObjectsFlag);
                    }

                    ksID++;
                }
            }
        }
    }
}

void ypaworld_func158__sub4__sub1__sub4__sub10(NC_STACK_ypaworld *yw, InputState *inpt, BriefengScreen *brf)
{
    brf->ViewingObject = BriefObject();

    brf->ActiveElementID = -1;
    brf->ElementsCount = 0;
    brf->StartTime = brf->CurrTime;
    brf->ObjDescription = get_lang_string(yw->string_pointers_p2, 152, "TECHNOLOGY UPGRADES");

    for ( const MapGem &gem : yw->_Gems)
    {
        if ( gem.MbStatus != 1 )
            brf->ElementsCount++;
    }

    brf->Stage = (brf->ElementsCount == 0) + 14;
}

void ypaworld_func158__sub4__sub1__sub4__sub11(NC_STACK_ypaworld *yw, InputState *inpt, BriefengScreen *brf)
{
    int elmID = (brf->CurrTime - brf->StartTime) / 2500;
    if ( elmID >= brf->ElementsCount )
    {
        brf->Stage = 15;
    }
    else if ( elmID != brf->ActiveElementID )
    {
        brf->ActiveElementID = elmID;

        const MapGem * pGem = &yw->_Gems[0];

        for (const MapGem &gem : yw->_Gems)
        {
            pGem = &gem;

            if (gem.MbStatus != 1)
            {
                if (elmID == 0)
                    break;

                elmID--;
            }
        }

        float xpos = pGem->SecX * 1200.0 + 600.0;
        float ypos = -(pGem->SecY * 1200.0 + 600.0);
        int v13 = yw->BuildProtos[pGem->BuildingID].SecType;
        const char *v14 = get_lang_string(yw->string_pointers_p2, 158, "TECH UPGRADE");

        yw->BriefingSetObject( BriefObject( BriefObject::TYPE_SECTOR, v13, xpos, ypos, 26, 144, 25, 
                                            v14 ),  
                               brf->AddObjectsFlag);
    }
}

void ypaworld_func158__sub4__sub1__sub4__sub12(NC_STACK_ypaworld *yw, InputState *inpt, BriefengScreen *brf)
{
    brf->ViewingObject = BriefObject();

    brf->ActiveElementID = -1;
    brf->StartTime = brf->CurrTime;

    brf->ObjDescription = get_lang_string(yw->string_pointers_p2, 153, "ENEMY DEFENSE STATIONS");
    brf->ElementsCount = 0;

    for (size_t i = 1; i < brf->Desc.Robos.size(); i++)
    {
        if (brf->Desc.Robos[i].MbStatus != 1)
            brf->ElementsCount++;
    }

    brf->Stage = 17 + (brf->ElementsCount == 0);
}

void ypaworld_func158__sub4__sub1__sub4__sub13(NC_STACK_ypaworld *yw, InputState *inpt, BriefengScreen *brf)
{
    int v5 = (brf->CurrTime - brf->StartTime) / 2500;

    if ( v5 >= brf->ElementsCount )
    {
        brf->Stage = 18;
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

        const char *v11 = get_lang_string(yw->string_pointers_p2, robo->VhclID + 1200, yw->VhclProtos[ robo->VhclID ].name.c_str());
        yw->BriefingSetObject( BriefObject( BriefObject::TYPE_VEHICLE, robo->VhclID, robo->Pos.x, robo->Pos.z, 26, 128 + robo->Owner, 25, 
                                            v11 ),  
                               brf->AddObjectsFlag);
    }
}

void ypaworld_func158__sub4__sub1__sub4__sub14(NC_STACK_ypaworld *yw, InputState *inpt, BriefengScreen *brf)
{
    brf->ViewingObject = BriefObject();

    brf->ActiveElementID = -1;
    brf->ElementsCount = 0;
    brf->StartTime = brf->CurrTime;

    brf->ObjDescription = get_lang_string(yw->string_pointers_p2, 154, "ENEMY FORCES");

    for ( const MapSquad &squad : brf->Desc.Squads )
    {
        if ( squad.Owner != brf->Desc.PlayerOwner && squad.MbStatus != 1)
            brf->ElementsCount++;
    }

    brf->Stage = (brf->ElementsCount == 0) + 20;
}

void ypaworld_func158__sub4__sub1__sub4__sub15(NC_STACK_ypaworld *yw, InputState *inpt, BriefengScreen *brf)
{
    int v5 = (brf->CurrTime - brf->StartTime) / 2500;
    if ( v5 >= brf->ElementsCount )
    {
        brf->Stage = 21;
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
            const char *v11 = get_lang_string(yw->string_pointers_p2, squad->VhclID + 1200, yw->VhclProtos[squad->VhclID].name.c_str());

            yw->BriefingSetObject( BriefObject( BriefObject::TYPE_VEHICLE, squad->VhclID, squad->X, squad->Z, 26, 136 + squad->Owner, 36, 
                                                fmt::sprintf("%d %s", squad->Count, v11) ),  
                                   brf->AddObjectsFlag);
        }
    }
}

void ypaworld_func158__sub4__sub1__sub4__sub16(NC_STACK_ypaworld *yw, InputState *inpt, BriefengScreen *brf)
{
    brf->ViewingObject = BriefObject();
    
    brf->ActiveElementID = -1;
    brf->ElementsCount = 0;
    brf->Stage = 23;
    brf->ObjDescription = "FRIENDLY SUPPORT FORCES";

    brf->StartTime = brf->CurrTime;

    for ( const MapSquad &it : brf->Desc.Squads )
    {
        if ( it.Owner == brf->Desc.PlayerOwner && it.MbStatus != 1)
            brf->ElementsCount++;
    }

    brf->Stage = (brf->ElementsCount == 0) + 23;
}

void ypaworld_func158__sub4__sub1__sub4__sub17(NC_STACK_ypaworld *yw, InputState *inpt, BriefengScreen *brf)
{
    int v5 = (brf->CurrTime - brf->StartTime) / 2500;
    if ( v5 >= brf->ElementsCount )
    {
        brf->Stage = 24;
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
            const char *v11 = get_lang_string(yw->string_pointers_p2, squad->VhclID + 1200, yw->VhclProtos[squad->VhclID].name.c_str());

            yw->BriefingSetObject( BriefObject( BriefObject::TYPE_VEHICLE, squad->VhclID, squad->X, squad->Z, 26, 136 + squad->Owner, 25, 
                                                fmt::sprintf("%d %s", squad->Count, v11) ),  
                                   brf->AddObjectsFlag);
        }
    }
}

void ypaworld_func158__sub4__sub1__sub4__sub18(NC_STACK_ypaworld *yw, InputState *inpt, BriefengScreen *brf)
{
    brf->ViewingObject = BriefObject();

    brf->ActiveElementID = -1;
    brf->ElementsCount = 0;

    brf->Stage = 26;
    brf->StartTime = brf->CurrTime;

    brf->ObjDescription = get_lang_string(yw->string_pointers_p2, 156, "TRANSPORTER GATES");

    for ( const MapGate &gate : yw->_levelInfo->Gates )
    {
        if (gate.MbStatus != 1)
            brf->ElementsCount++;
    }

    brf->Stage = (brf->ElementsCount == 0) + 26;
}

void ypaworld_func158__sub4__sub1__sub4__sub19(NC_STACK_ypaworld *yw, InputState *inpt, BriefengScreen *brf)
{
    int elmID = (brf->CurrTime - brf->StartTime) / 2500;

    if ( elmID >= brf->ElementsCount )
    {
        brf->Stage = 27;
    }
    else if ( elmID != brf->ActiveElementID )
    {
        brf->ActiveElementID = elmID;

        const MapGate *pGate = &yw->_levelInfo->Gates[0];

        for ( const MapGate &gate : yw->_levelInfo->Gates )
        {
            pGate = &gate;

            if (gate.MbStatus != 1)
            {
                if ( elmID == 0 )
                    break;

                elmID--;
            }
        }

        float xpos = pGate->SecX * 1200.0 + 600.0;
        float ypos = -(pGate->SecY * 1200.0 + 600.0);
        int v13 = yw->BuildProtos[ pGate->ClosedBldID ].SecType;
        const char *v14 = get_lang_string(yw->string_pointers_p2, 159, "BEAM GATE");

        yw->BriefingSetObject( BriefObject( BriefObject::TYPE_SECTOR, v13, xpos, ypos, 26, 145, 25, 
                                            v14 ),  
                               brf->AddObjectsFlag);
    }
}

void ypaworld_func158__sub4__sub1__sub4__sub21(NC_STACK_ypaworld *yw, InputState *inpt, BriefengScreen *brf)
{
    int v20 = -1;

    if ( !inpt->ClickInf.selected_btn )
    {
        float mx = (float)inpt->ClickInf.move.ScreenPos.x / (float)yw->screen_width;
        float my = (float)inpt->ClickInf.move.ScreenPos.y / (float)yw->screen_height;

        for (size_t i = 0; i < brf->Objects.size(); i++)
        {
            BriefObject &obj = brf->Objects[i];

            float v13 = obj.X / yw->map_Width_meters * (brf->MapBlitEnd.x2 - brf->MapBlitEnd.x1) + brf->MapBlitEnd.x1;
            float v15 = (v13 + 1.0) * 0.5;

            float v14 = -obj.Y / yw->map_Height_meters * (brf->MapBlitEnd.y2 - brf->MapBlitEnd.y1) + brf->MapBlitEnd.y1;
            float v23 = 0.5 * (v14 + 1.0);

            if ( v15 + -0.025 < mx && v15 + 0.025 > mx && v23 + -0.025 < my && v23 + 0.025 > my )
                v20 = i;
        }

        if ( v20 == -1 || v20 == brf->SelectedObjID )
        {
            if ( v20 != brf->SelectedObjID && brf->SelectedObjID != -1 )
            {
                brf->ViewingObject = BriefObject();
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

void sub_4ED434(NC_STACK_ypaworld *yw, BriefengScreen *brf)
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
                FontUA::set_txtColor(&v3, yw->iniColors[63].r, yw->iniColors[63].g, yw->iniColors[63].b);

                v3 = FontUA::TextRelWidthItem(yw->tiles[16], v3, line.c_str(), v12, 4);

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

        const char *v21 = get_lang_string(yw->string_pointers_p2, 2467, "LOADING MISSION OBJECTIVES...");

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

            FontUA::set_txtColor(&v3, yw->iniColors[63].r, yw->iniColors[63].g, yw->iniColors[63].b);

            v3 = FontUA::TextRelWidthItem(yw->tiles[16], v3, v21, v24, 4);

            v3 = stru_5C91D0.ItemsPostLayout(yw, v3, 16, "   ");
        }
    }

    FontUA::set_end(&v3);
}

void ypaworld_func158__sub4__sub1__sub4__sub1(NC_STACK_ypaworld *yw, BriefengScreen *brf)
{
    char v27[1024];
    char *pos = v27;

    int v39 = -1;

    for ( BriefObject &obj : brf->Objects)
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

            int v38 = yw->tiles[(int)obj.TileSet]->h / 2;

            float v15 = (brf->MapBlitEnd.x2 - brf->MapBlitEnd.x1) * (obj.X / yw->map_Width_meters) + brf->MapBlitEnd.x1;
            float v14 = (brf->MapBlitEnd.y2 - brf->MapBlitEnd.y1) * (-obj.Y / yw->map_Height_meters) + brf->MapBlitEnd.y1;

            FontUA::set_center_xpos(&pos, (v15 * (yw->screen_width / 2)) - v38);
            FontUA::set_center_ypos(&pos, (v14 * (yw->screen_height / 2)) - v38);

            FontUA::store_u8(&pos, obj.TileID);
        }
    }

    FontUA::set_end(&pos);

    w3d_a209 v28;

    v28.cmdbuf = v27;
    v28.includ = NULL;

    yw->_win3d->raster_func209(&v28);
}

void ypaworld_func158__sub4__sub1__sub4__sub0(NC_STACK_ypaworld *yw)
{
    int w = yw->screen_width / 2;
    int h = yw->screen_height / 2;

    int v34 = w * 0.03125;
    int v35 = w * -0.934375;
    int v37 = h * -0.9333333333333333;

    char buf[260];

    char *pos = buf;

    FontUA::select_tileset(&pos, 16);
    FontUA::set_center_xpos(&pos, v35);
    FontUA::set_center_ypos(&pos, 4 + v37);
    FontUA::set_txtColor(&pos, yw->iniColors[66].r, yw->iniColors[66].g, yw->iniColors[66].b);

    const char *v7 = get_lang_string(yw->string_pointers_p2, yw->_levelInfo->LevelID + 1800, yw->_levelInfo->MapName.c_str());

    pos = FontUA::FormateCenteredSkipableItem(yw->tiles[16], pos, v7, v34 - v35);
    FontUA::set_end(&pos);

    w3d_a209 v32;
    v32.cmdbuf = buf;
    v32.includ = NULL;

    yw->_win3d->raster_func209(&v32);
}

void ypaworld_func158__sub4__sub1__sub4__sub2(NC_STACK_ypaworld *yw, BriefengScreen *brf, InputState *inpt, int obj_id, char a4)
{
    if ( brf->ViewingObject.ObjType )
    {
        int v8 = brf->CurrTime - brf->ViewingObjectStartTime;
        float v26 = -brf->ViewingObject.Y / yw->map_Height_meters;
        float v27 = brf->ViewingObject.X / yw->map_Width_meters;

        if ( v8 > 50 )
        {
            if ( a4 & 1 )
            {
                w3d_func198arg v20;
                w3d_func198arg v21;

                v21.x1 = (brf->MapBlitEnd.x2 - brf->MapBlitEnd.x1) * v27 + brf->MapBlitEnd.x1;
                v21.x2 = v21.x1;
                v21.y1 = (brf->MapBlitEnd.y2 - brf->MapBlitEnd.y1) * v26 + brf->MapBlitEnd.y1;
                v21.y2 = brf->ViewingObjectRect.y2;

                v20.y1 = v21.y2;
                v20.y2 = v21.y2;

                if ( obj_id >= 3 )
                {
                    v20.x1 = v21.x1;
                    v20.x2 = brf->ViewingObjectRect.x2;
                }
                else
                {
                    v20.x1 = brf->ViewingObjectRect.x1;
                    v20.x2 = v21.x1;
                }

                yw->_win3d->raster_func217(0xA0A0);
                yw->_win3d->raster_func198(&v20);

                yw->_win3d->raster_func217(0x4040);
                yw->_win3d->raster_func198(&v21);
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

                int xpos = ((brf->ViewingObjectRect.x1 + brf->ViewingObjectRect.x2) / 2.0) * (yw->screen_width / 2);
                int ypos = ((yw->screen_height / 2) * brf->ViewingObjectRect.y2 - yw->tiles[16]->h + -1.0);

                char cmdbuf[128];
                char *pos = cmdbuf;

                FontUA::select_tileset(&pos, 16);
                FontUA::set_center_xpos(&pos, xpos);
                FontUA::set_center_ypos(&pos, ypos);
                FontUA::set_txtColor(&pos, yw->iniColors[66].r, yw->iniColors[66].g, yw->iniColors[66].b);

                pos = FontUA::TextRelWidthItem(yw->tiles[16], pos, brf->ViewingObject.Title.c_str(), v13, 16);

                FontUA::set_end(&pos);

                w3d_a209 v23;
                v23.cmdbuf = cmdbuf;
                v23.includ = NULL;

                yw->_win3d->raster_func209(&v23);
            }
        }
    }
}

void ypaworld_func158__sub4__sub1__sub4(NC_STACK_ypaworld *yw, UserData *usr, InputState *inpt)
{
    BriefengScreen *brf = &yw->brief;

    if ( yw->brief.Stage == 28 )
    {
        yw->sub_4491A0(yw->brief.MovieStr);
        brf->Stage = 4;
    }
    else
    {

        switch ( yw->brief.TimerStatus )
        {
        case 0:
            yw->brief.CurrTime += inpt->Period;
            break;

        case 1:
            inpt->Period = 1;
            yw->brief.CurrTime += inpt->Period;
            break;

        case 2:
            yw->brief.CurrTime += inpt->Period;

            if ( yw->brief.Stage == 30 )
            {
                yw->brief.Stage = 31;
                yw->brief.TimerStatus = 0;
            }
            else if ( yw->brief.ActiveElementID == -1 )
            {
                yw->brief.TimerStatus = 0;
            }
            else
            {
                switch (yw->brief.Stage)
                {
                case 8:
                case 11:
                case 14:
                case 17:
                case 20:
                case 23:
                case 26:
                    yw->brief.CurrTime = 2500 * (yw->brief.ActiveElementID + 1) + yw->brief.StartTime;
                    break;
                default:
                    break;
                }

                yw->brief.TimerStatus = 0;
            }
            break;

        case 3:
            yw->brief.TimerStatus = 0;
            yw->brief.Stage = 29;
            break;

        default:
            break;
        }

        switch ( yw->brief.Stage )
        {
        case 4:
            ypaworld_func158__sub4__sub1__sub4__sub3(yw, inpt, brf);
            break;
        case 5:
            ypaworld_func158__sub4__sub1__sub4__sub4(yw, inpt, brf);
            break;
        case 6:
            ypaworld_func158__sub4__sub1__sub4__sub5(yw, inpt, brf);
            break;
        case 7:
            ypaworld_func158__sub4__sub1__sub4__sub6(yw, inpt, brf);
            break;
        case 8:
            if ( yw->brief.SelectedObjID == -1 )
                ypaworld_func158__sub4__sub1__sub4__sub7(yw, inpt, brf);
            break;
        case 9:
            yw->brief.Stage = 25;
            break;
        case 10:
            yw_BriefSetupKeySectors(yw, inpt, brf);
            break;
        case 11:
            if ( yw->brief.SelectedObjID == -1 )
                yw_BriefUpdateKeySectors(yw, inpt, brf);
            break;
        case 12:
            yw->brief.Stage = 13;
            break;
        case 13:
            ypaworld_func158__sub4__sub1__sub4__sub10(yw, inpt, brf);
            break;
        case 14:
            if ( yw->brief.SelectedObjID == -1 )
                ypaworld_func158__sub4__sub1__sub4__sub11(yw, inpt, brf);
            break;
        case 15:
            yw->brief.Stage = 16;
            break;
        case 16:
            ypaworld_func158__sub4__sub1__sub4__sub12(yw, inpt, brf);
            break;
        case 17:
            if ( yw->brief.SelectedObjID == -1 )
                ypaworld_func158__sub4__sub1__sub4__sub13(yw, inpt, brf);
            break;
        case 18:
            yw->brief.Stage = 19;
            break;
        case 19:
            ypaworld_func158__sub4__sub1__sub4__sub14(yw, inpt, brf);
            break;
        case 20:
            if ( yw->brief.SelectedObjID == -1 )
                ypaworld_func158__sub4__sub1__sub4__sub15(yw, inpt, brf);
            break;
        case 21:
            yw->brief.Stage = 22;
            break;
        case 22:
            ypaworld_func158__sub4__sub1__sub4__sub16(yw, inpt, brf);
            break;
        case 23:
            if ( yw->brief.SelectedObjID == -1 )
                ypaworld_func158__sub4__sub1__sub4__sub17(yw, inpt, brf);
            break;
        case 24:
            yw->brief.AddObjectsFlag = false;
            yw->brief.Stage = 7;
            break;
        case 25:
            ypaworld_func158__sub4__sub1__sub4__sub18(yw, inpt, brf);
            break;
        case 26:
            if ( yw->brief.SelectedObjID == -1 )
                ypaworld_func158__sub4__sub1__sub4__sub19(yw, inpt, brf);
            break;
        case 27:
            yw->brief.Stage = 10;
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
            rstr_arg204 a4;

            a4.pbitm = brf->BriefingMapImg->GetResBmp();

            a4.float4 = -1.0;
            a4.floatC = 1.0;
            a4.float8 = -1.0;
            a4.float10 = 1.0;
            a4.float14 = -1.0;
            a4.float1C = 1.0;
            a4.float18 = -1.0;
            a4.float20 = 1.0;

            yw->_win3d->raster_func204(&a4);
        }

        yw->_win3d->raster_func204(&brf->MapBlitParams);

        yw->_win3d->draw2DandFlush();

        ypaworld_func158__DrawVehicle(yw, brf, inpt);

        ypaworld_func158__sub4__sub1__sub4__sub1(yw, brf);
        ypaworld_func158__sub4__sub1__sub4__sub0(yw);

        w3d_a209 v10;
        v10 = stru_5C91D0.cmdstrm;

        yw->_win3d->raster_func209(&v10);

        ypaworld_func158__sub4__sub1__sub4__sub2(yw, brf, inpt, 0, 2);
    }
}




// Debrif


void sub_449310(ua_fRect *rect)
{
    if ( rect->x1 > 1.0 )
        rect->x1 = 1.0;
    else if ( rect->x1 < -1.0 )
        rect->x1 = -1.0;

    if ( rect->y1 > 1.0 )
        rect->y1 = 1.0;
    else if ( rect->y1 < -1.0 )
        rect->y1 = -1.0;

    if ( rect->x2 > 1.0 )
        rect->x2 = 1.0;
    else if ( rect->x2 < -1.0 )
        rect->x2 = -1.0;

    if ( rect->y2 > 1.0 )
        rect->y2 = 1.0;
    else if ( rect->y2 < -1.0 )
        rect->y2 = -1.0;
}

void ypaworld_func158__sub4__sub1__sub6__sub0(NC_STACK_ypaworld *yw, InputState *struc, BriefengScreen *brf)
{
    brf->StartTime = brf->CurrTime;
    brf->Stage = 5;

    brf->MapBlitParams.pbitm = brf->MbmapImg->GetResBmp();

    float v21, v22;

    if ( yw->_mapWidth > yw->_mapHeight )
    {
        v21 = 1.0;
        v22 = (float)yw->_mapHeight / (float)yw->_mapWidth;
    }
    else if ( yw->_mapHeight > yw->_mapWidth )
    {
        v22 = 1.0;
        v21 = (float)yw->_mapWidth / (float)yw->_mapHeight;
    }
    else
    {
        v21 = 1.0;
        v22 = 1.0;
    }

    float v19 = 1.0 / (float)yw->_mapHeight;
    float v20 = 1.0 / (float)yw->_mapWidth;

    int v8, v9;
    if ( brf->ZoomFromGate )
    {
        v8 = yw->_levelInfo->Gates[ yw->_levelInfo->GateCompleteID ].SecX;
        v9 = yw->_levelInfo->Gates[ yw->_levelInfo->GateCompleteID ].SecY;
    }
    else
    {
        v9 = yw->_mapHeight / 2;
        v8 = yw->_mapWidth / 2;
    }

    float v11 = 2.0 * ((float)v9 / (float)yw->_mapHeight) - 1.0;
    float v12 = 2.0 * ((float)v8 / (float)yw->_mapWidth) - 1.0;

    brf->MapBlitStart.x1 = v12 - v20;
    brf->MapBlitStart.y1 = v11 - v19;
    brf->MapBlitStart.x2 = v20 + v12;
    brf->MapBlitStart.y2 = v11 + v19;
    sub_449310(&brf->MapBlitStart);

    brf->MapBlitEnd.x1 = -0.4515625 - v21 * 0.4828125;
    brf->MapBlitEnd.y1 = -0.325 - v22 * 0.6083333333333334;
    brf->MapBlitEnd.x2 = brf->MapBlitEnd.x1 + v21 * 0.965625;
    brf->MapBlitEnd.y2 = brf->MapBlitEnd.y1 + v22 * 1.216666666666667;
    sub_449310(&brf->MapBlitEnd);

    brf->MapBlitParams.float4 = brf->MapBlitStart.x1;
    brf->MapBlitParams.float8 = brf->MapBlitStart.y1;
    brf->MapBlitParams.floatC = brf->MapBlitStart.x2;
    brf->MapBlitParams.float10 = brf->MapBlitStart.y2;

    brf->MapBlitParams.float14 = brf->MapBlitEnd.x1;
    brf->MapBlitParams.float18 = brf->MapBlitEnd.y1;
    brf->MapBlitParams.float1C = brf->MapBlitEnd.x2;
    brf->MapBlitParams.float20 = brf->MapBlitEnd.y2;

    if ( yw->GameShell )
        SFXEngine::SFXe.startSound(&yw->GameShell->samples1_info, 11);
}

void ypaworld_func158__sub4__sub1__sub6__sub1(NC_STACK_ypaworld *yw, InputState *struc, BriefengScreen *brf)
{
    int v4 = brf->CurrTime - brf->StartTime;

    if ( v4 >= 600 )
    {
        brf->MapBlitParams.float4 = -1.0;
        brf->MapBlitParams.float8 = -1.0;
        brf->MapBlitParams.floatC = 1.0;
        brf->MapBlitParams.float10 = 1.0;

        brf->MapBlitParams.float14 = brf->MapBlitEnd.x1;
        brf->MapBlitParams.float18 = brf->MapBlitEnd.y1;
        brf->MapBlitParams.float1C = brf->MapBlitEnd.x2;
        brf->MapBlitParams.float20 = brf->MapBlitEnd.y2;
        brf->Stage = 6;
    }
    else
    {
        float v3 = v4 / 600.0;
        brf->MapBlitParams.float4 = brf->MapBlitStart.x1 + (-1.0 - brf->MapBlitStart.x1) * v3;
        brf->MapBlitParams.float8 = brf->MapBlitStart.y1 + (-1.0 - brf->MapBlitStart.y1) * v3;
        brf->MapBlitParams.floatC = brf->MapBlitStart.x2 + (1.0 - brf->MapBlitStart.x2) * v3;
        brf->MapBlitParams.float10 = brf->MapBlitStart.y2 + (1.0 - brf->MapBlitStart.y2) * v3;
        brf->MapBlitParams.float14 = brf->MapBlitEnd.x1;
        brf->MapBlitParams.float18 = brf->MapBlitEnd.y1;
        brf->MapBlitParams.float1C = brf->MapBlitEnd.x2;
        brf->MapBlitParams.float20 = brf->MapBlitEnd.y2;
    }
}

void ypaworld_func158__sub4__sub1__sub6__sub2(NC_STACK_ypaworld *yw, InputState *struc, BriefengScreen *brf)
{
    brf->StartTime = brf->CurrTime;
    brf->Stage = 8;
    brf->LastFrameTimeStamp = 0;

    brf->OwnMap = *yw->copyof_ownermap;
    brf->TypMap = *yw->copyof_typemap;

    for (int i = 0; i < 8; i++)
        brf->StatsGlobal[i] = yw->playerstatus[i];

    for (auto &x : brf->StatsIngame)
        x.clear();

    brf->Upgrades.clear();
}

void yw_DebriefConqSector(NC_STACK_ypaworld *yw, BriefengScreen *brf, World::History::Conq *arg, uint32_t time, uint32_t stime)
{
    uint32_t dtime = time - stime;

    brf->OwnMap(arg->secX, arg->secY) = arg->owner;

    if ( stime == brf->LastFrameTimeStamp )
    {
        arg->AddScore(&brf->StatsIngame);

        if ( yw->GameShell )
            SFXEngine::SFXe.startSound(&yw->GameShell->samples1_info, 12);
    }

    if ( dtime < 30000 )
    {
        if ( arg->owner )
        {
            if ( brf->VectorGfx[3] )
            {
                float v20 = (brf->MapBlitEnd.x2 - brf->MapBlitEnd.x1) / (float)yw->_mapWidth;
                float v21 = (brf->MapBlitEnd.y2 - brf->MapBlitEnd.y1) / (float)yw->_mapHeight;

                float a3a = (float)arg->secX * v20 + brf->MapBlitEnd.x1 + v20 * 0.5;
                float a4a = (float)arg->secY * v21 + brf->MapBlitEnd.y1 + v21 * 0.5;

                float v19 = 1.0 - (float )dtime / 30000.0;

                if ( v19 > 1.0 )
                    v19 = 1.0;
                else if ( v19 < 0.0)
                    v19 = 0.0;

                float a9 = v20 * v19;
                float a10 = v21 * v19;

                yw_RenderVector2D(yw, brf->VectorGfx[3]->GetSkelet(), a3a, a4a, 1.0, 0.0, 0.0, 1.0, a9, a10,  yw_GetColor(yw, arg->owner), NULL, NULL);
            }
        }
    }
}

void yw_DebriefVhclKill(NC_STACK_ypaworld *yw, BriefengScreen *brf, World::History::VhclKill *arg, uint32_t time, uint32_t stime)
{
    int own = arg->owners & 7;
    uint32_t dtime = time - stime;

    if ( stime == brf->LastFrameTimeStamp )
    {
        arg->AddScore(&brf->StatsIngame);

        if ( yw->GameShell )
            SFXEngine::SFXe.startSound(&yw->GameShell->samples1_info, 13);
    }

    if ( dtime < 120000 )
    {
        if ( brf->VectorGfx[1] )
        {
            float v12 = brf->MapBlitEnd.x2 - brf->MapBlitEnd.x1;
            float v13 = brf->MapBlitEnd.y2 - brf->MapBlitEnd.y1;

            float a3a = v12 * (arg->posX / 256.0) + brf->MapBlitEnd.x1;
            float a4a = v13 * (arg->posY / 256.0) + brf->MapBlitEnd.y1;

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

            float a9 = ((v12 / (float)yw->_mapWidth) / 2.0) * v25;
            float a10 = ((v13 / (float)yw->_mapHeight) / 2.0) * v25;

            yw_RenderVector2D(yw, brf->VectorGfx[1]->GetSkelet(), a3a, a4a, 1.0, 0.0, 0.0, 1.0, a9, a10, yw_GetColor(yw, own), NULL, NULL);
        }
    }
}

void yw_DebriefVhclCreate(NC_STACK_ypaworld *yw, BriefengScreen *brf, World::History::VhclCreate *arg, int time, int stime)
{
    int dtime = time - stime;
    if ( dtime < 45000 )
    {
        if ( brf->VectorGfx[1] )
        {
            float v13 = brf->MapBlitEnd.x2 - brf->MapBlitEnd.x1;
            float v14 = brf->MapBlitEnd.y2 - brf->MapBlitEnd.y1;

            float a3a = v13 * (arg->posX / 256.0) + brf->MapBlitEnd.x1;
            float a4a = v14 * (arg->posY / 256.0) + brf->MapBlitEnd.y1;

            float v22 = dtime / 45000.0;

            if ( v22 > 1.0 )
                v22 = 1.0;
            else if ( v22 < 0.0 )
                v22 = 0.0;

            float a9 = ((v13 / (float)yw->_mapWidth) / 8.0) * v22;
            float a10 = ((v14 / (float)yw->_mapHeight) / 8.0) * v22;

            yw_RenderVector2D(yw, brf->VectorGfx[1]->GetSkelet(), a3a, a4a, 1.0, 0.0, 0.0, 1.0, a9, a10, yw_GetColor(yw, arg->owner), NULL, NULL);
        }
    }
}

void yw_DebriefAddTechUpgrade(NC_STACK_ypaworld *yw, BriefengScreen *brf, World::History::Upgrade *arg)
{
    if ( !yw->field_727c || arg->owner == yw->field_7280 )
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

void yw_DebriefRenderSectorsOwners(NC_STACK_ypaworld *yw, BriefengScreen *brf)
{
    float v3 = (brf->MapBlitEnd.x2 - brf->MapBlitEnd.x1) / (float)yw->_mapWidth;
    float v4 = (brf->MapBlitEnd.y2 - brf->MapBlitEnd.y1) / (float)yw->_mapHeight;

    float v19 = v3 / 10.0;
    float v16 = v4 / 10.0;

    float v21 = brf->MapBlitEnd.y1 + v4 * 0.5;

    for (int yy = 0; yy < yw->_mapHeight; yy++)
    {
        float v23 = brf->MapBlitEnd.x1 + v3 * 0.5;
        
        uint8_t *ownmap = brf->OwnMap.Line(yy);

        for (int xx = 0; xx < yw->_mapWidth; xx++)
        {
            int owner = *ownmap;

            if ( owner != 0 )
            {
                if ( xx > 0 && xx < yw->_mapWidth - 1
                        && yy > 0 && yy < yw->_mapHeight - 1 )
                {
                    w3d_func198arg arg198;
                    w3d_func198arg arg198_1;

                    arg198.x1 = v23 - v19;
                    arg198.y1 = v21;
                    arg198.x2 = v19 + v23;
                    arg198.y2 = v21;

                    arg198_1.x1 = v23;
                    arg198_1.y1 = v21 - v16;
                    arg198_1.x2 = v23;
                    arg198_1.y2 = v21 + v16;

                    yw->_win3d->raster_func217( yw_GetColor(yw, owner) );
                    yw->_win3d->raster_func198(&arg198);
                    yw->_win3d->raster_func198(&arg198_1);
                }
            }

            v23 += v3;
            ownmap++;
        }

        v21 += v4;
    }
}


void ypaworld_func158__sub4__sub1__sub6__sub3__sub6(NC_STACK_ypaworld *yw, BriefengScreen *brf)
{
    int v14 = (yw->screen_width / 2) * -0.934375;
    int v13 = (yw->screen_width / 2) * 0.03125;
    int v16 = (yw->screen_height / 2) * -0.9333333333333333;

    const char *v7 = get_lang_string(yw->string_pointers_p2, yw->_levelInfo->LevelID + 1800, yw->_levelInfo->MapName.c_str());

    char cmdBuff[264];
    char *cur = cmdBuff;

    FontUA::select_tileset(&cur, 16);
    FontUA::set_center_xpos(&cur, v14);
    FontUA::set_center_ypos(&cur, v16);

    FontUA::set_txtColor(&cur, yw->iniColors[66].r, yw->iniColors[66].g, yw->iniColors[66].b);

    cur = FontUA::FormateCenteredSkipableItem(yw->tiles[16], cur, v7, v13 - v14);

    FontUA::set_end(&cur);

    w3d_a209 arg209;
    arg209.cmdbuf = cmdBuff;
    arg209.includ = 0;

    yw->_win3d->raster_func209(&arg209);
}

char * yw_DebriefKillsTitleLine(NC_STACK_ypaworld *yw, BriefengScreen *brf, char *in, int a4)
{
    char *cur = in;

    FontUA::set_txtColor(&cur, yw->iniColors[67].r, yw->iniColors[67].g, yw->iniColors[67].b);
    FontUA::ColumnItem elms[3];

    elms[0].txt = get_lang_string(yw->string_pointers_p2, 2450, "KILLS");
    elms[0].spaceChar = 32;
    elms[0].prefixChar = 0;
    elms[0].postfixChar = 0;
    elms[0].width = a4 * 0.4;
    elms[0].flags = 36;
    elms[0].fontID = 15;

    elms[1].txt = get_lang_string(yw->string_pointers_p2, 2451, "BY PLAYER");
    elms[1].fontID = 15;
    elms[1].width = a4 * 0.3;
    elms[1].spaceChar = 32;
    elms[1].flags = 36;
    elms[1].prefixChar = 0;
    elms[1].postfixChar = 0;

    elms[2].txt = get_lang_string(yw->string_pointers_p2, 2452, "ALL");
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

char * yw_DebriefKillsScore(NC_STACK_ypaworld *yw, BriefengScreen *brf, char *in, int a4)
{
    char *cur = in;
    int a2 = 0;

    debrif_t1 v28[8];

    for (int i = 0; i < 8; i++)
    {
        if ( (1 << i) & yw->_levelInfo->OwnerMask )
        {
            v28[a2].owner = i;
            v28[a2].status = brf->StatsIngame[i].destroyed;
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
            who = get_lang_string(yw->string_pointers_p2, 2411, "RESISTANCE");
            clr_id = 1;
            break;

        case 2:
            who = get_lang_string(yw->string_pointers_p2, 2412, "SULGOGARS");
            clr_id = 2;
            break;

        case 3:
            who = get_lang_string(yw->string_pointers_p2, 2413, "MYKONIANS");
            clr_id = 3;
            break;

        case 4:
            who = get_lang_string(yw->string_pointers_p2, 2414, "TAERKASTEN");
            clr_id = 4;
            break;

        case 5:
            who = get_lang_string(yw->string_pointers_p2, 2415, "BLACK SECT");
            clr_id = 5;
            break;

        case 6:
            who = get_lang_string(yw->string_pointers_p2, 2416, "GHORKOV");
            clr_id = 6;
            break;

        default:
            who = get_lang_string(yw->string_pointers_p2, 2417, "NEUTRAL");
            clr_id = 7;
            break;
        }

        FontUA::ColumnItem elms[3];

        FontUA::set_txtColor(&cur, yw->iniColors[clr_id].r, yw->iniColors[clr_id].g, yw->iniColors[clr_id].b);

        elms[0].txt = who;
        elms[0].spaceChar = 32;
        elms[0].fontID = 15;
        elms[0].width = a4 * 0.4;
        elms[0].prefixChar = 0;
        elms[0].postfixChar = 0;
        elms[0].flags = 36;

        if ( yw->field_727c || v28[i].owner == yw->playerOwner )
            elms[1].txt = fmt::sprintf("%d", brf->StatsIngame[ v28[i].owner ].destroyedByUser);
        else
            elms[1].txt = "-";
        elms[1].spaceChar = 32;
        elms[1].fontID = 15;
        elms[1].prefixChar = 0;
        elms[1].postfixChar = 0;
        elms[1].flags = 36;
        elms[1].width = a4 * 0.3;

        elms[2].txt = fmt::sprintf("%d", brf->StatsIngame[ v28[i].owner ].destroyed);
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

    FontUA::set_txtColor(&cur, yw->iniColors[67].r, yw->iniColors[67].g, yw->iniColors[67].b);

    FontUA::ColumnItem elm;

    elm.txt = get_lang_string(yw->string_pointers_p2, 2453, "SCORE");
    elm.width = a4;
    elm.spaceChar = 32;
    elm.fontID = 15;
    elm.prefixChar = 0;
    elm.postfixChar = 0;
    elm.flags = 36;

    return FormateColumnItem(yw, cur, 1, &elm);
}

char *yw_DebriefMPlayScore(NC_STACK_ypaworld *yw, BriefengScreen *brf, char *in, int a4)
{
    char *cur = in;

    if ( yw->field_727c )
    {
        cur = yw_DebriefMPlayScoreTitle(yw, cur, a4);

        FontUA::next_line(&cur);

        debrif_t1 v32[8];
        int a2 = 0;

        for (int i = 0; i < 8; i++)
        {
            if ( (1 << i) & yw->_levelInfo->OwnerMask )
            {
                v32[a2].owner = i;
                v32[a2].status = brf->StatsIngame[i].score;
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
                who = get_lang_string(yw->string_pointers_p2, 2411, "RESISTANCE");
                clr_id = 1;
                break;

            case 2:
                who = get_lang_string(yw->string_pointers_p2, 2412, "SULGOGARS");
                clr_id = 2;
                break;

            case 3:
                who = get_lang_string(yw->string_pointers_p2, 2413, "MYKONIANS");
                clr_id = 3;
                break;

            case 4:
                who = get_lang_string(yw->string_pointers_p2, 2414, "TAERKASTEN");
                clr_id = 4;
                break;

            case 5:
                who = get_lang_string(yw->string_pointers_p2, 2415, "BLACK SECT");
                clr_id = 5;
                break;

            case 6:
                who = get_lang_string(yw->string_pointers_p2, 2416, "GHORKOV");
                clr_id = 6;
                break;

            default:
                who = get_lang_string(yw->string_pointers_p2, 2417, "NEUTRAL");
                clr_id = 7;
                break;
            }

            FontUA::set_txtColor(&cur, yw->iniColors[clr_id].r, yw->iniColors[clr_id].g, yw->iniColors[clr_id].b);

            FontUA::ColumnItem a4a[2];

            a4a[0].txt = who;
            a4a[0].spaceChar = 32;
            a4a[0].fontID = 15;
            a4a[0].prefixChar = 0;
            a4a[0].postfixChar = 0;
            a4a[0].flags = 36;
            a4a[0].width = a4 * 0.5;

            a4a[1].txt = fmt::sprintf("%d", brf->StatsIngame[ v32[i].owner ].score);
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
        FontUA::set_txtColor(&cur, yw->iniColors[67].r, yw->iniColors[67].g, yw->iniColors[67].b);

        FontUA::ColumnItem v35[2];

        v35[0].txt = get_lang_string(yw->string_pointers_p2, 2457, "SCORE THIS MISSION:");
        v35[0].spaceChar = 32;
        v35[0].prefixChar = 0;
        v35[0].postfixChar = 0;
        v35[0].width = a4 * 0.7;
        v35[0].fontID = 15;
        v35[0].flags = 36;

        v35[1].txt = fmt::sprintf("%d", brf->StatsIngame[yw->playerOwner].score);
        v35[1].fontID = 15;
        v35[1].spaceChar = 32;
        v35[1].prefixChar = 0;
        v35[1].width = a4 * 0.3;
        v35[1].postfixChar = 0;
        v35[1].flags = 36;

        cur = FormateColumnItem(yw, cur, 2, v35);

        FontUA::next_line(&cur);

        v35[0].txt = get_lang_string(yw->string_pointers_p2, 2458, "SCORE OVERALL:");
        v35[0].flags = 36;
        v35[0].spaceChar = 32;
        v35[0].width = a4 * 0.7;
        v35[0].prefixChar = 0;
        v35[0].postfixChar = 0;
        v35[0].fontID = 15;

        

        v35[1].txt = fmt::sprintf("%d", brf->StatsIngame[yw->playerOwner].score + brf->StatsGlobal[yw->playerOwner].score);
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

char * yw_DebriefRenderTime(NC_STACK_ypaworld *yw, BriefengScreen *brf, char *in, int a4)
{
    char *cur = in;
    FontUA::set_txtColor(&cur, yw->iniColors[67].r, yw->iniColors[67].g, yw->iniColors[67].b);

    if ( yw->field_727c )
    {
        FontUA::ColumnItem v30[2];
        v30[0].txt = get_lang_string(yw->string_pointers_p2, 2456, "PLAYING TIME:");
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
        a4a[0].txt = get_lang_string(yw->string_pointers_p2, 2454, "PLAYING TIME THIS MISSION:");
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

        a4a[0].txt = get_lang_string(yw->string_pointers_p2, 2455, "PLAYING TIME OVERALL:");
        a4a[0].flags = 36;
        a4a[0].spaceChar = 32;
        a4a[0].width = a4 * 0.7;
        a4a[0].prefixChar = 0;
        a4a[0].postfixChar = 0;
        a4a[0].fontID = 15;

        v19 = (brf->LastFrameTimeStamp + brf->StatsGlobal[yw->playerOwner].elapsedTime) / 1024;

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

char * yw_DebriefScoreTable(NC_STACK_ypaworld *yw, BriefengScreen *brf, char *in)
{
    char *cur = in;

    FontUA::select_tileset(&cur, 15);
    FontUA::set_center_xpos(&cur, ((yw->screen_width / 2) * 0.15) );
    FontUA::set_center_ypos(&cur, ((yw->screen_height / 2) * -0.821) );

    int v14 = (yw->screen_width / 2) * 0.796875;

    cur = yw_DebriefKillsTitleLine(yw, brf, cur, v14);

    FontUA::next_line(&cur);

    cur = yw_DebriefKillsScore(yw, brf, cur, v14);

    FontUA::next_line(&cur);

    cur = yw_DebriefMPlayScore(yw, brf, cur, v14);

    FontUA::next_line(&cur);

    return yw_DebriefRenderTime(yw, brf, cur, v14);
}

char * yw_DebriefTechUpgradeLine(NC_STACK_ypaworld *yw, BriefengScreen *brf, const World::History::Upgrade &upg, char *in, int a5)
{
    char *cur = in;

    int lastWeapon = upg.lastWeapon;
    int lastVhcl   = upg.lastVhcl;
    int lastBuild  = upg.lastBuild;

    if ( !lastVhcl && lastWeapon )
    {
        for (int i = 0; i < 256; i++)
        {
            if (yw->VhclProtos[i].weapon == lastWeapon)
            {
                lastVhcl = i;
                break;
            }
        }

        if ( !lastVhcl )
            return cur;
    }

    if ( !lastWeapon && lastVhcl )
    {
        lastWeapon = yw->VhclProtos[ lastVhcl ].weapon;

        if ( lastWeapon == -1 )
            lastWeapon = 0;
    }

    VhclProto *vhcl = NULL;
    WeapProto *wpn  = NULL;
    TBuildingProto *bld = NULL;

    if ( lastVhcl )
        vhcl = &yw->VhclProtos[lastVhcl];

    if ( lastBuild )
        bld = &yw->BuildProtos[lastBuild];

    if ( lastWeapon )
        wpn = &yw->WeaponProtos[lastWeapon];

    std::string v13 = " ";
    std::string v14 = " ";
    std::string v33 = " ";

    if ( vhcl )
    {
        v33 = get_lang_string(yw->string_pointers_p2, lastVhcl + 1200, vhcl->name.c_str());
    }
    else if ( bld )
    {
        if ( yw->field_727c )
            v33 = get_lang_string(yw->string_pointers_p2, lastBuild + 1700, bld->Name.c_str());
        else
            v33 = get_lang_string(yw->string_pointers_p2, lastBuild + 1500, bld->Name.c_str());
    }

    switch ( upg.upgradeType )
    {
    case World::UPGRADE_WEAPON:
        if ( wpn )
        {
            if ( vhcl )
            {
                v13 = get_lang_string(yw->string_pointers_p2, 2459, "WEAPON UPGRADE:");

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
            v13 = get_lang_string(yw->string_pointers_p2, 2460, "ARMOR UPGRADE:");

            v14 = fmt::sprintf("(%d%%)", vhcl->shield);
        }
        break;

    case World::UPGRADE_VEHICLE:
        if ( vhcl )
        {
            v13 = get_lang_string(yw->string_pointers_p2, 2461, "NEW VEHICLE TECH:");
        }
        break;

    case World::UPGRADE_BUILDING:
        if ( bld )
        {
            v13 = get_lang_string(yw->string_pointers_p2, 2462, "NEW BUILDING TECH:");
        }
        break;

    case World::UPGRADE_RADARE:
        if ( vhcl )
        {
            v13 = get_lang_string(yw->string_pointers_p2, 2463, "RADAR UPGRADE:");
        }
        break;

    case World::UPGRADE_BLDVHCL:
        if ( vhcl )
        {
            if ( bld )
            {
                if ( yw->field_727c )
                    v14 = get_lang_string(yw->string_pointers_p2, lastBuild + 1700, bld->Name.c_str());
                else
                    v14 = get_lang_string(yw->string_pointers_p2, lastBuild + 1500, bld->Name.c_str());

                v13 = get_lang_string(yw->string_pointers_p2, 2464, "COMBINED UPGRADE:");
            }
        }
        break;

    case World::UPGRADE_GENERIC:
        v13 = get_lang_string(yw->string_pointers_p2, 2465, "GENERIC TECH UPGRADE");
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

char * yw_DebriefTechUpgradesTable(NC_STACK_ypaworld *yw, BriefengScreen *brf, char *in)
{
    char *cur = in;

    FontUA::select_tileset(&cur, 15);
    FontUA::set_center_xpos(&cur, ((yw->screen_width / 2) * -0.9875) );
    FontUA::set_center_ypos(&cur, ((yw->screen_height / 2) * 0.35) );

    FontUA::set_txtColor(&cur, yw->iniColors[67].r, yw->iniColors[67].g, yw->iniColors[67].b);

    for ( const auto &upg : brf->Upgrades )
        cur = yw_DebriefTechUpgradeLine(yw, brf, upg, cur,  (yw->screen_width / 2) * 0.984375 );

    return cur;
}

void yw_DebriefRunDebrief(NC_STACK_ypaworld *yw, InputState *struc, BriefengScreen *brf)
{
    char cmdbuf[2048];
    char *cur = cmdbuf;

    uint32_t dtime = brf->CurrTime - brf->StartTime;

    yw_DebriefRenderSectorsOwners(yw, brf);

    cur = yw_DebriefScoreTable(yw, brf, cur);
    cur = yw_DebriefTechUpgradesTable(yw, brf, cur);

    FontUA::set_end(&cur);

    uint32_t _lastFrameTimeStamp = 0;

    w3d_a209 v24;
    v24.includ = 0;
    v24.cmdbuf = cmdbuf;

    yw->_win3d->raster_func209(&v24);

    int v26 = 0;
    
    

    if ( brf->Stage == 8 )
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
                    
                    _lastFrameTimeStamp = frm->TimeStamp;

                    if ( _lastFrameTimeStamp >= brf->LastFrameTimeStamp )
                        brf->LastFrameTimeStamp = _lastFrameTimeStamp;

                    if ( _lastFrameTimeStamp >= dtime )
                    {
                        v26 = 1;
                        readLoop = false;
                    }
                }
                break;

                case World::History::TYPE_CONQ:
                    yw_DebriefConqSector(yw, brf, static_cast<World::History::Conq *>(decoder), dtime, _lastFrameTimeStamp);
                    break;

                case World::History::TYPE_VHCLKILL:
                    yw_DebriefVhclKill(yw, brf, static_cast<World::History::VhclKill *>(decoder), dtime, _lastFrameTimeStamp);
                    break;

                case World::History::TYPE_VHCLCREATE:
                    yw_DebriefVhclCreate(yw, brf, static_cast<World::History::VhclCreate *>(decoder), dtime, _lastFrameTimeStamp);
                    break;

                case World::History::TYPE_POWERST:
                    if ( _lastFrameTimeStamp == brf->LastFrameTimeStamp )
                    {
                        decoder->AddScore(&brf->StatsIngame);

                        if ( yw->GameShell )
                            SFXEngine::SFXe.startSound(&yw->GameShell->samples1_info, 14);
                    }
                    break;

                case World::History::TYPE_UPGRADE:
                    if ( _lastFrameTimeStamp == brf->LastFrameTimeStamp )
                    {
                        decoder->AddScore(&brf->StatsIngame);
                        yw_DebriefAddTechUpgrade(yw, brf, static_cast<World::History::Upgrade *>(decoder));

                        if ( yw->GameShell )
                            SFXEngine::SFXe.startSound(&yw->GameShell->samples1_info, 14);

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
            brf->Stage = 9;
            if ( yw->field_727c )
            {
                for (int i = 0; i < 8; i ++)
                    brf->StatsIngame[i] = yw->ingamePlayerStatus[i];
            }
        }
    }

    ypaworld_func158__sub4__sub1__sub6__sub3__sub6(yw, brf);
}

void yw_debriefUpdate(NC_STACK_ypaworld *yw, InputState *inpt)
{
    BriefengScreen *brf = &yw->brief;

    if ( yw->_history.Size() ) //FIXME
    {
        if ( yw->brief.TimerStatus == 0 )
        {
            if ( brf->Stage == 8 )
            {
                brf->CurrTime += 60 * inpt->Period;
            }
            else if ( brf->Stage != 9 )
            {
                brf->CurrTime += inpt->Period;
            }
        }
        else if ( yw->brief.TimerStatus == 1 )
        {
            inpt->Period = 1;
        }
        else if ( yw->brief.TimerStatus == 3 )
        {
            brf->TimerStatus = 0;
            brf->Stage = 7;
        }

        if ( brf->Stage != 4 )
        {
            if ( brf->BriefingMapImg )
            {
                rstr_arg204 arg204;
                arg204.pbitm = brf->BriefingMapImg->GetResBmp();

                arg204.float4 = -1.0;
                arg204.floatC = 1.0;
                arg204.float8 = -1.0;
                arg204.float10 = 1.0;
                arg204.float14 = -1.0;
                arg204.float1C = 1.0;
                arg204.float18 = -1.0;
                arg204.float20 = 1.0;

                yw->_win3d->raster_func204(&arg204);
            }

            yw->_win3d->raster_func204(&brf->MapBlitParams);
        }

        switch ( brf->Stage )
        {
        case 4:
            ypaworld_func158__sub4__sub1__sub6__sub0(yw, inpt, brf);
            break;

        case 5:
            ypaworld_func158__sub4__sub1__sub6__sub1(yw, inpt, brf);
            break;

        case 6:
            brf->Stage = 7;
            break;

        case 7:
            ypaworld_func158__sub4__sub1__sub6__sub2(yw, inpt, brf);
            break;

        case 8:
        case 9:
            yw_DebriefRunDebrief(yw, inpt, brf);
            break;

        default:
            break;
        }
    }
    else
    {
        yw->brief.Stage = 2;
    }
}
