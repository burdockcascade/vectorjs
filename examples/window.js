import {App, Color, Vector2} from "vectorjs";


const screenWidth = 800;
const screenHeight = 450;

class MyApplication {

    constructor() {
        this.textColor = Color.DARKBLUE;
        const startPos = new Vector2( 40, 40 );
        const lineSpacing = 30;
        this.titleTextPos = { x: startPos.x, y: startPos.y }
        this.totalMonitorTextPos = { x: startPos.x, y: startPos.y + lineSpacing * 2 }
        this.monitorTextPos = { x: startPos.x, y: startPos.y + lineSpacing * 3 }
        this.refreshRateTextPos = { x: startPos.x, y: startPos.y + lineSpacing * 4 }
        this.currentMonitorTextPos = { x: startPos.x, y: startPos.y + lineSpacing * 6 }
    }

    OnUpdate(dt, ctx) {

        // We can capture real-time window state from the updateContext
        this.currWidth = ctx.Screen.GetWidth();
        this.currHeight = ctx.Screen.GetHeight();

        // Update current Monitor info in case of changes (e.g., monitor added/removed)
        this.monitorIndex = ctx.Monitor.GetMonitor();
        this.monCount = ctx.Monitor.GetCount();
        this.physWidth = ctx.Monitor.GetPhysicalWidth(this.monitorIndex);
        this.physHeight = ctx.Monitor.GetPhysicalHeight(this.monitorIndex);
        this.refreshRate = ctx.Monitor.GetRefreshRate(this.monitorIndex);
    }

    OnDraw(render) {
        render.WithLayer2D((ctx) => {
            ctx.DrawText("MONITOR & WINDOW STATISTICS", this.titleTextPos, 25, Color.MAROON);

            // Displaying captured Monitor Info
            ctx.DrawText(`Total Monitors: ${this.monCount}`, this.totalMonitorTextPos, 20, this.textColor);
            ctx.DrawText(`Monitor ${this.monitorIndex} Physical Size: ${this.physWidth}mm x ${this.physHeight}mm`, this.monitorTextPos, 20, this.textColor);
            ctx.DrawText(`Refresh Rate: ${this.refreshRate} Hz`, this.refreshRateTextPos, 20, this.textColor);

            // Displaying real-time Window Info
            ctx.DrawText(`Current Window: ${this.currWidth}x${this.currHeight}`, this.currentMonitorTextPos, 20, Color.BLACK);

            ctx.DrawFPS(new Vector2(10, screenHeight - 30));
        });
    }
};

const app = new App(screenHeight, screenWidth, "Monitor & Window Info");
app.Run(new MyApplication());