#pragma once
#include <map>
#include <mutex>
#include <queue>
#include <SDL_mixer.h>

#include "SoundSystem.hpp"

// Supports .WAV sounds for soundEffects, .OGG Vorbis for music
class SDLSoundSystem final : public ISoundSystem
{
public:
	SDLSoundSystem();
	~SDLSoundSystem() override;

	SDLSoundSystem(const SDLSoundSystem&) = delete;
	SDLSoundSystem(SDLSoundSystem&&) noexcept = delete;
	SDLSoundSystem operator=(const SDLSoundSystem&) = delete;
	SDLSoundSystem operator=(SDLSoundSystem&&) noexcept = delete;

	void PlaySoundEffect(const std::string& path, const float volume) override;
	void PlayMusic(const std::string& path, const float volume) override;
	void StopSoundLoop(const std::string& path, const float volume) override;
	void StopAllSoundEffects() override;

private:
	// TODO: Apply initializer list & order guideline

	int ClampVolume(float volume);


	bool m_IsRunning = true;

	void ProcessQueue();

	std::queue<QueuedAudio> m_SoundEffectQueue;

	std::thread m_AudioThread;
	std::mutex m_AudioMutex;

	std::condition_variable m_NeedsAudio;

	Mix_Music* m_Music = nullptr;
};