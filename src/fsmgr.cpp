#include <string.h>
#include <stdarg.h>
#include <inttypes.h>

#if defined(WIN32) && !defined(__WINE__)

#include <windows.h>
#define FSD '\\'
#else

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#define FSD '/'
#endif

#include "fsmgr.h"


namespace FSMgr
{
static iDir directories("", "", NULL);

iNode::iNode(const char *_name, const char *filepath, iDir *_parent)
{
    name.assign(_name);
    path.assign(filepath);
    parent = _parent;
}

int iNode::getType()
{
    return type;
}

const char *iNode::getPath()
{
    return path.c_str();
}

const char *iNode::getName()
{
    return name.c_str();
}

void iNode::getVPath(std::string &out)
{
    if (parent != NULL)
    {
        parent->getVPath(out);
        out += FSD;
        out += getName();
    }
    else
        out = getName();
}




iFile::iFile(const char *_name, const char *filepath, iDir *_parent):
    iNode(_name, filepath, _parent)
{
    type = NTYPE_FILE;
}


iDir::iDir(const char *_name, const char *filepath, iDir *_parent):
    iNode(_name, filepath, _parent)
{
    type = NTYPE_DIR;
}

iDir::~iDir()
{
    flush();
}

void iDir::addNode(iNode *nw)
{
    nodes.push_back(nw);
}

iNode *iDir::getNode(const char *_name)
{
    iNode *founded = NULL;

    if ( strcmp(_name, ".") == 0 )
        return this;
    else if ( strcmp(_name, "..") == 0 )
        return parent;
    else
    {
        for (std::list<iNode *>::iterator it = nodes.begin(); it != nodes.end(); it++)
        {
            if ( !strcasecmp(_name, (*it)->getName()) )
            {
                founded = *it;
                break;
            }
        }
    }

    return founded;
}

void iDir::flush()
{
    for (std::list<iNode *>::iterator it = nodes.begin(); it != nodes.end(); it++)
        delete *it;

    nodes.clear();
}


void iDir::_dumpdir()
{
    std::string pth;
    getVPath(pth);
    printf("%s \t(%s)\n", pth.c_str(), getPath());

    for (std::list<iNode *>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        if ( (*it)->getType() == iNode::NTYPE_FILE )
        {
            printf("%s%c%s \t(%s)\n", pth.c_str(), FSD, (*it)->getName(), (*it)->getPath());
        }
    }

    for (std::list<iNode *>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        if ( (*it)->getType() == iNode::NTYPE_DIR )
        {
            ((iDir *) (*it) )->_dumpdir();
        }
    }
}


#if defined(WIN32) && !defined(__WINE__)
iDir *iDir::_scanDir(iDir *_node, const char *_name, const char *_path, iDir *_parent)
{
    std::string tmp = _path;
    tmp.append("\\*");

    WIN32_FIND_DATA fdata;
    HANDLE hf = FindFirstFile(tmp.c_str(), &fdata);
    if (hf == INVALID_HANDLE_VALUE)
        return NULL;

    tmp.pop_back(); // delete *

    iDir *ndr;

    if ( !_node )
        ndr = new iDir(_name, _path, _parent);
    else
    {
        _node->name.assign(_name);
        _node->path.assign(_path);
        _node->parent = _parent;

        ndr = _node;
    }

    do
    {
        if ( strcmp(fdata.cFileName, ".") != 0 && strcmp(fdata.cFileName, "..") != 0 )
        {
            std::string tmp2 = tmp;
            tmp2.append(fdata.cFileName);

            if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                iNode *sub = _scanDir(NULL, fdata.cFileName, tmp2.c_str(), ndr);
                if (sub)
                    ndr->addNode(sub);
            }
            else
            {
                ndr->addNode(new iFile(fdata.cFileName, tmp2.c_str(), ndr));
            }
        }
    }
    while ( FindNextFile(hf, &fdata) != 0);

    FindClose(hf);

