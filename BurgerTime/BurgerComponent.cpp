#include "BurgerComponent.h"

#include "ColliderComponent.h"
#include "GameCommands.h"
#include "Scene.h"
#include "SceneManager.h"
#include "TextureComponent.h"
#include "Transform.h"
#include "Utils.hpp"


void BurgerSliceComponent::BeginPlay()
{
	Component::BeginPlay();

	GetOwner().SetTag("BurgerSlice");

	InitializeSliceType();

	m_pTextureComponent = GetOwner().AddComponent<TextureComponent>("SpriteSheet.png");
	m_pTextureComponent->SetTextureSourceRect(m_TexSrcRect);
	{
		m_pFallDownCollider = GetOwner().AddComponent<ColliderComponent>(glm::vec2{16 * 3, 8}, true);
		m_pFallDownCollider->m_Offset.y = -4;

	    m_pFallDownCollider->OnBeginOverlap += [this](ColliderComponent* otherCollider) { FallDownBeginOverlap(otherCollider); };
	}

	// Player colliders
	{
		auto leftCollider = GetOwner().AddComponent<ColliderComponent>(glm::vec2{16, 8}, true);
		leftCollider->m_Offset = {-16, -4};

		auto middleCollider= GetOwner().AddComponent<ColliderComponent>(glm::vec2{16, 8}, true);
		middleCollider->m_Offset.y = -4;

		auto rightCollider = GetOwner().AddComponent<ColliderComponent>(glm::vec2{16, 8}, true);
		rightCollider->m_Offset = {16, -4};

		m_PlayerColliders.reserve(3);
		m_PlayerColliders.emplace_back(false, leftCollider);
		m_PlayerColliders.emplace_back(false, middleCollider);
		m_PlayerColliders.emplace_back(false, rightCollider);

		for (auto& colPair : m_PlayerColliders)
		{
			colPair.second->OnBeginOverlap += [this, &colPair](ColliderComponent* otherCollider)
			{
				OnBeginOverlap(colPair.second, otherCollider);
			};
		}
	}

	m_pSliceTransform = GetOwner().GetTransform();
	m_pSliceTransform->DisableVerticalMovement(true);
}

void BurgerSliceComponent::Update(const float)
{
	if (m_pSliceTransform) m_pSliceTransform->MoveDirection.y = -1;
}

void BurgerSliceComponent::InitializeSliceType()
{
	const auto* activeScene = dae::SceneManager::GetInstance().GetActiveScene();
	if (activeScene == nullptr)
	{
		throw std::runtime_error("");
		return;
	}

	const auto& cellSize = activeScene->GetCellSize();
	switch (m_SliceType)
	{
	case SliceType::BottomBread:
		m_TexSrcRect.LeftBottom.x = 7 * cellSize;
		m_TexSrcRect.LeftBottom.y = 3 * cellSize;
		m_TexSrcRect.width = cellSize * 2;
		m_TexSrcRect.height = cellSize / 2;
		break;
	case SliceType::Meat:
		m_TexSrcRect.LeftBottom.x = 7 * cellSize;
		m_TexSrcRect.LeftBottom.y = 4 * cellSize;
		m_TexSrcRect.width = cellSize * 2;
		m_TexSrcRect.height = cellSize / 2;
		break;
	case SliceType::Salad:
		m_TexSrcRect.LeftBottom.x = 7 * cellSize;
		m_TexSrcRect.LeftBottom.y = 5 * cellSize;
		m_TexSrcRect.width = cellSize * 2;
		m_TexSrcRect.height = cellSize / 2;
		break;
	case SliceType::TopBread:
		m_TexSrcRect.LeftBottom.x = 7 * cellSize;
		m_TexSrcRect.LeftBottom.y = 3.5f * cellSize;
		m_TexSrcRect.width = cellSize * 2;
		m_TexSrcRect.height = cellSize / 2;
		break;
	default: ;
	}
}

void BurgerSliceComponent::OnBeginOverlap(const ColliderComponent* thisCollider,  const ColliderComponent* otherCollider)
{
	if (otherCollider->GetOwner().GetTag() == "Player")
	{
		auto found = std::ranges::find_if(m_PlayerColliders, [thisCollider](const auto& colPair)
		{
			return thisCollider == colPair.second;
		});

		if (found != m_PlayerColliders.end())
		{
			found->first = true;
			CheckAllColliders();
		}
	}
}

void BurgerSliceComponent::FallDownBeginOverlap(ColliderComponent* otherCollider)
{
	if (&otherCollider->GetOwner() == &GetOwner())
		return;

	for (auto& colPair : m_PlayerColliders)
	{
		colPair.first = false;
	}

	const auto& colliderTag = otherCollider->GetOwner().GetTag();

	if (colliderTag == "Enemy")
	{
		m_IgnoreNextPlatform = true;
		m_PlatformCounter = 4;
	}
	else if (colliderTag == "Platform")
	{
		if (!m_IgnoreNextPlatform)
		{
			GetOwner().SetParent(&otherCollider->GetOwner(), true);
			m_pSliceTransform->DisableVerticalMovement(true);
		}
		else
		{
			// very hacky thing to do
			m_PlatformCounter--;

			if (m_PlatformCounter < 0)
				m_IgnoreNextPlatform = false;
		}
	}
	else if (colliderTag == "BurgerSlice")
	{
		if (auto otherSlice = otherCollider->GetOwner().GetComponent<BurgerSliceComponent>())
			otherSlice->DropDown();
	}
}

BurgerSliceComponent::BurgerSliceComponent(dae::GameObject& owner, const SliceType& sliceType)
	: Component(owner),
	  m_SliceType(sliceType)
{}

void BurgerSliceComponent::CheckAllColliders()
{
	auto found = std::ranges::find_if(m_PlayerColliders, [](const std::pair<bool, ColliderComponent*>& colPair)
	{
		return !colPair.first;
	});

	if (found == m_PlayerColliders.end())
	{
		DropDown();
	}
}

void BurgerSliceComponent::DropDown()
{
	GetOwner().SetParent(nullptr, true);

	if (m_pSliceTransform)
		m_pSliceTransform->DisableVerticalMovement(false);
}
