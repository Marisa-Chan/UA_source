#ifndef SAMPLE_H_INCLUDED
#define SAMPLE_H_INCLUDED

#include "rsrc.h"

struct sampl
{
    void *sample_buffer;
    int bufsz;
    int field_8;
    int SampleRate;
    int field_10;
};

struct __NC_STACK_sample
{
    sampl * p_sampl;
};

class NC_STACK_sample: public NC_STACK_rsrc
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
    virtual rsrc * rsrc_func64(stack_vals *stak);
    virtual size_t rsrc_func65(rsrc *pres);
    virtual void * sample_func128(void **arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_sample() {
        memset(&stack__sample, 0, sizeof(stack__sample));
    };
    virtual ~NC_STACK_sample() {};

    virtual const char * getClassName() {
        return "sample.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_sample();
    };

    enum SMPL_ATT
    {
        SMPL_ATT_PSAMPLE = 0x80002000,
        SMPL_ATT_TYPE = 0x80002001,
        SMPL_ATT_LEN = 0x80002002,
        SMPL_ATT_BUFFER = 0x80002003
    };

    virtual sampl *getSMPL_pSample();
    virtual int getSMPL_type();
    virtual int getSMPL_len();
    virtual void *getSMPL_buffer();

    //Data
    static const NewClassDescr description;

    __NC_STACK_sample stack__sample;
};

#endif // SAMPLE_H_INCLUDED
