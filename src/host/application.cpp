#include <string>
#include "application.hpp"
#include "qjs_wrapper.hpp"
#include "modules/rl_module.hpp"
#include "runtime.hpp"
#include "screens.hpp"
#include "hostapi.hpp"

Application::Application() {
    HostApi::register_host_api(engine);
    RaylibModule::register_raylib_module(engine);

    // Load runtime bytecode
    TraceLog(LOG_DEBUG, "Loading runtime");
    engine.register_module_bytecode("vectorjs", qjsc_runtime, qjsc_runtime_size);
}

void Application::Run(std::string scriptPath) const {

    Log::Info("Running script: {}", scriptPath);

    auto result = engine.eval_file(scriptPath.c_str(), qjs::EvalMode::Module);

    // Check if the script evaluation returned an error
    if (!result.has_value()) {
        const std::string errStr = result.error().to_string();
        VectorJS::show_bsod(errStr);
    }
}