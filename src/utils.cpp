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
