#include "includes.h"

void nc_FreeMem(void *mem)
{
	free(mem);
}

void *AllocVec(size_t size, int a2)
{
	void *mem = calloc(size, 1);

	if (!mem)
		ypa_log_out("_AllocVec(%d, %d) failed (Out of mem)!\n", size, a2);

	return mem;
}


char * file_path_copy_manipul(const char *src, char *dst, int size)
{
	char buf1[256];

	dst[size - 1] = 0;

	strcpy(buf1, src);

	char *v8;

	if ( strlen(buf1) <= 2 || buf1[1] != ':' )
		v8 = buf1;
	else
		v8 = buf1 + 2;

	while ( *v8 != ':' )
	{
		if ( !(*v8) )
		{
			v8 = NULL;
			break;
		}
		v8++;;
	}


	if ( !v8 )
		return strncpy(dst, buf1, size - 1);

	*v8 = 0;

	nnode_str *v11 = (nnode_str *)engines.stru_525D68.head;

	if ( engines.stru_525D68.head->next )
	{
		while ( strcasecmp(v11->str, buf1) )
		{
			v11 = (nnode_str *)v11->next;
			if ( !v11->next )
			{
				v11 = NULL;
				break;
			}
		}
	}
	else
	{
		v11 = NULL;
	}

	*v8 = '/';

	if ( v11 )
	{
		char buf2[512];

		strcpy(buf2, v11->str2);

		if (buf2[strlen(buf2) - 1] == ':')
			v8++;

		strcat(buf2, v8);

		strncpy(dst, buf2, size - 1);

		return file_path_copy_manipul(buf2, dst, size);
	}

	return strncpy(dst, buf1, size - 1);
}

void correct_slashes_and_3_ext(const char *src, char *dst, int sz)
{
	strncpy(dst, src, sz - 1);

	char *v4 = dst;

	while( (*v4) )
	{
		if (*v4 == '\\')
			*v4 = '/';
		v4++;
	}

	v4 = dst;

	while ((*v4))
	{
		if (*v4 == '.')
		{
			v4++;
			if (strlen(v4) > 3)
				v4[3] = 0;

            break;
		}
		v4++;
	}
}

FILE * FOpen(const char *src_path, const char *mode)
{
	char dst[256];
	char path[256];

	file_path_copy_manipul(src_path, path, 256);
	correct_slashes_and_3_ext(path, dst, 256);

	FILE *v4 = fopen(dst, mode);

	if ( v4 )
		engines.file_handles++;
	else
		ypa_log_out("_FOpen('%s','%s') failed!\n", dst, mode);

	return v4;
}

int FClose(FILE *a1)
{
  engines.file_handles--;
  return fclose(a1);
}
