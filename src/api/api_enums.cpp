#include "hostapi.hpp"
#include "js_utils.hpp"
#include "js_types.hpp"
#include <utility>

namespace HostApi {

    static JSValue create_js_color_instance(JSContext* ctx, const ::Color color) {
        return Utils::create_class_instance<JSColor>(ctx, js_color_class_id, color);
    }

    template <typename T>
    static void set_object_property(JSContext* ctx, const JSValue obj, const char* name, const T& val) {
        constexpr int flags = JS_PROP_ENUMERABLE | JS_PROP_CONFIGURABLE;

        if constexpr (std::is_integral_v<T> || std::is_enum_v<T>) {
            JS_DefinePropertyValueStr(ctx, obj, name, JS_NewInt32(ctx, static_cast<int32_t>(val)), flags);
        } else if constexpr (std::is_floating_point_v<T>) {
            JS_DefinePropertyValueStr(ctx, obj, name, JS_NewFloat64(ctx, static_cast<double>(val)), flags);
        } else if constexpr (std::is_convertible_v<T, std::string> || std::is_same_v<T, const char*>) {
            JS_DefinePropertyValueStr(ctx, obj, name, JS_NewString(ctx, std::string(val).c_str()), flags);
        } else if constexpr (std::is_same_v<T, ::Color>) {
            JS_DefinePropertyValueStr(ctx, obj, name, create_js_color_instance(ctx, val), flags);
        }
    }

    template <typename... Args>
    static void set_object_properties(JSContext* ctx, const JSValue obj, Args&&... entries) {
        (set_object_property(ctx, obj, entries.first, entries.second), ...);
    }

    template <typename... Pairs>
    static void export_object(JSContext* js_context, JSModuleDef* js_module_def, const char* obj_name, Pairs&&... pairs) {
        const JSValue obj = JS_NewObject(js_context);
        set_object_properties(js_context, obj, std::forward<Pairs>(pairs)...);
        JS_SetModuleExport(js_context, js_module_def, obj_name, obj);
    }

    #define BIND(val) std::pair{#val, val}

