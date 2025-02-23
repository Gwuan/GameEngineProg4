#pragma once
#include "glm.hpp"

namespace dae
{
	class GameObject;
}

class Transform
{
public:
    Transform();

    void SetPosition(dae::GameObject& owner, float x, float y);
    void SetPosition(dae::GameObject& owner, const glm::vec2& position);

    glm::vec2 GetLocalPosition() const { return m_LocalPosition; }
    glm::vec2 GetWorldPosition(dae::GameObject& owner);

    void MarkDirty();  // Marks this transform as dirty
    bool IsDirty() const { return m_IsDirty; } 

private:
    glm::vec2 m_LocalPosition;
    mutable glm::vec2 m_WorldPosition;
    mutable bool m_IsDirty;

    void UpdateWorldPosition(dae::GameObject& owner) const;
};
