#pragma once

#include <string_view>
#include <memory>
#include <raylib.h>
#include <qjspp.hpp>

namespace App::Modules {

    template <typename... Pairs>
    static void export_object(qjspp::Engine& engine, qjspp::ModuleBuilder& builder, const char* obj_name, Pairs&&... pairs) {
        qjspp::Value obj = qjspp::Value::make_object(engine.context());

        auto add_prop = [&]<typename Pair>(Pair&& pair) {
            auto&& key = pair.first;
            auto&& val = std::get<1>(pair);
            using T = std::decay_t<decltype(val)>;

            if constexpr (std::integral<T> || std::is_enum_v<T>) {
                obj.set(key, engine.make_int(static_cast<int>(val)));
            } else if constexpr (std::floating_point<T>) {
                obj.set(key, engine.make_double(val));
            } else if constexpr (std::convertible_to<T, std::string_view>) {
                obj.set(key, engine.make_string(std::string_view{val}));
            }
        };

        (add_prop(std::forward<Pairs>(pairs)), ...);

        builder.export_value(obj_name, std::move(obj));
    }

    struct JSColor {
        uint8_t r = 0, g = 0, b = 0, a = 0;

        constexpr JSColor() noexcept = default;
        constexpr JSColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept : r(r), g(g), b(b), a(a) {}
        explicit constexpr JSColor(Color color) noexcept : r(color.r), g(color.g), b(color.b), a(color.a) {}

        [[nodiscard]] constexpr operator Color(this const JSColor& self) noexcept {
            return Color{ .r = self.r, .g = self.g, .b = self.b, .a = self.a };
        }
    };

}