#include "js_types.hpp"
#include "js_bind.hpp"
#include "js_utils.hpp"
#include <iostream>

namespace HostApi {

    static JSDrawOptions parse_draw_options(JSContext* ctx, JSValueConst optionsObj) {
        JSDrawOptions options;
        if (JS_IsObject(optionsObj)) {
            Utils::try_get_opaque_property<JSColor>(ctx, optionsObj, "color", js_color_class_id, options.color);
            Utils::try_get_opaque_property<JSVector2>(ctx, optionsObj, "origin", js_vector2_class_id, options.origin);
            Utils::try_get_float_property(ctx, optionsObj, "rotation", options.rotation);
            Utils::try_get_bool_property(ctx, optionsObj, "wireframe", options.wireframe);
        }
        return options;
    }

    static JSTextOptions parse_text_options(JSContext* ctx, JSValueConst optionsObj) {
        JSTextOptions options;
        if (JS_IsObject(optionsObj)) {
            Utils::try_get_opaque_property<JSFont>(ctx, optionsObj, "font", js_font_class_id, options.font);
            Utils::try_get_opaque_property<JSColor>(ctx, optionsObj, "color", js_color_class_id, options.color);
            Utils::try_get_float_property(ctx, optionsObj, "rotation", options.rotation);
            Utils::try_get_float_property(ctx, optionsObj, "fontSize", options.fontSize);
            Utils::try_get_float_property(ctx, optionsObj, "spacing", options.spacing);
            Utils::try_get_opaque_property<JSVector2>(ctx, optionsObj, "origin", js_vector2_class_id, options.origin);
        }
        return options;
    }

    static JSValue create_update_context_object(JSContext* ctx) {
        Utils::ScopedJSValue update_obj(ctx, JS_NewObject(ctx));

        // Window Actions
        bind_fn<MinimizeWindow>(ctx, update_obj, "minimizeWindow");
        bind_fn<MaximizeWindow>(ctx, update_obj, "maximizeWindow");

        // Window Properties
        bind_fn<IsWindowFullscreen>(ctx, update_obj, "isWindowFullscreen");
        bind_fn<IsWindowHidden>(ctx, update_obj, "isWindowHidden");
        bind_fn<IsWindowResized>(ctx, update_obj, "isWindowResized");
        bind_fn<IsWindowMinimized>(ctx, update_obj, "isWindowMinimized");
        bind_fn<IsWindowMaximized>(ctx, update_obj, "isWindowMaximized");
        bind_fn<IsWindowFocused>(ctx, update_obj, "isWindowFocused");

        // Monitor
        bind_fn<GetScreenWidth>(ctx, update_obj, "getScreenWidth");
        bind_fn<GetScreenHeight>(ctx, update_obj, "getScreenHeight");
        bind_fn<GetRenderWidth>(ctx, update_obj, "getRenderWidth");
        bind_fn<GetRenderHeight>(ctx, update_obj, "getRenderHeight");
        bind_fn<GetMonitorCount>(ctx, update_obj, "getMonitorCount");
        bind_fn<GetCurrentMonitor>(ctx, update_obj, "getCurrentMonitor");

        // Keyboard Checks
        bind_fn<IsKeyPressed>(ctx, update_obj, "isKeyPressed");
        bind_fn<IsKeyDown>(ctx, update_obj, "isKeyDown");
        bind_fn<IsKeyReleased>(ctx, update_obj, "isKeyReleased");
        bind_fn<IsKeyUp>(ctx, update_obj, "isKeyUp");

        // Mouse Button Checks
        bind_fn<IsMouseButtonPressed>(ctx, update_obj, "isMouseButtonPressed");
        bind_fn<IsMouseButtonDown>(ctx, update_obj, "isMouseButtonDown");
        bind_fn<IsMouseButtonReleased>(ctx, update_obj, "isMouseButtonReleased");
        bind_fn<IsMouseButtonUp>(ctx, update_obj, "isMouseButtonUp");

        // Mouse State Getters
        bind_fn<GetMouseWheelMove>(ctx, update_obj, "getMouseWheelMove");
        bind_fn<GetMousePosition>(ctx, update_obj, "getMousePosition");

        return update_obj.release();
    }

