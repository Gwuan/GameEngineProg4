#include "CircularMovement.h"

void CircularMovement::FixedUpdate(const float) {}

void CircularMovement::Update(const float deltaTime)
{
    m_Angle += m_Speed * deltaTime; 

	float x;
	float y;

	if (m_UsingStaticCenterPoint)
	{
		x = m_DefaultStartPosition.x + m_Radius * cosf(m_Angle);
		y = m_DefaultStartPosition.y + m_Radius * sinf(m_Angle);
	}
	else
	{
		x = GetOwner().GetParent()->GetWorldPosition().x + m_Radius * glm::cos(m_Angle);
		y = GetOwner().GetParent()->GetWorldPosition().y + m_Radius * glm::sin(m_Angle);
	}


	std::cout << GetOwner().GetWorldPosition().x  << " " << GetOwner().GetWorldPosition().y << std::endl;

	GetOwner().SetPosition(x, y);

}

void CircularMovement::LateUpdate(const float) {}

void CircularMovement::Render() const{}

CircularMovement::CircularMovement(dae::GameObject& owner)
	: Component(owner),
	  m_Angle(0),
	  m_DefaultStartPosition(0.f, 0.f)
{
    if (!owner.GetParent())
    {
		m_UsingStaticCenterPoint = true;   
		m_DefaultStartPosition = owner.GetWorldPosition();
    }

}