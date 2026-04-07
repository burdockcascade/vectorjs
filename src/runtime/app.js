const window_ctx = {
    IsReady: _rl.IsWindowReady,
    IsMinimized: _rl.IsWindowMinimized,
    IsResized: _rl.IsWindowResized,
    IsHidden: _rl.IsWindowHidden,
    IsFocused: _rl.IsWindowFocused,
    IsFullscreen: _rl.IsFullscreen,
    ToggleFullscreen: _rl.ToggleFullscreen,
    SetTitle: _rl.SetWindowTitle,
    SetTargetFPS: _rl.SetTargetFPS,
}

const screen_ctx = {
    GetWidth: _rl.GetScreenWidth,
    GetHeight: _rl.GetScreenHeight,
}

const monitor_ctx = {
    GetCount: _rl.GetMonitorCount,
    GetWidth: _rl.GetMonitorWidth,
    GetHeight: _rl.GetMonitorHeight,
    GetPhysicalWidth: _rl.GetMonitorPhysicalWidth,
    GetPhysicalHeight: _rl.GetMonitorPhysicalHeight,
    GetRefreshRate: _rl.GetMonitorRefreshRate,
    GetMonitor: _rl.GetCurrentMonitor,
}

const initContext = {
    Window: window_ctx,
    Screen: screen_ctx,
    Monitor: monitor_ctx,
}

const updateContext = {
    GetFrameTime: _rl.GetFrameTime,
    Input: {
        IsKeyDown: _rl.IsKeyDown,
        IsKeyUp: _rl.IsKeyUp,
        IsKeyPressed: _rl.IsKeyPressed,
    },
    Mouse: {
        GetPosition: _rl.GetMousePosition,
        GetDelta: _rl.GetMouseDelta,
        IsButtonDown: _rl.IsMouseButtonDown,
        IsButtonUp: _rl.IsMouseButtonUp,
        IsButtonPressed: _rl.IsMouseButtonPressed,
    },
    Window: window_ctx,
    Screen: screen_ctx,
    Monitor: monitor_ctx,

}

const drawContext2D = {
    DrawFPS: (position) => _rl.DrawFPS(position.x, position.y),
    DrawRectangle: (position, size, color) => _rl.DrawRectangle(position.x, position.y, size.x, size.y, color),
    DrawCircle: (position, radius, color) => _rl.DrawCircle(position.x, position.y, radius, color),
    DrawText: (text, position, size, color) => _rl.DrawText(text, position.x, position.y, size, color)
}

const renderContext = {
    SetBackgroundColor: _rl.SetBackgroundColor,
    WithLayer2D: (cb) => {
        cb(drawContext2D);
    }
}

export class App {

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

}