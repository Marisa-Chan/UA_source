#ifndef IFFile_H_INCLUDED
#define IFFile_H_INCLUDED

#include <deque>
#include "system/fsmgr.h"

class IFFile : public FSMgr::iFileHandle
{
public:
    struct Context
    {
        uint32_t TAG = 0;
        uint32_t TAG_EXTENSION = 0;
        int32_t TAG_SIZE = 0;
        int32_t position = 0;
        
        Context() = default;
        Context(uint32_t tag, uint32_t tag_ext, int32_t tag_sz, int32_t pos) 
        : TAG(tag), TAG_EXTENSION(tag_ext), TAG_SIZE(tag_sz), position(pos) 
        {}
        
        bool Is(uint32_t tag) const
        {
            return TAG == tag;
        }
        
        bool Is(uint32_t tag, uint32_t ext) const
        {
            return TAG == tag && TAG_EXTENSION == ext;
        }

    };

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
        IFF_FLAGS_POP     = 2
    };
    
    IFFile() = default;

    IFFile(const std::string &diskPath, const std::string &mode);
    virtual ~IFFile() = default;
    
    IFFile(IFFile &&) = default;
    IFFile& operator=(IFFile &&) = default;
    
    IFFile(FSMgr::FileHandle *, bool del = true);
    IFFile(FSMgr::FileHandle &);
    IFFile(FSMgr::FileHandle &&);
    
    IFFile(const IFFile&) = delete;
    IFFile& operator=(const IFFile &) = delete;
    
    
    
    virtual void close() override;
    

    int pushChunk(uint32_t TAG1, uint32_t TAG2, int32_t TAG_SZ);
    int popChunk();
    int parse();
    bool skipChunk();
    
    size_t read(void *buf, size_t sz) override;
    size_t write(const void *buf, size_t sz) override;
    const IFFile::Context &GetCurrentChunk(); 
    
    virtual bool eof() const override;
    virtual bool OK() const override;
    virtual size_t tell() const override;
    virtual int seek(long int offset, int origin) override;

    std::string readStr(int maxSz);

    static IFFile *RsrcOpenIFFile(const std::string &filename, const std::string &mode);
    static IFFile UAOpenIFFile(const std::string &filename, const std::string &mode);

protected:
    FSMgr::FileHandle file_handle;
    bool _flagPop = false;
    int depth = 0;
    std::deque<Context> ctxStack;
};

#endif // MFILE_H_INCLUDED
