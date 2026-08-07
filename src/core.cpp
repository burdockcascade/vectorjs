#include <string>
#include "core.hpp"

#include "api/hostapi.hpp"

namespace VectorJS {

    Core::Core() {
        HostApi::make_vectorjs_module(ctx.raw_context());
    }

    void Core::eval_script(const std::string& scriptPath) {
        ctx.eval_file(scriptPath);
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
            DrawText(std::format("QuickJS Version: {}", HostApi::QUICKJS_VERSION_STR).c_str(), 40, GetScreenHeight() - 80, 20, LIGHTGRAY);
            DrawText(std::format("Raylib Version: {}", HostApi::RAYLIB_VERSION_STR).c_str(), 40, GetScreenHeight() - 60, 20, LIGHTGRAY);
            DrawText("Press ESC to exit.", 40, GetScreenHeight() - 40, 20, LIGHTGRAY);
            EndDrawing();
        }
        CloseWindow();
    }

    void show_bsod(const std::string &errStr) {
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
        CloseWindow();
    }

}