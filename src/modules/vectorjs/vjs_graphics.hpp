#pragma once

#include <raylib.h>

namespace App::Modules {

    struct JSCamera2D {
        Vector2 offset = { .x = 0.0f, .y = 0.0f };
        Vector2 target = { .x = 0.0f, .y = 0.0f };
        float rotation = 0.0f;
        float zoom = 1.0f;

        constexpr JSCamera2D() noexcept = default;
        constexpr JSCamera2D(Vector2 offset, Vector2 target, float rotation, float zoom) noexcept : offset(offset), target(target), rotation(rotation), zoom(zoom) {}

        explicit constexpr JSCamera2D(::Camera2D c) noexcept : offset(c.offset), target(c.target), rotation(c.rotation), zoom(c.zoom) {}

        [[nodiscard]] constexpr operator ::Camera2D(this JSCamera2D self) noexcept {
            return ::Camera2D{ .offset = self.offset, .target = self.target, .rotation = self.rotation, .zoom = self.zoom };
        }
    };

    struct JSFont {
        std::shared_ptr<::Font> font_ptr;

        JSFont() = default;
        explicit JSFont(std::string path, int baseSize = 64) {
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

}