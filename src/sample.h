#ifndef SAMPLE_H_INCLUDED
#define SAMPLE_H_INCLUDED

#include "rsrc.h"
#include "system/sound.h"

class NC_STACK_sample: public NC_STACK_rsrc
{
public:
    virtual size_t Init(IDVList &stak);
    virtual rsrc * rsrc_func64(IDVList &stak);
    virtual size_t rsrc_func65(rsrc *pres);

    NC_STACK_sample() {};
    virtual ~NC_STACK_sample() {};
    
    virtual const std::string ClassName() const {
        return __ClassName;
    };

    enum SMPL_ATT
    {
        SMPL_ATT_PSAMPLE = 0x80002000,
        SMPL_ATT_TYPE = 0x80002001,
        SMPL_ATT_LEN = 0x80002002,
        SMPL_ATT_BUFFER = 0x80002003
    };

    virtual TSampleData *GetSampleData();
    virtual int getSMPL_type();
    virtual int getSMPL_len();
    virtual void *getSMPL_buffer();

    //Data
    static constexpr const char * __ClassName = "sample.class";

public:
    TSampleData * p_sampl = NULL;
};

#endif // SAMPLE_H_INCLUDED
