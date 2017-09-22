#include "includes.h"
#include "engine_tform.h"
#include "math.h"
#include "utils.h"


SinCos *TFEngine::SinCos_table = NULL;

key_value_stru tform_keys[4] = {{"tform.backplane", KEY_TYPE_DIGIT, 0x1000},
    {"tform.frontplane", KEY_TYPE_DIGIT, 0x10},
    {"tform.zoomx", KEY_TYPE_DIGIT, 0x140},
    {"tform.zoomy", KEY_TYPE_DIGIT, 0xC8}
};

TFEngine TFe;

int TFEngine::init()
{
    get_keyvalue_from_ini(0, tform_keys, 4);

    SinCos_table = new SinCos[361];

    if ( !SinCos_table)
        return 0;

    for (int i = 0; i < 361; i++)
    {
        float rad = i * C_PI_180;

        SinCos_table[i].sin = sin(rad);
        SinCos_table[i].cos = cos(rad);
    }

    return 1;
}

void TFEngine::deinit()
{
    if (SinCos_table)
        delete[] SinCos_table;

    SinCos_table = NULL;
}
