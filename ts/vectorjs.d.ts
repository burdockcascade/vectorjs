declare module "vectorjs" {
    export class Vector2 {
        constructor(x: number, y: number);
        x: number;
        y: number;

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
        constructor(r: number, g: number, b: number, a: number);
        r: number;
        g: number;
        b: number;
        a: number;

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

    export class Rectangle {
        constructor(x: number, y: number, width: number, height: number);
        x: number;
        y: number;
        width: number;
        height: number;
    }

    export class Font {
        constructor(path: string, baseSize?: number);
    }

    export class Camera2D {
        constructor(
            offset?: Vector2,
            target?: Vector2,
            rotation?: number,
            zoom?: number
        );

        offset: Vector2;
        target: Vector2;
        rotation: number;
        zoom: number;

        move(deltaX: number, deltaY: number): void;
        move(delta: Vector2): void;
        moveX(deltaX: number): void;
        moveY(deltaY: number): void;
        zoomBy(factor: number): void;
        zoomIn(amount?: number): void;
        zoomOut(amount?: number): void;
    }

    export interface DrawOptions {
        color?: Color;
        rotation?: number;
        wireframe?: boolean;
        origin?: Vector2;
    }

    export interface TextOptions {
        font?: Font;
        color?: Color;
        rotation?: number;
        fontSize?: number;
        spacing?: number;
        origin?: Vector2;
    }

    export interface ShapesApi {
        drawPixel(pos: Vector2, color: Color): void;
        drawLine(start: Vector2, end: Vector2, options?: DrawOptions): void;
        drawRectangle(rect: Rectangle, options?: DrawOptions): void;
        drawCircle(center: Vector2, radius: number, options?: DrawOptions): void;
        drawTriangle(
            p1: Vector2,
            p2: Vector2,
            p3: Vector2,
            options?: DrawOptions
        ): void;
        drawEllipse(
            center: Vector2,
            radiusH: number,
            radiusV: number,
            options?: DrawOptions
        ): void;
    }

    export interface TextApi {
        drawText(pos: Vector2, text: string, options?: TextOptions): void;
    }

    export interface Render2DApi {
        drawFPS(pos: Vector2): void;
        shapes: ShapesApi;
        text: TextApi;
    }

    export interface RenderContext {
        clearBackground(color?: Color): void;
        withViewport2D(
            camera: Camera2D,
            callback: (r2d: Render2DApi) => void
        ): void;
        withViewport2D(callback: (r2d: Render2DApi) => void): void;
        withScreenSpace(callback: (r2d: Render2DApi) => void): void;
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
        isKeyPressed(keyCode: number): boolean;
        isKeyDown(keyCode: number): boolean;
        isKeyReleased(keyCode: number): boolean;
        isKeyUp(keyCode: number): boolean;

        // Mouse Button Checks
        isMouseButtonPressed(button: number): boolean;
        isMouseButtonDown(button: number): boolean;
        isMouseButtonReleased(button: number): boolean;
        isMouseButtonUp(button: number): boolean;

        // Custom Mouse Getters
        getMouseWheelMove(): number;
        getMousePosition(): Vector2;
    }

    export interface ApplicationConfig {
        onInit?: (this: ApplicationConfig) => void;
        onUpdate?: (this: ApplicationConfig, ctx: UpdateContext) => void;
        onDraw?: (this: ApplicationConfig, render: RenderContext) => void;
    }

    export class Application {
        constructor(width: number, height: number, title: string);
        run(config: ApplicationConfig): void;
    }

    export const Info: {
        readonly RAYLIB_VERSION_STR: string;
        readonly QUICKJS_VERSION_STR: string;
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

    /**
     * Represents a short audio clip loaded entirely into memory.
     * Ideal for sound effects and quick, repeatable noises.
     */
    export class Sound {
        /**
         * Loads a sound from the specified file path.
         * @param path The file path to the audio file (e.g., .wav, .ogg, .mp3)
         */
        constructor(path: string);

        /** Plays the sound. */
        play(): void;

        /** Stops the sound if it is currently playing. */
        stop(): void;

        /**
         * Sets the playback volume of the sound.
         * @param volume Volume level, where 1.0 is max and 0.0 is silent.
         */
        setVolume(volume: number): void;
    }

    /**
     * Represents a streaming audio track.
     * Ideal for background music (BGM) or long ambient tracks to save RAM.
     */
    export class Music {
        /**
         * Opens an audio stream from the specified file path.
         * @param path The file path to the audio file (e.g., .wav, .ogg, .mp3)
         */
        constructor(path: string);

        /** Starts playing the music stream. */
        play(): void;

        /** Stops the music stream and resets it to the beginning. */
        stop(): void;

        /** Pauses the music stream at its current position. */
        pause(): void;

        /** Resumes a paused music stream. */
        resume(): void;

        /**
         * Sets the playback volume of the music.
         * @param volume Volume level, where 1.0 is max and 0.0 is silent.
         */
        setVolume(volume: number): void;

        /**
         * Refills the audio buffer.
         * **Note:** This MUST be called every frame in your update loop for the music to keep playing!
         */
        update(): void;
    }
}