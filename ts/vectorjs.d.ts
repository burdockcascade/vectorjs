/**
 * TypeScript definitions for the `vectorjs` module.
 */
declare module "vectorjs" {
    // =========================================================================
    // Core & Options Interfaces
    // =========================================================================

    export interface DrawOptions {
        color?: Color;
        origin?: Vector2;
        rotation?: number;
        wireframe?: boolean;
    }

    export interface TextOptions {
        font?: Font;
        color?: Color;
        rotation?: number;
        fontSize?: number;
        spacing?: number;
        origin?: Vector2;
    }

    export interface ShapesContext {
        drawPixel(position: Vector2, color: Color): void;
        drawLine(start: Vector2, end: Vector2, options?: DrawOptions): void;
        drawRectangle(rectangle: Rectangle, options?: DrawOptions): void;
        drawCircle(center: Vector2, radius: number, options?: DrawOptions): void;
        drawTriangle(p1: Vector2, p2: Vector2, p3: Vector2, options?: DrawOptions): void;
        drawEllipse(center: Vector2, radiusH: number, radiusV: number, options?: DrawOptions): void;
    }

    export interface TextContext {
        drawText(position: Vector2, text: string, options?: TextOptions): void;
    }

    export interface Render2DContext {
        drawFPS(position: Vector2): void;
        shapes: ShapesContext;
        text: TextContext;
    }

    export interface RenderContext {
        clearBackground(color?: Color): void;
        withViewport2D(camera: Camera2D, callback: (r2d: Render2DContext) => void): void;
        withScreenSpace(callback: (r2d: Render2DContext) => void): void;
    }

    export interface UpdateContext {
        // Window Actions
        minimizeWindow(): void;
        maximizeWindow(): void;

        // Window Properties
        isWindowFullscreen(): boolean;
        isWindowHidden(): boolean;
        isWindowResized(): boolean;
        isWindowMinimized(): boolean;
        isWindowMaximized(): boolean;
        isWindowFocused(): boolean;

        // Monitor
        getScreenWidth(): number;
        getScreenHeight(): number;
        getRenderWidth(): number;
        getRenderHeight(): number;
        getMonitorCount(): number;
        getCurrentMonitor(): number;

        // Keyboard Checks
        isKeyPressed(key: number): boolean;
        isKeyDown(key: number): boolean;
        isKeyReleased(key: number): boolean;
        isKeyUp(key: number): boolean;

        // Mouse Checks
        isMouseButtonPressed(button: number): boolean;
        isMouseButtonDown(button: number): boolean;
        isMouseButtonReleased(button: number): boolean;
        isMouseButtonUp(button: number): boolean;
        getMouseWheelMove(): number;
        getMousePosition(): Vector2;
    }

    export interface AppConfig {
        onInit?: (this: AppConfig) => void;
        onUpdate?: (this: AppConfig, ctx: UpdateContext) => void;
        onDraw?: (this: AppConfig, ctx: RenderContext) => void;
        onEvent?: (this: AppConfig) => void;
    }

    // =========================================================================
    // Classes
    // =========================================================================

    export class Application {
        constructor(width: number, height: number, title: string);
        run(config: AppConfig): void;
    }

    export class Vector2 {
        x: number;
        y: number;
        constructor(x: number, y: number);
        add(other: Vector2): Vector2;
        subtract(other: Vector2): Vector2;
        multiply(other: Vector2): Vector2;
        scale(factor: number): Vector2;
        length(): number;
        normalize(): Vector2;
        dot(other: Vector2): number;
        distance(other: Vector2): number;
        negate(): Vector2;
        lerp(target: Vector2, factor: number): Vector2;
    }

    export class Color {
        r: number;
        g: number;
        b: number;
        a: number;
        constructor(r: number, g: number, b: number, a: number);
        lerp(target: Color, factor: number): Color;
        fade(alpha: number): Color;
        brightness(factor: number): Color;
        toInt(): number;
        contrast(factor: number): Color;
        alpha(alpha: number): Color;
        tint(tintColor: Color): Color;
        isEqual(other: Color): boolean;

        static fromHex(hex: number): Color;
        static fromHSV(hue: number, saturation: number, value: number): Color;
    }

    export class Camera2D {
        target: Vector2;
        offset: Vector2;
        rotation: number;
        zoom: number;

        constructor();
        constructor(offset: Vector2, target: Vector2, rotation?: number, zoom?: number);

        move(delta: Vector2): void;
        move(x: number, y: number): void;
        moveX(deltaX: number): void;
        moveY(deltaY: number): void;
        zoomBy(factor: number): void;
        zoomIn(amount?: number): void;
        zoomOut(amount?: number): void;
        toWorldSpace(screenPos: Vector2): Vector2;
        toScreenSpace(worldPos: Vector2): Vector2;
    }

    export class Font {
        constructor(path: string, fontSize?: number);
    }

    export class Sound {
        constructor(path: string);
        play(): void;
        pause(): void;
        resume(): void;
        stop(): void;
        setVolume(volume: number): void;
    }

    export class Music {
        constructor(path: string);
        play(): void;
        pause(): void;
        resume(): void;
        stop(): void;
        update(): void;
        setVolume(volume: number): void;
    }

    export class File {
        constructor(path?: string);
        readText(): string;
        writeText(content: string): boolean;
        appendText(content: string): boolean;
        exists(): boolean;
        remove(): boolean;

        static readText(path: string): string;
        static writeText(path: string, content: string): boolean;
        static exists(path: string): boolean;
    }

    // =========================================================================
    // Geometry
    // =========================================================================

    export class Rectangle {
        x: number;
        y: number;
        width: number;
        height: number;
        constructor(x: number, y: number, width: number, height: number);
        contains(point: Vector2): boolean;
        overlaps(other: Rectangle): boolean;
        getCollisionRect(other: Rectangle): Rectangle;
    }

