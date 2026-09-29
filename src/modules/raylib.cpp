#include <stdexcept>
#include <vector>
#include <sstream>
#include <format>
#include <raylib.h>
#include <qjspp.hpp>

namespace App::Modules {

    static qjspp::Value Vector2ToValue(const qjspp::Engine &js_engine, const Vector2& val) {
        auto obj = js_engine.make_object();
        obj.set("x", js_engine.make_double(val.x));
        obj.set("y", js_engine.make_double(val.y));
        return obj;
    }

    static Vector2 ValueToVector2(const qjspp::Value& val) {

        Vector2 out = {
            .x = 0.0f,
            .y = 0.0f
        };

        if (!val.is_object()) return out;

        if (const auto x = val.get("x"); !x.is_undefined()) {
            out.x = x.to_float();
        }

        if (const auto y = val.get("y"); !y.is_undefined()) {
            out.y = y.to_float();
        }

        return out;
    }

    static qjspp::Value Vector3ToValue(const qjspp::Engine &js_engine, const Vector3& val) {
        auto obj = js_engine.make_object();
        obj.set("x", js_engine.make_double(val.x));
        obj.set("y", js_engine.make_double(val.y));
        obj.set("z", js_engine.make_double(val.z));
        return obj;
    }

    static Vector3 ValueToVector3(const qjspp::Value& val) {

        Vector3 out = {
            .x = 0.0f,
            .y = 0.0f,
            .z = 0.0f
        };

        if (!val.is_object()) return out;

        if (const auto x = val.get("x"); !x.is_undefined() && x.is_number()) {
            out.x = x.to_float();
        }

        if (const auto y = val.get("y"); !y.is_undefined() && y.is_number()) {
            out.y = y.to_float();
        }

        if (const auto z = val.get("z"); !z.is_undefined() && z.is_number()) {
            out.z = z.to_float();
        }

        return out;
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

    static qjspp::Value RectangleToValue(const qjspp::Engine &js_engine, const Rectangle& val) {
        auto obj = js_engine.make_object();
        obj.set("x", js_engine.make_double(val.x));
        obj.set("y", js_engine.make_double(val.y));
        obj.set("width", js_engine.make_double(val.width));
        obj.set("height", js_engine.make_double(val.height));
        return obj;
    }

    static Rectangle ValueToRectangle(const qjspp::Value& val) {

        Rectangle out = {
            .x = 0.0f,
            .y = 0.0f,
            .width = 0.0f,
            .height = 0.0f
        };

        if (!val.is_object()) return out;

        if (const auto x = val.get("x"); !x.is_undefined() && x.is_number()) {
            out.x = x.to_float();
        }

        if (const auto y = val.get("y"); !y.is_undefined() && y.is_number()) {
            out.y = y.to_float();
        }

        if (const auto width = val.get("width"); !width.is_undefined() && width.is_number()) {
            out.width = width.to_float();
        }

        if (const auto height = val.get("height"); !height.is_undefined() && height.is_number()) {
            out.height = height.to_float();
        }

        return out;
    }

    static Camera2D ValueToCamera2D(const qjspp::Value& val) {

        Camera2D out = {
            .offset = {.x = 0.0f, .y = 0.0f},
            .target = {.x = 0.0f, .y = 0.0f},
            .rotation = 0.0f,
            .zoom = 1.0f
        };

        if (!val.is_object()) return out;

        if (const auto offset = val.get("offset"); !offset.is_undefined() && offset.is_object()) {
            out.offset = ValueToVector2(offset);
        }

        if (const auto target = val.get("target"); !target.is_undefined() && target.is_object()) {
            out.target = ValueToVector2(target);
        }

        if (const auto rotation = val.get("rotation"); !rotation.is_undefined() && rotation.is_number()) {
            out.rotation = rotation.to_float();
        }

        if (const auto zoom = val.get("zoom"); !zoom.is_undefined() && zoom.is_number()) {
            out.zoom = zoom.to_float();
        }

        return out;
    }

    static Camera3D ValueToCamera3D(const qjspp::Value& val) {

        Camera3D out = {
            .position = {.x = 0.0f, .y = 0.0f, .z = 0.0f},
            .target = {.x = 0.0f, .y = 0.0f, .z = 0.0f},
            .up = {.x = 0.0f, .y = 1.0f, .z = 0.0f},
            .fovy = 45.0f,
            .projection = CAMERA_PERSPECTIVE
        };

        if (!val.is_object()) return out;

        if (const auto position = val.get("position"); !position.is_undefined() && position.is_object()) {
            out.position = ValueToVector3(position);
        }

        if (const auto target = val.get("target"); !target.is_undefined() && target.is_object()) {
            out.target = ValueToVector3(target);
        }

        if (const auto up = val.get("up"); !up.is_undefined() && up.is_object()) {
            out.up = ValueToVector3(up);
        }

        if (const auto fovy = val.get("fovy"); !fovy.is_undefined() && fovy.is_number()) {
            out.fovy = fovy.to_float();
        }

        if (const auto projection = val.get("projection"); !projection.is_undefined() && projection.is_number()) {
            out.projection = static_cast<int>(projection.to_int());
        }

        return out;
    }

    static qjspp::Value MatrixToValue(const qjspp::Engine &js_engine, const Matrix& val) {
        auto obj = js_engine.make_object();
        obj.set("m0", js_engine.make_double(val.m0));
        obj.set("m4", js_engine.make_double(val.m4));
        obj.set("m8", js_engine.make_double(val.m8));
        obj.set("m12", js_engine.make_double(val.m12));
        obj.set("m1", js_engine.make_double(val.m1));
        obj.set("m5", js_engine.make_double(val.m5));
        obj.set("m9", js_engine.make_double(val.m9));
        obj.set("m13", js_engine.make_double(val.m13));
        obj.set("m2", js_engine.make_double(val.m2));
        obj.set("m6", js_engine.make_double(val.m6));
        obj.set("m10", js_engine.make_double(val.m10));
        obj.set("m14", js_engine.make_double(val.m14));
        obj.set("m3", js_engine.make_double(val.m3));
        obj.set("m7", js_engine.make_double(val.m7));
        obj.set("m11", js_engine.make_double(val.m11));
        obj.set("m15", js_engine.make_double(val.m15));
        return obj;
    }

    static qjspp::Value RayToValue(qjspp::Engine &js_engine, const Ray& val) {
        auto obj = js_engine.make_object();
        obj.set("position", Vector3ToValue(js_engine, val.position));
        obj.set("direction", Vector3ToValue(js_engine, val.direction));
        return obj;
    }

    static std::vector<Vector2> ParseVector2Array(const qjspp::Value& arr_val) {
        std::vector<Vector2> points;
        if (!arr_val.is_array()) return points;
        const auto items = arr_val.to_vector();
        points.reserve(items.size());
        for (const auto& item : items) {
            points.push_back(ValueToVector2(item));
        }
        return points;
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

    }

    static void register_draw2d_functions(qjspp::ModuleBuilder& mod, qjspp::Engine& engine) {

        mod.export_function("clearBackground", [&engine](const qjspp::ArgList& args) {
            ClearBackground(ValueToColor(args[0]));
            return engine.make_undefined();
        });

        mod.export_function("beginDrawing", [&engine](const qjspp::ArgList&) {
            BeginDrawing();
            return engine.make_undefined();
        });

        mod.export_function("endDrawing", [&engine](const qjspp::ArgList&) {
            EndDrawing();
            return engine.make_undefined();
        });

        mod.export_function("beginMode2D", [&engine](const qjspp::ArgList& args) {
            BeginMode2D(ValueToCamera2D(args[0]));
            return engine.make_undefined();
        });

        mod.export_function("endMode2D", [&engine](const qjspp::ArgList&) {
            EndMode2D();
            return engine.make_undefined();
        });

        mod.export_function("getScreenToWorldRay", [&engine](const qjspp::ArgList& args) {
            return RayToValue(engine, GetScreenToWorldRay(ValueToVector2(args[0]), ValueToCamera3D(args[1])));
        });

        mod.export_function("getScreenToWorldRayEx", [&engine](const qjspp::ArgList& args) {
            return RayToValue(engine, GetScreenToWorldRayEx(ValueToVector2(args[0]), ValueToCamera3D(args[1]), args[2].to_int(), args[3].to_int()));
        });

        mod.export_function("getWorldToScreen", [&engine](const qjspp::ArgList& args) {
            return Vector2ToValue(engine, GetWorldToScreen(ValueToVector3(args[0]), ValueToCamera3D(args[1])));
        });

        mod.export_function("getWorldToScreenEx", [&engine](const qjspp::ArgList& args) {
            return Vector2ToValue(engine, GetWorldToScreenEx(ValueToVector3(args[0]), ValueToCamera3D(args[1]), args[2].to_int(), args[3].to_int()));
        });

        mod.export_function("getWorldToScreen2D", [&engine](const qjspp::ArgList& args) {
            return Vector2ToValue(engine, GetWorldToScreen2D(ValueToVector2(args[0]), ValueToCamera2D(args[1])));
        });

        mod.export_function("getScreenToWorld2D", [&engine](const qjspp::ArgList& args) {
            return Vector2ToValue(engine, GetScreenToWorld2D(ValueToVector2(args[0]), ValueToCamera2D(args[1])));
        });

        mod.export_function("getCameraMatrix", [&engine](const qjspp::ArgList& args) {
            return MatrixToValue(engine, GetCameraMatrix(ValueToCamera3D(args[0])));
        });

        mod.export_function("getCameraMatrix2D", [&engine](const qjspp::ArgList& args) {
            return MatrixToValue(engine, GetCameraMatrix2D(ValueToCamera2D(args[0])));
        });

        mod.export_function("drawPixel", [&engine](const qjspp::ArgList& args) {
            DrawPixel(args[0].to_int(), args[1].to_int(), ValueToColor(args[2]));
            return engine.make_undefined();
        });

        mod.export_function("drawPixelV", [&engine](const qjspp::ArgList& args) {
            DrawPixelV(ValueToVector2(args[0]), ValueToColor(args[1]));
            return engine.make_undefined();
        });

        mod.export_function("drawLine", [&engine](const qjspp::ArgList& args) {
            DrawLine(args[0].to_int(), args[1].to_int(), args[2].to_int(), args[3].to_int(), ValueToColor(args[4]));
            return engine.make_undefined();
        });

        mod.export_function("drawLineV", [&engine](const qjspp::ArgList& args) {
            DrawLineV(ValueToVector2(args[0]), ValueToVector2(args[1]), ValueToColor(args[2]));
            return engine.make_undefined();
        });

        mod.export_function("drawLineEx", [&engine](const qjspp::ArgList& args) {
            DrawLineEx(ValueToVector2(args[0]), ValueToVector2(args[1]), args[2].to_float(), ValueToColor(args[3]));
            return engine.make_undefined();
        });

        mod.export_function("drawLineStrip", [&engine](const qjspp::ArgList& args) {
            const auto points = ParseVector2Array(args[0]);
            DrawLineStrip(points.data(), static_cast<int>(points.size()), ValueToColor(args[1]));
            return engine.make_undefined();
        });

        mod.export_function("drawLineBezier", [&engine](const qjspp::ArgList& args) {
            DrawLineBezier(ValueToVector2(args[0]), ValueToVector2(args[1]), args[2].to_float(), ValueToColor(args[3]));
            return engine.make_undefined();
        });

        mod.export_function("drawLineDashed", [&engine](const qjspp::ArgList& args) {
            DrawLineDashed(ValueToVector2(args[0]), ValueToVector2(args[1]), args[2].to_int(), args[3].to_int(), ValueToColor(args[4]));
            return engine.make_undefined();
        });

        mod.export_function("drawCircle", [&engine](const qjspp::ArgList& args) {
            DrawCircle(args[0].to_int(), args[1].to_int(), args[2].to_float(), ValueToColor(args[3]));
            return engine.make_undefined();
        });

        mod.export_function("drawCircleSector", [&engine](const qjspp::ArgList& args) {
            DrawCircleSector(ValueToVector2(args[0]), args[1].to_float(), args[2].to_float(), args[3].to_float(), args[4].to_int(), ValueToColor(args[5]));
            return engine.make_undefined();
        });

        mod.export_function("drawCircleSectorLines", [&engine](const qjspp::ArgList& args) {
            DrawCircleSectorLines(ValueToVector2(args[0]), args[1].to_float(), args[2].to_float(), args[3].to_float(), args[4].to_int(), ValueToColor(args[5]));
            return engine.make_undefined();
        });

        mod.export_function("drawCircleGradient", [&engine](const qjspp::ArgList& args) {
            DrawCircleGradient(ValueToVector2(args[0]), args[1].to_float(), ValueToColor(args[2]), ValueToColor(args[3]));
            return engine.make_undefined();
        });

        mod.export_function("drawCircleV", [&engine](const qjspp::ArgList& args) {
            DrawCircleV(ValueToVector2(args[0]), args[1].to_float(), ValueToColor(args[2]));
            return engine.make_undefined();
        });

        mod.export_function("drawCircleLines", [&engine](const qjspp::ArgList& args) {
            DrawCircleLines(args[0].to_int(), args[1].to_int(), args[2].to_float(), ValueToColor(args[3]));
            return engine.make_undefined();
        });

        mod.export_function("drawCircleLinesV", [&engine](const qjspp::ArgList& args) {
            DrawCircleLinesV(ValueToVector2(args[0]), args[1].to_float(), ValueToColor(args[2]));
            return engine.make_undefined();
        });

        mod.export_function("drawEllipse", [&engine](const qjspp::ArgList& args) {
            DrawEllipse(args[0].to_int(), args[1].to_int(), args[2].to_float(), args[3].to_float(), ValueToColor(args[4]));
            return engine.make_undefined();
        });

        mod.export_function("drawEllipseV", [&engine](const qjspp::ArgList& args) {
            DrawEllipseV(ValueToVector2(args[0]), args[1].to_float(), args[2].to_float(), ValueToColor(args[3]));
            return engine.make_undefined();
        });

        mod.export_function("drawEllipseLines", [&engine](const qjspp::ArgList& args) {
            DrawEllipseLines(args[0].to_int(), args[1].to_int(), args[2].to_float(), args[3].to_float(), ValueToColor(args[4]));
            return engine.make_undefined();
        });

        mod.export_function("drawEllipseLinesV", [&engine](const qjspp::ArgList& args) {
            DrawEllipseLinesV(ValueToVector2(args[0]), args[1].to_float(), args[2].to_float(), ValueToColor(args[3]));
            return engine.make_undefined();
        });

        mod.export_function("drawRing", [&engine](const qjspp::ArgList& args) {
            DrawRing(ValueToVector2(args[0]), args[1].to_float(), args[2].to_float(), args[3].to_float(), args[4].to_float(), args[5].to_int(), ValueToColor(args[6]));
            return engine.make_undefined();
        });

        mod.export_function("drawRingLines", [&engine](const qjspp::ArgList& args) {
            DrawRingLines(ValueToVector2(args[0]), args[1].to_float(), args[2].to_float(), args[3].to_float(), args[4].to_float(), args[5].to_int(), ValueToColor(args[6]));
            return engine.make_undefined();
        });

        mod.export_function("drawRectangle", [&engine](const qjspp::ArgList& args) {
            DrawRectangle(args[0].to_int(), args[1].to_int(), args[2].to_int(), args[3].to_int(), ValueToColor(args[4]));
            return engine.make_undefined();
        });

        mod.export_function("drawRectangleV", [&engine](const qjspp::ArgList& args) {
            DrawRectangleV(ValueToVector2(args[0]), ValueToVector2(args[1]), ValueToColor(args[2]));
            return engine.make_undefined();
        });

        mod.export_function("drawRectangleRec", [&engine](const qjspp::ArgList& args) {
            DrawRectangleRec(ValueToRectangle(args[0]), ValueToColor(args[1]));
            return engine.make_undefined();
        });

        mod.export_function("drawRectanglePro", [&engine](const qjspp::ArgList& args) {
            DrawRectanglePro(ValueToRectangle(args[0]), ValueToVector2(args[1]), args[2].to_float(), ValueToColor(args[3]));
            return engine.make_undefined();
        });

        mod.export_function("drawRectangleGradientV", [&engine](const qjspp::ArgList& args) {
            DrawRectangleGradientV(args[0].to_int(), args[1].to_int(), args[2].to_int(), args[3].to_int(), ValueToColor(args[4]), ValueToColor(args[5]));
            return engine.make_undefined();
        });

        mod.export_function("drawRectangleGradientH", [&engine](const qjspp::ArgList& args) {
            DrawRectangleGradientH(args[0].to_int(), args[1].to_int(), args[2].to_int(), args[3].to_int(), ValueToColor(args[4]), ValueToColor(args[5]));
            return engine.make_undefined();
        });

        mod.export_function("drawRectangleGradientEx", [&engine](const qjspp::ArgList& args) {
            DrawRectangleGradientEx(ValueToRectangle(args[0]), ValueToColor(args[1]), ValueToColor(args[2]), ValueToColor(args[3]), ValueToColor(args[4]));
            return engine.make_undefined();
        });

        mod.export_function("drawRectangleLines", [&engine](const qjspp::ArgList& args) {
            DrawRectangleLines(args[0].to_int(), args[1].to_int(), args[2].to_int(), args[3].to_int(), ValueToColor(args[4]));
            return engine.make_undefined();
        });

        mod.export_function("drawRectangleLinesEx", [&engine](const qjspp::ArgList& args) {
            DrawRectangleLinesEx(ValueToRectangle(args[0]), args[1].to_float(), ValueToColor(args[2]));
            return engine.make_undefined();
        });

        mod.export_function("drawRectangleRounded", [&engine](const qjspp::ArgList& args) {
            DrawRectangleRounded(ValueToRectangle(args[0]), args[1].to_float(), args[2].to_int(), ValueToColor(args[3]));
            return engine.make_undefined();
        });

        mod.export_function("drawRectangleRoundedLines", [&engine](const qjspp::ArgList& args) {
            DrawRectangleRoundedLines(ValueToRectangle(args[0]), args[1].to_float(), args[2].to_int(), ValueToColor(args[3]));
            return engine.make_undefined();
        });

        mod.export_function("drawRectangleRoundedLinesEx", [&engine](const qjspp::ArgList& args) {
            DrawRectangleRoundedLinesEx(ValueToRectangle(args[0]), args[1].to_float(), args[2].to_int(), args[3].to_float(), ValueToColor(args[4]));
            return engine.make_undefined();
        });

        mod.export_function("drawTriangle", [&engine](const qjspp::ArgList& args) {
            DrawTriangle(ValueToVector2(args[0]), ValueToVector2(args[1]), ValueToVector2(args[2]), ValueToColor(args[3]));
            return engine.make_undefined();
        });

        mod.export_function("drawTriangleLines", [&engine](const qjspp::ArgList& args) {
            DrawTriangleLines(ValueToVector2(args[0]), ValueToVector2(args[1]), ValueToVector2(args[2]), ValueToColor(args[3]));
            return engine.make_undefined();
        });

        mod.export_function("drawTriangleFan", [&engine](const qjspp::ArgList& args) {
            const auto points = ParseVector2Array(args[0]);
            DrawTriangleFan(points.data(), static_cast<int>(points.size()), ValueToColor(args[1]));
            return engine.make_undefined();
        });

        mod.export_function("drawTriangleStrip", [&engine](const qjspp::ArgList& args) {
            const auto points = ParseVector2Array(args[0]);
            DrawTriangleStrip(points.data(), static_cast<int>(points.size()), ValueToColor(args[1]));
            return engine.make_undefined();
        });

        mod.export_function("drawPoly", [&engine](const qjspp::ArgList& args) {
            DrawPoly(ValueToVector2(args[0]), args[1].to_int(), args[2].to_float(), args[3].to_float(), ValueToColor(args[4]));
            return engine.make_undefined();
        });

        mod.export_function("drawPolyLines", [&engine](const qjspp::ArgList& args) {
            DrawPolyLines(ValueToVector2(args[0]), args[1].to_int(), args[2].to_float(), args[3].to_float(), ValueToColor(args[4]));
            return engine.make_undefined();
        });

        mod.export_function("drawPolyLinesEx", [&engine](const qjspp::ArgList& args) {
            DrawPolyLinesEx(ValueToVector2(args[0]), args[1].to_int(), args[2].to_float(), args[3].to_float(), args[4].to_float(), ValueToColor(args[5]));
            return engine.make_undefined();
        });

        mod.export_function("drawSplineLinear", [&engine](const qjspp::ArgList& args) {
            const auto points = ParseVector2Array(args[0]);
            DrawSplineLinear(points.data(), static_cast<int>(points.size()), args[1].to_float(), ValueToColor(args[2]));
            return engine.make_undefined();
        });

        mod.export_function("drawSplineBasis", [&engine](const qjspp::ArgList& args) {
            const auto points = ParseVector2Array(args[0]);
            DrawSplineBasis(points.data(), static_cast<int>(points.size()), args[1].to_float(), ValueToColor(args[2]));
            return engine.make_undefined();
        });

        mod.export_function("drawSplineCatmullRom", [&engine](const qjspp::ArgList& args) {
            const auto points = ParseVector2Array(args[0]);
            DrawSplineCatmullRom(points.data(), static_cast<int>(points.size()), args[1].to_float(), ValueToColor(args[2]));
            return engine.make_undefined();
        });

        mod.export_function("drawSplineBezierQuadratic", [&engine](const qjspp::ArgList& args) {
            const auto points = ParseVector2Array(args[0]);
            DrawSplineBezierQuadratic(points.data(), static_cast<int>(points.size()), args[1].to_float(), ValueToColor(args[2]));
            return engine.make_undefined();
        });

        mod.export_function("drawSplineBezierCubic", [&engine](const qjspp::ArgList& args) {
            const auto points = ParseVector2Array(args[0]);
            DrawSplineBezierCubic(points.data(), static_cast<int>(points.size()), args[1].to_float(), ValueToColor(args[2]));
            return engine.make_undefined();
        });

        mod.export_function("drawSplineSegmentLinear", [&engine](const qjspp::ArgList& args) {
            DrawSplineSegmentLinear(ValueToVector2(args[0]), ValueToVector2(args[1]), args[2].to_float(), ValueToColor(args[3]));
            return engine.make_undefined();
        });

        mod.export_function("drawSplineSegmentBasis", [&engine](const qjspp::ArgList& args) {
            DrawSplineSegmentBasis(ValueToVector2(args[0]), ValueToVector2(args[1]), ValueToVector2(args[2]), ValueToVector2(args[3]), args[4].to_float(), ValueToColor(args[5]));
            return engine.make_undefined();
        });

        mod.export_function("drawSplineSegmentCatmullRom", [&engine](const qjspp::ArgList& args) {
            DrawSplineSegmentCatmullRom(ValueToVector2(args[0]), ValueToVector2(args[1]), ValueToVector2(args[2]), ValueToVector2(args[3]), args[4].to_float(), ValueToColor(args[5]));
            return engine.make_undefined();
        });

        mod.export_function("drawSplineSegmentBezierQuadratic", [&engine](const qjspp::ArgList& args) {
            DrawSplineSegmentBezierQuadratic(ValueToVector2(args[0]), ValueToVector2(args[1]), ValueToVector2(args[2]), args[3].to_float(), ValueToColor(args[4]));
            return engine.make_undefined();
        });

        mod.export_function("drawSplineSegmentBezierCubic", [&engine](const qjspp::ArgList& args) {
            DrawSplineSegmentBezierCubic(ValueToVector2(args[0]), ValueToVector2(args[1]), ValueToVector2(args[2]), ValueToVector2(args[3]), args[4].to_float(), ValueToColor(args[5]));
            return engine.make_undefined();
        });

        mod.export_function("getSplinePointLinear", [&engine](const qjspp::ArgList& args) {
            return Vector2ToValue(engine, GetSplinePointLinear(ValueToVector2(args[0]), ValueToVector2(args[1]), args[2].to_float()));
        });

        mod.export_function("getSplinePointBasis", [&engine](const qjspp::ArgList& args) {
            return Vector2ToValue(engine, GetSplinePointBasis(ValueToVector2(args[0]), ValueToVector2(args[1]), ValueToVector2(args[2]), ValueToVector2(args[3]), args[4].to_float()));
        });

        mod.export_function("getSplinePointCatmullRom", [&engine](const qjspp::ArgList& args) {
            return Vector2ToValue(engine, GetSplinePointCatmullRom(ValueToVector2(args[0]), ValueToVector2(args[1]), ValueToVector2(args[2]), ValueToVector2(args[3]), args[4].to_float()));
        });

        mod.export_function("getSplinePointBezierQuad", [&engine](const qjspp::ArgList& args) {
            return Vector2ToValue(engine, GetSplinePointBezierQuad(ValueToVector2(args[0]), ValueToVector2(args[1]), ValueToVector2(args[2]), args[3].to_float()));
        });

        mod.export_function("getSplinePointBezierCubic", [&engine](const qjspp::ArgList& args) {
            return Vector2ToValue(engine, GetSplinePointBezierCubic(ValueToVector2(args[0]), ValueToVector2(args[1]), ValueToVector2(args[2]), ValueToVector2(args[3]), args[4].to_float()));
        });

        mod.export_function("drawFPS", [&engine](const qjspp::ArgList& args) {
            DrawFPS(args[0].to_int(), args[1].to_int());
            return engine.make_undefined();
        });

    }

    static void register_input_functions(qjspp::ModuleBuilder& mod, qjspp::Engine& js_engine) {

        mod.export_function("isKeyPressed", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(IsKeyPressed(args[0].to_int()));
        });

        mod.export_function("isKeyPressedRepeat", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(IsKeyPressedRepeat(args[0].to_int()));
        });

