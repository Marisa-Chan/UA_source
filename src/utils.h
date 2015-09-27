#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#define SWAP32(A) ( ((A & 0xFF000000) >> 24) | ((A & 0xFF) << 24) | ((A & 0xFF0000) >> 8) | ((A & 0xFF00) << 24) )

#define MKTAG(A, B, C, D) ((A & 0xFF) | ((B & 0xFF) << 8) | ((C & 0xFF) << 16) | ((D & 0xFF) << 24))

#define TAG_FORM   MKTAG('F','O','R','M')
#define TAG_NONE   MKTAG(' ',' ',' ',' ')
#define TAG_NAME   MKTAG('N','A','M','E')
#define TAG_ROOT   MKTAG('R','O','O','T')

void va_to_arr(stack_vals *out, int sz, va_list*in);
void va_to_arr(stack_vals *out, int sz, unsigned int _id, va_list*in);

#endif // UTILS_H_INCLUDED
