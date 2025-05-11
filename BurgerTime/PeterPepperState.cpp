#include "PeterPepperState.h"

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Transform.h"

std::unique_ptr<PeterPepperState> PeterIdleState::Update()
{
	if (glm::length(m_pPeter->GetTransform()->velocity) >= .1f)
	{
		return std::make_unique<PeterMoveState>(*m_pPeter);
	}

	return nullptr;
}

void PeterIdleState::OnEnter()
{
	if (auto spriteAnim = m_pPeter->GetComponent<SpriteAnimation>())
	{
		constexpr SpriteAnimation::AnimationConfig idleConfig {
			.frameSize= glm::vec2{16.f, 16.f},
			.nrOfFrames= 1,
			.startRow= 0,
			.startColumn= 1,
			.totalDuration= 1.f
		};

		spriteAnim->ChangeConfig(idleConfig);
	}
}

std::unique_ptr<PeterPepperState> PeterMoveState::Update()
{
		std::cout << "H33ello" << std::endl;
	if (glm::length(m_pPeter->GetTransform()->velocity) <= 0.01f)
	{
		return std::make_unique<PeterIdleState>(*m_pPeter);
	}

	return nullptr;
}

void PeterMoveState::OnEnter()
{
	std::cout << "HEEYYYYY" << std::endl;

	if (auto spriteAnim = m_pPeter->GetComponent<SpriteAnimation>())
	{
		constexpr SpriteAnimation::AnimationConfig idleConfig {
			.frameSize= glm::vec2{16.f, 16.f},
			.nrOfFrames= 3,
			.startRow= 0,
			.startColumn= 3,
			.totalDuration= .3f
		};

		spriteAnim->ChangeConfig(idleConfig);
	}
}