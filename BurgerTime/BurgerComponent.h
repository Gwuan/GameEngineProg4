#pragma once
#include <Component.h>
#include <DataTypes.hpp>

#include "Event.h"

class ColliderComponent;
struct Rectf;
class TextureComponent;

enum class SliceType
{
	BottomBread,
	Meat,
	Salad,
	TopBread
};

class BurgerSliceComponent : public Component
{
public:
	void BeginPlay() override;
	void Update(const float deltaTime) override;
	void FixedUpdate(const float) override {}
	void LateUpdate(const float) override {}
	void Render() const override {}
private:
	void InitializeSliceType();

	void OnBeginOverlap(const ColliderComponent* thisCollider, const ColliderComponent* otherCollider);

	void FallDownBeginOverlap(ColliderComponent* otherCollider);

	template <typename T, typename ... Args>
	friend T* dae::GameObject::AddComponent(Args&&... args);

	explicit BurgerSliceComponent(dae::GameObject& owner, const SliceType& sliceType);
	void CheckAllColliders();
	void DropDown();

	bool m_IgnoreNextPlatform = false;

	Rectf m_TexSrcRect;
	TextureComponent* m_pTextureComponent = nullptr;

	static uint8_t m_CellSpan;

	std::vector<std::pair<bool, ColliderComponent*>> m_PlayerColliders;

	const SliceType m_SliceType; 

	ColliderComponent* m_pFallDownCollider = nullptr;

	Transform* m_pSliceTransform = nullptr;
};