#include <stdio.h>
#include <stdarg.h>
#include "log.h"

void ypa_log__ypa_general_log()
{
	FILE *v0 = fopen("env/ypa_log.txt", "w");
	if ( v0 )
	{
		fprintf(v0, "YPA General Log\n");
		fprintf(v0, "---------------\n");
		fclose(v0);
	}
}

void ypa_log_out(const char *format, ...)
{
	va_list va;
	va_start(va, format);
	FILE *f = fopen("env/ypa_log.txt", "a");
	if ( f )
	{
		vfprintf(f, format, va);
		fclose(f);
	}
	va_end(va);
}

void init_d3dlog()
{
	FILE *v0 = fopen("env/d3dlog.txt", "w");
	if ( v0 )
	{
		fprintf(v0, "YPA DD/D3D driver log\n---------------------\n");
		fclose(v0);
	}
}

void log_d3dlog(const char *format, ...)
{
	va_list va;
	va_start(va, format);
	FILE *f = fopen("env/d3dlog.txt", "a");
	if ( f )
	{
		vfprintf(f, format, va);
		fclose(f);
	}
	va_end(va);
}

void init_dinputlog()
{
	FILE *v0 = fopen("env/dinplog.txt", "w");
	if ( v0 )
	{
		fprintf(v0, "YPA DirectInput log\n---------------------\n");
		fclose(v0);
	}
}



void sub_412038(const char *msg)
{
	ypa_log_out("sub_412038: %s", msg);
}


