import { Application, Vector2, Palette, Keyboard } from "vectorjs";

const screenWidth = 800;
const screenHeight = 800;
const sunPos = new Vector2(screenWidth / 2, screenHeight / 2);
const fpsPos = new Vector2(10, 10);

// Define planets with varied speeds, orbital radii, and attached moons
const planets = [
    { name: "Mercury", radius: 70,  size: 6,  speed: 2.5, color: Palette.GRAY, moons: [] },
    { name: "Venus",   radius: 110, size: 10, speed: 1.8, color: Palette.ORANGE, moons: [] },
    {
        name: "Earth",
        radius: 170,
        size: 12,
        speed: 1.2,
        color: Palette.BLUE,
        moons: [
            { radius: 22, size: 3, speed: 4.0, color: Palette.LIGHT_GRAY }
        ]
    },
    {
        name: "Mars",
        radius: 240,
        size: 9,
        speed: 0.9,
        color: Palette.RED,
        moons: [
            { radius: 16, size: 2, speed: 5.0, color: Palette.LIGHT_GRAY },
            { radius: 22, size: 2, speed: 3.5, color: Palette.GRAY }
        ]
    },
    {
        name: "Jupiter",
        radius: 320,
        size: 22,
        speed: 0.5,
        color: Palette.BROWN,
        moons: [
            { radius: 30, size: 3, speed: 6.0, color: Palette.LIGHT_GRAY },
            { radius: 38, size: 4, speed: 4.2, color: Palette.WHITE },
            { radius: 46, size: 3, speed: 3.0, color: Palette.LIGHT_GRAY }
        ]
    }
];

const app = new Application(screenWidth, screenHeight, "Solar System Simulation");

// Simulation State
let isRunning = true;
let time = 0;

app.run({

    onUpdate(ctx) {
        // Toggle running state when Spacebar is pressed (fires once per press)
        if (ctx.isKeyPressed(Keyboard.KEY_SPACE)) {
            isRunning = !isRunning;
        }

        // Reset the animation
        if (ctx.isKeyPressed(Keyboard.KEY_R)) {
            time = 0;
        }

        // Advance simulation time only while running (assuming ~60 FPS)
        if (isRunning) {
            time += 1 / 60;
        }
    },

    onDraw(render) {
        render.withLayer2D((ctx) => {
            ctx.drawFPS(fpsPos);

            // 1. Draw the central Sun
            ctx.shapes.drawCircle(sunPos, 35, {
                color: Palette.YELLOW
            });

            // 2. Render each planet and its moons
            planets.forEach((planet) => {

                // Calculate planet position based on its distinct speed
                const angle = time * planet.speed;
                const planetPos = new Vector2(
                    sunPos.x + Math.cos(angle) * planet.radius,
                    sunPos.y + Math.sin(angle) * planet.radius
                );

                // Draw planet
                ctx.shapes.drawCircle(planetPos, planet.size, {
                    color: planet.color
                });

                // Draw moons orbiting around the planet
                planet.moons.forEach((moon) => {
                    const moonAngle = time * moon.speed;
                    const moonPos = new Vector2(
                        planetPos.x + Math.cos(moonAngle) * moon.radius,
                        planetPos.y + Math.sin(moonAngle) * moon.radius
                    );

                    ctx.shapes.drawCircle(moonPos, moon.size, {
                        color: moon.color
                    });
                });
            });
        });
    }
});