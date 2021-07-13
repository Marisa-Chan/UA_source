#ifndef WORLD_SAVEPARSERS_H_INCLUDED
#define WORLD_SAVEPARSERS_H_INCLUDED

#include "parsers.h"


namespace World
{
namespace Parsers
{

class SaveBact
{
protected:
    bool SaveBactParser(ScriptParser::Parser &parser, NC_STACK_ypabact *b, const std::string &p1, const std::string &p2);

    static NC_STACK_ypabact *_lastCommander;
    static NC_STACK_yparobo *_lastRobo;
};

class SaveRoboParser : public ScriptParser::DataHandler, public SaveBact
{
public:
    SaveRoboParser(NC_STACK_ypaworld *o) : _o(*o), _r(NULL) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    bool RoboParser(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);

    NC_STACK_ypaworld &_o;
    NC_STACK_yparobo *_r;
};

class SaveSquadParser : public ScriptParser::DataHandler, public SaveBact
{
public:
    SaveSquadParser(NC_STACK_ypaworld *o) : _o(*o), _c(NULL) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    NC_STACK_ypaworld &_o;
    NC_STACK_ypabact *_c;
};

class SaveGemParser : public ScriptParser::DataHandler
{
public:
    SaveGemParser(NC_STACK_ypaworld *o) : _o(*o) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    NC_STACK_ypaworld &_o;
};

class SaveExtraViewParser : public ScriptParser::DataHandler
{
public:
    SaveExtraViewParser(NC_STACK_ypaworld *o) : _o(*o) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt) { return !StriCmp(word, "begin_extraviewer"); };
protected:
    NC_STACK_ypaworld &_o;
};

class SaveKwFactorParser : public ScriptParser::DataHandler
{
public:
    SaveKwFactorParser(NC_STACK_ypaworld *o) : _o(*o) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt) { return !StriCmp(word, "begin_kwfactor"); };
protected:
    NC_STACK_ypaworld &_o;
};

class SaveGlobalsParser : public ScriptParser::DataHandler
{
public:
    SaveGlobalsParser(NC_STACK_ypaworld *o) : _o(*o) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt) { return !StriCmp(word, "begin_globals"); };
protected:
    NC_STACK_ypaworld &_o;
};

class SaveOwnerMapParser : public ScriptParser::DataHandler, public MapAsPlaneBytes
{
public:
    SaveOwnerMapParser(NC_STACK_ypaworld *o) : _o(*o) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    NC_STACK_ypaworld &_o;
};

class SaveBuildingMapParser : public ScriptParser::DataHandler, public MapAsPlaneBytes
{
public:
    SaveBuildingMapParser(NC_STACK_ypaworld *o) : _o(*o) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    NC_STACK_ypaworld &_o;
};

class SaveEnergyMapParser : public ScriptParser::DataHandler, public MapAsPlaneBytes
{
public:
    SaveEnergyMapParser(NC_STACK_ypaworld *o) : _o(*o) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    NC_STACK_ypaworld &_o;
};

class SaveLevelNumParser : public ScriptParser::DataHandler
{
public:
    SaveLevelNumParser(NC_STACK_ypaworld *o, int *lvlnum) : _o(*o), _levelNum(*lvlnum) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt) { return !StriCmp(word, "begin_levelnum"); };
protected:
    NC_STACK_ypaworld &_o;
    int &_levelNum;
};

class SaveHistoryParser : public ScriptParser::DataHandler
{
public:
    SaveHistoryParser(NC_STACK_ypaworld *o) : _o(*o) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt) { return !StriCmp(word, "begin_history"); };
protected:
    NC_STACK_ypaworld &_o;
};

class SaveMasksParser : public ScriptParser::DataHandler
{
public:
    SaveMasksParser(NC_STACK_ypaworld *o) : _o(*o) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt) { return !StriCmp(word, "begin_masks"); };
protected:
    NC_STACK_ypaworld &_o;
};

class SaveSuperBombParser : public ScriptParser::DataHandler
{
public:
    SaveSuperBombParser(NC_STACK_ypaworld *o) : _o(*o), _id(-1) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt) { return !StriCmp(word, "begin_superbomb"); };
protected:
    NC_STACK_ypaworld &_o;
    int _id;
};

class SaveLuaScriptParser : public ScriptParser::DataHandler
{
public:
    SaveLuaScriptParser(NC_STACK_ypaworld *o) : _o(*o) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    NC_STACK_ypaworld &_o;
};



}
}

#endif
