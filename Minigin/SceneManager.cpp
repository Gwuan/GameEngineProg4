#include "SceneManager.h"

#include <fstream>
#include <iostream>

#include "GameObject.h"
#include "Scene.h"
#include "Transform.h"
#include <nlohmann/json.hpp>

#include "JsonResolver.h"
#include "Utils.hpp"

void dae::SceneManager::Update(const float deltaTime)
{
	if (m_ActiveScene) m_ActiveScene->Update(deltaTime);
}

void dae::SceneManager::LateUpdate(const float deltaTime)
{
	if (m_ActiveScene) m_ActiveScene->LateUpdate(deltaTime);
}

void dae::SceneManager::Render()
{
	if (m_ActiveScene) m_ActiveScene->Render();
}

void dae::SceneManager::DebugRender()
{
	if (m_ActiveScene) m_ActiveScene->DebugRender();
}

void dae::SceneManager::OnNotify(dae::GameObject*, EventID event)
{
	auto hasher = std::hash<std::string>();

    if (event == HashUtils::make_sdbm_hash("NewActiveScene"))
    {
        if (m_ActiveScene) m_ActiveScene->BeginPlay();
    }
}

void dae::SceneManager::SetNewActiveScene(Scene* scene)
{
	m_ActiveScene = scene;
	m_NewSceneSubj.Notify(nullptr, HashUtils::make_sdbm_hash("NewActiveScene"));
}

dae::SceneManager::SceneManager()
{
	m_NewSceneSubj.AddObserver(this);
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name, const glm::vec2& gridSize, uint32_t cellSize, bool instantActivate = true)
{
	const size_t newSceneNameHash = std::hash<std::string>{}(name);

	auto sceneIt = std::ranges::find_if(m_scenes, [newSceneNameHash](const std::shared_ptr<Scene>& scene)
	{
		return newSceneNameHash == scene->GetHashedName();
	});

	if (sceneIt != m_scenes.end())
	{
		SetNewActiveScene(sceneIt->get());
		return *m_ActiveScene;
	}

    std::shared_ptr<Scene> scene(new Scene(name, gridSize, cellSize));
	m_scenes.push_back(scene);

	if (instantActivate) SetNewActiveScene(scene.get());
	return *scene;
}

bool dae::SceneManager::DeleteScene(const std::string& name)
{
	const size_t deletedSceneHash = std::hash<std::string>{}(name);

	if (m_ActiveScene->GetHashedName() == deletedSceneHash)
		m_ActiveScene = nullptr;

	auto result = std::erase_if(m_scenes, [deletedSceneHash](const std::shared_ptr<Scene>& scene)
	{
		return scene->GetHashedName() == deletedSceneHash;
	});

	return result == 0;
}

dae::Scene* dae::SceneManager::LoadSceneFromJson(const std::string& path)
{
	std::ifstream file(path);
	if (!file)
	{
		std::cout << "Failed to load the scene, will not perform loading transition\n";
		return nullptr;
	}

	nlohmann::json json;

	file >> json;

	try
	{
        if (!JsonResolver::IsLevelDataValid(json)) 
		{
			std::cout << "Failed to load the scene, will not perform loading transition\n";
			return nullptr;
        }

        const auto& gridSizeJson = json["gridSize"];
        if (!gridSizeJson.is_array() || gridSizeJson.size() != 2) 
		{
			std::cout << "Json level loading failed, gridSize not found or invalid\n";
			return nullptr;
        }

        glm::vec2 gridSize{};
		if (!JsonResolver::ExtractVector2("gridSize", json, gridSize))
			return nullptr;

        int cellSize = json["cellSize"].get<int>();
        const std::string& sceneName = json["name"].get_ref<const std::string&>();

        Scene& scene = CreateScene(sceneName, gridSize, cellSize, false);

        std::cout << "Loading scene with the name: " << sceneName << "..." << std::endl;

		auto allGameObjects = JsonResolver::RetrieveAllGameObjects("gameObjects", json, scene);

		std::ranges::for_each(allGameObjects, [&scene](std::shared_ptr<dae::GameObject> go)
		{
			scene.Add(go);
		});

		SetNewActiveScene(&scene);
		return &scene;
	}
	catch (const nlohmann::detail::type_error& typeError)
	{
		std::cout << typeError.what() << std::endl;
		return nullptr;
	}
}

void dae::SceneManager::FixedUpdate(const float fixedTime)
{
	if (m_ActiveScene) m_ActiveScene->FixedUpdate(fixedTime);
}