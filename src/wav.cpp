#include <inttypes.h>
#include "includes.h"
#include "nucleas.h"
#include "wav.h"
#include "utils.h"


stored_functions *classvtbl_get_wav();
class_return * class_set_wav(int, ...);

stored_functions wav_class_vtbl(class_set_wav);


class_stored wav_class_off (NULL, NULL, "MC2classes:wav.class", classvtbl_get_wav);


stored_functions *classvtbl_get_wav()
{
    return &wav_class_vtbl;
}

CLASSFUNC wav_funcs[1024];

struct __attribute__((packed)) RIFF_HDR
{
    uint32_t ChunkID;
    uint32_t ChunkSize;
    uint32_t Format;
};

struct __attribute__((packed)) RIFF_SUBCHUNK
{
    uint32_t SubchunkID;
    uint32_t SubchunkSize;
};

struct __attribute__((packed)) PCM_fmt
{
    uint16_t AudioFormat;
    uint16_t NumChannels;
    uint32_t SampleRate;
    uint32_t ByteRate;
    uint16_t BlockAlign;
    uint16_t BitsPerSample;
};

rsrc * wav_func64__sub0(NC_STACK_wav *obj, class_stru *zis, stack_vals *stak, const char *filname)
{
    char buf[256];
    rsrc *res = NULL;

    const char *restp = get_MC_str(MC_TYPE_RES);

    strcpy(buf, restp);
    strcat(buf, filname);

    FILE *fil = FOpen(buf, "rb");

    if ( fil )
    {
        RIFF_HDR rff;

        fread(&rff, sizeof(RIFF_HDR), 1, fil);

        rff.ChunkID = SWAP32(rff.ChunkID);
        rff.Format = SWAP32(rff.Format);

        if ( rff.ChunkID == TAG_RIFF && rff.Format == TAG_WAVE )
        {
            PCM_fmt fmt;

            while ( 1 )
            {
                RIFF_SUBCHUNK sbchunk;

                if ( fread(&sbchunk, sizeof(RIFF_SUBCHUNK), 1, fil) != 1 )
                    break;

                sbchunk.SubchunkID = SWAP32(sbchunk.SubchunkID);

                if (sbchunk.SubchunkID == TAG_data)
                {
                    stack_vals stk[3];

                    stk[0].id = 0x80002002;
                    stk[0].value = sbchunk.SubchunkSize;
                    stk[1].id = 0x80002001;
                    stk[1].value = 1;
                    stk[2].id = 2;
                    stk[2].value = (size_t)stak;

                    res = (rsrc *)call_parent(zis, obj, 64, stk); //Create sampl structure and alloc buff

                    if ( res )
                    {

                        sampl *smpl = (sampl *)res->data;

                        if ( !smpl )
                        {
                            //call_vtbl(obj, 65, res);
                            call_method(obj, 65, &res);
                            FClose(fil);
                            return NULL;
                        }

                        fread(smpl->sample_buffer, sbchunk.SubchunkSize, 1, fil);
                        smpl->SampleRate = fmt.SampleRate;
                    }
                }
                else if (sbchunk.SubchunkID == TAG_fmt)
                {
                    if (sbchunk.SubchunkSize == sizeof(PCM_fmt))
                        fread(&fmt, sizeof(PCM_fmt), 1, fil);
                    else
                        fseek(fil, sbchunk.SubchunkSize, SEEK_CUR);
                }
                else
                {
                    fseek(fil, sbchunk.SubchunkSize, SEEK_CUR);
                }
            }


        }
        else
        {
            ypa_log_out("wav.class: Not a wav file.\n");
        }

        FClose(fil);
    }
    return res;
}

rsrc *wav_func64(NC_STACK_wav *obj, class_stru *zis, stack_vals *stak)
{
    const char *filename = (const char *)find_id_in_stack_def_val(0x80001000, 0, stak);

    if ( filename )
        return wav_func64__sub0(obj, zis, stak, filename);

    return NULL;
}

class_return wav_class_descr;

class_return * class_set_wav(int , ...)
{

    memset(wav_funcs, 0, sizeof(CLASSFUNC) * 1024);

    wav_funcs[64] = (CLASSFUNC)wav_func64;

    wav_class_descr.parent = "sample.class";

    wav_class_descr.vtbl = wav_funcs;
    ////wav_class_descr.varSize = sizeof(__NC_STACK_wav);
    wav_class_descr.varSize = sizeof(NC_STACK_wav) - offsetof(NC_STACK_wav, stack__wav); //// HACK
    wav_class_descr.field_A = 0;
    return &wav_class_descr;
}
