#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

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

#define POW2(x)  ( (x) * (x) )

void va_to_arr(stack_vals *out, int sz, va_list in);
void va_to_arr(stack_vals *out, int sz, unsigned int _id, va_list in);

int read_yes_no_status(const char *file, int result);

void sub_4BF181(DWORD sec);

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

HCURSOR uaLoadCursor(HINSTANCE, const char *name);
HICON uaLoadIcon(HINSTANCE, const char *name);
int read_reg_key(const CHAR *keyname, char *outBuf, int bufsize);
const char *get_lang_string(char **array, int id, const char *def);
int strnicmp (const char *s1, const char *s2, size_t n);

void dprintf(const char *fmt, ...);

#endif // UTILS_H_INCLUDED
