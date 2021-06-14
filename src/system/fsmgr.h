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
    static FileHandle *openFileAlloc(const std::string &path, const std::string &mode);
    static FileHandle *openFileAlloc(iNode *nod, const std::string &mode);
    static FileHandle openFile(const std::string &path, const std::string &mode);
    static FileHandle openFile(iNode *nod, const std::string &mode);
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
    explicit DirIter(iDir *dr);
    DirIter();
    iNode *getNext();
    bool getNext(iNode **node);

    explicit operator bool() const;

private:
    iDir *_d;
    std::list<iNode *>::iterator _cur;
};


class iFileHandle
{
public:
    virtual ~iFileHandle() {};
    
    virtual void close() = 0;
    
    virtual size_t read(void *buf, size_t num) = 0;
    virtual size_t write(const void *buf, size_t num) = 0;

    virtual size_t tell() const = 0;
    virtual int seek(long int offset, int origin) = 0;
    
    virtual bool OK() const = 0;
    virtual bool eof() const = 0;
    
    virtual bool readErr();

    virtual uint8_t readU8();
    virtual int8_t readS8();
    virtual uint16_t readU16L();
    virtual int16_t readS16L();
    virtual uint16_t readU16B();
    virtual int16_t readS16B();
    virtual uint32_t readU32L();
    virtual int32_t readS32L();
    virtual uint32_t readU32B();
    virtual int32_t readS32B();
    virtual float readFloatL();
    virtual float readFloatB();

    virtual bool writeU8(uint8_t val);
    virtual bool writeS8(int8_t val);
    virtual bool writeU16L(uint16_t val);
    virtual bool writeS16L(int16_t val);
    virtual bool writeU16B(uint16_t val);
    virtual bool writeS16B(int16_t val);
    virtual bool writeU32L(uint32_t val);
    virtual bool writeS32L(int32_t val);
    virtual bool writeU32B(uint32_t val);
    virtual bool writeS32B(int32_t val);
    virtual bool writeFloatL(float val);
    virtual bool writeFloatB(float val);
    
protected:
    bool _ReadERR = false;
    bool _WriteERR = false;
};



class FileHandle : public iFileHandle
{
public:
    FileHandle() = default;
    FileHandle(const std::string &diskPath, const std::string &mode);
    virtual ~FileHandle();
    
    FileHandle(FileHandle &&b);
    FileHandle& operator=(FileHandle &&b);
    
    FileHandle(FileHandle *b, bool del = true);
    
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle &) = delete;

    virtual size_t read(void *buf, size_t num) override;
    virtual size_t write(const void *buf, size_t num)  override;

    virtual size_t tell() const override;
    virtual int seek(long int offset, int origin) override;
    
    virtual void close() override;
    
    virtual char *gets(char *str, int num);
    virtual int puts(const std::string &str);
    virtual int printf(const std::string &format, ...);
    virtual int vprintf(const std::string &format,va_list args);
    virtual bool ReadLine(std::string *out);

    virtual bool eof() const override;
    

    virtual bool OK() const override;

    static void closeFile(FileHandle *fl) {
        if (fl) delete fl;
    };
    
    bool IsWriting() const {
        return _writeMode;
    }

protected:
    FILE *hndl = NULL;
    bool _writeMode = false;
    
};

void dumpDir();



}

#endif
