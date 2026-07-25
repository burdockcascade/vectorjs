import { Application, Vector2, Palette } from "vectorjs";

// Window configuration
const screenWidth = 600;
const screenHeight = 600;
const center = new Vector2(screenWidth / 2, screenHeight / 2);
const fpsPos = new Vector2(10, 10);

// Clock dimensions
const clockRadius = 200;
const hourHandLength = 100;
const minuteHandLength = 140;
const secondHandLength = 160;

const app = new Application(screenWidth, screenHeight, "Analog Clock");

app.run({
    onDraw(render) {
        render.withLayer2D((ctx) => {

            // 1. Draw Clock Face Outer Ring & Center
            ctx.shapes.drawCircle(center, clockRadius, {
                color: Palette.WHITE
            });
            ctx.shapes.drawCircle(center, 8, {
                color: Palette.BLACK
            });

            // 2. Draw Hour Ticks (12 main markers)
            for (let i = 0; i < 12; i++) {
                const angle = (i * 30 - 90) * (Math.PI / 180);
                const innerRadius = clockRadius - 20;

                const tickStart = new Vector2(
                    center.x + innerRadius * Math.cos(angle),
                    center.y + innerRadius * Math.sin(angle)
                );
                const tickEnd = new Vector2(
                    center.x + clockRadius * Math.cos(angle),
                    center.y + clockRadius * Math.sin(angle)
                );

                ctx.shapes.drawLine(tickStart, tickEnd, {
                    color: Palette.GRAY
                });
            }

            // 3. Get Current Time
            const now = new Date();
            const hours = now.getHours() % 12;
            const minutes = now.getMinutes();
            const seconds = now.getSeconds();
            const milliseconds = now.getMilliseconds();

            // Smooth calculations for hand angles (in radians, -90 deg offset to start at 12 o'clock)
            const secondAngle = ((seconds + milliseconds / 1000) * 6 - 90) * (Math.PI / 180);
            const minuteAngle = ((minutes + seconds / 60) * 6 - 90) * (Math.PI / 180);
            const hourAngle = ((hours + minutes / 60) * 30 - 90) * (Math.PI / 180);

            // Calculate hand end positions
            const hourHandEnd = new Vector2(
                center.x + hourHandLength * Math.cos(hourAngle),
                center.y + hourHandLength * Math.sin(hourAngle)
            );
            const minuteHandEnd = new Vector2(
                center.x + minuteHandLength * Math.cos(minuteAngle),
                center.y + minuteHandLength * Math.sin(minuteAngle)
            );
            const secondHandEnd = new Vector2(
                center.x + secondHandLength * Math.cos(secondAngle),
                center.y + secondHandLength * Math.sin(secondAngle)
            );

            // 4. Draw Clock Hands
            // Hour Hand
            ctx.shapes.drawLine(center, hourHandEnd, {
                color: Palette.BLUE
            });

            // Minute Hand
            ctx.shapes.drawLine(center, minuteHandEnd, {
                color: Palette.GREEN
            });

            // Second Hand
            ctx.shapes.drawLine(center, secondHandEnd, {
                color: Palette.RED
            });
        });
    }
});