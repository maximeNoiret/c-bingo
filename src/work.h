#ifndef _WORK_H_
#define _WORK_H_

#include "draw.h"

struct Config {
        char *wordlist_src;
        bool autosave;
};

void generatePool(struct States *s);
void generateTexts(struct States *s, unsigned seed);

void saveTexts(struct States *s);
void loadTexts(struct States *s);

#endif  // _WORK_H_