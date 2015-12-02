#include "includes.h"
#include "utils.h"
#include "inttypes.h"


void va_to_arr(stack_vals *out, int sz, va_list in)
{
	for (int i = 0; i < sz; i++)
	{
		unsigned int id = va_arg(in, unsigned int);
		if (id == 0 || i == sz - 1)
		{
			out[i].id = 0;
			out[i].value = 0;
			break;
		}

		size_t value = va_arg(in,size_t);

		out[i].id = id;
		out[i].value = value;
	}
}

void va_to_arr(stack_vals *out, int sz, unsigned int _id, va_list in)
{
	if (_id != 0)
	{
		out[0].id = _id;
		out[0].value = va_arg(in, size_t);

		for (int i = 1; i < sz; i++)
		{
			unsigned int id = va_arg(in, unsigned int);
			if (id == 0 || i == sz - 1)
			{
				out[i].id = 0;
				out[i].value = 0;
				break;
			}

			size_t value = va_arg(in,size_t);

			out[i].id = id;
			out[i].value = value;
		}
	}
}


int read_yes_no_status(const char *file, int result)
{
	char buf[128];

	FILE *fil = fopen(file, "r");
	if ( fil )
	{
		if ( fgets(buf, 128, fil) )
		{
			char *lend = strpbrk(buf, "; \n");
			if ( lend )
				*lend = 0;
			result = strcasecmp(buf, "yes") == 0;
		}
		fclose(fil);
	}
	return result;
}


void sub_4BF181(DWORD sec)
{
	Sleep(sec);
}

float SWAP32F(float f)
{
	uint32_t tmp = *(uint32_t *)&f;
	tmp = SWAP32(tmp);
	return *(float *)&tmp;
}


static HCURSOR cursorCache[11];
static char    cursorNames[11][128];
static int     cursor_num = 0;


HCURSOR uaLoadCursor(HINSTANCE, const char *name)
{
	for(int i = 0; i < cursor_num; i++)
	{
		if (strcasecmp(name, cursorNames[i]) == 0)
		{
			return cursorCache[i];
		}
	}

	char buf[256];
	strcpy(buf, "res/");
	strcat(buf, name);
	strcat(buf, ".cur");

	HCURSOR tmp = LoadCursorFromFile(buf);
	if (!tmp)
		return NULL;

	if (cursor_num < 11)
	{
		cursorCache[cursor_num] = tmp;
		strcpy(cursorNames[cursor_num], name);
		cursor_num++;
	}
	return tmp;
}

HICON uaLoadIcon(HINSTANCE, const char *name)
{
	char buf[256];
	strcpy(buf, "res/");
	strcat(buf, name);
	strcat(buf, ".ico");

	return (HICON) LoadImage(NULL, buf, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
}
