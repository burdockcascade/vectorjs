#include <memory>
#include <raylib.h>
#include <qjspp.hpp>

#include "vjs_audio.hpp"

namespace App::Modules {

    void register_sound_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto sound = engine.make_class<JSSound>("Sound");

        // Constructor: const mySound = new vectorjs.Sound("assets/sfx.wav");
        sound.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSSound> {
            if (args.empty()) return nullptr;
            return std::make_unique<JSSound>(args[0].to_string());
        });

        // Instance Method: mySound.play()
        sound.instance_method("play", [](JSSound* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->sound_ptr) {
                PlaySound(*(self->sound_ptr));
            }
            return {};
        });

        // Instance Method: mySound.pause()
        sound.instance_method("pause", [](JSSound* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->sound_ptr) {
                PauseSound(*(self->sound_ptr));
            }
            return {};
        });

        // Instance Method: mySound.resume()
        sound.instance_method("resume", [](JSSound* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->sound_ptr) {
                ResumeSound(*(self->sound_ptr));
            }
            return {};
        });

        // Instance Method: mySound.stop()
        sound.instance_method("stop", [](JSSound* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->sound_ptr) {
                StopSound(*(self->sound_ptr));
            }
            return {};
        });

        // Instance Method: mySound.setVolume(0.5)
        sound.instance_method("setVolume", [](JSSound* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->sound_ptr && !args.empty()) {
                SetSoundVolume(*(self->sound_ptr), static_cast<float>(args[0].to_double()));
            }
            return {};
        });

        builder.export_class("Sound", sound.build());
    }

    void register_music_class(qjspp::Engine& engine, qjspp::ModuleBuilder& builder) {
        auto music = engine.make_class<JSMusic>("Music");

        // Constructor: const myMusic = new vectorjs.Sound("assets/sfx.wav");
        music.constructor([](const qjspp::ArgList& args) -> std::unique_ptr<JSMusic> {
            if (args.empty()) return nullptr;
            return std::make_unique<JSMusic>(args[0].to_string());
        });

        // Instance Method: myMusic.play()
        music.instance_method("play", [](JSMusic* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->music_ptr) {
                PlayMusicStream(*(self->music_ptr));
            }
            return {};
        });

        // Instance Method: myMusic.pause()
        music.instance_method("pause", [](JSMusic* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->music_ptr) {
                PauseMusicStream(*(self->music_ptr));
            }
            return {};
        });

        // Instance Method: myMusic.resume()
        music.instance_method("resume", [](JSMusic* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->music_ptr) {
                ResumeMusicStream(*(self->music_ptr));
            }
            return {};
        });

        // Instance Method: myMusic.stop()
        music.instance_method("stop", [](JSMusic* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->music_ptr) {
                StopMusicStream(*(self->music_ptr));
            }
            return {};
        });

        // Instance Method: myMusic.update()
        music.instance_method("update", [](JSMusic* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->music_ptr) {
                UpdateMusicStream(*(self->music_ptr));
            }
            return {};
        });

        // Instance Method: myMusic.setVolume(0.5)
        music.instance_method("setVolume", [](JSMusic* self, const qjspp::ArgList& args) -> qjspp::Value {
            if (self && self->music_ptr && !args.empty()) {
                SetMusicVolume(*(self->music_ptr), static_cast<float>(args[0].to_double()));
            }
            return {};
        });

        builder.export_class("Music", music.build());
    }

} // namespace VectorJS