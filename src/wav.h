#ifndef WAV_H_INCLUDED
#define WAV_H_INCLUDED

#include "sample.h"

class NC_STACK_wav: public NC_STACK_sample
{
public:
    virtual rsrc * rsrc_func64(IDVList &stak);

    NC_STACK_wav() {
    };
    virtual ~NC_STACK_wav() {};
    
    virtual const std::string ClassName() const {
        return __ClassName;
    };

    //Data
    static constexpr const char * __ClassName = "wav.class";
};

#endif // WAV_H_INCLUDED
