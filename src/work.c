#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "raygui.h"
#include "work.h"
#include "draw.h"

// TODO/FIXME: This code is horrendous. Reused loops, easily factorizable code, and ugly mess.
// This entire file dearly needs some proper love and refactorization. But for now, it works :shrug:


void refreshConfig(struct Config *c)
{
        // FIXME: This is messy no?
        c->board_spaces       = c->board_width * c->board_width;
        c->content_x          = GENERAL_PAD;  // NOTE: This will matter when scaling is added
        c->content_y          = (BAR_WIDTH + GENERAL_PAD);
        c->content_width      = (c->board_width * c->space_width + (c->board_width+1) * GENERAL_PAD);
        c->content_height     = c->content_width;
        c->bottom_bar_width   = BOTTOM_BAR_WIDTH;
        c->bottom_bar_btn_h   = BOTTOM_BAR_WIDTH - (2 * GENERAL_PAD);
        c->bottom_bar_btn_w   = c->board_width * BOTTOM_BUTTON_COEF;
        c->window_width       = c->content_width + (2 * GENERAL_PAD);
        c->window_height      = c->content_height + 2 * GENERAL_PAD + BAR_WIDTH + c->bottom_bar_width;
}

void applySettings(struct States *s)
{
        refreshConfig(s->config);
        refreshBoard(s);
        SetWindowSize(s->config->window_width, s->config->window_height);
}

// TODO: make helper function/macro for error message box

void generatePool(struct States *s)
{
        char *file = s->config->wordlist_src;
        FILE *f = fopen(file, "r");
        // TODO: Make this not close but instead inform to set wordlist source in config
        if (!f) {
                const char *fmt = "Couldn't open file. Make sure '%s' is next to executable.\n%s\n";
                s->errStop = false;
                s->page = PAGE_ERR;
                snprintf(s->msg, sizeof(s->msg), fmt, file, strerror(errno));
                fprintf(stderr, fmt, file, strerror(errno));
                perror("generatePool");
                return;
        }

        s->text_num = 0;
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

void generateTexts(struct States *s)
{
        // TODO: Make this not close but instead inform to set wordlist source in config and click refresh
        if (s->text_num < s->config->board_spaces) {
                const char *fmt = "%zu texts found.\nat least %u needed for board of width %u.\n";
                s->errStop = false;
                s->page = PAGE_ERR;
                snprintf(s->msg, sizeof(s->msg), fmt, s->text_num,
                        s->config->board_spaces, s->config->board_width);
                fprintf(stderr, fmt, s->text_num,
                        s->config->board_spaces, s->config->board_width);
                fputs("Not enough items in word list.\n", stderr);
                return;
        }

        int idx[s->text_num];
        for (size_t i = 0; i < s->text_num; ++i)
                idx[i] = i;

        for (size_t i = 0; i < s->config->board_spaces; ++i) {
                if (i == (s->config->board_spaces >> 1) && s->config->freeSpace) {
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

        for (size_t i = 0; i < s->config->board_spaces; ++i) {
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
        for (size_t i = 0; i < s->config->board_spaces; ++i) {
                s->slots[i] = (fgetc(f) == '1');  // TODO: error handling on fgetc ig?
                if (!fgets(s->pool[i], TEXT_MAX_LEN, f)) {
                        // TODO: make helper function/macro for error message box
                        const char *fmt = "Not enough items in '%s'\n";
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

                for (size_t i = 0; i < s->config->board_spaces; ++i) {
                        s->slot_texts[i] = s->pool[i];
                }
        }

        fclose(f);
}

void refreshBoard(struct States *s)
{
        memset(s->slots, 0, sizeof(s->slots));
        generatePool(s);
        generateTexts(s);
}