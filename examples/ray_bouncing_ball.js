import { Core, Shapes, Text, Palette } from "Raylib";

// 1. Initialization
const windowWidth = 800;
const windowHeight = 450;
const circleRadius = 30;

Core.InitWindow(windowWidth, windowHeight, "Raylib JS - Bouncing Circle");
Core.SetTargetFPS(60);

// 2. State Variables
let posX = windowWidth / 2;
let posY = windowHeight / 2;

let velX = 4;
let velY = 4;

// Colors (Assuming your wrapper handles objects as {r, g, b, a})
const ballColor = Palette.RED;
const textColor = Palette.LIGHTGRAY;
const bgColor = Palette.DARKBLUE;

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