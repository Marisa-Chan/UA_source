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
