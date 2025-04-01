#pragma once
#include "Component.h"
#include "Observer.h"

class TextComponent;

class HudDemoComponent final : public Component, public IObserver
{
public:
	void FixedUpdate(const float) override {}
	void Update(const float) override {}
	void LateUpdate(const float) override {}
	void Render() const override {}
	void DebugRender() override {}

	void OnNotify(dae::GameObject* object, EventID event) override;

protected:
	template <typename T, typename ... Args>
	friend T* dae::GameObject::AddComponent(Args&&... args);

	HudDemoComponent(dae::GameObject& owner, const glm::vec2& offset = {0, 0});

private:
	int m_Lives;
	uint32_t m_Score;

	const std::string m_DefaultLivesText;
	const std::string m_DefaultScoreText;

	TextComponent* m_LivesTextComp = nullptr;
	TextComponent* m_ScoreTextComp = nullptr;
};

