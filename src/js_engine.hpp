#pragma once

extern "C" {
#include <quickjs.h>
}
#include <string>
#include <string_view>

namespace VectorJS {
    class JSEngine {
        JSRuntime* rt{nullptr};
        JSContext* ctx{nullptr};

        void check_and_throw_exception(JSValue value) const;
        static std::string read_file_contents(const std::string& filepath);

    public:
        JSEngine();
        ~JSEngine();

        // Disable copies
        JSEngine(const JSEngine&) = delete;
        JSEngine& operator=(const JSEngine&) = delete;

        // Enable moves
        JSEngine(JSEngine&& other) noexcept;
        JSEngine& operator=(JSEngine&& other) noexcept;

        [[nodiscard]] JSContext* get_context() const { return ctx; }
        [[nodiscard]] JSRuntime* get_runtime() const { return rt; }

        void eval(std::string_view code, std::string_view filename = "input.js") const;

        [[nodiscard]] JSValue eval_value(std::string_view code, std::string_view filename = "input.js") const;

        void eval_file(const std::string& filepath, int eval_flags = JS_EVAL_TYPE_MODULE) const;
    };
}