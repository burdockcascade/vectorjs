#include "engine.hpp"

namespace Hooray {

    Engine::Engine(int width, int height, std::string title)
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
            Vector2 mouse_pos = GetMousePosition();
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
        InitWindow(width_, height_, title_.c_str());
        InitAudioDevice();
        SetTargetFPS(60);

        if (on_init_) {
            on_init_();
        }

        while (!WindowShouldClose()) {
            command_buffer_.clear();

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

    void Engine::display() const {
        InitWindow(width_, height_, title_.c_str());
        InitAudioDevice();
        SetTargetFPS(60);

        if (on_init_) {
            on_init_();
        }

        while (!WindowShouldClose()) {

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

    void Engine::execute_commands() const {
        for (const auto& cmd : command_buffer_.get_commands()) {
            std::visit([](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;

                if constexpr (std::is_same_v<T, Hooray::ClearBackground>) {
                    ClearBackground(arg.color);
                }
                else if constexpr (std::is_same_v<T, Hooray::SetViewport>) {
                    // Uses Raylib scissor mode to bound viewport rendering statefully
                    BeginScissorMode(
                        static_cast<int>(arg.bounds.x),
                        static_cast<int>(arg.bounds.y),
                        static_cast<int>(arg.bounds.width),
                        static_cast<int>(arg.bounds.height)
                    );
                }
                else if constexpr (std::is_same_v<T, Hooray::SetMatrix>) {
                    // Set custom matrix transformation if needed
                }
                else if constexpr (std::is_same_v<T, Hooray::DrawPixel>) {
                    DrawPixelV(arg.position, arg.color);
                }
                else if constexpr (std::is_same_v<T, Hooray::DrawLine>) {
                    DrawLineEx(arg.start, arg.end, arg.thickness, arg.color);
                }
                else if constexpr (std::is_same_v<T, Hooray::DrawCircle>) {
                    DrawCircleV(arg.center, arg.radius, arg.color);
                }
                else if constexpr (std::is_same_v<T, Hooray::DrawRectangle>) {
                    DrawRectangleRec(arg.rect, arg.color);
                }
                else if constexpr (std::is_same_v<T, Hooray::DrawTriangle>) {
                    DrawTriangle(arg.p1, arg.p2, arg.p3, arg.color);
                }
                else if constexpr (std::is_same_v<T, Hooray::DrawEllipse>) {
                    DrawEllipseV(arg.center, arg.radius_h, arg.radius_v, arg.color);
                }
                else if constexpr (std::is_same_v<T, Hooray::DrawText>) {
                    DrawTextEx(GetFontDefault(), arg.text, arg.position, arg.font_size, 1.0f, arg.color);
                }
            }, cmd);
        }
    }

}