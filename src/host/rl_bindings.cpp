#include "qjs_wrapper.hpp"
#include <raylib.h>
#include "rl_bindings.hpp"

namespace RaylibBindings {

    static void RegisterRaylibFunctions(const qjs::Engine &engine, qjs::Module &module) {

        // Core Functions
        auto core_obj = engine.make_object();

        // Window Management
        core_obj.set("InitWindow", ::InitWindow);
        core_obj.set("CloseWindow", ::CloseWindow);
        core_obj.set("WindowShouldClose", ::WindowShouldClose);
        core_obj.set("IsWindowReady", ::IsWindowReady);
        core_obj.set("IsWindowFullscreen", ::IsWindowFullscreen);
        core_obj.set("IsWindowHidden", ::IsWindowHidden);
        core_obj.set("IsWindowMinimized", ::IsWindowMinimized);
        core_obj.set("IsWindowMaximized", ::IsWindowMaximized);
        core_obj.set("IsWindowFocused", ::IsWindowFocused);
        core_obj.set("IsWindowResized", ::IsWindowResized);
        core_obj.set("ToggleFullscreen", ::ToggleFullscreen);
        core_obj.set("ToggleBorderlessWindowed", ::ToggleBorderlessWindowed);
        core_obj.set("RestoreWindow", ::RestoreWindow);
        core_obj.set("MaximizeWindow", ::MaximizeWindow);
        core_obj.set("MinimizeWindow", ::MinimizeWindow);
        core_obj.set("SetWindowTitle", ::SetWindowTitle);
        core_obj.set("SetWindowPosition", ::SetWindowPosition);
        core_obj.set("SetWindowMonitor", ::SetWindowMonitor);
        core_obj.set("SetWindowMinSize", ::SetWindowMinSize);
        core_obj.set("SetWindowMaxSize", ::SetWindowMaxSize);
        core_obj.set("SetWindowSize", ::SetWindowSize);
        core_obj.set("SetWindowFocused", ::SetWindowFocused);

        // Window State
        core_obj.set("IsWindowState", ::IsWindowState);
        core_obj.set("SetWindowState", ::SetWindowState);
        core_obj.set("ClearWindowState", ::ClearWindowState);

        // Screen and Display
        core_obj.set("GetScreenWidth", ::GetScreenWidth);
        core_obj.set("GetScreenHeight", ::GetScreenHeight);
        core_obj.set("GetMonitorCount", ::GetMonitorCount);
        core_obj.set("GetMonitorWidth", ::GetMonitorWidth);
        core_obj.set("GetMonitorHeight", ::GetMonitorHeight);
        core_obj.set("GetMonitorPhysicalWidth", ::GetMonitorPhysicalWidth);
        core_obj.set("GetMonitorPhysicalHeight", ::GetMonitorPhysicalHeight);
        core_obj.set("GetMonitorRefreshRate", ::GetMonitorRefreshRate);
        core_obj.set("GetCurrentMonitor", ::GetCurrentMonitor);

        // Keyboard
        core_obj.set("IsKeyPressed", ::IsKeyPressed);
        core_obj.set("IsKeyPressedRepeat", ::IsKeyPressedRepeat);
        core_obj.set("IsKeyDown", ::IsKeyDown);
        core_obj.set("IsKeyReleased", ::IsKeyReleased);
        core_obj.set("IsKeyUp", ::IsKeyUp);
        core_obj.set("GetKeyPressed", ::GetKeyPressed);
        core_obj.set("SetExitKey", ::SetExitKey);

        // Mouse
        core_obj.set("IsMouseButtonPressed", ::IsMouseButtonPressed);
        core_obj.set("IsMouseButtonDown", ::IsMouseButtonDown);
        core_obj.set("IsMouseButtonReleased", ::IsMouseButtonReleased);
        core_obj.set("IsMouseButtonUp", ::IsMouseButtonUp);
        core_obj.set("GetMouseX", ::GetMouseX);
        core_obj.set("GetMouseY", ::GetMouseY);
        core_obj.set("GetMousePosition", ::GetMousePosition);
        core_obj.set("GetMouseDelta", ::GetMouseDelta);
        core_obj.set("SetMousePosition", ::SetMousePosition);
        core_obj.set("SetMouseOffset", ::SetMouseOffset);
        core_obj.set("SetMouseScale", ::SetMouseScale);
        core_obj.set("GetMouseWheelMove", ::GetMouseWheelMove);
        core_obj.set("GetMouseWheelMoveV", ::GetMouseWheelMoveV);

        // Drawing
        core_obj.set("BeginDrawing", ::BeginDrawing);
        core_obj.set("EndDrawing", ::EndDrawing);

        // Timing
        core_obj.set("SetTargetFPS", ::SetTargetFPS);
        core_obj.set("GetFrameTime", ::GetFrameTime);

        // Misc
        core_obj.set("ClearBackground", ::ClearBackground);

        // Initialize Core functions in the module
        module.add("Core", core_obj);

        // SHAPES
        auto shapes_obj = engine.make_object();

        // Rectangle
        shapes_obj.set("DrawRectangle", ::DrawRectangle);
        shapes_obj.set("DrawRectangleLines", ::DrawRectangleLines);
        shapes_obj.set("DrawRectangleV", ::DrawRectangleV);
        shapes_obj.set("DrawRectangleLinesEx", ::DrawRectangleLinesEx);
        shapes_obj.set("DrawRectangleLinesEx", ::DrawRectangleLinesEx);
        shapes_obj.set("DrawRectangleGradientV", ::DrawRectangleGradientV);

        // Circle
        shapes_obj.set("DrawCircle", ::DrawCircle);
        shapes_obj.set("DrawCircleLines", ::DrawCircleLines);
        shapes_obj.set("DrawCircleV", ::DrawCircleV);
        shapes_obj.set("DrawCircleGradient", ::DrawCircleGradient);

        // Line
        shapes_obj.set("DrawLine", ::DrawLine);
        shapes_obj.set("DrawLineEx", ::DrawLineEx);
        shapes_obj.set("DrawLineV", ::DrawLineV);
        shapes_obj.set("DrawLineBezier", ::DrawLineBezier);

        // Initialize Shapes functions in the module
        module.add("Shapes", shapes_obj);

        // TEXT
        auto text_obj = engine.make_object();

        // Text
        text_obj.set("DrawFPS", ::DrawFPS);

        // fixme: this is a bit of a hack to convert the string to a C string, but it works for now. We might want to implement a more robust solution in the future.
        text_obj.set("DrawText", [](const std::string& text, int x, int y, int fontSize, Color color) {
            ::DrawText(text.c_str(), x, y, fontSize, color);
        });

        // Initialize Text functions in the module
        module.add("Text", text_obj);

    }

