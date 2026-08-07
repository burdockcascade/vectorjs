#include "hostapi.hpp"
#include "js_types.hpp"
#include "js_utils.hpp"
#include "../qjs.hpp"
#include <span>
#include <string_view>

#define JS_BIND_PROP(ClassType, ClassIDPtr, name, Member) \
    JS_CGETSET_DEF( \
        name, \
        (&HostApi::Utils::js_generic_getter<ClassType, decltype(ClassType::Member), &ClassType::Member, ClassIDPtr>), \
        (&HostApi::Utils::js_generic_setter<ClassType, decltype(ClassType::Member), &ClassType::Member, ClassIDPtr>) \
    )

namespace HostApi {

    static void register_application_class(JSContext* ctx, JSModuleDef* m) {
        static constexpr JSCFunctionListEntry proto_funcs[] = {
            JS_CFUNC_DEF("run", 1, [](JSContext* c, JSValueConst this_val, int argc, JSValueConst* argv) -> JSValue {
                const auto* app = Utils::get_opaque<JSApplication>(c, this_val, js_application_class_id);
                return app ? app->Run(c, this_val, argc, argv)
                           : JS_ThrowTypeError(c, "Invalid JSApplication instance");
            })
        };

        qjs::register_js_class(ctx, m, {
            .name = "Application",
            .class_id = js_application_class_id,
            .finalizer = [](JSRuntime*, JSValue val) {
                delete Utils::get_opaque<JSApplication>(val, js_application_class_id);
            },
            .constructor = [](JSContext* c, JSValueConst new_target, int argc, JSValueConst* argv) -> JSValue {
                const std::span args{argv, static_cast<size_t>(argc)};

                int32_t w = 0;
                int32_t h = 0;
                if (args.size() > 0 && JS_ToInt32(c, &w, args[0]) < 0) return JS_EXCEPTION;
                if (args.size() > 1 && JS_ToInt32(c, &h, args[1]) < 0) return JS_EXCEPTION;

                std::string title = args.size() > 2 ? Utils::js_to_std_string(c, args[2]) : "VectorJS Application";
                return Utils::create_js_instance<JSApplication>(c, new_target, js_application_class_id, w, h, std::move(title));
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
                delete Utils::get_opaque<JSColor>(val, js_color_class_id);
            },
            .constructor = [](JSContext* c, JSValueConst new_target, int argc, JSValueConst* argv) -> JSValue {
                const std::span args{argv, static_cast<size_t>(argc)};

                if (args.size() >= 4) {
                    int32_t r = 0;
                    int32_t g = 0;
                    int32_t b = 0;
                    int32_t a = 255;
                    if (args.size() > 0 && JS_ToInt32(c, &r, args[0]) < 0) return JS_EXCEPTION;
                    if (args.size() > 1 && JS_ToInt32(c, &g, args[1]) < 0) return JS_EXCEPTION;
                    if (args.size() > 2 && JS_ToInt32(c, &b, args[2]) < 0) return JS_EXCEPTION;
                    if (args.size() > 3 && JS_ToInt32(c, &a, args[3]) < 0) return JS_EXCEPTION;

                    return Utils::create_js_instance<JSColor>(
                        c, new_target, js_color_class_id,
                        static_cast<uint8_t>(r), static_cast<uint8_t>(g),
                        static_cast<uint8_t>(b), static_cast<uint8_t>(a)
                    );
                }
                return Utils::create_js_instance<JSColor>(c, new_target, js_color_class_id);
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
                delete Utils::get_opaque<JSVector2>(val, js_vector2_class_id);
            },
            .constructor = [](JSContext* c, JSValueConst new_target, int argc, JSValueConst* argv) -> JSValue {
                const std::span args{argv, static_cast<size_t>(argc)};

                double x = 0;
                double y = 0;
                if (args.size() > 0 && JS_ToFloat64(c, &x, args[0]) < 0) return JS_EXCEPTION;
                if (args.size() > 1 && JS_ToFloat64(c, &y, args[1]) < 0) return JS_EXCEPTION;

                return Utils::create_js_instance<JSVector2>(
                    c, new_target, js_vector2_class_id,
                    static_cast<float>(x), static_cast<float>(y)
                );
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
                delete Utils::get_opaque<JSRectangle>(val, js_rectangle_class_id);
            },
            .constructor = [](JSContext* c, JSValueConst new_target, int argc, JSValueConst* argv) -> JSValue {
                const std::span args{argv, static_cast<size_t>(argc)};

                double x = 0;
                double y = 0;
                double w = 0;
                double h = 0;
                if (args.size() > 0 && JS_ToFloat64(c, &x, args[0]) < 0) return JS_EXCEPTION;
                if (args.size() > 1 && JS_ToFloat64(c, &y, args[1]) < 0) return JS_EXCEPTION;
                if (args.size() > 2 && JS_ToFloat64(c, &w, args[2]) < 0) return JS_EXCEPTION;
                if (args.size() > 3 && JS_ToFloat64(c, &h, args[3]) < 0) return JS_EXCEPTION;

                return Utils::create_js_instance<JSRectangle>(
                    c, new_target, js_rectangle_class_id,
                    static_cast<float>(x), static_cast<float>(y),
                    static_cast<float>(w), static_cast<float>(h)
                );
            },
            .proto_funcs = proto_funcs
        });
    }

    static void register_font_class(JSContext* ctx, JSModuleDef* m) {
        qjs::register_js_class(ctx, m, {
            .name = "Font",
            .class_id = js_font_class_id,
            .finalizer = [](JSRuntime*, JSValue val) {
                delete Utils::get_opaque<JSFont>(val, js_font_class_id);
            },
            .constructor = [](JSContext* c, JSValueConst new_target, int argc, JSValueConst* argv) -> JSValue {
                const std::span args{argv, static_cast<size_t>(argc)};

                if (args.empty()) return JS_ThrowTypeError(c, "Font requires at least a path argument");

                auto font_path = Utils::js_to_std_string(c, args[0]);
                if (font_path.empty()) return JS_ThrowTypeError(c, "Font path must be a non-empty string");

                if (args.size() >= 2) {
                    int32_t size = 0;
                    if (JS_ToInt32(c, &size, args[1]) < 0) return JS_EXCEPTION;
                    if (size <= 0) return JS_ThrowRangeError(c, "Font size must be a positive integer");
                    return Utils::create_js_instance<JSFont>(c, new_target, js_font_class_id, font_path, size);
                }

                return Utils::create_js_instance<JSFont>(c, new_target, js_font_class_id, font_path);
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