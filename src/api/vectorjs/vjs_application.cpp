#include <iostream>
#include <optional>
#include <memory>
#include <raylib.h>
#include <qjspp.hpp>

#include "screens.hpp"
#include "vjs_structs.hpp"

namespace App::Module::VectorJS {

    namespace {
        struct JSDrawOptions {
            JSColor color{BLACK};
            float rotation = 0.0f;
            bool wireframe = false;
            JSVector2 origin{0.0f, 0.0f};
        };

        struct JSTextOptions {
            JSFont font;
            JSColor color{BLACK};
            float rotation = 0.0f;
            float fontSize = 24.0f;
            float spacing = 1.0f;
            JSVector2 origin{0.0f, 0.0f};
        };
    }

    static JSDrawOptions parse_draw_options(const qjspp::Value& options_val) {
        JSDrawOptions options;
        if (!options_val.is_object()) return options;

        if (options_val.has("color")) {
            if (const auto* col = qjspp::get_native_opaque<JSColor>(options_val.get("color"))) {
                options.color = *col;
            }
        }
        if (options_val.has("origin")) {
            if (const auto* orig = qjspp::get_native_opaque<JSVector2>(options_val.get("origin"))) {
                options.origin = *orig;
            }
        }
        if (options_val.has("rotation")) {
            options.rotation = static_cast<float>(options_val.get("rotation").to_double());
        }
        if (options_val.has("wireframe")) {
            options.wireframe = options_val.get("wireframe").to_bool();
        }

        return options;
    }

    static JSTextOptions parse_text_options(const qjspp::Value& options_val) {
        JSTextOptions options;
        if (!options_val.is_object()) return options;

        if (options_val.has("font")) {
            if (const auto* font = qjspp::get_native_opaque<JSFont>(options_val.get("font"))) {
                options.font = *font;
            }
        }
        if (options_val.has("color")) {
            if (const auto* col = qjspp::get_native_opaque<JSColor>(options_val.get("color"))) {
                options.color = *col;
            }
        }
        if (options_val.has("rotation")) {
            options.rotation = static_cast<float>(options_val.get("rotation").to_double());
        }
        if (options_val.has("fontSize")) {
            options.fontSize = static_cast<float>(options_val.get("fontSize").to_double());
        }
        if (options_val.has("spacing")) {
            options.spacing = static_cast<float>(options_val.get("spacing").to_double());
        }
        if (options_val.has("origin")) {
            if (const auto* orig = qjspp::get_native_opaque<JSVector2>(options_val.get("origin"))) {
                options.origin = *orig;
            }
        }

        return options;
    }

