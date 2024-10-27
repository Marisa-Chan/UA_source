#include <fmt/printf.h>
#include "common/common.h"
#include "../ini.h"
#include "../log.h"
#include <math.h>
#include "sound.h"
#include "common/common.h"
#include "inivals.h"


SFXEngine SFXEngine::SFXe;

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

    System::IniConf::ReadFromNucleusIni();

    audio_channels  = System::IniConf::AudioChannels.Get<int>();
    audio_volume    = System::IniConf::AudioVolume.Get<int>();
    audio_num_palfx = System::IniConf::AudioNumPalfx.Get<int>();
    audio_rev_stereo = System::IniConf::AudioRevStereo.Get<bool>();

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
        
        AudioStream = digDriver->CreateStream();
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


void SFXEngine::startSound(TSndCarrier *smpls, size_t id)
{
    if (id >= smpls->Sounds.size())
        return;
    
    TSoundSource *result = &smpls->Sounds.at(id);

    result->StartTime = currentTime;

    if ( result->IsFragmented() || result->PSample )
    {
        result->CurrentFrag = 0;
        result->SetEnabled(true);
        result->SetPlay(false);
    }
    if ( result->IsPFx() )
    {
        result->SetPFxEnable(true);
        result->SetPFxPlay(false);
        result->PFxMag = result->PPFx->mag0;
    }
    if ( result->IsShk() )
    {
        result->SetShkEnable(true);
        result->SetShkPlay(false);
        result->ShkMag = result->PShkFx->mag0;
    }
}

