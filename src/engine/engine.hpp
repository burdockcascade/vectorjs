#pragma once

#include <functional>
#include <memory>
#include <ranges>
#include <string>
#include "commands.hpp"
#include "raylib.h"

namespace Hooray {

    class LifecycleListener {
    public:
        virtual ~LifecycleListener() = default;
        virtual bool on_init() { return false; }
        virtual bool on_update(float delta_time) { return false; }
        virtual bool on_draw() { return false; }
    };

    class LifecycleManager {
    public:
        void add_listener(const std::shared_ptr<LifecycleListener>& listener) {
            listeners_.push_back(listener);
        }

        void remove_listener(const std::shared_ptr<LifecycleListener>& listener) {
            std::erase(listeners_, listener);
        }

        void notify_init() {
            for (auto& listener : listeners_) {
                listener->on_init();
            }
        }

        void notify_update(float delta_time) {
            for (auto& listener : listeners_) {
                listener->on_update(delta_time);
            }
        }

        void notify_draw() {
            for (auto& listener : listeners_) {
                listener->on_draw();
            }
        }

    private:
        std::vector<std::shared_ptr<LifecycleListener>> listeners_;
    };

    class InputListener {
    public:
        virtual ~InputListener() = default;
        virtual bool on_key_pressed(int key) { return false; }
        virtual bool on_mouse_pressed(int button, Vector2 pos) { return false; }
        virtual bool on_mouse_released(int button, Vector2 pos) { return false; }
        virtual bool on_mouse_moved(Vector2 pos) { return false; }
    };

    class InputManager {
    public:
        void add_listener(const std::shared_ptr<InputListener>& listener) {
            listeners_.push_back(listener);
        }

        void remove_listener(const std::shared_ptr<InputListener>& listener) {
            std::erase(listeners_, listener);
        }

        // --- State Polling API ---
        static bool is_key_down(int key) { return IsKeyDown(key); }
        static bool is_mouse_button_down(int button) { return IsMouseButtonDown(button); }
        static Vector2 get_mouse_position() { return GetMousePosition(); }
        static Vector2 get_mouse_delta() { return GetMouseDelta(); }

        // --- Process Discrete Events (Called once per frame) ---
        void process_frame() {
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

    private:
        std::vector<std::shared_ptr<InputListener>> listeners_;

        void dispatch_key_pressed(int key) {
            for (auto& listener : std::views::reverse(listeners_)) {
                if (listener->on_key_pressed(key)) break;
            }
        }

        void dispatch_mouse_pressed(int button, Vector2 pos) {
            for (auto& listener : std::views::reverse(listeners_)) {
                if (listener->on_mouse_pressed(button, pos)) break;
            }
        }

        void dispatch_mouse_released(int button, Vector2 pos) {
            for (auto& listener : std::views::reverse(listeners_)) {
                if (listener->on_mouse_released(button, pos)) break;
            }
        }

        void dispatch_mouse_moved(Vector2 pos) {
            for (auto& listener : std::views::reverse(listeners_)) {
                if (listener->on_mouse_moved(pos)) break;
            }
        }
    };

    class Engine {
    public:

        using Callback = std::function<void()>;
        using UpdateCallback = std::function<void(float)>;
        using KeyCallback = std::function<void(int key)>;
        using MouseButtonCallback = std::function<void(int button, Vector2 pos)>;
        using MouseMoveCallback = std::function<void(Vector2 pos)>;
        using MouseWheelCallback = std::function<void(Vector2 delta)>;

        Engine(int width, int height, std::string title);
        ~Engine();

        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;

        // Existing callback setters
        void set_on_init(Callback callback) { on_init_ = std::move(callback); }
        void set_on_update(UpdateCallback callback) { on_update_ = std::move(callback); }
        void set_on_draw(Callback callback) { on_draw_ = std::move(callback); }

        // Access to the Input System
        InputManager& get_input_manager() { return input_manager_; }
        [[nodiscard]] const InputManager& get_input_manager() const { return input_manager_; }

        LifecycleManager& get_lifecycle_manager() { return lifecycle_manager_; }
        [[nodiscard]] const LifecycleManager& get_lifecycle_manager() const { return lifecycle_manager_; }

        void run();

        void display();

        void run_loop(bool clear_buffer_after_frame);

        CommandBufferBuilder& get_buffer() { return command_buffer_; }

    private:
        int width_;
        int height_;
        std::string title_;
        CommandBufferBuilder command_buffer_{ 1024 };

        LifecycleManager lifecycle_manager_;
        InputManager input_manager_;

        Callback on_init_;
        UpdateCallback on_update_;
        Callback on_draw_;

        // New callback variables
        KeyCallback on_key_pressed_;
        MouseButtonCallback on_mouse_pressed_;
        MouseMoveCallback on_mouse_move_;
        MouseWheelCallback on_mouse_wheel_move_;
        MouseButtonCallback on_mouse_released_;

        void execute_commands() const;
    };

}