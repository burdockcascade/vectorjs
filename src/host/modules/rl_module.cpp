#include "../qjs_wrapper.hpp"
#include <raylib.h>
#include "rl_module.hpp"

namespace RaylibModule {

    static void RegisterRaylibFunctions(const qjs::Engine &engine, qjs::Module &module) {

        // Core functions
        auto core_obj = engine.make_object()
            .set("InitWindow", ::InitWindow)
            .set("CloseWindow", ::CloseWindow)
            .set("WindowShouldClose", ::WindowShouldClose)
            .set("IsWindowReady", ::IsWindowReady)
            .set("IsWindowFullscreen", ::IsWindowFullscreen)
            .set("IsWindowHidden", ::IsWindowHidden)
            .set("IsWindowMinimized", ::IsWindowMinimized)
            .set("IsWindowMaximized", ::IsWindowMaximized)
            .set("IsWindowFocused", ::IsWindowFocused)
            .set("IsWindowResized", ::IsWindowResized)
            .set("ToggleFullscreen", ::ToggleFullscreen)
            .set("ToggleBorderlessWindowed", ::ToggleBorderlessWindowed)
            .set("RestoreWindow", ::RestoreWindow)
            .set("MaximizeWindow", ::MaximizeWindow)
            .set("MinimizeWindow", ::MinimizeWindow)
            .set("SetWindowTitle", ::SetWindowTitle)
            .set("SetWindowPosition", ::SetWindowPosition)
            .set("SetWindowMonitor", ::SetWindowMonitor)
            .set("SetWindowMinSize", ::SetWindowMinSize)
            .set("SetWindowMaxSize", ::SetWindowMaxSize)
            .set("SetWindowSize", ::SetWindowSize)
            .set("SetWindowFocused", ::SetWindowFocused)
            .set("IsWindowState", ::IsWindowState)
            .set("SetWindowState", ::SetWindowState)
            .set("ClearWindowState", ::ClearWindowState)
            .set("GetScreenWidth", ::GetScreenWidth)
            .set("GetScreenHeight", ::GetScreenHeight)
            .set("GetMonitorCount", ::GetMonitorCount)
            .set("GetMonitorWidth", ::GetMonitorWidth)
            .set("GetMonitorHeight", ::GetMonitorHeight)
            .set("GetMonitorPhysicalWidth", ::GetMonitorPhysicalWidth)
            .set("GetMonitorPhysicalHeight", ::GetMonitorPhysicalHeight)
            .set("GetMonitorRefreshRate", ::GetMonitorRefreshRate)
            .set("GetCurrentMonitor", ::GetCurrentMonitor)
            .set("IsKeyPressed", ::IsKeyPressed)
            .set("IsKeyPressedRepeat", ::IsKeyPressedRepeat)
            .set("IsKeyDown", ::IsKeyDown)
            .set("IsKeyReleased", ::IsKeyReleased)
            .set("IsKeyUp", ::IsKeyUp)
            .set("GetKeyPressed", ::GetKeyPressed)
            .set("SetExitKey", ::SetExitKey)
            .set("IsMouseButtonPressed", ::IsMouseButtonPressed)
            .set("IsMouseButtonDown", ::IsMouseButtonDown)
            .set("IsMouseButtonReleased", ::IsMouseButtonReleased)
            .set("IsMouseButtonUp", ::IsMouseButtonUp)
            .set("GetMouseX", ::GetMouseX)
            .set("GetMouseY", ::GetMouseY)
            .set("GetMousePosition", ::GetMousePosition)
            .set("GetMouseDelta", ::GetMouseDelta)
            .set("SetMousePosition", ::SetMousePosition)
            .set("SetMouseOffset", ::SetMouseOffset)
            .set("SetMouseScale", ::SetMouseScale)
            .set("GetMouseWheelMove", ::GetMouseWheelMove)
            .set("GetMouseWheelMoveV", ::GetMouseWheelMoveV)
            .set("BeginDrawing", ::BeginDrawing)
            .set("EndDrawing", ::EndDrawing)
            .set("SetTargetFPS", ::SetTargetFPS)
            .set("GetFrameTime", ::GetFrameTime)
            .set("ClearBackground", ::ClearBackground);
        module.add("Core", core_obj);

        // Shape functions
        auto shapes_obj = engine.make_object()
            .set("DrawRectangle", ::DrawRectangle)
            .set("DrawRectangleLines", ::DrawRectangleLines)
            .set("DrawRectangleV", ::DrawRectangleV)
            .set("DrawRectangleLinesEx", ::DrawRectangleLinesEx)
            .set("DrawRectangleLinesEx", ::DrawRectangleLinesEx)
            .set("DrawRectangleGradientV", ::DrawRectangleGradientV)
            .set("DrawCircle", ::DrawCircle)
            .set("DrawCircleLines", ::DrawCircleLines)
            .set("DrawCircleV", ::DrawCircleV)
            .set("DrawCircleGradient", ::DrawCircleGradient)
            .set("DrawLine", ::DrawLine)
            .set("DrawLineEx", ::DrawLineEx)
            .set("DrawLineV", ::DrawLineV)
            .set("DrawLineBezier", ::DrawLineBezier);
        module.add("Shapes", shapes_obj);

        // Text functions
        auto text_obj = engine.make_object()
            .set("DrawFPS", ::DrawFPS)
            .set("DrawText", ::DrawText)
            .set("DrawTextEx", ::DrawTextEx)
            .set("DrawTextPro", ::DrawTextPro)
            .set("MeasureText", ::MeasureText)
            .set("MeasureTextEx", ::MeasureTextEx);
        module.add("Text", text_obj);

    }

