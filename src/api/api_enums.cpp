#include "hostapi.hpp"
#include "../qjs.hpp"
#include "js_types.hpp"
#include <utility>
#include <string_view>
#include <concepts>

namespace VectorJS {

    static JSValue create_js_color_instance(JSContext* ctx, ::Color color) noexcept {
        return qjs::create_class_instance<JSColor>(ctx, js_color_class_id, color);
    }

    template <typename T>
    static void set_object_property(JSContext* ctx, JSValue obj, const char* name, T&& val) {
        constexpr int flags = JS_PROP_ENUMERABLE | JS_PROP_CONFIGURABLE;

        if constexpr (std::integral<std::decay_t<T>> || std::is_enum_v<std::decay_t<T>>) {
            JS_DefinePropertyValueStr(ctx, obj, name, JS_NewInt32(ctx, static_cast<int32_t>(val)), flags);
        } else if constexpr (std::floating_point<std::decay_t<T>>) {
            JS_DefinePropertyValueStr(ctx, obj, name, JS_NewFloat64(ctx, static_cast<double>(val)), flags);
        } else if constexpr (std::convertible_to<T, std::string_view>) {
            const std::string_view sv{val};
            JS_DefinePropertyValueStr(ctx, obj, name, JS_NewStringLen(ctx, sv.data(), sv.size()), flags);
        } else if constexpr (std::same_as<std::decay_t<T>, ::Color>) {
            JS_DefinePropertyValueStr(ctx, obj, name, create_js_color_instance(ctx, val), flags);
        }
    }

    template <typename... Args>
    static void set_object_properties(JSContext* ctx, JSValue obj, Args&&... entries) {
        (set_object_property(ctx, obj, entries.first, std::forward<decltype(entries.second)>(entries.second)), ...);
    }

    template <typename... Pairs>
    static void export_object(JSContext* js_context, JSModuleDef* js_module_def, const char* obj_name, Pairs&&... pairs) {
        const JSValue obj = JS_NewObject(js_context);
        set_object_properties(js_context, obj, std::forward<Pairs>(pairs)...);
        JS_SetModuleExport(js_context, js_module_def, obj_name, obj);
    }

    #define BIND_ENUM(val) std::pair{#val, val}
    #define BIND_COLOR(ctx, name, r, g, b, a) \
    std::pair{name, ::Color{(unsigned char)(r), (unsigned char)(g), (unsigned char)(b), (unsigned char)(a)}}