void SFXEngine::sub_424000(TSndCarrier *smpls, size_t id)
{
    if (id >= smpls->Sounds.size())
        return;
    
    smpls->Sounds[id].SetShkEnable(false);
    smpls->Sounds[id].SetPFxEnable(false);
    smpls->Sounds[id].SetEnabled(false);
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

void SFXEngine::StopMusicTrack(bool reset)
{
    if (digDriver && musPlayer && musOn)
    {
        musPlayer->stop();
        if (reset)
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
        if ( musPlayer->open( fmt::sprintf("music/%d.ogg", trackID) ) )
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
void SFXEngine::StopCarrier(TSndCarrier *carr)
{
    for (int i = 0; i < audio_channels; i++)
    {
        if ( digDriver )
        {
            if (snd_channels[i].sndSouce && snd_channels[i].sndSouce->PCarrier == carr )
            {
                snd_channels[i].sndSouce->SetPlay(false);
                snd_channels[i].sndSouce->SetEnabled(false);

                snd_channels[i].hSample->stop();

                snd_channels[i].sndSouce = NULL;
            }

            if (soundSources[i] && soundSources[i]->PCarrier == carr )
            {
                soundSources[i]->SetPlay(false);
                soundSources[i]->SetEnabled(false);
                soundSources[i] = NULL;
            }
        }
    }

    for (int i = 0; i < audio_num_palfx; i++)
    {
        if ( palFXs[i] && palFXs[i]->PCarrier == carr )
        {
            palFXs[i]->SetPFxEnable(false);
            palFXs[i]->SetPFxPlay(false);
            palFXs[i] = NULL;
        }
    }

    for (int i = 0; i < dword_546E14; i++)
    {
        if ( ShakeFXs[i] && ShakeFXs[i]->PCarrier == carr )
        {
            ShakeFXs[i]->SetShkEnable(false);
            ShakeFXs[i]->SetShkPlay(false);
            ShakeFXs[i] = NULL;
        }
    }

    for (TSoundSource &snd : carr->Sounds)
    {
        snd.SetShkEnable(false);
        snd.SetPFxEnable(false);
        snd.SetEnabled(false);
    }
}

void SFXEngine::StopSource(TSoundSource *snd)
{
    for (int i = 0; i < audio_channels; i++)
    {
        if ( digDriver )
        {
            if (snd_channels[i].sndSouce == snd )
            {
                snd_channels[i].sndSouce->SetPlay(false);
                snd_channels[i].sndSouce->SetEnabled(false);

                snd_channels[i].hSample->stop();

                snd_channels[i].sndSouce = NULL;
            }

            if (soundSources[i] == snd )
            {
                soundSources[i]->SetPlay(false);
                soundSources[i]->SetEnabled(false);
                soundSources[i] = NULL;
            }
        }
    }

    for (int i = 0; i < audio_num_palfx; i++)
    {
        if ( palFXs[i] == snd )
        {
            palFXs[i]->SetPFxEnable(false);
            palFXs[i]->SetPFxPlay(false);
            palFXs[i] = NULL;
        }
    }

    for (int i = 0; i < dword_546E14; i++)
    {
        if ( ShakeFXs[i] == snd )
        {
            ShakeFXs[i]->SetShkEnable(false);
            ShakeFXs[i]->SetShkPlay(false);
            ShakeFXs[i] = NULL;
        }
    }
}

float SFXEngine::audio_rnd()
{
    float tmp = flt_546F18[dword_546F14];

    dword_546F14++;

    if ( dword_546F14 >= 64 )
        dword_546F14 = 0;

    return tmp;
}

void SFXEngine::audio_InsertSoundSource(TSoundSource *smpl)
{
    int min_i = -1;
    int v3 = 1000000;

    if ( digDriver->inited() )
    {
        for (int i = 0; i < audio_channels; i++)
        {
            if ( soundSources[i] )
            {
                if ( soundSources[i]->Priority < v3 )
                {
                    min_i = i;
                    v3 = soundSources[i]->Priority;
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

        if ( min_i != -1 && v3 < smpl->Priority )
            soundSources[min_i] = smpl;
    }
}

int SFXEngine::sub_423B3C(TSoundSource *smpl, int a2, int *a3)
{
    int i = 0;

    for (TSampleParams &pr : *(smpl->PFragments))
    {
        if ( !pr.Loop )
            break;

        int v9 = pr.SampleRate + pr.Sample->SampleRate + smpl->Pitch;

        if ( v9 <= 0 )
            v9 = 1;

        int v10 = (pr.Loop * pr.rlSmplCnt / 1024) / v9;

        if ( a2 < v10 )
            break;

        a2 -= v10;
        
        i++;
    }

    if ( a3 )
        *a3 = a2;

    return i;
}

void SFXEngine::SoundCarrierProcessSounds(TSndCarrier *smpls, float distance)
{
    float v10 = distance / 200.0;

    for (TSoundSource &snd : smpls->Sounds)
    {
        if ( snd.PSample || snd.IsFragmented() )
        {
            size_t fragId = 0;

            if ( snd.IsEnabled() )
            {
                int v6 = currentTime - snd.StartTime;

                if ( snd.IsFragmented() )
                {
                    if ( snd.IsPlay() )
                        fragId = snd.CurrentFrag;
                    else
                        fragId = sub_423B3C(&snd, currentTime - snd.StartTime, NULL);
                }

                bool ok = true;

                if ( !snd.IsLoop() && !snd.IsPlay() && v6 > 0 )
                {
                    if ( snd.IsFragmented() )
                    {
                        if ( fragId >= snd.PFragments->size() )
                        {
                            snd.SetEnabled(false);
                            ok = false;
                        }
                    }
                    else if ( (v6 * (snd.Pitch + snd.PSample->SampleRate) / 1024) > snd.PSample->bufsz )
                    {
                        snd.SetEnabled(false);
                        ok = false;
                    }
                }

                if (ok)
                {
                    snd.ResultVol = snd.Volume;

                    if ( snd.IsFragmented() )
                        snd.ResultVol += snd.PFragments->at(fragId).Vol;

                    if ( v10 >= 1.0 )
                        snd.ResultVol /= v10;

                    if ( snd.ResultVol > 4 )
                    {
                        snd.Priority = snd.ResultVol;
                        audio_InsertSoundSource(&snd);
                    }
                }

            }
        }
    }
}


void SFXEngine::audio_InsertPalFX(TSoundSource *smpl)
{
    int min_i = -1;
    int a1 = 1000000;

    for (int i = 0; i < audio_num_palfx; i++)
    {
        if ( palFXs[i] )
        {
            if ( palFXs[i]->PFxMag )
            {
                a1 = palFXs[i]->PFxMag;
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

    if ( min_i != -1 && a1 < smpl->PFxMag )
        palFXs[min_i] = smpl;
}

void SFXEngine::SoundCarrierProcessPFx(TSndCarrier *smpls, float distance)
{
    float v6 = distance / 300.0;

    for (TSoundSource &snd : smpls->Sounds)
    {
        if ( snd.IsPFx() )
        {
            if ( snd.IsPFxEnabled() )
            {
                if ( !snd.IsLoop() && (size_t)(snd.StartTime + snd.PPFx->time) < currentTime )
                    snd.SetPFxEnable(false);
            }

            if ( snd.IsPFxEnabled() )
            {
                if ( snd.IsLoop() )
                    snd.PFxMag = snd.PPFx->mag0;
                else
                    snd.PFxMag = (snd.PPFx->mag1 - snd.PPFx->mag0) * ((float)(currentTime - snd.StartTime) / (float)snd.PPFx->time) + snd.PPFx->mag0;

                if ( v6 >= 1.0 )
                    snd.PFxMag /= v6;

                audio_InsertPalFX(&snd);
            }
        }
    }
}

void SFXEngine::InsertShakeFX(TSoundSource *smpl)
{
    int min_i = -1;
    int var_1C = 1000;

    for (int i = 0; i < dword_546E14; i++)
    {
        TSoundSource *v5 = ShakeFXs[i];

        if ( v5 )
        {
            if ( var_1C > v5->ShkMag )
            {
                var_1C = v5->ShkMag;
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

    if ( min_i != -1 && var_1C < smpl->ShkMag )
        ShakeFXs[min_i] = smpl;
}

void SFXEngine::SoundCarrierProcessShake(TSndCarrier *smpls, float distance)
{
    for (TSoundSource &snd : smpls->Sounds)
    {
        if ( snd.IsShk() )
        {
            if ( snd.IsShkEnabled() )
            {
                if ( !snd.IsLoop() && (size_t)(snd.StartTime + snd.PShkFx->time) < currentTime )
                    snd.SetShkEnable(false);
            }

            if ( snd.IsShkEnabled() )
            {
                if ( snd.IsLoop() )
                    snd.ShkMag = snd.PShkFx->mag0;
                else
                    snd.ShkMag = (snd.PShkFx->mag1 - snd.PShkFx->mag0) * ((float)(currentTime - snd.StartTime) / (float)snd.PShkFx->time) + snd.PShkFx->mag0;

                float v6 = distance * snd.PShkFx->mute;

                if ( v6 >= 1.0 )
                    snd.ShkMag /= v6;

                InsertShakeFX(&snd);
            }
        }
    }
}

// Insert new sound
void SFXEngine::UpdateSoundCarrier(TSndCarrier *smpls)
{
    float distance = (smpls->Position - stru_547018).length();

    if ( distance < 6000.0 )
    {
        if ( digDriver->inited() )
            SoundCarrierProcessSounds(smpls, distance);

        if ( distance < 2400.0 )
        {
            if ( audio_num_palfx )
                SoundCarrierProcessPFx(smpls, distance);

            SoundCarrierProcessShake(smpls, distance);
        }
    }
}

void SFXEngine::sb_0x424c74__sub0()
{
    //Free ended channels
    for (int i = 0; i < audio_channels; i++)
    {
        TSoundSource *v2 = snd_channels[i].sndSouce;

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
                v2->SetPlay(false);

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
        TSoundSource *v3 = soundSources[i];

        if ( v3 && !v3->IsPlay() )
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

void SFXEngine::sb_0x424c74__sub2__sub1(TSoundSource *smpl)
{
    TSndCarrier *v2 = smpl->PCarrier;

    vec3d v3 = stru_547018 - v2->Position;
    float v27 = v3.length();

    vec3d v8 = v2->Vector - stru_547024;
    float v20 = v8.length();

    float v21 = v27  *  v20;
    float v19 = 0.0;

    if ( v21 > 0.0 )
        v19 = v3.dot( v8 ) / v21;

    int v14;

    if ( smpl->IsFragmented() )
        v14 = smpl->PFragments->at(smpl->CurrentFrag).SampleRate + smpl->PFragments->at(smpl->CurrentFrag).Sample->SampleRate + smpl->Pitch;
    else
        v14 = smpl->Pitch + smpl->PSample->SampleRate;

    if ( v14 < 2000 )
        v14 = 2000;
    else if ( v14 > 44100 )
        v14 = 44100;

    float v31 = stru_547030.AxisX().dot( v3 );

    smpl->ResultRate = v14 + v14 * (int)(v19 * v20) / 400;

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

    smpl->ResultPan = (v32 * 64.0 + 64.0);

    if ( smpl->ResultVol < 0 )
        smpl->ResultVol = 0;
    else if ( smpl->ResultVol > 127 )
        smpl->ResultVol = 127;

    if ( smpl->ResultPan < 0 )
        smpl->ResultPan = 0;
    else if ( smpl->ResultPan > 127 )
        smpl->ResultPan = 127;

    if ( v14 / 2 <= smpl->ResultRate )
    {
        if ( smpl->ResultRate > v14 * 2)
            smpl->ResultRate = v14 * 2;
    }
    else
    {
        smpl->ResultRate = v14 / 2;
    }
}

void SFXEngine::sound_eos_clbk(void *_smpl)
{
    walsmpl *smpl = static_cast<walsmpl *>(_smpl);

    int v2 = 0;
    TSoundSource *v3 = NULL;

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
        if ( v3->IsEnabled() )
        {
            if ( v3->IsFragmented() )
            {
                v3->CurrentFrag++;

                if ( v3->CurrentFrag >= v3->PFragments->size() )
                {
                    if ( v3->IsLoop() )
                        v3->CurrentFrag = 0;
                    else
                        v3->SetEnabled(false);
                }

                if ( v3->IsEnabled() )
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

                    TSampleParams *v10 = &v3->PFragments->at( v3->CurrentFrag );

                    wrapper_playSound(
                        SFXe.digDriver,
                        smpl,
                        sound_eos_clbk,
                        (char *)v10->Sample->Data + v10->rlOffset,
                        v10->rlSmplCnt,
                        v3->ResultRate,
                        v3->ResultVol,
                        SFXe.audio_volume,
                        v3->ResultPan,
                        v10->Loop,
                        0);
                }
            }
            else
            {
                v3->SetEnabled(false);
            }
        }
    }
}


void SFXEngine::sb_0x424c74__sub2__sub0(int id, TSoundSource *smpl)
{
    walsmpl *v3 = snd_channels[id].hSample;

    smpl->SetPlay(true);

    if ( smpl->IsFragmented() )
    {
        int v4 = currentTime - smpl->StartTime;
        int v8;

        if ( v4 <= 0 )
        {
            smpl->CurrentFrag = 0;
            v8 = 0;
        }
        else
        {
            int a3;
            size_t v7 = sub_423B3C(smpl, v4, &a3);

            if ( v7 == smpl->PFragments->size() )
                v7 = smpl->PFragments->size() - 1;

            v8 = ((smpl->ResultRate * a3) / 1024) % smpl->PFragments->at(v7).rlSmplCnt;
            smpl->CurrentFrag = v7;
        }

        TSampleParams *v9 = &smpl->PFragments->at(smpl->CurrentFrag);

        wrapper_playSound(
            digDriver,
            v3,
            sound_eos_clbk,
            (char *)v9->Sample->Data + v9->rlOffset,
            v9->rlSmplCnt,
            smpl->ResultRate,
            smpl->ResultVol,
            audio_volume,
            smpl->ResultPan,
            v9->Loop,
            v8);
    }
    else
    {
        wrapper_playSound(
            digDriver,
            v3,
            sound_eos_clbk,
            smpl->PSample->Data,
            smpl->PSample->bufsz,
            smpl->ResultRate,
            smpl->ResultVol,
            audio_volume,
            smpl->ResultPan,
            (smpl->IsLoop() ? 0 : 1),
            (int)((smpl->PSample->SampleRate + smpl->Pitch) * (currentTime - smpl->StartTime) >> 10) % smpl->PSample->bufsz);
    }
}

void SFXEngine::sb_0x424c74__sub2()
{
    for (int i = 0; i < audio_channels; i++)
    {
        TSoundSource *v2 = snd_channels[i].sndSouce;
        walsmpl *v3 = snd_channels[i].hSample;

        if ( v2 )
        {
            if ( v2->PSample || v2->IsFragmented() )
            {
                sb_0x424c74__sub2__sub1(snd_channels[i].sndSouce);

                if ( v2->IsPlay() )
                    wrapper_setSampleVRP(digDriver, v3, v2->ResultRate, v2->ResultVol, v2->ResultPan);
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
    float pwr = 1.0;
    std::vector<GFX::ColorFx> fxes;

    int i = 0;
    for (i = 0; i < audio_num_palfx; i++)
    {
        TSoundSource *v3 = palFXs[i];

        if ( !v3 )
            break;
        
        float magpwr = v3->PFxMag * GFX::Engine.GetColorEffectPower(v3->PPFx->slot);

        pwr -= magpwr;
        
        fxes.emplace_back(v3->PPFx->slot, magpwr);
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

    if ( pwr > 0.0 )
        fxes.emplace_back(0, pwr);

    GFX::Engine.SetColorEffectsPowers(fxes);
}

void SFXEngine::sb_0x424c74__sub4()
{
    vec3d tmp(0.0, 0.0, 0.0);

    int i = 0;

    for (i = 0; i < dword_546E14 && ShakeFXs[i]; i++)
    {
        TSoundSource *v2 = ShakeFXs[i];

        tmp.x += audio_rnd() * v2->ShkMag * v2->PShkFx->pos.x;
        tmp.y += audio_rnd() * v2->ShkMag * v2->PShkFx->pos.y;
        tmp.z += audio_rnd() * v2->ShkMag * v2->PShkFx->pos.z;
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

    soundSources.fill(NULL);
    palFXs.fill(NULL);
    ShakeFXs.fill(NULL);
}

void SFXEngine::StopPlayingSounds()
{
    if ( digDriver )
    {
        for (int i = 0; i < audio_channels; i++)
        {

            if (snd_channels[i].sndSouce )
            {
                snd_channels[i].sndSouce->SetPlay(false);
                snd_channels[i].sndSouce->SetEnabled(false);

                snd_channels[i].hSample->stop();

                snd_channels[i].sndSouce = NULL;
            }
        }
    }

    for (int i = 0; i < audio_channels; i++)
    {
        if (soundSources[i] )
        {
            soundSources[i]->SetPlay(false);
            soundSources[i]->SetEnabled(false);
            soundSources[i] = NULL;
        }
    }

    for (int i = 0; i < audio_num_palfx; i++)
    {
        if ( palFXs[i] )
        {
            palFXs[i]->SetPFxEnable(false);
            palFXs[i]->SetPFxPlay(false);
            palFXs[i] = NULL;
        }
    }

    for (int i = 0; i < dword_546E14; i++)
    {
        if ( ShakeFXs[i] )
        {
            ShakeFXs[i]->SetShkEnable(false);
            ShakeFXs[i]->SetShkPlay(false);
            ShakeFXs[i] = NULL;
        }
    }
}

TSoundSource *SFXEngine::SndGetTopShake()
{
    return ShakeFXs[0];
}
