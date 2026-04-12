#pragma once

#include <spdlog/spdlog.h>
#include <utility>
#include <raylib.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Log {

    inline void Init() {
        const auto logger = spdlog::stdout_color_mt("APP");
        spdlog::set_default_logger(logger);

        spdlog::set_pattern("%^[%T.%e] [%l] %v%$");
        spdlog::set_level(spdlog::level::info); // Set initial level
        spdlog::flush_on(spdlog::level::err);

        SetTraceLogLevel(LOG_NONE);
    }

    inline void SetLevel(const spdlog::level::level_enum level) {
        spdlog::set_level(level);
    }

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