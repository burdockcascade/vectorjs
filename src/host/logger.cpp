#include "logger.hpp"
#include <raylib.h>
#include <spdlog/sinks/stdout_color_sinks.h>

void Log::Init() {
    // Create and register the logger as the default global logger
    auto logger = spdlog::stdout_color_mt("APP");
    spdlog::set_default_logger(logger);

    spdlog::set_pattern("%^[%T.%e] [%l] %v%$");
    spdlog::set_level(spdlog::level::info); // Set initial level
    spdlog::flush_on(spdlog::level::err);

    SetTraceLogLevel(LOG_NONE);
}

void Log::SetLevel(spdlog::level::level_enum level) {
    spdlog::set_level(level);
}