#include <raylib.h>
#include <string.h>
#include <time.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "draw.h"
#include "work.h"

int main(void)
{
        srand(time(NULL));

        struct Config config;
        createConfigFile(CONFIG_FILE);
        loadConfigFile(&config, CONFIG_FILE);
        refreshConfig(&config);
        config.tmp = config.board_width;

        struct States states = {
                .config = &config,
                .pool = {},
                .slot_texts = {},
                .text_num = 0,
                .slots = {0},
                .page = PAGE_NONE,
                .buf = {0},
                .stop = false
        };

        if (config.autoreload) {
                strncpy(states.buf, "autosave.txt", 13);
                loadTexts(&states);
        } else {
                generatePool(&states);
                if (states.page != PAGE_ERR)
                        generateTexts(&states);
        }

        InitWindow(config.window_width, config.window_height, "Bingo");
        EnableEventWaiting();
        SetTargetFPS(20);
        SetExitKey(0);

        while (!states.stop)
        {
                BeginDrawing();

                if (IsKeyPressed(KEY_ESCAPE)) {
                        if (states.page == PAGE_SETT)
                                applySettings(&states);
                        states.page = PAGE_NONE;
                }

                // silly thingy, probably not a good idea but eh
                if (states.page != PAGE_NONE) DisableEventWaiting();
                else EnableEventWaiting();

                ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
                if (GuiWindowBox(
                        (Rectangle){0, 0, config.window_width, config.window_height},
                        "Bingo") == 1)
                        states.stop = true;

                drawBottomBar(&states);
                drawContent(&states);

                EndDrawing();
                if (WindowShouldClose())
                        states.stop = true;
        }

        if (!states.errStop && states.config->autosave) {
                strncpy(states.buf, "autosave.txt", MAX_FILE_PATH_LEN);
                saveTexts(&states);
        }

        CloseWindow();
        return 0;
}
