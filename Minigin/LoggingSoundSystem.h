#pragma once
#include <memory>
#include "SoundSystem.hpp"
class LoggingSoundSystem final :public ISoundSystem
{
public:
	LoggingSoundSystem(std::unique_ptr<ISoundSystem>&& soundSystem)
		: m_pRealSoundSystem(std::move(soundSystem))
	{}

	void PlaySoundEffect(const std::string& path, const float volume) override;

	void PlayMusic(const std::string& path, const float volume) override;
	void StopMusic() override;
	void PauseMusic() override;
	void ResumeMusic() override;

	void PauseAll() override;
	void ResumeAll() override;

private:
	std::unique_ptr<ISoundSystem> m_pRealSoundSystem = nullptr;
	static const std::string m_LogPrefix;

};

