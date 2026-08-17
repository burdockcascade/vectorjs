#include <array>
#include <qjspp.hpp>

#include "vjs_structs.hpp"
#include "../../info.hpp"

namespace App::Module::VectorJS {

    namespace {
        struct ColorEntry {
            const char* name;
            ::Color color;
        };
    }

    void register_palette_object(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        static constexpr std::array palette = {
            // Grays & Neutrals
            ColorEntry{.name = "WHITE", .color = {.r = 255, .g = 255, .b = 255, .a = 255}},
            ColorEntry{.name = "SNOW", .color = {.r = 255, .g = 250, .b = 250, .a = 255}},
            ColorEntry{.name = "IVORY", .color = {.r = 255, .g = 255, .b = 240, .a = 255}},
            ColorEntry{.name = "RAYWHITE", .color = {.r = 245, .g = 245, .b = 245, .a = 255}},
            ColorEntry{.name = "OFFWHITE", .color = {.r = 238, .g = 238, .b = 228, .a = 255}},
            ColorEntry{.name = "GAINSBORO", .color = {.r = 220, .g = 220, .b = 220, .a = 255}},
            ColorEntry{.name = "LIGHTGRAY", .color = {.r = 200, .g = 200, .b = 200, .a = 255}},
            ColorEntry{.name = "SILVER", .color = {.r = 192, .g = 192, .b = 192, .a = 255}},
            ColorEntry{.name = "GRAY", .color = {.r = 130, .g = 130, .b = 130, .a = 255}},
            ColorEntry{.name = "SLATE", .color = {.r = 112, .g = 128, .b = 144, .a = 255}},
            ColorEntry{.name = "DARKGRAY", .color = {.r = 80, .g = 80, .b = 80, .a = 255}},
            ColorEntry{.name = "CHARBLACK", .color = {.r = 30, .g = 30, .b = 30, .a = 255}},
            ColorEntry{.name = "BLACK", .color = {.r = 0, .g = 0, .b = 0, .a = 255}},
            ColorEntry{.name = "BLANK", .color = {.r = 0, .g = 0, .b = 0, .a = 0}},

            // Reds, Pinks & Oranges
            ColorEntry{.name = "SALMON", .color = {.r = 250, .g = 128, .b = 114, .a = 255}},
            ColorEntry{.name = "CORAL", .color = {.r = 255, .g = 127, .b = 80, .a = 255}},
            ColorEntry{.name = "TOMATO", .color = {.r = 255, .g = 99, .b = 71, .a = 255}},
            ColorEntry{.name = "CRIMSON", .color = {.r = 180, .g = 10, .b = 30, .a = 255}},
            ColorEntry{.name = "RED", .color = {.r = 230, .g = 41, .b = 55, .a = 255}},
            ColorEntry{.name = "PURE_RED", .color = {.r = 255, .g = 0, .b = 0, .a = 255}},
            ColorEntry{.name = "SCARLET", .color = {.r = 255, .g = 36, .b = 0, .a = 255}},
            ColorEntry{.name = "MAROON", .color = {.r = 190, .g = 33, .b = 55, .a = 255}},
            ColorEntry{.name = "BURGUNDY", .color = {.r = 128, .g = 0, .b = 32, .a = 255}},
            ColorEntry{.name = "PEACH", .color = {.r = 255, .g = 218, .b = 185, .a = 255}},
            ColorEntry{.name = "ORANGE", .color = {.r = 255, .g = 161, .b = 0, .a = 255}},
            ColorEntry{.name = "DARKORANGE", .color = {.r = 255, .g = 140, .b = 0, .a = 255}},
            ColorEntry{.name = "AMBER", .color = {.r = 255, .g = 191, .b = 0, .a = 255}},
            ColorEntry{.name = "GOLD", .color = {.r = 255, .g = 203, .b = 0, .a = 255}},
            ColorEntry{.name = "YELLOW", .color = {.r = 253, .g = 249, .b = 0, .a = 255}},
            ColorEntry{.name = "LEMON", .color = {.r = 255, .g = 247, .b = 0, .a = 255}},
            ColorEntry{.name = "ROSE", .color = {.r = 255, .g = 192, .b = 203, .a = 255}},
            ColorEntry{.name = "PINK", .color = {.r = 255, .g = 109, .b = 194, .a = 255}},
            ColorEntry{.name = "HOTPINK", .color = {.r = 255, .g = 20, .b = 147, .a = 255}},
            ColorEntry{.name = "DEEPPINK", .color = {.r = 255, .g = 20, .b = 147, .a = 255}},
            ColorEntry{.name = "MAGENTA", .color = {.r = 255, .g = 0, .b = 255, .a = 255}},
            ColorEntry{.name = "FUCHSIA", .color = {.r = 224, .g = 17, .b = 95, .a = 255}},

            // Greens & Teals
            ColorEntry{.name = "MINT", .color = {.r = 152, .g = 251, .b = 152, .a = 255}},
            ColorEntry{.name = "PASTELGREEN", .color = {.r = 119, .g = 221, .b = 119, .a = 255}},
            ColorEntry{.name = "LIME", .color = {.r = 0, .g = 158, .b = 47, .a = 255}},
            ColorEntry{.name = "LIMEGREEN", .color = {.r = 50, .g = 205, .b = 50, .a = 255}},
            ColorEntry{.name = "NEON_GREEN", .color = {.r = 57, .g = 255, .b = 20, .a = 255}},
            ColorEntry{.name = "GREEN", .color = {.r = 0, .g = 228, .b = 48, .a = 255}},
            ColorEntry{.name = "PURE_GREEN", .color = {.r = 0, .g = 255, .b = 0, .a = 255}},
            ColorEntry{.name = "EMERALD", .color = {.r = 80, .g = 200, .b = 120, .a = 255}},
            ColorEntry{.name = "DARKGREEN", .color = {.r = 0, .g = 117, .b = 44, .a = 255}},
            ColorEntry{.name = "FOREST", .color = {.r = 34, .g = 139, .b = 34, .a = 255}},
            ColorEntry{.name = "PINE", .color = {.r = 1, .g = 121, .b = 111, .a = 255}},
            ColorEntry{.name = "OLIVE", .color = {.r = 128, .g = 128, .b = 0, .a = 255}},
            ColorEntry{.name = "TURQUOISE", .color = {.r = 64, .g = 224, .b = 208, .a = 255}},
            ColorEntry{.name = "TEAL", .color = {.r = 0, .g = 128, .b = 128, .a = 255}},
            ColorEntry{.name = "SEA_GREEN", .color = {.r = 46, .g = 139, .b = 87, .a = 255}},

            // Blues & Cyans
            ColorEntry{.name = "ELECTRIC_BLUE", .color = {.r = 125, .g = 249, .b = 255, .a = 255}},
            ColorEntry{.name = "CYAN", .color = {.r = 0, .g = 255, .b = 255, .a = 255}},
            ColorEntry{.name = "AQUA", .color = {.r = 0, .g = 255, .b = 255, .a = 255}},
            ColorEntry{.name = "PASTELBLUE", .color = {.r = 174, .g = 198, .b = 207, .a = 255}},
            ColorEntry{.name = "SKYBLUE", .color = {.r = 102, .g = 191, .b = 255, .a = 255}},
            ColorEntry{.name = "DEEPSKYBLUE", .color = {.r = 0, .g = 191, .b = 255, .a = 255}},
            ColorEntry{.name = "CORNFLOWER", .color = {.r = 100, .g = 149, .b = 237, .a = 255}},
            ColorEntry{.name = "BLUE", .color = {.r = 0, .g = 121, .b = 241, .a = 255}},
            ColorEntry{.name = "PURE_BLUE", .color = {.r = 0, .g = 0, .b = 255, .a = 255}},
            ColorEntry{.name = "ROYALBLUE", .color = {.r = 65, .g = 105, .b = 225, .a = 255}},
            ColorEntry{.name = "DARKBLUE", .color = {.r = 0, .g = 82, .b = 172, .a = 255}},
            ColorEntry{.name = "NAVY", .color = {.r = 0, .g = 0, .b = 128, .a = 255}},
            ColorEntry{.name = "MIDNIGHTBLUE", .color = {.r = 25, .g = 25, .b = 112, .a = 255}},

            // Purples & Violets
            ColorEntry{.name = "LAVENDER", .color = {.r = 230, .g = 230, .b = 250, .a = 255}},
            ColorEntry{.name = "LILAC", .color = {.r = 200, .g = 162, .b = 200, .a = 255}},
            ColorEntry{.name = "PURPLE", .color = {.r = 200, .g = 122, .b = 255, .a = 255}},
            ColorEntry{.name = "PURE_PURPLE", .color = {.r = 128, .g = 0, .b = 128, .a = 255}},
            ColorEntry{.name = "AMETHYST", .color = {.r = 153, .g = 102, .b = 204, .a = 255}},
            ColorEntry{.name = "VIOLET", .color = {.r = 135, .g = 60, .b = 190, .a = 255}},
            ColorEntry{.name = "INDIGO", .color = {.r = 75, .g = 0, .b = 130, .a = 255}},
            ColorEntry{.name = "DARKPURPLE", .color = {.r = 112, .g = 31, .b = 126, .a = 255}},
            ColorEntry{.name = "PLUM", .color = {.r = 142, .g = 69, .b = 133, .a = 255}},
            ColorEntry{.name = "ORCHID", .color = {.r = 218, .g = 112, .b = 214, .a = 255}},

            // Browns & Earth Tones
            ColorEntry{.name = "WHEAT", .color = {.r = 245, .g = 222, .b = 179, .a = 255}},
            ColorEntry{.name = "BEIGE", .color = {.r = 211, .g = 176, .b = 131, .a = 255}},
            ColorEntry{.name = "TAN", .color = {.r = 210, .g = 180, .b = 140, .a = 255}},
            ColorEntry{.name = "SAND", .color = {.r = 194, .g = 178, .b = 128, .a = 255}},
            ColorEntry{.name = "KHAKI", .color = {.r = 189, .g = 183, .b = 107, .a = 255}},
            ColorEntry{.name = "TERRACOTTA", .color = {.r = 226, .g = 114, .b = 91, .a = 255}},
            ColorEntry{.name = "BRONZE", .color = {.r = 205, .g = 127, .b = 50, .a = 255}},
            ColorEntry{.name = "COPPER", .color = {.r = 184, .g = 115, .b = 51, .a = 255}},
            ColorEntry{.name = "RUST", .color = {.r = 183, .g = 65, .b = 14, .a = 255}},
            ColorEntry{.name = "BROWN", .color = {.r = 127, .g = 106, .b = 91, .a = 255}},
            ColorEntry{.name = "SADDLEBROWN", .color = {.r = 139, .g = 69, .b = 19, .a = 255}},
            ColorEntry{.name = "DARKBROWN", .color = {.r = 76, .g = 63, .b = 47, .a = 255}},
            ColorEntry{.name = "COFFEE", .color = {.r = 111, .g = 78, .b = 55, .a = 255}},
            ColorEntry{.name = "CHOCOLATE", .color = {.r = 210, .g = 105, .b = 30, .a = 255}}
        };

        qjspp::Value obj = qjspp::Value::make_object(engine.context());

        for (const auto& [name, color] : palette) {
            auto color_ptr = std::make_unique<JSColor>(color);
            qjspp::Value color_val = qjspp::make_native_object<JSColor>(engine.context(), std::move(color_ptr));
            obj.set(name, color_val);
        }

        builder.export_value("Palette", std::move(obj));
    }

} // namespace VectorJS