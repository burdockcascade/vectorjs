import { Core, Shapes, Text, Info } from "Raylib";

// 1. Setup
const screenWidth = 600;
const screenHeight = 600;
const centerX = screenWidth / 2;
const centerY = screenHeight / 2;
const clockRadius = 250;

Core.InitWindow(screenWidth, screenHeight, "Raylib JS - Analog Clock");
Core.SetTargetFPS(60);

// Colors (Using Object format as required)
const colors = {
    bg: { r: 30, g: 30, b: 30, a: 255 },
    face: { r: 240, g: 240, b: 240, a: 255 },
    hourHand: { r: 50, g: 50, b: 50, a: 255 },
    minuteHand: { r: 80, g: 80, b: 80, a: 255 },
    secondHand: { r: 230, g: 30, b: 30, a: 255 }, // Red
    ticks: { r: 100, g: 100, b: 100, a: 255 }
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
    const sec = now.getSeconds() + now.getMilliseconds() / 1000;
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