#pragma once
#include <format>
#include <raylib.h>
#include <quickjs.h>

namespace HostApi {
    inline const std::string RAYLIB_VERSION_STR = std::format("{}.{}.{}", RAYLIB_VERSION_MAJOR, RAYLIB_VERSION_MINOR, RAYLIB_VERSION_PATCH);
    inline const std::string QUICKJS_VERSION_STR = std::format("{}.{}.{}", QJS_VERSION_MAJOR, QJS_VERSION_MINOR, QJS_VERSION_PATCH);

    void register_hapi_enums(JSContext* ctx, JSModuleDef* m);
    void register_hapi_classes(JSContext* ctx, JSModuleDef* m);

    inline void make_vectorjs_module(JSContext* ctx) {
        JSModuleDef* m = JS_NewCModule(ctx, "vectorjs", [](JSContext *c, JSModuleDef *m) -> int {
            register_hapi_classes(c, m);
            register_hapi_enums(c, m);
            return 0;
        });

        if (m) {
            JS_AddModuleExport(ctx, m, "Application");
            JS_AddModuleExport(ctx, m, "Color");
            JS_AddModuleExport(ctx, m, "Vector2");
            JS_AddModuleExport(ctx, m, "Rectangle");
            JS_AddModuleExport(ctx, m, "Palette");
            JS_AddModuleExport(ctx, m, "Info");
            JS_AddModuleExport(ctx, m, "ConfigFlags");
        }
    }
} // namespace HostApi