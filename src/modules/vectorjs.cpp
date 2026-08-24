#include <iostream>
#include <optional>
#include <memory>
#include <array>
#include <fstream>
#include <raylib.h>
#include <qjspp.hpp>

#include "info.hpp"
#include "vectorjs.hpp"
#include "engine/engine.hpp"
#include "engine/commands.hpp"

namespace App::Modules {

#pragma region Application

    // =================================================================================================================
    // Application

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
        if (options_val.has("source")) {
            if (const auto* src = qjspp::get_native_opaque<JSRectangle>(options_val.get("source"))) {
                options.source = *src;
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

        #define BIND_DOUBLE_FN(js_name, cpp_fn) \
            update_obj.set(js_name, engine.make_function([&engine](const qjspp::ArgList&) -> qjspp::Value { \
                return engine.make_double(::cpp_fn()); \
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
            return engine.make_native_object(std::make_unique<JSVector2>(pos));
        }));

        // Time
        BIND_DOUBLE_FN("getDeltaTime", GetFrameTime);
        BIND_DOUBLE_FN("getTime", GetTime);
        BIND_INT_FN("getFPS", GetFPS);

        #undef BIND_VOID_FN 
        #undef BIND_BOOL_FN
        #undef BIND_INT_FN
        #undef BIND_INT_PARAM_BOOL_FN

        return update_obj;
    }

    static qjspp::Value create_shapes_object(const qjspp::Engine& engine, Hooray::CommandBufferBuilder& builder) {
        qjspp::Value shape_obj = engine.make_object();

        shape_obj.set("drawPixel", engine.make_function([&builder](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.size() < 2) throw std::runtime_error("drawPixel requires position and color arguments");
            const auto pos = qjspp::get_native_opaque<JSVector2>(args[0]);
            const auto* col = qjspp::get_native_opaque<JSColor>(args[1]);
            builder.draw_pixel(*pos, Color(col->r, col->g, col->b, col->a));
            return {};
        }));

        shape_obj.set("drawLine", engine.make_function([&builder](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.size() < 2) throw std::runtime_error("drawLine requires start and end positions");
            const auto* start = qjspp::get_native_opaque<JSVector2>(args[0]);
            const auto* end = qjspp::get_native_opaque<JSVector2>(args[1]);
            JSDrawOptions draw_options = parse_draw_options(args.size() > 2 ? args[2].clone() : qjspp::Value());
            builder.draw_line(*start, *end, draw_options.color);
            return {};
        }));

