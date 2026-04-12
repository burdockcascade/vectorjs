#pragma once
#include "qjs_wrapper.hpp"
#include "logger.hpp"

namespace HostApi {

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

    static void register_system_object(qjs::Engine& engine) {
        auto global = engine.global();

        auto system_obj = engine.make_object();
        system_obj.set_function("gc", [&engine] {
            engine.run_gc();
        });
        global.set_constant("system", std::move(system_obj));

    }

    inline void register_host_api(qjs::Engine& engine) {
        register_console_object(engine);
        register_system_object(engine);
    }

} // namespace HostApi

class Runner {
public:
    // Lifecycle
    explicit Runner(std::string path);

    // Deleted Copy/Assignment
    Runner(const Runner&) = delete;
    Runner& operator=(const Runner&) = delete;

    // Execution
    void Run() const;

private:
    std::string scriptPath;
    qjs::Engine engine;

};
