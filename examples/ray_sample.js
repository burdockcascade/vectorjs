import { Core, Texture, Text } from "Raylib";

// 1. Setup
const screenWidth = 800;
const screenHeight = 600;

// Accessing the new Info object for window title
Core.InitWindow(screenWidth, screenHeight, "Raylib Sample Image");
Core.SetTargetFPS(60);

// Colors using your new hex string support
const bgColor = { r: 24, g: 24, b: 24, a: 255 };
const my_sample_texture = Texture.LoadTexture("C:\\workspace\\c\\vectorjs\\examples\\assets\\sample.png");

// 2. Main Loop
while (!Core.WindowShouldClose()) {
    Core.BeginDrawing();
    Core.ClearBackground(bgColor);

    Texture.DrawTexture(my_sample_texture, 0, 0, { r: 255, g: 255, b: 255, a: 255 })

    Text.DrawText("Look at this masterpiece!", 10, 10, 20, { r: 255, g: 255, b: 255, a: 255 });

    Core.EndDrawing();
}

// Unload Texture
Texture.UnloadTexture(my_sample_texture);

// 3. Cleanup
Core.CloseWindow();