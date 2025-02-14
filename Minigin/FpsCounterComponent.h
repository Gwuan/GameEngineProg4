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
	std::shared_ptr<TextComponent> m_TextComponent;

};

