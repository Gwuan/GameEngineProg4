#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "CircularMovement.h"
#include "FpsCounterComponent.h"
#include "MemoryBenchComponent.h"
#include "Minigin.h"
#include "PlotComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "TextComponent.h"
#include "TextureComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<TextureComponent>("background.tga");
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->GetTransform()->SetPosition(216, 180);
	go->AddComponent<TextureComponent>("logo.tga");
	scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_shared<dae::GameObject>();
	to->GetTransform()->SetPosition(80, 20);
	to->AddComponent<TextComponent>("Programming 4 Assignment", font);
	scene.Add(to);

	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 25);
	auto fpsCounter = std::make_shared<dae::GameObject>();
	fpsCounter->AddComponent<TextComponent>(font);
	fpsCounter->AddComponent<FpsCounterComponent>();
	scene.Add(fpsCounter);

	auto basicPepper = std::make_shared<dae::GameObject>();
	basicPepper->GetTransform()->SetPosition(300, 300);
	basicPepper->AddComponent<TextureComponent>("../gameResources/pepper.png");
	basicPepper->AddComponent<CircularMovement>();
	scene.Add(basicPepper);

	auto pepper2 = std::make_shared<dae::GameObject>();
	pepper2->SetParent(basicPepper.get());
	pepper2->AddComponent<TextureComponent>("../gameResources/pepper.png");
	pepper2->AddComponent<CircularMovement>(RotationDirection::RIGHT);
	scene.Add(pepper2);

	auto benchEx2 = std::make_shared<dae::GameObject>();
	benchEx2->AddComponent<MemoryBenchComponent<uint32_t>>();

	scene.Add(benchEx2);
	auto benchEx3 = std::make_shared<dae::GameObject>();
	benchEx3->AddComponent<MemoryBenchComponent<GameObject3D>>();
	benchEx3->AddComponent<MemoryBenchComponent<GameObject3DAlt>>();
	scene.Add(benchEx3);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}