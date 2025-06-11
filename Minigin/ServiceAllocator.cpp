#include "ServiceAllocator.h"

#include "NullSoundSystem.hpp"
#include "SDLRenderer.h"

std::unique_ptr<ISoundSystem> ServiceAllocator::m_SoundSystemInstance = nullptr;
std::unique_ptr<IRendererService> ServiceAllocator::m_RendererInstance = nullptr;

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

IRendererService& ServiceAllocator::GetRenderer()
{
	if (m_RendererInstance == nullptr)
	{
		m_RendererInstance = std::move(std::make_unique<dae::SDLRenderer>());
	}

	return *m_RendererInstance;
}

void ServiceAllocator::RegisterSoundSystem(std::unique_ptr<IRendererService>&& renderer)
{
	if (renderer != nullptr)
	{
		m_RendererInstance = std::move(renderer);
	}
}