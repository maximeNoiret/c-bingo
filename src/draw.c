#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "raygui.h"

#include "draw.h"
#include "work.h"


const Rectangle content_rect = {CONTENT_X, CONTENT_Y, CONTENT_WIDTH, CONTENT_HEIGHT};

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
                CONTENT_Y + CONTENT_HEIGHT + GENERAL_PAD,
                CONTENT_WIDTH,
                BOTTOM_BAR_WIDTH - GENERAL_PAD
        };
        struct Stack botbar = {
                .boundaries = botbar_boundaries,
                .x = GENERAL_PAD,
                .y = CONTENT_Y + CONTENT_HEIGHT + GENERAL_PAD,
                .w = BOTTOM_BAR_BTN_W,
                .h = BOTTOM_BAR_BTN_H,
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

        Rectangle settings_btn = {
                botbar_boundaries.x + botbar_boundaries.width - BOTTOM_BAR_BTN_W,
                botbar_boundaries.y,
                BOTTOM_BAR_BTN_W,
                BOTTOM_BAR_BTN_H};
        if (!GuiIsLocked() &&
                CheckCollisionPointRec(GetMousePosition(), settings_btn) &&
                IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                s->page = PAGE_SETT;
                memset(s->buf, '\0', sizeof(s->buf));
        }
        GuiButton(settings_btn, "Settings");

        // refresh button is square. W = H
        Rectangle refresh_btn = {
                settings_btn.x - BOTTOM_BAR_BTN_H - GENERAL_PAD,
                botbar_boundaries.y,
                BOTTOM_BAR_BTN_H,
                BOTTOM_BAR_BTN_H};
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
                content_rect.x + GENERAL_PAD,
                content_rect.y + GENERAL_PAD,
                content_rect.width - 2*GENERAL_PAD,
                content_rect.height - 2*GENERAL_PAD
        };
        struct Stack bingo_board = {
                .boundaries = bingo_boundaries,
                .x = CONTENT_X + GENERAL_PAD,
                .y = CONTENT_Y + GENERAL_PAD,
                .w = ELEM_WIDTH,
                .h = ELEM_WIDTH,
                .gap = GENERAL_PAD
        };
        // bingo buttons
        if (s->page != PAGE_NONE) GuiLock();
        for (unsigned i = 0; i < TOTAL_ELEMS; ++i) {
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
        GuiGroupBox(content_rect, NULL);
        
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
                (Rectangle){50, 50, 200, 150},  // TODO: use adaptive size instead of fixed size
                "Save board",
                "Enter filepath to save board",
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
                (Rectangle){50, 50, 200, 150},  // TODO: use adaptive size instead of fixed size
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
        Rectangle settings_bound = {50, 50, 400, 300};  // TODO: use adaptive size instead of fixed size
        if (GuiWindowBox(settings_bound, "Settings") == 1)
                s->page = PAGE_NONE;

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
        int autosave = (int)s->config->autosave;
        GuiLabel(NextBottom(&labels), "Autosave");
        GuiToggleSlider(NextBottom(&controls), "OFF;ON", &autosave);

        s->config->autosave = (autosave == 1);

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