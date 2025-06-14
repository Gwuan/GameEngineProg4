#pragma once
#include <Singleton.h>

#include "Observer.h"

class ScoreManager final : public dae::Singleton<ScoreManager>, public IObserver
{
public:
	ScoreManager(const ScoreManager&) = delete;
	ScoreManager(ScoreManager&&) noexcept = delete;
	ScoreManager& operator=(const ScoreManager&) = delete;
	ScoreManager& operator=(ScoreManager&&) noexcept = delete;

	void OnNotify(dae::GameObject* object, EventID event) override;

	uint32_t GetExtraLives() const { return m_Player1Lives; }
	uint32_t GetScore() const { return m_Player1Score; }
	uint32_t GetAmountOfPepper() const { return m_AmountOfPepper; }

private:

	friend class Singleton<ScoreManager>;
	ScoreManager() = default;
	uint32_t m_Player1Score{};

	uint32_t m_Player1Lives{3};

	uint32_t m_AmountOfPepper{5};
};