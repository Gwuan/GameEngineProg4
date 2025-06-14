#pragma once
#include <Component.h>
class ScoreManager;

class WinScreen : public Component
{
public:
	void BeginPlay() override;
	void Update(const float) override {}
	void FixedUpdate(const float) override {}
	void LateUpdate(const float) override {}
	void Render() const override;

protected:
	template <typename T, typename... Args>
	friend T* dae::GameObject::AddComponent(Args&&... args);

	explicit WinScreen(dae::GameObject& owner) : Component(owner)
	{
	}

private:
	ScoreManager* m_pScoreManager = nullptr;
};

