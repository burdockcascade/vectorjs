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

        auto core = engine.new_module("vectorjs/core");
        register_application_class(engine, core);
        register_file_class(engine, core);
        register_color_class(engine, core);
        register_palette_object(engine, core);
        core.finalize();

        auto audio = engine.new_module("vectorjs/audio");
        register_sound_class(engine, audio);
        register_music_class(engine, audio);
        audio.finalize();

        auto math = engine.new_module("vectorjs/math");
        register_vector2(engine, math);
        math.finalize();

        auto g2d = engine.new_module("vectorjs/graphics2d");
        register_camera2d(engine, g2d);
        register_font_class(engine, g2d);
        g2d.finalize();

        auto input = engine.new_module("vectorjs/input");
        register_vjs_enums(engine, input);
        input.finalize();

        auto geometry = engine.new_module("vectorjs/geometry");
        register_rectangle(engine, geometry);
        register_circle(engine, geometry);
        register_triangle(engine, geometry);
        register_polygon(engine, geometry);
        geometry.finalize();

    }

}