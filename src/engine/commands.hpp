#pragma once

#include <vector>
#include <variant>
#include <type_traits>
#include <raylib.h>

namespace Hooray {

    // ==========================================
    // State Commands
    // ==========================================

    struct ClearBackground {
        Color color{ RAYWHITE };
    };

    struct BeginMode2D {
        Camera2D camera;
    };

    struct EndMode2D {};

    // ==========================================
    // Primitive Draw Commands
    // ==========================================

    struct DrawFPS {
        Vector2 position;
    };

    struct DrawPixel {
        Vector2 position;
        Color color;
    };

    struct DrawLine {
        Vector2 start;
        Vector2 end;
        float thickness{ 1.0f };
        Color color;
    };

    struct DrawCircle {
        Vector2 center;
        float radius;
        Color color;
    };

    struct DrawRectangle {
        Rectangle rect;
        Color color;
    };

    struct DrawTriangle {
        Vector2 p1;
        Vector2 p2;
        Vector2 p3;
        Color color;
    };

    struct DrawEllipse {
        Vector2 center;
        float radius_h;
        float radius_v;
        Color color;
    };

    struct DrawText {
        const char* text; // Pointer to static/arena memory; avoids std::string heap allocation
        Vector2 position;
        Font font_face;
        float font_size;
        float spacing;
        Color color;
    };

    struct DrawTexture {
        Texture2D texture;
        Vector2 position;
        Color tint{ WHITE };
    };

    // Perfect for sprite sheets and animations
    struct DrawTextureRec {
        Texture2D texture;
        Rectangle source;
        Vector2 position;
        Color tint{ WHITE };
    };

    struct DrawPoly {
        Vector2 center;
        int sides;
        float radius;
        float rotation; // Rotation in degrees
        Color color;
    };

    // ==========================================
    // Command Variant & Queue
    // ==========================================

    using Command = std::variant<
        ClearBackground,
        BeginMode2D,
        EndMode2D,
        DrawFPS,
        DrawPixel,
        DrawLine,
        DrawCircle,
        DrawRectangle,
        DrawTriangle,
        DrawEllipse,
        DrawText,
        DrawTexture,
        DrawTextureRec,
        DrawPoly
    >;

    // Contiguous linear buffer for cache-friendly execution
    class CommandBufferBuilder {
    private:
        std::vector<Command> queue;

    public:
        CommandBufferBuilder() = default;

        // Reserves contiguous memory up front to eliminate vector reallocations during frame updates
        explicit CommandBufferBuilder(size_t initial_capacity) {
            queue.reserve(initial_capacity);
        }

        // Generic push method for any valid command struct
        template <typename T>
        void push(T&& command) {
            static_assert(
                std::is_constructible_v<Command, T>,
                "Type provided is not a supported command in the variant queue."
            );
            queue.push_back(std::forward<T>(command));
        }

        // Helper ergonomics for pushing raw commands directly
        void clear_background(Color color = RAYWHITE) {
            queue.emplace_back(ClearBackground{ .color = color });
        }

        void start_mode_2d(Camera2D camera) {
            queue.emplace_back(BeginMode2D{camera});
        }

        void end_mode_2d() {
            queue.emplace_back(EndMode2D{});
        }

        void draw_fps(Vector2 pos) {
            queue.emplace_back(DrawFPS{ .position = pos });
        }

        void draw_circle(Vector2 center, float radius, Color color) {
            queue.emplace_back(DrawCircle{ .center = center, .radius = radius, .color = color });
        }

        void draw_rectangle(Rectangle rect, Color color) {
            queue.emplace_back(DrawRectangle{ .rect = rect, .color = color });
        }

        void draw_pixel(Vector2 pos, Color color) {
            queue.emplace_back(DrawPixel{ .position = pos, .color = color });
        }

        void draw_line(Vector2 start, Vector2 end, Color color) {
            queue.emplace_back(DrawLine{ .start = start, .end = end });
        }

        void draw_triangle(Vector2 p1, Vector2 p2, Vector2 p3, Color color) {
            queue.emplace_back(DrawTriangle{ .p1 = p1, .p2 = p2, .color = color });
        }

        void draw_ellipse(Vector2 center, float radH, float radV, Color color) {
            queue.emplace_back(DrawEllipse{ .center = center, .radius_h = radH, .radius_v = radV, .color = color });
        }

        void draw_text(const char* text, const Vector2 pos, const Font &font_face, const float size, const Color color) {
            queue.emplace_back(DrawText{ .text = text, .position = pos, .font_face = font_face, .font_size = size, .color = color });
        }

        void draw_texture(const Texture2D& texture, Vector2 pos, Color tint = WHITE) {
            queue.emplace_back(DrawTexture{ .texture = texture, .position = pos, .tint = tint });
        }

        void draw_texture_rec(const Texture2D& texture, Rectangle source, Vector2 pos, Color tint = WHITE) {
            queue.emplace_back(DrawTextureRec{ .texture = texture, .source = source, .position = pos, .tint = tint });
        }

        void draw_poly(Vector2 center, int sides, float radius, float rotation, Color color) {
            queue.emplace_back(DrawPoly{
                .center = center,
                .sides = sides,
                .radius = radius,
                .rotation = rotation,
                .color = color
            });
        }

        // Read access for execution dispatcher
        [[nodiscard]] const std::vector<Command>& get_commands() const {
            return queue;
        }

        // Resets queue size while preserving memory capacity
        void clear() {
            queue.clear();
        }

        [[nodiscard]] bool empty() const {
            return queue.empty();
        }

        [[nodiscard]] size_t size() const {
            return queue.size();
        }


    };

}