    static qjspp::Value create_update_context_object(qjspp::Engine& engine) {
        qjspp::Value update_obj = engine.make_object();

        #define BIND_VOID_FN(js_name, cpp_fn) \
            update_obj.set(js_name, engine.make_function([](const qjspp::ArgList&) -> qjspp::Value { \
                ::cpp_fn(); \
                return {}; \
            }))

        #define BIND_BOOL_FN(js_name, cpp_fn) \
            update_obj.set(js_name, engine.make_function([&engine](const qjspp::ArgList&) -> qjspp::Value { \
                return engine.make_bool(::cpp_fn()); \
            }))

        #define BIND_INT_FN(js_name, cpp_fn) \
            update_obj.set(js_name, engine.make_function([&engine](const qjspp::ArgList&) -> qjspp::Value { \
                return engine.make_int(::cpp_fn()); \
            }))

        #define BIND_INT_PARAM_BOOL_FN(js_name, cpp_fn, err_msg) \
            update_obj.set(js_name, engine.make_function([&engine](const qjspp::ArgList& args) -> qjspp::Value { \
                if (args.empty()) throw std::runtime_error(err_msg); \
                return engine.make_bool(::cpp_fn(args[0].to_int())); \
            }))

        // Window Actions
        BIND_VOID_FN("minimizeWindow", MinimizeWindow);
        BIND_VOID_FN("maximizeWindow", MaximizeWindow);

        // Window Properties
        BIND_BOOL_FN("isWindowFullscreen", IsWindowFullscreen);
        BIND_BOOL_FN("isWindowHidden", IsWindowHidden);
        BIND_BOOL_FN("isWindowResized", IsWindowResized);
        BIND_BOOL_FN("isWindowMinimized", IsWindowMinimized);
        BIND_BOOL_FN("isWindowMaximized", IsWindowMaximized);
        BIND_BOOL_FN("isWindowFocused", IsWindowFocused);

        // Monitor
        BIND_INT_FN("getScreenWidth", GetScreenWidth);
        BIND_INT_FN("getScreenHeight", GetScreenHeight);
        BIND_INT_FN("getRenderWidth", GetRenderWidth);
        BIND_INT_FN("getRenderHeight", GetRenderHeight);
        BIND_INT_FN("getMonitorCount", GetMonitorCount);
        BIND_INT_FN("getCurrentMonitor", GetCurrentMonitor);

        // Keyboard Checks
        BIND_INT_PARAM_BOOL_FN("isKeyPressed", IsKeyPressed, "isKeyPressed requires a key code argument");
        BIND_INT_PARAM_BOOL_FN("isKeyDown", IsKeyDown, "isKeyDown requires a key code argument");
        BIND_INT_PARAM_BOOL_FN("isKeyReleased", IsKeyReleased, "isKeyReleased requires a key code argument");
        BIND_INT_PARAM_BOOL_FN("isKeyUp", IsKeyUp, "isKeyUp requires a key code argument");

        // Mouse Button Checks
        BIND_INT_PARAM_BOOL_FN("isMouseButtonPressed", IsMouseButtonPressed, "isMouseButtonPressed requires a button argument");
        BIND_INT_PARAM_BOOL_FN("isMouseButtonDown", IsMouseButtonDown, "isMouseButtonDown requires a button argument");
        BIND_INT_PARAM_BOOL_FN("isMouseButtonReleased", IsMouseButtonReleased, "isMouseButtonReleased requires a button argument");
        BIND_INT_PARAM_BOOL_FN("isMouseButtonUp", IsMouseButtonUp, "isMouseButtonUp requires a button argument");

        // Custom Mouse Getters
        update_obj.set("getMouseWheelMove", engine.make_function([&engine](const qjspp::ArgList&) -> qjspp::Value {
            return engine.make_double(GetMouseWheelMove());
        }));

        update_obj.set("getMousePosition", engine.make_function([&engine](const qjspp::ArgList&) -> qjspp::Value {
            const Vector2 pos = GetMousePosition();
            return qjspp::make_native_object(engine.context(), std::make_unique<JSVector2>(pos));
        }));

        #undef BIND_VOID_FN
        #undef BIND_BOOL_FN
        #undef BIND_INT_FN
        #undef BIND_INT_PARAM_BOOL_FN

        return update_obj;
    }

    static qjspp::Value create_shapes_object(const qjspp::Engine& engine) {
        qjspp::Value shape_obj = engine.make_object();

        shape_obj.set("drawPixel", engine.make_function([](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.size() < 2) throw std::runtime_error("drawPixel requires position and color arguments");
            auto* pos = qjspp::get_native_opaque<JSVector2>(args[0]);
            auto* col = qjspp::get_native_opaque<JSColor>(args[1]);

            DrawPixelV(pos ? static_cast<Vector2>(*pos) : Vector2{.x = 0, .y = 0},
                         col ? static_cast<::Color>(*col) : RAYWHITE);
            return {};
        }));

