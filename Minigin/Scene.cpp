#include "Scene.h"
#include "GameObject.h"

#include <algorithm>
#include <chrono>

using namespace dae;

unsigned int Scene::m_idCounter = 0;


Scene::Scene(const std::string& name) : m_name(name) {}

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
	for (const auto& object : m_objects)
	{
		object->Render();
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