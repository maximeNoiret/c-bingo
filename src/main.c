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
        SetTargetFPS(60);

        bool stop = false;

        // TODO: load text from file (should create a file.c or data.c ig?)

        struct States states = {
                .pool = {},
                .slot_texts = {},
                .text_num = 0,
                .slots = {0}
        };

        generatePool(&states);
        generateTexts(&states, time(NULL));

        while (!stop)
        {
                BeginDrawing();

                ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
                if (GuiWindowBox(
                        (Rectangle){0, 0, WINDOW_WIDTH, WINDOW_HEIGHT},
                        "Bingo") == 1)
                        stop = true;

                drawContent(&states);

                EndDrawing();
                if (WindowShouldClose())
                        stop = true;
        }

        CloseWindow();
        return 0;
}
