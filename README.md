![Build Status](https://github.com/burdockcascade/vectorjs/actions/workflows/verify-build.yml/badge.svg)
![License](https://img.shields.io/badge/License-MIT-green)
![C++ 23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)
![Powered by Raylib](https://img.shields.io/badge/Powered%20by-Raylib_5.5-blue)
![QuickJS-ng](https://img.shields.io/badge/QuickJS--ng-0.13.0-orange)

# VectorJS
VectorJS is a lightweight, cross-platform game engine and runner that allows you to build 2D and 3D games/apps using JavaScript powered by the high-performance [Raylib](https://github.com/raysan5/raylib) graphics library and the [QuickJS-NG](https://github.com/quickjs-ng/quickjs) engine.

## Status
This project is current work-in-progress and the API will change without warning.

## Features
* Fast JavaScript Execution: Uses QuickJS-ng for a small footprint and high-speed script execution.
* Raylib Integration: Built-in bindings for Raylib functions including window management, 2D drawing, 3D drawing, input handling, and timing.
* Modern JS API: Provides a clean, object-oriented JavaScript API for application management, math (Vector2/Vector3), and colors.
* Cross-Platform: Runs on Windows, Linux, and macOS.

## Example
### Simple
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

### Analog Clock
```js
import { Core, Shapes, Text, Info } from "Raylib";

// Setup
const screenWidth = 600;
const screenHeight = 600;
const centerX = screenWidth / 2;
const centerY = screenHeight / 2;
const clockRadius = 250;
const isTicking = true;

Core.InitWindow(screenWidth, screenHeight, "Raylib JS - Analog Clock");
Core.SetTargetFPS(60);

// Colors
const colors = {
    bg: { r: 30, g: 30, b: 30, a: 255 },
    face: { r: 240, g: 240, b: 240, a: 255 },
    hourHand: { r: 50, g: 50, b: 50, a: 255 },
    minuteHand: { r: 80, g: 80, b: 80, a: 255 },
    secondHand: { r: 230, g: 30, b: 30, a: 255 }, // Red
    ticks: { r: 100, g: 100, b: 100, a: 255 }
};

function drawHand(value, max, length, thickness, color) {
    let angle = (value / max) * 360 * (Math.PI / 180);
    let endX = centerX + Math.sin(angle) * length;
    let endY = centerY - Math.cos(angle) * length;
    Shapes.DrawLineEx(
        { x: centerX, y: centerY },
        { x: endX, y: endY },
        thickness,
        color
    );
}

// Main Loop
while (!Core.WindowShouldClose()) {
    // Get Current Time
    const now = new Date();
    const sec = isTicking
        ? now.getSeconds()
        : now.getSeconds() + (now.getMilliseconds() / 1000);
    const min = now.getMinutes() + sec / 60;
    const hrs = (now.getHours() % 12) + min / 60;

    Core.BeginDrawing();
    Core.ClearBackground(colors.bg);

    // Draw Clock Face
    Shapes.DrawCircle(centerX, centerY, clockRadius, colors.face);
    Shapes.DrawCircleLines(centerX, centerY, clockRadius, colors.hourHand);

    // Draw Hour Ticks
    for (let i = 0; i < 12; i++) {
        let angle = i * 30 * (Math.PI / 180);
        let startX = centerX + Math.sin(angle) * (clockRadius - 20);
        let startY = centerY - Math.cos(angle) * (clockRadius - 20);
        let endX = centerX + Math.sin(angle) * clockRadius;
        let endY = centerY - Math.cos(angle) * clockRadius;
        Shapes.DrawLineEx({x: startX, y: startY}, {x: endX, y: endY}, 4, colors.ticks);
    }

    // Hour Hand (12-hour scale)
    drawHand(hrs, 12, clockRadius * 0.5, 8, colors.hourHand);

    // Minute Hand (60-minute scale)
    drawHand(min, 60, clockRadius * 0.75, 5, colors.minuteHand);

    // Second Hand (60-second scale)
    drawHand(sec, 60, clockRadius * 0.85, 2, colors.secondHand);

    // Center Pin
    Shapes.DrawCircle(centerX, centerY, 8, colors.hourHand);

    // Footer Info
    Text.DrawText("Raylib Version: " + Info.Version, 10, screenHeight - 25, 20, colors.face);

    Core.EndDrawing();
}

// Cleanup
Core.CloseWindow();
```