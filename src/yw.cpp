#include <inttypes.h>
#include <string.h>
#include <math.h>
#include "includes.h"
#include "yw.h"

#include "def_parser.h"

#include "yw_internal.h"

#include "button.h"
#include "font.h"



stored_functions *classvtbl_get_ypaworld();
class_return * class_set_ypaworld(int, ...);

stored_functions ypaworld_class_vtbl(class_set_ypaworld);


class_stored ypaworld_class_off (NULL, NULL, "MC2classes:ypaworld.class", classvtbl_get_ypaworld);


stored_functions *classvtbl_get_ypaworld()
{
    return &ypaworld_class_vtbl;
}

key_value_stru ypaworld_keys[4] =
{
    {"netgame.exclusivegem", KEY_TYPE_BOOL, 1},
    {"net.waitstart", KEY_TYPE_DIGIT, 150000},
    {"net.kickoff", KEY_TYPE_DIGIT, 20000},
    {"game.debug", KEY_TYPE_BOOL, 0}
};

CLASSFUNC ypaworld_funcs[1024];

polys *p_outPolys;
polysDat *p_polysdata;
polysDat *p_polysdata_end;
Key_stru keySS[256];

int word_5A50C2;
int word_5A50AC;
int word_5A50B0;
int dword_5A50B2;
int dword_5A50B2_h;
int word_5A50AE;
int word_5A50BC;
int word_5A50BA;
int word_5A50BE;
int word_5A50C0;

int dword_5A50B6;
int dword_5A50B6_h;

char **ypaworld__string_pointers;

listview stru_5C91D0;


int sub_4493B0(scrCallBack *arg)
{
    if ( arg->field_18 )
    {
        if ( !strcasecmp(arg->p1, "end") )
        {
            arg->field_18 = 0;
            return 2;
        }
        else if ( arg->p1 && arg->p2 )
        {
            set_prefix_replacement(arg->p1, arg->p2);
            ypa_log_out("parsing assign.txt: set assign %s to %s\n", arg->p1, arg->p2);
            return 0;
        }
        else
        {
            return 4;
        }
    }
    else if ( !strcasecmp(arg->p1, "begin_assign") )
    {
        arg->field_18 = 1;
        return 1;
    }

    return 3;
}


int ypaworld_func0__sub0(const char *file)
{
    scrCallBack v3;

    memset(&v3, 0, sizeof(scrCallBack));
    v3.func = sub_4493B0;
    return def_parseFile(file, 1, &v3, 0);
}

void ypaworld_func0__sub1()
{
    char replace[1024];

    for (int i = 0; i < 6; i++)
    {
        const char *prefix;
        switch (i)
        {
        case 0:
            prefix = "data";
            break;
        case 1:
            prefix = "save";
            break;
        case 2:
            prefix = "help";
            break;
        case 3:
            prefix = "mov";
            break;
        case 4:
            prefix = "levels";
            break;
        case 5:
            prefix = "mbpix";
            break;
        default:
            prefix = "nop";
            break;
        }

        if ( read_reg_key(prefix, replace, 1024) )
        {
            set_prefix_replacement(prefix, replace);
            ypa_log_out("parsing registry: set assign %s to %s\n", prefix, replace);
        }
    }
}

void sub_4711E0(_NC_STACK_ypaworld *yw)
{
    yw->very_big_array__p_begin = yw->lang_strings;
    yw->lang_strings__end = yw->lang_strings + 0x20000;

    memset(yw->lang_strings, 0, 0x20000);
    memset(yw->string_pointers, 0, 0x28A0);

    memset(yw->lang_name, 0, sizeof(yw->lang_name));
    strcpy(yw->lang_name, "default");
}

int yw_InitLocale(_NC_STACK_ypaworld *yw)
{
    int v3 = 0;
    yw->lang_strings = (char *)AllocVec(0x20000, 1);

    if ( yw->lang_strings )
    {
        yw->string_pointers = (char **)AllocVec(sizeof(char *) * 2600, 65537);

        if ( yw->string_pointers )
        {
            yw->string_pointers_p2 = yw->string_pointers;
            sub_4711E0(yw);
            v3 = 1;
        }
    }

    if ( !v3 )
    {
////	sub_4D99C4(); // free language dll

        if ( yw->lang_strings )
            nc_FreeMem(yw->lang_strings);

        if ( yw->string_pointers )
            nc_FreeMem(yw->string_pointers);

        yw->very_big_array__p_begin = NULL;
        yw->lang_strings__end = NULL;
        yw->string_pointers = NULL;
        yw->lang_strings = NULL;
    }
    return v3;
}

int sub_4DA354(_NC_STACK_ypaworld *yw, const char *filename)
{
    scrCallBack clbk[3];
    memset(clbk, 0, sizeof(clbk));

    char buf[256];

    strcpy(buf, get_prefix_replacement("rsrc"));
    set_prefix_replacement("rsrc", "data:");

    clbk[0].world = yw;
    clbk[0].func = VhclProtoParser;
    clbk[1].world = yw;
    clbk[1].func = WeaponProtoParser;
    clbk[2].world2 = yw;
    clbk[2].func = BuildProtoParser;

    int res = def_parseFile(filename, 3, clbk, 1);
    set_prefix_replacement("rsrc", buf);
    return res;
}

int init_prototypes(_NC_STACK_ypaworld *yw)
{
    yw->VhclProtos = (VhclProto *)AllocVec(sizeof(VhclProto) * 256, 65537);
    yw->WeaponProtos = (WeapProto *)AllocVec(sizeof(WeapProto) * 128, 65537);
    yw->BuildProtos = (BuildProto *)AllocVec(sizeof(BuildProto) * 128, 65537);
    yw->RoboProtos = (roboProto *)AllocVec(sizeof(roboProto) * 16, 65537);

    if ( yw->VhclProtos && yw->WeaponProtos && yw->BuildProtos && yw->RoboProtos )
    {
        if ( sub_4DA354(yw, "data:scripts/startup.scr") )
            return 1;
    }

    return 0;
}

int yw_initAttrs(NC_STACK_ypaworld *obj, _NC_STACK_ypaworld *yw, stack_vals *stak)
{
    yw->fxnumber = 16;
    yw->field_1368 = find_id_in_stack_def_val(0x8000200E, 5, stak);
    yw->field_15e4 = find_id_in_stack_def_val(0x80002007, 1400, stak);
    yw->field_15e8 = find_id_in_stack_def_val(0x80002008, 600, stak);
    yw->field_15ec = find_id_in_stack_def_val(0x80002009, 4200, stak);
    yw->field_15f0 = find_id_in_stack_def_val(0x8000200A, 1100, stak);
    yw->sectors_maxX = find_id_in_stack_def_val(0x80002000, 64, stak);
    yw->sectors_maxY = find_id_in_stack_def_val(0x80002001, 64, stak);
    yw->sectors_maxX2 = yw->sectors_maxX;
    yw->sectors_maxY2 = yw->sectors_maxY;
    yw->field_15f4 = find_id_in_stack_def_val(0x8000200B, -550, stak);
    yw->field_15f8 = find_id_in_stack_def_val(0x8000200C, 1, stak);
    yw->field_15fc = find_id_in_stack_def_val(0x8000200D, 1, stak);

    char *v4 = (char *)find_id_in_stack_def_val(0x8000201D, 0, stak);
    yw->buildDate = v4;
    if ( v4 )
    {
        for (char *pchr = v4; *pchr; pchr++)
        {
            char chr = toupper(*pchr);
            if ( chr < ' ' || chr > 'Z' )
                chr = '*';
            *pchr = chr;
        }
    }
    return 1;
}


recorder *sub_48025C()
{
    printf("MAKE ME %s\n","sub_48025C");
    return (recorder *)AllocVec(sizeof(recorder), 65537);
}

int yw_InitSceneRecorder(_NC_STACK_ypaworld *yw)
{
    yw->sceneRecorder = sub_48025C();
    return yw->sceneRecorder != 0;
}

NC_STACK_ypaworld *ypaworld_func0(class_stru *clss, class_stru *zis, stack_vals *stak)
{
    NC_STACK_ypaworld *obj = (NC_STACK_ypaworld *)call_parent(zis, clss, 0, stak);

    if (obj)
    {
        _NC_STACK_ypaworld *yw = (_NC_STACK_ypaworld *)&obj->stack__ypaworld;

        yw->self_full = obj;

        init_mc_res_class_engine_strings(MC_TYPE_RES, "rsrc:");
        set_prefix_replacement("rsrc", "mc2res");
        set_prefix_replacement("data", "Data");
        set_prefix_replacement("save", "Save");
        set_prefix_replacement("help", "Help");
        set_prefix_replacement("mov", "Data:mov");
        set_prefix_replacement("levels", "Levels");
        set_prefix_replacement("mbpix", "levels:mbpix");

        if ( !ypaworld_func0__sub0("env/assign.txt") )
            ypa_log_out("Warning, no env/assign.txt script.\n");

        ypaworld_func0__sub1();

        if ( !yw_InitLocale(yw) )
        {
            ypa_log_out("yw_main.c/OM_NEW: yw_InitLocale() failed!\n");
            call_method(obj, 1);
            return NULL;
        }

        const char *langname = "language";
        call_method(obj, 166, &langname); // Load lang strings

//		if ( !make_CD_CHECK(1, 1, v10, v9) )
//		{
//			call_method(obj, 1);
//			return NULL;
//		}

        yw->vhcls_models = (vhclBases *)AllocVec(sizeof(vhclBases) * 512, 65537);
        yw->legos = (cityBases *)AllocVec(sizeof(cityBases) * 256, 65537);
        yw->subSectors = (subSec *)AllocVec(sizeof(subSec) * 256, 65537);
        yw->secTypes = (secType *)AllocVec(sizeof(secType) * 256, 65537);

        if ( !yw->vhcls_models || !yw->legos || !yw->subSectors || !yw->secTypes )
        {
            call_method(obj, 1);
            return NULL;
        }

        if ( !init_prototypes(yw) )
        {
            ypa_log_out("ERROR: couldn't initialize prototypes.\n");
            call_method(obj, 1);
            return NULL;
        }
        call_vtbl(obj, 3, 0x80001020, &p_outPolys, 0x80001021, &p_polysdata, 0x80001022, &p_polysdata_end, 0);

        int width, height;
        gfxEngine__getter(0x80003003, &width, 0x80003004, &height, 0);

        yw->screen_width = width;
        yw->screen_height = height;

        init_list(&yw->field_48);
        init_list(&yw->field_17a0);
        init_list(&yw->field_54);

        if ( !yw_initAttrs(obj, yw, stak) )
        {
            ypa_log_out("yw_main.c/OM_NEW: yw_initAttrs() failed!\n");
            call_method(obj, 1);
            return 0;
        }

        yw->field_130[0] = 3;
        for (int i = 1; i <= 100; i++ )
            yw->field_130[i] = 2;
        for (int i = 101; i <= 200; i++ )
            yw->field_130[i] = 1;
        for (int i = 201; i < 256; i++ )
            yw->field_130[i] = 0;

        for (int j = 0; j < 64; j++)
        {
            for (int i = 0; i < 64; i++)
            {
                yw->sqrt_table[j][i] = sqrt(j * j + i * i);
            }
        }

        yw->cells = (cellArea *)AllocVec(sizeof(cellArea) * yw->sectors_maxX * yw->sectors_maxY, 65537);

        if ( !yw->cells )
        {
            ypa_log_out("yw_main.c/OM_NEW: alloc of cell area failed!\n");
            call_method(obj, 1);
            return NULL;
        }
        if ( !yw_InitSceneRecorder(yw) )
        {
            ypa_log_out("yw_main.c/OM_NEW: init scene recorder failed!\n");
            call_method(obj, 1);
            return NULL;
        }
        if ( !yw_InitTooltips(yw) )
        {
            ypa_log_out("yw_main.c/OM_NEW: yw_InitTooltips() failed!\n");
            call_method(obj, 1);
            return NULL;
        }

        yw->one_game_res = 1;
        yw->shell_default_res = (640 << 12) | 480;
        yw->game_default_res = (640 << 12) | 480;

        if ( !yw_InitLevelNet(yw) )
        {
            ypa_log_out("yw_main.c/OM_NEW: yw_InitLevelNet() failed!\n");
            call_method(obj, 1);
            return NULL;
        }

        yw->field_73CE |= 0x10;

        if ( !yw_InitNetwork(yw) )
        {
            ypa_log_out("yw_main.c/OM_NEW: yw_InitNetwork() failed!\n");
            call_method(obj, 1);
            return NULL;
        }

        yw->str17_NOT_FALSE = 0;
        yw->field_138c = 0;

    }
    else
    {
        ypa_log_out("yw_main.c/OM_NEW: _supermethoda() failed!\n");
        obj = NULL;
    }

    return obj;
}


void ypaworld_func1(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func2(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func3__sub0(NC_STACK_ypaworld *obj, _NC_STACK_ypaworld *yw, stack_vals *stak)
{
    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == 0)
            break;
        else if (stk->id == 2)
        {
            stk = (stack_vals *)stk->value;
        }
        else if ( stk->id == 3 )
        {
            stk += stk->value;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;


            case 0x80002018:
                *(char ***)stk->value = yw->string_pointers;
                break;


            }
            stk++;
        }
    }
}

void ypaworld_func3(NC_STACK_ypaworld *obj, class_stru *zis, stack_vals *stak)
{
    _NC_STACK_ypaworld *yw = &obj->stack__ypaworld;

    ypaworld_func3__sub0(obj, yw, stak);

    call_parent(zis, obj, 3, stak);
}


