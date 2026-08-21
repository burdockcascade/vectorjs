import { Application, Palette, Vector2, Keyboard, Camera2D, Color } from "vectorjs";

const enable_orbit_paths = true;
const enable_starfield = true;

const screenWidth = 1200;
const screenHeight = 1200;
const sunPos = new Vector2(screenWidth / 2, screenHeight / 2);
const fpsPos = new Vector2(10, 10);

const camera = new Camera2D(new Vector2(0, 0), new Vector2(0, 0), 0, 1.0);

const starCount = 200;
const stars = Array.from({ length: starCount }, () => ({
    pos: new Vector2(Math.random() * screenWidth, Math.random() * screenHeight),
    size: Math.random() < 0.8 ? 1 : 2,
    color: Math.random() < 0.2 ? Palette.LIGHTGRAY : Palette.WHITE,
    phase: Math.random() * Math.PI * 2
}));

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
            { radius: 22, size: 3, speed: 4.0, color: Palette.LIGHTGRAY }
        ]
    },
    {
        name: "Mars",
        radius: 240,
        size: 9,
        speed: 0.9,
        color: Palette.RED,
        moons: [
            { radius: 16, size: 2, speed: 5.0, color: Palette.LIGHTGRAY },
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
            { radius: 30, size: 3, speed: 6.0, color: Palette.LIGHTGRAY },
            { radius: 38, size: 4, speed: 4.2, color: Palette.WHITE },
            { radius: 46, size: 3, speed: 3.0, color: Palette.LIGHTGRAY },
            { radius: 54, size: 3, speed: 2.1, color: Palette.GRAY }
        ]
    },
    {
        name: "Saturn",
        radius: 410,
        size: 18,
        speed: 0.35,
        color: Palette.GOLD || Palette.YELLOW,
        moons: [
            { radius: 28, size: 4, speed: 3.8, color: Palette.LIGHTGRAY },
            { radius: 36, size: 3, speed: 2.7, color: Palette.WHITE },
            { radius: 44, size: 2, speed: 2.0, color: Palette.GRAY }
        ]
    },
    {
        name: "Uranus",
        radius: 490,
        size: 15,
        speed: 0.25,
        color: Palette.SKYBLUE,
        moons: [
            { radius: 24, size: 3, speed: 3.2, color: Palette.LIGHTGRAY },
            { radius: 30, size: 2, speed: 2.3, color: Palette.WHITE }
        ]
    },
    {
        name: "Neptune",
        radius: 550,
        size: 14,
        speed: 0.18,
        color: Palette.DARKBLUE || Palette.BLUE,
        moons: [
            { radius: 22, size: 3, speed: -2.5, color: Palette.LIGHTGRAY }
        ]
    }
];

const app = new Application(screenWidth, screenHeight, "Solar System Simulation");

let isRunning = true;
let time = 0;

app.run({
    onUpdate(ctx) {
        if (ctx.isKeyPressed(Keyboard.KEY_SPACE)) {
            isRunning = !isRunning;
        }

        if (ctx.isKeyPressed(Keyboard.KEY_R)) {
            time = 0;
            camera.target = new Vector2(0, 0);
        }

        if (ctx.isKeyDown(Keyboard.KEY_UP)) {
            camera.moveY(-10);
        }

        if (ctx.isKeyDown(Keyboard.KEY_DOWN)) {
            camera.moveY(10);
        }

        if (isRunning) {
            time += ctx.getDeltaTime();
        }
    },

    onDraw(render) {
        render.clearBackground(Palette.BLACK);

        render.withScreenSpace((ctx) => {
            ctx.drawFPS(fpsPos);
        });

        render.withViewport2D(camera, (ctx) => {
            if (enable_starfield) {
                stars.forEach((star) => {
                    const twinkleAmount = 0.4 + 0.6 * Math.sin(time * 3 + star.phase);
                    const starColor = star.color.fade(twinkleAmount);

                    ctx.shapes.drawCircle(star.pos, star.size, {
                        color: starColor
                    });
                });
            }

            if (enable_orbit_paths) {
                planets.forEach((planet) => {
                    const totalDots = 64;
                    for (let i = 0; i < totalDots; i++) {
                        const dotAngle = (i / totalDots) * Math.PI * 2;

                        const dotPos = new Vector2(
                            sunPos.x + Math.cos(dotAngle) * planet.radius,
                            sunPos.y + Math.sin(dotAngle) * planet.radius
                        );

                        ctx.shapes.drawCircle(dotPos, 1.5, {
                            color: planet.color
                        });
                    }
                });
            }

            const pulse = (Math.sin(time * 2) + 1) * 0.5;
            const sunGlow = Palette.YELLOW.brightness(0.3 * pulse);
            const sunColor = Palette.YELLOW.lerp(Palette.GOLD || Palette.ORANGE, pulse * 0.2);

            ctx.shapes.drawCircle(sunPos, 40, { color: sunGlow.fade(0.4) });
            ctx.shapes.drawCircle(sunPos, 35, { color: sunColor });

            planets.forEach((planet) => {
                const angle = time * planet.speed;

                // FIX: Compute X and Y natively in JS
                const planetPos = new Vector2(
                    sunPos.x + Math.cos(angle) * planet.radius,
                    sunPos.y + Math.sin(angle) * planet.radius
                );

                ctx.shapes.drawCircle(planetPos, planet.size, {
                    color: planet.color
                });

                planet.moons.forEach((moon) => {
                    const moonAngle = time * moon.speed;

                    // FIX: Compute X and Y natively in JS
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