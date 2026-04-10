import { Core, Text } from "Raylib";

Core.InitWindow(800, 600, "Hello Raylib Module!");

while (!Core.WindowShouldClose()) {
    Core.BeginDrawing();
    Core.ClearBackground({ r: 255, g: 255, b: 255, a: 255 });
    Text.DrawText("Hello Raylib Module!", 190, 200, 20, { r: 255, g: 0, b: 0, a: 255 });
    Core.EndDrawing();
}
Core.CloseWindow();