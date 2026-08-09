#include "hostapi.hpp"
#include "js_types.hpp"
#include "../qjs.hpp"
#include <span>
#include <string_view>

#define JS_BIND_PROP(ClassType, ClassIDPtr, name, Member) \
    JS_CGETSET_DEF( \
        name, \
        (&qjs::js_generic_getter<ClassType, decltype(ClassType::Member), &ClassType::Member, ClassIDPtr>), \
        (&qjs::js_generic_setter<ClassType, decltype(ClassType::Member), &ClassType::Member, ClassIDPtr>) \
    )

#define JS_ARG_TO_INT32(ctx, var, arg, default_val) \
    int32_t var = (default_val);                   \
    if (JS_ToInt32((ctx), &(var), (arg)) < 0)      \
        return JS_EXCEPTION;

#define JS_ARG_TO_FLOAT64(ctx, var, arg, default_val) \
    double var = (default_val);                       \
    if (JS_ToFloat64((ctx), &(var), (arg)) < 0)       \
        return JS_EXCEPTION;

#define JS_ARG_TO_STRING(ctx, var, arg, default_val) \
    std::string var = qjs::js_to_std_string(ctx, arg, default_val);

namespace VectorJS {

    static void register_application_class(JSContext* ctx, JSModuleDef* m) {
        static constexpr JSCFunctionListEntry proto_funcs[] = {
            JS_CFUNC_DEF("run", 1, [](JSContext* c, JSValueConst this_val, int argc, JSValueConst* argv) -> JSValue {
                const auto* app = qjs::get_opaque<JSApplication>(c, this_val, js_application_class_id);
                return app ? app->Run(c, this_val, argc, argv)
                           : JS_ThrowTypeError(c, "Invalid JSApplication instance");
            })
        };

        qjs::register_js_class(ctx, m, {
            .name = "Application",
            .class_id = js_application_class_id,
            .finalizer = [](JSRuntime*, JSValue val) {
                delete qjs::get_opaque<JSApplication>(val, js_application_class_id);
            },
            .constructor = [](JSContext* c, JSValueConst new_target, int argc, JSValueConst* argv) -> JSValue {
                if (const std::span args{argv, static_cast<size_t>(argc)}; args.size() == 3) {
                    JS_ARG_TO_INT32(c, w, args[0], 0);
                    JS_ARG_TO_INT32(c, h, args[1], 0);
                    JS_ARG_TO_STRING(c, title, args[2], "VectorJS Application")
                    return qjs::create_js_instance<JSApplication>(c, new_target, js_application_class_id, w, h, std::move(title));
                }
                return JS_UNDEFINED;
            },
            .proto_funcs = proto_funcs
        });
    }

