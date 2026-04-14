#include "../qjs_wrapper.hpp"
#include <raylib.h>
#include "rl_module.hpp"

namespace RaylibModule {

    void register_raylib_functions(const qjs::Engine &engine, qjs::Module &module) {

        // Core functions
        auto core_obj = engine.make_object()
            .set_function("InitWindow", ::InitWindow)
            .set_function("CloseWindow", ::CloseWindow)
            .set_function("WindowShouldClose", ::WindowShouldClose)
            .set_function("IsWindowReady", ::IsWindowReady)
            .set_function("IsWindowFullscreen", ::IsWindowFullscreen)
            .set_function("IsWindowHidden", ::IsWindowHidden)
            .set_function("IsWindowMinimized", ::IsWindowMinimized)
            .set_function("IsWindowMaximized", ::IsWindowMaximized)
            .set_function("IsWindowFocused", ::IsWindowFocused)
            .set_function("IsWindowResized", ::IsWindowResized)
            .set_function("ToggleFullscreen", ::ToggleFullscreen)
            .set_function("ToggleBorderlessWindowed", ::ToggleBorderlessWindowed)
            .set_function("RestoreWindow", ::RestoreWindow)
            .set_function("MaximizeWindow", ::MaximizeWindow)
            .set_function("MinimizeWindow", ::MinimizeWindow)
            .set_function("SetWindowTitle", ::SetWindowTitle)
            .set_function("SetWindowPosition", ::SetWindowPosition)
            .set_function("SetWindowMonitor", ::SetWindowMonitor)
            .set_function("SetWindowMinSize", ::SetWindowMinSize)
            .set_function("SetWindowMaxSize", ::SetWindowMaxSize)
            .set_function("SetWindowSize", ::SetWindowSize)
            .set_function("SetWindowFocused", ::SetWindowFocused)
            .set_function("IsWindowState", ::IsWindowState)
            .set_function("SetWindowState", ::SetWindowState)
            .set_function("ClearWindowState", ::ClearWindowState)
            .set_function("GetScreenWidth", ::GetScreenWidth)
            .set_function("GetScreenHeight", ::GetScreenHeight)
            .set_function("GetMonitorCount", ::GetMonitorCount)
            .set_function("GetMonitorWidth", ::GetMonitorWidth)
            .set_function("GetMonitorHeight", ::GetMonitorHeight)
            .set_function("GetMonitorPhysicalWidth", ::GetMonitorPhysicalWidth)
            .set_function("GetMonitorPhysicalHeight", ::GetMonitorPhysicalHeight)
            .set_function("GetMonitorRefreshRate", ::GetMonitorRefreshRate)
            .set_function("GetCurrentMonitor", ::GetCurrentMonitor)
            .set_function("BeginDrawing", ::BeginDrawing)
            .set_function("EndDrawing", ::EndDrawing)
            .set_function("BeginMode2D", ::BeginMode2D)
            .set_function("EndMode2D", ::EndMode2D)
            .set_function("BeginMode3D", ::BeginMode3D)
            .set_function("BeginMode3D", ::BeginMode3D)
            .set_function("SetTargetFPS", ::SetTargetFPS)
            .set_function("GetFrameTime", ::GetFrameTime)
            .set_function("ClearBackground", ::ClearBackground);
        module.add("Core", core_obj);

        // Input functions
        auto input_obj = engine.make_object()
            .set_function("IsKeyPressed", ::IsKeyPressed)
            .set_function("IsKeyPressedRepeat", ::IsKeyPressedRepeat)
            .set_function("IsKeyDown", ::IsKeyDown)
            .set_function("IsKeyReleased", ::IsKeyReleased)
            .set_function("IsKeyUp", ::IsKeyUp)
            .set_function("GetKeyPressed", ::GetKeyPressed)
            .set_function("SetExitKey", ::SetExitKey)
            .set_function("IsMouseButtonPressed", ::IsMouseButtonPressed)
            .set_function("IsMouseButtonDown", ::IsMouseButtonDown)
            .set_function("IsMouseButtonReleased", ::IsMouseButtonReleased)
            .set_function("IsMouseButtonUp", ::IsMouseButtonUp)
            .set_function("GetMouseX", ::GetMouseX)
            .set_function("GetMouseY", ::GetMouseY)
            .set_function("GetMousePosition", ::GetMousePosition)
            .set_function("GetMouseDelta", ::GetMouseDelta)
            .set_function("SetMousePosition", ::SetMousePosition)
            .set_function("SetMouseOffset", ::SetMouseOffset)
            .set_function("SetMouseScale", ::SetMouseScale)
            .set_function("GetMouseWheelMove", ::GetMouseWheelMove)
            .set_function("GetMouseWheelMoveV", ::GetMouseWheelMoveV);
        module.add("Input", input_obj);

        // Shape functions
        auto shapes_obj = engine.make_object()
            .set_function("DrawRectangle", ::DrawRectangle)
            .set_function("DrawRectangleLines", ::DrawRectangleLines)
            .set_function("DrawRectangleV", ::DrawRectangleV)
            .set_function("DrawRectangleLinesEx", ::DrawRectangleLinesEx)
            .set_function("DrawRectangleLinesEx", ::DrawRectangleLinesEx)
            .set_function("DrawRectangleGradientV", ::DrawRectangleGradientV)
            .set_function("DrawCircle", ::DrawCircle)
            .set_function("DrawCircleLines", ::DrawCircleLines)
            .set_function("DrawCircleV", ::DrawCircleV)
            .set_function("DrawCircleGradient", ::DrawCircleGradient)
            .set_function("DrawLine", ::DrawLine)
            .set_function("DrawLineEx", ::DrawLineEx)
            .set_function("DrawLineV", ::DrawLineV)
            .set_function("DrawLineBezier", ::DrawLineBezier);
        module.add("Shapes", shapes_obj);

        // Text functions
        auto text_obj = engine.make_object()
            .set_function("DrawFPS", ::DrawFPS)
            .set_function("DrawText", ::DrawText)
            .set_function("DrawTextEx", ::DrawTextEx)
            .set_function("DrawTextPro", ::DrawTextPro)
            .set_function("MeasureText", ::MeasureText)
            .set_function("MeasureTextEx", ::MeasureTextEx)
            .set_function("SetTextLineSpacing", ::SetTextLineSpacing);
        module.add("Text", text_obj);

        // Texture functions
        auto texture_obj = engine.make_object()
            .set_function("LoadTexture", ::LoadTexture)
            .set_function("UnloadTexture", ::UnloadTexture)
            .set_function("DrawTexture", ::DrawTexture)
            .set_function("DrawTextureEx", ::DrawTextureEx)
            .set_function("DrawTextureRec", ::DrawTextureRec)
            .set_function("DrawTexturePro", ::DrawTexturePro);
        module.add("Texture", texture_obj);

        // Models functions
        auto models_obj = engine.make_object()
            .set_function("LoadModel", ::LoadModel)
            .set_function("UnloadModel", ::UnloadModel);
        module.add("Models", models_obj);

        // Audio
        auto audio_obj = engine.make_object()
            .set_function("InitAudioDevice", InitAudioDevice)
            .set_function("CloseAudioDevice", CloseAudioDevice)
            .set_function("IsAudioDeviceReady", IsAudioDeviceReady)
            .set_function("SetMasterVolume", SetMasterVolume)
            .set_function("GetMasterVolume", GetMasterVolume)
            .set_function("LoadSound", ::LoadSound)
            .set_function("UnloadSound", ::UnloadSound)
            .set_function("PlaySound", PlaySound)
            .set_function("StopSound", StopSound)
            .set_function("PauseSound", PauseSound)
            .set_function("ResumeSound", ResumeSound)
            .set_function("IsSoundPlaying", IsSoundPlaying);
        module.add("Audio", audio_obj);

    }

}