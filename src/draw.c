#include <raylib.h>
#include <math.h>
#include <stdio.h>
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

void drawContent(struct States *states)
{
        GuiGroupBox(content_rect, NULL);
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
        for (unsigned i = 0; i < TOTAL_ELEMS; ++i) {
                Rectangle btn_rect = NextRight(&bingo_board);
                if (
                        !GuiIsLocked() &&
                        CheckCollisionPointRec(GetMousePosition(), btn_rect) &&
                        IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                        states->slots[i] ^= 1;
                char text[TEXT_MAX_LEN];
                snprintf(text, TEXT_MAX_LEN, "%s%s", states->slot_texts[i], (states->slots[i] ? "\n[X]" : ""));
                GuiButton(btn_rect, text);
        }
}