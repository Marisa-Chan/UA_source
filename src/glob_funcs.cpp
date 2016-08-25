#include "includes.h"

void nc_FreeMem(void *mem)
{
    free(mem);
}

void *AllocVec(size_t size, int a2)
{
    void *mem = calloc(1, size);

    if (!mem)
        ypa_log_out("_AllocVec(%d, %d) failed (Out of mem)!\n", size, a2);

    return mem;
}


char * file_path_copy_manipul(const char *src, char *dst, int size)
{
    char buf1[256];

    dst[size - 1] = 0;

    strcpy(buf1, src);

    char *v8;

    if ( strlen(buf1) <= 2 || buf1[1] != ':' )
        v8 = buf1;
    else
        v8 = buf1 + 2;

    while ( *v8 != ':' )
    {
        if ( !(*v8) )
        {
            v8 = NULL;
            break;
        }
        v8++;
    }

    if ( !v8 )
        return strncpy(dst, buf1, size - 1);

    *v8 = 0;

    nnode_str *v11 = (nnode_str *)engines.stru_525D68.head;

    if ( v11->next )
    {
        while ( strcasecmp(v11->str, buf1) )
        {
            v11 = (nnode_str *)v11->next;
            if ( !v11->next )
            {
                v11 = NULL;
                break;
            }
        }
    }
    else
    {
        v11 = NULL;
    }

    *v8 = '/';

    if ( v11 )
    {
        char buf2[512];

        strcpy(buf2, v11->str2);

        if (buf2[strlen(buf2) - 1] == ':')
            v8++;

        strcat(buf2, v8);

        strncpy(dst, buf2, size - 1);

        return file_path_copy_manipul(buf2, dst, size);
    }

    return strncpy(dst, buf1, size - 1);
}

const char * get_prefix_replacement(const char *prefix)
{
    nnode_str *cur = (nnode_str *)engines.stru_525D68.head;

    if ( !cur->next )
        return NULL;

    while ( strcasecmp(cur->str, prefix) )
    {
        cur = (nnode_str *)cur->next;
        if ( !cur->next )
            return NULL;
    }

    if ( cur )
        return cur->str2;

    return NULL;
}

int set_prefix_replacement(const char *str1, const char *str2)
{
    nnode_str *cur = (nnode_str *)engines.stru_525D68.head;

    if ( cur->next )
    {
        while ( strcasecmp(cur->str, str1) )
        {
            cur = (nnode_str *)cur->next;
            if ( !cur->next )
            {
                cur = NULL;
                break;
            }
        }
    }
    else
        cur = NULL;

    if ( !cur )
    {
        cur = (nnode_str *)AllocVec(sizeof(nnode_str), 65537);

        if (cur)
            AddHead(&engines.stru_525D68, cur);
    }

    if ( cur )
    {
        strcpy(cur->str, str1);
        strcpy(cur->str2, str2);
    }

    return cur != NULL;
}


void correct_slashes_and_3_ext(const char *src, char *dst, int sz)
{
    strncpy(dst, src, sz - 1);

    char *v4 = dst;

    while( (*v4) )
    {
        if (*v4 == '/')
            *v4 = '\\';
        v4++;
    }

    v4 = dst;

    while ((*v4))
    {
        if (*v4 == '.')
        {
            v4++;
            if (strlen(v4) > 3)
                v4[3] = 0;

            break;
        }
        v4++;
    }
}



void sub_412810(const char *a1, char *a2, int num)
{
    char v6[256];
    file_path_copy_manipul(a1, v6, 256);
    correct_slashes_and_3_ext(v6, a2, num);
}

