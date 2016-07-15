#include <inttypes.h>
#include "includes.h"
#include "nucleas.h"
#include "wav.h"
#include "utils.h"


const NewClassDescr NC_STACK_wav::description("wav.class", &newinstance);

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

rsrc * wav_func64__sub0(NC_STACK_wav *obj, stack_vals *stak, const char *filname)
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

                    stk[0].id = NC_STACK_sample::SMPL_ATT_LEN;
                    stk[0].value = sbchunk.SubchunkSize;
                    stk[1].id = NC_STACK_sample::SMPL_ATT_TYPE;
                    stk[1].value = 1;
                    stk[2].id = 2;
                    stk[2].value = (size_t)stak;

                    res = obj->NC_STACK_sample::rsrc_func64(stk); //Create sampl structure and alloc buff

                    if ( res )
                    {

                        sampl *smpl = (sampl *)res->data;

                        if ( !smpl )
                        {
                            obj->rsrc_func65(&res);
                            FClose(fil);
                            return NULL;
                        }

                        fread(smpl->sample_buffer, sbchunk.SubchunkSize, 1, fil);
                        smpl->SampleRate = fmt.SampleRate;
                    }
                }
                else if (sbchunk.SubchunkID == TAG_fmt)
                {
                    if (sbchunk.SubchunkSize >= sizeof(PCM_fmt))
                    {
                        fread(&fmt, sizeof(PCM_fmt), 1, fil);

                        if ( sbchunk.SubchunkSize > sizeof(PCM_fmt) )
                            fseek(fil, sbchunk.SubchunkSize - sizeof(PCM_fmt), SEEK_CUR); // For some files this section bigger, so skip extra bytes
                    }
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

rsrc * NC_STACK_wav::rsrc_func64(stack_vals *stak)
{
    const char *filename = (const char *)find_id_in_stack_def_val(RSRC_ATT_NAME, 0, stak);

    if ( filename )
        return wav_func64__sub0(this, stak, filename);

    return NULL;
}

size_t NC_STACK_wav::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 64:
        return (size_t)rsrc_func64( (stack_vals *)data );
    default:
        break;
    }
    return NC_STACK_sample::compatcall(method_id, data);
}