        mod.export_function("isKeyDown", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(IsKeyDown(args[0].to_int()));
        });

        mod.export_function("isKeyReleased", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(IsKeyReleased(args[0].to_int()));
        });

        mod.export_function("isKeyUp", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(IsKeyUp(args[0].to_int()));
        });

        mod.export_function("getKeyPressed", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_int(GetKeyPressed());
        });

        mod.export_function("getCharPressed", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_int(GetCharPressed());
        });

        mod.export_function("getKeyName", [&js_engine](const qjspp::ArgList& args) {
            const char* name = GetKeyName(args[0].to_int());
            return name ? js_engine.make_string(name) : js_engine.make_null();
        });

        mod.export_function("setExitKey", [&js_engine](const qjspp::ArgList& args) {
            SetExitKey(args[0].to_int());
            return js_engine.make_undefined();
        });

        mod.export_function("isGamepadAvailable", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(IsGamepadAvailable(args[0].to_int()));
        });

        mod.export_function("getGamepadName", [&js_engine](const qjspp::ArgList& args) {
            const char* name = GetGamepadName(args[0].to_int());
            return name ? js_engine.make_string(name) : js_engine.make_null();
        });

        mod.export_function("isGamepadButtonPressed", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(IsGamepadButtonPressed(args[0].to_int(), args[1].to_int()));
        });

        mod.export_function("isGamepadButtonDown", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(IsGamepadButtonDown(args[0].to_int(), args[1].to_int()));
        });

        mod.export_function("isGamepadButtonReleased", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(IsGamepadButtonReleased(args[0].to_int(), args[1].to_int()));
        });

        mod.export_function("isGamepadButtonUp", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(IsGamepadButtonUp(args[0].to_int(), args[1].to_int()));
        });

        mod.export_function("getGamepadButtonPressed", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_int(GetGamepadButtonPressed());
        });

        mod.export_function("getGamepadAxisCount", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_int(GetGamepadAxisCount(args[0].to_int()));
        });

        mod.export_function("getGamepadAxisMovement", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_double(GetGamepadAxisMovement(args[0].to_int(), args[1].to_int()));
        });

        mod.export_function("setGamepadMappings", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_int(SetGamepadMappings(args[0].to_string().c_str()));
        });

        mod.export_function("setGamepadVibration", [&js_engine](const qjspp::ArgList& args) {
            SetGamepadVibration(args[0].to_int(), args[1].to_float(), args[2].to_float(), args[3].to_float());
            return js_engine.make_undefined();
        });

        mod.export_function("isMouseButtonPressed", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(IsMouseButtonPressed(args[0].to_int()));
        });

        mod.export_function("isMouseButtonDown", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(IsMouseButtonDown(args[0].to_int()));
        });

        mod.export_function("isMouseButtonReleased", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(IsMouseButtonReleased(args[0].to_int()));
        });

        mod.export_function("isMouseButtonUp", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(IsMouseButtonUp(args[0].to_int()));
        });

        mod.export_function("getMouseX", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_int(GetMouseX());
        });

        mod.export_function("getMouseY", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_int(GetMouseY());
        });

        mod.export_function("getMousePosition", [&js_engine](const qjspp::ArgList&) {
            return Vector2ToValue(js_engine, GetMousePosition());
        });

        mod.export_function("getMouseDelta", [&js_engine](const qjspp::ArgList&) {
            return Vector2ToValue(js_engine, GetMouseDelta());
        });

        mod.export_function("setMousePosition", [&js_engine](const qjspp::ArgList& args) {
            SetMousePosition(args[0].to_int(), args[1].to_int());
            return js_engine.make_undefined();
        });

        mod.export_function("setMouseOffset", [&js_engine](const qjspp::ArgList& args) {
            SetMouseOffset(args[0].to_int(), args[1].to_int());
            return js_engine.make_undefined();
        });

        mod.export_function("setMouseScale", [&js_engine](const qjspp::ArgList& args) {
            SetMouseScale(args[0].to_float(), args[1].to_float());
            return js_engine.make_undefined();
        });

        mod.export_function("getMouseWheelMove", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_double(GetMouseWheelMove());
        });

        mod.export_function("getMouseWheelMoveV", [&js_engine](const qjspp::ArgList&) {
            return Vector2ToValue(js_engine, GetMouseWheelMoveV());
        });

        mod.export_function("setMouseCursor", [&js_engine](const qjspp::ArgList& args) {
            SetMouseCursor(args[0].to_int());
            return js_engine.make_undefined();
        });

        mod.export_function("getTouchX", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_int(GetTouchX());
        });

        mod.export_function("getTouchY", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_int(GetTouchY());
        });

        mod.export_function("getTouchPosition", [&js_engine](const qjspp::ArgList& args) {
            return Vector2ToValue(js_engine, GetTouchPosition(args[0].to_int()));
        });

        mod.export_function("getTouchPointId", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_int(GetTouchPointId(args[0].to_int()));
        });

        mod.export_function("getTouchPointCount", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_int(GetTouchPointCount());
        });

        mod.export_function("setGesturesEnabled", [&js_engine](const qjspp::ArgList& args) {
            SetGesturesEnabled(static_cast<unsigned int>(args[0].to_int()));
            return js_engine.make_undefined();
        });

        mod.export_function("isGestureDetected", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(IsGestureDetected(static_cast<unsigned int>(args[0].to_int())));
        });

        mod.export_function("getGestureDetected", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_int(GetGestureDetected());
        });

        mod.export_function("getGestureHoldDuration", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_double(GetGestureHoldDuration());
        });

        mod.export_function("getGestureDragVector", [&js_engine](const qjspp::ArgList&) {
            return Vector2ToValue(js_engine, GetGestureDragVector());
        });

        mod.export_function("getGestureDragAngle", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_double(GetGestureDragAngle());
        });

        mod.export_function("getGesturePinchVector", [&js_engine](const qjspp::ArgList&) {
            return Vector2ToValue(js_engine, GetGesturePinchVector());
        });

        mod.export_function("getGesturePinchAngle", [&js_engine](const qjspp::ArgList&) {
            return js_engine.make_double(GetGesturePinchAngle());
        });

    }

    static void register_collision_functions(qjspp::ModuleBuilder& mod, qjspp::Engine& js_engine) {

        mod.export_function("checkCollisionRecs", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(CheckCollisionRecs(ValueToRectangle(args[0]), ValueToRectangle(args[1])));
        });

        mod.export_function("checkCollisionCircles", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(CheckCollisionCircles(ValueToVector2(args[0]), args[1].to_float(), ValueToVector2(args[2]), args[3].to_float()));
        });

        mod.export_function("checkCollisionCircleRec", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(CheckCollisionCircleRec(ValueToVector2(args[0]), args[1].to_float(), ValueToRectangle(args[2])));
        });

        mod.export_function("checkCollisionCircleLine", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(CheckCollisionCircleLine(ValueToVector2(args[0]), args[1].to_float(), ValueToVector2(args[2]), ValueToVector2(args[3])));
        });

        mod.export_function("checkCollisionPointRec", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(CheckCollisionPointRec(ValueToVector2(args[0]), ValueToRectangle(args[1])));
        });

        mod.export_function("checkCollisionPointCircle", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(CheckCollisionPointCircle(ValueToVector2(args[0]), ValueToVector2(args[1]), args[2].to_float()));
        });

        mod.export_function("checkCollisionPointTriangle", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(CheckCollisionPointTriangle(ValueToVector2(args[0]), ValueToVector2(args[1]), ValueToVector2(args[2]), ValueToVector2(args[3])));
        });

        mod.export_function("checkCollisionPointLine", [&js_engine](const qjspp::ArgList& args) {
            return js_engine.make_bool(CheckCollisionPointLine(ValueToVector2(args[0]), ValueToVector2(args[1]), ValueToVector2(args[2]), args[3].to_int()));
        });

        mod.export_function("checkCollisionPointPoly", [&js_engine](const qjspp::ArgList& args) {
            const auto points = ParseVector2Array(args[1]);
            return js_engine.make_bool(CheckCollisionPointPoly(ValueToVector2(args[0]), points.data(), static_cast<int>(points.size())));
        });

        mod.export_function("getCollisionRec", [&js_engine](const qjspp::ArgList& args) {
            return RectangleToValue(js_engine, GetCollisionRec(ValueToRectangle(args[0]), ValueToRectangle(args[1])));
        });

    }

    static void register_text_functions(qjspp::ModuleBuilder& mod, qjspp::Engine& js_engine) {

        mod.export_function("drawText", [&js_engine](const qjspp::ArgList& args) {
            DrawText(args[0].to_string().c_str(), args[1].to_int(), args[2].to_int(), args[3].to_int(), ValueToColor(args[4]));
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

    void register_raylib_module(qjspp::Engine& js_engine) {
        auto mod = js_engine.new_module("raylib");
        register_core_functions(mod, js_engine);
        register_draw2d_functions(mod, js_engine);
        register_input_functions(mod, js_engine);
        register_text_functions(mod, js_engine);
        register_collision_functions(mod, js_engine);
        register_color_constants(mod, js_engine);

        mod.export_value("VERSION", js_engine.make_string(std::format("{}.{}.{}", RAYLIB_VERSION_MAJOR, RAYLIB_VERSION_MINOR, RAYLIB_VERSION_PATCH)));

        mod.finalize();
    }

}