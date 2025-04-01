#pragma once
#include <cstdint>

#include "MovementComponent.h"

namespace dae
{
	class GameObject;
}

class MovementComponent;

class Command
{
public:
	explicit Command(dae::GameObject* object);
	virtual ~Command();
	virtual void Execute() = 0;

protected:
	dae::GameObject* GetGameObject() const { return m_pObject; }

private:
	dae::GameObject* m_pObject;
};

