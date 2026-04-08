declare module "vectorjs" {
    /**
     * Vector2 math for 2D operations.
     */
    export class Vector2 {
        x: number;
        y: number;
        constructor(x?: number, y?: number);
        clone(): Vector2;
        copyFrom(v: Vector2): this;
        set(x: number, y: number): this;
        equals(v: Vector2, epsilon?: number): boolean;
        add(v: Vector2): this;
        sub(v: Vector2): this;
        scale(scalar: number): this;
        lengthSq(): number;
        length(): number;
        distanceTo(v: Vector2): number;
        normalize(): this;
        dot(v: Vector2): number;
        limit(max: number): this;
        static add(v1: Vector2, v2: Vector2): Vector2;
        static sub(v1: Vector2, v2: Vector2): Vector2;
        static lerp(v1: Vector2, v2: Vector2, t: number): Vector2;
    }

    export class Vector3 {
        x: number;
        y: number;
        z: number;
        constructor(x?: number, y?: number, z?: number);
        clone(): Vector3;
        copyFrom(v: Vector3): this;
        set(x: number, y: number, z: number): this;
        add(v: Vector3): this;
        sub(v: Vector3): this;
        scale(scalar: number): this;
        lengthSq(): number;
        length(): number;
        normalize(): this;
        dot(v: Vector3): number;
        cross(v: Vector3): this;
        static add(v1: Vector3, v2: Vector3): Vector3;
        static sub(v1: Vector3, v2: Vector3): Vector3;
        static cross(v1: Vector3, v2: Vector3): Vector3;
    }

    export class Color {
        r: number;
        g: number;
        b: number;
        a: number;
        constructor(r?: number, g?: number, b?: number, a?: number);
        set(r: number, g: number, b: number, a?: number): this;
        clone(): Color;
        static fromHex(hex: number): Color;
        static fromNormalized(r: number, g: number, b: number, a?: number): Color;
        static get WHITE(): Color;
        static get BLACK(): Color;
        static get RED(): Color;
        static get GREEN(): Color;
        static get BLUE(): Color;
        static get RAYWHITE(): Color;
        static get BLANK(): Color;
        static get SKYBLUE(): Color;
        static get DARKBLUE(): Color;
        static get MAROON(): Color;
        static get VIOLET(): Color;
        static get ORANGE(): Color;
        static GetRandomColor(): Color;
    }

    // --- Context Interfaces for Lifecycle Hooks ---

    export interface WindowContext {
        IsReady: () => boolean;
        IsMinimized: () => boolean;
        IsResized: () => boolean;
        IsHidden: () => boolean;
        IsFocused: () => boolean;
        IsFullscreen: () => boolean;
        ToggleFullscreen: () => void;
        SetTitle: (title: string) => void;
        SetTargetFPS: (fps: number) => void;
        SetPosition: (x: number, y: number) => void;
        SetMonitor: (monitor: number) => void;
        SetMinSize: (width: number, height: number) => void;
        SetMaxSize: (width: number, height: number) => void;
        SetFocused: () => void;
    }

    export interface ScreenContext {
        GetWidth: () => number;
        GetHeight: () => number;
    }

    export interface MonitorContext {
        GetCount: () => number;
        GetWidth: (monitor: number) => number;
        GetHeight: (monitor: number) => number;
        GetPhysicalWidth: (monitor: number) => number;
        GetPhysicalHeight: (monitor: number) => number;
        GetRefreshRate: (monitor: number) => number;
        GetMonitor: () => number;
    }

    export interface InitContext {
        Window: WindowContext;
        Screen: ScreenContext;
        Monitor: MonitorContext;
    }

    export interface UpdateContext extends InitContext {
        GetFrameTime: () => number;
        Input: {
            IsKeyDown: (key: number) => boolean;
            IsKeyUp: (key: number) => boolean;
            IsKeyPressed: (key: number) => boolean;
        };
        Mouse: {
            GetPosition: () => Vector2;
            GetDelta: () => Vector2;
            IsButtonDown: (button: number) => boolean;
            IsButtonUp: (button: number) => boolean;
            IsButtonPressed: (button: number) => boolean;
        };
    }

    export interface DrawContext2D {
        DrawFPS: (position: Vector2) => void;
        DrawRectangle: (position: Vector2, size: Vector2, color: Color) => void;
        DrawCircle: (position: Vector2, radius: number, color: Color) => void;
        DrawText: (text: string, position: Vector2, size: number, color: Color) => void;
    }

    export interface RenderContext {
        SetBackgroundColor: (color: Color) => void;
        WithLayer2D: (cb: (ctx: DrawContext2D) => void) => void;
    }

    export interface IGame {
        OnStart?: (ctx: InitContext) => void;
        OnUpdate?: (dt: number, ctx: UpdateContext) => void;
        OnDraw?: (ctx: RenderContext) => void;
        OnEnd?: () => void;
    }

    /**
     * Main App class to run the game loop.
     */
    export class App {
        constructor(height: number, width: number, title: string);
        Run(game: IGame): void;
        SetConfigFlag(flag: number): void;
        SetFPS(fps: number): void;
        SetExitKey(key: number): void;
    }
}