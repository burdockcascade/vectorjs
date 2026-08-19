#pragma once
#include <qjspp.hpp>

namespace App::Modules {

    void register_vjs_enums(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_color_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_palette_object(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_camera2d(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_application_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_font_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_sound_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_music_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);

    // Math
    void register_vector2(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_rectangle(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_circle(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_triangle(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);
    void register_polygon(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);

    void register_file_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);

    inline void register_vectorjs_module(qjspp::Engine& engine) {
        auto m = engine.new_module("vectorjs");

        register_application_class(engine, m);

        // Audio
        register_sound_class(engine, m);
        register_music_class(engine, m);

        // Color
        register_color_class(engine, m);
        register_palette_object(engine, m);

        // Graphics
        register_camera2d(engine, m);
        register_font_class(engine, m);

        // Math
        register_vector2(engine, m);

        // Geometry
        register_rectangle(engine, m);
        register_circle(engine, m);
        register_triangle(engine, m);
        register_polygon(engine, m);

        // Input
        register_vjs_enums(engine, m);

        register_file_class(engine, m);

        m.finalize();
    }

}