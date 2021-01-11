#ifndef WORLD_PARSERS_H_INCLUDED
#define WORLD_PARSERS_H_INCLUDED

#include <string>
#include "../types.h"
#include "../utils.h"
#include "../common.h"

#include "../def_parser.h"


class NC_STACK_ypaworld;
class NC_STACK_ypabact;
class NC_STACK_yparobo;
class NC_STACK_bitmap;
class UserData;
struct save_status;
struct vhclSndFX;
struct VhclProto;
struct WeapProto;
struct TBuildingProto;
struct LevelDesc;
struct MapRobo;
struct MapGem;
struct MapSquad;
struct MapGate;
struct dbmapProto;
struct MapSuperItem;
struct stru_LevelNet;

namespace World
{
namespace Parsers
{

class UserParser : public ScriptParser::DataHandler
{
public:
    UserParser(NC_STACK_ypaworld *o) : _o(*o) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    bool ReadUserNameFile(const std::string &filename);

    NC_STACK_ypaworld &_o;
};

class InputParser : public ScriptParser::DataHandler
{
public:
    InputParser(NC_STACK_ypaworld *o) : _o(*o) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    NC_STACK_ypaworld &_o;
};


class ColorParser : public ScriptParser::DataHandler
{
public:
    ColorParser(NC_STACK_ypaworld *o) : _o(*o) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt) { return !StriCmp(word, "begin_colors"); };
protected:
    NC_STACK_ypaworld &_o;
};

class MovieParser : public ScriptParser::DataHandler
{
public:
    MovieParser(NC_STACK_ypaworld *o) : _o(*o) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    NC_STACK_ypaworld &_o;
};

class BkgParser : public ScriptParser::DataHandler
{
public:
    BkgParser(NC_STACK_ypaworld *o);
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt) { return !StriCmp(word, "begin_bg"); };

protected:
    stru_LevelNet &_o;
};

class SuperItemParser : public ScriptParser::DataHandler
{
public:
    SuperItemParser(NC_STACK_ypaworld *o) : _o(*o) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    NC_STACK_ypaworld &_o;
};

class MiscParser : public ScriptParser::DataHandler
{
public:
    MiscParser(NC_STACK_ypaworld *o) : _o(*o) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    NC_STACK_ypaworld &_o;
};



class VideoParser : public ScriptParser::DataHandler
{
public:
    VideoParser(NC_STACK_ypaworld *o) : _o(*o) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt) { return !StriCmp(word, "new_video"); };
protected:
    NC_STACK_ypaworld &_o;
};

class SoundParser : public ScriptParser::DataHandler
{
public:
    SoundParser(NC_STACK_ypaworld *o) : _o(*o) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt) { return !StriCmp(word, "new_sound"); };
protected:
    NC_STACK_ypaworld &_o;
};

class LevelStatusParser : public ScriptParser::DataHandler
{
public:
    LevelStatusParser(NC_STACK_ypaworld *o, bool setFlag) : _o(*o), _levelId(0), _setFlag(setFlag) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    NC_STACK_ypaworld &_o;
    int _levelId;
    const bool _setFlag;
};

class BuddyParser : public ScriptParser::DataHandler
{
public:
    BuddyParser(NC_STACK_ypaworld *o) : _o(*o) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    NC_STACK_ypaworld &_o;
};

class ShellParser : public ScriptParser::DataHandler
{
public:
    ShellParser(NC_STACK_ypaworld *o) : _o(*o) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt) { return !StriCmp(word, "new_shell"); };
protected:
    void ParseStatus(save_status *status, const std::string &p2);

    NC_STACK_ypaworld &_o;
};

class FxParser
{
protected:
    virtual vhclSndFX *GetSndFxByName(const std::string &sndname) = 0;

    bool ParseExtSampleDef(vhclSndFX *sndfx, const std::string &p2);
    int ParseSndFX(const std::string &p1, const std::string &p2);
};

class VhclProtoParser : public ScriptParser::DataHandler, public FxParser
{
friend FxParser;
public:
    VhclProtoParser(NC_STACK_ypaworld *o) : _o(*o), _vhcl(NULL), _vhclID(-1), _gunID(-1), _collID(-1) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    virtual vhclSndFX *GetSndFxByName(const std::string &sndname);

