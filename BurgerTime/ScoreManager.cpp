#include "ScoreManager.h"

#include "EnemyType.h"
#include "GameObject.h"
#include "Utils.hpp"

void ScoreManager::OnNotify(dae::GameObject*, EventID event)
{
	switch (event)
	{
	case HashUtils::make_sdbm_hash("OnEnemyDeath"):
			m_Player1Score += 100;
	case HashUtils::make_sdbm_hash("HotdogKilled"):
			m_Player1Score += g_MrHotDog.score;
		break;
	case HashUtils::make_sdbm_hash("PickleKilled"):
			m_Player1Score += g_MrPickle.score;
		break;
	case HashUtils::make_sdbm_hash("EggKilled"):
			m_Player1Score += g_MrHotDog.score;
	case HashUtils::make_sdbm_hash("Crushed"):
			m_Player1Score += 500;
		break;
	}
}