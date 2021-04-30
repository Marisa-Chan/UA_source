#ifndef ENGINE_MILES_H_INCLUDED
#define ENGINE_MILES_H_INCLUDED

#include <array>
#include "gfx.h"
#include "../wrapal.h"

#define AUDIO_CHANNELS   16

struct TSndCarrier;

struct TSndFXParam
{
    int time = 0;
    int slot = 0;
    float mag0 = 0.0;
    float mag1 = 0.0;
};

struct TSndFxPosParam : TSndFXParam
{
    float mute = 0.0;
    vec3d pos;
};

struct TSampleData
{
    void *Data = NULL;
    int bufsz = 0;
    int field_8 = 0;
    int SampleRate = 0;
    int field_10 = 0;
};

struct TSampleParams
{
    TSampleData *Sample = NULL;
    uint8_t Loop = 0;
    int16_t Vol = 0;
    int SampleRate = 0;
    size_t Offset = 0;
    int SampleCnt = 0;
    int rlOffset = 0;
    int rlSmplCnt = 0;
};

struct TSoundSource
{
    enum Flags
    {
        FLAG_LOOP     = (1 << 0),
        FLAG_ENABLED  = (1 << 1),
        FLAG_PLAY     = (1 << 2),
        
        FLAG_PFX      = (1 << 3),
        FLAG_PFXEN    = (1 << 4),
        FLAG_PFXPL    = (1 << 5),
        
        FLAG_SHK      = (1 << 6),
        FLAG_SHKEN    = (1 << 7),
        FLAG_SHKPL    = (1 << 8),
        
        FLAG_FRAGM    = (1 << 9),
    };
    
    
    TSampleData *PSample = NULL;
    TSndFXParam *PPFx = NULL;
    TSndFxPosParam *PShkFx = NULL;
    std::vector<TSampleParams> *PFragments = NULL;
    int16_t Volume = 0;
    uint16_t Flags = 0;
    int Pitch = 0;
    TSndCarrier *PCarrier = NULL;
    int16_t Priority = 0;
    size_t CurrentFrag = 0;
    size_t StartTime = 0;
    float PFxMag = 0.0;
    float ShkMag = 0.0;
    int16_t ResultVol = 0;
    int16_t ResultPan = 0;
    int ResultRate = 0;
    
    inline bool IsLoop()
    {
        return (Flags & FLAG_LOOP) != 0;
    }
    
    inline bool IsEnabled()
    {
        return (Flags & FLAG_ENABLED) != 0;
    }
    
    inline bool IsPlay()
    {
        return (Flags & FLAG_PLAY) != 0;
    }
    
    inline bool IsPFx()
    {
        return (Flags & FLAG_PFX) != 0;
    }
    
    inline bool IsPFxEnabled()
    {
        return (Flags & FLAG_PFXEN) != 0;
    }
    
    inline bool IsPFxPlay()
    {
        return (Flags & FLAG_PFXPL) != 0;
    }
    
    inline bool IsShk()
    {
        return (Flags & FLAG_SHK) != 0;
    }
    
    inline bool IsShkEnabled()
    {
        return (Flags & FLAG_SHKEN) != 0;
    }
    
    inline bool IsShkPlay()
    {
        return (Flags & FLAG_SHKPL) != 0;
    }
    
    inline bool IsFragmented()
    {
        return (Flags & FLAG_FRAGM) != 0;
    }
    
    
    
    inline void SetLoop(bool en)
    {
        if (en)
            Flags |= FLAG_LOOP;
        else
            Flags &= ~FLAG_LOOP;
    }
    
    inline void SetEnabled(bool en)
    {
        if (en)
            Flags |= FLAG_ENABLED;
        else
            Flags &= ~FLAG_ENABLED;
    }
    
    inline void SetPlay(bool en)
    {
        if (en)
            Flags |= FLAG_PLAY;
        else
            Flags &= ~FLAG_PLAY;
    }
    
    inline void SetPFx(bool en)
    {
        if (en)
            Flags |= FLAG_PFX;
        else
            Flags &= ~FLAG_PFX;
    }
    
