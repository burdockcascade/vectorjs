#include "qjspp.hpp"
#include "js_types.hpp"
#include "../../info.hpp"

namespace VectorJS {

    void register_color_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto cls = engine.make_class<JSColor>("Color");

        cls.constructor([](const std::vector<qjspp::Value>& args) -> std::unique_ptr<JSColor> {
            if (args.size() < 4) return nullptr;
            auto r = static_cast<uint8_t>(args[0].to_int());
            auto g = static_cast<uint8_t>(args[1].to_int());
            auto b = static_cast<uint8_t>(args[2].to_int());
            auto a = static_cast<uint8_t>(args[3].to_int());
            return std::make_unique<JSColor>(r, g, b, a);
        });

        cls.property("r",
            [](JSContext* ctx, JSColor* self) { return qjspp::Value::make_int(ctx, self->r); },
            [](JSColor* self, const qjspp::Value& val) { self->r = static_cast<uint8_t>(val.to_int()); }
        );
        cls.property("g",
            [](JSContext* ctx, JSColor* self) { return qjspp::Value::make_int(ctx, self->g); },
            [](JSColor* self, const qjspp::Value& val) { self->g = static_cast<uint8_t>(val.to_int()); }
        );
        cls.property("b",
            [](JSContext* ctx, JSColor* self) { return qjspp::Value::make_int(ctx, self->b); },
            [](JSColor* self, const qjspp::Value& val) { self->b = static_cast<uint8_t>(val.to_int()); }
        );
        cls.property("a",
            [](JSContext* ctx, JSColor* self) { return qjspp::Value::make_int(ctx, self->a); },
            [](JSColor* self, const qjspp::Value& val) { self->a = static_cast<uint8_t>(val.to_int()); }
        );

        cls.instance_method("lerp", [](JSColor* self, const std::vector<qjspp::Value>& args) -> qjspp::Value {
            if (args.size() < 2) return {};
            auto* target = qjspp::get_native_opaque<JSColor>(args[0]);
            if (!self || !target) return {};

            double factor = args[1].to_double();
            Color result = ColorLerp(*self, *target, static_cast<float>(factor));
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSColor>(result));
        });

        cls.instance_method("fade", [](JSColor* self, const std::vector<qjspp::Value>& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            double alpha = args[0].to_double();
            Color result = ::Fade(*self, static_cast<float>(alpha));
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSColor>(result));
        });

        cls.instance_method("brightness", [](JSColor* self, const std::vector<qjspp::Value>& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            double factor = args[0].to_double();
            Color result = ColorBrightness(*self, static_cast<float>(factor));
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSColor>(result));
        });

        cls.instance_method("toInt", [](JSColor* self, const std::vector<qjspp::Value>& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            return qjspp::Value::make_int(args[0].context(), ColorToInt(*self));
        });

        builder.export_class("Color", cls.build());
    }

} // namespace VectorJS