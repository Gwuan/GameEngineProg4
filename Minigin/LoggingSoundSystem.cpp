#include "LoggingSoundSystem.h"

#include <iostream>

const std::string LoggingSoundSystem::m_LogPrefix{ "[SoundSystem Log] " };

void LoggingSoundSystem::PlaySoundEffect(const std::string& path, const float volume)
{
	m_pRealSoundSystem->PlaySoundEffect(path, volume);
	std::cout << m_LogPrefix << "Playing SoundEffect with path \'" << path << "\' with volume: " << volume << std::endl;
}

void LoggingSoundSystem::PlayMusic(const std::string& path, const float volume)
{
	m_pRealSoundSystem->PlayMusic(path, volume);
	std::cout << m_LogPrefix << "Playing Music with path \'" << path << "\' with volume: " << volume << std::endl;
}

void LoggingSoundSystem::StopMusic()
{
	m_pRealSoundSystem->StopMusic();
	std::cout << m_LogPrefix << "Music stopped" << std::endl;
}

void LoggingSoundSystem::PauseMusic()
{
	m_pRealSoundSystem->PauseMusic();
	std::cout << m_LogPrefix << "Music paused" << std::endl;
}

void LoggingSoundSystem::ResumeMusic()
{
	m_pRealSoundSystem->ResumeMusic();
	std::cout << m_LogPrefix << "Music resumed" << std::endl;
}

void LoggingSoundSystem::PauseAll()
{
	m_pRealSoundSystem->PauseAll();
	std::cout << m_LogPrefix << "Music & SoundEffects paused" << std::endl;
}

void LoggingSoundSystem::ResumeAll()
{
	m_pRealSoundSystem->ResumeAll();
	std::cout << m_LogPrefix << "Music & SoundEffects resumed" << std::endl;
}