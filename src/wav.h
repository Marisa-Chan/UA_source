#ifndef WAV_H_INCLUDED
#define WAV_H_INCLUDED

#include "sample.h"

extern class_stored wav_class_off;

struct NC_STACK_wav;

struct __NC_STACK_wav
{

};

struct NC_STACK_wav : public NC_STACK_sample
{
    __NC_STACK_wav stack__wav;
};

#endif // WAV_H_INCLUDED
