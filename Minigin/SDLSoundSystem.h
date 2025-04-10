#pragma once
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
	void PauseMusic() override;
	void ResumeMusic() override;
	void StopMusic() override;

	void PauseAll() override;
	void ResumeAll() override;

private:
	class Impl;
	Impl* m_pImpl;
};