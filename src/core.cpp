#include <string>
#include <filesystem>
#include <iostream>
#include "core.hpp"
#include "engine/commands.hpp"
#include "engine/engine.hpp"
#include "info.hpp"

#include "modules/modules.hpp"

namespace App {

    constexpr int DEFAULT_FPS = 60;
    constexpr int WIN_HEIGHT = 600;
    constexpr int WIN_WIDTH = 800;

    Core::Core() {
        Modules::register_vectorjs_module(engine);
    }

    void Core::eval_script(const std::string& scriptPath) const {
        try {
            engine.exec_file(std::filesystem::path(scriptPath), JS_EVAL_TYPE_MODULE);
        } catch (const std::exception& e) {
            std::cerr << "Script evaluation failed: " << e.what() << '\n';
            show_bsod(e.what());
        }
    }

    void show_welcome() {

        // Allocate long-lived text strings to ensure pointer validity during draw calls
        std::string qjs_str = std::format("QuickJS Version: {}", QUICKJS_VERSION_STR);
        std::string raylib_str = std::format("Raylib Version: {}", RAYLIB_VERSION_STR);

        Hooray::Engine rengine{WIN_WIDTH, WIN_HEIGHT, "Welcome"};

        auto& builder = rengine.get_buffer();
        builder.clear_background(RAYWHITE);
        builder.draw_text("VectorJS", { 40.0f, 40.0f }, GetFontDefault(), 80.0f, DARKBLUE);
        builder.draw_text("Welcome to the VectorJS!", { 40.0f, 140.0f }, GetFontDefault(), 20.0f, DARKGRAY);
        builder.draw_text(qjs_str.c_str(), { 40.0f, 520.0f }, GetFontDefault(), 20.0f, LIGHTGRAY);
        builder.draw_text(raylib_str.c_str(), { 40.0f, 540.0f }, GetFontDefault(), 20.0f, LIGHTGRAY);
        builder.draw_text("Press ESC to exit.", { 40.0f, 560.0f }, GetFontDefault(), 20.0f, LIGHTGRAY);
        rengine.display();

    }

    void show_bsod(const std::string &errStr) {
        if (IsWindowReady()) CloseWindow();

        Hooray::Engine rengine{WIN_WIDTH, WIN_HEIGHT, "VectorJS - Fatal Error"};
        auto& builder = rengine.get_buffer();

        builder.clear_background(BLUE);
        builder.draw_text(":(", { 40.0f, 30.0f }, GetFontDefault(), 60.0f, WHITE);
        builder.draw_text("Your VectorJS script ran into a problem and crashed.", { 40.0f, 100.0f }, GetFontDefault(), 18.0f, WHITE);

        float yPos = 140.0f;
        std::istringstream stream(errStr);
        std::string line;
        while (std::getline(stream, line)) {
            if (yPos > static_cast<float>(WIN_HEIGHT - 60)) break; // Keep within window bounds

            // Ensure string contents remain alive by storing or passing directly to builder
            builder.draw_text(line.c_str(), { 40.0f, yPos }, GetFontDefault(), 14.0f, LIGHTGRAY);
            yPos += 18.0f;
        }

        builder.draw_text("Press ESC to exit.", { 40.0f, static_cast<float>(WIN_HEIGHT - 40) }, GetFontDefault(), 16.0f, LIGHTGRAY);

        rengine.display();
    }

}