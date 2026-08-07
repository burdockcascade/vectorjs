#pragma once

#include <quickjs.h>
#include <string>
#include <stdexcept>
#include <string_view>
#include <optional>
#include <cstdint>
#include <span>

namespace qjs {

    // Wraps dynamic QuickJS JSValue types
    class Value {
    private:
        JSContext* ctx = nullptr;
        JSValue val = JS_UNDEFINED;

    public:
        Value() = default;
        Value(JSContext* ctx, JSValue val);

        // RAII move/copy management
        ~Value();
        Value(const Value& other);
        Value& operator=(const Value& other);
        Value(Value&& other) noexcept;
        Value& operator=(Value&& other) noexcept;

        // Type checks
        bool isNumber() const;
        bool isString() const;
        bool isBool() const;
        bool isObject() const;
        bool isException() const;
        bool isNullOrUndefined() const;

        // Conversions
        std::optional<double> toDouble() const;
        std::optional<int32_t> toInt32() const;
        std::optional<bool> toBool() const;
        std::optional<std::string> toString() const;

        explicit(false) operator JSValue(this const Value& self) noexcept;

        void reset() noexcept;
        JSValue release() noexcept;
        JSValue get() const;
    };

    // RAII Wrapper for JSContext and JSRuntime
    class Context {
    private:
        JSRuntime* rt = nullptr;
        JSContext* ctx = nullptr;

    public:
        Context();
        ~Context();

        void check_and_throw_exception(JSValue value);

        std::string read_file_contents(const std::string &filepath);

        void eval_file(const std::string &filepath, int eval_flags = JS_EVAL_TYPE_MODULE);

        Context(const Context&) = delete;
        Context& operator=(const Context&) = delete;

        Context(Context&&) noexcept = default;
        Context& operator=(Context&&) noexcept = default;

        // Evaluates a script string
        Value eval(std::string_view code, std::string_view filename = "<input>");

        // Export primitives back to JavaScript globals
        void set_global_number(const std::string& name, double val);
        void set_global_string(const std::string& name, const std::string& val);

        JSContext* raw_context() const;
        JSRuntime* raw_runtime() const;
    };

    // --- Class Registration Helper ---

    struct ClassDefConfig {
        std::string_view name;
        JSClassID& class_id;
        JSClassFinalizer* finalizer = nullptr;
        JSCFunction* constructor = nullptr;
        std::span<const JSCFunctionListEntry> proto_funcs{};
    };

    void register_js_class(JSContext* ctx, JSModuleDef* m, const ClassDefConfig& config);

} // namespace qjs