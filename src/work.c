#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "raygui.h"
#include "work.h"
#include "draw.h"

// TODO/FIXME: This code is horrendous. Reused loops, easily factorizable code, and ugly mess.
// This entire file dearly needs some proper love and refactorization. But for now, it works :shrug:

// TODO: make helper function/macro for error message box

void generatePool(struct States *s)
{
        char *file = s->config->wordlist_src;
        FILE *f = fopen(file, "r");

        if (!f) {
                const char *fmt = "Couldn't open file. Make sure %s is next to executable.\n%s\n";
                s->errStop = true;
                s->page = PAGE_ERR;
                snprintf(s->msg, sizeof(s->msg), fmt, file, strerror(errno));
                fprintf(stderr, fmt, file, strerror(errno));
                perror("generatePool");
                return;
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

        if (s->text_num < TOTAL_ELEMS) {
                const char *fmt = "%zu texts found.\nat least %u needed for board of width %u.\n";
                s->errStop = true;
                s->page = PAGE_ERR;
                snprintf(s->msg, sizeof(s->msg), fmt, s->text_num, TOTAL_ELEMS, ELEMS);
                fprintf(stderr, fmt, s->text_num, TOTAL_ELEMS, ELEMS);
                fputs("Not enough items in word list.\n", stderr);
                return;
        }

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

void saveTexts(struct States *s)
{
        FILE *f = fopen(s->buf, "w");

        if (!f) {
                const char *fmt = "Couldn't open file to save.\n%s\n";
                s->errStop = false;
                s->page = PAGE_ERR;
                snprintf(s->msg, sizeof(s->msg), fmt, strerror(errno));
                fprintf(stderr, fmt, strerror(errno));
                perror("saveTexts");
                return;
        }

        for (size_t i = 0; i < TOTAL_ELEMS; ++i) {
                char tmp[TEXT_MAX_LEN+2];
                tmp[0] = (s->slots[i] ? '1' : '0');
                strncpy(tmp+1, s->slot_texts[i], TEXT_MAX_LEN);
                // TODO?: make helper functions to clean up the code. (like nl2sp or smth)
                for (char *c = tmp+1; *c; ++c)
                        if (*c == '\n')
                                *c = '|';
                fputs(tmp, f);
                fputc('\n', f);
        }

        fclose(f);
}

void loadTexts(struct States *s)
{
        FILE *f = fopen(s->buf, "r");

        if (!f) {
                const char *fmt = "Couldn't open file to load.\n%s\n";
                s->errStop = false;
                s->page = PAGE_ERR;
                snprintf(s->msg, sizeof(s->msg), fmt, strerror(errno));
                fprintf(stderr, fmt, strerror(errno));
                perror("saveTexts");
                return;
        }

        // TODO: add proper incorrect file errors handling
        //         stuff like wrong formatting, etc idk
        for (size_t i = 0; i < TOTAL_ELEMS; ++i) {
                s->slots[i] = (fgetc(f) == '1');  // TODO: error handling on fgetc ig?
                if (!fgets(s->pool[i], TEXT_MAX_LEN, f)) {
                        // TODO: make helper function/macro for error message box
                        const char *fmt = "Not enough items in %s\n";
                        s->errStop = false;
                        s->page = PAGE_ERR;
                        snprintf(s->msg, sizeof(s->msg), fmt, s->buf);
                        fprintf(stderr, fmt, s->buf);
                        return;
                }
                s->pool[i][strlen(s->pool[i]) - 1] = '\0';
                for (char *c = s->pool[i]; *c; ++c)
                        if (*c == '|')
                                *c = '\n';

                for (size_t i = 0; i < TOTAL_ELEMS; ++i) {
                        s->slot_texts[i] = s->pool[i];
                }
        }

        fclose(f);
}