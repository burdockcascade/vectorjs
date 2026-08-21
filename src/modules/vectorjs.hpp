#pragma once

#include <raylib.h>
#include <qjspp.hpp>
#include <span>

namespace App::Modules {

    template <typename... Pairs>
    static void export_object(qjspp::Engine& engine, qjspp::ModuleBuilder& builder, const char* obj_name, Pairs&&... pairs) {
        qjspp::Value obj = qjspp::Value::make_object(engine.context());

        auto add_prop = [&]<typename Pair>(Pair&& pair) {
            auto&& key = pair.first;
            auto&& val = std::get<1>(pair);
            using T = std::decay_t<decltype(val)>;

            if constexpr (std::integral<T> || std::is_enum_v<T>) {
                obj.set(key, engine.make_int(static_cast<int>(val)));
            } else if constexpr (std::floating_point<T>) {
                obj.set(key, engine.make_double(val));
            } else if constexpr (std::convertible_to<T, std::string_view>) {
                obj.set(key, engine.make_string(std::string_view{val}));
            }
        };

        (add_prop(std::forward<Pairs>(pairs)), ...);

        builder.export_value(obj_name, std::move(obj));
    }

    struct JSFont {
        std::shared_ptr<::Font> font_ptr;

        JSFont() = default;
        explicit JSFont(std::string path, int baseSize = 64) {
            const ::Font f = LoadFontEx(path.data(), baseSize, nullptr, 0);
            if (f.texture.id != 0) {
                SetTextureFilter(f.texture, TEXTURE_FILTER_BILINEAR);
            }

            font_ptr = std::shared_ptr<::Font>(new ::Font(f), [](::Font* pf) {
                if (pf) {
                    if (pf->texture.id != 0) {
                        UnloadFont(*pf);
                    }
                    delete pf;
                }
            });
        }
    };

#pragma region Color

    struct JSColor {
        uint8_t r = 0, g = 0, b = 0, a = 0;

        constexpr JSColor() noexcept = default;
        constexpr JSColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept : r(r), g(g), b(b), a(a) {}
        explicit constexpr JSColor(Color color) noexcept : r(color.r), g(color.g), b(color.b), a(color.a) {}

        [[nodiscard]] constexpr operator Color(this const JSColor& self) noexcept {
            return Color{ .r = self.r, .g = self.g, .b = self.b, .a = self.a };
        }

        [[nodiscard]] JSColor with_lerp(JSColor target, float factor) const noexcept {
            return JSColor(ColorLerp(*this, target, factor));
        }

        JSColor& apply_lerp(JSColor target, float factor) noexcept {
            *this = with_lerp(target, factor);
            return *this;
        }

        [[nodiscard]] JSColor with_fade(float alpha) const noexcept {
            return JSColor(Fade(*this, alpha));
        }

        JSColor& apply_fade(float alpha) noexcept {
            *this = with_fade(alpha);
            return *this;
        }

        [[nodiscard]] JSColor with_brightness(float factor) const noexcept {
            return JSColor(ColorBrightness(*this, factor));
        }

        JSColor& apply_brightness(float factor) noexcept {
            *this = with_brightness(factor);
            return *this;
        }

        [[nodiscard]] JSColor with_contrast(float factor) const noexcept {
            return JSColor(ColorContrast(*this, factor));
        }

        JSColor& apply_contrast(float factor) noexcept {
            *this = with_contrast(factor);
            return *this;
        }

        [[nodiscard]] JSColor with_alpha(float alpha) const noexcept {
            return JSColor(ColorAlpha(*this, alpha));
        }

        JSColor& apply_alpha(float alpha) noexcept {
            *this = with_alpha(alpha);
            return *this;
        }

        [[nodiscard]] JSColor with_tint(JSColor tintColor) const noexcept {
            return JSColor(ColorTint(*this, tintColor));
        }

        JSColor& apply_tint(JSColor tintColor) noexcept {
            *this = with_tint(tintColor);
            return *this;
        }

        [[nodiscard]] int to_int() const noexcept {
            return ColorToInt(*this);
        }

        [[nodiscard]] bool is_equal(JSColor other) const noexcept {
            return ColorIsEqual(*this, other);
        }

    };

#pragma endregion

#pragma region Graphics

    struct JSTexture {
        std::shared_ptr<::Texture2D> texture_ptr;

        JSTexture() = default;
        explicit JSTexture(const std::string& path) {
            const ::Texture2D tex = LoadTexture(path.c_str());
            if (tex.id != 0) {
                SetTextureFilter(tex, TEXTURE_FILTER_BILINEAR);
            }

            texture_ptr = std::shared_ptr<::Texture2D>(new ::Texture2D(tex), [](::Texture2D* pt) {
                if (pt) {
                    if (pt->id != 0) {
                        UnloadTexture(*pt);
                    }
                    delete pt;
                }
            });
        }

