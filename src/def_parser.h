#ifndef DEFPARSER_H_INCLUDED
#define DEFPARSER_H_INCLUDED

#include "fsmgr.h"

struct scrCallBack;

typedef int (*SCRCALLBACKFUNC)(scrCallBack *);

struct scrCallBack
{
    SCRCALLBACKFUNC func;
    void *dataForStore;
    char *p1;
    char *p2;
    FSMgr::FileHandle *file;
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

int sb_0x4d9f1c(FSMgr::FileHandle *fil, const char *filename, int callbacks_num, scrCallBack *callbacks, int *line_number, int flag);

#endif //DEFPARSER_H_INCLUDED
