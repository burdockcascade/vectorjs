#include <CLI/CLI.hpp>
#include <string>
#include <print>
#include <exception> 
#include "runner.hpp"
#include "logger.hpp"
#include "screens.hpp"

int main(const int argc, char** argv) {

    Log::Init();

    CLI::App args{"VectorJS"};

    std::string scriptPath;
    args.add_option("script", scriptPath, "Path to the JS game script")
        ->check(CLI::ExistingFile);

    bool verbose = false;
    args.add_flag("-d,--debug", verbose, "Enable debug logging");

    CLI11_PARSE(args, argc, argv);

    // Unified level setting
    if (verbose) {
        Log::SetLevel(spdlog::level::debug);
    }

    if (scriptPath.empty()) {
        VectorJS::show_welcome();
        return 0;
    }

    try {
        const Runner runner(scriptPath);
        runner.Run();
    } catch (const std::exception& e) {
        Log::Error("{}", e.what());
        return 1;
    }

    return 0;
}