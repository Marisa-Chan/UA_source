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
            result = StrGetBool(buf);
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

uint32_t fileCrc32(const std::string &filename, uint32_t _crc)
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
    std::replace(str.begin(), str.end(), '/', '\\');

    size_t pos = str.rfind('.');
    if (pos != std::string::npos && (str.length() - pos - 1) > 3)
        str.resize(pos + 3 + 1);
}

bool uaFileExist(const std::string &src_path)
{
    std::string path = file_path_copy_manipul(src_path);
    correctSeparatorAndExt(path);

    return FSMgr::iDir::fileExist(path);
}

//FSMgr::FileHandle *uaOpenFile(const char *src_path, const char *mode)
//{
//    std::string path;
//    file_path_copy_manipul(src_path, path);
//    correctSeparatorAndExt(path);
//
//    FSMgr::FileHandle *v4 = FSMgr::iDir::openFile(path.c_str(), mode);
//
//    if ( v4 )
//        engines.file_handles++;
//    else
//        ypa_log_out("uaOpenFile('%s','%s') failed!\n", path.c_str(), mode);
//
//    return v4;
//}

FSMgr::FileHandle *uaOpenFile(const std::string &src_path, const std::string &mode)
{
    std::string path = file_path_copy_manipul(src_path);
    correctSeparatorAndExt(path);

    FSMgr::FileHandle *v4 = FSMgr::iDir::openFile(path, mode);

    if ( v4 )
        engines.file_handles++;
    else
        ypa_log_out("uaOpenFile('%s','%s') failed!\n", path.c_str(), mode.c_str());

    return v4;
}

FSMgr::DirIter uaOpenDir(const std::string &dir)
{
    std::string dst = file_path_copy_manipul(dir);
    correctSeparatorAndExt(dst);

    return FSMgr::iDir::readDir(dst);
}

bool uaDeleteFile(const std::string &path)
{
    std::string dst = file_path_copy_manipul(path);
    correctSeparatorAndExt(dst);

    return FSMgr::iDir::deleteFile(dst);
}

bool uaDeleteDir(const std::string &path)
{
    std::string dst = file_path_copy_manipul(path);
    correctSeparatorAndExt(dst);

    return FSMgr::iDir::deleteDir(dst);
}

bool uaCreateDir(const std::string &path)
{
    std::string dst = file_path_copy_manipul(path);
    correctSeparatorAndExt(dst);

    return FSMgr::iDir::createDir(dst);
}

int StriCmp(const std::string &a, const std::string &b)
{
	return strcasecmp(a.c_str(), b.c_str());
}

bool StrGetBool(const std::string &str)
{
    return !StriCmp(str, "yes") || !StriCmp(str, "true") || !StriCmp(str, "on");
}