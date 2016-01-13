#include <stdio.h>
#include "includes.h"
#include "yw.h"
#include "def_parser.h"


int sb_0x4d9f1c(FILE *fil, const char *filename, int callbacks_num, scrCallBack *callbacks, int *line_number, int flag);


int def_parseFile(const char *filename, int num, scrCallBack *callbacks, int flag)
{
    int v9 = 1;

    FILE *fil = FOpen(filename, "r");
    if ( fil )
    {
        int a5 = 0;
        int v6 = 2;
        while (v6 == 2)
            v6 = sb_0x4d9f1c(fil, filename, num, callbacks, &a5, flag);

        if ( v6 >= 3 && v6 <= 5 )
            v9 = 0;

        FClose(fil);
    }
    else
    {
        v9 = 0;
    }
    return v9;
}






char *sb_0x4d9f1c__sub0(FILE *fil, char *p1, char *p2, char **p3, char **p4, int *line_number)
{
    int lopp = 1;
    *p1 = 0;
    *p2 = 0;
    *p3 = NULL;
    *p4 = NULL;

    char *prev_token = NULL;
    char buf[4096];

    char *v7;

    while ( lopp )
    {
        v7 = fgets(buf, 4096, fil);

        if ( !v7 )
            break;

        (*line_number)++;

        char *line_end = strpbrk(buf, ";\n");
        if ( line_end )
            *line_end = 0;

        char *token = NULL;

        if ( prev_token )
        {
            token = strtok(buf, "= \t");
        }
        else
        {
            token = strtok(buf, "= \t");
            prev_token = token;

            if ( token )
            {
                strcpy(p1, token);

                token = strtok(0, "= \t");
            }
        }

        if ( token )
        {
            if ( token[strlen(token) - 1] == '\\' )
                token[strlen(token) - 1] = '\n';
            else
                lopp = 0;

            strcat(p2, token);
        }
        else if ( prev_token )
        {
            lopp = 0;
        }
    }

    if ( *p1 )
        *p3 = p1;

    if ( *p2 )
        *p4 = p2;

    return v7;
}


int sb_0x4d9f1c(FILE *fil, const char *filename, int callbacks_num, scrCallBack *callbacks, int *line_number, int flag)
{
    char p1[64];
    char p2[8192];

    char *pp1;
    char *pp2;


    scrCallBack *returned1 = NULL;

    while ( sb_0x4d9f1c__sub0(fil, p1, p2, &pp1, &pp2, line_number) )
    {
        if ( returned1 )
        {
            returned1->p1 = pp1;
            returned1->p2 = pp2;
            returned1->line_number = *line_number;
            returned1->file = fil;

            int v9 = returned1->func(returned1);

            if ( v9 )
            {
                if ( v9 == 3 )
                    ypa_log_out("PARSE ERROR: script %s line #%d unknown keyword %s.\n", filename, *line_number, pp1);
                else if ( v9 == 4 )
                    ypa_log_out("PARSE ERROR: script %s line #%d data expected or bogus data.\n", filename, *line_number);

                return v9;
            }
        }
        else if ( flag & 4 || strcasecmp(pp1, "include") != 0 )
        {
            for (int i = 0; i < callbacks_num; i++)
            {
                scrCallBack *clbk = &callbacks[i];
                if ( clbk->func )
                {
                    clbk->field_18 = 0;
                    clbk->line_number = *line_number;
                    clbk->p1 = pp1;
                    clbk->p2 = pp2;
                    clbk->file = fil;

                    if ( clbk->func(clbk) == 1 )
                    {
                        returned1 = clbk;
                        break;
                    }

                }
            }

            if ( !returned1 && flag & 1 )
            {
                ypa_log_out("PARSE ERROR: script %s line #%d unknown keyword %s.\n", filename, *line_number, pp1);
                return 3;
            }
        }
        else  // include
        {
            if ( !def_parseFile(pp2, callbacks_num, callbacks, flag) )
            {
                ypa_log_out("ERROR: script %s line #%d include %s failed!\n", filename, *line_number, pp2);
                return 4;
            }
        }
    }

    if ( returned1 )
    {
        ypa_log_out("PARSE ERROR: script %s unexpected EOF!\n", filename);
        return 5;
    }

    return 6;
}


