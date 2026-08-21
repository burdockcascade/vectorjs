import { Application, Vector2, Palette, Rectangle } from "vectorjs";

const app = new Application(800, 600, "2D Modern Analog Clock");

app.run({
    onDraw(render) {
        render.clearBackground(Palette.CHARBLACK);

        render.withScreenSpace((screen) => {
            const center = new Vector2(400, 300);
            const radius = 200;

            // Get live local time components
            const now = new Date();
            const hours = now.getHours() % 12;
            const minutes = now.getMinutes();
            const seconds = now.getSeconds();
            const milliseconds = now.getMilliseconds();

            // Smooth continuous angles for hands (in radians)
            const secWithMs = seconds + milliseconds / 1000;
            const secondAngle = (secWithMs / 60) * Math.PI * 2 - Math.PI / 2;
            const minuteAngle = ((minutes + secWithMs / 60) / 60) * Math.PI * 2 - Math.PI / 2;
            const hourAngle = ((hours + minutes / 60) / 12) * Math.PI * 2 - Math.PI / 2;

            // 1. Clock Outer Frame & Face Background
            screen.shapes.drawCircle(center, radius + 15, { color: Palette.SLATE });
            screen.shapes.drawCircle(center, radius + 10, { color: Palette.OFFWHITE });
            screen.shapes.drawCircle(center, radius, { color: Palette.CHARBLACK });

            // 2. Hour / Minute Dial Ticks
            for (let i = 0; i < 60; i++) {
                const angle = (i / 60) * Math.PI * 2 - Math.PI / 2;
                const isHourTick = i % 5 === 0;

                const innerRadius = isHourTick ? radius - 20 : radius - 10;
                const tickColor = isHourTick ? Palette.WHITE : Palette.GRAY;

                const start = new Vector2(
                    center.x + Math.cos(angle) * innerRadius,
                    center.y + Math.sin(angle) * innerRadius
                );
                const end = new Vector2(
                    center.x + Math.cos(angle) * (radius - 5),
                    center.y + Math.sin(angle) * (radius - 5)
                );

                screen.shapes.drawLine(start, end, { color: tickColor });
            }

            // Helper function to draw rotated clock hand rectangles
            const drawHand = (angle, length, width, color) => {
                const rect = new Rectangle(center.x, center.y, length, width);
                screen.shapes.drawRectangle(rect, {
                    color: color,
                    rotation: (angle * 180) / Math.PI,
                    origin: new Vector2(0, width / 2)
                });
            };

            // 3. Clock Hands
            // Hour Hand
            drawHand(hourAngle, radius * 0.5, 8, Palette.WHITE);

            // Minute Hand
            drawHand(minuteAngle, radius * 0.75, 5, Palette.CYAN);

            // Second Hand (Sweeping Neon Accent with Tail Offset)
            const secTailLength = 30;
            const secTailPos = new Vector2(
                center.x - Math.cos(secondAngle) * secTailLength,
                center.y - Math.sin(secondAngle) * secTailLength
            );
            const secTipPos = new Vector2(
                center.x + Math.cos(secondAngle) * (radius - 15),
                center.y + Math.sin(secondAngle) * (radius - 15)
            );
            screen.shapes.drawLine(secTailPos, secTipPos, { color: Palette.HOTPINK });

            // 4. Center Pin Cap
            screen.shapes.drawCircle(center, 10, { color: Palette.HOTPINK });
            screen.shapes.drawCircle(center, 4, { color: Palette.WHITE });
        });
    }
});