import { Application, Vector2, Color, Palette, Rectangle } from "vectorjs";

let particles = [];
let rings = [];

const app = new Application(800, 600, "2D Orbiting Neon Swarm");

app.run({
    onInit() {
        // Initialize interactive orbiting particles with unique radius, speed, and color
        for (let i = 0; i < 180; i++) {
            particles.push({
                angle: Math.random() * Math.PI * 2,
                orbitRadius: 40 + Math.random() * 260,
                speed: (Math.random() - 0.5) * 0.04,
                size: 2 + Math.random() * 6,
                baseColor: i % 2 === 0 ? Palette.ELECTRIC_BLUE : Palette.HOTPINK,
                offsetY: (Math.random() - 0.5) * 50
            });
        }

        // Initialize surrounding background shapes (growing rings)
        for (let i = 0; i < 5; i++) {
            rings.push({
                radius: i * 60,
                maxRadius: 300,
                speed: 30 + Math.random() * 20
            });
        }
    },

    onUpdate(ctx) {
        const dt = ctx.getDeltaTime();

        this.time = ctx.getTime();
        this.mousePos = ctx.getMousePosition();

        // Rotate orbiting shapes
        for (let p of particles) {
            p.angle += p.speed;
        }

        // Expand animated geometric background rings
        for (let r of rings) {
            r.radius += r.speed * dt;
            if (r.radius > r.maxRadius) {
                r.radius = 0;
            }
        }
    },

    onDraw(render) {
        // Dark background layer
        render.clearBackground(Palette.CHARBLACK);

        render.withScreenSpace((screen) => {

            // Center position reacting slightly to mouse movement
            const centerX = 400;
            const centerY = 300;

            // 1. Draw animated expanding geometric rings
            for (let r of rings) {
                const alpha = 1.0 - (r.radius / r.maxRadius);
                const ringColor = Palette.PURPLE.alpha(alpha * 0.4);

                screen.shapes.drawCircle(
                    new Vector2(centerX, centerY),
                    r.radius,
                    { color: ringColor }
                );
            }

            // 2. Draw rotating gradient center star/diamond structure
            const diamondSize = 35 + Math.sin(this.time * 3) * 10;
            const coreColor = Palette.CYAN.lerp(Palette.MAGENTA, (Math.sin(this.time * 2) + 1) / 2);

            screen.shapes.drawRectangle(
                new Rectangle(centerX, centerY, diamondSize, diamondSize),
                {
                    color: coreColor,
                    rotation: this.time * 45,
                    origin: new Vector2(diamondSize / 2, diamondSize / 2)
                }
            );

            // 3. Draw orbiting particle cloud with dynamic positions and colors
            for (let p of particles) {
                const x = centerX + Math.cos(p.angle) * p.orbitRadius;
                const y = centerY + Math.sin(p.angle) * p.orbitRadius + Math.sin(this.time * 2 + p.angle) * 15;

                // Shift dynamic color tone over time
                const lerpFactor = (Math.sin(this.time + p.angle) + 1) / 2;
                const particleColor = p.baseColor.clone().lerp(Palette.YELLOW, lerpFactor);

                // Main particle shape
                screen.shapes.drawCircle(
                    new Vector2(x, y),
                    p.size,
                    { color: particleColor }
                );

                // Outer ambient glow circle
                screen.shapes.drawCircle(
                    new Vector2(x, y),
                    p.size * 2,
                    { color: particleColor.clone().alpha(0.25) }
                );
            }

            // 4. Draw vibrant connecting geometry line pulses
            for (let i = 0; i < 12; i++) {
                const p1 = particles[i * 10];
                const p2 = particles[(i * 10 + 15) % particles.length];

                const x1 = centerX + Math.cos(p1.angle) * p1.orbitRadius;
                const y1 = centerY + Math.sin(p1.angle) * p1.orbitRadius;
                const x2 = centerX + Math.cos(p2.angle) * p2.orbitRadius;
                const y2 = centerY + Math.sin(p2.angle) * p2.orbitRadius;

                screen.shapes.drawLine(
                    new Vector2(x1, y1),
                    new Vector2(x2, y2),
                    { color: Palette.LIME.alpha(0.2) }
                );
            }
        });
    }
});