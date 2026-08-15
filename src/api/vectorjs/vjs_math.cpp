#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <qjspp.hpp>

#include "../hostapi.hpp"
#include "js_types.hpp"

namespace VectorJS {

    static void register_vector2_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto cls = engine.make_class<JSVector2>("Vector2");

        cls.constructor([](const std::vector<qjspp::Value>& args) -> std::unique_ptr<JSVector2> {
            if (args.size() < 2) return nullptr;
            return std::make_unique<JSVector2>(static_cast<float>(args[0].to_double()), static_cast<float>(args[1].to_double()));
        });

        cls.property("x",
            [](JSContext* ctx, JSVector2* self) { return qjspp::Value::make_double(ctx, self->x); },
            [](JSVector2* self, const qjspp::Value& val) { self->x = static_cast<float>(val.to_double()); }
        );

        cls.property("y",
            [](JSContext* ctx, JSVector2* self) { return qjspp::Value::make_double(ctx, self->y); },
            [](JSVector2* self, const qjspp::Value& val) { self->y = static_cast<float>(val.to_double()); }
        );

        cls.instance_method("add", [](JSVector2* self, const std::vector<qjspp::Value>& args) -> qjspp::Value {
            auto* other = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!self || !other) return {};
            Vector2 res = Vector2Add(*self, *other);
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSVector2>(res));
        });

        cls.instance_method("scale", [](JSVector2* self, const std::vector<qjspp::Value>& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            Vector2 res = Vector2Scale(*self, static_cast<float>(args[0].to_double()));
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSVector2>(res));
        });

        cls.instance_method("length", [](JSVector2* self, const std::vector<qjspp::Value>& args) -> qjspp::Value {
            if (!self) return {};
            return qjspp::Value::make_double(args[0].context(), Vector2Length(*self));
        });

        cls.instance_method("normalize", [](JSVector2* self, const std::vector<qjspp::Value>& args) -> qjspp::Value {
            if (!self) return {};
            Vector2 res = Vector2Normalize(*self);
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSVector2>(res));
        });

        cls.instance_method("subtract", [](JSVector2* self, const std::vector<qjspp::Value>& args) -> qjspp::Value {
            auto* other = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!self || !other) return {};
            Vector2 res = Vector2Subtract(*self, *other);
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSVector2>(res));
        });

        cls.instance_method("multiply", [](JSVector2* self, const std::vector<qjspp::Value>& args) -> qjspp::Value {
            auto* other = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!self || !other) return {};
            Vector2 res = Vector2Multiply(*self, *other);
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSVector2>(res));
        });

        cls.instance_method("dot", [](JSVector2* self, const std::vector<qjspp::Value>& args) -> qjspp::Value {
            auto* other = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!self || !other) return {};
            return qjspp::Value::make_double(args[0].context(), Vector2DotProduct(*self, *other));
        });

        cls.instance_method("distance", [](JSVector2* self, const std::vector<qjspp::Value>& args) -> qjspp::Value {
            auto* other = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!self || !other) return {};
            return qjspp::Value::make_double(args[0].context(), Vector2Distance(*self, *other));
        });

        cls.instance_method("negate", [](JSVector2* self, const std::vector<qjspp::Value>& args) -> qjspp::Value {
            if (!self) return {};
            Vector2 res = Vector2Negate(*self);
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSVector2>(res));
        });

        cls.instance_method("lerp", [](JSVector2* self, const std::vector<qjspp::Value>& args) -> qjspp::Value {
            if (args.size() < 2) return {};
            auto* target = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!self || !target) return {};
            Vector2 res = Vector2Lerp(*self, *target, static_cast<float>(args[1].to_double()));
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSVector2>(res));
        });

        builder.export_class("Vector2", cls.build());
    }

    static void register_rectangle_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto cls = engine.make_class<JSRectangle>("Rectangle");

        cls.constructor([](const std::vector<qjspp::Value>& args) -> std::unique_ptr<JSRectangle> {
            if (args.size() < 4) return nullptr;
            return std::make_unique<JSRectangle>(
                static_cast<float>(args[0].to_double()),
                static_cast<float>(args[1].to_double()),
                static_cast<float>(args[2].to_double()),
                static_cast<float>(args[3].to_double())
            );
        });

        cls.property("x",
            [](JSContext* ctx, JSRectangle* self) { return qjspp::Value::make_double(ctx, self->x); },
            [](JSRectangle* self, const qjspp::Value& val) { self->x = static_cast<float>(val.to_double()); }
        );
        cls.property("y",
            [](JSContext* ctx, JSRectangle* self) { return qjspp::Value::make_double(ctx, self->y); },
            [](JSRectangle* self, const qjspp::Value& val) { self->y = static_cast<float>(val.to_double()); }
        );
        cls.property("width",
            [](JSContext* ctx, JSRectangle* self) { return qjspp::Value::make_double(ctx, self->width); },
            [](JSRectangle* self, const qjspp::Value& val) { self->width = static_cast<float>(val.to_double()); }
        );
        cls.property("height",
            [](JSContext* ctx, JSRectangle* self) { return qjspp::Value::make_double(ctx, self->height); },
            [](JSRectangle* self, const qjspp::Value& val) { self->height = static_cast<float>(val.to_double()); }
        );

        builder.export_class("Rectangle", cls.build());
    }

    void register_math(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        register_vector2_class(engine, builder);
        register_rectangle_class(engine, builder);
    }

} // namespace VectorJS