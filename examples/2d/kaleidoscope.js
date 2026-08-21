import { Application, Vector2, Color, Palette, Triangle } from "vectorjs";

let petals = [];
let t = 0;

const app = new Application(800, 600, "2D Chromatic Kinetic Kaleidoscope");

app.run({
    onInit() {
        // Instantiate overlapping geometric flower petals arranged in symmetrical rings
        const totalRings = 4;
        const petalsPerRing = 12;

        for (let r = 1; r <= totalRings; r++) {
            for (let i = 0; i < petalsPerRing; i++) {
                const baseAngle = (i / petalsPerRing) * Math.PI * 2;
                petals.push({
                    ring: r,
                    index: i,
                    angle: baseAngle,
                    distance: r * 55,
                    size: 20 + r * 12,
                    primaryColor: r % 2 === 0 ? Palette.FUCHSIA : Palette.CYAN,
                    secondaryColor: r % 2 === 0 ? Palette.AMBER : Palette.ELECTRIC_BLUE,
                    speedMultiplier: (r % 2 === 0 ? 1 : -1) * (0.8 / r)
                });
            }
        }
    },

    onUpdate(ctx) {
        const dt = ctx.getDeltaTime();
        t += dt;

        // Dynamically rotate and pulsate individual petals based on ring index
        for (let p of petals) {
            p.angle += p.speedMultiplier * dt;
            p.currentDistance = p.distance + Math.sin(t * 2 + p.ring) * 20;
        }
    },

    onDraw(render) {
        // Dark, rich background
        render.clearBackground(Palette.MIDNIGHTBLUE);

        render.withScreenSpace((screen) => {
            const center = new Vector2(400, 300);

            // 1. Draw glowing background constellation lines running toward center
            for (let i = 0; i < 24; i++) {
                const rayAngle = (i / 24) * Math.PI * 2 + t * 0.2;
                const endPos = new Vector2(
                    center.x + Math.cos(rayAngle) * 380,
                    center.y + Math.sin(rayAngle) * 380
                );

                const rayColor = Palette.PURPLE.alpha(0.15 + Math.sin(t * 3 + i) * 0.1);
                screen.shapes.drawLine(center, endPos, { color: rayColor });
            }

            // 2. Draw animated geometric kaleidoscope petals (Triangles & Circles)
            for (let p of petals) {
                const posX = center.x + Math.cos(p.angle) * p.currentDistance;
                const posY = center.y + Math.sin(p.angle) * p.currentDistance;
                const pos = new Vector2(posX, posY);

                // Morph color smoothly across HSV space using lerp and contrast
                const colorPhase = (Math.sin(t * 1.5 + p.angle * 2) + 1) / 2;
                const currentColor = p.primaryColor.clone().lerp(p.secondaryColor, colorPhase).brightness(0.2);

                // Calculate rotated triangle vertices pointing outward from center
                const perpAngle = p.angle + Math.PI / 2;
                const p1 = new Vector2(
                    posX + Math.cos(p.angle) * p.size,
                    posY + Math.sin(p.angle) * p.size
                );
                const p2 = new Vector2(
                    posX + Math.cos(perpAngle) * (p.size * 0.4),
                    posY + Math.sin(perpAngle) * (p.size * 0.4)
                );
                const p3 = new Vector2(
                    posX - Math.cos(perpAngle) * (p.size * 0.4),
                    posY - Math.sin(perpAngle) * (p.size * 0.4)
                );

                // Draw solid triangle body
                screen.shapes.drawTriangle(p1, p2, p3, { color: currentColor });

                // Draw glowing tip highlights
                screen.shapes.drawCircle(p1, p.size * 0.25, {
                    color: Palette.WHITE.lerp(currentColor, 0.3)
                });
            }

            // 3. Draw central pulsing geometric core
            const pulseRadius = 25 + Math.sin(t * 4) * 8;
            const coreColor = Palette.GOLD.lerp(Palette.HOTPINK, (Math.cos(t * 3) + 1) / 2);

            // Layered translucent core circles creating a neon lens flare effect
            screen.shapes.drawCircle(center, pulseRadius * 2.5, { color: coreColor.clone().alpha(0.15) });
            screen.shapes.drawCircle(center, pulseRadius * 1.5, { color: coreColor.clone().alpha(0.35) });
            screen.shapes.drawCircle(center, pulseRadius, { color: Palette.WHITE });
        });
    }
});