#include <CLI/CLI.hpp>
#include <string>
#include <print>
#include <exception>
#include "application.hpp"
#include "logger.hpp"
#include "screens.hpp"

int main(const int argc, char** argv) {

    Log::Init();

    CLI::App app{"VectorJS"};

    // Run (Subcommand)
    bool debug_mode = false;
    std::string scriptInput;
    const auto run_cmd = app.add_subcommand("run", "Run a JS game script");
    run_cmd->add_option("script", scriptInput, "Path to the JS game script")->required();
    run_cmd->add_flag("-d,--debug", debug_mode, "Enable debug logging");

    // Run (argument)
    app.add_option("script", scriptInput, "Path to the JS game script");

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    if (app.get_subcommands().empty()) {
        VectorJS::show_welcome();
        VectorJS::show_help();
        return 0;
    }

    if (debug_mode) {
        Log::SetLevel(Log::Level::Trace);
    }

    // Run Script
    if (run_cmd->parsed() || !scriptInput.empty()) {
        try {
            const Application runner(scriptInput);
            runner.Run();
        } catch (const std::exception& e) {
            Log::Error("{}", e.what());
            return 1;
        }
    }

    return 0;
}