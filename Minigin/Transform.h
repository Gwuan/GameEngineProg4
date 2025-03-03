#pragma once
#include "glm.hpp"

namespace dae
{
	class GameObject;
}

class Transform
{
public:
    Transform(dae::GameObject& owner);

    void SetPosition(float x, float y);

    glm::vec2 GetLocalPosition() const { return m_LocalPosition; }
    glm::vec2 GetWorldPosition();

    void MarkDirty();
    bool IsDirty() const { return m_IsDirty; } 

private:
    dae::GameObject& m_Owner;
    glm::vec2 m_LocalPosition;
    mutable glm::vec2 m_WorldPosition;
    mutable bool m_IsDirty;

    void UpdateWorldPosition();
};
