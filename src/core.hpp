#pragma once
#include "js_engine.hpp"

namespace VectorJS {

    class Core {
    public:
        // Lifecycle
        explicit Core();

        ~Core() = default;

        // Execution
        void eval_script(const std::string& scriptPath) const;

    private:
        JSEngine js_engine;

    };

    void show_welcome();
    void show_bsod(const std::string &errStr);

}
