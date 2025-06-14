#include "LevelManager.h"

#include <filesystem>

#include "SceneManager.h"
#include "ScoreManager.h"
#include "Utils.hpp"

void LevelManager::OnNotify(dae::GameObject*, EventID event)
{
	switch (event)
	{
	case HashUtils::make_sdbm_hash("PlayerDied"):
		if (ScoreManager::GetInstance().GetExtraLives() == 0)
			GameOver();
		else
			HandleLevelRestart();

		break;
	case HashUtils::make_sdbm_hash("LevelCompleted"):
		LoadNextLevel();
		break;
	default: ;
	}
}

void LevelManager::GameOver()
{
	m_CurrentLevel = -1;
	dae::SceneManager::GetInstance().LoadSceneFromJson("../Data/Levels/GameOver.json");
}

void LevelManager::HandleLevelRestart()
{
	dae::SceneManager::GetInstance().LoadSceneFromJson("../Data/Levels/LoadScreen.json");
	
	const std::string nextFile = "../Data/Levels/Level" + std::to_string(m_CurrentLevel) + ".json";
	std::filesystem::path f{nextFile};
	if (std::filesystem::exists(f))
		dae::SceneManager::GetInstance().LoadSceneFromJson(nextFile);

}

void LevelManager::LoadNextLevel()
{
	m_CurrentLevel++;

	if (m_CurrentLevel <= 0)
	{
		dae::SceneManager::GetInstance().LoadSceneFromJson("../Data/Levels/MainMenu.json");
		return;;
	}

	const std::string nextFile = "../Data/Levels/Level" + std::to_string(m_CurrentLevel) + ".json";
	std::filesystem::path f{nextFile};
	if (std::filesystem::exists(f))
		dae::SceneManager::GetInstance().LoadSceneFromJson(nextFile);
	else
	{
		m_CurrentLevel = -1;
		dae::SceneManager::GetInstance().LoadSceneFromJson("../Data/Levels/WinScreen.json");
	}
}
