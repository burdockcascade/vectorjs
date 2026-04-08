// src/runtime/color.js
var Color = class _Color {
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
    return new _Color(this.r, this.g, this.b, this.a);
  }
  /** Helper to create from Hex (e.g., 0xFF0000FF) */
  static fromHex(hex) {
    return new _Color(
      hex >> 24 & 255,
      hex >> 16 & 255,
      hex >> 8 & 255,
      hex & 255
    );
  }
  /** Normalized floats (0.0 to 1.0) to Raylib bytes (0 to 255) */
  static fromNormalized(r, g, b, a = 1) {
    return new _Color(
      Math.floor(r * 255),
      Math.floor(g * 255),
      Math.floor(b * 255),
      Math.floor(a * 255)
    );
  }
  // --- Common Raylib Colors (Static Constants) ---
  static get WHITE() {
    return new _Color(255, 255, 255, 255);
  }
  static get BLACK() {
    return new _Color(0, 0, 0, 255);
  }
  static get RED() {
    return new _Color(230, 41, 55, 255);
  }
  static get GREEN() {
    return new _Color(0, 228, 48, 255);
  }
  static get BLUE() {
    return new _Color(0, 121, 241, 255);
  }
  static get RAYWHITE() {
    return new _Color(245, 245, 245, 255);
  }
  static get BLANK() {
    return new _Color(0, 0, 0, 0);
  }
  // --- Fancy Colors ---
  static get SKYBLUE() {
    return new _Color(102, 191, 255, 255);
  }
  static get DARKBLUE() {
    return new _Color(0, 82, 172, 255);
  }
  static get MAROON() {
    return new _Color(190, 33, 55, 255);
  }
  static get VIOLET() {
    return new _Color(135, 60, 190, 255);
  }
  static get ORANGE() {
    return new _Color(255, 161, 0, 255);
  }
  /** Returns a random color */
  static GetRandomColor() {
    return new _Color(
      Math.floor(Math.random() * 256),
      Math.floor(Math.random() * 256),
      Math.floor(Math.random() * 256),
      255
    );
  }
};

// src/runtime/app.js
var window_ctx = {
  IsReady: _rl.IsWindowReady,
  IsMinimized: _rl.IsWindowMinimized,
  IsResized: _rl.IsWindowResized,
  IsHidden: _rl.IsWindowHidden,
  IsFocused: _rl.IsWindowFocused,
  IsFullscreen: _rl.IsFullscreen,
  ToggleFullscreen: _rl.ToggleFullscreen,
  SetTitle: _rl.SetWindowTitle,
  SetTargetFPS: _rl.SetTargetFPS,
  SetPosition: _rl.SetWindowPosition,
  SetMonitor: _rl.SetWindowMonitor,
  SetMinSize: _rl.SetWindowMinSize,
  SetMaxSize: _rl.SetWindowMaxSize,
  SetFocused: _rl.SetWindowFocused
};
var screen_ctx = {
  GetWidth: _rl.GetScreenWidth,
  GetHeight: _rl.GetScreenHeight
};
var monitor_ctx = {
  GetCount: _rl.GetMonitorCount,
  GetWidth: _rl.GetMonitorWidth,
  GetHeight: _rl.GetMonitorHeight,
  GetPhysicalWidth: _rl.GetMonitorPhysicalWidth,
  GetPhysicalHeight: _rl.GetMonitorPhysicalHeight,
  GetRefreshRate: _rl.GetMonitorRefreshRate,
  GetMonitor: _rl.GetCurrentMonitor
};
var initContext = {
  Window: window_ctx,
  Screen: screen_ctx,
  Monitor: monitor_ctx
};
var updateContext = {
  GetFrameTime: _rl.GetFrameTime,
  Input: {
    IsKeyDown: _rl.IsKeyDown,
    IsKeyUp: _rl.IsKeyUp,
    IsKeyPressed: _rl.IsKeyPressed
  },
  Mouse: {
    GetPosition: _rl.GetMousePosition,
    GetDelta: _rl.GetMouseDelta,
    IsButtonDown: _rl.IsMouseButtonDown,
    IsButtonUp: _rl.IsMouseButtonUp,
    IsButtonPressed: _rl.IsMouseButtonPressed
  },
  Window: window_ctx,
  Screen: screen_ctx,
  Monitor: monitor_ctx
};
var drawContext2D = {
  DrawFPS: (position) => _rl.DrawFPS(position.x, position.y),
  DrawRectangle: (position, size, color) => _rl.DrawRectangle(position.x, position.y, size.x, size.y, color),
  DrawCircle: (position, radius, color) => _rl.DrawCircle(position.x, position.y, radius, color),
  DrawText: (text, position, size, color) => _rl.DrawText(text, position.x, position.y, size, color)
};
var renderContext = {
  SetBackgroundColor: _rl.SetBackgroundColor,
  WithLayer2D: (cb) => {
    cb(drawContext2D);
  }
};
var Application = class {
  constructor(height, width, title) {
    this.height = height;
    this.width = width;
    this.title = title;
    this.fps = 60;
  }
  Run(game) {
    _rl.InitWindow(this.width, this.height, this.title);
    _rl.SetTargetFPS(this.fps);
    if (game.OnStart) {
      game.OnStart(initContext);
    }
    while (!_rl.WindowShouldClose()) {
      if (game.OnUpdate) {
        game.OnUpdate(_rl.GetFrameTime(), updateContext);
      }
      _rl.BeginDrawing();
      _rl.ClearBackground(Color.WHITE);
      if (game.OnDraw) {
        game.OnDraw(renderContext);
      }
      _rl.EndDrawing();
    }
    if (game.OnEnd) {
      game.OnEnd();
    }
    _rl.CloseWindow();
  }
  SetConfigFlag(flag) {
    _rl.SetConfigFlag(flag);
  }
  SetFPS(fps) {
    this.fps = fps;
  }
  SetExitKey(key) {
    _rl.SetExitKey(key);
  }
};

// src/runtime/math.js
var Vector2 = class _Vector2 {
  constructor(x = 0, y = 0) {
    this.x = x;
    this.y = y;
  }
  // --- Utility Methods ---
  clone() {
    return new _Vector2(this.x, this.y);
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
  equals(v, epsilon = 1e-5) {
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
    if (lenSq > 1e-7) {
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
    return new _Vector2(v1.x + v2.x, v1.y + v2.y);
  }
  static sub(v1, v2) {
    return new _Vector2(v1.x - v2.x, v1.y - v2.y);
  }
  static lerp(v1, v2, t) {
    return new _Vector2(
      v1.x + (v2.x - v1.x) * t,
      v1.y + (v2.y - v1.y) * t
    );
  }
};
var Vector3 = class _Vector3 {
  constructor(x = 0, y = 0, z = 0) {
    this.x = x;
    this.y = y;
    this.z = z;
  }
  // --- Utility Methods ---
  clone() {
    return new _Vector3(this.x, this.y, this.z);
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
    if (lenSq > 1e-7) {
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
    return new _Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
  }
  static sub(v1, v2) {
    return new _Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
  }
  static cross(v1, v2) {
    return new _Vector3(
      v1.y * v2.z - v1.z * v2.y,
      v1.z * v2.x - v1.x * v2.z,
      v1.x * v2.y - v1.y * v2.x
    );
  }
};
export {
  Application,
  Color,
  Vector2,
  Vector3
};
