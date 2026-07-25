import {Application, Vector2, Palette, Font, Info} from "vectorjs";

const screenWidth = 600;
const screenHeight = 800;
const line1 = new Vector2(10, 10);
const line2 = new Vector2(10, 30);

const app = new Application(screenWidth, screenHeight,"Info");
app.run({

    onInit() {
        this.customFont = new Font("C:\\workspace\\c\\vectorjs\\examples\\assets\\AnonymousPro-Regular.ttf");
    },

    onDraw(render) {
        render.withLayer2D((ctx) => {
            ctx.text.drawText(line1, `RAYLIB VERSION: ${Info.RAYLIB_VERSION}`)
            ctx.text.drawText(line2, `QUICKJS VERSION: ${Info.QUICKJS_VERSION}`)
        });
    }
});