import * as RL from "raylib";

const screenWidth = 800;
const screenHeight = 800;

RL.initWindow(screenWidth, screenHeight, "VectorJS");

while (!RL.windowShouldClose()) {
    RL.beginDrawing();
    RL.drawText("Hello World", 10, 10, 20, RL.BLUE);
    RL.endDrawing();
}

RL.closeWindow();