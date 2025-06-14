#pragma once
#include <Singleton.h>

#include "Observer.h"

// I know, this is probably the 100th time that I created a Singleton
// I'm running out of time
class LevelManager final : public dae::Singleton<LevelManager>, public IObserver
{
public:
	LevelManager(const LevelManager&) = delete;
	LevelManager(LevelManager&&) noexcept = delete;
	LevelManager& operator=(const LevelManager&) = delete;
	LevelManager& operator=(LevelManager&&) noexcept = delete;

	void LoadNextLevel();
private:
	void OnNotify(dae::GameObject* object, EventID event) override;

	void GameOver();
	void HandleLevelRestart();

	friend Singleton<LevelManager>;
	LevelManager() = default;
	int m_CurrentLevel = 0;
};

