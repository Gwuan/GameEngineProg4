#pragma once
#include "SceneManager.h"

namespace dae
{
	class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
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

	private:
		void KillGameObjects();

		explicit Scene(const std::string& name);

		std::string m_name;
		std::vector<uint32_t> m_ObjectKillList;
		std::vector<std::shared_ptr<GameObject>> m_objects{};

		static unsigned int m_idCounter; 
	};

}
