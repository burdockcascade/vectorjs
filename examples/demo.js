import { Application, Vector2 } from "vectorjs";

const screenWidth = 800;
const screenHeight = 450;
const fpsPos = new Vector2(10, 10);

class MyApplication {

    OnDraw(render) {
        render.WithLayer2D((ctx) => {
            ctx.DrawFPS(fpsPos);
        });
    }
}

const app = new Application(screenHeight, screenWidth, "Monitor & Window Info");
app.Run(new MyApplication());