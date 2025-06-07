#pragma once
#include "Component.h"
#include "PeterPepperState.h"


class ColliderComponent;

class PeterPepperComponent final : public Component
{
public:
	void Update(const float deltaTime) override;
	void FixedUpdate(const float) override {}
	void LateUpdate(const float) override;
	void Render() const override {}

	PeterPepperComponent(const PeterPepperComponent&) = delete;
	PeterPepperComponent(PeterPepperComponent&&) noexcept = delete;
	PeterPepperComponent& operator=(const PeterPepperComponent&) = delete;
	PeterPepperComponent& operator=(PeterPepperComponent&&) noexcept = delete;

	void RequestShoot();
	bool IsShootRequested() const { return this->m_ShootRequested; }

protected:
	template <typename T, typename ... Args>
	friend T* dae::GameObject::AddComponent(Args&&... args);

	friend PeterPepperState;

	explicit PeterPepperComponent(dae::GameObject& owner);

private:
	bool m_ShootRequested = false;
	void PlaySoundOnOverlap(const ColliderComponent* otherCollider);


	std::unique_ptr<PeterPepperState> m_State;
};