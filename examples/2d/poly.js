import { Application, Vector2, Palette, Polygon } from "vectorjs";

let time = 0;

// Base shape vertex definitions (normalized unit offsets)
const SHAPES = {
    // 1. Star (10 points)
    STAR: Array.from({ length: 10 }, (_, i) => {
        const r = i % 2 === 0 ? 120 : 50;
        const a = (i / 10) * Math.PI * 2 - Math.PI / 2;
        return new Vector2(Math.cos(a) * r, Math.sin(a) * r);
    }),

    // 2. Regular Decagon
    DECAGON: Array.from({ length: 10 }, (_, i) => {
        const a = (i / 10) * Math.PI * 2 - Math.PI / 2;
        return new Vector2(Math.cos(a) * 110, Math.sin(a) * 110);
    }),

    // 3. Flower / Gear Wave
    FLOWER: Array.from({ length: 10 }, (_, i) => {
        const a = (i / 10) * Math.PI * 2 - Math.PI / 2;
        const r = 90 + Math.sin(i * Math.PI) * 40;
        return new Vector2(Math.cos(a) * r, Math.sin(a) * r);
    }),

    // 4. Hourglass
    HOURGLASS: [
        new Vector2(-100, -100), new Vector2(0, -90), new Vector2(100, -100),
        new Vector2(20, 0), new Vector2(100, 100), new Vector2(0, 90),
        new Vector2(-100, 100), new Vector2(-20, 0), new Vector2(-60, -50),
        new Vector2(60, -50)
    ]
};

const shapeKeys = Object.keys(SHAPES);
const CENTER = new Vector2(400, 300);

const app = new Application(800, 600, "2D Morphing Polygon Matrix");

app.run({
    onUpdate(ctx) {
        time += ctx.getDeltaTime();
    },

    onDraw(render) {
        render.clearBackground(Palette.CHARBLACK);

        render.withScreenSpace((screen) => {
            // Determine current and target shapes for smooth interpolation
            const cycleDuration = 3.0;
            const totalShapes = shapeKeys.length;
            const progress = (time % (cycleDuration * totalShapes)) / cycleDuration;

            const currentIndex = Math.floor(progress);
            const nextIndex = (currentIndex + 1) % totalShapes;

            // Smooth step easing factor between 0 and 1
            const rawFactor = progress - currentIndex;
            const easeFactor = rawFactor * rawFactor * (3 - 2 * rawFactor);

            const currentVertices = SHAPES[shapeKeys[currentIndex]];
            const nextVertices = SHAPES[shapeKeys[nextIndex]];

            // Interpolate main morphing polygon vertices
            const morphedPoints = currentVertices.map((v1, i) => {
                const v2 = nextVertices[i];
                return new Vector2(
                    CENTER.x + v1.x + (v2.x - v1.x) * easeFactor,
                    CENTER.y + v1.y + (v2.y - v1.y) * easeFactor
                );
            });

            // Color morphing matching the shape transition
            const themeColor = Palette.TURQUOISE
                .withLerp(Palette.HOTPINK, (Math.sin(time * 2) + 1) / 2)
                .withBrightness(0.2);

            // 1. Draw glowing background radial echo polygons
            for (let layer = 4; layer >= 1; layer--) {
                const scale = 1.0 + layer * 0.25 + Math.sin(time * 3 + layer) * 0.05;
                const layerPoints = morphedPoints.map(p => {
                    return new Vector2(
                        CENTER.x + (p.x - CENTER.x) * scale,
                        CENTER.y + (p.y - CENTER.y) * scale
                    );
                });

                // Draw perimeter wireframe edges using lines
                for (let i = 0; i < layerPoints.length; i++) {
                    const nextPt = layerPoints[(i + 1) % layerPoints.length];
                    screen.shapes.drawLine(layerPoints[i], nextPt, {
                        color: themeColor.withAlpha(0.15 / layer)
                    });
                }
            }

            // 2. Draw interior dynamic geometric triangulation web
            for (let i = 0; i < morphedPoints.length; i++) {
                const p1 = morphedPoints[i];
                const p2 = morphedPoints[(i + 3) % morphedPoints.length];
                const p3 = morphedPoints[(i + 5) % morphedPoints.length];

                // Inner web triangle wireframes
                screen.shapes.drawTriangle(p1, p2, CENTER, {
                    color: Palette.VIOLET.withAlpha(0.12)
                });

                screen.shapes.drawLine(p1, p3, {
                    color: Palette.ELECTRIC_BLUE.withAlpha(0.25)
                });
            }

            // 3. Draw primary morphing polygon structure
            for (let i = 0; i < morphedPoints.length; i++) {
                const p1 = morphedPoints[i];
                const p2 = morphedPoints[(i + 1) % morphedPoints.length];

                // Perimeter structural lines
                screen.shapes.drawLine(p1, p2, { color: themeColor });

                // Corner node spheres
                screen.shapes.drawCircle(p1, 6, { color: Palette.WHITE });
                screen.shapes.drawCircle(p1, 12, { color: themeColor.withAlpha(0.3) });
            }

            // 4. Core focal point
            const coreSize = 12 + Math.sin(time * 6) * 4;
            screen.shapes.drawCircle(CENTER, coreSize, { color: Palette.GOLD });
            screen.shapes.drawCircle(CENTER, coreSize * 2, { color: Palette.GOLD.withAlpha(0.2) });
        });
    }
});