#include <SDL.h>
#include "InputManager.h"
#include <algorithm>
#include <iostream>
#include <SDL_syswm.h>
#include "Gamepad.h"
#include "backends/imgui_impl_sdl2.h"

constexpr auto MAX_GAMEPADS = 4;

dae::InputManager::InputManager()
	: m_BindedCommands(),
	  m_CurrentKeyboardState(SDL_GetKeyboardState(nullptr)),
	  m_Gamepads(MAX_GAMEPADS),
	  m_PreviousKeyboardState(),
	  m_PendingKillList()
{
	// Initialize gamepads
	for (uint32_t controllerIdx = 0; controllerIdx < MAX_GAMEPADS; controllerIdx++)
	{
		m_Gamepads[controllerIdx] = std::make_unique<Gamepad>(controllerIdx);
	}
}

dae::InputManager::~InputManager()
{
	// Clear all bindings to ensure proper cleanup
	m_BindedCommands.clear();
	m_PendingKillList.clear();
	m_CurrentKeyboardState = nullptr;
}

bool dae::InputManager::ProcessInput()
{
	// Copy current keyboard state to previous state
	std::memcpy(m_PreviousKeyboardState, m_CurrentKeyboardState, SDL_NUM_SCANCODES);

	// Process SDL events
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			return false;
		}
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	for (auto& gamepad : m_Gamepads)
	{
		gamepad->Update();
	}

	KillUnbindedCommands();

	HandleBindedInput();

	return true;
}

void dae::InputManager::HandleBindedInput()
{
	for (const auto& bindedCommand : m_BindedCommands)
	{
		const auto& [controllerIdx, gamepadButton, keyScancode, inputAction] = bindedCommand.first;
		auto* command = bindedCommand.second.get();

		if (!command)
		{
			continue;
		}

		bool shouldExecute = false;

		switch (inputAction)
		{
		case InputAction::PRESSED:
			shouldExecute = (controllerIdx < m_Gamepads.size() &&
					m_Gamepads[controllerIdx]->IsButtonPressed(gamepadButton, controllerIdx)) ||
				IsKeyPressed(keyScancode);
			break;

		case InputAction::RELEASED:
			shouldExecute = (controllerIdx < m_Gamepads.size() &&
					m_Gamepads[controllerIdx]->IsButtonReleased(gamepadButton, controllerIdx)) ||
				IsKeyReleased(keyScancode);
			break;

		case InputAction::HOLD:
			shouldExecute = (controllerIdx < m_Gamepads.size() &&
					m_Gamepads[controllerIdx]->IsButtonHoldDown(gamepadButton, controllerIdx)) ||
				IsKeyHoldDown(keyScancode);
			break;
		}

		if (shouldExecute)
		{
			command->Execute();
		}
	}
}

void dae::InputManager::BindCommand(unsigned int controllerIdx, Gamepad::GamepadButton button,
                                    SDL_Scancode keyboardKey, InputAction inputAction,
                                    std::unique_ptr<Command> command)
{
	if (!command)
	{
		return;
	}

	ControllerKey inputKey = std::make_tuple(controllerIdx, button, keyboardKey, inputAction);

	auto existingBinding =
		std::ranges::find_if(m_BindedCommands,[&inputKey](const auto& pair)
			{
				return pair.first == inputKey;
			});

	if (existingBinding != m_BindedCommands.end())
	{
		// Replace existing binding
		existingBinding->second = std::move(command);
	}
	else
	{
		m_BindedCommands.emplace_back(inputKey, std::move(command));
	}
}

bool dae::InputManager::UnbindCommand(unsigned int controllerIdx, Gamepad::GamepadButton button,
                                      SDL_Scancode keyboardKey, InputAction inputAction)
{
	ControllerKey needToFind = std::make_tuple(controllerIdx, button, keyboardKey, inputAction);

	// Check if the binding exists
	auto it = std::find_if(m_BindedCommands.begin(), m_BindedCommands.end(),
	                       [&needToFind](const auto& pair)
	                       {
		                       return pair.first == needToFind;
	                       });

	if (it != m_BindedCommands.end())
	{
		m_PendingKillList.push_back(needToFind);
		return true;
	}

	return false;
}

void dae::InputManager::KillUnbindedCommands()
{
	if (m_PendingKillList.empty())
	{
		return;
	}

	// Sort and remove duplicates from kill list
	std::sort(m_PendingKillList.begin(), m_PendingKillList.end());
	auto newEnd = std::unique(m_PendingKillList.begin(), m_PendingKillList.end());
	m_PendingKillList.erase(newEnd, m_PendingKillList.end());

	// Remove commands from the vector
	for (const auto& keyToKill : m_PendingKillList)
	{
		auto it = std::find_if(m_BindedCommands.begin(), m_BindedCommands.end(),
		                       [&keyToKill](const auto& pair)
		                       {
			                       return pair.first == keyToKill;
		                       });

		if (it != m_BindedCommands.end())
		{
			m_BindedCommands.erase(it);
		}
	}

	m_PendingKillList.clear();
}

bool dae::InputManager::IsKeyPressed(SDL_Scancode scancode) const
{
	if (scancode == SDL_SCANCODE_UNKNOWN || scancode >= SDL_NUM_SCANCODES)
	{
		return false;
	}
	return m_CurrentKeyboardState[scancode] && !m_PreviousKeyboardState[scancode];
}

bool dae::InputManager::IsKeyReleased(SDL_Scancode scancode) const
{
	if (scancode == SDL_SCANCODE_UNKNOWN || scancode >= SDL_NUM_SCANCODES)
	{
		return false;
	}
	return !m_CurrentKeyboardState[scancode] && m_PreviousKeyboardState[scancode];
}

bool dae::InputManager::IsKeyHoldDown(SDL_Scancode scancode) const
{
	if (scancode == SDL_SCANCODE_UNKNOWN || scancode >= SDL_NUM_SCANCODES)
	{
		return false;
	}
	return m_CurrentKeyboardState[scancode];
}