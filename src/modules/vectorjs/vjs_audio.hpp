#pragma once

#include <memory>
#include <raylib.h>
#include <qjspp.hpp>

namespace App::Modules {

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

}