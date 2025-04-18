#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <managers/AssetManager.h>
#include <string>

class SoundManager {
public:
    static SoundManager& Instance();

    bool Initialize();
    void Shutdown();

    void PlayMusic(const std::string& musicId, int loops = -1);
    void StopMusic();
    void PauseMusic();
    void ResumeMusic();
    bool IsMusicPlaying() const;

    void PlaySound(const std::string& soundId, int channel = -1, int loops = 0);
    void StopSound(int channel = -1);
    void StopAllSounds();

    void SetMusicVolume(int volume);
    int GetMusicVolume() const;
    
    void SetSoundVolume(int volume);
    int GetSoundVolume() const;

    void SetMasterVolume(int volume);
    int GetMasterVolume() const;

    void SetMusicEnabled(bool enabled);
    bool IsMusicEnabled() const;

    void SetSoundEnabled(bool enabled);
    bool IsSoundEnabled() const;

private:
    SoundManager();
    ~SoundManager();

    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;

    AssetManager& m_assetManager;

    bool m_initialized;
    bool m_musicEnabled;
    bool m_soundEnabled;
    int m_musicVolume;
    int m_soundVolume;
    int m_masterVolume;
    std::string m_currentMusicId;
};
