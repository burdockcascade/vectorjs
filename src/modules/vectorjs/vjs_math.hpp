#pragma once
#include <raylib.h>

namespace App::Modules {

    struct JSVector2 {
        float x = 0.0f, y = 0.0f;

        constexpr JSVector2() noexcept = default;
        constexpr JSVector2(float x, float y) noexcept : x(x), y(y) {}
        explicit constexpr JSVector2(Vector2 v) noexcept : x(v.x), y(v.y) {}

        [[nodiscard]] constexpr operator Vector2(this JSVector2 self) noexcept {
            return Vector2{ .x = self.x, .y = self.y };
        }
    };

}