    static void RegisterRaylibEnums(qjs::Engine &ns, qjs::Module &module) {

        // Enum: ConfigFlags
        auto ConfigFlags_obj = ns.make_object()
            .set("FLAG_VSYNC_HINT", FLAG_VSYNC_HINT)
            .set("FLAG_FULLSCREEN_MODE", FLAG_FULLSCREEN_MODE)
            .set("FLAG_WINDOW_RESIZABLE", FLAG_WINDOW_RESIZABLE)
            .set("FLAG_WINDOW_UNDECORATED", FLAG_WINDOW_UNDECORATED)
            .set("FLAG_WINDOW_HIDDEN", FLAG_WINDOW_HIDDEN)
            .set("FLAG_WINDOW_MINIMIZED", FLAG_WINDOW_MINIMIZED)
            .set("FLAG_WINDOW_MAXIMIZED", FLAG_WINDOW_MAXIMIZED)
            .set("FLAG_WINDOW_UNFOCUSED", FLAG_WINDOW_UNFOCUSED)
            .set("FLAG_WINDOW_TOPMOST", FLAG_WINDOW_TOPMOST)
            .set("FLAG_WINDOW_ALWAYS_RUN", FLAG_WINDOW_ALWAYS_RUN)
            .set("FLAG_WINDOW_TRANSPARENT", FLAG_WINDOW_TRANSPARENT)
            .set("FLAG_WINDOW_HIGHDPI", FLAG_WINDOW_HIGHDPI)
            .set("FLAG_WINDOW_MOUSE_PASSTHROUGH", FLAG_WINDOW_MOUSE_PASSTHROUGH)
            .set("FLAG_BORDERLESS_WINDOWED_MODE", FLAG_BORDERLESS_WINDOWED_MODE)
            .set("FLAG_MSAA_4X_HINT", FLAG_MSAA_4X_HINT)
            .set("FLAG_INTERLACED_HINT", FLAG_INTERLACED_HINT);
        module.add("ConfigFlags", ConfigFlags_obj);

        // Enum: KeyboardKey
        auto KeyboardKey_obj = ns.make_object()
            .set("KEY_NULL", KEY_NULL)
            .set("KEY_APOSTROPHE", KEY_APOSTROPHE)
            .set("KEY_COMMA", KEY_COMMA)
            .set("KEY_MINUS", KEY_MINUS)
            .set("KEY_PERIOD", KEY_PERIOD)
            .set("KEY_SLASH", KEY_SLASH)
            .set("KEY_ZERO", KEY_ZERO)
            .set("KEY_ONE", KEY_ONE)
            .set("KEY_TWO", KEY_TWO)
            .set("KEY_THREE", KEY_THREE)
            .set("KEY_FOUR", KEY_FOUR)
            .set("KEY_FIVE", KEY_FIVE)
            .set("KEY_SIX", KEY_SIX)
            .set("KEY_SEVEN", KEY_SEVEN)
            .set("KEY_EIGHT", KEY_EIGHT)
            .set("KEY_NINE", KEY_NINE)
            .set("KEY_SEMICOLON", KEY_SEMICOLON)
            .set("KEY_EQUAL", KEY_EQUAL)
            .set("KEY_A", KEY_A)
            .set("KEY_B", KEY_B)
            .set("KEY_C", KEY_C)
            .set("KEY_D", KEY_D)
            .set("KEY_E", KEY_E)
            .set("KEY_F", KEY_F)
            .set("KEY_G", KEY_G)
            .set("KEY_H", KEY_H)
            .set("KEY_I", KEY_I)
            .set("KEY_J", KEY_J)
            .set("KEY_K", KEY_K)
            .set("KEY_L", KEY_L)
            .set("KEY_M", KEY_M)
            .set("KEY_N", KEY_N)
            .set("KEY_O", KEY_O)
            .set("KEY_P", KEY_P)
            .set("KEY_Q", KEY_Q)
            .set("KEY_R", KEY_R)
            .set("KEY_S", KEY_S)
            .set("KEY_T", KEY_T)
            .set("KEY_U", KEY_U)
            .set("KEY_V", KEY_V)
            .set("KEY_W", KEY_W)
            .set("KEY_X", KEY_X)
            .set("KEY_Y", KEY_Y)
            .set("KEY_Z", KEY_Z)
            .set("KEY_LEFT_BRACKET", KEY_LEFT_BRACKET)
            .set("KEY_BACKSLASH", KEY_BACKSLASH)
            .set("KEY_RIGHT_BRACKET", KEY_RIGHT_BRACKET)
            .set("KEY_GRAVE", KEY_GRAVE)
            .set("KEY_SPACE", KEY_SPACE)
            .set("KEY_ESCAPE", KEY_ESCAPE)
            .set("KEY_ENTER", KEY_ENTER)
            .set("KEY_TAB", KEY_TAB)
            .set("KEY_BACKSPACE", KEY_BACKSPACE)
            .set("KEY_INSERT", KEY_INSERT)
            .set("KEY_DELETE", KEY_DELETE)
            .set("KEY_RIGHT", KEY_RIGHT)
            .set("KEY_LEFT", KEY_LEFT)
            .set("KEY_DOWN", KEY_DOWN)
            .set("KEY_UP", KEY_UP)
            .set("KEY_PAGE_UP", KEY_PAGE_UP)
            .set("KEY_PAGE_DOWN", KEY_PAGE_DOWN)
            .set("KEY_HOME", KEY_HOME)
            .set("KEY_END", KEY_END)
            .set("KEY_CAPS_LOCK", KEY_CAPS_LOCK)
            .set("KEY_SCROLL_LOCK", KEY_SCROLL_LOCK)
            .set("KEY_NUM_LOCK", KEY_NUM_LOCK)
            .set("KEY_PRINT_SCREEN", KEY_PRINT_SCREEN)
            .set("KEY_PAUSE", KEY_PAUSE)
            .set("KEY_F1", KEY_F1)
            .set("KEY_F2", KEY_F2)
            .set("KEY_F3", KEY_F3)
            .set("KEY_F4", KEY_F4)
            .set("KEY_F5", KEY_F5)
            .set("KEY_F6", KEY_F6)
            .set("KEY_F7", KEY_F7)
            .set("KEY_F8", KEY_F8)
            .set("KEY_F9", KEY_F9)
            .set("KEY_F10", KEY_F10)
            .set("KEY_F11", KEY_F11)
            .set("KEY_F12", KEY_F12)
            .set("KEY_LEFT_SHIFT", KEY_LEFT_SHIFT)
            .set("KEY_LEFT_CONTROL", KEY_LEFT_CONTROL)
            .set("KEY_LEFT_ALT", KEY_LEFT_ALT)
            .set("KEY_LEFT_SUPER", KEY_LEFT_SUPER)
            .set("KEY_RIGHT_SHIFT", KEY_RIGHT_SHIFT)
            .set("KEY_RIGHT_CONTROL", KEY_RIGHT_CONTROL)
            .set("KEY_RIGHT_ALT", KEY_RIGHT_ALT)
            .set("KEY_RIGHT_SUPER", KEY_RIGHT_SUPER)
            .set("KEY_KB_MENU", KEY_KB_MENU)
            .set("KEY_KP_0", KEY_KP_0)
            .set("KEY_KP_1", KEY_KP_1)
            .set("KEY_KP_2", KEY_KP_2)
            .set("KEY_KP_3", KEY_KP_3)
            .set("KEY_KP_4", KEY_KP_4)
            .set("KEY_KP_5", KEY_KP_5)
            .set("KEY_KP_6", KEY_KP_6)
            .set("KEY_KP_7", KEY_KP_7)
            .set("KEY_KP_8", KEY_KP_8)
            .set("KEY_KP_9", KEY_KP_9)
            .set("KEY_KP_DECIMAL", KEY_KP_DECIMAL)
            .set("KEY_KP_DIVIDE", KEY_KP_DIVIDE)
            .set("KEY_KP_MULTIPLY", KEY_KP_MULTIPLY)
            .set("KEY_KP_SUBTRACT", KEY_KP_SUBTRACT)
            .set("KEY_KP_ADD", KEY_KP_ADD)
            .set("KEY_KP_ENTER", KEY_KP_ENTER)
            .set("KEY_KP_EQUAL", KEY_KP_EQUAL)
            .set("KEY_BACK", KEY_BACK)
            .set("KEY_MENU", KEY_MENU)
            .set("KEY_VOLUME_UP", KEY_VOLUME_UP)
            .set("KEY_VOLUME_DOWN", KEY_VOLUME_DOWN);
        module.add("KeyboardKey", KeyboardKey_obj);

        // Enum: MouseButton
        auto MouseButton_obj = ns.make_object()
            .set("MOUSE_BUTTON_LEFT", MOUSE_BUTTON_LEFT)
            .set("MOUSE_BUTTON_RIGHT", MOUSE_BUTTON_RIGHT)
            .set("MOUSE_BUTTON_MIDDLE", MOUSE_BUTTON_MIDDLE)
            .set("MOUSE_BUTTON_SIDE", MOUSE_BUTTON_SIDE)
            .set("MOUSE_BUTTON_EXTRA", MOUSE_BUTTON_EXTRA)
            .set("MOUSE_BUTTON_FORWARD", MOUSE_BUTTON_FORWARD)
            .set("MOUSE_BUTTON_BACK", MOUSE_BUTTON_BACK);
        module.add("MouseButton", MouseButton_obj);

        // Enum: MouseCursor
        auto MouseCursor_obj = ns.make_object()
            .set("MOUSE_CURSOR_DEFAULT", MOUSE_CURSOR_DEFAULT)
            .set("MOUSE_CURSOR_ARROW", MOUSE_CURSOR_ARROW)
            .set("MOUSE_CURSOR_IBEAM", MOUSE_CURSOR_IBEAM)
            .set("MOUSE_CURSOR_CROSSHAIR", MOUSE_CURSOR_CROSSHAIR)
            .set("MOUSE_CURSOR_POINTING_HAND", MOUSE_CURSOR_POINTING_HAND)
            .set("MOUSE_CURSOR_RESIZE_EW", MOUSE_CURSOR_RESIZE_EW)
            .set("MOUSE_CURSOR_RESIZE_NS", MOUSE_CURSOR_RESIZE_NS)
            .set("MOUSE_CURSOR_RESIZE_NWSE", MOUSE_CURSOR_RESIZE_NWSE)
            .set("MOUSE_CURSOR_RESIZE_NESW", MOUSE_CURSOR_RESIZE_NESW)
            .set("MOUSE_CURSOR_RESIZE_ALL", MOUSE_CURSOR_RESIZE_ALL)
            .set("MOUSE_CURSOR_NOT_ALLOWED", MOUSE_CURSOR_NOT_ALLOWED);
        module.add("MouseCursor", MouseCursor_obj);

        // Enum: GamepadButton
        auto GamepadButton_obj = ns.make_object()
            .set("GAMEPAD_BUTTON_UNKNOWN", GAMEPAD_BUTTON_UNKNOWN)
            .set("GAMEPAD_BUTTON_LEFT_FACE_UP", GAMEPAD_BUTTON_LEFT_FACE_UP)
            .set("GAMEPAD_BUTTON_LEFT_FACE_RIGHT", GAMEPAD_BUTTON_LEFT_FACE_RIGHT)
            .set("GAMEPAD_BUTTON_LEFT_FACE_DOWN", GAMEPAD_BUTTON_LEFT_FACE_DOWN)
            .set("GAMEPAD_BUTTON_LEFT_FACE_LEFT", GAMEPAD_BUTTON_LEFT_FACE_LEFT)
            .set("GAMEPAD_BUTTON_RIGHT_FACE_UP", GAMEPAD_BUTTON_RIGHT_FACE_UP)
            .set("GAMEPAD_BUTTON_RIGHT_FACE_RIGHT", GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)
            .set("GAMEPAD_BUTTON_RIGHT_FACE_DOWN", GAMEPAD_BUTTON_RIGHT_FACE_DOWN)
            .set("GAMEPAD_BUTTON_RIGHT_FACE_LEFT", GAMEPAD_BUTTON_RIGHT_FACE_LEFT)
            .set("GAMEPAD_BUTTON_LEFT_TRIGGER_1", GAMEPAD_BUTTON_LEFT_TRIGGER_1)
            .set("GAMEPAD_BUTTON_LEFT_TRIGGER_2", GAMEPAD_BUTTON_LEFT_TRIGGER_2)
            .set("GAMEPAD_BUTTON_RIGHT_TRIGGER_1", GAMEPAD_BUTTON_RIGHT_TRIGGER_1)
            .set("GAMEPAD_BUTTON_RIGHT_TRIGGER_2", GAMEPAD_BUTTON_RIGHT_TRIGGER_2)
            .set("GAMEPAD_BUTTON_MIDDLE_LEFT", GAMEPAD_BUTTON_MIDDLE_LEFT)
            .set("GAMEPAD_BUTTON_MIDDLE", GAMEPAD_BUTTON_MIDDLE)
            .set("GAMEPAD_BUTTON_MIDDLE_RIGHT", GAMEPAD_BUTTON_MIDDLE_RIGHT)
            .set("GAMEPAD_BUTTON_LEFT_THUMB", GAMEPAD_BUTTON_LEFT_THUMB)
            .set("GAMEPAD_BUTTON_RIGHT_THUMB", GAMEPAD_BUTTON_RIGHT_THUMB);
        module.add("GamepadButton", GamepadButton_obj);

        // Enum: GamepadAxis
        auto GamepadAxis_obj = ns.make_object()
            .set("GAMEPAD_AXIS_LEFT_X", GAMEPAD_AXIS_LEFT_X)
            .set("GAMEPAD_AXIS_LEFT_Y", GAMEPAD_AXIS_LEFT_Y)
            .set("GAMEPAD_AXIS_RIGHT_X", GAMEPAD_AXIS_RIGHT_X)
            .set("GAMEPAD_AXIS_RIGHT_Y", GAMEPAD_AXIS_RIGHT_Y)
            .set("GAMEPAD_AXIS_LEFT_TRIGGER", GAMEPAD_AXIS_LEFT_TRIGGER)
            .set("GAMEPAD_AXIS_RIGHT_TRIGGER", GAMEPAD_AXIS_RIGHT_TRIGGER);
        module.add("GamepadAxis", GamepadAxis_obj);

        // Enum: CameraMode
        auto CameraMode_obj = ns.make_object()
            .set("CAMERA_CUSTOM", CAMERA_CUSTOM)
            .set("CAMERA_FREE", CAMERA_FREE)
            .set("CAMERA_ORBITAL", CAMERA_ORBITAL)
            .set("CAMERA_FIRST_PERSON", CAMERA_FIRST_PERSON)
            .set("CAMERA_THIRD_PERSON", CAMERA_THIRD_PERSON);
        module.add("CameraMode", CameraMode_obj);

        // Enum: CameraProjection
        auto CameraProjection_obj = ns.make_object()
            .set("CAMERA_PERSPECTIVE", CAMERA_PERSPECTIVE)
            .set("CAMERA_ORTHOGRAPHIC", CAMERA_ORTHOGRAPHIC);
        module.add("CameraProjection", CameraProjection_obj);

    }

    void RegisterRaylibModule(qjs::Engine &engine) {

        SetRandomSeed(static_cast<unsigned int>(time(nullptr)));

        // Create the module and get a reference to it for further exports
        auto raylib_mod = engine.define_module("Raylib");

        // Initialize all function groups in the module
        RegisterRaylibFunctions(engine, raylib_mod);

        // Initialize all enums in the module
        RegisterRaylibEnums(engine, raylib_mod);

    }

}