#include "Scene.h"
#include "GameObject.h"

#include <algorithm>
#include <chrono>
#include <ranges>

#include "SDLRenderer.h"
#include "Utils.hpp"

using namespace dae;

unsigned int Scene::m_idCounter = 0;


Scene::Scene(const std::string& name, const glm::vec2& gridSize, uint32_t cellSize)
: m_name(name),
  m_NameHash(),
  m_GridCellSize(cellSize)
{
	m_NameHash = std::hash<std::string>{}(name);
	InitializeGrid(gridSize);
	m_pMapConfig = std::make_unique<MapViewConfig>(m_Grid, static_cast<float>(m_GridCellSize));
}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	if (auto activeScene = SceneManager::GetInstance().GetActiveScene())
	{
		if (activeScene == this)
			object->BeginPlay();
	}
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::BeginPlay()
{
	for(auto& object : m_objects)
	{
		object->BeginPlay();
	}
}

void Scene::FixedUpdate(const float fixedTime)
{
	for (auto& object : m_objects)
	{
		object->FixedUpdate(fixedTime);
	}
}

void Scene::Update(const float deltaTime)
{
	for (auto& object : m_objects)
	{
			object->Update(deltaTime);
	}
}

void Scene::LateUpdate(const float deltaTime)
{
	for (uint32_t i{}; i < m_objects.size(); ++i)
	{
		auto& object = m_objects[i];

		if (object->NeedsDestroyed())
		{
			m_ObjectKillList.push_back(i);
		}
		else
		{
			object->LateUpdate(deltaTime);
		}
	}

	KillGameObjects();
}

void Scene::Render() const
{
	std::ranges::for_each(m_objects, [](const auto& object) 
		{
			object->Render();
		}
    );
}

void Scene::DebugRender()
{
    std::ranges::for_each(m_objects, [](const auto& object) 
		{
    		object->DebugRender();
		}
    );
}

std::pair<bool, glm::vec2> Scene::GridToWorld(uint32_t column, uint32_t row) const
{
	try
	{
        return std::make_pair(true, m_Grid.at(row).at(column).GetCenterPoint());
	}
	catch (const std::out_of_range& ex)
	{
		std::cout << "Invalid grid position: " << ex.what() << std::endl;;
		return std::make_pair(false, glm::vec2{});
	}
}

std::pair<bool, glm::ivec2> Scene::WorldToGrid(const glm::vec2& worldPos) const
{
	uint32_t rowIdx = 0;
	uint32_t colIdx = 0;
	for (const auto& row : m_Grid)
	{
		for (const auto& column : row)
		{
			if (IsPointInRectf(worldPos, column))
				return std::make_pair(true, glm::ivec2{colIdx, rowIdx});

			colIdx++;
		}
		rowIdx++;
		colIdx = 0;
	}

	return std::make_pair(false, glm::ivec2{});
}

dae::GameObject* Scene::FindObjectWithTag(const std::string& tag) const
{
	auto found = std::ranges::find_if(m_objects, [tag](const auto& object)
	{
		return tag == object->GetTag();
	});

	if (found != m_objects.end())
		return found->get();

	return nullptr;
}

std::vector<dae::GameObject*> Scene::FindObjectsWithTag(const std::string& tag) const
{
	std::vector<dae::GameObject*> result;

	for (const auto object : m_objects)
    {
        if (object->GetTag() == tag)
        {
            result.push_back(object.get());        }
    }

	return result;
}

void Scene::KillGameObjects()
{
    if (m_ObjectKillList.empty())
        return;

	// Eliminate index shifting problems by sorting
    std::ranges::sort(std::ranges::reverse_view(m_ObjectKillList));

	// Remove duplicates
	auto it = std::ranges::unique(m_ObjectKillList).begin(); 
	m_ObjectKillList.erase(it, m_ObjectKillList.end()); 

    for (auto index : m_ObjectKillList)
    {
        if (index < m_objects.size())
            m_objects.erase(m_objects.begin() + index);
    }

    m_ObjectKillList.clear();
}

void Scene::InitializeGrid(const glm::vec2& gridSize)
{
	const uint32_t totalCellsWidth = static_cast<uint32_t>(gridSize.x);
	const uint32_t totalCellsHeight = static_cast<uint32_t>(gridSize.y);
	const float cellSizeF = static_cast<float>(m_GridCellSize);

	m_Grid.resize(totalCellsHeight);

	for (uint32_t y{}; y < totalCellsHeight; ++y)
	{
		m_Grid[y].reserve(totalCellsWidth);

		for (uint32_t x{}; x < totalCellsWidth; ++x)
		{
			const glm::vec2 cellLeftBottom = {cellSizeF * x, cellSizeF * y};
			m_Grid[y].emplace_back(cellLeftBottom, cellSizeF, cellSizeF);
		}
	}
}