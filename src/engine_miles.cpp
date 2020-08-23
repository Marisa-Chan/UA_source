#include "includes.h"
#include "engine_miles.h"

#include <math.h>

SFXEngine SFXEngine::SFXe;

Common::Ini::KeyList audio_keys
{
    Common::Ini::Key("audio.channels", Common::Ini::KT_DIGIT, (int32_t)4),
    Common::Ini::Key("audio.volume",   Common::Ini::KT_DIGIT, (int32_t)127),
    Common::Ini::Key("audio.num_palfx", Common::Ini::KT_DIGIT, (int32_t)4),
    Common::Ini::Key("audio.rev_stereo", Common::Ini::KT_BOOL)
};

void wrapper_setSampleVRP(void *, walsmpl *hSample, int rate, int volume, int pan)
{
    hSample->playback_rate(rate);
    hSample->volume(volume);
    hSample->pan(pan);
}

void wrapper_playSound(waldev *driver, walsmpl *hSample, void (*funceos)(void *), void *start, int len, int rate, int vol, int mastersnd, int pan, int loop_cnt, int pos)
{
    hSample->reset();
    hSample->EOS_callback(funceos);
    hSample->address(start, len, rate, AL_FORMAT_MONO8);
    hSample->setMasterVolume(mastersnd);
    hSample->volume(vol);
    hSample->pan(pan);
    hSample->loop_count(loop_cnt);
    hSample->play();

    if (pos > 0)
        hSample->position(pos);
}