        shape_obj.set("drawRectangle", engine.make_function([&builder](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.empty()) throw std::runtime_error("drawRectangle requires a Rectangle argument");
            auto* rect = qjspp::get_native_opaque<JSRectangle>(args[0]);

            JSDrawOptions draw_options = parse_draw_options(args.size() > 1 ? args[1].clone() : qjspp::Value());

            if (rect) {
                builder.draw_rectangle(*rect, draw_options.color);
            }

            return {};
        }));

        shape_obj.set("drawCircle", engine.make_function([&builder](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.size() < 2) throw std::runtime_error("drawCircle requires center and radius arguments");
            const auto* center = qjspp::get_native_opaque<JSVector2>(args[0]);
            const auto rad = args[1].to_float();
            JSDrawOptions draw_options = parse_draw_options(args.size() > 2 ? args[2].clone() : qjspp::Value());
            builder.draw_circle(*center, rad, draw_options.color);
            return {};
        }));

        shape_obj.set("drawTriangle", engine.make_function([&builder](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.size() < 3) throw std::runtime_error("drawTriangle requires 3 Vector2 point arguments");
            auto* p1 = qjspp::get_native_opaque<JSVector2>(args[0]);
            auto* p2 = qjspp::get_native_opaque<JSVector2>(args[1]);
            auto* p3 = qjspp::get_native_opaque<JSVector2>(args[2]);
            JSDrawOptions draw_options = parse_draw_options(args.size() > 3 ? args[3].clone() : qjspp::Value());
            builder.draw_triangle(*p1, *p2, *p3, draw_options.color);
            return {};
        }));

        shape_obj.set("drawEllipse", engine.make_function([&builder](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.size() < 3) throw std::runtime_error("drawEllipse requires center, radiusH, and radiusV arguments");
            const auto* center = qjspp::get_native_opaque<JSVector2>(args[0]);
            const auto radH = args[1].to_float();
            const auto radV = args[2].to_float();
            JSDrawOptions draw_options = parse_draw_options(args.size() > 3 ? args[3].clone() : qjspp::Value());
            builder.draw_ellipse(*center, radH, radV, draw_options.color);
            return {};
        }));

        return shape_obj;
    }

    static qjspp::Value create_text_object(const qjspp::Engine& engine, Hooray::CommandBufferBuilder& builder) {
        qjspp::Value text_obj = engine.make_object();

        text_obj.set("drawText", engine.make_function([&builder](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.size() < 2) throw std::runtime_error("drawText requires position and text string arguments");
            auto* pos = qjspp::get_native_opaque<JSVector2>(args[0]);
            const std::string txt_str = args[1].to_string();

            JSTextOptions options = parse_text_options(args.size() > 2 ? args[2].clone() : qjspp::Value());
            Font fontToUse = GetFontDefault();
            if (options.font.font_ptr && options.font.font_ptr->texture.id != 0) {
                fontToUse = *options.font.font_ptr;
            }

            builder.draw_text(txt_str.c_str(), *pos, fontToUse, options.fontSize, options.color);

            return {};
        }));

        return text_obj;
    }

    static qjspp::Value create_viewport2d_function(const qjspp::Engine& engine, const qjspp::Value& r2d_val, Hooray::CommandBufferBuilder& builder) {
        auto r2d_ptr = std::make_shared<qjspp::Value>(r2d_val.clone());

        return engine.make_function([r2d_ptr, &builder](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.empty()) throw std::runtime_error("withViewport2D requires at least a callback function");
            if (const auto* cam_ptr = qjspp::get_native_opaque<JSCamera2D>(args[0])) {
                const qjspp::Value callback = args[1].clone();
                builder.start_mode_2d(*cam_ptr);
                std::ignore = callback.call({r2d_ptr->clone()});
                builder.end_mode_2d();
            }
            return {};
        });
    }

    static qjspp::Value create_screen_space_function(const qjspp::Engine& engine, const qjspp::Value& r2d_val, Hooray::CommandBufferBuilder& builder) {
        auto r2d_ptr = std::make_shared<qjspp::Value>(r2d_val.clone());
        return engine.make_function([r2d_ptr, &builder](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.empty()) throw std::runtime_error("withScreenSpace requires a callback function");
            std::ignore = args[0].call({r2d_ptr->clone()});
            return {};
        });
    }

    static qjspp::Value create_draw_render_object(const qjspp::Engine& engine, Hooray::CommandBufferBuilder& builder) {
        qjspp::Value render2d_obj = engine.make_object();

        // 1. Full Texture Drawing: render.drawTexture(texture, destRectangle, [options])
        render2d_obj.set("drawTexture", engine.make_function([&builder](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.size() < 2) throw std::runtime_error("drawTexture requires a Texture and a destination Rectangle");

            auto* tex = qjspp::get_native_opaque<JSTexture>(args[0]);
            auto* dest = qjspp::get_native_opaque<JSRectangle>(args[1]);
            if (!tex || !tex->texture_ptr || tex->texture_ptr->id == 0 || !dest) return {};

            JSDrawOptions options = parse_draw_options(args.size() > 2 ? args[2].clone() : qjspp::Value());

            // Default source is whole image unless explicitly provided
            Rectangle src_rect = (options.source.width > 0 && options.source.height > 0)
                ? static_cast<Rectangle>(options.source)
                : Rectangle{ 0.0f, 0.0f, static_cast<float>(tex->get_width()), static_cast<float>(tex->get_height()) };

            builder.draw_texture_pro(*(tex->texture_ptr), src_rect, *dest, options.origin, options.rotation, options.color);

            return {};
        }));

        // 2. Sprite Sheet Drawing: render.drawSprite(texture, sourceRect, destRect, [options])
        render2d_obj.set("drawSprite", engine.make_function([&builder](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.size() < 3) throw std::runtime_error("drawSprite requires Texture, source Rectangle, and destination Rectangle");

            auto* tex = qjspp::get_native_opaque<JSTexture>(args[0]);
            auto* src = qjspp::get_native_opaque<JSRectangle>(args[1]);
            auto* dest = qjspp::get_native_opaque<JSRectangle>(args[2]);
            if (!tex || !tex->texture_ptr || tex->texture_ptr->id == 0 || !src || !dest) return {};

            JSDrawOptions options = parse_draw_options(args.size() > 3 ? args[3].clone() : qjspp::Value());

            builder.draw_texture_pro(*(tex->texture_ptr), *src, *dest, options.origin, options.rotation, options.color);
            return {};
        }));

        // Add FPS
        render2d_obj.set("drawFPS", engine.make_function([&builder](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.empty()) throw std::runtime_error("drawFPS requires a Vector2 position argument");
            if (auto pos = qjspp::get_native_opaque<JSVector2>(args[0])) {
                builder.draw_fps(*pos);
            }
            return {};
        }));

        // Add Sub Objects
        render2d_obj.set("shapes", create_shapes_object(engine, builder));
        render2d_obj.set("text", create_text_object(engine, builder));

        // Layer Wrappers
        qjspp::Value render_obj = engine.make_object();
        render_obj.set("withViewport2D", create_viewport2d_function(engine, render2d_obj, builder));
        render_obj.set("withScreenSpace", create_screen_space_function(engine, render2d_obj, builder));

        // ClearBackground
        render_obj.set("clearBackground", engine.make_function([&builder](const qjspp::ArgList& args) -> qjspp::Value {
            auto* color = args.empty() ? nullptr : qjspp::get_native_opaque<JSColor>(args[0]);
            builder.clear_background(color ? static_cast<::Color>(*color) : BLACK);
            return {};
        }));

        return render_obj;
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

        cls.instance_method("run", [&engine](JSApplication* app, const qjspp::ArgList& args) -> qjspp::Value {
            if (!app) return {};

            qjspp::Value user_app = args[0].clone();

            // Setup onInit callback
            if (const qjspp::Value on_init_func = user_app.get("onInit"); on_init_func.is_function()) {
                auto func_ptr = std::make_shared<qjspp::Value>(on_init_func.clone());
                auto app_ptr = std::make_shared<qjspp::Value>(user_app.clone());
                app->rengine.set_on_init([func_ptr, app_ptr]() {
                    std::ignore = func_ptr->call_method(*app_ptr, {});
                });
            }

            // Setup onUpdate callback
            if (const qjspp::Value on_update_func = user_app.get("onUpdate"); on_update_func.is_function()) {
                auto func_ptr = std::make_shared<qjspp::Value>(on_update_func.clone());
                auto app_ptr = std::make_shared<qjspp::Value>(user_app.clone());
                auto update_obj_ptr = std::make_shared<qjspp::Value>(create_update_context_object(engine));
                app->rengine.set_on_update([func_ptr, app_ptr, update_obj_ptr](float delta) {
                    std::ignore = func_ptr->call_method(*app_ptr, { update_obj_ptr->clone() });
                });
            }

            // Setup onDraw callback
            if (const qjspp::Value on_draw_func = user_app.get("onDraw"); on_draw_func.is_function()) {
                auto func_ptr = std::make_shared<qjspp::Value>(on_draw_func.clone());
                auto app_ptr = std::make_shared<qjspp::Value>(user_app.clone());
                auto render_obj_ptr = std::make_shared<qjspp::Value>(create_draw_render_object(engine, app->rengine.get_buffer()));
                app->rengine.set_on_draw([func_ptr, app_ptr, render_obj_ptr]() {
                    std::ignore = func_ptr->call_method(*app_ptr, { render_obj_ptr->clone() });
                });
            }

            app->rengine.run();

            return {};
        });

        builder.export_class("Application", cls.build());
    }

