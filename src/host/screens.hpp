#pragma once

#include "raylib.h"

namespace VectorJS {

    constexpr int DEFAULT_FPS = 60;
    constexpr int WIN_HEIGHT = 600;
    constexpr int WIN_WIDTH = 800;

    inline void show_welcome() {
        InitWindow(WIN_WIDTH, WIN_HEIGHT, "VectorJS - Welcome");
        SetTargetFPS(DEFAULT_FPS);

        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawText("VectorJS", 40, 40, 80, DARKBLUE);
            DrawText("Welcome to the VectorJS runtime!", 40, 140, 20, DARKGRAY);

            DrawText("To run a game, pass a script path as an argument.", 40, 190, 20, GRAY);
            DrawText("Example: vectorjs my_game.js", 40, 230, 20, GRAY);

            DrawText("Press ESC to exit.", 40, GetScreenHeight() - 40, 20, LIGHTGRAY);
            EndDrawing();
        }

        if (IsWindowReady()) {
            CloseWindow();
        }
    }

    inline void show_bsod(const std::string &errStr) {
        if (!IsWindowReady()) {
            InitWindow(WIN_WIDTH, WIN_HEIGHT, "VectorJS - Fatal Error");
            SetTargetFPS(DEFAULT_FPS);
        }

        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(BLUE);
            DrawText(":(", 40, 40, 80, WHITE);
            DrawText("Your VectorJS script ran into a problem and crashed.", 40, 140, 20, WHITE);
            DrawText(errStr.c_str(), 40, 190, 20, LIGHTGRAY);
            DrawText("Press ESC to exit.", 40, GetScreenHeight() - 40, 20, LIGHTGRAY);
            EndDrawing();
        }

        // Clean up the window after the user dismisses the BSOD
        if (IsWindowReady()) {
            CloseWindow();
        }
    }
}