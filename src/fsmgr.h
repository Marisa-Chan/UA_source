#ifndef SFMGR_H_INCLUDED
#define SFMGR_H_INCLUDED

#define BYTEORDER_LITTLE

#undef min
#undef max

#include <string>
#include <list>
#include <inttypes.h>

namespace FSMgr
{
class DirIter;
class iDir;
class FileHandle;

class iNode
{
    friend class iDir;

public:
    iNode(const std::string &name, const std::string &path, iDir *parent = NULL);
    virtual ~iNode() {};

    int getType();
    std::string getPath() const;
    //const std::string &getPath();
    std::string getVPath() const;
    std::string getName() const;
    //const std::string &getName();

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
    iFile(const std::string &name, const std::string &path, iDir *parent = NULL);
};


class iDir: public iNode
{
    friend class DirIter;

public:
    iDir(const std::string &name, const std::string &path, iDir *parent = NULL);
    virtual ~iDir();

    void addNode(iNode *nw);

    iNode *getNode(const std::string &name);

    void _dumpdir();

    void flush();

    static iNode *findNode(const std::string &path);
    static bool fileExist(const std::string &path);

    static void setBaseDir(const std::string &path);
    static bool replacePath(const std::string &path, const std::string &diskPath);

    static bool createDir(const std::string &path);
    static bool deleteDir(const std::string &path);
    static DirIter readDir(const std::string &path);
    static FileHandle *openFile(const std::string &path, const std::string &mode);
    static FileHandle *openFile(iNode *nod, const std::string &mode);
    static bool deleteFile(const std::string &path);


protected:
    std::list<iNode *> nodes;

    static iDir *_scanDir(iDir *_node, const std::string &_name, const std::string &_path, iDir *_parent);
    static iNode *_createNodeFromPath(const std::string &diskPath);
    static iNode *_parseNodePath(const std::string &path, std::string *out);
};



class DirIter
{
public:
    DirIter(iDir *dr);
    DirIter();
    iNode *getNext();
    bool getNext(iNode **node);

    explicit operator bool() const;

private:
    iDir *_d;
    std::list<iNode *>::iterator _cur;
};

class FileHandle
{
public:
    FileHandle(const std::string &diskPath, const std::string &mode);
    virtual ~FileHandle();

    size_t read(void *buf, size_t num);
    size_t write(const void *buf, size_t num);

    size_t tell();
    int seek(long int offset, int origin);
    char *gets(char *str, int num);
    int puts(const std::string &str);
    int printf(const std::string &format, ...);
    int vprintf(const std::string &format,va_list args);
    bool ReadLine(std::string *out);

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

    bool eof();
    bool readErr();

    virtual bool OK();

    static void closeFile(FileHandle *fl) {
        if (fl) delete fl;
    };

protected:
    FILE *hndl;

private:
    bool _ReadERR;
};

void dumpDir();



}

#endif
