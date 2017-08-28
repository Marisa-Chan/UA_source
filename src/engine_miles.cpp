#include "includes.h"
#include "engine_miles.h"

#include "wrapal.h"

#include <math.h>

#define AUDIO_CHANNELS   16


SFXEngine SFXe;



struct sample
{
    walsmpl *hSample;
    userdata_sample_info *sndSouce;
};

struct soundSys
{
    //HDIGDRIVER digDriver;
    waldev *digDriver;
    int audio_volume;
    int audio_channels;
    int audio_num_palfx;
    int dword_546E14; // num_shakeFX
    sample snd_channels[AUDIO_CHANNELS];
    userdata_sample_info *soundSources[AUDIO_CHANNELS];
    userdata_sample_info *palFXs[8];
    userdata_sample_info *ShakeFXs[4];
    int audio_rev_stereo;
    int dword_546F0C;
    size_t currentTime;
    int dword_546F14;
    float flt_546F18[64];
    xyz stru_547018;
    xyz stru_547024;
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
};

static soundSys sndSys;

key_value_stru audio_keys[4] =
{
    {"audio.channels", KEY_TYPE_DIGIT, 4},
    {"audio.volume", KEY_TYPE_DIGIT, 127},
    {"audio.num_palfx", KEY_TYPE_DIGIT, 4},
    {"audio.rev_stereo", KEY_TYPE_BOOL, 0}
};

void wrapper_setSampleVRP(void *, walsmpl *hSample, int rate, int volume, int pan)
{
    hSample->playback_rate(rate);
    hSample->volume(volume);
    hSample->pan(pan);
}

void wrapper_playSound(waldev *driver, walsmpl *hSample, void (*funceos)(void *), void *start, int len, int rate, int vol, int pan, int loop_cnt, int pos)
{
    hSample->reset();
    hSample->EOS_callback(funceos);
    hSample->address(start, len, rate, AL_FORMAT_MONO8);
    hSample->volume(vol);
    hSample->pan(pan);
    hSample->loop_count(loop_cnt);
    hSample->play();

    if (pos > 0)
        hSample->position(pos);
}


int SFXEngine::init()
{
    memset(&sndSys, 0, sizeof(sndSys));

    get_keyvalue_from_ini(NULL, audio_keys, 4);

    sndSys.audio_channels = audio_keys[0].value.val;
    sndSys.audio_volume = audio_keys[1].value.val;
    sndSys.audio_num_palfx = audio_keys[2].value.val;
    sndSys.audio_rev_stereo = audio_keys[3].value.val;

    if ( sndSys.audio_volume > 127 )
        sndSys.audio_volume = 127;

    if ( sndSys.audio_channels < 1 )
        sndSys.audio_channels = 1;

    if ( sndSys.audio_channels > AUDIO_CHANNELS)
        sndSys.audio_channels = AUDIO_CHANNELS;

    if ( sndSys.audio_num_palfx > 8 )
        sndSys.audio_num_palfx = 8;

    sndSys.dword_546E14 = 4;
    sndSys.dword_546F14 = 0;

    for (int i = 0; i < 64; i++)
        sndSys.flt_546F18[i] = (float)(16383 - (rand() & 0x7FFF) ) / 16383.0;

    sndSys.digDriver = new waldev(); //miles_init(sndSys.audio_channels);

    if ( sndSys.digDriver->inited() )
    {
        for (int i = 0; i < sndSys.audio_channels; i++)
        {
            sndSys.snd_channels[i].hSample = sndSys.digDriver->newSample(); //miles_allocSample(sndSys.digDriver);
            sndSys.snd_channels[i].hSample->setMasterVolume(sndSys.audio_volume);

            if ( !sndSys.snd_channels[i].hSample )
            {
                sndSys.audio_channels = i;
                break;
            }
        }

        sndSys.musPlayer = sndSys.digDriver->createMusicPlayer();
    }
    else
    {
        ypa_log_out("Couldn't initialize audio!\n");
    }

    return 1;
}

