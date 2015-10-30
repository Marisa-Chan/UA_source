#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#define SWAP32(A) ( ((A & 0xFF000000) >> 24) | ((A & 0xFF) << 24) | ((A & 0xFF0000) >> 8) | ((A & 0xFF00) << 8) )
#define SWAP16(A) ( ((A & 0xFF) << 8) | ((A & 0xFF00) >> 8) )

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

void va_to_arr(stack_vals *out, int sz, va_list in);
void va_to_arr(stack_vals *out, int sz, unsigned int _id, va_list in);

int read_yes_no_status(const char *file, int result);

void sub_4BF181(DWORD sec);

#endif // UTILS_H_INCLUDED
