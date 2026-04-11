/**
 * TypeScript definitions for the Raylib QuickJS Module.
 * Based on the rl_module.cpp mappings.
 */

declare module "Raylib" {
    // --- Raylib Structs ---
    export interface Vector2 {
        x: number;
        y: number;
    }

    export interface Color {
        r: number;
        g: number;
        b: number;
        a: number;
    }

    // --- Enums ---
    export namespace ConfigFlags {
        const FLAG_VSYNC_HINT: number;
        const FLAG_FULLSCREEN_MODE: number;
        const FLAG_WINDOW_RESIZABLE: number;
        const FLAG_WINDOW_UNDECORATED: number;
        const FLAG_WINDOW_HIDDEN: number;
        const FLAG_WINDOW_MINIMIZED: number;
        const FLAG_WINDOW_MAXIMIZED: number;
        const FLAG_WINDOW_UNFOCUSED: number;
        const FLAG_WINDOW_TOPMOST: number;
        const FLAG_WINDOW_ALWAYS_RUN: number;
        const FLAG_WINDOW_TRANSPARENT: number;
        const FLAG_WINDOW_HIGHDPI: number;
        const FLAG_WINDOW_MOUSE_PASSTHROUGH: number;
        const FLAG_BORDERLESS_WINDOWED_MODE: number;
        const FLAG_MSAA_4X_HINT: number;
        const FLAG_INTERLACED_HINT: number;
    }

    export namespace KeyboardKey {
        const KEY_NULL: number;
        const KEY_APOSTROPHE: number;
        const KEY_COMMA: number;
        const KEY_MINUS: number;
        const KEY_PERIOD: number;
        const KEY_SLASH: number;
        const KEY_ZERO: number;
        const KEY_ONE: number;
        const KEY_TWO: number;
        const KEY_THREE: number;
        const KEY_FOUR: number;
        const KEY_FIVE: number;
        const KEY_SIX: number;
        const KEY_SEVEN: number;
        const KEY_EIGHT: number;
        const KEY_NINE: number;
        const KEY_SEMICOLON: number;
        const KEY_EQUAL: number;
        const KEY_A: number;
        const KEY_B: number;
        const KEY_C: number;
        const KEY_D: number;
        const KEY_E: number;
        const KEY_F: number;
        const KEY_G: number;
        const KEY_H: number;
        const KEY_I: number;
        const KEY_J: number;
        const KEY_K: number;
        const KEY_L: number;
        const KEY_M: number;
        const KEY_N: number;
        const KEY_O: number;
        const KEY_P: number;
        const KEY_Q: number;
        const KEY_R: number;
        const KEY_S: number;
        const KEY_T: number;
        const KEY_U: number;
        const KEY_V: number;
        const KEY_W: number;
        const KEY_X: number;
        const KEY_Y: number;
        const KEY_Z: number;
        const KEY_LEFT_BRACKET: number;
        const KEY_BACKSLASH: number;
        const KEY_RIGHT_BRACKET: number;
        const KEY_GRAVE: number;
        const KEY_SPACE: number;
        const KEY_ESCAPE: number;
        const KEY_ENTER: number;
        const KEY_TAB: number;
        const KEY_BACKSPACE: number;
        const KEY_INSERT: number;
        const KEY_DELETE: number;
        const KEY_RIGHT: number;
        const KEY_LEFT: number;
        const KEY_DOWN: number;
        const KEY_UP: number;
        const KEY_PAGE_UP: number;
        const KEY_PAGE_DOWN: number;
        const KEY_HOME: number;
        const KEY_END: number;
        const KEY_CAPS_LOCK: number;
        const KEY_SCROLL_LOCK: number;
        const KEY_NUM_LOCK: number;
        const KEY_PRINT_SCREEN: number;
        const KEY_PAUSE: number;
        const KEY_F1: number;
        const KEY_F2: number;
        const KEY_F3: number;
        const KEY_F4: number;
        const KEY_F5: number;
        const KEY_F6: number;
        const KEY_F7: number;
        const KEY_F8: number;
        const KEY_F9: number;
        const KEY_F10: number;
        const KEY_F11: number;
        const KEY_F12: number;
        const KEY_LEFT_SHIFT: number;
        const KEY_LEFT_CONTROL: number;
        const KEY_LEFT_ALT: number;
        const KEY_LEFT_SUPER: number;
        const KEY_RIGHT_SHIFT: number;
        const KEY_RIGHT_CONTROL: number;
        const KEY_RIGHT_ALT: number;
        const KEY_RIGHT_SUPER: number;
        const KEY_KB_MENU: number;
        const KEY_KP_0: number;
        const KEY_KP_1: number;
        const KEY_KP_2: number;
        const KEY_KP_3: number;
        const KEY_KP_4: number;
        const KEY_KP_5: number;
        const KEY_KP_6: number;
        const KEY_KP_7: number;
        const KEY_KP_8: number;
        const KEY_KP_9: number;
        const KEY_KP_DECIMAL: number;
        const KEY_KP_DIVIDE: number;
        const KEY_KP_MULTIPLY: number;
        const KEY_KP_SUBTRACT: number;
        const KEY_KP_ADD: number;
        const KEY_KP_ENTER: number;
        const KEY_KP_EQUAL: number;
        const KEY_BACK: number;
        const KEY_MENU: number;
        const KEY_VOLUME_UP: number;
        const KEY_VOLUME_DOWN: number;
    }

