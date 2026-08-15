#include <memory>
#include <raylib.h>
#include <qjspp.hpp>

#include "js_types.hpp"

namespace VectorJS {

    void register_camera2d(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto cls = engine.make_class<JSCamera2D>("Camera2D");

        cls.constructor([](const std::vector<qjspp::Value>& args) -> std::unique_ptr<JSCamera2D> {
            if (args.size() >= 2) {
                auto* offset = qjspp::get_native_opaque<JSVector2>(args[0]);
                auto* target = qjspp::get_native_opaque<JSVector2>(args[1]);

                Vector2 off = offset ? static_cast<Vector2>(*offset) : Vector2{0, 0};
                Vector2 tgt = target ? static_cast<Vector2>(*target) : Vector2{0, 0};
                float rot = args.size() > 2 ? static_cast<float>(args[2].to_double()) : 0.0f;
                float zoom = args.size() > 3 ? static_cast<float>(args[3].to_double()) : 1.0f;

                return std::make_unique<JSCamera2D>(off, tgt, rot, zoom);
            }
            return std::make_unique<JSCamera2D>();
        });

        cls.property("target",
            [](JSContext* ctx, JSCamera2D* self) {
                return qjspp::make_native_object(ctx, std::make_unique<JSVector2>(self->target));
            },
            [](JSCamera2D* self, const qjspp::Value& val) {
                auto* vec = qjspp::get_native_opaque<JSVector2>(val);
                if (vec) self->target = *vec;
            }
        );

        cls.property("offset",
            [](JSContext* ctx, JSCamera2D* self) {
                return qjspp::make_native_object(ctx, std::make_unique<JSVector2>(self->offset));
            },
            [](JSCamera2D* self, const qjspp::Value& val) {
                auto* vec = qjspp::get_native_opaque<JSVector2>(val);
                if (vec) self->offset = *vec;
            }
        );

        cls.property("rotation",
            [](JSContext* ctx, JSCamera2D* self) { return qjspp::Value::make_double(ctx, self->rotation); },
            [](JSCamera2D* self, const qjspp::Value& val) { self->rotation = static_cast<float>(val.to_double()); }
        );

        cls.property("zoom",
            [](JSContext* ctx, JSCamera2D* self) { return qjspp::Value::make_double(ctx, self->zoom); },
            [](JSCamera2D* self, const qjspp::Value& val) { self->zoom = static_cast<float>(val.to_double()); }
        );

        builder.export_class("Camera2D", cls.build());
    }

}