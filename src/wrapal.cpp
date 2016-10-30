#include <SDL2/SDL_timer.h>

#include <math.h>
#include <stdio.h>

#include "wrapal.h"

static const float hpi = 3.1415926 / 2.0;

#define WRAP_AL_BUFFS   3
#define WRAP_AL_BUFFSZ  1024
#define WRAP_AL_MUSSBUFF (4096 * 2 * 2)

#define WRAP_STOPPED   0
#define WRAP_PLAYING   1
#define WRAP_PAUSED    2

#ifdef WRAP_DEBUG

// If in debug mode, perform a test on every call
// The do-while loop is needed so that alCheck can be used as a single statement in if/else branches
#define alCheck(expr) do { expr; alCheckError(__FILE__, __LINE__); } while (false)

#else

// Else, we don't add any overhead
#define alCheck(expr) (expr)

#endif


void alCheckError(const char* file, unsigned int line)
{
    ALenum errorCode = alGetError();

    if (errorCode != AL_NO_ERROR)
    {
        const char *error = "Unknown error";

        switch (errorCode)
        {
        case AL_INVALID_NAME:
        {
            error = "AL_INVALID_NAME";
            break;
        }

        case AL_INVALID_ENUM:
        {
            error = "AL_INVALID_ENUM";
            break;
        }

        case AL_INVALID_VALUE:
        {
            error = "AL_INVALID_VALUE";
            break;
        }

        case AL_INVALID_OPERATION:
        {
            error = "AL_INVALID_OPERATION";
            break;
        }

        case AL_OUT_OF_MEMORY:
        {
            error = "AL_OUT_OF_MEMORY";
            break;
        }
        }

        // Log the error
        printf("An internal OpenAL call failed in %s : %d (%s)\n", file, line, error);
    }
}


waldev::waldev()
{
    _inited = false;

    ALfloat ListenerOri[] = { 0.0, 0.0, 1.0,  0.0, 1.0, 0.0 };

    dev = alcOpenDevice(NULL);

    if (dev)
    {
        cont = alcCreateContext(dev, NULL); // In some configurations fpu was not in nearest mode

        if (cont)
        {
            alcMakeContextCurrent(cont);

            alCheck(alListener3f(AL_POSITION, 0.0, 0.0, 0.0));
            alCheck(alListener3f(AL_VELOCITY, 0.0, 0.0, 0.0));
            alCheck(alListenerfv(AL_ORIENTATION, ListenerOri));

            _inited = true;
            ende = false;

            sample_list.clear();

            mutex = SDL_CreateMutex();
            updateThread = SDL_CreateThread(_updateThread, "", this);
        }
        else
        {
            printf("OpenAL: context not created!\n");
            alcCloseDevice(dev);
            dev = NULL;
        }
    }
    else
        printf("OpenAL: no dev!\n");
}

waldev::~waldev()
{
    ende = true;

    int status;
    SDL_WaitThread(updateThread, &status);

    SDL_DestroyMutex(mutex);

    for(std::list<CTsmpl *>::iterator it = sample_list.begin(); it != sample_list.end(); ++it)
    {
        if (*it)
            delete *it;
    }

    alcDestroyContext(cont);
    alcCloseDevice(dev);
}

bool waldev::inited()
{
    return _inited;
}

walsmpl *waldev::newSample()
{
    walsmpl *tmp = new walsmpl(this);

    if (SDL_LockMutex(mutex) == 0)
    {
        sample_list.push_back(tmp);

        SDL_UnlockMutex(mutex);
    }
    return tmp;
}

walmus *waldev::createMusicPlayer()
{
    walmus *tmp = new walmus(this);

    if (SDL_LockMutex(mutex) == 0)
    {
        sample_list.push_back(tmp);

        SDL_UnlockMutex(mutex);
    }
    return tmp;
}

