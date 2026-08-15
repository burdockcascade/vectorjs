#pragma once
#include <raylib.h>
#include <string>
#include <filesystem>
#include "info.hpp"

namespace VectorJS {

    constexpr int DEFAULT_FPS = 60;
    constexpr int WIN_HEIGHT = 600;
    constexpr int WIN_WIDTH = 800;

    inline void show_welcome() {
        InitWindow(WIN_WIDTH, WIN_HEIGHT, "VectorJS");
        SetTargetFPS(DEFAULT_FPS);
        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("VectorJS", 40, 40, 80, DARKBLUE);
            DrawText("Welcome to the VectorJS!", 40, 140, 20, DARKGRAY);
            DrawText(std::format("QuickJS Version: {}", QUICKJS_VERSION_STR).c_str(), 40, GetScreenHeight() - 80, 20, LIGHTGRAY);
            DrawText(std::format("Raylib Version: {}", RAYLIB_VERSION_STR).c_str(), 40, GetScreenHeight() - 60, 20, LIGHTGRAY);
            DrawText("Press ESC to exit.", 40, GetScreenHeight() - 40, 20, LIGHTGRAY);
            EndDrawing();
        }
        CloseWindow();
    }

    inline void show_bsod(const std::string &errStr) {
        if (IsWindowReady()) CloseWindow();
        InitWindow(WIN_WIDTH, WIN_HEIGHT, "VectorJS - Fatal Error");
        SetTargetFPS(DEFAULT_FPS);
        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(BLUE);
            DrawText(":(", 40, 30, 60, WHITE);
            DrawText("Your VectorJS script ran into a problem and crashed.", 40, 100, 18, WHITE);
            int yPos = 140;
            std::istringstream stream(errStr);
            std::string line;
            while (std::getline(stream, line)) {
                if (yPos > WIN_HEIGHT - 60) break; // Keep within window bounds
                DrawText(line.c_str(), 40, yPos, 14, LIGHTGRAY);
                yPos += 18;
            }
            DrawText("Press ESC to exit.", 40, GetScreenHeight() - 40, 16, LIGHTGRAY);
            EndDrawing();
        }
        CloseWindow();
    }

}