#pragma once

#include <string>
#include <memory>
#include <cstdint>
#include <raylib.h>
#include <quickjs.h>

namespace HostApi {

    inline JSClassID js_color_class_id;
    inline JSClassID js_vector2_class_id;
    inline JSClassID js_rectangle_class_id;
    inline JSClassID js_font_class_id;
    inline JSClassID js_application_class_id;

    struct JSApplication {
        JSApplication(int w, int h, const std::string& title);
        static JSValue Run(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    };

    struct JSColor {
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
        uint8_t a = 0;
        JSColor() = default;
        JSColor(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) : r(r), g(g), b(b), a(a) {}
        explicit constexpr JSColor(const Color color) : r(color.r), g(color.g), b(color.b), a(color.a) {}
        [[nodiscard]] constexpr operator Color() const { return Color{ r, g, b, a }; }
    };

    struct JSVector2 {
        float x = 0.0f;
        float y = 0.0f;
        JSVector2() = default;
        JSVector2(const float x, const float y) : x(x), y(y) {}
        explicit constexpr JSVector2(const Vector2 v) : x(v.x), y(v.y) {}
        [[nodiscard]] constexpr operator Vector2() const { return Vector2 { x, y }; }
    };

    struct JSRectangle {
        float x = 0.0f;
        float y = 0.0f;
        float width = 0.0f;
        float height = 0.0f;
        JSRectangle() = default;
        JSRectangle(const float x, const float y, const float width, const float height) : x(x), y(y), width(width), height(height) {}
        explicit constexpr JSRectangle(const Rectangle r) : x(r.x), y(r.y), width(r.width), height(r.height) {}
        [[nodiscard]] constexpr operator Rectangle() const { return Rectangle { x, y, width, height }; }
    };

    struct JSFont {
        std::shared_ptr<Font> font_ptr;
        JSFont() = default;
        explicit JSFont(const std::string& path, const int baseSize = 64) {
            const Font f = LoadFontEx(path.c_str(), baseSize, nullptr, 0);
            if (f.texture.id != 0) {
                SetTextureFilter(f.texture, TEXTURE_FILTER_BILINEAR);
            }
            font_ptr = std::shared_ptr<Font>(new Font(f), [](const Font* pf) {
                if (pf->texture.id != 0) {
                    UnloadFont(*pf);
                }
                delete pf;
            });
        }
    };

    struct JSDrawOptions {
        JSColor color = JSColor(BLACK);
        float rotation = 0.0f;
        bool wireframe = false;
        JSVector2 origin = JSVector2(0, 0);
    };

    struct JSTextOptions {
        JSFont font;
        JSColor color = JSColor(BLACK);
        float rotation = 0.0f;
        float fontSize = 24.0f;
        float spacing = 1.0f;
        JSVector2 origin = JSVector2(0, 0);
    };

}