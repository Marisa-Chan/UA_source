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

FILE * FOpen(const char *src_path, const char *mode)
{
    char dst[256];
    char path[256];

    file_path_copy_manipul(src_path, path, 256);
    correct_slashes_and_3_ext(path, dst, 256);

    FILE *v4 = fopen(dst, mode);

    if ( v4 )
        engines.file_handles++;
    else
        ypa_log_out("_FOpen('%s','%s') failed!\n", dst, mode);

    return v4;
}

int FClose(FILE *a1)
{
    engines.file_handles--;
    return fclose(a1);
}

int _NTFindNextFileWithAttr(HANDLE hFindFile, DWORD attr, WIN32_FIND_DATA *data)
{
    while ( 1 )
    {
        if ( !data->dwFileAttributes )
            data->dwFileAttributes = 128;

        if ( attr & data->dwFileAttributes )
            break;

        if ( !FindNextFile(hFindFile, data) )
            return 0;
    }
    return 1;
}

void _MakeDOSDT(const FILETIME *lpFileTime, LPWORD lpFatDate, LPWORD lpFatTime)
{
    FILETIME FileTime;

    FileTimeToLocalFileTime(lpFileTime, &FileTime);
    FileTimeToDosDateTime(&FileTime, lpFatDate, lpFatTime);
}

void _GetNTDirInfo(ncDir *dir, WIN32_FIND_DATA *data)
{
    _MakeDOSDT(&data->ftLastWriteTime, (LPWORD)&dir->field_18, (LPWORD)&dir->field_16);
    dir->field_15 = data->dwFileAttributes;
    dir->field_1A = data->nFileSizeLow;

    strncpy(dir->cFileName, data->cFileName, 255);
    dir->cFileName[255] = 0;
}

ncDir * _int_opendir(LPCSTR lpFileName, DWORD attr, ncDir *dir)
{
    WIN32_FIND_DATA v11;

    HANDLE hndl = FindFirstFile(lpFileName, &v11);
    if ( hndl == (HANDLE)-1 )
    {
        return 0;
    }
    if ( !_NTFindNextFileWithAttr(hndl, attr, &v11) )
    {
        return 0;
    }
    dir->hndl = hndl;
    dir->attr = attr;

    _GetNTDirInfo(dir, &v11);

    dir->field_120 = 1;
    return dir;
}


int is_directory(const char *a1)
{
    if ( !*a1 )
        return 0;

    while ( a1[1] )
        a1++;

    if ( *a1 == '\\' || *a1 == '/' || *a1 == '.' )
        return 1;

    return 0;
}

ncDir *_r_nc_opendir(const char *path, DWORD flags)
{
    char buf[260];

    ncDir *dir = (ncDir *)AllocVec(sizeof(ncDir), 0);
    dir->hndl = 0;
    dir->field_15 = 0x10;

    if ( is_directory(path) )
    {
        dir->field_15 = 0x10;
    }
    else if ( !_int_opendir(path, flags, dir) )
    {
        nc_FreeMem(dir);
        return 0;
    }

    if ( dir->field_15 & 0x10 )
    {
        int v7 = 0;
        while ( 1 )
        {
            char v8 = *path;
            buf[v7] = *path;

            if ( !v8 )
                break;

            char v10 = *path;
            if ( v10 != '*' && v10 != '?' )
            {
                v7++;
                path++;
                if ( v7 >= 260 )
                    break;
            }
            else
                return dir;
        }

        if ( v7 )
        {
            if ( buf[v7 - 1] != '\\' && buf[v7 - 1] != '/' )
            {
                v7++;
                buf[v7 - 1] = '\\';
            }
        }
        strcat(buf, "*.*");

        if ( dir->hndl )
            FindClose(dir->hndl);

        if ( !_int_opendir(buf, flags, dir) )
        {
            nc_FreeMem(dir);
            return 0;
        }
    }
    return dir;
}

ncDir *nc_opendir(const char *a2)
{
    return _r_nc_opendir(a2, 0xFFFFFFF7);
}


ncDir *OpenDir(const char *dir)
{
    char v3[256];
    char src[256];

    file_path_copy_manipul(dir, src, 256);
    correct_slashes_and_3_ext(src, v3, 256);
    return nc_opendir(v3);
}



ncDir *_readdir(ncDir *dir)
{
    WIN32_FIND_DATA v6;

    if ( dir )
    {
        if ( dir->field_120 )
        {
            dir->field_120 = 0;
            return dir;
        }

        if ( !FindNextFile(dir->hndl, &v6) )
            return NULL;

        if ( !_NTFindNextFileWithAttr(dir->hndl, dir->attr, &v6) )
            return NULL;

        _GetNTDirInfo(dir, &v6);
    }

    return dir;
}


dirEntry *ReadDir(ncDir *_dir, dirEntry *dir_entry)
{
    memset(dir_entry, 0, sizeof(dirEntry));
    ncDir *dir = _readdir(_dir);

    if ( dir )
    {
        if ( dir->field_15 & 0x10 )
            dir_entry->field_0 |= 1;
        else
            dir_entry->field_4 = dir->field_1A;

        strcpy(dir_entry->e_name, dir->cFileName);
        return dir_entry;
    }

    return NULL;
}



int __closedir(ncDir *dir)
{
    if ( dir && dir->field_120 <= 1 )
    {
        if ( FindClose(dir->hndl) )
        {
            dir->field_120 = 2;
            nc_FreeMem(dir);
        }
    }

    return 0;
}

void CloseDir(ncDir *dir)
{
    __closedir(dir);
}




void sub_412810(char *a1, char *a2, int num)
{
    char v6[256];
    file_path_copy_manipul(a1, v6, 256);
    correct_slashes_and_3_ext(v6, a2, num);
}




int _delete_file(const char *lpFileName)
{
    if ( DeleteFile(lpFileName) )
        return 0;

    return -1;
}

int delete_file(const char *path)
{
    char tmp2[256];
    char tmp[256];

    file_path_copy_manipul(path, tmp, 256);
    correct_slashes_and_3_ext(tmp, tmp2, 256);
    return _delete_file(tmp2) == 0;
}



int rmdir(const char *a1)
{
    if ( RemoveDirectory(a1) )
        return 0;

    return -1;
}

int removeDirectory(const char *path)
{
    char tmp2[256];
    char tmp[256];

    file_path_copy_manipul(path, tmp, 256);
    correct_slashes_and_3_ext(tmp, tmp2, 256);
    return rmdir(tmp2) == 0;
}
