#include <stdio.h>
#include <stdlib.h>
#include "includes.h"
#include "IFFile.h"
#include "utils.h"
#include "env.h"



IFFile::IFFile(const std::string &diskPath, const std::string &mode)
: file_handle(diskPath, mode)
{
    ctxStack.emplace_front(TAG_FORM, TAG_NONE, 0x80000000, 0);
}


IFFile::IFFile(FSMgr::FileHandle *f, bool del)
: file_handle(f, false)
{
    ctxStack.emplace_front(TAG_FORM, TAG_NONE, 0x80000000, 0);
    
    if (f && del)
        delete f;
}

IFFile::IFFile(FSMgr::FileHandle &f)
: file_handle(&f, false)
{
    ctxStack.emplace_front(TAG_FORM, TAG_NONE, 0x80000000, 0);
}

IFFile::IFFile(FSMgr::FileHandle &&f)
: file_handle( std::move(f) )
{
    ctxStack.emplace_front(TAG_FORM, TAG_NONE, 0x80000000, 0);
}

IFFile *IFFile::RsrcOpenIFFile(const std::string &filename, const std::string &mode)
{
    std::string tmpBuf = "rsrc:";
    tmpBuf += filename;
    
    tmpBuf = correctSeparatorAndExt( Common::Env.ApplyPrefix( tmpBuf ) );

    if ( !FSMgr::iDir::fileExist(tmpBuf) )
        return NULL;
    
    FSMgr::FileHandle f = FSMgr::iDir::openFile(tmpBuf, mode);
    return new IFFile(f);
}

IFFile *IFFile::UAOpenIFFileAlloc(const std::string &filename, const std::string &mode)
{
    std::string tmpBuf = correctSeparatorAndExt( Common::Env.ApplyPrefix( filename ) );

    if ( !FSMgr::iDir::fileExist(tmpBuf) )
        return NULL;
    
    return new IFFile(FSMgr::iDir::openFileAlloc(tmpBuf, mode));
}

IFFile IFFile::UAOpenIFFile(const std::string &filename, const std::string &mode)
{
    std::string tmpBuf = correctSeparatorAndExt( Common::Env.ApplyPrefix( filename ) );

    if ( !FSMgr::iDir::fileExist(tmpBuf) )
        return IFFile();
    
    return IFFile(FSMgr::iDir::openFileAlloc(tmpBuf, mode));
}

int IFFile::pushChunk(uint32_t TAG1, uint32_t TAG2, int32_t TAG_SZ)
{
    int32_t position = 0;

    if ( file_handle.IsWriting() )
    {
        Context &currentCtx = ctxStack.front();

        if ( currentCtx.TAG != TAG_FORM )
            return IFF_ERR_SYNTAX;

        if ( !file_handle.writeU32B(TAG2) )
            return IFF_ERR_WRITE;

        if ( !file_handle.writeU32B(TAG_SZ) )
            return IFF_ERR_WRITE;

        if ( TAG2 == TAG_FORM )
        {
            if ( !file_handle.writeU32B(TAG1) )
                return IFF_ERR_WRITE;

            position = 4;
        }
        else
        {
            TAG1 = currentCtx.TAG_EXTENSION;
        }
    }
    else                                          // READING
    {
        Context &currentCtx = ctxStack.front();

        if ( currentCtx.TAG != TAG_FORM )
            return IFF_ERR_EOC;

        if ( currentCtx.TAG_SIZE == currentCtx.position )
            return IFF_ERR_EOC;

        TAG2 = file_handle.readU32B();

        TAG_SZ = file_handle.readU32B();

        TAG1 = currentCtx.TAG_EXTENSION;

        if ( TAG2 == TAG_FORM )
        {
            TAG1 = file_handle.readU32B();

            position = 4;
        }
    }

    ctxStack.emplace_front(TAG2, TAG1, TAG_SZ, position);

    depth++;

    return IFF_ERR_OK;
}

