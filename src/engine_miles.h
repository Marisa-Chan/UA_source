#ifndef ENGINE_MILES_H_INCLUDED
#define ENGINE_MILES_H_INCLUDED

#include "engine_tform.h"
#include "sample.h"

struct samples_collection1;

struct sndFXprm
{
    int time;
    int slot;
    float mag0;
    float mag1;

    sndFXprm()
    {
        clear();
    }

    void clear()
    {
        time = 0;
        slot = 0;
        mag0 = 0.0;
        mag1 = 0.0;
    }
};

struct sndFXprm2 : sndFXprm
{
    float mute;
    vec3d pos;

    sndFXprm2()
    {
        clear();
    }

    void clear()
    {
        pos = vec3d(0.0, 0.0, 0.0);
        time = 0;
        slot = 0;
        mag0 = 0.0;
        mag1 = 0.0;
        mute = 0.0;
    }
};

struct sndExt
{
    sampl *sample;
    uint8_t loop;
    int16_t vol;
    int smplRate;
    size_t offset;
    int smplCnt;
    int rlOffset;
    int rlSmplCnt;

    sndExt()
    {
        clear();
    }

    void clear()
    {
        sample = NULL;
        loop = 0;
        vol = 0;
        smplRate = 0;
        offset = 0;
        smplCnt = 0;
        rlOffset = 0;
        rlSmplCnt = 0;
    }
};

struct sndExtends
{
    int cnt;
    sndExt sndExts[8];

    sndExtends()
    {
        clear();
    }

    void clear()
    {
        cnt = 0;

        for(int i = 0; i < 8; i++)
            sndExts[i].clear();
    }
};

struct userdata_sample_info
{
    sampl *psampl;
    sndFXprm *paletteFX;
    sndFXprm2 *shakeFX;
    sndExtends *smplExt;
    int16_t volume;
    uint16_t flags;
    int pitch;
    samples_collection1 *parent_sample_collection;
    int16_t priority;
    int16_t fragmentID;
    size_t startTime;
    float palMag;
    float shkMag;
    int16_t resultVol;
    int16_t resultPan;
    int resultRate;
};

struct samples_collection1
{
    vec3d field_0;
    vec3d field_C;
    userdata_sample_info samples_data[16];
};

void sub_423DB0(samples_collection1 *smpls);
void startSound(samples_collection1 *a1, int a2);
void sub_424000(samples_collection1 *smpls, int a2);
void sub_423DD8(samples_collection1 *smpls);
void sub_424CC8();

void SetMusicVolume(int vol);
void SetMusicIgnoreCommandsFlag(bool flag);
void SetMusicTrack(int trackID, int minDelay, int maxDelay);
void StopMusicTrack();
void PlayMusicTrack();

void sb_0x4242e0(samples_collection1 *smpls);
const mat3x3 &sb_0x424c74(); //Update sounds and return shake matrix

void sub_423EFC(int a1, const vec3d &a2, const vec3d &a3, const mat3x3 &a4);

class SFXEngine
{
public:
    SFXEngine() {};

    int init();
    void deinit();

    void setMasterVolume(int);
    void setReverseStereo(int);

    int getMasterVolume();
};

extern SFXEngine SFXe;

#endif // ENGINE_MILES_H_INCLUDED
