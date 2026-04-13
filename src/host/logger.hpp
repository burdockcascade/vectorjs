#pragma once

#include <chrono>
#include <format>
#include <print>
#include <mutex>
#include <string_view>
#include <utility>
#include <raylib.h>

namespace Log {

    // 1. Define our own Log Levels
    enum class Level {
        Trace = 0,
        Debug,
        Info,
        Warn,
        Error,
        Critical,
        Off
    };

    // Global state for the logger
    inline auto current_level = Level::Info;
    inline std::mutex log_mutex;

    // 2. ANSI Color Codes for terminal formatting
    namespace Color {
        constexpr std::string_view Reset    = "\033[0m";
        constexpr std::string_view Trace    = "\033[37m";    // White
        constexpr std::string_view Debug    = "\033[36m";    // Cyan
        constexpr std::string_view Info     = "\033[32m";    // Green
        constexpr std::string_view Warn     = "\033[33m";    // Yellow
        constexpr std::string_view Error    = "\033[31m";    // Red
        constexpr std::string_view Critical = "\033[1;31m";  // Bold Red
    }

    // Initialize the logger
    inline void Init() {
        // Disable Raylib's internal logging, just as before
        SetTraceLogLevel(LOG_NONE);
    }

    // Set the active log level
    inline void SetLevel(const Level level) {
        current_level = level;
    }

    // Helper function to handle the core logging logic
    template<typename... Args>
    void LogMessage(const Level level, std::string_view level_str, std::string_view color, std::format_string<Args...> fmt, Args&&... args) {
        // Don't print if the log level is too low
        if (level < current_level) return;

        // Get the current time truncated to milliseconds
        auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());

        // Format the user's message
        auto msg = std::format(fmt, std::forward<Args>(args)...);

        // Lock the mutex to ensure thread-safe printing to the console
        std::lock_guard lock(log_mutex);

        // Use C++23 std::println to output the final formatted string
        // Format: [Color][Time] [Level] Message [Reset]
        std::println("{}[{:%T}] [{}] {}{}", color, now, level_str, msg, Color::Reset);
    }

    // 3. Public logging methods
    template<typename... Args>
    void Trace(std::format_string<Args...> fmt, Args&&... args) {
        LogMessage(Level::Trace, "trace", Color::Trace, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Debug(std::format_string<Args...> fmt, Args&&... args) {
        LogMessage(Level::Debug, "debug", Color::Debug, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Info(std::format_string<Args...> fmt, Args&&... args) {
        LogMessage(Level::Info, "info", Color::Info, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Warn(std::format_string<Args...> fmt, Args&&... args) {
        LogMessage(Level::Warn, "warn", Color::Warn, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Error(std::format_string<Args...> fmt, Args&&... args) {
        LogMessage(Level::Error, "error", Color::Error, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Critical(std::format_string<Args...> fmt, Args&&... args) {
        LogMessage(Level::Critical, "critical", Color::Critical, fmt, std::forward<Args>(args)...);
    }
}