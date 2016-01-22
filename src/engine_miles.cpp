#include "includes.h"
#include "engine_miles.h"

stored_functions *classvtbl_get_milesEngine();

int milesEngine__init(unsigned int, ...);
void milesEngine__deinit();
void milesEngine__setter(unsigned int, ...);
void milesEngine__getter(unsigned int, ...);


stored_functions_engine miles_engine_vtbl = {milesEngine__init, milesEngine__deinit, milesEngine__setter, milesEngine__getter};



class_stored miles_engine_off (NULL, NULL, "MC2engines:miles.engine", classvtbl_get_milesEngine);

userdata_sample_info *soundSources[8];
int ShakeFXs[4];
int palFXs[8];

int dword_546DD8;
xyz stru_547018;
xyz stru_547024;
mat3x3 stru_547030;



stored_functions *classvtbl_get_milesEngine()
{
    return (stored_functions *)&miles_engine_vtbl;
}

int milesEngine__init(unsigned int, ...)
{
////TODO
    return 1;
}

void milesEngine__deinit()
{
////TODO
}

void milesEngine__setter(unsigned int, ...)
{
////TODO
}

void milesEngine__getter(unsigned int, ...)
{
////TODO
}


void sub_423DB0(samples_collection1 *smpls)
{
    memset(smpls, 0, sizeof(samples_collection1));

    for (int i = 0; i < 16; i++)
    {
        smpls->samples_data[i].parent_sample_collection = smpls;
    }
}

int sub_4444D4(CDAUDIO_t *arg)
{
    printf("MAKE ME %s\n", "sub_4444D4");
    return 0;
}

userdata_sample_info *sub_423F74(samples_collection1 *a1, int a2)
{
    userdata_sample_info *result = &a1->samples_data[a2];

    result->field_20 = dword_546F10;
    if ( result->field_13 & 2 || result->field_0 )
    {
        result->field_1E = 0;
        result->field_12 |= 2;
        result->field_12 &= 0xFB;
    }
    if ( result->field_12 & 8 )
    {
        result->field_12 |= 0x10;
        result->field_12 &= 0xDF;
        result->field_24 = result->field_4[1];
    }
    if ( result->field_12 & 0x40 )
    {
        result->field_12 |= 0x80;
        result->field_13 &= 0xFE;
        result->field_28 = result->field_8[1];
    }
    return result;
}

void sub_424000(samples_collection1 *smpls, int a2)
{
    smpls->samples_data[a2].field_12 &= 0x6D;
}

int sub_4448C0(int *a1)
{
    //printf("STUB %s\n", "sub_4448C0");
    return 1;//AIL_driver && dword_546DDC && AIL_redbook_set_volume(AIL_driver, *a1) == *a1;
}

void sub_423DD8(samples_collection1 *smpls)
{
    printf("STUB %s\n", "sub_423DD8");
}

void sb_0x4242e0(samples_collection1 *smpls)
{
    //printf("STUB %s\n", "sb_0x4242e0");
}

void sb_0x424c74()
{
    printf("STUB %s\n", "sb_0x424c74");
}

void sub_423EFC(int a1, xyz *a2, xyz *a3, mat3x3 *a4)
{
    dword_546F10 += a1;

    stru_547018.sx = a2->sx;
    stru_547018.sy = a2->sy;
    stru_547018.sz = a2->sz;

    stru_547024.sx = a3->sx;
    stru_547024.sy = a3->sy;
    stru_547024.sz = a3->sz;

    memcpy(&stru_547030, a4, sizeof(mat3x3));

    memset(soundSources, 0, sizeof(stored_functions) * 8);
    memset(palFXs, 0, sizeof(int) * 8);
    memset(ShakeFXs, 0, sizeof(int) * 4);

    dword_546DD8 = dword_546F10;
}
