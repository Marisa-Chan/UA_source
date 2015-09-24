#include <stdio.h>
#include <stdlib.h>
#include "includes.h"
#include "MFILE.h"
#include "utils.h"

MFILE_S1 * GET_FORM_INFO_OR_NULL(MFILE *a1)
{
	MFILE_S1 *head = (MFILE_S1 *)a1->list.head;
	if ( head->TAG == TAG_FORM && head->TAG_EXTENSION == TAG_NONE )
		return NULL;
	return head;
}

int sub_413290(MFILE *a1)
{
	MFILE_S1 *v2 = (MFILE_S1 *)a1->list.head;
	int Offset = v2->TAG_SIZE;

	if ( v2->TAG == TAG_FORM && v2->TAG_EXTENSION == TAG_NONE )
		return -9;

	if ( a1->flags.write_stream & 1 )
	{
		if ( Offset == -1 )
		{
			Offset = v2->cur_pos;
			if ( fseek(a1->file_handle, -v2->cur_pos - 4, SEEK_CUR) )
				return -7;

			__int32 tmp = SWAP32(v2->cur_pos);

			if ( fwrite(&tmp, 4, 1, a1->file_handle) != 1 )
				return -6;

			if ( fseek(a1->file_handle, v2->cur_pos, 1) )
				return -7;
		}

		if ( v2->cur_pos < Offset )
			return -8;

		if ( Offset & 1 )
		{
			int tmp = 0;
			if ( fwrite(&tmp, 1, 1, a1->file_handle) != 1 )
				return -6;

			Offset++;
		}
		RemHead(&a1->list);
		nc_FreeMem(v2);

		((MFILE_S1 *)(a1->list.head))->cur_pos += Offset + 8;

		a1->field_8--;

		return 0;
	}
	else
	{
		if ( v2->TAG != TAG_FORM )
		{
			if ( v2->cur_pos < Offset && fseek(a1->file_handle, Offset -  v2->cur_pos, SEEK_CUR) )
				return -7;

			if ( Offset & 1 )
			{
				if ( fseek(a1->file_handle, 1, SEEK_CUR) )
					return -7;

				Offset++;
			}
		}

		RemHead(&a1->list);
		nc_FreeMem(v2);

		v2 = (MFILE_S1 *)a1->list.head;
		if ( v2->TAG != TAG_FORM || v2->TAG_EXTENSION != TAG_NONE )
		{
			v2->cur_pos += Offset + 8;
			a1->field_8--;
			return 0;
		}
		else
			return -1;
	}
}

signed int sub_412FC0(MFILE *a1, unsigned int TAG1, unsigned int TAG2, int a4)
{
	if ( a1->flags.write_stream & 1 )             // WRITING
	{
		__int32 TAG_EXTENSION = TAG1;

		MFILE_S1 *head = (MFILE_S1 *)a1->list.head;

		if ( head->TAG != TAG_FORM )
			return -9;

		int cur_pos = 0;

		__int32 tmp = SWAP32(TAG2);

		if ( fwrite(&tmp, 4, 1, a1->file_handle) != 1 )
			return -6;

		tmp = SWAP32(a4);

		if ( fwrite(&tmp, 4, 1, a1->file_handle) != 1 )
			return -6;

		if ( TAG2 == TAG_FORM )
		{
			tmp = SWAP32(TAG_EXTENSION);

			if ( fwrite(&tmp, 4, 1, a1->file_handle) != 1 )
				return -6;

			cur_pos = 4;
		}
		else
		{
			TAG_EXTENSION = head->TAG_EXTENSION;
		}

		MFILE_S1 *v10 = (MFILE_S1 *)AllocVec(sizeof(MFILE_S1), 65537);

		if ( !v10 )
			return -4;

		v10->TAG = TAG2;
		v10->TAG_EXTENSION = TAG_EXTENSION;
		v10->TAG_SIZE = a4;
		v10->cur_pos = cur_pos;

		AddHead(&a1->list, v10);

		a1->field_8++;

		return 0;
	}
	else                                          // READING
	{
		MFILE_S1 *head = (MFILE_S1 *)a1->list.head;

		if ( head->TAG != TAG_FORM )
			return -2;

		if ( head->TAG_SIZE == head->cur_pos )
			return -2;

		int cur_pos = 0;

		__int32 tmp;
		__int32 tmp2;
		__int32 tmp3 = head->TAG_EXTENSION;


		if ( fread(&tmp, 4, 1, a1->file_handle) != 1 )
			return -5;

		if ( fread(&tmp2, 4, 1, a1->file_handle) != 1 )
			return -5;

		tmp = SWAP32(tmp);
		tmp2 = SWAP32(tmp2);

		if ( tmp == TAG_FORM )
		{
			if ( fread(&tmp3, 4, 1, a1->file_handle) != 1 )
				return -5;

			tmp3 = SWAP32(tmp3);
			cur_pos = 4;
		}
		MFILE_S1 *v20 = (MFILE_S1 *)AllocVec(sizeof(MFILE_S1), 65537);

		if ( !v20 )
			return -4;

		v20->TAG = tmp;
		v20->TAG_EXTENSION = tmp3;
		v20->TAG_SIZE = tmp2;
		v20->cur_pos = cur_pos;

		AddHead(&a1->list, v20);

		a1->field_8++;

		return 0;
	}

	return 0;
}


