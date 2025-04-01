#include "Command.h"

#include "GameObject.h"
#include "MovementComponent.h"
#include "SubjectComponent.h"

Command::Command(dae::GameObject* object)
	: m_pObject(object)
{}

Command::~Command()
{
	m_pObject = nullptr;
}