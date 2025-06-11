#include "Scene.h"
#include "GameObject.h"

#include <algorithm>
#include <chrono>

#include "SDLRenderer.h"

using namespace dae;

unsigned int Scene::m_idCounter = 0;


Scene::Scene(const std::string& name, const glm::vec2& gridSize, uint32_t cellSize)
: m_name(name),
  m_NameHash(),
  m_GridCellSize(cellSize)
{
	m_NameHash = std::hash<std::string>{}(name);
	InitializeGrid(gridSize);
}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
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

glm::vec2 Scene::GridToWorld(uint32_t column, uint32_t row) const
{
	try
	{
		return m_Grid.at(row).at(column);	
	}
	catch (const std::out_of_range& ex)
	{
		std::cout << "Invalid grid position: " << ex.what() << std::endl;;
		return glm::vec2{};
	}
}

void Scene::KillGameObjects()
{
	if (m_ObjectKillList.empty())
		return;
	for (auto& index : m_ObjectKillList)
	{
		m_objects.erase(m_objects.begin() + index);
	}

	m_ObjectKillList.clear();
}

void Scene::InitializeGrid(const glm::vec2& gridSize)
{
	const uint32_t totalCellsWidth = static_cast<uint32_t>(gridSize.x);
	const uint32_t totalCellsHeight = static_cast<uint32_t>(gridSize.y);

	m_Grid.resize(totalCellsHeight);

	for (uint32_t y{}; y < totalCellsHeight; ++y)
	{
		m_Grid[y].reserve(totalCellsWidth);

		for (uint32_t x{}; x < totalCellsWidth; ++x)
		{
			m_Grid[y].emplace_back(m_GridCellSize * x, m_GridCellSize * y);
		}
	}
}