    return ndr;
}
#else
iDir *iDir::_scanDir(iDir *_node, const char *_name, const char *_path, iDir *_parent)
{
    DIR *dr = opendir(_path);
    if (!dr)
        return NULL;

    iDir *ndr;

    if ( !_node )
        ndr = new iDir(_name, _path, _parent);
    else
    {
        _node->name.assign(_name);
        _node->path.assign(_path);
        _node->parent = _parent;

        ndr = _node;
    }

    std::string tmp = _path;
    tmp.append("/");

    for (dirent *ent = readdir(dr); ent; ent = readdir(dr))
    {
        if (ent->d_type == DT_REG || ent->d_type == DT_DIR)
        {
            if ( strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0 )
            {
                std::string tmp2 = tmp;
                tmp2.append(ent->d_name);

                if (ent->d_type == DT_REG)
                {
                    ndr->addNode(new iFile(ent->d_name, tmp2.c_str(), ndr) );
                }
                else
                {
                    iNode *sub = _scanDir(NULL, ent->d_name, tmp2.c_str(), ndr);
                    if (sub)
                        ndr->addNode(sub);
                }
            }
        }
    }

    closedir(dr);

    return ndr;
}
#endif // WIN32


void iDir::setBaseDir(const char *_path)
{
    std::string tmp = _path;

    if (tmp.length() > 1 && (tmp.back() == '\\' || tmp.back() == '/') )
        tmp.pop_back();

    if (tmp == "")
        tmp = ".";

    directories.flush();

    _scanDir(&directories, "", tmp.c_str(), NULL);
}

iNode *iDir::_createNodeFromPath(const char *diskPath)
{
    iNode *node = NULL;

    std::string tmp = diskPath;
    if (tmp.length() > 1 && (tmp.back() == '\\' || tmp.back() == '/') )
        tmp.pop_back();

    if (tmp == "")
        tmp = ".";

    std::string tmpName = tmp;

    size_t ps = tmpName.find_last_of("\\/");

    if (ps != std::string::npos)
        tmpName = tmpName.substr(ps + 1);

#if defined(WIN32) && !defined(__WINE__)
    DWORD attr = GetFileAttributes(diskPath);
    if ( attr != INVALID_FILE_ATTRIBUTES)
    {
        if ( attr & FILE_ATTRIBUTE_DIRECTORY )
            node = _scanDir(NULL, tmpName.c_str(), tmp.c_str(), NULL);
        else
            node = new iFile(tmpName.c_str(), tmp.c_str(), NULL);
    }
#else
    struct stat attr;

    if ( stat(diskPath, &attr) == 0)
    {
        if ( S_ISDIR(attr.st_mode) )
            node = _scanDir(NULL, tmpName.c_str(), tmp.c_str(), NULL);
        else if ( S_ISREG(attr.st_mode) )
            node = new iFile(tmpName.c_str(), tmp.c_str(), NULL);
    }
#endif // WIN32

    return node;
}


bool iDir::replacePath(const char *path, const char *diskPath)
{
    std::string leaved;
    iNode *oldNode = _parseNodePath(path, leaved);

    if (!oldNode || leaved != "")
        return false;

    if (oldNode->parent == NULL) //It's root node, we can't replace it
        return false;

    iNode *newNode = _createNodeFromPath(diskPath);

    if (!newNode)
        return false;

    iDir *upNode = oldNode->parent;

    for (std::list<iNode *>::iterator it = upNode->nodes.begin(); it != upNode->nodes.end(); it++)
    {
        if ( *it == oldNode )
        {
            newNode->name = oldNode->getName();
            newNode->parent = upNode;
            *it = newNode;

            delete oldNode;
            return true;
        }
    }

    delete newNode;
    return false;
}





iNode *iDir::_parseNodePath(const char *path, std::string &out)
{
    iDir *curdir = &directories;
    iNode *node = curdir;
    out.assign("");

    int start = 0;
    int letters = 0;
    int i = 0;

    while( path[i] != 0 )
    {
        if ( path[i] == '/' || path[i] == '\\' )
        {
            if (letters > 0)
            {
                std::string tmp;
                tmp.assign(path + start, letters);

                iNode *nd = curdir->getNode(tmp.c_str());

                if (!nd)
                {
                    out.assign(path + start);
                    return node;
                }
                else
                {
                    if (nd->getType() == iNode::NTYPE_DIR)
                    {
                        curdir = (iDir *)nd;

                        if (!curdir)
                            return NULL;

                        node = nd;
                    }
                    else
                        return nd;
                }
            }

            start = i + 1;
            letters = 0;
        }
        else
        {
            letters++;
        }

        i++;
    }

    if (letters > 0)
    {
        std::string tmp;
        tmp.assign(path + start, letters);

        iNode *nd = curdir->getNode(tmp.c_str());

        if (!nd)
        {
            out.assign(path + start);
            return node;
        }

        node = nd;
    }

    return node;
}


