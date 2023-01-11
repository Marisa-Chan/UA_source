#ifndef IWIMP_H_INCLUDED
#define IWIMP_H_INCLUDED

#include <list>

#include "idev.h"
#include "system/inpt.h"
#include "utils.h"


class NC_STACK_iwimp: public NC_STACK_idev
{
public:
    virtual size_t Init(IDVList &stak);
    virtual bool HasFocus() {
        return true;
    };
    virtual void AddClickBoxFront(ClickBox *box);
    virtual void AddClickBoxBack(ClickBox *box);
    virtual void RemoveClickBox(ClickBox *box);
    virtual void CheckClick(TClickBoxInf *arg);

    NC_STACK_iwimp() {
        clear();
    };
    virtual ~NC_STACK_iwimp() {};
    

protected:
    void clear();

    void GetClick(TClickBoxInf *click, TMousePos &mouse);

//Data
public:
protected:
    std::list<ClickBox *> cboxList;
    ClickBox *selectedCbox;
    int32_t selectedButton;
};

#endif // IWIMP_H_INCLUDED
