#pragma once
#include <raylib.h>

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

    template<>
    struct converter<AudioStream> {
        static AudioStream get(JSContext* ctx, const JSValue v) {
            int64_t buffer_ptr = 0, processor_ptr = 0;
            uint32_t sampleRate = 0, sampleSize = 0, channels = 0;

            const JSValue b_val = JS_GetPropertyStr(ctx, v, "buffer");
            const JSValue p_val = JS_GetPropertyStr(ctx, v, "processor");
            const JSValue sr_val = JS_GetPropertyStr(ctx, v, "sampleRate");
            const JSValue ss_val = JS_GetPropertyStr(ctx, v, "sampleSize");
            const JSValue c_val = JS_GetPropertyStr(ctx, v, "channels");

            JS_ToInt64(ctx, &buffer_ptr, b_val);
            JS_ToInt64(ctx, &processor_ptr, p_val);
            JS_ToUint32(ctx, &sampleRate, sr_val);
            JS_ToUint32(ctx, &sampleSize, ss_val);
            JS_ToUint32(ctx, &channels, c_val);

            JS_FreeValue(ctx, b_val);
            JS_FreeValue(ctx, p_val);
            JS_FreeValue(ctx, sr_val);
            JS_FreeValue(ctx, ss_val);
            JS_FreeValue(ctx, c_val);

            return AudioStream{
                reinterpret_cast<rAudioBuffer*>(buffer_ptr),
                reinterpret_cast<rAudioProcessor*>(processor_ptr),
                sampleRate,
                sampleSize,
                channels
            };
        }

        static Value put(JSContext* ctx, const AudioStream &val) {
            const JSValue obj = JS_NewObject(ctx);
            JS_SetPropertyStr(ctx, obj, "buffer", JS_NewInt64(ctx, reinterpret_cast<int64_t>(val.buffer)));
            JS_SetPropertyStr(ctx, obj, "processor", JS_NewInt64(ctx, reinterpret_cast<int64_t>(val.processor)));
            JS_SetPropertyStr(ctx, obj, "sampleRate", JS_NewUint32(ctx, val.sampleRate));
            JS_SetPropertyStr(ctx, obj, "sampleSize", JS_NewUint32(ctx, val.sampleSize));
            JS_SetPropertyStr(ctx, obj, "channels", JS_NewUint32(ctx, val.channels));
            return { ctx, obj };
        }
    };

    template<>
    struct converter<Sound> {
        static Sound get(JSContext* ctx, const JSValue v) {
            uint32_t frameCount = 0;

            const JSValue stream_val = JS_GetPropertyStr(ctx, v, "stream");
            const JSValue fc_val = JS_GetPropertyStr(ctx, v, "frameCount");

            const AudioStream stream = converter<AudioStream>::get(ctx, stream_val);
            JS_ToUint32(ctx, &frameCount, fc_val);

            JS_FreeValue(ctx, stream_val);
            JS_FreeValue(ctx, fc_val);

            return Sound{stream, frameCount};
        }

        static Value put(JSContext* ctx, const Sound &val) {
            const JSValue obj = JS_NewObject(ctx);
            Value stream_val = converter<AudioStream>::put(ctx, val.stream);

            // Duplicate the stream_val because JS_SetPropertyStr steals one reference
            // and the qjs::Value wrapper will try to free it on destruction.
            JS_SetPropertyStr(ctx, obj, "stream", JS_DupValue(ctx, stream_val.get()));
            JS_SetPropertyStr(ctx, obj, "frameCount", JS_NewUint32(ctx, val.frameCount));
            return { ctx, obj };
        }
    };

}