#pragma endregion

#pragma region Audio

    // =================================================================================================================
    // Audio

    static void register_sound_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto sound = engine.make_class<JSSound>("Sound");

        // Constructor: const mySound = new vectorjs.Sound("assets/sfx.wav");
        sound.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSSound> {
            if (args.empty()) return nullptr;
            return std::make_unique<JSSound>(args[0].to_string());
        });

        // Instance Method: mySound.play()
        sound.instance_method("play", [&engine](JSSound* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->sound_ptr) {
                PlaySound(*(self->sound_ptr));
            }
            return engine.make_undefined();
        });

        // Instance Method: mySound.pause()
        sound.instance_method("pause", [&engine](JSSound* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->sound_ptr) {
                PauseSound(*(self->sound_ptr));
            }
            return engine.make_undefined();
        });

        // Instance Method: mySound.resume()
        sound.instance_method("resume", [&engine](JSSound* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->sound_ptr) {
                ResumeSound(*(self->sound_ptr));
            }
            return engine.make_undefined();
        });

        // Instance Method: mySound.stop()
        sound.instance_method("stop", [&engine](JSSound* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->sound_ptr) {
                StopSound(*(self->sound_ptr));
            }
            return engine.make_undefined();
        });

        // Instance Method: mySound.setVolume(0.5)
        sound.instance_method("setVolume", [&engine](JSSound* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->sound_ptr && !args.empty()) {
                SetSoundVolume(*(self->sound_ptr), static_cast<float>(args[0].to_double()));
            }
            return engine.make_undefined();
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
        music.instance_method("play", [&engine](JSMusic* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->music_ptr) {
                PlayMusicStream(*(self->music_ptr));
            }
            return engine.make_undefined();
        });

        // Instance Method: myMusic.pause()
        music.instance_method("pause", [&engine](JSMusic* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->music_ptr) {
                PauseMusicStream(*(self->music_ptr));
            }
            return engine.make_undefined();
        });

        // Instance Method: myMusic.resume()
        music.instance_method("resume", [&engine](JSMusic* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->music_ptr) {
                ResumeMusicStream(*(self->music_ptr));
            }
            return engine.make_undefined();
        });

        // Instance Method: myMusic.stop()
        music.instance_method("stop", [&engine](JSMusic* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->music_ptr) {
                StopMusicStream(*(self->music_ptr));
            }
            return engine.make_undefined();
        });

        // Instance Method: myMusic.update()
        music.instance_method("update", [&engine](JSMusic* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->music_ptr) {
                UpdateMusicStream(*(self->music_ptr));
            }
            return engine.make_undefined();
        });

        // Instance Method: myMusic.setVolume(0.5)
        music.instance_method("setVolume", [&engine](JSMusic* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->music_ptr && !args.empty()) {
                SetMusicVolume(*(self->music_ptr), static_cast<float>(args[0].to_double()));
            }
            return engine.make_undefined();
        });

        builder.export_class("Music", music.build());
    }

