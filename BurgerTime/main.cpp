#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "BurgerComponent.h"
#include "ColliderComponent.h"
#include "EnemyComponent.h"
#include "EnemyType.h"
#include "GameCommands.h"
#include "InputManager.h"
#include "JsonResolver.h"
#include "Minigin.h"
#include "PeterPepperComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SpriteAnimation.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "Transform.h"

void RegisterGameComponents()
{
	auto& componentFactory = ComponentFactory::GetInstance();

	componentFactory.Register("BurgerSliceComponent",
    [](dae::GameObject& go, const nlohmann::json& jsonData) -> Component*
    {
        if (!jsonData.contains("sliceType") ||
            !jsonData["sliceType"].is_string()) 
        {
            throw std::runtime_error("The given sliceType is invalid.");
        }

        std::string sliceTypeStr = jsonData["sliceType"].get<std::string>();

        SliceType sliceType = SliceType::BottomBread;

        if (sliceTypeStr == "BottomBread") {
            sliceType = SliceType::BottomBread;
        } 
        else if (sliceTypeStr == "Meat") {
            sliceType = SliceType::Meat;
        } 
        else if (sliceTypeStr == "Salad") {
            sliceType = SliceType::Salad;
        } 
        else if (sliceTypeStr == "TopBread") {
            sliceType = SliceType::TopBread;
        } 
        else {
            throw std::runtime_error("Unknown SliceType.");
        }

        return go.AddComponent<BurgerSliceComponent>(sliceType);
    });

	componentFactory.Register("EnemyComponent",
    [](dae::GameObject& go, const nlohmann::json& jsonData) -> Component*
    {
        if (!jsonData.contains("enemyType") ||
            !jsonData["enemyType"].is_string()) 
        {
            throw std::runtime_error("The given enemyType is invalid.");
        }

        std::string enemyTypeStr = jsonData["enemyType"].get<std::string>();

        EnemyType* enemyType = {};

        if (enemyTypeStr == "MrHotdog") {
            enemyType = &g_MrHotDog;
        } 
        else if (enemyTypeStr == "MrPickle") {
            enemyType = &g_MrPickle;
        } 
        else if (enemyTypeStr == "MrEgg") {
            enemyType = &g_MrEgg;
        } 
        else {
            throw std::runtime_error("Unknown EnemyType.");
        }

        return go.AddComponent<EnemyComponent>(*enemyType);
    });
}


void load()
{
	RegisterGameComponents();

	// auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	//auto go = std::make_shared<dae::GameObject>();
	//go->AddComponent<TextureComponent>("background.tga");
	//scene.Add(go);

	//go = std::make_shared<dae::GameObject>();
	//go->GetTransform()->SetPosition(216, 180);
	//go->AddComponent<TextureComponent>("logo.tga");
	//scene.Add(go);

	//auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//auto to = std::make_shared<dae::GameObject>();
	//to->GetTransform()->SetPosition(80, 20);
	//to->AddComponent<TextComponent>("Programming 4 Assignment", font);
	//scene.Add(to);

	//font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 25);
	//auto fpsCounter = std::make_shared<dae::GameObject>();
	//fpsCounter->AddComponent<TextComponent>(font);
	//fpsCounter->AddComponent<FpsCounterComponent>();
	//scene.Add(fpsCounter);

	//auto peterPepper = std::make_shared<dae::GameObject>();
	//peterPepper->GetTransform()->SetPosition(300, 300);
	//peterPepper->AddComponent<PeterPepperComponent>();
	//scene.Add(peterPepper);

	//auto msSalt = std::make_shared<dae::GameObject>(glm::vec2{300.f, 300.f}, true); 
	////msSalt->AddComponent<TextureComponent>("../gameResources/salt.png");
	//SpriteAnimation::AnimationConfig msSaltConfig
	//{
	//	glm::vec2{16, 16},
	//	3,
	//	0,
	//	3,
	//	.6f
	//};
	//auto spriteTexture = dae::ResourceManager::GetInstance().LoadTexture("SpriteSheet.png");
	//msSalt->AddComponent<SpriteAnimation>(spriteTexture, msSaltConfig);
	//msSalt->AddComponent<ColliderComponent>(ColliderComponent::Rect{{0, 0}, 32, 32}, false);

	//scene.Add(msSalt);

	auto scene = dae::SceneManager::GetInstance().LoadSceneFromJson("../Data/Levels/TestLevel.json");
	if (!scene)
		return;

	auto peterPepper = std::make_shared<dae::GameObject>(glm::vec2{220, 220});
	peterPepper->AddComponent<PeterPepperComponent>();
	scene->Add(peterPepper);

	auto& input = dae::InputManager::GetInstance();
	// Input for peterPepper
	input.BindCommand(0, Gamepad::GamepadButton::NONE, SDL_SCANCODE_W, dae::InputAction::HOLD, std::make_unique<MovePepperCommand>(peterPepper.get(), glm::vec2{0.f, 1.f}));
	input.BindCommand(0, Gamepad::GamepadButton::NONE, SDL_SCANCODE_S, dae::InputAction::HOLD, std::make_unique<MovePepperCommand>(peterPepper.get(), glm::vec2{0.f, -1.f}));
	input.BindCommand(0, Gamepad::GamepadButton::NONE, SDL_SCANCODE_A, dae::InputAction::HOLD, std::make_unique<MovePepperCommand>(peterPepper.get(), glm::vec2{-1.f, 0.f}));
	input.BindCommand(0, Gamepad::GamepadButton::NONE, SDL_SCANCODE_D, dae::InputAction::HOLD, std::make_unique<MovePepperCommand>(peterPepper.get(), glm::vec2{1.f, 0.f}));
	input.BindCommand(0, Gamepad::GamepadButton::NONE, SDL_SCANCODE_E, dae::InputAction::PRESSED, std::make_unique<ShootPepper>(peterPepper.get()));

	//input.BindCommand(0, Gamepad::GamepadButton::DPAD_UP, SDL_SCANCODE_UNKNOWN, dae::InputAction::HOLD, std::make_unique<MoveSaltCommand>(msSalt.get(), glm::vec2{0.f, 1.f}));
	//input.BindCommand(0, Gamepad::GamepadButton::DPAD_DOWN, SDL_SCANCODE_UNKNOWN, dae::InputAction::HOLD, std::make_unique<MoveSaltCommand>(msSalt.get(), glm::vec2{0.f, -1.f}));
	//input.BindCommand(0, Gamepad::GamepadButton::DPAD_LEFT, SDL_SCANCODE_UNKNOWN, dae::InputAction::HOLD, std::make_unique<MoveSaltCommand>(msSalt.get(), glm::vec2{-1.f, 0.f}));
	//input.BindCommand(0, Gamepad::GamepadButton::DPAD_RIGHT, SDL_SCANCODE_UNKNOWN, dae::InputAction::HOLD, std::make_unique<MoveSaltCommand>(msSalt.get(), glm::vec2{1.f, 0.f}));
}


int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}