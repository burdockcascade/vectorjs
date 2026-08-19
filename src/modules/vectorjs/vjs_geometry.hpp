#pragma once

#include <raylib.h>
#include <qjspp.hpp>

namespace App::Modules {

    struct JSRectangle {
        float x = 0.0f, y = 0.0f, width = 0.0f, height = 0.0f;

        constexpr JSRectangle() noexcept = default;
        constexpr JSRectangle(float x, float y, float width, float height) noexcept : x(x), y(y), width(width), height(height) {}
        explicit constexpr JSRectangle(::Rectangle r) noexcept : x(r.x), y(r.y), width(r.width), height(r.height) {}

        [[nodiscard]] constexpr operator ::Rectangle(this JSRectangle self) noexcept {
            return ::Rectangle{ .x = self.x, .y = self.y, .width = self.width, .height = self.height };
        }
    };

    struct JSCircle {
        Vector2 center;
        float radius;
    };

    struct JSTriangle {
        Vector2 v1;
        Vector2 v2;
        Vector2 v3;
    };

    struct JSPoint {
        Vector2 position;
    };

    struct JSLine {
        Vector2 start;
        Vector2 end;
    };

    struct JSPolygon {
        std::vector<Vector2> points;
    };

}