bool iDir::createDir(const char *path)
{
    std::string leaved;
    iNode *node = _parseNodePath(path, leaved);

    if (node)
    {
        if ( node->getType() != NTYPE_DIR )
            return false;

        if ( leaved != "") //If not exist
        {
            if (leaved.length() > 1 && (leaved.back() == '\\' || leaved.back() == '/') )
                leaved.pop_back();

            if ( leaved.find_first_of("\\/") == std::string::npos )
            {
                std::string newPath = node->getPath();
                newPath += FSD;
                newPath += leaved;

                iDir *newDir = NULL;
#if defined(WIN32) && !defined(__WINE__)
                if (CreateDirectory(newPath.c_str(), NULL))
                    newDir = new iDir(leaved.c_str(), newPath.c_str(), (iDir *)node);
#else
                if (mkdir(newPath.c_str(), 0755) == 0)
                    newDir = new iDir(leaved.c_str(), newPath.c_str(), (iDir *)node);
#endif // WIN32

                if (!newDir)
                    return false;

                ((iDir *)node)->addNode(newDir);
            }
            else
                return false;
        }
    }
    else
        return false;

    return true;
}

bool iDir::deleteDir(const char *path)
{
    std::string leaved;
    iNode *node = _parseNodePath(path, leaved);

    if (node)
    {
        if ( node->getType() != NTYPE_DIR )
            return false;

        if ( leaved != "") //If not exist
            return false;

        if ( node->parent == NULL )
            return false;

#if defined(WIN32) && !defined(__WINE__)
        if ( !RemoveDirectory(node->getPath()) )
            return false;
#else
        if (rmdir(node->getPath()) == -1)
            return false;
#endif // WIN32

        node->parent->nodes.remove(node);
        delete node;
    }
    else
        return false;

    return true;
}

DirIter *iDir::readDir(const char *path)
{
    std::string leaved;
    iNode *node = _parseNodePath(path, leaved);

    if (!node)
        return NULL;

    if ( node->getType() != NTYPE_DIR )
        return NULL;

    if ( leaved != "") //If not exist
        return NULL;

    return new DirIter( (iDir *)node );
}


iNode *iDir::findNode(const char *path)
{
    std::string leaved;
    iNode *node = _parseNodePath(path, leaved);

    if (!node)
        return NULL;

    if ( leaved != "") //If not exist
        return NULL;

    return node;
}

bool iDir::fileExist(const char *path)
{
    iNode *tmp = findNode(path);

    if (!tmp)
        return false;

    if (tmp->getType() != NTYPE_FILE)
        return false;

    return true;
}


FileHandle *iDir::openFile(iNode *nod, const char *mode)
{
    if (!nod)
        return NULL;

    if ( nod->getType() != NTYPE_FILE )
        return NULL;

    FileHandle * fhnd = new FileHandle(nod->getPath(), mode);
    if (!fhnd->OK())
    {
        delete fhnd;
        return NULL;
    }

    return fhnd;
}

FileHandle *iDir::openFile(const char *path, const char *mode)
{
    std::string leaved;
    iNode *node = _parseNodePath(path, leaved);

    if (!node)
        return NULL;

    if ( leaved != "" ) // Not exists
    {
        if (*mode == 'r') //it's must be exist for reading
            return NULL;

        if (node->getType() != NTYPE_DIR ) // If node not dir
            return NULL;

        if ( leaved.find_first_of("\\/") != std::string::npos ) // With path
            return NULL;

        iDir *dr = (iDir *)node;

        std::string newPath = dr->getPath();
        newPath += FSD;
        newPath += leaved;

        FileHandle * fhnd = new FileHandle(newPath.c_str(), mode);

        if (!fhnd->OK())
        {
            delete fhnd;
            return NULL;
        }

        dr->addNode( new iFile(leaved.c_str(), newPath.c_str(), dr) );
        return fhnd;
    }
    else if ( node->getType() == NTYPE_FILE ) // if exist and file
    {
        FileHandle * fhnd = new FileHandle(node->getPath(), mode);

        if (!fhnd->OK())
        {
            delete fhnd;
            return NULL;
        }

        return fhnd;
    }

    return NULL;
}


