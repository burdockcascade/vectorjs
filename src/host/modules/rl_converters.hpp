#pragma once
#include <raylib.h>

namespace qjs {

    template <typename T>
    T get_prop(JSContext* ctx, const JSValue obj, const char* prop_name) {
        JSValue val = JS_GetPropertyStr(ctx, obj, prop_name);
        T result = converter<T>::get(ctx, val);
        JS_FreeValue(ctx, val);
        return result;
    }

    template <typename T>
    void set_prop(JSContext* ctx, JSValue obj, const char* prop_name, T&& val) {
        // Convert the C++ value to a managed Value
        const Value js_val = converter<std::decay_t<T>>::put(ctx, std::forward<T>(val));

        // JS_SetPropertyStr "steals" a reference, meaning it takes ownership.
        // Because Value's destructor also calls JS_FreeValue, we MUST duplicate
        // the reference here to prevent a double-free crash.
        JS_SetPropertyStr(ctx, obj, prop_name, JS_DupValue(ctx, js_val.get()));
    }

    template<>
    struct converter<Color> {
        static Color get(JSContext* ctx, const JSValue v) {
            const auto r = get_prop<unsigned char>(ctx, v, "r");
            const auto g = get_prop<unsigned char>(ctx, v, "g");
            const auto b = get_prop<unsigned char>(ctx, v, "b");
            const auto a = get_prop<unsigned char>(ctx, v, "a");
            return Color{r, g, b, a};
        }
        static Value put(JSContext* ctx, const Color val) {
            const JSValue obj = JS_NewObject(ctx);
            set_prop(ctx, obj, "r", val.r);
            set_prop(ctx, obj, "g", val.g);
            set_prop(ctx, obj, "b", val.b);
            set_prop(ctx, obj, "a", val.a);
            return { ctx, obj };
        }
    };

    template<>
    struct converter<Vector2> {
        static Vector2 get(JSContext* ctx, const JSValue v) {
            const auto x = get_prop<float>(ctx, v, "x");
            const auto y = get_prop<float>(ctx, v, "y");
            return Vector2{x, y};
        }
        static Value put(JSContext* ctx, const Vector2 val) {
            const JSValue obj = JS_NewObject(ctx);
            set_prop(ctx, obj, "x", val.x);
            set_prop(ctx, obj, "y", val.y);
            return { ctx, obj };
        }
    };

    template<>
    struct converter<Texture> {
        static Texture get(JSContext* ctx, const JSValue v) {
            const auto id = get_prop<uint32_t>(ctx, v, "id");
            const auto width = get_prop<int32_t>(ctx, v, "width");
            const auto height = get_prop<int32_t>(ctx, v, "height");
            const auto mipmaps = get_prop<int32_t>(ctx, v, "mipmaps");
            const auto format = get_prop<int32_t>(ctx, v, "format");
            return Texture{id, width, height, mipmaps, format};
        }

        static Value put(JSContext* ctx, const Texture &val) {
            const JSValue obj = JS_NewObject(ctx);
            set_prop(ctx, obj, "id", val.id);
            set_prop(ctx, obj, "width", val.width);
            set_prop(ctx, obj, "height", val.height);
            set_prop(ctx, obj, "mipmaps", val.mipmaps);
            set_prop(ctx, obj, "format", val.format);
            return { ctx, obj };
        }
    };

    template<>
    struct converter<AudioStream> {
        static AudioStream get(JSContext* ctx, const JSValue v) {
            const auto buffer_ptr = reinterpret_cast<rAudioBuffer*>(get_prop<int64_t>(ctx, v, "buffer"));
            const auto processor_ptr = reinterpret_cast<rAudioProcessor*>(get_prop<int64_t>(ctx, v, "processor"));
            const auto sampleRate = get_prop<uint32_t>(ctx, v, "sampleRate");
            const auto sampleSize = get_prop<uint32_t>(ctx, v, "sampleSize");
            const auto channels = get_prop<uint32_t>(ctx, v, "channels");
            return AudioStream{buffer_ptr, processor_ptr, sampleRate, sampleSize, channels};
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
            const auto frameCount = get_prop<uint32_t>(ctx, v, "frameCount");
            const auto stream = get_prop<AudioStream>(ctx, v, "stream");
            return Sound{stream, frameCount};
        }

        static Value put(JSContext* ctx, const Sound &val) {
            JSValue obj = JS_NewObject(ctx);
            set_prop(ctx, obj, "stream", val.stream);
            set_prop(ctx, obj, "frameCount", val.frameCount);
            return { ctx, obj };
        }
    };

}