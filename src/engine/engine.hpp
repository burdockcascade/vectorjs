#pragma once

#include <functional>
#include <string>
#include "commands.hpp"
#include "raylib.h"

namespace Hooray {

    class Engine {
    public:

        using Callback = std::function<void()>;
        using UpdateCallback = std::function<void(float)>;
        using KeyCallback = std::function<void(int key)>;
        using MouseButtonCallback = std::function<void(int button, Vector2 pos)>;
        using MouseMoveCallback = std::function<void(Vector2 pos)>;
        using MouseWheelCallback = std::function<void(Vector2 delta)>;
        using MouseButtonCallback = std::function<void(int button, Vector2 pos)>;

        Engine(int width, int height, std::string title);
        ~Engine();

        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;

        // Existing callback setters
        void set_on_init(Callback callback) { on_init_ = std::move(callback); }
        void set_on_update(UpdateCallback callback) { on_update_ = std::move(callback); }
        void set_on_draw(Callback callback) { on_draw_ = std::move(callback); }

        // New Input callback setters
        void set_on_key_pressed(KeyCallback callback) { on_key_pressed_ = std::move(callback); }
        void set_on_mouse_pressed(MouseButtonCallback callback) { on_mouse_pressed_ = std::move(callback); }
        void set_on_mouse_move(MouseMoveCallback callback) { on_mouse_move_ = std::move(callback); }
        void set_on_mousewheel_move(MouseWheelCallback callback) { on_mouse_wheel_move_ = std::move(callback); }
        void set_on_mouse_released(MouseButtonCallback callback) { on_mouse_released_ = std::move(callback); }

        void run();

        void display();

        void run_loop(bool clear_buffer_after_frame);

        CommandBufferBuilder& get_buffer() { return command_buffer_; }

    private:
        int width_;
        int height_;
        std::string title_;
        CommandBufferBuilder command_buffer_{ 1024 };

        Callback on_init_;
        UpdateCallback on_update_;
        Callback on_draw_;

        // New callback variables
        KeyCallback on_key_pressed_;
        MouseButtonCallback on_mouse_pressed_;
        MouseMoveCallback on_mouse_move_;
        MouseWheelCallback on_mouse_wheel_move_;
        MouseButtonCallback on_mouse_released_;

        void process_input() const;
        void execute_commands() const;
    };

}