    export namespace MouseButton {
        const MOUSE_BUTTON_LEFT: number;
        const MOUSE_BUTTON_RIGHT: number;
        const MOUSE_BUTTON_MIDDLE: number;
        const MOUSE_BUTTON_SIDE: number;
        const MOUSE_BUTTON_EXTRA: number;
        const MOUSE_BUTTON_FORWARD: number;
        const MOUSE_BUTTON_BACK: number;
    }

    export namespace MouseCursor {
        const MOUSE_CURSOR_DEFAULT: number;
        const MOUSE_CURSOR_ARROW: number;
        const MOUSE_CURSOR_IBEAM: number;
        const MOUSE_CURSOR_CROSSHAIR: number;
        const MOUSE_CURSOR_POINTING_HAND: number;
        const MOUSE_CURSOR_RESIZE_EW: number;
        const MOUSE_CURSOR_RESIZE_NS: number;
        const MOUSE_CURSOR_RESIZE_NWSE: number;
        const MOUSE_CURSOR_RESIZE_NESW: number;
        const MOUSE_CURSOR_RESIZE_ALL: number;
        const MOUSE_CURSOR_NOT_ALLOWED: number;
    }

    export namespace GamepadButton {
        const GAMEPAD_BUTTON_UNKNOWN: number;
        const GAMEPAD_BUTTON_LEFT_FACE_UP: number;
        const GAMEPAD_BUTTON_LEFT_FACE_RIGHT: number;
        const GAMEPAD_BUTTON_LEFT_FACE_DOWN: number;
        const GAMEPAD_BUTTON_LEFT_FACE_LEFT: number;
        const GAMEPAD_BUTTON_RIGHT_FACE_UP: number;
        const GAMEPAD_BUTTON_RIGHT_FACE_RIGHT: number;
        const GAMEPAD_BUTTON_RIGHT_FACE_DOWN: number;
        const GAMEPAD_BUTTON_RIGHT_FACE_LEFT: number;
        const GAMEPAD_BUTTON_LEFT_TRIGGER_1: number;
        const GAMEPAD_BUTTON_LEFT_TRIGGER_2: number;
        const GAMEPAD_BUTTON_RIGHT_TRIGGER_1: number;
        const GAMEPAD_BUTTON_RIGHT_TRIGGER_2: number;
        const GAMEPAD_BUTTON_MIDDLE_LEFT: number;
        const GAMEPAD_BUTTON_MIDDLE: number;
        const GAMEPAD_BUTTON_MIDDLE_RIGHT: number;
        const GAMEPAD_BUTTON_LEFT_THUMB: number;
        const GAMEPAD_BUTTON_RIGHT_THUMB: number;
    }