    static JSValue create_shapes_object(JSContext* ctx) {
        Utils::ScopedJSValue shape_obj(ctx, JS_NewObject(ctx));

        JS_SetPropertyStr(ctx, shape_obj, "drawPixel", JS_NewCFunction(ctx, [](JSContext* c, JSValueConst, int argc, JSValueConst* argv) -> JSValue {
            if (argc < 2) return JS_ThrowTypeError(c, "drawPixel requires position and color arguments");
            const auto pos = Utils::get_opaque_or<JSVector2>(c, argv[0], js_vector2_class_id, {0, 0});
            const auto col = Utils::get_opaque_or<JSColor>(c, argv[1], js_color_class_id, JSColor(RAYWHITE));
            ::DrawPixelV(pos, col);
            return JS_UNDEFINED;
        }, "drawPixel", 2));

        JS_SetPropertyStr(ctx, shape_obj, "drawLine", JS_NewCFunction(ctx, [](JSContext* c, JSValueConst, int argc, JSValueConst* argv) -> JSValue {
            if (argc < 2) return JS_ThrowTypeError(c, "drawLine requires start and end positions");
            const auto start = Utils::get_opaque_or<JSVector2>(c, argv[0], js_vector2_class_id, {0, 0});
            const auto end = Utils::get_opaque_or<JSVector2>(c, argv[1], js_vector2_class_id, {0, 0});
            const JSDrawOptions draw_options = parse_draw_options(c, argc > 2 ? argv[2] : JS_UNDEFINED);
            ::DrawLineV(start, end, draw_options.color);
            return JS_UNDEFINED;
        }, "drawLine", 3));

        JS_SetPropertyStr(ctx, shape_obj, "drawRectangle", JS_NewCFunction(ctx, [](JSContext* c, JSValueConst, int argc, JSValueConst* argv) -> JSValue {
            if (argc < 1) return JS_ThrowTypeError(c, "drawRectangle requires a Rectangle argument");
            const auto rect = Utils::get_opaque_or<JSRectangle>(c, argv[0], js_rectangle_class_id, {0, 0, 0, 0});
            const JSDrawOptions draw_options = parse_draw_options(c, argc > 1 ? argv[1] : JS_UNDEFINED);
            ::DrawRectanglePro(rect, draw_options.origin, draw_options.rotation, draw_options.color);
            return JS_UNDEFINED;
        }, "drawRectangle", 2));

        JS_SetPropertyStr(ctx, shape_obj, "drawCircle", JS_NewCFunction(ctx, [](JSContext* c, JSValueConst, int argc, JSValueConst* argv) -> JSValue {
            if (argc < 2) return JS_ThrowTypeError(c, "drawCircle requires center and radius arguments");
            const auto center = Utils::get_opaque_or<JSVector2>(c, argv[0], js_vector2_class_id, {0, 0});
            double rad = 0;
            if (JS_ToFloat64(c, &rad, argv[1]) != 0) {
                return JS_ThrowTypeError(c, "drawCircle radius must be a number");
            }
            const JSDrawOptions draw_options = parse_draw_options(c, argc > 2 ? argv[2] : JS_UNDEFINED);
            ::DrawCircleV(center, static_cast<float>(rad), draw_options.color);
            return JS_UNDEFINED;
        }, "drawCircle", 3));

        JS_SetPropertyStr(ctx, shape_obj, "drawTriangle", JS_NewCFunction(ctx, [](JSContext* c, JSValueConst, int argc, JSValueConst* argv) -> JSValue {
            if (argc < 3) return JS_ThrowTypeError(c, "drawTriangle requires 3 Vector2 point arguments");
            const auto p1 = Utils::get_opaque_or<JSVector2>(c, argv[0], js_vector2_class_id, {0, 0});
            const auto p2 = Utils::get_opaque_or<JSVector2>(c, argv[1], js_vector2_class_id, {0, 0});
            const auto p3 = Utils::get_opaque_or<JSVector2>(c, argv[2], js_vector2_class_id, {0, 0});
            const JSDrawOptions draw_options = parse_draw_options(c, argc > 3 ? argv[3] : JS_UNDEFINED);
            ::DrawTriangle(p1, p2, p3, draw_options.color);
            return JS_UNDEFINED;
        }, "drawTriangle", 4));

        JS_SetPropertyStr(ctx, shape_obj, "drawEllipse", JS_NewCFunction(ctx, [](JSContext* c, JSValueConst, int argc, JSValueConst* argv) -> JSValue {
            if (argc < 3) return JS_ThrowTypeError(c, "drawEllipse requires center, radiusH, and radiusV arguments");
            const auto center = Utils::get_opaque_or<JSVector2>(c, argv[0], js_vector2_class_id, {0, 0});
            double radH = 0, radV = 0;
            if (JS_ToFloat64(c, &radH, argv[1]) != 0 || JS_ToFloat64(c, &radV, argv[2]) != 0) {
                return JS_ThrowTypeError(c, "drawEllipse radii parameters must be numbers");
            }
            const JSDrawOptions draw_options = parse_draw_options(c, argc > 3 ? argv[3] : JS_UNDEFINED);
            ::DrawEllipseV(center, static_cast<float>(radH), static_cast<float>(radV), draw_options.color);
            return JS_UNDEFINED;
        }, "drawEllipse", 4));

        return shape_obj.release();
    }

