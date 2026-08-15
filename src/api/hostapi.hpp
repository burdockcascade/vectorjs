#pragma once
#include "qjspp.hpp"
#include "vectorjs/vectorjs.hpp"

namespace VectorJS {

    inline void make_vectorjs_module(qjspp::Engine& engine) {
        auto m = engine.new_module("vectorjs");
        register_hapi_enums(engine, m);
        register_color_class(engine, m);
        register_palette_object(engine, m);
        register_camera2d(engine, m);
        register_application(engine, m);
        register_text(engine, m);
        register_math(engine, m);
        m.build();
    }
} // namespace HostApi