#pragma endregion

#pragma region Color

    // =================================================================================================================
    // Color

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
            qjspp::Value color_val = engine.make_native_object<JSColor>(std::move(color_ptr));
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

        color.instance_method("lerp", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.size() < 2) return {};
            const auto* target = qjspp::get_native_opaque<JSColor>(args[0]);
            self->apply_lerp(*target, args[1].to_float());
            return args.get_this();
        });

        color.instance_method("fade", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            self->apply_fade(args[0].to_float());
            return args.get_this();
        });

        color.instance_method("brightness", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            self->apply_brightness(args[0].to_float());
            return args.get_this();
        });

        color.instance_method("contrast", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            self->apply_contrast(args[0].to_float());
            return args.get_this();
        });

        color.instance_method("alpha", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            self->apply_alpha(args[0].to_float());
            return args.get_this();
        });

        color.instance_method("tint", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            const auto* tintColor = qjspp::get_native_opaque<JSColor>(args[0]);
            if (!tintColor) return {};
            self->apply_tint(*tintColor);
            return args.get_this();
        });

        color.instance_method("toInt", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self) return {};
            return qjspp::Value::make_int(args[0].context(), self->to_int());
        });

        color.instance_method("isEqual", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return qjspp::Value::make_bool(args[0].context(), false);
            const auto* other = qjspp::get_native_opaque<JSColor>(args[0]);
            const bool equal = other && self->is_equal(*other);
            return qjspp::Value::make_bool(args[0].context(), equal);
        });

        color.instance_method("clone", [&engine](JSColor* self, const qjspp::ArgList&) -> qjspp::Value {
            if (!self) return {};
            return engine.make_native_object(std::make_unique<JSColor>(self->clone()));
        });

        builder.export_class("Color", color.build());
    }

#pragma endregion

#pragma region File

    // =================================================================================================================
    // File

    JSFile::JSFile(std::filesystem::path path)
        : m_path(std::move(path)) {}

    FileError JSFile::map_error_code(const std::error_code& ec) noexcept {
        if (ec == std::errc::no_such_file_or_directory) return FileError::NotFound;
        if (ec == std::errc::permission_denied) return FileError::AccessDenied;
        if (ec == std::errc::file_exists) return FileError::AlreadyExists;
        if (ec == std::errc::io_error) return FileError::ReadError;
        return FileError::Unknown;
    }

    JSFile::ResultString JSFile::read_text(const std::filesystem::path& path) {
        std::error_code ec;
        if (!std::filesystem::exists(path, ec)) {
            return std::unexpected(FileError::NotFound);
        }

        std::ifstream file(path, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            return std::unexpected(FileError::AccessDenied);
        }

        const auto size = std::filesystem::file_size(path, ec);
        if (ec) return std::unexpected(map_error_code(ec));

        std::string content(size, '\0');
        if (!file.read(content.data(), static_cast<std::streamsize>(size))) {
            return std::unexpected(FileError::ReadError);
        }

        return content;
    }

    JSFile::ResultBytes JSFile::read_bytes(const std::filesystem::path& path) {
        std::error_code ec;
        if (!std::filesystem::exists(path, ec)) {
            return std::unexpected(FileError::NotFound);
        }

        std::ifstream file(path, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            return std::unexpected(FileError::AccessDenied);
        }

        const auto size = std::filesystem::file_size(path, ec);
        if (ec) return std::unexpected(map_error_code(ec));

        std::vector<std::byte> buffer(size);
        if (!file.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(size))) {
            return std::unexpected(FileError::ReadError);
        }

        return buffer;
    }

    JSFile::ResultBool JSFile::write_text(const std::filesystem::path& path, std::string_view content) {
        std::ofstream file(path, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!file.is_open()) {
            return std::unexpected(FileError::AccessDenied);
        }

        if (!file.write(content.data(), static_cast<std::streamsize>(content.size()))) {
            return std::unexpected(FileError::WriteError);
        }

        return true;
    }

    JSFile::ResultBool JSFile::write_bytes(const std::filesystem::path& path, std::span<const std::byte> data) {
        std::ofstream file(path, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!file.is_open()) {
            return std::unexpected(FileError::AccessDenied);
        }

        if (!file.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()))) {
            return std::unexpected(FileError::WriteError);
        }

        return true;
    }

    JSFile::ResultBool JSFile::append_text(const std::filesystem::path& path, std::string_view content) {
        std::ofstream file(path, std::ios::out | std::ios::binary | std::ios::app);
        if (!file.is_open()) {
            return std::unexpected(FileError::AccessDenied);
        }

        if (!file.write(content.data(), static_cast<std::streamsize>(content.size()))) {
            return std::unexpected(FileError::WriteError);
        }

        return true;
    }

    bool JSFile::exists(const std::filesystem::path& path) noexcept {
        std::error_code ec;
        return std::filesystem::exists(path, ec);
    }

    JSFile::ResultBool JSFile::remove(const std::filesystem::path& path) noexcept {
        std::error_code ec;
        bool removed = std::filesystem::remove(path, ec);
        if (ec) {
            return std::unexpected(map_error_code(ec));
        }
        return removed;
    }

    void register_file_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto cls = engine.make_class<JSFile>("File");

        cls.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSFile> {
            if (args.empty()) return std::make_unique<JSFile>();
            return std::make_unique<JSFile>(args[0].to_string());
        });

        // Instance Methods
        cls.instance_method("readText", [&engine](const JSFile* self, const qjspp::ArgList&) -> qjspp::Value {
            if (!self) return {};
            auto res = JSFile::read_text(self->get_path());
            if (!res) throw std::runtime_error("Failed to read file");
            return engine.make_string(*res);
        });

        cls.instance_method("writeText", [&engine](JSFile* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            auto res = JSFile::write_text(self->get_path(), args[0].to_string());
            if (!res) throw std::runtime_error("Failed to write to file");
            return engine.make_bool(true);
        });

        cls.instance_method("appendText", [&engine](JSFile* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            auto res = JSFile::append_text(self->get_path(), args[0].to_string());
            if (!res) throw std::runtime_error("Failed to append to file");
            return engine.make_bool(true);
        });

        cls.instance_method("exists", [&engine](const JSFile* self, const qjspp::ArgList&) -> qjspp::Value {
            if (!self) return engine.make_bool(false);
            return engine.make_bool(JSFile::exists(self->get_path()));
        });

        cls.instance_method("remove", [&engine](const JSFile* self, const qjspp::ArgList&) -> qjspp::Value {
            if (!self) return engine.make_bool(false);
            auto res = JSFile::remove(self->get_path());
            return engine.make_bool(res.value_or(false));
        });

        cls.static_method("exists", [&engine](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.empty()) return engine.make_bool(false);
            return engine.make_bool(JSFile::exists(args[0].to_string()));
        });

        builder.export_class("File", cls.build());
    }

