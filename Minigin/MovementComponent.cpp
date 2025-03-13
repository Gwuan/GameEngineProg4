#include "MovementComponent.h"

MovementComponent::MovementComponent(dae::GameObject& owner)
	: Component(owner),
	  m_pOwnerTransform(owner.GetTransform())
{}

void MovementComponent::FixedUpdate(const float) {}

void MovementComponent::Update(const float deltaTime)
{
	auto newPosition = m_pOwnerTransform->GetLocalPosition() + 
		m_MovementDirection * m_MovementSpeed * deltaTime;

	m_pOwnerTransform->SetPosition(newPosition);

	// Reset movementDirection
	m_MovementDirection = {0.f, 0.f};
}

void MovementComponent::LateUpdate(const float) {}
void MovementComponent::Render() const {}

void MovementComponent::SetMovementSpeed(float speed)
{
	if (speed < 0.f)
	{
		throw std::runtime_error("Impossible to set movement speed below 0.");
	}

	m_MovementSpeed = speed;
}

void MovementComponent::Move(MoveDirection desiredDirection)
{
	switch (desiredDirection)
	{
	case MoveDirection::LEFT:
		m_MovementDirection += glm::vec2(-1.f, 0.f);
		break;
	case MoveDirection::RIGHT:
		m_MovementDirection += glm::vec2(1.f, 0.f);
		break;
	case MoveDirection::DOWN:
		m_MovementDirection += glm::vec2(0.f, 1.f);
		break;
	case MoveDirection::UP:
		m_MovementDirection += glm::vec2(0.f, -1.f);
		break;
	}	
}



void MovementComponent::Move(glm::vec2 direction)
{
	m_MovementDirection = normalize(direction);
}

void MovementComponent::Move(float x, float y)
{
	Move({x, y});
}



