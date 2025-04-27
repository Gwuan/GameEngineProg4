#pragma once
#include "GameObject.h"

namespace dae
{
	class GameObject;
}

class Component
{
public:
	virtual void BeginPlay() {};
	virtual void Update(const float deltaTime) = 0;
	virtual void FixedUpdate(const float fixedTimeStep) = 0;
	virtual void LateUpdate(const float deltaTime) = 0;
	virtual void Render() const = 0;
	virtual void DebugRender() {}

	void Destroy() { m_IsDead = true;}
	bool NeedDestroyed() const { return m_IsDead; }

	virtual ~Component() = default;

	Component(const Component& other) = delete;
	Component(Component&& other) noexcept = delete;
	Component& operator=(const Component& other) = delete;
	Component& operator=(Component&& other) noexcept = delete;
	dae::GameObject& GetOwner() const { return m_Owner; }

protected:
	explicit Component(dae::GameObject& owner)
		: m_IsDead(false), m_Owner(owner) {}

private:
	bool m_IsDead;
	dae::GameObject& m_Owner;
};