#ifndef LSTVW_H_INCLUDED
#define LSTVW_H_INCLUDED

#include <list>

class NC_STACK_ypaworld;

class GuiBase;
typedef std::list<GuiBase *> GuiBaseList;

class GuiBase : public ClickBox
{
public:
    int flags;
    ClickBox iconBox;
    char *iconString;
    w3d_a209 cmdstrm;
    void (*postDraw)(NC_STACK_ypaworld *);

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

    GuiBase() : flags(0), iconString(NULL), postDraw(NULL), AttachedTo(NULL) {};


    inline bool IsClosed() {
        return ((flags & FLAG_CLOSED) != 0);
    };
    inline bool IsOpen() {
        return ((flags & FLAG_CLOSED) == 0);
    };

protected:
    GuiBaseList *AttachedTo;
};

class GuiList : public GuiBase
{
public:
    int scrollTimer;
    int listFlags;
    int closeChar;
    int16_t numEntries;
    int16_t shownEntries;
    int16_t firstShownEntries;
    int16_t selectedEntry;
    int16_t maxShownEntries;
    int16_t minShownEntries;
    int16_t mouseItem;
    int16_t entryHeight;
    int16_t upperVborder;
    int16_t lowerVborder;
    int16_t entryWidth;
    int16_t minEntryWidth;
    int16_t maxEntryWidth;
    char title[64];
    char *scrollbar;
    char *itemBlock;
    int16_t rszX;
    int16_t rszY;
    int16_t scrlX;
    int16_t scrlY;
    int16_t scrlSize;
    int16_t btnSize;
    int16_t btnStart;

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
        const char *title; //1
        bool resizeable; //2
        int numEntries; //3
        int shownEntries; //4
        int firstShownEntry; //5
        int selectedEntry; //6
        int maxShownEntries; //7
        bool withIcon; //8
        int iconPos; //9
        int iconChar; // A
        int entryHeight; // B
        int entryWidth; // C
        int minEntryWidth; // D
        int maxEntryWidth; //E
        int enabled;//F
        int vborder;//10
        bool instantInput;//11
        int minShownEntries;//12
        bool staticItems;//13
        int closeChar;//14
        int upperVborder;//15
        int lowerVborder;//16
        bool keyboardInput;//17
        bool withHelp;//18

        tInit()
        {
            Init();
        }

        void Init()
        {
            title = NULL;
            resizeable = false;
            numEntries = 0;
            shownEntries = 0;
            firstShownEntry = 0;
            selectedEntry = 0;
            maxShownEntries = 0;
            withIcon = false;
            iconPos = 0;
            iconChar = 0;
            entryHeight = 16;
            entryWidth = 80;
            minEntryWidth = 60;
            maxEntryWidth = 1024;
            enabled = false;
            vborder = 0;
            instantInput = false;
            minShownEntries = 0;
            staticItems = false;
            closeChar = 65;
            upperVborder = 0;
            lowerVborder = 0;
            keyboardInput = false;
            withHelp = true;
        }
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

    void InputHandle(NC_STACK_ypaworld *yw, InputState *struc);

    void PosOnSelected(int a2);

    char *ItemsPreLayout(NC_STACK_ypaworld *yw, char *cmdbuf, int tileset, const char *a5);
    char *ItemsPostLayout(NC_STACK_ypaworld *yw, char *cmdbuf, int tileset, const char *a5);
};


#endif //LSTVW_H_INCLUDED
