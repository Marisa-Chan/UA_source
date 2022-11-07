#ifndef WORLD_HISTORY_H_INCLUDED
#define WORLD_HISTORY_H_INCLUDED

#include <array>
#include "../memstream.h"
#include "../types.h"


namespace World
{
namespace History
{
    
enum TYPES
{
    TYPE_UNK        = 0,
    TYPE_FRAME      = 1, 
    TYPE_CONQ       = 2,
    TYPE_VHCLKILL   = 3,
    TYPE_VHCLCREATE = 4,
    // 5 unused
    TYPE_POWERST    = 6,
    TYPE_UPGRADE    = 7,

    // Num elements
    TYPES_COUNT      = 8
};
    
class Record
{
public:
    const uint8_t type;
    const size_t dataSize;    
    
public: 
    Record() : type(TYPE_UNK), dataSize(0) {};
    virtual ~Record() {};
    virtual Common::ByteArray MakeByteArray() const { return Common::ByteArray();};
    virtual void WriteBytes(void *dst) const {} ;
    virtual void ReadBytes(const void *bt) {};
    
    virtual void AddScore(std::array<TPlayerStatus, World::CVFractionsCount> *score) const {};
    
    Record& operator=(const Record& b)
    {
        return *this;
    }
    
    bool ReadByteArray(const Common::ByteArray &bt)
    {
        if (bt.size() < dataSize)
            return false;
        
        ReadBytes(bt.data());
        return true;
    }
    
protected:
    Record(uint8_t tp, size_t sz) : type(tp), dataSize(sz) {};
};

class Frame: public Record
{
public:
    uint32_t TimeStamp;
public:
    Frame() : Record(TYPE_FRAME, 4) {};
    Frame(uint32_t ts) 
    : Record(TYPE_FRAME, 4) 
    {
        TimeStamp = ts;
    };
    
    virtual Common::ByteArray MakeByteArray() const
    {
        Common::ByteArray tmp;
        tmp.push_back(TYPE_FRAME);
        Common::AppendByteUL32(&tmp, TimeStamp);
        return tmp;
    }
    
    virtual void WriteBytes(void *dst) const
    {
        uint8_t *d = (uint8_t *)dst;
        d[0] = TYPE_FRAME;
        Utils::ByteUL32(&d[1], TimeStamp);
    }
    
    virtual void ReadBytes(const void *bt)
    {
        TimeStamp = Utils::UL32Byte(bt);
    }
};

class Conq: public Record
{
public:
    int32_t secX;
    int32_t secY;
    uint8_t owner;
public:
    Conq() : Record(TYPE_CONQ, 3) {};
    Conq(int32_t x, int32_t y, uint8_t ow)
     : Record(TYPE_CONQ, 3)
    {
        secX = x;
        secY = y;
        owner = ow;
    };
    
    virtual Common::ByteArray MakeByteArray() const
    {
        Common::ByteArray tmp;
        tmp.push_back(TYPE_CONQ);
        tmp.push_back(secX);
        tmp.push_back(secY);
        tmp.push_back(owner);
        return tmp;
    }
    
    virtual void WriteBytes(void *dst) const
    {
        uint8_t *d = (uint8_t *)dst;
        d[0] = TYPE_CONQ;
        d[1] = secX;
        d[2] = secY;
        d[3] = owner;
    }
    
    virtual void ReadBytes(const void *bt)
    {
        const uint8_t *d = (const uint8_t *)bt;
        secX = d[0];
        secY = d[1];
        owner = d[2];
    }
    
    virtual void AddScore(std::array<TPlayerStatus, 8> *score) const
    {
        if (owner >= score->size())
            return;
        
        score->at(owner).SectorsTaked++;
        score->at(owner).Score++;
    }
};

class VhclKill: public Record
{
public:
    uint8_t owners; // 0..2 victim   3..5 killer   6  victim is user   7  killer is user
    uint16_t vp;  // 0x8000 if robo
    uint8_t posX;
    uint8_t posY;
public:
    VhclKill() : Record(TYPE_VHCLKILL, 5) {};
    VhclKill(uint8_t owns, uint16_t vps, uint8_t x, uint8_t y) 
    : Record(TYPE_VHCLKILL, 5) 
    {
        owners = owns;
        vp = vps;
        posX = x;
        posY = y;
    };
    
