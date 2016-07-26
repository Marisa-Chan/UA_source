#include "includes.h"
#include "engine_miles.h"

#include "wrapal.h"

#include <math.h>

#define AUDIO_CHANNELS   16


SFXEngine SFXe;

int dword_546DD8;

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
    int dword_546F10;
    int dword_546F14;
    float flt_546F18[64];
    xyz stru_547018;
    xyz stru_547024;
    mat3x3 stru_547030;
    mat3x3 stru_547054;
};

soundSys sndSys;

key_value_stru audio_keys[4] = {
    {"audio.channels", KEY_TYPE_DIGIT, 4},
    {"audio.volume", KEY_TYPE_DIGIT, 127},
    {"audio.num_palfx", KEY_TYPE_DIGIT, 4},
    {"audio.rev_stereo", KEY_TYPE_BOOL, 0}
};

void wrapper_setVolume(waldev *driver, int volume)
{
    if (driver)
        driver->master_volume(volume);
}

void wrapper_openRedBook()
{
    printf("MAKE INIT FOR MUSIC STREAM!\n");
}

void wrapper_endSample(void *, walsmpl *hSample)
{
    hSample->stop();
}

void wrapper_setSampleVRP(void *, walsmpl *hSample, int rate, int volume, int pan)
{
    hSample->playback_rate(rate);
    hSample->volume(volume);
    hSample->pan(pan);
}

void wrapper_playSound(waldev *driver, walsmpl *hSample, void (*funceos)(walsmpl *), void *start, int len, int rate, int vol, int pan, int loop_cnt, int pos)
{
    hSample->init();
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

            if ( !sndSys.snd_channels[i].hSample )
            {
                sndSys.audio_channels = i;
                break;
            }
        }

        wrapper_setVolume(sndSys.digDriver, sndSys.audio_volume);
        wrapper_openRedBook();
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
    wrapper_setVolume(sndSys.digDriver, vol);
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

int sub_4444D4(CDAUDIO_t *arg)
{
    printf("STUB %s\n", "sub_4444D4");
    return 0;
}

