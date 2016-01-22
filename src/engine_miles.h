#ifndef ENGINE_MILES_H_INCLUDED
#define ENGINE_MILES_H_INCLUDED

#include "engine_tform.h"

extern class_stored miles_engine_off;


struct samples_collection1;

struct userdata_sample_info
{
    int field_0;
    float *field_4;
    float *field_8;
    int *field_C;
    int16_t volume;
    char field_12;
    char field_13;
    int pitch;
    samples_collection1 *parent_sample_collection;
    int16_t field_1C;
    int16_t field_1E;
    int field_20;
    float field_24;
    float field_28;
    int16_t vol;
    int16_t pan;
    int rate;
};

struct samples_collection1
{
    float field_0;
    float field_4;
    float field_8;
    int field_C;
    float field_10;
    float field_14;
    userdata_sample_info samples_data[16];
};

struct CDAUDIO_t
{
    int command;
    int track_id;
    int field_8;
    int field_C;
};

void sub_423DB0(samples_collection1 *smpls);
int sub_4444D4(CDAUDIO_t *arg);
userdata_sample_info *sub_423F74(samples_collection1 *a1, int a2);
void sub_424000(samples_collection1 *smpls, int a2);
int sub_4448C0(int *a1);
void sub_423DD8(samples_collection1 *smpls);

void milesEngine__setter(unsigned int, ...);

void sb_0x4242e0(samples_collection1 *smpls);
void sb_0x424c74();

void sub_423EFC(int a1, xyz *a2, xyz *a3, mat3x3 *a4);

#endif // ENGINE_MILES_H_INCLUDED
