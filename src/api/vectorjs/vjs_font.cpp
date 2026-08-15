#include <memory>
#include <string>
#include <qjspp.hpp>

#include "js_types.hpp"

namespace VectorJS {

    static void register_font_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto cls = engine.make_class<JSFont>("Font");

        cls.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSFont> {
            if (args.empty()) return nullptr;
            std::string path = args[0].to_string();
            if (args.size() >= 2) {
                return std::make_unique<JSFont>(path, args[1].to_int());
            }
            return std::make_unique<JSFont>(path);
        });

        builder.export_class("Font", cls.build());
    }

    void register_text(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        register_font_class(engine, builder);
    }

} // namespace VectorJS