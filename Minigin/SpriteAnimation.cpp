#include "SpriteAnimation.h"

#include "IRendererService.h"
#include "ServiceAllocator.h"
#include "Transform.h"
#include "ITexture2D.h"

void SpriteAnimation::Update(const float deltaTime)
{
    m_ElapsedTime += deltaTime;

    if (m_ElapsedTime >= m_TimeEachFrame)
    {
        m_ElapsedTime = 0.0f;
        m_CurrentFrameID = (m_CurrentFrameID + 1) % m_Config.nrOfFrames;
    }
}

void SpriteAnimation::Render() const
{
    if (!m_SpriteTexture)
        return;
    const glm::vec2 pos = GetOwner().GetTransform()->GetWorldPosition() + m_PositionOffset;
    const float frameWidth = m_Config.frameSize.x;
    const float frameHeight = m_Config.frameSize.y;

    Rectf region
	{
		{
			(m_CurrentFrameID + m_Config.startColumn) * frameWidth,
            m_Config.startRow * frameHeight
		},
        frameWidth,
        frameHeight
    };

    Rectf dstRect
	{
        {
	        pos.x - (frameWidth /2),
        	pos.y - (frameHeight /2)
        },
        static_cast<float>(frameWidth),
        static_cast<float>(frameHeight)
    };

    ServiceAllocator::GetRenderer().RenderTextureRegion(
        *m_SpriteTexture,
        region,
        dstRect,
        m_Flip
    );
}

void SpriteAnimation::ChangeConfig(const AnimationConfig& newConfig)
{
    m_Config = newConfig;
    m_TimeEachFrame = newConfig.totalDuration / newConfig.nrOfFrames;
    m_ElapsedTime = 0.0f;
    m_CurrentFrameID = 0;
}

SpriteAnimation::SpriteAnimation(dae::GameObject& owner, std::shared_ptr<ITexture2D> spriteTexture,
                                 const AnimationConfig& animationConfig)
    : Component(owner),
      m_SpriteTexture(std::move(spriteTexture)),
      m_Config(animationConfig),
      m_TimeEachFrame(animationConfig.totalDuration / animationConfig.nrOfFrames),
      m_ElapsedTime(0.0f),
      m_CurrentFrameID(0),
	  m_Flip(TextureFlip::None)
{}