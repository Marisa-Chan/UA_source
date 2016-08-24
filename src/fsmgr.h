#ifndef SFMGR_H_INCLUDED
#define SFMGR_H_INCLUDED

#define BYTEORDER_LITTLE

#include <string>
#include <list>

namespace FSMgr
{
class DirIter;
class iDir;
class FileHandle;

class iNode
{
friend class iDir;

public:
    iNode(const char *name, const char *path, iDir *parent = NULL);
    virtual ~iNode(){};

    int getType();
    const char *getPath();
    void getVPath(std::string &out);
    const char *getName();

    enum NTYPE
    {
        NTYPE_DIR = 1,
        NTYPE_FILE = 2
    };

protected:
    std::string path;
    std::string name;
    int type;

    iDir *parent;
};


class iFile: public iNode
{
public:
    iFile(const char *name, const char *path, iDir *parent = NULL);
};


class iDir: public iNode
{
friend class DirIter;

public:
    iDir(const char *name, const char *path, iDir *parent = NULL);
    virtual ~iDir();

    void addNode(iNode *nw);

    iNode *getNode(const char *name);

    void _dumpdir();

    void flush();

    static iNode *findNode(const char *path);
    static bool fileExist(const char *path);

    static void setBaseDir(const char *path);
    static bool replacePath(const char *path, const char *diskPath);

    static bool createDir(const char *path);
    static bool deleteDir(const char *path);
    static DirIter *readDir(const char *path);
    static FileHandle *openFile(const char *path, const char *mode);
    static FileHandle *openFile(iNode *nod, const char *mode);
    static bool deleteFile(const char *path);


protected:
    std::list<iNode *> nodes;

    static iDir *_scanDir(iDir *_node, const char *_name, const char *_path, iDir *_parent);
    static iNode *_createNodeFromPath(const char *diskPath);
    static iNode *_parseNodePath(const char *path, std::string &out);
};



class DirIter
{
public:
    DirIter(iDir *dr);
    iNode *getNext();

private:
    std::list<iNode *> *lst;
    std::list<iNode *>::iterator cur;
};

class FileHandle
{
public:
    FileHandle(const char *diskPath, const char *mode);
    virtual ~FileHandle();

    size_t read(void *buf, size_t num);
    size_t write(void *buf, size_t num);

    size_t tell();
    int seek(long int offset, int origin);

    uint8_t readU8();
    int8_t readS8();
    uint16_t readU16L();
    int16_t readS16L();
    uint16_t readU16B();
    int16_t readS16B();
    uint32_t readU32L();
    int32_t readS32L();
    uint32_t readU32B();
    int32_t readS32B();
    float readFloatL();
    float readFloatB();

    bool writeU8(uint8_t val);
    bool writeS8(int8_t val);
    bool writeU16L(uint16_t val);
    bool writeS16L(int16_t val);
    bool writeU16B(uint16_t val);
    bool writeS16B(int16_t val);
    bool writeU32L(uint32_t val);
    bool writeS32L(int32_t val);
    bool writeU32B(uint32_t val);
    bool writeS32B(int32_t val);
    bool writeFloatL(float val);
    bool writeFloatB(float val);


    virtual bool OK();

    static void closeFile(FileHandle *fl) {if (fl) delete fl;};

protected:
    FILE *hndl;
};

void dumpDir();



}

#endif
