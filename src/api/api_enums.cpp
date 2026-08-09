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

    void register_hapi_enums(JSContext* ctx, JSModuleDef* m) {

        export_object(ctx, m, "Palette",
            // Grays & Neutrals
            std::pair{"WHITE", ::Color{ .r = 255, .g = 255, .b = 255, .a = 255 }},
            std::pair{"SNOW", ::Color{ .r = 255, .g = 250, .b = 250, .a = 255 }},
            std::pair{"IVORY", ::Color{ .r = 255, .g = 255, .b = 240, .a = 255 }},
            std::pair{"RAYWHITE", ::Color{ .r = 245, .g = 245, .b = 245, .a = 255 }},
            std::pair{"OFFWHITE", ::Color{ .r = 238, .g = 238, .b = 228, .a = 255 }},
            std::pair{"LIGHTGRAY", ::Color{ .r = 200, .g = 200, .b = 200, .a = 255 }},
            std::pair{"SILVER", ::Color{ .r = 192, .g = 192, .b = 192, .a = 255 }},
            std::pair{"GRAY", ::Color{ .r = 130, .g = 130, .b = 130, .a = 255 }},
            std::pair{"SLATE", ::Color{ .r = 112, .g = 128, .b = 144, .a = 255 }},
            std::pair{"DARKGRAY", ::Color{ .r = 80, .g = 80, .b = 80, .a = 255 }},
            std::pair{"CHARBLACK", ::Color{ .r = 30, .g = 30, .b = 30, .a = 255 }},
            std::pair{"BLACK", ::Color{ .r = 0, .g = 0, .b = 0, .a = 255 }},
            std::pair{"BLANK", ::Color{ .r = 0, .g = 0, .b = 0, .a = 0 }},

            // Reds, Pinks & Oranges
            std::pair{"SALMON", ::Color{ .r = 250, .g = 128, .b = 114, .a = 255 }},
            std::pair{"CORAL", ::Color{ .r = 255, .g = 127, .b = 80, .a = 255 }},
            std::pair{"CRIMSON", ::Color{ .r = 180, .g = 10, .b = 30, .a = 255 }},
            std::pair{"RED", ::Color{ .r = 230, .g = 41, .b = 55, .a = 255 }},
            std::pair{"PURE_RED", ::Color{ .r = 255, .g = 0, .b = 0, .a = 255 }},
            std::pair{"SCARLET", ::Color{ .r = 255, .g = 36, .b = 0, .a = 255 }},
            std::pair{"MAROON", ::Color{ .r = 190, .g = 33, .b = 55, .a = 255 }},
            std::pair{"BURGUNDY", ::Color{ .r = 128, .g = 0, .b = 32, .a = 255 }},
            std::pair{"PEACH", ::Color{ .r = 255, .g = 218, .b = 185, .a = 255 }},
            std::pair{"ORANGE", ::Color{ .r = 255, .g = 161, .b = 0, .a = 255 }},
            std::pair{"DARKORANGE", ::Color{ .r = 255, .g = 140, .b = 0, .a = 255 }},
            std::pair{"AMBER", ::Color{ .r = 255, .g = 191, .b = 0, .a = 255 }},
            std::pair{"GOLD", ::Color{ .r = 255, .g = 203, .b = 0, .a = 255 }},
            std::pair{"YELLOW", ::Color{ .r = 253, .g = 249, .b = 0, .a = 255 }},
            std::pair{"LEMON", ::Color{ .r = 255, .g = 247, .b = 0, .a = 255 }},
            std::pair{"ROSE", ::Color{ .r = 255, .g = 192, .b = 203, .a = 255 }},
            std::pair{"PINK", ::Color{ .r = 255, .g = 109, .b = 194, .a = 255 }},
            std::pair{"HOTPINK", ::Color{ .r = 255, .g = 20, .b = 147, .a = 255 }},
            std::pair{"MAGENTA", ::Color{ .r = 255, .g = 0, .b = 255, .a = 255 }},
            std::pair{"FUCHSIA", ::Color{ .r = 224, .g = 17, .b = 95, .a = 255 }},

            // Greens & Teals
            std::pair{"MINT", ::Color{ .r = 152, .g = 251, .b = 152, .a = 255 }},
            std::pair{"PASTELGREEN", ::Color{ .r = 119, .g = 221, .b = 119, .a = 255 }},
            std::pair{"LIME", ::Color{ .r = 0, .g = 158, .b = 47, .a = 255 }},
            std::pair{"NEON_GREEN", ::Color{ .r = 57, .g = 255, .b = 20, .a = 255 }},
            std::pair{"GREEN", ::Color{ .r = 0, .g = 228, .b = 48, .a = 255 }},
            std::pair{"PURE_GREEN", ::Color{ .r = 0, .g = 255, .b = 0, .a = 255 }},
            std::pair{"EMERALD", ::Color{ .r = 80, .g = 200, .b = 120, .a = 255 }},
            std::pair{"DARKGREEN", ::Color{ .r = 0, .g = 117, .b = 44, .a = 255 }},
            std::pair{"FOREST", ::Color{ .r = 34, .g = 139, .b = 34, .a = 255 }},
            std::pair{"PINE", ::Color{ .r = 1, .g = 121, .b = 111, .a = 255 }},
            std::pair{"OLIVE", ::Color{ .r = 128, .g = 128, .b = 0, .a = 255 }},
            std::pair{"TURQUOISE", ::Color{ .r = 64, .g = 224, .b = 208, .a = 255 }},
            std::pair{"TEAL", ::Color{ .r = 0, .g = 128, .b = 128, .a = 255 }},

            // Blues & Cyans
            std::pair{"ELECTRIC_BLUE", ::Color{ .r = 125, .g = 249, .b = 255, .a = 255 }},
            std::pair{"CYAN", ::Color{ .r = 0, .g = 255, .b = 255, .a = 255 }},
            std::pair{"AQUA", ::Color{ .r = 0, .g = 255, .b = 255, .a = 255 }},
            std::pair{"PASTELBLUE", ::Color{ .r = 174, .g = 198, .b = 207, .a = 255 }},
            std::pair{"SKYBLUE", ::Color{ .r = 102, .g = 191, .b = 255, .a = 255 }},
            std::pair{"CORNFLOWER", ::Color{ .r = 100, .g = 149, .b = 237, .a = 255 }},
            std::pair{"BLUE", ::Color{ .r = 0, .g = 121, .b = 241, .a = 255 }},
            std::pair{"PURE_BLUE", ::Color{ .r = 0, .g = 0, .b = 255, .a = 255 }},
            std::pair{"ROYALBLUE", ::Color{ .r = 65, .g = 105, .b = 225, .a = 255 }},
            std::pair{"DARKBLUE", ::Color{ .r = 0, .g = 82, .b = 172, .a = 255 }},
            std::pair{"NAVY", ::Color{ .r = 0, .g = 0, .b = 128, .a = 255 }},
            std::pair{"MIDNIGHTBLUE", ::Color{ .r = 25, .g = 25, .b = 112, .a = 255 }},

            // Purples & Violets
            std::pair{"LAVENDER", ::Color{ .r = 230, .g = 230, .b = 250, .a = 255 }},
            std::pair{"LILAC", ::Color{ .r = 200, .g = 162, .b = 200, .a = 255 }},
            std::pair{"PURPLE", ::Color{ .r = 200, .g = 122, .b = 255, .a = 255 }},
            std::pair{"PURE_PURPLE", ::Color{ .r = 128, .g = 0, .b = 128, .a = 255 }},
            std::pair{"AMETHYST", ::Color{ .r = 153, .g = 102, .b = 204, .a = 255 }},
            std::pair{"VIOLET", ::Color{ .r = 135, .g = 60, .b = 190, .a = 255 }},
            std::pair{"INDIGO", ::Color{ .r = 75, .g = 0, .b = 130, .a = 255 }},
            std::pair{"DARKPURPLE", ::Color{ .r = 112, .g = 31, .b = 126, .a = 255 }},
            std::pair{"PLUM", ::Color{ .r = 142, .g = 69, .b = 133, .a = 255 }},

            // Browns & Earth Tones
            std::pair{"WHEAT", ::Color{ .r = 245, .g = 222, .b = 179, .a = 255 }},
            std::pair{"BEIGE", ::Color{ .r = 211, .g = 176, .b = 131, .a = 255 }},
            std::pair{"TAN", ::Color{ .r = 210, .g = 180, .b = 140, .a = 255 }},
            std::pair{"SAND", ::Color{ .r = 194, .g = 178, .b = 128, .a = 255 }},
            std::pair{"KHAKI", ::Color{ .r = 189, .g = 183, .b = 107, .a = 255 }},
            std::pair{"TERRACOTTA", ::Color{ .r = 226, .g = 114, .b = 91, .a = 255 }},
            std::pair{"BRONZE", ::Color{ .r = 205, .g = 127, .b = 50, .a = 255 }},
            std::pair{"RUST", ::Color{ .r = 183, .g = 65, .b = 14, .a = 255 }},
            std::pair{"BROWN", ::Color{ .r = 127, .g = 106, .b = 91, .a = 255 }},
            std::pair{"SADDLEBROWN", ::Color{ .r = 139, .g = 69, .b = 19, .a = 255 }},
            std::pair{"DARKBROWN", ::Color{ .r = 76, .g = 63, .b = 47, .a = 255 }},
            std::pair{"COFFEE", ::Color{ .r = 111, .g = 78, .b = 55, .a = 255 }}
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

} // namespace HostApi