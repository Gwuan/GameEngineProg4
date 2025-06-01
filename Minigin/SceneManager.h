#pragma once
#include <vector>
#include <string>
#include <memory>
#include <vec2.hpp>

#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name, const glm::vec2& gridSize, uint32_t cellSize);

		void LoadSceneFromJson(const std::string& path);

		void BeginPlay();

		void FixedUpdate(const float fixedTime);
		void Update(const float deltaTime);
		void LateUpdate(const float deltaTime);
		void Render();
		void DebugRender();


	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;
	};
}