        shape_obj.set("drawLine", engine.make_function([](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.size() < 2) throw std::runtime_error("drawLine requires start and end positions");
            auto* start = qjspp::get_native_opaque<JSVector2>(args[0]);
            auto* end = qjspp::get_native_opaque<JSVector2>(args[1]);

            JSDrawOptions draw_options = parse_draw_options(args.size() > 2 ? args[2].clone() : qjspp::Value());
            DrawLineV(start ? static_cast<Vector2>(*start) : Vector2{.x = 0, .y = 0},
                        end ? static_cast<Vector2>(*end) : Vector2{.x = 0, .y = 0},
                        draw_options.color);
            return {};
        }));

        shape_obj.set("drawRectangle", engine.make_function([](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.empty()) throw std::runtime_error("drawRectangle requires a Rectangle argument");
            auto* rect = qjspp::get_native_opaque<JSRectangle>(args[0]);

            JSDrawOptions draw_options = parse_draw_options(args.size() > 1 ? args[1].clone() : qjspp::Value());
            DrawRectanglePro(rect ? static_cast<::Rectangle>(*rect) : ::Rectangle{.x = 0, .y = 0, .width = 0, .height = 0},
                               draw_options.origin,
                               draw_options.rotation,
                               draw_options.color);
            return {};
        }));

        shape_obj.set("drawCircle", engine.make_function([](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.size() < 2) throw std::runtime_error("drawCircle requires center and radius arguments");
            auto* center = qjspp::get_native_opaque<JSVector2>(args[0]);
            const double rad = args[1].to_double();

            JSDrawOptions draw_options = parse_draw_options(args.size() > 2 ? args[2].clone() : qjspp::Value());
            DrawCircleV(center ? static_cast<Vector2>(*center) : Vector2{.x = 0, .y = 0},
                          static_cast<float>(rad),
                          draw_options.color);
            return {};
        }));

        shape_obj.set("drawTriangle", engine.make_function([](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.size() < 3) throw std::runtime_error("drawTriangle requires 3 Vector2 point arguments");
            auto* p1 = qjspp::get_native_opaque<JSVector2>(args[0]);
            auto* p2 = qjspp::get_native_opaque<JSVector2>(args[1]);
            auto* p3 = qjspp::get_native_opaque<JSVector2>(args[2]);

            JSDrawOptions draw_options = parse_draw_options(args.size() > 3 ? args[3].clone() : qjspp::Value());
            DrawTriangle(p1 ? static_cast<Vector2>(*p1) : Vector2{.x = 0, .y = 0},
                           p2 ? static_cast<Vector2>(*p2) : Vector2{.x = 0, .y = 0},
                           p3 ? static_cast<Vector2>(*p3) : Vector2{.x = 0, .y = 0},
                           draw_options.color);
            return {};
        }));

        shape_obj.set("drawEllipse", engine.make_function([](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.size() < 3) throw std::runtime_error("drawEllipse requires center, radiusH, and radiusV arguments");
            auto* center = qjspp::get_native_opaque<JSVector2>(args[0]);
            const double radH = args[1].to_double();
            const double radV = args[2].to_double();

            JSDrawOptions draw_options = parse_draw_options(args.size() > 3 ? args[3].clone() : qjspp::Value());
            DrawEllipseV(center ? static_cast<Vector2>(*center) : Vector2{.x = 0, .y = 0},
                           static_cast<float>(radH),
                           static_cast<float>(radV),
                           draw_options.color);
            return {};
        }));

        return shape_obj;
    }

    static qjspp::Value create_text_object(const qjspp::Engine& engine) {
        qjspp::Value text_obj = engine.make_object();

        text_obj.set("drawText", engine.make_function([](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.size() < 2) throw std::runtime_error("drawText requires position and text string arguments");
            auto* pos = qjspp::get_native_opaque<JSVector2>(args[0]);
            const std::string txt_str = args[1].to_string();

            JSTextOptions options = parse_text_options(args.size() > 2 ? args[2].clone() : qjspp::Value());
            Font fontToUse = GetFontDefault();
            if (options.font.font_ptr && options.font.font_ptr->texture.id != 0) {
                fontToUse = *options.font.font_ptr;
            }

            DrawTextPro(fontToUse,
                          txt_str.c_str(),
                          pos ? static_cast<Vector2>(*pos) : Vector2{.x = 0, .y = 0},
                          options.origin,
                          options.rotation,
                          options.fontSize,
                          options.spacing,
                          options.color);
            return {};
        }));

        return text_obj;
    }

    static qjspp::Value create_viewport2d_function(const qjspp::Engine& engine, const qjspp::Value& r2d_val) {
        auto r2d_ptr = std::make_shared<qjspp::Value>(r2d_val.clone());
        return engine.make_function([r2d_ptr](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.empty()) throw std::runtime_error("withViewport2D requires at least a callback function");

            JSCamera2D camera;
            if (const auto* cam_ptr = qjspp::get_native_opaque<JSCamera2D>(args[0])) {
                camera = *cam_ptr;
            } else {
                return {};
            }

            const qjspp::Value callback = args[1].clone();

            BeginMode2D(camera);
            std::ignore = callback.call({r2d_ptr->clone()});
            EndMode2D();

            return {};
        });
    }

    static qjspp::Value create_screen_space_function(const qjspp::Engine& engine, const qjspp::Value& r2d_val) {
        auto r2d_ptr = std::make_shared<qjspp::Value>(r2d_val.clone());
        return engine.make_function([r2d_ptr](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.empty()) throw std::runtime_error("withScreenSpace requires a callback function");
            (void)args[0].call({r2d_ptr->clone()});
            return {};
        });
    }

    static qjspp::Value create_draw_render_object(const qjspp::Engine& engine) {
        qjspp::Value render2d_obj = engine.make_object();

        // Add FPS
        render2d_obj.set("drawFPS", engine.make_function([](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.empty()) throw std::runtime_error("drawFPS requires a Vector2 position argument");
            if (const auto* pos = qjspp::get_native_opaque<JSVector2>(args[0])) {
                DrawFPS(static_cast<int>(pos->x), static_cast<int>(pos->y));
            }
            return {};
        }));

        // Add Sub Objects
        render2d_obj.set("shapes", create_shapes_object(engine));
        render2d_obj.set("text", create_text_object(engine));

        // Layer Wrappers
        qjspp::Value render_obj = engine.make_object();
        render_obj.set("withViewport2D", create_viewport2d_function(engine, render2d_obj));
        render_obj.set("withScreenSpace", create_screen_space_function(engine, render2d_obj));

        // ClearBackground
        render_obj.set("clearBackground", engine.make_function([](const qjspp::ArgList& args) -> qjspp::Value {
            auto* color = args.empty() ? nullptr : qjspp::get_native_opaque<JSColor>(args[0]);
            ::ClearBackground(color ? static_cast<::Color>(*color) : BLACK);
            return {};
        }));

        return render_obj;
    }

    JSApplication::JSApplication(qjspp::Engine& engine, const int w, const int h, const std::string_view title): engine(engine) {
        InitWindow(w, h, title.data());
        InitAudioDevice();
        SetTargetFPS(60);
    }

    qjspp::Value JSApplication::run(const qjspp::ArgList& args) const {
        if (args.empty()) {
            throw std::runtime_error("Expected a user application config object");
        }

        const qjspp::Value user_app = args[0].clone();
        const qjspp::Value on_init_func = user_app.get("onInit");
        const qjspp::Value on_update_func = user_app.get("onUpdate");
        const qjspp::Value on_draw_func = user_app.get("onDraw");

        // Instantiate rendering and context objects ONCE outside the frame loop
        const qjspp::Value update_obj = create_update_context_object(engine);
        const qjspp::Value render_obj = create_draw_render_object(engine);

        try {
            if (!on_init_func.is_undefined()) {
                std::ignore = on_init_func.call_method(user_app, {});
            }

            while (!WindowShouldClose()) {
                BeginDrawing();

                if (!on_update_func.is_undefined()) {
                    std::ignore = on_update_func.call_method(user_app, {update_obj.clone()});
                }

                if (!on_draw_func.is_undefined()) {
                    std::ignore = on_draw_func.call_method(user_app, {render_obj.clone()});
                }

                EndDrawing();
            }
        } catch (const std::exception& e) {
            // Catch the qjspp/JS exception and pass the message to the BSOD screen
            std::cerr << "JS Exception during execution: " << e.what() << std::endl;
            show_bsod(e.what());
            return {};
        }

        if (IsWindowReady()) {
            CloseAudioDevice();
            CloseWindow();
        }

        return {};
    }

    void register_application_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto cls = engine.make_class<JSApplication>("Application");

        cls.constructor([&engine](const qjspp::ArgList& args) -> std::unique_ptr<JSApplication> {
            if (args.size() < 3) return nullptr;
            int w = args[0].to_int();
            int h = args[1].to_int();
            std::string title = args[2].to_string();

            return std::make_unique<JSApplication>(engine, w, h, title);
        });

        cls.instance_method("run", [](const JSApplication* app, const qjspp::ArgList& args) -> qjspp::Value {
            if (!app) return {};
            return app->run(args);
        });

        builder.export_class("Application", cls.build());
    }

} // namespace VectorJS