#include <string>
#include <fstream>
#include "runner.hpp"
#include "runtime.hpp"
#include "qjs_wrapper.hpp"
#include "rl_bindings.hpp"

static void declare_api(qjs::Engine& engine) {
    auto global = engine.global();

    // Create a global 'console' object
    auto console = engine.make_object();

    // Basic logging functions
    console.set("log", [](std::string msg) {
        TraceLog(LOG_INFO, "%s", msg.c_str());
    });

    // Error logging
    console.set("error", [](std::string msg) {
        TraceLog(LOG_ERROR, "%s", msg.c_str());
    });

    // Warning logging
    console.set("warn", [](std::string msg) {
        TraceLog(LOG_WARNING, "%s", msg.c_str());
    });

}

Runner::Runner(std::string path) : scriptPath(std::move(path)) {
    declare_api(engine);
    RaylibBindings::InitRaylib(engine);

    // Load runtime bytecode
    TraceLog(LOG_DEBUG, "Loading runtime");
    engine.register_module_bytecode("vectorjs", qjsc_runtime, qjsc_runtime_size);

}

void Runner::Run() const {

    // Check if script exists
    if (const std::ifstream t(scriptPath); !t.is_open()) {
        TraceLog(LOG_ERROR, "Failed to open script: %s", scriptPath.c_str());
        return;
    }

    // Run user script
    TraceLog(LOG_INFO, "Running script: %s", scriptPath.c_str());
    if (auto result = engine.eval_file(scriptPath.c_str(), qjs::EvalMode::Module); !result) {
        TraceLog(LOG_ERROR, "Failed to run script: %s", result.error().c_str());
        return;
    }

}