void log_d3d_fail(const char *title, const char *msg, unsigned int errID)
{
	const char *err;
	switch(errID)
	{
	case 0x88760005:
		err = "AlreadyInitialized";
		break;
	case 0x8876023E:
		err = "CltFastCantClip";
		break;
	case 0x8876000A:
		err = "CannotAttachSurface";
		break;
	case 0x88760014:
		err = "CannotDetachSurface";
		break;
	case 0x88760249:
		err = "CantCreateDC";
		break;
	case 0x88760247:
		err = "CantDuplicate";
		break;
	case 0x887601B3:
		err = "CantLockSurface";
		break;
	case 0x88760280:
		err = "CantPageLock";
		break;
	case 0x88760294:
		err = "CantPageUnlock";
		break;
	case 0x88760237:
		err = "ClipperIsUsingHwnd";
		break;
	case 0x88760190:
		err = "ColorKeyNotSet";
		break;
	case 0x88760028:
		err = "CurrentlyNotAvail";
		break;
	case 0x8876026C:
		err = "DCAlreadyCreated";
		break;
	case 0x88760232:
		err = "DirectDrawAlreadyCreated";
		break;
	case 0x88760037:
		err = "Exception";
		break;
	case 0x88760245:
		err = "ExclusiveModeAlreadySet";
		break;
	case 0x80004005:
		err = "Generic";
		break;
	case 0x8876005A:
		err = "HeightAlign";
		break;
	case 0x8876023B:
		err = "HwndAlreadySet";
		break;
	case 0x8876023A:
		err = "HwndSubClassed";
		break;
	case 0x8876024C:
		err = "ImplicitlyCreated";
		break;
	case 0x8876005F:
		err = "IncompatiblePrimary";
		break;
	case 0x88760064:
		err = "InvalidCaps";
		break;
	case 0x8876006E:
		err = "InvalidClipList";
		break;
	case 0x88760231:
		err = "InvalidDirectDrawGUID";
		break;
	case 0x88760078:
		err = "InvalidMode";
		break;
	case 0x88760082:
		err = "InvalidObject";
		break;
	case 0x80070057:
		err = "InvalidParams";
		break;
	case 0x88760091:
		err = "InvalidPixelFormat";
		break;
	case 0x88760243:
		err = "InvalidPosition";
		break;
	case 0x88760096:
		err = "InvalidRect";
		break;
	case 0x88760250:
		err = "InvalidSurfaceType";
		break;
	case 0x887600A0:
		err = "LockedSurfaces";
		break;
	case 0x887600AA:
		err = "No3D";
		break;
	case 0x887600B4:
		err = "NoAlphaHW";
		break;
	case 0x8876023F:
		err = "NoBltHW";
		break;
	case 0x887600CD:
		err = "NoClipList";
		break;
	case 0x88760238:
		err = "NoClipperAttached";
		break;
	case 0x887600D2:
		err = "NoColorConvHW";
		break;
	case 0x887600D7:
		err = "NoColorKey";
		break;
	case 0x887600DC:
		err = "NoColorKeyHW";
		break;
	case 0x887600D4:
		err = "NoCooperativeLevelSet";
		break;
	case 0x8876024A:
		err = "NoDC";
		break;
	case 0x88760240:
		err = "NoDDRopsHW";
		break;
	case 0x88760233:
		err = "NoDirectDrawHW";
		break;
	case 0x887600DE:
		err = "NoDirectDrawSupport";
		break;
	case 0x88760235:
		err = "NoEmulation";
		break;
	case 0x887600E1:
		err = "NoExclusiveMode";
		break;
	case 0x887600E6:
		err = "NoFlipHW";
		break;
	case 0x887600F0:
		err = "NoGDI";
		break;
	case 0x8876024F:
		err = "NoMipMapHW";
		break;
	case 0x887600FA:
		err = "NoMirrorHW";
		break;
	case 0x88760242:
		err = "NoOverlayDest";
		break;
	case 0x88760104:
		err = "NoOverlayHW";
		break;
	case 0x8876023C:
		err = "NoPaletteAttached";
		break;
	case 0x8876023D:
		err = "NoPaletteHW";
		break;
	case 0x88760118:
		err = "NoRasterOpHW";
		break;
	case 0x88760122:
		err = "NoRotationHW";
		break;
	case 0x88760136:
		err = "NoStretchHW";
		break;
	case 0x8876013C:
		err = "Not4BitColor";
		break;
	case 0x8876013D:
		err = "Not4BitColorIndex";
		break;
	case 0x88760140:
		err = "Not8BitColor";
		break;
	case 0x88760244:
		err = "NotAOverlaySurface";
		break;
	case 0x8876014A:
		err = "NoTextureHW";
		break;
	case 0x88760246:
		err = "NotFlippable";
		break;
	case 0x887600FF:
		err = "NotFound";
		break;
	case 0x800401F0:
		err = "NotInitialized";
		break;
	case 0x88760248:
		err = "NotLocked";
		break;
	case 0x887602A8:
		err = "NotPageLocked";
		break;
	case 0x8876024D:
		err = "NotPalettized";
		break;
	case 0x8876014F:
		err = "NoVSyncHW";
		break;
	case 0x88760154:
		err = "NoZBufferHW";
		break;
	case 0x8876015E:
		err = "NoZOverlayHW";
		break;
	case 0x88760168:
		err = "OutOfCaps";
		break;
	case 0x8007000E:
		err = "OutOfMemory";
		break;
	case 0x8876017C:
		err = "OutOfVideoMemory";
		break;
	case 0x8876017E:
		err = "OverlayCantClip";
		break;
	case 0x88760180:
		err = "OverlayColorKeyOnlyOneActive";
		break;
	case 0x88760241:
		err = "OverlayNotVisible";
		break;
	case 0x88760183:
		err = "PaletteBusy";
		break;
	case 0x88760234:
		err = "PrimarySurfaceAlreadyExists";
		break;
	case 0x88760236:
		err = "RegionTooSmall";
		break;
	case 0x8876019A:
		err = "SurfaceAlreadyAttached";
		break;
	case 0x887601A4:
		err = "SurfaceAlreadyDependent";
		break;
	case 0x887601AE:
		err = "SurfaceBusy";
		break;
	case 0x887601B8:
		err = "SurfaceIsObscured";
		break;
	case 0x887601C2:
		err = "SurfaceLost";
		break;
	case 0x887601CC:
		err = "SurfaceNotAttached";
		break;
	case 0x887601D6:
		err = "TooBigHeight";
		break;
	case 0x887601E0:
		err = "TooBigSize";
		break;
	case 0x887601EA:
		err = "TooBigWidth";
		break;
	case 0x80004001:
		err = "Unsupported";
		break;
	case 0x887601FE:
		err = "UnsupportedFormat";
		break;
	case 0x88760208:
		err = "UnsupportedMask";
		break;
	case 0x8876024E:
		err = "UnsupportedMode";
		break;
	case 0x88760219:
		err = "VerticalBlankInProgress";
		break;
	case 0x8876021C:
		err = "WasStillDrawing";
		break;
	case 0x8876024B:
		err = "WrongMode";
		break;
	case 0x88760230:
		err = "XAlign";
		break;
	case 0x887602BC:
		err = "BadMajorVersion";
		break;
	case 0x887602BD:
		err = "BadMinorVersion";
		break;
	case 0x887602C6:
		err = "ExecuteCreateFailed";
		break;
	case 0x887602C7:
		err = "ExecuteDestroyFailed";
		break;
	case 0x887602C8:
		err = "ExecuteLockFailed";
		break;
	case 0x887602C9:
		err = "ExecuteUnlockFailed";
		break;
	case 0x887602CB:
		err = "ExecuteNotLocked";
		break;
	case 0x887602CC:
		err = "ExecuteFailed";
		break;
	case 0x887602CD:
		err = "ExecuteClippedFailed";
		break;
	case 0x887602D0:
		err = "TextureNoSupport";
		break;
	case 0x887602D1:
		err = "TextureCreateFailed";
		break;
	case 0x887602D2:
		err = "TextureDestroyFailed";
		break;
	case 0x887602D3:
		err = "TextureLockFailed";
		break;
	case 0x887602D4:
		err = "TextureUnlockFailed";
		break;
	case 0x887602D5:
		err = "TextureUnlockFailed";
		break;
	case 0x887602D6:
		err = "TextureSwapFailed";
		break;
	case 0x887602D7:
		err = "TextureLocked";
		break;
	case 0x887602D8:
		err = "TextureNotLocked";
		break;
	case 0x887602D9:
		err = "TextureGetSurfFailed";
		break;
	case 0x887602DA:
		err = "MatrixCreateFailed";
		break;
	case 0x887602DB:
		err = "MatrixDestroyFailed";
		break;
	case 0x887602DC:
		err = "MatrixSetDataFailed";
		break;
	case 0x887602DD:
		err = "MatrixGetDataFailed";
		break;
	case 0x887602DE:
		err = "SetViewportDataFailed";
		break;
	case 0x887602E4:
		err = "MaterialCreateFailed";
		break;
	case 0x887602E5:
		err = "MaterialDestroyFailed";
		break;
	case 0x887602E6:
		err = "MaterialSetDataFailed";
		break;
	case 0x887602E7:
		err = "MaterialGetDataFailed";
		break;
	case 0x887602EE:
		err = "LightSetFailed";
		break;
	case 0x887602F8:
		err = "SceneInScene";
		break;
	case 0x887602F9:
		err = "SceneNotInScene";
		break;
	case 0x887602FA:
		err = "SceneBeginFailed";
		break;
	case 0x887602FB:
		err = "SceneEndFailed";
		break;
	default :
		err = "";
	}
	log_d3dlog("FAIL MSG: title=%s, msg=%s, err=%s\n", title, msg, err);
}







