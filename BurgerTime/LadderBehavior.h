#pragma once
#include <Component.h>
class LadderBehavior final : public Component
{
public:
	void BeginPlay() override;
	void Update(const float deltaTime) override;
	void FixedUpdate(const float fixedTimeStep) override;
	void LateUpdate(const float deltaTime) override;
	void Render() const override;
};