    export namespace GamepadAxis {
        const GAMEPAD_AXIS_LEFT_X: number;
        const GAMEPAD_AXIS_LEFT_Y: number;
        const GAMEPAD_AXIS_RIGHT_X: number;
        const GAMEPAD_AXIS_RIGHT_Y: number;
        const GAMEPAD_AXIS_LEFT_TRIGGER: number;
        const GAMEPAD_AXIS_RIGHT_TRIGGER: number;
    }

    export namespace CameraMode {
        const CAMERA_CUSTOM: number;
        const CAMERA_FREE: number;
        const CAMERA_ORBITAL: number;
        const CAMERA_FIRST_PERSON: number;
        const CAMERA_THIRD_PERSON: number;
    }

    export namespace CameraProjection {
        const CAMERA_PERSPECTIVE: number;
        const CAMERA_ORTHOGRAPHIC: number;
    }

    // --- Core Functions ---
    export namespace Core {
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
        function IsWindowState(flag: number): boolean;
        function SetWindowState(flag: number): void;
        function ClearWindowState(flag: number): void;
        function GetScreenWidth(): number;
        function GetScreenHeight(): number;
        function GetMonitorCount(): number;
        function GetMonitorWidth(monitor: number): number;
        function GetMonitorHeight(monitor: number): number;
        function GetMonitorPhysicalWidth(monitor: number): number;
        function GetMonitorPhysicalHeight(monitor: number): number;
        function GetMonitorRefreshRate(monitor: number): number;
        function GetCurrentMonitor(): number;
        function IsKeyPressed(key: number): boolean;
        function IsKeyPressedRepeat(key: number): boolean;
        function IsKeyDown(key: number): boolean;
        function IsKeyReleased(key: number): boolean;
        function IsKeyUp(key: number): boolean;
        function GetKeyPressed(): number;
        function SetExitKey(key: number): void;
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
        function BeginDrawing(): void;
        function EndDrawing(): void;
        function SetTargetFPS(fps: number): void;
        function GetFrameTime(): number;
        function ClearBackground(color: Color): void;
    }

    // --- Shapes Functions ---
    export namespace Shapes {
        function DrawRectangle(posX: number, posY: number, width: number, height: number, color: Color): void;
        function DrawRectangleLines(posX: number, posY: number, width: number, height: number, color: Color): void;
        function DrawRectangleV(position: Vector2, size: Vector2, color: Color): void;
        function DrawRectangleLinesEx(rec: { x: number, y: number, width: number, height: number }, lineThick: number, color: Color): void;
        function DrawRectangleGradientV(posX: number, posY: number, width: number, height: number, color1: Color, color2: Color): void;
        function DrawCircle(centerX: number, centerY: number, radius: number, color: Color): void;
        function DrawCircleLines(centerX: number, centerY: number, radius: number, color: Color): void;
        function DrawCircleV(center: Vector2, radius: number, color: Color): void;
        function DrawCircleGradient(centerX: number, centerY: number, radius: number, color1: Color, color2: Color): void;
        function DrawLine(startPosX: number, startPosY: number, endPosX: number, endPosY: number, color: Color): void;
        function DrawLineEx(startPos: Vector2, endPos: Vector2, thick: number, color: Color): void;
        function DrawLineV(startPos: Vector2, endPos: Vector2, color: Color): void;
        function DrawLineBezier(startPos: Vector2, endPos: Vector2, thick: number, color: Color): void;
    }

    // --- Text Functions ---
    export namespace Text {
        function DrawFPS(posX: number, posY: number): void;
        function DrawText(text: string, posX: number, posY: number, fontSize: number, color: Color): void;
        function DrawTextEx(font: any, text: string, position: Vector2, fontSize: number, spacing: number, tint: Color): void;
        function DrawTextPro(font: any, text: string, position: Vector2, origin: Vector2, rotation: number, fontSize: number, spacing: number, tint: Color): void;
        function MeasureText(text: string, fontSize: number): number;
        function MeasureTextEx(font: any, text: string, fontSize: number, spacing: number): Vector2;
    }
}