void log_dinputlog(const char *format, ...)
{
	va_list va;
	va_start(va, format);
	FILE *f = fopen("env/dinplog.txt", "a");
	if ( f )
	{
		vfprintf(f, format, va);
		fclose(f);
	}
	va_end(va);
}


void log_dinput_error(const char *title, const char *msg, int msgid)
{
	switch ( msgid )
	{
	case 0x800700AA:
		log_dinputlog("FAIL MSG: title=%s, msg=%s, err=%s\n", title, msg, "Acquired");
		break;
	case 0x80070077:
		log_dinputlog("FAIL MSG: title=%s, msg=%s, err=%s\n", title, msg, "BadDriverVer");
		break;
	case 0x80040201:
		log_dinputlog("FAIL MSG: title=%s, msg=%s, err=%s\n", title, msg, "DeviceFull");
		break;
	case 0x80040154:
		log_dinputlog("FAIL MSG: title=%s, msg=%s, err=%s\n", title, msg, "DeviceNotReg");
		break;
	case 0x80040208:
		log_dinputlog("FAIL MSG: title=%s, msg=%s, err=%s\n", title, msg, "EffectPlaying");
		break;
	case 0x80040204:
		log_dinputlog("FAIL MSG: title=%s, msg=%s, err=%s\n", title, msg, "HasEffects");
		break;
	case 0x80004005:
		log_dinputlog("FAIL MSG: title=%s, msg=%s, err=%s\n", title, msg, "Generic");
		break;
	case 0x80070005:
		log_dinputlog("FAIL MSG: title=%s, msg=%s, err=%s\n", title, msg, "HandleExists or ReadOnly");
		break;
	case 0x8007001E:
		log_dinputlog("FAIL MSG: title=%s, msg=%s, err=%s\n", title, msg, "InputLost");
		break;
	case 0x80070057:
		log_dinputlog("FAIL MSG: title=%s, msg=%s, err=%s\n", title, msg, "InvalidParam");
		break;
	case 0x80040110:
		log_dinputlog("FAIL MSG: title=%s, msg=%s, err=%s\n", title, msg, "NoAggregation");
		break;
	case 0x8007000C:
		log_dinputlog("FAIL MSG: title=%s, msg=%s, err=%s\n", title, msg, "NotAcquired");
		break;
	case 0x80040205:
		log_dinputlog("FAIL MSG: title=%s, msg=%s, err=%s\n", title, msg, "NotExclusiveAcquired");
		break;
	case 0x80040202:
		log_dinputlog("FAIL MSG: title=%s, msg=%s, err=%s\n", title, msg, "MoreData");
		break;
	case 0x80004002:
		log_dinputlog("FAIL MSG: title=%s, msg=%s, err=%s\n", title, msg, "NoInterface");
		break;
	case 0x80040203:
		log_dinputlog("FAIL MSG: title=%s, msg=%s, err=%s\n", title, msg, "NotDownloaded");
		break;
	case 0x80040207:
		log_dinputlog("FAIL MSG: title=%s, msg=%s, err=%s\n", title, msg, "NotBuffered");
		break;
	case 0x80070015:
		log_dinputlog("FAIL MSG: title=%s, msg=%s, err=%s\n", title, msg, "NotInitialized");
		break;
	case 0x80070002:
		log_dinputlog("FAIL MSG: title=%s, msg=%s, err=%s\n", title, msg, "ObjectNotFound");
		break;
	case 0x8007000E:
		log_dinputlog("FAIL MSG: title=%s, msg=%s, err=%s\n", title, msg, "OutOfMemory");
		break;
	case 0x80004001:
		log_dinputlog("FAIL MSG: title=%s, msg=%s, err=%s\n", title, msg, "Unsupported");
		break;
	default:
		log_dinputlog("FAIL MSG: title=%s, msg=%s, err=%s\n", title, msg, "<unknown>");
		break;
	}
}
