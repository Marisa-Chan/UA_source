#ifndef IDEV_H_INCLUDED
#define IDEV_H_INCLUDED

#include "nucleas.h"

class NC_STACK_idev: public NC_STACK_nucleus
{
public:
    virtual bool GetState() {
        return false;
    };
    virtual float GetSlider() { 
        return 0.0; 
    };
    virtual void QueryKeyboard(TInputState *arg) {};
    virtual bool BindKey(const std::string &keyName) {
        return false;
    };

    virtual void ResetSlider() {};
    virtual void ForceFeedBack(uint8_t state, uint8_t effID, float p1 = 0.0, float p2 = 0.0, float p3 = 0.0, float p4 = 0.0) {};

    NC_STACK_idev() {};
    virtual ~NC_STACK_idev() {};

    virtual const std::string ClassName() const {
        return __ClassName;
    };

    //Data
    static constexpr const char * __ClassName = "idev.class";
};

#endif // IDEV_H_INCLUDED