void waldev::deleteSample(CTsmpl *smpl)
{
    if (SDL_LockMutex(mutex) == 0)
    {

        for(std::list<CTsmpl *>::iterator it = sample_list.begin(); it != sample_list.end(); ++it)
        {
            if (*it == smpl)
            {
                delete smpl;
                sample_list.erase(it);
                break;
            }
        }

        SDL_UnlockMutex(mutex);
    }
}

void waldev::master_volume(int vol)
{
    alCheck(alListenerf(AL_GAIN, (float)vol / 127.0 ));
}

bool waldev::update()
{
    if (SDL_LockMutex(mutex) == 0)
    {
        for(std::list<CTsmpl *>::iterator it = sample_list.begin(); it != sample_list.end(); ++it)
        {
            CTsmpl *smpl = *it;

            smpl->update();
        }

        SDL_UnlockMutex(mutex);
    }

    return ende;
}


int waldev::_updateThread(void *data)
{
    waldev* dev = (waldev *)data;

    if (dev)
    {
        for (;;)
        {
            if ( dev->update() )
                break;

            SDL_Delay(10);
        }
    }

    return 0;
}










walsmpl::walsmpl(waldev *_dev):
    CTsmpl(_dev)
{
    loops = 1;
    pos = 0;
    len = 0;
    start = NULL;
}

void walsmpl::init()
{
    if ( SDL_LockMutex(mutex) == 0)
    {
        alCheck(alSourceStop(source));

        clearQueue();

        alCheck(alSource3f(source, AL_POSITION, 0.0, 0.0, 0.0));
        alCheck(alSourcef(source, AL_GAIN, 1.0));
        alCheck(alSourcef(source, AL_PITCH, 1.0));

        status = WRAP_STOPPED;
        loops = 1;
        pos = 0;
        len = 0;
        start = NULL;
        eosfunc = NULL;
        endStreamed = false;

        SDL_UnlockMutex(mutex);
    }
}

int walsmpl::fill_n_queue(int bufID)
{
    if ( !start || !len )
        return 2; //No data

    if ( pos >= len )
        return 1; //EOS

    unsigned int req_len = WRAP_AL_BUFFSZ;

    if ( len - pos < req_len )
        req_len = len - pos;

    if (req_len )
    {
        ALuint bfid = buffers[bufID];

        alCheck(alBufferData(bfid, format, ((char *)start + pos), req_len, freq));
        used[bufID] = true;

        alCheck(alSourceQueueBuffers(source, 1, &bfid));

        pos += req_len;
    }

    if (pos >= len)
    {
        if ( loops == 1 )
            return 1; // EOS

        if ( loops != 0 )  // And > 1
            loops--;

        pos = 0;
    }

    return 0; // Has data
}


void walsmpl::address(void *_start, size_t _size, int _freq, ALenum _format)
{
    if ( SDL_LockMutex(mutex) == 0)
    {
        alCheck(alSourceStop(source));

        clearQueue();

        start = _start;
        len = _size;
        pos = 0;

        freq = _freq;
        format = _format;

        endStreamed = false;

        status = WRAP_STOPPED;

        SDL_UnlockMutex(mutex);
    }
}

bool walsmpl::isqueued()
{
    ALint queued;
    alCheck(alGetSourcei(source, AL_BUFFERS_QUEUED, &queued));

    return queued != 0;
}

void walsmpl::play()
{
    if ( SDL_LockMutex(mutex) == 0)
    {
        if ( status == WRAP_STOPPED || status == WRAP_PAUSED )
        {
            endStreamed = false;

            if ( !isqueued() )
            {
                for (int i = 0; i < WRAP_AL_BUFFS; i++)
                {
                    if ( fill_n_queue( i ) > 0 )
                    {
                        endStreamed = true;
                        break;
                    }
                }
            }

            alCheck(alSourcePlay(source));

            status = WRAP_PLAYING;
        }

        SDL_UnlockMutex(mutex);
    }
}

