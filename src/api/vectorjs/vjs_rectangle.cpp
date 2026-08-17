#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <qjspp.hpp>

#include "vjs_structs.hpp"

namespace App::Module::VectorJS {

    void register_rectangle(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto rectangle = engine.make_class<JSRectangle>("Rectangle");

        rectangle.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSRectangle> {
            if (args.size() < 4) return nullptr;
            return std::make_unique<JSRectangle>(
                static_cast<float>(args[0].to_double()),
                static_cast<float>(args[1].to_double()),
                static_cast<float>(args[2].to_double()),
                static_cast<float>(args[3].to_double())
            );
        });

        rectangle.property("x",
            [](JSContext* ctx, JSRectangle* self) { return qjspp::Value::make_double(ctx, self->x); },
            [](JSRectangle* self, const qjspp::Value& val) { self->x = static_cast<float>(val.to_double()); }
        );

        rectangle.property("y",
            [](JSContext* ctx, JSRectangle* self) { return qjspp::Value::make_double(ctx, self->y); },
            [](JSRectangle* self, const qjspp::Value& val) { self->y = static_cast<float>(val.to_double()); }
        );

        rectangle.property("width",
            [](JSContext* ctx, JSRectangle* self) { return qjspp::Value::make_double(ctx, self->width); },
            [](JSRectangle* self, const qjspp::Value& val) { self->width = static_cast<float>(val.to_double()); }
        );

        rectangle.property("height",
            [](JSContext* ctx, JSRectangle* self) { return qjspp::Value::make_double(ctx, self->height); },
            [](JSRectangle* self, const qjspp::Value& val) { self->height = static_cast<float>(val.to_double()); }
        );

        builder.export_class("Rectangle", rectangle.build());
    }

} // namespace VectorJS