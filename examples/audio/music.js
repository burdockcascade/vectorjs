import {Application, Vector2, Music } from "vectorjs";

const screenWidth = 400;
const screenHeight = 400;
const fpsPos = new Vector2(10, 10);
let music;

const app = new Application(screenWidth, screenHeight,"Music Player");
app.run({

    onInit() {
        music = new Music("C:/workspace/c/vectorjs/examples/assets/country.mp3");
        music.play();
        music.setVolume(0.5)
    },

    onUpdate(ctx) {
        if (music) {
            music.update();
        }
    },

    onDraw(render) {
        render.clearBackground();
        render.withScreenSpace((ctx) => {
            ctx.drawFPS(fpsPos);
        });
    }
});