#pragma endregion

#pragma region Geometry

    // =================================================================================================================
    // Geometry

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

        rectangle.instance_method("contains", [&engine](JSRectangle* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return engine.make_bool(false);
            auto* point = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!point) return engine.make_bool(false);
            return engine.make_bool(self->contains(*point));
        });

        rectangle.instance_method("overlaps", [&engine](JSRectangle* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return engine.make_bool(false);
            auto* rec2 = qjspp::get_native_opaque<JSRectangle>(args[0]);
            if (!rec2) return engine.make_bool(false);
            return engine.make_bool(self->overlaps(*rec2));
        });

        rectangle.instance_method("getCollisionRect", [&engine](JSRectangle* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            auto* rec2 = qjspp::get_native_opaque<JSRectangle>(args[0]);
            if (!rec2) return {};
            return engine.make_native_object(std::make_unique<JSRectangle>(self->get_collision_rect(*rec2)));
        });

        builder.export_class("Rectangle", rectangle.build());
    }

    static void register_circle(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto circle = engine.make_class<JSCircle>("Circle");

        // Constructor: new Circle(x, y, radius)
        circle.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSCircle> {
            if (args.size() < 3) return nullptr;
            return std::make_unique<JSCircle>(JSCircle{
                Vector2{ static_cast<float>(args[0].to_double()), static_cast<float>(args[1].to_double()) },
                static_cast<float>(args[2].to_double())
            });
        });

        // Properties: x, y, radius
        circle.property("x",
            [](JSContext* ctx, JSCircle* self) { return qjspp::Value::make_double(ctx, self->center.x); },
            [](JSCircle* self, const qjspp::Value& val) { self->center.x = static_cast<float>(val.to_double()); }
        );

        circle.property("y",
            [](JSContext* ctx, JSCircle* self) { return qjspp::Value::make_double(ctx, self->center.y); },
            [](JSCircle* self, const qjspp::Value& val) { self->center.y = static_cast<float>(val.to_double()); }
        );

        circle.property("radius",
            [](JSContext* ctx, JSCircle* self) { return qjspp::Value::make_double(ctx, self->radius); },
            [](JSCircle* self, const qjspp::Value& val) { self->radius = static_cast<float>(val.to_double()); }
        );

        // Collision: Check if a Vector2 point is inside this circle
        circle.instance_method("contains", [&engine](JSCircle* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return engine.make_bool(false);
            auto point = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!point) return engine.make_bool(false);
            return engine.make_bool(self->contains(*point));
        });

        // Collision: Check overlap with another Circle or a Rectangle
        circle.instance_method("overlaps", [&engine](JSCircle* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return engine.make_bool(false);

            if (const auto otherCircle = qjspp::get_native_opaque<JSCircle>(args[0])) {
                return engine.make_bool(self->overlaps(*otherCircle));
            }

            if (const auto rec = qjspp::get_native_opaque<JSRectangle>(args[0])) {
                return engine.make_bool(self->overlaps(*rec));
            }

            return engine.make_bool(false);
        });

        builder.export_class("Circle", circle.build());
    }

    static void register_triangle(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto triangle = engine.make_class<JSTriangle>("Triangle");

        // Constructor: new Triangle(v1, v2, v3) expecting Vector2 objects
        triangle.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSTriangle> {
            if (args.size() < 3) return nullptr;
            auto* p1 = qjspp::get_native_opaque<JSVector2>(args[0]);
            auto* p2 = qjspp::get_native_opaque<JSVector2>(args[1]);
            auto* p3 = qjspp::get_native_opaque<JSVector2>(args[2]);
            if (!p1 || !p2 || !p3) return nullptr;
            return std::make_unique<JSTriangle>(JSTriangle{ *p1, *p2, *p3 });
        });

        // Properties: v1, v2, v3
        triangle.property("v1",
            [&engine](JSContext* ctx, JSTriangle* self) {
                return engine.make_native_object(std::make_unique<JSVector2>(self->v1));
            },
            [](JSTriangle* self, const qjspp::Value& val) {
                if (auto* v = qjspp::get_native_opaque<JSVector2>(val)) self->v1 = *v;
            }
        );

        triangle.property("v2",
            [&engine](JSContext* ctx, JSTriangle* self) {
                return engine.make_native_object(std::make_unique<JSVector2>(self->v2));
            },
            [](JSTriangle* self, const qjspp::Value& val) {
                if (auto* v = qjspp::get_native_opaque<JSVector2>(val)) self->v2 = *v;
            }
        );

        triangle.property("v3",
            [&engine](JSContext* ctx, JSTriangle* self) {
                return engine.make_native_object(std::make_unique<JSVector2>(self->v3));
            },
            [](JSTriangle* self, const qjspp::Value& val) {
                if (auto* v = qjspp::get_native_opaque<JSVector2>(val)) self->v3 = *v;
            }
        );

        // Collision: Check if a Vector2 point is inside this triangle
        triangle.instance_method("contains", [&engine](JSTriangle* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return engine.make_bool(false);
            auto* point = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!point) return engine.make_bool(false);
            return engine.make_bool(CheckCollisionPointTriangle(*point, self->v1, self->v2, self->v3));
        });

        builder.export_class("Triangle", triangle.build());
    }

    static void register_point(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto point = engine.make_class<JSPoint>("Point");

        // Constructor: new Point(x, y)
        point.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSPoint> {
            if (args.size() < 2) return nullptr;
            return std::make_unique<JSPoint>(JSPoint{
                Vector2{ static_cast<float>(args[0].to_double()), static_cast<float>(args[1].to_double()) }
            });
        });

        // Properties: x, y
        point.property("x",
            [](JSContext* ctx, JSPoint* self) { return qjspp::Value::make_double(ctx, self->position.x); },
            [](JSPoint* self, const qjspp::Value& val) { self->position.x = static_cast<float>(val.to_double()); }
        );

        point.property("y",
            [](JSContext* ctx, JSPoint* self) { return qjspp::Value::make_double(ctx, self->position.y); },
            [](JSPoint* self, const qjspp::Value& val) { self->position.y = static_cast<float>(val.to_double()); }
        );

        // Collision: Check if this point lies inside Rectangle, Circle, or Triangle
        point.instance_method("intersects", [&engine](JSPoint* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return engine.make_bool(false);

            if (auto* rec = qjspp::get_native_opaque<JSRectangle>(args[0])) {
                return engine.make_bool(CheckCollisionPointRec(self->position, *rec));
            }

            if (auto* circle = qjspp::get_native_opaque<JSCircle>(args[0])) {
                return engine.make_bool(CheckCollisionPointCircle(self->position, circle->center, circle->radius));
            }

            if (auto* tri = qjspp::get_native_opaque<JSTriangle>(args[0])) {
                return engine.make_bool(CheckCollisionPointTriangle(self->position, tri->v1, tri->v2, tri->v3));
            }

            return engine.make_bool(false);
        });

        builder.export_class("Point", point.build());
    }

    static void register_line(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto line = engine.make_class<JSLine>("Line");

        // Constructor: new Line(p1, p2) expecting Vector2 objects
        line.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSLine> {
            if (args.size() < 2) return nullptr;
            auto* p1 = qjspp::get_native_opaque<JSVector2>(args[0]);
            auto* p2 = qjspp::get_native_opaque<JSVector2>(args[1]);
            if (!p1 || !p2) return nullptr;
            return std::make_unique<JSLine>(JSLine{ *p1, *p2 });
        });

        // Properties: start, end
        line.property("start",
            [&engine](JSContext* ctx, JSLine* self) {
                return engine.make_native_object(std::make_unique<JSVector2>(self->start));
            },
            [](JSLine* self, const qjspp::Value& val) {
                if (auto* v = qjspp::get_native_opaque<JSVector2>(val)) self->start = *v;
            }
        );

        line.property("end",
            [&engine](JSContext* ctx, JSLine* self) {
                return engine.make_native_object(std::make_unique<JSVector2>(self->end));
            },
            [](JSLine* self, const qjspp::Value& val) {
                if (auto* v = qjspp::get_native_opaque<JSVector2>(val)) self->end = *v;
            }
        );

        // Collision: Check intersection with another Line segment
        line.instance_method("intersects", [&engine](JSLine* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return engine.make_bool(false);

            auto* otherLine = qjspp::get_native_opaque<JSLine>(args[0]);
            if (!otherLine) return engine.make_bool(false);

            Vector2 collisionPoint = { 0 };
            bool hasCollision = CheckCollisionLines(
                self->start, self->end,
                otherLine->start, otherLine->end,
                &collisionPoint
            );

            return engine.make_bool(hasCollision);
        });

        // Get collision intersection point with another Line (returns Vector2 or null)
        line.instance_method("getIntersection", [&engine](JSLine* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};

            auto* otherLine = qjspp::get_native_opaque<JSLine>(args[0]);
            if (!otherLine) return {};

            Vector2 collisionPoint = { 0 };
            if (CheckCollisionLines(self->start, self->end, otherLine->start, otherLine->end, &collisionPoint)) {
                return engine.make_native_object(std::make_unique<JSVector2>(collisionPoint));
            }

            return {};
        });

        builder.export_class("Line", line.build());
    }

    static void register_polygon(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto polygon = engine.make_class<JSPolygon>("Polygon");

        polygon.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSPolygon> {

            if (args.empty()) return nullptr;

            std::vector<Vector2> pts;
            if (args[0].is_array()) {
                for (const auto& elem : args[0].to_vector()) {
                    if (auto* v = qjspp::get_native_opaque<JSVector2>(elem)) {
                        pts.push_back(*v);
                    }
                }
            }

            if (pts.size() < 3) return nullptr;
            return std::make_unique<JSPolygon>(JSPolygon{ std::move(pts) });
        });

        // Property: points count
        polygon.property("vertexCount",
            [](JSContext* ctx, JSPolygon* self) {
                return qjspp::Value::make_int(ctx, static_cast<int32_t>(self->points.size()));
            },
            nullptr
        );

        // Method: getPoint(index) -> Vector2
        polygon.instance_method("getPoint", [&engine](JSPolygon* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            int index = args[0].to_int();
            if (index < 0 || index >= static_cast<int>(self->points.size())) return {};
            return engine.make_native_object(std::make_unique<JSVector2>(self->points[index]));
        });

        // Collision: Point vs Polygon check using Raylib's native function
        polygon.instance_method("contains", [&engine](JSPolygon* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return engine.make_bool(false);

            auto* point = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!point) return engine.make_bool(false);

            // Raylib's CheckCollisionPointPoly takes (Vector2 point, const Vector2 *points, int pointCount)
            bool collision = CheckCollisionPointPoly(*point, self->points.data(), static_cast<int>(self->points.size()));
            return engine.make_bool(collision);
        });

        builder.export_class("Polygon", polygon.build());
    }

