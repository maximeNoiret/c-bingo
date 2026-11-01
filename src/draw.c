#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "raygui.h"

#include "draw.h"
#include "work.h"


Rectangle NextRight(struct Stack *s)
{
        Rectangle r = { s->x, s->y, s->w, s->h };
        s->x += s->w + s->gap;
        if (s->x + (float)s->w > s->boundaries.x + s->boundaries.width) {
                s->x = s->boundaries.x;
                s->y += s->h + s->gap;
        }
        return r;
}

Rectangle NextBottom(struct Stack *s)
{
        Rectangle r = { s->x, s->y, s->w, s->h };
        s->y += s->h + s->gap;
        if (s->y + (float)s->h > s->boundaries.y + s->boundaries.height) {
                s->y = s->boundaries.y;
                s->x += s->w + s->gap;
        }
        return r;
}

void drawBottomBar(struct States *s)
{
        Rectangle botbar_boundaries = {
                GENERAL_PAD,
                s->config->content_y + s->config->content_height + GENERAL_PAD,
                s->config->content_width,
                s->config->bottom_bar_width - GENERAL_PAD
        };
        struct Stack botbar = {
                .boundaries = botbar_boundaries,
                .x = GENERAL_PAD,
                .y = s->config->content_y + s->config->content_height + GENERAL_PAD,
                .w = s->config->bottom_bar_btn_w,
                .h = s->config->bottom_bar_btn_h,
                .gap = GENERAL_PAD*2
        };
        // SAVE button
        Rectangle btn_rect = NextRight(&botbar);
        if (!GuiIsLocked() &&
                CheckCollisionPointRec(GetMousePosition(), btn_rect) &&
                IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                s->page = PAGE_SAVE;
                memset(s->buf, '\0', sizeof(s->buf));
        }
        GuiButton(btn_rect, "Save Board");
        // LOAD button
        btn_rect = NextRight(&botbar);
        if (!GuiIsLocked() &&
                CheckCollisionPointRec(GetMousePosition(), btn_rect) &&
                IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                s->page = PAGE_LOAD;
                memset(s->buf, '\0', sizeof(s->buf));
        }
        GuiButton(btn_rect, "Load Board");

        // SETTINGS button
        Rectangle settings_btn = {
                botbar_boundaries.x + botbar_boundaries.width - s->config->bottom_bar_btn_w,
                botbar_boundaries.y,
                s->config->bottom_bar_btn_w,
                s->config->bottom_bar_btn_h};
        if (!GuiIsLocked() &&
                CheckCollisionPointRec(GetMousePosition(), settings_btn) &&
                IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                s->page = PAGE_SETT;
                memset(s->buf, '\0', sizeof(s->buf));
        }
        GuiButton(settings_btn, "Settings");

        // refresh button is square. W = H
        Rectangle refresh_btn = {
                settings_btn.x - s->config->bottom_bar_btn_h - GENERAL_PAD,
                botbar_boundaries.y,
                s->config->bottom_bar_btn_h,
                s->config->bottom_bar_btn_h};
        if (!GuiIsLocked() &&
                CheckCollisionPointRec(GetMousePosition(), refresh_btn) &&
                IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                refreshBoard(s);
        }
        GuiButton(refresh_btn, "#77#");
}

