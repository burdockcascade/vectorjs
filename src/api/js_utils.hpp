#pragma once
#include <quickjs.h>
#include <utility>
#include <span>
#include <string_view>
#include <memory>
#include <type_traits>
#include <optional>

#include "qjs.hpp"

namespace HostApi::Utils {
    
    inline std::string js_to_std_string(JSContext* ctx, JSValueConst val, std::string_view fallback = "") {
        const char* str = JS_ToCString(ctx, val);
        if (!str) return std::string(fallback);
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
    std::optional<T> try_get_opaque(JSContext* ctx, JSValueConst val, JSClassID class_id) {
        if (JS_IsUndefined(val) || JS_IsNull(val) || !JS_IsObject(val)) return std::nullopt;
        if (auto* ptr = get_opaque<T>(ctx, val, class_id)) {
            return *ptr;
        }
        return std::nullopt;
    }

    template <typename T>
    std::optional<T> try_get_opaque_property(JSContext* ctx, JSValueConst obj, const char* prop_name, JSClassID class_id) {
        if (!JS_IsObject(obj)) return std::nullopt;

        qjs::Value prop(ctx, JS_GetPropertyStr(ctx, obj, prop_name));
        if (!JS_IsUndefined(prop.get()) && !JS_IsNull(prop.get())) {
            if (auto* ptr = get_opaque<T>(ctx, prop.get(), class_id)) {
                return *ptr;
            }
        }
        return std::nullopt;
    }

    inline std::optional<float> try_get_float_property(JSContext* ctx, JSValueConst obj, const char* prop_name) {
        if (!JS_IsObject(obj)) return std::nullopt;

        qjs::Value prop(ctx, JS_GetPropertyStr(ctx, obj, prop_name));
        if (!JS_IsUndefined(prop.get()) && JS_IsNumber(prop.get())) {
            double temp = 0;
            if (JS_ToFloat64(ctx, &temp, prop.get()) == 0) {
                return static_cast<float>(temp);
            }
        }
        return std::nullopt;
    }

    inline std::optional<bool> try_get_bool_property(JSContext* ctx, JSValueConst obj, const char* prop_name) {
        if (!JS_IsObject(obj)) return std::nullopt;

        qjs::Value prop(ctx, JS_GetPropertyStr(ctx, obj, prop_name));
        if (!JS_IsUndefined(prop.get()) && JS_IsBool(prop.get())) {
            return static_cast<bool>(JS_ToBool(ctx, prop.get()));
        }
        return std::nullopt;
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
        qjs::Value proto(ctx, JS_GetPropertyStr(ctx, new_target, "prototype"));
        if (JS_IsException(proto.get())) return JS_EXCEPTION;

        JSValue obj = JS_NewObjectProtoClass(ctx, proto.get(), class_id);
        if (JS_IsException(obj)) return JS_EXCEPTION;

        auto instance = std::make_unique<T>(std::forward<Args>(args)...);
        JS_SetOpaque(obj, instance.release());
        return obj;
    }

    template <typename T, typename... Args>
    JSValue create_class_instance(JSContext* ctx, JSClassID class_id, Args&&... args) {
        qjs::Value proto(ctx, JS_GetClassProto(ctx, class_id));
        if (JS_IsException(proto.get())) return JS_EXCEPTION;

        JSValue obj = JS_NewObjectProtoClass(ctx, proto.get(), class_id);
        if (JS_IsException(obj)) return JS_EXCEPTION;

        auto instance = std::make_unique<T>(std::forward<Args>(args)...);
        JS_SetOpaque(obj, instance.release());
        return obj;
    }

} // namespace HostApi::Utils