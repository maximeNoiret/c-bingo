#ifndef _WORK_H_
#define _WORK_H_

#include "draw.h"

struct Config {
        char wordlist_src[MAX_FILE_PATH_LEN];
        bool autosave;
};

void generatePool(struct States *s);
void generateTexts(struct States *s);

void saveTexts(struct States *s);
void loadTexts(struct States *s);
void refreshBoard(struct States *s);

#endif  // _WORK_H_