void SFXEngine::deinit()
{
    //deinit_redbook();

    if (sndSys.digDriver)
    {
        delete sndSys.digDriver;
        sndSys.digDriver = NULL;
    }
}

void SFXEngine::setMasterVolume(int vol)
{
    sndSys.audio_volume = vol;

    for (int i = 0; i < sndSys.audio_channels; i++)
        sndSys.snd_channels[i].hSample->setMasterVolume(sndSys.audio_volume);

//    wrapper_setVolume(sndSys.digDriver, vol);
}

void SFXEngine::setReverseStereo(int rev)
{
    sndSys.audio_rev_stereo = rev;
}

int SFXEngine::getMasterVolume()
{
    return sndSys.audio_volume;
}

void sub_423DB0(samples_collection1 *smpls)
{
    memset(smpls, 0, sizeof(samples_collection1));

    for (int i = 0; i < 16; i++)
    {
        smpls->samples_data[i].parent_sample_collection = smpls;
    }
}

void startSound(samples_collection1 *smpls, int a2)
{
    userdata_sample_info *result = &smpls->samples_data[a2];

    result->startTime = sndSys.currentTime;

    if ( result->flags & 0x200 || result->psampl )
    {
        result->fragmentID = 0;
        result->flags |= 2;
        result->flags &= ~4;
    }
    if ( result->flags & 8 )
    {
        result->flags |= 0x10;
        result->flags &= ~0x20;
        result->palMag = result->paletteFX->mag0;
    }
    if ( result->flags & 0x40 )
    {
        result->flags |= 0x80;
        result->flags &= ~0x100;
        result->shkMag = result->shakeFX->mag0;
    }
}

void sub_424000(samples_collection1 *smpls, int a2)
{
    smpls->samples_data[a2].flags &= ~(0x80 | 0x10 | 2);
}




void SetMusicIgnoreCommandsFlag(bool flag)
{
    if (!flag)
        sndSys.musTrack = 0;

    sndSys.musOn = flag;
}

void SetMusicVolume(int vol)
{
    if (sndSys.digDriver && sndSys.musPlayer && sndSys.musOn)
    {
        sndSys.musPlayer->volume(vol);
    }
}

void StopMusicTrack()
{
    if (sndSys.digDriver && sndSys.musPlayer && sndSys.musOn)
    {
        sndSys.musPlayer->stop();
        sndSys.musTrack = 0;
    }
}

void PlayMusicTrack()
{
    if (sndSys.digDriver && sndSys.musPlayer && sndSys.musOn  && sndSys.musTrack > 0)
    {
        sndSys.musPlayer->play();
    }
}

void SetMusicTrack(int trackID, int minDelay, int maxDelay)
{
    if (sndSys.digDriver && sndSys.musPlayer && sndSys.musOn && trackID > 0)
    {
        char buf[64];
        sprintf(buf, "%d", trackID);

        std::string str = "music/";
        str += buf;
        str += ".ogg";

        if ( sndSys.musPlayer->open(str.c_str()) )
        {
            sndSys.musTrack = trackID;
            sndSys.musMinDelay = minDelay;
            sndSys.musMaxDelay = maxDelay;
        }
        else
            sndSys.musTrack = 0;
    }
}

