#include "GameCommands.h"

#include <chrono>
#include <SDL_syswm.h>

#include "ServiceAllocator.h"
#include "SoundSystem.hpp"
#include "Subject.h"

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

void SoundTestCommand::Execute()
{
	ServiceAllocator::GetSoundSystem().PlaySoundEffect("../gameResources/sounds/level_start.wav", 0.5f);
}

void StartMusicCommand::Execute()
{
	ServiceAllocator::GetSoundSystem().PlayMusic("../gameResources/sounds/background_music.ogg", 0.5f);
}

void StopMusicCommand::Execute()
{
	ServiceAllocator::GetSoundSystem().StopMusic();
}

void PauseMusicCommand::Execute()
{
	ServiceAllocator::GetSoundSystem().PauseMusic();
}

void ResumeMusicCommand::Execute()
{
	ServiceAllocator::GetSoundSystem().ResumeMusic();
}

void PauseAllAudioCommand::Execute()
{
	ServiceAllocator::GetSoundSystem().PauseAll();
}

void ResumeAllAudioCommand::Execute()
{
	ServiceAllocator::GetSoundSystem().ResumeAll();
}
