#include "Transform.h"
#include "GameObject.h" 

Transform::Transform(dae::GameObject& owner, glm::vec2 pos)
    : Component(owner),
	  velocity(0.f, 0.f),
	  m_LocalPosition(pos),
	  m_WorldPosition(0.f, 0.f),
	  m_MoveSpeed(50.f),
	  m_IsDirty(true)
{}

void Transform::Update(const float deltaTime)
{
    if (glm::length<2>(velocity) >= 0.2f)
    {
        velocity = (glm::normalize(velocity) * m_MoveSpeed) * deltaTime;
        velocity.y *= -1.f;
    	m_LocalPosition += velocity;
        MarkDirty();

    	velocity.x = 0.f;
        velocity.y = 0.f;
    }
}

void Transform::SetPosition(glm::vec2 newPosition)
{
    m_LocalPosition = newPosition;
    MarkDirty();
}

void Transform::SetPosition(float x, float y)
{
    SetPosition({x, y});
}

void Transform::MarkDirty()
{
    if (m_IsDirty)  // Avoid unnecessary calls
        return;

    m_IsDirty = true;

    for (dae::GameObject* child : GetOwner().GetChildren())
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
    if (auto parent = GetOwner().GetParent())
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