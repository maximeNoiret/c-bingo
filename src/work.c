#include <stdio.h>
#include <string.h>
#include "raygui.h"
#include "draw.h"

// Work code will go here. Prolly just a click(index) function or smth.

void generatePool(struct States *s)
{
        FILE *f = fopen("wordlist.txt", "r");

        for (size_t i = 0; i < TEXT_NUM; ++i) {
                if (!fgets(s->pool[i], TEXT_MAX_LEN, f))
                        break;
                s->pool[i][strlen(s->pool[i]) - 1] = '\0';  // trim trailing newline
                for (char *c = s->pool[i]; *c; ++c)
                        if (*c == '|')
                                *c = '\n';
        }

        fclose(f);
}

void generateTexts(struct States *s)
{
        // TODO: random selection from pool.
        for (size_t i = 0; i < TEXT_NUM; ++i) {
                s->slot_texts[i] = s->pool[i];
        }
}