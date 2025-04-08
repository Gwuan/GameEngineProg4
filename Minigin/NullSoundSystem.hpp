#pragma once
#include <iostream>

#include "SoundSystem.hpp"


class NullSoundSystem final : public ISoundSystem
{
public:
	void PlaySoundEffect(const std::string&, const float) override { std::cout << "Attempted to use PlaySoundEffect()" << std::endl; }
	void StopAllSoundEffects() override { std::cout << "Attempted to use StopAllSoundEffects()" << std::endl; } 
	void PlayMusic(const std::string&, const float) override { std::cout << "Attempted to use PlayMusic()" << std::endl; }
	void StopSoundLoop(const std::string&, const float) override { std::cout << "Attempted to use StopSoundLoop()" << std::endl; }
};