    void register_hapi_enums(JSContext* ctx, JSModuleDef* m) {

        export_object(ctx, m, "Palette",
            // Grays & Neutrals
            BIND_COLOR(ctx, "WHITE", 255, 255, 255, 255),
            BIND_COLOR(ctx, "SNOW", 255, 250, 250, 255),
            BIND_COLOR(ctx, "IVORY", 255, 255, 240, 255),
            BIND_COLOR(ctx, "RAYWHITE", 245, 245, 245, 255),
            BIND_COLOR(ctx, "OFFWHITE", 238, 238, 228, 255),
            BIND_COLOR(ctx, "LIGHTGRAY", 200, 200, 200, 255),
            BIND_COLOR(ctx, "SILVER", 192, 192, 192, 255),
            BIND_COLOR(ctx, "GRAY", 130, 130, 130, 255),
            BIND_COLOR(ctx, "SLATE", 112, 128, 144, 255),
            BIND_COLOR(ctx, "DARKGRAY", 80, 80, 80, 255),
            BIND_COLOR(ctx, "CHARBLACK", 30, 30, 30, 255),
            BIND_COLOR(ctx, "BLACK", 0, 0, 0, 255),
            BIND_COLOR(ctx, "BLANK", 0, 0, 0, 0),

            // Reds, Pinks & Oranges
            BIND_COLOR(ctx, "SALMON", 250, 128, 114, 255),
            BIND_COLOR(ctx, "CORAL", 255, 127, 80, 255),
            BIND_COLOR(ctx, "CRIMSON", 180, 10, 30, 255),
            BIND_COLOR(ctx, "RED", 230, 41, 55, 255),
            BIND_COLOR(ctx, "PURE_RED", 255, 0, 0, 255),
            BIND_COLOR(ctx, "SCARLET", 255, 36, 0, 255),
            BIND_COLOR(ctx, "MAROON", 190, 33, 55, 255),
            BIND_COLOR(ctx, "BURGUNDY", 128, 0, 32, 255),
            BIND_COLOR(ctx, "PEACH", 255, 218, 185, 255),
            BIND_COLOR(ctx, "ORANGE", 255, 161, 0, 255),
            BIND_COLOR(ctx, "DARKORANGE", 255, 140, 0, 255),
            BIND_COLOR(ctx, "AMBER", 255, 191, 0, 255),
            BIND_COLOR(ctx, "GOLD", 255, 203, 0, 255),
            BIND_COLOR(ctx, "YELLOW", 253, 249, 0, 255),
            BIND_COLOR(ctx, "LEMON", 255, 247, 0, 255),
            BIND_COLOR(ctx, "ROSE", 255, 192, 203, 255),
            BIND_COLOR(ctx, "PINK", 255, 109, 194, 255),
            BIND_COLOR(ctx, "HOTPINK", 255, 20, 147, 255),
            BIND_COLOR(ctx, "MAGENTA", 255, 0, 255, 255),
            BIND_COLOR(ctx, "FUCHSIA", 224, 17, 95, 255),

            // Greens & Teals
            BIND_COLOR(ctx, "MINT", 152, 251, 152, 255),
            BIND_COLOR(ctx, "PASTELGREEN", 119, 221, 119, 255),
            BIND_COLOR(ctx, "LIME", 0, 158, 47, 255),
            BIND_COLOR(ctx, "NEON_GREEN", 57, 255, 20, 255),
            BIND_COLOR(ctx, "GREEN", 0, 228, 48, 255),
            BIND_COLOR(ctx, "PURE_GREEN", 0, 255, 0, 255),
            BIND_COLOR(ctx, "EMERALD", 80, 200, 120, 255),
            BIND_COLOR(ctx, "DARKGREEN", 0, 117, 44, 255),
            BIND_COLOR(ctx, "FOREST", 34, 139, 34, 255),
            BIND_COLOR(ctx, "PINE", 1, 121, 111, 255),
            BIND_COLOR(ctx, "OLIVE", 128, 128, 0, 255),
            BIND_COLOR(ctx, "TURQUOISE", 64, 224, 208, 255),
            BIND_COLOR(ctx, "TEAL", 0, 128, 128, 255),

            // Blues & Cyans
            BIND_COLOR(ctx, "ELECTRIC_BLUE", 125, 249, 255, 255),
            BIND_COLOR(ctx, "CYAN", 0, 255, 255, 255),
            BIND_COLOR(ctx, "AQUA", 0, 255, 255, 255),
            BIND_COLOR(ctx, "PASTELBLUE", 174, 198, 207, 255),
            BIND_COLOR(ctx, "SKYBLUE", 102, 191, 255, 255),
            BIND_COLOR(ctx, "CORNFLOWER", 100, 149, 237, 255),
            BIND_COLOR(ctx, "BLUE", 0, 121, 241, 255),
            BIND_COLOR(ctx, "PURE_BLUE", 0, 0, 255, 255),
            BIND_COLOR(ctx, "ROYALBLUE", 65, 105, 225, 255),
            BIND_COLOR(ctx, "DARKBLUE", 0, 82, 172, 255),
            BIND_COLOR(ctx, "NAVY", 0, 0, 128, 255),
            BIND_COLOR(ctx, "MIDNIGHTBLUE", 25, 25, 112, 255),

            // Purples & Violets
            BIND_COLOR(ctx, "LAVENDER", 230, 230, 250, 255),
            BIND_COLOR(ctx, "LILAC", 200, 162, 200, 255),
            BIND_COLOR(ctx, "PURPLE", 200, 122, 255, 255),
            BIND_COLOR(ctx, "PURE_PURPLE", 128, 0, 128, 255),
            BIND_COLOR(ctx, "AMETHYST", 153, 102, 204, 255),
            BIND_COLOR(ctx, "VIOLET", 135, 60, 190, 255),
            BIND_COLOR(ctx, "INDIGO", 75, 0, 130, 255),
            BIND_COLOR(ctx, "DARKPURPLE", 112, 31, 126, 255),
            BIND_COLOR(ctx, "PLUM", 142, 69, 133, 255),

            // Browns & Earth Tones
            BIND_COLOR(ctx, "WHEAT", 245, 222, 179, 255),
            BIND_COLOR(ctx, "BEIGE", 211, 176, 131, 255),
            BIND_COLOR(ctx, "TAN", 210, 180, 140, 255),
            BIND_COLOR(ctx, "SAND", 194, 178, 128, 255),
            BIND_COLOR(ctx, "KHAKI", 189, 183, 107, 255),
            BIND_COLOR(ctx, "TERRACOTTA", 226, 114, 91, 255),
            BIND_COLOR(ctx, "BRONZE", 205, 127, 50, 255),
            BIND_COLOR(ctx, "RUST", 183, 65, 14, 255),
            BIND_COLOR(ctx, "BROWN", 127, 106, 91, 255),
            BIND_COLOR(ctx, "SADDLEBROWN", 139, 69, 19, 255),
            BIND_COLOR(ctx, "DARKBROWN", 76, 63, 47, 255),
            BIND_COLOR(ctx, "COFFEE", 111, 78, 55, 255)
        );

        // --- Info Object ---
        export_object(ctx, m, "Info",
            BIND_ENUM(RAYLIB_VERSION_STR),
            BIND_ENUM(QUICKJS_VERSION_STR)
        );

        // --- ConfigFlags Object ---
        export_object(ctx, m, "ConfigFlags",
            BIND_ENUM(FLAG_VSYNC_HINT),
            BIND_ENUM(FLAG_FULLSCREEN_MODE),
            BIND_ENUM(FLAG_WINDOW_RESIZABLE),
            BIND_ENUM(FLAG_WINDOW_UNDECORATED),
            BIND_ENUM(FLAG_WINDOW_HIDDEN),
            BIND_ENUM(FLAG_WINDOW_MINIMIZED),
            BIND_ENUM(FLAG_WINDOW_MAXIMIZED),
            BIND_ENUM(FLAG_WINDOW_UNFOCUSED),
            BIND_ENUM(FLAG_WINDOW_TOPMOST),
            BIND_ENUM(FLAG_WINDOW_ALWAYS_RUN),
            BIND_ENUM(FLAG_WINDOW_TRANSPARENT),
            BIND_ENUM(FLAG_WINDOW_HIGHDPI),
            BIND_ENUM(FLAG_WINDOW_MOUSE_PASSTHROUGH),
            BIND_ENUM(FLAG_BORDERLESS_WINDOWED_MODE),
            BIND_ENUM(FLAG_MSAA_4X_HINT),
            BIND_ENUM(FLAG_INTERLACED_HINT)
        );

        // --- Keyboard Object ---
        export_object(ctx, m, "Keyboard",
            BIND_ENUM(KEY_NULL),
            BIND_ENUM(KEY_APOSTROPHE),
            BIND_ENUM(KEY_COMMA),
            BIND_ENUM(KEY_MINUS),
            BIND_ENUM(KEY_PERIOD),
            BIND_ENUM(KEY_SLASH),
            BIND_ENUM(KEY_ZERO),
            BIND_ENUM(KEY_ONE),
            BIND_ENUM(KEY_TWO),
            BIND_ENUM(KEY_THREE),
            BIND_ENUM(KEY_FOUR),
            BIND_ENUM(KEY_FIVE),
            BIND_ENUM(KEY_SIX),
            BIND_ENUM(KEY_SEVEN),
            BIND_ENUM(KEY_EIGHT),
            BIND_ENUM(KEY_NINE),
            BIND_ENUM(KEY_SEMICOLON),
            BIND_ENUM(KEY_EQUAL),
            BIND_ENUM(KEY_A),
            BIND_ENUM(KEY_B),
            BIND_ENUM(KEY_C),
            BIND_ENUM(KEY_D),
            BIND_ENUM(KEY_E),
            BIND_ENUM(KEY_F),
            BIND_ENUM(KEY_G),
            BIND_ENUM(KEY_H),
            BIND_ENUM(KEY_I),
            BIND_ENUM(KEY_J),
            BIND_ENUM(KEY_K),
            BIND_ENUM(KEY_L),
            BIND_ENUM(KEY_M),
            BIND_ENUM(KEY_N),
            BIND_ENUM(KEY_O),
            BIND_ENUM(KEY_P),
            BIND_ENUM(KEY_Q),
            BIND_ENUM(KEY_R),
            BIND_ENUM(KEY_S),
            BIND_ENUM(KEY_T),
            BIND_ENUM(KEY_U),
            BIND_ENUM(KEY_V),
            BIND_ENUM(KEY_W),
            BIND_ENUM(KEY_X),
            BIND_ENUM(KEY_Y),
            BIND_ENUM(KEY_Z),
            BIND_ENUM(KEY_LEFT_BRACKET),
            BIND_ENUM(KEY_BACKSLASH),
            BIND_ENUM(KEY_RIGHT_BRACKET),
            BIND_ENUM(KEY_GRAVE),
            BIND_ENUM(KEY_SPACE),
            BIND_ENUM(KEY_ESCAPE),
            BIND_ENUM(KEY_ENTER),
            BIND_ENUM(KEY_TAB),
            BIND_ENUM(KEY_BACKSPACE),
            BIND_ENUM(KEY_INSERT),
            BIND_ENUM(KEY_DELETE),
            BIND_ENUM(KEY_RIGHT),
            BIND_ENUM(KEY_LEFT),
            BIND_ENUM(KEY_DOWN),
            BIND_ENUM(KEY_UP),
            BIND_ENUM(KEY_PAGE_UP),
            BIND_ENUM(KEY_PAGE_DOWN),
            BIND_ENUM(KEY_HOME),
            BIND_ENUM(KEY_END),
            BIND_ENUM(KEY_CAPS_LOCK),
            BIND_ENUM(KEY_SCROLL_LOCK),
            BIND_ENUM(KEY_NUM_LOCK),
            BIND_ENUM(KEY_PRINT_SCREEN),
            BIND_ENUM(KEY_PAUSE),
            BIND_ENUM(KEY_F1),
            BIND_ENUM(KEY_F2),
            BIND_ENUM(KEY_F3),
            BIND_ENUM(KEY_F4),
            BIND_ENUM(KEY_F5),
            BIND_ENUM(KEY_F6),
            BIND_ENUM(KEY_F7),
            BIND_ENUM(KEY_F8),
            BIND_ENUM(KEY_F9),
            BIND_ENUM(KEY_F10),
            BIND_ENUM(KEY_F11),
            BIND_ENUM(KEY_F12),
            BIND_ENUM(KEY_LEFT_SHIFT),
            BIND_ENUM(KEY_LEFT_CONTROL),
            BIND_ENUM(KEY_LEFT_ALT),
            BIND_ENUM(KEY_LEFT_SUPER),
            BIND_ENUM(KEY_RIGHT_SHIFT),
            BIND_ENUM(KEY_RIGHT_CONTROL),
            BIND_ENUM(KEY_RIGHT_ALT),
            BIND_ENUM(KEY_RIGHT_SUPER),
            BIND_ENUM(KEY_KB_MENU),
            BIND_ENUM(KEY_KP_0),
            BIND_ENUM(KEY_KP_1),
            BIND_ENUM(KEY_KP_2),
            BIND_ENUM(KEY_KP_3),
            BIND_ENUM(KEY_KP_4),
            BIND_ENUM(KEY_KP_5),
            BIND_ENUM(KEY_KP_6),
            BIND_ENUM(KEY_KP_7),
            BIND_ENUM(KEY_KP_8),
            BIND_ENUM(KEY_KP_9),
            BIND_ENUM(KEY_KP_DECIMAL),
            BIND_ENUM(KEY_KP_DIVIDE),
            BIND_ENUM(KEY_KP_MULTIPLY),
            BIND_ENUM(KEY_KP_SUBTRACT),
            BIND_ENUM(KEY_KP_ADD),
            BIND_ENUM(KEY_KP_ENTER),
            BIND_ENUM(KEY_KP_EQUAL),
            BIND_ENUM(KEY_BACK),
            BIND_ENUM(KEY_MENU),
            BIND_ENUM(KEY_VOLUME_UP),
            BIND_ENUM(KEY_VOLUME_DOWN)
        );

        // --- MouseButton Object ---
        export_object(ctx, m, "MouseButton",
            BIND_ENUM(MOUSE_BUTTON_LEFT),
            BIND_ENUM(MOUSE_BUTTON_RIGHT),
            BIND_ENUM(MOUSE_BUTTON_MIDDLE),
            BIND_ENUM(MOUSE_BUTTON_SIDE),
            BIND_ENUM(MOUSE_BUTTON_EXTRA),
            BIND_ENUM(MOUSE_BUTTON_FORWARD),
            BIND_ENUM(MOUSE_BUTTON_BACK)
        );

        // --- MouseCursor Object ---
        export_object(ctx, m, "MouseCursor",
            BIND_ENUM(MOUSE_CURSOR_DEFAULT),
            BIND_ENUM(MOUSE_CURSOR_ARROW),
            BIND_ENUM(MOUSE_CURSOR_IBEAM),
            BIND_ENUM(MOUSE_CURSOR_CROSSHAIR),
            BIND_ENUM(MOUSE_CURSOR_POINTING_HAND),
            BIND_ENUM(MOUSE_CURSOR_RESIZE_EW),
            BIND_ENUM(MOUSE_CURSOR_RESIZE_NS),
            BIND_ENUM(MOUSE_CURSOR_RESIZE_NWSE),
            BIND_ENUM(MOUSE_CURSOR_RESIZE_NESW),
            BIND_ENUM(MOUSE_CURSOR_RESIZE_ALL),
            BIND_ENUM(MOUSE_CURSOR_NOT_ALLOWED)
        );
    }

    #undef BIND_ENUM
    #undef BIND_COLOR

} // namespace HostApi