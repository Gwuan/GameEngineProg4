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
	const auto& bounds = GetBox().GetVertices();

	ServiceAllocator::GetRenderer().RenderLine(bounds[0], bounds[1]);
	ServiceAllocator::GetRenderer().RenderLine(bounds[1], bounds[2]);
	ServiceAllocator::GetRenderer().RenderLine(bounds[2], bounds[3]);
	ServiceAllocator::GetRenderer().RenderLine(bounds[3], bounds[0]);
}

Rectf ColliderComponent::GetBox() const
{
	const auto& centerPos = GetOwner().GetTransform()->GetWorldPosition();

	return Rectf{
		{
			centerPos.x - (m_BoxSize.x / 2),
			centerPos.y - (m_BoxSize.y / 2)
		},
		m_BoxSize.x,
		m_BoxSize.y
	};
}

ColliderComponent::~ColliderComponent()
{
	CollisionSystem::GetInstance().UnregisterCollider(*this);
}

bool ColliderComponent::CheckCollision(ColliderComponent* other)
{
	auto bounds = this->GetBox().GetVertices();
	auto otherBounds = other->GetBox().GetVertices();

	bool leftToRight = bounds[0].x > otherBounds[1].x;
	bool rightToLeft = bounds[1].x < otherBounds[0].x;
	bool botToUp = bounds[2].y < otherBounds[0].y;
	bool upToBot = bounds[0].y > otherBounds[2].y;

	return !(leftToRight || rightToLeft || botToUp || upToBot);
}

ColliderComponent::ColliderComponent(dae::GameObject& owner, const glm::vec2& boxSize, bool isTrigger)
	: Component(owner),
	  m_ID(m_Counter),
	  m_BoxSize(boxSize),
	  m_IsTrigger(isTrigger)
{
	if (m_Counter == UINT16_MAX) return;

	m_Counter++;
	CollisionSystem::GetInstance().RegisterCollider(*this);
}