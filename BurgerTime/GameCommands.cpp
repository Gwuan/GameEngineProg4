#include "GameCommands.h"

#include <chrono>
#include <SDL_syswm.h>

#include "ServiceAllocator.h"
#include "SoundSystem.hpp"
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

void SoundTestCommand::Execute()
{
	const auto startTime = std::chrono::high_resolution_clock::now();	
	ServiceAllocator::GetSoundSystem().PlaySoundEffect("../gameResources/sounds/level_start.wav", 0.5f);
	// ServiceAllocator::GetSoundSystem().PlayMusic("../gameResources/sounds/background_music.ogg", 1.f);
	const auto endTime = std::chrono::high_resolution_clock::now();

	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
	std::cout << "Play took: " << elapsed << std::endl;
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