void drawBoard(struct States *s)
{
        // content boundaries and stack
        Rectangle bingo_boundaries = {
                s->config->content_x + GENERAL_PAD,
                s->config->content_y + GENERAL_PAD,
                s->config->content_width - 2*GENERAL_PAD,
                s->config->content_height - 2*GENERAL_PAD
        };
        struct Stack bingo_board = {
                .boundaries = bingo_boundaries,
                .x = s->config->content_x + GENERAL_PAD,
                .y = s->config->content_y + GENERAL_PAD,
                .w = s->config->space_width,
                .h = s->config->space_width,
                .gap = GENERAL_PAD
        };
        // bingo buttons
        if (s->page != PAGE_NONE) GuiLock();
        for (unsigned i = 0; i < s->config->board_spaces; ++i) {
                Rectangle btn_rect = NextRight(&bingo_board);
                if (
                        !GuiIsLocked() &&
                        CheckCollisionPointRec(GetMousePosition(), btn_rect) &&
                        IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                        s->slots[i] ^= 1;
                char text[TEXT_MAX_LEN];
                snprintf(text, TEXT_MAX_LEN, "%s%s", s->slot_texts[i], (s->slots[i] ? "\n[X]" : ""));
                GuiButton(btn_rect, text);
        }

        GuiUnlock();
}

void drawContent(struct States *s)
{
        GuiGroupBox((Rectangle){
                s->config->content_x,
                s->config->content_y,
                s->config->content_width,
                s->config->content_height}, NULL);
        
        drawBoard(s);
        switch (s->page) {
        case PAGE_NONE:
                break;
        case PAGE_SAVE:
                drawSaveDialog(s);
                break;
        case PAGE_LOAD:
                drawLoadDialog(s);
                break;
        case PAGE_SETT:
                drawSettingsPage(s);
                break;
        case PAGE_ERR:
                drawErrorMessage(s);
                break;
        }
}

void drawSaveDialog(struct States *s) {
        enum SAVE_BTNS btnActive = SAVE_NONE;

        GuiTextInputBox(
                (Rectangle){
                        PAGE_PAD,
                        PAGE_PAD,
                        200,
                        150},  // TODO: use adaptive size instead of fixed size
                "Save board",
                "Enter filepath to save board to",
                s->buf,
                MAX_FILE_PATH_LEN,
                "Cancel;Save",
                &btnActive,
                false);

        if (IsKeyPressed(KEY_ENTER))
                btnActive = SAVE_CONFIRM;

        switch(btnActive) {
        case SAVE_NONE:
                break;
        case SAVE_CONFIRM:
                saveTexts(s);
                if (s->page == PAGE_ERR)
                        break;
                [[fallthrough]];
        case SAVE_CLOSE:
        case SAVE_CANCEL:
                btnActive = SAVE_NONE;
                s->page = PAGE_NONE;
                break;
        }
}

void drawLoadDialog(struct States *s)
{
        enum SAVE_BTNS btnActive = SAVE_NONE;

        GuiTextInputBox(
                (Rectangle){
                        PAGE_PAD,
                        PAGE_PAD,
                        200,
                        150},  // TODO: use adaptive size instead of fixed size
                "Load board",
                "Enter filepath to load board from",
                s->buf,
                MAX_FILE_PATH_LEN,
                "Cancel;Load",
                &btnActive,
                false);

        if (IsKeyPressed(KEY_ENTER))
                btnActive = SAVE_CONFIRM;

        switch(btnActive) {
        case SAVE_NONE:
                break;
        case SAVE_CONFIRM:
                loadTexts(s);
                if (s->page == PAGE_ERR)
                        break;
                [[fallthrough]];
        case SAVE_CLOSE:
        case SAVE_CANCEL:
                btnActive = SAVE_NONE;
                s->page = PAGE_NONE;
                break;
        }
}

void drawSettingsPage(struct States *s)
{
        Rectangle settings_bound = {
                PAGE_PAD,
                PAGE_PAD,
                s->config->window_width-PAGE_PAD2,
                s->config->window_height-PAGE_PAD2};
        if (GuiWindowBox(settings_bound, "Settings") == 1) {
                applySettings(s);
                s->page = PAGE_NONE;
        }

        struct Stack labels = {
                .boundaries = (Rectangle) {
                        settings_bound.x + (GENERAL_PAD << 2),
                        settings_bound.y + GENERAL_PAD + BAR_WIDTH,
                        settings_bound.width - (GENERAL_PAD << 1),
                        settings_bound.height - (GENERAL_PAD << 1)
                },
                .x = settings_bound.x + (GENERAL_PAD << 2),
                .y = settings_bound.y + GENERAL_PAD + BAR_WIDTH,
                .w = settings_bound.width / 4,
                .h = BAR_WIDTH,  // TODO: Test different label height? idk
                .gap = GENERAL_PAD
        };

        struct Stack controls = {
                .boundaries = settings_bound,
                .x = labels.x + labels.w,  // TODO: see if this is good or not
                .y = settings_bound.y + GENERAL_PAD + BAR_WIDTH,
                .w = settings_bound.width / 4,
                .h = BAR_WIDTH,  // TODO: Test different label height? idk
                .gap = GENERAL_PAD
        };

        // autosave
        int tmp = (int)s->config->autosave;
        GuiLabel(NextBottom(&labels), "Autosave");
        GuiToggleSlider(NextBottom(&controls), "OFF;ON", &tmp);
        s->config->autosave = (tmp == 1);

        // auto-reload autosave
        tmp = (int)s->config->autoreload;
        GuiLabel(NextBottom(&labels), "Autoreload");
        GuiToggleSlider(NextBottom(&controls), "OFF;ON", &tmp);
        s->config->autoreload = (tmp == 1);

        // middle free space
        tmp = (int)s->config->freeSpace;
        GuiLabel(NextBottom(&labels), "Free Space");
        GuiToggleSlider(NextBottom(&controls), "OFF;ON", &tmp);
        s->config->freeSpace = (tmp == 1);

        // wordlist source file
        GuiLabel(NextBottom(&labels), "Source File");
        GuiTextBox(NextBottom(&controls), s->config->wordlist_src, sizeof(s->config->wordlist_src), true);

        // board size
        int boardSize = s->config->board_width - 3;
        GuiLabel(NextBottom(&labels), "Board Width");
        GuiToggleSlider(NextBottom(&controls), "3;4;5;6;7", &boardSize);
        s->config->board_width = boardSize + 3;
}

void drawErrorMessage(struct States *s)
{
        int btn = -1;
        GuiMessageBox(
                (Rectangle){50, 50, 400, 300},  // TODO: use adaptive size instead of fixed size
                "Error",
                s->msg,
                "Ok",
                &btn);

        if (btn >= 0) {
                if (s->errStop)
                        s->stop = true;
                else
                        s->page = PAGE_NONE;
        }
}