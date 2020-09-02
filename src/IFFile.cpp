#include <stdio.h>
#include <stdlib.h>
#include "includes.h"
#include "IFFile.h"
#include "utils.h"


void IFFile::_Init()
{
    file_handle = NULL;
    flags = 0;
    depth = 0;
    ctxStack.clear();
    freeAfterUse = false;

    Context *ctx = new Context;

    if ( !ctx )
    {
        printf("Can't create IFFile root context\n");
        exit(1);
    }

    ctx->TAG = TAG_FORM;
    ctx->TAG_EXTENSION = TAG_NONE;
    ctx->TAG_SIZE = 0x80000000;
    ctx->position = 0;

    ctxStack.push_front(ctx);
}

IFFile::IFFile(FSMgr::FileHandle *handle, bool forWrite, bool _freeAfterUse)
{
    _Init();
    file_handle = handle;

    if (forWrite)
        flags |= IFF_FLAGS_WRITE;

    freeAfterUse = _freeAfterUse;
}

IFFile::~IFFile()
{
    for(CtxList::iterator it = ctxStack.begin(); it != ctxStack.end(); it++)
        delete *it;

    if (freeAfterUse)
        delete file_handle;
}

IFFile *IFFile::openIFFile(const std::string &filename, bool forWrite)
{
    std::string tmpBuf = "rsrc:";
    tmpBuf += filename;

    FSMgr::FileHandle *fil;

    if (forWrite)
        fil = uaOpenFile(tmpBuf, "wb");
    else
        fil = uaOpenFile(tmpBuf, "rb");

    if ( !fil )
        return NULL;

    IFFile *tmp = new IFFile(fil, forWrite, true);

    if (!tmp)
        delete fil;

    return tmp;
}

int IFFile::pushChunk(uint32_t TAG1, uint32_t TAG2, int32_t TAG_SZ)
{
    int32_t position = 0;

    if ( flags & IFF_FLAGS_WRITE )
    {
        Context *currentCtx = ctxStack.front();

        if ( currentCtx->TAG != TAG_FORM )
            return IFF_ERR_SYNTAX;

        if ( !file_handle->writeU32B(TAG2) )
            return IFF_ERR_WRITE;

        if ( !file_handle->writeU32B(TAG_SZ) )
            return IFF_ERR_WRITE;

        if ( TAG2 == TAG_FORM )
        {
            if ( !file_handle->writeU32B(TAG1) )
                return IFF_ERR_WRITE;

            position = 4;
        }
        else
        {
            TAG1 = currentCtx->TAG_EXTENSION;
        }
    }
    else                                          // READING
    {
        Context *currentCtx = ctxStack.front();

        if ( currentCtx->TAG != TAG_FORM )
            return IFF_ERR_EOC;

        if ( currentCtx->TAG_SIZE == currentCtx->position )
            return IFF_ERR_EOC;

        TAG2 = file_handle->readU32B();

        TAG_SZ = file_handle->readU32B();

        TAG1 = currentCtx->TAG_EXTENSION;

        if ( TAG2 == TAG_FORM )
        {
            TAG1 = file_handle->readU32B();

            position = 4;
        }
    }

    Context *ctx = new Context();

    if ( !ctx )
        return IFF_ERR_MEM;

    ctx->TAG = TAG2;
    ctx->TAG_EXTENSION = TAG1;
    ctx->TAG_SIZE = TAG_SZ;
    ctx->position = position;

    ctxStack.push_front(ctx);

    depth++;

    return IFF_ERR_OK;
}

int IFFile::popChunk()
{
    Context *ctx = ctxStack.front();
    int TAG_SZ = ctx->TAG_SIZE;

    if ( ctx->TAG == TAG_FORM && ctx->TAG_EXTENSION == TAG_NONE )
        return IFF_ERR_SYNTAX;

    if ( flags & IFF_FLAGS_WRITE )
    {
        if ( TAG_SZ == -1 ) // UNKNOWN SIZE
        {
            TAG_SZ = ctx->position;

            if ( file_handle->seek(-(ctx->position + 4), SEEK_CUR) ) // seek for write TAG SIZE
                return IFF_ERR_SEEK;

            if ( !file_handle->writeU32B(ctx->position) )
                return IFF_ERR_WRITE;

            if ( file_handle->seek(ctx->position, SEEK_CUR) )
                return IFF_ERR_SEEK;
        }

        if ( ctx->position < TAG_SZ ) // if we not in the end
            return IFF_ERR_CORRUPT;

        if ( TAG_SZ & 1 ) // pad 1 byte
        {
            if ( !file_handle->writeU8(0) )
                return -IFF_ERR_WRITE;

            TAG_SZ++;
        }

        ctxStack.pop_front();
        delete ctx;

        ctxStack.front()->position += TAG_SZ + 8;

        depth--;
    }
    else
    {
        if ( ctx->TAG != TAG_FORM )
        {
            if ( ctx->position < TAG_SZ && file_handle->seek(TAG_SZ -  ctx->position, SEEK_CUR) )
                return IFF_ERR_SEEK;

            if ( TAG_SZ & 1 ) // pad 1 byte
            {
                if ( file_handle->seek(1, SEEK_CUR) )
                    return IFF_ERR_SEEK;

                TAG_SZ++;
            }
        }

        ctxStack.pop_front();
        delete ctx;

        ctx = ctxStack.front();
        if ( ctx->TAG == TAG_FORM && ctx->TAG_EXTENSION == TAG_NONE )
            return IFF_ERR_EOF;

        ctx->position += TAG_SZ + 8;
        depth--;
    }

    return IFF_ERR_OK;
}

