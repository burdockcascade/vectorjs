/**
 * TypeScript definitions for the internal Raylib module baked into the host.
 * This module provides direct access to the Raylib C functions via QuickJS.
 */
declare module "Raylib" {
    /**
     * Represents a Raylib Color (RGBA, 0-255).
     */
    export interface Color {
        r: number;
        g: number;
        b: number;
        a: number;
    }

    /**
     * Represents a 2D Vector.
     */
    export interface Vector2 {
        x: number;
        y: number;
    }

    /**
     * Core window and system management functions.
     */
    export namespace Core {
        // Window Management
        function InitWindow(width: number, height: number, title: string): void;
        function CloseWindow(): void;
        function WindowShouldClose(): boolean;
        function IsWindowReady(): boolean;
        function IsWindowFullscreen(): boolean;
        function IsWindowHidden(): boolean;
        function IsWindowMinimized(): boolean;
        function IsWindowMaximized(): boolean;
        function IsWindowFocused(): boolean;
        function IsWindowResized(): boolean;
        function ToggleFullscreen(): void;
        function ToggleBorderlessWindowed(): void;
        function RestoreWindow(): void;
        function MaximizeWindow(): void;
        function MinimizeWindow(): void;
        function SetWindowTitle(title: string): void;
        function SetWindowPosition(x: number, y: number): void;
        function SetWindowMonitor(monitor: number): void;
        function SetWindowMinSize(width: number, height: number): void;
        function SetWindowMaxSize(width: number, height: number): void;
        function SetWindowSize(width: number, height: number): void;
        function SetWindowFocused(): void;

        // Window State
        function IsWindowState(flag: number): boolean;
        function SetWindowState(flag: number): void;
        function ClearWindowState(flag: number): void;

        // Screen and Display
        function GetScreenWidth(): number;
        function GetScreenHeight(): number;
        function GetMonitorCount(): number;
        function GetMonitorWidth(monitor: number): number;
        function GetMonitorHeight(monitor: number): number;
        function GetMonitorPhysicalWidth(monitor: number): number;
        function GetMonitorPhysicalHeight(monitor: number): number;
        function GetMonitorRefreshRate(monitor: number): number;
        function GetCurrentMonitor(): number;

        // Keyboard Input
        function IsKeyPressed(key: number): boolean;
        function IsKeyPressedRepeat(key: number): boolean;
        function IsKeyDown(key: number): boolean;
        function IsKeyReleased(key: number): boolean;
        function IsKeyUp(key: number): boolean;
        function GetKeyPressed(): number;
        function SetExitKey(key: number): void;

        // Mouse Input
        function IsMouseButtonPressed(button: number): boolean;
        function IsMouseButtonDown(button: number): boolean;
        function IsMouseButtonReleased(button: number): boolean;
        function IsMouseButtonUp(button: number): boolean;
        function GetMouseX(): number;
        function GetMouseY(): number;
        function GetMousePosition(): Vector2;
        function GetMouseDelta(): Vector2;
        function SetMousePosition(x: number, y: number): void;
        function SetMouseOffset(offsetX: number, offsetY: number): void;
        function SetMouseScale(scaleX: number, scaleY: number): void;
        function GetMouseWheelMove(): number;
        function GetMouseWheelMoveV(): Vector2;

        // Drawing
        function BeginDrawing(): void;
        function EndDrawing(): void;

        // Timing
        function SetTargetFPS(fps: number): void;
        function GetFrameTime(): number;

        // Misc
        function ClearBackground(color: Color): void;
    }

    /**
     * Text rendering and UI functions.
     */
    export namespace Text {
        function DrawFPS(x: number, y: number): void;
        function DrawText(text: string, x: number, y: number, fontSize: number, color: Color): void;
    }

    /**
     * Shape drawing functions.
     * Note: Currently empty in src/host/rl_bindings.cpp, but defined as a namespace.
     */
    export namespace Shapes {
        // Functions like DrawRectangle and DrawCircle are currently
        // proxied through app.js rather than direct Raylib module exports.
    }
}