// Shutoff samples set
void sub_423DD8(samples_collection1 *smpls)
{
    for (int i = 0; i < sndSys.audio_channels; i++)
    {
        if ( sndSys.digDriver )
        {
            if (sndSys.snd_channels[i].sndSouce && sndSys.snd_channels[i].sndSouce->parent_sample_collection == smpls )
            {
                sndSys.snd_channels[i].sndSouce->flags &= ~(4 | 2);

                sndSys.snd_channels[i].hSample->stop();

                sndSys.snd_channels[i].sndSouce = NULL;
            }

            if (sndSys.soundSources[i] && sndSys.soundSources[i]->parent_sample_collection == smpls )
            {
                sndSys.soundSources[i]->flags &= ~(4 | 2);
                sndSys.soundSources[i] = NULL;
            }
        }
    }

    for (int i = 0; i < sndSys.audio_num_palfx; i++)
    {
        if ( sndSys.palFXs[i] && sndSys.palFXs[i]->parent_sample_collection == smpls )
        {
            sndSys.palFXs[i]->flags &= ~(0x20 | 0x10);
            sndSys.palFXs[i] = NULL;
        }
    }

    for (int i = 0; i < sndSys.dword_546E14; i++)
    {
        if ( sndSys.ShakeFXs[i] && sndSys.ShakeFXs[i]->parent_sample_collection == smpls )
        {
            sndSys.ShakeFXs[i]->flags &= ~(0x100 | 0x80);
            sndSys.ShakeFXs[i] = NULL;
        }
    }

    for (int i = 0; i < 16; i++)
        smpls->samples_data[i].flags &= ~(0x80 | 0x10 | 2);

}

float audio_rnd()
{
    float tmp = sndSys.flt_546F18[sndSys.dword_546F14];

    sndSys.dword_546F14++;

    if ( sndSys.dword_546F14 >= 64 )
        sndSys.dword_546F14 = 0;

    return tmp;
}

void audio_InsertSoundSource(userdata_sample_info *smpl)
{
    int min_i = -1;
    int v3 = 1000000;

    if ( sndSys.digDriver->inited() )
    {
        for (int i = 0; i < sndSys.audio_channels; i++)
        {
            if ( sndSys.soundSources[i] )
            {
                if ( sndSys.soundSources[i]->priority < v3 )
                {
                    min_i = i;
                    v3 = sndSys.soundSources[i]->priority;
                }
            }
            else
            {
                min_i = i;
                v3 = 0;
                i = sndSys.audio_channels;
            }
        }

        if ( min_i == -1 )
            ypa_log_out("-> audio_InsertSoundSource(): <min_i> not initialized.\n");

        if ( min_i != -1 && v3 < smpl->priority )
            sndSys.soundSources[min_i] = smpl;
    }
}

int sub_423B3C(userdata_sample_info *smpl, int a2, int *a3)
{
    int i = 0;

    for (i = 0; i < smpl->smplExt->cnt; i++)
    {
        if ( !smpl->smplExt->sndExts[0].loop )
            break;

        int v9 = smpl->smplExt->sndExts[0].smplRate + smpl->smplExt->sndExts[0].sample->SampleRate + smpl->pitch;

        if ( v9 <= 0 )
            v9 = 1;

        int v10 = (smpl->smplExt->sndExts[0].loop * smpl->smplExt->sndExts[0].rlSmplCnt / 1024) / v9;

        if ( a2 < v10 )
            break;

        a2 -= v10;
    }

    if ( a3 )
        *a3 = a2;

    return i;
}

void sb_0x4242e0__sub0(samples_collection1 *smpls, float a2)
{
    float v10 = a2 / 200.0;

    for (int i = 0; i < 16; i++)
    {
        userdata_sample_info *v2 = &smpls->samples_data[i];

        if ( v2->psampl || v2->flags & 0x200 )
        {
            int v5 = 0;

            if ( v2->flags & 2 )
            {
                int v6 = sndSys.currentTime - v2->startTime;

                if ( v2->flags & 0x200 )
                {
                    if ( v2->flags & 4 )
                        v5 = v2->fragmentID;
                    else
                        v5 = sub_423B3C(v2, sndSys.currentTime - v2->startTime, NULL);
                }

                int ok = 1;

                if ( !(v2->flags & 1) && !(v2->flags & 4) && v6 > 0 )
                {
                    if ( v2->flags & 0x200 )
                    {
                        if ( v5 >= v2->smplExt->cnt )
                        {
                            v2->flags &= ~2;
                            ok = 0;
                        }
                    }
                    else if ( (v6 * (v2->pitch + v2->psampl->SampleRate) / 1024) > v2->psampl->bufsz )
                    {
                        v2->flags &= ~2;
                        ok = 0;
                    }
                }

                if (ok)
                {
                    v2->resultVol = v2->volume;

                    if ( v2->flags & 0x200 )
                        v2->resultVol += v2->smplExt->sndExts[v5].vol;

                    if ( v10 >= 1.0 )
                        v2->resultVol /= v10;

                    if ( v2->resultVol > 4 )
                    {
                        v2->priority = v2->resultVol;
                        audio_InsertSoundSource(v2);
                    }
                }

            }
        }
    }
}