    NC_STACK_ypaworld &_o;
    VhclProto *_vhcl;
    int32_t _vhclID;
    int32_t _gunID;
    int32_t _collID;
};


class WeaponProtoParser : public ScriptParser::DataHandler, public FxParser
{
friend FxParser;
public:
    WeaponProtoParser(NC_STACK_ypaworld *o) : _o(*o), _wpn(NULL) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    virtual vhclSndFX *GetSndFxByName(const std::string &sndname);

    NC_STACK_ypaworld &_o;
    WeapProto *_wpn;
};

class BuildProtoParser : public ScriptParser::DataHandler
{
public:
    BuildProtoParser(NC_STACK_ypaworld *o) : _o(*o), _bld(NULL), _gunID(-1) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    NC_STACK_ypaworld &_o;
    TBuildingProto *_bld;
    size_t _gunID;
};


class LevelDataParser : public ScriptParser::DataHandler
{
public:
    LevelDataParser(NC_STACK_ypaworld *o, LevelDesc *p) : _o(*o), _m(*p) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    NC_STACK_ypaworld &_o;
    LevelDesc &_m;
};

class MapRobosParser : public ScriptParser::DataHandler
{
public:
    MapRobosParser(LevelDesc *m) : _m(*m), _r(NULL) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    LevelDesc &_m;
    MapRobo *_r;
};

class MapSizesParser : public ScriptParser::DataHandler
{
public:
    MapSizesParser(LevelDesc *m) : _m(*m) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt) { return !StriCmp(word, "begin_maps"); };
protected:
    void ParseSizes(ScriptParser::Parser &parser, int *outx, int *outy);

    LevelDesc &_m;
};

class LevelGemParser : public ScriptParser::DataHandler
{
public:
    LevelGemParser(NC_STACK_ypaworld *o) : _o(*o), _g(NULL) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    NC_STACK_ypaworld &_o;
    MapGem *_g;
};

class LevelSquadParser : public ScriptParser::DataHandler
{
public:
    LevelSquadParser(LevelDesc *m) : _m(*m), _s(NULL) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    LevelDesc &_m;
    MapSquad *_s;
};

class LevelGatesParser : public ScriptParser::DataHandler
{
public:
    LevelGatesParser(NC_STACK_ypaworld *o) : _o(*o), _g(NULL) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    NC_STACK_ypaworld &_o;
    MapGate *_g;
};

class LevelMbMapParser : public ScriptParser::DataHandler
{
public:
    LevelMbMapParser(LevelDesc *m) : _m(*m), _d(NULL) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    LevelDesc &_m;
    dbmapProto *_d;
};

class MapAsPlaneBytes
{
protected:
    Common::PlaneBytes *ReadMapAsPlaneBytes(ScriptParser::Parser &parser);
};

class LevelMapsParser : public ScriptParser::DataHandler, public MapAsPlaneBytes
{
public:
    LevelMapsParser(NC_STACK_ypaworld *o, LevelDesc *m) : _o(*o), _m(*m) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    NC_STACK_ypaworld &_o;
    LevelDesc &_m;
};

class LevelDebMapParser : public ScriptParser::DataHandler
{
public:
    LevelDebMapParser(LevelDesc *m) : _m(*m), _d(NULL) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    LevelDesc &_m;
    dbmapProto *_d;
};

class LevelEnableParser : public ScriptParser::DataHandler
{
public:
    LevelEnableParser(NC_STACK_ypaworld *o) : _o(*o), _fraction(0) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    NC_STACK_ypaworld &_o;
    int _fraction;
};

class LevelSuperItemsParser : public ScriptParser::DataHandler
{
public:
    LevelSuperItemsParser(NC_STACK_ypaworld *o) : _o(*o), _s(NULL) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt);
protected:
    NC_STACK_ypaworld &_o;
    MapSuperItem *_s;
};

class ShellSoundParser : public ScriptParser::DataHandler
{
public:
    ShellSoundParser(UserData *o) : _o(*o) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt) { return !StriCmp(word, "begin_shellsounds"); };
protected:
    UserData &_o;
};

class ShellTracksParser : public ScriptParser::DataHandler
{
public:
    ShellTracksParser(UserData *o) : _o(*o) {} ;
    virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
    virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt) { return !StriCmp(word, "begin_shelltracks"); };
protected:
    UserData &_o;
};

}
}

#endif
