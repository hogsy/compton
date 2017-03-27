// Virtual Critters, Copyright (C) 2016-2017 Mark Elsworth Sowden

#include "../Shared.h"

#include <allegro5/allegro_audio.h>

#include "AudioManager.hpp"

class AudioManager {
public:
    ~AudioManager();

    static AudioManager *GetInstance() {
        if(!instance_) {
            instance_ = new AudioManager();
        }

        return instance_;
    }



protected:
private:
    AudioManager();

    AudioManager *instance_;
};

AudioManager::AudioManager() {
    al_install_audio();
    if(!al_is_audio_installed()) {

    }
}

AudioManager::~AudioManager() {
    al_uninstall_audio();
}
