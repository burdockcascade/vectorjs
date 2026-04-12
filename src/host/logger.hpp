#pragma once
#include <spdlog/spdlog.h>
#include <utility>

namespace Log {
    void Init();
    void SetLevel(spdlog::level::level_enum level);

    // Variadic templates to replace macros
    template<typename... Args>
    void Trace(spdlog::format_string_t<Args...> fmt, Args&&... args) {
        spdlog::trace(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Debug(spdlog::format_string_t<Args...> fmt, Args&&... args) {
        spdlog::debug(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Info(spdlog::format_string_t<Args...> fmt, Args&&... args) {
        spdlog::info(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Warn(spdlog::format_string_t<Args...> fmt, Args&&... args) {
        spdlog::warn(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Error(spdlog::format_string_t<Args...> fmt, Args&&... args) {
        spdlog::error(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Critical(spdlog::format_string_t<Args...> fmt, Args&&... args) {
        spdlog::critical(fmt, std::forward<Args>(args)...);
    }
}