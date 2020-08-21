#ifndef WAV_H_INCLUDED
#define WAV_H_INCLUDED

#include "sample.h"

struct NC_STACK_wav;

struct __NC_STACK_wav
{

};

class NC_STACK_wav: public NC_STACK_sample
{
public:
    virtual rsrc * rsrc_func64(IDVList &stak);

    NC_STACK_wav() {
        memset(&stack__wav, 0, sizeof(stack__wav));
    };
    virtual ~NC_STACK_wav() {};
    
    virtual const std::string &GetClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_wav();
    };

    //Data
    static const Nucleus::ClassDescr description;

    __NC_STACK_wav stack__wav;
};

#endif // WAV_H_INCLUDED
