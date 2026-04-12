#include <string>
#include <raylib.h> // Ensure we include Raylib to use its rendering functions
#include "runner.hpp"
#include "qjs_wrapper.hpp"
#include "modules/rl_module.hpp"
#include "logger.hpp"

Runner::Runner(std::string path) : scriptPath(std::move(path)) {
    HostApi::register_host_api(engine);
    RaylibModule::register_raylib_module(engine);

    // todo: runtime for future release
    // Load runtime bytecode
    //TraceLog(LOG_DEBUG, "Loading runtime");
    //engine.register_module_bytecode("vectorjs", qjsc_runtime, qjsc_runtime_size);
}

static void show_bsod(const std::string &errStr) {
    if (!IsWindowReady()) {
        InitWindow(800, 600, "VectorJS - Fatal Error");
        SetTargetFPS(60);
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLUE);
        DrawText(":(", 40, 40, 80, WHITE);
        DrawText("Your VectorJS script ran into a problem and crashed.", 40, 140, 20, WHITE);
        DrawText(errStr.c_str(), 40, 190, 20, LIGHTGRAY);
        DrawText("Press ESC to exit.", 40, GetScreenHeight() - 40, 20, WHITE);
        EndDrawing();
    }

    // Clean up the window after the user dismisses the BSOD
    if (IsWindowReady()) {
        CloseWindow();
    }
}

void Runner::Run() const {

    auto result = engine.eval_file(scriptPath.c_str(), qjs::EvalMode::Module);

    // Check if the script evaluation returned an error
    if (!result.has_value()) {
        std::string errStr = result.error().to_string();
        show_bsod(errStr);
    }
}