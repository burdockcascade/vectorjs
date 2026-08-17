#include <memory>
#include <raylib.h>
#include <qjspp.hpp>

#include "vjs_structs.hpp"

namespace App::Module::VectorJS {

    void register_camera2d(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto camera = engine.make_class<JSCamera2D>("Camera2D");

        camera.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSCamera2D> {
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

        // =========================================================================
        // Instance Methods
        // =========================================================================

        // Move target along x (horizontal) and y (vertical) axes
        camera.instance_method("move", [&engine](JSCamera2D* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (args.size() >= 2) {
                self->target.x += static_cast<float>(args[0].to_double());
                self->target.y += static_cast<float>(args[1].to_double());
            } else if (args.size() == 1) {
                if (auto* vec = qjspp::get_native_opaque<JSVector2>(args[0])) {
                    self->target.x += vec->x;
                    self->target.y += vec->y;
                }
            }
            return engine.make_undefined();
        });

        // Horizontal movement: moveX(deltaX)
        camera.instance_method("moveX", [&engine](JSCamera2D* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!args.empty()) {
                self->target.x += static_cast<float>(args[0].to_double());
            }
            return engine.make_undefined();
        });

        // Vertical movement: moveY(deltaY)
        camera.instance_method("moveY", [&engine](JSCamera2D* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!args.empty()) {
                self->target.y += static_cast<float>(args[0].to_double());
            }
            return engine.make_undefined();
        });

        // Relative zoom adjusting: zoomBy(factor)
        camera.instance_method("zoomBy", [&engine](JSCamera2D* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!args.empty()) {
                float factor = static_cast<float>(args[0].to_double());
                self->zoom += factor;
                if (self->zoom < 0.001f) self->zoom = 0.001f;
            }
            return engine.make_undefined();
        });

        // Zoom In helper: zoomIn(amount = 0.1)
        camera.instance_method("zoomIn", [&engine](JSCamera2D* self, const qjspp::ArgList& args) -> qjspp::Value {
            float amount = !args.empty() ? static_cast<float>(args[0].to_double()) : 0.1f;
            self->zoom += amount;
            if (self->zoom < 0.001f) self->zoom = 0.001f;
            return engine.make_undefined();
        });

        // Zoom Out helper: zoomOut(amount = 0.1)
        camera.instance_method("zoomOut", [&engine](JSCamera2D* self, const qjspp::ArgList& args) -> qjspp::Value {
            float amount = !args.empty() ? static_cast<float>(args[0].to_double()) : 0.1f;
            self->zoom -= amount;
            if (self->zoom < 0.001f) self->zoom = 0.001f;
            return engine.make_undefined();
        });

        // =========================================================================
        // Properties
        // =========================================================================

        camera.property("target",
            [](JSContext* ctx, JSCamera2D* self) {
                return qjspp::make_native_object(ctx, std::make_unique<JSVector2>(self->target));
            },
            [](JSCamera2D* self, const qjspp::Value& val) {
                auto* vec = qjspp::get_native_opaque<JSVector2>(val);
                if (vec) self->target = *vec;
            }
        );

        camera.property("offset",
            [](JSContext* ctx, JSCamera2D* self) {
                return qjspp::make_native_object(ctx, std::make_unique<JSVector2>(self->offset));
            },
            [](JSCamera2D* self, const qjspp::Value& val) {
                auto* vec = qjspp::get_native_opaque<JSVector2>(val);
                if (vec) self->offset = *vec;
            }
        );

        camera.property("rotation",
            [](JSContext* ctx, JSCamera2D* self) { return qjspp::Value::make_double(ctx, self->rotation); },
            [](JSCamera2D* self, const qjspp::Value& val) { self->rotation = static_cast<float>(val.to_double()); }
        );

        camera.property("zoom",
            [](JSContext* ctx, JSCamera2D* self) { return qjspp::Value::make_double(ctx, self->zoom); },
            [](JSCamera2D* self, const qjspp::Value& val) {
                auto z = static_cast<float>(val.to_double());
                self->zoom = (z < 0.001f) ? 0.001f : z;
            }
        );

        builder.export_class("Camera2D", camera.build());
    }

}