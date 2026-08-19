#pragma once

#include <raylib.h>
#include <qjspp.hpp>
#include "vjs_color.hpp"
#include "vjs_math.hpp"
#include "vjs_graphics.hpp"

namespace App::Modules {

    struct JSApplication {
        qjspp::Engine& engine;
        JSApplication(qjspp::Engine& engine, int w, int h, const std::string& title);
        [[nodiscard]] qjspp::Value run(const qjspp::ArgList& args) const;
        ~JSApplication();
    };

    struct JSDrawOptions {
        JSColor color{BLACK};
        float rotation = 0.0f;
        bool wireframe = false;
        JSVector2 origin{0.0f, 0.0f};
    };

    struct JSTextOptions {
        JSFont font;
        JSColor color{BLACK};
        float rotation = 0.0f;
        float fontSize = 24.0f;
        float spacing = 1.0f;
        JSVector2 origin{0.0f, 0.0f};
    };

}