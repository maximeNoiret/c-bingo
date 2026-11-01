#ifndef _DRAW_H_
#define _DRAW_H_

#define GENERAL_PAD             2
#define BAR_WIDTH               24
#define BOTTOM_BAR_WIDTH        40
#define BOTTOM_BUTTON_COEF      28

// These two will go once settings file is implemented
#define DEFAULT_ELEMS           5
#define DEFAULT_ELEM_WIDTH      100
#define MAX_ELEM_COUNT          121  // TODO: This is equivalent to 11x11.

#define TEXT_NUM        128
#define TEXT_MAX_LEN    64

#define MAX_FILE_PATH_LEN 256
#define MAX_MSG_LEN MAX_FILE_PATH_LEN+64

struct Stack {
        Rectangle boundaries;
        float x, y, w, h, gap;
};

Rectangle NextRight(struct Stack *s);

enum SUB_PAGE {
        PAGE_NONE,
        PAGE_SAVE,
        PAGE_LOAD,
        PAGE_SETT,
        PAGE_ERR
};

enum SAVE_BTNS {
        SAVE_NONE = -1,
        SAVE_CLOSE,
        SAVE_CANCEL,
        SAVE_CONFIRM
};

// TODO: group elements to have better padding
struct States {
        struct Config *config;
        char pool[TEXT_NUM][TEXT_MAX_LEN];
        char *slot_texts[MAX_ELEM_COUNT];
        size_t text_num;
        int slots[MAX_ELEM_COUNT];
        enum SUB_PAGE page;
        char buf[MAX_FILE_PATH_LEN];
        char msg[MAX_MSG_LEN];
        bool stop;
        bool errStop;
};

void drawBottomBar(struct States *s);
void drawContent(struct States *s);

void drawSaveDialog(struct States *s);
void drawLoadDialog(struct States *s);
void drawSettingsPage(struct States *s);
void drawErrorMessage(struct States *s);


#endif  // _DRAW_H_