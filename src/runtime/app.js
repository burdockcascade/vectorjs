import { Core, Shapes, Text } from "Raylib";
import { Color } from './color.js';

const window_ctx = {
    IsReady: Core.IsWindowReady,
    IsMinimized: Core.IsWindowMinimized,
    IsResized: Core.IsWindowResized,
    IsHidden: Core.IsWindowHidden,
    IsFocused: Core.IsWindowFocused,
    IsFullscreen: Core.IsWindowFullscreen,
    ToggleFullscreen: Core.ToggleFullscreen,
    SetTitle: Core.SetWindowTitle,
    SetTargetFPS: Core.SetTargetFPS,
    SetPosition: Core.SetWindowPosition,
    SetMonitor: Core.SetWindowMonitor,
    SetMinSize: Core.SetWindowMinSize,
    SetMaxSize: Core.SetWindowMaxSize,
    SetFocused: Core.SetWindowFocused,
}

const screen_ctx = {
    GetWidth: Core.GetScreenWidth,
    GetHeight: Core.GetScreenHeight,
}

const monitor_ctx = {
    GetCount: Core.GetMonitorCount,
    GetWidth: Core.GetMonitorWidth,
    GetHeight: Core.GetMonitorHeight,
    GetPhysicalWidth: Core.GetMonitorPhysicalWidth,
    GetPhysicalHeight: Core.GetMonitorPhysicalHeight,
    GetRefreshRate: Core.GetMonitorRefreshRate,
    GetMonitor: Core.GetCurrentMonitor,
}

const initContext = {
    Window: window_ctx,
    Screen: screen_ctx,
    Monitor: monitor_ctx,
}

const updateContext = {
    GetFrameTime: Core.GetFrameTime,
    Input: {
        IsKeyDown: Core.IsKeyDown,
        IsKeyUp: Core.IsKeyUp,
        IsKeyPressed: Core.IsKeyPressed,
    },
    Mouse: {
        GetPosition: Core.GetMousePosition,
        GetDelta: Core.GetMouseDelta,
        IsButtonDown: Core.IsMouseButtonDown,
        IsButtonUp: Core.IsMouseButtonUp,
        IsButtonPressed: Core.IsMouseButtonPressed,
    },
    Window: window_ctx,
    Screen: screen_ctx,
    Monitor: monitor_ctx,

}

const drawContext2D = {
    DrawFPS: (position) => Text.DrawFPS(position.x, position.y),
    DrawRectangle: (position, size, color) => Shapes.DrawRectangle(position.x, position.y, size.x, size.y, color),
    DrawCircle: (position, radius, color) => Shapes.DrawCircle(position.x, position.y, radius, color),
    DrawText: (text, position, size, color) => Text.DrawText(text, position.x, position.y, size, color)
}

const renderContext = {
    WithLayer2D: (cb) => {
        cb(drawContext2D);
    }
}

export class Application {

    constructor(height, width, title) {
        this.height = height;
        this.width = width;
        this.title = title;
        this.fps = 60;
    }

    Run(game) {

        Core.InitWindow(this.width, this.height, this.title);

        Core.SetTargetFPS(this.fps);

        if (game.OnStart) {
            game.OnStart(initContext);
        }

        while (!Core.WindowShouldClose()) {

            if (game.OnUpdate) {
                game.OnUpdate(Core.GetFrameTime(), updateContext);
            }

            Core.BeginDrawing();

            Core.ClearBackground(Color.WHITE);

            if (game.OnDraw) {
                game.OnDraw(renderContext);
            }

            Core.EndDrawing();
        }

        if (game.OnEnd) {
            game.OnEnd();
        }

        Core.CloseWindow();
    }

    SetConfigFlag(flag) {
        Core.SetConfigFlag(flag);
    }

    SetFPS(fps) {
        this.fps = fps;
    }

    SetExitKey(key) {
        Core.SetExitKey(key);
    }

}