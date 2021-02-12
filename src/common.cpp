#include "common.h"

namespace Common
{
BitMan::BitMan(std::initializer_list<uint32_t> bits)
{
    Set(bits);
}

uint32_t BitMan::GetSize() const 
{ 
    return BASE::size() * 32; 
}

void BitMan::Resize(uint32_t sz) 
{ 
    BASE::resize( (sz + 31) / 32 , 0 ); 
}
    
BitMan::operator bool() const
{
    for ( uint32_t t : *this )
    {
        if (t != 0)
            return true;
    }
    return false;
}

bool BitMan::Is(uint32_t bit) const
{
    uint32_t bt = bit >> 5;
    if (bt >= BASE::size())
        return false;

    return (BASE::at(bt) & (1 << (bit & 0x1F))) != 0;  
}

bool BitMan::Is(std::initializer_list<uint32_t> bits) const
{
    for (uint32_t i: bits)
    {
        if (!Is(i))
            return false;
    }
    return true;
}

bool BitMan::Is(const BitMan &bits) const
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

bool BitMan::IsAny(std::initializer_list<uint32_t> bits) const
{
    for (uint32_t i: bits)
    {
        if (Is(i))
            return true;
    }
    return false;
}

bool BitMan::IsAny(const BitMan &bits) const
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

void BitMan::Set(uint32_t bit)
{
    uint32_t bt = bit >> 5;
    if (bt >= BASE::size())
        BASE::resize(bt + 1, 0);

    BASE::at(bt) |= (1 << (bit & 0x1F));  
}

void BitMan::Set(std::initializer_list<uint32_t> bits)
{
    for (uint32_t i: bits)
        Set(i);
}

void BitMan::Set(const BitMan &bits)
{
    if (BASE::size() < bits.size())
        BASE::resize( bits.size() , 0 );

    for(size_t i = 0; i < bits.size(); i++)
        at(i) |= bits.at(i);
}

void BitMan::UnSet(uint32_t bit)
{
    uint32_t bt = bit >> 5;
    if (bt < BASE::size())        
        BASE::at(bt) &= ~(1 << (bit & 0x1F));  
}

void BitMan::UnSet(std::initializer_list<uint32_t> bits)
{
    for (uint32_t i: bits)
        UnSet(i);
}

void BitMan::UnSet(const BitMan &bits)
{
    for(size_t i = 0; i < MIN(BASE::size(), bits.size()); i++)
        at(i) &= ~bits.at(i);
}

void BitMan::XSet(uint32_t bit)
{
    uint32_t bt = bit >> 5;
    if (bt >= BASE::size())
        BASE::resize(bt + 1, 0);      

    BASE::at(bt) ^= (1 << (bit & 0x1F));  
}

void BitMan::XSet(std::initializer_list<uint32_t> bits)
{
    for (uint32_t i: bits)
        XSet(i);
}

void BitMan::XSet(const BitMan &bits)
{
    if (BASE::size() < bits.size())
        BASE::resize( bits.size() , 0 );

    for(size_t i = 0; i < bits.size(); i++)
        at(i) ^= bits.at(i);
}

BitMan BitMan::Not() const
{
    BitMan tmp( GetSize() );

    for(size_t i = 0; i < BASE::size(); i++)
        tmp.at(i) = ~at(i);

    return tmp;
}

BitMan BitMan::And(uint32_t bit) const
{
    if (Is(bit))
        return BitMan({bit});
    return BitMan();
}

BitMan BitMan::And(std::initializer_list<uint32_t> bits) const
{
    BitMan tmp;
    for (uint32_t i: bits)
    {
        if (Is(i))
            tmp.Set(i);
    }
    return tmp;
}

BitMan BitMan::And(const BitMan &bits) const
{
    BitMan tmp;
    tmp.Resize( MIN(GetSize(), bits.GetSize()) );

    for(size_t i = 0; i < size(); i++)
        tmp.at(i) = BASE::at(i) & bits.at(i);

    return tmp;
}

BitMan BitMan::Or(uint32_t bit) const
{
    BitMan tmp(*this);
    tmp.Set(bit);
    return tmp;
}

BitMan BitMan::Or(std::initializer_list<uint32_t> bits) const
{
    BitMan tmp(*this);
    for (uint32_t i: bits)
        tmp.Set(i);
    return tmp;
}

BitMan BitMan::Or(const BitMan &bits) const
{
    BitMan tmp(*this);

    if (tmp.GetSize() < bits.GetSize())
        tmp.Resize( bits.GetSize() );

    for(size_t i = 0; i < bits.size(); i++)
        tmp.at(i) |= bits.at(i);

    return tmp;
}

BitMan BitMan::Xor(uint32_t bit) const
{
    BitMan tmp(*this);
    tmp.XSet(bit);
    return tmp;
}

BitMan BitMan::Xor(std::initializer_list<uint32_t> bits) const
{
    BitMan tmp(*this);
    for (uint32_t i: bits)
        tmp.XSet(i);
    return tmp;
}

BitMan BitMan::Xor(const BitMan &bits) const
{
    BitMan tmp(*this);

    if (tmp.GetSize() < bits.GetSize())
        tmp.Resize( bits.GetSize() );

    for(size_t i = 0; i < bits.size(); i++)
        tmp.at(i) ^= bits.at(i);

    return tmp;
}

}
