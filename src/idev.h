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

    virtual void ResetSlider() {};
    
    virtual bool BindKey(const std::string &keyName) {return false;};

    NC_STACK_idev() {};
    virtual ~NC_STACK_idev() {};

    virtual const std::string ClassName() const {
        return __ClassName;
    };

    //Data
    static constexpr const char * __ClassName = "idev.class";
};

#endif // IDEV_H_INCLUDED
