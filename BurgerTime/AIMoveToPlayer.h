#pragma once
#include <Component.h>
#include <queue>
#include <vector>

#include "Scene.h"

class AIMoveToPlayer final : public Component
{
public:
	void BeginPlay() override;
	void Update(const float deltaTime) override;
	void FixedUpdate(const float) override {}
	void LateUpdate(const float) override {}
	void Render() const override {}

protected:
	template <typename T, typename... Args>
	friend T* dae::GameObject::AddComponent(Args&&... args);

	explicit AIMoveToPlayer(dae::GameObject& owner) : Component(owner){}
private:
	void GetSceneInfo();

	void GatherWalkables();
	std::vector<glm::ivec2> FindPath(const glm::ivec2& startCell, const glm::ivec2& goalCell) const;
	bool IsWalkable(const glm::ivec2& cell) const;

	dae::Scene* m_pCurrentScene = nullptr;
	std::unique_ptr<std::vector<std::vector<Rectf>>> m_CurrentMap = nullptr;
	Transform* m_pAiTransform = nullptr;

	std::vector<glm::ivec2> m_Path;

	std::vector<dae::GameObject*> m_pWalkables;

	static constexpr std::array<glm::ivec2, 4> directions
	{
		glm::ivec2(1, 0),
		glm::ivec2(-1, 0),
		glm::ivec2(0, 1),
		glm::ivec2(0, -1)
	};
};
