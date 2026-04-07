export class Vector2 {
    constructor(x = 0, y = 0) {
        this.x = x;
        this.y = y;
    }

    // --- Utility Methods ---

    clone() {
        return new Vector2(this.x, this.y);
    }

    copyFrom(v) {
        this.x = v.x;
        this.y = v.y;
        return this;
    }

    set(x, y) {
        this.x = x;
        this.y = y;
        return this;
    }

    equals(v, epsilon = 0.00001) {
        return Math.abs(this.x - v.x) < epsilon && Math.abs(this.y - v.y) < epsilon;
    }

    // --- Math Operations (In-place) ---

    add(v) {
        this.x += v.x;
        this.y += v.y;
        return this;
    }

    sub(v) {
        this.x -= v.x;
        this.y -= v.y;
        return this;
    }

    scale(scalar) {
        this.x *= scalar;
        this.y *= scalar;
        return this;
    }

    // --- Calculations ---

    /** Returns the squared length. Faster than length() for comparisons. */
    lengthSq() {
        return this.x * this.x + this.y * this.y;
    }

    length() {
        return Math.sqrt(this.lengthSq());
    }

    distanceTo(v) {
        const dx = this.x - v.x;
        const dy = this.y - v.y;
        return Math.sqrt(dx * dx + dy * dy);
    }

    normalize() {
        const lenSq = this.lengthSq();
        // Check against a tiny epsilon to avoid division by zero/NaN
        if (lenSq > 0.0000001) {
            this.scale(1 / Math.sqrt(lenSq));
        }
        return this;
    }

    dot(v) {
        return this.x * v.x + this.y * v.y;
    }

    /** Constrains the vector magnitude to a maximum value */
    limit(max) {
        const mSq = this.lengthSq();
        if (mSq > max * max) {
            this.divide(Math.sqrt(mSq)).scale(max);
        }
        return this;
    }

    // --- Static Helpers ---

    static add(v1, v2) {
        return new Vector2(v1.x + v2.x, v1.y + v2.y);
    }

    static sub(v1, v2) {
        return new Vector2(v1.x - v2.x, v1.y - v2.y);
    }

    static lerp(v1, v2, t) {
        return new Vector2(
            v1.x + (v2.x - v1.x) * t,
            v1.y + (v2.y - v1.y) * t
        );
    }
}

/** * Vector3 Class
 * Represents a 3D vector (x, y, z) with in-place operations to minimize GC.
 */
export class Vector3 {
    constructor(x = 0, y = 0, z = 0) {
        this.x = x;
        this.y = y;
        this.z = z;
    }

    // --- Utility Methods ---

    clone() {
        return new Vector3(this.x, this.y, this.z);
    }

    copyFrom(v) {
        this.x = v.x;
        this.y = v.y;
        this.z = v.z;
        return this;
    }

    set(x, y, z) {
        this.x = x;
        this.y = y;
        this.z = z;
        return this;
    }

    // --- Math Operations (In-place) ---

    add(v) {
        this.x += v.x;
        this.y += v.y;
        this.z += v.z;
        return this;
    }

    sub(v) {
        this.x -= v.x;
        this.y -= v.y;
        this.z -= v.z;
        return this;
    }

    scale(scalar) {
        this.x *= scalar;
        this.y *= scalar;
        this.z *= scalar;
        return this;
    }

    // --- Calculations ---

    lengthSq() {
        return this.x * this.x + this.y * this.y + this.z * this.z;
    }

    length() {
        return Math.sqrt(this.lengthSq());
    }

    normalize() {
        const lenSq = this.lengthSq();
        if (lenSq > 0.0000001) {
            this.scale(1 / Math.sqrt(lenSq));
        }
        return this;
    }

    dot(v) {
        return this.x * v.x + this.y * v.y + this.z * v.z;
    }

    /** * Calculates the Cross Product (this x v).
     * Useful for finding a vector perpendicular to two others.
     */
    cross(v) {
        const ax = this.x, ay = this.y, az = this.z;
        const bx = v.x, by = v.y, bz = v.z;

        this.x = ay * bz - az * by;
        this.y = az * bx - ax * bz;
        this.z = ax * by - ay * bx;
        
        return this;
    }

    // --- Static Helpers ---

    static add(v1, v2) {
        return new Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    }

    static sub(v1, v2) {
        return new Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    }

    static cross(v1, v2) {
        return new Vector3(
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x
        );
    }
}