#include "hostapi.hpp"
#include "js_types.hpp"
#include "js_utils.hpp"
#include "js_context.hpp"
#include "../config.hpp"

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
                if (argc > 0) JS_ToInt32(c, &h, argv[0]);
                if (argc > 1) JS_ToInt32(c, &w, argv[1]);
                std::string title = argc > 2 ? Utils::js_to_std_string(c, argv[2]) : "VectorJS Application";
                return Utils::create_js_instance<JSApplication>(c, new_target, js_application_class_id, h, w, std::move(title));
            },
            .proto_funcs = proto_funcs
        });
    }

    JSApplication::JSApplication(const int h, const int w, const std::string& title) {
        InitWindow(w, h, title.c_str());
        SetTargetFPS(60);
    }

    JSValue JSApplication::Run(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
        if (argc < 1) return JS_ThrowTypeError(ctx, "Expected a user application config object");
        JSValueConst user_app = argv[0];

        Utils::ScopedJSValue on_init_func(ctx, JS_GetPropertyStr(ctx, user_app, "onInit"));
        Utils::ScopedJSValue on_update_func(ctx, JS_GetPropertyStr(ctx, user_app, "onUpdate"));
        Utils::ScopedJSValue on_draw_func(ctx, JS_GetPropertyStr(ctx, user_app, "onDraw"));

        // Instantiate rendering and context objects ONCE outside the frame loop
        Utils::ScopedJSValue update_obj(ctx, create_update_context_object(ctx));
        Utils::ScopedJSValue render_obj(ctx, create_draw_render_object(ctx));

        if (JS_IsFunction(ctx, on_init_func)) {
            Utils::ScopedJSValue ret(ctx, JS_Call(ctx, on_init_func, user_app, 0, nullptr));
            if (JS_IsException(ret)) return JS_EXCEPTION;
        }

        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            if (JS_IsFunction(ctx, on_update_func)) {
                JSValue u = update_obj.get();
                Utils::ScopedJSValue ret(ctx, JS_Call(ctx, on_update_func, user_app, 1, &u));
                if (JS_IsException(ret)) {
                    EndDrawing();
                    break;
                }
            }
            if (JS_IsFunction(ctx, on_draw_func)) {
                JSValue r = render_obj.get();
                Utils::ScopedJSValue ret(ctx, JS_Call(ctx, on_draw_func, user_app, 1, &r));
                if (JS_IsException(ret)) {
                    EndDrawing();
                    break;
                }
            }
            EndDrawing();
        }

        if (IsWindowReady()) {
            CloseWindow();
        }

        return JS_UNDEFINED;
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
                    JS_ToInt32(c, &r, argv[0]);
                    JS_ToInt32(c, &g, argv[1]);
                    JS_ToInt32(c, &b, argv[2]);
                    JS_ToInt32(c, &a, argv[3]);

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
                int32_t x = 0, y = 0;
                if (argc > 0) JS_ToInt32(c, &x, argv[0]);
                if (argc > 1) JS_ToInt32(c, &y, argv[1]);

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
                if (argc > 0) JS_ToFloat64(c, &x, argv[0]);
                if (argc > 1) JS_ToFloat64(c, &y, argv[1]);
                if (argc > 2) JS_ToFloat64(c, &w, argv[2]);
                if (argc > 3) JS_ToFloat64(c, &h, argv[3]);

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
                if (font_path.empty()) return JS_EXCEPTION;

                if (argc >= 2) {
                    int32_t size = 0;
                    JS_ToInt32(c, &size, argv[1]);
                    return Utils::create_js_instance<JSFont>(c, new_target, js_font_class_id, font_path, size);
                }

                return Utils::create_js_instance<JSFont>(c, new_target, js_font_class_id, font_path);
            }
        });
    }

    static void register_image_class(JSContext* ctx, JSModuleDef* m) {
        static constexpr JSCFunctionListEntry proto_funcs[] = {
            JS_CGETSET_DEF(
                "width",
                ([](JSContext* c, JSValueConst this_val) -> JSValue {
                    const auto* img = Utils::get_opaque<JSImage>(this_val, js_image_class_id);
                    return img ? JS_NewInt32(c, img->get_width()) : JS_UNDEFINED;
                }),
                nullptr
            ),
            JS_CGETSET_DEF(
                "height",
                ([](JSContext* c, JSValueConst this_val) -> JSValue {
                    const auto* img = Utils::get_opaque<JSImage>(this_val, js_image_class_id);
                    return img ? JS_NewInt32(c, img->get_height()) : JS_UNDEFINED;
                }),
                nullptr
            )
        };

        Utils::register_js_class(ctx, m, {
            .name = "Image",
            .class_id = js_image_class_id,
            .finalizer = [](auto, JSValue val) {
                delete Utils::get_opaque<JSImage>(val, js_image_class_id);
            },
            .constructor = [](auto c, auto new_target, int argc, auto argv) -> JSValue {
                if (argc < 1) return JS_ThrowTypeError(c, "Image constructor requires a file path argument");

                auto image_path = Utils::js_to_std_string(c, argv[0]);
                if (image_path.empty()) return JS_EXCEPTION;

                return Utils::create_js_instance<JSImage>(c, new_target, js_image_class_id, image_path);
            },
            .proto_funcs = proto_funcs
        });
    }

    void register_hapi_classes(JSContext* ctx, JSModuleDef* m) {

        register_application_class(ctx, m);

        register_color_class(ctx, m);
        register_vector2_class(ctx, m);
        register_rectangle_class(ctx, m);
        register_image_class(ctx, m);

        if constexpr (config::features::enable_fonts) {
            register_font_class(ctx, m);
        }
    }

} // namespace HostApi

#undef JS_BIND_PROP