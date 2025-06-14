#pragma once
#include <Component.h>
class GameOverScreen : public Component
{
public:
	void Update(const float) override {}
	void FixedUpdate(const float) override{}
	void LateUpdate(const float) override {}
	void Render() const override;

protected:
	template <typename T, typename ... Args>
	friend T* dae::GameObject::AddComponent(Args&&... args);

	explicit GameOverScreen(dae::GameObject& owner);
};

