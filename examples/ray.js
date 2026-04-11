import { Core, Shapes, Text } from "Raylib";

// 1. Initialization
const screenWidth = 800;
const screenHeight = 450;
const circleRadius = 30;

Core.InitWindow(screenWidth, screenHeight, "Raylib JS - Bouncing Circle");
Core.SetTargetFPS(60);

// 2. State Variables
let posX = screenWidth / 2;
let posY = screenHeight / 2;

let velX = 4;
let velY = 4;

// Colors (Assuming your wrapper handles objects as {r, g, b, a})
const ballColor = { r: 255, g: 0, b: 0, a: 255 }; // Red
const textColor = { r: 200, g: 200, b: 200, a: 255 }; // Light Gray
const bgColor = { r: 18, g: 18, b: 18, a: 255 }; // Dark Background

// 3. Main Game Loop
while (!Core.WindowShouldClose()) {

    // --- Update Logic ---
    posX += velX;
    posY += velY;

    // Bounce off Left/Right
    if (posX + circleRadius >= Core.GetScreenWidth() || posX - circleRadius <= 0) {
        velX *= -1;
    }

    // Bounce off Top/Bottom
    if (posY + circleRadius >= Core.GetScreenHeight() || posY - circleRadius <= 0) {
        velY *= -1;
    }

    // --- Drawing Logic ---
    Core.BeginDrawing();
    Core.ClearBackground(bgColor);

    // Draw the "DVD" Circle
    Shapes.DrawCircle(posX, posY, circleRadius, ballColor);

    // Draw UI Text
    Text.DrawText("BOUNCING DVD CIRCLE", 10, 10, 20, textColor);
    Text.DrawFPS(10, 40);

    Core.EndDrawing();
}

// 4. Cleanup
Core.CloseWindow();