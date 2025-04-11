#pragma once
#include <memory>

class ISoundSystem;

class ServiceAllocator final 
{
public:
	static ISoundSystem& GetSoundSystem();
	static void RegisterSoundSystem(std::unique_ptr<ISoundSystem>&& soundSystem);

private:
	static std::unique_ptr<ISoundSystem> m_SoundSystemInstance;
};