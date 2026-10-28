#ifndef _DRAW_H_
#define _DRAW_H_


#define GENERAL_PAD             2
#define BAR_WIDTH               24

constexpr unsigned BOTTOM_BAR_WIDTH     = 40;
constexpr unsigned BOTTOM_BAR_BTN_H     = BOTTOM_BAR_WIDTH - 2 * GENERAL_PAD;
constexpr unsigned BOTTOM_BAR_BTN_W     = 150;

constexpr unsigned ELEMS                = 5;
constexpr unsigned TOTAL_ELEMS          = ELEMS*ELEMS;
constexpr unsigned ELEM_WIDTH           = 100;
constexpr unsigned CONTENT_X            = GENERAL_PAD;
constexpr unsigned CONTENT_Y            = (BAR_WIDTH + GENERAL_PAD);
constexpr unsigned CONTENT_WIDTH        = (ELEMS * ELEM_WIDTH + (ELEMS+1) * GENERAL_PAD);
constexpr unsigned CONTENT_HEIGHT       = (ELEMS * ELEM_WIDTH + (ELEMS+1) * GENERAL_PAD);

constexpr unsigned WINDOW_WIDTH         = (CONTENT_WIDTH + 2*GENERAL_PAD);
constexpr unsigned WINDOW_HEIGHT        = (CONTENT_HEIGHT + 2*GENERAL_PAD + BAR_WIDTH + BOTTOM_BAR_WIDTH);

#define TEXT_NUM        128
#define TEXT_MAX_LEN    64

#define MAX_FILE_PATH_LEN 64

struct Stack {
        Rectangle boundaries;
        float x, y, w, h, gap;
};

Rectangle NextRight(struct Stack *s);

enum SUB_PAGE {
        PAGE_NONE,
        PAGE_SAVE,
        PAGE_LOAD
};

enum SAVE_BTNS {
        SAVE_NONE = -1,
        SAVE_CLOSE,
        SAVE_CANCEL,
        SAVE_CONFIRM
};

struct States {
        char pool[TEXT_NUM][TEXT_MAX_LEN];
        char *slot_texts[TOTAL_ELEMS];
        size_t text_num;
        int slots[TOTAL_ELEMS];
        enum SUB_PAGE page;
        char buf[MAX_FILE_PATH_LEN];
};

void drawBottomBar(struct States *s);
void drawContent(struct States *s);

void drawSaveDialog(struct States *s);
void drawLoadDialog(struct States *s);

#endif  // _DRAW_H_