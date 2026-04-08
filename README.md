![Build Status](https://github.com/vectorjsdev/vectorjs/actions/workflows/verify-build.yml/badge.svg)
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
const screenWidth = 800;
const screenHeight = 450;

class MyApplication {

    constructor() {
        this.textColor = Color.DARKBLUE;
        const startPos = { x: 40, y: 40 };
        const lineSpacing = 30;
        this.titleTextPos = { x: startPos.x, y: startPos.y }
        this.totalMonitorTextPos = { x: startPos.x, y: startPos.y + lineSpacing * 2 }
        this.monitorTextPos = { x: startPos.x, y: startPos.y + lineSpacing * 3 }
        this.refreshRateTextPos = { x: startPos.x, y: startPos.y + lineSpacing * 4 }
        this.currentMonitorTextPos = { x: startPos.x, y: startPos.y + lineSpacing * 6 }
    }

    OnUpdate(dt, context) {
        // We can capture real-time window state from the updateContext
        this.currWidth = context.Screen.GetWidth();
        this.currHeight = context.Screen.GetHeight();

        // Update current Monitor info in case of changes (e.g., monitor added/removed)
        this.monitorIndex = context.Monitor.GetMonitor();
        this.monCount = context.Monitor.GetCount();
        this.physWidth = context.Monitor.GetPhysicalWidth(this.monitorIndex);
        this.physHeight = context.Monitor.GetPhysicalHeight(this.monitorIndex);
        this.refreshRate = context.Monitor.GetRefreshRate(this.monitorIndex);
    }

    OnDraw(render) {
        render.WithLayer2D((ctx) => {
            ctx.DrawText("MONITOR & WINDOW STATISTICS", this.titleTextPos, 25, Color.MAROON);

            // Displaying captured Monitor Info
            ctx.DrawText(`Total Monitors: ${this.monCount}`, this.totalMonitorTextPos, 20, this.textColor);
            ctx.DrawText(`Monitor ${this.monitorIndex} Physical Size: ${this.physWidth}mm x ${this.physHeight}mm`, this.monitorTextPos, 20, this.textColor);
            ctx.DrawText(`Refresh Rate: ${this.refreshRate} Hz`, this.refreshRateTextPos, 20, this.textColor);

            // Displaying real-time Window Info
            ctx.DrawText(`Current Window: ${this.currWidth}x${this.currHeight}`, this.currentMonitorTextPos, 20, Color.BLACK);

            ctx.DrawFPS({ x: 10, y: screenHeight - 30 });
        });
    }
};

const app = new App(screenHeight, screenWidth, "Monitor & Window Info");
app.Run(new MyApplication());
```