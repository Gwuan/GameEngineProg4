#pragma once
#include "Component.h"

class ColliderComponent;

class PeterPepperComponent final : public Component
{
public:
	void Update(const float) override {}
	void FixedUpdate(const float) override {}
	void LateUpdate(const float) override {}
	void Render() const override {}

	PeterPepperComponent(const PeterPepperComponent&) = delete;
	PeterPepperComponent(PeterPepperComponent&&) noexcept = delete;
	PeterPepperComponent& operator=(const PeterPepperComponent&) = delete;
	PeterPepperComponent& operator=(PeterPepperComponent&&) noexcept = delete;

protected:
	template <typename T, typename ... Args>
	friend T* dae::GameObject::AddComponent(Args&&... args);

	explicit PeterPepperComponent(dae::GameObject& owner);

private:
	void PlaySoundOnOverlap(const ColliderComponent* otherCollider);

};