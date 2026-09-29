#include <string>
#include <filesystem>
#include <iostream>
#include <sstream>
#include "core.hpp"

#include "modules/modules.hpp"

namespace App {

    constexpr int DEFAULT_FPS = 60;
    constexpr int WIN_HEIGHT = 600;
    constexpr int WIN_WIDTH = 800;

    Core::Core() {
        Modules::register_raylib_module(js_engine);
    }

    void Core::eval_script(const std::string& scriptPath) const {

        // Set the QuickJS version as a global variable in the JS context
        js_engine.set_global("QUICKJS_VERSION", std::format("{}.{}.{}", QJS_VERSION_MAJOR, QJS_VERSION_MINOR, QJS_VERSION_PATCH));

        try {
            js_engine.exec_file(std::filesystem::path(scriptPath), JS_EVAL_TYPE_MODULE);
        } catch (const std::exception& e) {
            std::cerr << "Script evaluation failed: " << e.what() << '\n';
            show_bsod(e.what());
        }
    }

    void show_welcome() {


    }

    void show_bsod(const std::string &errStr) {

    }

}