void audio_InsertPalFX(userdata_sample_info *smpl)
{
    int min_i = -1;
    int a1 = 1000000;

    for (int i = 0; i < sndSys.audio_num_palfx; i++)
    {
        if ( sndSys.palFXs[i] )
        {
            if ( sndSys.palFXs[i]->palMag )
            {
                a1 = sndSys.palFXs[i]->palMag;
                min_i = i;
            }
        }
        else
        {
            min_i = i;
            a1 = 0;
            i = sndSys.audio_num_palfx;
        }
    }

    if ( min_i == -1 )
        ypa_log_out("-> audio_InsertPalFX(): <min_i> not initialized.\n");

    if ( min_i != -1 && a1 < smpl->palMag )
        sndSys.palFXs[min_i] = smpl;
}

void sb_0x4242e0__sub1(samples_collection1 *smpls, float a2)
{
    float v6 = a2 / 300.0;

    for (int i = 0; i < 16; i++)
    {
        userdata_sample_info *v2 = &smpls->samples_data[i];

        if ( v2->flags & 8 )
        {
            if ( v2->flags & 0x10 )
            {
                if ( !(v2->flags & 1) && (size_t)(v2->startTime + v2->paletteFX->time) < sndSys.currentTime )
                    v2->flags &= ~0x10;
            }

            if ( v2->flags & 0x10 )
            {
                if ( v2->flags & 1 )
                    v2->palMag = v2->paletteFX->mag0;
                else
                    v2->palMag = (v2->paletteFX->mag1 - v2->paletteFX->mag0) * ((float)(sndSys.currentTime - v2->startTime) / (float)v2->paletteFX->time) + v2->paletteFX->mag0;

                if ( v6 >= 1.0 )
                    v2->palMag /= v6;

                audio_InsertPalFX(v2);
            }
        }
    }
}

void InsertShakeFX(userdata_sample_info *smpl)
{
    int min_i = -1;
    int var_1C = 1000;

    for (int i = 0; i < sndSys.dword_546E14; i++)
    {
        userdata_sample_info *v5 = sndSys.ShakeFXs[i];

        if ( v5 )
        {
            if ( var_1C > v5->shkMag )
            {
                var_1C = v5->shkMag;
                min_i = i;
            }
        }
        else
        {
            min_i = i;
            var_1C = 0;
            i = sndSys.dword_546E14;
        }
    }

    if ( min_i == -1 )
        ypa_log_out("-> audio_InsertShakeFX(): <min_i> not initialized.\n");

    if ( min_i != -1 && var_1C < smpl->shkMag )
        sndSys.ShakeFXs[min_i] = smpl;
}

void sb_0x4242e0__sub2(samples_collection1 *smpls, float a2)
{
    for (int i = 0; i < 16; i++)
    {
        userdata_sample_info *v2 = &smpls->samples_data[i];

        if ( v2->flags & 0x40 )
        {
            if ( v2->flags & 0x80 )
            {
                if ( !(v2->flags & 1) && (size_t)(v2->startTime + v2->shakeFX->time) < sndSys.currentTime )
                    v2->flags &= ~0x80;
            }

            if ( v2->flags & 0x80 )
            {
                if ( v2->flags & 1 )
                    v2->shkMag = v2->shakeFX->mag0;
                else
                    v2->shkMag = (v2->shakeFX->mag1 - v2->shakeFX->mag0) * ((float)(sndSys.currentTime - v2->startTime) / (float)v2->shakeFX->time) + v2->shakeFX->mag0;

                float v6 = a2 * v2->shakeFX->mute;

                if ( v6 >= 1.0 )
                    v2->shkMag /= v6;

                InsertShakeFX(v2);
            }
        }
    }
}

