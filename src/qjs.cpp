#include "qjs.hpp"
#include <quickjs.h>
#include <fstream>
#include <sstream>

namespace qjs {

    Value::Value(JSContext* ctx, JSValue val) : ctx(ctx), val(val) {}

    Value::~Value() {
        if (ctx) {
            JS_FreeValue(ctx, val);
        }
    }

    Value::Value(const Value& other) : ctx(other.ctx) {
        if (ctx) {
            val = JS_DupValue(ctx, other.val);
        }
    }

    Value::operator JSValue(this const Value& self) noexcept {
        return self.get();
    }

    Value& Value::operator=(const Value& other) {
        if (this != &other) {
            if (ctx) {
                JS_FreeValue(ctx, val);
            }
            ctx = other.ctx;
            val = ctx ? JS_DupValue(ctx, other.val) : JS_UNDEFINED;
        }
        return *this;
    }

    Value::Value(Value&& other) noexcept : ctx(other.ctx), val(other.val) {
        other.ctx = nullptr;
        other.val = JS_UNDEFINED;
    }

    Value& Value::operator=(Value&& other) noexcept {
        if (this != &other) {
            reset();
            ctx = std::exchange(other.ctx, nullptr);
            val = std::exchange(other.val, JS_UNDEFINED);
        }
        return *this;
    }

    bool Value::isNumber() const {
        return JS_IsNumber(val);
    }

    bool Value::isString() const {
        return JS_IsString(val);
    }

    bool Value::isBool() const {
        return JS_IsBool(val);
    }

    bool Value::isObject() const {
        return JS_IsObject(val);
    }

    bool Value::isException() const {
        return JS_IsException(val);
    }

    bool Value::isNullOrUndefined() const {
        return JS_IsNull(val) || JS_IsUndefined(val);
    }

    std::optional<double> Value::toDouble() const {
        double result;
        if (JS_ToFloat64(ctx, &result, val) == 0) return result;
        return std::nullopt;
    }

    std::optional<int32_t> Value::toInt32() const {
        int32_t result;
        if (JS_ToInt32(ctx, &result, val) == 0) return result;
        return std::nullopt;
    }

    std::optional<bool> Value::toBool() const {
        int res = JS_ToBool(ctx, val);
        if (res < 0) return std::nullopt;
        return res != 0;
    }

    std::optional<std::string> Value::toString() const {
        const char* str = JS_ToCString(ctx, val);
        if (!str) return std::nullopt;
        std::string cppStr(str);
        JS_FreeCString(ctx, str);
        return cppStr;
    }

    void Value::reset() noexcept {
        if (ctx && !JS_IsUndefined(val)) {
            JS_FreeValue(ctx, val);
            val = JS_UNDEFINED;
        }
    }

    JSValue Value::release() noexcept {
        JSValue temp = val;
        val = JS_UNDEFINED;
        ctx = nullptr;
        return temp;
    }

    JSValue Value::get() const { return val; }

    Context::Context() {
        rt = JS_NewRuntime();
        if (!rt) throw std::runtime_error("Failed to initialize QuickJS Runtime");
        ctx = JS_NewContext(rt);
        if (!ctx) {
            JS_FreeRuntime(rt);
            throw std::runtime_error("Failed to initialize QuickJS Context");
        }
    }

    Context::~Context() {
        if (ctx) JS_FreeContext(ctx);
        if (rt) JS_FreeRuntime(rt);
    }

    void Context::check_and_throw_exception(JSValue value) {
        if (!JS_IsException(value)) {
            return;
        }

        const JSValue exception_val = JS_GetException(ctx);
        std::string error_msg = "JavaScript Exception";

        const char* msg = JS_ToCString(ctx, exception_val);
        if (msg) {
            error_msg = msg;
            JS_FreeCString(ctx, msg);
        }

        // Attempt to extract stack trace
        JSValue stack_val = JS_GetPropertyStr(ctx, exception_val, "stack");
        if (!JS_IsUndefined(stack_val)) {
            const char* stack_str = JS_ToCString(ctx, stack_val);
            if (stack_str) {
                error_msg += "\nStack trace:\n";
                error_msg += stack_str;
                JS_FreeCString(ctx, stack_str);
            }
        }

        JS_FreeValue(ctx, stack_val);
        JS_FreeValue(ctx, exception_val);
        JS_FreeValue(ctx, value);

        throw std::runtime_error(error_msg);
    }

    std::string Context::read_file_contents(const std::string& filepath) {
        std::ifstream file(filepath, std::ios::in | std::ios::binary);
        if (!file) {
            throw std::runtime_error("Failed to open file: " + filepath);
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }


    void Context::eval_file(const std::string& filepath, int eval_flags) {
        std::string code = read_file_contents(filepath);
        JSValue result = JS_Eval(ctx, code.c_str(), code.length(), filepath.c_str(), eval_flags);

        // Unified exception handling with stack trace support
        check_and_throw_exception(result);
        JS_FreeValue(ctx, result);
    }

    Value Context::eval(std::string_view code, std::string_view filename) {
        JSValue res = JS_Eval(ctx, code.data(), code.size(), filename.data(), JS_EVAL_TYPE_GLOBAL);

        if (JS_IsException(res)) {
            JSValue exception = JS_GetException(ctx);
            const char* str = JS_ToCString(ctx, exception);
            std::string errMsg = str ? str : "Unknown QuickJS Exception";
            if (str) JS_FreeCString(ctx, str);
            JS_FreeValue(ctx, exception);
            JS_FreeValue(ctx, res);
            throw std::runtime_error("JS Execution Error: " + errMsg);
        }

        return Value(ctx, res);
    }

    void Context::set_global_number(const std::string& name, double val) {
        JSValue global = JS_GetGlobalObject(ctx);
        JS_SetPropertyStr(ctx, global, name.c_str(), JS_NewFloat64(ctx, val));
        JS_FreeValue(ctx, global);
    }

    void Context::set_global_string(const std::string& name, const std::string& val) {
        JSValue global = JS_GetGlobalObject(ctx);
        JS_SetPropertyStr(ctx, global, name.c_str(), JS_NewString(ctx, val.c_str()));
        JS_FreeValue(ctx, global);
    }

    JSContext* Context::raw_context() const { return ctx; }
    JSRuntime* Context::raw_runtime() const { return rt; }

    void register_js_class(JSContext* ctx, JSModuleDef* m, const ClassDefConfig& config) {
        JSRuntime* const rt = JS_GetRuntime(ctx);

        if (config.class_id == 0) {
            JS_NewClassID(rt, &config.class_id);
        }

        const std::string name_str{config.name};

        const JSClassDef class_def{
            .class_name = name_str.c_str(),
            .finalizer = config.finalizer
        };
        JS_NewClass(rt, config.class_id, &class_def);

        JSValue proto = JS_NewObject(ctx);

        if (!config.proto_funcs.empty()) {
            JS_SetPropertyFunctionList(
                ctx,
                proto,
                config.proto_funcs.data(),
                static_cast<int>(config.proto_funcs.size())
            );
        }

        JS_SetClassProto(ctx, config.class_id, proto);

        const JSValue ctor = JS_NewCFunction2(ctx, config.constructor, name_str.c_str(), 0, JS_CFUNC_constructor, 0);
        JS_SetConstructor(ctx, ctor, proto);
        JS_SetModuleExport(ctx, m, name_str.c_str(), ctor);
    }


} // namespace qjs