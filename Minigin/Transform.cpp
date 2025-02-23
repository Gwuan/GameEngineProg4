#include "Transform.h"
#include "GameObject.h"

Transform::Transform()
    : m_LocalPosition(0.0f, 0.0f), m_WorldPosition(0.0f, 0.0f), m_IsDirty(true) {}

void Transform::SetPosition(dae::GameObject& owner, float x, float y)
{
	m_LocalPosition = {x, y};
    MarkDirty();

    // Mark all children as dirty, doesn't seem to work either
    for (auto child : owner.GetChildren())
    {
        if (child) 
        {
            child->GetTransform().MarkDirty();
        }
    }
}

void Transform::SetPosition(dae::GameObject& owner, const glm::vec2& position)
{
    SetPosition(owner, position.x, position.y);
}

void Transform::MarkDirty()
{
    m_IsDirty = true;
}

glm::vec2 Transform::GetWorldPosition(dae::GameObject& owner)
{
    if (m_IsDirty)
    {
        UpdateWorldPosition(owner);
    }
    return m_WorldPosition;
}

void Transform::UpdateWorldPosition(dae::GameObject& owner) const
{
    if (!m_IsDirty)
        return;

    if (auto parent = owner.GetParent())
    {
        auto world = parent->GetTransform().GetWorldPosition(*parent);
        m_WorldPosition = world + m_LocalPosition;
    }
    else
    {
        m_WorldPosition = m_LocalPosition;
    }

    m_IsDirty = false;
}