bool iDir::deleteFile(const char *path)
{
    iNode *node = findNode(path);

    if (!node)
        return false;

    if (node->getType() != NTYPE_FILE)
        return false;

#if defined(WIN32) && !defined(__WINE__)
    if (DeleteFile(node->getPath()) == 0)
        return false;
#else
    if (remove(node->getPath()) != 0)
        return false;
#endif // WIN32

    if (node->parent)
        node->parent->nodes.remove(node);

    delete node;

    return true;
}





void dumpDir()
{
    directories._dumpdir();
}


DirIter::DirIter(iDir *dr)
{
    lst = &dr->nodes;
    cur = dr->nodes.begin();
}

iNode *DirIter::getNext()
{
    if (cur == lst->end())
        return NULL;

    iNode *ret = *cur;
    cur++;

    return ret;
}

bool DirIter::getNext(iNode * &node)
{
    if (cur == lst->end())
    {
        node = NULL;
        return false;
    }

    node = *cur;
    cur++;

    return true;
}


FileHandle::FileHandle(const char *diskPath, const char *mode)
{
    hndl = fopen(diskPath, mode);
    _ReadERR = false;
}

FileHandle::~FileHandle()
{
    if (hndl)
        fclose(hndl);
}

bool FileHandle::OK()
{
    if (hndl)
        return true;
    else
        return false;
}

bool FileHandle::eof()
{
    if (!hndl)
        return true;

    return feof(hndl) != 0;
}

bool FileHandle::readErr()
{
    bool tmp = _ReadERR;
    _ReadERR = false;
    return tmp;
}

size_t FileHandle::read(void *buf, size_t num)
{
    if (!hndl)
        return 0;

    size_t sz = fread(buf, 1, num, hndl);

    if (sz != num)
        _ReadERR = true;

    return sz;
}

size_t FileHandle::write(const void *buf, size_t num)
{
    if (!hndl)
        return 0;

    return fwrite(buf, 1, num, hndl);
}

size_t FileHandle::tell()
{
    if (!hndl)
        return 0;

    return ftell(hndl);
}

int FileHandle::seek(long int offset, int origin)
{
    if (!hndl)
        return -100;

    return fseek(hndl, offset, origin);
}

char *FileHandle::gets(char *str, int num)
{
    if (!hndl)
        return NULL;

    return fgets(str, num, hndl);
}

int FileHandle::puts(const char *str)
{
    if (!hndl)
        return -100;

    return fputs(str, hndl);
}

int FileHandle::printf(const char *format, ...)
{
    if (!hndl)
        return -100;

    va_list args;
    va_start (args, format);

    int num = vfprintf(hndl, format, args);

    va_end (args);

    return num;
}

int FileHandle::vprintf(const char *format, va_list args)
{
    if (!hndl)
        return -100;

    int num = vfprintf(hndl, format, args);
    return num;
}

uint8_t FileHandle::readU8()
{
    if (!hndl)
        return 0;

    uint8_t val = 0;
    read(&val, 1);
    return val;
}

int8_t FileHandle::readS8()
{
    if (!hndl)
        return 0;

    int8_t val = 0;
    read(&val, 1);
    return val;
}

uint16_t FileHandle::readU16L()
{
    if (!hndl)
        return 0;

    uint16_t val = 0;
    read(&val, 2);

#ifdef BYTEORDER_LITTLE
    return val;
#else
    return ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#endif // BYTEORDER_LITTLE
}

int16_t FileHandle::readS16L()
{
    if (!hndl)
        return 0;

    int16_t val = 0;
    read(&val, 2);

#ifdef BYTEORDER_LITTLE
    return val;
#else
    return ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#endif // BYTEORDER_LITTLE
}

uint16_t FileHandle::readU16B()
{
    if (!hndl)
        return 0;

    uint16_t val = 0;
    read(&val, 2);

#ifdef BYTEORDER_LITTLE
    return ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#else
    return val;
#endif // BYTEORDER_LITTLE
}

int16_t FileHandle::readS16B()
{
    if (!hndl)
        return 0;

    int16_t val = 0;
    read(&val, 2);

#ifdef BYTEORDER_LITTLE
    return ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#else
    return val;
#endif // BYTEORDER_LITTLE
}

