#include "FpsCounterComponent.h"
#include "TextComponent.h"
#include "ostream"
#include <iomanip>

FpsCounterComponent::FpsCounterComponent(dae::GameObject* owner)
	: Component(owner)
{}

void FpsCounterComponent::BeginPlay()
{
	m_TextComponent = GetOwner()->GetComponent<TextComponent>();
}

void FpsCounterComponent::Update(const float deltaTime)
{
	const float fps = 1.f / deltaTime;

	std::ostringstream stream;
	stream << std::fixed << std::setprecision(1) << fps;

	m_TextComponent->SetText(stream.str() + " FPS");
}
