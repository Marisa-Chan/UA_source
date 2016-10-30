#ifndef WRAP_AL_H_INCLUDED
#define WRAP_AL_H_INCLUDED

#include <list>
#include <deque>
#include <AL/al.h>
#include <AL/alc.h>

#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_mutex.h>

#define OV_EXCLUDE_STATIC_CALLBACKS
#include <vorbis/vorbisfile.h>

#include "fsmgr.h"

class waldev;

class CTsmpl
{
    friend class waldev;

public:
    virtual void update();

    virtual void volume(int vol);
    virtual void setMasterVolume(int vol);

    void EOS_callback( void (*func)(void *) );

protected:
    CTsmpl(waldev *dev);
    virtual ~CTsmpl();
    virtual int fill_n_queue(int bufID) = 0;

    void clearQueue();

    waldev *device;

    ALuint source;
    std::deque<ALuint> buffers;
    std::deque<bool> used;

    ALenum format;
    int freq;

    SDL_mutex *mutex;
    int status;
    bool endStreamed;

    ALfloat cVolume;
    ALfloat mVolume;

    void (*eosfunc)(void *);
};

class walsmpl: public CTsmpl
{
    friend waldev;

public:
    void init();
    void address(void *start, size_t size, int freq, ALenum format);
    void play();
    void stop();
    void update();

    void loop_count(int loops);

    void pan(int pan);
    void playback_rate(int newfreq);
    void position(size_t pos);

protected:
    walsmpl(waldev *dev);

    int fill_n_queue(int bufID);

    bool isqueued();

    int loops;

    void *start;
    size_t len;
    size_t pos;
};



class walmus: public CTsmpl
{
    friend waldev;

public:
    void init();

    bool open(const char *fname);
    void close();

//    int isPlaying();
    void play();
//    void pause();
    void stop();
    size_t getLen();
//    void update();

protected:
    walmus(waldev *dev);
    ~walmus();

    OggVorbis_File m_vorbis;
    FSMgr::FileHandle *hndl;

    int fill_n_queue(int bufID);

    std::deque<int16_t *> smplBuffers;

    size_t len;
};


class waldev
{
public:
    waldev();
    ~waldev();

    bool inited();

    walsmpl *newSample();
    void deleteSample(CTsmpl *smpl);

    void master_volume(int vol);

    walmus *createMusicPlayer();

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
