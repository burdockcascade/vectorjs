#pragma once
#include <type_traits>
#include <tuple>
#include <utility>
#include <string>

#include "js_utils.hpp"


namespace HostApi {

    template <typename T>
    JSValue to_js_value(JSContext* ctx, T&& val) {
        using Decayed = std::decay_t<T>;

        if constexpr (std::is_same_v<Decayed, bool>) {
            return JS_NewBool(ctx, val);
        } else if constexpr (std::is_integral_v<Decayed> || std::is_enum_v<Decayed>) {
            return JS_NewInt64(ctx, static_cast<int64_t>(val));
        } else if constexpr (std::is_floating_point_v<Decayed>) {
            return JS_NewFloat64(ctx, static_cast<double>(val));
        } else if constexpr (requires { val.x; val.y; }) {
            return Utils::create_class_instance<JSVector2>(ctx, js_vector2_class_id, val.x, val.y);
        } else {
            return JS_UNDEFINED;
        }
    }

    template <typename T>
    bool from_js_value(JSContext* ctx, JSValueConst js_val, T& out_val) {
        using Decayed = std::decay_t<T>;

        if constexpr (std::is_same_v<Decayed, bool>) {
            out_val = JS_ToBool(ctx, js_val);
            return true;
        } else if constexpr (std::is_integral_v<Decayed> || std::is_enum_v<Decayed>) {
            int64_t val;
            if (JS_ToInt64(ctx, &val, js_val) < 0) return false;
            out_val = static_cast<Decayed>(val);
            return true;
        } else if constexpr (std::is_floating_point_v<Decayed>) {
            double val;
            if (JS_ToFloat64(ctx, &val, js_val) < 0) return false;
            out_val = static_cast<Decayed>(val);
            return true;
        } else if constexpr (std::is_same_v<Decayed, const char*> || std::is_same_v<Decayed, std::string>) {
            const char* str = JS_ToCString(ctx, js_val);
            if (!str) return false;
            out_val = str;
            JS_FreeCString(ctx, str);
            return true;
        } else {
            return false;
        }
    }

    template <typename T>
    struct FunctionTraits;

    // Regular function pointers
    template <typename R, typename... Args>
    struct FunctionTraits<R(*)(Args...)> {
        using ReturnType = R;
        using ArgTuple = std::tuple<std::decay_t<Args>...>;
        static constexpr size_t ArgCount = sizeof...(Args);
    };

    // noexcept function pointers (C++17+)
    template <typename R, typename... Args>
    struct FunctionTraits<R(*)(Args...) noexcept> {
        using ReturnType = R;
        using ArgTuple = std::tuple<std::decay_t<Args>...>;
        static constexpr size_t ArgCount = sizeof...(Args);
    };

    // 4. Fully generic binding function
    template <auto Func>
    void bind_fn(JSContext* ctx, JSValueConst obj, const char* js_name) {
        using Traits = FunctionTraits<decltype(Func)>;
        using ReturnType = Traits::ReturnType;
        using ArgTuple = Traits::ArgTuple;
        constexpr size_t ArgCount = Traits::ArgCount;

        JS_SetPropertyStr(ctx, obj, js_name, JS_NewCFunction(
            ctx,
            [](JSContext* c, JSValueConst, int argc, JSValueConst* argv) -> JSValue {
                if (argc < static_cast<int>(ArgCount)) {
                    return JS_ThrowTypeError(c, "Invalid argument count");
                }

                ArgTuple args_tuple;
                bool success = [&]<size_t... Is>(std::index_sequence<Is...>) {
                    return (from_js_value(c, argv[Is], std::get<Is>(args_tuple)) && ...);
                }(std::make_index_sequence<ArgCount>{});

                if (!success) {
                    return JS_ThrowTypeError(c, "Invalid argument type");
                }

                if constexpr (std::is_void_v<ReturnType>) {
                    std::apply(Func, args_tuple);
                    return JS_UNDEFINED;
                } else {
                    return to_js_value(c, std::apply(Func, args_tuple));
                }
            },
            js_name,
            static_cast<int>(ArgCount)
        ));
    }
}
