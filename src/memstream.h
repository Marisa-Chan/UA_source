#ifndef MEBLOCK_H_INCLUDED
#define MEBLOCK_H_INCLUDED

#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <deque>
#include <vector>

namespace Common
{

typedef std::vector<uint8_t> ByteArray;

inline void Append(ByteArray* a, const ByteArray& b)
{
    if (b.size())
        a->insert(a->end(), b.begin(), b.end());
}

inline void AppendByteUL16(ByteArray* a, uint16_t v)
{
    a->push_back(v & 0xFF);
    a->push_back((v >> 8) & 0xFF);
}

inline void AppendByteUL32(ByteArray* a, uint32_t v)
{
    a->push_back(v & 0xFF);
    a->push_back((v >> 8) & 0xFF);
    a->push_back((v >> 16) & 0xFF);
    a->push_back((v >> 24) & 0xFF);
}

inline ByteArray ByteUL16(uint16_t v)
{
    ByteArray tmp(2);
    tmp[0] = v & 0xFF;
    tmp[1] = (v >> 8) & 0xFF;
    return tmp;
}

inline ByteArray ByteUL32(uint32_t v)
{
    ByteArray tmp(4);
    tmp[0] = v & 0xFF;
    tmp[1] = (v >> 8) & 0xFF;
    tmp[2] = (v >> 16) & 0xFF;
    tmp[3] = (v >> 24) & 0xFF;
    return tmp;
}


class MeBlock
{
protected:
    uint8_t       *_mem;
    size_t        _endPos;
    bool          _finish;  // if block finished - endpos will be fixed
    size_t        _pos;
    
public:
    const size_t  _reservedSize; // blocksize
    
public:
    MeBlock(size_t sz = 0x400) 
    : _reservedSize(sz)
    {
        _mem = new uint8_t[sz];
        
        _finish = false;
        _pos = 0;
        _endPos = 0;
    }
    
    ~MeBlock()
    {
        if (_mem)
            delete[] _mem;
    }
    
    size_t GetUnusedSize()
    {
        return _reservedSize - _endPos;
    }
    
    size_t GetSize()
    {
        return _endPos;
    }
    
    void *Alloc(size_t sz)
    {
        if (_finish)
            return NULL;
        
        if (_reservedSize - _endPos < sz)
            return NULL;
        
        void *tmp = _mem + _endPos;
        _endPos += sz;
        return tmp;
    }
    
    /***
     * Return number writed bytes
     ***/
    size_t Write(const void *data, size_t sz)
    {
        size_t e = _reservedSize;
        
        if (_finish)
            e = _endPos;
        
        if (_pos < 0 || _pos >= e)
            return 0;
        
        size_t maxWrite = e - _pos;
        
        if (maxWrite > sz)
            maxWrite = sz; //Can write all
        
        memcpy(_mem + _pos, data, maxWrite);
        
        _pos += maxWrite;
        if (!_finish && _endPos < _pos)
            _endPos = _pos;
        
        return maxWrite;
    }
    
    size_t Write(const ByteArray &data)
    {
        size_t e = _reservedSize;
        
        if (_finish)
            e = _endPos;
        
        if (_pos < 0 || _pos >= e)
            return 0;
        
        size_t maxWrite = e - _pos;
        
        if (maxWrite > data.size())
            maxWrite = data.size(); //Can write all
        
        memcpy(_mem + _pos, data.data(), maxWrite);
        
        _pos += maxWrite;
        if (!_finish && _endPos < _pos)
            _endPos = _pos;
        
        return maxWrite;
    }
    
    size_t Read(size_t pos, void *dst, size_t sz)
    {
        size_t e = _reservedSize;
        
        if (_finish)
            e = _endPos;
        
        if (pos < 0 || pos >= e)
            return 0;
        
        size_t maxRead = e - pos;
        
        if (maxRead > sz)
            maxRead = sz; //Can read all
        
        memcpy(dst, _mem + pos, maxRead);
        
        return maxRead;
    }
    
    ByteArray Read(size_t pos, size_t sz)
    {
        size_t e = _reservedSize;
        
        if (_finish)
            e = _endPos;
        
        if (pos < 0 || pos >= e)
            return ByteArray();
        
        size_t maxRead = e - pos;
        
        if (maxRead > sz)
            maxRead = sz; //Can read all
        
        return ByteArray(_mem + pos, _mem + pos + maxRead);
    }
    
    bool SetWritePos(size_t pos)
    {       
        if (pos < 0 || pos > _endPos)
            return false;
        
        _pos = pos;
        return true;
    }
    
    void Lock()
    {
        _finish = true;
    }
    
    bool IsLocked()
    {
        return _finish;
    }
    
    void Clean()
    {
        _finish = false;
        _pos = 0;
        _endPos = 0;
    }
};

class BlocksStream
{
public:
class Reader;
friend class Reader;

