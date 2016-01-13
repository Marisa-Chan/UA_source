#ifndef DEFPARSER_H_INCLUDED
#define DEFPARSER_H_INCLUDED

struct scrCallBack;

typedef int (*SCRCALLBACKFUNC)(scrCallBack *);

struct scrCallBack
{
    SCRCALLBACKFUNC func;
    void *dataForStore;
    char *p1;
    char *p2;
    FILE *file;
    int line_number;
    int field_18;
    _NC_STACK_ypaworld *world;
    _NC_STACK_ypaworld *world2;
    int field_24;
    int roboID;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
};

int def_parseFile(const char *filename, int num, scrCallBack *callbacks, int flag);

#endif //DEFPARSER_H_INCLUDED
