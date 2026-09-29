#include <stdexcept>
#include <vector>
#include <raylib.h>
#include <qjspp.hpp>

namespace App::Modules {

    static qjspp::Value Vector2ToValue(const qjspp::Engine &js_engine, const Vector2& val) {
        auto obj = js_engine.make_object();
        obj.set("x", js_engine.make_double(val.x));
        obj.set("y", js_engine.make_double(val.y));
        return obj;
    }

    static qjspp::Value ColorToValue(const qjspp::Engine &js_engine, const Color& val) {
        auto obj = js_engine.make_object();
        obj.set("r", js_engine.make_int(val.r));
        obj.set("g", js_engine.make_int(val.g));
        obj.set("b", js_engine.make_int(val.b));
        obj.set("a", js_engine.make_int(val.a));
        return obj;
    }

    static Color ValueToColor(const qjspp::Value& val) {

        Color out = {
            .r = 0,
            .g = 0,
            .b = 0,
            .a = 255
        };

        if (!val.is_object()) return out;

        if (const auto r = val.get("r"); !r.is_undefined() && r.is_number()) {
            out.r = static_cast<unsigned char>(r.to_int());
        }

        if (const auto g = val.get("g"); !g.is_undefined() && g.is_number()) {
            out.g = static_cast<unsigned char>(g.to_int());
        }

        if (const auto b = val.get("b"); !b.is_undefined() && b.is_number()) {
            out.b = static_cast<unsigned char>(b.to_int());
        }

        if (const auto a = val.get("a"); !a.is_undefined() && a.is_number()) {
            out.a = static_cast<unsigned char>(a.to_int());
        }

        return out;
    }

    static void register_core_functions(qjspp::ModuleBuilder& mod, qjspp::Engine& js_engine) {

        mod.export_function("initWindow", [&js_engine](const qjspp::ArgList& args) {
            InitWindow(args[0].to_int(), args[1].to_int(), args[2].to_string().c_str());
            return js_engine.make_undefined();
        });

        mod.export_function("closeWindow", [&js_engine](const qjspp::ArgList&) {
            CloseWindow();
            return js_engine.make_undefined();
        });

        mod.export_function("windowShouldClose", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_bool(WindowShouldClose());
        });

