import { Core, Input, Texture, Text, Audio, Palette, KeyboardKey } from "Raylib";

// 1. Setup
const screenWidth = 800;
const screenHeight = 600;

// Accessing the new Info object for window title
Core.InitWindow(screenWidth, screenHeight, "Raylib Sample Image");
Core.SetTargetFPS(60);

// Colors using your new hex string support
const bgColor = { r: 24, g: 24, b: 24, a: 255 };
const my_sample_texture = Texture.LoadTexture("C:\\workspace\\c\\vectorjs\\examples\\assets\\sample.png");

Audio.InitAudioDevice();
const sound = Audio.LoadSound("C:\\workspace\\c\\vectorjs\\examples\\assets\\sound.wav");

// 2. Main Loop
while (!Core.WindowShouldClose()) {
    Core.BeginDrawing();
    Core.ClearBackground(bgColor);

    Texture.DrawTexture(my_sample_texture, 0, 0, Palette.WHITE);

    Text.DrawText("Press 's' to play a sound!", 10, 10, 20, Palette.RED);

    if (Input.IsKeyPressed(KeyboardKey.KEY_S)) {
        Audio.PlaySound(sound);
    }

    Core.EndDrawing();
}

// Unload Texture
Texture.UnloadTexture(my_sample_texture);

// 3. Cleanup
Core.CloseWindow();