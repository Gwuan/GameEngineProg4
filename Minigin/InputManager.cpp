#include <SDL.h>
#include "InputManager.h"

#include <iostream>
#include <SDL_syswm.h>

#include "Gamepad.h"
#include "backends/imgui_impl_sdl2.h"

constexpr auto MAX_GAMEPADS = 4;

dae::InputManager::InputManager()
	: m_BindedCommands(),
	  m_CurrentKeyboardState(SDL_GetKeyboardState(nullptr)),
	  m_Gamepads(MAX_GAMEPADS),
	  m_PreviousKeyboardState()
{
	// Creates bloat for unused controllers, I might want look into how I can handle
	// controller connection/disconnection.
	for (uint32_t controllerIdx{}; controllerIdx < MAX_GAMEPADS; controllerIdx++)
	{
		m_Gamepads[controllerIdx] = std::make_unique<Gamepad>(controllerIdx);
	}
}

dae::InputManager::~InputManager()
{
	m_CurrentKeyboardState = nullptr;
}

bool dae::InputManager::ProcessInput()
{
	std::memcpy(m_PreviousKeyboardState, m_CurrentKeyboardState, SDL_NUM_SCANCODES);

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

	HandleBindedInput();

	return true;
}

void dae::InputManager::HandleBindedInput()
{
	for (auto& bindedCommand : m_BindedCommands)
	{
		const unsigned& controllerIdx = std::get<0>(bindedCommand.first);
		const Gamepad::GamepadButton& gamepadButton = std::get<1>(bindedCommand.first);
		const SDL_Scancode& keyScancode = std::get<2>(bindedCommand.first);
		const InputAction& inputAction = std::get<3>(bindedCommand.first);
		auto command = bindedCommand.second.get();

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

bool dae::InputManager::UnbindCommand(unsigned int controllerIdx, Gamepad::GamepadButton button, SDL_Scancode keyboardKey, InputAction inputAction)
{
	if (auto found = m_BindedCommands.find(ControllerKey(controllerIdx, button, keyboardKey, inputAction)); found != m_BindedCommands.end())
	{
		m_BindedCommands.erase(found);
		return true;
	}

	return false;
}

bool dae::InputManager::IsKeyPressed(SDL_Scancode scancode) const
{
	return m_CurrentKeyboardState[scancode] && !m_PreviousKeyboardState[scancode];
}

bool dae::InputManager::IsKeyReleased(SDL_Scancode scancode) const
{
	return !m_CurrentKeyboardState[scancode] && m_PreviousKeyboardState[scancode];
}

bool dae::InputManager::IsKeyHoldDown(SDL_Scancode scancode) const
{
	return (m_CurrentKeyboardState[scancode] && scancode != SDL_SCANCODE_UNKNOWN);
}