#include <inttypes.h>
#include "includes.h"
#include "nucleas.h"
#include "engine_input.h"
#include "iwimp.h"
#include "utils.h"



const Nucleus::ClassDescr NC_STACK_iwimp::description("iwimp.class", &newinstance);



size_t NC_STACK_iwimp::func0(IDVList &stak)
{
    if ( !NC_STACK_idev::func0(stak) )
        return 0;

    cboxList.clear();

    return 1;
}

void NC_STACK_iwimp::AddClickBoxFront(ClickBox *box)
{
    cboxList.push_front(box);
}

void NC_STACK_iwimp::AddClickBoxBack(ClickBox *box)
{
    cboxList.push_back(box);
}

void NC_STACK_iwimp::RemoveClickBox(ClickBox *box)
{
    cboxList.remove(box);
}

void NC_STACK_iwimp::GetClick(ClickBoxInf *click, MousePos &mouse)
{
    for(std::list<ClickBox *>::iterator it = cboxList.begin();
            it != cboxList.end();
            it++)
    {
        ClickBox *box = *it;

        mouse.BoxPos.x = mouse.ScreenPos.x - box->x;
        mouse.BoxPos.y = mouse.ScreenPos.y - box->y;

        if ( mouse.BoxPos.x >= 0 && mouse.BoxPos.x < box->w &&
                mouse.BoxPos.y >= 0 && mouse.BoxPos.y < box->h )
        {
            click->selected_btn = box;

            for (unsigned int i = 0; i < box->buttons.size(); i++)
            {
                ButtonBox &btn = box->buttons[i];

                mouse.BtnPos.x = mouse.BoxPos.x - btn.x;
                mouse.BtnPos.y = mouse.BoxPos.y - btn.y;

                if ( mouse.BtnPos.x >= 0 && mouse.BtnPos.x < btn.w &&
                        mouse.BtnPos.y >= 0 && mouse.BtnPos.y < btn.h )
                {
                    click->selected_btnID = i;
                    break;
                }
            }
            break;
        }
    }
}

void NC_STACK_iwimp::CheckClick(ClickBoxInf *arg)
{
    arg->flag |= ClickBoxInf::FLAG_OK;
    arg->selected_btn = NULL;
    arg->selected_btnID = -1;

    GetClick(arg, arg->move);

    if ( arg->flag & ClickBoxInf::FLAG_LM_HOLD )
    {
        if ( arg->selected_btnID >= 0 &&
                selectedCbox == arg->selected_btn &&
                selectedButton == arg->selected_btnID )
            arg->flag |= ClickBoxInf::FLAG_BTN_HOLD;
    }

    if ( arg->flag & ClickBoxInf::FLAG_LM_DOWN )
    {
        GetClick(arg, arg->ldw_pos);

        if ( arg->selected_btnID == -1 )
        {
            selectedCbox = 0;
            selectedButton = -1;
        }
        else
        {
            selectedCbox = arg->selected_btn;
            selectedButton = arg->selected_btnID;
            arg->flag |= ClickBoxInf::FLAG_BTN_DOWN;
        }
    }

    if ( arg->flag & ClickBoxInf::FLAG_LM_UP )
    {
        GetClick(arg, arg->lup_pos);

        if ( arg->selected_btn )
        {
            if ( arg->selected_btn == selectedCbox &&
                    arg->selected_btnID == selectedButton )
                arg->flag |= ClickBoxInf::FLAG_BTN_UP;
        }
        selectedButton = -1;
        selectedCbox = 0;
    }
}

void NC_STACK_iwimp::clear()
{
    cboxList.clear();
    selectedCbox = 0;
    selectedButton = 0;
}


