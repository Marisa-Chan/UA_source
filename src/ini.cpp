#include <stdio.h>
#include "ini.h"
#include "includes.h"


int get_keyvalue_from_ini(const char *ini_filename, key_value_stru *key, unsigned int key_count)
{
    char buf[256];

    if ( !ini_filename )
        ini_filename = "nucleus.ini";

    FSMgr::FileHandle *fil = FSMgr::iDir::openFile(ini_filename, "r");

    if ( !fil )
        return 0;

    int param_line = 0;
    while ( 1 )
    {
        char * readed = fil->gets(buf, 256);

        if ( !readed )
        {
            if ( param_line >= engines.some_params_count )
                break;

            strcpy(buf, engines.some_params_pointers[param_line]);
            param_line++;
        }

        char *strip = strpbrk(buf, ";\r\n");
        if ( strip )
            *strip = 0;

        char *token = strtok(buf, "= \t");
        if ( token )
        {
            for (unsigned int i = 0; i < key_count; ++i )
            {
                key_value_stru *kkey = key + i;
                if ( !strcasecmp(kkey->key, token) )
                {
                    char *tmp = NULL;

                    switch ( kkey->key_type )
                    {
                    case KEY_TYPE_DIGIT:
                        tmp = strtok(0, "= \t");
                        if ( tmp )
                            kkey->value.val = strtol(tmp, NULL, 0);
                        break;

                    case KEY_TYPE_BOOL:
                        tmp = strtok(0, "= \t");
                        if ( tmp )
                        {
                            if ( StrGetBool(tmp) )
                                kkey->value.val = 1;
                            else
                                kkey->value.val = 0;
                        }
                        break;

                    case KEY_TYPE_STRING1:
                        tmp = strtok(0, "= \t");
                        if ( tmp )
                            strncpy((char *)kkey->value.pval, tmp, 128);
                        break;

                    case KEY_TYPE_STRING2:
                        tmp = strtok(0, "=");
                        if ( tmp )
                            strncpy((char *)kkey->value.pval, tmp, 128);
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }

    delete fil;
    return 1;
}