// Insert new sound
void sb_0x4242e0(samples_collection1 *smpls)
{
    float a2 = sqrt( POW2(smpls->field_0.sx - sndSys.stru_547018.sx) + POW2(smpls->field_0.sz - sndSys.stru_547018.sz) + POW2(smpls->field_0.sy - sndSys.stru_547018.sy) );

    if ( a2 < 6000.0 )
    {
        if ( sndSys.digDriver->inited() )
            sb_0x4242e0__sub0(smpls, a2);

        if ( a2 < 2400.0 )
        {
            if ( sndSys.audio_num_palfx )
                sb_0x4242e0__sub1(smpls, a2);

            sb_0x4242e0__sub2(smpls, a2);
        }
    }
}

void sb_0x424c74__sub0()
{
    //Free ended channels
    for (int i = 0; i < sndSys.audio_channels; i++)
    {
        userdata_sample_info *v2 = sndSys.snd_channels[i].sndSouce;

        if ( v2 )
        {
            int v3 = 1;

            for (int j = 0; j < sndSys.audio_channels; j++)
            {
                if ( v2 == sndSys.soundSources[j] )
                {
                    v3 = 0;
                    break;
                }
            }

            if ( v3 )
            {
                v2->flags &= ~4;

                sndSys.snd_channels[i].sndSouce = NULL;

                sndSys.snd_channels[i].hSample->stop();
            }
        }
    }
}

void sb_0x424c74__sub1()
{
    //Find free snd_channel
    int v1 = 0;

    for (int i = 0; i < sndSys.audio_channels; i++)
    {
        userdata_sample_info *v3 = sndSys.soundSources[i];

        if ( v3 && !(v3->flags & 4) )
        {
            while ( v1 < sndSys.audio_channels )
            {
                if ( !sndSys.snd_channels[v1].sndSouce )
                {
                    sndSys.snd_channels[v1].sndSouce = v3;
                    break;
                }
                else
                {
                    v1++;
                }
            }

            if ( v1 == sndSys.audio_channels )
                break;
        }
    }
}

void sb_0x424c74__sub2__sub1(userdata_sample_info *smpl)
{
    samples_collection1 *v2 = smpl->parent_sample_collection;

    float v3 = sndSys.stru_547018.sx - v2->field_0.sx;
    float v4 = sndSys.stru_547018.sy - v2->field_0.sy;
    float v6 = sndSys.stru_547018.sz - v2->field_0.sz;

    float v27 = sqrt(POW2(v3) + POW2(v4) + POW2(v6));

    float v8 = v2->field_C - sndSys.stru_547024.sx;
    float v9 = v2->field_10 - sndSys.stru_547024.sy;
    float v11 = v2->field_14 - sndSys.stru_547024.sz;

    float v20 = sqrt(POW2(v8) + POW2(v9) + POW2(v11));

    float v21 = v27  *  v20;
    float v19;

    if ( v21 <= 0.0 )
        v19 = 0.0;
    else
        v19 = (v3 * v8 + v4 * v9 + v6 * v11) / v21;

    int v14;

    if ( smpl->flags & 0x200 )
        v14 = smpl->smplExt->sndExts[smpl->fragmentID].smplRate + smpl->smplExt->sndExts[smpl->fragmentID].sample->SampleRate + smpl->pitch;
    else
        v14 = smpl->pitch + smpl->psampl->SampleRate;

    if ( v14 < 2000 )
        v14 = 2000;
    else if ( v14 > 44100 )
        v14 = 44100;

    float v31 = sndSys.stru_547030.m00 * v3 + sndSys.stru_547030.m01 * v4 + sndSys.stru_547030.m02 * v6;

    smpl->resultRate = v14 + v14 * (int)(v19 * v20) / 400;

    float v32;

    if ( v27 <= 0.0 )
    {
        v32 = 0.0;
    }
    else if ( sndSys.audio_rev_stereo )
    {
        v32 = v31 / v27;
    }
    else
    {
        v32 = v31 / -v27;
    }

    smpl->resultPan = (v32 * 64.0 + 64.0);

    if ( smpl->resultVol < 0 )
        smpl->resultVol = 0;
    else if ( smpl->resultVol > 127 )
        smpl->resultVol = 127;

    if ( smpl->resultPan < 0 )
        smpl->resultPan = 0;
    else if ( smpl->resultPan > 127 )
        smpl->resultPan = 127;

    if ( v14 / 2 <= smpl->resultRate )
    {
        if ( smpl->resultRate > v14 * 2)
            smpl->resultRate = v14 * 2;
    }
    else
    {
        smpl->resultRate = v14 / 2;
    }
}

