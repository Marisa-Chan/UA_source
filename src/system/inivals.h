#ifndef WORLD_INIVALS_H_INCLUDED
#define WORLD_INIVALS_H_INCLUDED

#include "../ini.h"

namespace System 
{
    
class IniConf
{   
public:

public:
    static void Init();
    static bool ReadFromNucleusIni();
    static bool ReadFromIni(const std::string &fname);
    
    static Common::Ini::Key GfxDither;
    static Common::Ini::Key GfxFilter;
    static Common::Ini::Key GfxAntialias;
    static Common::Ini::Key GfxAlpha;
    static Common::Ini::Key GfxZbufWhenTracy;
    static Common::Ini::Key GfxColorkey;
    static Common::Ini::Key GfxForceEmul;
    static Common::Ini::Key GfxForceSoftCursor;
    static Common::Ini::Key GfxAllModes;
    static Common::Ini::Key GfxMoviePlayer;
    static Common::Ini::Key GfxForceAlphaTex;
    static Common::Ini::Key GfxUseDrawPrimitive;
    static Common::Ini::Key GfxDisableLowres;
    static Common::Ini::Key GfxExportWindowMode;
    static Common::Ini::Key GfxBlending;
    static Common::Ini::Key GfxSolidFont;
    static Common::Ini::Key GfxVsync;
    static Common::Ini::Key GfxMaxFps;
    static Common::Ini::Key GfxNewSky;
    static Common::Ini::Key GfxSkyDistance;
    static Common::Ini::Key GfxSkyLength;
    
    static Common::Ini::Key GfxMode;
    static Common::Ini::Key GfxXRes;
    static Common::Ini::Key GfxYRes;
    static Common::Ini::Key GfxPalette;
    static Common::Ini::Key GfxDisplay;
    static Common::Ini::Key GfxDisplay2;
    
    static Common::Ini::Key GfxColorEffects;
    static Common::Ini::Key GfxColorEffPower1;
    static Common::Ini::Key GfxColorEffPower2;
    static Common::Ini::Key GfxColorEffPower3;
    static Common::Ini::Key GfxColorEffPower4;
    static Common::Ini::Key GfxColorEffPower5;
    static Common::Ini::Key GfxColorEffPower6;
    static Common::Ini::Key GfxColorEffPower7;
    
