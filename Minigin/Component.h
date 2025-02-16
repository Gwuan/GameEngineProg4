#pragma once
#include "GameObject.h"

namespace dae
{
	class GameObject;
}

class Component
{
public:
	virtual void BeginPlay();
	virtual void Update(const float deltaTime);
	virtual void FixedUpdate(const float fixedTimeStep);
	virtual void Render() const;

	void Destroy() { m_IsDead = true;}
	bool NeedDestroyed() const { return m_IsDead; }

	Component() = delete;

	Component(const Component& other) = delete;
	Component(Component&& other) noexcept = delete;
	Component& operator=(const Component& other) = delete;
	Component& operator=(Component&& other) noexcept = delete;
protected:
	explicit Component(dae::GameObject* owner);
	virtual ~Component();

	dae::GameObject* GetOwner() const { return this->m_pOwner; }
private:

	dae::GameObject* m_pOwner;
	bool m_IsDead = false;
};

