#pragma once
#include <string>

class ISoundSystem
{

public:
	virtual ~ISoundSystem() = default;

	virtual void PlaySoundEffect(const std::string& path, const float volume) = 0;

	virtual void PlayMusic(const std::string& path, const float volume) = 0;
	virtual void StopMusic() = 0;
	virtual void PauseMusic() = 0;
	virtual void ResumeMusic() = 0;

	virtual void PauseAll() = 0;
	virtual void ResumeAll() = 0;

protected:
	struct QueuedAudio
	{
		std::string path;
		float volume;
		bool isSoundEffect;
	};
};