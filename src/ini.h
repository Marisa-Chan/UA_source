#ifndef INI_H_INCLUDED
#define INI_H_INCLUDED

union valP
{
	int val;
	void *pval;

	valP()
	{
	    pval = NULL;
	    val = 0;
	}

	valP(char *str)
	{
	    pval = (void *)str;
	}
};

enum KEY_TYPE
{
	KEY_TYPE_DIGIT = 0x0,
	KEY_TYPE_STRING1 = 0x1,
	KEY_TYPE_BOOL = 0x2,
	KEY_TYPE_STRING2 = 0x3,
};

struct key_value_stru
{
	const char *key;
	KEY_TYPE key_type;
	valP value;
};

int get_keyvalue_from_ini(const char *ini_filename, key_value_stru *key, unsigned int key_count);

#endif // INI_H_INCLUDED
