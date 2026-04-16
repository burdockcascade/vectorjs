![Build Status](https://github.com/burdockcascade/vectorjs/actions/workflows/verify-build.yml/badge.svg)
![License](https://img.shields.io/github/license/burdockcascade/vectorjs)
![C++ 23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)
[![Powered by Raylib](https://img.shields.io/badge/Raylib-5.5-blue)](https://github.com/raysan5/raylib)
[![QuickJS-ng](https://img.shields.io/badge/QuickJS-0.14.0-blue)](https://github.com/quickjs-ng/quickjs)
[![xmake](https://img.shields.io/badge/build-xmake-007ACC?logo=cplusplus&logoColor=white)](https://xmake.io)

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

### Analog Clock
```js
import { Core, Shapes, Text, Info, Palette } from "Raylib";

// 1. Setup
const screenWidth = 600;
const screenHeight = 600;
const centerX = screenWidth / 2;
const centerY = screenHeight / 2;
const clockRadius = 250;
const isTicking = true;

Core.InitWindow(screenWidth, screenHeight, "Raylib JS - Analog Clock");
Core.SetTargetFPS(60);

const colors = {
    bg: Palette.DARKGRAY,
    face: Palette.WHITE,
    hourHand: Palette.BLACK,
    minuteHand: Palette.BLACK,
    secondHand: Palette.RED,
    ticks: Palette.BLACK
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

// 2. Main Loop
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

// 3. Cleanup
Core.CloseWindow();
```