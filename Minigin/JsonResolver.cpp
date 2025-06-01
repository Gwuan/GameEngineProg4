#include "JsonResolver.h"

#include "GameObject.h"
#include <nlohmann/json.hpp>
#include <algorithm>

#include "TextureComponent.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "TextComponent.h"
#include "Transform.h"

bool JsonResolver::IsLevelDataValid(const nlohmann::json& levelData)
{
	return levelData.contains("name") && 
		   levelData.contains("gridSize") && 
		   levelData.contains("cellSize");
}

void JsonResolver::AddComponents(dae::GameObject& go, const nlohmann::json& goData)
{
	if (!goData.contains("components"))
		return;

	for (const nlohmann::json& component : goData["components"])
	{
		ComponentFactory::GetInstance().CreateComponent(go, component);
	}
}

bool JsonResolver::ExtractVector2(const std::string& objectKey, const nlohmann::json& data, glm::vec2& outResult)
{
	if (objectKey.empty() || !data.contains(objectKey))
		return false;

	const nlohmann::json& rawData = data[objectKey];

	if (!rawData.is_array() || rawData.size() != 2)
		return false;

	outResult = glm::vec2{rawData[0].get<float>(), rawData[1].get<float>()};
	return true;
}

std::vector<std::shared_ptr<dae::GameObject>> JsonResolver::RetrieveAllGameObjects(
    const std::string& jsonKey, const nlohmann::json& jsonData, const dae::Scene& scene)
{
    std::vector<std::shared_ptr<dae::GameObject>> gameObjects;
    
    if (!jsonData.contains(jsonKey) || !jsonData[jsonKey].is_array()) {
        return gameObjects;
    }
    
    for (const auto& goJsonData : jsonData[jsonKey]) 
	{
        auto go = RetrieveGameObject(goJsonData, scene);
        gameObjects.push_back(go);
        
        // Handle nested children recursively
        if (goJsonData.contains("children") && goJsonData["children"].is_array()) {
            // Recursively process all child objects
            auto childObjects = RetrieveAllGameObjects("children", goJsonData, scene);
            
            std::ranges::for_each(childObjects, [go](std::shared_ptr<dae::GameObject>& child) 
            {
                child->SetParent(go.get());
            });
            
            // Move all child objects into the main vector
            gameObjects.insert(
				gameObjects.end(),
				std::make_move_iterator(childObjects.begin()),
				std::make_move_iterator(childObjects.end())
			);
        }
    }
    
    return gameObjects;
}

std::shared_ptr<dae::GameObject> JsonResolver::RetrieveGameObject(
    const nlohmann::json& goData, const dae::Scene& scene)
{
    auto go = std::make_shared<dae::GameObject>();
    
    // check grid position first, then world position
    if (glm::vec2 gridPos; JsonResolver::ExtractVector2("gridPos", goData, gridPos)) 
	{
        go->GetTransform()->SetPosition(scene.GridToWorld(
            static_cast<uint32_t>(gridPos.x), 
            static_cast<uint32_t>(gridPos.y)));
    }
	else if (glm::vec2 position; JsonResolver::ExtractVector2("position", goData, position)) 
	{
        // Use direct world position if grid position not available
        go->GetTransform()->SetPosition(position);
    }
    
    JsonResolver::AddComponents(*go, goData);
    return go;
}

ComponentFactory::ComponentFactory()
{
	RegisterEngineComponents();
}

void ComponentFactory::Register(const std::string& componentType, ComponentCreator creator)
{
	m_Creators[componentType] = std::move(creator);
}

Component* ComponentFactory::CreateComponent(dae::GameObject& owner, const nlohmann::json& data)
{
	try
	{
		// Retrieve component type
		const std::string& componentType = data["type"].get_ref<const std::string&>();
		auto creatorIt = m_Creators.find(componentType);

		if (creatorIt == m_Creators.end())
		{
			std::cout << "Error while trying to create component from JsonData: No suitable create for " << componentType << " found\n";
			return nullptr;
		}

		return creatorIt->second(owner, data);
	}
	catch (const nlohmann::detail::exception& ex)
	{
		std::cout << "Error while trying to create component from jsonData: " << ex.what() << std::endl;
		return nullptr;
	}
}

void ComponentFactory::RegisterEngineComponents()
{
	this->Register("TextureComponent",
	[](dae::GameObject& go, const nlohmann::json& jsonData) -> Component*
	{
		if (!jsonData["texture"].is_string())
			throw std::runtime_error("The given texture path is not a string");

		return go.AddComponent<TextureComponent>(jsonData["texture"]);
	});

	this->Register("TextComponent",
	[](dae::GameObject& go, const nlohmann::json& jsonData) -> Component*
	{
		if (!jsonData.contains("text") || !jsonData["text"].is_string())
			throw std::runtime_error("The given text is not a string or not present");

		if (!jsonData.contains("font") || !jsonData["font"].is_string())
			throw std::runtime_error("The given font path is not a string");

		std::shared_ptr<dae::Font> font = dae::ResourceManager::GetInstance().LoadFont(jsonData["font"], jsonData["size"]);

		return go.AddComponent<TextComponent>(jsonData["text"], font);
	});
}