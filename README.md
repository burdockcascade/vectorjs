![Build Status](https://github.com/burdockcascade/vectorjs/actions/workflows/verify-build.yml/badge.svg)
![License](https://img.shields.io/badge/License-MIT-green)
![C++ 23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)
![Powered by Raylib](https://img.shields.io/badge/Powered%20by-Raylib_5.5-blue)
![QuickJS-ng](https://img.shields.io/badge/QuickJS--ng-0.13.0-orange)

# VectorJS
VectorJS is a lightweight, cross-platform game engine and runner that allows you to build 2D and 3D games/apps using JavaScript powered by the high-performance [Raylib](https://github.com/raysan5/raylib) graphics library and the [QuickJS-NG](https://github.com/quickjs-ng/quickjs) engine.

## Features
* Fast JavaScript Execution: Uses QuickJS-ng for a small footprint and high-speed script execution.
* Raylib Integration: Built-in bindings for Raylib functions including window management, 2D drawing, 3D drawing, input handling, and timing.
* Modern JS API: Provides a clean, object-oriented JavaScript API for application management, math (Vector2/Vector3), and colors.
* Cross-Platform: Runs on Windows, Linux, and macOS.

## Example
```js
import { Core, Text, Shapes } from "Raylib";

Core.InitWindow(800, 600, "Hello Raylib Module!");

while (!Core.WindowShouldClose()) {
    Core.BeginDrawing();
    Core.ClearBackground({ r: 255, g: 255, b: 255, a: 255 });
    Shapes.DrawRectangle(100, 100, 200, 200, { r: 0, g: 255, b: 0, a: 255 });
    Text.DrawText("Hello Raylib Module!", 190, 200, 20, { r: 255, g: 0, b: 0, a: 255 });
    Core.EndDrawing();
}
Core.CloseWindow();
```