#pragma once
#include "Component.h"
#include <glm.hpp>

class CircularMovement final : public Component
{
	void FixedUpdate(const float fixedTimeStep) override;
	void Update(const float deltaTime) override;
	void LateUpdate(const float deltaTime) override;
	void Render() const override;

protected:
	explicit CircularMovement(dae::GameObject& owner);

	template <typename T, typename ... Args>
	friend T* dae::GameObject::AddComponent(Args&&... args);

private:
	bool m_UsingStaticCenterPoint = false;
	float m_Speed = 10;
	float m_Radius = 10;
	float m_Angle;
	glm::vec2 m_DefaultStartPosition;
};

