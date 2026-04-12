#pragma once
#include <raylib.h>
#include "../qjs_wrapper.hpp"

namespace qjs {

    template<>
    struct converter<Color> {
        static Color get(JSContext* ctx, const JSValue v) {
            uint32_t r, g, b, a;
            const JSValue r_val = JS_GetPropertyStr(ctx, v, "r");
            const JSValue g_val = JS_GetPropertyStr(ctx, v, "g");
            const JSValue b_val = JS_GetPropertyStr(ctx, v, "b");
            const JSValue a_val = JS_GetPropertyStr(ctx, v, "a");

            JS_ToUint32(ctx, &r, r_val);
            JS_ToUint32(ctx, &g, g_val);
            JS_ToUint32(ctx, &b, b_val);
            JS_ToUint32(ctx, &a, a_val);

            JS_FreeValue(ctx, r_val); JS_FreeValue(ctx, g_val);
            JS_FreeValue(ctx, b_val); JS_FreeValue(ctx, a_val);

            return Color{static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b), static_cast<unsigned char>(a)};
        }
        static Value put(JSContext* ctx, const Color val) {
            const JSValue obj = JS_NewObject(ctx);
            JS_SetPropertyStr(ctx, obj, "r", JS_NewInt32(ctx, val.r));
            JS_SetPropertyStr(ctx, obj, "g", JS_NewInt32(ctx, val.g));
            JS_SetPropertyStr(ctx, obj, "b", JS_NewInt32(ctx, val.b));
            JS_SetPropertyStr(ctx, obj, "a", JS_NewInt32(ctx, val.a));
            return { ctx, obj };
        }
    };

    template<>
    struct converter<Vector2> {
        static Vector2 get(JSContext* ctx, const JSValue v) {
            double x, y;
            const JSValue x_val = JS_GetPropertyStr(ctx, v, "x");
            const JSValue y_val = JS_GetPropertyStr(ctx, v, "y");
            JS_ToFloat64(ctx, &x, x_val);
            JS_ToFloat64(ctx, &y, y_val);
            JS_FreeValue(ctx, x_val); JS_FreeValue(ctx, y_val);
            return Vector2{static_cast<float>(x), static_cast<float>(y)};
        }
        static Value put(JSContext* ctx, const Vector2 val) {
            const JSValue obj = JS_NewObject(ctx);
            JS_SetPropertyStr(ctx, obj, "x", JS_NewFloat64(ctx, val.x));
            JS_SetPropertyStr(ctx, obj, "y", JS_NewFloat64(ctx, val.y));
            return { ctx, obj };
        }
    };

    template<>
    struct converter<Texture> {
        static Texture get(JSContext* ctx, const JSValue v) {
            uint32_t id;
            int32_t width;
            int32_t height;
            int32_t mipmaps;
            int32_t format;

            const JSValue id_val = JS_GetPropertyStr(ctx, v, "id");
            const JSValue width_val = JS_GetPropertyStr(ctx, v, "width");
            const JSValue height_val = JS_GetPropertyStr(ctx, v, "height");
            const JSValue mipmaps_val = JS_GetPropertyStr(ctx, v, "mipmaps");
            const JSValue format_val = JS_GetPropertyStr(ctx, v, "format");

            JS_ToUint32(ctx, &id, id_val);
            JS_ToInt32(ctx, &width, width_val);
            JS_ToInt32(ctx, &height, height_val);
            JS_ToInt32(ctx, &mipmaps, mipmaps_val);
            JS_ToInt32(ctx, &format, format_val);

            JS_FreeValue(ctx, id_val); 
            JS_FreeValue(ctx, width_val);
            JS_FreeValue(ctx, height_val);
            JS_FreeValue(ctx, mipmaps_val);
            JS_FreeValue(ctx, format_val);
            return Texture{id, width, height, mipmaps, format};
        }

        static Value put(JSContext* ctx, const Texture &val) {
            const JSValue obj = JS_NewObject(ctx);
            JS_SetPropertyStr(ctx, obj, "id", JS_NewUint32(ctx, val.id));
            JS_SetPropertyStr(ctx, obj, "width", JS_NewInt32(ctx, val.width));
            JS_SetPropertyStr(ctx, obj, "height", JS_NewInt32(ctx, val.height));
            JS_SetPropertyStr(ctx, obj, "mipmaps", JS_NewInt32(ctx, val.mipmaps));
            JS_SetPropertyStr(ctx, obj, "format", JS_NewInt32(ctx, val.format));
            return { ctx, obj };
        }
    };

}

namespace RaylibModule {
    void register_raylib_module(qjs::Engine &engine);
}