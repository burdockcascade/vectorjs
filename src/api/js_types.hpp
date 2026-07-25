#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstdint>
#include <raylib.h>

namespace HostApi {

    inline JSClassID js_color_class_id;
    inline JSClassID js_vector2_class_id;
    inline JSClassID js_rectangle_class_id;
    inline JSClassID js_font_class_id;
    inline JSClassID js_image_class_id;
    inline JSClassID js_application_class_id;

    struct JSApplication {
        JSApplication(int h, int w, const std::string& title);
        static JSValue Run(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
    };

    struct JSColor {
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
        uint8_t a = 0;
        JSColor() = default;
        JSColor(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) : r(r), g(g), b(b), a(a) {}
        explicit constexpr JSColor(const Color color) : r(color.r), g(color.g), b(color.b), a(color.a) {}
        [[nodiscard]] constexpr operator Color() const { return Color{ r, g, b, a }; }
    };

    struct JSVector2 {
        float x = 0.0f;
        float y = 0.0f;
        JSVector2() = default;
        JSVector2(const float x, const float y) : x(x), y(y) {}
        explicit constexpr JSVector2(const Vector2 v) : x(v.x), y(v.y) {}
        [[nodiscard]] constexpr operator Vector2() const { return Vector2 { x, y }; }
    };

    struct JSVector3 {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        JSVector3() = default;
        JSVector3(const float x, const float y, const float z = 0.0f) : x(x), y(y), z(z) {}
        explicit constexpr JSVector3(const Vector3 v) : x(v.x), y(v.y), z(v.z) {}
        [[nodiscard]] constexpr operator Vector3() const { return Vector3 { x, y, z }; }
    };

    struct JSVector4 {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;
        JSVector4() = default;
        JSVector4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}
        explicit constexpr JSVector4(const Vector4 v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
        [[nodiscard]] constexpr operator Vector4() const { return Vector4 { x, y, z, w }; }
    };

    struct JSMatrix {
        float m0 = 1.0f, m4 = 0.0f, m8 = 0.0f, m12 = 0.0f;
        float m1 = 0.0f, m5 = 1.0f, m9 = 0.0f, m13 = 0.0f;
        float m2 = 0.0f, m6 = 0.0f, m10 = 1.0f, m14 = 0.0f;
        float m3 = 0.0f, m7 = 0.0f, m11 = 0.0f, m15 = 1.0f;
        JSMatrix() = default;
        explicit constexpr JSMatrix(const Matrix m) :
            m0(m.m0), m4(m.m4), m8(m.m8), m12(m.m12),
            m1(m.m1), m5(m.m5), m9(m.m9), m13(m.m13),
            m2(m.m2), m6(m.m6), m10(m.m10), m14(m.m14),
            m3(m.m3), m7(m.m7), m11(m.m11), m15(m.m15) {}
        [[nodiscard]] constexpr operator Matrix() const {
            return Matrix {
                m0, m4, m8, m12,
                m1, m5, m9, m13,
                m2, m6, m10, m14,
                m3, m7, m11, m15
            };
        }
    };

    struct JSRectangle {
        float x = 0.0f;
        float y = 0.0f;
        float width = 0.0f;
        float height = 0.0f;
        JSRectangle() = default;
        JSRectangle(const float x, const float y, const float width, const float height) : x(x), y(y), width(width), height(height) {}
        explicit constexpr JSRectangle(const Rectangle r) : x(r.x), y(r.y), width(r.width), height(r.height) {}
        [[nodiscard]] constexpr operator Rectangle() const { return Rectangle { x, y, width, height }; }
    };

    struct JSCamera2D {
        JSVector2 offset;
        JSVector2 target;
        float rotation = 0.0f;
        float zoom = 1.0f;
        JSCamera2D() = default;
        JSCamera2D(const JSVector2& offset, const JSVector2& target, float rotation = 0.0f, float zoom = 1.0f)
            : offset(offset), target(target), rotation(rotation), zoom(zoom) {}
        explicit constexpr JSCamera2D(const Camera2D c)
            : offset(c.offset), target(c.target), rotation(c.rotation), zoom(c.zoom) {}
        [[nodiscard]] constexpr operator Camera2D() const { return Camera2D { offset, target, rotation, zoom }; }
    };

    struct JSCamera3D {
        JSVector3 position;
        JSVector3 target;
        JSVector3 up{ 0.0f, 1.0f, 0.0f };
        float fovy = 45.0f;
        int projection = CAMERA_PERSPECTIVE;
        JSCamera3D() = default;
        JSCamera3D(const JSVector3& pos, const JSVector3& target, const JSVector3& up = {0.0f, 1.0f, 0.0f}, float fovy = 45.0f, int proj = CAMERA_PERSPECTIVE)
            : position(pos), target(target), up(up), fovy(fovy), projection(proj) {}
        explicit constexpr JSCamera3D(const Camera3D c)
            : position(c.position), target(c.target), up(c.up), fovy(c.fovy), projection(c.projection) {}
        [[nodiscard]] constexpr operator Camera3D() const { return Camera3D { position, target, up, fovy, projection }; }
    };

