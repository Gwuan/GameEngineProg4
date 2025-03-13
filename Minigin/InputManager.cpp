#include <SDL.h>
#include "InputManager.h"

#include <iostream>
#include <SDL_syswm.h>

#include "Gamepad.h"
#include "backends/imgui_impl_sdl2.h"

constexpr auto MAX_GAMEPADS = 4;

dae::InputManager::InputManager()
	: m_CurrentKeyboardState(static_cast<Uint8*>(malloc(sizeof(Uint8))))
{
	// Creates bloat for unused controllers, I might want look into how I can handle
	// controller connection/disconnection. 
	m_Gamepads.reserve(MAX_GAMEPADS);
	for (uint32_t i{}; i < MAX_GAMEPADS; i++)
	{
		m_Gamepads.emplace_back(std::make_unique<Gamepad>(i));
	}

	m_PreviousKeyboardState.resize(SDL_NUM_SCANCODES);

	m_KeysReleasedThisFrame.resize(SDL_NUM_SCANCODES);
	m_KeysPressedThisFrame.resize(SDL_NUM_SCANCODES);
}

dae::InputManager::~InputManager()
{
}

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	for (auto& gamepad : m_Gamepads)
	{
		gamepad->Update();
	}

	// Not sure if this needs to be pimpled away
	std::copy(m_CurrentKeyboardState, m_CurrentKeyboardState + SDL_NUM_SCANCODES, m_PreviousKeyboardState.begin());
	m_CurrentKeyboardState = SDL_GetKeyboardState(nullptr);

	for (uint32_t i{}; i < SDL_NUM_SCANCODES; i++)
	{
		const Uint8 keyChanged = m_CurrentKeyboardState[i] ^ m_PreviousKeyboardState[i];
		m_KeysPressedThisFrame[i] = keyChanged & m_CurrentKeyboardState[i];
		m_KeysReleasedThisFrame[i] = keyChanged & (~m_CurrentKeyboardState[i]);
	}

	HandleBindedInput();

	return true;
}

// TODO: Add keyboard support
void dae::InputManager::HandleBindedInput()
{
	for (auto& bindedCommand : m_BindedCommands)
	{
		auto& controllerIdx = std::get<0>(bindedCommand.first);
		auto& gamepadButton = std::get<1>(bindedCommand.first);
		auto& keyScancode = std::get<2>(bindedCommand.first);
		InputAction inputAction = std::get<3>(bindedCommand.first);
		auto& command = bindedCommand.second;

		switch (inputAction)
		{
		case InputAction::PRESSED:
			if (m_Gamepads[controllerIdx]->IsButtonPressed(gamepadButton, controllerIdx) || 
				IsKeyPressed(keyScancode))
			{
				command->Execute();
			}
			break;
		case InputAction::RELEASED:
			if (m_Gamepads[controllerIdx]->IsButtonReleased(gamepadButton, controllerIdx) || 
				IsKeyReleased(keyScancode))
			{
				command->Execute();
			}
			
			break;
		case InputAction::HOLD:
			if (m_Gamepads[controllerIdx]->IsButtonHoldDown(gamepadButton, controllerIdx) || 
				IsKeyHoldDown(keyScancode))
			{
				command->Execute();
			}
			break;
		}
	}
}

void dae::InputManager::BindCommand(unsigned int controllerIdx, Gamepad::GamepadButton button,
			SDL_Scancode keyboardKey, InputAction inputAction, std::unique_ptr<Command> command)
{
	ControllerKey inputKey = std::make_tuple(controllerIdx, button, keyboardKey, inputAction);
	m_BindedCommands.insert_or_assign(inputKey, std::move(command));
}

bool dae::InputManager::IsKeyPressed(SDL_Scancode scancode) const
{
	return (m_KeysPressedThisFrame[scancode] && scancode != SDL_SCANCODE_UNKNOWN);
}

bool dae::InputManager::IsKeyReleased(SDL_Scancode scancode) const
{
	return (m_KeysReleasedThisFrame[scancode] && scancode != SDL_SCANCODE_UNKNOWN);
}

bool dae::InputManager::IsKeyHoldDown(SDL_Scancode scancode) const
{
	return (m_CurrentKeyboardState[scancode] && scancode != SDL_SCANCODE_UNKNOWN);
}

