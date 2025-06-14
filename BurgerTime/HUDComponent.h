#pragma once
#include "Component.h"
#include "Observer.h"

class ScoreManager;
class TextComponent;

class HUDComponent final : public Component
{
public:
	void BeginPlay() override;
	void FixedUpdate(const float) override {}
	void Update(const float) override {}
	void LateUpdate(const float) override {}
	void Render() const override;
	void DebugRender() override {}

protected:
	template <typename T, typename ... Args>
	friend T* dae::GameObject::AddComponent(Args&&... args);

	HUDComponent(dae::GameObject& owner);

private:
	ScoreManager* m_pScoreManager = nullptr;
};