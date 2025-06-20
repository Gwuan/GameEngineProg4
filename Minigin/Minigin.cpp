#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "SDLRenderer.h"
#include "ResourceManager.h"

#include <chrono>
#include <thread>

#include "CollisionSystem.h"
#include "JsonResolver.h"
#include "LoggingSoundSystem.h"
#include "NullSoundSystem.hpp"
#include "SDLSoundSystem.h"
#include "ServiceAllocator.h"
#include "TextureComponent.h"

SDL_Window* g_window{};
const float dae::Minigin::m_MsPerFrame{16.667f}; // 16.667ms -> 60 fps
const float dae::Minigin::m_FixedTimeStep{0.02f};


void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	ServiceAllocator::GetRenderer().Init(g_window);
	ResourceManager::GetInstance().Init(dataPath);
}

dae::Minigin::~Minigin()
{
	ServiceAllocator::GetRenderer().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();

	auto& renderer = ServiceAllocator::GetRenderer();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();
	auto& colSystem = CollisionSystem::GetInstance();

	#ifdef _DEBUG
	ServiceAllocator::RegisterSoundSystem(std::make_unique<LoggingSoundSystem>(
		std::make_unique<NullSoundSystem>())
	);
	#else
	ServiceAllocator::RegisterSoundSystem(std::make_unique<SDLSoundSystem>()); 
	#endif

	// The game loop is basically a copy pasta from the slides,
	// I have a feeling that this loop has to be refactored anyway in the future.
	// So it is fine for now that it is copied. I don't want to spend too
	// much time at this right now.
	bool doContinue = true;
	auto lastTime = std::chrono::high_resolution_clock::now();
	float lag{};
	while (doContinue)
	{
		const auto currentTime = std::chrono::high_resolution_clock::now();
		const float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;
		lag += deltaTime;

		doContinue = input.ProcessInput();

		while (lag >= m_MsPerFrame)
		{
			sceneManager.FixedUpdate(m_FixedTimeStep);
			lag -= m_FixedTimeStep;
		} 

		sceneManager.Update(deltaTime);
		colSystem.Update();
		sceneManager.LateUpdate(deltaTime);
		renderer.Render();

#ifdef _DEBUG
		renderer.DebugRender();
#endif


		const auto sleepTime = currentTime + std::chrono::milliseconds(static_cast<long long>(m_MsPerFrame)) - std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(sleepTime);
	}
}