void walsmpl::stop()
{
    if ( SDL_LockMutex(mutex) == 0)
    {
        alCheck(alSourceStop(source));

        status = WRAP_STOPPED;

        clearQueue();

        pos = 0;
        endStreamed = false;

        SDL_UnlockMutex(mutex);
    }
}

void walsmpl::position(size_t newpos)
{
    if ( SDL_LockMutex(mutex) == 0)
    {
        if (newpos >= 0 && newpos < len)
        {
            if ( status == WRAP_PLAYING )
                alCheck(alSourceStop(source));

            clearQueue();

            pos = newpos;
            endStreamed = false;

            for (int i = 0; i < WRAP_AL_BUFFS; i++)
            {
                if ( fill_n_queue( i ) > 0 )
                {
                    endStreamed = true;
                    break;
                }
            }

            if ( status == WRAP_PLAYING )
                alCheck(alSourcePlay(source));
        }

        SDL_UnlockMutex(mutex);
    }
}

void walsmpl::loop_count(int _loops)
{
    if ( SDL_LockMutex(mutex) == 0)
    {
        loops = _loops;

        SDL_UnlockMutex(mutex);
    }
}

void walsmpl::pan(int _pan)
{
    if ( SDL_LockMutex(mutex) == 0)
    {
        float perc = ((float)_pan - 64.0) / 64.0;

        float angl = hpi + perc * hpi;

        alCheck(alSource3f(source, AL_POSITION, (float)cos(angl), 0.0, (float)sin(angl)));

        SDL_UnlockMutex(mutex);
    }
}

void walsmpl::playback_rate(int newfreq)
{
    if ( SDL_LockMutex(mutex) == 0)
    {
        alCheck(alSourcef(source, AL_PITCH, (float)newfreq / (float)freq));

        SDL_UnlockMutex(mutex);
    }
}

void walsmpl::update()
{
    if ( SDL_TryLockMutex(mutex) == 0) // we can't wait!
    {
        if ( status == WRAP_PLAYING )
        {
            ALint state;
            alCheck(alGetSourcei(source, AL_SOURCE_STATE, &state));

            if (state == AL_STOPPED)
            {
                if (endStreamed)
                {
                    if (eosfunc)
                        eosfunc(this);

                    clearQueue();

                    status = WRAP_STOPPED;
                }
                else
                {
                    clearQueue();

                    for (int i = 0; i < WRAP_AL_BUFFS; i++)
                    {
                        if ( used[i] == false )
                        {
                            if ( fill_n_queue(i) > 0 )
                            {
                                endStreamed = true;
                                break;
                            }
                        }
                    }

                    alCheck(alSourcePlay(source));
                }
            }
            else if ( state == AL_PLAYING )
            {
                ALint processed = 0;
                alCheck(alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed));

                while (processed--)
                {
                    ALuint bufid;
                    alCheck(alSourceUnqueueBuffers(source, 1, &bufid));

                    for (int i = 0; i < WRAP_AL_BUFFS; i++)
                    {
                        if ( buffers[i] == bufid )
                        {
                            used[i] = false;
                            break;
                        }
                    }
                }

                if ( !endStreamed )
                {
                    for (int i = 0; i < WRAP_AL_BUFFS; i++)
                    {
                        if ( used[i] == false )
                        {
                            if ( fill_n_queue(i) > 0 )
                            {
                                endStreamed = true;
                                break;
                            }
                        }
                    }
                }

            }
        }

        SDL_UnlockMutex(mutex);
    }
}










size_t read(void* ptr, size_t size, size_t nmemb, void* data)
{
    FSMgr::FileHandle *stream = static_cast<FSMgr::FileHandle *>(data);
    return stream->read(ptr, size * nmemb);
}

int seek(void* data, ogg_int64_t offset, int whence)
{
    FSMgr::FileHandle *stream = static_cast<FSMgr::FileHandle *>(data);
    return stream->seek(offset, whence);
}

long tell(void* data)
{
    FSMgr::FileHandle *stream = static_cast<FSMgr::FileHandle *>(data);
    return stream->tell();
}