        mod.export_function("isWindowReady", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_bool(IsWindowReady());
        });

        mod.export_function("isWindowFullscreen", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_bool(IsWindowFullscreen());
        });

        mod.export_function("isWindowHidden", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_bool(IsWindowHidden());
        });

        mod.export_function("isWindowMinimized", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_bool(IsWindowMinimized());
        });

        mod.export_function("isWindowMaximized", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_bool(IsWindowMaximized());
        });

        mod.export_function("isWindowFocused", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_bool(IsWindowFocused());
        });

        mod.export_function("isWindowResized", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_bool(IsWindowResized());
        });

        mod.export_function("isWindowState", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(IsWindowState(static_cast<unsigned int>(args[0].to_int())));
        });

        mod.export_function("setWindowState", [&js_engine](const qjspp::ArgList& args) {
            SetWindowState(static_cast<unsigned int>(args[0].to_int()));
            return js_engine.make_undefined();
        });

        mod.export_function("clearWindowState", [&js_engine](const qjspp::ArgList& args) {
            ClearWindowState(static_cast<unsigned int>(args[0].to_int()));
            return js_engine.make_undefined();
        });

        mod.export_function("toggleFullscreen", [&js_engine](const qjspp::ArgList&) {
            ToggleFullscreen();
            return js_engine.make_undefined();
        });

        mod.export_function("toggleBorderlessWindowed", [&js_engine](const qjspp::ArgList&) {
            ToggleBorderlessWindowed();
            return js_engine.make_undefined();
        });

        mod.export_function("maximizeWindow", [&js_engine](const qjspp::ArgList&) {
            MaximizeWindow();
            return js_engine.make_undefined();
        });

        mod.export_function("minimizeWindow", [&js_engine](const qjspp::ArgList&) {
            MinimizeWindow();
            return js_engine.make_undefined();
        });

        mod.export_function("restoreWindow", [&js_engine](const qjspp::ArgList&) {
            RestoreWindow();
            return js_engine.make_undefined();
        });

        mod.export_function("setWindowTitle", [&js_engine](const qjspp::ArgList& args) {
            SetWindowTitle(args[0].to_string().c_str());
            return js_engine.make_undefined();
        });

        mod.export_function("setWindowPosition", [&js_engine](const qjspp::ArgList& args) {
            SetWindowPosition(args[0].to_int(), args[1].to_int());
            return js_engine.make_undefined();
        });

        mod.export_function("setWindowMonitor", [&js_engine](const qjspp::ArgList& args) {
            SetWindowMonitor(args[0].to_int());
            return js_engine.make_undefined();
        });

        mod.export_function("setWindowMinSize", [&js_engine](const qjspp::ArgList& args) {
            SetWindowMinSize(args[0].to_int(), args[1].to_int());
            return js_engine.make_undefined();
        });

        mod.export_function("setWindowMaxSize", [&js_engine](const qjspp::ArgList& args) {
            SetWindowMaxSize(args[0].to_int(), args[1].to_int());
            return js_engine.make_undefined();
        });

        mod.export_function("setWindowSize", [&js_engine](const qjspp::ArgList& args) {
            SetWindowSize(args[0].to_int(), args[1].to_int());
            return js_engine.make_undefined();
        });

        mod.export_function("setWindowOpacity", [&js_engine](const qjspp::ArgList& args) {
            SetWindowOpacity(args[0].to_float());
            return js_engine.make_undefined();
        });

        mod.export_function("setWindowFocused", [&js_engine](const qjspp::ArgList&) {
            SetWindowFocused();
            return js_engine.make_undefined();
        });

        mod.export_function("getScreenWidth", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_int(GetScreenWidth());
        });

        mod.export_function("getScreenHeight", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_int(GetScreenHeight());
        });

        mod.export_function("getRenderWidth", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_int(GetRenderWidth());
        });

        mod.export_function("getRenderHeight", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_int(GetRenderHeight());
        });

        mod.export_function("getMonitorCount", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_int(GetMonitorCount());
        });

        mod.export_function("getCurrentMonitor", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_int(GetCurrentMonitor());
        });

        mod.export_function("getMonitorPosition", [&js_engine](const qjspp::ArgList& args) {
            return Vector2ToValue(js_engine, GetMonitorPosition(args[0].to_int()));
        });

        mod.export_function("getMonitorWidth", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_int(GetMonitorWidth(args[0].to_int()));
        });

        mod.export_function("getMonitorHeight", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_int(GetMonitorHeight(args[0].to_int()));
        });

        mod.export_function("getMonitorPhysicalWidth", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_int(GetMonitorPhysicalWidth(args[0].to_int()));
        });

        mod.export_function("getMonitorPhysicalHeight", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_int(GetMonitorPhysicalHeight(args[0].to_int()));
        });

        mod.export_function("getMonitorRefreshRate", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_int(GetMonitorRefreshRate(args[0].to_int()));
        });

        mod.export_function("getWindowPosition", [&js_engine](const qjspp::ArgList&) {
            return Vector2ToValue(js_engine, GetWindowPosition());
        });

        mod.export_function("getWindowScaleDPI", [&js_engine](const qjspp::ArgList&) {
            return Vector2ToValue(js_engine, GetWindowScaleDPI());
        });

        mod.export_function("getMonitorName", [&js_engine](const qjspp::ArgList& args) {
            const char* name = GetMonitorName(args[0].to_int());
            return name ? js_engine.make_string(name) : js_engine.make_null();
        });

        mod.export_function("showCursor", [&js_engine](const qjspp::ArgList&) {
            ShowCursor();
            return js_engine.make_undefined();
        });

        mod.export_function("hideCursor", [&js_engine](const qjspp::ArgList&) {
            HideCursor();
            return js_engine.make_undefined();
        });

        mod.export_function("isCursorHidden", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_bool(IsCursorHidden());
        });

        mod.export_function("enableCursor", [&js_engine](const qjspp::ArgList&) {
            EnableCursor();
            return js_engine.make_undefined();
        });

        mod.export_function("disableCursor", [&js_engine](const qjspp::ArgList&) {
            DisableCursor();
            return js_engine.make_undefined();
        });

        mod.export_function("isCursorOnScreen", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_bool(IsCursorOnScreen());
        });

        mod.export_function("setTargetFPS", [&js_engine](const qjspp::ArgList& args) {
            SetTargetFPS(args[0].to_int());
            return js_engine.make_undefined();
        });

        mod.export_function("getFrameTime", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_double(GetFrameTime());
        });

        mod.export_function("getTime", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_double(GetTime());
        });

        mod.export_function("getFPS", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_int(GetFPS());
        });

        mod.export_function("takeScreenshot", [&js_engine](const qjspp::ArgList& args) {
            TakeScreenshot(args[0].to_string().c_str());
            return js_engine.make_undefined();
        });

        mod.export_function("setConfigFlags", [&js_engine](const qjspp::ArgList& args) {
            SetConfigFlags(static_cast<unsigned int>(args[0].to_int()));
            return js_engine.make_undefined();
        });

        mod.export_function("clearBackground", [&js_engine](const qjspp::ArgList& args) {
            ClearBackground(ValueToColor(args[0]));
            return js_engine.make_undefined();
        });

        mod.export_function("beginDrawing", [&js_engine](const qjspp::ArgList&) {
            BeginDrawing();
            return js_engine.make_undefined();
        });

        mod.export_function("endDrawing", [&js_engine](const qjspp::ArgList&) {
            EndDrawing();
            return js_engine.make_undefined();
        });

    }

    static void register_color_constants(qjspp::ModuleBuilder& mod, qjspp::Engine& js_engine) {

        mod.export_value("LIGHTGRAY", ColorToValue(js_engine, LIGHTGRAY));
        mod.export_value("GRAY", ColorToValue(js_engine, GRAY));
        mod.export_value("DARKGRAY", ColorToValue(js_engine, DARKGRAY));
        mod.export_value("YELLOW", ColorToValue(js_engine, YELLOW));
        mod.export_value("GOLD", ColorToValue(js_engine, GOLD));
        mod.export_value("ORANGE", ColorToValue(js_engine, ORANGE));
        mod.export_value("PINK", ColorToValue(js_engine, PINK));
        mod.export_value("RED", ColorToValue(js_engine, RED));
        mod.export_value("MAROON", ColorToValue(js_engine, MAROON));
        mod.export_value("GREEN", ColorToValue(js_engine, GREEN));
        mod.export_value("LIME", ColorToValue(js_engine, LIME));
        mod.export_value("DARKGREEN", ColorToValue(js_engine, DARKGREEN));
        mod.export_value("SKYBLUE", ColorToValue(js_engine, SKYBLUE));
        mod.export_value("BLUE", ColorToValue(js_engine, BLUE));
        mod.export_value("DARKBLUE", ColorToValue(js_engine, DARKBLUE));
        mod.export_value("PURPLE", ColorToValue(js_engine, PURPLE));
        mod.export_value("VIOLET", ColorToValue(js_engine, VIOLET));
        mod.export_value("DARKPURPLE", ColorToValue(js_engine, DARKPURPLE));
        mod.export_value("BEIGE", ColorToValue(js_engine, BEIGE));
        mod.export_value("BROWN", ColorToValue(js_engine, BROWN));
        mod.export_value("DARKBROWN", ColorToValue(js_engine, DARKBROWN));
        mod.export_value("WHITE", ColorToValue(js_engine, WHITE));
        mod.export_value("BLACK", ColorToValue(js_engine, BLACK));
        mod.export_value("BLANK", ColorToValue(js_engine, BLANK));
        mod.export_value("MAGENTA", ColorToValue(js_engine, MAGENTA));
        mod.export_value("RAYWHITE", ColorToValue(js_engine, RAYWHITE));

    }

    static void register_text_functions(qjspp::ModuleBuilder& mod, qjspp::Engine& js_engine) {

        mod.export_function("drawText", [&js_engine](const qjspp::ArgList& args) {
            DrawText(args[0].to_string().c_str(), args[1].to_int(), args[2].to_int(), args[3].to_int(), ValueToColor(args[4]));
            return js_engine.make_undefined();
        });

    }

    void register_raylib_module(qjspp::Engine& js_engine) {
        auto mod = js_engine.new_module("raylib");
        register_core_functions(mod, js_engine);
        register_text_functions(mod, js_engine);
        register_color_constants(mod, js_engine);

        mod.export_value("VERSION", js_engine.make_string(std::format("{}.{}.{}", RAYLIB_VERSION_MAJOR, RAYLIB_VERSION_MINOR, RAYLIB_VERSION_PATCH)));

        mod.finalize();
    }

}