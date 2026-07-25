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
}

} // namespace HostApi