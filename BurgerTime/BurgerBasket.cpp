#include "BurgerBasket.h"

#include "BurgerComponent.h"
#include "ColliderComponent.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "TextureComponent.h"
#include "Utils.hpp"

void BurgerBasket::BeginPlay()
{
	Component::BeginPlay();

	if (const auto scene = dae::SceneManager::GetInstance().GetActiveScene())
	{
		auto sliceTrigger = GetOwner().AddComponent<ColliderComponent>(glm::vec2{scene->GetCellSize() * 3, scene->GetCellSize()}, true);
		sliceTrigger->OnBeginOverlap += [this](ColliderComponent* other){ OnSliceReceived(other); };
		sliceTrigger->m_Offset.y = scene->GetCellSize() * 3.5f;

		auto hardFloor = GetOwner().AddComponent<ColliderComponent>(glm::vec2{scene->GetCellSize() * 3, scene->GetCellSize() / 8.f}, false);
		hardFloor->m_Offset.y = 6.f;

		auto texture = GetOwner().AddComponent<TextureComponent>("SpriteSheet.png");
		const Rectf src = 
		{
			{
				7 * 16.f,
				7 * 16.f
			},
			16.f * 2.f,
			16.f/ 2.f
		};

		texture->SetTextureSourceRect(src);
		
	}
}

void BurgerBasket::OnSliceReceived(const ColliderComponent* other)
{
	if (other->GetOwner().GetTag() == "BurgerSlice")
	{
		if (auto slice = GetOwner().GetComponent<BurgerSliceComponent>())
		{
			if (slice->GetSliceType() == SliceType::TopBread)
				Notify(&GetOwner(), HashUtils::make_sdbm_hash("BasketFull"));
		}
	}
}