    virtual Common::ByteArray MakeByteArray() const
    {
        Common::ByteArray tmp;
        tmp.push_back(TYPE_VHCLKILL);
        tmp.push_back(owners);
        Common::AppendByteUL16(&tmp, vp);
        tmp.push_back(posX);
        tmp.push_back(posY);
        return tmp;
    }
    
    virtual void WriteBytes(void *dst) const
    {
        uint8_t *d = (uint8_t *)dst;
        d[0] = TYPE_VHCLKILL;
        d[1] = owners;
        Utils::ByteUL16(&d[2], vp);
        d[4] = posX;
        d[5] = posY;
    }
    
    virtual void ReadBytes(const void *bt)
    {
        const uint8_t *d = (const uint8_t *)bt;
        owners = d[0];
        vp = Utils::UL16Byte(&d[1]);
        posX = d[3];
        posY = d[4];
    }
    
    virtual void AddScore(std::array<TPlayerStatus, 8> *score) const
    {
        uint8_t owner = (owners >> 3) & 7;
        int flags = owners & 0xC0;
        
        if (owner >= score->size())
            return;
        
        score->at(owner).DestroyedUnits++;

        if ( flags == 0x80 )
        {
            score->at(owner).DestroyedByUser++;
            score->at(owner).Score += 20;
        }
        else if ( flags == 0xC0 )
        {
            score->at(owner).DestroyedByUser++;
            score->at(owner).Score += 200;
        }
        else
        {
            score->at(owner).Score += 10;
        }

        if ( vp & 0x8000 )
            score->at(owner).Score += 1000;
    }    
};


class VhclCreate: public Record
{
public:
    uint8_t owner;
    uint16_t vp; 
    uint8_t posX;
    uint8_t posY;
public:
    VhclCreate() : Record(TYPE_VHCLCREATE, 5) {};
    VhclCreate(uint8_t own, uint16_t visual, uint8_t x, uint8_t y) 
    : Record(TYPE_VHCLCREATE, 5) 
    {
        owner = own;
        vp = visual;
        posX = x;
        posY = y;
    };
    
    virtual Common::ByteArray MakeByteArray() const
    {
        Common::ByteArray tmp;
        tmp.push_back(TYPE_VHCLCREATE);
        tmp.push_back(owner);
        Common::AppendByteUL16(&tmp, vp);
        tmp.push_back(posX);
        tmp.push_back(posY);
        return tmp;
    }
    
    virtual void WriteBytes(void *dst) const
    {
        uint8_t *d = (uint8_t *)dst;
        d[0] = TYPE_VHCLCREATE;
        d[1] = owner;
        Utils::ByteUL16(&d[2], vp);
        d[4] = posX;
        d[5] = posY;
    }
    
    virtual void ReadBytes(const void *bt)
    {
        const uint8_t *d = (const uint8_t *)bt;
        owner = d[0];
        vp = Utils::UL16Byte(&d[1]);
        posX = d[3];
        posY = d[4];
    }
};


class PowerST: public Record
{
public:
    int32_t secX;
    int32_t secY;
    uint8_t owner;
public:
    PowerST() : Record(TYPE_POWERST, 3) {};
    PowerST(int32_t x, int32_t y, uint8_t ow)
     : Record(TYPE_POWERST, 3)
    {
        secX = x;
        secY = y;
        owner = ow;
    };
    
    virtual Common::ByteArray MakeByteArray() const
    {
        Common::ByteArray tmp;
        tmp.push_back(TYPE_POWERST);
        tmp.push_back(secX);
        tmp.push_back(secY);
        tmp.push_back(owner);
        return tmp;
    }
    
    virtual void WriteBytes(void *dst) const
    {
        uint8_t *d = (uint8_t *)dst;
        d[0] = TYPE_POWERST;
        d[1] = secX;
        d[2] = secY;
        d[3] = owner;
    }
    
