declare module "vectorjs" {

    // Type definitions for VectorJS / Raylib bindings

    export interface JSDrawOptions {
        color?: Color;
        origin?: Vector2;
        rotation?: number;
        wireframe?: boolean;
    }

    export interface JSTextOptions {
        font?: Font;
        color?: Color;
        rotation?: number;
        fontSize?: number;
        spacing?: number;
        origin?: Vector2;
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

    export interface ShapesRenderer {
        drawPixel(pos: Vector2, color: Color): void;
        drawLine(start: Vector2, end: Vector2, options?: JSDrawOptions): void;
        drawRectangle(rect: Rectangle, options?: JSDrawOptions): void;
        drawCircle(center: Vector2, radius: number, options?: JSDrawOptions): void;
        drawTriangle(p1: Vector2, p2: Vector2, p3: Vector2, options?: JSDrawOptions): void;
        drawEllipse(center: Vector2, radiusH: number, radiusV: number, options?: JSDrawOptions): void;
    }

    export interface TextRenderer {
        drawText(pos: Vector2, text: string, options?: JSTextOptions): void;
    }

    export interface Render2DContext {
        drawFPS(pos: Vector2): void;
        shapes: ShapesRenderer;
        text: TextRenderer;
    }

    export interface RenderContext {
        withViewport2D(camera: Camera2D, callback: (r2d: Render2DContext) => void): void;
        withScreenSpace(callback: (r2d: Render2DContext) => void): void;
        clearBackground(color?: Color): void;
    }

    export interface UserApplicationHooks {
        onInit?(): void;
        onUpdate?(ctx: UpdateContext): void;
        onDraw?(ctx: RenderContext): void;
        onEvent?(): void;
    }

    export class Application {
        constructor(width: number, height: number, title: string);
        run(userApp: UserApplicationHooks): void;
    }

    export class Vector2 {
        constructor(x: number, y: number);
        x: number;
        y: number;

        add(other: Vector2): Vector2;
        scale(factor: number): Vector2;
        length(): number;
        normalize(): Vector2;
        subtract(other: Vector2): Vector2;
        multiply(other: Vector2): Vector2;
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

    export class Rectangle {
        constructor(x: number, y: number, width: number, height: number);
        x: number;
        y: number;
        width: number;
        height: number;

        contains(point: Vector2): boolean;
        overlaps(other: Rectangle): boolean;
        getCollisionRect(other: Rectangle): Rectangle;
    }

    export class Circle {
        constructor(x: number, y: number, radius: number);
        x: number;
        y: number;
        radius: number;

        contains(point: Vector2): boolean;
        overlaps(other: Circle | Rectangle): boolean;
    }

    export class Triangle {
        constructor(v1: Vector2, v2: Vector2, v3: Vector2);
        v1: Vector2;
        v2: Vector2;
        v3: Vector2;

        contains(point: Vector2): boolean;
    }

    export class Point {
        constructor(x: number, y: number);
        x: number;
        y: number;

        intersects(shape: Rectangle | Circle | Triangle): boolean;
    }

    export class Line {
        constructor(start: Vector2, end: Vector2);
        start: Vector2;
        end: Vector2;

        intersects(other: Line): boolean;
        getIntersection(other: Line): Vector2 | null;
    }

    export class Polygon {
        constructor(points: Vector2[]);
        readonly vertexCount: number;

        getPoint(index: number): Vector2 | null;
        contains(point: Vector2): boolean;
    }

    export class Camera2D {
        constructor(offset?: Vector2, target?: Vector2, rotation?: number, zoom?: number);
        target: Vector2;
        offset: Vector2;
        rotation: number;
        zoom: number;

        move(x: number, y: number): void;
        move(delta: Vector2): void;
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

    export const Palette: Record<string, Color>;

    export const Info: {
        RAYLIB_VERSION: string;
        QUICKJS_VERSION: string;
    };

    export const ConfigFlags: Record<string, number>;
    export const Keyboard: Record<string, number>;
    export const MouseButton: Record<string, number>;
    export const MouseCursor: Record<string, number>;

}