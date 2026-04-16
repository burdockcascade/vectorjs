#include <CLI/CLI.hpp>
#include <string>
#include <print>
#include <exception>
#include "application.hpp"
#include "logger.hpp"
#include "screens.hpp"

int main(const int argc, char** argv) {

    Log::Init();

    CLI::App cli{"VectorJS"};

    // Run (Subcommand)
    bool debug_mode = false;
    std::string scriptInput;
    const auto run_cmd = cli.add_subcommand("run", "Run a JS game script");
    run_cmd->add_option("script", scriptInput, "Path to the JS game script")->required();
    run_cmd->add_flag("-d,--debug", debug_mode, "Enable debug logging");

    // Compile
    std::string compileInput;
    std::string outputFile;
    const auto compile_cmd = cli.add_subcommand("compile", "Compile a JS script into bytecode");
    compile_cmd->add_option("script", compileInput, "Path to the JS script")->required();
    compile_cmd->add_option("-o,--output", outputFile, "Output bytecode file path");

    // Run (argument)
    cli.add_option("script", scriptInput, "Path to the JS game script");

    try {
        cli.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return cli.exit(e);
    }

    if (cli.get_subcommands().empty() && scriptInput.empty()) {
        VectorJS::show_help();
        return 0;
    }

    if (debug_mode) {
        Log::SetLevel(Log::Level::Trace);
    }

    // Start App
    const Application app{};


    // Run Script
    if (run_cmd->parsed() || !scriptInput.empty()) {
        try {
            app.Run(scriptInput);
        } catch (const std::exception& e) {
            Log::Error("{}", e.what());
            return 1;
        }
    }

    return 0;
}