    virtual void ReadBytes(const void *bt)
    {
        const uint8_t *d = (const uint8_t *)bt;
        secX = d[0];
        secY = d[1];
        owner = d[2];
    }
    
    virtual void AddScore(std::array<TPlayerStatus, 8> *score) const
    {
        if (owner >= score->size())
            return;
        
        score->at(owner).Score += 100;
        score->at(owner).Power++;
    }
};


class Upgrade: public Record
{   
public:
    int32_t secX;
    int32_t secY;
    uint8_t owner;
    uint8_t upgradeType;
    uint16_t lastVhcl;
    uint16_t lastWeapon;
    uint16_t lastBuild;
    
public:
    Upgrade() 
    : Record(TYPE_UPGRADE, 11) 
    {
        secX = 0;
        secY = 0;
        owner = 0;
        upgradeType = UPGRADE_NONE;
        lastVhcl = 0;
        lastWeapon = 0;
        lastBuild = 0;
    };
    
    Upgrade(int32_t sctX, int32_t sctY, uint8_t own, uint8_t tp, uint16_t lastV, uint16_t lastW, uint16_t lastB)
    : Record(TYPE_UPGRADE, 11)
    {
        secX = sctX;
        secY = sctY;
        owner = own;
        upgradeType = tp;
        lastVhcl = lastV;
        lastWeapon = lastW;
        lastBuild = lastB;
    }

    virtual Common::ByteArray MakeByteArray() const
    {
        Common::ByteArray tmp;
        tmp.push_back(TYPE_UPGRADE);
        tmp.push_back(secX);
        tmp.push_back(secY);
        tmp.push_back(owner);
        tmp.push_back(upgradeType);
        tmp.push_back(0);          // Padding
        Common::AppendByteUL16(&tmp, lastVhcl);
        Common::AppendByteUL16(&tmp, lastWeapon);
        Common::AppendByteUL16(&tmp, lastBuild);
        return tmp;
    }
    
    virtual void WriteBytes(void *dst) const
    {
        uint8_t *d = (uint8_t *)dst;
        d[0] = TYPE_UPGRADE;
        d[1] = secX;
        d[2] = secY;
        d[3] = owner;
        d[4] = upgradeType;
        d[5] = 0;
        Utils::ByteUL16(&d[6], lastVhcl);
        Utils::ByteUL16(&d[8], lastWeapon);
        Utils::ByteUL16(&d[10], lastBuild);
    }
    
    virtual void ReadBytes(const void *bt)
    {
        const uint8_t *d = (const uint8_t *)bt;
        secX = d[0];
        secY = d[1];
        owner = d[2];
        upgradeType = d[3];
        lastVhcl = Utils::UL16Byte(&d[5]);
        lastWeapon = Utils::UL16Byte(&d[7]);
        lastBuild = Utils::UL16Byte(&d[9]);
    }
    
    virtual void AddScore(std::array<TPlayerStatus, 8> *score) const
    {
        if (owner >= score->size())
            return;
        
        score->at(owner).Score += 500;
        score->at(owner).Upgrades++;
    }
};



class Instance
{
public:
    Instance()
    {
        instances[TYPE_UNK] = NULL;
        instances[TYPE_FRAME] = new Frame;
        instances[TYPE_CONQ] = new Conq;
        instances[TYPE_VHCLKILL] = new VhclKill;
        instances[TYPE_VHCLCREATE] = new VhclCreate;
        instances[5] = NULL;
        instances[TYPE_POWERST] = new PowerST;
        instances[TYPE_UPGRADE] = new Upgrade;
    }
    
    ~Instance()
    {
        for(Record * r : instances)
        {
            if (r)
                delete r;
        }
    }
    
    Record *operator[](size_t i)
    {
        if (i < 0 || i >= instances.size())
            return NULL;
        return instances[i];
    }
    
    
protected:
    std::array<Record *, TYPES_COUNT> instances;
};


inline bool FillDecoderBStrm(Record *d, Common::BlocksStream::Reader *r)
{
    return d->ReadByteArray( r->Read( d->dataSize ) );
}


}
}

#endif