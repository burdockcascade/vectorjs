import { Application, Vector2, Palette } from "vectorjs";

const screenWidth = 800;
const screenHeight = 800;
const sunPos = new Vector2(screenWidth / 2, screenHeight / 2);
const fpsPos = new Vector2(10, 10);

// Define planets with varied speeds and orbital radii
const planets = [
    { name: "Mercury", radius: 70,  size: 6,  speed: 2.5, color: Palette.GRAY },
    { name: "Venus",   radius: 110, size: 10, speed: 1.8, color: Palette.ORANGE },
    { name: "Earth",   radius: 170, size: 12, speed: 1.2, color: Palette.BLUE },
    { name: "Mars",    radius: 240, size: 9,  speed: 0.9, color: Palette.RED },
    { name: "Jupiter", radius: 320, size: 22, speed: 0.5, color: Palette.BROWN }
];

const app = new Application(screenHeight, screenWidth, "Solar System Simulation");

app.run({
    onDraw(render) {
        render.withLayer2D((ctx) => {
            ctx.drawFPS(fpsPos);

            const time = Date.now() * 0.001;

            // 1. Draw the central Sun
            ctx.shapes.drawCircle(sunPos, 35, {
                color: Palette.YELLOW
            });

            // 2. Render each planet along its orbital path
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
            });
        });
    }
});