void sub_423F74(samples_collection1 *smpls, int a2)
{
    userdata_sample_info *result = &smpls->samples_data[a2];

    result->field_20 = sndSys.dword_546F10;

    if ( result->field_13 & 2 || result->psampl )
    {
        result->field_1E = 0;
        result->field_12 |= 2;
        result->field_12 &= 0xFB;
    }
    if ( result->field_12 & 8 )
    {
        result->field_12 |= 0x10;
        result->field_12 &= 0xDF;
        result->field_24 = result->field_4->mag0;
    }
    if ( result->field_12 & 0x40 )
    {
        result->field_12 |= 0x80;
        result->field_13 &= 0xFE;
        result->field_28 = result->field_8->mag0;
    }
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

// Shutoff samples set
void sub_423DD8(samples_collection1 *smpls)
{
    for (int i = 0; i < sndSys.audio_channels; i++)
    {
        if ( sndSys.digDriver )
        {
            if (sndSys.snd_channels[i].sndSouce && sndSys.snd_channels[i].sndSouce->parent_sample_collection == smpls )
            {
                sndSys.snd_channels[i].sndSouce->field_12 &= 0xF9;

                wrapper_endSample(sndSys.digDriver, sndSys.snd_channels[i].hSample);

                sndSys.snd_channels[i].sndSouce = NULL;
            }

            if (sndSys.soundSources[i] && sndSys.soundSources[i]->parent_sample_collection == smpls )
            {
                sndSys.soundSources[i]->field_12 &= 0xF9;
                sndSys.soundSources[i] = NULL;
            }
        }
    }

    for (int i = 0; i < sndSys.audio_num_palfx; i++)
    {
        if ( sndSys.palFXs[i] && sndSys.palFXs[i]->parent_sample_collection == smpls )
        {
            sndSys.palFXs[i]->field_12 &= 0xCF;
            sndSys.palFXs[i] = NULL;
        }
    }

    for (int i = 0; i < sndSys.dword_546E14; i++)
    {
        if ( sndSys.ShakeFXs[i] && sndSys.ShakeFXs[i]->parent_sample_collection == smpls )
        {
            sndSys.ShakeFXs[i]->field_12 &= 0x7F;
            sndSys.ShakeFXs[i]->field_13 &= 0xFE;
            sndSys.ShakeFXs[i] = NULL;
        }
    }

    for (int i = 0; i < 16; i++)
        smpls->samples_data[i].field_12 &= 0x6D;

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
                if ( sndSys.soundSources[i]->field_1C < v3 )
                {
                    min_i = i;
                    v3 = sndSys.soundSources[i]->field_1C;
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

        if ( min_i != -1 && v3 < smpl->field_1C )
            sndSys.soundSources[min_i] = smpl;
    }
}

int sub_423B3C(userdata_sample_info *smpl, int a2, int *a3)
{
    int i = 0;

    for (i = 0; i < smpl->field_C->cnt; i++)
    {
        if ( !smpl->field_C->sndExts[0].field_4 )
            break;

        int v9 = smpl->field_C->sndExts[0].field_8 + smpl->field_C->sndExts[0].sample->SampleRate + smpl->pitch;

        if ( v9 <= 0 )
            v9 = 1;

        int v10 = (smpl->field_C->sndExts[0].field_4 * smpl->field_C->sndExts[0].field_18 / 1024) / v9;

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

        if ( v2->psampl || v2->field_13 & 2 )
        {
            int v5 = 0;

            if ( v2->field_12 & 2 )
            {
                int v6 = sndSys.dword_546F10 - v2->field_20;

                if ( v2->field_13 & 2 )
                {
                    if ( v2->field_12 & 4 )
                        v5 = v2->field_1E;
                    else
                        v5 = sub_423B3C(v2, sndSys.dword_546F10 - v2->field_20, 0);
                }

                int ok = 1;

                if ( !(v2->field_12 & 1) && !(v2->field_12 & 4) && v6 > 0 )
                {
                    if ( v2->field_13 & 2 )
                    {
                        if ( v5 >= v2->field_C->cnt )
                        {
                            v2->field_12 &= 0xFD;
                            ok = 0;
                        }
                    }
                    else if ( (v6 * (v2->pitch + v2->psampl->SampleRate) / 1024) > v2->psampl->bufsz )
                    {
                        v2->field_12 &= 0xFD;
                        ok = 0;
                    }
                }

                if (ok)
                {
                    v2->vol = v2->volume;

                    if ( v2->field_13 & 2 )
                        v2->vol += v2->field_C->sndExts[v5].field_6;

                    if ( v10 >= 1.0 )
                        v2->vol /= v10;

                    if ( v2->vol > 4 )
                    {
                        v2->field_1C = v2->vol;
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
            if ( sndSys.palFXs[i]->field_24 )
            {
                a1 = sndSys.palFXs[i]->field_24;
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

    if ( min_i != -1 && a1 < smpl->field_24 )
        sndSys.palFXs[min_i] = smpl;
}

void sb_0x4242e0__sub1(samples_collection1 *smpls, float a2)
{
    float v6 = a2 / 300.0;

    for (int i = 0; i < 16; i++)
    {
        userdata_sample_info *v2 = &smpls->samples_data[i];

        if ( v2->field_12 & 8 )
        {
            if ( v2->field_12 & 0x10 )
            {
                if ( !(v2->field_12 & 1) && v2->field_20 + v2->field_4->time < sndSys.dword_546F10 )
                    v2->field_12 &= 0xEF;
            }

            if ( v2->field_12 & 0x10 )
            {
                if ( v2->field_12 & 1 )
                    v2->field_24 = v2->field_4->mag0;
                else
                    v2->field_24 = (v2->field_4->mag1 - v2->field_4->mag0) * ((float)(sndSys.dword_546F10 - v2->field_20) / (float)v2->field_4->time) + v2->field_4->mag0;

                if ( v6 >= 1.0 )
                    v2->field_24 /= v6;

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
            if ( var_1C > v5->field_28 )
            {
                var_1C = v5->field_28;
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

    if ( min_i != -1 && var_1C < smpl->field_28 )
        sndSys.ShakeFXs[min_i] = smpl;
}

void sb_0x4242e0__sub2(samples_collection1 *smpls, float a2)
{
    for (int i = 0; i < 16; i++)
    {
        userdata_sample_info *v2 = &smpls->samples_data[i];

        if ( v2->field_12 & 0x40 )
        {
            if ( v2->field_12 & 0x80 )
            {
                if ( !(v2->field_12 & 1) && v2->field_20 + v2->field_8->time < sndSys.dword_546F10 )
                    v2->field_12 &= 0x7F;
            }

            if ( v2->field_12 & 0x80 )
            {
                if ( v2->field_12 & 1 )
                    v2->field_28 = v2->field_8->mag0;
                else
                    v2->field_28 = (v2->field_8->mag1 - v2->field_8->mag0) * ((float)(sndSys.dword_546F10 - v2->field_20) / (float)v2->field_8->time) + v2->field_8->mag0;

                float v6 = a2 * v2->field_8->mute;

                if ( v6 >= 1.0 )
                    v2->field_28 /= v6;

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
                v2->field_12 &= 0xFB;

                sndSys.snd_channels[i].sndSouce = NULL;

                wrapper_endSample(sndSys.digDriver, sndSys.snd_channels[i].hSample);
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

        if ( v3 && !(v3->field_12 & 4) )
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

    if ( smpl->field_13 & 2 )
        v14 = smpl->field_C->sndExts[smpl->field_1E].field_8 + smpl->field_C->sndExts[smpl->field_1E].sample->SampleRate + smpl->pitch;
    else
        v14 = smpl->pitch + smpl->psampl->SampleRate;

    if ( v14 < 2000 )
        v14 = 2000;
    else if ( v14 > 44100 )
        v14 = 44100;

    float v31 = sndSys.stru_547030.m00 * v3 + sndSys.stru_547030.m01 * v4 + sndSys.stru_547030.m02 * v6;

    smpl->rate = v14 + v14 * (int)(v19 * v20) / 400;

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

    smpl->pan = (v32 * 64.0 + 64.0);

    if ( smpl->vol < 0 )
        smpl->vol = 0;
    else if ( smpl->vol > 127 )
        smpl->vol = 127;

    if ( smpl->pan < 0 )
        smpl->pan = 0;
    else if ( smpl->pan > 127 )
        smpl->pan = 127;

    if ( v14 / 2 <= smpl->rate )
    {
        if ( smpl->rate > v14 * 2)
            smpl->rate = v14 * 2;
    }
    else
    {
        smpl->rate = v14 / 2;
    }
}

void sound_eos_clbk(walsmpl *smpl)
{
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
        if ( v3->field_12 & 2 )
        {
            if ( v3->field_13 & 2 )
            {
                v3->field_1E++;

                if ( v3->field_1E >= v3->field_C->cnt )
                {
                    if ( v3->field_12 & 1 )
                        v3->field_1E = 0;
                    else
                        v3->field_12 &= 0xFD;
                }

                if ( v3->field_12 & 2 )
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

                    sndExt *v10 = &v3->field_C->sndExts[ v3->field_1E ];

                    wrapper_playSound(
                        sndSys.digDriver,
                        smpl,
                        sound_eos_clbk,
                        (char *)v10->sample->sample_buffer + v10->field_14,
                        v10->field_18,
                        v3->rate,
                        v3->vol,
                        v3->pan,
                        v10->field_4,
                        0);
                }
            }
            else
            {
                v3->field_12 &= 0xFD;
            }
        }
    }
}


void sb_0x424c74__sub2__sub0(int id, userdata_sample_info *smpl)
{
    walsmpl *v3 = sndSys.snd_channels[id].hSample;

    smpl->field_12 |= 4;

    if ( smpl->field_13 & 2 )
    {
        int v4 = sndSys.dword_546F10 - smpl->field_20;
        int v8;

        if ( v4 <= 0 )
        {
            smpl->field_1E = 0;
            v8 = 0;
        }
        else
        {
            int a3;
            int v7 = sub_423B3C(smpl, v4, &a3);

            if ( v7 == smpl->field_C->cnt )
                v7 = smpl->field_C->cnt - 1;

            v8 = ((smpl->rate * a3) / 1024) % smpl->field_C->sndExts[v7].field_18;
            smpl->field_1E = v7;
        }

        sndExt *v9 = &smpl->field_C->sndExts[smpl->field_1E];

        wrapper_playSound(
            sndSys.digDriver,
            v3,
            sound_eos_clbk,
            (char *)v9->sample->sample_buffer + v9->field_14,
            v9->field_18,
            smpl->rate,
            smpl->vol,
            smpl->pan,
            v9->field_4,
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
            smpl->rate,
            smpl->vol,
            smpl->pan,
            (smpl->field_12 & 1) == 0,
            (int)((smpl->psampl->SampleRate + smpl->pitch) * (sndSys.dword_546F10 - smpl->field_20) >> 10) % smpl->psampl->bufsz);
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
            if ( v2->psampl || v2->field_13 & 2 )
            {
                sb_0x424c74__sub2__sub1(sndSys.snd_channels[i].sndSouce);

                if ( v2->field_12 & 4 )
                    wrapper_setSampleVRP(sndSys.digDriver, v3, v2->rate, v2->vol, v2->pan);
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
    int v0 = 0;
    int v16 = 256;

    int v10[9];
    int v12[9];

    for (int i = 0; i < sndSys.audio_num_palfx; i++)
    {
        userdata_sample_info *v3 = sndSys.palFXs[i];

        if ( !v3 )
            break;

        v16 -= v3->field_24;

        v10[i] = v3->field_4->slot;

        v12[i] = v3->field_24 * 256.0;

        v16 -= v12[i];

        v0 = i;
    }

    if ( v0 )
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
        v10[v0] = 0;
        v12[v0] = v16;

        v0++;
    }

    NC_STACK_display *pwin3d = GFXe.getC3D();

    if ( pwin3d )
    {
        rstr_262_arg arg262;
        arg262.pdword4 = v10;
        arg262.dword0 = v0;
        arg262.pdword8 = v12;

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

        tmp.sx += audio_rnd() * v2->field_28 * v2->field_8->pos.sx;

        tmp.sy += audio_rnd() * v2->field_28 * v2->field_8->pos.sy;

        tmp.sz += audio_rnd() * v2->field_28 * v2->field_8->pos.sz;
    }

    if ( i > 0 )
    {
        sndSys.stru_547054.m00 = cos(tmp.sz) * cos(tmp.sy) - sin(tmp.sz) * sin(tmp.sx) * sin(tmp.sy);
        sndSys.stru_547054.m01 = -sin(tmp.sz) * cos(tmp.sx);
        sndSys.stru_547054.m02 = sin(tmp.sz) * sin(tmp.sx) * cos(tmp.sy) + cos(tmp.sz) * sin(tmp.sy);

        sndSys.stru_547054.m10 = cos(tmp.sz) * sin(tmp.sx) * sin(tmp.sy) + sin(tmp.sz) * cos(tmp.sy);
        sndSys.stru_547054.m11 = cos(tmp.sz) * cos(tmp.sx);
        sndSys.stru_547054.m12 = sin(tmp.sz) * sin(tmp.sy) - cos(tmp.sz) * sin(tmp.sx) * cos(tmp.sy);

        sndSys.stru_547054.m20 = -cos(tmp.sx) * sin(tmp.sy);
        sndSys.stru_547054.m21 = sin(tmp.sx);
        sndSys.stru_547054.m22 = cos(tmp.sx) * cos(tmp.sy);
    }
    else
    {
        memset(&sndSys.stru_547054, 0, sizeof(sndSys.stru_547054));

        sndSys.stru_547054.m00 = 1.0;
        sndSys.stru_547054.m11 = 1.0;
        sndSys.stru_547054.m22 = 1.0;
    }

}

void sb_0x424c74__sub5(int *)
{
    //printf(" MAKE redbook update\n");
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

    int v2[2];
    v2[0] = sndSys.dword_546F10;
    v2[1] = 1;

    sb_0x424c74__sub5(v2);

    return &sndSys.stru_547054;
}

void sub_423EFC(int a1, xyz *a2, xyz *a3, mat3x3 *a4)
{
    sndSys.dword_546F10 += a1;

    sndSys.stru_547018 = *a2;

    sndSys.stru_547024 = *a3;

    sndSys.stru_547030 = *a4;

    memset(sndSys.soundSources, 0, sizeof(sndSys.soundSources));
    memset(sndSys.palFXs, 0, sizeof(sndSys.palFXs));
    memset(sndSys.ShakeFXs, 0, sizeof(sndSys.ShakeFXs));

    dword_546DD8 = sndSys.dword_546F10;
}

void sub_424CC8()
{
    if ( sndSys.digDriver )
    {
        for (int i = 0; i < sndSys.audio_channels; i++)
        {

            if (sndSys.snd_channels[i].sndSouce )
            {
                sndSys.snd_channels[i].sndSouce->field_12 &= 0xF9;

                wrapper_endSample(sndSys.digDriver, sndSys.snd_channels[i].hSample);

                sndSys.snd_channels[i].sndSouce = NULL;
            }
        }
    }

    for (int i = 0; i < sndSys.audio_channels; i++)
    {
        if (sndSys.soundSources[i] )
        {
            sndSys.soundSources[i]->field_12 &= 0xF9;
            sndSys.soundSources[i] = NULL;
        }
    }

    for (int i = 0; i < sndSys.audio_num_palfx; i++)
    {
        if ( sndSys.palFXs[i] )
        {
            sndSys.palFXs[i]->field_12 &= 0xCF;
            sndSys.palFXs[i] = NULL;
        }
    }

    for (int i = 0; i < sndSys.dword_546E14; i++)
    {
        if ( sndSys.ShakeFXs[i] )
        {
            sndSys.ShakeFXs[i]->field_12 &= 0x7F;
            sndSys.ShakeFXs[i]->field_13 &= 0xFE;
            sndSys.ShakeFXs[i] = NULL;
        }
    }
}