void ypaworld_func64(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func129(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func130(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func131(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func132(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func133(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func134(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func135(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func136(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func137(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func138(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func139(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func140(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func143(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func144(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func145(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func146(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func147(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func148(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func149(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func150(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func151(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func153(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}

void sub_46D2B4(NC_STACK_ypaworld *obj, UserData *usr)
{
    NC_STACK_input *input_class = NULL;

    int v10 = usr->field_D36;

    inputEngine__getter(0x80001009, &input_class, 0);

    for (int i = 0; i <= 48; i++)
    {
        winp_68arg winpArg;
        input__func66__params v7;

        winpArg.id = i;
        winpArg.keyname = "nop";

        v7.field_0 = 3;
        v7.funcID = 68;
        v7.field_4 = 0;
        v7.vals = &winpArg;

        call_method(input_class, 66, &v7);
    }

    for (int i = 1; i <= 45; i++)
    {
        usr->field_D36 = i;
        call_method(obj, 173, usr);
    }

    usr->field_D36 = v10;
}


size_t ypaworld_func154(NC_STACK_ypaworld *obj, class_stru *zis, UserData *usr)
{
    _NC_STACK_ypaworld *yw = &obj->stack__ypaworld;

    yw->GameShell = usr;
    usr->p_ypaworld = yw;

    yw->field_2d90->field_40 = 8;
    usr->field_46 = 1;

    get_keyvalue_from_ini(0, ypaworld_keys, 4);

    yw->netgame_exclusivegem = ypaworld_keys[0].value.val;

    call_vtbl(obj, 3, 0x80002018, &ypaworld__string_pointers, 0);

    init_list(&usr->files_list);
    init_list(&usr->video_mode_list);
    init_list(&usr->lang_dlls);

    set_keys_vals(yw);

    fill_videmodes_list(usr);
    listSaveDir(usr, "save:");
    listLocaleDir(usr, "locale");


    strcpy(usr->usernamedir, "NEWUSER");

    usr->field_0x8 = 1;
    usr->field_1612 = 0;
    usr->field_D36 = 1;


    usr->usernamedir_len = strlen(usr->usernamedir);

    usr->samples2_info.field_0 = 0;
    usr->samples2_info.field_4 = 0;
    usr->samples2_info.field_8 = 0;
    usr->samples2_info.field_C = 0;
    usr->samples2_info.field_10 = 0;
    usr->samples2_info.field_14 = 0;

    usr->samples1_info.field_0 = usr->samples2_info.field_0;
    usr->samples1_info.field_4 = usr->samples2_info.field_4;
    usr->samples1_info.field_8 = usr->samples2_info.field_8;
    usr->samples1_info.field_C = usr->samples2_info.field_C;
    usr->samples1_info.field_10 = usr->samples2_info.field_10;
    usr->samples1_info.field_14 = usr->samples2_info.field_14;

    for (int i = 0; i < 16; i++)
    {
        usr->samples2_info.samples_data[i].volume = 127;
        usr->samples2_info.samples_data[i].pitch = 0;
        usr->samples1_info.samples_data[i].volume = usr->samples2_info.samples_data[i].volume;
        usr->samples1_info.samples_data[i].pitch = usr->samples2_info.samples_data[i].pitch;
    }

    sub_423DB0(&usr->samples1_info);
    sub_423DB0(&usr->samples2_info);
    sub_423DB0(&usr->field_782);

    if ( !ShellSoundsLoad(usr) )
    {
        ypa_log_out("Error: Unable to load from Shell.ini\n");
        return 0;
    }

    usr->field_FBE = 0;

    usr->keyConfig[3].inp_type = 2;
    usr->keyConfig[3].keyID = 3;
    usr->keyConfig[3].KeyCode = 39;
    usr->keyConfig[3].slider_neg = 37;


    usr->keyConfig[4].inp_type = 2;
    usr->keyConfig[4].keyID = 4;
    usr->keyConfig[4].KeyCode = 38;
    usr->keyConfig[4].slider_neg = 40;

    usr->keyConfig[8].inp_type = 2;
    usr->keyConfig[8].keyID = 0;
    usr->keyConfig[8].KeyCode = 39;
    usr->keyConfig[8].slider_neg = 37;


    usr->keyConfig[6].inp_type = 2;
    usr->keyConfig[6].keyID = 1;
    usr->keyConfig[6].KeyCode = 38;
    usr->keyConfig[6].slider_neg = 40;

    usr->keyConfig[7].inp_type = 2;
    usr->keyConfig[7].keyID = 2;
    usr->keyConfig[7].KeyCode = 17;
    usr->keyConfig[7].slider_neg = 16;

    usr->keyConfig[5].inp_type = 2;
    usr->keyConfig[5].keyID = 5;
    usr->keyConfig[5].KeyCode = 65;
    usr->keyConfig[5].slider_neg = 89;

    usr->keyConfig[10].inp_type = 1;
    usr->keyConfig[10].keyID = 0;
    usr->keyConfig[10].KeyCode = 32;

    usr->keyConfig[11].inp_type = 1;
    usr->keyConfig[11].keyID = 1;
    usr->keyConfig[11].KeyCode = 9;

    usr->keyConfig[12].inp_type = 1;
    usr->keyConfig[12].keyID = 2;
    usr->keyConfig[12].KeyCode = 13;

    usr->keyConfig[9].inp_type = 1;
    usr->keyConfig[9].keyID = 3;
    usr->keyConfig[9].KeyCode = 96;

    usr->keyConfig[14].inp_type = 3;
    usr->keyConfig[14].keyID = 25;
    usr->keyConfig[14].KeyCode = 86;

    usr->keyConfig[17].inp_type = 3;
    usr->keyConfig[17].keyID = 2;
    usr->keyConfig[17].KeyCode = 78;

    usr->keyConfig[18].inp_type = 3;
    usr->keyConfig[18].keyID = 3;
    usr->keyConfig[18].KeyCode = 65;

    usr->keyConfig[16].inp_type = 3;
    usr->keyConfig[16].keyID = 0;
    usr->keyConfig[16].KeyCode = 79;

    usr->keyConfig[37].inp_type = 3;
    usr->keyConfig[37].keyID = 1;
    usr->keyConfig[37].KeyCode = 32;

    usr->keyConfig[35].inp_type = 3;
    usr->keyConfig[35].keyID = 4;
    usr->keyConfig[35].KeyCode = 67;

    usr->keyConfig[15].inp_type = 3;
    usr->keyConfig[15].keyID = 7;
    usr->keyConfig[15].KeyCode = 71;

    usr->keyConfig[25].inp_type = 3;
    usr->keyConfig[25].keyID = 8;
    usr->keyConfig[25].KeyCode = 77;

    usr->keyConfig[19].inp_type = 3;
    usr->keyConfig[19].keyID = 9;
    usr->keyConfig[19].KeyCode = 70;

    usr->keyConfig[27].inp_type = 3;
    usr->keyConfig[27].keyID = 10;
    usr->keyConfig[27].KeyCode = 0;

    usr->keyConfig[28].inp_type = 3;
    usr->keyConfig[28].keyID = 11;
    usr->keyConfig[28].KeyCode = 0;

    usr->keyConfig[29].inp_type = 3;
    usr->keyConfig[29].keyID = 12;
    usr->keyConfig[29].KeyCode = 0;

    usr->keyConfig[31].inp_type = 3;
    usr->keyConfig[31].keyID = 14;
    usr->keyConfig[31].KeyCode = 0;

    usr->keyConfig[32].inp_type = 3;
    usr->keyConfig[32].keyID = 16;
    usr->keyConfig[32].KeyCode = 0;

    usr->keyConfig[33].inp_type = 3;
    usr->keyConfig[33].keyID = 17;
    usr->keyConfig[33].KeyCode = 0;

    usr->keyConfig[30].inp_type = 3;
    usr->keyConfig[30].keyID = 18;
    usr->keyConfig[30].KeyCode = 0;

    usr->keyConfig[41].inp_type = 3;
    usr->keyConfig[41].keyID = 20;
    usr->keyConfig[41].KeyCode = 112;

    usr->keyConfig[38].inp_type = 3;
    usr->keyConfig[38].keyID = 21;
    usr->keyConfig[38].KeyCode = 113;

    usr->keyConfig[40].inp_type = 3;
    usr->keyConfig[40].keyID = 22;
    usr->keyConfig[40].KeyCode = 114;

    usr->keyConfig[39].inp_type = 3;
    usr->keyConfig[39].keyID = 23;
    usr->keyConfig[39].KeyCode = 115;

    usr->keyConfig[2].inp_type = 3;
    usr->keyConfig[2].keyID = 24;
    usr->keyConfig[2].KeyCode = 27;

    usr->keyConfig[34].inp_type = 3;
    usr->keyConfig[34].keyID = 27;
    usr->keyConfig[34].KeyCode = 0;

    usr->keyConfig[42].inp_type = 3;
    usr->keyConfig[42].keyID = 31;
    usr->keyConfig[42].KeyCode = 8;

    usr->keyConfig[1].inp_type = 3;
    usr->keyConfig[1].keyID = 32;
    usr->keyConfig[1].KeyCode = 0;

    usr->keyConfig[43].inp_type = 3;
    usr->keyConfig[43].keyID = 37;
    usr->keyConfig[43].KeyCode = 101;

    usr->keyConfig[20].inp_type = 3;
    usr->keyConfig[20].keyID = 38;
    usr->keyConfig[20].KeyCode = 49;

    usr->keyConfig[21].inp_type = 3;
    usr->keyConfig[21].keyID = 39;
    usr->keyConfig[21].KeyCode = 50;

    usr->keyConfig[22].inp_type = 3;
    usr->keyConfig[22].keyID = 40;
    usr->keyConfig[22].KeyCode = 51;

    usr->keyConfig[23].inp_type = 3;
    usr->keyConfig[23].keyID = 41;
    usr->keyConfig[23].KeyCode = 52;

    usr->keyConfig[24].inp_type = 3;
    usr->keyConfig[24].keyID = 42;
    usr->keyConfig[24].KeyCode = 53;

    usr->keyConfig[26].inp_type = 1;
    usr->keyConfig[26].keyID = 4;
    usr->keyConfig[26].KeyCode = 16;

    usr->keyConfig[44].inp_type = 3;
    usr->keyConfig[44].keyID = 43;
    usr->keyConfig[44].KeyCode = 0;

    usr->keyConfig[36].inp_type = 3;
    usr->keyConfig[36].keyID = 44;
    usr->keyConfig[36].KeyCode = 0;

    usr->keyConfig[13].inp_type = 3;
    usr->keyConfig[13].keyID = 45;
    usr->keyConfig[13].KeyCode = 0;

    usr->keyConfig[45].inp_type = 3;
    usr->keyConfig[45].keyID = 46;
    usr->keyConfig[45].KeyCode = 0;

    sub_46D2B4(obj, usr);

    for (int i = 1; i < 46; i++)
    {
        usr->keyConfig[i].field_8 = usr->keyConfig[i].KeyCode;
        usr->keyConfig[i].field_A = usr->keyConfig[i].slider_neg;
    }

    for (int i = 1; i < 46; i++)
    {
        usr->keyConfig[i].field_C = usr->keyConfig[i].KeyCode;
        usr->keyConfig[i].field_E = usr->keyConfig[i].slider_neg;
    }

    usr->field_1756 = -2;

    windp_arg87 v67;

    if (!call_method(yw->windp, 87, &v67) )
    {
        ypa_log_out("Error while remote start check\n");
        return  0;
    }

    if ( v67.field_41 )
    {
        strcpy(yw->GameShell->callSIGN, v67.callSIGN);

        if ( v67.field_40 )
            yw->GameShell->field_0x1cd8 = 1;
        else
            yw->GameShell->field_0x1cd8 = 0;

        yw->GameShell->field_1CEA = 1;

        usr->field_1C86 = 0;
        usr->field_46 = 6;

        windp_arg79 v68;

        if ( usr->field_0x1cd8 )
        {
            v68.field_0 = 0;
            v68.field_4 = 0;

            while ( call_method(yw->windp, 79, &v68) && strcasecmp(v68.field_8, usr->callSIGN) )
                v68.field_4++;

            usr->netTP1[v68.field_4].field_43 = 1;
            usr->field_1CE9 = 1;
            usr->field_1C3A = 3;
        }
        else
        {
            usr->field_1C3A = 4;
        }

        v68.field_0 = 0;
        v68.field_4 = 0;

        while ( call_method(yw->windp, 79, &v68) )
        {
            strncpy(yw->GameShell->netTP1[v68.field_4].field_50, v68.field_8, 64);
            v68.field_4++;
        }
        usr->field_2888 = 400;
        usr->field_288C = 400;
    }
    else
    {
        yw->GameShell->field_1CEA = 0;
    }

    usr->field_545B = 200000;

    if ( !usr->field_1CEA )
        ypaworld_func154__sub0(yw);

    return 1;
}


void ypaworld_func155(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}



void sub_4E6FEC(_NC_STACK_ypaworld *yw)
{
    stru_LevelNet *lvlnet;

    lvlnet = yw->LevelNet;
    if ( lvlnet->ilbm_menu_map )
    {
        delete_class_obj(lvlnet->ilbm_menu_map);
        lvlnet->ilbm_menu_map = NULL;
    }
    if ( lvlnet->ilbm_rollover_map )
    {
        delete_class_obj(lvlnet->ilbm_rollover_map);
        lvlnet->ilbm_rollover_map = NULL;
    }
    if ( lvlnet->ilbm_finished_map )
    {
        delete_class_obj(lvlnet->ilbm_finished_map);
        lvlnet->ilbm_finished_map = NULL;
    }
    if ( lvlnet->ilbm_enabled_map )
    {
        delete_class_obj(lvlnet->ilbm_enabled_map);
        lvlnet->ilbm_enabled_map = NULL;
    }
    if ( lvlnet->ilbm_mask_map )
    {
        delete_class_obj(lvlnet->ilbm_mask_map);
        lvlnet->ilbm_mask_map = NULL;
    }
}


void sb_0x4e75e8__sub1(_NC_STACK_ypaworld *yw, int a2)
{
    int v37 = 1;

    if ( yw->LevelNet->bg_n )
    {
        char buf[256];
        strcpy(buf, get_prefix_replacement("rsrc"));

        set_prefix_replacement("rsrc", "levels:");

        int v38 = 0;
        int v39 = 65535;
        for (int i = 0; i < yw->LevelNet->bg_n; i++)
        {

            int xx = (yw->LevelNet->background_map[i].size_x - yw->screen_width);
            int yy = (yw->LevelNet->background_map[i].size_y - yw->screen_height);

            int sq = sqrt(xx * xx + yy * yy);

            if (sq < v39)
            {
                v38 = i;
                v39 = sq;
            }
        }

        char *menu_map = NULL;
        char *rollover_map = NULL;
        char *mask_map = NULL;
        char *finished_map = NULL;
        char *enabled_map = NULL;

        NC_STACK_ilbm *ilbm_menu_map  = NULL;
        NC_STACK_ilbm *ilbm_rollover_map = NULL;
        NC_STACK_ilbm *ilbm_mask_map = NULL;
        NC_STACK_ilbm *ilbm_finished_map = NULL;
        NC_STACK_ilbm *ilbm_enabled_map = NULL;

        switch ( a2 )
        {
        case 1:
        case 2:
        case 3:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            menu_map  = yw->LevelNet->menu_map[v38].map_name;
            rollover_map = yw->LevelNet->settings_map[v38].map_name;
            break;
        case 4:
            menu_map  = yw->LevelNet->tut_background_map[v38].map_name;
            mask_map = yw->LevelNet->tut_mask_map[v38].map_name;
            rollover_map = yw->LevelNet->tut_rollover_map[v38].map_name;
            break;
        case 5:
            menu_map  = yw->LevelNet->background_map[v38].map_name;
            rollover_map = yw->LevelNet->rollover_map[v38].map_name;
            finished_map = yw->LevelNet->finished_map[v38].map_name;
            mask_map = yw->LevelNet->mask_map[v38].map_name;
            enabled_map = yw->LevelNet->enabled_map[v38].map_name;
            break;
        default:
            break;
        }

        if ( menu_map )
        {
            ilbm_menu_map = (NC_STACK_ilbm *)init_get_class("ilbm.class", 0x80001000, menu_map, 0x80002008, 1, 0x80002009, 1, 0);
            if ( !ilbm_menu_map )
            {
                ypa_log_out("world.ini: Could not load %s\n", menu_map);
                v37 = 0;
            }
        }

        if ( rollover_map )
        {
            ilbm_rollover_map = (NC_STACK_ilbm *)init_get_class("ilbm.class", 0x80001000, rollover_map, 0x80002008, 1, 0x80002009, 1, 0);
            if ( !ilbm_rollover_map )
            {
                ypa_log_out("world.ini: Could not load %s\n", rollover_map);
                v37 = 0;
            }
        }

        if ( finished_map )
        {
            ilbm_finished_map = (NC_STACK_ilbm *)init_get_class("ilbm.class", 0x80001000, finished_map, 0x80002008, 1, 0x80002009, 1, 0);
            if ( !ilbm_finished_map )
            {
                ypa_log_out("world.ini: Could not load %s\n", finished_map);
                v37 = 0;
            }
        }

        if ( enabled_map )
        {
            ilbm_enabled_map = (NC_STACK_ilbm *)init_get_class("ilbm.class", 0x80001000, enabled_map, 0x80002008, 1, 0x80002009, 1, 0);
            if ( !ilbm_enabled_map )
            {
                ypa_log_out("world.ini: Could not load %s\n", enabled_map);
                v37 = 0;
            }
        }
        if ( mask_map )
        {
            ilbm_mask_map = (NC_STACK_ilbm *)init_get_class("ilbm.class", 0x80001000, mask_map, 0);
            if ( !ilbm_mask_map )
            {
                ypa_log_out("world.ini: Could not load %s\n", mask_map);
                v37 = 0;
            }
        }

        set_prefix_replacement("rsrc", buf);

        if ( !v37 )
        {
            if ( ilbm_menu_map )
            {
                delete_class_obj(ilbm_menu_map);
                ilbm_menu_map = NULL;
            }
            if ( ilbm_rollover_map )
            {
                delete_class_obj(ilbm_rollover_map);
                ilbm_rollover_map = NULL;
            }
            if ( ilbm_finished_map )
            {
                delete_class_obj(ilbm_finished_map);
                ilbm_finished_map = NULL;
            }
            if ( ilbm_enabled_map )
            {
                delete_class_obj(ilbm_enabled_map);
                ilbm_enabled_map = NULL;
            }
            if ( ilbm_mask_map )
            {
                delete_class_obj(ilbm_mask_map);
                ilbm_mask_map = NULL;
            }
        }
        sub_4E6FEC(yw);
        yw->LevelNet->ilbm_menu_map = ilbm_menu_map;
        yw->LevelNet->ilbm_mask_map = ilbm_mask_map;
        yw->LevelNet->ilbm_rollover_map = ilbm_rollover_map;
        yw->LevelNet->ilbm_finished_map = ilbm_finished_map;
        yw->LevelNet->ilbm_enabled_map = ilbm_enabled_map;
    }
}

void sb_0x4e75e8__sub0(_NC_STACK_ypaworld *yw)
{
    lvlnet_t1 regions[256];

    if ( yw->LevelNet->ilbm_mask_map )
    {
        for (int i = 0; i < 256; i++)
        {
            regions[i].field_0 = 10000;
            regions[i].field_4 = 10000;
            regions[i].field_8 = -10000;
            regions[i].field_C = -10000;
        }

        bitmap_intern *bitm;
        call_vtbl(yw->LevelNet->ilbm_mask_map, 3, 0x80002000, &bitm, 0);

        for (int y = 0; y < bitm->height; y++ )
        {
            uint8_t *ln = ((uint8_t *)bitm->buffer + y * bitm->pitch);

            for (int x = 0; x < bitm->width; x++)
            {

                int v7 = ln[x];
                if ( v7 < 256 )
                {
                    lvlnet_t1 *v8 = &regions[v7];

                    if ( x < v8->field_0 )
                        v8->field_0 = x;

                    if ( x > v8->field_8 )
                        v8->field_8 = x;

                    if ( y < v8->field_4 )
                        v8->field_4 = y;

                    if ( y > v8->field_C )
                        v8->field_C = y;
                }
            }
        }

        for (int i = 0; i < 256; i++)
        {
            mapINFO *minf = &yw->LevelNet->mapInfos[i];

            if ( minf->field_0 && minf->field_0 != 4 && regions[i].field_0 != 10000 )
            {
                minf->field_9C.x1 = 2.0 * ((float)(regions[i].field_0) / (float)bitm->width) + -1.0;
                minf->field_9C.x2 = 2.0 * ((float)(regions[i].field_8) / (float)bitm->width) + -1.0;
                minf->field_9C.y1 = 2.0 * ((float)(regions[i].field_4) / (float)bitm->height) + -1.0;
                minf->field_9C.y2 = 2.0 * ((float)(regions[i].field_C) / (float)bitm->height) + -1.0;
            }
            else
            {
                minf->field_9C.x2 = 0;
                minf->field_9C.y1 = 0;
                minf->field_9C.y2 = 0;
                minf->field_9C.x1 = 0;
            }
        }
    }
}

void sb_0x4e75e8(_NC_STACK_ypaworld *yw, int a2)
{
    sb_0x4e75e8__sub1(yw, a2);
    if ( a2 >= 4 && a2 <= 5 )
    {
        yw->field_81AB = 0;
        yw->brief.field_2E68 = 0;
        yw->LevelNet->field_BE38 = 0;

        sb_0x4e75e8__sub0(yw);

        yw->TOD_ID = loadTOD(yw, "tod.def");

        int v6 = yw->TOD_ID + 1;

        if ( (v6 + 2490) > 2510 )
            v6 = 0;
        writeTOD(yw, "tod.def", v6);
    }
}

int ypaworld_func156__sub2(_NC_STACK_ypaworld *yw)
{
    NC_STACK_win3d *win3d;
    gfxEngine__getter(0x8000300D, &win3d, 0);

    int v5[4];
    v5[0] = -(yw->screen_width >> 1);
    v5[2] = yw->screen_width >> 1;
    v5[1] = -(yw->screen_height >> 1);
    v5[3] = yw->screen_height >> 1;

    call_method(win3d, 211, v5);
    sb_0x4e75e8(yw, yw->GameShell->field_46);
    return 1;
}

size_t ypaworld_func156(NC_STACK_ypaworld *obj, class_stru *zis, UserData *usr)
{
    _NC_STACK_ypaworld *yw = &obj->stack__ypaworld;

    if ( !yw->one_game_res )
    {
        int v247[2];
        v247[0] = yw->shell_default_res;
        v247[1] = 0;
        call_method(obj, 174, v247);
    }

    if ( !yw_LoadSet(yw, 46) )
    {
        ypa_log_out("Unable to load set for shell\n");
        return 0;
    }

    usr->field_3426 = 0;
    usr->field_D3A = 1;
    usr->field_5457 = 0;
    usr->p_ypaworld->icon_energy__h = 0;
    usr->field_D52 = 0;
    usr->p_ypaworld->field_81AF = NULL;
    usr->field_1CEF = 0;

    if ( usr->default_lang_dll )
    {
        char *v237 = usr->default_lang_dll->langDllName;
        if ( ! call_method(obj, 166, &v237) )
            ypa_log_out("Warning: Catalogue not found\n");
    }
    else
    {
        ypa_log_out("Warning: No Language selected, use default set\n");
    }

    ypaworld_func156__sub1(usr);

    if ( !ypaworld_func156__sub2(yw) )
    {
        ypa_log_out("Could not init level select stuff!\n");
        return 0;
    }

    gfxEngine__getter(0x8000300D, &yw->win3d, 0);

    if ( !yw->win3d )
    {
        ypa_log_out("No GfxObject in OpengameShell!\n");
        return 0;
    }

    displ_arg263 v233;

    v233.bitm = yw->pointers__bitm[0];
    v233.pointer_id = 1;

    call_method(yw->win3d, 263, &v233);

    fill_videmodes_list(usr);

    wdd_func324arg v227;
    v227.name = NULL;
    v227.guid = NULL;
    v227.currr = 0;

    int v261 = 0;
    int v3 = 0;

    while ( 1 )
    {
        call_method(yw->win3d, 324, &v227);
        if ( !v227.name )
            break;

        if ( v227.name )
        {
            if ( v227.currr & 1 )
            {
                strcpy(usr->win3d_guid, v227.guid);

                if ( !strcmp(v227.name, "software") )
                {
                    strcpy(usr->win3d_name, get_lang_string(yw->string_pointers_p2, 2472, "2472 = Software"));
                }
                else
                {
                    strcpy(usr->win3d_name, v227.name);
                }

                v3 = v261;
            }
            v261++;
        }
    }

    if ( usr->GFX_flags & 4 )
    {
        stack_vals tmp[2];
        tmp[0].id = 0x80005000;
        tmp[0].value = 1;
        tmp[1].id = 0;
        tmp[1].value = 0;

        call_method(usr->p_ypaworld->win3d, 2, &tmp);
    }
    else
    {
        stack_vals tmp[2];
        tmp[0].id = 0x80005000;
        tmp[0].value = 0;
        tmp[1].id = 0;
        tmp[1].value = 0;

        call_method(usr->p_ypaworld->win3d, 2, &tmp);
    }

    set_keys_vals(yw);

    usr->keyConfig[1].slider_name = get_lang_string(ypaworld__string_pointers, 544, "PAUSE");
    usr->keyConfig[2].slider_name = get_lang_string(ypaworld__string_pointers, 536, "QUIT");
    usr->keyConfig[3].slider_name = get_lang_string(ypaworld__string_pointers, 500, "DRIVE DIR");
    usr->keyConfig[4].slider_name = get_lang_string(ypaworld__string_pointers, 501, "DRIVE SPEED");
    usr->keyConfig[5].slider_name = get_lang_string(ypaworld__string_pointers, 511, "GUN HEIGHT");
    usr->keyConfig[6].slider_name = get_lang_string(ypaworld__string_pointers, 502, "FLY HEIGHT");
    usr->keyConfig[7].slider_name = get_lang_string(ypaworld__string_pointers, 503, "FLY SPEED");
    usr->keyConfig[8].slider_name = get_lang_string(ypaworld__string_pointers, 504, "FLY DIR");
    usr->keyConfig[9].slider_name = get_lang_string(ypaworld__string_pointers, 505, "STOP");
    usr->keyConfig[10].slider_name = get_lang_string(ypaworld__string_pointers, 506, "FIRE");
    usr->keyConfig[11].slider_name = get_lang_string(ypaworld__string_pointers, 507, "FIRE VIEW");
    usr->keyConfig[12].slider_name = get_lang_string(ypaworld__string_pointers, 508, "FIRE GUN");
    usr->keyConfig[13].slider_name = get_lang_string(ypaworld__string_pointers, 561, "MAKE CURRENT VEHICLE COMMANDER");
    usr->keyConfig[14].slider_name = get_lang_string(ypaworld__string_pointers, 541, "HEADUP DISPLAY");
    usr->keyConfig[15].slider_name = get_lang_string(ypaworld__string_pointers, 520, "AUTOPILOT");
    usr->keyConfig[16].slider_name = get_lang_string(ypaworld__string_pointers, 513, "ORDER");
    usr->keyConfig[17].slider_name = get_lang_string(ypaworld__string_pointers, 515, "NEW");
    usr->keyConfig[18].slider_name = get_lang_string(ypaworld__string_pointers, 516, "ADD");
    usr->keyConfig[19].slider_name = get_lang_string(ypaworld__string_pointers, 522, "FINDER");
    usr->keyConfig[20].slider_name = get_lang_string(ypaworld__string_pointers, 553, "AGGR: COME BACK");
    usr->keyConfig[21].slider_name = get_lang_string(ypaworld__string_pointers, 554, "AGGR: FIGHT TARGET");
    usr->keyConfig[22].slider_name = get_lang_string(ypaworld__string_pointers, 555, "AGGR: FIGHT ENEMIES TOO");
    usr->keyConfig[23].slider_name = get_lang_string(ypaworld__string_pointers, 556, "AGGR: CONQUER ALL ENEMY AREA TOO");
    usr->keyConfig[24].slider_name = get_lang_string(ypaworld__string_pointers, 557, "AGGR: GO AMOK");
    usr->keyConfig[25].slider_name = get_lang_string(ypaworld__string_pointers, 521, "MAP");
    usr->keyConfig[26].slider_name = get_lang_string(ypaworld__string_pointers, 558, "SELECT WAYPOINT");
    usr->keyConfig[27].slider_name = get_lang_string(ypaworld__string_pointers, 523, "LANDSCAPE");
    usr->keyConfig[28].slider_name = get_lang_string(ypaworld__string_pointers, 524, "OWNER");
    usr->keyConfig[29].slider_name = get_lang_string(ypaworld__string_pointers, 525, "HEIGHT");
    usr->keyConfig[30].slider_name = get_lang_string(ypaworld__string_pointers, 531, "MAP MINI");
    usr->keyConfig[31].slider_name = get_lang_string(ypaworld__string_pointers, 527, "LOCK VIEWER");
    usr->keyConfig[32].slider_name = get_lang_string(ypaworld__string_pointers, 529, "ZOOM IN");
    usr->keyConfig[33].slider_name = get_lang_string(ypaworld__string_pointers, 530, "ZOOM OUT");
    usr->keyConfig[34].slider_name = get_lang_string(ypaworld__string_pointers, 538, "LOGWIN");
    usr->keyConfig[35].slider_name = get_lang_string(ypaworld__string_pointers, 517, "CONTROL");
    usr->keyConfig[36].slider_name = get_lang_string(ypaworld__string_pointers, 560, "GOTO LAST OCCUPIED VEHICLE");
    usr->keyConfig[37].slider_name = get_lang_string(ypaworld__string_pointers, 514, "FIGHT");
    usr->keyConfig[38].slider_name = get_lang_string(ypaworld__string_pointers, 533, "TO ROBO");
    usr->keyConfig[39].slider_name = get_lang_string(ypaworld__string_pointers, 535, "TO COMMANDER");
    usr->keyConfig[40].slider_name = get_lang_string(ypaworld__string_pointers, 534, "NEXT MAN");
    usr->keyConfig[41].slider_name = get_lang_string(ypaworld__string_pointers, 532, "NEXT COM");
    usr->keyConfig[42].slider_name = get_lang_string(ypaworld__string_pointers, 543, "JUMP TO LASTMSG-SENDER");
    usr->keyConfig[43].slider_name = get_lang_string(ypaworld__string_pointers, 552, "MESSAGE TO ALL PLAYERS");
    usr->keyConfig[44].slider_name = get_lang_string(ypaworld__string_pointers, 559, "HELP");
    usr->keyConfig[45].slider_name = get_lang_string(ypaworld__string_pointers, 562, "SITUATION ANALYZER");

    int v259_4;

    if ( yw->screen_width < 512 )
    {
        word_5A50C0 = 2;
        word_5A50C2 = 2;
        v259_4 = 8;
        word_5A50AC = 250;
        dword_5A50B2 = 210;
        word_5A50AE = 200;
        word_5A50BC = 220;
        word_5A50BA = 300;
        word_5A50BE = 270;
    }
    else
    {
        word_5A50C0 = 3;
        word_5A50C2 = 3;
        v259_4 = 16;
        word_5A50AC = 450;
        dword_5A50B2 = 380;
        word_5A50AE = 280;
        word_5A50BC = 390;
        word_5A50BA = 500;
        word_5A50BE = 480;
    }

    int v278_4 = yw->screen_width * 0.7;
    int v285 = yw->screen_height * 0.8;

    int v278 = (yw->screen_width - v278_4) / 2;

    int v273 = yw->font_default_h;
    if ( yw->screen_width >= 512 )
        v273 += (yw->screen_height - 384) / 2;

    int v267 = 0;
    int v269;

    if ( yw->screen_width < 512 )
        v269 = v285 - yw->font_default_h;
    else
        v269 = v285 - yw->font_default_h - (yw->screen_height - 384) / 2;

    int v270 = ( v278_4 - 2 * word_5A50C0 )/ 3;

    int v258 = v269;

    int v262 = v270;

    int v264 = word_5A50C0 + v270;
    int v274 = 2 * word_5A50C0 + 2 * v270;

    int v276 = v269;
    int v298 = (v278_4 - 2 * word_5A50C0) / 3;

    usr->titel_button = (NC_STACK_button *)init_get_class("button.class", 0x80001003, 0, 0x80001004, 0, 0x80001005, yw->screen_width, 0x80001006, yw->screen_height, 0);
    if ( !usr->titel_button )
    {
        ypa_log_out("Unable to create Titel-Button-Object\n");
        return 0;
    }

    int v70 = 0;
    button_64_arg btn_64arg;

    btn_64arg.tileset_down = 19;
    btn_64arg.tileset_up = 18;
    btn_64arg.button_type = 1;
    btn_64arg.field_3A = 30;
    btn_64arg.xpos = yw->screen_width * 0.3328125;
    btn_64arg.ypos = yw->screen_height * 0.2291666666666666;
    btn_64arg.width = yw->screen_width / 3;
    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 80, "GAME");
    btn_64arg.caption2 = 0;
    btn_64arg.field_1C = 0;
    btn_64arg.down_id = 1251;
    btn_64arg.pressed_id = 0;
    btn_64arg.button_id = 1018;
    btn_64arg.up_id = 1024;
    btn_64arg.state = 112;
    btn_64arg.txt_r = yw->iniColors[68].r;
    btn_64arg.txt_g = yw->iniColors[68].g;
    btn_64arg.txt_b = yw->iniColors[68].b;

    if ( call_method(usr->titel_button, 64, &btn_64arg) )
    {
        btn_64arg.ypos = yw->screen_height * 0.3083333333333334;
        btn_64arg.field_1C = 0;
        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 81, "NETWORK");
        btn_64arg.caption2 = 0;
        btn_64arg.up_id = 1022;
        btn_64arg.pressed_id = 0;
        btn_64arg.down_id = 1251;
        btn_64arg.button_id = 1016;

        if ( call_method(usr->titel_button, 64, &btn_64arg) )
        {
            btn_64arg.xpos = yw->screen_width * 0.3328125;
            btn_64arg.ypos = yw->screen_height * 0.4333333333333334;
            btn_64arg.width = yw->screen_width / 3;
            btn_64arg.field_1C = 0;
            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 83, "INPUT");
            btn_64arg.caption2 = 0;
            btn_64arg.pressed_id = 0;
            btn_64arg.down_id = 1251;
            btn_64arg.button_id = 1003;
            btn_64arg.up_id = 1007;

            if ( call_method(usr->titel_button, 64, &btn_64arg) )
            {
                btn_64arg.ypos = yw->screen_height * 0.5125;
                btn_64arg.field_1C = 0;
                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 84, "SETTINGS");
                btn_64arg.caption2 = 0;
                btn_64arg.up_id = 1005;
                btn_64arg.pressed_id = 0;
                btn_64arg.down_id = 1251;
                btn_64arg.button_id = 1004;

                if ( call_method(usr->titel_button, 64, &btn_64arg) )
                {
                    btn_64arg.ypos = yw->screen_height * 0.5916666666666667;
                    btn_64arg.field_1C = 0;
                    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 85, "PLAYER");
                    btn_64arg.caption2 = 0;
                    btn_64arg.pressed_id = 0;
                    btn_64arg.down_id = 1251;
                    btn_64arg.up_id = 1001;
                    btn_64arg.button_id = 1001;

                    if ( call_method(usr->titel_button, 64, &btn_64arg) )
                    {
                        btn_64arg.xpos = yw->screen_width * 0.890625;
                        btn_64arg.ypos = yw->screen_height * 0.9583333333333334;
                        btn_64arg.width = yw->screen_width * 0.1;
                        btn_64arg.field_1C = 0;
                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 86, "LOCALE");
                        btn_64arg.caption2 = 0;
                        btn_64arg.up_id = 1011;
                        btn_64arg.pressed_id = 0;
                        btn_64arg.down_id = 1251;
                        btn_64arg.button_id = 1008;

                        if ( call_method(usr->titel_button, 64, &btn_64arg) )
                        {
                            btn_64arg.xpos = yw->screen_width * 0.3328125;
                            btn_64arg.ypos = yw->screen_height * 0.7166666666666667;
                            btn_64arg.width = yw->screen_width / 3;
                            btn_64arg.field_1C = 0;
                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 87, "HELP");
                            btn_64arg.caption2 = 0;
                            btn_64arg.pressed_id = 0;
                            btn_64arg.down_id = 1251;
                            btn_64arg.button_id = 1017;
                            btn_64arg.up_id = 1025;

                            if ( call_method(usr->titel_button, 64, &btn_64arg) )
                            {
                                btn_64arg.ypos = yw->screen_height * 0.7958333333333333;
                                btn_64arg.field_1C = 0;
                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 88, "QUIT");
                                btn_64arg.caption2 = 0;
                                btn_64arg.up_id = 1013;
                                btn_64arg.pressed_id = 0;
                                btn_64arg.down_id = 1251;
                                btn_64arg.button_id = 1007;

                                if ( call_method(usr->titel_button, 64, &btn_64arg) )
                                    v70 = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    if ( !v70 )
    {
        ypa_log_out("Unable to add button to Titel\n");
        return 0;
    }

    button_66arg v228;

    if ( usr->lang_dlls_count <= 1 )
    {

        v228.butID = 1008;
        call_method( usr->titel_button, 67, &v228);
    }

    int v238 = 2;
    call_method(usr->titel_button, 68, &v238);

    dword_5A50B6_h = yw->screen_width / 4 - 20;
    usr->sub_bar_button = (NC_STACK_button *)init_get_class("button.class", 0x80001003, 0, 0x80001004, yw->screen_height - yw->font_default_h, 0x80001005, yw->screen_width, 0x80001006, yw->font_default_h, 0);

    if ( !usr->sub_bar_button )
    {
        ypa_log_out("Unable to create Button-Object\n");
        return 0;
    }

    v70 = 0;

    btn_64arg.tileset_down = 19;
    btn_64arg.field_3A = 30;
    btn_64arg.ypos = 0;
    btn_64arg.field_1C = 0;
    btn_64arg.button_type = 1;
    btn_64arg.tileset_up = 18;
    btn_64arg.xpos = dword_5A50B6_h + word_5A50C0;
    btn_64arg.width = dword_5A50B6_h;
    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 640, "REWIND");
    btn_64arg.caption2 = 0;
    btn_64arg.down_id = 1251;
    btn_64arg.pressed_id = 0;
    btn_64arg.state = 112;
    btn_64arg.button_id = 1011;
    btn_64arg.up_id = 1016;

    if ( call_method(usr->sub_bar_button, 64, &btn_64arg) )
    {
        btn_64arg.xpos = 2 * (word_5A50C0 + dword_5A50B6_h);
        btn_64arg.field_1C = 0;
        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 641, "STEP FORWARD");
        btn_64arg.caption2 = 0;
        btn_64arg.down_id = 0;
        btn_64arg.up_id = 1020;
        btn_64arg.pressed_id = 1018;
        btn_64arg.button_id = 1013;

        if ( call_method(usr->sub_bar_button, 64, &btn_64arg) )
        {
            btn_64arg.xpos = 0;
            btn_64arg.field_1C = 0;
            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 643, "START GAME");
            btn_64arg.caption2 = 0;
            btn_64arg.up_id = 1019;
            btn_64arg.pressed_id = 0;
            btn_64arg.down_id = 1251;
            btn_64arg.button_id = 1014;

            if ( call_method(usr->sub_bar_button, 64, &btn_64arg) )
            {
                btn_64arg.xpos = (yw->screen_width - 3 * dword_5A50B6_h - 2 * word_5A50C0);
                btn_64arg.field_1C = 0;
                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 2422, "GOTO LOADSAVE");
                btn_64arg.caption2 = 0;
                btn_64arg.pressed_id = 0;
                btn_64arg.down_id = 1251;
                btn_64arg.button_id = 1020;
                btn_64arg.up_id = 1026;

                if ( call_method(usr->sub_bar_button, 64, &btn_64arg) )
                {
                    btn_64arg.xpos = (yw->screen_width - 2 * dword_5A50B6_h - word_5A50C0);
                    btn_64arg.field_1C = 0;
                    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 642, "LOAD GAME");
                    btn_64arg.caption2 = 0;
                    btn_64arg.up_id = 1021;
                    btn_64arg.pressed_id = 0;
                    btn_64arg.down_id = 1251;
                    btn_64arg.button_id = 1015;

                    if ( call_method(usr->sub_bar_button, 64, &btn_64arg) )
                    {
                        btn_64arg.field_1C = 0;
                        btn_64arg.xpos = yw->screen_width - dword_5A50B6_h;
                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 644, "GO BACK");
                        btn_64arg.caption2 = 0;
                        btn_64arg.pressed_id = 0;
                        btn_64arg.down_id = 1251;
                        btn_64arg.button_id = 1019;
                        btn_64arg.up_id = 1013;

                        if ( call_method(usr->sub_bar_button, 64, &btn_64arg) )
                        {
                            v70 = 1;
                        }
                    }
                }
            }
        }
    }
    if ( !v70 )
    {
        ypa_log_out("Unable to add button to sub-bar\n");
        return 0;
    }

    if ( usr->field_3426 != 1 )
    {
        v228.butID = 1015;
        v228.field_4 = 0;
        call_method(usr->sub_bar_button, 67, &v228);
    }

    v228.field_4 = 0;
    v228.butID = 1014;
    call_method(usr->sub_bar_button, 67, &v228);

    v228.butID = 1013;
    call_method(usr->sub_bar_button, 67, &v228);

    v228.butID = 1011;
    call_method(usr->sub_bar_button, 67, &v228);

    v238 = 2;
    call_method(usr->sub_bar_button, 68, &v238);

    usr->confirm_button = (NC_STACK_button *)init_get_class("button.class", 0x80001003, 0, 0x80001004, 0, 0x80001005, yw->screen_width, 0x80001006, yw->screen_height, 0);
    if ( !usr->confirm_button )
    {
        ypa_log_out("Unable to create Confirm-Button-Object\n");
        return 0;
    }

    btn_64arg.tileset_up = 18;
    btn_64arg.tileset_down = 19;
    btn_64arg.field_3A = 30;
    btn_64arg.button_type = 1;
    btn_64arg.xpos = yw->screen_width * 0.25;
    btn_64arg.ypos = yw->screen_height * 0.53125;
    btn_64arg.width = yw->screen_width * 0.125;
    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 2, "OK");
    btn_64arg.caption2 = 0;
    btn_64arg.field_1C = 0;
    btn_64arg.pressed_id = 0;
    btn_64arg.state = 112;
    btn_64arg.up_id = 1350;
    btn_64arg.down_id = 1251;
    btn_64arg.button_id = 1300;
    btn_64arg.txt_r = yw->iniColors[68].r;
    btn_64arg.txt_g = yw->iniColors[68].g;
    btn_64arg.txt_b = yw->iniColors[68].b;

    if ( call_method(usr->confirm_button, 64, &btn_64arg) )
    {
        btn_64arg.xpos = yw->screen_width * 0.625;
        btn_64arg.field_1C = 0;
        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 3, "CANCEL");
        btn_64arg.up_id = 1351;
        btn_64arg.caption2 = 0;
        btn_64arg.button_id = 1301;
        btn_64arg.down_id = 1251;
        btn_64arg.pressed_id = 0;

        if ( call_method(usr->confirm_button, 64, &btn_64arg) )
        {
            btn_64arg.tileset_down = 16;
            btn_64arg.button_type = 3;
            btn_64arg.tileset_up = 16;
            btn_64arg.field_3A = 16;
            btn_64arg.xpos = yw->screen_width * 0.25;
            btn_64arg.ypos = yw->screen_height * 0.4375;
            btn_64arg.caption = " ";
            btn_64arg.caption2 = 0;
            btn_64arg.field_1C = 0;
            btn_64arg.down_id = 0;
            btn_64arg.up_id = 0;
            btn_64arg.pressed_id = 0;
            btn_64arg.state = 96;
            btn_64arg.button_id = 1302;
            btn_64arg.width = yw->screen_width * 0.5;
            btn_64arg.txt_r = yw->iniColors[60].r;
            btn_64arg.txt_g = yw->iniColors[60].g;
            btn_64arg.txt_b = yw->iniColors[60].b;

            if ( call_method(usr->confirm_button, 64, &btn_64arg) )
            {
                btn_64arg.button_id = 1303;
                btn_64arg.ypos = yw->screen_height * 0.46875;
                btn_64arg.caption = " ";
                btn_64arg.state = 96;
                btn_64arg.caption2 = 0;

                call_method(usr->confirm_button, 64, &btn_64arg);
            }
        }
    }

    v228.butID = 1300;
    call_method(usr->confirm_button, 67, &v228);

    v228.butID = 1301;
    call_method(usr->confirm_button, 67, &v228);

    v238 = 2;
    call_method(usr->confirm_button, 68, &v238);

    dword_5A50B2_h = v278_4 - yw->font_yscrl_bkg_w;

    if ( !lstvw_init(yw, &usr->input_listview,
                     0x80000002,
                     0,
                     0x80000003,
                     45,
                     0x80000004,
                     8,
                     0x80000005,
                     0,
                     0x80000006,
                     0,
                     0x80000007,
                     8,
                     0x80000008,
                     0,
                     0x8000000B,
                     yw->font_default_h,
                     0x8000000C,
                     dword_5A50B2_h,
                     0x8000000F,
                     1,
                     0x80000010,
                     yw->field_1a38,
                     0x80000011,
                     0,
                     0x80000017,
                     1,
                     0) )
    {
        ypa_log_out("Unable to create Input-ListView\n");
        return 0;
    }

    usr->input_listview.frm_1.btn_xpos = v278;
    usr->input_listview.frm_1.btn_ypos = v273 + (word_5A50C2 + yw->font_default_h) * 4;

    usr->field_D5A = v278;
    usr->field_0xd5c = v273;

    usr->button_input_button = (NC_STACK_button *)init_get_class("button.class",
                               0x80001003,
                               (int)usr->field_D5A,
                               0x80001004,
                               (int)usr->field_0xd5c,
                               0x80001005,
                               (int)(yw->screen_width - usr->field_D5A),
                               0x80001006,
                               (int)(yw->screen_height - usr->field_0xd5c),
                               0);
    if ( !usr->button_input_button )
    {
        ypa_log_out("Unable to create Input-Button\n");
        return 0;
    }


    v70 = 0;
    btn_64arg.tileset_down = 16;
    btn_64arg.tileset_up = 16;
    btn_64arg.field_3A = 16;
    btn_64arg.xpos = 0;
    btn_64arg.button_type = 3;
    btn_64arg.ypos = 0;
    btn_64arg.width = v278_4;
    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 309, "INPUT SETTINGS");
    btn_64arg.down_id = 0;
    btn_64arg.caption2 = 0;
    btn_64arg.field_1C = 0;
    btn_64arg.up_id = 0;
    btn_64arg.pressed_id = 0;
    btn_64arg.button_id = 1057;
    btn_64arg.state = 64;
    btn_64arg.txt_r = yw->iniColors[68].r;
    btn_64arg.txt_g = yw->iniColors[68].g;
    btn_64arg.txt_b = yw->iniColors[68].b;

    if ( call_method(usr->button_input_button, 64, &btn_64arg) )
    {
        btn_64arg.xpos = 0;
        btn_64arg.ypos = word_5A50C2 + yw->font_default_h;
        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 310, "2");
        btn_64arg.caption2 = 0;
        btn_64arg.pressed_id = 0;
        btn_64arg.button_id = 1058;
        btn_64arg.txt_r = yw->iniColors[60].r;
        btn_64arg.txt_g = yw->iniColors[60].g;
        btn_64arg.txt_b = yw->iniColors[60].b;

        if ( call_method(usr->button_input_button, 64, &btn_64arg) )
        {
            btn_64arg.xpos = 0;
            btn_64arg.ypos = 2 * (yw->font_default_h + word_5A50C2);
            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 311, "3");
            btn_64arg.caption2 = 0;
            btn_64arg.pressed_id = 0;
            btn_64arg.button_id = 1059;

            if ( call_method(usr->button_input_button, 64, &btn_64arg) )
            {
                btn_64arg.xpos = 0;
                btn_64arg.ypos = 3 * (word_5A50C2 + yw->font_default_h);
                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 312, "4");
                btn_64arg.caption2 = 0;
                btn_64arg.pressed_id = 0;
                btn_64arg.button_id = 1060;

                if ( call_method(usr->button_input_button, 64, &btn_64arg) )
                {
                    btn_64arg.tileset_down = 19;
                    btn_64arg.field_3A = 30;
                    btn_64arg.tileset_up = 18;
                    btn_64arg.button_type = 2;
                    btn_64arg.xpos = v278_4 / 6;
                    btn_64arg.caption = "g";
                    btn_64arg.caption2 = "g";
                    btn_64arg.up_id = 1051;
                    btn_64arg.field_1C = 0;
                    btn_64arg.pressed_id = 0;
                    btn_64arg.state = 0;
                    btn_64arg.ypos = 6 * word_5A50C2 + 14 * yw->font_default_h;
                    btn_64arg.width = v259_4;
                    btn_64arg.down_id = 1050;
                    btn_64arg.button_id = 1050;

                    if ( call_method(usr->button_input_button, 64, &btn_64arg) )
                    {
                        btn_64arg.tileset_down = 16;
                        btn_64arg.tileset_up = 16;
                        btn_64arg.field_3A = 16;
                        btn_64arg.xpos = (v259_4 + word_5A50C0 + v278_4 / 6);
                        btn_64arg.button_type = 3;
                        btn_64arg.width = (v278_4 / 2 - word_5A50C0);
                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 305, "JOYSTICK");
                        btn_64arg.button_id = 2;
                        btn_64arg.caption2 = 0;
                        btn_64arg.field_1C = 0;
                        btn_64arg.down_id = 0;
                        btn_64arg.up_id = 0;
                        btn_64arg.pressed_id = 0;
                        btn_64arg.state = 64;
                        btn_64arg.txt_r = yw->iniColors[60].r;
                        btn_64arg.txt_g = yw->iniColors[60].g;
                        btn_64arg.txt_b = yw->iniColors[60].b;

                        if ( call_method(usr->button_input_button, 64, &btn_64arg) )
                        {
                            btn_64arg.tileset_down = 19;
                            btn_64arg.field_3A = 30;
                            btn_64arg.tileset_up = 18;
                            btn_64arg.caption = "g";
                            btn_64arg.caption2 = "g";
                            btn_64arg.button_type = 2;
                            btn_64arg.xpos = word_5A50C0 + (v278_4 / 2);
                            btn_64arg.width = v259_4;
                            btn_64arg.down_id = 1058;
                            btn_64arg.field_1C = 0;
                            btn_64arg.pressed_id = 0;
                            btn_64arg.button_id = 1061;
                            btn_64arg.up_id = 1059;
                            btn_64arg.state = 0;

                            if ( call_method(usr->button_input_button, 64, &btn_64arg) )
                            {
                                btn_64arg.tileset_down = 16;
                                btn_64arg.tileset_up = 16;
                                btn_64arg.field_3A = 16;
                                btn_64arg.button_type = 3;
                                btn_64arg.xpos = (v259_4 + (v278_4 / 2) + 2 * word_5A50C0);
                                btn_64arg.width = ((v278_4 / 2) - word_5A50C0);
                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 2433, "ALTERNATE JOYSTICK MODEL");
                                btn_64arg.caption2 = 0;
                                btn_64arg.field_1C = 0;
                                btn_64arg.down_id = 0;
                                btn_64arg.up_id = 0;
                                btn_64arg.pressed_id = 0;
                                btn_64arg.state = 64;
                                btn_64arg.button_id = 2;
                                btn_64arg.txt_r = yw->iniColors[60].r;
                                btn_64arg.txt_g = yw->iniColors[60].g;
                                btn_64arg.txt_b = yw->iniColors[60].b;

                                if ( call_method(usr->button_input_button, 64, &btn_64arg) )
                                {
                                    btn_64arg.tileset_down = 19;
                                    btn_64arg.field_3A = 30;
                                    btn_64arg.tileset_up = 18;
                                    btn_64arg.button_type = 2;
                                    btn_64arg.xpos = v278_4 / 3;
                                    btn_64arg.caption = "g";
                                    btn_64arg.caption2 = "g";
                                    btn_64arg.up_id = 1055;
                                    btn_64arg.button_id = 1055;
                                    btn_64arg.ypos = 7 * word_5A50C2 + (15 * yw->font_default_h);
                                    btn_64arg.field_1C = 0;
                                    btn_64arg.pressed_id = 0;
                                    btn_64arg.width = v259_4;
                                    btn_64arg.state = 0;
                                    btn_64arg.down_id = 1056;

                                    if ( call_method(usr->button_input_button, 64, &btn_64arg) )
                                    {
                                        btn_64arg.tileset_down = 16;
                                        btn_64arg.tileset_up = 16;
                                        btn_64arg.field_3A = 16;
                                        btn_64arg.button_type = 3;
                                        btn_64arg.xpos = (v259_4 + (v278_4 / 3) + word_5A50C0);
                                        btn_64arg.width = v278_4 / 2;
                                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 306, "DISABLE FORCE FEEDBACK");
                                        btn_64arg.button_id = 2;
                                        btn_64arg.caption2 = 0;
                                        btn_64arg.field_1C = 0;
                                        btn_64arg.down_id = 0;
                                        btn_64arg.state = 64;
                                        btn_64arg.up_id = 0;
                                        btn_64arg.pressed_id = 0;

                                        if ( call_method(usr->button_input_button, 64, &btn_64arg) )
                                        {
                                            btn_64arg.tileset_down = 19;
                                            btn_64arg.tileset_up = 18;
                                            btn_64arg.button_type = 1;
                                            btn_64arg.field_3A = 30;
                                            btn_64arg.xpos = v278_4 / 6;
                                            btn_64arg.ypos = 5 * word_5A50C2 + 13 * yw->font_default_h;
                                            btn_64arg.width = (v278_4 / 3 - word_5A50C0);
                                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 307, "SWITCH OFF");
                                            btn_64arg.down_id = 1251;
                                            btn_64arg.state = 112;
                                            btn_64arg.caption2 = 0;
                                            btn_64arg.field_1C = 0;
                                            btn_64arg.pressed_id = 0;
                                            btn_64arg.up_id = 1057;
                                            btn_64arg.button_id = 1056;
                                            btn_64arg.txt_r = yw->iniColors[68].r;
                                            btn_64arg.txt_g = yw->iniColors[68].g;
                                            btn_64arg.txt_b = yw->iniColors[68].b;

                                            if ( call_method(usr->button_input_button, 64, &btn_64arg) )
                                            {
                                                btn_64arg.xpos = word_5A50C0 + v278_4 / 2;
                                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 13, "RESET");
                                                btn_64arg.caption2 = 0;
                                                btn_64arg.field_1C = 0;
                                                btn_64arg.pressed_id = 0;
                                                btn_64arg.up_id = 1053;
                                                btn_64arg.button_id = 1053;

                                                if ( call_method(usr->button_input_button, 64, &btn_64arg) )
                                                {
                                                    btn_64arg.xpos = v267;
                                                    btn_64arg.ypos = v269;
                                                    btn_64arg.width = v270;
                                                    btn_64arg.button_type = 1;
                                                    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 2, "OK");
                                                    btn_64arg.caption2 = 0;
                                                    btn_64arg.field_1C = 0;
                                                    btn_64arg.pressed_id = 0;
                                                    btn_64arg.button_id = 1051;
                                                    btn_64arg.up_id = 1052;
                                                    btn_64arg.down_id = 1251;

                                                    if ( call_method(usr->button_input_button, 64, &btn_64arg) )
                                                    {
                                                        btn_64arg.xpos = v274;
                                                        btn_64arg.ypos = v258;
                                                        btn_64arg.width = v262;
                                                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 20, "HELP");
                                                        btn_64arg.up_id = 1250;
                                                        btn_64arg.caption2 = 0;
                                                        btn_64arg.button_id = 1052;
                                                        btn_64arg.field_1C = 0;
                                                        btn_64arg.pressed_id = 0;

                                                        if ( call_method(usr->button_input_button, 64, &btn_64arg) )
                                                        {
                                                            btn_64arg.xpos = v264;
                                                            btn_64arg.ypos = v276;
                                                            btn_64arg.width = v298;
                                                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 3, "CANCEL");
                                                            btn_64arg.up_id = 1054;
                                                            btn_64arg.button_id = 1054;
                                                            btn_64arg.caption2 = 0;
                                                            btn_64arg.field_1C = 0;
                                                            btn_64arg.pressed_id = 0;

                                                            if ( call_method(usr->button_input_button, 64, &btn_64arg) )
                                                            {
                                                                v70 = 1;
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
                    }
                }
            }
        }
    }
    if ( !v70 )
    {
        ypa_log_out("Unable to add input-button\n");
        return 0;
    }

    v238 = 2;
    call_method( usr->button_input_button, 68, &v238);

    int cnt = listCnt(&usr->video_mode_list);
    int v294 = v278_4 - 3 * word_5A50C0 - yw->font_yscrl_bkg_w;
    int v94 = (v278_4 - 3 * word_5A50C0 - yw->font_yscrl_bkg_w) * 0.6;

    if ( !lstvw_init(
                yw,
                &usr->video_listvw,
                0x80000002,
                0,
                0x80000003,
                cnt,
                0x80000004,
                4,
                0x80000005,
                0,
                0x80000006,
                0,
                0x80000007,
                4,
                0x80000008,
                0,
                0x8000000B,
                yw->font_default_h,
                0x8000000C,
                v94,
                0x8000000F,
                1,
                0x80000010,
                yw->field_1a38,
                0x80000011,
                1,
                0x80000017,
                1,
                0) )
    {
        ypa_log_out("Unable to create Game-Video-Menu\n");
        return 0;
    }

    if ( !lstvw_init(
                yw,
                &usr->d3d_listvw,
                0x80000002,
                0,
                0x80000003,
                v261,
                0x80000004,
                4,
                0x80000005,
                0,
                0x80000006,
                v3,
                0x80000007,
                4,
                0x80000008,
                0,
                0x8000000B,
                yw->font_default_h,
                0x8000000C,
                v94,
                0x8000000F,
                1,
                0x80000010,
                yw->field_1a38,
                0x80000011,
                1,
                0x80000017,
                1,
                0) )
    {
        ypa_log_out("Unable to create D3D-Menu\n");
        return 0;
    }

    usr->field_13AA = v278;
    usr->field_0x13ac = v273;

    usr->video_button = (NC_STACK_button *)init_get_class(
                            "button.class",
                            0x80001003,
                            usr->field_13AA,
                            0x80001004,
                            usr->field_0x13ac,
                            0x80001005,
                            yw->screen_width - usr->field_13AA,
                            0x80001006,
                            yw->screen_height - usr->field_0x13ac,
                            0);

    if ( !usr->video_button )
    {
        ypa_log_out("Unable to create Video-Button\n");
        return 0;
    }

    int v98 = v294 * 0.4;
    int v99 = v278 + word_5A50C0 + v98;

    usr->video_listvw.frm_1.btn_xpos = v99;
    usr->video_listvw.frm_1.btn_ypos = 6 * word_5A50C2 + 6 * yw->font_default_h + v273;

    usr->d3d_listvw.frm_1.btn_xpos = v99;
    usr->d3d_listvw.frm_1.btn_ypos = 7 * word_5A50C2 + 7 * yw->font_default_h + v273;

    btn_64arg.tileset_down = 16;
    btn_64arg.tileset_up = 16;
    btn_64arg.field_3A = 16;
    btn_64arg.button_type = 3;
    btn_64arg.xpos = 0;
    btn_64arg.ypos = 0;
    btn_64arg.width = v278_4;
    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 327, "GAME SETTINGS");
    btn_64arg.caption2 = 0;
    btn_64arg.field_1C = 0;
    btn_64arg.down_id = 0;
    btn_64arg.up_id = 0;
    btn_64arg.pressed_id = 0;
    btn_64arg.state = 64;
    btn_64arg.button_id = 1168;
    btn_64arg.txt_r = yw->iniColors[68].r;
    btn_64arg.txt_g = yw->iniColors[68].g;
    btn_64arg.txt_b = yw->iniColors[68].b;

    if ( call_method(usr->video_button, 64, &btn_64arg) )
    {
        btn_64arg.xpos = 0;
        btn_64arg.ypos = word_5A50C2 + yw->font_default_h;
        btn_64arg.width = v278_4;
        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 328, "2");
        btn_64arg.caption2 = 0;
        btn_64arg.button_id = 1169;
        btn_64arg.txt_r = yw->iniColors[60].r;
        btn_64arg.txt_g = yw->iniColors[60].g;
        btn_64arg.txt_b = yw->iniColors[60].b;

        if ( call_method(usr->video_button, 64, &btn_64arg) )
        {
            btn_64arg.xpos = 0;
            btn_64arg.ypos = 2 * (yw->font_default_h + word_5A50C2);
            btn_64arg.width = v278_4;
            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 329, "3");
            btn_64arg.button_id = 1170;
            btn_64arg.caption2 = 0;

            if ( call_method(usr->video_button, 64, &btn_64arg) )
            {
                btn_64arg.xpos = 0;
                btn_64arg.width = v278_4;
                btn_64arg.ypos = 3 * (yw->font_default_h + word_5A50C2);
                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 330, "4");
                btn_64arg.caption2 = 0;
                btn_64arg.button_id = 1171;

                if ( call_method(usr->video_button, 64, &btn_64arg) )
                {
                    btn_64arg.tileset_down = 16;
                    btn_64arg.tileset_up = 16;
                    btn_64arg.field_3A = 16;
                    btn_64arg.xpos = 0;
                    btn_64arg.button_type = 3;
                    btn_64arg.ypos = 5 * (yw->font_default_h + word_5A50C2);
                    btn_64arg.width = v98;
                    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 340, "RESOLUTION SHELL");
                    btn_64arg.caption2 = 0;
                    btn_64arg.field_1C = 0;
                    btn_64arg.down_id = 0;
                    btn_64arg.up_id = 0;
                    btn_64arg.pressed_id = 0;
                    btn_64arg.button_id = 2;
                    btn_64arg.state = 80;
                    btn_64arg.txt_r = yw->iniColors[60].r;
                    btn_64arg.txt_g = yw->iniColors[60].g;
                    btn_64arg.txt_b = yw->iniColors[60].b;

                    if ( call_method(usr->video_button, 64, &btn_64arg) )
                    {
                        video_mode_node *vnode = (video_mode_node *)usr->video_mode_list.head;
                        while (vnode->next)
                        {
                            if (vnode->sort_id == yw->game_default_res)
                                break;

                            vnode = (video_mode_node *)vnode->next;
                        }

                        btn_64arg.tileset_down = 19;
                        btn_64arg.field_3A = 30;
                        btn_64arg.button_type = 2;
                        btn_64arg.caption = vnode->name;
                        btn_64arg.caption2 = 0;
                        btn_64arg.field_1C = 0;
                        btn_64arg.pressed_id = 0;
                        btn_64arg.tileset_up = 18;
                        btn_64arg.down_id = 1100;
                        btn_64arg.button_id = 1156;
                        btn_64arg.xpos = word_5A50C0 + v294 * 0.4;
                        btn_64arg.up_id = 1101;
                        btn_64arg.state = 112;
                        btn_64arg.width = v294 * 0.6;
                        btn_64arg.txt_r = yw->iniColors[68].r;
                        btn_64arg.txt_g = yw->iniColors[68].g;
                        btn_64arg.txt_b = yw->iniColors[68].b;

                        if ( call_method(usr->video_button, 64, &btn_64arg) )
                        {
                            btn_64arg.tileset_down = 16;
                            btn_64arg.tileset_up = 16;
                            btn_64arg.field_3A = 16;
                            btn_64arg.button_type = 3;
                            btn_64arg.xpos = 0;
                            btn_64arg.ypos = 2 * (3 * (word_5A50C2 + yw->font_default_h));
                            btn_64arg.width = v294 * 0.4;
                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 352, "SELECT 3D DEVICE");
                            btn_64arg.caption2 = 0;
                            btn_64arg.field_1C = 0;
                            btn_64arg.down_id = 0;
                            btn_64arg.up_id = 0;
                            btn_64arg.pressed_id = 0;
                            btn_64arg.state = 80;
                            btn_64arg.button_id = 2;
                            btn_64arg.txt_r = yw->iniColors[60].r;
                            btn_64arg.txt_g = yw->iniColors[60].g;
                            btn_64arg.txt_b = yw->iniColors[60].b;

                            if ( call_method(usr->video_button, 64, &btn_64arg) )
                            {
                                btn_64arg.width = v294 * 0.6;
                                btn_64arg.tileset_down = 19;
                                btn_64arg.field_3A = 30;
                                btn_64arg.button_type = 2;
                                btn_64arg.down_id = 1134;
                                btn_64arg.up_id = 1135;
                                btn_64arg.tileset_up = 18;
                                btn_64arg.caption2 = 0;
                                btn_64arg.field_1C = 0;
                                btn_64arg.pressed_id = 0;
                                btn_64arg.xpos = word_5A50C0 + v294 * 0.4;
                                btn_64arg.state = 112;
                                btn_64arg.caption = usr->win3d_name;
                                btn_64arg.button_id = 1172;
                                btn_64arg.txt_r = yw->iniColors[68].r;
                                btn_64arg.txt_g = yw->iniColors[68].g;
                                btn_64arg.txt_b = yw->iniColors[68].b;

                                if ( call_method(usr->video_button, 64, &btn_64arg) )
                                {
                                    int v117 = dword_5A50B2 - 6 * word_5A50C0 - 2 * v259_4;

                                    btn_64arg.tileset_down = 19;
                                    btn_64arg.button_type = 2;
                                    btn_64arg.tileset_up = 18;
                                    btn_64arg.field_3A = 30;
                                    btn_64arg.xpos = 0;
                                    btn_64arg.caption = "g";
                                    btn_64arg.caption2 = "g";
                                    btn_64arg.down_id = 1102;
                                    btn_64arg.width = v259_4;
                                    btn_64arg.up_id = 1103;
                                    btn_64arg.ypos = 7 * (word_5A50C2 + yw->font_default_h);
                                    btn_64arg.field_1C = 0;
                                    btn_64arg.pressed_id = 0;
                                    btn_64arg.state = 0;
                                    btn_64arg.button_id = 1157;

                                    if ( call_method(usr->video_button, 64, &btn_64arg) )
                                    {
                                        int v120 = v117 / 2;

                                        btn_64arg.tileset_down = 16;
                                        btn_64arg.tileset_up = 16;
                                        btn_64arg.field_3A = 16;
                                        btn_64arg.xpos = v259_4 + word_5A50C0;
                                        btn_64arg.button_type = 3;
                                        btn_64arg.width = v120;
                                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 344, "FAR VIEW");
                                        btn_64arg.caption2 = 0;
                                        btn_64arg.field_1C = 0;
                                        btn_64arg.down_id = 0;
                                        btn_64arg.up_id = 0;
                                        btn_64arg.pressed_id = 0;
                                        btn_64arg.state = 64;
                                        btn_64arg.button_id = 2;
                                        btn_64arg.txt_r = yw->iniColors[60].r;
                                        btn_64arg.txt_g = yw->iniColors[60].g;
                                        btn_64arg.txt_b = yw->iniColors[60].b;

                                        if ( call_method(usr->video_button, 64, &btn_64arg) )
                                        {
                                            btn_64arg.tileset_down = 19;
                                            btn_64arg.tileset_up = 18;
                                            btn_64arg.width = v259_4;
                                            btn_64arg.caption = "g";
                                            btn_64arg.caption2 = "g";
                                            btn_64arg.field_3A = 30;
                                            btn_64arg.up_id = 1107;
                                            btn_64arg.button_type = 2;
                                            btn_64arg.down_id = 1106;
                                            btn_64arg.xpos = 3 * word_5A50C0 + v259_4 + v120;
                                            btn_64arg.field_1C = 0;
                                            btn_64arg.pressed_id = 0;
                                            btn_64arg.state = 0;
                                            btn_64arg.button_id = 1160;

                                            if ( call_method(usr->video_button, 64, &btn_64arg) )
                                            {
                                                btn_64arg.tileset_down = 16;
                                                btn_64arg.tileset_up = 16;
                                                btn_64arg.field_3A = 16;
                                                btn_64arg.width = v120;
                                                btn_64arg.xpos = 4 * word_5A50C0 + v120 + 2 * v259_4;
                                                btn_64arg.button_type = 3;
                                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 345, "HEAVEN");
                                                btn_64arg.caption2 = 0;
                                                btn_64arg.field_1C = 0;
                                                btn_64arg.down_id = 0;
                                                btn_64arg.up_id = 0;
                                                btn_64arg.pressed_id = 0;
                                                btn_64arg.button_id = 2;
                                                btn_64arg.state = 64;

                                                if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                {
                                                    btn_64arg.tileset_down = 19;
                                                    btn_64arg.tileset_up = 18;
                                                    btn_64arg.button_type = 2;
                                                    btn_64arg.xpos = 0;
                                                    btn_64arg.field_3A = 30;
                                                    btn_64arg.width = v259_4;
                                                    btn_64arg.caption = "g";
                                                    btn_64arg.caption2 = "g";
                                                    btn_64arg.field_1C = 0;
                                                    btn_64arg.pressed_id = 0;
                                                    btn_64arg.ypos = 8 * (yw->font_default_h + word_5A50C2);
                                                    btn_64arg.down_id = 1132;
                                                    btn_64arg.up_id = 1133;
                                                    btn_64arg.button_id = 1165;
                                                    btn_64arg.state = 0;

                                                    if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                    {
                                                        btn_64arg.tileset_down = 16;
                                                        btn_64arg.tileset_up = 16;
                                                        btn_64arg.field_3A = 16;
                                                        btn_64arg.width = v120;
                                                        btn_64arg.xpos = v259_4 + word_5A50C0;
                                                        btn_64arg.button_type = 3;
                                                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 350, "SW MOUSEPOINTER");
                                                        btn_64arg.caption2 = 0;
                                                        btn_64arg.field_1C = 0;
                                                        btn_64arg.down_id = 0;
                                                        btn_64arg.up_id = 0;
                                                        btn_64arg.pressed_id = 0;
                                                        btn_64arg.button_id = 2;
                                                        btn_64arg.state = 64;

                                                        if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                        {
                                                            btn_64arg.width = v259_4;
                                                            btn_64arg.tileset_down = 19;
                                                            btn_64arg.tileset_up = 18;
                                                            btn_64arg.button_type = 2;
                                                            btn_64arg.field_3A = 30;
                                                            btn_64arg.down_id = 1130;
                                                            btn_64arg.field_1C = 0;
                                                            btn_64arg.pressed_id = 0;
                                                            btn_64arg.state = 0;
                                                            btn_64arg.caption = "g";
                                                            btn_64arg.caption2 = "g";
                                                            btn_64arg.up_id = 1131;
                                                            btn_64arg.button_id = 1166;
                                                            btn_64arg.xpos = 3 * word_5A50C0 + v259_4 + v120;

                                                            if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                            {
                                                                btn_64arg.tileset_down = 16;
                                                                btn_64arg.tileset_up = 16;
                                                                btn_64arg.field_3A = 16;
                                                                btn_64arg.button_type = 3;
                                                                btn_64arg.xpos = 4 * word_5A50C0 + v120 + 2 * v259_4;
                                                                btn_64arg.width = v120;
                                                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 2432, "OPENGL LIKE (:-)");
                                                                btn_64arg.state = 64;
                                                                btn_64arg.caption2 = 0;
                                                                btn_64arg.field_1C = 0;
                                                                btn_64arg.down_id = 0;
                                                                btn_64arg.up_id = 0;
                                                                btn_64arg.pressed_id = 0;
                                                                btn_64arg.button_id = 2;

                                                                if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                                {
                                                                    btn_64arg.button_type = 3;
                                                                    btn_64arg.xpos = v259_4 + word_5A50C0;
                                                                    btn_64arg.ypos = 9 * (word_5A50C2 + yw->font_default_h);
                                                                    btn_64arg.width = v120;
                                                                    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 2431, "USE 16BIT TEXTURE");
                                                                    btn_64arg.state = 64;
                                                                    btn_64arg.caption2 = 0;
                                                                    btn_64arg.field_1C = 0;
                                                                    btn_64arg.down_id = 0;
                                                                    btn_64arg.up_id = 0;
                                                                    btn_64arg.pressed_id = 0;
                                                                    btn_64arg.button_id = 0;

                                                                    if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                                    {
                                                                        btn_64arg.width = v259_4;
                                                                        btn_64arg.tileset_down = 19;
                                                                        btn_64arg.tileset_up = 18;
                                                                        btn_64arg.button_type = 2;
                                                                        btn_64arg.pressed_id = 0;
                                                                        btn_64arg.state = 0;
                                                                        btn_64arg.field_3A = 30;
                                                                        btn_64arg.xpos = 0;
                                                                        btn_64arg.field_1C = 0;
                                                                        btn_64arg.button_id = 1150;
                                                                        btn_64arg.caption = "g";
                                                                        btn_64arg.caption2 = "g";
                                                                        btn_64arg.down_id = 1113;
                                                                        btn_64arg.up_id = 1114;

                                                                        if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                                        {
                                                                            btn_64arg.tileset_down = 16;
                                                                            btn_64arg.tileset_up = 16;
                                                                            btn_64arg.field_3A = 16;
                                                                            btn_64arg.button_type = 3;
                                                                            btn_64arg.xpos = v120 + 2 * v259_4 + 4 * word_5A50C0;
                                                                            btn_64arg.width = v120;
                                                                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 326, "ENABLE CD AUDIO");
                                                                            btn_64arg.caption2 = 0;
                                                                            btn_64arg.field_1C = 0;
                                                                            btn_64arg.down_id = 0;
                                                                            btn_64arg.up_id = 0;
                                                                            btn_64arg.pressed_id = 0;
                                                                            btn_64arg.button_id = 0;
                                                                            btn_64arg.state = 64;

                                                                            if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                                            {
                                                                                btn_64arg.width = v259_4;
                                                                                btn_64arg.tileset_down = 19;
                                                                                btn_64arg.tileset_up = 18;
                                                                                btn_64arg.button_type = 2;
                                                                                btn_64arg.field_3A = 30;
                                                                                btn_64arg.down_id = 1128;
                                                                                btn_64arg.field_1C = 0;
                                                                                btn_64arg.pressed_id = 0;
                                                                                btn_64arg.state = 0;
                                                                                btn_64arg.caption = "g";
                                                                                btn_64arg.caption2 = "g";
                                                                                btn_64arg.up_id = 1129;
                                                                                btn_64arg.button_id = 1164;
                                                                                btn_64arg.xpos = 3 * word_5A50C0 + v259_4 + v120;

                                                                                if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                                                {
                                                                                    btn_64arg.tileset_down = 19;
                                                                                    btn_64arg.field_3A = 30;
                                                                                    btn_64arg.tileset_up = 18;
                                                                                    btn_64arg.xpos = 0;
                                                                                    btn_64arg.button_type = 2;
                                                                                    btn_64arg.width = v259_4;
                                                                                    btn_64arg.caption = "g";
                                                                                    btn_64arg.caption2 = "g";
                                                                                    btn_64arg.ypos = 10 * (word_5A50C2 + yw->font_default_h);
                                                                                    btn_64arg.down_id = 1126;
                                                                                    btn_64arg.field_1C = 0;
                                                                                    btn_64arg.pressed_id = 0;
                                                                                    btn_64arg.button_id = 1163;
                                                                                    btn_64arg.up_id = 1127;
                                                                                    btn_64arg.state = 0;

                                                                                    if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                                                    {
                                                                                        btn_64arg.tileset_down = 16;
                                                                                        btn_64arg.tileset_up = 16;
                                                                                        btn_64arg.field_3A = 16;
                                                                                        btn_64arg.button_type = 3;
                                                                                        btn_64arg.xpos = v259_4 + word_5A50C0;
                                                                                        btn_64arg.width = v120 - v259_4;
                                                                                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 325, "ENEMY INDICATOR");
                                                                                        btn_64arg.caption2 = 0;
                                                                                        btn_64arg.field_1C = 0;
                                                                                        btn_64arg.down_id = 0;
                                                                                        btn_64arg.up_id = 0;
                                                                                        btn_64arg.pressed_id = 0;
                                                                                        btn_64arg.button_id = 0;
                                                                                        btn_64arg.state = 64;

                                                                                        if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                                                        {
                                                                                            btn_64arg.tileset_down = 16;
                                                                                            btn_64arg.tileset_up = 16;
                                                                                            btn_64arg.field_3A = 16;
                                                                                            btn_64arg.button_type = 3;
                                                                                            btn_64arg.xpos = v120 + 2 * v259_4 + 4 * word_5A50C0;
                                                                                            btn_64arg.width = v120;
                                                                                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 323, "INVERT LEFT-RIGHT DIVISION ");
                                                                                            btn_64arg.caption2 = 0;
                                                                                            btn_64arg.field_1C = 0;
                                                                                            btn_64arg.down_id = 0;
                                                                                            btn_64arg.up_id = 0;
                                                                                            btn_64arg.pressed_id = 0;
                                                                                            btn_64arg.button_id = 0;
                                                                                            btn_64arg.state = 64;

                                                                                            if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                                                            {
                                                                                                btn_64arg.width = v259_4;
                                                                                                btn_64arg.tileset_down = 19;
                                                                                                btn_64arg.tileset_up = 18;
                                                                                                btn_64arg.field_1C = 0;
                                                                                                btn_64arg.pressed_id = 0;
                                                                                                btn_64arg.state = 0;
                                                                                                btn_64arg.caption = "g";
                                                                                                btn_64arg.caption2 = "g";
                                                                                                btn_64arg.field_3A = 30;
                                                                                                btn_64arg.button_id = 1151;
                                                                                                btn_64arg.xpos = 3 * word_5A50C0 + v259_4 + v120;
                                                                                                btn_64arg.down_id = 1112;
                                                                                                btn_64arg.button_type = 2;
                                                                                                btn_64arg.up_id = 1111;

                                                                                                if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                                                                {
                                                                                                    btn_64arg.tileset_down = 16;
                                                                                                    btn_64arg.tileset_up = 16;
                                                                                                    btn_64arg.field_3A = 16;
                                                                                                    btn_64arg.button_type = 3;
                                                                                                    btn_64arg.xpos = 0;
                                                                                                    btn_64arg.ypos = 11 * (yw->font_default_h + word_5A50C2);
                                                                                                    btn_64arg.width = (dword_5A50B2 - 5 * word_5A50C0) * 0.3;
                                                                                                    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 343, "DESTRUCTION FX");
                                                                                                    btn_64arg.caption2 = 0;
                                                                                                    btn_64arg.field_1C = 0;
                                                                                                    btn_64arg.down_id = 0;
                                                                                                    btn_64arg.up_id = 0;
                                                                                                    btn_64arg.pressed_id = 0;
                                                                                                    btn_64arg.button_id = 2;
                                                                                                    btn_64arg.state = 64;

                                                                                                    if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                                                                    {
                                                                                                        button_str2_t2 v225;

                                                                                                        v225.field_0 = 8;
                                                                                                        v225.field_2 = 16;
                                                                                                        v225.field_4 = 0;

                                                                                                        btn_64arg.caption2 = 0;
                                                                                                        btn_64arg.field_1C = 0;
                                                                                                        btn_64arg.tileset_down = 18;
                                                                                                        btn_64arg.tileset_up = 18;
                                                                                                        btn_64arg.field_3A = 30;
                                                                                                        btn_64arg.button_type = 5;
                                                                                                        btn_64arg.pressed_id = 1110;
                                                                                                        btn_64arg.button_id = 1159;
                                                                                                        btn_64arg.xpos = word_5A50C0 + (dword_5A50B2 - 5 * word_5A50C0) * 0.3;
                                                                                                        btn_64arg.caption = " ";
                                                                                                        btn_64arg.down_id = 1108;
                                                                                                        btn_64arg.state = 0;
                                                                                                        btn_64arg.field_34 = &v225;
                                                                                                        btn_64arg.up_id = 1109;
                                                                                                        btn_64arg.width = (dword_5A50B2 - 5 * word_5A50C0) * 0.55;

                                                                                                        if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                                                                        {
                                                                                                            btn_64arg.tileset_down = 16;
                                                                                                            btn_64arg.tileset_up = 16;
                                                                                                            btn_64arg.field_3A = 16;
                                                                                                            btn_64arg.button_type = 3;
                                                                                                            btn_64arg.caption2 = 0;
                                                                                                            btn_64arg.field_1C = 0;
                                                                                                            btn_64arg.xpos = word_5A50C0 + (dword_5A50B2 - 5 * word_5A50C0) * 0.85;
                                                                                                            btn_64arg.down_id = 0;
                                                                                                            btn_64arg.up_id = 0;
                                                                                                            btn_64arg.pressed_id = 0;
                                                                                                            btn_64arg.width = (dword_5A50B2 - 5 * word_5A50C0) * 0.15;
                                                                                                            btn_64arg.button_id = 1158;
                                                                                                            btn_64arg.caption = " 4";
                                                                                                            btn_64arg.state = 112;

                                                                                                            if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                                                                            {
                                                                                                                btn_64arg.button_type = 3;
                                                                                                                btn_64arg.xpos = 0;
                                                                                                                btn_64arg.ypos = 12 * (word_5A50C2 + yw->font_default_h);
                                                                                                                btn_64arg.width = (dword_5A50B2 - 5 * word_5A50C0) * 0.3;
                                                                                                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 321, "FX VOLUME");
                                                                                                                btn_64arg.caption2 = 0;
                                                                                                                btn_64arg.field_1C = 0;
                                                                                                                btn_64arg.down_id = 0;
                                                                                                                btn_64arg.up_id = 0;
                                                                                                                btn_64arg.pressed_id = 0;
                                                                                                                btn_64arg.button_id = 2;
                                                                                                                btn_64arg.state = 64;

                                                                                                                if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                                                                                {
                                                                                                                    btn_64arg.width = (dword_5A50B2 - 5 * word_5A50C0) * 0.55;

                                                                                                                    v225.field_4 = 1;
                                                                                                                    v225.field_2 = 127;
                                                                                                                    v225.field_0 = 100;

                                                                                                                    btn_64arg.field_3A = 30;
                                                                                                                    btn_64arg.tileset_down = 18;
                                                                                                                    btn_64arg.tileset_up = 18;
                                                                                                                    btn_64arg.button_type = 5;
                                                                                                                    btn_64arg.caption2 = 0;
                                                                                                                    btn_64arg.field_1C = 0;
                                                                                                                    btn_64arg.button_id = 1152;
                                                                                                                    btn_64arg.xpos = word_5A50C0 + (dword_5A50B2 - 5 * word_5A50C0) * 0.3;
                                                                                                                    btn_64arg.caption = " ";
                                                                                                                    btn_64arg.down_id = 1115;
                                                                                                                    btn_64arg.up_id = 1117;
                                                                                                                    btn_64arg.field_34 = &v225;
                                                                                                                    btn_64arg.pressed_id = 1116;
                                                                                                                    btn_64arg.state = 0;

                                                                                                                    if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                                                                                    {
                                                                                                                        btn_64arg.button_type = 3;
                                                                                                                        btn_64arg.tileset_down = 16;
                                                                                                                        btn_64arg.tileset_up = 16;
                                                                                                                        btn_64arg.field_3A = 16;
                                                                                                                        btn_64arg.caption = "4";
                                                                                                                        btn_64arg.button_id = 1153;
                                                                                                                        btn_64arg.caption2 = 0;
                                                                                                                        btn_64arg.xpos = (2 * word_5A50C0) + (dword_5A50B2 - 5 * word_5A50C0) * 0.85;
                                                                                                                        btn_64arg.field_1C = 0;
                                                                                                                        btn_64arg.width = (dword_5A50B2 - 5 * word_5A50C0) * 0.15;
                                                                                                                        btn_64arg.down_id = 0;
                                                                                                                        btn_64arg.state = 96;
                                                                                                                        btn_64arg.up_id = 0;
                                                                                                                        btn_64arg.pressed_id = 0;

                                                                                                                        if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                                                                                        {
                                                                                                                            btn_64arg.button_type = 3;
                                                                                                                            btn_64arg.xpos = 0;
                                                                                                                            btn_64arg.width = (dword_5A50B2 - 5 * word_5A50C0) * 0.3;
                                                                                                                            btn_64arg.ypos = 13 * (word_5A50C2 + yw->font_default_h);
                                                                                                                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 324, "CD VOLUME");
                                                                                                                            btn_64arg.caption2 = 0;
                                                                                                                            btn_64arg.field_1C = 0;
                                                                                                                            btn_64arg.down_id = 0;
                                                                                                                            btn_64arg.up_id = 0;
                                                                                                                            btn_64arg.pressed_id = 0;
                                                                                                                            btn_64arg.state = 64;
                                                                                                                            btn_64arg.button_id = 2;

                                                                                                                            if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                                                                                            {
                                                                                                                                btn_64arg.width = (dword_5A50B2 - 5 * word_5A50C0) * 0.55;
                                                                                                                                v225.field_4 = 1;
                                                                                                                                v225.field_2 = 127;
                                                                                                                                v225.field_0 = 100;

                                                                                                                                btn_64arg.tileset_down = 18;
                                                                                                                                btn_64arg.tileset_up = 18;
                                                                                                                                btn_64arg.up_id = 1120;
                                                                                                                                btn_64arg.field_3A = 30;
                                                                                                                                btn_64arg.button_type = 5;
                                                                                                                                btn_64arg.caption2 = 0;
                                                                                                                                btn_64arg.field_1C = 0;
                                                                                                                                btn_64arg.down_id = 1118;
                                                                                                                                btn_64arg.xpos = word_5A50C0 + (dword_5A50B2 - 5 * word_5A50C0) * 0.3;
                                                                                                                                btn_64arg.caption = " ";
                                                                                                                                btn_64arg.pressed_id = 1119;
                                                                                                                                btn_64arg.field_34 = &v225;
                                                                                                                                btn_64arg.state = 0;
                                                                                                                                btn_64arg.button_id = 1154;

                                                                                                                                if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                                                                                                {
                                                                                                                                    btn_64arg.tileset_down = 16;
                                                                                                                                    btn_64arg.tileset_up = 16;
                                                                                                                                    btn_64arg.field_3A = 16;
                                                                                                                                    btn_64arg.button_type = 3;
                                                                                                                                    btn_64arg.caption = "4";
                                                                                                                                    btn_64arg.caption2 = 0;
                                                                                                                                    btn_64arg.field_1C = 0;
                                                                                                                                    btn_64arg.down_id = 0;
                                                                                                                                    btn_64arg.up_id = 0;
                                                                                                                                    btn_64arg.pressed_id = 0;
                                                                                                                                    btn_64arg.xpos = (2 * word_5A50C0) + (dword_5A50B2 - 5 * word_5A50C0) * 0.85;
                                                                                                                                    btn_64arg.width = (dword_5A50B2 - 5 * word_5A50C0) * 0.15;
                                                                                                                                    btn_64arg.button_id = 1155;
                                                                                                                                    btn_64arg.state = 96;

                                                                                                                                    if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                                                                                                    {
                                                                                                                                        btn_64arg.tileset_up = 18;
                                                                                                                                        btn_64arg.field_3A = 30;
                                                                                                                                        btn_64arg.button_type = 1;
                                                                                                                                        btn_64arg.xpos = v267;
                                                                                                                                        btn_64arg.ypos = v269;
                                                                                                                                        btn_64arg.width = v270;
                                                                                                                                        btn_64arg.tileset_down = 19;
                                                                                                                                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 2, "OK");
                                                                                                                                        btn_64arg.up_id = 1124;
                                                                                                                                        btn_64arg.caption2 = 0;
                                                                                                                                        btn_64arg.field_1C = 0;
                                                                                                                                        btn_64arg.down_id = 0;
                                                                                                                                        btn_64arg.pressed_id = 0;
                                                                                                                                        btn_64arg.state = 112;
                                                                                                                                        btn_64arg.button_id = 1161;
                                                                                                                                        btn_64arg.txt_r = yw->iniColors[68].r;
                                                                                                                                        btn_64arg.txt_g = yw->iniColors[68].g;
                                                                                                                                        btn_64arg.txt_b = yw->iniColors[68].b;

                                                                                                                                        if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                                                                                                        {
                                                                                                                                            btn_64arg.xpos = v274;
                                                                                                                                            btn_64arg.ypos = v258;
                                                                                                                                            btn_64arg.width = v262;
                                                                                                                                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 20, "HELP");
                                                                                                                                            btn_64arg.up_id = 1250;
                                                                                                                                            btn_64arg.caption2 = 0;
                                                                                                                                            btn_64arg.field_1C = 0;
                                                                                                                                            btn_64arg.down_id = 0;
                                                                                                                                            btn_64arg.pressed_id = 0;
                                                                                                                                            btn_64arg.button_id = 1167;

                                                                                                                                            if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                                                                                                            {
                                                                                                                                                btn_64arg.xpos = v264;
                                                                                                                                                btn_64arg.ypos = v276;
                                                                                                                                                btn_64arg.width = v298;
                                                                                                                                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 3, "CANCEL");
                                                                                                                                                btn_64arg.up_id = 1125;
                                                                                                                                                btn_64arg.caption2 = 0;
                                                                                                                                                btn_64arg.field_1C = 0;
                                                                                                                                                btn_64arg.down_id = 0;
                                                                                                                                                btn_64arg.pressed_id = 0;
                                                                                                                                                btn_64arg.button_id = 1162;

                                                                                                                                                if ( call_method(usr->video_button, 64, &btn_64arg) )
                                                                                                                                                {
                                                                                                                                                    v70 = 1;
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
                        }
                    }
                }
            }
        }
    }
    if ( !v70 )
    {
        ypa_log_out("Unable to add video-button\n");
        return 0;
    }

    button_66arg v229;
    v229.butID = 1151;
    v229.field_4 = ((usr->snd__flags2 & 1) == 0) + 1;

    call_method(usr->video_button, 73, &v229);


    v238 = 2;
    call_method(usr->video_button, 68, &v238);

    word_5A50B0 = v278_4;

    if ( !lstvw_init(
                yw,
                &usr->disk_listvw,
                0x80000002,
                0,
                0x80000003,
                listCnt(&usr->files_list),
                0x80000004,
                10,
                0x80000005,
                0,
                0x80000006,
                0,
                0x80000007,
                10,
                0x80000008,
                0,
                0x8000000B,
                yw->font_default_h,
                0x8000000C,
                v278_4,
                0x8000000F,
                1,
                0x80000010,
                yw->field_1a38,
                0x80000011,
                0,
                0x80000017,
                1,
                0) )
    {
        ypa_log_out("Unable to create disk-listview\n");
        return 0;
    }

    usr->field_0x175c = v278;
    usr->field_175E = v273;

    usr->disk_button = (NC_STACK_button *)init_get_class(
                           "button.class",
                           0x80001003,
                           usr->field_0x175c,
                           0x80001004,
                           usr->field_175E,
                           0x80001005,
                           yw->screen_width - usr->field_0x175c,
                           0x80001006,
                           yw->screen_height - usr->field_175E,
                           0);
    if ( !usr->disk_button )
    {
        ypa_log_out("Unable to create disk-buttonobject\n");
        return 0;
    }

    usr->field_0x1758 = v278;
    usr->field_175A = 4 * (word_5A50C2 + yw->font_default_h) + v273;

    usr->disk_listvw.frm_1.btn_xpos = usr->field_0x1758;
    usr->disk_listvw.frm_1.btn_ypos = usr->field_175A;

    strcpy(usr->usernamedir, usr->user_name);

    usr->usernamedir_len = strlen(usr->usernamedir);

    char v223[300];

    if ( usr->field_0x1744 )
        sprintf(v223, "%s%s", usr->usernamedir, "h");
    else
        sprintf(v223, "%s", usr->usernamedir);


    v70 = 0;

    btn_64arg.tileset_down = 16;
    btn_64arg.tileset_up = 16;
    btn_64arg.field_3A = 16;
    btn_64arg.button_type = 3;

    btn_64arg.xpos = 0;
    btn_64arg.ypos = 0;
    btn_64arg.width = v278_4;
    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 367, "LOAD, CREATE OR DELETE PLAYER");
    btn_64arg.caption2 = 0;
    btn_64arg.button_id = 1108;
    btn_64arg.state = 64;
    btn_64arg.txt_r = yw->iniColors[68].r;
    btn_64arg.txt_g = yw->iniColors[68].g;
    btn_64arg.txt_b = yw->iniColors[68].b;

    if ( call_method(usr->disk_button, 64, &btn_64arg) )
    {
        btn_64arg.xpos = 0;
        btn_64arg.ypos = word_5A50C0 + yw->font_default_h;
        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 368, "2");
        btn_64arg.button_id = 1109;
        btn_64arg.caption2 = 0;
        btn_64arg.txt_r = yw->iniColors[60].r;
        btn_64arg.txt_g = yw->iniColors[60].g;
        btn_64arg.txt_b = yw->iniColors[60].b;

        if ( call_method(usr->disk_button, 64, &btn_64arg))
        {
            btn_64arg.xpos = 0;
            btn_64arg.ypos = 2 * (yw->font_default_h + word_5A50C0);
            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 369, "3");
            btn_64arg.caption2 = 0;
            btn_64arg.button_id = 1110;

            if ( call_method(usr->disk_button, 64, &btn_64arg) )
            {
                btn_64arg.xpos = 0;
                btn_64arg.ypos = 3 * (word_5A50C0 + yw->font_default_h);
                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 370, "4");
                btn_64arg.caption2 = 0;
                btn_64arg.button_id = 1111;

                if ( call_method(usr->disk_button, 64, &btn_64arg) )
                {
                    btn_64arg.tileset_down = 17;
                    btn_64arg.tileset_up = 17;
                    btn_64arg.field_3A = 17;
                    btn_64arg.xpos = 0;
                    btn_64arg.button_type = 3;
                    btn_64arg.width = v278_4;
                    btn_64arg.caption2 = 0;
                    btn_64arg.field_1C = 0;
                    btn_64arg.down_id = 0;
                    btn_64arg.up_id = 0;
                    btn_64arg.pressed_id = 0;
                    btn_64arg.caption = v223;
                    btn_64arg.state = 112;
                    btn_64arg.button_id = 1100;
                    btn_64arg.ypos = 6 * word_5A50C0 + 14 * yw->font_default_h;

                    if ( call_method(usr->disk_button, 64, &btn_64arg) )
                    {
                        btn_64arg.tileset_down = 19;
                        btn_64arg.tileset_up = 18;
                        btn_64arg.field_3A = 30;
                        btn_64arg.button_type = 1;
                        btn_64arg.xpos = word_5A50C0 + (v278_4 - 3 * word_5A50C0) * 0.25;
                        btn_64arg.ypos = 7 * word_5A50C0 + 15 * yw->font_default_h;
                        btn_64arg.width = (v278_4 - 3 * word_5A50C0) * 0.25;
                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 360, "LOAD");
                        btn_64arg.down_id = 1251;
                        btn_64arg.up_id = 1160;
                        btn_64arg.caption2 = 0;
                        btn_64arg.field_1C = 0;
                        btn_64arg.pressed_id = 0;
                        btn_64arg.button_id = 1101;
                        btn_64arg.state = 112;
                        btn_64arg.txt_r = yw->iniColors[68].r;
                        btn_64arg.txt_g = yw->iniColors[68].g;
                        btn_64arg.txt_b = yw->iniColors[68].b;

                        if ( call_method(usr->disk_button, 64, &btn_64arg) )
                        {
                            btn_64arg.xpos = (3 * word_5A50C0) + (v278_4 - 3 * word_5A50C0) * 0.75;
                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 362, "DELETE");
                            btn_64arg.caption2 = 0;
                            btn_64arg.up_id = 1161;
                            btn_64arg.button_id = 1102;

                            if ( call_method(usr->disk_button, 64, &btn_64arg) )
                            {
                                btn_64arg.xpos = 0;
                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 363, "NEW GAME");
                                btn_64arg.button_id = 1103;
                                btn_64arg.caption2 = 0;
                                btn_64arg.up_id = 1162;

                                if ( call_method(usr->disk_button, 64, &btn_64arg) )
                                {
                                    btn_64arg.xpos = (2 * word_5A50C0) + (v278_4 - 3 * word_5A50C0) * 0.5;
                                    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 361, "SAVE");
                                    btn_64arg.button_id = 1104;
                                    btn_64arg.caption2 = 0;
                                    btn_64arg.up_id = 1163;

                                    if ( call_method(usr->disk_button, 64, &btn_64arg) )
                                    {
                                        btn_64arg.xpos = v267;
                                        btn_64arg.ypos = v269;
                                        btn_64arg.width = v270;
                                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 2, "OK");
                                        btn_64arg.caption2 = 0;
                                        btn_64arg.field_1C = 0;
                                        btn_64arg.down_id = 1251;
                                        btn_64arg.button_id = 1105;
                                        btn_64arg.pressed_id = 0;
                                        btn_64arg.up_id = 1164;

                                        if ( call_method(usr->disk_button, 64, &btn_64arg) )
                                        {
                                            btn_64arg.ypos = v258;
                                            btn_64arg.width = v262;
                                            btn_64arg.xpos = v274;
                                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 20, "HELP");
                                            btn_64arg.button_id = 1107;
                                            btn_64arg.caption2 = 0;
                                            btn_64arg.up_id = 1250;

                                            if ( call_method(usr->disk_button, 64, &btn_64arg) )
                                            {
                                                btn_64arg.ypos = v276;
                                                btn_64arg.width = v298;
                                                btn_64arg.xpos = v264;
                                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 3, "CANCEL");
                                                btn_64arg.button_id = 1106;
                                                btn_64arg.caption2 = 0;
                                                btn_64arg.up_id = 1165;

                                                if ( call_method(usr->disk_button, 64, &btn_64arg) )
                                                {
                                                    v70 = 1;
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
        }
    }
    if ( !v70 )
        ypa_log_out("Unable to add button to disk-buttonobject\n");


    v238 = 2;
    call_method(usr->disk_button, 68, &v238);

    v228.field_4 = 0;
    v228.butID = 1105;
    call_method(usr->disk_button, 67, &v228);

    if ( !lstvw_init(
                yw,
                &usr->local_listvw,
                0x80000002,
                0,
                0x80000003,
                10,
                0x80000004,
                10,
                0x80000005,
                0,
                0x80000006,
                0,
                0x80000007,
                10,
                0x80000008,
                0,
                0x8000000B,
                yw->font_default_h,
                0x8000000C,
                v278_4 - yw->font_yscrl_bkg_w,
                0x8000000F,
                1,
                0x80000010,
                yw->field_1a38,
                0x80000011,
                0,
                0x80000017,
                1,
                0) )
    {
        ypa_log_out("Unable to create local-listview\n");
        return 0;
    }

    usr->field_19C6 = v278;
    usr->field_0x19c8 = v273;

    usr->locale_button = (NC_STACK_button *)init_get_class(
                             "button.class",
                             0x80001003,
                             usr->field_19C6,
                             0x80001004,
                             usr->field_0x19c8,
                             0x80001005,
                             yw->screen_width - usr->field_19C6,
                             0x80001006,
                             yw->screen_height - usr->field_0x19c8,
                             0);
    if ( !usr->locale_button )
    {
        ypa_log_out("Unable to create locale-buttonobject\n");
        return 0;
    }

    usr->local_listvw.frm_1.btn_xpos = v278;
    usr->local_listvw.frm_1.btn_ypos = 4 * (word_5A50C2 + yw->font_default_h) + v273;

    v70 = 0;
    btn_64arg.tileset_down = 16;
    btn_64arg.tileset_up = 16;
    btn_64arg.button_type = 3;
    btn_64arg.xpos = 0;
    btn_64arg.width = v278_4;
    btn_64arg.field_3A = 30;
    btn_64arg.ypos = 0;
    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 395, "SELECT NEW LANGUAGE");
    btn_64arg.caption2 = 0;
    btn_64arg.field_1C = 0;
    btn_64arg.down_id = 0;
    btn_64arg.up_id = 0;
    btn_64arg.pressed_id = 0;
    btn_64arg.state = 64;
    btn_64arg.button_id = 1253;
    btn_64arg.txt_r = yw->iniColors[68].r;
    btn_64arg.txt_g = yw->iniColors[68].g;
    btn_64arg.txt_b = yw->iniColors[68].b;

    if ( call_method(usr->locale_button, 64, &btn_64arg) )
    {
        btn_64arg.xpos = 0;
        btn_64arg.ypos = word_5A50C2 + yw->font_default_h;
        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 396, "2");
        btn_64arg.caption2 = 0;
        btn_64arg.button_id = 1254;
        btn_64arg.txt_r = yw->iniColors[60].r;
        btn_64arg.txt_g = yw->iniColors[60].g;
        btn_64arg.txt_b = yw->iniColors[60].b;

        if ( call_method(usr->locale_button, 64, &btn_64arg) )
        {
            btn_64arg.xpos = 0;
            btn_64arg.ypos = 2 * (yw->font_default_h + word_5A50C2);
            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 397, "3");
            btn_64arg.caption2 = 0;
            btn_64arg.button_id = 1255;

            if ( call_method(usr->locale_button, 64, &btn_64arg) )
            {
                btn_64arg.xpos = 0;
                btn_64arg.ypos = 3 * (word_5A50C2 + yw->font_default_h);
                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 398, "4");
                btn_64arg.caption2 = 0;
                btn_64arg.button_id = 1256;

                if ( call_method(usr->locale_button, 64, &btn_64arg) )
                {
                    btn_64arg.tileset_down = 19;
                    btn_64arg.tileset_up = 18;
                    btn_64arg.button_type = 1;
                    btn_64arg.field_3A = 30;
                    btn_64arg.xpos = v267 - (usr->field_19C6 - v278);
                    btn_64arg.ypos = v269 - (usr->field_0x19c8 - v273);
                    btn_64arg.width = v270;
                    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 2, "OK");
                    btn_64arg.caption2 = 0;
                    btn_64arg.field_1C = 0;
                    btn_64arg.down_id = 0;
                    btn_64arg.pressed_id = 0;
                    btn_64arg.up_id = 1300;
                    btn_64arg.state = 112;
                    btn_64arg.button_id = 1250;
                    btn_64arg.txt_r = yw->iniColors[68].r;
                    btn_64arg.txt_g = yw->iniColors[68].g;
                    btn_64arg.txt_b = yw->iniColors[68].b;

                    if ( call_method(usr->locale_button, 64, &btn_64arg) )
                    {
                        btn_64arg.xpos = v274 - (usr->field_19C6 - v267);
                        btn_64arg.ypos = v258 - (usr->field_0x19c8 - v269);
                        btn_64arg.width = v262;
                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 20, "HELP");
                        btn_64arg.button_id = 1252;
                        btn_64arg.caption2 = 0;
                        btn_64arg.field_1C = 0;
                        btn_64arg.down_id = 0;
                        btn_64arg.up_id = 1250;
                        btn_64arg.pressed_id = 0;

                        if ( call_method(usr->locale_button, 64, &btn_64arg) )
                        {
                            btn_64arg.xpos = v264 - (usr->field_19C6 - v267);
                            btn_64arg.ypos = v276 - (usr->field_0x19c8 - v269);
                            btn_64arg.width = v298;
                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 3, "CANCEL");
                            btn_64arg.caption2 = 0;
                            btn_64arg.field_1C = 0;
                            btn_64arg.down_id = 0;
                            btn_64arg.pressed_id = 0;
                            btn_64arg.up_id = 1301;
                            btn_64arg.button_id = 1251;

                            if ( call_method(usr->locale_button, 64, &btn_64arg) )
                                v70 = 1;
                        }
                    }
                }
            }
        }
    }
    if ( !v70 )
    {
        ypa_log_out("Unable to add locale-button\n");
        return 0;
    }

    v238 = 2;
    call_method(usr->locale_button, 68, &v238);

    usr->field_19DE = 0;
    usr->field_0x19e0 = v273;

    usr->about_button = (NC_STACK_button *)init_get_class(
                            "button.class",
                            0x80001003,
                            usr->field_19DE,
                            0x80001004,
                            usr->field_0x19e0,
                            0x80001005,
                            yw->screen_width - usr->field_19DE,
                            0x80001006,
                            yw->screen_height - usr->field_0x19e0,
                            0);
    if ( !usr->about_button )
    {
        ypa_log_out("Unable to create sound-buttonobject\n");
        return 0;
    }

    v70 = 0;
    btn_64arg.tileset_down = 16;
    btn_64arg.tileset_up = 16;
    btn_64arg.field_3A = 16;
    btn_64arg.button_type = 3;
    btn_64arg.xpos = 0;
    btn_64arg.width = (yw->screen_width - 4 * word_5A50C0);
    btn_64arg.caption2 = 0;
    btn_64arg.field_1C = 0;
    btn_64arg.down_id = 0;
    btn_64arg.up_id = 0;
    btn_64arg.pressed_id = 0;
    btn_64arg.button_id = 2;
    btn_64arg.ypos = word_5A50C2 + yw->font_default_h;
    btn_64arg.state = 96;
    btn_64arg.caption = "Fuer den Kauf dieses erzgebirgischen Qualitaetsspielzeuges bedanken sich";

    if ( call_method(usr->about_button, 64, &btn_64arg) )
    {
        btn_64arg.ypos = 2 * (yw->font_default_h + word_5A50C2);
        btn_64arg.caption = "Bernd Beyreuther,";

        if ( call_method(usr->about_button, 64, &btn_64arg) )
        {
            btn_64arg.ypos = 3 * (word_5A50C2 + yw->font_default_h);
            btn_64arg.caption = "Andre 'Floh' Weissflog, Andreas Flemming,";

            if ( call_method(usr->about_button, 64, &btn_64arg) )
            {
                btn_64arg.ypos = 4 * (yw->font_default_h + word_5A50C2);
                btn_64arg.caption = "Stefan 'Metzel Hetzel' Karau, Sylvius Lack,";

                if ( call_method(usr->about_button, 64, &btn_64arg) )
                {
                    btn_64arg.ypos = 5 * (word_5A50C2 + yw->font_default_h);
                    btn_64arg.caption = "Dietmar 'Didi' Koebelin, Nico Nitsch, Steffen Priebus, ";

                    if ( call_method(usr->about_button, 64, &btn_64arg) )
                    {
                        btn_64arg.ypos = 6 * (yw->font_default_h + word_5A50C2);
                        btn_64arg.caption = "Stefan Warias, Henrik Volkening und";

                        if ( call_method(usr->about_button, 64, &btn_64arg) )
                        {
                            btn_64arg.ypos = 7 * (word_5A50C2 + yw->font_default_h);
                            btn_64arg.caption = "Uta Kapp";

                            if ( call_method(usr->about_button, 64, &btn_64arg) )
                            {
                                btn_64arg.ypos = 8 * (yw->font_default_h + word_5A50C2);
                                btn_64arg.caption = " ";

                                if ( call_method(usr->about_button, 64, &btn_64arg) )
                                {
                                    btn_64arg.ypos = 9 * (yw->font_default_h + word_5A50C2);
                                    btn_64arg.caption = "Unser Dank gilt:";

                                    if ( call_method(usr->about_button, 64, &btn_64arg) )
                                    {
                                        btn_64arg.ypos = 10 * (yw->font_default_h + word_5A50C2);
                                        btn_64arg.caption = "dem gesamten Microsoft Team, besonders";

                                        if ( call_method(usr->about_button, 64, &btn_64arg) )
                                        {
                                            btn_64arg.ypos = 11 * (word_5A50C2 + yw->font_default_h);
                                            btn_64arg.caption = "Michael Lyons, Jonathan Sposato und Earnest Yuen";

                                            if ( call_method(usr->about_button, 64, &btn_64arg) )
                                            {
                                                btn_64arg.ypos = 12 * (yw->font_default_h + word_5A50C2);
                                                btn_64arg.caption = "weiterhin";

                                                if ( call_method(usr->about_button, 64, &btn_64arg) )
                                                {
                                                    btn_64arg.ypos = 13 * (yw->font_default_h + word_5A50C2);
                                                    btn_64arg.caption = "Robert Birker, Andre 'Goetz' Blechschmidt, Jan Blechschmidt, Stephan Bludau,";

                                                    if ( call_method(usr->about_button, 64, &btn_64arg) )
                                                    {
                                                        btn_64arg.ypos = 14 * (yw->font_default_h + word_5A50C2);
                                                        btn_64arg.caption = "Andre Kunth, Markus Lorenz, Dirk Mansbart";

                                                        if ( call_method(usr->about_button, 64, &btn_64arg) )
                                                        {
                                                            btn_64arg.ypos = 15 * (word_5A50C2 + yw->font_default_h);
                                                            btn_64arg.caption = "und natuerlich";

                                                            if ( call_method(usr->about_button, 64, &btn_64arg) )
                                                            {
                                                                btn_64arg.ypos = 16 * (yw->font_default_h + word_5A50C2);
                                                                btn_64arg.caption = "        GoldEd - dPaint - SAS/C";

                                                                if ( call_method(usr->about_button, 64, &btn_64arg) )
                                                                {
                                                                    v70 = 1;
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
                        }
                    }
                }
            }
        }
    }
    if ( !v70 )
    {
        ypa_log_out("Unable to add about-button\n");
        return 0;
    }

    v238 = 2;
    call_method(usr->about_button, 68, &v238);

    dword_5A50B6 = v278_4 - yw->font_yscrl_bkg_w;

    if ( !lstvw_init(
                yw,
                &usr->network_listvw,
                0x80000002,
                0,
                0x80000003,
                12,
                0x80000004,
                12,
                0x80000005,
                0,
                0x80000006,
                0,
                0x80000007,
                12,
                0x80000008,
                0,
                0x8000000B,
                yw->font_default_h,
                0x8000000C,
                dword_5A50B2_h,
                0x8000000F,
                1,
                0x80000010,
                yw->field_1a38,
                0x80000011,
                0,
                0x80000017,
                1,
                0) )
    {
        ypa_log_out("Unable to create network-listview\n");
        return 0;
    }

    usr->field_1C32 = v278;
    usr->field_0x1c34 = v273 - yw->font_default_h;

    usr->network_button = (NC_STACK_button *)init_get_class(
                              "button.class",
                              0x80001003,
                              usr->field_1C32,
                              0x80001004,
                              usr->field_0x1c34,
                              0x80001005,
                              yw->screen_width - usr->field_1C32,
                              0x80001006,
                              yw->screen_height - usr->field_0x1c34,
                              0);
    if ( !usr->network_button )
    {
        ypa_log_out("Unable to create network-buttonobject\n");
        return 0;
    }

    usr->field_1C2E = usr->field_1C32;
    usr->field_0x1c30 = 3 * (word_5A50C2 + yw->font_default_h) + usr->field_0x1c34;

    usr->network_listvw.frm_1.btn_xpos = usr->field_1C2E;
    usr->network_listvw.frm_1.btn_ypos = usr->field_0x1c30;

    btn_64arg.tileset_down = 17;
    btn_64arg.tileset_up = 17;
    btn_64arg.field_3A = 17;
    btn_64arg.button_type = 3;
    btn_64arg.xpos = 0;
    btn_64arg.caption = "???";
    btn_64arg.caption2 = 0;
    btn_64arg.field_1C = 0;
    btn_64arg.down_id = 0;
    btn_64arg.up_id = 0;
    btn_64arg.pressed_id = 0;
    btn_64arg.ypos = 14 * (word_5A50C2 + yw->font_default_h);
    btn_64arg.button_id = 1200;
    btn_64arg.state = 64;
    btn_64arg.width = dword_5A50B6 * 0.8;
    btn_64arg.txt_r = yw->iniColors[60].r;
    btn_64arg.txt_g = yw->iniColors[60].g;
    btn_64arg.txt_b = yw->iniColors[60].b;

    v70 = 0;

    if ( call_method(usr->network_button, 64, &btn_64arg) )
    {
        btn_64arg.tileset_down = 19;
        btn_64arg.tileset_up = 18;
        btn_64arg.xpos = word_5A50C0 + dword_5A50B6 * 0.8;
        btn_64arg.field_3A = 30;
        btn_64arg.width = dword_5A50B6 * 0.2 - word_5A50C0;
        btn_64arg.button_type = 1;
        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 405, "SEND");
        btn_64arg.caption2 = 0;
        btn_64arg.up_id = 1210;
        btn_64arg.pressed_id = 0;
        btn_64arg.button_id = 1225;
        btn_64arg.state = 112;
        btn_64arg.txt_r = yw->iniColors[68].r;
        btn_64arg.txt_g = yw->iniColors[68].g;
        btn_64arg.txt_b = yw->iniColors[68].b;

        if ( call_method(usr->network_button, 64, &btn_64arg) )
        {
            int v284 = ((dword_5A50B6 - 3 * word_5A50C0) * 0.25 - 3 * word_5A50C0) * 0.25;

            tiles_stru *v198 = win3d_select_tileset(8);

            btn_64arg.tileset_down = 16;
            btn_64arg.tileset_up = 16;
            btn_64arg.field_3A = 16;
            btn_64arg.button_type = 3;
            btn_64arg.ypos = (15 * (word_5A50C2 + yw->font_default_h));
            btn_64arg.xpos = 0;
            btn_64arg.width = dword_5A50B6 * 0.4 - 2 * word_5A50C0;
            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 424, "SELECT YOUR RACE");
            btn_64arg.caption2 = 0;
            btn_64arg.down_id = 0;
            btn_64arg.state = 320;
            btn_64arg.button_id = 1220;
            btn_64arg.txt_r = yw->iniColors[60].r;
            btn_64arg.txt_g = yw->iniColors[60].g;
            btn_64arg.txt_b = yw->iniColors[60].b;

            if ( call_method(usr->network_button, 64, &btn_64arg) )
            {
                btn_64arg.tileset_down = 8;
                btn_64arg.tileset_up = 8;
                btn_64arg.field_3A = 30;
                btn_64arg.button_type = 4;
                btn_64arg.xpos += btn_64arg.width + 2 * word_5A50C0;
                btn_64arg.width = v198->chars[65].width;
                btn_64arg.caption = "A";
                btn_64arg.caption2 = "B";
                btn_64arg.button_id = 1206;
                btn_64arg.down_id = 1204;
                btn_64arg.state = 0;

                if ( call_method(usr->network_button, 64, &btn_64arg) )
                {
                    btn_64arg.caption = "C";
                    btn_64arg.caption2 = "D";
                    btn_64arg.down_id = 1205;
                    btn_64arg.button_id = 1207;
                    btn_64arg.xpos += v284 + word_5A50C0;

                    if ( call_method(usr->network_button, 64, &btn_64arg) )
                    {
                        btn_64arg.caption = "E";
                        btn_64arg.caption2 = "F";
                        btn_64arg.down_id = 1206;
                        btn_64arg.button_id = 1208;
                        btn_64arg.xpos += v284 + word_5A50C0;

                        if ( call_method(usr->network_button, 64, &btn_64arg) )
                        {
                            btn_64arg.caption = "G";
                            btn_64arg.caption2 = "H";
                            btn_64arg.down_id = 1207;
                            btn_64arg.button_id = 1209;
                            btn_64arg.xpos += v284 + word_5A50C0;

                            if ( call_method(usr->network_button, 64, &btn_64arg) )
                            {
                                btn_64arg.tileset_down = 19;
                                btn_64arg.tileset_up = 18;
                                btn_64arg.field_3A = 30;
                                btn_64arg.button_type = 1;
                                btn_64arg.xpos += v284 + 2 * word_5A50C0;
                                btn_64arg.width = dword_5A50B2_h - btn_64arg.xpos;
                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 401, "BACK");
                                btn_64arg.caption2 = 0;
                                btn_64arg.pressed_id = 0;
                                btn_64arg.button_id = 1205;
                                btn_64arg.state = 112;
                                btn_64arg.up_id = 1203;
                                btn_64arg.down_id = 1251;
                                btn_64arg.txt_r = yw->iniColors[68].r;
                                btn_64arg.txt_g = yw->iniColors[68].g;
                                btn_64arg.txt_b = yw->iniColors[68].b;

                                if ( call_method(usr->network_button, 64, &btn_64arg) )
                                {
                                    btn_64arg.tileset_down = 16;
                                    btn_64arg.xpos = 0;
                                    btn_64arg.ypos = 0;
                                    btn_64arg.tileset_up = 16;
                                    btn_64arg.field_3A = 16;
                                    btn_64arg.width = dword_5A50B2_h;
                                    btn_64arg.button_type = 3;
                                    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 410, "SELECT PROVIDER");
                                    btn_64arg.caption2 = 0;
                                    btn_64arg.down_id = 0;
                                    btn_64arg.up_id = 0;
                                    btn_64arg.button_id = 1204;
                                    btn_64arg.pressed_id = 0;
                                    btn_64arg.state = 64;

                                    if ( call_method(usr->network_button, 64, &btn_64arg) )
                                    {
                                        btn_64arg.xpos = 0;
                                        btn_64arg.ypos = word_5A50C0 + yw->font_default_h;
                                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 425, "2");
                                        btn_64arg.caption2 = 0;
                                        btn_64arg.button_id = 1222;
                                        btn_64arg.txt_r = yw->iniColors[60].r;
                                        btn_64arg.txt_g = yw->iniColors[60].g;
                                        btn_64arg.txt_b = yw->iniColors[60].b;

                                        if ( call_method(usr->network_button, 64, &btn_64arg) )
                                        {
                                            btn_64arg.xpos = 0;
                                            btn_64arg.ypos = 2 * (yw->font_default_h + word_5A50C0);
                                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 426, "3");
                                            btn_64arg.caption2 = 0;
                                            btn_64arg.button_id = 1223;

                                            if ( call_method(usr->network_button, 64, &btn_64arg) )
                                            {
                                                btn_64arg.tileset_down = 19;
                                                btn_64arg.tileset_up = 18;
                                                btn_64arg.button_type = 1;
                                                btn_64arg.field_3A = 30;
                                                btn_64arg.xpos = dword_5A50B6 * 0.3;
                                                btn_64arg.ypos = (word_5A50C0 + yw->font_default_h) * 15.2;
                                                btn_64arg.width = dword_5A50B6 * 0.4;
                                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 402, "NEW");
                                                btn_64arg.button_id = 1202;
                                                btn_64arg.state = 112;
                                                btn_64arg.down_id = 1251;
                                                btn_64arg.up_id = 1201;
                                                btn_64arg.caption2 = 0;
                                                btn_64arg.pressed_id = 0;
                                                btn_64arg.txt_r = yw->iniColors[68].r;
                                                btn_64arg.txt_g = yw->iniColors[68].g;
                                                btn_64arg.txt_b = yw->iniColors[68].b;

                                                if ( call_method(usr->network_button, 64, &btn_64arg) )
                                                {
                                                    btn_64arg.xpos = v267;
                                                    btn_64arg.ypos = v269 + yw->font_default_h;
                                                    btn_64arg.width = v270;
                                                    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 400, "NEXT");
                                                    btn_64arg.caption2 = 0;
                                                    btn_64arg.pressed_id = 0;
                                                    btn_64arg.button_id = 1201;
                                                    btn_64arg.up_id = 1200;

                                                    if ( call_method(usr->network_button, 64, &btn_64arg) )
                                                    {
                                                        btn_64arg.xpos = v274;
                                                        btn_64arg.ypos = v258 + yw->font_default_h;
                                                        btn_64arg.width = v262;
                                                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 20, "HELP");
                                                        btn_64arg.caption2 = 0;
                                                        btn_64arg.up_id = 1250;
                                                        btn_64arg.pressed_id = 0;
                                                        btn_64arg.button_id = 1218;

                                                        if ( call_method(usr->network_button, 64, &btn_64arg) )
                                                        {
                                                            btn_64arg.xpos = v264;
                                                            btn_64arg.ypos = v276 + yw->font_default_h;
                                                            btn_64arg.width = v298;
                                                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 403, "CANCEL");
                                                            btn_64arg.caption2 = 0;
                                                            btn_64arg.up_id = 1202;
                                                            btn_64arg.pressed_id = 0;
                                                            btn_64arg.button_id = 1203;

                                                            if ( call_method(usr->network_button, 64, &btn_64arg) )
                                                            {
                                                                int v204;

                                                                if ( yw->screen_width < 512 )
                                                                    v204 = 6 * v259_4;
                                                                else
                                                                    v204 = 4 * v259_4;

                                                                btn_64arg.tileset_down = 16;
                                                                btn_64arg.tileset_up = 16;
                                                                btn_64arg.field_3A = 16;
                                                                btn_64arg.button_type = 3;
                                                                btn_64arg.xpos = v204 + v259_4;
                                                                btn_64arg.ypos = 4 * (yw->font_default_h + word_5A50C0);
                                                                btn_64arg.caption = " ";
                                                                btn_64arg.width = dword_5A50B2_h - v204 - v259_4;
                                                                btn_64arg.state = 64;
                                                                btn_64arg.caption2 = 0;
                                                                btn_64arg.down_id = 0;
                                                                btn_64arg.up_id = 0;
                                                                btn_64arg.pressed_id = 0;
                                                                btn_64arg.button_id = 1210;
                                                                btn_64arg.txt_r = yw->iniColors[60].r;
                                                                btn_64arg.txt_g = yw->iniColors[60].g;
                                                                btn_64arg.txt_b = yw->iniColors[60].b;

                                                                if ( call_method(usr->network_button, 64, &btn_64arg) )
                                                                {
                                                                    btn_64arg.ypos = 5 * (word_5A50C0 + yw->font_default_h);
                                                                    btn_64arg.button_id = 1211;

                                                                    if ( call_method(usr->network_button, 64, &btn_64arg) )
                                                                    {
                                                                        btn_64arg.ypos = 6 * (word_5A50C0 + yw->font_default_h);
                                                                        btn_64arg.button_id = 1212;

                                                                        if ( call_method(usr->network_button, 64, &btn_64arg) )
                                                                        {
                                                                            btn_64arg.ypos = 7 * (word_5A50C0 + yw->font_default_h);
                                                                            btn_64arg.button_id = 1213;

                                                                            if ( call_method(usr->network_button, 64, &btn_64arg) )
                                                                            {
                                                                                btn_64arg.tileset_down = 8;
                                                                                btn_64arg.tileset_up = 8;
                                                                                btn_64arg.field_3A = 8;
                                                                                btn_64arg.button_type = 3;
                                                                                btn_64arg.xpos = 0;
                                                                                btn_64arg.width = v204;
                                                                                btn_64arg.caption2 = 0;
                                                                                btn_64arg.down_id = 0;
                                                                                btn_64arg.up_id = 0;
                                                                                btn_64arg.ypos = 4 * (yw->font_default_h + word_5A50C0);
                                                                                btn_64arg.pressed_id = 0;
                                                                                btn_64arg.caption = " ";
                                                                                btn_64arg.state = 0;
                                                                                btn_64arg.button_id = 1214;
                                                                                btn_64arg.txt_r = yw->iniColors[60].r;
                                                                                btn_64arg.txt_g = yw->iniColors[60].g;
                                                                                btn_64arg.txt_b = yw->iniColors[60].b;

                                                                                if ( call_method(usr->network_button, 64, &btn_64arg) )
                                                                                {
                                                                                    btn_64arg.ypos = 5 * (yw->font_default_h + word_5A50C0);
                                                                                    btn_64arg.button_id = 1215;

                                                                                    if ( call_method(usr->network_button, 64, &btn_64arg) )
                                                                                    {
                                                                                        btn_64arg.ypos = 6 * (word_5A50C0 + yw->font_default_h);
                                                                                        btn_64arg.button_id = 1216;

                                                                                        if ( call_method(usr->network_button, 64, &btn_64arg) )
                                                                                        {
                                                                                            btn_64arg.ypos = 7 * (word_5A50C0 + yw->font_default_h);
                                                                                            btn_64arg.button_id = 1217;

                                                                                            if ( call_method(usr->network_button, 64, &btn_64arg) )
                                                                                            {
                                                                                                btn_64arg.tileset_down = 19;
                                                                                                btn_64arg.tileset_up = 18;
                                                                                                btn_64arg.xpos = v267;
                                                                                                btn_64arg.field_3A = 30;
                                                                                                btn_64arg.button_type = 2;
                                                                                                btn_64arg.width = v259_4;
                                                                                                btn_64arg.caption = "g";
                                                                                                btn_64arg.caption2 = "g";
                                                                                                btn_64arg.field_1C = 0;
                                                                                                btn_64arg.pressed_id = 0;
                                                                                                btn_64arg.button_id = 1219;
                                                                                                btn_64arg.ypos = v269 + yw->font_default_h;
                                                                                                btn_64arg.down_id = 1208;
                                                                                                btn_64arg.state = 0;
                                                                                                btn_64arg.up_id = 1209;

                                                                                                if ( call_method(usr->network_button, 64, &btn_64arg) )
                                                                                                {
                                                                                                    btn_64arg.tileset_down = 16;
                                                                                                    btn_64arg.tileset_up = 16;
                                                                                                    btn_64arg.field_3A = 16;
                                                                                                    btn_64arg.button_type = 3;
                                                                                                    btn_64arg.xpos += word_5A50C0 + v259_4;
                                                                                                    btn_64arg.width = v270 - v259_4 - word_5A50C0;
                                                                                                    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 409, "READY");
                                                                                                    btn_64arg.caption2 = 0;
                                                                                                    btn_64arg.field_1C = 0;
                                                                                                    btn_64arg.down_id = 0;
                                                                                                    btn_64arg.up_id = 0;
                                                                                                    btn_64arg.pressed_id = 0;
                                                                                                    btn_64arg.state = 64;
                                                                                                    btn_64arg.button_id = 1221;

                                                                                                    if ( call_method(usr->network_button, 64, &btn_64arg) )
                                                                                                    {
                                                                                                        btn_64arg.xpos = 0;
                                                                                                        btn_64arg.tileset_down = 16;
                                                                                                        btn_64arg.tileset_up = 16;
                                                                                                        btn_64arg.field_3A = 16;
                                                                                                        btn_64arg.button_type = 3;
                                                                                                        btn_64arg.ypos = 3 * (yw->font_default_h + word_5A50C0);
                                                                                                        btn_64arg.width = dword_5A50B6 * 0.3;
                                                                                                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 600, "YOU PLAY");
                                                                                                        btn_64arg.caption2 = 0;
                                                                                                        btn_64arg.down_id = 0;
                                                                                                        btn_64arg.up_id = 0;
                                                                                                        btn_64arg.pressed_id = 0;
                                                                                                        btn_64arg.button_id = 1227;
                                                                                                        btn_64arg.state = 64;
                                                                                                        btn_64arg.txt_r = yw->iniColors[68].r;
                                                                                                        btn_64arg.txt_g = yw->iniColors[68].g;
                                                                                                        btn_64arg.txt_b = yw->iniColors[68].b;

                                                                                                        if ( call_method(usr->network_button, 64, &btn_64arg) )
                                                                                                        {
                                                                                                            btn_64arg.xpos = dword_5A50B6 * 0.3;
                                                                                                            btn_64arg.button_type = 3;
                                                                                                            btn_64arg.width = dword_5A50B6 * 0.7;
                                                                                                            btn_64arg.state = 64;
                                                                                                            btn_64arg.button_id = 1226;
                                                                                                            btn_64arg.caption = "...";

                                                                                                            if ( call_method(usr->network_button, 64, &btn_64arg) )
                                                                                                            {
                                                                                                                btn_64arg.tileset_down = 16;
                                                                                                                btn_64arg.tileset_up = 16;
                                                                                                                btn_64arg.field_3A = 16;
                                                                                                                btn_64arg.button_type = 3;
                                                                                                                btn_64arg.xpos = 0;
                                                                                                                btn_64arg.ypos = (14 * (word_5A50C2 + yw->font_default_h));
                                                                                                                btn_64arg.width = dword_5A50B2_h;
                                                                                                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 2402, "PRESS SPACEBAR TO UPDATE SESSION LIST");
                                                                                                                btn_64arg.caption2 = 0;
                                                                                                                btn_64arg.down_id = 0;
                                                                                                                btn_64arg.up_id = 0;
                                                                                                                btn_64arg.pressed_id = 0;
                                                                                                                btn_64arg.button_id = 1228;
                                                                                                                btn_64arg.state = 64;
                                                                                                                btn_64arg.txt_r = yw->iniColors[60].r;
                                                                                                                btn_64arg.txt_g = yw->iniColors[60].g;
                                                                                                                btn_64arg.txt_b = yw->iniColors[60].b;

                                                                                                                if ( call_method(usr->network_button, 64, &btn_64arg) )
                                                                                                                    v70 = 1;
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
                            }
                        }
                    }
                }
            }
        }
    }
    if ( !v70 )
    {
        ypa_log_out("Unable to add network-button\n");
        return 0;
    }

    v228.butID = 1210;
    v228.field_4 = 0;
    call_method(usr->network_button, 67, &v228);

    v228.butID = 1211;
    call_method(usr->network_button, 67, &v228);

    v228.butID = 1212;
    call_method(usr->network_button, 67, &v228);

    v228.butID = 1213;
    call_method(usr->network_button, 67, &v228);

    v228.butID = 1214;
    call_method(usr->network_button, 67, &v228);

    v228.butID = 1215;
    call_method(usr->network_button, 67, &v228);

    v228.butID = 1216;
    call_method(usr->network_button, 67, &v228);

    v228.butID = 1217;
    call_method(usr->network_button, 67, &v228);

    v238 = 2;
    call_method(usr->network_button, 68, &v238);

    switch (usr->field_46)
    {
    default:
    case 0:
    case 1:
    case 2:
    case 3:
        v238 = 1;
        call_method(usr->titel_button, 68, &v238);
        break;
    case 4:
    case 5:
        v238 = 1;
        call_method(usr->sub_bar_button, 68, &v238);

        if ( usr->field_0xc )
        {
            button_66arg v231;
            v231.field_4 = 0;
            v231.butID = 1014;

            call_method(usr->sub_bar_button, 67, &v231);

            v231.butID = 1013;
            call_method(usr->sub_bar_button, 67, &v231);
        }
        break;
    case 6:
        v238 = 1;
        call_method(usr->network_button, 68, &v238);
        break;
    }


    call_method(obj, 167, usr);

    sub_423F74(&usr->samples1_info, 6);

    usr->field_0x0 = 1;

    usr->field_0x4 = 1;
    if ( usr->field_1CEA )
    {
        sub_46C524(usr);
        usr->p_ypaworld->field_757E = 1;
        usr->field_1CD5 = 14;
        usr->field_1CD6 = 1;
    }
    else
    {
        yw_netcleanup(usr->p_ypaworld);
        usr->field_1C3A = 0;
    }

    usr->field_0x1c3c = -1;

    if ( usr->p_ypaworld->snd__cdsound & 1 )
    {
        CDAUDIO_t cdaud;

        cdaud.command = 1;
        sub_4444D4(&cdaud);

        cdaud.command = 7;
        cdaud.field_8 = yw->GameShell->shelltrack__adv.field_0;
        cdaud.field_C = yw->GameShell->shelltrack__adv.field_4;
        cdaud.track_id = usr->shelltrack;
        sub_4444D4(&cdaud);

        cdaud.command = 2;
        sub_4444D4(&cdaud);
    }

    return 1;
}


void ypaworld_func157(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}

//Draw bkg or briefing
void ypaworld_func158__sub4(_NC_STACK_ypaworld *yw, UserData *usr, struC5 *struc5)
{
    if ( usr->field_4A )
        sb_0x4e75e8(yw, usr->field_46);

    if ( yw->GameShell->field_46 == 0 )
    {
        ypaworld_func158__sub4__sub0(yw, yw->LevelNet->ilbm_rollover_map);
    }
    else if ( yw->GameShell->field_46 == 1 )
    {
        ypaworld_func158__sub4__sub0(yw, yw->LevelNet->ilbm_menu_map);
    }
    else if ( yw->GameShell->field_46 >= 4 && yw->GameShell->field_46 <= 5 )
    {
        ypaworld_func158__sub4__sub1(yw, usr, struc5);
    }
    else
    {
        ypaworld_func158__sub4__sub0(yw, yw->LevelNet->ilbm_rollover_map);
    }
}


char byte_5AFC10[64];

char * sb_0x481264__sub0(_NC_STACK_ypaworld *yw, int a2)
{
    byte_5AFC10[0] = 0;

    if (  !yw->GameShell || !keySS[a2].title_by_language )
        return NULL;

    sprintf(byte_5AFC10, "[%s]", keySS[a2].title_by_language);
    return byte_5AFC10;
}

void draw_tooltip(_NC_STACK_ypaworld *yw)
{
    if ( yw->field_17c0 || (yw->field_17c4 && !yw->field_1b1c) )
    {
        const char *tooltip = get_lang_string(yw->string_pointers_p2, yw->field_17c4 + 800, yw->tooltips[ yw->field_17c4 ]);
        int v15 = -(yw->font_default_h + yw->icon0___h + yw->font_default_h / 4);
        char *v2 = NULL;
        if ( yw->field_17c8 )
        {
            NC_STACK_input *v13;
            inputEngine__getter(0x80001009, &v13, 0);

            int v11[2];
            v11[0] = 0;
            v11[1] = yw->field_17c8;

            input__func66__params v9;

            v9.funcID = 70;
            v9.vals = &v11;
            v9.field_0 = 3;
            v9.field_4 = 0;

            call_method(v13, 66, &v9);

            if ( v11[0] )
            {
                v2 = sb_0x481264__sub0(yw, v11[0]);
                if ( v2 )
                    v15 = -(yw->icon0___h + 2 * yw->font_default_h + yw->font_default_h / 4);
            }
        }

        char buf[1024];

        char *pos = buf;

        fntcmd_select_tileset(&pos, 15);
        fntcmd_set_xpos(&pos, 0);
        fntcmd_set_ypos(&pos, v15);

        if ( v2 )
        {
            fntcmd_set_txtColor(&pos, yw->iniColors[61].r, yw->iniColors[61].g, yw->iniColors[61].b);

            pos = sub_45148C(yw->tiles[15], pos, v2, yw->screen_width);

            fntcmd_next_line(&pos);
        }

        fntcmd_set_txtColor(&pos, yw->iniColors[63].r, yw->iniColors[63].g, yw->iniColors[63].b);

        pos = sub_45148C(yw->tiles[15], pos, tooltip, yw->screen_width);

        fntcmd_set_end(&pos);

        w3d_a209 v10;

        v10.cmdbuf = buf;
        v10.includ = 0;

        call_method(yw->win3d, 209, &v10);
    }
    yw->field_17c8 = 0;
    yw->field_17c4 = 0;
}

//Make screenshot
void sub_4476AC(_NC_STACK_ypaworld *yw)
{
    char a1a[256];
    sprintf(a1a, "env/snaps/f_%04d", yw->field_2424);

    yw->field_2424++;

    NC_STACK_win3d *w3d;
    gfxEngine__getter(0x8000300D, &w3d, 0);

    char *v7 = a1a;

    call_method(w3d, 274, &v7);
}

int word_5A50DA = 0;
int word_5A50D8 = 0;

int sub_46D3EC(struC5 *struc)
{
    int v1 = struc->winp131arg.flag & 0xFE;
    int v2 = struc->winp131arg.move[0].x != word_5A50DA || word_5A50D8 != struc->winp131arg.move[0].y;

    word_5A50DA = struc->winp131arg.move[0].x;
    word_5A50D8 = struc->winp131arg.move[0].y;

    return struc->downed_key_2 || struc->downed_key || struc->dword8 || v2 || v1;
}

void ypaworld_func158(NC_STACK_ypaworld *obj, class_stru *zis, UserData *usr)
{
    _NC_STACK_ypaworld *yw = &obj->stack__ypaworld;
    usr->field_0x2fbc = 0;

    xyz stru_515034;
    stru_515034.sx = 0;
    stru_515034.sy = 0;
    stru_515034.sz = 0;

    mat3x3 stru_515040;
    stru_515040.m00 = 1.0;
    stru_515040.m01 = 0.0;
    stru_515040.m02 = 0.0;
    stru_515040.m10 = 0.0;
    stru_515040.m11 = 1.0;
    stru_515040.m12 = 0.0;
    stru_515040.m20 = 0.0;
    stru_515040.m21 = 0.0;
    stru_515040.m22 = 1.0;

    sub_423EFC(usr->field_3E, &stru_515034, &stru_515034, &stru_515040);

    gfxEngine__getter(0x8000300D, &yw->win3d, 0);

    call_method(yw->win3d, 257);

    int v7 = usr->field_46;
    usr->field_4A = 0;

    ypaworld_func158__sub0(yw, usr);

    if ( v7 != usr->field_46 )
        usr->field_4A = 1;

    ypaworld_func158__sub4(yw, usr, usr->field_3A);

    call_method(yw->win3d, 215);

    draw_tooltip(yw);

    ypaworld_func158__sub3(yw, usr);

    call_method(yw->win3d, 216);

    if ( yw->field_757E )
    {
        yw->field_7586 -= usr->field_3E;
        if ( yw->field_7586 <= 0 )
        {
//      v18 = 1;
//      v17 = usr->callSIGN;
//      v20 = 2;
//      v19 = 0;
//      v21 = 1;
//
//      call_method(yw->windp, 82, &v17);
            yw->field_7586 = 100;
        }
    }

    sb_0x4242e0(&usr->samples1_info);
    sb_0x4242e0(&usr->samples2_info);
    sb_0x4242e0(&usr->field_782);

//  if ( usr->field_0x4 )
//    nullsub_7();

    call_method(yw->win3d, 258, 0);

    if ( usr->field_0x4 )
    {
        usr->field_0x4 = 0;
//    nullsub_7();
    }

    if ( sub_449678(yw, usr->field_3A, VK_MULTIPLY) )
        sub_4476AC(yw);

    if ( usr->field_1C3A == 4 )
    {
        ypaworld_func158__sub2(yw);
        ypaworld_func158__sub1(usr);
    }

    int v15 = sub_46D3EC(usr->field_3A);

    if ( v15 )
        usr->field_5457 = usr->field_42;

    if ( (usr->field_42 - usr->field_5457) > usr->field_545B && usr->field_46 == 1 )
        usr->field_0x2fbc = 5;

    usr->field_0xc = 0;

    if ( yw->field_81AF )
    {
        const char *v22 = yw->field_81AF;
        call_method(obj, 185, &v22);

        yw->field_81AF = NULL;
    }
}


void ypaworld_func159(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func160(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func161(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func162(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func163(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func164(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func165(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


char * sb_0x471428__sub0(char *a1, const char *a2)
{
    const char *tmp = a2;

    while ( *tmp )
    {
        if (tmp[0] == '\\')
        {
            if (tmp[1] == '\\')
            {
                *a1 = '\n';
                tmp++;
            }
            else if (tmp[1] == 's')
            {
                *a1 = ' ';
                tmp++;
            }
        }
        else
            *a1 = *tmp;

        a1++;
        tmp++;
    }
    *a1 = 0;
    a1++;
    return a1;
}

int locale_parser(scrCallBack *arg)
{
    _NC_STACK_ypaworld *yw = (_NC_STACK_ypaworld *)arg->dataForStore;

    if ( arg->field_18 )
    {
        if ( !strcasecmp(arg->p1, "end") )
        {
            arg->field_18 = 0;
            return 2;
        }

        int id = strtol(arg->p1, NULL, 0);
        int err = 0;

        if ( id < 2600 )
        {
            if ( yw->very_big_array__p_begin < yw->lang_strings__end )
            {
                if ( !yw->string_pointers[id] )
                {
                    yw->string_pointers[id] = yw->very_big_array__p_begin;
                    yw->very_big_array__p_begin = sb_0x471428__sub0(yw->very_big_array__p_begin, arg->p2);
                    err = 0;
                }
                else
                {
                    printf("%s %s\n",arg->p1, arg->p2);
                    err = 3;
                }
            }
            else
            {
                err = 2;
            }
        }
        else
        {
            err = 1;
        }

        if ( err == 1 )
        {
            ypa_log_out("Locale parser: id [%d]аtoo big.\n", id);
            return 4;
        }
        else if ( err == 2 )
        {
            ypa_log_out("Locale parser: buffer overflow at id [%d].\n", id);
            return 4;
        }
        else if ( err == 3 )
        {
            ypa_log_out("Locale parser: id [%d] already defined.\n", id);
            return 4;
        }

        return 0;
    }
    else if ( !strcasecmp(arg->p1, "begin_locale") )
    {
        arg->field_18 = 1;
        return 1;

    }

    return 3;
}

int load_lang_lng(_NC_STACK_ypaworld *yw, const char *lang)
{
    char buf[128];
    sprintf(buf, "locale/%s.lng", lang);

    scrCallBack pars;
    memset(&pars, 0, sizeof(scrCallBack));

    pars.dataForStore = yw;
    pars.func = locale_parser;

    if ( !def_parseFile(buf, 1, &pars, 1) )
    {
        ypa_log_out("ERROR: Could not load language file '%s'!!!\n", buf);
        return 0;
    }

    return 1;
}

size_t ypaworld_func166(NC_STACK_ypaworld *obj, class_stru *zis,const char **langname)
{
    _NC_STACK_ypaworld *yw = &obj->stack__ypaworld;

    sub_4711E0(yw);
    strcpy(yw->lang_name, *langname);

    int v19 = 0;
//// We do not use this dll-garbage
//  char buf[256];
//  char LibFileName[256];
//  sprintf(buf, "locale/%s.dll", yw->lang_name);
//  sub_412810(buf, LibFileName, 256);
//  if ( load_lang_dll(LibFileName, yw->lang_strings, yw->lang_strings__end, yw->string_pointers, 2600) )
//    v19 = 1;

    if ( v19 || load_lang_lng(yw, yw->lang_name) )
    {
        const char *v11 = NULL;

        if ( yw->screen_width >= 512 )
            v11 = get_lang_string(yw->string_pointers_p2, 15, "MS Sans Serif,12,400,0");
        else
            v11 = get_lang_string(yw->string_pointers_p2, 16, "Arial,8,400,0");

        load_font(v11);

        NC_STACK_win3d *win3d = NULL;
        gfxEngine__getter(0x8000300D, &win3d, 0);

        if ( !strcasecmp( get_lang_string(yw->string_pointers_p2, 17, "FALSE") , "FALSE") )
        {
            if ( win3d )
                call_vtbl(win3d, 2, 0x80005002, 0, 0);
            yw->str17_NOT_FALSE = 0;
        }
        else
        {
            if ( win3d )
                call_vtbl(win3d, 2, 0x80005002, 1, 0);
            yw->str17_NOT_FALSE = 1;
        }

        return 1;
    }

    sub_4711E0(yw);
    return 0;
}


void ypaworld_func167__sub0(UserData *usr)
{
    button_66arg v9;
    v9.butID = 1050;
    v9.field_4 = ((usr->p_ypaworld->field_73CE & 4) != 0) + 1;

    call_method(usr->button_input_button, 73, &v9);

    v9.butID = 1061;
    v9.field_4 = (usr->inp_altjoystick == 0) + 1;
    call_method(usr->button_input_button, 73, &v9);

    v9.butID = 1055;
    v9.field_4 = ((usr->p_ypaworld->field_73CE & 8) != 0) + 1;
    call_method(usr->button_input_button, 73, &v9);

    if ( usr->field_D36 )
    {
        int v7 = usr->field_D36 - 1;
        int v8 = usr->input_listview.element_count_max + usr->input_listview.scroll_pos;

        if ( v7 >= usr->input_listview.scroll_pos && v7 < v8 )
        {
            if ( v8 > usr->input_listview.elements_for_scroll_size )
                usr->input_listview.scroll_pos = usr->input_listview.elements_for_scroll_size - usr->input_listview.element_count_max;

            if ( usr->input_listview.scroll_pos < 0 )
                usr->input_listview.scroll_pos = 0;
        }
        else if ( usr->input_listview.elements_for_scroll_size - v7 <= usr->input_listview.element_count_max )
        {
            usr->input_listview.scroll_pos = usr->input_listview.elements_for_scroll_size - usr->input_listview.element_count_max;
        }
        else
        {
            usr->input_listview.scroll_pos = v7;
        }
    }
}

// Update menu values
void ypaworld_func167(NC_STACK_ypaworld *obj, class_stru *zis, UserData *usr)
{
    if ( usr->field_1612 )
    {
        sub_4DDFA4(&usr->disk_listvw, usr->field_1612 - 1);

        button_66arg v18;
        v18.field_4 = 1;
        v18.butID = 1101;
        call_method(usr->disk_button, 66, &v18);

        v18.butID = 1102;
        call_method(usr->disk_button, 66, &v18);

        v18.butID = 1103;
        call_method(usr->disk_button, 66, &v18);
    }
    else
    {
        button_66arg v18;
        v18.field_4 = 1;
        v18.butID = 1101;
        call_method(usr->disk_button, 67, &v18);

        v18.butID = 1102;
        call_method(usr->disk_button, 67, &v18);

        v18.butID = 1103;
        call_method(usr->disk_button, 67, &v18);
    }

    button_66arg v16;
    v16.butID = 1151;
    v16.field_4 = ((usr->snd__flags2 & 1) == 0) + 1;

    call_method(usr->video_button, 73, &v16);


    v16.butID = 1150;
    v16.field_4 = ((usr->GFX_flags & 0x10) == 0) + 1;
    call_method(usr->video_button, 73, &v16);

    v16.field_4 = ((usr->snd__flags2 & 0x10) == 0) + 1;
    v16.butID = 1164;
    call_method(usr->video_button, 73, &v16);

    int v19;
    v19 = 1152;

    button_str2_t2 *tmp = (button_str2_t2 *)call_method(usr->video_button, 74, &v19);
    tmp->field_0 = usr->snd__volume;

    call_method(usr->video_button, 75, &v19);


    v19 = 1154;
    tmp = (button_str2_t2 *)call_method(usr->video_button, 74, &v19);
    tmp->field_0 = usr->snd__cdvolume;

    call_method(usr->video_button, 75, &v19);

    v16.butID = 1163;
    v16.field_4 = (usr->enemyindicator == 0) + 1;
    call_method(usr->video_button, 73, &v16);

    v16.butID = 1157;
    v16.field_4 = ((usr->GFX_flags & 1) == 0) + 1;
    call_method(usr->video_button, 73, &v16);

    v16.field_4 = ((usr->GFX_flags & 2) == 0) + 1;
    v16.butID = 1160;
    call_method(usr->video_button, 73, &v16);

    v16.butID = 1165;
    v16.field_4 = ((usr->GFX_flags & 4) == 0) + 1;
    call_method(usr->video_button, 73, &v16);

    v16.butID = 1166;
    v16.field_4 = ((usr->GFX_flags & 8) == 0) + 1;
    call_method(usr->video_button, 73, &v16);

    video_mode_node *node = (video_mode_node *)usr->video_mode_list.head;

    while (node->next)
    {
        if (usr->p_ypaworld->game_default_res == node->sort_id )
            break;

        node = (video_mode_node *)node->next;
    }

    button_71arg v15;
    v15.field_4 = node->name;
    v15.field_8 = node->name;
    v15.butID = 1156;

    call_method(usr->video_button, 71, &v15);

    v19 = 1159;
    tmp = (button_str2_t2 *)call_method(usr->video_button, 74, &v19);
    tmp->field_0 = usr->fxnumber;

    call_method(usr->video_button, 75, &v19);

    ypaworld_func167__sub0(usr);
}


void ypaworld_func168(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func169(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func170(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


// Saving game
size_t ypaworld_func171(NC_STACK_ypaworld *obj, class_stru *zis, yw_arg172 *arg)
{
    UserData *usr = arg->usr;

    if ( !arg->usertxt || !arg->field_4 )
    {
        ypa_log_out("No names for save action\n");
        return 0;
    }

    char a1a[300];
    sprintf(a1a, "save:%s", arg->usertxt);

    FILE *sfil = FOpen(a1a, "w");

    if ( !sfil )
        return 1;

    if ( arg->field_8 & 1 && !yw_write_user(sfil, usr) )
    {
        ypa_log_out("Unable to write user data to file\n");
        FClose(sfil);
        return 0;
    }

    if ( arg->field_8 & 2 && !yw_write_input(sfil, usr) )
    {
        ypa_log_out("Unable to write input data to file\n");
        FClose(sfil);
        return 0;
    }

    if ( arg->field_8 & 8 && !yw_write_sound(sfil, usr) )
    {
        ypa_log_out("Unable to write sound data to file\n");
        FClose(sfil);
        return 0;
    }

    if ( arg->field_8 & 4 && !yw_write_video(sfil, usr) )
    {
        ypa_log_out("Unable to write video data to file\n");
        FClose(sfil);
        return 0;
    }

    if ( arg->field_8 & 0x10 && !yw_write_levels_statuses(sfil, usr->p_ypaworld) )
    {
        ypa_log_out("Unable to write score data to file\n");
        FClose(sfil);
        return 0;
    }

    if ( arg->field_8 & 0x80 && !yw_write_buddies(sfil, usr->p_ypaworld) )
    {
        ypa_log_out("Unable to write buddies to file\n");
        FClose(sfil);
        return 0;
    }

    if ( arg->field_8 & 0x20 && !yw_write_shell(sfil, usr) )
    {
        ypa_log_out("Unable to write shell data to file\n");
        FClose(sfil);
        return 0;
    }

    if ( arg->field_8 & 0x40 && !yw_write_item_modifers(usr->p_ypaworld, sfil) )
    {
        ypa_log_out("Unable to write build info to file\n");
        FClose(sfil);
        return 0;
    }

    FClose(sfil);
    return 1;
}



int ypaworld_func172__sub0(UserData *usr, const char *fname, int parsers_mask, NC_STACK_ypaworld *ywo)
{
    scrCallBack v18[10];

    memset(v18, 0, sizeof(v18));

    int parsers_number = 0;

    if ( parsers_mask & 1 )
    {
        v18[parsers_number].world = (_NC_STACK_ypaworld *)ywo;
        v18[parsers_number].func = parseSaveUser;
        v18[parsers_number].world2 = usr->p_ypaworld;

        parsers_number++;
    }

    if ( parsers_mask & 2 )
    {
        v18[parsers_number].world = (_NC_STACK_ypaworld *)ywo;
        v18[parsers_number].func = parseSaveInput;
        v18[parsers_number].dataForStore = usr;

        parsers_number++;
    }

    if ( parsers_mask & 4 )
    {
        v18[parsers_number].world = (_NC_STACK_ypaworld *)ywo;
        v18[parsers_number].func = parseSaveVideo;
        v18[parsers_number].dataForStore = usr;

        parsers_number++;
    }

    if ( parsers_mask & 8 )
    {
        v18[parsers_number].world = (_NC_STACK_ypaworld *)ywo;
        v18[parsers_number].func = parseSaveSound;
        v18[parsers_number].dataForStore = usr;

        parsers_number++;
    }

    if ( parsers_mask & 0x10 )
    {
        v18[parsers_number].world = (_NC_STACK_ypaworld *)&usr->snd__flags1;
        v18[parsers_number].func = parseSaveLevelStatus;
        v18[parsers_number].dataForStore = usr->p_ypaworld;

        parsers_number++;
    }

    if ( parsers_mask & 0x80 )
    {
        v18[parsers_number].func = parseSaveBuddy;
        v18[parsers_number].dataForStore = usr->p_ypaworld;

        parsers_number++;
    }

    if ( parsers_mask & 0x20 )
    {
        v18[parsers_number].world = (_NC_STACK_ypaworld *)ywo;
        v18[parsers_number].func = parseSaveShell;
        v18[parsers_number].dataForStore = usr->p_ypaworld;

        parsers_number++;
    }

    if ( parsers_mask & 0x40 )
    {
        v18[parsers_number].world = usr->p_ypaworld;
        v18[parsers_number].func = VhclProtoParser;

        parsers_number++;

        v18[parsers_number].world = usr->p_ypaworld;
        v18[parsers_number].func = WeaponProtoParser;

        parsers_number++;

        v18[parsers_number].world2 = usr->p_ypaworld;
        v18[parsers_number].func = BuildProtoParser;

        parsers_number++;
    }

    return def_parseFile(fname, parsers_number, v18, 2);
}

// Load user save
size_t ypaworld_func172(NC_STACK_ypaworld *obj, class_stru *zis, yw_arg172 *arg)
{
    char a1a[300];

    UserData *usr = arg->usr;

    if ( arg->field_8 & 0x10 )
    {
        if ( usr->field_0x8 )
        {
            usr->field_0x8 = 0;
        }
        else
        {
            sprintf(a1a, "%s/user.txt", usr->user_name);

            yw_arg172 v12;
            v12.usr = usr;
            v12.usertxt = a1a;
            v12.field_10 = 0;
            v12.field_4 = usr->user_name;
            v12.field_8 = 255;

            call_method(obj, 171, &v12);
        }
    }

    usr->snd__flags1 = 0;

    if ( arg->field_8 & 0x80 )
        usr->p_ypaworld->field_2d90->buddies_count = 0;

    char buf[300];
    sprintf(buf, "save:%s", arg->usertxt);

    if ( !ypaworld_func172__sub0(usr, buf, arg->field_8, obj) )
    {
        ypa_log_out("Error while loading information from %s\n", arg->usertxt);
        return 0;
    }

    if ( (arg->field_10 & 1) && !usr->field_0x0 && !call_method(obj, 156, usr) ) // Init menus
    {
        ypa_log_out("Unable to open GameShell\n");
        return 0;
    }

    if ( arg->field_8 & 0x10 && usr->snd__flags1 & 0x10 )
    {
        strcpy(usr->user_name, arg->field_4);
    }

    if ( arg->field_8 & 2 )
        sub_457BC0(usr);

    if ( arg->field_10 & 1 )
        call_method(obj, 167, usr); // Update menu values

    return 1;
}


size_t ypaworld_func173(NC_STACK_ypaworld *obj, class_stru *zis, UserData *usr)
{
    char v28[500];
    memset(v28, 0, 500);

    if ( usr->field_D36 < 1 || usr->field_D36 > 45 )
        return 0;

    inp_key_setting *v5 = &usr->keyConfig[usr->field_D36];

    if ( !keySS[v5->field_10].short_name )
        return 0;

    if ( v5->inp_type == 2 && !keySS[v5->slider_neg].short_name )
        return 0;

    if ( v5->inp_type == 2 )
    {
        strcpy(v28, "~#");
        strcat(v28, "winp:");
        strcat(v28, keySS[v5->slider_neg].short_name);
        strcat(v28, " #");
        strcat(v28, "winp:");
    }
    else if ( v5->inp_type == 1 )
    {
        strcpy(v28, "winp:");
    }

    if ( !keySS[ v5->KeyCode ].short_name )
        return 0;

    strcat(v28, keySS[ v5->KeyCode ].short_name);

    NC_STACK_input *v38 = NULL;

    inputEngine__getter(0x80001009, &v38, 0);

    if ( v5->inp_type == 3 )
    {
        input__func66__params v29;
        winp_68arg v33;

        v33.keyname = v28;
        v33.id = v5->keyID;

        v29.field_0 = 3;
        v29.vals = &v33;
        v29.field_4 = 0;
        v29.funcID = 68;

        if ( !call_method(v38, 66, &v29) )
            ypa_log_out("input.engine: WARNING: Hotkey[%d] (%s) not accepted.\n", v5->keyID, v33.keyname);
    }
    else
    {
        input__func64__params v30;
        v30.item_number = v5->keyID;
        v30.value = v28;
        if ( v5->inp_type == 1 )
        {
            v30.type_id = 4;
            if ( !call_method(v38, 64, &v30) )
                ypa_log_out("input.engine: WARNING: Button[%d] (%s) not accepted.\n", v5->keyID, v28);
        }
        else
        {
            v30.type_id = 5;
            if ( !call_method(v38, 64, &v30) )
                ypa_log_out("input.engine: WARNING: Slider[%d] (%s) not accepted.\n", v5->keyID, v28);
        }
    }
    v5->field_A = v5->slider_neg;
    v5->field_8 = v5->KeyCode;
    return 1;
}


void ypaworld_func174(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func175(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func176(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func177(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func179(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func180(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func181(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func182(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


size_t ypaworld_func183(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
    return 0;
}


void ypaworld_func184(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func185(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}




class_return ypaworld_class_descr;

class_return * class_set_ypaworld(int, ...)
{
    memset(ypaworld_funcs, 0, sizeof(CLASSFUNC) * 1024);

    ypaworld_funcs[0] = (CLASSFUNC)ypaworld_func0;
    ypaworld_funcs[1] = (CLASSFUNC)ypaworld_func1;
    ypaworld_funcs[2] = (CLASSFUNC)ypaworld_func2;
    ypaworld_funcs[3] = (CLASSFUNC)ypaworld_func3;
    ypaworld_funcs[64] = (CLASSFUNC)ypaworld_func64;
    ypaworld_funcs[129] = (CLASSFUNC)ypaworld_func129;
    ypaworld_funcs[130] = (CLASSFUNC)ypaworld_func130;
    ypaworld_funcs[131] = (CLASSFUNC)ypaworld_func131;
    ypaworld_funcs[132] = (CLASSFUNC)ypaworld_func132;
    ypaworld_funcs[133] = (CLASSFUNC)ypaworld_func133;
    ypaworld_funcs[134] = (CLASSFUNC)ypaworld_func134;
    ypaworld_funcs[135] = (CLASSFUNC)ypaworld_func135;
    ypaworld_funcs[136] = (CLASSFUNC)ypaworld_func136;
    ypaworld_funcs[137] = (CLASSFUNC)ypaworld_func137;
    ypaworld_funcs[138] = (CLASSFUNC)ypaworld_func138;
    ypaworld_funcs[139] = (CLASSFUNC)ypaworld_func139;
    ypaworld_funcs[140] = (CLASSFUNC)ypaworld_func140;
    ypaworld_funcs[143] = (CLASSFUNC)ypaworld_func143;
    ypaworld_funcs[144] = (CLASSFUNC)ypaworld_func144;
    ypaworld_funcs[145] = (CLASSFUNC)ypaworld_func145;
    ypaworld_funcs[146] = (CLASSFUNC)ypaworld_func146;
    ypaworld_funcs[147] = (CLASSFUNC)ypaworld_func147;
    ypaworld_funcs[148] = (CLASSFUNC)ypaworld_func148;
    ypaworld_funcs[149] = (CLASSFUNC)ypaworld_func149;
    ypaworld_funcs[150] = (CLASSFUNC)ypaworld_func150;
    ypaworld_funcs[151] = (CLASSFUNC)ypaworld_func151;
    ypaworld_funcs[153] = (CLASSFUNC)ypaworld_func153;
    ypaworld_funcs[154] = (CLASSFUNC)ypaworld_func154;
    ypaworld_funcs[155] = (CLASSFUNC)ypaworld_func155;
    ypaworld_funcs[156] = (CLASSFUNC)ypaworld_func156;
    ypaworld_funcs[157] = (CLASSFUNC)ypaworld_func157;
    ypaworld_funcs[158] = (CLASSFUNC)ypaworld_func158;
    ypaworld_funcs[159] = (CLASSFUNC)ypaworld_func159;
    ypaworld_funcs[160] = NULL;
    ypaworld_funcs[161] = (CLASSFUNC)ypaworld_func161;
    ypaworld_funcs[162] = (CLASSFUNC)ypaworld_func162;
    ypaworld_funcs[163] = (CLASSFUNC)ypaworld_func163;
    ypaworld_funcs[164] = (CLASSFUNC)ypaworld_func164;
    ypaworld_funcs[165] = (CLASSFUNC)ypaworld_func165;
    ypaworld_funcs[166] = (CLASSFUNC)ypaworld_func166;
    ypaworld_funcs[167] = (CLASSFUNC)ypaworld_func167;
    ypaworld_funcs[168] = (CLASSFUNC)ypaworld_func168;
    ypaworld_funcs[169] = (CLASSFUNC)ypaworld_func169;
    ypaworld_funcs[170] = (CLASSFUNC)ypaworld_func170;
    ypaworld_funcs[171] = (CLASSFUNC)ypaworld_func171;
    ypaworld_funcs[172] = (CLASSFUNC)ypaworld_func172;
    ypaworld_funcs[173] = (CLASSFUNC)ypaworld_func173;
    ypaworld_funcs[174] = (CLASSFUNC)ypaworld_func174;
    ypaworld_funcs[175] = (CLASSFUNC)ypaworld_func175;
    ypaworld_funcs[176] = (CLASSFUNC)ypaworld_func176;
    ypaworld_funcs[177] = (CLASSFUNC)ypaworld_func177;
    ypaworld_funcs[179] = (CLASSFUNC)ypaworld_func179;
    ypaworld_funcs[180] = (CLASSFUNC)ypaworld_func180;
    ypaworld_funcs[181] = (CLASSFUNC)ypaworld_func181;
    ypaworld_funcs[182] = (CLASSFUNC)ypaworld_func182;
    ypaworld_funcs[183] = (CLASSFUNC)ypaworld_func183;
    ypaworld_funcs[184] = (CLASSFUNC)ypaworld_func184;
    ypaworld_funcs[185] = (CLASSFUNC)ypaworld_func185;

    ypaworld_class_descr.parent = "base.class";

    ypaworld_class_descr.vtbl = ypaworld_funcs;
    ////ypaworld_class_descr.varSize = sizeof(__NC_STACK_ypaworld);
    ypaworld_class_descr.varSize = sizeof(NC_STACK_ypaworld) - offsetof(NC_STACK_ypaworld, stack__ypaworld); //// HACK
    ypaworld_class_descr.field_A = 0;
    return &ypaworld_class_descr;
}
