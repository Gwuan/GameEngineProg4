#include "GameCommands.h"
#include "SubjectComponent.h"

MovePepperCommand::MovePepperCommand(dae::GameObject* object, MoveDirection direction)
	: Command(object),
	  m_Direction(direction)
{
	m_pMovementComp = object->GetComponent<MovementComponent>();
}

void MovePepperCommand::Execute()
{
	m_pMovementComp->Move(m_Direction);
}

MoveSaltCommand::MoveSaltCommand(dae::GameObject* object, MoveDirection direction)
	: Command(object),
	  m_Direction(direction)
{
	m_pMovementComp = object->GetComponent<MovementComponent>();
	m_pMovementComp->SetMovementSpeed(100.0f);
}

void MoveSaltCommand::Execute()
{
	m_pMovementComp->Move(m_Direction);
}

void DieCommand::Execute()
{
	GetGameObject()->GetSubject()->Notify(GetGameObject(), "PlayerDied");
}

void KillHotDogCommand::Execute()
{
	GetGameObject()->GetSubject()->Notify(GetGameObject(), "HotDogKilled");
}

void KillPickleCommand::Execute()
{
	GetGameObject()->GetSubject()->Notify(GetGameObject(), "PickleKilled");
}