    class Reader
    {
    friend class BlocksStream;
    public:
        size_t Size()
        {
            return _o.Size();
        }
        
        size_t Tell()
        {
            return _pos;
        }
        
        bool SetPos(size_t pos)
        {
            if (pos < 0 || pos > _o.Size())
                return false;
            
            size_t off = 0;
            size_t i = 0;
            for(MeBlock *blk : _o._blocks)
            {
                if (pos >= off && pos < off + blk->GetSize() )
                {
                    _blockPos = pos - off;
                    _idx = i;
                    _pos = pos;
                    return true;
                }
                
                off += blk->GetSize();
                i++;
            }            
            return false;
        }     
        
        size_t Read(void *data, size_t sz)
        {
            if (_o._blocks.empty())
                return 0;
            
            size_t readed = 0;
            while(readed != sz && _idx < _o._blocks.size())
            {
                MeBlock *blk = _o._blocks[_idx];
                
                size_t r = blk->Read(_blockPos, (uint8_t *)data + readed, sz - readed);

                readed += r;
                
                _pos += r;
                _blockPos += r;

                if (_blockPos >= blk->GetSize())
                {
                    _blockPos = 0;
                    _idx++;
                }
            }

            return readed;
        }
        
        ByteArray Read(size_t sz)
        {
            ByteArray dat;
            
            if (_o._blocks.empty())
                return dat;
            
            size_t readed = 0;
            
            while(readed != sz && _idx < _o._blocks.size())
            {
                MeBlock *blk = _o._blocks[_idx];
                
                ByteArray r = blk->Read(_blockPos, sz - readed);
                
                if(!r.empty())
                    dat.insert(dat.end(), r.begin(), r.end());                    

                readed += r.size();
                
                _pos += r.size();
                _blockPos += r.size();

                if (_blockPos >= blk->GetSize())
                {
                    _blockPos = 0;
                    _idx++;
                }
            }

            return dat;
        }
        
        bool ReadU8(uint8_t *v)
        {
            return Read(v, 1) == 1;
        }

        bool ReadUL16(uint16_t *v)
        {
            return Read(v, 2) == 2;
        }

        bool ReadUL32(uint32_t *v)
        {
            return Read(v, 4) == 4;
        }
        
        uint8_t ReadU8()
        {
            uint8_t v = 0;
            Read(&v, 1);
            return v;
        }

        uint16_t ReadUL16()
        {
            uint16_t v = 0;
            if ( Read(&v, 2) == 2 )
                return v;
            return 0;
        }

        uint32_t ReadUL32()
        {
            uint32_t v = 0;
            if ( Read(&v, 4) == 4 )
                return v;
            return 0;
        }
        
        bool Eof()
        {
            return _pos >= _o.Size();
        }
        
    protected:
        Reader(BlocksStream &o) 
        : _o(o)
        {
            _pos = 0;
            _idx = 0;
            _blockPos = 0;
        };
        
    protected:
        BlocksStream & _o;
        size_t _pos;
        size_t _idx;
        size_t _blockPos;
    };
    
public:
    BlocksStream(size_t blkSize = 4096)
    : _blkSize(blkSize)
    {
        _size = 0;
    }
    
    ~BlocksStream()
    {
        for (MeBlock *blk : _blocks)
            delete blk;
    }
    
    size_t Write(const void *data, size_t sz)
    {
        size_t writen = 0;
        size_t err = 0;
        while(writen != sz && err < 3)
        {
            if (_blocks.empty())
                _blocks.push_back( new MeBlock(_blkSize) );
            else
            {
                size_t w = _blocks.back()->Write((uint8_t *)data + writen, sz - writen);
                
                writen += w;
                
                if (w == 0)
                    err++;
                else
                    err = 0;
                                
                if (writen != sz) // if no space in block
                {
                    _blocks.back()->Lock();
                    _blocks.push_back( new MeBlock(_blkSize) );
                }
            }
        }
        _size += writen;
        return writen;
    }
    
    size_t Write(const ByteArray &data)
    {
        return Write(data.data(), data.size());
    }
    
    bool WriteU8(uint8_t v)
    {
        return Write(&v, 1) == 1;
    }
    
    bool WriteUL16(uint16_t v)
    {
        return Write(&v, 2) == 2;
    }
    
    bool WriteUL32(uint32_t v)
    {
        return Write(&v, 4) == 4;
    }
    
    size_t Size()
    {
        return _size;
    }
    
    void Clear()
    {
        for (MeBlock *blk : _blocks)
            delete blk;
        
        _blocks.clear();
        
        _size = 0;
    }
    
    Reader GetReader()
    {
        return Reader(*this);
    }
    
protected:
    typedef std::deque<MeBlock *> _tBlockList;

    _tBlockList    _blocks;
    size_t         _size;

    const size_t _blkSize;
};

}

#endif