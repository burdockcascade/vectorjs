#pragma once
#include <string>
#include <qjspp.hpp>

namespace App {

    void show_welcome();
    void show_bsod(const std::string &errStr);

    class Core {
    public:
        // Lifecycle
        explicit Core();

        ~Core() = default;

        // Execution
        void eval_script(const std::string& scriptPath) const;

    private:
        qjspp::Engine engine = qjspp::Engine::large();

    };

}
