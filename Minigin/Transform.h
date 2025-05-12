#pragma once
#include "Component.h"
#include "glm.hpp"

class Transform final : public Component
{
public:
    void BeginPlay() override {}
    void Update(const float deltaTime) override;
    void FixedUpdate(const float) override {}
    void LateUpdate(float deltaTime) override;
    void Render() const override {}
    void DebugRender() override {}

	explicit Transform(dae::GameObject& owner, glm::vec2 pos = {0.f, 0.f});
	~Transform() override = default;

    Transform(const Transform&) = delete;
    Transform(Transform&&) noexcept = delete;
    Transform& operator=(const Transform&) = delete;
    Transform& operator=(Transform&&) noexcept = delete;

    void SetPosition(glm::vec2 newPosition);
    void SetPosition(float x, float y);

    void SetMoveSpeed(float speed) { m_MoveSpeed = speed >= 0.f ? speed : 0; }
    float GetMoveSpeed() const { return m_MoveSpeed; }

    glm::vec2 MoveDirection;

    glm::vec2 GetVelocity() const { return m_Velocity; }
    glm::vec2 GetForwardVector() const { return m_ForwardVector; }

    glm::vec2 GetLocalPosition() const { return m_LocalPosition; }
    glm::vec2 GetWorldPosition();

    void MarkDirty();
    bool IsDirty() const { return m_IsDirty; } 

private:
	void UpdateWorldPosition();

	glm::vec2 m_LocalPosition;
    mutable glm::vec2 m_WorldPosition;
    float m_MoveSpeed;
	mutable bool m_IsDirty;

    glm::vec2 m_Velocity = glm::vec2{0, 0};
    glm::vec2 m_ForwardVector = glm::vec2{1.f, 0};
};