static ov_callbacks callbacks = {&read, &seek, NULL, &tell};


walmus::walmus(waldev *_dev):
    CTsmpl(_dev)
{
    smplBuffers.resize(WRAP_AL_BUFFS);

    for (int i = 0; i < WRAP_AL_BUFFS; i++)
        smplBuffers[i] = new int16_t[ WRAP_AL_MUSSBUFF / 2 + (WRAP_AL_MUSSBUFF % 2)];

    hndl = NULL;
}

walmus::~walmus()
{
    alCheck(alSourceStop(source));

    for (int i = 0; i < WRAP_AL_BUFFS; i++)
        delete smplBuffers[i];
}

bool walmus::open(const char *fname)
{
    if ( SDL_LockMutex(mutex) == 0)
    {
        clearQueue();
        status = WRAP_STOPPED;
        endStreamed = false;

        if (hndl)
        {
            ov_clear(&m_vorbis);
            delete hndl;
        }

        hndl = FSMgr::iDir::openFile(fname, "rb");

        if (!hndl)
            return false;

        int status = ov_open_callbacks(hndl, &m_vorbis, NULL, 0, callbacks);
        if (status < 0)
        {
            delete hndl;
            hndl = NULL;
            return false;
        }

        vorbis_info* vorbisInfo = ov_info(&m_vorbis, -1);

        if (vorbisInfo->channels == 1)
            format = AL_FORMAT_MONO16;
        else if (vorbisInfo->channels == 2)
            format = AL_FORMAT_STEREO16;

        freq = vorbisInfo->rate;

        SDL_UnlockMutex(mutex);
        return true;
    }
    return false;
}

int walmus::fill_n_queue(int bufID)
{
    if ( !hndl )
        return 2; //No data

    long totalRead = 0;

    while (totalRead < WRAP_AL_MUSSBUFF)
    {
        long maxRead = WRAP_AL_MUSSBUFF - totalRead;
        long readed = ov_read(&m_vorbis, ((char *)smplBuffers[bufID]) + totalRead, maxRead, 0, 2, 1, NULL);

        if (readed == 0 && totalRead == 0)
            return 1; //EOS

        totalRead += readed;
    }

    ALuint bfid = buffers[bufID];

    alCheck(alBufferData(bfid, format, smplBuffers[bufID], totalRead, freq));
    used[bufID] = true;

    alCheck(alSourceQueueBuffers(source, 1, &bfid));

    return 0; // Has data
}


void walmus::play()
{
    if ( SDL_LockMutex(mutex) == 0)
    {
        if (hndl)
        {
            if ( status == WRAP_STOPPED ) // || status == WRAP_PAUSED )
            {
                ov_pcm_seek(&m_vorbis, 0);

                alCheck(alSourceStop(source));
                clearQueue();
                endStreamed = false;

                for (int i = 0; i < WRAP_AL_BUFFS; i++)
                {
                    if ( fill_n_queue( i ) > 0 )
                    {
                        endStreamed = true;
                        break;
                    }
                }

                alCheck(alSourcePlay(source));
                status = WRAP_PLAYING;
            }
            else if ( status == WRAP_PAUSED )
            {
                alCheck(alSourcePlay(source));
                status = WRAP_PLAYING;
            }
        }

        SDL_UnlockMutex(mutex);
    }
}

void walmus::stop()
{
    if ( SDL_LockMutex(mutex) == 0)
    {
        alCheck(alSourceStop(source));

        status = WRAP_STOPPED;

        clearQueue();

        endStreamed = false;

        SDL_UnlockMutex(mutex);
    }
}








