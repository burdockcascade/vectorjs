#pragma once
#include <qjspp.hpp>

namespace App::Modules {

    void register_vjs_enums(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_color_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_palette_object(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_camera2d(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_application_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_font_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_vector2(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_rectangle(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_sound_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_music_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);

    inline void register_vectorjs_module(qjspp::Engine& engine) {
        auto m = engine.new_module("vectorjs");
        register_vjs_enums(engine, m);
        register_color_class(engine, m);
        register_palette_object(engine, m);
        register_camera2d(engine, m);
        register_application_class(engine, m);
        register_font_class(engine, m);
        register_vector2(engine, m);
        register_rectangle(engine, m);
        register_sound_class(engine, m);
        register_music_class(engine, m);
        m.finalize();
    }

}