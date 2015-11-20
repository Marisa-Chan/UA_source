#ifndef SAMPLE_H_INCLUDED
#define SAMPLE_H_INCLUDED

#include "rsrc.h"

extern class_stored sample_class_off;

struct NC_STACK_sample;

struct sampl
{
	void *sample_buffer;
	int bufsz;
	int field_8;
	int SampleRate;
	int field_10;
};

struct __NC_STACK_sample
{
	sampl * p_sampl;
};

struct NC_STACK_sample : public NC_STACK_rsrc
{
	__NC_STACK_sample stack__sample;
};

#endif // SAMPLE_H_INCLUDED
