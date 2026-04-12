#include "../qjs_wrapper.hpp"
#include <raylib.h>
#include "rl_module.hpp"

namespace RaylibModule {

    void register_raylib_functions(const qjs::Engine &engine, qjs::Module &module) {

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

        // Texture functions
         auto texture_obj = engine.make_object()
            .set("LoadTexture", ::LoadTexture)
            .set("UnloadTexture", ::UnloadTexture)
            .set("DrawTexture", ::DrawTexture)
            .set("DrawTextureEx", ::DrawTextureEx)
            .set("DrawTextureRec", ::DrawTextureRec)
            .set("DrawTexturePro", ::DrawTexturePro);
        module.add("Texture", texture_obj);

    }

}