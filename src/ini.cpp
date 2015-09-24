#include <stdio.h>
#include "ini.h"
#include "includes.h"


int get_keyvalue_from_ini(const char *ini_filename, key_value_stru *key, unsigned int key_count)
{
	char buf[256];

	if ( !ini_filename )
		ini_filename = "nucleus.ini";

	FILE *fil = fopen(ini_filename, "r");

	if ( fil )
	{
		int param_line = 0;
		while ( 1 )
		{
			char * readed = fgets(buf, 256, fil);

			if ( !readed && param_line >= engines.some_params_count )
				break;

			if ( !readed )
			{
				strcpy(buf, engines.some_params_pointers[param_line]);
				param_line++;
			}

			char *strip = strpbrk(buf, ";\r\n");
			if ( strip )
				*strip = 0;

			char *token = strtok(buf, "= \t");
			if ( token )
			{
				for (unsigned int i = 0; i < key_count; ++i )
				{
					key_value_stru *kkey = key + i;
					if ( !strcasecmp(kkey->key, token) )
					{
						char *tmp = NULL;

						switch ( kkey->key_type )
						{
						case KEY_TYPE_DIGIT:
							tmp = strtok(0, "= \t");
							if ( tmp )
								kkey->value.val = strtol(tmp, NULL, 0);
							break;

						case KEY_TYPE_BOOL:
							tmp = strtok(0, "= \t");
							if ( tmp )
							{
								if ( !strcasecmp("yes", tmp) || !strcasecmp("true", tmp) || !strcasecmp("on", tmp) )
									kkey->value.val = 1;
								else
									kkey->value.val = 0;
							}
							break;

						case KEY_TYPE_STRING1:
							tmp = strtok(0, "= \t");
							if ( tmp )
								strncpy((char *)kkey->value.pval, tmp, 128);
							break;

						case KEY_TYPE_STRING2:
							tmp = strtok(0, "=");
							if ( tmp )
								strncpy((char *)kkey->value.pval, tmp, 128);
							break;
						default:
							continue;
						}
					}
				}
			}
		}

		fclose(fil);
		return 1;
	}
	return 0;
}
