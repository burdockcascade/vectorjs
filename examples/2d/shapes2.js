import { Application, Vector2, Palette } from "vectorjs";

const screenWidth = 800;
const screenHeight = 600;
const center = new Vector2(screenWidth / 2, screenHeight / 2);
const fpsPos = new Vector2(10, 10);

const app = new Application(screenHeight, screenWidth, "Animated Shapes");

app.run({
    onDraw(render) {
        render.withLayer2D((ctx) => {
            ctx.drawFPS(fpsPos);

            // Time variable driving all animations
            const time = Date.now() * 0.003;

            // 1. Pulsing Center Circle
            const pulseRadius = 50 + Math.sin(time * 2) * 20;
            ctx.shapes.drawCircle(center, pulseRadius, {
                color: Palette.RED
            });

            // 2. Bouncing Ball (Vertical Sinusoidal Motion)
            const bounceY = center.y + Math.abs(Math.sin(time * 3)) * -200 + 100;
            const bouncePos = new Vector2(150, bounceY);
            ctx.shapes.drawCircle(bouncePos, 30, {
                color: Palette.ORANGE
            });

            // 3. Orbiting Satellite (Circular Path)
            const orbitRadius = 180;
            const orbitPos = new Vector2(
                center.x + Math.cos(time) * orbitRadius,
                center.y + Math.sin(time) * orbitRadius
            );

            // Connection line to center
            ctx.shapes.drawLine(center, orbitPos, {
                color: Palette.GRAY
            });

            ctx.shapes.drawCircle(orbitPos, 15, {
                color: Palette.BLUE
            });

            // 4. Rotating Triangle Vertices
            const triangleRadius = 80;
            const triangleCenter = new Vector2(650, 200);

            const p1 = new Vector2(
                triangleCenter.x + Math.cos(time) * triangleRadius,
                triangleCenter.y + Math.sin(time) * triangleRadius
            );
            const p2 = new Vector2(
                triangleCenter.x + Math.cos(time + (2 * Math.PI / 3)) * triangleRadius,
                triangleCenter.y + Math.sin(time + (2 * Math.PI / 3)) * triangleRadius
            );
            const p3 = new Vector2(
                triangleCenter.x + Math.cos(time + (4 * Math.PI / 3)) * triangleRadius,
                triangleCenter.y + Math.sin(time + (4 * Math.PI / 3)) * triangleRadius
            );

            ctx.shapes.drawTriangle(p1, p2, p3, {
                color: Palette.GREEN
            });

            // 5. Morphing Ellipse (Width & Height Oscillations)
            const ellipsePos = new Vector2(650, 450);
            const radiusX = 60 + Math.cos(time * 2) * 30;
            const radiusY = 60 + Math.sin(time * 2) * 30;

            ctx.shapes.drawEllipse(ellipsePos, radiusX, radiusY, {
                color: Palette.BROWN
            });
        });
    }
});