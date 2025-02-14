#pragma once
#include <iostream>

#include "GameObject.h"

namespace dae
{
	class GameObject;
}

// This is an abstract class
class Component
{
public:
	virtual void BeginPlay();
	virtual void Update(const float deltaTime);
	virtual void FixedUpdate(const float fixedTimeStep);
	virtual void Render() const;

	bool NeedDestroyed() const { return m_IsDead; }
	void  Destroy() { m_IsDead = true;}

protected:
	explicit Component(dae::GameObject* owner);
	virtual ~Component();

	dae::GameObject* GetOwner() const { return this->m_pOwner; }
	// void operator delete(void*) = delete;
	friend class dae::GameObject;
private:

	dae::GameObject* m_pOwner;
	bool m_IsDead = false;
};

