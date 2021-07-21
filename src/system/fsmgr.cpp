#include <string.h>
#include <stdarg.h>
#include <inttypes.h>

#if defined(WIN32) && !defined(__WINE__)

#include <windows.h>

#else

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#endif

#include "fsmgr.h"
#include "../utils.h"


namespace FSMgr
{
#if defined(WIN32) && !defined(__WINE__)
const static std::string FSD("\\");
#else
const static std::string FSD("/");
#endif

static iDir directories("", "");

iNode::iNode(const std::string &_name, const std::string &filepath)
{
    name = _name;
    path = filepath;
}

int iNode::getType()
{
    return type;
}

std::string iNode::getPath() const
{
    return path;
}

std::string iNode::getName() const
{
    return name;
}

std::string iNode::getVPath() const
{
    if (parent != NULL)
        return parent->getVPath() + FSD + name;
    else
        return name;
}

bool iNode::Detach()
{
    if (!parent)
        return true;

    return parent->Detach(this);
}




iFile::iFile(const std::string &_name, const std::string &filepath):
    iNode(_name, filepath)
{
    type = NTYPE_FILE;
}


iDir::iDir(const std::string &_name, const std::string &filepath):
    iNode(_name, filepath)
{
    type = NTYPE_DIR;
}

iDir::~iDir()
{
    flush();
}

iDir *iDir::GetRoot()
{
    return &directories;
}

void iDir::addNode(iNode *nw)
{
    nw->Detach();
    nw->parent = this;
    nodes.push_back(nw);
}

iNode *iDir::getNode(const std::string &n)
{
    if ( n == "." )
        return this;
    else if ( n == ".." )
        return parent;
    else
    {
        for (const auto &i : nodes)
        {
            if ( !StriCmp(n, i->name) )
                return i;
        }
    }

    return NULL;
}

void iDir::flush()
{
    while(!nodes.empty())
    {
        iNode *n = nodes.front();
        nodes.pop_front();
        
        n->parent = NULL;
        delete n;
    }
}


void iDir::_dumpdir()
{
    std::string pth = getVPath();
    printf("%s \t(%s)\n", pth.c_str(), getPath().c_str());

    for (std::list<iNode *>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        if ( (*it)->getType() == iNode::NTYPE_FILE )
        {
            printf("%s%s%s \t(%s)\n", pth.c_str(), FSD.c_str(), (*it)->getName().c_str(), (*it)->getPath().c_str());
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
iDir *iDir::_scanDir(iDir *_node, const std::string &_name, const std::string &_path, iDir *_parent)
{
    std::string tmp = _path + "\\*";

    WIN32_FIND_DATA fdata;
    HANDLE hf = FindFirstFile(tmp.c_str(), &fdata);
    if (hf == INVALID_HANDLE_VALUE)
        return NULL;

    tmp.pop_back(); // delete *

    iDir *ndr;

    if ( !_node )
    {
        ndr = new iDir(_name, _path);
        ndr->parent = _parent;
    }
    else
    {
        _node->name = _name;
        _node->path = _path;
        _node->parent = _parent;

        ndr = _node;
    }

    do
    {
        if ( strcmp(fdata.cFileName, ".") != 0 && strcmp(fdata.cFileName, "..") != 0 )
        {
            std::string tmp2 = tmp + fdata.cFileName;

            if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                iNode *sub = _scanDir(NULL, fdata.cFileName, tmp2, ndr);
                if (sub)
                    ndr->addNode(sub);
            }
            else
            {
                ndr->addNode(new iFile(fdata.cFileName, tmp2));
            }
        }
    }
    while ( FindNextFile(hf, &fdata) != 0);

    FindClose(hf);

    return ndr;
}
#else
iDir *iDir::_scanDir(iDir *_node, const std::string &_name, const std::string &_path, iDir *_parent)
{
    DIR *dr = opendir(_path.c_str());
    if (!dr)
        return NULL;

    iDir *ndr;

    if ( !_node )
    {
        ndr = new iDir(_name, _path);
        ndr->parent = _parent;
    }
    else
    {
        _node->name = _name;
        _node->path = _path;
        _node->parent = _parent;

        ndr = _node;
    }

    std::string tmp = _path + "/";

    for (dirent *ent = readdir(dr); ent; ent = readdir(dr))
    {
        if (ent->d_type == DT_REG || ent->d_type == DT_DIR)
        {
            if ( strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0 )
            {
                std::string tmp2 = tmp + ent->d_name;

                if (ent->d_type == DT_REG)
                {
                    ndr->addNode(new iFile(ent->d_name, tmp2) );
                }
                else
                {
                    iNode *sub = _scanDir(NULL, ent->d_name, tmp2, ndr);
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


void iDir::setBaseDir(const std::string &_path)
{
    std::string tmp = _path;

    if (!tmp.empty() && (tmp.back() == '\\' || tmp.back() == '/') )
        tmp.pop_back();

    if (tmp.empty())
        tmp = ".";

    directories.flush();

    _scanDir(&directories, "", tmp, NULL);
}

iNode *iDir::_createNodeFromPath(const std::string &diskPath)
{
    iNode *node = NULL;

    std::string tmp = diskPath;
    if (!tmp.empty() && (tmp.back() == '\\' || tmp.back() == '/') )
        tmp.pop_back();

    if (tmp.empty())
        tmp = ".";

    std::string tmpName = tmp;

    size_t ps = tmpName.find_last_of("\\/");

    if (ps != std::string::npos)
        tmpName = tmpName.substr(ps + 1);

#if defined(WIN32) && !defined(__WINE__)
    DWORD attr = GetFileAttributes(diskPath.c_str());
    if ( attr != INVALID_FILE_ATTRIBUTES)
    {
        if ( attr & FILE_ATTRIBUTE_DIRECTORY )
            node = _scanDir(NULL, tmpName, tmp, NULL);
        else
            node = new iFile(tmpName, tmp);
    }
#else
    struct stat attr;

    if ( stat(diskPath.c_str(), &attr) == 0)
    {
        if ( S_ISDIR(attr.st_mode) )
            node = _scanDir(NULL, tmpName, tmp, NULL);
        else if ( S_ISREG(attr.st_mode) )
            node = new iFile(tmpName, tmp);
    }
#endif // WIN32

    return node;
}


bool iDir::replacePath(const std::string &path, const std::string &diskPath)
{
    std::string leaved;
    iNode *oldNode = directories._parseNodePath(path, &leaved);

    if (!oldNode || !leaved.empty())
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
            newNode->name = oldNode->name;
            newNode->parent = upNode;
            *it = newNode;

            delete oldNode;
            return true;
        }
    }

    delete newNode;
    return false;
}





iNode *iDir::_parseNodePath(const std::string &vpath, std::string *out)
{
    iDir *curdir = this;
    iNode *node = this;
    out->clear();

    size_t pos = 0;
    while(pos != std::string::npos)
    {
        size_t start = pos;
        pos = vpath.find_first_of("/\\", start);

        std::string vname;
        if (pos == std::string::npos)
            vname = vpath.substr(start);
        else
        {
            vname = vpath.substr(start, pos - start);
            pos++; //Next symbol
        }

        if (!vname.empty())
        {
            iNode *nd = curdir->getNode( vname );

            if (!nd)
            {
                *out = vpath.substr(start);
                return node;
            }
            else
            {
                if (nd->getType() == iNode::NTYPE_DIR)
                {
                    curdir = (iDir *)nd;
                    node = nd;
                }
                else
                    return nd;
            }
        }
    }

    return node;
}


bool iDir::createDir(const std::string &path)
{
    std::string leaved;
    iNode *node = directories._parseNodePath(path, &leaved);

    if (node)
    {
        if ( node->getType() != NTYPE_DIR )
            return false;

        if ( !leaved.empty() ) //If not exist
        {
            if (leaved.length() > 1 && (leaved.back() == '\\' || leaved.back() == '/') )
                leaved.pop_back();

            if ( leaved.find_first_of("\\/") == std::string::npos )
            {
                std::string newPath = node->path + FSD + leaved;

                iDir *newDir = NULL;
#if defined(WIN32) && !defined(__WINE__)
                if (CreateDirectory(newPath.c_str(), NULL))
                    newDir = new iDir(leaved, newPath);
#else
                if (mkdir(newPath.c_str(), 0755) == 0)
                    newDir = new iDir(leaved, newPath);
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

iDir *iDir::MakeDir(const std::string &vname)
{
    if (vname.empty() || vname.find_first_of("\\/") != std::string::npos)
        return NULL;
    
    iNode *node = getNode(vname);
    if (node)
    {
        if ( node->getType() != NTYPE_DIR )
            return NULL;
        return (iDir *)node;
    }
    
    std::string newPath = path + FSD + vname;
    
    iDir *newDir = NULL;
#if defined(WIN32) && !defined(__WINE__)
    if (CreateDirectory(newPath.c_str(), NULL))
        newDir = new iDir(vname, newPath);
#else
    if (mkdir(newPath.c_str(), 0755) == 0)
        newDir = new iDir(vname, newPath);
#endif // WIN32

    if (!newDir)
        return NULL;

    addNode(newDir);
    return newDir;
}

bool iDir::deleteDir(const std::string &path)
{
    std::string leaved;
    iNode *node = directories._parseNodePath(path, &leaved);

    if (node)
    {
        if ( node->getType() != NTYPE_DIR )
            return false;

        if ( !leaved.empty() ) //If not exist
            return false;

        if ( node->parent == NULL )
            return false;

#if defined(WIN32) && !defined(__WINE__)
        if ( !RemoveDirectory(node->path.c_str()) )
            return false;
#else
        if (rmdir(node->path.c_str()) == -1)
            return false;
#endif // WIN32

        node->parent->nodes.remove(node);
        delete node;
    }
    else
        return false;

    return true;
}

DirIter iDir::readDir(const std::string &path)
{
    std::string leaved;
    iNode *node = directories._parseNodePath(path, &leaved);

    if (!node)
        return DirIter();

    if ( node->getType() != NTYPE_DIR )
        return DirIter();

    if ( !leaved.empty() ) //If not exist
        return DirIter();

    return DirIter( (iDir *)node );
}

void iDir::Override(iDir *nod)
{
    path = nod->path;
            
    for(std::list<iNode *>::iterator it = nod->nodes.begin(); 
        it != nod->nodes.end(); 
        it = nodes.erase(it))
    {
        iNode *nev = *it;
        // Will be erased from list, so do silent detach
        nev->parent = NULL;
        
        iNode *old = getNode((*it)->name);
        
        if (old)
        {
            // Do mix by override
            if ( nev->type == NTYPE_DIR &&
                 nev->type == old->type )
            {
                dynamic_cast<iDir *>(old)->Override( dynamic_cast<iDir *>(nev) );
                delete nev;
            }
            else // Replace
            {
                delete old;
                
                addNode(nev);
            }
        }
        else
            addNode(nev);
    }
}


iNode *iDir::findNode(const std::string &path)
{
    if (path.empty())
        return &directories;
        
    std::string leaved;
    iNode *node = directories._parseNodePath(path, &leaved);

    if (!node)
        return NULL;

    if ( !leaved.empty() ) //If not exist
        return NULL;

    return node;
}

bool iDir::fileExist(const std::string &path)
{
    iNode *tmp = findNode(path);

    if (!tmp)
        return false;

    if (tmp->getType() != NTYPE_FILE)
        return false;

    return true;
}


FileHandle *iDir::openFileAlloc(iNode *nod, const std::string &mode)
{
    if (!nod)
        return NULL;

    if ( nod->getType() != NTYPE_FILE )
        return NULL;

    FileHandle * fhnd = new FileHandle(nod->path, mode);
    if (!fhnd->OK())
    {
        delete fhnd;
        return NULL;
    }

    return fhnd;
}

FileHandle *iDir::openFileAlloc(const std::string &path, const std::string &mode)
{
    std::string leaved;
    iNode *node = directories._parseNodePath(path, &leaved);

    if (!node)
        return NULL;

    if ( !leaved.empty() ) // Not exists
    {
        if (mode.find('r') != std::string::npos) //it's must be exist for reading
            return NULL;

        if (node->getType() != NTYPE_DIR ) // If node not dir
            return NULL;

        if ( leaved.find_first_of("\\/") != std::string::npos ) // With path
            return NULL;
        
        if (node->getType() != NTYPE_DIR)
            return NULL;

        iDir *dr = (iDir *)node;

        std::string newPath = dr->path + FSD + leaved;

        FileHandle * fhnd = new FileHandle(newPath, mode);

        if (!fhnd->OK())
        {
            delete fhnd;
            return NULL;
        }

        dr->addNode( new iFile(leaved, newPath) );
        return fhnd;
    }
    else if ( node->getType() == NTYPE_FILE ) // if exist and file
    {
        FileHandle * fhnd = new FileHandle(node->path, mode);

        if (!fhnd->OK())
        {
            delete fhnd;
            return NULL;
        }

        return fhnd;
    }

    return NULL;
}

FileHandle iDir::openFile(iNode *nod, const std::string &mode)
{
    if (!nod)
        return NULL;

    if ( nod->getType() != NTYPE_FILE )
        return NULL;

    return FileHandle(nod->path, mode);
}

FileHandle iDir::openFile(const std::string &path, const std::string &mode)
{
    std::string leaved;
    iNode *node = directories._parseNodePath(path, &leaved);

    if (!node)
        return FileHandle();

    if ( !leaved.empty() ) // Not exists
    {
        if (mode.find('r') != std::string::npos) //it's must be exist for reading
            return FileHandle();

        if (node->getType() != NTYPE_DIR ) // If node not dir
            return FileHandle();

        if ( leaved.find_first_of("\\/") != std::string::npos ) // With path
            return FileHandle();
        
        if (node->getType() != NTYPE_DIR)
            return FileHandle();

        iDir *dr = (iDir *)node;

        std::string newPath = dr->path + FSD + leaved;

        FileHandle fhnd = FileHandle(newPath, mode);

        if (!fhnd.OK())
            return fhnd;

        dr->addNode( new iFile(leaved, newPath) );
        return fhnd;
    }
    else if ( node->getType() == NTYPE_FILE ) // if exist and file
    {
        return FileHandle(node->path, mode);
    }

    return FileHandle();    
}


bool iDir::deleteFile(const std::string &path)
{
    iNode *node = findNode(path);

    if (!node)
        return false;

    if (node->getType() != NTYPE_FILE)
        return false;

#if defined(WIN32) && !defined(__WINE__)
    if (DeleteFile(node->path.c_str()) == 0)
        return false;
#else
    if (remove(node->path.c_str()) != 0)
        return false;
#endif // WIN32

    if (node->parent)
        node->parent->nodes.remove(node);

    delete node;

    return true;
}

bool iDir::Detach(iNode *node)
{
    for(std::list<iNode *>::iterator nod = nodes.begin(); nod != nodes.end(); nod++)
    {
        if (*nod == node)
        {
            if (node->parent == this)
                node->parent = NULL;
            nodes.erase(nod);
            return true;
        }
    }
    
    return false;
}

bool iDir::Detach(const std::string &vname)
{
    for(std::list<iNode *>::iterator nod = nodes.begin(); nod != nodes.end(); nod++)
    {
        if (!StriCmp((*nod)->name, vname))
        {
            (*nod)->parent = NULL;
            delete *nod;
            nodes.erase(nod);
            return true;
        }
    }
    return false;
}

void dumpDir()
{
    directories._dumpdir();
}


DirIter::DirIter(iDir *dr)
: _d(dr), _cur(dr->nodes.begin())
{
}

DirIter::DirIter()
: _d(NULL)
{
}

iNode *DirIter::getNext()
{
    if (!_d || _cur == _d->nodes.end())
        return NULL;

    iNode *ret = *_cur;
    _cur++;

    return ret;
}

bool DirIter::getNext(iNode **node)
{
    if (!_d || _cur == _d->nodes.end())
    {
        *node = NULL;
        return false;
    }

    *node = *_cur;
    _cur++;

    return true;
}

DirIter::operator bool() const
{
    return _d != NULL;
}


FileHandle::FileHandle(const std::string &diskPath, const std::string &mode)
{
    hndl = fopen(diskPath.c_str(), mode.c_str());
    
    if (mode.find("w") != std::string::npos)
        _writeMode = true;
}

FileHandle::FileHandle(FileHandle &&b)
{
    hndl = b.hndl;
    b.hndl = NULL;
    
    _writeMode = b._writeMode;
    _ReadERR = b._ReadERR;
}

FileHandle::FileHandle(FileHandle *b, bool del)
{
    if (b)
    {
        hndl = b->hndl;
        b->hndl = NULL;

        _writeMode = b->_writeMode;
        _ReadERR = b->_ReadERR;

        if (del)
            delete b;
    }
}

FileHandle& FileHandle::operator=(FileHandle &&b)
{
    if (hndl)
        fclose(hndl);
    
    hndl = b.hndl;
    b.hndl = NULL;
    
    _writeMode = b._writeMode;
    _ReadERR = b._ReadERR;
    return *this;
}

FileHandle::~FileHandle()
{
    if (hndl)
        fclose(hndl);
}

bool FileHandle::OK() const
{
    if (hndl)
        return true;
    else
        return false;
}

bool FileHandle::eof() const
{
    if (!hndl)
        return true;

    return feof(hndl) != 0;
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

    size_t sz = fwrite(buf, 1, num, hndl);
    if (sz != num)
        _WriteERR = true;

    return sz;
}

size_t FileHandle::tell() const
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

void FileHandle::close()
{
    if (hndl)
    {
        fclose(hndl);
        hndl = NULL;
    }
}

char *FileHandle::gets(char *str, int num)
{
    if (!hndl)
        return NULL;

    return fgets(str, num, hndl);
}

int FileHandle::puts(const std::string &str)
{
    if (!hndl)
        return -100;

    return fputs(str.c_str(), hndl);
}

int FileHandle::printf(const std::string &format, ...)
{
    if (!hndl)
        return -100;

    va_list args;
    va_start (args, format);

    int num = vfprintf(hndl, format.c_str(), args);

    va_end (args);

    return num;
}

int FileHandle::vprintf(const std::string &format, va_list args)
{
    if (!hndl)
        return -100;

    int num = vfprintf(hndl, format.c_str(), args);
    return num;
}


bool FileHandle::ReadLine(std::string *out)
{
	if (!hndl)
        return false;

	out->clear();
	char buf[256];
	bool ok = false;

	while(fgets(buf, 256, hndl))
	{
		ok = true;
		(*out) += buf;
		if (out->back() == '\n' || out->back() == '\r')
			break;
	}
	return ok;
}

bool iFileHandle::readErr()
{
    bool tmp = _ReadERR;
    _ReadERR = false;
    return tmp;
}

uint8_t iFileHandle::readU8()
{
    if (!OK())
        return 0;

    uint8_t val = 0;
    if (read(&val, 1) != 1)
        _ReadERR = true;
    return val;
}

int8_t iFileHandle::readS8()
{
    if (!OK())
        return 0;

    int8_t val = 0;
    if (read(&val, 1) != 1)
        _ReadERR = true;
    return val;
}

uint16_t iFileHandle::readU16L()
{
    if (!OK())
        return 0;

    uint16_t val = 0;
    if (read(&val, 2) != 2)
        _ReadERR = true;

#ifdef BYTEORDER_LITTLE
    return val;
#else
    return ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#endif // BYTEORDER_LITTLE
}

int16_t iFileHandle::readS16L()
{
    if (!OK())
        return 0;

    int16_t val = 0;
    if (read(&val, 2) != 2)
        _ReadERR = true;

#ifdef BYTEORDER_LITTLE
    return val;
#else
    return ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#endif // BYTEORDER_LITTLE
}

uint16_t iFileHandle::readU16B()
{
    if (!OK())
        return 0;

    uint16_t val = 0;
    if (read(&val, 2) != 2)
        _ReadERR = true;

#ifdef BYTEORDER_LITTLE
    return ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#else
    return val;
#endif // BYTEORDER_LITTLE
}

int16_t iFileHandle::readS16B()
{
    if (!OK())
        return 0;

    int16_t val = 0;
    if (read(&val, 2) != 2)
        _ReadERR = true;

#ifdef BYTEORDER_LITTLE
    return ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#else
    return val;
#endif // BYTEORDER_LITTLE
}

uint32_t iFileHandle::readU32L()
{
    if (!OK())
        return 0;

    uint32_t val = 0;
    if (read(&val, 4) != 4)
        _ReadERR = true;

#ifdef BYTEORDER_LITTLE
    return val;
#else
    return ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
}

int32_t iFileHandle::readS32L()
{
    if (!OK())
        return 0;

    int32_t val = 0;
    if (read(&val, 4) != 4)
        _ReadERR = true;

#ifdef BYTEORDER_LITTLE
    return val;
#else
    return ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
}

uint32_t iFileHandle::readU32B()
{
    if (!OK())
        return 0;

    uint32_t val = 0;
    if (read(&val, 4) != 4)
        _ReadERR = true;

#ifdef BYTEORDER_LITTLE
    return ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#else
    return val;
#endif // BYTEORDER_LITTLE
}

int32_t iFileHandle::readS32B()
{
    if (!OK())
        return 0;

    int32_t val = 0;
    if (read(&val, 4) != 4)
        _ReadERR = true;

#ifdef BYTEORDER_LITTLE
    return ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#else
    return val;
#endif // BYTEORDER_LITTLE
}

float iFileHandle::readFloatL()
{
    if (!OK())
        return 0.0;

    float val = 0.0;
    if (read(&val, 4) != 4)
        _ReadERR = true;

#ifndef BYTEORDER_LITTLE
    uint32_t *p = (uint32_t *)&val;
    *p = ((*p & 0xFF000000) >> 24) | ((*p & 0xFF0000) >> 8) | ((*p & 0xFF00) << 8) | ((*p & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return val;
}

float iFileHandle::readFloatB()
{
    if (!OK())
        return 0.0;

    float val = 0.0;
    if (read(&val, 4) != 4)
        _ReadERR = true;

#ifdef BYTEORDER_LITTLE
    uint32_t *p = (uint32_t *)&val;
    *p = ((*p & 0xFF000000) >> 24) | ((*p & 0xFF0000) >> 8) | ((*p & 0xFF00) << 8) | ((*p & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return val;
}

bool iFileHandle::writeU8(uint8_t val)
{
    if (!OK())
        return false;

    return write(&val, 1) == 1;
}

bool iFileHandle::writeS8(int8_t val)
{
    if (!OK())
        return false;

    return write(&val, 1) == 1;
}

bool iFileHandle::writeU16L(uint16_t val)
{
    if (!OK())
        return false;

#ifndef BYTEORDER_LITTLE
    val = ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#endif // BYTEORDER_LITTLE
    return write(&val, 2) == 2;
}

bool iFileHandle::writeS16L(int16_t val)
{
    if (!OK())
        return false;

#ifndef BYTEORDER_LITTLE
    val = ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#endif // BYTEORDER_LITTLE
    return write(&val, 2) == 2;
}

bool iFileHandle::writeU16B(uint16_t val)
{
    if (!OK())
        return false;

#ifdef BYTEORDER_LITTLE
    val = ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#endif // BYTEORDER_LITTLE
    return write(&val, 2) == 2;
}

bool iFileHandle::writeS16B(int16_t val)
{
    if (!OK())
        return false;

#ifdef BYTEORDER_LITTLE
    val = ((val & 0xFF00) >> 8) | ((val & 0xFF) << 8);
#endif // BYTEORDER_LITTLE
    return write(&val, 2) == 2;
}

bool iFileHandle::writeU32L(uint32_t val)
{
    if (!OK())
        return false;

#ifndef BYTEORDER_LITTLE
    val = ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}

bool iFileHandle::writeS32L(int32_t val)
{
    if (!OK())
        return false;

#ifndef BYTEORDER_LITTLE
    val = ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}

bool iFileHandle::writeU32B(uint32_t val)
{
    if (!OK())
        return false;

#ifdef BYTEORDER_LITTLE
    val = ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}

bool iFileHandle::writeS32B(int32_t val)
{
    if (!OK())
        return false;

#ifdef BYTEORDER_LITTLE
    val = ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8) | ((val & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}

bool iFileHandle::writeFloatL(float val)
{
    if (!OK())
        return false;

#ifndef BYTEORDER_LITTLE
    uint32_t *p = (uint32_t *)&val;
    *p = ((*p & 0xFF000000) >> 24) | ((*p & 0xFF0000) >> 8) | ((*p & 0xFF00) << 8) | ((*p & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}

bool iFileHandle::writeFloatB(float val)
{
    if (!OK())
        return false;

#ifdef BYTEORDER_LITTLE
    uint32_t *p = (uint32_t *)&val;
    *p = ((*p & 0xFF000000) >> 24) | ((*p & 0xFF0000) >> 8) | ((*p & 0xFF00) << 8) | ((*p & 0xFF) << 24);
#endif // BYTEORDER_LITTLE
    return write(&val, 4) == 4;
}

}
