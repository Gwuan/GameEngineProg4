#pragma once
#include <memory>

namespace dae
{
	class GameObject;
}

class PeterPepperState
{
public:
	PeterPepperState(dae::GameObject& peterPepper)
		: m_pPeter(&peterPepper) {}

	virtual ~PeterPepperState() = default;

	virtual std::unique_ptr<PeterPepperState> Update() = 0;

	virtual void OnEnter() {}
	virtual void OnExit() {}

protected:

	dae::GameObject* m_pPeter;
};

class PeterIdleState final : public PeterPepperState
{
public:
	PeterIdleState(dae::GameObject& peterPepper)
		: PeterPepperState(peterPepper) {}

	PeterIdleState(const PeterIdleState&) = delete;
	PeterIdleState(PeterIdleState&&) noexcept = delete;
	PeterIdleState& operator=(const PeterIdleState&) = delete;
	PeterIdleState& operator=(PeterIdleState&&) noexcept = delete;

	std::unique_ptr<PeterPepperState> Update() override;

	void OnEnter() override;
};

class PeterMoveState final : public PeterPepperState
{
public:
	PeterMoveState(dae::GameObject& peterPepper)
		: PeterPepperState(peterPepper) {}

	PeterMoveState(const PeterMoveState&) = delete;
	PeterMoveState(PeterMoveState&&) noexcept = delete;
	PeterMoveState& operator=(const PeterMoveState&) = delete;
	PeterMoveState& operator=(PeterMoveState&&) noexcept = delete;

	std::unique_ptr<PeterPepperState> Update() override;

	void OnEnter() override;
};