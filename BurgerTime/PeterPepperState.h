#pragma once
#include <memory>

class PeterPepperComponent;
class SpriteAnimation;
class ColliderComponent;

namespace dae
{
	class GameObject;
}

class PeterPepperState
{
public:
	PeterPepperState(PeterPepperComponent& peterPepper)
		: m_pPeter(&peterPepper) {}

	virtual ~PeterPepperState() = default;

	virtual void OnEnter() {}
	virtual std::unique_ptr<PeterPepperState> Update(float deltaTime) = 0;
	virtual void OnExit() {}

protected:

	PeterPepperComponent* m_pPeter;
};

class PeterIdleState final : public PeterPepperState
{
public:
	PeterIdleState(PeterPepperComponent& peterPepper)
		: PeterPepperState(peterPepper) {}

	PeterIdleState(const PeterIdleState&) = delete;
	PeterIdleState(PeterIdleState&&) noexcept = delete;
	PeterIdleState& operator=(const PeterIdleState&) = delete;
	PeterIdleState& operator=(PeterIdleState&&) noexcept = delete;

	std::unique_ptr<PeterPepperState> Update(float deltaTime) override;

	void OnEnter() override;
};

class PeterMoveState final : public PeterPepperState
{
public:
	PeterMoveState(PeterPepperComponent& peterPepper)
		: PeterPepperState(peterPepper) {}

	PeterMoveState(const PeterMoveState&) = delete;
	PeterMoveState(PeterMoveState&&) noexcept = delete;
	PeterMoveState& operator=(const PeterMoveState&) = delete;
	PeterMoveState& operator=(PeterMoveState&&) noexcept = delete;

	std::unique_ptr<PeterPepperState> Update(float deltaTime) override;

	void OnEnter() override;
};

class PeterThrowPepperState final : public PeterPepperState
{
public:
	PeterThrowPepperState(PeterPepperComponent& peterPepper)
		: PeterPepperState(peterPepper) {}

	PeterThrowPepperState(const PeterThrowPepperState&) = delete;
	PeterThrowPepperState(PeterThrowPepperState&&) noexcept = delete;
	PeterThrowPepperState& operator=(const PeterThrowPepperState&) = delete;
	PeterThrowPepperState& operator=(PeterThrowPepperState&&) noexcept = delete;

	std::unique_ptr<PeterPepperState> Update(float deltaTime) override;

	void OnEnter() override;
	void OnExit() override;

private:
	static const float m_MaxTime;
	float m_TotalElapsedTime = 0.f;

	ColliderComponent* m_PepperTrigger = nullptr;
	SpriteAnimation* m_PepperAnimation = nullptr;
};