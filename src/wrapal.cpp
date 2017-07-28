#include <SDL2/SDL_timer.h>

#include <math.h>
#include <stdio.h>

#include "wrapal.h"

static const float hpi = 3.1415926 / 2.0;

#define WRAP_AL_BUFFS   3
#define WRAP_AL_BUFFSZ  1024
#define WRAP_AL_MUSSBUFF (4096 * 2 * 2)

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
    CTsmpl(_dev, WRAP_AL_BUFFSZ)
{
    pos = 0;
    len = 0;
    start = NULL;
}

size_t walsmpl::_read(void *buf, size_t bufsz)
{
    if (pos >= len)
        return 0;

    if (bufsz > len - pos)
        bufsz = len - pos;

    if (bufsz)
        memcpy(buf, (uint8_t *)start + pos, bufsz);

    pos += bufsz;

    return bufsz;
}

void walsmpl::_rewind()
{
    pos = 0;
}

void walsmpl::_reset()
{
    CTsmpl::_reset();

    pos = 0;
    len = 0;
    start = NULL;
}


void walsmpl::address(void *_start, size_t _size, int _freq, ALenum _format)
{
    if ( SDL_LockMutex(mutex) == 0)
    {
        alCheck(alSourceStop(source));

        _clearQueue();

        start = _start;
        len = _size;
        pos = 0;

        freq = _freq;
        format = _format;

        endStreamed = false;

        status = SMPL_STATUS_STOPPED;

        SDL_UnlockMutex(mutex);
    }
}

