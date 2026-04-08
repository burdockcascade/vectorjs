const screenWidth = 800;
const screenHeight = 450;

class MyApplication {

    OnUpdate(dt, context) {
        // We can capture real-time window state from the updateContext
        this.currWidth = context.Screen.GetWidth();
        this.currHeight = context.Screen.GetHeight();

        // Update current Monitor info in case of changes (e.g., monitor added/removed)
        this.monitorIndex = context.Monitor.GetMonitor();
        this.monCount = context.Monitor.GetCount();
        this.physWidth = context.Monitor.GetPhysicalWidth(this.monitorIndex);
        this.physHeight = context.Monitor.GetPhysicalHeight(this.monitorIndex);
        this.refreshRate = context.Monitor.GetRefreshRate(this.monitorIndex);
    }

    OnDraw(render) {
        render.WithLayer2D((ctx) => {
            const textColor = Color.DARKBLUE;
            const startPos = { x: 40, y: 40 };
            const lineSpacing = 30;

            ctx.DrawText("MONITOR & WINDOW STATISTICS", startPos, 25, Color.MAROON);

            // Displaying captured Monitor Info
            ctx.DrawText(`Total Monitors: ${this.monCount}`,
                { x: startPos.x, y: startPos.y + lineSpacing * 2 }, 20, textColor);

            ctx.DrawText(`Monitor ${this.monitorIndex} Physical Size: ${this.physWidth}mm x ${this.physHeight}mm`,
                { x: startPos.x, y: startPos.y + lineSpacing * 3 }, 20, textColor);

            ctx.DrawText(`Refresh Rate: ${this.refreshRate} Hz`,
                { x: startPos.x, y: startPos.y + lineSpacing * 4 }, 20, textColor);

            // Displaying real-time Window Info
            ctx.DrawText(`Current Window: ${this.currWidth}x${this.currHeight}`,
                { x: startPos.x, y: startPos.y + lineSpacing * 6 }, 20, Color.BLACK);

            ctx.DrawFPS({ x: 10, y: screenHeight - 30 });
        });
    }
};

const app = new App(screenHeight, screenWidth, "Monitor & Window Info");
app.Run(new MyApplication());