CTsmpl::CTsmpl(waldev *_dev)
{
    device = _dev;

    mutex = SDL_CreateMutex();

    alCheck(alGenSources(1, &source));

    buffers.resize(WRAP_AL_BUFFS);
    used.resize(WRAP_AL_BUFFS);

    for (int i = 0; i < WRAP_AL_BUFFS; i++)
    {
        ALuint tmp = 0;
        alCheck(alGenBuffers(1, &tmp));
        buffers[i] = tmp;
        used[i] = false;
    }

    alCheck(alSource3f(source, AL_POSITION, 0.0, 0.0, 0.0));
    alCheck(alSourcef(source, AL_GAIN, 1.0));
    alCheck(alSourcef(source, AL_PITCH, 1.0));

    status = WRAP_STOPPED;
    freq = 44100;
    format = AL_FORMAT_STEREO16;
    endStreamed = false;
    eosfunc = NULL;
    cVolume = 1.0;
    mVolume = 1.0;
}

CTsmpl::~CTsmpl()
{
    alCheck(alSourceStop(source));

    clearQueue();

    for (int i = 0; i < WRAP_AL_BUFFS; i++)
    {
        ALuint tmp = buffers[i];
        alCheck(alDeleteBuffers(1, &tmp));
    }

    alCheck(alDeleteSources(1, &source));

    SDL_DestroyMutex(mutex);
}

void CTsmpl::update()
{
    if ( SDL_TryLockMutex(mutex) == 0) // we can't wait!
    {
        if ( status == WRAP_PLAYING )
        {
            ALint state;
            alCheck(alGetSourcei(source, AL_SOURCE_STATE, &state));

            if (state == AL_STOPPED)
            {
                if (endStreamed)
                {
                    clearQueue();

                    status = WRAP_STOPPED;
                }
                else
                {
                    clearQueue();

                    for (int i = 0; i < WRAP_AL_BUFFS; i++)
                    {
                        if ( used[i] == false )
                        {
                            if ( fill_n_queue(i) > 0 )
                            {
                                endStreamed = true;
                                break;
                            }
                        }
                    }

                    alCheck(alSourcePlay(source));
                }
            }
            else if ( state == AL_PLAYING )
            {
                ALint processed = 0;
                alCheck(alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed));

                while (processed--)
                {
                    ALuint bufid;
                    alCheck(alSourceUnqueueBuffers(source, 1, &bufid));

                    for (int i = 0; i < WRAP_AL_BUFFS; i++)
                    {
                        if ( buffers[i] == bufid )
                        {
                            used[i] = false;
                            break;
                        }
                    }
                }

                if ( !endStreamed )
                {
                    for (int i = 0; i < WRAP_AL_BUFFS; i++)
                    {
                        if ( used[i] == false )
                        {
                            if ( fill_n_queue(i) > 0 )
                            {
                                endStreamed = true;
                                break;
                            }
                        }
                    }
                }

            }
        }

        SDL_UnlockMutex(mutex);
    }
}


void CTsmpl::clearQueue()
{
    ALint queued;
    alCheck(alGetSourcei(source, AL_BUFFERS_QUEUED, &queued));

    for (ALint i = 0; i < queued; ++i)
    {
        ALuint buffer;
        alCheck(alSourceUnqueueBuffers(source, 1, &buffer));

        for (int j = 0; j < WRAP_AL_BUFFS; ++j)
        {
            if ( buffers[j] == buffer)
            {
                used[j] = false;
                break;
            }
        }
    }
}

void CTsmpl::EOS_callback( void (*func)(void *) )
{
    if ( SDL_LockMutex(mutex) == 0)
    {
        eosfunc = func;

        SDL_UnlockMutex(mutex);
    }
}

void CTsmpl::volume(int _vol)
{
    if ( SDL_LockMutex(mutex) == 0)
    {
        cVolume = (float)_vol / 127.0;
        alCheck(alSourcef(source, AL_GAIN, cVolume * mVolume));

        SDL_UnlockMutex(mutex);
    }
}

void CTsmpl::setMasterVolume(int _vol)
{
    if ( SDL_LockMutex(mutex) == 0)
    {
        mVolume = (float)_vol / 127.0;
        alCheck(alSourcef(source, AL_GAIN, cVolume * mVolume));

        SDL_UnlockMutex(mutex);
    }
}
