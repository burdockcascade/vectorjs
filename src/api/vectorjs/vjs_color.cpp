#include <qjspp.hpp>

#include "js_types.hpp"
#include "../../info.hpp"

namespace App::Module::VectorJS {

    void register_color_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto color = engine.make_class<JSColor>("Color");

        // Constructor: Takes RGBA integer values (4 arguments) to create a new JSColor instance
        color.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSColor> {
            if (args.size() < 4) return nullptr;
            auto r = static_cast<uint8_t>(args[0].to_int());
            auto g = static_cast<uint8_t>(args[1].to_int());
            auto b = static_cast<uint8_t>(args[2].to_int());
            auto a = static_cast<uint8_t>(args[3].to_int());
            return std::make_unique<JSColor>(r, g, b, a);
        });

        // Property: 'r' (Red channel getter/setter)
        color.property("r",
            [](JSContext* ctx, JSColor* self) { return qjspp::Value::make_int(ctx, self->r); },
            [](JSColor* self, const qjspp::Value& val) { self->r = static_cast<uint8_t>(val.to_int()); }
        );

        // Property: 'g' (Green channel getter/setter)
        color.property("g",
            [](JSContext* ctx, JSColor* self) { return qjspp::Value::make_int(ctx, self->g); },
            [](JSColor* self, const qjspp::Value& val) { self->g = static_cast<uint8_t>(val.to_int()); }
        );

        // Property: 'b' (Blue channel getter/setter)
        color.property("b",
            [](JSContext* ctx, JSColor* self) { return qjspp::Value::make_int(ctx, self->b); },
            [](JSColor* self, const qjspp::Value& val) { self->b = static_cast<uint8_t>(val.to_int()); }
        );

        // Property: 'a' (Alpha channel getter/setter)
        color.property("a",
            [](JSContext* ctx, JSColor* self) { return qjspp::Value::make_int(ctx, self->a); },
            [](JSColor* self, const qjspp::Value& val) { self->a = static_cast<uint8_t>(val.to_int()); }
        );

        // Instance Method: Linearly interpolates (lerp) towards a target color using a float factor
        color.instance_method("lerp", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (args.size() < 2) return {};
            auto* target = qjspp::get_native_opaque<JSColor>(args[0]);
            if (!self || !target) return {};

            const double factor = args[1].to_double();
            Color result = ColorLerp(*self, *target, static_cast<float>(factor));
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSColor>(result));
        });

        // Instance Method: Returns a new color with adjusted opacity (alpha factor)
        color.instance_method("fade", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            const double alpha = args[0].to_double();
            Color result = Fade(*self, static_cast<float>(alpha));
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSColor>(result));
        });

        // Instance Method: Adjusts the brightness of the color by a specified factor
        color.instance_method("brightness", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            const double factor = args[0].to_double();
            Color result = ColorBrightness(*self, static_cast<float>(factor));
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSColor>(result));
        });

        // Instance Method: Converts the color to its packed integer representation
        color.instance_method("toInt", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self) return {};
            return qjspp::Value::make_int(args[0].context(), ColorToInt(*self));
        });

        // Instance Method: Adjusts the contrast of the color by a specified factor
        color.instance_method("contrast", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            Color result = ColorContrast(*self, static_cast<float>(args[0].to_double()));
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSColor>(result));
        });

        // Instance Method: Sets the alpha (transparency) of the color
        color.instance_method("alpha", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            Color result = ColorAlpha(*self, static_cast<float>(args[0].to_double()));
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSColor>(result));
        });

        // Instance Method: Tints the current color with another JSColor instance
        color.instance_method("tint", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            auto* tintColor = qjspp::get_native_opaque<JSColor>(args[0]);
            if (!tintColor) return {};
            Color result = ColorTint(*self, *tintColor);
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSColor>(result));
        });

        // Static Method: Factory function creating a Color object from a hexadecimal integer
        color.static_method("fromHex", [](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.empty()) return {};
            Color result = GetColor(static_cast<unsigned int>(args[0].to_int()));
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSColor>(result));
        });

        // Static Method: Factory function creating a Color object from HSV parameters (hue, saturation, value)
        color.static_method("fromHSV", [](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.size() < 3) return {};
            float h = static_cast<float>(args[0].to_double());
            float s = static_cast<float>(args[1].to_double());
            float v = static_cast<float>(args[2].to_double());
            Color result = ColorFromHSV(h, s, v);
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSColor>(result));
        });

        // Instance Method: Checks equality against another JSColor instance
        color.instance_method("isEqual", [](JSColor* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return qjspp::Value::make_bool(args[0].context(), false);
            auto* other = qjspp::get_native_opaque<JSColor>(args[0]);
            bool equal = other && ColorIsEqual(*self, *other);
            return qjspp::Value::make_bool(args[0].context(), equal);
        });

        builder.export_class("Color", color.build());
    }

} // namespace VectorJS