int SFXEngine::init()
{
    digDriver = NULL;
    audio_volume = 0;
    audio_channels = 0;
    audio_num_palfx = 0;
    dword_546E14 = 0;

    memset(soundSources, 0, sizeof(soundSources));
    memset(palFXs, 0, sizeof(palFXs));
    memset(ShakeFXs, 0, sizeof(ShakeFXs));

    audio_rev_stereo = false;
    dword_546F0C = 0;
    currentTime = 0;
    dword_546F14 = 0;

    for (int i = 0; i < 64; i++)
        flt_546F18[i] = 0.0;

    musPlayer = NULL;
    musWait = false;
    musOn = false;
    musWaitSTime = 0;
    musWaitDelay = 0;
    musMinDelay = 0;
    musMaxDelay = 0;
    musTrack = 0;

    Common::Ini::ParseIniFile(NC_STACK_nucleus::DefaultIniFile, &audio_keys);

    audio_channels  = audio_keys[0].Get<int>();
    audio_volume    = audio_keys[1].Get<int>();
    audio_num_palfx = audio_keys[2].Get<int>();
    audio_rev_stereo = audio_keys[3].Get<bool>();

    if ( audio_volume > 127 )
        audio_volume = 127;

    if ( audio_channels < 1 )
        audio_channels = 1;

    if ( audio_channels > AUDIO_CHANNELS)
        audio_channels = AUDIO_CHANNELS;

    if ( audio_num_palfx > 8 )
        audio_num_palfx = 8;

    dword_546E14 = 4;
    dword_546F14 = 0;

    for (int i = 0; i < 64; i++)
        flt_546F18[i] = (float)(16383 - (rand() & 0x7FFF) ) / 16383.0;

    digDriver = new waldev(); //miles_init(audio_channels);

    if ( digDriver->inited() )
    {
        for (int i = 0; i < audio_channels; i++)
        {
            snd_channels[i].hSample = digDriver->newSample(); //miles_allocSample(digDriver);
            snd_channels[i].hSample->setMasterVolume(audio_volume);

            if ( !snd_channels[i].hSample )
            {
                audio_channels = i;
                break;
            }
        }

        musPlayer = digDriver->createMusicPlayer();
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

    if (digDriver)
    {
        delete digDriver;
        digDriver = NULL;
    }
}

void SFXEngine::setMasterVolume(int vol)
{
    audio_volume = vol;

    for (int i = 0; i < audio_channels; i++)
        snd_channels[i].hSample->setMasterVolume(audio_volume);

//    wrapper_setVolume(digDriver, vol);
}

void SFXEngine::setReverseStereo(bool rev)
{
    audio_rev_stereo = rev;
}

int SFXEngine::getMasterVolume()
{
    return audio_volume;
}

void SFXEngine::sub_423DB0(samples_collection1 *smpls)
{
    memset(smpls, 0, sizeof(samples_collection1));

    for (int i = 0; i < 16; i++)
    {
        smpls->samples_data[i].parent_sample_collection = smpls;
    }
}

void SFXEngine::startSound(samples_collection1 *smpls, int a2)
{
    userdata_sample_info *result = &smpls->samples_data[a2];

    result->startTime = currentTime;

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

void SFXEngine::sub_424000(samples_collection1 *smpls, int a2)
{
    smpls->samples_data[a2].flags &= ~(0x80 | 0x10 | 2);
}




void SFXEngine::SetMusicIgnoreCommandsFlag(bool flag)
{
    if (!flag)
        musTrack = 0;

    musOn = flag;
}

void SFXEngine::SetMusicVolume(int vol)
{
    if (digDriver && musPlayer && musOn)
    {
        musPlayer->volume(vol);
    }
}

void SFXEngine::StopMusicTrack()
{
    if (digDriver && musPlayer && musOn)
    {
        musPlayer->stop();
        musTrack = 0;
    }
}

void SFXEngine::PlayMusicTrack()
{
    if (digDriver && musPlayer && musOn  && musTrack > 0)
    {
        musPlayer->play();
    }
}

void SFXEngine::SetMusicTrack(int trackID, int minDelay, int maxDelay)
{
    if (digDriver && musPlayer && musOn && trackID > 0)
    {
        char buf[64];
        sprintf(buf, "%d", trackID);

        std::string str = "music/";
        str += buf;
        str += ".ogg";

        if ( musPlayer->open(str.c_str()) )
        {
            musTrack = trackID;
            musMinDelay = minDelay;
            musMaxDelay = maxDelay;
        }
        else
            musTrack = 0;
    }
}

// Shutoff samples set
void SFXEngine::sub_423DD8(samples_collection1 *smpls)
{
    for (int i = 0; i < audio_channels; i++)
    {
        if ( digDriver )
        {
            if (snd_channels[i].sndSouce && snd_channels[i].sndSouce->parent_sample_collection == smpls )
            {
                snd_channels[i].sndSouce->flags &= ~(4 | 2);

                snd_channels[i].hSample->stop();

                snd_channels[i].sndSouce = NULL;
            }

            if (soundSources[i] && soundSources[i]->parent_sample_collection == smpls )
            {
                soundSources[i]->flags &= ~(4 | 2);
                soundSources[i] = NULL;
            }
        }
    }

    for (int i = 0; i < audio_num_palfx; i++)
    {
        if ( palFXs[i] && palFXs[i]->parent_sample_collection == smpls )
        {
            palFXs[i]->flags &= ~(0x20 | 0x10);
            palFXs[i] = NULL;
        }
    }

    for (int i = 0; i < dword_546E14; i++)
    {
        if ( ShakeFXs[i] && ShakeFXs[i]->parent_sample_collection == smpls )
        {
            ShakeFXs[i]->flags &= ~(0x100 | 0x80);
            ShakeFXs[i] = NULL;
        }
    }

    for (int i = 0; i < 16; i++)
        smpls->samples_data[i].flags &= ~(0x80 | 0x10 | 2);

}

float SFXEngine::audio_rnd()
{
    float tmp = flt_546F18[dword_546F14];

    dword_546F14++;

    if ( dword_546F14 >= 64 )
        dword_546F14 = 0;

    return tmp;
}

void SFXEngine::audio_InsertSoundSource(userdata_sample_info *smpl)
{
    int min_i = -1;
    int v3 = 1000000;

    if ( digDriver->inited() )
    {
        for (int i = 0; i < audio_channels; i++)
        {
            if ( soundSources[i] )
            {
                if ( soundSources[i]->priority < v3 )
                {
                    min_i = i;
                    v3 = soundSources[i]->priority;
                }
            }
            else
            {
                min_i = i;
                v3 = 0;
                i = audio_channels;
            }
        }

        if ( min_i == -1 )
            ypa_log_out("-> audio_InsertSoundSource(): <min_i> not initialized.\n");

        if ( min_i != -1 && v3 < smpl->priority )
            soundSources[min_i] = smpl;
    }
}

int SFXEngine::sub_423B3C(userdata_sample_info *smpl, int a2, int *a3)
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

void SFXEngine::sb_0x4242e0__sub0(samples_collection1 *smpls, float a2)
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
                int v6 = currentTime - v2->startTime;

                if ( v2->flags & 0x200 )
                {
                    if ( v2->flags & 4 )
                        v5 = v2->fragmentID;
                    else
                        v5 = sub_423B3C(v2, currentTime - v2->startTime, NULL);
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


void SFXEngine::audio_InsertPalFX(userdata_sample_info *smpl)
{
    int min_i = -1;
    int a1 = 1000000;

    for (int i = 0; i < audio_num_palfx; i++)
    {
        if ( palFXs[i] )
        {
            if ( palFXs[i]->palMag )
            {
                a1 = palFXs[i]->palMag;
                min_i = i;
            }
        }
        else
        {
            min_i = i;
            a1 = 0;
            i = audio_num_palfx;
        }
    }

    if ( min_i == -1 )
        ypa_log_out("-> audio_InsertPalFX(): <min_i> not initialized.\n");

    if ( min_i != -1 && a1 < smpl->palMag )
        palFXs[min_i] = smpl;
}

void SFXEngine::sb_0x4242e0__sub1(samples_collection1 *smpls, float a2)
{
    float v6 = a2 / 300.0;

    for (int i = 0; i < 16; i++)
    {
        userdata_sample_info *v2 = &smpls->samples_data[i];

        if ( v2->flags & 8 )
        {
            if ( v2->flags & 0x10 )
            {
                if ( !(v2->flags & 1) && (size_t)(v2->startTime + v2->paletteFX->time) < currentTime )
                    v2->flags &= ~0x10;
            }

            if ( v2->flags & 0x10 )
            {
                if ( v2->flags & 1 )
                    v2->palMag = v2->paletteFX->mag0;
                else
                    v2->palMag = (v2->paletteFX->mag1 - v2->paletteFX->mag0) * ((float)(currentTime - v2->startTime) / (float)v2->paletteFX->time) + v2->paletteFX->mag0;

                if ( v6 >= 1.0 )
                    v2->palMag /= v6;

                audio_InsertPalFX(v2);
            }
        }
    }
}

void SFXEngine::InsertShakeFX(userdata_sample_info *smpl)
{
    int min_i = -1;
    int var_1C = 1000;

    for (int i = 0; i < dword_546E14; i++)
    {
        userdata_sample_info *v5 = ShakeFXs[i];

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
            i = dword_546E14;
        }
    }

    if ( min_i == -1 )
        ypa_log_out("-> audio_InsertShakeFX(): <min_i> not initialized.\n");

    if ( min_i != -1 && var_1C < smpl->shkMag )
        ShakeFXs[min_i] = smpl;
}

void SFXEngine::sb_0x4242e0__sub2(samples_collection1 *smpls, float a2)
{
    for (int i = 0; i < 16; i++)
    {
        userdata_sample_info *v2 = &smpls->samples_data[i];

        if ( v2->flags & 0x40 )
        {
            if ( v2->flags & 0x80 )
            {
                if ( !(v2->flags & 1) && (size_t)(v2->startTime + v2->shakeFX->time) < currentTime )
                    v2->flags &= ~0x80;
            }

            if ( v2->flags & 0x80 )
            {
                if ( v2->flags & 1 )
                    v2->shkMag = v2->shakeFX->mag0;
                else
                    v2->shkMag = (v2->shakeFX->mag1 - v2->shakeFX->mag0) * ((float)(currentTime - v2->startTime) / (float)v2->shakeFX->time) + v2->shakeFX->mag0;

                float v6 = a2 * v2->shakeFX->mute;

                if ( v6 >= 1.0 )
                    v2->shkMag /= v6;

                InsertShakeFX(v2);
            }
        }
    }
}

// Insert new sound
void SFXEngine::sb_0x4242e0(samples_collection1 *smpls)
{
    float a2 = (smpls->field_0 - stru_547018).length();

    if ( a2 < 6000.0 )
    {
        if ( digDriver->inited() )
            sb_0x4242e0__sub0(smpls, a2);

        if ( a2 < 2400.0 )
        {
            if ( audio_num_palfx )
                sb_0x4242e0__sub1(smpls, a2);

            sb_0x4242e0__sub2(smpls, a2);
        }
    }
}

void SFXEngine::sb_0x424c74__sub0()
{
    //Free ended channels
    for (int i = 0; i < audio_channels; i++)
    {
        userdata_sample_info *v2 = snd_channels[i].sndSouce;

        if ( v2 )
        {
            int v3 = 1;

            for (int j = 0; j < audio_channels; j++)
            {
                if ( v2 == soundSources[j] )
                {
                    v3 = 0;
                    break;
                }
            }

            if ( v3 )
            {
                v2->flags &= ~4;

                snd_channels[i].sndSouce = NULL;

                snd_channels[i].hSample->stop();
            }
        }
    }
}

void SFXEngine::sb_0x424c74__sub1()
{
    //Find free snd_channel
    int v1 = 0;

    for (int i = 0; i < audio_channels; i++)
    {
        userdata_sample_info *v3 = soundSources[i];

        if ( v3 && !(v3->flags & 4) )
        {
            while ( v1 < audio_channels )
            {
                if ( !snd_channels[v1].sndSouce )
                {
                    snd_channels[v1].sndSouce = v3;
                    break;
                }
                else
                {
                    v1++;
                }
            }

            if ( v1 == audio_channels )
                break;
        }
    }
}

void SFXEngine::sb_0x424c74__sub2__sub1(userdata_sample_info *smpl)
{
    samples_collection1 *v2 = smpl->parent_sample_collection;

    vec3d v3 = stru_547018 - v2->field_0;
    float v27 = v3.length();

    vec3d v8 = v2->field_C - stru_547024;
    float v20 = v8.length();

    float v21 = v27  *  v20;
    float v19 = 0.0;

    if ( v21 > 0.0 )
        v19 = v3.dot( v8 ) / v21;

    int v14;

    if ( smpl->flags & 0x200 )
        v14 = smpl->smplExt->sndExts[smpl->fragmentID].smplRate + smpl->smplExt->sndExts[smpl->fragmentID].sample->SampleRate + smpl->pitch;
    else
        v14 = smpl->pitch + smpl->psampl->SampleRate;

    if ( v14 < 2000 )
        v14 = 2000;
    else if ( v14 > 44100 )
        v14 = 44100;

    float v31 = stru_547030.AxisX().dot( v3 );

    smpl->resultRate = v14 + v14 * (int)(v19 * v20) / 400;

    float v32;

    if ( v27 <= 0.0 )
    {
        v32 = 0.0;
    }
    else if ( audio_rev_stereo )
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

void SFXEngine::sound_eos_clbk(void *_smpl)
{
    walsmpl *smpl = static_cast<walsmpl *>(_smpl);

    int v2 = 0;
    userdata_sample_info *v3 = NULL;

    for (int i = 0; i < SFXe.audio_channels; i++)
    {
        if ( smpl == SFXe.snd_channels[i].hSample )
        {
            v2 = i;
            v3 = SFXe.snd_channels[i].sndSouce;
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
                    for (int i = 0; i < SFXe.audio_channels; i++)
                    {
                        if ( !SFXe.snd_channels[i].sndSouce )
                        {
                            smpl = SFXe.snd_channels[i].hSample;
                            SFXe.snd_channels[v2].sndSouce = NULL;
                            SFXe.snd_channels[i].sndSouce = v3;
                        }
                    }

                    sndExt *v10 = &v3->smplExt->sndExts[ v3->fragmentID ];

                    wrapper_playSound(
                        SFXe.digDriver,
                        smpl,
                        sound_eos_clbk,
                        (char *)v10->sample->sample_buffer + v10->rlOffset,
                        v10->rlSmplCnt,
                        v3->resultRate,
                        v3->resultVol,
                        SFXe.audio_volume,
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


void SFXEngine::sb_0x424c74__sub2__sub0(int id, userdata_sample_info *smpl)
{
    walsmpl *v3 = snd_channels[id].hSample;

    smpl->flags |= 4;

    if ( smpl->flags & 0x200 )
    {
        int v4 = currentTime - smpl->startTime;
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
            digDriver,
            v3,
            sound_eos_clbk,
            (char *)v9->sample->sample_buffer + v9->rlOffset,
            v9->rlSmplCnt,
            smpl->resultRate,
            smpl->resultVol,
            audio_volume,
            smpl->resultPan,
            v9->loop,
            v8);
    }
    else
    {
        wrapper_playSound(
            digDriver,
            v3,
            sound_eos_clbk,
            smpl->psampl->sample_buffer,
            smpl->psampl->bufsz,
            smpl->resultRate,
            smpl->resultVol,
            audio_volume,
            smpl->resultPan,
            (smpl->flags & 1) == 0,
            (int)((smpl->psampl->SampleRate + smpl->pitch) * (currentTime - smpl->startTime) >> 10) % smpl->psampl->bufsz);
    }
}

void SFXEngine::sb_0x424c74__sub2()
{
    for (int i = 0; i < audio_channels; i++)
    {
        userdata_sample_info *v2 = snd_channels[i].sndSouce;
        walsmpl *v3 = snd_channels[i].hSample;

        if ( v2 )
        {
            if ( v2->psampl || v2->flags & 0x200 )
            {
                sb_0x424c74__sub2__sub1(snd_channels[i].sndSouce);

                if ( v2->flags & 4 )
                    wrapper_setSampleVRP(digDriver, v3, v2->resultRate, v2->resultVol, v2->resultPan);
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

void SFXEngine::sb_0x424c74__sub3()
{
    int i = 0;
    int v16 = 256;

    int v10[9];
    int v12[9];

    for (i = 0; i < audio_num_palfx; i++)
    {
        userdata_sample_info *v3 = palFXs[i];

        if ( !v3 )
            break;

        v16 -= v3->palMag;

        v10[i] = v3->paletteFX->slot;

        v12[i] = v3->palMag * 256.0;

        v16 -= v12[i];
    }

    if ( i )
    {
        dword_546F0C = 0;
    }
    else
    {
        if ( dword_546F0C )
            return;

        dword_546F0C = 1;
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

void SFXEngine::sb_0x424c74__sub4()
{
    vec3d tmp(0.0, 0.0, 0.0);

    int i = 0;

    for (i = 0; i < dword_546E14 && ShakeFXs[i]; i++)
    {
        userdata_sample_info *v2 = ShakeFXs[i];

        tmp.x += audio_rnd() * v2->shkMag * v2->shakeFX->pos.x;
        tmp.y += audio_rnd() * v2->shkMag * v2->shakeFX->pos.y;
        tmp.z += audio_rnd() * v2->shkMag * v2->shakeFX->pos.z;
    }

    if ( i > 0 )
        shakeMatrix = mat3x3::Euler_ZXY(tmp);
    else
        shakeMatrix = mat3x3::Ident();
}

void SFXEngine::UpdateMusic()
{
    if (musPlayer && musOn)
    {
        if (musWait)
        {
            if (musWaitSTime + musWaitDelay <= currentTime)
            {
                musWait = false;
                musPlayer->stop();
                musPlayer->play();
            }
        }
        else
        {
            if( musPlayer->isStopped() )
            {
                if( musTrack ) // if audio track was setted - replay it, but do some delay
                {
                    musWait = true;
                    musWaitSTime = currentTime;
                    musWaitDelay = musMinDelay + (musMaxDelay - musMinDelay) * (currentTime % 30) / 30;
                }
            }
        }
    }
}


const mat3x3 &SFXEngine::sb_0x424c74()
{
    if ( digDriver->inited() )
    {
        sb_0x424c74__sub0();
        sb_0x424c74__sub1();
        sb_0x424c74__sub2();
    }

    if ( audio_num_palfx )
        sb_0x424c74__sub3();

    sb_0x424c74__sub4();

    UpdateMusic();

    return shakeMatrix;
}

void SFXEngine::sub_423EFC(int a1, const vec3d &a2, const vec3d &a3, const mat3x3 &a4)
{
    currentTime += a1;

    stru_547018 = a2;
    stru_547024 = a3;
    stru_547030 = a4;

    memset(soundSources, 0, sizeof(soundSources));
    memset(palFXs, 0, sizeof(palFXs));
    memset(ShakeFXs, 0, sizeof(ShakeFXs));
}

void SFXEngine::sub_424CC8()
{
    if ( digDriver )
    {
        for (int i = 0; i < audio_channels; i++)
        {

            if (snd_channels[i].sndSouce )
            {
                snd_channels[i].sndSouce->flags &= ~(4 | 2);

                snd_channels[i].hSample->stop();

                snd_channels[i].sndSouce = NULL;
            }
        }
    }

    for (int i = 0; i < audio_channels; i++)
    {
        if (soundSources[i] )
        {
            soundSources[i]->flags &= ~(4 | 2);
            soundSources[i] = NULL;
        }
    }

    for (int i = 0; i < audio_num_palfx; i++)
    {
        if ( palFXs[i] )
        {
            palFXs[i]->flags &= ~(0x20 | 0x10);
            palFXs[i] = NULL;
        }
    }

    for (int i = 0; i < dword_546E14; i++)
    {
        if ( ShakeFXs[i] )
        {
            ShakeFXs[i]->flags &= ~(0x100 | 0x80);
            ShakeFXs[i] = NULL;
        }
    }
}

userdata_sample_info *SFXEngine::SndGetTopShake()
{
    return ShakeFXs[0];
}
