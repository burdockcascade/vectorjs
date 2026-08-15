#pragma once
#include <format>
#include <raylib.h>
#include <quickjs.h>

namespace VectorJS {
    inline const std::string RAYLIB_VERSION_STR = std::format("{}.{}.{}", RAYLIB_VERSION_MAJOR, RAYLIB_VERSION_MINOR, RAYLIB_VERSION_PATCH);
    inline const std::string QUICKJS_VERSION_STR = std::format("{}.{}.{}", QJS_VERSION_MAJOR, QJS_VERSION_MINOR, QJS_VERSION_PATCH);
}
