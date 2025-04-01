#include "CircularMovement.h"

void CircularMovement::FixedUpdate(const float) {}

void CircularMovement::Update(const float deltaTime)
{
	switch (m_Direction)
	{
	case RotationDirection::LEFT:
		m_Angle += m_Speed * deltaTime;
		break;
	case RotationDirection::RIGHT:
		m_Angle -= m_Speed * deltaTime;
		break;
	}


	const float x{ m_CenterPoint.x + (cosf(m_Angle) * m_Radius) };
	const float y{ m_CenterPoint.y + (sinf(m_Angle) * m_Radius) };

	m_OwnerTransform->SetPosition(x, y);
}

void CircularMovement::LateUpdate(const float) {}

void CircularMovement::Render() const{}

CircularMovement::CircularMovement(dae::GameObject& owner, RotationDirection desiredDirection)
	: Component(owner),
	  m_Speed(5),
	  m_Radius(25),
	  m_Angle(0),
	  m_OwnerTransform(owner.GetTransform()),
	  m_CenterPoint(m_OwnerTransform->GetLocalPosition()),
	  m_Direction(desiredDirection)
{
}