#include "GameCommands.h"

#include <chrono>
#include <SDL_syswm.h>

#include "LevelManager.h"
#include "PeterPepperComponent.h"
#include "ServiceAllocator.h"
#include "SoundSystem.hpp"
#include "Subject.h"
#include "Transform.h"


void MovePepperCommand::Execute()
{
	GetGameObject()->GetTransform()->MoveDirection += m_Direction;
}

void MoveSaltCommand::Execute()
{
	GetGameObject()->GetTransform()->MoveDirection += glm::vec2{m_Direction.x * 2, m_Direction.y *  2};
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

void SkipLevelCommand::Execute()
{
	LevelManager::GetInstance().LoadNextLevel();
}

void ShootPepper::Execute()
{
	if (auto peterComp = GetGameObject()->GetComponent<PeterPepperComponent>())
		peterComp->RequestShoot();
}
