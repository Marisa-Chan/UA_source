#ifndef BITMAN_H_INCLUDED
#define BITMAN_H_INCLUDED

#include "common.h"


namespace Common
{

template <std::size_t _BNm>
struct BitMan : protected std::array<uint32_t, (_BNm + 31) / 32>
{
protected:
    typedef std::array<uint32_t, (_BNm + 31) / 32> BASE;
public:
    BitMan() { BASE::fill(0); };
    BitMan(const BitMan &) = default;
    BitMan(BitMan &&) = default;
    
    BitMan(std::initializer_list<uint32_t> bits) { Set(bits); }
    
    BitMan& operator=(BitMan &&) = default;
    BitMan& operator=(const BitMan &) = default;
    
    using BASE::at;
    
    void clear();
    
    constexpr uint32_t GetSize() const { return BASE::size() * 32; };
    
    operator bool() const
    {
        for ( uint32_t t : *this )
        {
            if (t != 0)
                return true;
        }
        return false;
    }
    
    bool Is(uint32_t bit) const
    {
        uint32_t bt = bit >> 5;
        if (bt >= BASE::size())
            return false;

        return (BASE::at(bt) & (1 << (bit & 0x1F))) != 0;  
    }
    
    bool Is(std::initializer_list<uint32_t> bits) const
    {
        for (uint32_t i: bits)
        {
            if (!Is(i))
                return false;
        }
        return true;
    }
    
    bool Is(const BitMan &bits) const
    {
        for(size_t i = 0; i < MIN(BASE::size(), bits.size()); i++)
        {
            if ((at(i) & bits.at(i)) != bits.at(i))
                return false;
        }

        if (BASE::size() < bits.size())
        {
            for(size_t i = BASE::size(); i < bits.size(); i++)
            {
                if (bits.at(i) != 0)
                    return false;
            }
        }
        return true;  
    }
    
    bool IsAny(std::initializer_list<uint32_t> bits) const
    {
        for (uint32_t i: bits)
        {
            if (Is(i))
                return true;
        }
        return false;
    }
    
    bool IsAny(const BitMan &bits) const
    {
        for(size_t i = 0; i < MIN(BASE::size(), bits.size()); i++)
        {
            if ((at(i) & bits.at(i)) != 0)
                return true;
        }

        if (BASE::size() < bits.size())
        {
            for(size_t i = BASE::size(); i < bits.size(); i++)
            {
                if (bits.at(i) != 0)
                    return false;
            }
        }
        return true;  
    }
    
    void Set(uint32_t bit)
    {
        uint32_t bt = bit >> 5; // /32
        if (bt < BASE::size())
            BASE::at(bt) |= (1 << (bit & 0x1F));  
    }
    
    void Set(std::initializer_list<uint32_t> bits)
    {
        for (uint32_t i: bits)
            Set(i);
    }
    
    void Set(const BitMan &bits)
    {
        for(size_t i = 0; i < MIN(BASE::size(), bits.size()); i++)
            at(i) |= bits.at(i);
    }
    
    void UnSet(uint32_t bit)
    {
        uint32_t bt = bit >> 5;
        if (bt < BASE::size())        
            BASE::at(bt) &= ~(1 << (bit & 0x1F));  
    }
    
    void UnSet(std::initializer_list<uint32_t> bits)
    {
        for (uint32_t i: bits)
            UnSet(i);
    }
    
    void UnSet(const BitMan &bits)
    {
        for(size_t i = 0; i < MIN(BASE::size(), bits.size()); i++)
            at(i) &= ~bits.at(i);
    }
    
    void XSet(uint32_t bit)
    {
        uint32_t bt = bit >> 5; // /32
        if (bt < BASE::size())    
            BASE::at(bt) ^= (1 << (bit & 0x1F));  
    }
    
    void XSet(std::initializer_list<uint32_t> bits)
    {
        for (uint32_t i: bits)
            XSet(i);
    }
    
    void XSet(const BitMan &bits)
    {
        for(size_t i = 0; i < MIN(BASE::size(), bits.size()); i++)
            at(i) ^= bits.at(i);
    }
    
    BitMan Not() const
    {
        BitMan tmp( GetSize() );

        for(size_t i = 0; i < BASE::size(); i++)
            tmp.at(i) = ~at(i);

        return tmp;
    }
    
