#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <qjspp.hpp>

#include "vjs_structs.hpp"

namespace App::Module::VectorJS {

    void register_vector2(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto vector2 = engine.make_class<JSVector2>("Vector2");

        vector2.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSVector2> {
            if (args.size() < 2) return nullptr;
            return std::make_unique<JSVector2>(static_cast<float>(args[0].to_double()), static_cast<float>(args[1].to_double()));
        });

        vector2.property("x",
            [](JSContext* ctx, JSVector2* self) { return qjspp::Value::make_double(ctx, self->x); },
            [](JSVector2* self, const qjspp::Value& val) { self->x = static_cast<float>(val.to_double()); }
        );

        vector2.property("y",
            [](JSContext* ctx, JSVector2* self) { return qjspp::Value::make_double(ctx, self->y); },
            [](JSVector2* self, const qjspp::Value& val) { self->y = static_cast<float>(val.to_double()); }
        );

        vector2.instance_method("add", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            auto* other = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!self || !other) return {};
            Vector2 res = Vector2Add(*self, *other);
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSVector2>(res));
        });

        vector2.instance_method("scale", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            Vector2 res = Vector2Scale(*self, static_cast<float>(args[0].to_double()));
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSVector2>(res));
        });

        vector2.instance_method("length", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self) return {};
            return qjspp::Value::make_double(args[0].context(), Vector2Length(*self));
        });

        vector2.instance_method("normalize", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self) return {};
            Vector2 res = Vector2Normalize(*self);
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSVector2>(res));
        });

        vector2.instance_method("subtract", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            auto* other = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!self || !other) return {};
            Vector2 res = Vector2Subtract(*self, *other);
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSVector2>(res));
        });

        vector2.instance_method("multiply", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            auto* other = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!self || !other) return {};
            Vector2 res = Vector2Multiply(*self, *other);
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSVector2>(res));
        });

        vector2.instance_method("dot", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            auto* other = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!self || !other) return {};
            return qjspp::Value::make_double(args[0].context(), Vector2DotProduct(*self, *other));
        });

        vector2.instance_method("distance", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            auto* other = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!self || !other) return {};
            return qjspp::Value::make_double(args[0].context(), Vector2Distance(*self, *other));
        });

        vector2.instance_method("negate", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self) return {};
            Vector2 res = Vector2Negate(*self);
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSVector2>(res));
        });

        vector2.instance_method("lerp", [](JSVector2* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (args.size() < 2) return {};
            auto* target = qjspp::get_native_opaque<JSVector2>(args[0]);
            if (!self || !target) return {};
            Vector2 res = Vector2Lerp(*self, *target, static_cast<float>(args[1].to_double()));
            return qjspp::make_native_object(args[0].context(), std::make_unique<JSVector2>(res));
        });

        builder.export_class("Vector2", vector2.build());
    }

} // namespace VectorJS