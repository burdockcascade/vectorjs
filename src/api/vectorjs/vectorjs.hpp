#pragma once
#include <qjspp.hpp>

namespace VectorJS {

    void register_hapi_enums(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_color_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_palette_object(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_camera2d(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);

    void register_application(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_text(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_math(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    
} // namespace HostApi