void sound_eos_clbk(void *_smpl)
{
    walsmpl *smpl = static_cast<walsmpl *>(_smpl);

    int v2 = 0;
    userdata_sample_info *v3 = NULL;

    for (int i = 0; i < sndSys.audio_channels; i++)
    {
        if ( smpl == sndSys.snd_channels[i].hSample )
        {
            v2 = i;
            v3 = sndSys.snd_channels[i].sndSouce;
            break;
        }
    }

    if ( v3 )
    {
        if ( v3->flags & 2 )
        {
            if ( v3->flags & 0x200 )
            {
                v3->fragmentID++;

                if ( v3->fragmentID >= v3->smplExt->cnt )
                {
                    if ( v3->flags & 1 )
                        v3->fragmentID = 0;
                    else
                        v3->flags &= ~2;
                }

                if ( v3->flags & 2 )
                {
                    for (int i = 0; i < sndSys.audio_channels; i++)
                    {
                        if ( !sndSys.snd_channels[i].sndSouce )
                        {
                            smpl = sndSys.snd_channels[i].hSample;
                            sndSys.snd_channels[v2].sndSouce = NULL;
                            sndSys.snd_channels[i].sndSouce = v3;
                        }
                    }

                    sndExt *v10 = &v3->smplExt->sndExts[ v3->fragmentID ];

                    wrapper_playSound(
                        sndSys.digDriver,
                        smpl,
                        sound_eos_clbk,
                        (char *)v10->sample->sample_buffer + v10->rlOffset,
                        v10->rlSmplCnt,
                        v3->resultRate,
                        v3->resultVol,
                        v3->resultPan,
                        v10->loop,
                        0);
                }
            }
            else
            {
                v3->flags &= ~2;
            }
        }
    }
}


void sb_0x424c74__sub2__sub0(int id, userdata_sample_info *smpl)
{
    walsmpl *v3 = sndSys.snd_channels[id].hSample;

    smpl->flags |= 4;

    if ( smpl->flags & 0x200 )
    {
        int v4 = sndSys.currentTime - smpl->startTime;
        int v8;

        if ( v4 <= 0 )
        {
            smpl->fragmentID = 0;
            v8 = 0;
        }
        else
        {
            int a3;
            int v7 = sub_423B3C(smpl, v4, &a3);

            if ( v7 == smpl->smplExt->cnt )
                v7 = smpl->smplExt->cnt - 1;

            v8 = ((smpl->resultRate * a3) / 1024) % smpl->smplExt->sndExts[v7].rlSmplCnt;
            smpl->fragmentID = v7;
        }

        sndExt *v9 = &smpl->smplExt->sndExts[smpl->fragmentID];

        wrapper_playSound(
            sndSys.digDriver,
            v3,
            sound_eos_clbk,
            (char *)v9->sample->sample_buffer + v9->rlOffset,
            v9->rlSmplCnt,
            smpl->resultRate,
            smpl->resultVol,
            smpl->resultPan,
            v9->loop,
            v8);
    }
    else
    {
        wrapper_playSound(
            sndSys.digDriver,
            v3,
            sound_eos_clbk,
            smpl->psampl->sample_buffer,
            smpl->psampl->bufsz,
            smpl->resultRate,
            smpl->resultVol,
            smpl->resultPan,
            (smpl->flags & 1) == 0,
            (int)((smpl->psampl->SampleRate + smpl->pitch) * (sndSys.currentTime - smpl->startTime) >> 10) % smpl->psampl->bufsz);
    }
}

