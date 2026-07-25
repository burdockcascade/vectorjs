import {Application, Rectangle, Vector2, Palette, Font} from "vectorjs";

const screenWidth = 600;
const screenHeight = 800;
const fpsPos = new Vector2(10, 10);
const origin = new Vector2(0, 0);

const app = new Application(screenWidth, screenHeight,"Window");
app.run({

    onInit() {
        this.customFont = new Font("C:\\workspace\\c\\vectorjs\\examples\\assets\\AnonymousPro-Regular.ttf");
    },

    onDraw(render) {
        render.withLayer2D((ctx) => {
            ctx.text.drawText(fpsPos, "this is a sample text", {
                font: this.customFont,
                color: Palette.RED,
                fontSize: 24.0,
                spacing: 2.0,
                rotation: 0.0,
                origin: origin
            })
        });
    }
});