#ifndef YUFO_H_INCLUDED
#define YUFO_H_INCLUDED

#include "nucleas.h"
#include "ypabact.h"

class NC_STACK_ypaufo: public NC_STACK_ypabact
{
public:
    virtual size_t Init(IDVList &stak);
    virtual size_t Deinit();
    virtual size_t func2(IDVList &stak);
    virtual void AI_layer3(update_msg *arg);
    virtual void User_layer(update_msg *arg);
    virtual void Move(move_msg *arg);
    virtual size_t SetPosition(bact_arg80 *arg);
    virtual void Renew();

    NC_STACK_ypaufo() 
    {
        _ufoTogo        = 0.0;
        _ufoTogoRisidue = 0.0;
        _ufoBoost       = 0.0;
        _ufoProcAngle   = 0.0;
        _ufoFlags       = 0;
    };
    virtual ~NC_STACK_ypaufo() {};
    
    virtual const std::string ClassName() const {
        return __ClassName;
    };

    enum UFO_ATT
    {
        UFO_ATT_TOGO = 0x80002000
    };

    virtual void setBACT_inputting(bool) override;
    virtual void setUFO_togo(int);

    virtual int getUFO_togo();

    //Data
    static constexpr const char * __ClassName = "ypaufo.class";

    
    float _ufoTogo        = 0.0;
    float _ufoTogoRisidue = 0.0;
    float _ufoBoost       = 0.0;
    float _ufoProcAngle   = 0.0;
    int   _ufoFlags       = 0;
};

#endif // YUFO_H_INCLUDED
