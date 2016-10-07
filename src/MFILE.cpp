#include <stdio.h>
#include <stdlib.h>
#include "includes.h"
#include "MFILE.h"
#include "utils.h"

MFILE_S1 * GET_FORM_INFO_OR_NULL(MFILE *a1)
{
    MFILE_S1 *head = a1->list.front();
    if ( head->TAG == TAG_FORM && head->TAG_EXTENSION == TAG_NONE )
        return NULL;
    return head;
}

int sub_413290(MFILE *a1)
{
    MFILE_S1 *v2 = a1->list.front();
    int Offset = v2->TAG_SIZE;

    if ( v2->TAG == TAG_FORM && v2->TAG_EXTENSION == TAG_NONE )
        return -9;

    if ( a1->flags.write_stream & 1 )
    {
        if ( Offset == -1 )
        {
            Offset = v2->cur_pos;
            if ( a1->file_handle->seek(-v2->cur_pos - 4, SEEK_CUR) )
                return -7;

            int32_t tmp = SWAP32(v2->cur_pos);

            if ( a1->file_handle->write(&tmp, 4) != 4 )
                return -6;

            if ( a1->file_handle->seek(v2->cur_pos, SEEK_CUR) )
                return -7;
        }

        if ( v2->cur_pos < Offset )
            return -8;

        if ( Offset & 1 )
        {
            int tmp = 0;
            if ( a1->file_handle->write(&tmp, 1) != 1 )
                return -6;

            Offset++;
        }

        a1->list.pop_front();
        delete v2;

        a1->list.front()->cur_pos += Offset + 8;

        a1->field_8--;

        return 0;
    }
    else
    {
        if ( v2->TAG != TAG_FORM )
        {
            if ( v2->cur_pos < Offset && a1->file_handle->seek(Offset -  v2->cur_pos, SEEK_CUR) )
                return -7;

            if ( Offset & 1 )
            {
                if ( a1->file_handle->seek(1, SEEK_CUR) )
                    return -7;

                Offset++;
            }
        }

        a1->list.pop_front();
        delete v2;

        v2 = a1->list.front();
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
        int32_t TAG_EXTENSION = TAG1;

        MFILE_S1 *head = a1->list.front();

        if ( head->TAG != TAG_FORM )
            return -9;

        int cur_pos = 0;

        int32_t tmp = SWAP32(TAG2);

        if ( a1->file_handle->write(&tmp, 4) != 4 )
            return -6;

        tmp = SWAP32(a4);

        if ( a1->file_handle->write(&tmp, 4) != 4 )
            return -6;

        if ( TAG2 == TAG_FORM )
        {
            tmp = SWAP32(TAG_EXTENSION);

            if ( a1->file_handle->write(&tmp, 4) != 4 )
                return -6;

            cur_pos = 4;
        }
        else
        {
            TAG_EXTENSION = head->TAG_EXTENSION;
        }

        MFILE_S1 *v10 = new MFILE_S1();

        if ( !v10 )
            return -4;

        v10->TAG = TAG2;
        v10->TAG_EXTENSION = TAG_EXTENSION;
        v10->TAG_SIZE = a4;
        v10->cur_pos = cur_pos;

        a1->list.push_front(v10);

        a1->field_8++;

        return 0;
    }
    else                                          // READING
    {
        MFILE_S1 *head = a1->list.front();

        if ( head->TAG != TAG_FORM )
            return -2;

        if ( head->TAG_SIZE == head->cur_pos )
            return -2;

        int cur_pos = 0;

        int32_t tmp;
        int32_t tmp2;
        int32_t tmp3 = head->TAG_EXTENSION;


        if ( a1->file_handle->read(&tmp, 4) != 4 )
            return -5;

        if ( a1->file_handle->read(&tmp2, 4) != 4 )
            return -5;

        tmp = SWAP32(tmp);
        tmp2 = SWAP32(tmp2);

        if ( tmp == TAG_FORM )
        {
            if ( a1->file_handle->read(&tmp3, 4) != 4 )
                return -5;

            tmp3 = SWAP32(tmp3);
            cur_pos = 4;
        }
        MFILE_S1 *v20 = new MFILE_S1();

        if ( !v20 )
            return -4;

        v20->TAG = tmp;
        v20->TAG_EXTENSION = tmp3;
        v20->TAG_SIZE = tmp2;
        v20->cur_pos = cur_pos;

        a1->list.push_front(v20);

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

    MFILE_S1 *head = a1->list.front();

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

                size_t readed = a1->file_handle->read(dst, sz_to_read);

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

                    size_t readed = a1->file_handle->read(dst, sz_to_read);

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

    MFILE_S1 *head = a1->list.front();
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
            writed = a1->file_handle->write(a3, a2);
            if ( writed != a2 )
                return -6;
        }
        else
        {
            a2 = head->TAG_SIZE - head->cur_pos;

            if ( head->TAG_SIZE != head->cur_pos )
            {
                head->cur_pos += a2;
                writed = a1->file_handle->write(a3, a2);
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


MFILE *new_MFILE()
{
    MFILE *mfile = new MFILE;

    if ( !mfile )
        return NULL;

    mfile->file_handle = NULL;
    mfile->flags.fl0 = 0;
    mfile->flags.fl1 = 0;
    mfile->flags.fl2 = 0;
    mfile->flags.write_stream = 0;
    mfile->field_8 = 0;

    //new(&mfile->list) std::list<MFILE_S1 *>;
    //mfile->list.clear();
    //init_list(&mfile->list);

    MFILE_S1 *sub = new MFILE_S1;

    if ( !sub )
    {
        delete mfile;
        return NULL;
    }

    sub->TAG = TAG_FORM;
    sub->TAG_EXTENSION = TAG_NONE;
    sub->TAG_SIZE = 0x80000000;

    mfile->list.push_front(sub);
    return mfile;
}

int sub_412F98(MFILE *a1, int a2)
{
    a1->flags.write_stream = a2;
    return 0;
}


void del_MFILE(MFILE *fil)
{
    if ( fil )
    {
        for (std::list<MFILE_S1 *>::iterator it = fil->list.begin(); it != fil->list.end(); it++)
            delete *it;

        delete fil;
    }
}

MFILE * open_mfile(const char *filename, int flag)
{
    char tmpBuf[256];
    const char *mc_type = get_MC_str(MC_TYPE_RES);

    strcpy(tmpBuf, mc_type);
    strcat(tmpBuf, filename);

    MFILE *mfil = new_MFILE();

    if ( !mfil )
        return NULL;

    const char *mode;
    if (flag)
        mode = "wb";
    else
        mode = "rb";

    FSMgr::FileHandle *fil = uaOpenFile(tmpBuf, mode);

    mfil->file_handle = fil;
    if ( fil )
    {
        if ( !sub_412F98(mfil, flag) )
            return mfil;

        delete mfil->file_handle;
    }
    del_MFILE(mfil);

    return NULL;
}

void close_mfile(MFILE *mfil)
{
    if ( mfil )
    {
        delete mfil->file_handle;
        del_MFILE(mfil);
    }
}
