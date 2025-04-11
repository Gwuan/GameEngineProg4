#include "ServiceAllocator.h"

#include "NullSoundSystem.hpp"

std::unique_ptr<ISoundSystem> ServiceAllocator::m_SoundSystemInstance = nullptr;

ISoundSystem& ServiceAllocator::GetSoundSystem()
{
	if (m_SoundSystemInstance == nullptr)
	{
		m_SoundSystemInstance = std::move(std::make_unique<NullSoundSystem>());
	}

	return *m_SoundSystemInstance;
}

void ServiceAllocator::RegisterSoundSystem(std::unique_ptr<ISoundSystem>&& soundSystem)
{
	if (soundSystem != nullptr)
	{
		m_SoundSystemInstance = std::move(soundSystem);
	}
}