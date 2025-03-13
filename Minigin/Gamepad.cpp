#include <cstdint>
#include <iostream>
#include <SDL_syswm.h>

#include "Windows.h"
#include "Xinput.h"
#include "GamePad.h"
#include "imgui.h"
#include "imgui_internal.h"

class Gamepad::GamepadImpl
{
public:
	GamepadImpl(uint32_t controllerIdx);

	void Update();

	// Continuous action
	bool IsButtonHoldDown(GamepadButton button, uint32_t controllerIdx);

	// Single action
	bool IsButtonPressed(GamepadButton button, uint32_t controllerIdx) const;
	bool IsButtonReleased(GamepadButton button, uint32_t) const;

private:
	XINPUT_STATE m_PreviousState{};
	XINPUT_STATE m_CurrentState{};

	uint32_t m_ControllerIdx = 0;

	WORD m_ButtonsPressedThisFrame{};
	WORD m_ButtonsReleasedThisFrame{};
};

Gamepad::Gamepad(uint32_t controllerIdx)
{
	m_pImpl = new GamepadImpl(controllerIdx);
}

Gamepad::~Gamepad()
{
	delete m_pImpl;
	m_pImpl = nullptr;
}

void Gamepad::Update()
{
	m_pImpl->Update();
}

bool Gamepad::IsButtonHoldDown(GamepadButton button, uint32_t controllerIdx) const
{
	return m_pImpl->IsButtonHoldDown(button, controllerIdx);
}

bool Gamepad::IsButtonPressed(GamepadButton button, uint32_t controllerIdx) const
{
	return m_pImpl->IsButtonPressed(button, controllerIdx);
}

bool Gamepad::IsButtonReleased(GamepadButton button, uint32_t controllerIdx) const
{
	return m_pImpl->IsButtonReleased(button, controllerIdx);
}

Gamepad::GamepadImpl::GamepadImpl(uint32_t controllerIdx)
{
	m_ControllerIdx = controllerIdx;
}

void Gamepad::GamepadImpl::Update()
{
	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIdx, &m_CurrentState);


	auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;

	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
}

bool Gamepad::GamepadImpl::IsButtonHoldDown(GamepadButton button, uint32_t )
{
	return ((static_cast<unsigned>(button) & m_CurrentState.Gamepad.wButtons) != 0);
}

bool Gamepad::GamepadImpl::IsButtonPressed(GamepadButton button, uint32_t ) const
{
	return ((static_cast<unsigned>(button) & m_ButtonsPressedThisFrame) != 0);
}

bool Gamepad::GamepadImpl::IsButtonReleased(GamepadButton button, uint32_t) const
{
	return ((static_cast<unsigned>(button) & m_ButtonsReleasedThisFrame) != 0);
}

