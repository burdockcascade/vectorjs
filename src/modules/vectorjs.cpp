#include <iostream>
#include <optional>
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <qjspp.hpp>
#include <array>

#include "vectorjs.hpp"
#include "screens.hpp"

namespace App::Modules {

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

    JSApplication::JSApplication(qjspp::Engine& engine, const int w, const int h, const std::string& title): engine(engine) {
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

        return {};
    }

    JSApplication::~JSApplication() {
        if (IsAudioDeviceReady()) {
            CloseAudioDevice();
        }
        if (IsWindowReady()) {
            CloseWindow();
        }
    }

    static void register_application_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
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

    static void register_sound_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto sound = engine.make_class<JSSound>("Sound");

        // Constructor: const mySound = new vectorjs.Sound("assets/sfx.wav");
        sound.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSSound> {
            if (args.empty()) return nullptr;
            return std::make_unique<JSSound>(args[0].to_string());
        });

        // Instance Method: mySound.play()
        sound.instance_method("play", [](JSSound* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->sound_ptr) {
                PlaySound(*(self->sound_ptr));
            }
            return {};
        });

        // Instance Method: mySound.pause()
        sound.instance_method("pause", [](JSSound* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->sound_ptr) {
                PauseSound(*(self->sound_ptr));
            }
            return {};
        });

        // Instance Method: mySound.resume()
        sound.instance_method("resume", [](JSSound* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->sound_ptr) {
                ResumeSound(*(self->sound_ptr));
            }
            return {};
        });

        // Instance Method: mySound.stop()
        sound.instance_method("stop", [](JSSound* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->sound_ptr) {
                StopSound(*(self->sound_ptr));
            }
            return {};
        });

        // Instance Method: mySound.setVolume(0.5)
        sound.instance_method("setVolume", [](JSSound* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->sound_ptr && !args.empty()) {
                SetSoundVolume(*(self->sound_ptr), static_cast<float>(args[0].to_double()));
            }
            return {};
        });

        builder.export_class("Sound", sound.build());
    }

    static void register_music_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto music = engine.make_class<JSMusic>("Music");

        // Constructor: const myMusic = new vectorjs.Sound("assets/sfx.wav");
        music.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSMusic> {
            if (args.empty()) return nullptr;
            return std::make_unique<JSMusic>(args[0].to_string());
        });

        // Instance Method: myMusic.play()
        music.instance_method("play", [](JSMusic* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->music_ptr) {
                PlayMusicStream(*(self->music_ptr));
            }
            return {};
        });

        // Instance Method: myMusic.pause()
        music.instance_method("pause", [](JSMusic* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->music_ptr) {
                PauseMusicStream(*(self->music_ptr));
            }
            return {};
        });

        // Instance Method: myMusic.resume()
        music.instance_method("resume", [](JSMusic* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->music_ptr) {
                ResumeMusicStream(*(self->music_ptr));
            }
            return {};
        });

        // Instance Method: myMusic.stop()
        music.instance_method("stop", [](JSMusic* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->music_ptr) {
                StopMusicStream(*(self->music_ptr));
            }
            return {};
        });

        // Instance Method: myMusic.update()
        music.instance_method("update", [](JSMusic* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->music_ptr) {
                UpdateMusicStream(*(self->music_ptr));
            }
            return {};
        });

        // Instance Method: myMusic.setVolume(0.5)
        music.instance_method("setVolume", [](JSMusic* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->music_ptr && !args.empty()) {
                SetMusicVolume(*(self->music_ptr), static_cast<float>(args[0].to_double()));
            }
            return {};
        });

        builder.export_class("Music", music.build());
    }

    static void register_vector2(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto vector2 = engine.make_class<JSVector2>("Vector2");

        vector2.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSVector2> {
            if (args.size() < 2) return nullptr;
            return std::make_unique<JSVector2>(static_cast<float>(args[0].to_double()), static_cast<float>(args[1].to_double()));
        });

        vector2.property("x",
            [](JSContext* ctx, JSVector2* self) { return qjspp::Value::make_double(ctx, self->x); },
            [](JSVector2* self, const qjspp::Value& val) { self->x = static_cast<float>(val.to_double()); }
        );

        vector2.property("y",
            [](JSContext* ctx, JSVector2* self) { return qjspp::Value::make_double(ctx, self->y); },
            [](JSVector2* self, const qjspp::Value& val) { self->y = static_cast<float>(val.to_double()); }
        );

        vector2.instance_method("add", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            auto* other = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!self || !other) return {};
            Vector2 res = Vector2Add(*self, *other);
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSVector2>(res));
        });

        vector2.instance_method("scale", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            Vector2 res = Vector2Scale(*self, static_cast<float>(args[0].to_double()));
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSVector2>(res));
        });

        vector2.instance_method("length", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self) return {};
            return qjspp::Value::make_double(args[0].context(), Vector2Length(*self));
        });

        vector2.instance_method("normalize", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self) return {};
            Vector2 res = Vector2Normalize(*self);
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSVector2>(res));
        });

        vector2.instance_method("subtract", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            auto* other = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!self || !other) return {};
            Vector2 res = Vector2Subtract(*self, *other);
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSVector2>(res));
        });

        vector2.instance_method("multiply", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            auto* other = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!self || !other) return {};
            Vector2 res = Vector2Multiply(*self, *other);
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSVector2>(res));
        });

        vector2.instance_method("dot", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            auto* other = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!self || !other) return {};
            return qjspp::Value::make_double(args[0].context(), Vector2DotProduct(*self, *other));
        });

        vector2.instance_method("distance", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            auto* other = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!self || !other) return {};
            return qjspp::Value::make_double(args[0].context(), Vector2Distance(*self, *other));
        });

        vector2.instance_method("negate", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self) return {};
            Vector2 res = Vector2Negate(*self);
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSVector2>(res));
        });

        vector2.instance_method("lerp", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (args.size() < 2) return {};
            auto* target = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!self || !target) return {};
            Vector2 res = Vector2Lerp(*self, *target, static_cast<float>(args[1].to_double()));
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSVector2>(res));
        });

        builder.export_class("Vector2", vector2.build());
    }

    static void register_rectangle(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto rectangle = engine.make_class<JSRectangle>("Rectangle");

        rectangle.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSRectangle> {
            if (args.size() < 4) return nullptr;
            return std::make_unique<JSRectangle>(
                static_cast<float>(args[0].to_double()),
                static_cast<float>(args[1].to_double()),
                static_cast<float>(args[2].to_double()),
                static_cast<float>(args[3].to_double())
            );
        });

        rectangle.property("x",
            [](JSContext* ctx, JSRectangle* self) { return qjspp::Value::make_double(ctx, self->x); },
            [](JSRectangle* self, const qjspp::Value& val) { self->x = static_cast<float>(val.to_double()); }
        );

        rectangle.property("y",
            [](JSContext* ctx, JSRectangle* self) { return qjspp::Value::make_double(ctx, self->y); },
            [](JSRectangle* self, const qjspp::Value& val) { self->y = static_cast<float>(val.to_double()); }
        );

        rectangle.property("width",
            [](JSContext* ctx, JSRectangle* self) { return qjspp::Value::make_double(ctx, self->width); },
            [](JSRectangle* self, const qjspp::Value& val) { self->width = static_cast<float>(val.to_double()); }
        );

        rectangle.property("height",
            [](JSContext* ctx, JSRectangle* self) { return qjspp::Value::make_double(ctx, self->height); },
            [](JSRectangle* self, const qjspp::Value& val) { self->height = static_cast<float>(val.to_double()); }
        );

        builder.export_class("Rectangle", rectangle.build());
    }

    static void register_font_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto font = engine.make_class<JSFont>("Font");

        font.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSFont> {
            if (args.empty()) return nullptr;
            std::string path = args[0].to_string();
            if (args.size() >= 2) {
                return std::make_unique<JSFont>(path, args[1].to_int());
            }
            return std::make_unique<JSFont>(path);
        });

        builder.export_class("Font", font.build());
    }

    static void register_camera2d(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto camera = engine.make_class<JSCamera2D>("Camera2D");

        camera.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSCamera2D> {
            if (args.size() >= 2) {
                auto* offset = qjspp::get_native_opaque<JSVector2>(args[0]);
                auto* target = qjspp::get_native_opaque<JSVector2>(args[1]);

                Vector2 off = offset ? static_cast<Vector2>(*offset) : Vector2{0, 0};
                Vector2 tgt = target ? static_cast<Vector2>(*target) : Vector2{0, 0};
                float rot = args.size() > 2 ? static_cast<float>(args[2].to_double()) : 0.0f;
                float zoom = args.size() > 3 ? static_cast<float>(args[3].to_double()) : 1.0f;

                return std::make_unique<JSCamera2D>(off, tgt, rot, zoom);
            }
            return std::make_unique<JSCamera2D>();
        });

        // =========================================================================
        // Instance Methods
        // =========================================================================

        // Move target along x (horizontal) and y (vertical) axes
        camera.instance_method("move", [&engine](JSCamera2D* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (args.size() >= 2) {
                self->target.x += static_cast<float>(args[0].to_double());
                self->target.y += static_cast<float>(args[1].to_double());
            } else if (args.size() == 1) {
                if (auto* vec = qjspp::get_native_opaque<JSVector2>(args[0])) {
                    self->target.x += vec->x;
                    self->target.y += vec->y;
                }
            }
            return engine.make_undefined();
        });

        // Horizontal movement: moveX(deltaX)
        camera.instance_method("moveX", [&engine](JSCamera2D* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!args.empty()) {
                self->target.x += static_cast<float>(args[0].to_double());
            }
            return engine.make_undefined();
        });

        // Vertical movement: moveY(deltaY)
        camera.instance_method("moveY", [&engine](JSCamera2D* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!args.empty()) {
                self->target.y += static_cast<float>(args[0].to_double());
            }
            return engine.make_undefined();
        });

        // Relative zoom adjusting: zoomBy(factor)
        camera.instance_method("zoomBy", [&engine](JSCamera2D* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!args.empty()) {
                self->zoom *= static_cast<float>(args[0].to_double());
            }
            if (self->zoom < 0.001f) self->zoom = 0.001f;
            return engine.make_undefined();
        });

        // Zoom In helper: zoomIn(amount = 0.1)
        camera.instance_method("zoomIn", [&engine](JSCamera2D* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!args.empty()) {
                self->zoom += static_cast<float>(args[0].to_double());
            }
            if (self->zoom < 0.001f) self->zoom = 0.001f;
            return engine.make_undefined();
        });

        // Zoom Out helper: zoomOut(amount = 0.1)
        camera.instance_method("zoomOut", [&engine](JSCamera2D* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!args.empty()) {
                self->zoom -= static_cast<float>(args[0].to_double());
            }
            if (self->zoom < 0.001f) self->zoom = 0.001f;
            return engine.make_undefined();
        });

        // Convert screen space position to world space
        camera.instance_method("toWorldSpace", [](JSCamera2D* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            auto* screen_pos = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!screen_pos) return {};

            Vector2 world_pos = GetScreenToWorld2D(*screen_pos, *self);
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSVector2>(world_pos));
        });

        // Convert world space position to screen space
        camera.instance_method("toScreenSpace", [](JSCamera2D* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            auto* world_pos = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!world_pos) return {};

            Vector2 screen_pos = GetWorldToScreen2D(*world_pos, *self);
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSVector2>(screen_pos));
        });

        // =========================================================================
        // Properties
        // =========================================================================

        camera.property("target",
            [](JSContext* ctx, JSCamera2D* self) {
                return qjspp::make_native_object(ctx, std::make_unique<JSVector2>(self->target));
            },
            [](JSCamera2D* self, const qjspp::Value& val) {
                auto* vec = qjspp::get_native_opaque<JSVector2>(val);
                if (vec) self->target = *vec;
            }
        );

        camera.property("offset",
            [](JSContext* ctx, JSCamera2D* self) {
                return qjspp::make_native_object(ctx, std::make_unique<JSVector2>(self->offset));
            },
            [](JSCamera2D* self, const qjspp::Value& val) {
                auto* vec = qjspp::get_native_opaque<JSVector2>(val);
                if (vec) self->offset = *vec;
            }
        );

        camera.property("rotation",
            [](JSContext* ctx, JSCamera2D* self) { return qjspp::Value::make_double(ctx, self->rotation); },
            [](JSCamera2D* self, const qjspp::Value& val) { self->rotation = static_cast<float>(val.to_double()); }
        );

        camera.property("zoom",
            [](JSContext* ctx, JSCamera2D* self) { return qjspp::Value::make_double(ctx, self->zoom); },
            [](JSCamera2D* self, const qjspp::Value& val) {
                auto z = static_cast<float>(val.to_double());
                self->zoom = (z < 0.001f) ? 0.001f : z;
            }
        );

        builder.export_class("Camera2D", camera.build());
    }

    static void register_palette_object(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {

        struct ColorEntry {
            const char* name;
            ::Color color;
        };

        static constexpr std::array palette = {
            // Grays & Neutrals
            ColorEntry{.name = "WHITE", .color = {.r = 255, .g = 255, .b = 255, .a = 255}},
            ColorEntry{.name = "SNOW", .color = {.r = 255, .g = 250, .b = 250, .a = 255}},
            ColorEntry{.name = "IVORY", .color = {.r = 255, .g = 255, .b = 240, .a = 255}},
            ColorEntry{.name = "RAYWHITE", .color = {.r = 245, .g = 245, .b = 245, .a = 255}},
            ColorEntry{.name = "OFFWHITE", .color = {.r = 238, .g = 238, .b = 228, .a = 255}},
            ColorEntry{.name = "GAINSBORO", .color = {.r = 220, .g = 220, .b = 220, .a = 255}},
            ColorEntry{.name = "LIGHTGRAY", .color = {.r = 200, .g = 200, .b = 200, .a = 255}},
            ColorEntry{.name = "SILVER", .color = {.r = 192, .g = 192, .b = 192, .a = 255}},
            ColorEntry{.name = "GRAY", .color = {.r = 130, .g = 130, .b = 130, .a = 255}},
            ColorEntry{.name = "SLATE", .color = {.r = 112, .g = 128, .b = 144, .a = 255}},
            ColorEntry{.name = "DARKGRAY", .color = {.r = 80, .g = 80, .b = 80, .a = 255}},
            ColorEntry{.name = "CHARBLACK", .color = {.r = 30, .g = 30, .b = 30, .a = 255}},
            ColorEntry{.name = "BLACK", .color = {.r = 0, .g = 0, .b = 0, .a = 255}},
            ColorEntry{.name = "BLANK", .color = {.r = 0, .g = 0, .b = 0, .a = 0}},

            // Reds, Pinks & Oranges
            ColorEntry{.name = "SALMON", .color = {.r = 250, .g = 128, .b = 114, .a = 255}},
            ColorEntry{.name = "CORAL", .color = {.r = 255, .g = 127, .b = 80, .a = 255}},
            ColorEntry{.name = "TOMATO", .color = {.r = 255, .g = 99, .b = 71, .a = 255}},
            ColorEntry{.name = "CRIMSON", .color = {.r = 180, .g = 10, .b = 30, .a = 255}},
            ColorEntry{.name = "RED", .color = {.r = 230, .g = 41, .b = 55, .a = 255}},
            ColorEntry{.name = "PURE_RED", .color = {.r = 255, .g = 0, .b = 0, .a = 255}},
            ColorEntry{.name = "SCARLET", .color = {.r = 255, .g = 36, .b = 0, .a = 255}},
            ColorEntry{.name = "MAROON", .color = {.r = 190, .g = 33, .b = 55, .a = 255}},
            ColorEntry{.name = "BURGUNDY", .color = {.r = 128, .g = 0, .b = 32, .a = 255}},
            ColorEntry{.name = "PEACH", .color = {.r = 255, .g = 218, .b = 185, .a = 255}},
            ColorEntry{.name = "ORANGE", .color = {.r = 255, .g = 161, .b = 0, .a = 255}},
            ColorEntry{.name = "DARKORANGE", .color = {.r = 255, .g = 140, .b = 0, .a = 255}},
            ColorEntry{.name = "AMBER", .color = {.r = 255, .g = 191, .b = 0, .a = 255}},
            ColorEntry{.name = "GOLD", .color = {.r = 255, .g = 203, .b = 0, .a = 255}},
            ColorEntry{.name = "YELLOW", .color = {.r = 253, .g = 249, .b = 0, .a = 255}},
            ColorEntry{.name = "LEMON", .color = {.r = 255, .g = 247, .b = 0, .a = 255}},
            ColorEntry{.name = "ROSE", .color = {.r = 255, .g = 192, .b = 203, .a = 255}},
            ColorEntry{.name = "PINK", .color = {.r = 255, .g = 109, .b = 194, .a = 255}},
            ColorEntry{.name = "HOTPINK", .color = {.r = 255, .g = 20, .b = 147, .a = 255}},
            ColorEntry{.name = "DEEPPINK", .color = {.r = 255, .g = 20, .b = 147, .a = 255}},
            ColorEntry{.name = "MAGENTA", .color = {.r = 255, .g = 0, .b = 255, .a = 255}},
            ColorEntry{.name = "FUCHSIA", .color = {.r = 224, .g = 17, .b = 95, .a = 255}},

            // Greens & Teals
            ColorEntry{.name = "MINT", .color = {.r = 152, .g = 251, .b = 152, .a = 255}},
            ColorEntry{.name = "PASTELGREEN", .color = {.r = 119, .g = 221, .b = 119, .a = 255}},
            ColorEntry{.name = "LIME", .color = {.r = 0, .g = 158, .b = 47, .a = 255}},
            ColorEntry{.name = "LIMEGREEN", .color = {.r = 50, .g = 205, .b = 50, .a = 255}},
            ColorEntry{.name = "NEON_GREEN", .color = {.r = 57, .g = 255, .b = 20, .a = 255}},
            ColorEntry{.name = "GREEN", .color = {.r = 0, .g = 228, .b = 48, .a = 255}},
            ColorEntry{.name = "PURE_GREEN", .color = {.r = 0, .g = 255, .b = 0, .a = 255}},
            ColorEntry{.name = "EMERALD", .color = {.r = 80, .g = 200, .b = 120, .a = 255}},
            ColorEntry{.name = "DARKGREEN", .color = {.r = 0, .g = 117, .b = 44, .a = 255}},
            ColorEntry{.name = "FOREST", .color = {.r = 34, .g = 139, .b = 34, .a = 255}},
            ColorEntry{.name = "PINE", .color = {.r = 1, .g = 121, .b = 111, .a = 255}},
            ColorEntry{.name = "OLIVE", .color = {.r = 128, .g = 128, .b = 0, .a = 255}},
            ColorEntry{.name = "TURQUOISE", .color = {.r = 64, .g = 224, .b = 208, .a = 255}},
            ColorEntry{.name = "TEAL", .color = {.r = 0, .g = 128, .b = 128, .a = 255}},
            ColorEntry{.name = "SEA_GREEN", .color = {.r = 46, .g = 139, .b = 87, .a = 255}},

            // Blues & Cyans
            ColorEntry{.name = "ELECTRIC_BLUE", .color = {.r = 125, .g = 249, .b = 255, .a = 255}},
            ColorEntry{.name = "CYAN", .color = {.r = 0, .g = 255, .b = 255, .a = 255}},
            ColorEntry{.name = "AQUA", .color = {.r = 0, .g = 255, .b = 255, .a = 255}},
            ColorEntry{.name = "PASTELBLUE", .color = {.r = 174, .g = 198, .b = 207, .a = 255}},
            ColorEntry{.name = "SKYBLUE", .color = {.r = 102, .g = 191, .b = 255, .a = 255}},
            ColorEntry{.name = "DEEPSKYBLUE", .color = {.r = 0, .g = 191, .b = 255, .a = 255}},
            ColorEntry{.name = "CORNFLOWER", .color = {.r = 100, .g = 149, .b = 237, .a = 255}},
            ColorEntry{.name = "BLUE", .color = {.r = 0, .g = 121, .b = 241, .a = 255}},
            ColorEntry{.name = "PURE_BLUE", .color = {.r = 0, .g = 0, .b = 255, .a = 255}},
            ColorEntry{.name = "ROYALBLUE", .color = {.r = 65, .g = 105, .b = 225, .a = 255}},
            ColorEntry{.name = "DARKBLUE", .color = {.r = 0, .g = 82, .b = 172, .a = 255}},
            ColorEntry{.name = "NAVY", .color = {.r = 0, .g = 0, .b = 128, .a = 255}},
            ColorEntry{.name = "MIDNIGHTBLUE", .color = {.r = 25, .g = 25, .b = 112, .a = 255}},

            // Purples & Violets
            ColorEntry{.name = "LAVENDER", .color = {.r = 230, .g = 230, .b = 250, .a = 255}},
            ColorEntry{.name = "LILAC", .color = {.r = 200, .g = 162, .b = 200, .a = 255}},
            ColorEntry{.name = "PURPLE", .color = {.r = 200, .g = 122, .b = 255, .a = 255}},
            ColorEntry{.name = "PURE_PURPLE", .color = {.r = 128, .g = 0, .b = 128, .a = 255}},
            ColorEntry{.name = "AMETHYST", .color = {.r = 153, .g = 102, .b = 204, .a = 255}},
            ColorEntry{.name = "VIOLET", .color = {.r = 135, .g = 60, .b = 190, .a = 255}},
            ColorEntry{.name = "INDIGO", .color = {.r = 75, .g = 0, .b = 130, .a = 255}},
            ColorEntry{.name = "DARKPURPLE", .color = {.r = 112, .g = 31, .b = 126, .a = 255}},
            ColorEntry{.name = "PLUM", .color = {.r = 142, .g = 69, .b = 133, .a = 255}},
            ColorEntry{.name = "ORCHID", .color = {.r = 218, .g = 112, .b = 214, .a = 255}},

            // Browns & Earth Tones
            ColorEntry{.name = "WHEAT", .color = {.r = 245, .g = 222, .b = 179, .a = 255}},
            ColorEntry{.name = "BEIGE", .color = {.r = 211, .g = 176, .b = 131, .a = 255}},
            ColorEntry{.name = "TAN", .color = {.r = 210, .g = 180, .b = 140, .a = 255}},
            ColorEntry{.name = "SAND", .color = {.r = 194, .g = 178, .b = 128, .a = 255}},
            ColorEntry{.name = "KHAKI", .color = {.r = 189, .g = 183, .b = 107, .a = 255}},
            ColorEntry{.name = "TERRACOTTA", .color = {.r = 226, .g = 114, .b = 91, .a = 255}},
            ColorEntry{.name = "BRONZE", .color = {.r = 205, .g = 127, .b = 50, .a = 255}},
            ColorEntry{.name = "COPPER", .color = {.r = 184, .g = 115, .b = 51, .a = 255}},
            ColorEntry{.name = "RUST", .color = {.r = 183, .g = 65, .b = 14, .a = 255}},
            ColorEntry{.name = "BROWN", .color = {.r = 127, .g = 106, .b = 91, .a = 255}},
            ColorEntry{.name = "SADDLEBROWN", .color = {.r = 139, .g = 69, .b = 19, .a = 255}},
            ColorEntry{.name = "DARKBROWN", .color = {.r = 76, .g = 63, .b = 47, .a = 255}},
            ColorEntry{.name = "COFFEE", .color = {.r = 111, .g = 78, .b = 55, .a = 255}},
            ColorEntry{.name = "CHOCOLATE", .color = {.r = 210, .g = 105, .b = 30, .a = 255}}
        };

        qjspp::Value obj = qjspp::Value::make_object(engine.context());

        for (const auto& [name, color] : palette) {
            auto color_ptr = std::make_unique<JSColor>(color);
            qjspp::Value color_val = qjspp::make_native_object<JSColor>(engine.context(), std::move(color_ptr));
            obj.set(name, color_val);
        }

        builder.export_value("Palette", std::move(obj));
    }

    static void register_color_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto color = engine.make_class<JSColor>("Color");

        // Constructor: Takes RGBA integer values (4 arguments) to create a new JSColor instance
        color.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSColor> {
            if (args.size() < 4) return nullptr;
            auto r = static_cast<uint8_t>(args[0].to_int());
            auto g = static_cast<uint8_t>(args[1].to_int());
            auto b = static_cast<uint8_t>(args[2].to_int());
            auto a = static_cast<uint8_t>(args[3].to_int());
            return std::make_unique<JSColor>(r, g, b, a);
        });

        // Property: 'r' (Red channel getter/setter)
        color.property("r",
            [](JSContext* ctx, JSColor* self) { return qjspp::Value::make_int(ctx, self->r); },
            [](JSColor* self, const qjspp::Value& val) { self->r = static_cast<uint8_t>(val.to_int()); }
        );

        // Property: 'g' (Green channel getter/setter)
        color.property("g",
            [](JSContext* ctx, JSColor* self) { return qjspp::Value::make_int(ctx, self->g); },
            [](JSColor* self, const qjspp::Value& val) { self->g = static_cast<uint8_t>(val.to_int()); }
        );

        // Property: 'b' (Blue channel getter/setter)
        color.property("b",
            [](JSContext* ctx, JSColor* self) { return qjspp::Value::make_int(ctx, self->b); },
            [](JSColor* self, const qjspp::Value& val) { self->b = static_cast<uint8_t>(val.to_int()); }
        );

        // Property: 'a' (Alpha channel getter/setter)
        color.property("a",
            [](JSContext* ctx, JSColor* self) { return qjspp::Value::make_int(ctx, self->a); },
            [](JSColor* self, const qjspp::Value& val) { self->a = static_cast<uint8_t>(val.to_int()); }
        );

        // Instance Method: Linearly interpolates (lerp) towards a target color using a float factor
        color.instance_method("lerp", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (args.size() < 2) return {};
            auto* target = qjspp::get_native_opaque<JSColor>(args[0]);
            if (!self || !target) return {};

            const double factor = args[1].to_double();
            Color result = ColorLerp(*self, *target, static_cast<float>(factor));
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSColor>(result));
        });

        // Instance Method: Returns a new color with adjusted opacity (alpha factor)
        color.instance_method("fade", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            const double alpha = args[0].to_double();
            Color result = Fade(*self, static_cast<float>(alpha));
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSColor>(result));
        });

        // Instance Method: Adjusts the brightness of the color by a specified factor
        color.instance_method("brightness", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            const double factor = args[0].to_double();
            Color result = ColorBrightness(*self, static_cast<float>(factor));
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSColor>(result));
        });

        // Instance Method: Converts the color to its packed integer representation
        color.instance_method("toInt", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self) return {};
            return qjspp::Value::make_int(args[0].context(), ColorToInt(*self));
        });

        // Instance Method: Adjusts the contrast of the color by a specified factor
        color.instance_method("contrast", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            Color result = ColorContrast(*self, static_cast<float>(args[0].to_double()));
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSColor>(result));
        });

        // Instance Method: Sets the alpha (transparency) of the color
        color.instance_method("alpha", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            Color result = ColorAlpha(*self, static_cast<float>(args[0].to_double()));
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSColor>(result));
        });

        // Instance Method: Tints the current color with another JSColor instance
        color.instance_method("tint", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            auto* tintColor = qjspp::get_native_opaque<JSColor>(args[0]);
            if (!tintColor) return {};
            Color result = ColorTint(*self, *tintColor);
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSColor>(result));
        });

        // Static Method: Factory function creating a Color object from a hexadecimal integer
        color.static_method("fromHex", [](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.empty()) return {};
            Color result = GetColor(static_cast<unsigned int>(args[0].to_int()));
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSColor>(result));
        });

        // Static Method: Factory function creating a Color object from HSV parameters (hue, saturation, value)
        color.static_method("fromHSV", [](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.size() < 3) return {};
            float h = static_cast<float>(args[0].to_double());
            float s = static_cast<float>(args[1].to_double());
            float v = static_cast<float>(args[2].to_double());
            Color result = ColorFromHSV(h, s, v);
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSColor>(result));
        });

        // Instance Method: Checks equality against another JSColor instance
        color.instance_method("isEqual", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return qjspp::Value::make_bool(args[0].context(), false);
            auto* other = qjspp::get_native_opaque<JSColor>(args[0]);
            bool equal = other && ColorIsEqual(*self, *other);
            return qjspp::Value::make_bool(args[0].context(), equal);
        });

        builder.export_class("Color", color.build());
    }

    static void register_hapi_enums(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {

        #define BIND_ENUM(val) std::pair{#val, val}

        // --- Info Object ---
        export_object(engine, builder, "Info",
            std::pair("RAYLIB_VERSION", RAYLIB_VERSION_STR),
            std::pair("QUICKJS_VERSION", QUICKJS_VERSION_STR)
        );

        // --- ConfigFlags Object ---
        export_object(engine, builder, "ConfigFlags",
            BIND_ENUM(FLAG_VSYNC_HINT),
            BIND_ENUM(FLAG_FULLSCREEN_MODE),
            BIND_ENUM(FLAG_WINDOW_RESIZABLE),
            BIND_ENUM(FLAG_WINDOW_UNDECORATED),
            BIND_ENUM(FLAG_WINDOW_HIDDEN),
            BIND_ENUM(FLAG_WINDOW_MINIMIZED),
            BIND_ENUM(FLAG_WINDOW_MAXIMIZED),
            BIND_ENUM(FLAG_WINDOW_UNFOCUSED),
            BIND_ENUM(FLAG_WINDOW_TOPMOST),
            BIND_ENUM(FLAG_WINDOW_ALWAYS_RUN),
            BIND_ENUM(FLAG_WINDOW_TRANSPARENT),
            BIND_ENUM(FLAG_WINDOW_HIGHDPI),
            BIND_ENUM(FLAG_WINDOW_MOUSE_PASSTHROUGH),
            BIND_ENUM(FLAG_BORDERLESS_WINDOWED_MODE),
            BIND_ENUM(FLAG_MSAA_4X_HINT),
            BIND_ENUM(FLAG_INTERLACED_HINT)
        );

        // --- Keyboard Object ---
        export_object(engine, builder, "Keyboard",
            BIND_ENUM(KEY_NULL),
            BIND_ENUM(KEY_APOSTROPHE),
            BIND_ENUM(KEY_COMMA),
            BIND_ENUM(KEY_MINUS),
            BIND_ENUM(KEY_PERIOD),
            BIND_ENUM(KEY_SLASH),
            BIND_ENUM(KEY_ZERO),
            BIND_ENUM(KEY_ONE),
            BIND_ENUM(KEY_TWO),
            BIND_ENUM(KEY_THREE),
            BIND_ENUM(KEY_FOUR),
            BIND_ENUM(KEY_FIVE),
            BIND_ENUM(KEY_SIX),
            BIND_ENUM(KEY_SEVEN),
            BIND_ENUM(KEY_EIGHT),
            BIND_ENUM(KEY_NINE),
            BIND_ENUM(KEY_SEMICOLON),
            BIND_ENUM(KEY_EQUAL),
            BIND_ENUM(KEY_A),
            BIND_ENUM(KEY_B),
            BIND_ENUM(KEY_C),
            BIND_ENUM(KEY_D),
            BIND_ENUM(KEY_E),
            BIND_ENUM(KEY_F),
            BIND_ENUM(KEY_G),
            BIND_ENUM(KEY_H),
            BIND_ENUM(KEY_I),
            BIND_ENUM(KEY_J),
            BIND_ENUM(KEY_K),
            BIND_ENUM(KEY_L),
            BIND_ENUM(KEY_M),
            BIND_ENUM(KEY_N),
            BIND_ENUM(KEY_O),
            BIND_ENUM(KEY_P),
            BIND_ENUM(KEY_Q),
            BIND_ENUM(KEY_R),
            BIND_ENUM(KEY_S),
            BIND_ENUM(KEY_T),
            BIND_ENUM(KEY_U),
            BIND_ENUM(KEY_V),
            BIND_ENUM(KEY_W),
            BIND_ENUM(KEY_X),
            BIND_ENUM(KEY_Y),
            BIND_ENUM(KEY_Z),
            BIND_ENUM(KEY_LEFT_BRACKET),
            BIND_ENUM(KEY_BACKSLASH),
            BIND_ENUM(KEY_RIGHT_BRACKET),
            BIND_ENUM(KEY_GRAVE),
            BIND_ENUM(KEY_SPACE),
            BIND_ENUM(KEY_ESCAPE),
            BIND_ENUM(KEY_ENTER),
            BIND_ENUM(KEY_TAB),
            BIND_ENUM(KEY_BACKSPACE),
            BIND_ENUM(KEY_INSERT),
            BIND_ENUM(KEY_DELETE),
            BIND_ENUM(KEY_RIGHT),
            BIND_ENUM(KEY_LEFT),
            BIND_ENUM(KEY_DOWN),
            BIND_ENUM(KEY_UP),
            BIND_ENUM(KEY_PAGE_UP),
            BIND_ENUM(KEY_PAGE_DOWN),
            BIND_ENUM(KEY_HOME),
            BIND_ENUM(KEY_END),
            BIND_ENUM(KEY_CAPS_LOCK),
            BIND_ENUM(KEY_SCROLL_LOCK),
            BIND_ENUM(KEY_NUM_LOCK),
            BIND_ENUM(KEY_PRINT_SCREEN),
            BIND_ENUM(KEY_PAUSE),
            BIND_ENUM(KEY_F1),
            BIND_ENUM(KEY_F2),
            BIND_ENUM(KEY_F3),
            BIND_ENUM(KEY_F4),
            BIND_ENUM(KEY_F5),
            BIND_ENUM(KEY_F6),
            BIND_ENUM(KEY_F7),
            BIND_ENUM(KEY_F8),
            BIND_ENUM(KEY_F9),
            BIND_ENUM(KEY_F10),
            BIND_ENUM(KEY_F11),
            BIND_ENUM(KEY_F12),
            BIND_ENUM(KEY_LEFT_SHIFT),
            BIND_ENUM(KEY_LEFT_CONTROL),
            BIND_ENUM(KEY_LEFT_ALT),
            BIND_ENUM(KEY_LEFT_SUPER),
            BIND_ENUM(KEY_RIGHT_SHIFT),
            BIND_ENUM(KEY_RIGHT_CONTROL),
            BIND_ENUM(KEY_RIGHT_ALT),
            BIND_ENUM(KEY_RIGHT_SUPER),
            BIND_ENUM(KEY_KB_MENU),
            BIND_ENUM(KEY_KP_0),
            BIND_ENUM(KEY_KP_1),
            BIND_ENUM(KEY_KP_2),
            BIND_ENUM(KEY_KP_3),
            BIND_ENUM(KEY_KP_4),
            BIND_ENUM(KEY_KP_5),
            BIND_ENUM(KEY_KP_6),
            BIND_ENUM(KEY_KP_7),
            BIND_ENUM(KEY_KP_8),
            BIND_ENUM(KEY_KP_9),
            BIND_ENUM(KEY_KP_DECIMAL),
            BIND_ENUM(KEY_KP_DIVIDE),
            BIND_ENUM(KEY_KP_MULTIPLY),
            BIND_ENUM(KEY_KP_SUBTRACT),
            BIND_ENUM(KEY_KP_ADD),
            BIND_ENUM(KEY_KP_ENTER),
            BIND_ENUM(KEY_KP_EQUAL),
            BIND_ENUM(KEY_BACK),
            BIND_ENUM(KEY_MENU),
            BIND_ENUM(KEY_VOLUME_UP),
            BIND_ENUM(KEY_VOLUME_DOWN)
        );

        // --- MouseButton Object ---
        export_object(engine, builder, "MouseButton",
            BIND_ENUM(MOUSE_BUTTON_LEFT),
            BIND_ENUM(MOUSE_BUTTON_RIGHT),
            BIND_ENUM(MOUSE_BUTTON_MIDDLE),
            BIND_ENUM(MOUSE_BUTTON_SIDE),
            BIND_ENUM(MOUSE_BUTTON_EXTRA),
            BIND_ENUM(MOUSE_BUTTON_FORWARD),
            BIND_ENUM(MOUSE_BUTTON_BACK)
        );

        // --- MouseCursor Object ---
        export_object(engine, builder, "MouseCursor",
            BIND_ENUM(MOUSE_CURSOR_DEFAULT),
            BIND_ENUM(MOUSE_CURSOR_ARROW),
            BIND_ENUM(MOUSE_CURSOR_IBEAM),
            BIND_ENUM(MOUSE_CURSOR_CROSSHAIR),
            BIND_ENUM(MOUSE_CURSOR_POINTING_HAND),
            BIND_ENUM(MOUSE_CURSOR_RESIZE_EW),
            BIND_ENUM(MOUSE_CURSOR_RESIZE_NS),
            BIND_ENUM(MOUSE_CURSOR_RESIZE_NWSE),
            BIND_ENUM(MOUSE_CURSOR_RESIZE_NESW),
            BIND_ENUM(MOUSE_CURSOR_RESIZE_ALL),
            BIND_ENUM(MOUSE_CURSOR_NOT_ALLOWED)
        );

        #undef BIND_ENUM
    }

    void register_module(qjspp::Engine& engine) {
        auto m = engine.new_module("vectorjs");
        register_hapi_enums(engine, m);
        register_color_class(engine, m);
        register_palette_object(engine, m);
        register_camera2d(engine, m);
        register_application_class(engine, m);
        register_font_class(engine, m);
        register_vector2(engine, m);
        register_rectangle(engine, m);
        register_sound_class(engine, m);
        register_music_class(engine, m);
        m.finalize();
    }

} // namespace VectorJS