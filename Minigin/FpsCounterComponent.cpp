#include "FpsCounterComponent.h"

#include <format>

#include "TextComponent.h"

FpsCounterComponent::FpsCounterComponent(dae::GameObject& owner)
	: Component(owner),
	  m_TextComponent(nullptr)
{}

FpsCounterComponent::~FpsCounterComponent()
{
	m_TextComponent = nullptr;
}

void FpsCounterComponent::BeginPlay()
{
	m_TextComponent = GetOwner().GetComponent<TextComponent>();
}

void FpsCounterComponent::Update(const float deltaTime)
{
	if(m_TextComponent == nullptr)
		return;

	m_TotalElapsedTime += deltaTime;
	m_FrameCount++;

	if(m_TotalElapsedTime >= m_TimeTreshHold)
	{
		const float fps = static_cast<float>(m_FrameCount) / m_TotalElapsedTime;
		m_TextComponent->SetText(std::format("{:.1f} FPS", fps));

		m_TotalElapsedTime -= m_TimeTreshHold;
		m_FrameCount = 0;
	}
}
