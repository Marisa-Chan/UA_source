#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <math.h>
#include <string.h>

#define C_2PI		6.28318530717958647693
#define C_PI		3.14159265358979323846
#define C_PI_2		1.57079632679489661923
#define C_PI_180    0.01745329251994329577

#include "fsmgr.h"

#define SWAP32(A) ( (((A) >> 24) & 0xFF ) | (((A) & 0xFF) << 24) | (((A) & 0xFF0000) >> 8) | (((A) & 0xFF00) << 8) )
#define SWAP16(A) ( (((A) & 0xFF) << 8) | (((A) >> 8) & 0xFF) )

#define MKTAG(D, C, B, A) ((A & 0xFF) | ((B & 0xFF) << 8) | ((C & 0xFF) << 16) | ((D & 0xFF) << 24))

#define TAG_FORM   MKTAG('F','O','R','M')
#define TAG_NONE   MKTAG(' ',' ',' ',' ')
#define TAG_NAME   MKTAG('N','A','M','E')
#define TAG_ROOT   MKTAG('R','O','O','T')
#define TAG_CIBO   MKTAG('C','I','B','O')
#define TAG_NAM2   MKTAG('N','A','M','2')
#define TAG_OTL2   MKTAG('O','T','L','2')
#define TAG_ILBM   MKTAG('I','L','B','M')
#define TAG_VBMP   MKTAG('V','B','M','P')
#define TAG_BMHD   MKTAG('B','M','H','D')
#define TAG_HEAD   MKTAG('H','E','A','D')
#define TAG_CMAP   MKTAG('C','M','A','P')
#define TAG_BODY   MKTAG('B','O','D','Y')
#define TAG_STRC   MKTAG('S','T','R','C')
#define TAG_ADE    MKTAG('A','D','E',' ')
#define TAG_ADES   MKTAG('A','D','E','S')
#define TAG_KIDS   MKTAG('K','I','D','S')
#define TAG_SKLC   MKTAG('S','K','L','C')
#define TAG_TYPE   MKTAG('T','Y','P','E')
#define TAG_POOL   MKTAG('P','O','O','L')
#define TAG_POO2   MKTAG('P','O','O','2')
#define TAG_POLY   MKTAG('P','O','L','Y')
#define TAG_POL2   MKTAG('P','O','L','2')
#define TAG_SENS   MKTAG('S','E','N','S')
#define TAG_SEN2   MKTAG('S','E','N','2')
#define TAG_SKLT   MKTAG('S','K','L','T')
#define TAG_EMRS   MKTAG('E','M','R','S')
#define TAG_EMBD   MKTAG('E','M','B','D')
#define TAG_OBJT   MKTAG('O','B','J','T')
#define TAG_CLID   MKTAG('C','L','I','D')
#define TAG_AREA   MKTAG('A','R','E','A')
#define TAG_ATTS   MKTAG('A','T','T','S')
#define TAG_OLPL   MKTAG('O','L','P','L')
#define TAG_AMSH   MKTAG('A','M','S','H')
#define TAG_BASE   MKTAG('B','A','S','E')
#define TAG_WAVE   MKTAG('W','A','V','E')
#define TAG_RIFF   MKTAG('R','I','F','F')
#define TAG_data   MKTAG('d','a','t','a')
#define TAG_fmt    MKTAG('f','m','t',' ')
#define TAG_MC2    MKTAG('M','C','2',' ')
#define TAG_BANI   MKTAG('B','A','N','I')
#define TAG_VANM   MKTAG('V','A','N','M')
#define TAG_DATA   MKTAG('D','A','T','A')
#define TAG_PTCL   MKTAG('P','T','C','L')
#define TAG_FRAM   MKTAG('F','R','A','M')
#define TAG_FINF   MKTAG('F','I','N','F')
#define TAG_OINF   MKTAG('O','I','N','F')
#define TAG_AINF   MKTAG('A','I','N','F')
#define TAG_MODE   MKTAG('M','O','D','E')
#define TAG_SEQN   MKTAG('S','E','Q','N')
#define TAG_SINF   MKTAG('S','I','N','F')
#define TAG_FLSH   MKTAG('F','L','S','H')

#define POW2(x)  ( (x) * (x) )
#define MMAX(a, b) ((a) > (b) ? (a) : (b))
#define MMIN(a, b) ((a) < (b) ? (a) : (b))

#define NANCARRY_LOG
#define NDIV_CARRY_LOG

#ifndef NANCARRY_LOG

#define NANCARRY(x) { if (isnan(x)) x = 0.0; }

#else

#define NANCARRY(x) { if (isnan(x)) { \
                        x = 0.0;\
                        ypa_log_out("Nan occur %s:%d\n", __FILE__, __LINE__);} }

#endif

#ifndef NDIV_CARRY_LOG

#define NDIV_CARRY(x) { if (x == 0.0) x = 1.0; }

#else

#define NDIV_CARRY(x) { if (x == 0.0) { \
                        x = 1.0;\
                        ypa_log_out("Null on div occur %s:%d\n", __FILE__, __LINE__);} }

#endif


int read_yes_no_status(const char *file, int result);

float SWAP32F(float f);

struct __attribute__((packed)) shortPoint
{
    short x;
    short y;

    shortPoint(int a)
    {
        x = a & 0xFFFF;
        y = a >> 16;
    }

    shortPoint(): x(0), y(0) {}
};

const char *get_lang_string(char **array, int id, const char *def);

#ifndef strnicmp
int strnicmp (const char *s1, const char *s2, size_t n);
#endif

uint32_t fileCrc32(const char *filename, uint32_t _crc = 0);

void dprintf(const char *fmt, ...);

int dround(float val);
int dround(double val);

uint32_t profiler_begin();
uint32_t profiler_end(uint32_t prev);

FSMgr::FileHandle *uaOpenFile(const char *src_path, const char *mode);
FSMgr::DirIter *uaOpenDir(const char *dir);

bool uaDeleteFile(const char *path);
bool uaDeleteDir(const char *path);
bool uaCreateDir(const char *path);
bool uaFileExist(const char *src_path, const char *prefix);

inline double clp_asin(double x)
{
    if (x > 1.0)
        x = 1.0;
    else if (x < -1.0)
        x = -1.0;
    return asin(x);
}

inline double clp_acos(double x)
{
    if (x > 1.0)
        x = 1.0;
    else if (x < -1.0)
        x = -1.0;
    return acos(x);
}

inline double fSign(double x)
{
    if (x < 0.0)
        return -1.0;
    return 1.0;
}

void correctSeparatorAndExt(std::string &str);

#endif // UTILS_H_INCLUDED
