#include "hostapi.hpp"
#include "js_types.hpp"
#include "js_utils.hpp"

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

        Utils::register_js_class(ctx, m, {
            .name = "Application",
            .class_id = js_application_class_id,
            .finalizer = [](auto*, JSValue val) {
                delete Utils::get_opaque<JSApplication>(val, js_application_class_id);
            },
            .constructor = [](auto c, auto new_target, const int argc, auto argv) -> JSValue {
                int32_t h = 0, w = 0;
                if (argc > 0 && JS_ToInt32(c, &h, argv[0]) < 0) return JS_EXCEPTION;
                if (argc > 1 && JS_ToInt32(c, &w, argv[1]) < 0) return JS_EXCEPTION;
                std::string title = argc > 2 ? Utils::js_to_std_string(c, argv[2]) : "VectorJS Application";
                return Utils::create_js_instance<JSApplication>(c, new_target, js_application_class_id, h, w, std::move(title));
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

        Utils::register_js_class(ctx, m, {
            .name = "Color",
            .class_id = js_color_class_id,
            .finalizer = [](auto, JSValue val) {
                delete Utils::get_opaque<JSColor>(val, js_color_class_id);
            },
            .constructor = [](auto c, auto new_target, int argc, auto argv) -> JSValue {
                if (argc >= 4) {
                    int32_t r = 0, g = 0, b = 0, a = 255;
                    if (argc > 0 && JS_ToInt32(c, &r, argv[0]) < 0) return JS_EXCEPTION;
                    if (argc > 1 && JS_ToInt32(c, &g, argv[1]) < 0) return JS_EXCEPTION;
                    if (argc > 2 && JS_ToInt32(c, &b, argv[2]) < 0) return JS_EXCEPTION;
                    if (argc > 3 && JS_ToInt32(c, &a, argv[3]) < 0) return JS_EXCEPTION;

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

        Utils::register_js_class(ctx, m, {
            .name = "Vector2",
            .class_id = js_vector2_class_id,
            .finalizer = [](auto, JSValue val) {
                delete Utils::get_opaque<JSVector2>(val, js_vector2_class_id);
            },
            .constructor = [](auto c, auto new_target, int argc, auto argv) -> JSValue {
                double x = 0, y = 0;
                if (argc > 0 && JS_ToFloat64(c, &x, argv[0]) < 0) return JS_EXCEPTION;
                if (argc > 1 && JS_ToFloat64(c, &y, argv[1]) < 0) return JS_EXCEPTION;

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

        Utils::register_js_class(ctx, m, {
            .name = "Rectangle",
            .class_id = js_rectangle_class_id,
            .finalizer = [](auto, JSValue val) {
                delete Utils::get_opaque<JSRectangle>(val, js_rectangle_class_id);
            },
            .constructor = [](auto c, auto new_target, int argc, auto argv) -> JSValue {
                double x = 0, y = 0, w = 0, h = 0;
                if (argc > 0 && JS_ToFloat64(c, &x, argv[0]) < 0) return JS_EXCEPTION;
                if (argc > 1 && JS_ToFloat64(c, &y, argv[1]) < 0) return JS_EXCEPTION;
                if (argc > 2 && JS_ToFloat64(c, &w, argv[2]) < 0) return JS_EXCEPTION;
                if (argc > 3 && JS_ToFloat64(c, &h, argv[3]) < 0) return JS_EXCEPTION;

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
        Utils::register_js_class(ctx, m, {
            .name = "Font",
            .class_id = js_font_class_id,
            .finalizer = [](auto, JSValue val) {
                delete Utils::get_opaque<JSFont>(val, js_font_class_id);
            },
            .constructor = [](auto c, auto new_target, int argc, auto argv) -> JSValue {
                if (argc < 1) return JS_ThrowTypeError(c, "Font requires at least a path argument");

                auto font_path = Utils::js_to_std_string(c, argv[0]);
                if (font_path.empty()) return JS_ThrowTypeError(c, "Font path must be a non-empty string");

                if (argc >= 2) {
                    int32_t size = 0;
                    if (JS_ToInt32(c, &size, argv[1]) < 0) return JS_EXCEPTION;
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