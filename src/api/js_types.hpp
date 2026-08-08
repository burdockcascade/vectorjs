#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <utility>
#include <raylib.h>
#include <quickjs.h>

namespace VectorJS {

    inline JSClassID js_color_class_id;
    inline JSClassID js_vector2_class_id;
    inline JSClassID js_rectangle_class_id;
    inline JSClassID js_font_class_id;
    inline JSClassID js_application_class_id;

    struct JSApplication {
        JSApplication(int w, int h, std::string_view title);
        static JSValue Run(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    };

    struct JSColor {
        uint8_t r = 0, g = 0, b = 0, a = 0;

        constexpr JSColor() noexcept = default;
        constexpr JSColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept : r(r), g(g), b(b), a(a) {}
        explicit constexpr JSColor(Color color) noexcept : r(color.r), g(color.g), b(color.b), a(color.a) {}

        // C++23: Explicit object parameter (Deducing This)
        [[nodiscard]] constexpr operator Color(this JSColor self) noexcept {
            return Color{ self.r, self.g, self.b, self.a };
        }
    };

    struct JSVector2 {
        float x = 0.0f, y = 0.0f;

        constexpr JSVector2() noexcept = default;
        constexpr JSVector2(float x, float y) noexcept : x(x), y(y) {}
        explicit constexpr JSVector2(Vector2 v) noexcept : x(v.x), y(v.y) {}

        [[nodiscard]] constexpr operator Vector2(this JSVector2 self) noexcept {
            return Vector2{ self.x, self.y };
        }
    };

    struct JSRectangle {
        float x = 0.0f, y = 0.0f, width = 0.0f, height = 0.0f;

        constexpr JSRectangle() noexcept = default;
        constexpr JSRectangle(float x, float y, float width, float height) noexcept : x(x), y(y), width(width), height(height) {}
        explicit constexpr JSRectangle(Rectangle r) noexcept : x(r.x), y(r.y), width(r.width), height(r.height) {}

        // C++23: Explicit object parameter (Deducing This)
        [[nodiscard]] constexpr operator Rectangle(this JSRectangle self) noexcept {
            return Rectangle{ self.x, self.y, self.width, self.height };
        }
    };

    struct JSFont {
        std::shared_ptr<Font> font_ptr;

        JSFont() = default;
        explicit JSFont(std::string_view path, int baseSize = 64) {
            const Font f = LoadFontEx(path.data(), baseSize, nullptr, 0);
            if (f.texture.id != 0) {
                SetTextureFilter(f.texture, TEXTURE_FILTER_BILINEAR);
            }

            // Replaced manual new with std::make_shared and custom deleter
            font_ptr = std::shared_ptr<Font>(new Font(f), [](Font* pf) {
                if (pf) {
                    if (pf->texture.id != 0) {
                        UnloadFont(*pf);
                    }
                    delete pf;
                }
            });
        }
    };

    struct JSDrawOptions {
        JSColor color{BLACK};
        float rotation = 0.0f;
        bool wireframe = false;
        JSVector2 origin{0.0f, 0.0f};
    };

    struct JSTextOptions {
        JSFont font;
        JSColor color{BLACK};
        float rotation = 0.0f;
        float fontSize = 24.0f;
        float spacing = 1.0f;
        JSVector2 origin{0.0f, 0.0f};
    };

}