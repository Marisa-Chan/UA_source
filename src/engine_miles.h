#ifndef ENGINE_MILES_H_INCLUDED
#define ENGINE_MILES_H_INCLUDED

#include <array>
#include "engine_tform.h"
#include "sample.h"
#include "wrapal.h"

#define AUDIO_CHANNELS   16

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
    std::array<sndExt, 8> sndExts;

    sndExtends()
    {
        clear();
    }

    void clear()
    {
        cnt = 0;

        for(auto &x : sndExts)
            x.clear();
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

    userdata_sample_info()
    {
        clear();
    }

    void clear()
    {
        psampl = NULL;
        paletteFX = NULL;
        shakeFX = NULL;
        smplExt = NULL;
        volume = 0;
        flags = 0;
        pitch = 0;
        parent_sample_collection = NULL;
        priority = 0;
        fragmentID = 0;
        startTime = 0;
        palMag = 0.0;
        shkMag = 0.0;
        resultVol = 0;
        resultPan = 0;
        resultRate = 0;
    }
};

struct samples_collection1
{
    vec3d field_0;
    vec3d field_C;
    userdata_sample_info samples_data[16];

    samples_collection1()
    {
        clear();
    }

    void clear()
    {
        field_0 = vec3d(0.0);
        field_C = vec3d(0.0);

        for(int i = 0; i < 16; i++)
            samples_data[i].clear();
    }
};

class SFXEngine
{
public:
    int init();
    void deinit();

    void setMasterVolume(int);
    void setReverseStereo(bool);

    int getMasterVolume();


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
    userdata_sample_info *SndGetTopShake();

protected:

    float audio_rnd();
    void audio_InsertSoundSource(userdata_sample_info *smpl);
    int sub_423B3C(userdata_sample_info *smpl, int a2, int *a3);
    void sb_0x4242e0__sub0(samples_collection1 *smpls, float a2);
    void audio_InsertPalFX(userdata_sample_info *smpl);
    void sb_0x4242e0__sub1(samples_collection1 *smpls, float a2);
    void InsertShakeFX(userdata_sample_info *smpl);
    void sb_0x4242e0__sub2(samples_collection1 *smpls, float a2);
    void sb_0x424c74__sub0();
    void sb_0x424c74__sub1();
    void sb_0x424c74__sub2__sub1(userdata_sample_info *smpl);
    void sb_0x424c74__sub2__sub0(int id, userdata_sample_info *smpl);
    void sb_0x424c74__sub2();
    void sb_0x424c74__sub3();
    void sb_0x424c74__sub4();
    void UpdateMusic();

    static void sound_eos_clbk(void *_smpl);

private:
    SFXEngine() {}; // Private constructor for only one instance declared in this class

public:
    struct sample
    {
        walsmpl *hSample;
        userdata_sample_info *sndSouce;

        sample()
        {
            hSample = NULL;
            sndSouce = NULL;
        }
    };

    waldev *digDriver;
    int audio_volume;
    int audio_channels;
    int audio_num_palfx;
    int dword_546E14; // num_shakeFX
    sample snd_channels[AUDIO_CHANNELS];
    userdata_sample_info *soundSources[AUDIO_CHANNELS];
    userdata_sample_info *palFXs[8];
    userdata_sample_info *ShakeFXs[4];
    bool audio_rev_stereo;
    int dword_546F0C;
    size_t currentTime;
    int dword_546F14;
    float flt_546F18[64];
    vec3d stru_547018;
    vec3d stru_547024;
    mat3x3 stru_547030;
    mat3x3 shakeMatrix;

    walmus *musPlayer;
    bool musWait;
    bool musOn;
    size_t musWaitSTime;
    size_t musWaitDelay;
    int musMinDelay;
    int musMaxDelay;
    int musTrack;

public:
    static SFXEngine SFXe;
};

#endif // ENGINE_MILES_H_INCLUDED
