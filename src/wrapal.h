#ifndef WRAP_AL_H_INCLUDED
#define WRAP_AL_H_INCLUDED

#include <list>
#include <vector>
#include <queue>
#if defined(__APPLE__) && defined(__MACH__)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_mutex.h>

#define OV_EXCLUDE_STATIC_CALLBACKS
#include <vorbis/vorbisfile.h>

#include "system/fsmgr.h"

class waldev;

class CTsmpl
{
    friend class waldev;
    
protected:
    struct AlBuffer
    {
        ALuint ID = 0;
        std::vector<uint8_t> Samples;
        bool Used = false;
        size_t Size = 0;
        int32_t TS = -1; //Timestamp on end of buffer
    };

public:
    virtual void update();

    void loop_count(int loops);
    void volume(int vol);
    void setMasterVolume(int vol);
    void play();
    void stop();
    void reset();
    void playback_rate(int newfreq);
    void pan(int pan);

    void EOS_callback( void (*func)(void *) );

    bool isPlaying();
    bool isStopped();


    enum SMPL_STATUS
    {
        SMPL_STATUS_STOPPED = 0,
        SMPL_STATUS_PLAYING = 1,
        SMPL_STATUS_PAUSED  = 2
    };
    
    int GetQueueSize();
    bool IsFreeBuffer();
    size_t BuffersCapacity() const;
    
    uint32_t BuffersTime() const;
    uint32_t GetStreamedTime() const;
    uint32_t GetPlayedTime() const;
    int32_t GetTS() const;

protected:
    CTsmpl(waldev *dev, size_t bufsz);
    virtual ~CTsmpl();
    virtual size_t _read(void *buf, size_t bufsz, int32_t *endTs) = 0;
    virtual void _rewind() = 0;
    virtual void _stop(); //Non-blocable stop
    virtual void _reset();

    void _clearQueue();
    bool _fill_n_queue( AlBuffer *buf );
    
    uint32_t BytesToMsec(uint32_t bytes) const;

    const waldev *_device;

    ALuint _source;
    std::vector<AlBuffer> _buffers;

    ALenum _format;
    int _freq;

    SDL_mutex *_mutex;
    int _status;
    size_t _loops;

    bool _endStreamed;

    ALfloat _cVolume;
    ALfloat _mVolume;
    
    uint64_t _queuedBytes = 0;
    uint64_t _playedBytes = 0;
    
    int32_t _TS = 0;
    
    const size_t _BufSZ;

    void (*_eosfunc)(void *);
};

class walsmpl: public CTsmpl
{
    friend waldev;

public:
    void address(void *start, size_t size, int freq, ALenum format);
    void position(size_t pos);

protected:
    walsmpl(waldev *dev);

    virtual size_t _read(void *buf, size_t bufsz, int32_t *endTs);
    virtual void _rewind();
    virtual void _reset();

    void *_start;
    size_t _len;
    size_t _pos;
};



class walmus: public CTsmpl
{
    friend waldev;

public:
    bool open(const std::string &fname);
    size_t getLen();

protected:
    walmus(waldev *dev);
    ~walmus();

    virtual size_t _read(void *buf, size_t bufsz, int32_t *endTs);
    virtual void _rewind();

    OggVorbis_File m_vorbis;
    FSMgr::FileHandle *hndl;

    size_t len;
};


class WALStream: public CTsmpl
{
    friend waldev;
    
private:
    struct TSSamples
    {
        int32_t TS = -1;
        std::vector<uint8_t> Samples;
    };

public:
    void SetFormat(ALenum fmt, uint32_t freq);
    void Feed(void *data, size_t sz, int32_t ts = -1);
    
    int BuffersCount();
    size_t DataLeft();

protected:
    WALStream(waldev *dev);
    ~WALStream();

    virtual size_t _read(void *buf, size_t bufsz, int32_t *endTs);
    virtual void _rewind() {};
    virtual void _stop() override;
    
    std::queue< TSSamples >_queue;
    size_t _bufPos = 0;
    size_t _dataLeft = 0;
};


class waldev
{
public:
    waldev();
    ~waldev();

    bool inited();

    walsmpl *newSample();
    walmus *createMusicPlayer();
    void deleteSample(CTsmpl *smpl);

    void master_volume(int vol);
    
    WALStream *CreateStream();

private:
    ALCdevice *dev;
    ALCcontext *cont;
    bool _inited;
    bool ende;

    std::list<CTsmpl *> sample_list;

    bool update();

    static int _updateThread(void *data);

    SDL_mutex *mutex;
    SDL_Thread *updateThread;
};

#endif
