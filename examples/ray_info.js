import { Core, Text, Shapes, Info } from "Raylib";

// 1. Setup
const screenWidth = 400;
const screenHeight = 300;

// Accessing the new Info object for window title
Core.InitWindow(screenWidth, screenHeight, "Raylib Info - v" + Info.Version);
Core.SetTargetFPS(60);

// Colors using your new hex string support
const bgColor = { r: 24, g: 24, b: 24, a: 255 };
const textColor = { r: 0, g: 170, b: 0, a: 255 }; // Raylib Green
const headerColor = { r: 255, g: 255, b: 255, a: 255 };
const subtextColor = { r: 136, g: 136, b: 136, a: 255 };

// 2. Main Loop
while (!Core.WindowShouldClose()) {
    Core.BeginDrawing();
    Core.ClearBackground(bgColor);

    // Header
    Text.DrawText("RAYLIB MODULE INFO", 20, 20, 20, headerColor);
    Shapes.DrawLine(20, 45, 150, 45, textColor);

    // Displaying version metadata
    Text.DrawText(`Version: ${Info.Version}`, 20, 70, 20, textColor);
    Text.DrawText(`Major: ${Info.VersionMajor}`, 20, 100, 20, textColor);
    Text.DrawText(`Minor: ${Info.VersionMinor}`, 20, 130, 20, textColor);
    Text.DrawText(`Patch: ${Info.VersionPatch}`, 20, 160, 20, textColor);
    
    // Website link
    Text.DrawText(Info.Website, 20, 210, 20, subtextColor);

    // Display FPS from the Text module
    Text.DrawFPS(20, 260);

    Core.EndDrawing();
}

// 3. Cleanup
Core.CloseWindow();