#include <stdio.h>
#include <stdarg.h>
#include "log.h"

void ypa_log__ypa_general_log()
{
	FILE *v0 = fopen("env/ypa_log.txt", "w");
	if ( v0 )
	{
		fprintf(v0, "YPA General Log\n");
		fprintf(v0, "---------------\n");
		fclose(v0);
	}
}

void ypa_log_out(const char *format, ...)
{
    va_list va;
	va_start(va, format);
	FILE *f = fopen("env/ypa_log.txt", "a");
	if ( f )
	{
		vfprintf(f, format, va);
		fclose(f);
	}
	va_end(va);
}

void init_d3dlog()
{
	FILE *v0 = fopen("env/d3dlog.txt", "w");
	if ( v0 )
	{
		fprintf(v0, "YPA DD/D3D driver log\n---------------------\n");
		fclose(v0);
	}
}

void log_d3dlog(const char *format, ...)
{
    va_list va;
	va_start(va, format);
	FILE *f = fopen("env/d3dlog.txt", "a");
	if ( f )
	{
		vfprintf(f, format, va);
		fclose(f);
	}
	va_end(va);
}

void init_dinputlog()
{
	FILE *v0 = fopen("env/dinplog.txt", "w");
	if ( v0 )
	{
		fprintf(v0, "YPA DirectInput log\n---------------------\n");
		fclose(v0);
	}
}



void sub_412038(const char *msg)
{
    ypa_log_out("sub_412038: %s", msg);
}
