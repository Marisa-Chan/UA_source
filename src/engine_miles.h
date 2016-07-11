#ifndef ENGINE_MILES_H_INCLUDED
#define ENGINE_MILES_H_INCLUDED

#include "engine_tform.h"
#include "sample.h"

extern stored_functions_engine miles_engine_vtbl;


struct samples_collection1;

struct sndFXprm
{
    int time;
    int slot;
    float mag0;
    float mag1;
};

struct __attribute__((packed)) sndFXprm2 : sndFXprm
{
    float mute;
    xyz pos;
};

struct sndExt
{
    sampl *sample;
    int16_t field_4;
    int16_t field_6;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
};

struct sndExtends
{
    int cnt;
    sndExt sndExts[8];
};

struct userdata_sample_info
{
    sampl *psampl;
    sndFXprm *field_4;
    sndFXprm2 *field_8;
    sndExtends *field_C;
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
    xyz field_0;
    float field_C;
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
void sub_423F74(samples_collection1 *a1, int a2);
void sub_424000(samples_collection1 *smpls, int a2);
int sub_4448C0(int *a1);
void sub_423DD8(samples_collection1 *smpls);
void sub_424CC8();

void milesEngine__setter(unsigned int, ...);
void milesEngine__getter(unsigned int, ...);

void sb_0x4242e0(samples_collection1 *smpls);
mat3x3 *sb_0x424c74(); //Update sounds and return shake matrix

void sub_423EFC(int a1, xyz *a2, xyz *a3, mat3x3 *a4);

#endif // ENGINE_MILES_H_INCLUDED
