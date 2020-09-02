#include <inttypes.h>
#include "includes.h"
#include "nucleas.h"
#include "wav.h"
#include "utils.h"


const Nucleus::ClassDescr NC_STACK_wav::description("wav.class", &newinstance);

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

rsrc * wav_func64__sub0(NC_STACK_wav *obj, IDVList &stak, const std::string &filname)
{
    char buf[256];
    rsrc *res = NULL;

    strcpy(buf, "rsrc:");
    strcat(buf, filname.c_str());

    FSMgr::FileHandle *fil = uaOpenFile(buf, "rb");

    if ( fil )
    {
        RIFF_HDR rff;

        fil->read(&rff, sizeof(RIFF_HDR));

        rff.ChunkID = SWAP32(rff.ChunkID);
        rff.Format = SWAP32(rff.Format);

        if ( rff.ChunkID == TAG_RIFF && rff.Format == TAG_WAVE )
        {
            PCM_fmt fmt;

            while ( 1 )
            {
                RIFF_SUBCHUNK sbchunk;

                if ( fil->read(&sbchunk, sizeof(RIFF_SUBCHUNK)) != sizeof(RIFF_SUBCHUNK) )
                    break;

                sbchunk.SubchunkID = SWAP32(sbchunk.SubchunkID);

                if (sbchunk.SubchunkID == TAG_data)
                {
                    stak.Add(NC_STACK_sample::SMPL_ATT_LEN, (int32_t)sbchunk.SubchunkSize);
                    stak.Add(NC_STACK_sample::SMPL_ATT_TYPE, (int32_t)1);

                    res = obj->NC_STACK_sample::rsrc_func64(stak); //Create sampl structure and alloc buff

                    if ( res )
                    {

                        sampl *smpl = (sampl *)res->data;

                        if ( !smpl )
                        {
                            obj->rsrc_func65(res);
                            delete fil;
                            return NULL;
                        }

                        fil->read(smpl->sample_buffer, sbchunk.SubchunkSize);
                        smpl->SampleRate = fmt.SampleRate;
                    }
                }
                else if (sbchunk.SubchunkID == TAG_fmt)
                {
                    if (sbchunk.SubchunkSize >= sizeof(PCM_fmt))
                    {
                        fil->read(&fmt, sizeof(PCM_fmt));

                        if ( sbchunk.SubchunkSize > sizeof(PCM_fmt) )
                            fil->seek(sbchunk.SubchunkSize - sizeof(PCM_fmt), SEEK_CUR); // For some files this section bigger, so skip extra bytes
                    }
                    else
                        fil->seek(sbchunk.SubchunkSize, SEEK_CUR);
                }
                else
                {
                    fil->seek(sbchunk.SubchunkSize, SEEK_CUR);
                }
            }


        }
        else
        {
            ypa_log_out("wav.class: Not a wav file.\n");
        }

        delete fil;
    }
    return res;
}

rsrc * NC_STACK_wav::rsrc_func64(IDVList &stak)
{
    std::string filename = stak.Get<std::string>(RSRC_ATT_NAME, "");

    if ( !filename.empty() )
        return wav_func64__sub0(this, stak, filename);

    return NULL;
}