    BitMan And(uint32_t bit) const
    {
        if (Is(bit))
            return BitMan({bit});
        return BitMan();
    }
    
    BitMan And(std::initializer_list<uint32_t> bits) const
    {
        BitMan tmp;
        for (uint32_t i: bits)
        {
            if (Is(i))
                tmp.Set(i);
        }
        return tmp;
    }
    
    BitMan And(const BitMan &bits) const
    {
        BitMan<GetSize()> tmp = *this;

        for(size_t i = 0; i < BASE::size(); i++)
            tmp.at(i) &= bits.at(i);

        return tmp;
    }
    
    BitMan Or(uint32_t bit) const
    {
        BitMan tmp(*this);
        tmp.Set(bit);
        return tmp;
    }
    
    BitMan Or(std::initializer_list<uint32_t> bits) const
    {
        BitMan tmp(*this);
        for (uint32_t i: bits)
            tmp.Set(i);
        return tmp;
    }
    
    BitMan Or(const BitMan &bits) const
    {
        BitMan<GetSize()> tmp = *this;

        for(size_t i = 0; i < BASE::size(); i++)
            tmp.at(i) |= bits.at(i);

        return tmp;
    }
    
    BitMan Xor(uint32_t bit) const
    {
        BitMan tmp(*this);
        tmp.XSet(bit);
        return tmp;
    }
    
    BitMan Xor(std::initializer_list<uint32_t> bits) const
    {
        BitMan tmp(*this);
        for (uint32_t i: bits)
            tmp.XSet(i);
        return tmp;
    }
    
    BitMan Xor(const BitMan &bits) const
    {
        BitMan<GetSize()> tmp = *this;

        for(size_t i = 0; i < BASE::size(); i++)
            tmp.at(i) ^= bits.at(i);

        return tmp;
    }
};




class DynamicBitMan : protected std::vector<uint32_t>
{
protected:
    typedef std::vector<uint32_t> BASE;
public:
    DynamicBitMan() {};
    DynamicBitMan(uint32_t sz) : BASE::vector((sz + 31) / 32) {};
    DynamicBitMan(const DynamicBitMan & b) : BASE::vector(b) {};
    DynamicBitMan(DynamicBitMan && b) : BASE::vector(b) {};
    
    DynamicBitMan(std::initializer_list<uint32_t> bits) { Set(bits); }
    
    using BASE::operator=;
    using BASE::at;
    using BASE::clear;
    
    uint32_t GetSize() const { return BASE::size() * 32; }
    void Resize(uint32_t sz) { BASE::resize( (sz + 31) / 32 , 0 ); }
    
    operator bool() const
    {
        for ( uint32_t t : *this )
        {
            if (t != 0)
                return true;
        }
        return false;
    }
    
    bool Is(uint32_t bit) const
    {
        uint32_t bt = bit >> 5;
        if (bt >= BASE::size())
            return false;

        return (BASE::at(bt) & (1 << (bit & 0x1F))) != 0;  
    }
    
    bool Is(std::initializer_list<uint32_t> bits) const
    {
        for (uint32_t i: bits)
        {
            if (!Is(i))
                return false;
        }
        return true;
    }
    
    bool Is(const DynamicBitMan &bits) const
    {
        for(size_t i = 0; i < MIN(BASE::size(), bits.size()); i++)
        {
            if ((at(i) & bits.at(i)) != bits.at(i))
                return false;
        }

        if (BASE::size() < bits.size())
        {
            for(size_t i = BASE::size(); i < bits.size(); i++)
            {
                if (bits.at(i) != 0)
                    return false;
            }
        }
        return true;  
    }
    
    
    bool IsAny(std::initializer_list<uint32_t> bits) const
    {
        for (uint32_t i: bits)
        {
            if (Is(i))
                return true;
        }
        return false;
    }
    
    bool IsAny(const DynamicBitMan &bits) const
    {
        for(size_t i = 0; i < MIN(BASE::size(), bits.size()); i++)
        {
            if ((at(i) & bits.at(i)) != 0)
                return true;
        }

        if (BASE::size() < bits.size())
        {
            for(size_t i = BASE::size(); i < bits.size(); i++)
            {
                if (bits.at(i) != 0)
                    return false;
            }
        }
        return true;  
    }
    
