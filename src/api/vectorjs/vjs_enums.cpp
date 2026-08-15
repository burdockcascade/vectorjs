#include <utility>
#include <string_view>
#include <concepts>
#include <qjspp.hpp>

#include "../hostapi.hpp"
#include "js_types.hpp"
#include "../../info.hpp"

namespace VectorJS {

    template <typename T>
    static void set_object_property(qjspp::Engine& engine, qjspp::Value& obj, const char* name, T&& val) {
        if constexpr (std::integral<std::decay_t<T>> || std::is_enum_v<std::decay_t<T>>) {
            obj.set(name, engine.make_int(val));
        } else if constexpr (std::floating_point<std::decay_t<T>>) {
            obj.set(name, engine.make_double(val));
        } else if constexpr (std::convertible_to<T, std::string_view>) {
            obj.set(name, engine.make_string(std::string_view{val}));
        } else if constexpr (std::same_as<std::decay_t<T>, ::Color>) {
            auto color_ptr = std::make_unique<JSColor>(val);
            qjspp::Value color_val = qjspp::make_native_object<JSColor>(engine.context(), std::move(color_ptr));
            obj.set(name, color_val);
        }
    }

    template <typename... Args>
    static void set_object_properties(qjspp::Engine& engine, qjspp::Value& obj, Args&&... entries) {
        (set_object_property(engine, obj, entries.first, std::forward<decltype(entries.second)>(entries.second)), ...);
    }

    template <typename... Pairs>
    static void export_object(qjspp::Engine& engine, qjspp::ModuleBuilder& builder, const char* obj_name, Pairs&&... pairs) {
        qjspp::Value obj = qjspp::Value::make_object(engine.context());
        set_object_properties(engine, obj, std::forward<Pairs>(pairs)...);
        builder.export_value(obj_name, std::move(obj));
    }

    #define BIND_ENUM(val) std::pair{#val, val}

    void register_hapi_enums(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {


        // --- Info Object ---
        export_object(engine, builder, "Info",
            BIND_ENUM(RAYLIB_VERSION_STR),
            BIND_ENUM(QUICKJS_VERSION_STR)
        );

        // --- ConfigFlags Object ---
        export_object(engine, builder, "ConfigFlags",
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
        export_object(engine, builder, "Keyboard",
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
        export_object(engine, builder, "MouseButton",
            BIND_ENUM(MOUSE_BUTTON_LEFT),
            BIND_ENUM(MOUSE_BUTTON_RIGHT),
            BIND_ENUM(MOUSE_BUTTON_MIDDLE),
            BIND_ENUM(MOUSE_BUTTON_SIDE),
            BIND_ENUM(MOUSE_BUTTON_EXTRA),
            BIND_ENUM(MOUSE_BUTTON_FORWARD),
            BIND_ENUM(MOUSE_BUTTON_BACK)
        );

        // --- MouseCursor Object ---
        export_object(engine, builder, "MouseCursor",
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

} // namespace VectorJS