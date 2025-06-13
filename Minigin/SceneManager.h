#pragma once
#include "Singleton.h"
#include "Observer.h"

#include <glm.hpp>
#include <vector>
#include <memory>

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>, IObserver
	{
	public:
		Scene& CreateScene(const std::string& name, const glm::vec2& gridSize, uint32_t cellSize, bool instantActivate);

		bool DeleteScene(const std::string& name);

		Scene* LoadSceneFromJson(const std::string& path);

		void FixedUpdate(const float fixedTime);
		void Update(const float deltaTime);
		void LateUpdate(const float deltaTime);
		void Render();
		void DebugRender();

		Scene* GetActiveScene() const { return m_ActiveScene; }

		void OnNotify(dae::GameObject* object, EventID event) override;

		void SetNewActiveScene(Scene* scene);

	private:
		Subject m_NewSceneSubj{};
		Scene* m_ActiveScene = nullptr;

		friend class Singleton<SceneManager>;
		SceneManager();
		std::vector<std::shared_ptr<Scene>> m_scenes;
	};
}
