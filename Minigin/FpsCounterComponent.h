#pragma once
#include "Component.h"

class TextComponent;

class FpsCounterComponent final : public Component
{
public:
	explicit FpsCounterComponent(dae::GameObject* owner);

	void BeginPlay() override;
	void Update(const float deltaTime) override;

private:
	TextComponent* m_TextComponent;
	float m_Timer{0.5f};
	float m_TotalElapsedTime{};
	uint32_t m_FrameCount{};
};

