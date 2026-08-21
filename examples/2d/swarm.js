import { Application, Vector2, Color, Palette, Rectangle, Circle } from "vectorjs";

let particles = [];
let rings = [];

const NUM_PARTICLES = 180;

const width = 900;
const height = 700;

const centerX = width/2;
const centerY = height/2;


const app = new Application(width, height, "2D Orbiting Neon Swarm");

app.run({
    onInit() {
        // Initialize interactive orbiting particles with unique radius, speed, and color
        for (let i = 0; i < NUM_PARTICLES; i++) {
            particles.push({
                angle: Math.random() * Math.PI * 2,
                orbitRadius: 40 + Math.random() * 260,
                speed: (Math.random() - 0.5) * 0.04,
                size: 2 + Math.random() * 6,
                baseColor: i % 2 === 0 ? Palette.ELECTRIC_BLUE : Palette.HOTPINK,
                offsetY: (Math.random() - 0.5) * 50,
                isColliding: false,
                x: 0,
                y: 0
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

        // Rotate orbiting shapes and update current positions/circles
        for (let p of particles) {
            p.angle += p.speed;
            p.x = centerX + Math.cos(p.angle) * p.orbitRadius;
            p.y = centerY + Math.sin(p.angle) * p.orbitRadius + Math.sin(this.time * 2 + p.angle) * 15;
            p.circle = new Circle(p.x, p.y, p.size);
            p.isColliding = false;
        }

        // Check circle collision overlaps
        for (let i = 0; i < particles.length; i++) {
            for (let j = i + 1; j < particles.length; j++) {
                if (particles[i].circle.overlaps(particles[j].circle)) {
                    particles[i].isColliding = true;
                    particles[j].isColliding = true;
                }
            }
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
                let particleColor;
                if (p.isColliding) {
                    particleColor = Palette.COPPER;
                } else {
                    const lerpFactor = (Math.sin(this.time + p.angle) + 1) / 2;
                    particleColor = p.baseColor.clone().lerp(Palette.YELLOW, lerpFactor);
                }

                // Main particle shape
                screen.shapes.drawCircle(
                    new Vector2(p.x, p.y),
                    p.size,
                    { color: particleColor }
                );

                // Outer ambient glow circle
                screen.shapes.drawCircle(
                    new Vector2(p.x, p.y),
                    p.size * 2,
                    { color: particleColor.clone().alpha(0.25) }
                );
            }

            // 4. Draw vibrant connecting geometry line pulses
            if (particles.length > 0) {
                const lineCount = Math.min(12, particles.length);
                const step = Math.floor(particles.length / lineCount) || 1;
                const offset = Math.max(1, Math.floor(particles.length * 0.15));

                for (let i = 0; i < lineCount; i++) {
                    const idx1 = (i * step) % particles.length;
                    const idx2 = (idx1 + offset) % particles.length;

                    const p1 = particles[idx1];
                    const p2 = particles[idx2];

                    screen.shapes.drawLine(
                        new Vector2(p1.x, p1.y),
                        new Vector2(p2.x, p2.y),
                        { color: Palette.LIME.alpha(0.2) }
                    );
                }
            }
        });
    }
});