    void register_hapi_enums(JSContext* ctx, JSModuleDef* m) {

        // --- Palette Object ---
        export_object(ctx, m, "Palette",
            BIND(LIGHTGRAY),
            BIND(GRAY),
            BIND(DARKGRAY),
            BIND(YELLOW),
            BIND(GOLD),
            BIND(ORANGE),
            BIND(PINK),
            BIND(RED),
            BIND(MAROON),
            BIND(GREEN),
            BIND(LIME),
            BIND(DARKGREEN),
            BIND(SKYBLUE),
            BIND(BLUE),
            BIND(DARKBLUE),
            BIND(PURPLE),
            BIND(VIOLET),
            BIND(DARKPURPLE),
            BIND(BEIGE),
            BIND(BROWN),
            BIND(DARKBROWN),
            BIND(WHITE),
            BIND(BLACK),
            BIND(BLANK),
            BIND(MAGENTA),
            BIND(RAYWHITE)
        );

        // --- Info Object ---
        export_object(ctx, m, "Info",
            BIND(RAYLIB_VERSION_STR),
            BIND(QUICKJS_VERSION_STR)
        );

        // --- ConfigFlags Object ---
        export_object(ctx, m, "ConfigFlags",
            BIND(FLAG_VSYNC_HINT),
            BIND(FLAG_FULLSCREEN_MODE),
            BIND(FLAG_WINDOW_RESIZABLE),
            BIND(FLAG_WINDOW_UNDECORATED),
            BIND(FLAG_WINDOW_HIDDEN),
            BIND(FLAG_WINDOW_MINIMIZED),
            BIND(FLAG_WINDOW_MAXIMIZED),
            BIND(FLAG_WINDOW_UNFOCUSED),
            BIND(FLAG_WINDOW_TOPMOST),
            BIND(FLAG_WINDOW_ALWAYS_RUN),
            BIND(FLAG_WINDOW_TRANSPARENT),
            BIND(FLAG_WINDOW_HIGHDPI),
            BIND(FLAG_WINDOW_MOUSE_PASSTHROUGH),
            BIND(FLAG_BORDERLESS_WINDOWED_MODE),
            BIND(FLAG_MSAA_4X_HINT),
            BIND(FLAG_INTERLACED_HINT)
        );

        // --- Keyboard Object ---
        export_object(ctx, m, "Keyboard",
            BIND(KEY_NULL),
            BIND(KEY_APOSTROPHE),
            BIND(KEY_COMMA),
            BIND(KEY_MINUS),
            BIND(KEY_PERIOD),
            BIND(KEY_SLASH),
            BIND(KEY_ZERO),
            BIND(KEY_ONE),
            BIND(KEY_TWO),
            BIND(KEY_THREE),
            BIND(KEY_FOUR),
            BIND(KEY_FIVE),
            BIND(KEY_SIX),
            BIND(KEY_SEVEN),
            BIND(KEY_EIGHT),
            BIND(KEY_NINE),
            BIND(KEY_SEMICOLON),
            BIND(KEY_EQUAL),
            BIND(KEY_A),
            BIND(KEY_B),
            BIND(KEY_C),
            BIND(KEY_D),
            BIND(KEY_E),
            BIND(KEY_F),
            BIND(KEY_G),
            BIND(KEY_H),
            BIND(KEY_I),
            BIND(KEY_J),
            BIND(KEY_K),
            BIND(KEY_L),
            BIND(KEY_M),
            BIND(KEY_N),
            BIND(KEY_O),
            BIND(KEY_P),
            BIND(KEY_Q),
            BIND(KEY_R),
            BIND(KEY_S),
            BIND(KEY_T),
            BIND(KEY_U),
            BIND(KEY_V),
            BIND(KEY_W),
            BIND(KEY_X),
            BIND(KEY_Y),
            BIND(KEY_Z),
            BIND(KEY_LEFT_BRACKET),
            BIND(KEY_BACKSLASH),
            BIND(KEY_RIGHT_BRACKET),
            BIND(KEY_GRAVE),
            BIND(KEY_SPACE),
            BIND(KEY_ESCAPE),
            BIND(KEY_ENTER),
            BIND(KEY_TAB),
            BIND(KEY_BACKSPACE),
            BIND(KEY_INSERT),
            BIND(KEY_DELETE),
            BIND(KEY_RIGHT),
            BIND(KEY_LEFT),
            BIND(KEY_DOWN),
            BIND(KEY_UP),
            BIND(KEY_PAGE_UP),
            BIND(KEY_PAGE_DOWN),
            BIND(KEY_HOME),
            BIND(KEY_END),
            BIND(KEY_CAPS_LOCK),
            BIND(KEY_SCROLL_LOCK),
            BIND(KEY_NUM_LOCK),
            BIND(KEY_PRINT_SCREEN),
            BIND(KEY_PAUSE),
            BIND(KEY_F1),
            BIND(KEY_F2),
            BIND(KEY_F3),
            BIND(KEY_F4),
            BIND(KEY_F5),
            BIND(KEY_F6),
            BIND(KEY_F7),
            BIND(KEY_F8),
            BIND(KEY_F9),
            BIND(KEY_F10),
            BIND(KEY_F11),
            BIND(KEY_F12),
            BIND(KEY_LEFT_SHIFT),
            BIND(KEY_LEFT_CONTROL),
            BIND(KEY_LEFT_ALT),
            BIND(KEY_LEFT_SUPER),
            BIND(KEY_RIGHT_SHIFT),
            BIND(KEY_RIGHT_CONTROL),
            BIND(KEY_RIGHT_ALT),
            BIND(KEY_RIGHT_SUPER),
            BIND(KEY_KB_MENU),
            BIND(KEY_KP_0),
            BIND(KEY_KP_1),
            BIND(KEY_KP_2),
            BIND(KEY_KP_3),
            BIND(KEY_KP_4),
            BIND(KEY_KP_5),
            BIND(KEY_KP_6),
            BIND(KEY_KP_7),
            BIND(KEY_KP_8),
            BIND(KEY_KP_9),
            BIND(KEY_KP_DECIMAL),
            BIND(KEY_KP_DIVIDE),
            BIND(KEY_KP_MULTIPLY),
            BIND(KEY_KP_SUBTRACT),
            BIND(KEY_KP_ADD),
            BIND(KEY_KP_ENTER),
            BIND(KEY_KP_EQUAL),
            BIND(KEY_BACK),
            BIND(KEY_MENU),
            BIND(KEY_VOLUME_UP),
            BIND(KEY_VOLUME_DOWN)
        );

        // --- MouseButton Object ---
        export_object(ctx, m, "MouseButton",
            BIND(MOUSE_BUTTON_LEFT),
            BIND(MOUSE_BUTTON_RIGHT),
            BIND(MOUSE_BUTTON_MIDDLE),
            BIND(MOUSE_BUTTON_SIDE),
            BIND(MOUSE_BUTTON_EXTRA),
            BIND(MOUSE_BUTTON_FORWARD),
            BIND(MOUSE_BUTTON_BACK)
        );

        // --- MouseCursor Object ---
        export_object(ctx, m, "MouseCursor",
            BIND(MOUSE_CURSOR_DEFAULT),
            BIND(MOUSE_CURSOR_ARROW),
            BIND(MOUSE_CURSOR_IBEAM),
            BIND(MOUSE_CURSOR_CROSSHAIR),
            BIND(MOUSE_CURSOR_POINTING_HAND),
            BIND(MOUSE_CURSOR_RESIZE_EW),
            BIND(MOUSE_CURSOR_RESIZE_NS),
            BIND(MOUSE_CURSOR_RESIZE_NWSE),
            BIND(MOUSE_CURSOR_RESIZE_NESW),
            BIND(MOUSE_CURSOR_RESIZE_ALL),
            BIND(MOUSE_CURSOR_NOT_ALLOWED)
        );
    }

    #undef BIND

} // namespace HostApi