#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update(const float deltaTime)
{
	for (auto& scene : m_scenes)
	{
		scene->Update(deltaTime);
	}
}

void dae::SceneManager::LateUpdate(const float deltaTime)
{
	for (auto& scene : m_scenes)
	{
		scene->LateUpdate(deltaTime);
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

void dae::SceneManager::DebugRender()
{
	for (const auto& scene : m_scenes)
	{
		scene->DebugRender();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}

void dae::SceneManager::BeginPlay()
{
	for(const auto& scene : m_scenes)
	{
		scene->BeginPlay();
	}
}

void dae::SceneManager::FixedUpdate(const float fixedTime)
{
	for (const auto& scene : m_scenes)
	{
		scene->FixedUpdate(fixedTime);
	}
}