void walsmpl::position(size_t newpos)
{
    if ( SDL_LockMutex(mutex) == 0)
    {
        if (newpos >= 0 && newpos < len)
        {
            if ( status == SMPL_STATUS_PLAYING )
                alCheck(alSourceStop(source));

            _clearQueue();

            pos = newpos;
            endStreamed = false;

            for (int i = 0; i < WRAP_AL_BUFFS; i++)
            {
                if ( !_fill_n_queue( i ) )
                    break;
            }

            if ( status == SMPL_STATUS_PLAYING )
                alCheck(alSourcePlay(source));
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
    CTsmpl(_dev, WRAP_AL_MUSSBUFF)
{
    hndl = NULL;
}

walmus::~walmus()
{
    if (hndl)
        delete hndl;

    hndl = NULL;
}

bool walmus::open(const char *fname)
{
    if ( SDL_LockMutex(mutex) == 0)
    {
        _clearQueue();
        status = SMPL_STATUS_STOPPED;
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

        len = ov_pcm_total(&m_vorbis, -1) * 1000 / freq;

        SDL_UnlockMutex(mutex);
        return true;
    }
    return false;
}

size_t walmus::getLen()
{
    return len;
}

size_t walmus::_read(void *buf, size_t bufsz)
{
    if ( !hndl )
        return 0;

    return ov_read(&m_vorbis, (char *)buf, bufsz, 0, 2, 1, NULL);
}

void walmus::_rewind()
{
    if (hndl)
        ov_pcm_seek(&m_vorbis, 0);
}






CTsmpl::CTsmpl(waldev *_dev, size_t bufsz):
    BufSZ(bufsz)
{
    device = _dev;

    mutex = SDL_CreateMutex();

    alCheck(alGenSources(1, &source));

    buffers.resize(WRAP_AL_BUFFS);
    used.resize(WRAP_AL_BUFFS);
    smplBuffers.resize(WRAP_AL_BUFFS);

    for (int i = 0; i < WRAP_AL_BUFFS; i++)
    {
        ALuint tmp = 0;
        alCheck(alGenBuffers(1, &tmp));
        buffers[i] = tmp;
        used[i] = false;
        smplBuffers[i] = new uint8_t[ bufsz + bufsz % 2 ];
    }

    alCheck(alSource3f(source, AL_POSITION, 0.0, 0.0, 0.0));
    alCheck(alSourcef(source, AL_GAIN, 1.0));
    alCheck(alSourcef(source, AL_PITCH, 1.0));

    status = SMPL_STATUS_STOPPED;
    freq = 44100;
    format = AL_FORMAT_STEREO16;
    endStreamed = false;
    eosfunc = NULL;
    cVolume = 1.0;
    mVolume = 1.0;

    loops = 1;
}

CTsmpl::~CTsmpl()
{
    alCheck(alSourceStop(source));

    _clearQueue();

    for (int i = 0; i < WRAP_AL_BUFFS; i++)
    {
        ALuint tmp = buffers[i];
        alCheck(alDeleteBuffers(1, &tmp));
        delete [] smplBuffers[i];
    }

    alCheck(alDeleteSources(1, &source));

    SDL_DestroyMutex(mutex);
}

void CTsmpl::update()
{
    if ( SDL_TryLockMutex(mutex) == 0) // we can't wait!
    {
        if ( status == SMPL_STATUS_PLAYING )
        {
            ALint state;
            alCheck(alGetSourcei(source, AL_SOURCE_STATE, &state));

            if (state == AL_STOPPED)
            {
                if (endStreamed) // If data was streamed at all
                {
                    if (eosfunc)
                        eosfunc(this);

                    _clearQueue();

                    status = SMPL_STATUS_STOPPED;
                }
                else // If some data avaliable but for some reason buffers are already played -> continue
                {
                    _clearQueue();

                    for (int i = 0; i < WRAP_AL_BUFFS; i++)
                    {
                        if ( !_fill_n_queue(i) )
                            break;
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
                            used[i] = _fill_n_queue(i);
                    }
                }
            }
        }

        SDL_UnlockMutex(mutex);
    }
}


void CTsmpl::_clearQueue()
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

void CTsmpl::loop_count(int _loops)
{
    if ( SDL_LockMutex(mutex) == 0)
    {
        loops = _loops;

        SDL_UnlockMutex(mutex);
    }
}

bool CTsmpl::_fill_n_queue(int bufID)
{
    if (endStreamed)
        return false;

    size_t totalRead = 0;
    bool zeroread = false;

    while ( totalRead < BufSZ )
    {
        size_t maxRead = BufSZ - totalRead;
        size_t readed = _read( ((int8_t *)smplBuffers[bufID]) + totalRead, maxRead );

        if (readed == 0)
        {
            if (zeroread) //twice in row readed 0 bytes
            {
                loops = 1;
                endStreamed = true;
                break;
            }

            if ( loops == 1) //Last loop readed
            {
                endStreamed = true;
                break;
            }
            else
            {
                if (loops != 0)
                    loops--;

                _rewind();
            }

            zeroread = true;
        }
        else
            zeroread = false;

        totalRead += readed;
    }

    if (totalRead > 0)
    {
        ALuint bfid = buffers[bufID];

        alCheck(alBufferData(bfid, format, smplBuffers[bufID], totalRead, freq));
        used[bufID] = true;

        alCheck(alSourceQueueBuffers(source, 1, &bfid));
    }

    return totalRead > 0; // Has data
}

void CTsmpl::_stop()
{
    alCheck(alSourceStop(source));

    status = SMPL_STATUS_STOPPED;

    _clearQueue();

    endStreamed = false;

    _rewind();
}

void CTsmpl::_reset()
{
    alCheck(alSourceStop(source));

    _clearQueue();

    alCheck(alSource3f(source, AL_POSITION, 0.0, 0.0, 0.0));
    alCheck(alSourcef(source, AL_GAIN, 1.0));
    alCheck(alSourcef(source, AL_PITCH, 1.0));

    status = SMPL_STATUS_STOPPED;
    loops = 1;

    eosfunc = NULL;
    endStreamed = false;

    freq = 44100;
    format = AL_FORMAT_STEREO16;

    cVolume = 1.0;
    mVolume = 1.0;
}

void CTsmpl::reset()
{
    if ( SDL_LockMutex(mutex) == 0)
    {
        _reset();

        SDL_UnlockMutex(mutex);
    }
}

void CTsmpl::stop()
{
    if ( SDL_LockMutex(mutex) == 0)
    {
        _stop();

        SDL_UnlockMutex(mutex);
    }
}

void CTsmpl::pan(int _pan)
{
    if ( SDL_LockMutex(mutex) == 0)
    {
        float perc = ((float)_pan - 64.0) / 64.0;

        float angl = hpi + perc * hpi;

        alCheck(alSource3f(source, AL_POSITION, (float)cos(angl), 0.0, (float)sin(angl)));

        SDL_UnlockMutex(mutex);
    }
}

void CTsmpl::playback_rate(int newfreq)
{
    if ( SDL_LockMutex(mutex) == 0)
    {
        alCheck(alSourcef(source, AL_PITCH, (float)newfreq / (float)freq));

        SDL_UnlockMutex(mutex);
    }
}

void CTsmpl::play()
{
    if ( SDL_LockMutex(mutex) == 0)
    {
        if ( status == SMPL_STATUS_PLAYING )
            _stop(); //Rewind, clear flags

        for (int i = 0; i < WRAP_AL_BUFFS; i++)
        {
            if ( used[i] == false )
            {
                if ( !_fill_n_queue(i) ) //No data - don't try
                    break;
            }
        }

        alCheck(alSourcePlay(source));

        status = SMPL_STATUS_PLAYING;

        SDL_UnlockMutex(mutex);
    }
}


bool CTsmpl::isPlaying()
{
    return status == SMPL_STATUS_PLAYING;
}

bool CTsmpl::isStopped()
{
    return status == SMPL_STATUS_STOPPED;
}