    static JSValue create_text_object(JSContext* ctx) {
        Utils::ScopedJSValue text_obj(ctx, JS_NewObject(ctx));
        JS_SetPropertyStr(ctx, text_obj, "drawText", JS_NewCFunction(ctx, [](JSContext* c, JSValueConst, int argc, JSValueConst* argv) -> JSValue {
            if (argc < 2) return JS_ThrowTypeError(c, "drawText requires position and text string arguments");
            const auto pos = Utils::get_opaque_or<JSVector2>(c, argv[0], js_vector2_class_id, {0, 0});

            const char* txt_cstr = JS_ToCString(c, argv[1]);
            if (!txt_cstr) return JS_EXCEPTION;

            JSTextOptions options = parse_text_options(c, argc > 2 ? argv[2] : JS_UNDEFINED);
            Font fontToUse = GetFontDefault();
            if (options.font.font_ptr && options.font.font_ptr->texture.id != 0) {
                fontToUse = *options.font.font_ptr;
            }
            ::DrawTextPro(fontToUse, txt_cstr, pos, options.origin, options.rotation, options.fontSize, options.spacing, options.color);
            JS_FreeCString(c, txt_cstr);
            return JS_UNDEFINED;
        }, "drawText", 3));

        return text_obj.release();
    }

    static JSValue create_draw_render_object(JSContext* ctx) {

        Utils::ScopedJSValue render2d_obj(ctx, JS_NewObject(ctx));

        // Add FPS
        JS_SetPropertyStr(ctx, render2d_obj, "drawFPS", JS_NewCFunction(ctx, [](JSContext* c, JSValueConst, int argc, JSValueConst* argv) -> JSValue {
            if (argc < 1) return JS_ThrowTypeError(c, "drawFPS requires a Vector2 position argument");
            const auto pos = Utils::get_opaque_or<JSVector2>(c, argv[0], js_vector2_class_id, {0, 0});
            ::DrawFPS(static_cast<int>(pos.x), static_cast<int>(pos.y));
            return JS_UNDEFINED;
        }, "drawFPS", 1));

        // Add Sub Objects
        JS_SetPropertyStr(ctx, render2d_obj, "shapes", create_shapes_object(ctx));
        JS_SetPropertyStr(ctx, render2d_obj, "text", create_text_object(ctx));

        // --- Layer Wrappers ---
        const JSValue render_obj = JS_NewObject(ctx);
        JSValue r2d_val = render2d_obj.release();

        JS_SetPropertyStr(ctx, render_obj, "withLayer2D", JS_NewCFunctionData(ctx, [](JSContext* c, JSValueConst, int argc, JSValueConst* argv, int, JSValue* magic_argv) -> JSValue {
            if (argc > 0 && JS_IsFunction(c, argv[0])) {
                const Utils::ScopedJSValue res(c, JS_Call(c, argv[0], JS_UNDEFINED, 1, &magic_argv[0]));
                if (JS_IsException(res.get())) return JS_EXCEPTION;
            }
            return JS_UNDEFINED;
        }, 1, 0, 1, &r2d_val));

        JS_FreeValue(ctx, r2d_val);
        return render_obj;
    }

    JSApplication::JSApplication(const int w, const int h, const std::string& title) {
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
            if (Utils::ScopedJSValue ret(ctx, JS_Call(ctx, on_init_func, user_app, 0, nullptr)); JS_IsException(ret)) {
                return JS_EXCEPTION;
            }
        }

        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            if (JS_IsFunction(ctx, on_update_func)) {
                JSValue u = update_obj.get();
                Utils::ScopedJSValue ret(ctx, JS_Call(ctx, on_update_func, user_app, 1, &u));
                if (JS_IsException(ret.get())) {
                    EndDrawing();
                    if (IsWindowReady()) CloseWindow();
                    return JS_EXCEPTION;
                }
            }
            if (JS_IsFunction(ctx, on_draw_func)) {
                JSValue r = render_obj.get();
                Utils::ScopedJSValue ret(ctx, JS_Call(ctx, on_draw_func, user_app, 1, &r));
                if (JS_IsException(ret.get())) {
                    EndDrawing();
                    if (IsWindowReady()) CloseWindow();
                    return JS_EXCEPTION;
                }
            }
            EndDrawing();
        }

        if (IsWindowReady()) {
            CloseWindow();
        }

        return JS_UNDEFINED;
    }

} // namespace HostApi