    static void RegisterRaylibEnums(qjs::Engine &ns, qjs::Module &module) {

        // Enum: ConfigFlags
        auto ConfigFlags_obj = ns.make_object();
        ConfigFlags_obj.set("FLAG_VSYNC_HINT", FLAG_VSYNC_HINT);
        ConfigFlags_obj.set("FLAG_FULLSCREEN_MODE", FLAG_FULLSCREEN_MODE);
        ConfigFlags_obj.set("FLAG_WINDOW_RESIZABLE", FLAG_WINDOW_RESIZABLE);
        ConfigFlags_obj.set("FLAG_WINDOW_UNDECORATED", FLAG_WINDOW_UNDECORATED);
        ConfigFlags_obj.set("FLAG_WINDOW_HIDDEN", FLAG_WINDOW_HIDDEN);
        ConfigFlags_obj.set("FLAG_WINDOW_MINIMIZED", FLAG_WINDOW_MINIMIZED);
        ConfigFlags_obj.set("FLAG_WINDOW_MAXIMIZED", FLAG_WINDOW_MAXIMIZED);
        ConfigFlags_obj.set("FLAG_WINDOW_UNFOCUSED", FLAG_WINDOW_UNFOCUSED);
        ConfigFlags_obj.set("FLAG_WINDOW_TOPMOST", FLAG_WINDOW_TOPMOST);
        ConfigFlags_obj.set("FLAG_WINDOW_ALWAYS_RUN", FLAG_WINDOW_ALWAYS_RUN);
        ConfigFlags_obj.set("FLAG_WINDOW_TRANSPARENT", FLAG_WINDOW_TRANSPARENT);
        ConfigFlags_obj.set("FLAG_WINDOW_HIGHDPI", FLAG_WINDOW_HIGHDPI);
        ConfigFlags_obj.set("FLAG_WINDOW_MOUSE_PASSTHROUGH", FLAG_WINDOW_MOUSE_PASSTHROUGH);
        ConfigFlags_obj.set("FLAG_BORDERLESS_WINDOWED_MODE", FLAG_BORDERLESS_WINDOWED_MODE);
        ConfigFlags_obj.set("FLAG_MSAA_4X_HINT", FLAG_MSAA_4X_HINT);
        ConfigFlags_obj.set("FLAG_INTERLACED_HINT", FLAG_INTERLACED_HINT);
        module.add("ConfigFlags", ConfigFlags_obj);

        // Enum: KeyboardKey
        auto KeyboardKey_obj = ns.make_object();
        KeyboardKey_obj.set("KEY_NULL", KEY_NULL);
        KeyboardKey_obj.set("KEY_APOSTROPHE", KEY_APOSTROPHE);
        KeyboardKey_obj.set("KEY_COMMA", KEY_COMMA);
        KeyboardKey_obj.set("KEY_MINUS", KEY_MINUS);
        KeyboardKey_obj.set("KEY_PERIOD", KEY_PERIOD);
        KeyboardKey_obj.set("KEY_SLASH", KEY_SLASH);
        KeyboardKey_obj.set("KEY_ZERO", KEY_ZERO);
        KeyboardKey_obj.set("KEY_ONE", KEY_ONE);
        KeyboardKey_obj.set("KEY_TWO", KEY_TWO);
        KeyboardKey_obj.set("KEY_THREE", KEY_THREE);
        KeyboardKey_obj.set("KEY_FOUR", KEY_FOUR);
        KeyboardKey_obj.set("KEY_FIVE", KEY_FIVE);
        KeyboardKey_obj.set("KEY_SIX", KEY_SIX);
        KeyboardKey_obj.set("KEY_SEVEN", KEY_SEVEN);
        KeyboardKey_obj.set("KEY_EIGHT", KEY_EIGHT);
        KeyboardKey_obj.set("KEY_NINE", KEY_NINE);
        KeyboardKey_obj.set("KEY_SEMICOLON", KEY_SEMICOLON);
        KeyboardKey_obj.set("KEY_EQUAL", KEY_EQUAL);
        KeyboardKey_obj.set("KEY_A", KEY_A);
        KeyboardKey_obj.set("KEY_B", KEY_B);
        KeyboardKey_obj.set("KEY_C", KEY_C);
        KeyboardKey_obj.set("KEY_D", KEY_D);
        KeyboardKey_obj.set("KEY_E", KEY_E);
        KeyboardKey_obj.set("KEY_F", KEY_F);
        KeyboardKey_obj.set("KEY_G", KEY_G);
        KeyboardKey_obj.set("KEY_H", KEY_H);
        KeyboardKey_obj.set("KEY_I", KEY_I);
        KeyboardKey_obj.set("KEY_J", KEY_J);
        KeyboardKey_obj.set("KEY_K", KEY_K);
        KeyboardKey_obj.set("KEY_L", KEY_L);
        KeyboardKey_obj.set("KEY_M", KEY_M);
        KeyboardKey_obj.set("KEY_N", KEY_N);
        KeyboardKey_obj.set("KEY_O", KEY_O);
        KeyboardKey_obj.set("KEY_P", KEY_P);
        KeyboardKey_obj.set("KEY_Q", KEY_Q);
        KeyboardKey_obj.set("KEY_R", KEY_R);
        KeyboardKey_obj.set("KEY_S", KEY_S);
        KeyboardKey_obj.set("KEY_T", KEY_T);
        KeyboardKey_obj.set("KEY_U", KEY_U);
        KeyboardKey_obj.set("KEY_V", KEY_V);
        KeyboardKey_obj.set("KEY_W", KEY_W);
        KeyboardKey_obj.set("KEY_X", KEY_X);
        KeyboardKey_obj.set("KEY_Y", KEY_Y);
        KeyboardKey_obj.set("KEY_Z", KEY_Z);
        KeyboardKey_obj.set("KEY_LEFT_BRACKET", KEY_LEFT_BRACKET);
        KeyboardKey_obj.set("KEY_BACKSLASH", KEY_BACKSLASH);
        KeyboardKey_obj.set("KEY_RIGHT_BRACKET", KEY_RIGHT_BRACKET);
        KeyboardKey_obj.set("KEY_GRAVE", KEY_GRAVE);
        KeyboardKey_obj.set("KEY_SPACE", KEY_SPACE);
        KeyboardKey_obj.set("KEY_ESCAPE", KEY_ESCAPE);
        KeyboardKey_obj.set("KEY_ENTER", KEY_ENTER);
        KeyboardKey_obj.set("KEY_TAB", KEY_TAB);
        KeyboardKey_obj.set("KEY_BACKSPACE", KEY_BACKSPACE);
        KeyboardKey_obj.set("KEY_INSERT", KEY_INSERT);
        KeyboardKey_obj.set("KEY_DELETE", KEY_DELETE);
        KeyboardKey_obj.set("KEY_RIGHT", KEY_RIGHT);
        KeyboardKey_obj.set("KEY_LEFT", KEY_LEFT);
        KeyboardKey_obj.set("KEY_DOWN", KEY_DOWN);
        KeyboardKey_obj.set("KEY_UP", KEY_UP);
        KeyboardKey_obj.set("KEY_PAGE_UP", KEY_PAGE_UP);
        KeyboardKey_obj.set("KEY_PAGE_DOWN", KEY_PAGE_DOWN);
        KeyboardKey_obj.set("KEY_HOME", KEY_HOME);
        KeyboardKey_obj.set("KEY_END", KEY_END);
        KeyboardKey_obj.set("KEY_CAPS_LOCK", KEY_CAPS_LOCK);
        KeyboardKey_obj.set("KEY_SCROLL_LOCK", KEY_SCROLL_LOCK);
        KeyboardKey_obj.set("KEY_NUM_LOCK", KEY_NUM_LOCK);
        KeyboardKey_obj.set("KEY_PRINT_SCREEN", KEY_PRINT_SCREEN);
        KeyboardKey_obj.set("KEY_PAUSE", KEY_PAUSE);
        KeyboardKey_obj.set("KEY_F1", KEY_F1);
        KeyboardKey_obj.set("KEY_F2", KEY_F2);
        KeyboardKey_obj.set("KEY_F3", KEY_F3);
        KeyboardKey_obj.set("KEY_F4", KEY_F4);
        KeyboardKey_obj.set("KEY_F5", KEY_F5);
        KeyboardKey_obj.set("KEY_F6", KEY_F6);
        KeyboardKey_obj.set("KEY_F7", KEY_F7);
        KeyboardKey_obj.set("KEY_F8", KEY_F8);
        KeyboardKey_obj.set("KEY_F9", KEY_F9);
        KeyboardKey_obj.set("KEY_F10", KEY_F10);
        KeyboardKey_obj.set("KEY_F11", KEY_F11);
        KeyboardKey_obj.set("KEY_F12", KEY_F12);
        KeyboardKey_obj.set("KEY_LEFT_SHIFT", KEY_LEFT_SHIFT);
        KeyboardKey_obj.set("KEY_LEFT_CONTROL", KEY_LEFT_CONTROL);
        KeyboardKey_obj.set("KEY_LEFT_ALT", KEY_LEFT_ALT);
        KeyboardKey_obj.set("KEY_LEFT_SUPER", KEY_LEFT_SUPER);
        KeyboardKey_obj.set("KEY_RIGHT_SHIFT", KEY_RIGHT_SHIFT);
        KeyboardKey_obj.set("KEY_RIGHT_CONTROL", KEY_RIGHT_CONTROL);
        KeyboardKey_obj.set("KEY_RIGHT_ALT", KEY_RIGHT_ALT);
        KeyboardKey_obj.set("KEY_RIGHT_SUPER", KEY_RIGHT_SUPER);
        KeyboardKey_obj.set("KEY_KB_MENU", KEY_KB_MENU);
        KeyboardKey_obj.set("KEY_KP_0", KEY_KP_0);
        KeyboardKey_obj.set("KEY_KP_1", KEY_KP_1);
        KeyboardKey_obj.set("KEY_KP_2", KEY_KP_2);
        KeyboardKey_obj.set("KEY_KP_3", KEY_KP_3);
        KeyboardKey_obj.set("KEY_KP_4", KEY_KP_4);
        KeyboardKey_obj.set("KEY_KP_5", KEY_KP_5);
        KeyboardKey_obj.set("KEY_KP_6", KEY_KP_6);
        KeyboardKey_obj.set("KEY_KP_7", KEY_KP_7);
        KeyboardKey_obj.set("KEY_KP_8", KEY_KP_8);
        KeyboardKey_obj.set("KEY_KP_9", KEY_KP_9);
        KeyboardKey_obj.set("KEY_KP_DECIMAL", KEY_KP_DECIMAL);
        KeyboardKey_obj.set("KEY_KP_DIVIDE", KEY_KP_DIVIDE);
        KeyboardKey_obj.set("KEY_KP_MULTIPLY", KEY_KP_MULTIPLY);
        KeyboardKey_obj.set("KEY_KP_SUBTRACT", KEY_KP_SUBTRACT);
        KeyboardKey_obj.set("KEY_KP_ADD", KEY_KP_ADD);
        KeyboardKey_obj.set("KEY_KP_ENTER", KEY_KP_ENTER);
        KeyboardKey_obj.set("KEY_KP_EQUAL", KEY_KP_EQUAL);
        KeyboardKey_obj.set("KEY_BACK", KEY_BACK);
        KeyboardKey_obj.set("KEY_MENU", KEY_MENU);
        KeyboardKey_obj.set("KEY_VOLUME_UP", KEY_VOLUME_UP);
        KeyboardKey_obj.set("KEY_VOLUME_DOWN", KEY_VOLUME_DOWN);
        module.add("KeyboardKey", KeyboardKey_obj);

        // Enum: MouseButton
        auto MouseButton_obj = ns.make_object();
        MouseButton_obj.set("MOUSE_BUTTON_LEFT", MOUSE_BUTTON_LEFT);
        MouseButton_obj.set("MOUSE_BUTTON_RIGHT", MOUSE_BUTTON_RIGHT);
        MouseButton_obj.set("MOUSE_BUTTON_MIDDLE", MOUSE_BUTTON_MIDDLE);
        MouseButton_obj.set("MOUSE_BUTTON_SIDE", MOUSE_BUTTON_SIDE);
        MouseButton_obj.set("MOUSE_BUTTON_EXTRA", MOUSE_BUTTON_EXTRA);
        MouseButton_obj.set("MOUSE_BUTTON_FORWARD", MOUSE_BUTTON_FORWARD);
        MouseButton_obj.set("MOUSE_BUTTON_BACK", MOUSE_BUTTON_BACK);
        module.add("MouseButton", MouseButton_obj);

        // Enum: MouseCursor
        auto MouseCursor_obj = ns.make_object();
        MouseCursor_obj.set("MOUSE_CURSOR_DEFAULT", MOUSE_CURSOR_DEFAULT);
        MouseCursor_obj.set("MOUSE_CURSOR_ARROW", MOUSE_CURSOR_ARROW);
        MouseCursor_obj.set("MOUSE_CURSOR_IBEAM", MOUSE_CURSOR_IBEAM);
        MouseCursor_obj.set("MOUSE_CURSOR_CROSSHAIR", MOUSE_CURSOR_CROSSHAIR);
        MouseCursor_obj.set("MOUSE_CURSOR_POINTING_HAND", MOUSE_CURSOR_POINTING_HAND);
        MouseCursor_obj.set("MOUSE_CURSOR_RESIZE_EW", MOUSE_CURSOR_RESIZE_EW);
        MouseCursor_obj.set("MOUSE_CURSOR_RESIZE_NS", MOUSE_CURSOR_RESIZE_NS);
        MouseCursor_obj.set("MOUSE_CURSOR_RESIZE_NWSE", MOUSE_CURSOR_RESIZE_NWSE);
        MouseCursor_obj.set("MOUSE_CURSOR_RESIZE_NESW", MOUSE_CURSOR_RESIZE_NESW);
        MouseCursor_obj.set("MOUSE_CURSOR_RESIZE_ALL", MOUSE_CURSOR_RESIZE_ALL);
        MouseCursor_obj.set("MOUSE_CURSOR_NOT_ALLOWED", MOUSE_CURSOR_NOT_ALLOWED);
        module.add("MouseCursor", MouseCursor_obj);

        // Enum: GamepadButton
        auto GamepadButton_obj = ns.make_object();
        GamepadButton_obj.set("GAMEPAD_BUTTON_UNKNOWN", GAMEPAD_BUTTON_UNKNOWN);
        GamepadButton_obj.set("GAMEPAD_BUTTON_LEFT_FACE_UP", GAMEPAD_BUTTON_LEFT_FACE_UP);
        GamepadButton_obj.set("GAMEPAD_BUTTON_LEFT_FACE_RIGHT", GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
        GamepadButton_obj.set("GAMEPAD_BUTTON_LEFT_FACE_DOWN", GAMEPAD_BUTTON_LEFT_FACE_DOWN);
        GamepadButton_obj.set("GAMEPAD_BUTTON_LEFT_FACE_LEFT", GAMEPAD_BUTTON_LEFT_FACE_LEFT);
        GamepadButton_obj.set("GAMEPAD_BUTTON_RIGHT_FACE_UP", GAMEPAD_BUTTON_RIGHT_FACE_UP);
        GamepadButton_obj.set("GAMEPAD_BUTTON_RIGHT_FACE_RIGHT", GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
        GamepadButton_obj.set("GAMEPAD_BUTTON_RIGHT_FACE_DOWN", GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
        GamepadButton_obj.set("GAMEPAD_BUTTON_RIGHT_FACE_LEFT", GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
        GamepadButton_obj.set("GAMEPAD_BUTTON_LEFT_TRIGGER_1", GAMEPAD_BUTTON_LEFT_TRIGGER_1);
        GamepadButton_obj.set("GAMEPAD_BUTTON_LEFT_TRIGGER_2", GAMEPAD_BUTTON_LEFT_TRIGGER_2);
        GamepadButton_obj.set("GAMEPAD_BUTTON_RIGHT_TRIGGER_1", GAMEPAD_BUTTON_RIGHT_TRIGGER_1);
        GamepadButton_obj.set("GAMEPAD_BUTTON_RIGHT_TRIGGER_2", GAMEPAD_BUTTON_RIGHT_TRIGGER_2);
        GamepadButton_obj.set("GAMEPAD_BUTTON_MIDDLE_LEFT", GAMEPAD_BUTTON_MIDDLE_LEFT);
        GamepadButton_obj.set("GAMEPAD_BUTTON_MIDDLE", GAMEPAD_BUTTON_MIDDLE);
        GamepadButton_obj.set("GAMEPAD_BUTTON_MIDDLE_RIGHT", GAMEPAD_BUTTON_MIDDLE_RIGHT);
        GamepadButton_obj.set("GAMEPAD_BUTTON_LEFT_THUMB", GAMEPAD_BUTTON_LEFT_THUMB);
        GamepadButton_obj.set("GAMEPAD_BUTTON_RIGHT_THUMB", GAMEPAD_BUTTON_RIGHT_THUMB);
        module.add("GamepadButton", GamepadButton_obj);

        // Enum: GamepadAxis
        auto GamepadAxis_obj = ns.make_object();
        GamepadAxis_obj.set("GAMEPAD_AXIS_LEFT_X", GAMEPAD_AXIS_LEFT_X);
        GamepadAxis_obj.set("GAMEPAD_AXIS_LEFT_Y", GAMEPAD_AXIS_LEFT_Y);
        GamepadAxis_obj.set("GAMEPAD_AXIS_RIGHT_X", GAMEPAD_AXIS_RIGHT_X);
        GamepadAxis_obj.set("GAMEPAD_AXIS_RIGHT_Y", GAMEPAD_AXIS_RIGHT_Y);
        GamepadAxis_obj.set("GAMEPAD_AXIS_LEFT_TRIGGER", GAMEPAD_AXIS_LEFT_TRIGGER);
        GamepadAxis_obj.set("GAMEPAD_AXIS_RIGHT_TRIGGER", GAMEPAD_AXIS_RIGHT_TRIGGER);
        module.add("GamepadAxis", GamepadAxis_obj);

        // Enum: CameraMode
        auto CameraMode_obj = ns.make_object();
        CameraMode_obj.set("CAMERA_CUSTOM", CAMERA_CUSTOM);
        CameraMode_obj.set("CAMERA_FREE", CAMERA_FREE);
        CameraMode_obj.set("CAMERA_ORBITAL", CAMERA_ORBITAL);
        CameraMode_obj.set("CAMERA_FIRST_PERSON", CAMERA_FIRST_PERSON);
        CameraMode_obj.set("CAMERA_THIRD_PERSON", CAMERA_THIRD_PERSON);
        module.add("CameraMode", CameraMode_obj);

        // Enum: CameraProjection
        auto CameraProjection_obj = ns.make_object();
        CameraProjection_obj.set("CAMERA_PERSPECTIVE", CAMERA_PERSPECTIVE);
        CameraProjection_obj.set("CAMERA_ORTHOGRAPHIC", CAMERA_ORTHOGRAPHIC);
        module.add("CameraProjection", CameraProjection_obj);

    }

    void InitRaylib(qjs::Engine &engine) {

        SetRandomSeed(static_cast<unsigned int>(time(nullptr)));

        // Create the module and get a reference to it for further exports
        auto raylib_mod = engine.define_module("Raylib");

        // Initialize all function groups in the module
        RegisterRaylibFunctions(engine, raylib_mod);

        // Initialize all enums in the module
        RegisterRaylibEnums(engine, raylib_mod);

    }

}