int IFFile::parse()
{
    int res = IFF_ERR_SYNTAX;

    if (flags & IFF_FLAGS_POP)
    {
        res = popChunk();
        flags &= ~IFF_FLAGS_POP;

        if (res != IFF_ERR_OK)
            return res;
    }

    res = pushChunk(TAG_NONE, TAG_NONE, -1);
    if ( res == IFF_ERR_EOC )
        flags |= IFF_FLAGS_POP;

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

int32_t IFFile::read(void *buf, int32_t sz)
{
    Context *ctx = getCurrentChunk();

    if ( !ctx )
        return IFF_ERR_SYNTAX;

    if ( ctx->TAG == TAG_FORM )
        return IFF_ERR_SYNTAX;

    if ( sz + ctx->position > ctx->TAG_SIZE )
    {
        if (ctx->TAG_SIZE == ctx->position)
            return 0;

        sz = ctx->TAG_SIZE - ctx->position;
    }

    ctx->position += sz;

    int32_t readed = file_handle->read(buf, sz);

    if ( readed != sz )
        return IFF_ERR_READ;

    return readed;
}

int32_t IFFile::write(const void *buf, int32_t sz)
{
    Context *ctx = getCurrentChunk();

    if ( !ctx )
        return IFF_ERR_SYNTAX;

    if ( ctx->TAG == TAG_FORM )
        return IFF_ERR_SYNTAX;

    if ( ctx->TAG_SIZE != -1 &&
            ctx->position + sz > ctx->TAG_SIZE)
    {
        if ( ctx->TAG_SIZE == ctx->position )
            return 0;

        sz = ctx->TAG_SIZE - ctx->position;
    }

    ctx->position += sz;
    int32_t writed = file_handle->write(buf, sz);

    if ( writed != sz )
        return IFF_ERR_WRITE;

    return writed;
}

IFFile::Context *IFFile::getCurrentChunk()
{
    Context *ctx = ctxStack.front();
    if ( ctx->TAG == TAG_FORM && ctx->TAG_EXTENSION == TAG_NONE )
        return NULL;
    return ctx;
}

bool IFFile::readU8(uint8_t &dst)
{
    return read(&dst, 1) == 1;
}

bool IFFile::readS8(int8_t &dst)
{
    return read(&dst, 1) == 1;
}

bool IFFile::readU16L(uint16_t &dst)
{
    int32_t sz = read(&dst, 2);

    if (sz != 2)
        return false;

#ifndef BYTEORDER_LITTLE
    dst = ((dst & 0xFF00) >> 8) | ((dst & 0xFF) << 8);
#endif // BYTEORDER_LITTLE

    return true;
}

bool IFFile::readS16L(int16_t &dst)
{
    int32_t sz = read(&dst, 2);

    if (sz != 2)
        return false;

#ifndef BYTEORDER_LITTLE
    dst = ((dst & 0xFF00) >> 8) | ((dst & 0xFF) << 8);
#endif // BYTEORDER_LITTLE

    return true;
}

bool IFFile::readU16B(uint16_t &dst)
{
    int32_t sz = read(&dst, 2);

    if (sz != 2)
        return false;

#ifdef BYTEORDER_LITTLE
    dst = ((dst & 0xFF00) >> 8) | ((dst & 0xFF) << 8);
#endif // BYTEORDER_LITTLE

    return true;
}

bool IFFile::readS16B(int16_t &dst)
{
    int32_t sz = read(&dst, 2);

    if (sz != 2)
        return false;

#ifdef BYTEORDER_LITTLE
    dst = ((dst & 0xFF00) >> 8) | ((dst & 0xFF) << 8);
#endif // BYTEORDER_LITTLE

    return true;
}

bool IFFile::readU32L(uint32_t &dst)
{
    int32_t sz = read(&dst, 4);

    if (sz != 4)
        return false;

#ifndef BYTEORDER_LITTLE
    dst = ((dst & 0xFF000000) >> 24) | ((dst & 0xFF0000) >> 8) | ((dst & 0xFF00) << 8) | ((dst & 0xFF) << 24);
#endif // BYTEORDER_LITTLE

    return true;
}

bool IFFile::readS32L(int32_t &dst)
{
    int32_t sz = read(&dst, 4);

    if (sz != 4)
        return false;

#ifndef BYTEORDER_LITTLE
    dst = ((dst & 0xFF000000) >> 24) | ((dst & 0xFF0000) >> 8) | ((dst & 0xFF00) << 8) | ((dst & 0xFF) << 24);
#endif // BYTEORDER_LITTLE

    return true;
}

bool IFFile::readU32B(uint32_t &dst)
{
    int32_t sz = read(&dst, 4);

    if (sz != 4)
        return false;

#ifdef BYTEORDER_LITTLE
    dst = ((dst & 0xFF000000) >> 24) | ((dst & 0xFF0000) >> 8) | ((dst & 0xFF00) << 8) | ((dst & 0xFF) << 24);
#endif // BYTEORDER_LITTLE

    return true;
}

bool IFFile::readS32B(int32_t &dst)
{
    int32_t sz = read(&dst, 4);

    if (sz != 4)
        return false;

#ifdef BYTEORDER_LITTLE
    dst = ((dst & 0xFF000000) >> 24) | ((dst & 0xFF0000) >> 8) | ((dst & 0xFF00) << 8) | ((dst & 0xFF) << 24);
#endif // BYTEORDER_LITTLE

    return true;
}

bool IFFile::readFloatL(float &dst)
{
    int32_t sz = read(&dst, 4);

    if (sz != 4)
        return false;

#ifndef BYTEORDER_LITTLE
    uint32_t *p = (uint32_t *)&dst;
    *p = ((*p & 0xFF000000) >> 24) | ((*p & 0xFF0000) >> 8) | ((*p & 0xFF00) << 8) | ((*p & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return true;
}

bool IFFile::readFloatB(float &dst)
{
    int32_t sz = read(&dst, 4);

    if (sz != 4)
        return false;

#ifdef BYTEORDER_LITTLE
    uint32_t *p = (uint32_t *)&dst;
    *p = ((*p & 0xFF000000) >> 24) | ((*p & 0xFF0000) >> 8) | ((*p & 0xFF00) << 8) | ((*p & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return true;
}

bool IFFile::readFloatL(double &dst)
{
    float tmp;
    if (!readFloatL(tmp))
        return false;

    dst = tmp;
    return true;
}

bool IFFile::readFloatB(double &dst)
{
    float tmp;
    if (!readFloatB(tmp))
        return false;

    dst = tmp;
    return true;
}

std::string IFFile::readStr(int maxSz)
{
    char *bf = new char[maxSz];
    std::string tmp(bf, read(bf, maxSz));
    delete[] bf;
    return tmp;
}

bool IFFile::writeU8(uint8_t val)
{
    return write(&val, 1) == 1;
}

bool IFFile::writeS8(int8_t val)
{
    return write(&val, 1) == 1;
}

bool IFFile::writeU16L(uint16_t val)
{
#ifndef BYTEORDER_LITTLE
    val = ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#endif // BYTEORDER_LITTLE
    return write(&val, 2) == 2;
}

bool IFFile::writeS16L(int16_t val)
{
#ifndef BYTEORDER_LITTLE
    val = ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#endif // BYTEORDER_LITTLE
    return write(&val, 2) == 2;
}

bool IFFile::writeU16B(uint16_t val)
{
#ifdef BYTEORDER_LITTLE
    val = ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#endif // BYTEORDER_LITTLE
    return write(&val, 2) == 2;
}

bool IFFile::writeS16B(int16_t val)
{
#ifdef BYTEORDER_LITTLE
    val = ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#endif // BYTEORDER_LITTLE
    return write(&val, 2) == 2;
}

bool IFFile::writeU32L(uint32_t val)
{
#ifndef BYTEORDER_LITTLE
    val = ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}

bool IFFile::writeS32L(int32_t val)
{
#ifndef BYTEORDER_LITTLE
    val = ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}

bool IFFile::writeU32B(uint32_t val)
{
#ifdef BYTEORDER_LITTLE
    val = ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}

bool IFFile::writeS32B(int32_t val)
{
#ifdef BYTEORDER_LITTLE
    val = ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}

bool IFFile::writeFloatL(float val)
{

#ifndef BYTEORDER_LITTLE
    uint32_t *p = (uint32_t *)&val;
    *p = ((*p & 0xFF000000) >> 24) | ((*p & 0xFF0000) >> 8) | ((*p & 0xFF00) << 8) | ((*p & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}

bool IFFile::writeFloatB(float val)
{
#ifdef BYTEORDER_LITTLE
    uint32_t *p = (uint32_t *)&val;
    *p = ((*p & 0xFF000000) >> 24) | ((*p & 0xFF0000) >> 8) | ((*p & 0xFF00) << 8) | ((*p & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}
