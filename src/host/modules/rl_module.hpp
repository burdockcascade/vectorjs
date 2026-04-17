#pragma once
#include "../qjs_wrapper.hpp"
#include "rl_converters.hpp"

namespace RaylibModule {

    void register_raylib_functions(const qjs::Engine &engine, qjs::Module &module);
    void register_raylib_objects(const qjs::Engine &engine, qjs::Module &module);

    inline void register_raylib_module(qjs::Engine &engine) {

        // Create the module and get a reference to it for further exports
        auto raylib_mod = engine.define_module("Raylib");

        // Initialize all function groups in the module
        register_raylib_functions(engine, raylib_mod);

        // Initialize all enums in the module
        register_raylib_objects(engine, raylib_mod);

    }
}