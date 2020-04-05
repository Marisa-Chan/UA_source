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


std::string file_path_copy_manipul(const std::string &src)
{
    std::string buf1 = src;

    int stpos = 0;
    if ( buf1.length() > 2 && buf1[1] == ':' ) //disk name, e.g. C:/D:/Y:/...
        stpos = 2;

    size_t fndPos = buf1.find(':', stpos);

    if ( fndPos == std::string::npos )
        return buf1;


    std::string kword = buf1.substr(0, fndPos);

    for (std::list<TKVPair>::iterator it = engines.kvPairs.begin(); it != engines.kvPairs.end(); it++)
    {
        if ( !StriCmp(it->name, kword) )
        {
            std::string buf2;
            if (it->value.back() == ':') // if value has colon - do not add slash
                buf2 = it->value + buf1.substr(fndPos + 1);
            else
                buf2 = it->value + '/' + buf1.substr(fndPos + 1);

            return file_path_copy_manipul(buf2);
        }
    }

    buf1[fndPos] = '/';

    return buf1;
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

std::string get_prefix_replacement(const std::string &prefix)
{
    for( auto &x : engines.kvPairs )
    {
        if ( !StriCmp(x.name, prefix ) )
            return x.value;
    }

    return "";
}

void set_prefix_replacement(const std::string &str1, const std::string &str2)
{
    for (std::list<TKVPair>::iterator it = engines.kvPairs.begin(); it != engines.kvPairs.end(); it++)
    {
        if ( !StriCmp(it->name, str1) )
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

std::string SetPathKeys(const std::string &name, const std::string &value)
{
    for( auto &x : engines.kvPairs )
    {
        if ( !StriCmp(x.name, name) )
        {
            std::string old = x.value;
            x.value = value;
            return old;
        }
    }

    engines.kvPairs.emplace_front();
    TKVPair &tmp = engines.kvPairs.front();
    tmp.name = name;
    tmp.value = value;
    return "";
}


void sub_412810(const std::string &src, std::string &dst)
{
    dst = file_path_copy_manipul(src);
    correctSeparatorAndExt(dst);
}