    struct JSRay {
        JSVector3 position;
        JSVector3 direction;
        JSRay() = default;
        JSRay(const JSVector3& pos, const JSVector3& dir) : position(pos), direction(dir) {}
        explicit constexpr JSRay(const Ray r) : position(r.position), direction(r.direction) {}
        [[nodiscard]] constexpr operator Ray() const { return Ray { position, direction }; }
    };

    struct JSRayCollision {
        bool hit = false;
        float distance = 0.0f;
        JSVector3 point;
        JSVector3 normal;
        JSRayCollision() = default;
        explicit constexpr JSRayCollision(const RayCollision rc)
            : hit(rc.hit), distance(rc.distance), point(rc.point), normal(rc.normal) {}
        [[nodiscard]] constexpr operator RayCollision() const { return RayCollision { hit, distance, point, normal }; }
    };

    struct JSBoundingBox {
        JSVector3 min;
        JSVector3 max;
        JSBoundingBox() = default;
        JSBoundingBox(const JSVector3& min, const JSVector3& max) : min(min), max(max) {}
        explicit constexpr JSBoundingBox(const BoundingBox bb) : min(bb.min), max(bb.max) {}
        [[nodiscard]] constexpr operator BoundingBox() const { return BoundingBox { min, max }; }
    };

    struct JSNPatchInfo {
        JSRectangle source;
        int left = 0;
        int top = 0;
        int right = 0;
        int bottom = 0;
        int layout = NPATCH_NINE_PATCH;
        JSNPatchInfo() = default;
        explicit constexpr JSNPatchInfo(const NPatchInfo np)
            : source(np.source), left(np.left), top(np.top), right(np.right), bottom(np.bottom), layout(np.layout) {}
        [[nodiscard]] constexpr operator NPatchInfo() const { return NPatchInfo { source, left, top, right, bottom, layout }; }
    };

    struct JSGlyphInfo {
        int value = 0;
        int offsetX = 0;
        int offsetY = 0;
        int advanceX = 0;
        Image image{};
        JSGlyphInfo() = default;
        explicit JSGlyphInfo(const GlyphInfo g)
            : value(g.value), offsetX(g.offsetX), offsetY(g.offsetY), advanceX(g.advanceX), image(g.image) {}
        [[nodiscard]] operator GlyphInfo() const { return GlyphInfo { value, offsetX, offsetY, advanceX, image }; }
    };

    struct JSTransform {
        JSVector3 translation;
        JSVector4 rotation{0, 0, 0, 1};
        JSVector3 scale{1, 1, 1};
        JSTransform() = default;
        explicit constexpr JSTransform(const Transform t)
            : translation(t.translation), rotation(t.rotation), scale(t.scale) {}
        [[nodiscard]] constexpr operator Transform() const { return Transform { translation, rotation, scale }; }
    };

    struct JSImage {
        std::shared_ptr<Texture2D> texture_ptr;

        JSImage() = default;

        explicit JSImage(const std::string& path) {
            Texture2D tex = LoadTexture(path.c_str());
            texture_ptr = std::shared_ptr<Texture2D>(new Texture2D(tex), [](const Texture2D* pt) {
                if (pt->id != 0) {
                    UnloadTexture(*pt);
                }
                delete pt;
            });
        }

        [[nodiscard]] Texture2D get_texture() const {
            return texture_ptr ? *texture_ptr : Texture2D{ 0 };
        }

        [[nodiscard]] int get_width() const { return texture_ptr ? texture_ptr->width : 0; }
        [[nodiscard]] int get_height() const { return texture_ptr ? texture_ptr->height : 0; }
    };

    struct JSTexture {
        std::shared_ptr<Texture2D> texture_ptr;
        JSTexture() = default;
        explicit JSTexture(const std::string& path) {
            Texture2D tex = LoadTexture(path.c_str());
            texture_ptr = std::shared_ptr<Texture2D>(new Texture2D(tex), [](const Texture2D* pt) {
                if (pt->id != 0) {
                    UnloadTexture(*pt);
                }
                delete pt;
            });
        }
    };

    struct JSRenderTexture {
        std::shared_ptr<RenderTexture2D> target_ptr;
        JSRenderTexture() = default;
        JSRenderTexture(int width, int height) {
            RenderTexture2D target = LoadRenderTexture(width, height);
            target_ptr = std::shared_ptr<RenderTexture2D>(new RenderTexture2D(target), [](const RenderTexture2D* prt) {
                if (prt->id != 0) {
                    UnloadRenderTexture(*prt);
                }
                delete prt;
            });
        }
    };

