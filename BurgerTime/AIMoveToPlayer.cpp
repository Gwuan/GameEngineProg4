#include "AIMoveToPlayer.h"

#include <set>

#include "AINode.hpp"
#include "SceneManager.h"
#include "Transform.h"
#include "Utils.hpp"
#include "DataTypes.hpp"
#include "GameCommands.h"
#include <glm.hpp>
#include <algorithm>

void AIMoveToPlayer::BeginPlay()
{
	GetSceneInfo();
}

void AIMoveToPlayer::Update(const float)
{
	if (!m_pCurrentScene || !m_pAiTransform)
	{
		GetSceneInfo();
		return;
	}


	auto player = m_pCurrentScene->FindObjectWithTag("Player");
	if (!player)
		return;

	auto playerTransform = player->GetTransform();
	auto goal = m_pCurrentScene->WorldToGrid(playerTransform->GetWorldPosition());

	// Clear path if player moved significantly
	if (!m_Path.empty() && goal.first)
	{
		glm::ivec2 currentGoal = m_Path.back();

		const auto& currentGoalWorld = m_pCurrentScene->GridToWorld(currentGoal.x, currentGoal.y);
		const auto& goalWorld = m_pCurrentScene->GridToWorld(goal.second.x, goal.second.y);

		const bool oneNotInMap = !currentGoalWorld.first || !goalWorld.first;

		if (oneNotInMap || glm::length(glm::vec2(currentGoalWorld.second - goalWorld.second)) >= 1.0f) // Adjust threshold
		{
			m_Path.clear();
		}
	}

	// search path if empty
	if (m_Path.empty())
	{
		auto start = m_pCurrentScene->WorldToGrid(m_pAiTransform->GetWorldPosition());


		if (start.first && goal.first)
		{
			const glm::ivec2& startCell = start.second;
			const glm::ivec2& goalCell = goal.second;

			m_Path = FindPath(startCell, goalCell);
		}
	}

	// FollowPath
	if (m_Path.size() > 1)
	{
		auto currentTarget = m_pCurrentScene->GridToWorld(m_Path[1].x, m_Path[1].y );
		if (!currentTarget.first)
			return;

		glm::vec2 direction = currentTarget.second - m_pAiTransform->GetWorldPosition();

		float distance = glm::length(direction);

		if (distance < 0.5f)
		{
			// move to the following waypoint
			m_Path.erase(m_Path.begin());
			return;
		}

		direction /= distance;
		direction.y *= -1;

		m_pAiTransform->MoveDirection = direction;
	}
}

void AIMoveToPlayer::GetSceneInfo()
{
	m_pCurrentScene = dae::SceneManager::GetInstance().GetActiveScene();

	m_pAiTransform = GetOwner().GetTransform();

	if (m_pCurrentScene)
	{
		m_CurrentMap = std::make_unique<std::vector<std::vector<Rectf>>>(m_pCurrentScene->GetMap());
		GatherWalkables();
	}

}

void AIMoveToPlayer::GatherWalkables()
{
	auto platforms = m_pCurrentScene->FindObjectsWithTag("Platform");
	auto ladders = m_pCurrentScene->FindObjectsWithTag("Ladder");
	auto ladderEntries = m_pCurrentScene->FindObjectsWithTag("LadderEntry");

	m_pWalkables.insert(m_pWalkables.end(), platforms.begin(), platforms.end());
	m_pWalkables.insert(m_pWalkables.end(), ladders.begin(), ladders.end());
	m_pWalkables.insert(m_pWalkables.end(), ladderEntries.begin(), ladderEntries.end());
}

// Simple BFS
std::vector<glm::ivec2> AIMoveToPlayer::FindPath(const glm::ivec2& startCell, const glm::ivec2& goalCell) const
{
	std::queue<AINode*> q;
	std::set<glm::ivec2> visited;
	std::vector<AINode*> allNodes;

	auto start = new AINode(startCell);
	q.push(start);
	allNodes.push_back(start);

	visited.insert(startCell);

	AINode* path = nullptr;

	while (!q.empty())
	{
		AINode* current = q.front();
		q.pop();

		if (current->pos == goalCell)
		{
			path = current;
			break;
		}

		for (auto& dir : directions)
		{
			glm::ivec2 next = current->pos + dir;

			if (visited.find(next) == visited.end())
			{
				if (IsWalkable(next))
				{
					auto newNode = new AINode(next, current);
					q.push(newNode);
					allNodes.push_back(newNode);
					visited.insert(next);
				}
			}
		}
	}

	std::vector<glm::ivec2> finalRoute;

	while (path)
	{
		finalRoute.push_back(path->pos);
		path = path->parent;
	}
	std::ranges::reverse(finalRoute);

	for (AINode* node : allNodes)
	{
		delete node;
		node = nullptr;
	}

	allNodes.clear();

	return finalRoute;
}

bool AIMoveToPlayer::IsWalkable(const glm::ivec2& cell) const  
{
	const auto& currentPos = m_pCurrentScene->GridToWorld(cell.x, cell.y);
	if (!currentPos.first)
		return false;

	for (const auto walk : m_pWalkables)
	{
		const auto& walkWorld = walk->GetTransform()->GetWorldPosition();
		const float distance = glm::length(walkWorld - currentPos.second);
		if (distance <= 7.f)
			return true;
	}

	return false;
}
