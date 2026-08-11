#include <string>
#include "core.hpp"

#include <iostream>
#include <sstream>

#include "api/hostapi.hpp"

namespace VectorJS {

    Core::Core() {
        make_vectorjs_module(ctx.raw_context());
    }

    void Core::eval_script(const std::string& scriptPath) const {
        try {
            std::ignore = ctx.eval_file(scriptPath);
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            show_bsod(e.what());
        }
    }

    constexpr int DEFAULT_FPS = 60;
    constexpr int WIN_HEIGHT = 600;
    constexpr int WIN_WIDTH = 800;

    void show_welcome() {
        InitWindow(WIN_WIDTH, WIN_HEIGHT, "VectorJS");
        SetTargetFPS(DEFAULT_FPS);
        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("VectorJS", 40, 40, 80, DARKBLUE);
            DrawText("Welcome to the VectorJS!", 40, 140, 20, DARKGRAY);
            DrawText(std::format("QuickJS Version: {}", VectorJS::QUICKJS_VERSION_STR).c_str(), 40, GetScreenHeight() - 80, 20, LIGHTGRAY);
            DrawText(std::format("Raylib Version: {}", VectorJS::RAYLIB_VERSION_STR).c_str(), 40, GetScreenHeight() - 60, 20, LIGHTGRAY);
            DrawText("Press ESC to exit.", 40, GetScreenHeight() - 40, 20, LIGHTGRAY);
            EndDrawing();
        }
        CloseWindow();
    }

    void show_bsod(const std::string &errStr) {
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