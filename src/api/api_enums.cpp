#include "hostapi.hpp"
#include "js_utils.hpp"
#include "js_types.hpp"
#include <utility>

namespace HostApi {

    // Helper forward declaration / overload for JSColor initialization
    static JSValue create_js_color_instance(JSContext* ctx, const ::Color color) {
        return Utils::create_class_instance<JSColor>(ctx, js_color_class_id, color);
    }

    // --- Property & Object Setter Helpers ---

    template <typename T>
    static void set_object_property(JSContext* ctx, JSValueConst obj, const char* name, const T& val) {
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
    static void set_object_properties(JSContext* ctx, JSValueConst obj, Args&&... entries) {
        (set_object_property(ctx, obj, entries.first, entries.second), ...);
    }

    template <typename... Pairs>
    static void export_object(JSContext* js_context, JSModuleDef* js_module_def, const char* obj_name, Pairs&&... pairs) {
        const JSValue obj = JS_NewObject(js_context);
        set_object_properties(js_context, obj, std::forward<Pairs>(pairs)...);
        JS_SetModuleExport(js_context, js_module_def, obj_name, obj);
    }

    void register_hapi_enums(JSContext* ctx, JSModuleDef* m) {

        // --- Palette Object ---
        export_object(ctx, m, "Palette",
            std::pair{"LIGHTGRAY", LIGHTGRAY},
            std::pair{"GRAY", GRAY},
            std::pair{"DARKGRAY", DARKGRAY},
            std::pair{"YELLOW", YELLOW},
            std::pair{"GOLD", GOLD},
            std::pair{"ORANGE", ORANGE},
            std::pair{"PINK", PINK},
            std::pair{"RED", RED},
            std::pair{"MAROON", MAROON},
            std::pair{"GREEN", GREEN},
            std::pair{"LIME", LIME},
            std::pair{"DARKGREEN", DARKGREEN},
            std::pair{"SKYBLUE", SKYBLUE},
            std::pair{"BLUE", BLUE},
            std::pair{"DARKBLUE", DARKBLUE},
            std::pair{"PURPLE", PURPLE},
            std::pair{"VIOLET", VIOLET},
            std::pair{"DARKPURPLE", DARKPURPLE},
            std::pair{"BEIGE", BEIGE},
            std::pair{"BROWN", BROWN},
            std::pair{"DARKBROWN", DARKBROWN},
            std::pair{"WHITE", WHITE},
            std::pair{"BLACK", BLACK},
            std::pair{"BLANK", BLANK},
            std::pair{"MAGENTA", MAGENTA},
            std::pair{"RAYWHITE", RAYWHITE}
        );

        // --- Info Object ---
        export_object(ctx, m, "Info",
            std::pair{"RAYLIB_VERSION", RAYLIB_VERSION_STR},
            std::pair{"QUICKJS_VERSION", QUICKJS_VERSION_STR}
        );

        // --- ConfigFlags Object ---
        export_object(ctx, m, "ConfigFlags",
            std::pair{"FLAG_VSYNC_HINT", FLAG_VSYNC_HINT},
            std::pair{"FLAG_FULLSCREEN_MODE", FLAG_FULLSCREEN_MODE},
            std::pair{"FLAG_WINDOW_RESIZABLE", FLAG_WINDOW_RESIZABLE},
            std::pair{"FLAG_WINDOW_UNDECORATED", FLAG_WINDOW_UNDECORATED},
            std::pair{"FLAG_WINDOW_HIDDEN", FLAG_WINDOW_HIDDEN},
            std::pair{"FLAG_WINDOW_MINIMIZED", FLAG_WINDOW_MINIMIZED},
            std::pair{"FLAG_WINDOW_MAXIMIZED", FLAG_WINDOW_MAXIMIZED},
            std::pair{"FLAG_WINDOW_UNFOCUSED", FLAG_WINDOW_UNFOCUSED},
            std::pair{"FLAG_WINDOW_TOPMOST", FLAG_WINDOW_TOPMOST},
            std::pair{"FLAG_WINDOW_ALWAYS_RUN", FLAG_WINDOW_ALWAYS_RUN},
            std::pair{"FLAG_WINDOW_TRANSPARENT", FLAG_WINDOW_TRANSPARENT},
            std::pair{"FLAG_WINDOW_HIGHDPI", FLAG_WINDOW_HIGHDPI},
            std::pair{"FLAG_WINDOW_MOUSE_PASSTHROUGH", FLAG_WINDOW_MOUSE_PASSTHROUGH},
            std::pair{"FLAG_BORDERLESS_WINDOWED_MODE", FLAG_BORDERLESS_WINDOWED_MODE},
            std::pair{"FLAG_MSAA_4X_HINT", FLAG_MSAA_4X_HINT},
            std::pair{"FLAG_INTERLACED_HINT", FLAG_INTERLACED_HINT}
        );

        export_object(ctx, m, "Keyboard",
            std::pair{"KEY_NULL", KEY_NULL},
            std::pair{"KEY_APOSTROPHE", KEY_APOSTROPHE},
            std::pair{"KEY_COMMA", KEY_COMMA},
            std::pair{"KEY_MINUS", KEY_MINUS},
            std::pair{"KEY_PERIOD", KEY_PERIOD},
            std::pair{"KEY_SLASH", KEY_SLASH},
            std::pair{"KEY_ZERO", KEY_ZERO},
            std::pair{"KEY_ONE", KEY_ONE},
            std::pair{"KEY_TWO", KEY_TWO},
            std::pair{"KEY_THREE", KEY_THREE},
            std::pair{"KEY_FOUR", KEY_FOUR},
            std::pair{"KEY_FIVE", KEY_FIVE},
            std::pair{"KEY_SIX", KEY_SIX},
            std::pair{"KEY_SEVEN", KEY_SEVEN},
            std::pair{"KEY_EIGHT", KEY_EIGHT},
            std::pair{"KEY_NINE", KEY_NINE},
            std::pair{"KEY_SEMICOLON", KEY_SEMICOLON},
            std::pair{"KEY_EQUAL", KEY_EQUAL},
            std::pair{"KEY_A", KEY_A},
            std::pair{"KEY_B", KEY_B},
            std::pair{"KEY_C", KEY_C},
            std::pair{"KEY_D", KEY_D},
            std::pair{"KEY_E", KEY_E},
            std::pair{"KEY_F", KEY_F},
            std::pair{"KEY_G", KEY_G},
            std::pair{"KEY_H", KEY_H},
            std::pair{"KEY_I", KEY_I},
            std::pair{"KEY_J", KEY_J},
            std::pair{"KEY_K", KEY_K},
            std::pair{"KEY_L", KEY_L},
            std::pair{"KEY_M", KEY_M},
            std::pair{"KEY_N", KEY_N},
            std::pair{"KEY_O", KEY_O},
            std::pair{"KEY_P", KEY_P},
            std::pair{"KEY_Q", KEY_Q},
            std::pair{"KEY_R", KEY_R},
            std::pair{"KEY_S", KEY_S},
            std::pair{"KEY_T", KEY_T},
            std::pair{"KEY_U", KEY_U},
            std::pair{"KEY_V", KEY_V},
            std::pair{"KEY_W", KEY_W},
            std::pair{"KEY_X", KEY_X},
            std::pair{"KEY_Y", KEY_Y},
            std::pair{"KEY_Z", KEY_Z},
            std::pair{"KEY_LEFT_BRACKET", KEY_LEFT_BRACKET},
            std::pair{"KEY_BACKSLASH", KEY_BACKSLASH},
            std::pair{"KEY_RIGHT_BRACKET", KEY_RIGHT_BRACKET},
            std::pair{"KEY_GRAVE", KEY_GRAVE},
            std::pair{"KEY_SPACE", KEY_SPACE},
            std::pair{"KEY_ESCAPE", KEY_ESCAPE},
            std::pair{"KEY_ENTER", KEY_ENTER},
            std::pair{"KEY_TAB", KEY_TAB},
            std::pair{"KEY_BACKSPACE", KEY_BACKSPACE},
            std::pair{"KEY_INSERT", KEY_INSERT},
            std::pair{"KEY_DELETE", KEY_DELETE},
            std::pair{"KEY_RIGHT", KEY_RIGHT},
            std::pair{"KEY_LEFT", KEY_LEFT},
            std::pair{"KEY_DOWN", KEY_DOWN},
            std::pair{"KEY_UP", KEY_UP},
            std::pair{"KEY_PAGE_UP", KEY_PAGE_UP},
            std::pair{"KEY_PAGE_DOWN", KEY_PAGE_DOWN},
            std::pair{"KEY_HOME", KEY_HOME},
            std::pair{"KEY_END", KEY_END},
            std::pair{"KEY_CAPS_LOCK", KEY_CAPS_LOCK},
            std::pair{"KEY_SCROLL_LOCK", KEY_SCROLL_LOCK},
            std::pair{"KEY_NUM_LOCK", KEY_NUM_LOCK},
            std::pair{"KEY_PRINT_SCREEN", KEY_PRINT_SCREEN},
            std::pair{"KEY_PAUSE", KEY_PAUSE},
            std::pair{"KEY_F1", KEY_F1},
            std::pair{"KEY_F2", KEY_F2},
            std::pair{"KEY_F3", KEY_F3},
            std::pair{"KEY_F4", KEY_F4},
            std::pair{"KEY_F5", KEY_F5},
            std::pair{"KEY_F6", KEY_F6},
            std::pair{"KEY_F7", KEY_F7},
            std::pair{"KEY_F8", KEY_F8},
            std::pair{"KEY_F9", KEY_F9},
            std::pair{"KEY_F10", KEY_F10},
            std::pair{"KEY_F11", KEY_F11},
            std::pair{"KEY_F12", KEY_F12},
            std::pair{"KEY_LEFT_SHIFT", KEY_LEFT_SHIFT},
            std::pair{"KEY_LEFT_CONTROL", KEY_LEFT_CONTROL},
            std::pair{"KEY_LEFT_ALT", KEY_LEFT_ALT},
            std::pair{"KEY_LEFT_SUPER", KEY_LEFT_SUPER},
            std::pair{"KEY_RIGHT_SHIFT", KEY_RIGHT_SHIFT},
            std::pair{"KEY_RIGHT_CONTROL", KEY_RIGHT_CONTROL},
            std::pair{"KEY_RIGHT_ALT", KEY_RIGHT_ALT},
            std::pair{"KEY_RIGHT_SUPER", KEY_RIGHT_SUPER},
            std::pair{"KEY_KB_MENU", KEY_KB_MENU},
            std::pair{"KEY_KP_0", KEY_KP_0},
            std::pair{"KEY_KP_1", KEY_KP_1},
            std::pair{"KEY_KP_2", KEY_KP_2},
            std::pair{"KEY_KP_3", KEY_KP_3},
            std::pair{"KEY_KP_4", KEY_KP_4},
            std::pair{"KEY_KP_5", KEY_KP_5},
            std::pair{"KEY_KP_6", KEY_KP_6},
            std::pair{"KEY_KP_7", KEY_KP_7},
            std::pair{"KEY_KP_8", KEY_KP_8},
            std::pair{"KEY_KP_9", KEY_KP_9},
            std::pair{"KEY_KP_DECIMAL", KEY_KP_DECIMAL},
            std::pair{"KEY_KP_DIVIDE", KEY_KP_DIVIDE},
            std::pair{"KEY_KP_MULTIPLY", KEY_KP_MULTIPLY},
            std::pair{"KEY_KP_SUBTRACT", KEY_KP_SUBTRACT},
            std::pair{"KEY_KP_ADD", KEY_KP_ADD},
            std::pair{"KEY_KP_ENTER", KEY_KP_ENTER},
            std::pair{"KEY_KP_EQUAL", KEY_KP_EQUAL},
            std::pair{"KEY_BACK", KEY_BACK},
            std::pair{"KEY_MENU", KEY_MENU},
            std::pair{"KEY_VOLUME_UP", KEY_VOLUME_UP},
            std::pair{"KEY_VOLUME_DOWN", KEY_VOLUME_DOWN}
        );

        // Enum: MouseButton
        export_object(ctx, m, "MouseButton",
            std::pair{"MOUSE_BUTTON_LEFT", MOUSE_BUTTON_LEFT},
            std::pair{"MOUSE_BUTTON_RIGHT", MOUSE_BUTTON_RIGHT},
            std::pair{"MOUSE_BUTTON_MIDDLE", MOUSE_BUTTON_MIDDLE},
            std::pair{"MOUSE_BUTTON_SIDE", MOUSE_BUTTON_SIDE},
            std::pair{"MOUSE_BUTTON_EXTRA", MOUSE_BUTTON_EXTRA},
            std::pair{"MOUSE_BUTTON_FORWARD", MOUSE_BUTTON_FORWARD},
            std::pair{"MOUSE_BUTTON_BACK", MOUSE_BUTTON_BACK}
        );

        // Enum: MouseCursor
        export_object(ctx, m, "MouseCursor",
            std::pair{"MOUSE_CURSOR_DEFAULT", MOUSE_CURSOR_DEFAULT},
            std::pair{"MOUSE_CURSOR_ARROW", MOUSE_CURSOR_ARROW},
            std::pair{"MOUSE_CURSOR_IBEAM", MOUSE_CURSOR_IBEAM},
            std::pair{"MOUSE_CURSOR_CROSSHAIR", MOUSE_CURSOR_CROSSHAIR},
            std::pair{"MOUSE_CURSOR_POINTING_HAND", MOUSE_CURSOR_POINTING_HAND},
            std::pair{"MOUSE_CURSOR_RESIZE_EW", MOUSE_CURSOR_RESIZE_EW},
            std::pair{"MOUSE_CURSOR_RESIZE_NS", MOUSE_CURSOR_RESIZE_NS},
            std::pair{"MOUSE_CURSOR_RESIZE_NWSE", MOUSE_CURSOR_RESIZE_NWSE},
            std::pair{"MOUSE_CURSOR_RESIZE_NESW", MOUSE_CURSOR_RESIZE_NESW},
            std::pair{"MOUSE_CURSOR_RESIZE_ALL", MOUSE_CURSOR_RESIZE_ALL},
            std::pair{"MOUSE_CURSOR_NOT_ALLOWED", MOUSE_CURSOR_NOT_ALLOWED}
        );
}

} // namespace HostApi