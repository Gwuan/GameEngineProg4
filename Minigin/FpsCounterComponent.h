#pragma once
#include "Component.h"

class TextComponent;

class FpsCounterComponent final : public Component
{
public:
	void BeginPlay() override;

	void FixedUpdate(const float) override {}
	void Update(const float deltaTime) override;
	void LateUpdate(const float) override {}
	void Render() const override {}

	FpsCounterComponent() = delete;
	virtual ~FpsCounterComponent() override;

	FpsCounterComponent(const FpsCounterComponent& other) = delete;
	FpsCounterComponent(FpsCounterComponent&& other) noexcept = delete;
	FpsCounterComponent& operator=(const FpsCounterComponent& other) = delete;
	FpsCounterComponent& operator=(FpsCounterComponent&& other) noexcept = delete;

private:
	explicit FpsCounterComponent(dae::GameObject& owner);

	template <typename T, typename ... Args>
	friend T* dae::GameObject::AddComponent(Args&&... args);

	float m_TimeTreshHold{0.5f};
	float m_TotalElapsedTime{};
	uint32_t m_FrameCount{};
	TextComponent* m_TextComponent;
};

