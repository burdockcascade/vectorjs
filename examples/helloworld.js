import * as RL from "raylib";

const screenWidth = 800;
const screenHeight = 800;

RL.initWindow(screenWidth, screenHeight, "VectorJS");
RL.setTargetFPS(60);

while (!RL.windowShouldClose()) {
    RL.beginDrawing();
    RL.clearBackground(RL.WHITE);
    RL.endDrawing();
}

RL.closeWindow();