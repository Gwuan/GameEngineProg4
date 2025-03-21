#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "FpsCounterComponent.h"
#include "HudDemoComponent.h"
#include "InputManager.h"
#include "Minigin.h"
#include "MovementComponent.h"
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

	auto peterPepper = std::make_shared<dae::GameObject>();
	peterPepper->GetTransform()->SetPosition(300, 300);
	peterPepper->AddComponent<TextureComponent>("../gameResources/pepper.png");
	peterPepper->AddComponent<MovementComponent>();
	// basicPepper->AddComponent<CircularMovement>();
	scene.Add(peterPepper);

	auto msSalt = std::make_shared<dae::GameObject>(); 
	// pepper2->SetParent(basicPepper.get());
	msSalt->GetTransform()->SetPosition(330, 300);
	msSalt->AddComponent<TextureComponent>("../gameResources/salt.png");
	msSalt->AddComponent<MovementComponent>();
	//pepper2->AddComponent<CircularMovement>(RotationDirection::RIGHT);
	scene.Add(msSalt);

	auto HUD = std::make_shared<dae::GameObject>();
	auto hudPepper = HUD->AddComponent<HudDemoComponent>();
	auto hudSalt = HUD->AddComponent<HudDemoComponent>(glm::vec2{0, 72});
	HUD->GetTransform()->SetPosition(0, 250);
	scene.Add(HUD);

	peterPepper->GetSubject()->AddObserver(hudPepper);
	msSalt->GetSubject()->AddObserver(hudSalt);

	auto fontSmall = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 12);
	auto infoObject = std::make_shared<dae::GameObject>();
	infoObject->AddComponent<TextComponent>("Use WASD to move Pepper, C to inflict damage, Z to kill HotDog (+100 points), X to kill Pickle (+200 points)", fontSmall);
	auto infoSalt = infoObject->AddComponent<TextComponent>("Use the D-Pad to move Salt, X to inflict damage, A to kill HotDog (+100 points), B to kill Pickle (+200 points)", fontSmall);
	infoSalt->SetTextOffset({0, 12});
	infoObject->GetTransform()->SetPosition({0, 100});
	scene.Add(infoObject);

	auto& input = dae::InputManager::GetInstance();
	// Input for peterPepper
	input.BindCommand(0, Gamepad::GamepadButton::NONE, SDL_SCANCODE_W, dae::InputAction::HOLD, std::make_unique<MovePepperCommand>(peterPepper.get(), MoveDirection::UP));
	input.BindCommand(0, Gamepad::GamepadButton::NONE, SDL_SCANCODE_S, dae::InputAction::HOLD, std::make_unique<MovePepperCommand>(peterPepper.get(), MoveDirection::DOWN));
	input.BindCommand(0, Gamepad::GamepadButton::NONE, SDL_SCANCODE_A, dae::InputAction::HOLD, std::make_unique<MovePepperCommand>(peterPepper.get(), MoveDirection::LEFT));
	input.BindCommand(0, Gamepad::GamepadButton::NONE, SDL_SCANCODE_D, dae::InputAction::HOLD, std::make_unique<MovePepperCommand>(peterPepper.get(), MoveDirection::RIGHT));

	// NOTE: Pressed & Released state doesn't work for keyboard some reason, changed to hold for demonstration purposes 
	input.BindCommand(0, Gamepad::GamepadButton::NONE, SDL_SCANCODE_C, dae::InputAction::HOLD, std::make_unique<DieCommand>(peterPepper.get()));
	input.BindCommand(0, Gamepad::GamepadButton::NONE, SDL_SCANCODE_Z, dae::InputAction::HOLD, std::make_unique<KillHotDogCommand>(peterPepper.get()));
	input.BindCommand(0, Gamepad::GamepadButton::NONE, SDL_SCANCODE_X, dae::InputAction::HOLD, std::make_unique<KillPickleCommand>(peterPepper.get()));

	// Input for msSalt
	input.BindCommand(0, Gamepad::GamepadButton::DPAD_UP, SDL_SCANCODE_UNKNOWN, dae::InputAction::HOLD, std::make_unique<MoveSaltCommand>(msSalt.get(), MoveDirection::UP));
	input.BindCommand(0, Gamepad::GamepadButton::DPAD_DOWN, SDL_SCANCODE_UNKNOWN, dae::InputAction::HOLD, std::make_unique<MoveSaltCommand>(msSalt.get(), MoveDirection::DOWN));
	input.BindCommand(0, Gamepad::GamepadButton::DPAD_LEFT, SDL_SCANCODE_UNKNOWN, dae::InputAction::HOLD, std::make_unique<MoveSaltCommand>(msSalt.get(), MoveDirection::LEFT));
	input.BindCommand(0, Gamepad::GamepadButton::DPAD_RIGHT, SDL_SCANCODE_UNKNOWN, dae::InputAction::HOLD, std::make_unique<MoveSaltCommand>(msSalt.get(), MoveDirection::RIGHT));

	input.BindCommand(0, Gamepad::GamepadButton::BUTTON_X, SDL_SCANCODE_UNKNOWN, dae::InputAction::PRESSED, std::make_unique<DieCommand>(msSalt.get()));
	input.BindCommand(0, Gamepad::GamepadButton::BUTTON_A, SDL_SCANCODE_UNKNOWN, dae::InputAction::PRESSED, std::make_unique<KillHotDogCommand>(msSalt.get()));
	input.BindCommand(0, Gamepad::GamepadButton::BUTTON_B, SDL_SCANCODE_UNKNOWN, dae::InputAction::PRESSED, std::make_unique<KillPickleCommand>(msSalt.get()));
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}