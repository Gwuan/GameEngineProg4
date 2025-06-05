#pragma once

#include <SDL_render.h>

#include "Component.h"

class SpriteAnimation final : public Component
{
public:
    struct AnimationConfig final
    {
        glm::vec2 frameSize{};
        uint8_t nrOfFrames{ 0 };
        uint8_t startRow{ 0 };
        uint8_t startColumn{ 0 };
        float totalDuration{ 0.0f };
    };

    ~SpriteAnimation() override = default;

    void Update(float deltaTime) override;
    void FixedUpdate(float) override {}
    void LateUpdate(float) override {}
    void Render() const override;

    void SetPositionOffset(const glm::vec2& offset) { m_PositionOffset = offset; }
    void ChangeConfig(const AnimationConfig& newConfig);
    AnimationConfig GetConfig() const { return this->m_Config; }

    SDL_RendererFlip m_Flip = SDL_FLIP_NONE;

protected:
    template <typename T, typename... Args>
    friend T* dae::GameObject::AddComponent(Args&&... args);

    explicit SpriteAnimation(dae::GameObject& owner, std::shared_ptr<dae::Texture2D> spriteTexture, const AnimationConfig& animationConfig);

private:
    glm::vec2 m_PositionOffset = glm::vec2{};

    std::shared_ptr<dae::Texture2D> m_SpriteTexture;

	AnimationConfig m_Config{};

	float m_TimeEachFrame{ 0.0f };
    float m_ElapsedTime{ 0.0f };

    uint8_t m_CurrentFrameID{ 0 };
};