signed int read_next_IFF(MFILE *a1, int a2)
{
	signed int result = -9;
	if ( a2 == 2 )
	{
		if ( !(a1->flags.fl2 & 0x80) )
		{
			result = sub_412FC0(a1, TAG_NONE, TAG_NONE, -1);
			if ( result == -2 )
				a1->flags.fl2 |= 0x80;
		}
		else
		{
			result = sub_413290(a1);
			a1->flags.fl2 &= 0x7F;
			if (!result)
			{
				result = sub_412FC0(a1, TAG_NONE, TAG_NONE, -1);
				if ( result == -2 )
					a1->flags.fl2 |= 0x80;
			}
		}
	}
	return result;
}


int mfread(MFILE *a1, void *dst, int size)
{
	size_t sz_to_read = size;

	MFILE_S1 *head = (MFILE_S1 *)a1->list.head;

	if ( head->TAG == TAG_FORM && head->TAG_EXTENSION == TAG_NONE )
		head = NULL;

	if ( head )
	{
		if ( head->TAG == TAG_FORM )
			return -9;
		else
		{

			if ( size + head->cur_pos <= head->TAG_SIZE )
			{
				head->cur_pos += sz_to_read;

				size_t readed = fread(dst, 1, sz_to_read, a1->file_handle);

				if ( readed != sz_to_read )
					return -5;

				return readed;
			}
			else
			{
				sz_to_read = head->TAG_SIZE - head->cur_pos;

				if (head->TAG_SIZE != head->cur_pos)
				{
					head->cur_pos += sz_to_read;

					size_t readed = fread(dst, 1, sz_to_read, a1->file_handle);

					if ( readed != sz_to_read )
						return -5;

					return readed;
				}
				else
					return 0;
			}
		}
	}
	else
		return -9;

	return 0;
}


int read_default(MFILE *a1)
{
	while ( 1 )
	{
		int retResult;

		if ( a1->flags.fl2 & 0x80 )
		{
			retResult = sub_413290(a1);
			a1->flags.fl2 &= 0x7Fu;

			if (!retResult)
			{
				retResult = sub_412FC0(a1, TAG_NONE, TAG_NONE, -1);
				if ( retResult == -2 )
					a1->flags.fl2 |= 0x80u;
			}
		}
		else
		{
			retResult = sub_412FC0(a1, TAG_NONE, TAG_NONE, -1);
			if ( retResult == -2 )
				a1->flags.fl2 |= 0x80u;
		}

		if ( retResult == -2 )
			break;

		if ( retResult )
			return 0;

		retResult = read_default(a1);

		if ( !retResult )
			return retResult;
	}
	return 1;
}


int sub_413564(MFILE *a1, int a2, const void *a3)
{
	int writed = 0; // eax@5

	MFILE_S1 *head = (MFILE_S1 *)a1->list.head;
	if ( head->TAG == TAG_FORM && head->TAG_EXTENSION == TAG_NONE )
		head = 0;
	if ( head )
	{
		if ( head->TAG == TAG_FORM )
		{
			return -9;
		}
		else if ( head->TAG_SIZE == -1 || a2 + head->cur_pos <= head->TAG_SIZE)
		{
			head->cur_pos += a2;
			writed = fwrite(a3, 1, a2, a1->file_handle);
			if ( writed != a2 )
				return -6;
		}
		else
		{
			a2 = head->TAG_SIZE - head->cur_pos;

			if ( head->TAG_SIZE != head->cur_pos )
			{
				head->cur_pos += a2;
				writed = fwrite(a3, 1, a2, a1->file_handle);
				if ( writed != a2 )
					return -6;
			}
			else
				return 0;
		}
	}
	else
		return -9;

	return writed;
}
