#ifndef WRAP_AL_H_INCLUDED
#define WRAP_AL_H_INCLUDED

#include <list>
#include <deque>
#include <AL/al.h>
#include <AL/alc.h>

#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_mutex.h>

class waldev;

class walsmpl
{
    friend waldev;

public:
    void init();
    void address(void *start, size_t size, int freq, ALenum format);
    void play();
    void stop();
    void update();

    void loop_count(int loops);

    void volume(int vol);
    void pan(int pan);
    void playback_rate(int newfreq);
    void position(size_t pos);

    void EOS_callback( void (*func)(walsmpl *) );

private:
    walsmpl(waldev *dev);
    ~walsmpl();

    int fill_n_queue(int bufID);

    void clearQueue();

    bool isqueued();

    int status;



    waldev *device;

    ALuint source;
    std::deque<ALuint> buffers;
    std::deque<bool> used;
    ALint prev_state;

    int loops;

    SDL_mutex *mutex;

    void (*eosfunc)(walsmpl *);

    void *start;
    size_t len;
    size_t pos;

    bool endStreamed;

    ALenum format;
    int freq;
};





class waldev
{
public:
    waldev();
    ~waldev();

    bool inited();

    walsmpl *newSample();
    void deleteSample(walsmpl *smpl);

    void master_volume(int vol);

private:
    ALCdevice *dev;
    ALCcontext *cont;
    bool _inited;
    bool ende;

    std::list<walsmpl *> sample_list;

    bool update();

    static int _updateThread(void *data);

    SDL_mutex *mutex;
    SDL_Thread *updateThread;
};

#endif
