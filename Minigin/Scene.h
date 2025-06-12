#pragma once
#include "SceneManager.h"
#include <glm.hpp>

namespace dae
{
	class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name, const glm::vec2& gridSize, uint32_t cellSize, bool instantActivate);
	public:
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		void BeginPlay();
		void FixedUpdate(const float fixedTime);
		void Update(const float deltaTime);
		void LateUpdate(const float deltaTime);
		void Render() const;
		void DebugRender();

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		glm::vec2 GridToWorld(uint32_t column, uint32_t row) const;

		size_t GetHashedName() const { return this->m_NameHash; }

		bool operator==(const Scene& other) const
		{
			return this->m_NameHash == other.m_NameHash;
		}

	private:
		void KillGameObjects();

		void InitializeGrid(const glm::vec2& gridSize);

		explicit Scene(const std::string& name, const glm::vec2& gridSize, uint32_t cellSize);

		std::vector<std::vector<glm::vec2>> m_Grid;
		const uint32_t m_GridCellSize;

		std::string m_name;
		size_t m_NameHash;
		std::vector<uint32_t> m_ObjectKillList;
		std::vector<std::shared_ptr<GameObject>> m_objects{};

		static unsigned int m_idCounter; 
	};

}
