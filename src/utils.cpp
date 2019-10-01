#include <algorithm>

#include "includes.h"
#include "utils.h"
#include "inttypes.h"
#include "crc32.h"
#include <SDL2/SDL_timer.h>


int read_yes_no_status(const char *file, int result)
{
    char buf[128];

    FSMgr::FileHandle *fil = uaOpenFile(file, "r");
    if ( fil )
    {
        if ( fil->gets(buf, 128) )
        {
            char *lend = strpbrk(buf, "; \n\r");
            if ( lend )
                *lend = 0;
            result = strcasecmp(buf, "yes") == 0;
        }
        delete fil;
    }
    return result;
}

float SWAP32F(float f)
{
    uint32_t tmp = *(uint32_t *)&f;
    tmp = SWAP32(tmp);
    return *(float *)&tmp;
}


const char *get_lang_string(char **array, int id, const char *def)
{
    const char *v4 = array[id];
    if ( !v4 || !strcmp(v4, "<>") )
        return def;
    return v4;
}

#ifndef strnicmp
int strnicmp (const char *s1, const char *s2, size_t n)
{
    const char *s2end = s2 + n;

    while (s2 < s2end && *s2 != 0 && toupper(*s1) == toupper(*s2))
        s1++, s2++;
    if (s2end == s2)
        return 0;
    return (int) (toupper(*s1) - toupper(*s2));
}
#endif

int dround(float val)
{
    return val + 0.5;
}

int dround(double val)
{
    return val + 0.5;
}

uint32_t fileCrc32(const char *filename, uint32_t _crc)
{
    uint32_t crc = _crc;

    FSMgr::FileHandle *fil = uaOpenFile(filename, "rb");
    if (fil)
    {
        const size_t BUFSZ = 4 * 1024;
        void *tmp = malloc(BUFSZ); //4K block

        size_t readed = fil->read(tmp, BUFSZ);
        while(readed != 0)
        {
            crc = crc32(crc, tmp, readed);
            readed = fil->read(tmp, BUFSZ);
        }
        free(tmp);
        delete fil;
    }
    return crc;
}


int DO = 0; //Shutup "MAKE ME" screams

void dprintf(const char *fmt, ...)
{
    if (DO)
    {
        va_list args;
        va_start (args, fmt);

        vprintf(fmt, args);

        va_end(args);
    }
}

uint32_t profiler_begin()
{
    Uint64 freq = SDL_GetPerformanceFrequency();

    if ( !freq )
        return 0;

    Uint64 cnt = SDL_GetPerformanceCounter();
    return cnt / (freq / 10000);
}

uint32_t profiler_end(uint32_t prev)
{
    Uint64 freq = SDL_GetPerformanceFrequency();

    if ( !freq )
        return 0 - prev;

    Uint64 cnt = SDL_GetPerformanceCounter();
    return cnt / (freq / 10000) - prev;
}



void correctSeparatorAndExt(std::string &str)
{
    replace(str.begin(), str.end(), '/', '\\');

    size_t pos = str.find_last_of('.');
    if (pos != std::string::npos && (str.length() - pos - 1) > 3)
        str.resize(pos + 3 + 1);
}

bool uaFileExist(const char *src_path, const char *prefix)
{
    std::string pre_copy;
    if (prefix)
    {
        pre_copy = prefix;
        pre_copy += src_path;
    }
    else
        pre_copy = src_path;

    char path[256];
    file_path_copy_manipul(pre_copy.c_str(), path, 256);

    std::string dst = path;
    correctSeparatorAndExt(dst);

    return FSMgr::iDir::fileExist(dst.c_str());
}

FSMgr::FileHandle *uaOpenFile(const char *src_path, const char *mode)
{
    char path[256];
    file_path_copy_manipul(src_path, path, 256);

    std::string dst = path;
    correctSeparatorAndExt(dst);

    FSMgr::FileHandle *v4 = FSMgr::iDir::openFile(dst.c_str(), mode);

    if ( v4 )
        engines.file_handles++;
    else
        ypa_log_out("uaOpenFile('%s','%s') failed!\n", dst.c_str(), mode);

    return v4;
}

FSMgr::DirIter *uaOpenDir(const char *dir)
{
    char src[256];
    file_path_copy_manipul(dir, src, 256);

    std::string dst = src;
    correctSeparatorAndExt(dst);

    return FSMgr::iDir::readDir(dst.c_str());
}

bool uaDeleteFile(const char *path)
{
    char src[256];
    file_path_copy_manipul(path, src, 256);

    std::string dst = src;
    correctSeparatorAndExt(dst);

    return FSMgr::iDir::deleteFile(dst.c_str());
}

bool uaDeleteDir(const char *path)
{
    char src[256];
    file_path_copy_manipul(path, src, 256);

    std::string dst = src;
    correctSeparatorAndExt(dst);

    return FSMgr::iDir::deleteDir(dst.c_str());
}

bool uaCreateDir(const char *path)
{
    char src[256];
    file_path_copy_manipul(path, src, 256);

    std::string dst = src;
    correctSeparatorAndExt(dst);

    return FSMgr::iDir::createDir(dst.c_str());
}

