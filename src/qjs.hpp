#pragma once

#include <quickjs.h>
#include <string>
#include <stdexcept>
#include <string_view>
#include <optional>
#include <cstdint>
#include <span>
#include <memory>
#include <utility>
#include <type_traits>

namespace qjs {

    class JSValueHandle {
    private:
        JSContext* ctx = nullptr;
        JSValue val = JS_UNDEFINED;

    public:
        JSValueHandle() noexcept = default;
        JSValueHandle(JSContext* ctx, JSValue val) noexcept;

        // RAII management
        ~JSValueHandle();
        JSValueHandle(const JSValueHandle& other);
        JSValueHandle& operator=(const JSValueHandle& other);
        JSValueHandle(JSValueHandle&& other) noexcept;
        JSValueHandle& operator=(JSValueHandle&& other) noexcept;

        // Type checks
        [[nodiscard]] bool is_number() const;
        [[nodiscard]] bool is_string() const;
        [[nodiscard]] bool is_bool() const;
        [[nodiscard]] bool is_object() const;
        [[nodiscard]] bool is_exception() const;
        [[nodiscard]] bool is_null_or_undefined() const;

        // Conversions
        [[nodiscard]] std::optional<double> as_double() const;
        [[nodiscard]] std::optional<int32_t> as_int32() const;
        [[nodiscard]] std::optional<bool> as_bool() const;
        [[nodiscard]] std::optional<std::string> as_string() const;

        // Explicit conversion / getter
        [[nodiscard]] JSValue get() const noexcept;
        explicit operator JSValue() const noexcept;

        void reset() noexcept;
        JSValue release() noexcept;
    };


    // RAII Wrapper for JSContext and JSRuntime
    class Context {
    private:
        JSRuntime* rt = nullptr;
        JSContext* ctx = nullptr;
        void check_and_throw_exception(JSValue value) const;

    public:
        Context();
        ~Context();

        [[nodiscard]] JSValueHandle eval_file(const std::string &filepath, int eval_flags = JS_EVAL_TYPE_MODULE) const;
        [[nodiscard]] JSValueHandle eval(std::string_view code, std::string_view filename = "<input>", int eval_flags = JS_EVAL_TYPE_MODULE) const;

        Context(const Context&) = delete;

        Context& operator=(const Context&) = delete;

        Context(Context&&) noexcept = default;

        Context& operator=(Context&&) noexcept = default;

        // Export primitives back to JavaScript globals
        void set_global_number(const std::string& name, double val) const;
        void set_global_string(const std::string& name, const std::string& val) const;

        [[nodiscard]] JSContext* raw_context() const;
        [[nodiscard]] JSRuntime* raw_runtime() const;
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
    std::string js_to_std_string(JSContext* ctx, JSValueConst val, std::string_view fallback = "");

    std::optional<float> try_get_float_property(JSContext* ctx, JSValueConst obj, const char* prop_name);
    std::optional<bool> try_get_bool_property(JSContext* ctx, JSValueConst obj, const char* prop_name);

    template <typename T>
    T* get_opaque(JSContext* ctx, JSValueConst val, JSClassID class_id) {
        if (JS_IsUndefined(val) || JS_IsNull(val)) return nullptr;
        return static_cast<T*>(JS_GetOpaque2(ctx, val, class_id));
    }

    template <typename T>
    T* get_opaque(JSValueConst val, JSClassID class_id) {
        return static_cast<T*>(JS_GetOpaque(val, class_id));
    }

    template <typename T>
    std::optional<T> try_get_opaque(JSContext* ctx, JSValueConst val, JSClassID class_id) {
        if (JS_IsUndefined(val) || JS_IsNull(val) || !JS_IsObject(val)) return std::nullopt;
        if (auto* ptr = get_opaque<T>(ctx, val, class_id)) {
            return *ptr;
        }
        return std::nullopt;
    }

    template <typename ClassType, typename FieldType, FieldType ClassType::*Member, JSClassID* ClassID>
    JSValue js_generic_getter(JSContext* ctx, JSValueConst this_val) {
        auto* instance = get_opaque<ClassType>(ctx, this_val, *ClassID);
        if (!instance) return JS_EXCEPTION;

        if constexpr (std::is_integral_v<FieldType>) {
            return JS_NewInt32(ctx, static_cast<int32_t>(instance->*Member));
        } else if constexpr (std::is_floating_point_v<FieldType>) {
            return JS_NewFloat64(ctx, static_cast<double>(instance->*Member));
        }

        return JS_UNDEFINED;
    }

    template <typename ClassType, typename FieldType, FieldType ClassType::*Member, JSClassID* ClassID>
    JSValue js_generic_setter(JSContext* ctx, JSValueConst this_val, JSValueConst val) {
        auto* instance = get_opaque<ClassType>(ctx, this_val, *ClassID);
        if (!instance) return JS_EXCEPTION;

        if constexpr (std::is_integral_v<FieldType>) {
            int32_t v = 0;
            if (JS_ToInt32(ctx, &v, val) != 0) return JS_EXCEPTION;
            instance->*Member = static_cast<FieldType>(v);
        } else if constexpr (std::is_floating_point_v<FieldType>) {
            double v = 0;
            if (JS_ToFloat64(ctx, &v, val) != 0) return JS_EXCEPTION;
            instance->*Member = static_cast<FieldType>(v);
        }
        return JS_UNDEFINED;
    }

    template <typename T, typename... Args>
    JSValue create_js_instance(JSContext* ctx, JSValueConst new_target, JSClassID class_id, Args&&... args) {
        const JSValueHandle proto(ctx, JS_GetPropertyStr(ctx, new_target, "prototype"));
        if (JS_IsException(proto.get())) return JS_EXCEPTION;

        JSValueConst proto_val = proto.is_null_or_undefined() ? JS_NULL : proto.get();

        auto instance = std::make_unique<T>(std::forward<Args>(args)...);

        JSValue obj = JS_NewObjectProtoClass(ctx, proto_val, class_id);
        if (JS_IsException(obj)) {
            return JS_EXCEPTION;
        }

        JS_SetOpaque(obj, instance.release());
        return obj;
    }

    template <typename T, typename... Args>
    JSValue create_class_instance(JSContext* ctx, JSClassID class_id, Args&&... args) {
        const JSValueHandle proto(ctx, JS_GetClassProto(ctx, class_id));
        if (JS_IsException(proto.get())) return JS_EXCEPTION;

        auto instance = std::make_unique<T>(std::forward<Args>(args)...);

        JSValue obj = JS_NewObjectProtoClass(ctx, proto.get(), class_id);
        if (JS_IsException(obj)) {
            return JS_EXCEPTION;
        }

        JS_SetOpaque(obj, instance.release());
        return obj;
    }

    template <typename T>
    std::optional<T> try_get_opaque_property(JSContext* ctx, JSValueConst obj, const char* prop_name, JSClassID class_id) {
        if (!JS_IsObject(obj)) return std::nullopt;

        const JSValueHandle prop(ctx, JS_GetPropertyStr(ctx, obj, prop_name));
        if (!prop.is_null_or_undefined()) {
            if (auto* ptr = get_opaque<T>(ctx, prop.get(), class_id)) {
                return *ptr;
            }
        }
        return std::nullopt;
    }

} // namespace qjs