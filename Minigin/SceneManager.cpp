#include "SceneManager.h"

#include <fstream>
#include <iostream>

#include "GameObject.h"
#include "Scene.h"
#include "Transform.h"
#include <nlohmann/json.hpp>

#include "JsonResolver.h"

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

dae::Scene& dae::SceneManager::CreateScene(const std::string& name, const glm::vec2& gridSize, uint32_t cellSize = 32)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name, gridSize, cellSize));
	m_scenes.push_back(scene);
	return *scene;
}

void dae::SceneManager::LoadSceneFromJson(const std::string& path)
{
	std::ifstream file(path);
	if (!file)
	{
		std::cout << "Failed to load the scene, will not perform loading transition\n";
		return;
	}

	nlohmann::json json;

	file >> json;

	try
	{
        if (!JsonResolver::IsLevelDataValid(json)) 
		{
			std::cout << "Failed to load the scene, will not perform loading transition\n";
			return;
        }

        const auto& gridSizeJson = json["gridSize"];
        if (!gridSizeJson.is_array() || gridSizeJson.size() != 2) 
		{
			std::cout << "Json level loading failed, gridSize not found or invalid\n";
			return;
        }

        glm::vec2 gridSize{};
		if (!JsonResolver::ExtractVector2("gridSize", json, gridSize))
			return;

        int cellSize = json["cellSize"].get<int>();
        const std::string& sceneName = json["name"].get_ref<const std::string&>();

        Scene& scene = CreateScene(sceneName, gridSize, cellSize);

        std::cout << "Loading scene with the name: " << sceneName << "..." << std::endl;

		auto allGameObjects = JsonResolver::RetrieveAllGameObjects("gameObjects", json, scene);

		std::ranges::for_each(allGameObjects, [&scene](std::shared_ptr<dae::GameObject> go)
		{
			scene.Add(go);
		});
	}
	catch (const nlohmann::detail::type_error& typeError)
	{
		std::cout << typeError.what() << std::endl;
	}
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
