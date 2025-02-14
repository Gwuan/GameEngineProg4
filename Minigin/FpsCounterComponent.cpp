#include "FpsCounterComponent.h"

#include <format>

#include "TextComponent.h"

FpsCounterComponent::FpsCounterComponent(dae::GameObject* owner)
	: Component(owner)
{}

void FpsCounterComponent::BeginPlay()
{
	m_TextComponent = GetOwner()->GetComponent<TextComponent>();
}

void FpsCounterComponent::Update(const float deltaTime)
{
	if(m_TextComponent == nullptr)
		return;

	m_TotalElapsedTime += deltaTime;
	m_FrameCount++;

	if(m_TotalElapsedTime >= m_Timer)
	{
		const float fps = static_cast<float>(m_FrameCount) / m_TotalElapsedTime;
		m_TextComponent->SetText(std::format("{:.1f} FPS", fps));

		m_TotalElapsedTime = 0;
		m_FrameCount = 0;
	}
}
