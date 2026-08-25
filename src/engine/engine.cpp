#include "engine.hpp"
#include <ranges>
#include <variant>
#include "raylib.h"

namespace Hooray {

    namespace {
        template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
        template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
    }

    // ==========================================
    // LifecycleManager Implementation
    // ==========================================

    void LifecycleManager::add_listener(const std::shared_ptr<LifecycleListener>& listener) {
        listeners_.push_back(listener);
    }

    void LifecycleManager::remove_listener(const std::shared_ptr<LifecycleListener>& listener) {
        std::erase(listeners_, listener);
    }

    void LifecycleManager::notify_init() {
        for (auto& listener : listeners_) {
            listener->on_init();
        }
    }

    void LifecycleManager::notify_update(const float delta_time) {
        for (auto& listener : listeners_) {
            listener->on_update(delta_time);
        }
    }

    void LifecycleManager::notify_draw() {
        for (auto& listener : listeners_) {
            listener->on_draw();
        }
    }

    // ==========================================
    // InputManager Implementation
    // ==========================================

    void InputManager::add_listener(const std::shared_ptr<InputListener>& listener) {
        listeners_.push_back(listener);
    }

    void InputManager::remove_listener(const std::shared_ptr<InputListener>& listener) {
        std::erase(listeners_, listener);
    }

    bool InputManager::is_key_down(const int key) {
        return IsKeyDown(key);
    }

    bool InputManager::is_mouse_button_down(const int button) {
        return IsMouseButtonDown(button);
    }

    Vector2 InputManager::get_mouse_position() {
        return GetMousePosition();
    }

    Vector2 InputManager::get_mouse_delta() {
        return GetMouseDelta();
    }

    void InputManager::process_frame() {
        // 1. Keyboard Events
        int key = GetKeyPressed();
        while (key > 0) {
            dispatch_key_pressed(key);
            key = GetKeyPressed();
        }

        // 2. Mouse Press & Release Events
        const Vector2 mouse_pos = GetMousePosition();
        for (int button = MOUSE_BUTTON_LEFT; button <= MOUSE_BUTTON_BACK; ++button) {
            if (IsMouseButtonPressed(button)) {
                dispatch_mouse_pressed(button, mouse_pos);
            }
            if (IsMouseButtonReleased(button)) {
                dispatch_mouse_released(button, mouse_pos);
            }
        }

        // 3. Mouse Movement Events
        const Vector2 delta = GetMouseDelta();
        if (delta.x != 0.0f || delta.y != 0.0f) {
            dispatch_mouse_moved(mouse_pos);
        }
    }

    void InputManager::dispatch_key_pressed(const int key) {
        for (auto& listener : std::views::reverse(listeners_)) {
            if (listener->on_key_pressed(key)) break;
        }
    }

    void InputManager::dispatch_mouse_pressed(const int button, const Vector2 pos) {
        for (auto& listener : std::views::reverse(listeners_)) {
            if (listener->on_mouse_pressed(button, pos)) break;
        }
    }

    void InputManager::dispatch_mouse_released(const int button, const Vector2 pos) {
        for (auto& listener : std::views::reverse(listeners_)) {
            if (listener->on_mouse_released(button, pos)) break;
        }
    }

    void InputManager::dispatch_mouse_moved(const Vector2 pos) {
        for (auto& listener : std::views::reverse(listeners_)) {
            if (listener->on_mouse_moved(pos)) break;
        }
    }

    // ==========================================
    // Engine Implementation
    // ==========================================

    Engine::Engine(const int width, const int height, std::string title)
        : width_(width), height_(height), title_(std::move(title)) {}

    Engine::~Engine() = default;

    void Engine::set_on_init(Callback callback) {
        on_init_ = std::move(callback);
    }

    void Engine::set_on_update(UpdateCallback callback) {
        on_update_ = std::move(callback);
    }

    void Engine::set_on_draw(Callback callback) {
        on_draw_ = std::move(callback);
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

        lifecycle_manager_.notify_init();

        while (!WindowShouldClose()) {
            if (clear_buffer_after_frame) command_buffer_.clear();

            const float delta_time = GetFrameTime();

            // Update
            input_manager_.process_frame();
            lifecycle_manager_.notify_update(delta_time);

            // Draw
            BeginDrawing();
            lifecycle_manager_.notify_draw();
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
            std::visit(overloaded{
                [](const ClearBackground& arg) {
                    ::ClearBackground(arg.color);
                },
                [](const BeginMode2D& arg) {
                    ::BeginMode2D(arg.camera);
                },
                [](const EndMode2D&) {
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
                    ::DrawTextEx(arg.font_face, arg.text.c_str(), arg.position, arg.font_size, arg.spacing, arg.color);
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