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

static JSModuleDef* js_module_loader(JSContext* ctx, const char* module_name, void* opaque) {
    auto* engine = static_cast<qjs::Engine*>(opaque);
    std::string name(module_name);

    // 1. Priority: Internal Bytecode Modules
    // This allows: import { App } from 'runtime';
    if (engine->embedded_modules.contains(name)) {
        TraceLog(LOG_DEBUG, "[Module] Loading embedded: %s", module_name);
        JSValue mod_obj = engine->embedded_modules[name];

        // We return the pointer to the module definition created by JS_ReadObject
        return static_cast<JSModuleDef*>(JS_VALUE_GET_PTR(mod_obj));
    }

    // 2. Fallback: External File Modules
    TraceLog(LOG_DEBUG, "[Module] Searching disk: %s", module_name);

    std::ifstream file(name);
    if (!file.is_open()) {
        JS_ThrowReferenceError(ctx, "Could not open module: %s", module_name);
        return nullptr;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    // Compile as Module
    // Note: Use the filename as the 'name' in Eval for better error stack traces
    JSValue func_val = JS_Eval(ctx, source.c_str(), source.size(), module_name,
                               JS_EVAL_TYPE_MODULE | JS_EVAL_FLAG_COMPILE_ONLY);

    if (JS_IsException(func_val)) {
        // The exception is already set in the context, QuickJS will report it
        return nullptr;
    }

    // Extract module definition and free the temporary JSValue
    auto* m = static_cast<JSModuleDef*>(JS_VALUE_GET_PTR(func_val));
    JS_FreeValue(ctx, func_val);

    return m;
}

Runner::Runner(std::string path) : scriptPath(std::move(path)) {
    declare_api(engine);
    RaylibBindings::InitRaylib(engine);

    // Set the module loader
    engine.set_module_loader(js_module_loader, &engine);

    // Load runtime bytecode
    TraceLog(LOG_DEBUG, "Loading runtime");
    engine.register_bytecode_module("vectorjs", qjsc_runtime, qjsc_runtime_size);
}

void Runner::Run() const {

    // Check if script exists
    if (const std::ifstream t(scriptPath); !t.is_open()) {
        TraceLog(LOG_ERROR, "Failed to open script: %s", scriptPath.c_str());
        return;
    }

    // Run user script
    TraceLog(LOG_INFO, "Running script: %s", scriptPath.c_str());
    if (auto result = engine.run_file_as_module(scriptPath.c_str()); !result) {
        TraceLog(LOG_ERROR, "Failed to run script: %s", result.error().c_str());
        return;
    }

}

