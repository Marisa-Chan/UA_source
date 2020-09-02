#ifndef IFFile_H_INCLUDED
#define IFFile_H_INCLUDED

#include <list>
#include "fsmgr.h"

class IFFile
{
public:
    struct Context
    {
        uint32_t TAG;
        uint32_t TAG_EXTENSION;
        int32_t TAG_SIZE;
        int32_t position;
    };

    typedef std::list<Context *> CtxList;

    enum IFF_ERR
    {
        IFF_ERR_OK    = 0, // OK
        IFF_ERR_EOF   = -1, // End of file
        IFF_ERR_EOC   = -2, // End of context(TAG)
        IFF_ERR_MEM   = -4, // Memory allocation failed
        IFF_ERR_READ  = -5, // Read error
        IFF_ERR_WRITE = -6, // Write error
        IFF_ERR_SEEK  = -7, // Seek error
        IFF_ERR_CORRUPT = -8, // Data corrupted
        IFF_ERR_SYNTAX  = -9
    };

    enum IFF_FLAGS
    {
        IFF_FLAGS_WRITE   = 1,
        IFF_FLAGS_POP     = 2
    };

    IFFile(FSMgr::FileHandle *handle, bool forWrite = false, bool freeAfterUse = false);
    ~IFFile();

    int pushChunk(uint32_t TAG1, uint32_t TAG2, int32_t TAG_SZ);
    int popChunk();
    int parse();
    bool skipChunk();
    int32_t read(void *buf, int32_t sz);
    int32_t write(const void *buf, int32_t sz);
    IFFile::Context *getCurrentChunk();

    bool readU8(uint8_t &dst);
    bool readS8(int8_t &dst);
    bool readU16L(uint16_t &dst);
    bool readS16L(int16_t &dst);
    bool readU16B(uint16_t &dst);
    bool readS16B(int16_t &dst);
    bool readU32L(uint32_t &dst);
    bool readS32L(int32_t &dst);
    bool readU32B(uint32_t &dst);
    bool readS32B(int32_t &dst);
    bool readFloatL(float &dst);
    bool readFloatB(float &dst);
    bool readFloatL(double &dst);
    bool readFloatB(double &dst);
    std::string readStr(int maxSz);

    bool writeU8(uint8_t val);
    bool writeS8(int8_t val);
    bool writeU16L(uint16_t val);
    bool writeS16L(int16_t val);
    bool writeU16B(uint16_t val);
    bool writeS16B(int16_t val);
    bool writeU32L(uint32_t val);
    bool writeS32L(int32_t val);
    bool writeU32B(uint32_t val);
    bool writeS32B(int32_t val);
    bool writeFloatL(float val);
    bool writeFloatB(float val);

    static IFFile *openIFFile(const std::string &filename, bool forWrite);

private:
    void _Init();

protected:
    FSMgr::FileHandle *file_handle;
    uint32_t flags;
    int depth;
    CtxList ctxStack;
    bool freeAfterUse;
};

#endif // MFILE_H_INCLUDED
