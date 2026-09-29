#include <CLI/CLI.hpp>
#include "core.hpp"

int main(const int argc, char** argv) {

    CLI::App cli{"VectorJS"};

    // Run (argument)
    std::string scriptInput;
    cli.add_option("script", scriptInput, "Path to the JS game script");

    try {
        cli.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        std::cerr << e.what() << std::endl;
        return cli.exit(e);
    }

    try {
        App::Core app{};
        if (scriptInput.empty()) {
            App::show_welcome();
        } else {
            app.eval_script(scriptInput);
        }
    } catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        App::show_bsod(e.what());
        return 1;
    }

    return 0;
}