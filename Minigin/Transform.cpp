#include "Transform.h"

#include <windows.h>

#include "GameObject.h"

Transform::Transform(dae::GameObject& owner)
    : m_Owner(owner),
	  m_LocalPosition(0.0f, 0.0f),
	  m_WorldPosition(0.0f, 0.0f),
	  m_IsDirty(true) {}

void Transform::SetPosition(float x, float y)
{
	m_LocalPosition = {x, y};
    MarkDirty();
}

void Transform::MarkDirty()
{
    if (m_IsDirty)  // Avoid unnecessary calls
        return;

    m_IsDirty = true;

    for (dae::GameObject* child : m_Owner.GetChildren())
    {
	    child->GetTransform()->MarkDirty();
    }
}

glm::vec2 Transform::GetWorldPosition()
{
    if (m_IsDirty)
    {
        UpdateWorldPosition();
    }
    return m_WorldPosition;
}

void Transform::UpdateWorldPosition()
{
    if (auto parent = m_Owner.GetParent())
    {
        auto world = parent->GetTransform()->GetWorldPosition();
        m_WorldPosition = world + m_LocalPosition;
    }
    else
    {
        m_WorldPosition = m_LocalPosition;
    }

    m_IsDirty = false;
}