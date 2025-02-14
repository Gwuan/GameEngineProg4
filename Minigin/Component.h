#pragma once
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

protected:
	explicit Component(dae::GameObject* owner);
	~Component() = default;


	dae::GameObject* GetOwner() const { return this->m_pOwner; }

private:
	dae::GameObject* m_pOwner;
};