    void Set(uint32_t bit)
    {
        uint32_t bt = bit >> 5;
        if (bt >= BASE::size())
            BASE::resize(bt + 1, 0);

        BASE::at(bt) |= (1 << (bit & 0x1F));  
    }
    
    void Set(std::initializer_list<uint32_t> bits)
    {
        for (uint32_t i: bits)
            Set(i);
    }
    
    void Set(const DynamicBitMan &bits)
    {
        if (BASE::size() < bits.size())
            BASE::resize( bits.size() , 0 );

        for(size_t i = 0; i < bits.size(); i++)
            at(i) |= bits.at(i);
    }
    
    void UnSet(uint32_t bit)
    {
        uint32_t bt = bit >> 5;
        if (bt < BASE::size())        
            BASE::at(bt) &= ~(1 << (bit & 0x1F));  
    }
    
    void UnSet(std::initializer_list<uint32_t> bits)
    {
        for (uint32_t i: bits)
            UnSet(i);
    }
    
    void UnSet(const DynamicBitMan &bits)
    {
        for(size_t i = 0; i < MIN(BASE::size(), bits.size()); i++)
            at(i) &= ~bits.at(i);
    }
    
    void XSet(uint32_t bit)
    {
        uint32_t bt = bit >> 5;
        if (bt >= BASE::size())
            BASE::resize(bt + 1, 0);      

        BASE::at(bt) ^= (1 << (bit & 0x1F));  
    }
    
    void XSet(std::initializer_list<uint32_t> bits)
    {
        for (uint32_t i: bits)
            XSet(i);
    }
    
    void XSet(const DynamicBitMan &bits)
    {
        if (BASE::size() < bits.size())
            BASE::resize( bits.size() , 0 );

        for(size_t i = 0; i < bits.size(); i++)
            at(i) ^= bits.at(i);
    }
    
    DynamicBitMan Not() const
    {
        DynamicBitMan tmp( GetSize() );

        for(size_t i = 0; i < BASE::size(); i++)
            tmp.at(i) = ~at(i);

        return tmp;
    }
    
    DynamicBitMan And(uint32_t bit) const
    {
        if (Is(bit))
            return DynamicBitMan({bit});
        return DynamicBitMan();
    }
    
    DynamicBitMan And(std::initializer_list<uint32_t> bits) const
    {
        DynamicBitMan tmp;
        for (uint32_t i: bits)
        {
            if (Is(i))
                tmp.Set(i);
        }
        return tmp;
    }
    
    DynamicBitMan And(const DynamicBitMan &bits) const
    {
        DynamicBitMan tmp;
        tmp.Resize( MIN(GetSize(), bits.GetSize()) );

        for(size_t i = 0; i < size(); i++)
            tmp.at(i) = BASE::at(i) & bits.at(i);

        return tmp;
    }
    
    DynamicBitMan Or(uint32_t bit) const
    {
        DynamicBitMan tmp(*this);
        tmp.Set(bit);
        return tmp;
    }
    
    DynamicBitMan Or(std::initializer_list<uint32_t> bits) const
    {
        DynamicBitMan tmp(*this);
        for (uint32_t i: bits)
            tmp.Set(i);
        return tmp;
    }
    
    DynamicBitMan Or(const DynamicBitMan &bits) const
    {
        DynamicBitMan tmp(*this);

        if (tmp.GetSize() < bits.GetSize())
            tmp.Resize( bits.GetSize() );

        for(size_t i = 0; i < bits.size(); i++)
            tmp.at(i) |= bits.at(i);

        return tmp;
    }
    
    DynamicBitMan Xor(uint32_t bit) const
    {
        DynamicBitMan tmp(*this);
        tmp.XSet(bit);
        return tmp;
    }
    
    DynamicBitMan Xor(std::initializer_list<uint32_t> bits) const
    {
        DynamicBitMan tmp(*this);
        for (uint32_t i: bits)
            tmp.XSet(i);
        return tmp;
    }
    
    DynamicBitMan Xor(const DynamicBitMan &bits) const
    {
        DynamicBitMan tmp(*this);

        if (tmp.GetSize() < bits.GetSize())
            tmp.Resize( bits.GetSize() );

        for(size_t i = 0; i < bits.size(); i++)
            tmp.at(i) ^= bits.at(i);

        return tmp;
    }
};

}

#endif