import {Application, Vector2, Palette, Font} from "vectorjs";

const screenWidth = 800;
const screenHeight = 600;
const fpsPos = new Vector2(screenWidth/2, screenHeight/2);
const origin = new Vector2(1, 1)

const app = new Application(screenWidth, screenHeight,"Window");
app.run({

    onInit() {
        this.customFont = new Font("examples/assets/AnonymousPro-Regular.ttf");
    },

    onDraw(render) {
        render.withLayer2D((ctx) => {
            ctx.text.drawText(fpsPos, "this is a sample text", {
                font: this.customFont,
                color: Palette.RED,
                fontSize: 24.0,
                spacing: 2.0,
                rotation: 25.0,
                origin: origin
            })
        });
    }
});