        [[nodiscard]] int get_width() const noexcept { return texture_ptr ? texture_ptr->width : 0; }
        [[nodiscard]] int get_height() const noexcept { return texture_ptr ? texture_ptr->height : 0; }
    };

#pragma endregion

#pragma region Camera

    struct JSCamera2D {
        Vector2 offset = { .x = 0.0f, .y = 0.0f };
        Vector2 target = { .x = 0.0f, .y = 0.0f };
        float rotation = 0.0f;
        float zoom = 1.0f;

        constexpr JSCamera2D() noexcept = default;
        constexpr JSCamera2D(Vector2 offset, Vector2 target, float rotation, float zoom) noexcept : offset(offset), target(target), rotation(rotation), zoom(zoom) {}

        explicit constexpr JSCamera2D(::Camera2D c) noexcept : offset(c.offset), target(c.target), rotation(c.rotation), zoom(c.zoom) {}

        [[nodiscard]] constexpr operator ::Camera2D(this JSCamera2D self) noexcept {
            return ::Camera2D{ .offset = self.offset, .target = self.target, .rotation = self.rotation, .zoom = self.zoom };
        }
    };

#pragma endregion

#pragma region Math

    struct JSVector2 {
        float x = 0.0f, y = 0.0f;

        constexpr JSVector2() noexcept = default;
        constexpr JSVector2(float x, float y) noexcept : x(x), y(y) {}
        explicit constexpr JSVector2(Vector2 v) noexcept : x(v.x), y(v.y) {}

        [[nodiscard]] constexpr operator Vector2(this JSVector2 self) noexcept {
            return Vector2{ .x = self.x, .y = self.y };
        }

        JSVector2& add(JSVector2 other) {
            *this = JSVector2(Vector2Add(*this, other));
            return *this;
        }

    };

#pragma endregion

#pragma region Geometry

    struct JSRectangle {
        float x = 0.0f, y = 0.0f, width = 0.0f, height = 0.0f;

        constexpr JSRectangle() noexcept = default;
        constexpr JSRectangle(float x, float y, float width, float height) noexcept : x(x), y(y), width(width), height(height) {}
        explicit constexpr JSRectangle(::Rectangle r) noexcept : x(r.x), y(r.y), width(r.width), height(r.height) {}

        [[nodiscard]] constexpr operator ::Rectangle(this JSRectangle self) noexcept {
            return ::Rectangle{ .x = self.x, .y = self.y, .width = self.width, .height = self.height };
        }
    };

    struct JSCircle {
        Vector2 center;
        float radius;
    };

    struct JSTriangle {
        Vector2 v1;
        Vector2 v2;
        Vector2 v3;
    };

    struct JSPoint {
        Vector2 position;
    };

    struct JSLine {
        Vector2 start;
        Vector2 end;
    };

    struct JSPolygon {
        std::vector<Vector2> points;
    };

#pragma endregion

#pragma region Audio

    struct JSSound {
        std::shared_ptr<::Sound> sound_ptr;
        JSSound() = default;

        explicit JSSound(std::string path) {
            const ::Sound s = LoadSound(path.data());
            sound_ptr = std::shared_ptr<::Sound>(new ::Sound(s), [](::Sound* ps) {
                if (ps) {
                    if (IsAudioDeviceReady()) {
                        UnloadSound(*ps);
                    }
                    delete ps;
                }
            });
        }
    };

    struct JSMusic {
        std::shared_ptr<::Music> music_ptr;

        explicit JSMusic(std::string path) {
            const ::Music m = LoadMusicStream(path.data());
            music_ptr = std::shared_ptr<::Music>(new ::Music(m), [](::Music* pm) {
                if (pm) {
                    if (IsAudioDeviceReady()) {
                        UnloadMusicStream(*pm);
                    }
                    delete pm;
                }
            });
        }
    };

#pragma endregion

#pragma region Application

    struct JSDrawOptions {
        JSColor color{WHITE};
        float rotation = 0.0f;
        bool wireframe = false;
        JSVector2 origin{0.0f, 0.0f};
        JSRectangle source{0.0f, 0.0f, 0.0f, 0.0f};
    };

    struct JSTextOptions {
        JSFont font;
        JSColor color{BLACK};
        float rotation = 0.0f;
        float fontSize = 24.0f;
        float spacing = 1.0f;
        JSVector2 origin{0.0f, 0.0f};
    };

    class JSApplication {
    public:
        qjspp::Engine& engine;
        JSApplication(qjspp::Engine& engine, int w, int h, const std::string& title);
        [[nodiscard]] qjspp::Value run(const qjspp::ArgList& args) const;
        ~JSApplication();
    };

#pragma endregion

#pragma region File

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

#pragma endregion

}