    export class Circle {
        x: number;
        y: number;
        radius: number;
        constructor(x: number, y: number, radius: number);
        contains(point: Vector2): boolean;
        overlaps(other: Circle | Rectangle): boolean;
    }

    export class Triangle {
        v1: Vector2;
        v2: Vector2;
        v3: Vector2;
        constructor(v1: Vector2, v2: Vector2, v3: Vector2);
        contains(point: Vector2): boolean;
    }

    export class Point {
        x: number;
        y: number;
        constructor(x: number, y: number);
        intersects(shape: Rectangle | Circle | Triangle): boolean;
    }

    export class Line {
        start: Vector2;
        end: Vector2;
        constructor(p1: Vector2, p2: Vector2);
        intersects(other: Line): boolean;
        getIntersection(other: Line): Vector2 | null;
    }

    export class Polygon {
        readonly vertexCount: number;
        constructor(points: Vector2[]);
        getPoint(index: number): Vector2 | null;
        contains(point: Vector2): boolean;
    }

    // =========================================================================
    // Palette & Enums / Constants
    // =========================================================================

    export const Palette: {
        readonly WHITE: Color;
        readonly SNOW: Color;
        readonly IVORY: Color;
        readonly RAYWHITE: Color;
        readonly OFFWHITE: Color;
        readonly GAINSBORO: Color;
        readonly LIGHTGRAY: Color;
        readonly SILVER: Color;
        readonly GRAY: Color;
        readonly SLATE: Color;
        readonly DARKGRAY: Color;
        readonly CHARBLACK: Color;
        readonly BLACK: Color;
        readonly BLANK: Color;
        readonly SALMON: Color;
        readonly CORAL: Color;
        readonly TOMATO: Color;
        readonly CRIMSON: Color;
        readonly RED: Color;
        readonly PURE_RED: Color;
        readonly SCARLET: Color;
        readonly MAROON: Color;
        readonly BURGUNDY: Color;
        readonly PEACH: Color;
        readonly ORANGE: Color;
        readonly DARKORANGE: Color;
        readonly AMBER: Color;
        readonly GOLD: Color;
        readonly YELLOW: Color;
        readonly LEMON: Color;
        readonly ROSE: Color;
        readonly PINK: Color;
        readonly HOTPINK: Color;
        readonly DEEPPINK: Color;
        readonly MAGENTA: Color;
        readonly FUCHSIA: Color;
        readonly MINT: Color;
        readonly PASTELGREEN: Color;
        readonly LIME: Color;
        readonly LIMEGREEN: Color;
        readonly NEON_GREEN: Color;
        readonly GREEN: Color;
        readonly PURE_GREEN: Color;
        readonly EMERALD: Color;
        readonly DARKGREEN: Color;
        readonly FOREST: Color;
        readonly PINE: Color;
        readonly OLIVE: Color;
        readonly TURQUOISE: Color;
        readonly TEAL: Color;
        readonly SEA_GREEN: Color;
        readonly ELECTRIC_BLUE: Color;
        readonly CYAN: Color;
        readonly AQUA: Color;
        readonly PASTELBLUE: Color;
        readonly SKYBLUE: Color;
        readonly DEEPSKYBLUE: Color;
        readonly CORNFLOWER: Color;
        readonly BLUE: Color;
        readonly PURE_BLUE: Color;
        readonly ROYALBLUE: Color;
        readonly DARKBLUE: Color;
        readonly NAVY: Color;
        readonly MIDNIGHTBLUE: Color;
        readonly LAVENDER: Color;
        readonly LILAC: Color;
        readonly PURPLE: Color;
        readonly PURE_PURPLE: Color;
        readonly AMETHYST: Color;
        readonly VIOLET: Color;
        readonly INDIGO: Color;
        readonly DARKPURPLE: Color;
        readonly PLUM: Color;
        readonly ORCHID: Color;
        readonly WHEAT: Color;
        readonly BEIGE: Color;
        readonly TAN: Color;
        readonly SAND: Color;
        readonly KHAKI: Color;
        readonly TERRACOTTA: Color;
        readonly BRONZE: Color;
        readonly COPPER: Color;
        readonly RUST: Color;
        readonly BROWN: Color;
        readonly SADDLEBROWN: Color;
        readonly DARKBROWN: Color;
        readonly COFFEE: Color;
        readonly CHOCOLATE: Color;
    };

    export const Info: {
        readonly RAYLIB_VERSION: string;
        readonly QUICKJS_VERSION: string;
    };

    export const ConfigFlags: {
        readonly FLAG_VSYNC_HINT: number;
        readonly FLAG_FULLSCREEN_MODE: number;
        readonly FLAG_WINDOW_RESIZABLE: number;
        readonly FLAG_WINDOW_UNDECORATED: number;
        readonly FLAG_WINDOW_HIDDEN: number;
        readonly FLAG_WINDOW_MINIMIZED: number;
        readonly FLAG_WINDOW_MAXIMIZED: number;
        readonly FLAG_WINDOW_UNFOCUSED: number;
        readonly FLAG_WINDOW_TOPMOST: number;
        readonly FLAG_WINDOW_ALWAYS_RUN: number;
        readonly FLAG_WINDOW_TRANSPARENT: number;
        readonly FLAG_WINDOW_HIGHDPI: number;
        readonly FLAG_WINDOW_MOUSE_PASSTHROUGH: number;
        readonly FLAG_BORDERLESS_WINDOWED_MODE: number;
        readonly FLAG_MSAA_4X_HINT: number;
        readonly FLAG_INTERLACED_HINT: number;
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
}