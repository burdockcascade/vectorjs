#include "qjs.hpp"
#include <quickjs.h>
#include <fstream>
#include <sstream>

namespace qjs {

    JSValueHandle::JSValueHandle(JSContext* ctx, JSValue val) noexcept
        : ctx(ctx), val(val) {}

    JSValueHandle::~JSValueHandle() {
        reset();
    }

    JSValueHandle::JSValueHandle(const JSValueHandle& other) : ctx(other.ctx) {
        if (ctx) {
            val = JS_DupValue(ctx, other.val);
        } else {
            val = JS_UNDEFINED;
        }
    }

    JSValueHandle& JSValueHandle::operator=(const JSValueHandle& other) {
        if (this == &other) {
            return *this;
        }

        reset();
        ctx = other.ctx;
        if (ctx) {
            val = JS_DupValue(ctx, other.val);
        } else {
            val = JS_UNDEFINED;
        }
        return *this;
    }

    JSValueHandle::JSValueHandle(JSValueHandle&& other) noexcept
        : ctx(std::exchange(other.ctx, nullptr)),
          val(std::exchange(other.val, JS_UNDEFINED)) {}

    JSValueHandle& JSValueHandle::operator=(JSValueHandle&& other) noexcept {
        // Guard against self-move assignment (a = std::move(a))
        if (this == &other) {
            return *this;
        }

        reset(); // Free current handle before taking ownership of moved handle
        ctx = std::exchange(other.ctx, nullptr);
        val = std::exchange(other.val, JS_UNDEFINED);
        return *this;
    }

    bool JSValueHandle::is_number() const { return JS_IsNumber(val); }
    bool JSValueHandle::is_string() const { return JS_IsString(val); }
    bool JSValueHandle::is_bool() const { return JS_IsBool(val); }
    bool JSValueHandle::is_object() const { return JS_IsObject(val); }
    bool JSValueHandle::is_exception() const { return JS_IsException(val); }
    bool JSValueHandle::is_null_or_undefined() const {
        return JS_IsNull(val) || JS_IsUndefined(val);
    }

    std::optional<double> JSValueHandle::as_double() const {
        if (!ctx) return std::nullopt;
        double result;
        if (JS_ToFloat64(ctx, &result, val) == 0) return result;
        return std::nullopt;
    }

    std::optional<int32_t> JSValueHandle::as_int32() const {
        if (!ctx) return std::nullopt;
        int32_t result;
        if (JS_ToInt32(ctx, &result, val) == 0) return result;
        return std::nullopt;
    }

    std::optional<bool> JSValueHandle::as_bool() const {
        if (!ctx) return std::nullopt;
        int res = JS_ToBool(ctx, val);
        if (res < 0) return std::nullopt;
        return res != 0;
    }

    std::optional<std::string> JSValueHandle::as_string() const {
        if (!ctx) return std::nullopt;
        const char* str = JS_ToCString(ctx, val);
        if (!str) return std::nullopt;
        std::string cppStr(str);
        JS_FreeCString(ctx, str);
        return cppStr;
    }

    void JSValueHandle::reset() noexcept {
        if (ctx) {
            JS_FreeValue(ctx, val);
            ctx = nullptr;
        }
        val = JS_UNDEFINED;
    }

    JSValue JSValueHandle::release() noexcept {
        JSValue temp = val;
        val = JS_UNDEFINED;
        ctx = nullptr;
        return temp;
    }

    JSValue JSValueHandle::get() const noexcept {
        return val;
    }

    JSValueHandle::operator JSValue() const noexcept {
        return get();
    }

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

    void Context::check_and_throw_exception(JSValueConst value) const {
        // Guard against non-exception values
        if (!JS_IsException(value)) {
            return;
        }

        // Fetch the actual exception object from the context
        const JSValueHandle exception_val(ctx, JS_GetException(ctx));
        std::string error_msg = exception_val.as_string().value_or("JavaScript Exception");

        // Extract stack trace if available
        const JSValueHandle stack_val(ctx, JS_GetPropertyStr(ctx, exception_val.get(), "stack"));
        if (!stack_val.is_null_or_undefined()) {
            if (const auto stack_str = stack_val.as_string(); stack_str && !stack_str->empty()) {
                error_msg += "\nStack trace:\n";
                error_msg += *stack_str;
            }
        }

        throw std::runtime_error(error_msg);
    }

    JSValueHandle Context::eval(std::string_view code, std::string_view filename, int eval_flags) const {
        const std::string filename_str{filename};
        const JSValue res = JS_Eval(ctx, code.data(), code.size(), filename_str.c_str(), eval_flags);
        JSValueHandle handle(ctx, res);
        check_and_throw_exception(res);
        return handle;
    }

    JSValueHandle Context::eval_file(const std::string& filepath, int eval_flags) const {
        const std::ifstream file(filepath, std::ios::in | std::ios::binary);
        if (!file) {
            throw std::runtime_error("Failed to open file: " + filepath);
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return eval(buffer.str(), filepath, eval_flags);
    }

    void Context::set_global_number(const std::string& name, double val) const {
        JSValue global = JS_GetGlobalObject(ctx);
        JS_SetPropertyStr(ctx, global, name.c_str(), JS_NewFloat64(ctx, val));
        JS_FreeValue(ctx, global);
    }

    void Context::set_global_string(const std::string& name, const std::string& val) const {
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

        const JSClassDef class_def{
            .class_name = config.name.data(),
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

        const JSValue ctor = JS_NewCFunction2(ctx, config.constructor, config.name.data(), 0, JS_CFUNC_constructor, 0);

        JS_SetConstructor(ctx, ctor, proto);

        if (m) {
            JS_SetModuleExport(ctx, m, config.name.data(), ctor);
        }
    }

    std::string js_to_std_string(JSContext* ctx, JSValueConst val, std::string_view fallback) {
        const char* str = JS_ToCString(ctx, val);
        if (!str) return std::string(fallback);
        std::string result(str);
        JS_FreeCString(ctx, str);
        return result;
    }

    std::optional<float> try_get_float_property(JSContext* ctx, JSValueConst obj, const char* prop_name) {
        if (!JS_IsObject(obj)) return std::nullopt;

        const JSValueHandle prop(ctx, JS_GetPropertyStr(ctx, obj, prop_name));
        if (!prop.is_null_or_undefined()) {
            double temp = 0;
            if (JS_ToFloat64(ctx, &temp, prop.get()) == 0) {
                return static_cast<float>(temp);
            }
        }
        return std::nullopt;
    }

    std::optional<bool> try_get_bool_property(JSContext* ctx, JSValueConst obj, const char* prop_name) {
        if (!JS_IsObject(obj)) return std::nullopt;

        const JSValueHandle prop(ctx, JS_GetPropertyStr(ctx, obj, prop_name));
        if (!prop.is_null_or_undefined()) {
            return static_cast<bool>(JS_ToBool(ctx, prop.get()));
        }
        return std::nullopt;
    }

} // namespace qjs