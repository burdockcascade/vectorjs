#include "engine.hpp"

namespace Hooray {
    namespace {
        template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
        template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
    }

    Engine::Engine(const int width, const int height, std::string title)
        : width_(width), height_(height), title_(std::move(title)) {}

    Engine::~Engine() = default;

    void Engine::process_input() const {
        // Keyboard input handling
        if (on_key_pressed_) {
            int key = GetKeyPressed();
            while (key > 0) {
                on_key_pressed_(key);
                key = GetKeyPressed();
            }
        }

        // Mouse click handling
        if (on_mouse_pressed_) {
            const Vector2 mouse_pos = GetMousePosition();
            for (int button = MOUSE_BUTTON_LEFT; button <= MOUSE_BUTTON_BACK; ++button) {
                if (IsMouseButtonPressed(button)) {
                    on_mouse_pressed_(button, mouse_pos);
                }
            }
        }

        // Mouse move handling
        if (on_mouse_move_) {
            Vector2 delta = GetMouseDelta();
            if (delta.x != 0.0f || delta.y != 0.0f) {
                on_mouse_move_(GetMousePosition());
            }
        }

        // Mouse Wheel handling
        if (on_mouse_wheel_move_) {
            Vector2 delta = GetMouseWheelMoveV();
            if (delta.x != 0.0f || delta.y != 0.0f) {
                on_mouse_wheel_move_(GetMousePosition());
            }
        }
    }

    void Engine::run() {
        run_loop(true);
    }

    void Engine::display() {
        run_loop(false);
    }

    void Engine::run_loop(const bool clear_buffer_after_frame) {
        InitWindow(width_, height_, title_.c_str());
        InitAudioDevice();
        SetTargetFPS(60);

        if (on_init_) {
            on_init_();
        }

        while (!WindowShouldClose()) {
            if (clear_buffer_after_frame) command_buffer_.clear();

            process_input();

            if (on_update_) {
                on_update_(GetFrameTime());
            }

            BeginDrawing();

            if (on_draw_) {
                on_draw_();
            }

            execute_commands();

            EndDrawing();
        }

        if (IsAudioDeviceReady()) {
            CloseAudioDevice();
        }
        if (IsWindowReady()) {
            CloseWindow();
        }
    }

    // 2. The refactored execute_commands function
    void Engine::execute_commands() const {
        for (const auto& cmd : command_buffer_.get_commands()) {
            std::visit(overloaded{
                [](const ClearBackground& arg) {
                    ::ClearBackground(arg.color);
                },
                [](const BeginMode2D& arg) {
                    ::BeginMode2D(arg.camera);
                },
                [](const EndMode2D& arg) {
                    ::EndMode2D();
                },
                [](const DrawFPS& arg) {
                    ::DrawFPS(static_cast<int>(arg.position.x), static_cast<int>(arg.position.y));
                },
                [](const DrawPixel& arg) {
                    ::DrawPixelV(arg.position, arg.color);
                },
                [](const DrawLine& arg) {
                    ::DrawLineEx(arg.start, arg.end, arg.thickness, arg.color);
                },
                [](const DrawCircle& arg) {
                    ::DrawCircleV(arg.center, arg.radius, arg.color);
                },
                [](const DrawRectangle& arg) {
                    ::DrawRectangleRec(arg.rect, arg.color);
                },
                [](const DrawTriangle& arg) {
                    ::DrawTriangle(arg.p1, arg.p2, arg.p3, arg.color);
                },
                [](const DrawEllipse& arg) {
                    ::DrawEllipseV(arg.center, arg.radius_h, arg.radius_v, arg.color);
                },
                [](const DrawText& arg) {
                    ::DrawTextEx(arg.font_face, arg.text, arg.position, arg.font_size, arg.spacing, arg.color);
                },
                [](const DrawTexture& arg) {
                    ::DrawTextureV(arg.texture, arg.position, arg.tint);
                },
                [](const DrawTextureRec& arg) {
                    ::DrawTextureRec(arg.texture, arg.source, arg.position, arg.tint);
                },
                [](const DrawTexturePro& arg) {
                    ::DrawTexturePro(arg.texture, arg.source, arg.dest, arg.origin, arg.rotation, arg.tint);
                },
                [](const DrawPoly& arg) {
                    ::DrawPoly(arg.center, arg.sides, arg.radius, arg.rotation, arg.color);
                }
            }, cmd);
        }
    }

}