    static Common::Ini::Key InputDebug;
    static Common::Ini::Key InputTimer;
    static Common::Ini::Key InputWimp;
    static Common::Ini::Key InputKeyboard;
    static Common::Ini::Key InputButton0;
    static Common::Ini::Key InputButton1;
    static Common::Ini::Key InputButton2;
    static Common::Ini::Key InputButton3;
    static Common::Ini::Key InputButton4;
    static Common::Ini::Key InputButton5;
    static Common::Ini::Key InputButton6;
    static Common::Ini::Key InputButton7;
    static Common::Ini::Key InputButton8;
    static Common::Ini::Key InputButton9;
    static Common::Ini::Key InputButton10;
    static Common::Ini::Key InputButton11;
    static Common::Ini::Key InputButton12;
    static Common::Ini::Key InputButton13;
    static Common::Ini::Key InputButton14;
    static Common::Ini::Key InputButton15;
    static Common::Ini::Key InputButton16;
    static Common::Ini::Key InputButton17;
    static Common::Ini::Key InputButton18;
    static Common::Ini::Key InputButton19;
    static Common::Ini::Key InputButton20;
    static Common::Ini::Key InputButton21;
    static Common::Ini::Key InputButton22;
    static Common::Ini::Key InputButton23;
    static Common::Ini::Key InputButton24;
    static Common::Ini::Key InputButton25;
    static Common::Ini::Key InputButton26;
    static Common::Ini::Key InputButton27;
    static Common::Ini::Key InputButton28;
    static Common::Ini::Key InputButton29;
    static Common::Ini::Key InputButton30;
    static Common::Ini::Key InputButton31;
    static Common::Ini::Key InputSlider0;
    static Common::Ini::Key InputSlider1;
    static Common::Ini::Key InputSlider2;
    static Common::Ini::Key InputSlider3;
    static Common::Ini::Key InputSlider4;
    static Common::Ini::Key InputSlider5;
    static Common::Ini::Key InputSlider6;
    static Common::Ini::Key InputSlider7;
    static Common::Ini::Key InputSlider8;
    static Common::Ini::Key InputSlider9;
    static Common::Ini::Key InputSlider10;
    static Common::Ini::Key InputSlider11;
    static Common::Ini::Key InputSlider12;
    static Common::Ini::Key InputSlider13;
    static Common::Ini::Key InputSlider14;
    static Common::Ini::Key InputSlider15;
    static Common::Ini::Key InputSlider16;
    static Common::Ini::Key InputSlider17;
    static Common::Ini::Key InputSlider18;
    static Common::Ini::Key InputSlider19;
    static Common::Ini::Key InputSlider20;
    static Common::Ini::Key InputSlider21;
    static Common::Ini::Key InputSlider22;
    static Common::Ini::Key InputSlider23;
    static Common::Ini::Key InputSlider24;
    static Common::Ini::Key InputSlider25;
    static Common::Ini::Key InputSlider26;
    static Common::Ini::Key InputSlider27;
    static Common::Ini::Key InputSlider28;
    static Common::Ini::Key InputSlider29;
    static Common::Ini::Key InputSlider30;
    static Common::Ini::Key InputSlider31;
    static Common::Ini::Key InputHotkey0;
    static Common::Ini::Key InputHotkey1;
    static Common::Ini::Key InputHotkey2;
    static Common::Ini::Key InputHotkey3;
    static Common::Ini::Key InputHotkey4;
    static Common::Ini::Key InputHotkey5;
    static Common::Ini::Key InputHotkey6;
    static Common::Ini::Key InputHotkey7;
    static Common::Ini::Key InputHotkey8;
    static Common::Ini::Key InputHotkey9;
    static Common::Ini::Key InputHotkey10;
    static Common::Ini::Key InputHotkey11;
    static Common::Ini::Key InputHotkey12;
    static Common::Ini::Key InputHotkey13;
    static Common::Ini::Key InputHotkey14;
    static Common::Ini::Key InputHotkey15;
    static Common::Ini::Key InputHotkey16;
    static Common::Ini::Key InputHotkey17;
    static Common::Ini::Key InputHotkey18;
    static Common::Ini::Key InputHotkey19;
    static Common::Ini::Key InputHotkey20;
    static Common::Ini::Key InputHotkey21;
    static Common::Ini::Key InputHotkey22;
    static Common::Ini::Key InputHotkey23;
    static Common::Ini::Key InputHotkey24;
    static Common::Ini::Key InputHotkey25;
    static Common::Ini::Key InputHotkey26;
    static Common::Ini::Key InputHotkey27;
    static Common::Ini::Key InputHotkey28;
    static Common::Ini::Key InputHotkey29;
    static Common::Ini::Key InputHotkey30;
    static Common::Ini::Key InputHotkey31;
    static Common::Ini::Key InputHotkey32;
    static Common::Ini::Key InputHotkey33;
    static Common::Ini::Key InputHotkey34;
    static Common::Ini::Key InputHotkey35;
    static Common::Ini::Key InputHotkey36;
    static Common::Ini::Key InputHotkey37;
    static Common::Ini::Key InputHotkey38;
    static Common::Ini::Key InputHotkey39;
    static Common::Ini::Key InputHotkey40;
    static Common::Ini::Key InputHotkey41;
    static Common::Ini::Key InputHotkey42;
    static Common::Ini::Key InputHotkey43;
    static Common::Ini::Key InputHotkey44;
    static Common::Ini::Key InputHotkey45;
    static Common::Ini::Key InputHotkey46;
    static Common::Ini::Key InputHotkey47;
    
    static Common::Ini::Key AudioChannels;
    static Common::Ini::Key AudioVolume;
    static Common::Ini::Key AudioNumPalfx;
    static Common::Ini::Key AudioRevStereo;
    
    static Common::Ini::Key TformBackplane;
    static Common::Ini::Key TformFrontplane;
    static Common::Ini::Key TformZoomx;
    static Common::Ini::Key TformZoomy;
    
    static Common::Ini::Key NetGmode;
    static Common::Ini::Key NetVersionCheck;
    
    static Common::Ini::Key GameDebug;
    
    static Common::Ini::Key GameNewAI;
    static Common::Ini::Key GameTimeLine;
    
    static Common::Ini::Key NetGameExclusiveGem;
    static Common::Ini::Key NetWaitStart;
    static Common::Ini::Key NetKickoff;
    
    static Common::Ini::Key TuneWeaponRadius;
    
    static Common::Ini::Key ParticlesLimit;
    static Common::Ini::Key MenuWindowed;
    
private:
    static Common::Ini::PKeyList _varList;    
};

}

#endif