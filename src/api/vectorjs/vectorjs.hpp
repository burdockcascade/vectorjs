#pragma once
#include <qjspp.hpp>

namespace App::Module::VectorJS {

    void register_hapi_enums(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_color_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_palette_object(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_camera2d(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);

    void register_application(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_text(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_math(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);

    inline void register_module(qjspp::Engine& engine) {
        auto m = engine.new_module("vectorjs");
        register_hapi_enums(engine, m);
        register_color_class(engine, m);
        register_palette_object(engine, m);
        register_camera2d(engine, m);
        register_application(engine, m);
        register_text(engine, m);
        register_math(engine, m);
        m.finalize();
    }
    
} // namespace HostApi