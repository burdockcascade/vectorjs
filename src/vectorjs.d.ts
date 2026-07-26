declare module "vectorjs" {

    /**
     * Structure representing an RGBA color.
     */
    export class Color {
        r: number;
        g: number;
        b: number;
        a: number;
        constructor();
        constructor(r: number, g: number, b: number, a: number);
    }

    /**
     * Structure representing a 2D vector.
     */
    export class Vector2 {
        x: number;
        y: number;
        constructor();
        constructor(x: number, y: number);
    }

    /**
     * Structure representing a 2D rectangle.
     */
    export class Rectangle {
        x: number;
        y: number;
        width: number;
        height: number;
        constructor();
        constructor(x: number, y: number, width: number, height: number);
    }

    /**
     * Built-in color palette options matching Raylib defaults.
     */
    export const Palette: {
        readonly LIGHTGRAY: Color;
        readonly GRAY: Color;
        readonly DARKGRAY: Color;
        readonly YELLOW: Color;
        readonly GOLD: Color;
        readonly ORANGE: Color;
        readonly PINK: Color;
        readonly RED: Color;
        readonly MAROON: Color;
        readonly GREEN: Color;
        readonly LIME: Color;
        readonly DARKGREEN: Color;
        readonly SKYBLUE: Color;
        readonly BLUE: Color;
        readonly DARKBLUE: Color;
        readonly PURPLE: Color;
        readonly VIOLET: Color;
        readonly DARKPURPLE: Color;
        readonly BEIGE: Color;
        readonly BROWN: Color;
        readonly DARKBROWN: Color;
        readonly WHITE: Color;
        readonly BLACK: Color;
        readonly BLANK: Color;
        readonly MAGENTA: Color;
        readonly RAYWHITE: Color;
    };

    export const MouseButton: {
        readonly MOUSE_BUTTON_LEFT: number;
        readonly MOUSE_BUTTON_RIGHT: number;
        readonly MOUSE_BUTTON_MIDDLE: number;
        readonly MOUSE_BUTTON_SIDE: number;
        readonly MOUSE_BUTTON_EXTRA: number;
        readonly MOUSE_BUTTON_FORWARD: number;
        readonly MOUSE_BUTTON_BACK: number;
    };

    export const MouseCursor: {
        readonly MOUSE_CURSOR_DEFAULT: number;
        readonly MOUSE_CURSOR_ARROW: number;
        readonly MOUSE_CURSOR_IBEAM: number;
        readonly MOUSE_CURSOR_CROSSHAIR: number;
        readonly MOUSE_CURSOR_POINTING_HAND: number;
        readonly MOUSE_CURSOR_RESIZE_EW: number;
        readonly MOUSE_CURSOR_RESIZE_NS: number;
        readonly MOUSE_CURSOR_RESIZE_NWSE: number;
        readonly MOUSE_CURSOR_RESIZE_NESW: number;
        readonly MOUSE_CURSOR_RESIZE_ALL: number;
        readonly MOUSE_CURSOR_NOT_ALLOWED: number;
    };

    export const Keyboard: {
        readonly KEY_NULL: number;
        readonly KEY_APOSTROPHE: number;
        readonly KEY_COMMA: number;
        readonly KEY_MINUS: number;
        readonly KEY_PERIOD: number;
        readonly KEY_SLASH: number;
        readonly KEY_ZERO: number;
        readonly KEY_ONE: number;
        readonly KEY_TWO: number;
        readonly KEY_THREE: number;
        readonly KEY_FOUR: number;
        readonly KEY_FIVE: number;
        readonly KEY_SIX: number;
        readonly KEY_SEVEN: number;
        readonly KEY_EIGHT: number;
        readonly KEY_NINE: number;
        readonly KEY_SEMICOLON: number;
        readonly KEY_EQUAL: number;
        readonly KEY_A: number;
        readonly KEY_B: number;
        readonly KEY_C: number;
        readonly KEY_D: number;
        readonly KEY_E: number;
        readonly KEY_F: number;
        readonly KEY_G: number;
        readonly KEY_H: number;
        readonly KEY_I: number;
        readonly KEY_J: number;
        readonly KEY_K: number;
        readonly KEY_L: number;
        readonly KEY_M: number;
        readonly KEY_N: number;
        readonly KEY_O: number;
        readonly KEY_P: number;
        readonly KEY_Q: number;
        readonly KEY_R: number;
        readonly KEY_S: number;
        readonly KEY_T: number;
        readonly KEY_U: number;
        readonly KEY_V: number;
        readonly KEY_W: number;
        readonly KEY_X: number;
        readonly KEY_Y: number;
        readonly KEY_Z: number;
        readonly KEY_LEFT_BRACKET: number;
        readonly KEY_BACKSLASH: number;
        readonly KEY_RIGHT_BRACKET: number;
        readonly KEY_GRAVE: number;
        readonly KEY_SPACE: number;
        readonly KEY_ESCAPE: number;
        readonly KEY_ENTER: number;
        readonly KEY_TAB: number;
        readonly KEY_BACKSPACE: number;
        readonly KEY_INSERT: number;
        readonly KEY_DELETE: number;
        readonly KEY_RIGHT: number;
        readonly KEY_LEFT: number;
        readonly KEY_DOWN: number;
        readonly KEY_UP: number;
        readonly KEY_PAGE_UP: number;
        readonly KEY_PAGE_DOWN: number;
        readonly KEY_HOME: number;
        readonly KEY_END: number;
        readonly KEY_CAPS_LOCK: number;
        readonly KEY_SCROLL_LOCK: number;
        readonly KEY_NUM_LOCK: number;
        readonly KEY_PRINT_SCREEN: number;
        readonly KEY_PAUSE: number;
        readonly KEY_F1: number;
        readonly KEY_F2: number;
        readonly KEY_F3: number;
        readonly KEY_F4: number;
        readonly KEY_F5: number;
        readonly KEY_F6: number;
        readonly KEY_F7: number;
        readonly KEY_F8: number;
        readonly KEY_F9: number;
        readonly KEY_F10: number;
        readonly KEY_F11: number;
        readonly KEY_F12: number;
        readonly KEY_LEFT_SHIFT: number;
        readonly KEY_LEFT_CONTROL: number;
        readonly KEY_LEFT_ALT: number;
        readonly KEY_LEFT_SUPER: number;
        readonly KEY_RIGHT_SHIFT: number;
        readonly KEY_RIGHT_CONTROL: number;
        readonly KEY_RIGHT_ALT: number;
        readonly KEY_RIGHT_SUPER: number;
        readonly KEY_KB_MENU: number;
        readonly KEY_KP_0: number;
        readonly KEY_KP_1: number;
        readonly KEY_KP_2: number;
        readonly KEY_KP_3: number;
        readonly KEY_KP_4: number;
        readonly KEY_KP_5: number;
        readonly KEY_KP_6: number;
        readonly KEY_KP_7: number;
        readonly KEY_KP_8: number;
        readonly KEY_KP_9: number;
        readonly KEY_KP_DECIMAL: number;
        readonly KEY_KP_DIVIDE: number;
        readonly KEY_KP_MULTIPLY: number;
        readonly KEY_KP_SUBTRACT: number;
        readonly KEY_KP_ADD: number;
        readonly KEY_KP_ENTER: number;
        readonly KEY_KP_EQUAL: number;
        readonly KEY_BACK: number;
        readonly KEY_MENU: number;
        readonly KEY_VOLUME_UP: number;
        readonly KEY_VOLUME_DOWN: number;
    };

    export interface UpdateContext {
        // --- Keyboard Inputs ---
        isKeyPressed(key: number): boolean;
        isKeyDown(key: number): boolean;
        isKeyReleased(key: number): boolean;
        isKeyUp(key: number): boolean;

        // --- Mouse Inputs ---
        isMouseButtonPressed(button: number): boolean;
        isMouseButtonDown(button: number): boolean;
        isMouseButtonReleased(button: number): boolean;
        isMouseButtonUp(button: number): boolean;

        // --- Mouse Positioning ---
        getMousePosition(): Vector2;
        getMouseX(): number;
        getMouseY(): number;
        getMouseWheelMove(): number;
    }

    /**
     * Options passed as a raw JS object to shape drawing methods (like drawRectangle).
     */
    export interface DrawOptions {
        readonly color?: Color;
        readonly rotation?: number;
        readonly wireframe?: boolean;
        readonly origin?: Vector2;
    }

    /**
     * Exposes individual primitive 2D structural rendering mechanisms.
     */
    export interface Render2DShapes {
        drawPixel(position: Vector2, color: Color): void;
        drawLine(startPosition: Vector2, endPosition: Vector2, options: DrawOptions): void;
        drawRectangle(rect: Rectangle, options: DrawOptions): void;
        drawCircle(centre: Vector2, radius: number, options: DrawOptions): void;
        drawTriangle(point1: Vector2, point2: Vector2, point3: Vector2, options: DrawOptions): void;
        drawEllipse(center: Vector2, radiusH: number, radiusV: number, options: DrawOptions): void;
    }

    /**
     * Context parameter exposed to the `onDraw` hook loop.
     */
    export interface RenderContext {
        drawFPS(position: Vector2): void;
        readonly shapes: Render2DShapes;
        withLayer2D(callback: (layer: RenderContext) => void): void;
    }

    /**
     * Custom font resource loaded from a path.
     */
    export class Font {
        constructor(path: string);
        constructor(path: string, baseSize: number);
    }

    /**
     * Custom options configuring text rendering layout and styling.
     */
    export interface TextOptions {
        readonly font?: Font;
        readonly color?: Color;
        readonly rotation?: number;
        readonly fontSize?: number;
        readonly spacing?: number;
        readonly origin?: Vector2;
    }

    /**
     * Context parameter exposed to the `onUpdate` hook loop.
     */
    export interface UpdateContext {
        // Currently instantated as empty object context inside create_update_context_object
    }

    /**
     * Hook listener declarations to tie context events explicitly to runtime loops.
     */
    export interface UserApplication {
        onInit?(this: UserApplication): void;
        onUpdate?(this: UserApplication, ctx: UpdateContext): void;
        onDraw?(this: UserApplication, render: RenderContext): void;
    }

    /**
     * Core Application wrapper executing the rendering pipeline.
     */
    export class Application {
        constructor(width: number, height: number, title: string);
        run(userApp: UserApplication): void;
    }

}