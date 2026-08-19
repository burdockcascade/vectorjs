#include "vjs_file.hpp"
#include <fstream>

namespace App::Modules {

    JSFile::JSFile(std::filesystem::path path)
        : m_path(std::move(path)) {}

    FileError JSFile::map_error_code(const std::error_code& ec) noexcept {
        if (ec == std::errc::no_such_file_or_directory) return FileError::NotFound;
        if (ec == std::errc::permission_denied) return FileError::AccessDenied;
        if (ec == std::errc::file_exists) return FileError::AlreadyExists;
        if (ec == std::errc::io_error) return FileError::ReadError;
        return FileError::Unknown;
    }

    JSFile::ResultString JSFile::read_text(const std::filesystem::path& path) {
        std::error_code ec;
        if (!std::filesystem::exists(path, ec)) {
            return std::unexpected(FileError::NotFound);
        }

        std::ifstream file(path, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            return std::unexpected(FileError::AccessDenied);
        }

        const auto size = std::filesystem::file_size(path, ec);
        if (ec) return std::unexpected(map_error_code(ec));

        std::string content(size, '\0');
        if (!file.read(content.data(), static_cast<std::streamsize>(size))) {
            return std::unexpected(FileError::ReadError);
        }

        return content;
    }

    JSFile::ResultBytes JSFile::read_bytes(const std::filesystem::path& path) {
        std::error_code ec;
        if (!std::filesystem::exists(path, ec)) {
            return std::unexpected(FileError::NotFound);
        }

        std::ifstream file(path, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            return std::unexpected(FileError::AccessDenied);
        }

        const auto size = std::filesystem::file_size(path, ec);
        if (ec) return std::unexpected(map_error_code(ec));

        std::vector<std::byte> buffer(size);
        if (!file.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(size))) {
            return std::unexpected(FileError::ReadError);
        }

        return buffer;
    }

    JSFile::ResultBool JSFile::write_text(const std::filesystem::path& path, std::string_view content) {
        std::ofstream file(path, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!file.is_open()) {
            return std::unexpected(FileError::AccessDenied);
        }

        if (!file.write(content.data(), static_cast<std::streamsize>(content.size()))) {
            return std::unexpected(FileError::WriteError);
        }

        return true;
    }

    JSFile::ResultBool JSFile::write_bytes(const std::filesystem::path& path, std::span<const std::byte> data) {
        std::ofstream file(path, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!file.is_open()) {
            return std::unexpected(FileError::AccessDenied);
        }

        if (!file.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()))) {
            return std::unexpected(FileError::WriteError);
        }

        return true;
    }

    JSFile::ResultBool JSFile::append_text(const std::filesystem::path& path, std::string_view content) {
        std::ofstream file(path, std::ios::out | std::ios::binary | std::ios::app);
        if (!file.is_open()) {
            return std::unexpected(FileError::AccessDenied);
        }

        if (!file.write(content.data(), static_cast<std::streamsize>(content.size()))) {
            return std::unexpected(FileError::WriteError);
        }

        return true;
    }

    bool JSFile::exists(const std::filesystem::path& path) noexcept {
        std::error_code ec;
        return std::filesystem::exists(path, ec);
    }

    JSFile::ResultBool JSFile::remove(const std::filesystem::path& path) noexcept {
        std::error_code ec;
        bool removed = std::filesystem::remove(path, ec);
        if (ec) {
            return std::unexpected(map_error_code(ec));
        }
        return removed;
    }

    // ===========================
    // QuickJS Binding Integration
    // ===========================

    void register_file_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto cls = engine.make_class<JSFile>("File");

        cls.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSFile> {
            if (args.empty()) return std::make_unique<JSFile>();
            return std::make_unique<JSFile>(args[0].to_string());
        });

        // Instance Methods
        cls.instance_method("readText", [&engine](const JSFile* self, const qjspp::ArgList&) -> qjspp::Value {
            if (!self) return {};
            auto res = JSFile::read_text(self->get_path());
            if (!res) throw std::runtime_error("Failed to read file");
            return engine.make_string(*res);
        });

        cls.instance_method("writeText", [&engine](JSFile* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            auto res = JSFile::write_text(self->get_path(), args[0].to_string());
            if (!res) throw std::runtime_error("Failed to write to file");
            return engine.make_bool(true);
        });

        cls.instance_method("appendText", [&engine](JSFile* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (!self || args.empty()) return {};
            auto res = JSFile::append_text(self->get_path(), args[0].to_string());
            if (!res) throw std::runtime_error("Failed to append to file");
            return engine.make_bool(true);
        });

        cls.instance_method("exists", [&engine](const JSFile* self, const qjspp::ArgList&) -> qjspp::Value {
            if (!self) return engine.make_bool(false);
            return engine.make_bool(JSFile::exists(self->get_path()));
        });

        cls.instance_method("remove", [&engine](const JSFile* self, const qjspp::ArgList&) -> qjspp::Value {
            if (!self) return engine.make_bool(false);
            auto res = JSFile::remove(self->get_path());
            return engine.make_bool(res.value_or(false));
        });

        // Static Methods
        cls.static_method("readText", [&engine](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.empty()) return {};
            auto res = JSFile::read_text(args[0].to_string());
            if (!res) throw std::runtime_error("Failed to read file");
            return engine.make_string(*res);
        });

        cls.static_method("writeText", [&engine](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.size() < 2) return engine.make_undefined();
            auto res = JSFile::write_text(args[0].to_string(), args[1].to_string());
            if (!res) throw std::runtime_error("Failed to write to file");
            return engine.make_bool(true);
        });

        cls.static_method("exists", [&engine](const qjspp::ArgList& args) -> qjspp::Value {
            if (args.empty()) return engine.make_bool(false);
            return engine.make_bool(JSFile::exists(args[0].to_string()));
        });

        builder.export_class("File", cls.build());
    }

} // namespace App::Modules