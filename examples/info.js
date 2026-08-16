import {Application, Vector2, Palette, Info} from "vectorjs";

const screenWidth = 800;
const screenHeight = 800;
const line1 = new Vector2(10, 10);
const line2 = new Vector2(10, 30);

const app = new Application(screenWidth, screenHeight,"Info");
app.run({

    onDraw(render) {

        render.clearBackground(Palette.WHITE);

        render.withScreenSpace((ctx) => {
            ctx.text.drawText(line1, `RAYLIB VERSION: ${Info.RAYLIB_VERSION}`)
            ctx.text.drawText(line2, `QUICKJS VERSION: ${Info.QUICKJS_VERSION}`)
        });
    }
});