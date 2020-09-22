#ifndef DEFPARSER_H_INCLUDED
#define DEFPARSER_H_INCLUDED

#include <vector>
#include <string>
#include "fsmgr.h"
#include "types.h"


namespace ScriptParser
{
class Parser;


enum FLAGS
{
    FLAG_NO_SCOPE_SKIP  = 1,
    FLAG_NO_INCLUDE	    = 2
};

enum RESULTS
{
    RESULT_OK,
    RESULT_SCOPE_END,
    RESULT_UNKNOWN,
    RESULT_BAD_DATA,
    RESULT_UNEXP_EOF,
    RESULT_EOF,
};

class DataHandler
{
public:
    virtual ~DataHandler() {};

    /*** Data handling method
         p1 - often key
         p2 - data for key-value pairs or empty in cases like "begin_bg" or other
    ***/
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2) = 0;

    /** Generally method called for check is it needed scope
        But in some situations can do some init or do read
        stored data by parser.ReadLine
    **/

    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt) = 0;
};


class HandlersList: public std::vector<DataHandler *>
{
using std::vector<DataHandler *>::vector;
//using std::vector<DataHandler *>::push_back;
public:
    virtual ~HandlersList()
    {
        for (uint32_t i = 0; i < size(); i++)
            delete at(i);
    };

    void operator+=(DataHandler *b)
    {
        push_back(b);
    }
};


class Parser
{
private:
    enum
    {
        MODE_NO    = 0,
        MODE_FILE  = 1,
        MODE_SLIST = 2
    };

public:
    static bool ParseFile(const std::string &filename, HandlersList &callbacks, int flags = 0);
    static bool ParseStringList(const Engine::StringList &slist, HandlersList &callbacks, int flags = 0);

public:
    /** Useable from handlers **/
    bool ReadLine(std::string *out);

private:
    Parser(const std::string &name) : _Name(name), _file(NULL), _line(0), _mode(MODE_NO) {};
    int ParseRoutine(const std::string &filename, HandlersList &callbacks, int flags);
    bool ParseNextLine(std::string *p1, std::string *p2);
    
public:
    const std::string _Name;
private:
    FSMgr::FileHandle *_file;
    int _line;
    Engine::StringList::const_iterator _strlistIt;
    Engine::StringList::const_iterator _strlistEnd;
    int _mode;
};

/** Sort of sugar **/
static constexpr auto &ParseFile = Parser::ParseFile;
static constexpr auto &ParseStringList = Parser::ParseStringList;
}

#endif //DEFPARSER_H_INCLUDED
