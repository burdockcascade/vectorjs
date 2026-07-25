import {Application, Rectangle, Vector2, Palette } from "vectorjs";

const screenWidth = 600;
const screenHeight = 800;
const fpsPos = new Vector2(10, 10);
const rect = new Rectangle(30, 30, 200, 45);
const point1 = new Vector2(400, 150);
const point2 = new Vector2(300, 350);
const point3 = new Vector2(500, 350);
const point4 = new Vector2(700, 500);

const app = new Application(screenWidth, screenHeight,"Window");
app.run({

    onDraw(render) {
        render.withLayer2D((ctx) => {
            ctx.drawFPS(fpsPos);

            ctx.shapes.drawRectangle(rect, {
                color: Palette.RED
            })

            ctx.shapes.drawCircle(point1, 60, {
                color: Palette.GREEN
            })

            ctx.shapes.drawTriangle(point1, point2, point3, {
                color: Palette.BLUE
            })

            ctx.shapes.drawEllipse(point4, 75.0, 50.0, {
                color: Palette.ORANGE
            })

            ctx.shapes.drawLine(point4, point1, {
                color: Palette.BROWN
            });

        });
    }
});