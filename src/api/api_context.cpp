#include "hostapi.hpp"
#include "js_types.hpp"
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

    JSValue create_update_context_object(JSContext* ctx) {
        Utils::ScopedJSValue update_obj(ctx, JS_NewObject(ctx));

        JS_SetPropertyStr(ctx, update_obj, "isKeyPressed", JS_NewCFunction(ctx, [](JSContext* c, JSValueConst, int argc, JSValueConst* argv) -> JSValue {
            int key;
            if (argc < 1 || JS_ToInt32(c, &key, argv[0]) < 0) return JS_ThrowTypeError(c, "isKeyPressed requires a Keyboard key enum");
            return JS_NewBool(c, ::IsKeyPressed(key));
        }, "isKeyPressed", 1));

        JS_SetPropertyStr(ctx, update_obj, "isKeyDown", JS_NewCFunction(ctx, [](JSContext* c, JSValueConst, int argc, JSValueConst* argv) -> JSValue {
            int key;
            if (argc < 1 || JS_ToInt32(c, &key, argv[0]) < 0) return JS_ThrowTypeError(c, "isKeyDown requires a Keyboard key enum");
            return JS_NewBool(c, ::IsKeyDown(key));
        }, "isKeyDown", 1));

        JS_SetPropertyStr(ctx, update_obj, "isKeyReleased", JS_NewCFunction(ctx, [](JSContext* c, JSValueConst, int argc, JSValueConst* argv) -> JSValue {
            int key;
            if (argc < 1 || JS_ToInt32(c, &key, argv[0]) < 0) return JS_ThrowTypeError(c, "isKeyReleased requires a Keyboard key enum");
            return JS_NewBool(c, ::IsKeyReleased(key));
        }, "isKeyReleased", 1));

        JS_SetPropertyStr(ctx, update_obj, "isKeyUp", JS_NewCFunction(ctx, [](JSContext* c, JSValueConst, int argc, JSValueConst* argv) -> JSValue {
            int key;
            if (argc < 1 || JS_ToInt32(c, &key, argv[0]) < 0) return JS_ThrowTypeError(c, "isKeyUp requires a Keyboard key enum");
            return JS_NewBool(c, ::IsKeyUp(key));
        }, "isKeyUp", 1));

        JS_SetPropertyStr(ctx, update_obj, "isMouseButtonPressed", JS_NewCFunction(ctx, [](JSContext* c, JSValueConst, int argc, JSValueConst* argv) -> JSValue {
            int button;
            if (argc < 1 || JS_ToInt32(c, &button, argv[0]) < 0) return JS_ThrowTypeError(c, "isMouseButtonPressed requires a MouseButton enum");
            return JS_NewBool(c, ::IsMouseButtonPressed(button));
        }, "isMouseButtonPressed", 1));

        JS_SetPropertyStr(ctx, update_obj, "isMouseButtonDown", JS_NewCFunction(ctx, [](JSContext* c, JSValueConst, int argc, JSValueConst* argv) -> JSValue {
            int button;
            if (argc < 1 || JS_ToInt32(c, &button, argv[0]) < 0) return JS_ThrowTypeError(c, "isMouseButtonDown requires a MouseButton enum");
            return JS_NewBool(c, ::IsMouseButtonDown(button));
        }, "isMouseButtonDown", 1));

        JS_SetPropertyStr(ctx, update_obj, "isMouseButtonReleased", JS_NewCFunction(ctx, [](JSContext* c, JSValueConst, int argc, JSValueConst* argv) -> JSValue {
            int button;
            if (argc < 1 || JS_ToInt32(c, &button, argv[0]) < 0) return JS_ThrowTypeError(c, "isMouseButtonReleased requires a MouseButton enum");
            return JS_NewBool(c, ::IsMouseButtonReleased(button));
        }, "isMouseButtonReleased", 1));

        JS_SetPropertyStr(ctx, update_obj, "isMouseButtonUp", JS_NewCFunction(ctx, [](JSContext* c, JSValueConst, int argc, JSValueConst* argv) -> JSValue {
            int button;
            if (argc < 1 || JS_ToInt32(c, &button, argv[0]) < 0) return JS_ThrowTypeError(c, "isMouseButtonUp requires a MouseButton enum");
            return JS_NewBool(c, ::IsMouseButtonUp(button));
        }, "isMouseButtonUp", 1));

        JS_SetPropertyStr(ctx, update_obj, "getMousePosition", JS_NewCFunction(ctx, [](JSContext* c, JSValueConst, int, JSValueConst*) -> JSValue {
            Vector2 pos = ::GetMousePosition();
            return Utils::create_class_instance<JSVector2>(c, js_vector2_class_id, pos.x, pos.y);
        }, "getMousePosition", 0));

        JS_SetPropertyStr(ctx, update_obj, "getMouseX", JS_NewCFunction(ctx, [](JSContext* c, JSValueConst, int, JSValueConst*) -> JSValue {
            return JS_NewInt32(c, ::GetMouseX());
        }, "getMouseX", 0));

        JS_SetPropertyStr(ctx, update_obj, "getMouseY", JS_NewCFunction(ctx, [](JSContext* c, JSValueConst, int, JSValueConst*) -> JSValue {
            return JS_NewInt32(c, ::GetMouseY());
        }, "getMouseY", 0));

        JS_SetPropertyStr(ctx, update_obj, "getMouseWheelMove", JS_NewCFunction(ctx, [](JSContext* c, JSValueConst, int, JSValueConst*) -> JSValue {
            return JS_NewFloat64(c, ::GetMouseWheelMove());
        }, "getMouseWheelMove", 0));

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

    JSValue create_draw_render_object(JSContext* ctx) {

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

} // namespace HostApi