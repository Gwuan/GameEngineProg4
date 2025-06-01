#pragma once
#include <glm.hpp>
#include <nlohmann/json.hpp>
#include "Singleton.h"

namespace dae
{
	class Scene;
	class GameObject;
}

class Component;

namespace JsonResolver
{
	// Checks if the level data JSON is valid (required fields, types, etc.)
	bool IsLevelDataValid(const nlohmann::json& levelData);

	// Adds all components described in goData to the given GameObject
	void AddComponents(dae::GameObject& go, const nlohmann::json& goData);

	// Extracts glm::vec2 from JSON, returns true if successful
	bool ExtractVector2(const std::string& objectKey, const nlohmann::json& data, glm::vec2& outResult);

	// Creates a GameObject from JSON data (position, etc.)
	std::vector<std::shared_ptr<dae::GameObject>> RetrieveAllGameObjects(const std::string& jsonKey, const nlohmann::json& jsonData, const dae::Scene& scene);

	std::shared_ptr<dae::GameObject> RetrieveGameObject(const nlohmann::json& goData, const dae::Scene& scene);
}

class ComponentFactory final : public dae::Singleton<ComponentFactory>
{
public:
	ComponentFactory();

	using ComponentCreator = std::function<Component* (dae::GameObject&, const nlohmann::json&)>;

	void Register(const std::string& componentType, ComponentCreator creator);

	Component* CreateComponent(dae::GameObject& owner, const nlohmann::json& data);

private:
	void RegisterEngineComponents();

	std::unordered_map<std::string, ComponentCreator> m_Creators;
};