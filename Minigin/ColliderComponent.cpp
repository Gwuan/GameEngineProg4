#include "ColliderComponent.h"

#include <filesystem>
#include <SDL_render.h>

#include "CollisionSystem.h"
#include "IRendererService.h"
#include "ServiceAllocator.h"
#include "Transform.h"

uint16_t ColliderComponent::m_Counter = 0;

void ColliderComponent::DebugRender()
{
	const auto& bounds = m_BoundingBox.GetBounds(GetOwner().GetTransform()->GetWorldPosition());

	ServiceAllocator::GetRenderer().RenderLine(bounds[0], bounds[1]);
	ServiceAllocator::GetRenderer().RenderLine(bounds[1], bounds[2]);
	ServiceAllocator::GetRenderer().RenderLine(bounds[2], bounds[3]);
	ServiceAllocator::GetRenderer().RenderLine(bounds[3], bounds[0]);
}

ColliderComponent::~ColliderComponent()
{
	CollisionSystem::GetInstance().UnregisterCollider(*this);
}

bool ColliderComponent::CheckCollision(ColliderComponent* other)
{
	auto bounds = this->m_BoundingBox.GetBounds(GetOwner().GetTransform()->GetWorldPosition());
	auto otherBounds = other->m_BoundingBox.GetBounds(other->GetOwner().GetTransform()->GetWorldPosition());

	bool leftToRight = bounds[0].x > otherBounds[1].x;
	bool rightToLeft = bounds[1].x < otherBounds[0].x;
	bool botToUp = bounds[2].y < otherBounds[0].y;
	bool upToBot = bounds[0].y > otherBounds[2].y;

	return !(leftToRight || rightToLeft || botToUp || upToBot);
}

ColliderComponent::ColliderComponent(dae::GameObject& owner, const Rect& box,bool isTrigger)
	: Component(owner),
	  m_ID(m_Counter),
	  m_BoundingBox(box),
	  m_IsTrigger(isTrigger)
{
	if (m_Counter == UINT16_MAX) return;

	m_Counter++;
	CollisionSystem::GetInstance().RegisterCollider(*this);
}