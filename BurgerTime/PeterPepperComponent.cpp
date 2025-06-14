#include "PeterPepperComponent.h"

#include "ColliderComponent.h"
#include "GameCommands.h"
#include "InputManager.h"
#include "PeterPepperState.h"
#include "ResourceManager.h"
#include "ServiceAllocator.h"
#include "SoundSystem.hpp"
#include "SpriteAnimation.h"
#include "TextureComponent.h"
#include "Transform.h"
#include "Utils.hpp"
#include <SDL_keyboard.h>

#include "LevelManager.h"


void PeterPepperComponent::BeginPlay()
{
	GetOwner().GetTransform()->DisableVerticalMovement(true);
	GetOwner().SetTag("Player");

	auto& input = dae::InputManager::GetInstance();
	// Input for peterPepper
	input.BindCommand(0, Gamepad::GamepadButton::DPAD_UP, SDL_SCANCODE_W, dae::InputAction::HOLD, std::make_unique<MovePepperCommand>(&GetOwner(), glm::vec2{0.f, 1.f}));
	input.BindCommand(0, Gamepad::GamepadButton::DPAD_DOWN, SDL_SCANCODE_S, dae::InputAction::HOLD, std::make_unique<MovePepperCommand>(&GetOwner(), glm::vec2{0.f, -1.f}));
	input.BindCommand(0, Gamepad::GamepadButton::DPAD_LEFT, SDL_SCANCODE_A, dae::InputAction::HOLD, std::make_unique<MovePepperCommand>(&GetOwner(), glm::vec2{-1.f, 0.f}));
	input.BindCommand(0, Gamepad::GamepadButton::DPAD_RIGHT, SDL_SCANCODE_D, dae::InputAction::HOLD, std::make_unique<MovePepperCommand>(&GetOwner(), glm::vec2{1.f, 0.f}));

	input.BindCommand(0, Gamepad::GamepadButton::BUTTON_A, SDL_SCANCODE_E, dae::InputAction::PRESSED, std::make_unique<ShootPepper>(&GetOwner()));

	AddObserver(&LevelManager::GetInstance());
}

void PeterPepperComponent::Update(float deltaTime)
{
	if (auto newState = m_State->Update(deltaTime))
	{
		m_State->OnExit();
		m_State = std::move(newState);
		m_State->OnEnter();
	}
}

void PeterPepperComponent::LateUpdate(const float)
{
	if (m_ShootRequested)
		m_ShootRequested = false;
}

PeterPepperComponent::~PeterPepperComponent()
{
	auto& input = dae::InputManager::GetInstance();
	// Input for peterPepper
	input.UnbindCommand(0, Gamepad::GamepadButton::DPAD_UP, SDL_SCANCODE_W, dae::InputAction::HOLD);
	input.UnbindCommand(0, Gamepad::GamepadButton::DPAD_DOWN, SDL_SCANCODE_S, dae::InputAction::HOLD);
	input.UnbindCommand(0, Gamepad::GamepadButton::DPAD_LEFT, SDL_SCANCODE_A, dae::InputAction::HOLD);
	input.UnbindCommand(0, Gamepad::GamepadButton::DPAD_RIGHT, SDL_SCANCODE_D, dae::InputAction::HOLD);

	input.UnbindCommand(0, Gamepad::GamepadButton::BUTTON_A, SDL_SCANCODE_E, dae::InputAction::PRESSED);

	RemoveObserver(&LevelManager::GetInstance());
}

void PeterPepperComponent::RequestShoot()
{
	if (!m_ShootRequested)
		m_ShootRequested = true;
}

PeterPepperComponent::PeterPepperComponent(dae::GameObject& owner)
	: Component(owner)
{
	owner.AddComponent<ColliderComponent>(glm::vec2{14, 14}, false);
	auto ladderCollider = owner.AddComponent<ColliderComponent>(glm::vec2{4, 12}, true);
	ladderCollider->m_Offset.y = -4;

	auto spriteSheet = dae::ResourceManager::GetInstance().LoadTexture("SpriteSheet.png");
	owner.AddComponent<SpriteAnimation>(spriteSheet, SpriteAnimation::AnimationConfig{});


	ladderCollider->OnBeginOverlap += [this](ColliderComponent* other) { OnBeginOverlap(other); };
	ladderCollider->OnEndOverlap += [this](ColliderComponent* other) { OnEndOverlap(other); };

	m_State = std::make_unique<PeterIdleState>(*this);
	m_State->OnEnter();
}

void PeterPepperComponent::PlaySoundOnOverlap(const ColliderComponent*)
{
	ServiceAllocator::GetSoundSystem().PlaySoundEffect("../gameResources/sounds/level_start.wav", 1.f);
}

void PeterPepperComponent::OnBeginOverlap(const ColliderComponent* otherCollider)
{
	if (m_IsDead || !otherCollider)
		return;

	if (otherCollider->GetOwner().GetTag().find("Ladder") != std::string::npos)
	{
		m_LadderCounter++;
		if (otherCollider->GetOwner().GetTag() == "LadderEntry")
		{
			m_OnLadderEntry = true;
		}

		Notify(&GetOwner(), HashUtils::make_sdbm_hash("OnLadderCountChange"));
	}
	else if (otherCollider->GetOwner().GetTag() == "Enemy")
	{
		if (!m_IsDead)
		{
			Notify(&GetOwner(), HashUtils::make_sdbm_hash("PlayerDied"));

			ServiceAllocator::GetSoundSystem().StopMusic();
			ServiceAllocator::GetSoundSystem().PlaySoundEffect("../Data/Sounds/Death.wav", 0.8f);
			m_IsDead = true;
		}
	}
}

void PeterPepperComponent::OnEndOverlap(const ColliderComponent* otherCollider)
{
	if (otherCollider->GetOwner().GetTag().find("Ladder") != std::string::npos)
	{
		m_LadderCounter--;
		m_LadderCounter = std::max(m_LadderCounter, 0);

		if (otherCollider->GetOwner().GetTag() == "LadderEntry")
		{
			m_OnLadderEntry = false;
		}

		Notify(&GetOwner(), HashUtils::make_sdbm_hash("OnLadderCountChange"));
	}
}