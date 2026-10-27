#include "raylib.h"
#include "string.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "draw.h"

int main(void)
{
        InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Bingo");
        EnableEventWaiting();
        SetTargetFPS(60);

        bool stop = false;

        char *texts[25] = {
                "Summoning",
                "Rebirth",
                "Reincarnation",
                "OP for no\nreason",
                "Somehow knows\nthe language",
                "Bad long intro",
                "20 years later...",
                "Cheat skill",
                "Goddess\nappears",
                "Adventurers'\nGuild",
                "Status screen",
                "Demon Lord",
                "Truck-kun\n(FREE SPACE)",
                "Slime\nprotagonist",
                "Harem forms\nunexplained",
                "Modern knowledge\nsolves everything",
                "Dies from\nmundane cause",
                "Elf companion",
                "Beast-girl\nslave",
                "Rank-up\nexam",
                "Tavern\nbrawl",
                "Blushing\nat praise",
                "Weakest class\nis secretly best",
                "Villainess\nreincarnation",
                "Overpowered\nin episode 1"
        };

        struct States states = {
                .slot_texts = NULL,
                .slots = {0}
        };

        states.slot_texts = texts;

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
