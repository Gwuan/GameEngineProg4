#include "SpriteAnimation.h"

#include <SDL_rect.h>

#include "Renderer.h"
#include "Transform.h"

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

    SDL_Rect region{};
    const int frameWidth = static_cast<int>(m_Config.frameSize.x);
    const int frameHeight = static_cast<int>(m_Config.frameSize.y);

    region.x = (m_CurrentFrameID + m_Config.startColumn) * frameWidth;
    region.y = m_Config.startRow * frameHeight;
    region.w = frameWidth;
    region.h = frameHeight;

    const glm::vec2 pos = GetOwner().GetTransform()->GetWorldPosition();
    dae::Renderer::GetInstance().RenderTextureRegion(
        *m_SpriteTexture,
        region,
        pos.x, pos.y,
        static_cast<float>(frameWidth),
        static_cast<float>(frameHeight)
    );
}

void SpriteAnimation::ChangeConfig(const AnimationConfig& newConfig)
{
    m_Config = newConfig;
    m_TimeEachFrame = newConfig.totalDuration / newConfig.nrOfFrames;
    m_ElapsedTime = 0.0f;
    m_CurrentFrameID = 0;
}

SpriteAnimation::SpriteAnimation(dae::GameObject& owner, std::shared_ptr<dae::Texture2D> spriteTexture,
                                 const AnimationConfig& animationConfig)
    : Component(owner),
      m_SpriteTexture(std::move(spriteTexture)),
      m_Config(animationConfig),
      m_TimeEachFrame(animationConfig.totalDuration / animationConfig.nrOfFrames),
      m_ElapsedTime(0.0f),
      m_CurrentFrameID(0)
{}