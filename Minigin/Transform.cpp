#include "Transform.h"
#include "GameObject.h" 

Transform::Transform(dae::GameObject& owner, glm::vec2 pos)
    : Component(owner),
	  MoveDirection(0.f, 0.f),
	  m_LocalPosition(pos),
	  m_WorldPosition(0.f, 0.f),
	  m_MoveSpeed(50.f),
	  m_IsDirty(true)
{}

void Transform::Update(const float deltaTime)
{
    if (GetOwner().IsStatic() || !m_MovementEnabled)
        return;

	if (m_HorizontalMovementDisabled)
    {
	    MoveDirection.x = 0.f;
    }
    if (m_VerticalMovementDisabled)
    {
	    MoveDirection.y = 0.f;
    }

    if (glm::length<2>(MoveDirection) >= 0.2f)
    {
        m_Velocity = glm::normalize(MoveDirection) * m_MoveSpeed;
        m_Velocity.y *= -1.f;

    	m_LocalPosition += m_Velocity * deltaTime;
        MarkDirty();
    }
}

void Transform::LateUpdate(float)
{
    if (m_Velocity != glm::vec2{})
    {
		m_ForwardVector = glm::normalize(m_Velocity);
    }
    m_Velocity = glm::vec2{};
    MoveDirection = glm::vec2{};
}

void Transform::SetPosition(glm::vec2 newPosition)
{
    if (GetOwner().IsStatic())
        return;

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