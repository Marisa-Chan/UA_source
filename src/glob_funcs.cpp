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

    std::list<TKVPair *>::iterator it;
    for (it = engines.kvPairs.begin(); it != engines.kvPairs.end(); it++)
    {
        if ( strcasecmp((*it)->name.c_str(), buf1) == 0 )
            break;
    }

    *v8 = '/';

    if ( it != engines.kvPairs.end() )
    {
        char buf2[512];

        strcpy(buf2, (*it)->value.c_str());

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
    for(std::list<TKVPair *>::iterator it = engines.kvPairs.begin(); it != engines.kvPairs.end(); it++)
    {
        if ( strcasecmp((*it)->name.c_str(), prefix ) == 0 )
            return (*it)->value.c_str();
    }

    return "";
}

void set_prefix_replacement(const char *str1, const char *str2)
{
    for (std::list<TKVPair *>::iterator it = engines.kvPairs.begin(); it != engines.kvPairs.end(); it++)
    {
        if ( strcasecmp((*it)->name.c_str(), str1) == 0 )
        {
            (*it)->value = str2;
            return;
        }
    }

    TKVPair *tmp = new TKVPair;

    tmp->name = str1;
    tmp->value = str2;

    engines.kvPairs.push_front(tmp);
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

