#include <string>
#include <fstream>
#include "runner.hpp"
#include "runtime.hpp"
#include "qjs_wrapper.hpp"
#include "rl_bindings.hpp"

static void add_console_utilities(const qjs::ObjectBinder& global) {

    // Create a global 'console' object
    auto console = global.create_object("console");
    
    // Basic logging functions
    console.register_function("log", [](std::string msg) {
        TraceLog(LOG_INFO, "%s", msg.c_str());
    });

    // Error logging
    console.register_function("error", [](std::string msg) {
        TraceLog(LOG_ERROR, "%s", msg.c_str());
    });

    // Warning logging
    console.register_function("warn", [](std::string msg) {
        TraceLog(LOG_WARNING, "%s", msg.c_str());
    });

}

static void declare_api(qjs::Engine& engine) {
    auto global = engine.get_global_object();

    // Declare any additional global utilities or namespaces here
    add_console_utilities(global);

}

Runner::Runner(std::string path) : scriptPath(std::move(path)) {
    declare_api(engine);
    RaylibBindings::InitRaylib(engine);
}

void Runner::Run() const {

    // Check if script exists
    if (const std::ifstream t(scriptPath); !t.is_open()) {
        TraceLog(LOG_ERROR, "Failed to open script: %s", scriptPath.c_str());
        return;
    }

    // Load runtime bytecode
    TraceLog(LOG_INFO, "Loading runtime");
    if (auto result = engine.run_bytecode(qjsc_runtime, qjsc_runtime_size); !result) {
        TraceLog(LOG_ERROR, "Failed to load runtime bytecode: %s", result.error().c_str());
        return;
    }

    // Run user script
    TraceLog(LOG_INFO, "Running script: %s", scriptPath.c_str());
    if (auto result = engine.run_file(scriptPath.c_str()); !result) {
        TraceLog(LOG_ERROR, "Failed to run script: %s", result.error().c_str());
        return;
    }

}

