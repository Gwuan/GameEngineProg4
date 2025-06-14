#pragma once

#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "Command.h"
#include "Gamepad.h"
#include "Singleton.h"

#include <SDL_keyboard.h>

class Gamepad;

namespace dae
{
	enum class InputAction : uint8_t
	{
		PRESSED,
		RELEASED,
		HOLD
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager() override;

		bool ProcessInput();
		void HandleBindedInput();

		void BindCommand(unsigned int controllerIdx, Gamepad::GamepadButton button,
			SDL_Scancode keyboardKey, InputAction inputAction, std::unique_ptr<Command> command);

		bool UnbindCommand(unsigned int controllerIdx, Gamepad::GamepadButton button,
			SDL_Scancode keyboardKey, InputAction inputAction);

	private:
		void KillUnbindedCommands();

		bool IsKeyPressed(SDL_Scancode scancode) const;
		bool IsKeyReleased(SDL_Scancode scancode) const;
		bool IsKeyHoldDown(SDL_Scancode scancode) const;
		
		using ControllerKey = std::tuple<unsigned int, Gamepad::GamepadButton, SDL_Scancode, InputAction>;
		// Apparently a map container likes to corrupt my data somehow
		using InputMap = std::vector<std::pair<ControllerKey, std::unique_ptr<Command>>>;
		InputMap m_BindedCommands;

		std::vector<ControllerKey> m_PendingKillList;

		const Uint8* m_CurrentKeyboardState;
		std::vector<std::unique_ptr<Gamepad>> m_Gamepads;

		// Keyboard Related
		bool m_PreviousKeyboardState[SDL_NUM_SCANCODES]{};
	};
}