void sb_0x424c74__sub2()
{
    for (int i = 0; i < sndSys.audio_channels; i++)
    {
        userdata_sample_info *v2 = sndSys.snd_channels[i].sndSouce;
        walsmpl *v3 = sndSys.snd_channels[i].hSample;

        if ( v2 )
        {
            if ( v2->psampl || v2->flags & 0x200 )
            {
                sb_0x424c74__sub2__sub1(sndSys.snd_channels[i].sndSouce);

                if ( v2->flags & 4 )
                    wrapper_setSampleVRP(sndSys.digDriver, v3, v2->resultRate, v2->resultVol, v2->resultPan);
                else
                    sb_0x424c74__sub2__sub0(i, v2);
            }
            else
            {
                ypa_log_out("AUDIO ENGINE ERROR #1!\n");
            }
        }
    }
}

void sb_0x424c74__sub3()
{
    int i = 0;
    int v16 = 256;

    int v10[9];
    int v12[9];

    for (i = 0; i < sndSys.audio_num_palfx; i++)
    {
        userdata_sample_info *v3 = sndSys.palFXs[i];

        if ( !v3 )
            break;

        v16 -= v3->palMag;

        v10[i] = v3->paletteFX->slot;

        v12[i] = v3->palMag * 256.0;

        v16 -= v12[i];
    }

    if ( i )
    {
        sndSys.dword_546F0C = 0;
    }
    else
    {
        if ( sndSys.dword_546F0C )
            return;

        sndSys.dword_546F0C = 1;
    }

    if ( v16 > 0 )
    {
        v10[i] = 0;
        v12[i] = v16;

        i++;
    }

    NC_STACK_display *pwin3d = GFXEngine::GFXe.getC3D();

    if ( pwin3d )
    {
        rstr_262_arg arg262;
        arg262.slot = v10;
        arg262.cnt = i;
        arg262.weight = v12;

        pwin3d->display_func262(&arg262);
    }
}

void sb_0x424c74__sub4()
{
    xyz tmp;
    tmp.sx = 0.0;
    tmp.sy = 0.0;
    tmp.sz = 0.0;

    int i = 0;

    for (i = 0; i < sndSys.dword_546E14 && sndSys.ShakeFXs[i]; i++)
    {
        userdata_sample_info *v2 = sndSys.ShakeFXs[i];

        tmp.sx += audio_rnd() * v2->shkMag * v2->shakeFX->pos.sx;

        tmp.sy += audio_rnd() * v2->shkMag * v2->shakeFX->pos.sy;

        tmp.sz += audio_rnd() * v2->shkMag * v2->shakeFX->pos.sz;
    }

    if ( i > 0 )
    {
        sndSys.shakeMatrix.m00 = cos(tmp.sz) * cos(tmp.sy) - sin(tmp.sz) * sin(tmp.sx) * sin(tmp.sy);
        sndSys.shakeMatrix.m01 = -sin(tmp.sz) * cos(tmp.sx);
        sndSys.shakeMatrix.m02 = sin(tmp.sz) * sin(tmp.sx) * cos(tmp.sy) + cos(tmp.sz) * sin(tmp.sy);

        sndSys.shakeMatrix.m10 = cos(tmp.sz) * sin(tmp.sx) * sin(tmp.sy) + sin(tmp.sz) * cos(tmp.sy);
        sndSys.shakeMatrix.m11 = cos(tmp.sz) * cos(tmp.sx);
        sndSys.shakeMatrix.m12 = sin(tmp.sz) * sin(tmp.sy) - cos(tmp.sz) * sin(tmp.sx) * cos(tmp.sy);

        sndSys.shakeMatrix.m20 = -cos(tmp.sx) * sin(tmp.sy);
        sndSys.shakeMatrix.m21 = sin(tmp.sx);
        sndSys.shakeMatrix.m22 = cos(tmp.sx) * cos(tmp.sy);
    }
    else
    {
        memset(&sndSys.shakeMatrix, 0, sizeof(sndSys.shakeMatrix));

        sndSys.shakeMatrix.m00 = 1.0;
        sndSys.shakeMatrix.m11 = 1.0;
        sndSys.shakeMatrix.m22 = 1.0;
    }

}