int IFFile::popChunk()
{
    Context &ctx = ctxStack.front();
    int TAG_SZ = ctx.TAG_SIZE;

    if ( ctx.TAG == TAG_FORM && ctx.TAG_EXTENSION == TAG_NONE )
        return IFF_ERR_SYNTAX;

    if ( file_handle.IsWriting() )
    {
        if ( TAG_SZ == -1 ) // UNKNOWN SIZE
        {
            TAG_SZ = ctx.position;

            if ( file_handle.seek(-(ctx.position + 4), SEEK_CUR) ) // seek for write TAG SIZE
                return IFF_ERR_SEEK;

            if ( !file_handle.writeU32B(ctx.position) )
                return IFF_ERR_WRITE;

            if ( file_handle.seek(ctx.position, SEEK_CUR) )
                return IFF_ERR_SEEK;
        }

        if ( ctx.position < TAG_SZ ) // if we not in the end
            return IFF_ERR_CORRUPT;

        if ( TAG_SZ & 1 ) // pad 1 byte
        {
            if ( !file_handle.writeU8(0) )
                return -IFF_ERR_WRITE;

            TAG_SZ++;
        }

        ctxStack.pop_front();

        ctxStack.front().position += TAG_SZ + 8;

        depth--;
    }
    else
    {
        if ( ctx.TAG != TAG_FORM )
        {
            if ( ctx.position < TAG_SZ && file_handle.seek(TAG_SZ -  ctx.position, SEEK_CUR) )
                return IFF_ERR_SEEK;

            if ( TAG_SZ & 1 ) // pad 1 byte
            {
                if ( file_handle.seek(1, SEEK_CUR) )
                    return IFF_ERR_SEEK;

                TAG_SZ++;
            }
        }

        ctxStack.pop_front();

        Context &ctx2 = ctxStack.front();
        if ( ctx2.TAG == TAG_FORM && ctx2.TAG_EXTENSION == TAG_NONE )
            return IFF_ERR_EOF;

        ctx2.position += TAG_SZ + 8;
        depth--;
    }

    return IFF_ERR_OK;
}

int IFFile::parse()
{
    int res = IFF_ERR_SYNTAX;

    if (_flagPop)
    {
        res = popChunk();
        _flagPop = false;

        if (res != IFF_ERR_OK)
            return res;
    }

    res = pushChunk(TAG_NONE, TAG_NONE, -1);
    if ( res == IFF_ERR_EOC )
        _flagPop = true;

    return res;
}

bool IFFile::skipChunk()
{
    while ( 1 )
    {
        int res = parse();

        if (res == IFF_ERR_EOC)
            break;
        else if ( res )
            return false;

        if ( !skipChunk() )
            return false;
    }

    return true;
}

size_t IFFile::read(void *buf, size_t sz)
{
    Context &ctx = ctxStack.front();

    if ( ctx.Is(TAG_FORM, TAG_NONE) )
        return IFF_ERR_SYNTAX;

    if ( ctx.TAG == TAG_FORM )
        return IFF_ERR_SYNTAX;

    if ( sz + ctx.position > (size_t)ctx.TAG_SIZE )
    {
        if (ctx.TAG_SIZE == ctx.position)
            return 0;

        sz = ctx.TAG_SIZE - ctx.position;
    }

    ctx.position += sz;

    size_t readed = file_handle.read(buf, sz);

    if ( readed != sz )
        return IFF_ERR_READ;

    return readed;
}

size_t IFFile::write(const void *buf, size_t sz)
{
    Context &ctx = ctxStack.front();

    if ( ctx.Is(TAG_FORM, TAG_NONE) )
        return IFF_ERR_SYNTAX;

    if ( ctx.TAG == TAG_FORM )
        return IFF_ERR_SYNTAX;

    if ( ctx.TAG_SIZE != -1 &&
            ctx.position + sz > (size_t)ctx.TAG_SIZE)
    {
        if ( ctx.TAG_SIZE == ctx.position )
            return 0;

        sz = ctx.TAG_SIZE - ctx.position;
    }

    ctx.position += sz;
    size_t writed = file_handle.write(buf, sz);

    if ( writed != sz )
        return IFF_ERR_WRITE;

    return writed;
}

const IFFile::Context &IFFile::GetCurrentChunk()
{
    return ctxStack.front();
}

std::string IFFile::readStr(int maxSz)
{
    char *bf = new char[maxSz]();
    std::string tmp(bf, read(bf, maxSz));
    delete[] bf;
    return tmp;
}


bool IFFile::eof() const
{
    return file_handle.eof();
}

bool IFFile::OK() const
{
    return file_handle.OK();
}

size_t IFFile::tell() const
{
    return file_handle.tell();
}

int IFFile::seek(long int offset, int origin)
{
    return 0;
}

void IFFile::close()
{
    file_handle.close();
}