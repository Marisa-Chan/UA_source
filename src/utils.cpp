#include "includes.h"
#include "utils.h"


void va_to_arr(stack_vals *out, int sz, va_list*in)
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

void va_to_arr(stack_vals *out, int sz, unsigned int _id, va_list*in)
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