    inline void SetPFxEnable(bool en)
    {
        if (en)
            Flags |= FLAG_PFXEN;
        else
            Flags &= ~FLAG_PFXEN;
    }
    
    inline void SetPFxPlay(bool en)
    {
        if (en)
            Flags |= FLAG_PFXPL;
        else
            Flags &= ~FLAG_PFXPL;
    }
    
    inline void SetShk(bool en)
    {
        if (en)
            Flags |= FLAG_SHK;
        else
            Flags &= ~FLAG_SHK;
    }
    
    inline void SetShkEnable(bool en)
    {
        if (en)
            Flags |= FLAG_SHKEN;
        else
            Flags &= ~FLAG_SHKEN;
    }
    
    inline void SetShkPlay(bool en)
    {
        if (en)
            Flags |= FLAG_SHKPL;
        else
            Flags &= ~FLAG_SHKPL;
    }
    
    inline void SetFragmented(bool en)
    {
        if (en)
            Flags |= FLAG_FRAGM;
        else
            Flags &= ~FLAG_FRAGM;
    }
    
    TSoundSource(TSndCarrier *parent)
    : PCarrier(parent)
    {}
};

struct TSndCarrier
{
    vec3d Position;
    vec3d Vector;
    std::vector<TSoundSource> Sounds;
    
    void Clear()
    {
        Position = vec3d();
        Vector = vec3d();
        Sounds.clear();
    }
    
    void Resize(size_t sz)
    {
        Clear();
        
        for(size_t i = 0; i < sz; i++)
            Sounds.emplace_back(this);
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


    void startSound(TSndCarrier *a1, int a2);
    void sub_424000(TSndCarrier *smpls, int a2);
    void StopCarrier(TSndCarrier *smpls);
    void StopSource(TSoundSource *snd);
    void StopPlayingSounds();
    
    void ForceStopSource(TSndCarrier *smpls, int id)
    {
        StopSource(&smpls->Sounds.at(id));
    }

    void SetMusicVolume(int vol);
    void SetMusicIgnoreCommandsFlag(bool flag);
    void SetMusicTrack(int trackID, int minDelay, int maxDelay);
    void StopMusicTrack();
    void PlayMusicTrack();

    void UpdateSoundCarrier(TSndCarrier *smpls);
    const mat3x3 &sb_0x424c74(); //Update sounds and return shake matrix

    void sub_423EFC(int a1, const vec3d &a2, const vec3d &a3, const mat3x3 &a4);
    TSoundSource *SndGetTopShake();

protected:

    float audio_rnd();
    void audio_InsertSoundSource(TSoundSource *smpl);
    int sub_423B3C(TSoundSource *smpl, int a2, int *a3);
    void SoundCarrierProcessSounds(TSndCarrier *smpls, float a2);
    void audio_InsertPalFX(TSoundSource *smpl);
    void SoundCarrierProcessPFx(TSndCarrier *smpls, float a2);
    void InsertShakeFX(TSoundSource *smpl);
    void SoundCarrierProcessShake(TSndCarrier *smpls, float a2);
    void sb_0x424c74__sub0();
    void sb_0x424c74__sub1();
    void sb_0x424c74__sub2__sub1(TSoundSource *smpl);
    void sb_0x424c74__sub2__sub0(int id, TSoundSource *smpl);
    void sb_0x424c74__sub2();
    void sb_0x424c74__sub3();
    void sb_0x424c74__sub4();
    void UpdateMusic();

    static void sound_eos_clbk(void *_smpl);

private:
    SFXEngine() {}; // Private constructor for only one instance declared in this class

public:
    struct TSoundChannel
    {
        walsmpl *hSample;
        TSoundSource *sndSouce;

        TSoundChannel()
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
    TSoundChannel snd_channels[AUDIO_CHANNELS];
    TSoundSource *soundSources[AUDIO_CHANNELS];
    TSoundSource *palFXs[8];
    TSoundSource *ShakeFXs[4];
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
    
    WALStream *AudioStream;

public:
    static SFXEngine SFXe;
};

#endif // ENGINE_MILES_H_INCLUDED
