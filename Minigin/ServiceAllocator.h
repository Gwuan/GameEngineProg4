#pragma once
#include <memory>

class IRendererService;
class ISoundSystem;

class ServiceAllocator final 
{
public:
	static ISoundSystem& GetSoundSystem();
	static void RegisterSoundSystem(std::unique_ptr<ISoundSystem>&& soundSystem);

	static IRendererService& GetRenderer();
	static void RegisterSoundSystem(std::unique_ptr<IRendererService>&& renderer);

private:
	static std::unique_ptr<ISoundSystem> m_SoundSystemInstance;
	static std::unique_ptr<IRendererService> m_RendererInstance;
};