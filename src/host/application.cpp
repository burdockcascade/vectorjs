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

void Application::eval_script(std::string scriptPath) const {

    Log::Info("Running script: {}", scriptPath);

    auto result = engine.eval_file(scriptPath.c_str(), qjs::EvalMode::Module);

    // Check if the script evaluation returned an error
    if (!result.has_value()) {
        const std::string errStr = result.error().to_string();
        VectorJS::show_bsod(errStr);
    }
}

void Application::eval_bytecode(const std::string& bytecodePath) {
    Log::Info("Executing bytecode: {}", bytecodePath);

    // Open the file and determine size
    std::ifstream file(bytecodePath, std::ios::binary | std::ios::ate);
    if (!file) {
        Log::Error("Failed to open bytecode file: {}", bytecodePath);
        return;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read file content into a buffer
    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        Log::Error("Failed to read bytecode content: {}", bytecodePath);
        return;
    }

    // Register this bytecode as a module named "bytecode_main"
    engine.register_module_bytecode("bytecode_main", buffer.data(), buffer.size());

    // Execute a small script to import and trigger the module
    auto result = engine.eval("import 'bytecode_main';", "loader", qjs::EvalMode::Module);

    if (!result.has_value()) {
        const std::string errStr = result.error().to_string();
        VectorJS::show_bsod(errStr);
    }
}

void Application::compile_file_to_file(const std::string &compileInput, const std::string &output_file) const {
    Log::Info("Compiling {}", compileInput);

    // Invoke the engine's compilation logic
    auto result = engine.compile_file_to_bytecode(compileInput, true, true, qjs::EvalMode::Module);

    // Check for compilation errors
    if (!result.has_value()) {
        Log::Error("Compilation failed: {}", result.error().to_string());
        // Since this is a fatal build-step error, we throw or return early
        throw std::runtime_error("Failed to compile script to bytecode.");
    }

    // Open the output file in binary mode
    std::ofstream out(output_file, std::ios::binary);
    if (!out) {
        Log::Error("Failed to open output file for writing: {}", output_file);
        throw std::runtime_error("Disk I/O error during compilation.");
    }

    // Write the bytecode vector to the file
    const auto& bytecode = result.value();
    out.write(reinterpret_cast<const char*>(bytecode.data()), bytecode.size());

    if (out.good()) {
        Log::Info("Successfully compiled to {} ({} bytes)", output_file, bytecode.size());
    } else {
        Log::Error("Failed to write bytecode to {}", output_file);
    }
}