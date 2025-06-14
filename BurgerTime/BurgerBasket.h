#pragma once
#include <Component.h>

#include "Subject.h"
class ColliderComponent;

class BurgerBasket : public Component, public Subject
{
public:
	void BeginPlay() override;
	void Update(const float) override {}
	void FixedUpdate(const float) override {}
	void LateUpdate(const float) override {}
	void Render() const override {}

protected:
	template <typename T, typename ... Args>
	friend T* dae::GameObject::AddComponent(Args&&... args);

	explicit BurgerBasket(dae::GameObject& owner) : Component(owner) {}

private:
	void OnSliceReceived(const ColliderComponent* other);
};