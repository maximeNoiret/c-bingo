#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "raygui.h"
#include "draw.h"

// Work code will go here. Prolly just a click(index) function or smth.

void generatePool(struct States *s)
{
        FILE *f = fopen("wordlist.txt", "r");

        if (!f) {
                fputs("Couldn't open file. Make sure 'wordlist.txt' is next to executable.\n", stderr);
                perror("generatePool");
                exit(1);
        }

        for (size_t i = 0; i < TEXT_NUM; ++i) {
                if (!fgets(s->pool[i], TEXT_MAX_LEN, f))
                        break;
                s->pool[i][strlen(s->pool[i]) - 1] = '\0';  // trim trailing newline
                for (char *c = s->pool[i]; *c; ++c)
                        if (*c == '|')
                                *c = '\n';
                ++s->text_num;
        }

        fclose(f);
}

void generateTexts(struct States *s, unsigned seed)
{
        srand(seed);

        int idx[s->text_num];
        for (size_t i = 0; i < s->text_num; ++i)
                idx[i] = i;

        for (size_t i = 0; i < TOTAL_ELEMS; ++i) {
                if (i == (TOTAL_ELEMS >> 1)) {
                        s->slot_texts[i] = "Bad Anime\n(FREE SLOT)";
                        continue;
                }
                int j = i + rand() % (s->text_num - i);
                int tmp = idx[i];
                idx[i] = idx[j];
                idx[j] = tmp;
                s->slot_texts[i] = s->pool[idx[i]];
        }
}