#pragma once
#include <memory>

#include "EnemyType.h"
#include "Observer.h"

class MovePepperCommand;
struct EnemyType;
class EnemyComponent;

class EnemyState
{
public:
	EnemyState(EnemyComponent& enemyComponent, const EnemyType& type)
		: m_pEnemy(&enemyComponent), m_pType(&type) {}

	virtual ~EnemyState() = default;

	virtual void OnEnter() {}
	virtual std::unique_ptr<EnemyState> Update(float deltaTime) = 0;
	virtual void OnExit() {}

protected:
	EnemyComponent* m_pEnemy;
	const EnemyType* m_pType;
};

class EnemyMoveState final : public EnemyState, public IObserver
{
public:
	EnemyMoveState(EnemyComponent& enemyComponent, const EnemyType& type)
		: EnemyState(enemyComponent, type) {}

	EnemyMoveState(const EnemyMoveState&) = delete;
	EnemyMoveState(EnemyMoveState&&) noexcept = delete;
	EnemyMoveState& operator=(const EnemyMoveState&) = delete;
	EnemyMoveState& operator=(EnemyMoveState&&) noexcept = delete;

	void OnNotify(dae::GameObject* object, EventID event) override;

	void OnEnter() override;
	std::unique_ptr<EnemyState> Update(float deltaTime) override;
	void OnExit() override;

private:
	Transform* m_pTransform = nullptr;
	SpriteAnimation* m_pAnimation = nullptr;

	bool m_OnLadder = false;
	bool m_IsCrunched = false;
	bool m_IsDead = false;
};

class EnemyCrushState final : public EnemyState
{
public:
	EnemyCrushState(EnemyComponent& enemyComponent, const EnemyType& type)
		: EnemyState(enemyComponent, type),
		  m_ElapsedTime(),
		  m_DestroyTime(type.crushAnimation.totalDuration) {}

	EnemyCrushState(const EnemyCrushState&) = delete;
	EnemyCrushState(EnemyCrushState&&) noexcept = delete;
	EnemyCrushState& operator=(const EnemyCrushState&) = delete;
	EnemyCrushState& operator=(EnemyCrushState&&) noexcept = delete;

	void OnEnter() override;
	std::unique_ptr<EnemyState> Update(float deltaTime) override;
private:
	float m_ElapsedTime{};
	float m_DestroyTime;
};

class EnemyDieState final : public EnemyState
{
public:
	EnemyDieState(EnemyComponent& enemyComponent, const EnemyType& type)
		: EnemyState(enemyComponent, type),
		  m_ElapsedTime(),
		  m_DestroyTime(type.dieAnimation.totalDuration) {}

	EnemyDieState(const EnemyDieState&) = delete;
	EnemyDieState(EnemyDieState&&) noexcept = delete;
	EnemyDieState& operator=(const EnemyDieState&) = delete;
	EnemyDieState& operator=(EnemyDieState&&) noexcept = delete;

	void OnEnter() override;
	std::unique_ptr<EnemyState> Update(float deltaTime) override;

private:
	float m_ElapsedTime{};
	float m_DestroyTime;
};