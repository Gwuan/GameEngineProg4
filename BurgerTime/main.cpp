#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <SDL_syswm.h>

#include "FpsCounterComponent.h"
#include "GameCommands.h"
#include "HudDemoComponent.h"
#include "InputManager.h"
#include "Minigin.h"
#include "MovementComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SDLSoundSystem.h"
#include "ServiceAllocator.h"
#include "SoundSystem.hpp"
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
	scene.Add(peterPepper);

	auto msSalt = std::make_shared<dae::GameObject>(); 
	msSalt->GetTransform()->SetPosition(330, 300);
	msSalt->AddComponent<TextureComponent>("../gameResources/salt.png");
	msSalt->AddComponent<MovementComponent>();
	scene.Add(msSalt);

	auto& input = dae::InputManager::GetInstance();

	ServiceAllocator::RegisterSoundSystem(std::make_unique<SDLSoundSystem>());

	// Input for peterPepper
	input.BindCommand(0, Gamepad::GamepadButton::NONE, SDL_SCANCODE_W, dae::InputAction::HOLD, std::make_unique<MovePepperCommand>(peterPepper.get(), MoveDirection::UP));
	input.BindCommand(0, Gamepad::GamepadButton::NONE, SDL_SCANCODE_S, dae::InputAction::HOLD, std::make_unique<MovePepperCommand>(peterPepper.get(), MoveDirection::DOWN));
	input.BindCommand(0, Gamepad::GamepadButton::NONE, SDL_SCANCODE_A, dae::InputAction::HOLD, std::make_unique<MovePepperCommand>(peterPepper.get(), MoveDirection::LEFT));
	input.BindCommand(0, Gamepad::GamepadButton::NONE, SDL_SCANCODE_D, dae::InputAction::HOLD, std::make_unique<MovePepperCommand>(peterPepper.get(), MoveDirection::RIGHT));

	// Input for msSalt
	input.BindCommand(0, Gamepad::GamepadButton::DPAD_UP, SDL_SCANCODE_UNKNOWN, dae::InputAction::HOLD, std::make_unique<MoveSaltCommand>(msSalt.get(), MoveDirection::UP));
	input.BindCommand(0, Gamepad::GamepadButton::DPAD_DOWN, SDL_SCANCODE_UNKNOWN, dae::InputAction::HOLD, std::make_unique<MoveSaltCommand>(msSalt.get(), MoveDirection::DOWN));
	input.BindCommand(0, Gamepad::GamepadButton::DPAD_LEFT, SDL_SCANCODE_UNKNOWN, dae::InputAction::HOLD, std::make_unique<MoveSaltCommand>(msSalt.get(), MoveDirection::LEFT));
	input.BindCommand(0, Gamepad::GamepadButton::DPAD_RIGHT, SDL_SCANCODE_UNKNOWN, dae::InputAction::HOLD, std::make_unique<MoveSaltCommand>(msSalt.get(), MoveDirection::RIGHT));

	input.BindCommand(0, Gamepad::GamepadButton::BUTTON_A, SDL_SCANCODE_UNKNOWN, dae::InputAction::PRESSED, std::make_unique<SoundTestCommand>(peterPepper.get()));

}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}