#pragma endregion

#pragma region Graphics

    static void register_texture_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto texture = engine.make_class<JSTexture>("Texture");

        // Constructor: const tex = new vectorjs.Texture("assets/hero.png");
        texture.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSTexture> {
            if (args.empty()) return nullptr;
            return std::make_unique<JSTexture>(args[0].to_string());
        });

        // Read-only properties
        texture.property("width",
            [](JSContext* ctx, JSTexture* self) { return qjspp::Value::make_int(ctx, self ? self->get_width() : 0); },
            nullptr
        );

        texture.property("height",
            [](JSContext* ctx, JSTexture* self) { return qjspp::Value::make_int(ctx, self ? self->get_height() : 0); },
            nullptr
        );

        builder.export_class("Texture", texture.build());
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
        camera.instance_method("toWorldSpace", [&engine](JSCamera2D* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            auto* screen_pos = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!screen_pos) return {};

            Vector2 world_pos = GetScreenToWorld2D(*screen_pos, *self);
            return engine.make_native_object(std::make_unique<JSVector2>(world_pos));
        });

        // Convert world space position to screen space
        camera.instance_method("toScreenSpace", [&engine](JSCamera2D* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            auto* world_pos = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!world_pos) return {};

            Vector2 screen_pos = GetWorldToScreen2D(*world_pos, *self);
            return engine.make_native_object(std::make_unique<JSVector2>(screen_pos));
        });

        // =========================================================================
        // Properties
        // =========================================================================

        camera.property("target",
            [&engine](JSContext* ctx, JSCamera2D* self) {
                return engine.make_native_object(std::make_unique<JSVector2>(self->target));
            },
            [](JSCamera2D* self, const qjspp::Value& val) {
                auto* vec = qjspp::get_native_opaque<JSVector2>(val);
                if (vec) self->target = *vec;
            }
        );

        camera.property("offset",
            [&engine](JSContext* ctx, JSCamera2D* self) {
                return engine.make_native_object(std::make_unique<JSVector2>(self->offset));
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

#pragma endregion

#pragma region Input

    // =================================================================================================================
    // Input

    static void register_vjs_enums(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {

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

#pragma endregion

#pragma region Math

    static void register_vector2(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto vector2 = engine.make_class<JSVector2>("Vector2");

        vector2.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSVector2> {
            if (args.size() < 2) return nullptr;
            return std::make_unique<JSVector2>(static_cast<float>(args[0].to_double()), static_cast<float>(args[1].to_double()));
        });

        // Properties
        vector2.property("x",
            [](JSContext* ctx, JSVector2* self) { return qjspp::Value::make_double(ctx, self->x); },
            [](JSVector2* self, const qjspp::Value& val) { self->x = static_cast<float>(val.to_double()); }
        );

        vector2.property("y",
            [](JSContext* ctx, JSVector2* self) { return qjspp::Value::make_double(ctx, self->y); },
            [](JSVector2* self, const qjspp::Value& val) { self->y = static_cast<float>(val.to_double()); }
        );

        vector2.instance_method("add", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            if (const auto* other = qjspp::get_native_opaque<JSVector2>(args[0])) {
                self->apply_add(*other);
            }
            return args.get_this();
        });

        vector2.instance_method("subtract", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            if (const auto* other = qjspp::get_native_opaque<JSVector2>(args[0])) {
                self->apply_subtract(*other);
            }
            return args.get_this();
        });

        vector2.instance_method("multiply", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            if (const auto* other = qjspp::get_native_opaque<JSVector2>(args[0])) {
                self->apply_multiply(*other);
            }
            return args.get_this();
        });

        vector2.instance_method("scale", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            self->apply_scale(args[0].to_float());
            return args.get_this();
        });

        vector2.instance_method("normalize", [](JSVector2* self, const qjspp::ArgList&) -> qjspp::Value {
            if (!self) return {};
            self->apply_normalize();
            return {};
        });

        vector2.instance_method("negate", [](JSVector2* self, const qjspp::ArgList&) -> qjspp::Value {
            if (!self) return {};
            self->apply_negate();
            return {};
        });

        vector2.instance_method("lerp", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.size() < 2) return {};
            if (const auto* target = qjspp::get_native_opaque<JSVector2>(args[0])) {
                self->apply_lerp(*target, args[1].to_float());
            }
            return args.get_this();
        });

        // --- Scalar / Utility Operations ---
        vector2.instance_method("getLength", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self) return {};
            return qjspp::Value::make_double(args[0].context(), self->length());
        });

        vector2.instance_method("dot", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            const auto* other = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!other) return {};
            return qjspp::Value::make_double(args[0].context(), self->dot(*other));
        });

        vector2.instance_method("distance", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            const auto* other = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!other) return {};
            return qjspp::Value::make_double(args[0].context(), self->distance(*other));
        });

        vector2.instance_method("isEqual", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return qjspp::Value::make_bool(args[0].context(), false);
            const auto* other = qjspp::get_native_opaque<JSVector2>(args[0]);
            return qjspp::Value::make_bool(args[0].context(), other && self->is_equal(*other));
        });

        builder.export_class("Vector2", vector2.build());
    }

#pragma endregion

    void register_vectorjs_module(qjspp::Engine& engine) {
        auto mod = engine.new_module("vectorjs");

        // Core
        register_application_class(engine, mod);
        register_file_class(engine, mod);
        register_color_class(engine, mod);
        register_palette_object(engine, mod);

        // Audio
        register_sound_class(engine, mod);
        register_music_class(engine, mod);

        // Math
        register_vector2(engine, mod);

        // Graphics2d
        register_texture_class(engine, mod);
        register_camera2d(engine, mod);
        register_font_class(engine, mod);

        // Input
        register_vjs_enums(engine, mod);

        // Geometry
        register_rectangle(engine, mod);
        register_circle(engine, mod);
        register_triangle(engine, mod);
        register_polygon(engine, mod);
        register_line(engine, mod);
        register_point(engine, mod);

        mod.finalize();

    }

} // namespace VectorJS