#include <print>
#include <string>
#include <fstream>
#include "runner.hpp"
#include "qjs_wrapper.hpp"
#include "modules/rl_module.hpp"
#include "logger.hpp"

static void register_console_object(qjs::Engine& engine) {
    auto global = engine.global();

    // Create a global 'console' object
    auto console = engine.make_object();

    // Basic logging functions
    console.set_function("log", [](std::string msg) {
        Log::Info("{}", msg);
    });

    // Error logging
    console.set_function("error", [](std::string msg) {
        Log::Error("{}", msg);
    });

    // Warning logging
    console.set_function("warn", [](std::string msg) {
        Log::Warn("{}", msg);
    });

    global.set_constant("console", std::move(console));

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
    Log::Debug("Running script: {}", scriptPath);
    auto result = engine.eval_file(scriptPath.c_str(), qjs::EvalMode::Module);
    if (!result.has_value()) {
        Log::Error("{}", result.error().to_string());
    }

}

