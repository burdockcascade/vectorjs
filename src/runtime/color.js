// color.js
export class Color {
    constructor(r = 255, g = 255, b = 255, a = 255) {
        this.r = r;
        this.g = g;
        this.b = b;
        this.a = a;
    }

    /** Set RGBA values directly */
    set(r, g, b, a = 255) {
        this.r = r;
        this.g = g;
        this.b = b;
        this.a = a;
        return this;
    }

    /** Returns a new Color with the same values */
    clone() {
        return new Color(this.r, this.g, this.b, this.a);
    }

    /** Helper to create from Hex (e.g., 0xFF0000FF) */
    static fromHex(hex) {
        return new Color(
            (hex >> 24) & 0xFF,
            (hex >> 16) & 0xFF,
            (hex >> 8) & 0xFF,
            hex & 0xFF
        );
    }

    /** Normalized floats (0.0 to 1.0) to Raylib bytes (0 to 255) */
    static fromNormalized(r, g, b, a = 1.0) {
        return new Color(
            Math.floor(r * 255),
            Math.floor(g * 255),
            Math.floor(b * 255),
            Math.floor(a * 255)
        );
    }

    // --- Common Raylib Colors (Static Constants) ---
    static get WHITE()     { return new Color(255, 255, 255, 255); }
    static get BLACK()     { return new Color(0, 0, 0, 255); }
    static get RED()       { return new Color(230, 41, 55, 255); }
    static get GREEN()     { return new Color(0, 228, 48, 255); }
    static get BLUE()      { return new Color(0, 121, 241, 255); }
    static get RAYWHITE()  { return new Color(245, 245, 245, 255); }
    static get BLANK()     { return new Color(0, 0, 0, 0); }

    // --- Fancy Colors ---
    static get SKYBLUE()   { return new Color(102, 191, 255, 255); }
    static get DARKBLUE()  { return new Color(0, 82, 172, 255); }
    static get MAROON()    { return new Color(190, 33, 55, 255); }
    static get VIOLET()    { return new Color(135, 60, 190, 255); }
    static get ORANGE()    { return new Color(255, 161, 0, 255); }
    

    /** Returns a random color */
    static GetRandomColor() {
        return new Color(
            Math.floor(Math.random() * 256),
            Math.floor(Math.random() * 256),
            Math.floor(Math.random() * 256),
            255
        );
    }
}