#include <string>
#include <filesystem>
#include <iostream>
#include "screens.hpp"
#include "core.hpp"

#include "api/vectorjs/vectorjs.hpp"

namespace App {

    Core::Core() {
        Module::VectorJS::register_module(engine);
    }

    void Core::eval_script(const std::string& scriptPath) const {
        try {
            engine.exec_file(std::filesystem::path(scriptPath), JS_EVAL_TYPE_MODULE);
        } catch (const std::exception& e) {
            std::cerr << "Script evaluation failed: " << e.what() << '\n';
            show_bsod(e.what());
        }
    }

}