#pragma once
#include "qjs_wrapper.hpp"
#include "logger.hpp"

namespace HostApi {

    static void register_console_object(qjs::Engine& engine) {
        auto global = engine.global();

        auto console = engine.make_object()
            .set_function("log", [](std::string msg) {
                Log::Info("{}", msg);
            })
            .set_function("error", [](std::string msg) {
                Log::Error("{}", msg);
            })
            .set_function("warn", [](std::string msg) {
                Log::Warn("{}", msg);
            })
            .set_function("debug", [](std::string msg) {
                Log::Debug("{}", msg);
            })
            .set_function("critical", [](std::string msg) {
                Log::Critical("{}", msg);
            })
            .set_function("trace", [](std::string msg) {
                Log::Trace("{}", msg);
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