uint32_t FileHandle::readU32L()
{
    if (!hndl)
        return 0;

    uint32_t val = 0;
    read(&val, 4);

#ifdef BYTEORDER_LITTLE
    return val;
#else
    return ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
}

int32_t FileHandle::readS32L()
{
    if (!hndl)
        return 0;

    int32_t val = 0;
    read(&val, 4);

#ifdef BYTEORDER_LITTLE
    return val;
#else
    return ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
}

uint32_t FileHandle::readU32B()
{
    if (!hndl)
        return 0;

    uint32_t val = 0;
    read(&val, 4);

#ifdef BYTEORDER_LITTLE
    return ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#else
    return val;
#endif // BYTEORDER_LITTLE
}

int32_t FileHandle::readS32B()
{
    if (!hndl)
        return 0;

    int32_t val = 0;
    read(&val, 4);

#ifdef BYTEORDER_LITTLE
    return ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#else
    return val;
#endif // BYTEORDER_LITTLE
}

float FileHandle::readFloatL()
{
    if (!hndl)
        return 0.0;

    float val = 0.0;
    read(&val, 4);

#ifndef BYTEORDER_LITTLE
    uint32_t *p = (uint32_t *)&val;
    *p = ((*p & 0xFF000000) >> 24) | ((*p & 0xFF0000) >> 8) | ((*p & 0xFF00) << 8) | ((*p & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return val;
}

float FileHandle::readFloatB()
{
    if (!hndl)
        return 0.0;

    float val = 0.0;
    read(&val, 4);

#ifdef BYTEORDER_LITTLE
    uint32_t *p = (uint32_t *)&val;
    *p = ((*p & 0xFF000000) >> 24) | ((*p & 0xFF0000) >> 8) | ((*p & 0xFF00) << 8) | ((*p & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return val;
}

bool FileHandle::writeU8(uint8_t val)
{
    if (!hndl)
        return false;

    return write(&val, 1) == 1;
}

bool FileHandle::writeS8(int8_t val)
{
    if (!hndl)
        return false;

    return write(&val, 1) == 1;
}

bool FileHandle::writeU16L(uint16_t val)
{
    if (!hndl)
        return false;

#ifndef BYTEORDER_LITTLE
    val = ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#endif // BYTEORDER_LITTLE
    return write(&val, 2) == 2;
}

bool FileHandle::writeS16L(int16_t val)
{
    if (!hndl)
        return false;

#ifndef BYTEORDER_LITTLE
    val = ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#endif // BYTEORDER_LITTLE
    return write(&val, 2) == 2;
}

bool FileHandle::writeU16B(uint16_t val)
{
    if (!hndl)
        return false;

#ifdef BYTEORDER_LITTLE
    val = ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#endif // BYTEORDER_LITTLE
    return write(&val, 2) == 2;
}

bool FileHandle::writeS16B(int16_t val)
{
    if (!hndl)
        return false;

#ifdef BYTEORDER_LITTLE
    val = ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#endif // BYTEORDER_LITTLE
    return write(&val, 2) == 2;
}

bool FileHandle::writeU32L(uint32_t val)
{
    if (!hndl)
        return false;

#ifndef BYTEORDER_LITTLE
    val = ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}

bool FileHandle::writeS32L(int32_t val)
{
    if (!hndl)
        return false;

#ifndef BYTEORDER_LITTLE
    val = ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}

bool FileHandle::writeU32B(uint32_t val)
{
    if (!hndl)
        return false;

#ifdef BYTEORDER_LITTLE
    val = ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}

bool FileHandle::writeS32B(int32_t val)
{
    if (!hndl)
        return false;

#ifdef BYTEORDER_LITTLE
    val = ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}

bool FileHandle::writeFloatL(float val)
{
    if (!hndl)
        return false;

#ifndef BYTEORDER_LITTLE
    uint32_t *p = (uint32_t *)&val;
    *p = ((*p & 0xFF000000) >> 24) | ((*p & 0xFF0000) >> 8) | ((*p & 0xFF00) << 8) | ((*p & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}

bool FileHandle::writeFloatB(float val)
{
    if (!hndl)
        return false;

#ifdef BYTEORDER_LITTLE
    uint32_t *p = (uint32_t *)&val;
    *p = ((*p & 0xFF000000) >> 24) | ((*p & 0xFF0000) >> 8) | ((*p & 0xFF00) << 8) | ((*p & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}

}
