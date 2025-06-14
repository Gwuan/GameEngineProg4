#pragma once
#include <Component.h>
class ScoreManager;

class LoadingScreen : public Component
{
public:
	void BeginPlay() override;
	void Update(const float deltaTime) override;
	void FixedUpdate(const float) override {}
	void LateUpdate(const float) override {}
	void Render() const override;

protected:
	template <typename T, typename... Args>
	friend T* dae::GameObject::AddComponent(Args&&... args);

	explicit LoadingScreen(dae::GameObject& owner) : Component(owner)
	{
	}

private:
	ScoreManager* m_pScoreManager = nullptr;
	float m_ElapsedTime = 0.f;
	static const float m_MaxLoadTime;
	bool m_LoadingMap = false;
};
