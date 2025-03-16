#include "HudDemoComponent.h"

#include "ResourceManager.h"
#include "TextComponent.h"

HudDemoComponent::HudDemoComponent(dae::GameObject& owner, const glm::vec2& offset)
	: Component(owner),
	  m_Lives(3),
	  m_Score(),
	  m_DefaultLivesText("Lives: "),
	  m_DefaultScoreText("Score: ")
{
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	m_LivesTextComp = GetOwner().AddComponent<TextComponent>(m_DefaultLivesText + std::to_string(m_Lives), font);
	m_ScoreTextComp = GetOwner().AddComponent<TextComponent>(m_DefaultScoreText + std::to_string(m_Score), font);

	m_LivesTextComp->SetTextOffset(offset);
	m_ScoreTextComp->SetTextOffset(offset + glm::vec2(0, 36));
}
void HudDemoComponent::OnNotify(dae::GameObject*, EventID event)
{
	std::hash<std::string> hasher;

	if (event == hasher("PlayerDied"))
	{
		m_Lives--;
		m_LivesTextComp->SetText(m_DefaultLivesText + std::to_string(m_Lives));
	}
	if (event == hasher("HotDogKilled"))
	{
		m_Score += 100;
		m_ScoreTextComp->SetText(m_DefaultScoreText + std::to_string(m_Score));
	}
	if (event == hasher("PickleKilled"))
	{
		m_Score += 200;
		m_ScoreTextComp->SetText(m_DefaultScoreText + std::to_string(m_Score));
	}
}