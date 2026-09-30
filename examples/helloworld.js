import * as RL from "raylib";

const screenWidth = 800;
const screenHeight = 800;

RL.initWindow(screenWidth, screenHeight, "VectorJS");
RL.setTargetFPS(60);

while (!RL.windowShouldClose()) {
    RL.beginDrawing();
    RL.clearBackground(RL.WHITE);
    RL.drawText("Hello, World!", 190, 200, 20, RL.MAROON);
    RL.drawRectangle(100, 100, 200, 200, RL.BLUE);
    RL.endDrawing();
}

RL.closeWindow();