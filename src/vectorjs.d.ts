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