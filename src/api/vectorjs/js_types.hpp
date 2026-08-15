#pragma once

#include <string_view>
#include <memory>
#include <vector>
#include <raylib.h>
#include <qjspp.hpp>

namespace VectorJS {

    struct JSApplication {
        JSApplication(int w, int h, std::string_view title);
        qjspp::Value run(const std::vector<qjspp::Value>& args);
    };

    struct JSColor {
        uint8_t r = 0, g = 0, b = 0, a = 0;

        constexpr JSColor() noexcept = default;
        constexpr JSColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept : r(r), g(g), b(b), a(a) {}
        explicit constexpr JSColor(::Color color) noexcept : r(color.r), g(color.g), b(color.b), a(color.a) {}

        [[nodiscard]] constexpr operator ::Color(this const JSColor& self) noexcept {
            return ::Color{ .r = self.r, .g = self.g, .b = self.b, .a = self.a };
        }
    };

    struct JSVector2 {
        float x = 0.0f, y = 0.0f;

        constexpr JSVector2() noexcept = default;
        constexpr JSVector2(float x, float y) noexcept : x(x), y(y) {}
        explicit constexpr JSVector2(::Vector2 v) noexcept : x(v.x), y(v.y) {}

        [[nodiscard]] constexpr operator ::Vector2(this JSVector2 self) noexcept {
            return ::Vector2{ .x = self.x, .y = self.y };
        }
    };

    struct JSRectangle {
        float x = 0.0f, y = 0.0f, width = 0.0f, height = 0.0f;

        constexpr JSRectangle() noexcept = default;
        constexpr JSRectangle(float x, float y, float width, float height) noexcept : x(x), y(y), width(width), height(height) {}
        explicit constexpr JSRectangle(::Rectangle r) noexcept : x(r.x), y(r.y), width(r.width), height(r.height) {}

        [[nodiscard]] constexpr operator ::Rectangle(this JSRectangle self) noexcept {
            return ::Rectangle{ .x = self.x, .y = self.y, .width = self.width, .height = self.height };
        }
    };

    struct JSFont {
        std::shared_ptr<::Font> font_ptr;

        JSFont() = default;
        explicit JSFont(std::string_view path, int baseSize = 64) {
            const ::Font f = LoadFontEx(path.data(), baseSize, nullptr, 0);
            if (f.texture.id != 0) {
                SetTextureFilter(f.texture, TEXTURE_FILTER_BILINEAR);
            }

            font_ptr = std::shared_ptr<::Font>(new ::Font(f), [](::Font* pf) {
                if (pf) {
                    if (pf->texture.id != 0) {
                        UnloadFont(*pf);
                    }
                    delete pf;
                }
            });
        }
    };

    struct JSCamera2D {
        ::Vector2 offset{ .x = 0.0f, .y = 0.0f };
        ::Vector2 target{ .x = 0.0f, .y = 0.0f };
        float rotation = 0.0f;
        float zoom = 1.0f;

        constexpr JSCamera2D() noexcept = default;
        constexpr JSCamera2D(::Vector2 offset, ::Vector2 target, float rotation, float zoom) noexcept : offset(offset), target(target), rotation(rotation), zoom(zoom) {}

        explicit constexpr JSCamera2D(::Camera2D c) noexcept : offset(c.offset), target(c.target), rotation(c.rotation), zoom(c.zoom) {}

        [[nodiscard]] constexpr operator ::Camera2D(this JSCamera2D self) noexcept {
            return ::Camera2D{ .offset = self.offset, .target = self.target, .rotation = self.rotation, .zoom = self.zoom };
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