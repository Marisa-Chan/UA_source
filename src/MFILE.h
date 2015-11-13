#ifndef MFILE_H_INCLUDED
#define MFILE_H_INCLUDED

#include "nlist.h"

struct MFILE_flags
{
  char write_stream;
  char fl0;
  char fl1;
  char fl2;
};

struct MFILE
{
	FILE *file_handle;
	MFILE_flags flags;
	int field_8;
	nlist list;
};

struct MFILE_S1 : public nnode
{
	int TAG;
	int TAG_EXTENSION;
	int TAG_SIZE;
	int cur_pos;
};

MFILE_S1 * GET_FORM_INFO_OR_NULL(MFILE *a1);
signed int read_next_IFF(MFILE *a1, int a2);
int mfread(MFILE *a1, void *dst, int size);
int read_default(MFILE *a1);
int sub_413564(MFILE *a1, int a2, const void *a3);
signed int sub_412FC0(MFILE *a1, unsigned int TAG1, unsigned int TAG2, int a4);
int sub_413290(MFILE *a1);
MFILE *new_MFILE();

int sub_412F98(MFILE *a1, int a2);
void del_MFILE(MFILE *fil);
MFILE * open_mfile(const char *filename, int flag);
void close_mfile(MFILE *mfil);

#endif // MFILE_H_INCLUDED
