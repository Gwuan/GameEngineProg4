#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "ColliderComponent.h"
#include "FpsCounterComponent.h"
#include "GameCommands.h"
#include "HudDemoComponent.h"
#include "InputManager.h"
#include "Minigin.h"
#include "PeterPepperComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SDLSoundSystem.h"
#include "ServiceAllocator.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "Transform.h"

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
	peterPepper->AddComponent<PeterPepperComponent>();
	scene.Add(peterPepper);

	auto msSalt = std::make_shared<dae::GameObject>(); 
	msSalt->GetTransform()->SetPosition(360, 300);
	msSalt->AddComponent<TextureComponent>("../gameResources/salt.png");
	msSalt->AddComponent<ColliderComponent>(ColliderComponent::Rect{{0, 0}, 32, 32}, true, false);

	scene.Add(msSalt);

	auto& input = dae::InputManager::GetInstance();

	// Input for peterPepper
	input.BindCommand(0, Gamepad::GamepadButton::NONE, SDL_SCANCODE_W, dae::InputAction::HOLD, std::make_unique<MovePepperCommand>(peterPepper.get(), glm::vec2{0.f, 1.f}));
	input.BindCommand(0, Gamepad::GamepadButton::NONE, SDL_SCANCODE_S, dae::InputAction::HOLD, std::make_unique<MovePepperCommand>(peterPepper.get(), glm::vec2{0.f, -1.f}));
	input.BindCommand(0, Gamepad::GamepadButton::NONE, SDL_SCANCODE_A, dae::InputAction::HOLD, std::make_unique<MovePepperCommand>(peterPepper.get(), glm::vec2{-1.f, 0.f}));
	input.BindCommand(0, Gamepad::GamepadButton::NONE, SDL_SCANCODE_D, dae::InputAction::HOLD, std::make_unique<MovePepperCommand>(peterPepper.get(), glm::vec2{1.f, 0.f}));

	// Input for msSalt
	input.BindCommand(0, Gamepad::GamepadButton::DPAD_UP, SDL_SCANCODE_UNKNOWN, dae::InputAction::HOLD, std::make_unique<MoveSaltCommand>(msSalt.get(), glm::vec2{0.f, 1.f}));
	input.BindCommand(0, Gamepad::GamepadButton::DPAD_DOWN, SDL_SCANCODE_UNKNOWN, dae::InputAction::HOLD, std::make_unique<MoveSaltCommand>(msSalt.get(), glm::vec2{0.f, -1.f}));
	input.BindCommand(0, Gamepad::GamepadButton::DPAD_LEFT, SDL_SCANCODE_UNKNOWN, dae::InputAction::HOLD, std::make_unique<MoveSaltCommand>(msSalt.get(), glm::vec2{-1.f, 0.f}));
	input.BindCommand(0, Gamepad::GamepadButton::DPAD_RIGHT, SDL_SCANCODE_UNKNOWN, dae::InputAction::HOLD, std::make_unique<MoveSaltCommand>(msSalt.get(), glm::vec2{1.f, 0.f}));

	input.BindCommand(0, Gamepad::GamepadButton::BUTTON_A, SDL_SCANCODE_UNKNOWN, dae::InputAction::PRESSED, std::make_unique<StartMusicCommand>(peterPepper.get()));
	input.BindCommand(0, Gamepad::GamepadButton::BUTTON_B, SDL_SCANCODE_UNKNOWN, dae::InputAction::PRESSED, std::make_unique<StopMusicCommand>(peterPepper.get()));
	input.BindCommand(0, Gamepad::GamepadButton::BUTTON_X, SDL_SCANCODE_UNKNOWN, dae::InputAction::PRESSED, std::make_unique<PauseMusicCommand>(peterPepper.get()));
	input.BindCommand(0, Gamepad::GamepadButton::BUTTON_Y, SDL_SCANCODE_UNKNOWN, dae::InputAction::PRESSED, std::make_unique<ResumeMusicCommand>(peterPepper.get()));

	input.BindCommand(0, Gamepad::GamepadButton::LEFT_THUMB, SDL_SCANCODE_UNKNOWN, dae::InputAction::PRESSED, std::make_unique<SoundTestCommand>(peterPepper.get()));

	input.BindCommand(0, Gamepad::GamepadButton::LEFT_SHOULDER, SDL_SCANCODE_UNKNOWN, dae::InputAction::PRESSED, std::make_unique<PauseAllAudioCommand>(peterPepper.get()));
	input.BindCommand(0, Gamepad::GamepadButton::RIGHT_SHOULDER, SDL_SCANCODE_UNKNOWN, dae::InputAction::PRESSED, std::make_unique<ResumeAllAudioCommand>(peterPepper.get()));

}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}