    static void register_color_class(JSContext* ctx, JSModuleDef* m) {
        static constexpr JSCFunctionListEntry proto_funcs[] = {
            JS_BIND_PROP(JSColor, &js_color_class_id, "r", r),
            JS_BIND_PROP(JSColor, &js_color_class_id, "g", g),
            JS_BIND_PROP(JSColor, &js_color_class_id, "b", b),
            JS_BIND_PROP(JSColor, &js_color_class_id, "a", a),
        };

        qjs::register_js_class(ctx, m, {
            .name = "Color",
            .class_id = js_color_class_id,
            .finalizer = [](JSRuntime*, JSValue val) {
                delete qjs::get_opaque<JSColor>(val, js_color_class_id);
            },
            .constructor = [](JSContext* c, JSValueConst new_target, int argc, JSValueConst* argv) -> JSValue {
                if (const std::span args{argv, static_cast<size_t>(argc)}; args.size() == 4) {
                    JS_ARG_TO_INT32(c, r, args[0], 0);
                    JS_ARG_TO_INT32(c, g, args[1], 0);
                    JS_ARG_TO_INT32(c, b, args[2], 0);
                    JS_ARG_TO_INT32(c, a, args[3], 255);
                    return qjs::create_js_instance<JSColor>(c, new_target, js_color_class_id, static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b), static_cast<uint8_t>(a));
                }
                return JS_UNDEFINED;
            },
            .proto_funcs = proto_funcs
        });
    }

    static void register_vector2_class(JSContext* ctx, JSModuleDef* m) {
        static constexpr JSCFunctionListEntry proto_funcs[] = {
            JS_BIND_PROP(JSVector2, &js_vector2_class_id, "x", x),
            JS_BIND_PROP(JSVector2, &js_vector2_class_id, "y", y),
        };

        qjs::register_js_class(ctx, m, {
            .name = "Vector2",
            .class_id = js_vector2_class_id,
            .finalizer = [](JSRuntime*, JSValue val) {
                delete qjs::get_opaque<JSVector2>(val, js_vector2_class_id);
            },
            .constructor = [](JSContext* c, JSValueConst new_target, int argc, JSValueConst* argv) -> JSValue {
                if (const std::span args{argv, static_cast<size_t>(argc)}; args.size() == 2) {
                    JS_ARG_TO_FLOAT64(c, x, args[0], 0);
                    JS_ARG_TO_FLOAT64(c, y, args[1], 0);
                    return qjs::create_js_instance<JSVector2>(c, new_target, js_vector2_class_id, static_cast<float>(x), static_cast<float>(y));
                }
                return JS_UNDEFINED;
            },
            .proto_funcs = proto_funcs
        });
    }

    static void register_rectangle_class(JSContext* ctx, JSModuleDef* m) {
        static constexpr JSCFunctionListEntry proto_funcs[] = {
            JS_BIND_PROP(JSRectangle, &js_rectangle_class_id, "x", x),
            JS_BIND_PROP(JSRectangle, &js_rectangle_class_id, "y", y),
            JS_BIND_PROP(JSRectangle, &js_rectangle_class_id, "width", width),
            JS_BIND_PROP(JSRectangle, &js_rectangle_class_id, "height", height),
        };

        qjs::register_js_class(ctx, m, {
            .name = "Rectangle",
            .class_id = js_rectangle_class_id,
            .finalizer = [](JSRuntime*, JSValue val) {
                delete qjs::get_opaque<JSRectangle>(val, js_rectangle_class_id);
            },
            .constructor = [](JSContext* c, JSValueConst new_target, int argc, JSValueConst* argv) -> JSValue {
                if (const std::span args{argv, static_cast<size_t>(argc)}; args.size() == 4) {
                    JS_ARG_TO_FLOAT64(c, x, args[0], 0);
                    JS_ARG_TO_FLOAT64(c, y, args[1], 0);
                    JS_ARG_TO_FLOAT64(c, w, args[2], 0);
                    JS_ARG_TO_FLOAT64(c, h, args[3], 0);
                    return qjs::create_js_instance<JSRectangle>(c, new_target, js_rectangle_class_id,static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h));
                }
                return JS_UNDEFINED;
            },
            .proto_funcs = proto_funcs
        });
    }

    static void register_font_class(JSContext* ctx, JSModuleDef* m) {
        qjs::register_js_class(ctx, m, {
            .name = "Font",
            .class_id = js_font_class_id,
            .finalizer = [](JSRuntime*, JSValue val) {
                delete qjs::get_opaque<JSFont>(val, js_font_class_id);
            },
            .constructor = [](JSContext* c, JSValueConst new_target, int argc, JSValueConst* argv) -> JSValue {
                const std::span args{argv, static_cast<size_t>(argc)};

                if (args.empty()) return JS_ThrowTypeError(c, "Font requires at least a path argument");

                JS_ARG_TO_STRING(c, font_path, args[2], "")
                if (font_path.empty()) return JS_ThrowTypeError(c, "Font path must be a non-empty string");

                if (args.size() >= 2) {
                    int32_t size = 0;
                    if (JS_ToInt32(c, &size, args[1]) < 0) return JS_EXCEPTION;
                    if (size <= 0) return JS_ThrowRangeError(c, "Font size must be a positive integer");
                    return qjs::create_js_instance<JSFont>(c, new_target, js_font_class_id, font_path, size);
                }

                return qjs::create_js_instance<JSFont>(c, new_target, js_font_class_id, font_path);
            }
        });
    }

    void register_hapi_classes(JSContext* ctx, JSModuleDef* m) {
        register_application_class(ctx, m);
        register_color_class(ctx, m);
        register_vector2_class(ctx, m);
        register_rectangle_class(ctx, m);
        register_font_class(ctx, m);
    }

} // namespace HostApi

#undef JS_BIND_PROP