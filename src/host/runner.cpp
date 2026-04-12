#include <print>
#include <string>
#include <fstream>
#include "runner.hpp"
#include "qjs_wrapper.hpp"
#include "modules/rl_module.hpp"

static void register_console_object(qjs::Engine& engine) {
    auto global = engine.global();

    // Create a global 'console' object
    auto console = engine.make_object();

    // Basic logging functions
    console.set("log", [](std::string msg) {
        std::println("[log] {}", msg);
    });

    // Error logging
    console.set("error", [](std::string msg) {
        std::println("[error] {}", msg);
    });

    // Warning logging
    console.set("warn", [](std::string msg) {
        std::println("[warn] {}", msg);
    });

    global.set("console", std::move(console));

}

Runner::Runner(std::string path) : scriptPath(std::move(path)) {
    register_console_object(engine);
    RaylibModule::register_raylib_module(engine);

    // todo: runtime for future release
    // Load runtime bytecode
    //TraceLog(LOG_DEBUG, "Loading runtime");
    //engine.register_module_bytecode("vectorjs", qjsc_runtime, qjsc_runtime_size);

}

void Runner::Run() const {

    // Run user script
    TraceLog(LOG_INFO, "Running script: %s", scriptPath.c_str());
    if (const auto result = engine.eval_file(scriptPath.c_str(), qjs::EvalMode::Module); !result) {
        std::println("Error: {}", result.error());
    }

}

