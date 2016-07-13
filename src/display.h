#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include "raster.h"

struct __NC_STACK_display
{
    UA_PALETTE palette;
    UA_PALETTE field_300[8];
    bitmap_intern *pointer_bitm;
    int field_1b04;
};


struct rstr_261_arg
{
    int pal_id;
    int entrie_id;
    int pal_num;
    UA_PALETTE *palette;
};

struct rstr_262_arg
{
    int dword0;
    int *pdword4;
    int *pdword8;
};

struct displ_arg263
{
    bitmap_intern *bitm;
    int pointer_id;
};

struct windd_arg256
{
    int sort_id;
    int width;
    int height;
    char name[32];
};

class NC_STACK_display: public NC_STACK_raster
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func1(stack_vals *stak);
    virtual size_t func2(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
    virtual size_t display_func256(windd_arg256 *inout) {
        return 0;
    };
    virtual void display_func257(stack_vals *) {};
    virtual void display_func258(stack_vals *) {};
    virtual void display_func259(stack_vals *) {};
    virtual void display_func260(stack_vals *) {};
    virtual void display_func261(rstr_261_arg *arg);
    virtual void display_func262(rstr_262_arg *arg);
    virtual void display_func263(displ_arg263 *arg);
    virtual void display_func264(void *);
    virtual void display_func265(void *);
    virtual size_t display_func266(bitmap_intern **pbitm);
    virtual void display_func267(bitmap_intern **);
    virtual void display_func268(bitmap_intern **pbitm);
    virtual size_t display_func269(bitmap_intern **);
    virtual void display_func270(bitmap_intern **);
    virtual void display_func271(stack_vals *stak) {};
    virtual void display_func272(stack_vals *) {};
    virtual UA_PALETTE * display_func273(rstr_261_arg *arg);
    virtual void display_func274(const char **);


    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_display() {
        memset(&stack__display, 0, sizeof(stack__display));
    };
    virtual ~NC_STACK_display() {};

    virtual const char * getClassName() {
        return "display.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_display();
    };

    enum DISP_ATT
    {
        DISP_ATT_DISPLAY_ID = 0x80004000,
        DISP_ATT_DISPLAY_INF = 0x80004001,
    };

    //Set
    virtual void setBMD_palette(UA_PALETTE *);

    //Get
    virtual UA_PALETTE *getBMD_palette();

    //Data
    static const NewClassDescr description;

    __NC_STACK_display stack__display;
};

#endif // DISPLAY_H_INCLUDED
