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


void file_path_copy_manipul(const std::string &src, std::string &dst)
{
    std::string buf1 = src;

    int stpos = 0;
    if ( buf1.length() > 2 && buf1[1] == ':' ) //disk name, e.g. C:/D:/Y:/...
        stpos = 2;

    size_t fndPos = buf1.find(':', stpos);

    if ( fndPos == std::string::npos )
    {
        dst = buf1;
        return;
    }


    std::string kword = buf1.substr(0, fndPos);

    for (std::list<TKVPair>::iterator it = engines.kvPairs.begin(); it != engines.kvPairs.end(); it++)
    {
        if ( strcasecmp(it->name.c_str(), kword.c_str()) == 0 )
        {
            std::string buf2;
            if (it->value.back() == ':') // if value has colon - do not add slash
                buf2 = it->value + buf1.substr(fndPos + 1);
            else
                buf2 = it->value + '/' + buf1.substr(fndPos + 1);

            dst = buf2;
            file_path_copy_manipul(buf2.c_str(), dst);

            return;
        }
    }

    buf1[fndPos] = '/';

    dst = buf1;
}

const char * get_prefix_replacement(const char *prefix)
{
    for(std::list<TKVPair>::iterator it = engines.kvPairs.begin(); it != engines.kvPairs.end(); it++)
    {
        if ( strcasecmp(it->name.c_str(), prefix ) == 0 )
            return it->value.c_str();
    }

    return "";
}

void set_prefix_replacement(const char *str1, const char *str2)
{
    for (std::list<TKVPair>::iterator it = engines.kvPairs.begin(); it != engines.kvPairs.end(); it++)
    {
        if ( strcasecmp(it->name.c_str(), str1) == 0 )
        {
            it->value = str2;
            return;
        }
    }

    engines.kvPairs.emplace_front();

    TKVPair &tmp = engines.kvPairs.front();

    tmp.name = str1;
    tmp.value = str2;
}


void sub_412810(const std::string &src, std::string &dst)
{
    file_path_copy_manipul(src, dst);
    correctSeparatorAndExt(dst);
}

