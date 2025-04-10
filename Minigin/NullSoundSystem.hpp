#pragma once
#include <iostream>

#include "SoundSystem.hpp"


class NullSoundSystem final : public ISoundSystem
{
public:
	void PlaySoundEffect(const std::string&, const float) override {}

	void PlayMusic(const std::string&, const float) override {}
	void PauseMusic() override {}
	void ResumeMusic() override {}
	void StopMusic() override {}

	void PauseAll() override {}
	void ResumeAll() override {}
};