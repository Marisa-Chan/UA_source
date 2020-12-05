#include <SDL2/SDL_timer.h>

#include <math.h>
#include <stdio.h>

#include "utils.h"
#include "wrapal.h"


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
    _pos = 0;
    _len = 0;
    _start = NULL;
}

size_t walsmpl::_read(void *buf, size_t bufsz)
{
    if (_pos >= _len)
        return 0;

    if (bufsz > _len - _pos)
        bufsz = _len - _pos;

    if (bufsz)
        memcpy(buf, (uint8_t *)_start + _pos, bufsz);

    _pos += bufsz;

    return bufsz;
}

void walsmpl::_rewind()
{
    _pos = 0;
}

void walsmpl::_reset()
{
    CTsmpl::_reset();

    _pos = 0;
    _len = 0;
    _start = NULL;
}


void walsmpl::address(void *start, size_t size, int freq, ALenum format)
{
    if ( SDL_LockMutex(_mutex) == 0)
    {
        alCheck(alSourceStop(_source));

        _clearQueue();

        _start = start;
        _len = size;
        _pos = 0;

        _freq = freq;
        _format = format;

        _endStreamed = false;

        _status = SMPL_STATUS_STOPPED;

        SDL_UnlockMutex(_mutex);
    }
}

void walsmpl::position(size_t newpos)
{
    if ( SDL_LockMutex(_mutex) == 0)
    {
        if (newpos >= 0 && newpos < _len)
        {
            if ( _status == SMPL_STATUS_PLAYING )
                alCheck(alSourceStop(_source));

            _clearQueue();

            _pos = newpos;
            _endStreamed = false;

            for (int i = 0; i < WRAP_AL_BUFFS; i++)
            {
                if ( !_fill_n_queue( i ) )
                    break;
            }

            if ( _status == SMPL_STATUS_PLAYING )
                alCheck(alSourcePlay(_source));
        }

        SDL_UnlockMutex(_mutex);
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

bool walmus::open(const std::string &fname)
{
    if ( SDL_LockMutex(_mutex) == 0)
    {
        _clearQueue();
        _status = SMPL_STATUS_STOPPED;
        _endStreamed = false;

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
            _format = AL_FORMAT_MONO16;
        else if (vorbisInfo->channels == 2)
            _format = AL_FORMAT_STEREO16;

        _freq = vorbisInfo->rate;

        len = ov_pcm_total(&m_vorbis, -1) * 1000 / _freq;

        SDL_UnlockMutex(_mutex);
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






CTsmpl::CTsmpl(waldev *dev, size_t bufsz)
:   _device(dev)
,   _BufSZ(bufsz)
{
    _mutex = SDL_CreateMutex();

    alCheck(alGenSources(1, &_source));

    _buffers.resize(WRAP_AL_BUFFS);
    _used.resize(WRAP_AL_BUFFS);
    _smplBuffers.resize(WRAP_AL_BUFFS);

    for (int i = 0; i < WRAP_AL_BUFFS; i++)
    {
        ALuint tmp = 0;
        alCheck(alGenBuffers(1, &tmp));
        _buffers[i] = tmp;
        _used[i] = false;
        _smplBuffers[i].resize(bufsz + bufsz % 2);
    }

    alCheck(alSource3f(_source, AL_POSITION, 0.0, 0.0, 0.0));
    alCheck(alSourcef(_source, AL_GAIN, 1.0));
    alCheck(alSourcef(_source, AL_PITCH, 1.0));

    _status = SMPL_STATUS_STOPPED;
    _freq = 44100;
    _format = AL_FORMAT_STEREO16;
    _endStreamed = false;
    _eosfunc = NULL;
    _cVolume = 1.0;
    _mVolume = 1.0;

    _loops = 1;
}

CTsmpl::~CTsmpl()
{
    alCheck(alSourceStop(_source));

    _clearQueue();

    alCheck(alDeleteBuffers(_buffers.size(), _buffers.data() ));

    alCheck(alDeleteSources(1, &_source));

    SDL_DestroyMutex(_mutex);
}

void CTsmpl::update()
{
    if ( SDL_TryLockMutex(_mutex) == 0) // we can't wait!
    {
        if ( _status == SMPL_STATUS_PLAYING )
        {
            ALint state;
            alCheck(alGetSourcei(_source, AL_SOURCE_STATE, &state));

            if (state == AL_STOPPED)
            {
                if (_endStreamed) // If data was streamed at all
                {
                    if (_eosfunc)
                        _eosfunc(this);

                    _clearQueue();

                    _status = SMPL_STATUS_STOPPED;
                }
                else // If some data avaliable but for some reason buffers are already played -> continue
                {
                    _clearQueue();

                    for (int i = 0; i < WRAP_AL_BUFFS; i++)
                    {
                        if ( !_fill_n_queue(i) )
                            break;
                    }

                    alCheck(alSourcePlay(_source));
                }
            }
            else if ( state == AL_PLAYING )
            {
                ALint processed = 0;
                alCheck(alGetSourcei(_source, AL_BUFFERS_PROCESSED, &processed));

                while (processed--)
                {
                    ALuint bufid;
                    alCheck(alSourceUnqueueBuffers(_source, 1, &bufid));

                    for (int i = 0; i < WRAP_AL_BUFFS; i++)
                    {
                        if ( _buffers[i] == bufid )
                            _used[i] = _fill_n_queue(i);
                    }
                }
            }
        }

        SDL_UnlockMutex(_mutex);
    }
}


void CTsmpl::_clearQueue()
{
    ALint queued;
    alCheck(alGetSourcei(_source, AL_BUFFERS_QUEUED, &queued));

    for (ALint i = 0; i < queued; ++i)
    {
        ALuint buffer;
        alCheck(alSourceUnqueueBuffers(_source, 1, &buffer));

        for (int j = 0; j < WRAP_AL_BUFFS; ++j)
        {
            if ( _buffers[j] == buffer)
            {
                _used[j] = false;
                break;
            }
        }
    }
}

void CTsmpl::EOS_callback( void (*func)(void *) )
{
    if ( SDL_LockMutex(_mutex) == 0)
    {
        _eosfunc = func;

        SDL_UnlockMutex(_mutex);
    }
}

void CTsmpl::volume(int vol)
{
    if ( SDL_LockMutex(_mutex) == 0)
    {
        _cVolume = (float)vol / 127.0;
        alCheck(alSourcef(_source, AL_GAIN, _cVolume * _mVolume));

        SDL_UnlockMutex(_mutex);
    }
}

void CTsmpl::setMasterVolume(int vol)
{
    if ( SDL_LockMutex(_mutex) == 0)
    {
        _mVolume = (float)vol / 127.0;
        alCheck(alSourcef(_source, AL_GAIN, _cVolume * _mVolume));

        SDL_UnlockMutex(_mutex);
    }
}

void CTsmpl::loop_count(int loops)
{
    if ( SDL_LockMutex(_mutex) == 0)
    {
        _loops = loops;

        SDL_UnlockMutex(_mutex);
    }
}

bool CTsmpl::_fill_n_queue(int bufID)
{
    if (_endStreamed)
        return false;

    size_t totalRead = 0;
    bool zeroread = false;

    while ( totalRead < _BufSZ )
    {
        size_t maxRead = _BufSZ - totalRead;
        size_t readed = _read( _smplBuffers[bufID].data() + totalRead, maxRead );

        if (readed == 0)
        {
            if (zeroread) //twice in row readed 0 bytes
            {
                _loops = 1;
                _endStreamed = true;
                break;
            }

            if ( _loops == 1) //Last loop readed
            {
                _endStreamed = true;
                break;
            }
            else
            {
                if (_loops != 0)
                    _loops--;

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
        ALuint bfid = _buffers[bufID];

        alCheck(alBufferData(bfid, _format, _smplBuffers[bufID].data(), totalRead, _freq));
        _used[bufID] = true;

        alCheck(alSourceQueueBuffers(_source, 1, &bfid));
    }

    return totalRead > 0; // Has data
}

void CTsmpl::_stop()
{
    alCheck(alSourceStop(_source));

    _status = SMPL_STATUS_STOPPED;

    _clearQueue();

    _endStreamed = false;

    _rewind();
}

void CTsmpl::_reset()
{
    alCheck(alSourceStop(_source));

    _clearQueue();

    alCheck(alSource3f(_source, AL_POSITION, 0.0, 0.0, 0.0));
    alCheck(alSourcef(_source, AL_GAIN, 1.0));
    alCheck(alSourcef(_source, AL_PITCH, 1.0));

    _status = SMPL_STATUS_STOPPED;
    _loops = 1;

    _eosfunc = NULL;
    _endStreamed = false;

    _freq = 44100;
    _format = AL_FORMAT_STEREO16;

    _cVolume = 1.0;
    _mVolume = 1.0;
}

void CTsmpl::reset()
{
    if ( SDL_LockMutex(_mutex) == 0)
    {
        _reset();

        SDL_UnlockMutex(_mutex);
    }
}

void CTsmpl::stop()
{
    if ( SDL_LockMutex(_mutex) == 0)
    {
        _stop();

        SDL_UnlockMutex(_mutex);
    }
}

void CTsmpl::pan(int _pan)
{
    if ( SDL_LockMutex(_mutex) == 0)
    {
        float perc = ((float)_pan - 64.0) / 64.0;

        float angl = C_PI_2 + perc * C_PI_2;

        alCheck(alSource3f(_source, AL_POSITION, (float)cos(angl), 0.0, (float)sin(angl)));

        SDL_UnlockMutex(_mutex);
    }
}

void CTsmpl::playback_rate(int newfreq)
{
    if ( SDL_LockMutex(_mutex) == 0)
    {
        alCheck(alSourcef(_source, AL_PITCH, (float)newfreq / (float)_freq));

        SDL_UnlockMutex(_mutex);
    }
}

void CTsmpl::play()
{
    if ( SDL_LockMutex(_mutex) == 0)
    {
        if ( _status == SMPL_STATUS_PLAYING )
            _stop(); //Rewind, clear flags

        for (int i = 0; i < WRAP_AL_BUFFS; i++)
        {
            if ( _used[i] == false )
            {
                if ( !_fill_n_queue(i) ) //No data - don't try
                    break;
            }
        }

        alCheck(alSourcePlay(_source));

        _status = SMPL_STATUS_PLAYING;

        SDL_UnlockMutex(_mutex);
    }
}


bool CTsmpl::isPlaying()
{
    return _status == SMPL_STATUS_PLAYING;
}

bool CTsmpl::isStopped()
{
    return _status == SMPL_STATUS_STOPPED;
}
