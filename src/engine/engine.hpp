#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "commands.hpp"

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
        void add_listener(const std::shared_ptr<LifecycleListener>& listener);
        void remove_listener(const std::shared_ptr<LifecycleListener>& listener);

        void notify_init();
        void notify_update(float delta_time);
        void notify_draw();

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
        void add_listener(const std::shared_ptr<InputListener>& listener);
        void remove_listener(const std::shared_ptr<InputListener>& listener);

        // --- State Polling API ---
        static bool is_key_down(int key);
        static bool is_mouse_button_down(int button);
        static Vector2 get_mouse_position();
        static Vector2 get_mouse_delta();

        // --- Process Discrete Events (Called once per frame) ---
        void process_frame();

    private:
        std::vector<std::shared_ptr<InputListener>> listeners_;

        void dispatch_key_pressed(int key);
        void dispatch_mouse_pressed(int button, Vector2 pos);
        void dispatch_mouse_released(int button, Vector2 pos);
        void dispatch_mouse_moved(Vector2 pos);
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

        // Callback setters
        void set_on_init(Callback callback);
        void set_on_update(UpdateCallback callback);
        void set_on_draw(Callback callback);

        // Access to Managers
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

        KeyCallback on_key_pressed_;
        MouseButtonCallback on_mouse_pressed_;
        MouseMoveCallback on_mouse_move_;
        MouseWheelCallback on_mouse_wheel_move_;
        MouseButtonCallback on_mouse_released_;

        void execute_commands() const;
    };

}