void UpdateMusic()
{
    if (sndSys.musPlayer && sndSys.musOn)
    {
        if (sndSys.musWait)
        {
            if (sndSys.musWaitSTime + sndSys.musWaitDelay <= sndSys.currentTime)
            {
                sndSys.musWait = false;
                sndSys.musPlayer->stop();
                sndSys.musPlayer->play();
            }
        }
        else
        {
            if( sndSys.musPlayer->isStopped() )
            {
                if( sndSys.musTrack ) // if audio track was setted - replay it, but do some delay
                {
                    sndSys.musWait = true;
                    sndSys.musWaitSTime = sndSys.currentTime;
                    sndSys.musWaitDelay = sndSys.musMinDelay + (sndSys.musMaxDelay - sndSys.musMinDelay) * (sndSys.currentTime % 30) / 30;
                }
            }
        }
    }
}


mat3x3 *sb_0x424c74()
{
    if ( sndSys.digDriver->inited() )
    {
        sb_0x424c74__sub0();
        sb_0x424c74__sub1();
        sb_0x424c74__sub2();
    }

    if ( sndSys.audio_num_palfx )
        sb_0x424c74__sub3();

    sb_0x424c74__sub4();

    UpdateMusic();

    return &sndSys.shakeMatrix;
}

void sub_423EFC(int a1, xyz *a2, xyz *a3, mat3x3 *a4)
{
    sndSys.currentTime += a1;

    sndSys.stru_547018 = *a2;

    sndSys.stru_547024 = *a3;

    sndSys.stru_547030 = *a4;

    memset(sndSys.soundSources, 0, sizeof(sndSys.soundSources));
    memset(sndSys.palFXs, 0, sizeof(sndSys.palFXs));
    memset(sndSys.ShakeFXs, 0, sizeof(sndSys.ShakeFXs));
}

void sub_424CC8()
{
    if ( sndSys.digDriver )
    {
        for (int i = 0; i < sndSys.audio_channels; i++)
        {

            if (sndSys.snd_channels[i].sndSouce )
            {
                sndSys.snd_channels[i].sndSouce->flags &= ~(4 | 2);

                sndSys.snd_channels[i].hSample->stop();

                sndSys.snd_channels[i].sndSouce = NULL;
            }
        }
    }

    for (int i = 0; i < sndSys.audio_channels; i++)
    {
        if (sndSys.soundSources[i] )
        {
            sndSys.soundSources[i]->flags &= ~(4 | 2);
            sndSys.soundSources[i] = NULL;
        }
    }

    for (int i = 0; i < sndSys.audio_num_palfx; i++)
    {
        if ( sndSys.palFXs[i] )
        {
            sndSys.palFXs[i]->flags &= ~(0x20 | 0x10);
            sndSys.palFXs[i] = NULL;
        }
    }

    for (int i = 0; i < sndSys.dword_546E14; i++)
    {
        if ( sndSys.ShakeFXs[i] )
        {
            sndSys.ShakeFXs[i]->flags &= ~(0x100 | 0x80);
            sndSys.ShakeFXs[i] = NULL;
        }
    }
}
