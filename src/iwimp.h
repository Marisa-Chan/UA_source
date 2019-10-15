#ifndef IWIMP_H_INCLUDED
#define IWIMP_H_INCLUDED

#include <list>

#include "idev.h"
#include "engine_input.h"
#include "utils.h"


class NC_STACK_iwimp: public NC_STACK_idev
{
public:
    virtual size_t func0(IDVList *stak);
    virtual bool HasFocus() {
        return true;
    };
    virtual void AddClickBoxFront(ClickBox *box);
    virtual void AddClickBoxBack(ClickBox *box);
    virtual void RemoveClickBox(ClickBox *box);
    virtual void CheckClick(ClickBoxInf *arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_iwimp() {
        clear();
    };
    virtual ~NC_STACK_iwimp() {};

    virtual const char * getClassName() {
        return "iwimp.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_iwimp();
    };

protected:
    void clear();

    void GetClick(ClickBoxInf *click, MousePos &mouse);

//Data
public:
    static const NewClassDescr description;

protected:
    std::list<ClickBox *> cboxList;
    ClickBox *selectedCbox;
    int selectedButton;
};

#endif // IWIMP_H_INCLUDED
