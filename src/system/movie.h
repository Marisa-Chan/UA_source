#ifndef ENGINE_MOVIE_H_INCLUDED
#define ENGINE_MOVIE_H_INCLUDED

#include <string>
#include <SDL2/SDL.h>

namespace System
{

class TMovie
{
private:
struct Context;

public:
    ~TMovie();
    
    void Init();
    void PlayMovie(const std::string &fname);

private:
    TMovie();
    
    bool OpenFile(const std::string &fname);
    void Close();
    
    void ReadFrames();
    
    void ProcessFrame(uint32_t tm);
    void ProcessAudio();
    
    static int EventsWatcher(void *, SDL_Event *event);
    
public:
    static TMovie Instance;
private:
    Context *_ctx = NULL;
};  

static constexpr TMovie &Movie = TMovie::Instance;
    
}

#endif