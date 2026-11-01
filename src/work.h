#ifndef _WORK_H_
#define _WORK_H_

#include "draw.h"

// TODO: Choose proper types for elements (not just unsigned everywhere lmao)
struct Config {
        char wordlist_src[MAX_FILE_PATH_LEN];
        unsigned board_width;
        unsigned board_spaces;
        unsigned space_width;
        unsigned content_x;
        unsigned content_y;
        unsigned content_width;
        unsigned content_height;
        unsigned bottom_bar_width;
        unsigned bottom_bar_btn_h;
        unsigned bottom_bar_btn_w;
        unsigned window_width;
        unsigned window_height;
        bool autosave;
        bool freeSpace;
};

void refreshConfig(struct Config *c);

void generatePool(struct States *s);
void generateTexts(struct States *s);

void saveTexts(struct States *s);
void loadTexts(struct States *s);
void refreshBoard(struct States *s);

#endif  // _WORK_H_