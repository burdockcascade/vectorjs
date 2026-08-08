#pragma once
#include <string>
#include "qjs.hpp"

namespace VectorJS {

    class Core {
    public:
        // Lifecycle
        explicit Core();

        ~Core() = default;

        // Execution
        void eval_script(const std::string& scriptPath) const;

    private:
        qjs::Context ctx;

    };

    void show_welcome();
    void show_bsod(const std::string &errStr);

}
