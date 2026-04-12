#include <CLI/CLI.hpp>
#include <raylib.h>
#include <string>
#include <print>
#include <exception> 
#include "runner.hpp"

int main(const int argc, char** argv) {

    SetTraceLogLevel(LOG_NONE);

    CLI::App args{"VectorJS"};

    // Required option for script path
    std::string scriptPath;
    args.add_option("script", scriptPath, "Path to the JS game script")
        ->check(CLI::ExistingFile);

    // Optional flag for verbose logging
    bool verbose = false;
    args.add_flag("-d,--debug", verbose, "Enable debug logging");

    // Enable verbose logging if the flag is set
    if (verbose) {
        SetTraceLogLevel(LOG_DEBUG);
    }

    CLI11_PARSE(args, argc, argv);

    try {
        const Runner runner(scriptPath);
        runner.Run();
    } catch (const std::exception& e) {
        std::println("Error: {}", e.what());
        return 1;
    }

    return 0;
}