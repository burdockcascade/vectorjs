#pragma once
#include <quickjs.h>
#include <utility>
#include <span>
#include <string_view>
#include <memory>
#include <type_traits>

namespace HostApi::Utils {

    class ScopedJSValue {
    public:
        ScopedJSValue(JSContext* ctx, JSValue val) : ctx_(ctx), val_(val) {}
        ~ScopedJSValue() {
            if (ctx_ && !JS_IsUndefined(val_)) {
                JS_FreeValue(ctx_, val_);
            }
        }

        ScopedJSValue(const ScopedJSValue&) = delete;
        ScopedJSValue& operator=(const ScopedJSValue&) = delete;

        ScopedJSValue(ScopedJSValue&& other) noexcept
            : ctx_(std::exchange(other.ctx_, nullptr)), val_(std::exchange(other.val_, JS_UNDEFINED)) {}

        ScopedJSValue& operator=(ScopedJSValue&& other) noexcept {
            if (this != &other) {
                if (ctx_ && !JS_IsUndefined(val_)) JS_FreeValue(ctx_, val_);
                ctx_ = std::exchange(other.ctx_, nullptr);
                val_ = std::exchange(other.val_, JS_UNDEFINED);
            }
            return *this;
        }

        [[nodiscard]] JSValue get() const { return val_; }
        [[nodiscard]] JSValue release() {
            JSValue temp = val_;
            val_ = JS_UNDEFINED;
            return temp;
        }
        operator JSValue() const { return val_; }

    private:
        JSContext* ctx_{nullptr};
        JSValue val_{JS_UNDEFINED};
    };

    inline std::string js_to_std_string(JSContext* ctx, JSValueConst val, const std::string& fallback = "") {
        const char* str = JS_ToCString(ctx, val);
        if (!str) return fallback;
        std::string result(str);
        JS_FreeCString(ctx, str);
        return result;
    }

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
    T get_opaque_or(JSContext* ctx, JSValueConst val, JSClassID class_id, const T& default_val) {
        if (auto* ptr = get_opaque<T>(ctx, val, class_id)) {
            return *ptr;
        }
        return default_val;
    }

    template <typename T>
    bool try_get_opaque_property(JSContext* ctx, JSValueConst obj, const char* prop_name, JSClassID class_id, T& out_val) {
        if (!JS_IsObject(obj)) return false;

        ScopedJSValue prop(ctx, JS_GetPropertyStr(ctx, obj, prop_name));
        if (!JS_IsUndefined(prop.get()) && !JS_IsNull(prop.get())) {
            if (auto* ptr = get_opaque<T>(ctx, prop.get(), class_id)) {
                out_val = *ptr;
                return true;
            }
        }
        return false;
    }

    inline bool try_get_float_property(JSContext* ctx, JSValueConst obj, const char* prop_name, float& out_val) {
        if (!JS_IsObject(obj)) return false;

        ScopedJSValue prop(ctx, JS_GetPropertyStr(ctx, obj, prop_name));
        if (!JS_IsUndefined(prop.get()) && JS_IsNumber(prop.get())) {
            double temp = 0;
            if (JS_ToFloat64(ctx, &temp, prop.get()) == 0) {
                out_val = static_cast<float>(temp);
                return true;
            }
        }
        return false;
    }

    inline bool try_get_bool_property(JSContext* ctx, JSValueConst obj, const char* prop_name, bool& out_val) {
        if (!JS_IsObject(obj)) return false;

        ScopedJSValue prop(ctx, JS_GetPropertyStr(ctx, obj, prop_name));
        if (!JS_IsUndefined(prop.get()) && JS_IsBool(prop.get())) {
            out_val = static_cast<bool>(JS_ToBool(ctx, prop.get()));
            return true;
        }
        return false;
    }

    // --- 1. Generic Getters and Setters ---

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

    // --- 2. Exception-Safe Instance Creation ---

    template <typename T, typename... Args>
    JSValue create_js_instance(JSContext* ctx, JSValueConst new_target, JSClassID class_id, Args&&... args) {
        ScopedJSValue proto(ctx, JS_GetPropertyStr(ctx, new_target, "prototype"));
        if (JS_IsException(proto.get())) return JS_EXCEPTION;

        JSValue obj = JS_NewObjectProtoClass(ctx, proto.get(), class_id);
        if (JS_IsException(obj)) return JS_EXCEPTION;

        auto instance = std::make_unique<T>(std::forward<Args>(args)...);
        JS_SetOpaque(obj, instance.release());
        return obj;
    }

    template <typename T, typename... Args>
    JSValue create_class_instance(JSContext* ctx, JSClassID class_id, Args&&... args) {
        ScopedJSValue proto(ctx, JS_GetClassProto(ctx, class_id));
        if (JS_IsException(proto.get())) return JS_EXCEPTION;

        JSValue obj = JS_NewObjectProtoClass(ctx, proto.get(), class_id);
        if (JS_IsException(obj)) return JS_EXCEPTION;

        auto instance = std::make_unique<T>(std::forward<Args>(args)...);
        JS_SetOpaque(obj, instance.release());
        return obj;
    }

    // --- Class Registration Helper ---

    struct ClassDefConfig {
        std::string_view name;
        JSClassID& class_id;
        JSClassFinalizer* finalizer = nullptr;
        JSCFunction* constructor = nullptr;
        std::span<const JSCFunctionListEntry> proto_funcs{};
    };

    inline void register_js_class(JSContext* ctx, JSModuleDef* m, const ClassDefConfig& config) {
        JSRuntime* const rt = JS_GetRuntime(ctx);

        if (config.class_id == 0) {
            JS_NewClassID(rt, &config.class_id);
        }

        const JSClassDef class_def{
            .class_name = config.name.data(),
            .finalizer = config.finalizer
        };
        JS_NewClass(rt, config.class_id, &class_def);

        const ScopedJSValue proto(ctx, JS_NewObject(ctx));

        if (!config.proto_funcs.empty()) {
            JS_SetPropertyFunctionList(
                ctx,
                proto.get(),
                config.proto_funcs.data(),
                static_cast<int>(config.proto_funcs.size())
            );
        }
        JS_SetClassProto(ctx, config.class_id, proto.get());

        const JSValue ctor = JS_NewCFunction2(ctx, config.constructor, config.name.data(), 0, JS_CFUNC_constructor, 0);
        JS_SetConstructor(ctx, ctor, proto.get());

        JS_SetModuleExport(ctx, m, config.name.data(), ctor);
    }

} // namespace HostApi::Utils