    struct JSFont {
        std::shared_ptr<Font> font_ptr;
        JSFont() = default;
        explicit JSFont(const std::string& path, const int baseSize = 64) {
            const Font f = LoadFontEx(path.c_str(), baseSize, nullptr, 0);
            if (f.texture.id != 0) {
                SetTextureFilter(f.texture, TEXTURE_FILTER_BILINEAR);
            }
            font_ptr = std::shared_ptr<Font>(new Font(f), [](const Font* pf) {
                if (pf->texture.id != 0) {
                    UnloadFont(*pf);
                }
                delete pf;
            });
        }
    };

    struct JSMesh {
        std::shared_ptr<Mesh> mesh_ptr;
        JSMesh() = default;
        explicit JSMesh(Mesh mesh) {
            mesh_ptr = std::shared_ptr<Mesh>(new Mesh(mesh), [](const Mesh* pm) {
                UnloadMesh(*pm);
                delete pm;
            });
        }
    };

    struct JSShader {
        std::shared_ptr<Shader> shader_ptr;
        JSShader() = default;
        JSShader(const std::string& vsPath, const std::string& fsPath) {
            Shader s = LoadShader(vsPath.empty() ? nullptr : vsPath.c_str(),
                                  fsPath.empty() ? nullptr : fsPath.c_str());
            shader_ptr = std::shared_ptr<Shader>(new Shader(s), [](const Shader* ps) {
                if (ps->id != 0) {
                    UnloadShader(*ps);
                }
                delete ps;
            });
        }
    };

    struct JSMaterial {
        std::shared_ptr<Material> material_ptr;
        JSMaterial() = default;
        explicit JSMaterial(Material mat) {
            material_ptr = std::shared_ptr<Material>(new Material(mat), [](const Material* pm) {
                UnloadMaterial(*pm);
                delete pm;
            });
        }
    };

    struct JSModel {
        std::shared_ptr<Model> model_ptr;
        JSModel() = default;
        explicit JSModel(const std::string& path) {
            Model m = LoadModel(path.c_str());
            model_ptr = std::shared_ptr<Model>(new Model(m), [](const Model* pm) {
                UnloadModel(*pm);
                delete pm;
            });
        }
    };

    struct JSWave {
        std::shared_ptr<Wave> wave_ptr;
        JSWave() = default;
        explicit JSWave(const std::string& path) {
            Wave w = LoadWave(path.c_str());
            wave_ptr = std::shared_ptr<Wave>(new Wave(w), [](const Wave* pw) {
                UnloadWave(*pw);
                delete pw;
            });
        }
    };

    struct JSSound {
        std::shared_ptr<Sound> sound_ptr;
        JSSound() = default;
        explicit JSSound(const std::string& path) {
            Sound s = LoadSound(path.c_str());
            sound_ptr = std::shared_ptr<Sound>(new Sound(s), [](const Sound* ps) {
                UnloadSound(*ps);
                delete ps;
            });
        }
    };

    struct JSMusic {
        std::shared_ptr<Music> music_ptr;
        JSMusic() = default;
        explicit JSMusic(const std::string& path) {
            Music m = LoadMusicStream(path.c_str());
            music_ptr = std::shared_ptr<Music>(new Music(m), [](const Music* pm) {
                UnloadMusicStream(*pm);
                delete pm;
            });
        }
    };

    struct JSAudioStream {
        std::shared_ptr<AudioStream> stream_ptr;
        JSAudioStream() = default;
        JSAudioStream(unsigned int sampleRate, unsigned int sampleSize, unsigned int channels) {
            AudioStream stream = LoadAudioStream(sampleRate, sampleSize, channels);
            stream_ptr = std::shared_ptr<AudioStream>(new AudioStream(stream), [](const AudioStream* pas) {
                UnloadAudioStream(*pas);
                delete pas;
            });
        }
    };

    struct JSFilePathList {
        std::shared_ptr<FilePathList> files_ptr;
        JSFilePathList() = default;
        explicit JSFilePathList(const std::string& dirPath) {
            FilePathList f = LoadDirectoryFiles(dirPath.c_str());
            files_ptr = std::shared_ptr<FilePathList>(new FilePathList(f), [](const FilePathList* pf) {
                UnloadDirectoryFiles(*pf);
                delete pf;
            });
        }
    };

    struct JSDrawOptions {
        JSColor color = JSColor(BLACK);
        float rotation = 0.0f;
        bool wireframe = false;
        JSVector2 origin = JSVector2(0, 0);
    };

    struct JSTextOptions {
        JSFont font;
        JSColor color = JSColor(BLACK);
        float rotation = 0.0f;
        float fontSize = 24.0f;
        float spacing = 1.0f;
        JSVector2 origin = JSVector2(0, 0);
    };

}