#pragma once

class ISoundSystem
{

public:
	struct QueuedAudio
	{
		const std::string path;
		const int volume;
		const bool isSoundEffect;
	};

	virtual ~ISoundSystem() = default;

	virtual void PlaySoundEffect(const std::string& path, const float volume) = 0;
	virtual void StopAllSoundEffects() = 0;

	virtual void PlayMusic(const std::string& path, const float volume) = 0;
	virtual void StopSoundLoop(const std::string& path, const float volume) = 0;
};