#ifndef LSTVW_H_INCLUDED
#define LSTVW_H_INCLUDED

#include <list>

class NC_STACK_ypaworld;

class GuiBase;
typedef std::list<GuiBase *> GuiBaseList;

class GuiBase : public ClickBox
{
public:
    int flags = 0;
    ClickBox iconBox;
    char *iconString = NULL;
    w3d_a209 cmdstrm;
    void (*postDraw)(NC_STACK_ypaworld *) = NULL;

    enum FLAG
    {
        FLAG_ICONIFED = 1,
        FLAG_IN_LIST = 2,
        FLAG_WITH_ICON = 4,
        FLAG_WITH_DRAGBAR = 8,
        FLAG_WITH_CLOSE = 0x10,
        FLAG_CLOSED = 0x20,
        FLAG_CLOSE_DOWN = 0x40,
        FLAG_HELP_DOWN = 0x80,
        FLAG_WITH_HELP = 0x100
    };

    void Detach();
    void Attach(GuiBaseList &);

    static char * FormateTitle(NC_STACK_ypaworld *yw, int xpos, int ypos, int w, const std::string &title, char *in, uint8_t postf_char, int flag);

    inline bool IsClosed() const {
        return ((flags & FLAG_CLOSED) != 0);
    };
    inline bool IsOpen() const {
        return ((flags & FLAG_CLOSED) == 0);
    };

protected:
    GuiBaseList *AttachedTo = NULL;
};

class GuiList : public GuiBase
{
public:
    int scrollTimer = 0;
    int listFlags = 0;
    int closeChar = 0;
    int16_t numEntries = 0;
    int16_t shownEntries = 0;
    int16_t firstShownEntries = 0;
    int16_t selectedEntry = 0;
    int16_t maxShownEntries = 0;
    int16_t minShownEntries = 0;
    int16_t mouseItem = 0;
    int16_t entryHeight = 0;
    int16_t upperVborder = 0;
    int16_t lowerVborder = 0;
    int16_t entryWidth = 0;
    int16_t minEntryWidth = 0;
    int16_t maxEntryWidth = 0;
    char title[64] = {0};
    char *scrollbar = NULL;
    char *itemBlock = NULL;
    int16_t rszX = 0;
    int16_t rszY = 0;
    int16_t scrlX = 0;
    int16_t scrlY = 0;
    int16_t scrlSize = 0;
    int16_t btnSize = 0;
    int16_t btnStart = 0;

    enum GLIST_FLAG
    {
        GLIST_FLAG_ENABLED = 1,
        GLIST_FLAG_WITH_TITLEBAR = 2,
        GLIST_FLAG_RESIZEABLE = 4,
        GLIST_FLAG_WITH_ICON = 8,
        GLIST_FLAG_INSTANT_INPUT = 0x10,
        GLIST_FLAG_IN_RESIZING = 0x20,
        GLIST_FLAG_IN_SCROLLING = 0x40,
        GLIST_FLAG_IN_SELECT = 0x80,
        GLIST_FLAG_STATIC = 0x100,
        GLIST_FLAG_NO_SCROLL = 0x200,
        GLIST_FLAG_SEL_DONE = 0x400,
        GLIST_FLAG_KEYB_INPUT = 0x800
    };

    struct tInit
    {
        std::string title; //1
        bool resizeable = false; //2
        int numEntries = 0; //3
        int shownEntries = 0; //4
        int firstShownEntry = 0; //5
        int selectedEntry = 0; //6
        int maxShownEntries = 0; //7
        bool withIcon = false; //8
        int iconPos = 0; //9
        int iconChar = 0; // A
        int entryHeight = 16; // B
        int entryWidth = 80; // C
        int minEntryWidth = 60; // D
        int maxEntryWidth = 1024; //E
        int enabled = false;//F
        int vborder = 0;//10
        bool instantInput = false;//11
        int minShownEntries = 0;//12
        bool staticItems = false;//13
        int closeChar = 65;//14
        int upperVborder = 0;//15
        int lowerVborder = 0;//16
        bool keyboardInput = false;//17
        bool withHelp = true;//18
    };


    int Init(NC_STACK_ypaworld *yw, tInit &parameters);
    int InitBuffers(NC_STACK_ypaworld *yw);
    int initDialogStrings(NC_STACK_ypaworld *yw);
    int initButtons();

    void Free();

    void SetRect(NC_STACK_ypaworld *yw, int xpos, int ypos);

    void Formate(NC_STACK_ypaworld *yw);
    void FormateTitle(NC_STACK_ypaworld *yw);
    void FormateScrollbar(NC_STACK_ypaworld *yw);
    void FormateItemBlock(NC_STACK_ypaworld *yw);
    void ScrollParamsFromEntries(NC_STACK_ypaworld *yw);

    void InputHandle(NC_STACK_ypaworld *yw, TInputState *struc);

    void PosOnSelected(int a2);

    char *ItemsPreLayout(NC_STACK_ypaworld *yw, char *cmdbuf, int tileset, const char *a5);
    char *ItemsPostLayout(NC_STACK_ypaworld *yw, char *cmdbuf, int tileset, const char *a5);
};


#endif //LSTVW_H_INCLUDED
