#pragma once
#include "Component.h"

class TextureComponent final : public Component
{
public:
	TextureComponent(dae::GameObject* owner);
	TextureComponent(dae::GameObject* owner, const std::string& filename);
	~TextureComponent() = default;

	TextureComponent(const TextureComponent& other) = delete;
	TextureComponent(TextureComponent&& other) noexcept = delete;
	TextureComponent& operator=(const TextureComponent& other) = delete;
	TextureComponent& operator=(TextureComponent&& other) noexcept = delete;

	void Render() const override;

	void SetTexture(const std::string& filename);

private:

	std::shared_ptr<dae::Texture2D> m_Texture = nullptr;
};

