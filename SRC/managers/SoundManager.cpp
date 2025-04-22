#include <managers/SoundManager.h>
#include <iostream>
#include <utils/GameSettings.h>

SoundManager& SoundManager::Instance() {
    static SoundManager instance;
    return instance;
}

SoundManager::SoundManager()
    : m_assetManager(AssetManager::Instance()),
      m_initialized(false),
      m_musicEnabled(GameSettings::MUSIC_ENABLED),
      m_soundEnabled(GameSettings::SOUND_ENABLED),
      m_musicVolume(static_cast<int>(GameSettings::MUSIC_VOLUME * MIX_MAX_VOLUME)),
      m_soundVolume(static_cast<int>(GameSettings::SOUND_VOLUME * MIX_MAX_VOLUME)),
      m_masterVolume(MIX_MAX_VOLUME) {
}

SoundManager::~SoundManager() {
    Shutdown();
}

bool SoundManager::Initialize() {
    if (m_initialized) return true;

    int flags = MIX_INIT_MP3 | MIX_INIT_OGG;
    int initted = Mix_Init(flags);
    if ((initted & flags) != flags) {
        std::cerr << "Mix_Init failed: " << Mix_GetError() << std::endl;
        return false;
    }

    SDL_Init(SDL_INIT_AUDIO);
    
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }
    
    // Allocate more channels for sound effects (default is 8)
    Mix_AllocateChannels(32);
    
    m_initialized = true;
    return true;
}

void SoundManager::Shutdown() {
    if (!m_initialized) return;
    
    StopMusic();
    StopAllSounds();
    
    Mix_CloseAudio();
    Mix_Quit();
    
    m_initialized = false;
}


void SoundManager::PlayMusic(const std::string& musicId, int loops) {
    if (!m_initialized || !m_musicEnabled) return;
    
    Mix_Music* music = m_assetManager.GetMusic(musicId);
    if (!music) {
        std::cerr << "Music not found: " << musicId << std::endl;
        return;
    }
    
    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }
    
    if (Mix_PlayMusic(music, loops) == -1) {
        std::cerr << "Failed to play music: " << Mix_GetError() << std::endl;
        return;
    }
    
    m_currentMusicId = musicId;
    SetMusicVolume(m_musicVolume);
}

void SoundManager::StopMusic() {
    if (!m_initialized) return;
    
    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }
    m_currentMusicId = "";
}

void SoundManager::PauseMusic() {
    if (!m_initialized) return;
    
    if (Mix_PlayingMusic() && !Mix_PausedMusic()) {
        Mix_PauseMusic();
    }
}

void SoundManager::ResumeMusic() {
    if (!m_initialized) return;
    
    if (Mix_PausedMusic()) {
        Mix_ResumeMusic();
    }
}

bool SoundManager::IsMusicPlaying() const {
    if (!m_initialized) return false;
    
    return Mix_PlayingMusic() && !Mix_PausedMusic();
}

void SoundManager::PlaySound(const std::string& soundId, int channel, int loops) {
    if (!m_initialized || !m_soundEnabled) return;
    
    Mix_Chunk* sound = m_assetManager.GetSound(soundId);
    if (!sound) {
        std::cerr << "Sound not found: " << soundId << std::endl;
        return;
    }
    
    int playChannel = Mix_PlayChannel(channel, sound, loops);
    if (playChannel == -1) {
        std::cerr << "Failed to play sound: " << Mix_GetError() << std::endl;
        return;
    }
    
    Mix_Volume(playChannel, (m_soundVolume * m_masterVolume) / MIX_MAX_VOLUME);
}

void SoundManager::StopSound(int channel) {
    if (!m_initialized) return;
    
    Mix_HaltChannel(channel);
}

void SoundManager::StopAllSounds() {
    if (!m_initialized) return;
    
    Mix_HaltChannel(-1);
}

void SoundManager::SetMusicVolume(int volume) {
    m_musicVolume = volume;
    if (m_musicVolume < 0) m_musicVolume = 0;
    if (m_musicVolume > MIX_MAX_VOLUME) m_musicVolume = MIX_MAX_VOLUME;
    
    if (!m_initialized) return;
    
    int adjustedVolume = (m_musicVolume * m_masterVolume) / MIX_MAX_VOLUME;
    Mix_VolumeMusic(adjustedVolume);
}

int SoundManager::GetMusicVolume() const {
    return m_musicVolume;
}

void SoundManager::SetSoundVolume(int volume) {
    m_soundVolume = volume;
    if (m_soundVolume < 0) m_soundVolume = 0;
    if (m_soundVolume > MIX_MAX_VOLUME) m_soundVolume = MIX_MAX_VOLUME;
    
    if (!m_initialized) return;
    
    int adjustedVolume = (m_soundVolume * m_masterVolume) / MIX_MAX_VOLUME;
    Mix_Volume(-1, adjustedVolume);
}

int SoundManager::GetSoundVolume() const {
    return m_soundVolume;
}

void SoundManager::SetMasterVolume(int volume) {
    m_masterVolume = volume;
    if (m_masterVolume < 0) m_masterVolume = 0;
    if (m_masterVolume > MIX_MAX_VOLUME) m_masterVolume = MIX_MAX_VOLUME;
    
    if (!m_initialized) return;
    
    SetMusicVolume(m_musicVolume);
    SetSoundVolume(m_soundVolume);
}

int SoundManager::GetMasterVolume() const {
    return m_masterVolume;
}

void SoundManager::SetMusicEnabled(bool enabled) {
    m_musicEnabled = enabled;
    
    if (!m_initialized) return;
    
    if (!m_musicEnabled) {
        StopMusic();
    } else {
        PlayMusic("main_theme", -1);
    }
}

bool SoundManager::IsMusicEnabled() const {
    return m_musicEnabled;
}

void SoundManager::SetSoundEnabled(bool enabled) {
    m_soundEnabled = enabled;
    
    if (!m_initialized || !enabled) {
        StopAllSounds();
    }
}

bool SoundManager::IsSoundEnabled() const {
    return m_soundEnabled;
}
