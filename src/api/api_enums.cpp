#include "hostapi.hpp"
#include "js_utils.hpp"
#include <utility>

namespace HostApi {

void register_hapi_enums(JSContext* ctx, JSModuleDef* m) {

    static auto export_object = []<typename... Pairs>(JSContext* js_context, JSModuleDef* js_module_def, const char* obj_name, Pairs&&... pairs) {
        const JSValue obj = JS_NewObject(js_context);
        Utils::set_object_properties(js_context, obj, std::forward<Pairs>(pairs)...);
        JS_SetModuleExport(js_context, js_module_def, obj_name, obj);
    };

    constexpr static auto P = []<typename V>(const char* name, V&& val) {
        return std::pair{name, std::forward<V>(val)};
    };

    // --- Palette Object ---
    export_object(ctx, m, "Palette",
        P("LIGHTGRAY", LIGHTGRAY),
        P("GRAY", GRAY),
        P("DARKGRAY", DARKGRAY),
        P("YELLOW", YELLOW),
        P("GOLD", GOLD),
        P("ORANGE", ORANGE),
        P("PINK", PINK),
        P("RED", RED),
        P("MAROON", MAROON),
        P("GREEN", GREEN),
        P("LIME", LIME),
        P("DARKGREEN", DARKGREEN),
        P("SKYBLUE", SKYBLUE),
        P("BLUE", BLUE),
        P("DARKBLUE", DARKBLUE),
        P("PURPLE", PURPLE),
        P("VIOLET", VIOLET),
        P("DARKPURPLE", DARKPURPLE),
        P("BEIGE", BEIGE),
        P("BROWN", BROWN),
        P("DARKBROWN", DARKBROWN),
        P("WHITE", WHITE),
        P("BLACK", BLACK),
        P("BLANK", BLANK),
        P("MAGENTA", MAGENTA),
        P("RAYWHITE", RAYWHITE)
    );

    // --- Info Object ---
    export_object(ctx, m, "Info",
        P("RAYLIB_VERSION", RAYLIB_VERSION_STR),
        P("QUICKJS_VERSION", QUICKJS_VERSION_STR)
    );

    // --- ConfigFlags Object ---
    export_object(ctx, m, "ConfigFlags",
        P("FLAG_VSYNC_HINT", FLAG_VSYNC_HINT),
        P("FLAG_FULLSCREEN_MODE", FLAG_FULLSCREEN_MODE),
        P("FLAG_WINDOW_RESIZABLE", FLAG_WINDOW_RESIZABLE),
        P("FLAG_WINDOW_UNDECORATED", FLAG_WINDOW_UNDECORATED),
        P("FLAG_WINDOW_HIDDEN", FLAG_WINDOW_HIDDEN),
        P("FLAG_WINDOW_MINIMIZED", FLAG_WINDOW_MINIMIZED),
        P("FLAG_WINDOW_MAXIMIZED", FLAG_WINDOW_MAXIMIZED),
        P("FLAG_WINDOW_UNFOCUSED", FLAG_WINDOW_UNFOCUSED),
        P("FLAG_WINDOW_TOPMOST", FLAG_WINDOW_TOPMOST),
        P("FLAG_WINDOW_ALWAYS_RUN", FLAG_WINDOW_ALWAYS_RUN),
        P("FLAG_WINDOW_TRANSPARENT", FLAG_WINDOW_TRANSPARENT),
        P("FLAG_WINDOW_HIGHDPI", FLAG_WINDOW_HIGHDPI),
        P("FLAG_WINDOW_MOUSE_PASSTHROUGH", FLAG_WINDOW_MOUSE_PASSTHROUGH),
        P("FLAG_BORDERLESS_WINDOWED_MODE", FLAG_BORDERLESS_WINDOWED_MODE),
        P("FLAG_MSAA_4X_HINT", FLAG_MSAA_4X_HINT),
        P("FLAG_INTERLACED_HINT", FLAG_INTERLACED_HINT)
    );
    
    export_object(ctx, m, "Keyboard",
            P("KEY_NULL", KEY_NULL),
            P("KEY_APOSTROPHE", KEY_APOSTROPHE),
            P("KEY_COMMA", KEY_COMMA),
            P("KEY_MINUS", KEY_MINUS),
            P("KEY_PERIOD", KEY_PERIOD),
            P("KEY_SLASH", KEY_SLASH),
            P("KEY_ZERO", KEY_ZERO),
            P("KEY_ONE", KEY_ONE),
            P("KEY_TWO", KEY_TWO),
            P("KEY_THREE", KEY_THREE),
            P("KEY_FOUR", KEY_FOUR),
            P("KEY_FIVE", KEY_FIVE),
            P("KEY_SIX", KEY_SIX),
            P("KEY_SEVEN", KEY_SEVEN),
            P("KEY_EIGHT", KEY_EIGHT),
            P("KEY_NINE", KEY_NINE),
            P("KEY_SEMICOLON", KEY_SEMICOLON),
            P("KEY_EQUAL", KEY_EQUAL),
            P("KEY_A", KEY_A),
            P("KEY_B", KEY_B),
            P("KEY_C", KEY_C),
            P("KEY_D", KEY_D),
            P("KEY_E", KEY_E),
            P("KEY_F", KEY_F),
            P("KEY_G", KEY_G),
            P("KEY_H", KEY_H),
            P("KEY_I", KEY_I),
            P("KEY_J", KEY_J),
            P("KEY_K", KEY_K),
            P("KEY_L", KEY_L),
            P("KEY_M", KEY_M),
            P("KEY_N", KEY_N),
            P("KEY_O", KEY_O),
            P("KEY_P", KEY_P),
            P("KEY_Q", KEY_Q),
            P("KEY_R", KEY_R),
            P("KEY_S", KEY_S),
            P("KEY_T", KEY_T),
            P("KEY_U", KEY_U),
            P("KEY_V", KEY_V),
            P("KEY_W", KEY_W),
            P("KEY_X", KEY_X),
            P("KEY_Y", KEY_Y),
            P("KEY_Z", KEY_Z),
            P("KEY_LEFT_BRACKET", KEY_LEFT_BRACKET),
            P("KEY_BACKSLASH", KEY_BACKSLASH),
            P("KEY_RIGHT_BRACKET", KEY_RIGHT_BRACKET),
            P("KEY_GRAVE", KEY_GRAVE),
            P("KEY_SPACE", KEY_SPACE),
            P("KEY_ESCAPE", KEY_ESCAPE),
            P("KEY_ENTER", KEY_ENTER),
            P("KEY_TAB", KEY_TAB),
            P("KEY_BACKSPACE", KEY_BACKSPACE),
            P("KEY_INSERT", KEY_INSERT),
            P("KEY_DELETE", KEY_DELETE),
            P("KEY_RIGHT", KEY_RIGHT),
            P("KEY_LEFT", KEY_LEFT),
            P("KEY_DOWN", KEY_DOWN),
            P("KEY_UP", KEY_UP),
            P("KEY_PAGE_UP", KEY_PAGE_UP),
            P("KEY_PAGE_DOWN", KEY_PAGE_DOWN),
            P("KEY_HOME", KEY_HOME),
            P("KEY_END", KEY_END),
            P("KEY_CAPS_LOCK", KEY_CAPS_LOCK),
            P("KEY_SCROLL_LOCK", KEY_SCROLL_LOCK),
            P("KEY_NUM_LOCK", KEY_NUM_LOCK),
            P("KEY_PRINT_SCREEN", KEY_PRINT_SCREEN),
            P("KEY_PAUSE", KEY_PAUSE),
            P("KEY_F1", KEY_F1),
            P("KEY_F2", KEY_F2),
            P("KEY_F3", KEY_F3),
            P("KEY_F4", KEY_F4),
            P("KEY_F5", KEY_F5),
            P("KEY_F6", KEY_F6),
            P("KEY_F7", KEY_F7),
            P("KEY_F8", KEY_F8),
            P("KEY_F9", KEY_F9),
            P("KEY_F10", KEY_F10),
            P("KEY_F11", KEY_F11),
            P("KEY_F12", KEY_F12),
            P("KEY_LEFT_SHIFT", KEY_LEFT_SHIFT),
            P("KEY_LEFT_CONTROL", KEY_LEFT_CONTROL),
            P("KEY_LEFT_ALT", KEY_LEFT_ALT),
            P("KEY_LEFT_SUPER", KEY_LEFT_SUPER),
            P("KEY_RIGHT_SHIFT", KEY_RIGHT_SHIFT),
            P("KEY_RIGHT_CONTROL", KEY_RIGHT_CONTROL),
            P("KEY_RIGHT_ALT", KEY_RIGHT_ALT),
            P("KEY_RIGHT_SUPER", KEY_RIGHT_SUPER),
            P("KEY_KB_MENU", KEY_KB_MENU),
            P("KEY_KP_0", KEY_KP_0),
            P("KEY_KP_1", KEY_KP_1),
            P("KEY_KP_2", KEY_KP_2),
            P("KEY_KP_3", KEY_KP_3),
            P("KEY_KP_4", KEY_KP_4),
            P("KEY_KP_5", KEY_KP_5),
            P("KEY_KP_6", KEY_KP_6),
            P("KEY_KP_7", KEY_KP_7),
            P("KEY_KP_8", KEY_KP_8),
            P("KEY_KP_9", KEY_KP_9),
            P("KEY_KP_DECIMAL", KEY_KP_DECIMAL),
            P("KEY_KP_DIVIDE", KEY_KP_DIVIDE),
            P("KEY_KP_MULTIPLY", KEY_KP_MULTIPLY),
            P("KEY_KP_SUBTRACT", KEY_KP_SUBTRACT),
            P("KEY_KP_ADD", KEY_KP_ADD),
            P("KEY_KP_ENTER", KEY_KP_ENTER),
            P("KEY_KP_EQUAL", KEY_KP_EQUAL),
            P("KEY_BACK", KEY_BACK),
            P("KEY_MENU", KEY_MENU),
            P("KEY_VOLUME_UP", KEY_VOLUME_UP),
            P("KEY_VOLUME_DOWN", KEY_VOLUME_DOWN)
        );

        // Enum: MouseButton
        export_object(ctx, m, "MouseButton",
            P("MOUSE_BUTTON_LEFT", MOUSE_BUTTON_LEFT),
            P("MOUSE_BUTTON_RIGHT", MOUSE_BUTTON_RIGHT),
            P("MOUSE_BUTTON_MIDDLE", MOUSE_BUTTON_MIDDLE),
            P("MOUSE_BUTTON_SIDE", MOUSE_BUTTON_SIDE),
            P("MOUSE_BUTTON_EXTRA", MOUSE_BUTTON_EXTRA),
            P("MOUSE_BUTTON_FORWARD", MOUSE_BUTTON_FORWARD),
            P("MOUSE_BUTTON_BACK", MOUSE_BUTTON_BACK)
        );

        // Enum: MouseCursor
        export_object(ctx, m, "MouseCursor",
            P("MOUSE_CURSOR_DEFAULT", MOUSE_CURSOR_DEFAULT),
            P("MOUSE_CURSOR_ARROW", MOUSE_CURSOR_ARROW),
            P("MOUSE_CURSOR_IBEAM", MOUSE_CURSOR_IBEAM),
            P("MOUSE_CURSOR_CROSSHAIR", MOUSE_CURSOR_CROSSHAIR),
            P("MOUSE_CURSOR_POINTING_HAND", MOUSE_CURSOR_POINTING_HAND),
            P("MOUSE_CURSOR_RESIZE_EW", MOUSE_CURSOR_RESIZE_EW),
            P("MOUSE_CURSOR_RESIZE_NS", MOUSE_CURSOR_RESIZE_NS),
            P("MOUSE_CURSOR_RESIZE_NWSE", MOUSE_CURSOR_RESIZE_NWSE),
            P("MOUSE_CURSOR_RESIZE_NESW", MOUSE_CURSOR_RESIZE_NESW),
            P("MOUSE_CURSOR_RESIZE_ALL", MOUSE_CURSOR_RESIZE_ALL),
            P("MOUSE_CURSOR_NOT_ALLOWED", MOUSE_CURSOR_NOT_ALLOWED)
        );
}

} // namespace HostApi