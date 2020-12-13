#ifndef YCAR_H_INCLUDED
#define YCAR_H_INCLUDED

#include "nucleas.h"
#include "ypatank.h"

class NC_STACK_ypacar: public NC_STACK_ypatank
{
public:
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();
    virtual size_t func2(IDVList &stak);
    virtual void User_layer(update_msg *arg);
    virtual int AlignVehicleAI(float dtime, vec3d *pNormal);
    virtual int AlignVehicleUser(float dtime, const vec3d &oldDir);

    NC_STACK_ypacar();
    virtual ~NC_STACK_ypacar() {};

    virtual const std::string &ClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_ypacar();
    };

    enum CAR_ATT
    {
        CAR_ATT_KAMIKAZE = 0x80003000,
        CAR_ATT_BLAST = 0x80003001
    };

    virtual void setCAR_kamikaze(bool kam);
    virtual void setCAR_blast(int blast);

    virtual bool getCAR_kamikaze();
    virtual int getCAR_blast();
    
    vec3d CarTip(float dtime, const vec3d &oldDir, vec3d rot);
    void DoKamikaze();

    //Data
public:
    static const Nucleus::ClassDescr description;

protected:
    bool _carKamikaze;
    int _carBlast;
};

#endif // YCAR_H_INCLUDED
