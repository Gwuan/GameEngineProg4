#pragma once
class Gamepad final
{
public:
	enum class GamepadButton
	{
		DPAD_UP = 0x0001,
		DPAD_DOWN = 0x0002,
		DPAD_LEFT = 0x0004,
		DPAD_RIGHT = 0x0008,

		START = 0x0010,
		BACK = 0x0020,

		LEFT_THUMB = 0x0040,
		RIGHT_THUMB = 0x0080,

		LEFT_SHOULDER = 0x0100,
		RIGHT_SHOULDER = 0x0200,

		BUTTON_A = 0x1000,
		BUTTON_B = 0x2000,
		BUTTON_X = 0x4000,
		BUTTON_Y = 0x8000,

		NONE = 0
	};

	Gamepad(uint32_t controllerIdx);
	~Gamepad();


	class GamepadImpl;
	GamepadImpl* m_pImpl;

public:
	void Update();

	// Continuous action
	bool IsButtonHoldDown(GamepadButton button, uint32_t controllerIdx) const;

	// Single action
	bool IsButtonPressed(GamepadButton button, uint32_t controllerIdx) const ;
	bool IsButtonReleased(GamepadButton button, uint32_t controllerIdx) const;


};

