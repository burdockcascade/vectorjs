#pragma once

#include <filesystem>
#include <expected>
#include <string>
#include <string_view>
#include <vector>
#include <span>
#include <cstddef>
#include <system_error>
#include <qjspp.hpp>

namespace App::Modules {

    enum class FileError {
        NotFound,
        AccessDenied,
        AlreadyExists,
        ReadError,
        WriteError,
        Unknown
    };

    class JSFile {
    public:
        using ResultBool = std::expected<bool, FileError>;
        using ResultString = std::expected<std::string, FileError>;
        using ResultBytes = std::expected<std::vector<std::byte>, FileError>;

        JSFile() = default;
        explicit JSFile(std::filesystem::path path);

        // Core C++23 File I/O Operations
        [[nodiscard]] static ResultString read_text(const std::filesystem::path& path);
        [[nodiscard]] static ResultBytes read_bytes(const std::filesystem::path& path);

        [[nodiscard]] static ResultBool write_text(const std::filesystem::path& path, std::string_view content);
        [[nodiscard]] static ResultBool write_bytes(const std::filesystem::path& path, std::span<const std::byte> data);

        [[nodiscard]] static ResultBool append_text(const std::filesystem::path& path, std::string_view content);
        [[nodiscard]] static bool exists(const std::filesystem::path& path) noexcept;
        [[nodiscard]] static ResultBool remove(const std::filesystem::path& path) noexcept;

        // Path utility getters
        [[nodiscard]] const std::filesystem::path& get_path() const noexcept { return m_path; }
        void set_path(std::filesystem::path path) { m_path = std::move(path); }

    private:
        std::filesystem::path m_path;
        static FileError map_error_code(const std::error_code& ec) noexcept;
    };

    // Registration function to expose JSFile to QuickJS via qjspp
    void register_file_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder);

} // namespace App::Modules