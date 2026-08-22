#pragma once

#include <functional>
#include <string>
#include "commands.hpp"
#include "raylib.h"

namespace Engine {

    class Engine {
    public:

        using Callback = std::function<void()>;
        using UpdateCallback = std::function<void(float)>;
        using KeyCallback = std::function<void(int key)>;               // Key pressed event
        using MouseButtonCallback = std::function<void(int button, Vector2 pos)>; // Mouse click event
        using MouseMoveCallback = std::function<void(Vector2 pos)>;    // Mouse movement event

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

        void run();
        void display() const;

        Commands::CommandBufferBuilder& get_buffer() { return command_buffer_; }

    private:
        int width_;
        int height_;
        std::string title_;
        Commands::CommandBufferBuilder command_buffer_{ 1024 };

        Callback on_init_;
        UpdateCallback on_update_;
        Callback on_draw_;

        // New callback variables
        KeyCallback on_key_pressed_;
        MouseButtonCallback on_mouse_pressed_;
        MouseMoveCallback on_mouse_move_;

        void process_input() const;
        void execute_commands() const;
    };

}