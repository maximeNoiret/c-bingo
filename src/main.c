#include <raylib.h>
#include <string.h>
#include <time.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "draw.h"
#include "work.h"

int main(void)
{
        InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Bingo");
        EnableEventWaiting();
        SetTargetFPS(20);

        bool stop = false;

        // TODO: load text from file (should create a file.c or data.c ig?)

        struct States states = {
                .pool = {},
                .slot_texts = {},
                .text_num = 0,
                .slots = {0},
                .page = PAGE_NONE,
                .buf = {0}
        };

        generatePool(&states, "wordlist.txt");
        generateTexts(&states, time(NULL));

        while (!stop)
        {
                BeginDrawing();

                // silly thingy, probably not a good idea but eh
                if (states.page != PAGE_NONE) DisableEventWaiting();
                else EnableEventWaiting();

                ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
                if (GuiWindowBox(
                        (Rectangle){0, 0, WINDOW_WIDTH, WINDOW_HEIGHT},
                        "Bingo") == 1)
                        stop = true;

                drawBottomBar(&states);
                drawContent(&states);

                EndDrawing();
                if (WindowShouldClose())
                        stop = true;
        }

        strncpy(states.buf, "autosave.txt", MAX_FILE_PATH_LEN);
        saveTexts(&states);

        CloseWindow();
        return 0;
}
