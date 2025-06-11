#pragma once
#include "Component.h"
#include "DataTypes.hpp"

class ITexture2D;

namespace dae
{
	class Font;
}

class TextComponent final : public Component
{
public:
	void FixedUpdate(const float) override {}
	void Update(const float deltaTime) override;
	void LateUpdate(const float) override {}
	void Render() const override;

	void SetText(const std::string& text);
	void SetTextOffset(const glm::vec2& offset) { m_TextOffset = offset; };

	void SetColor(const ColorRGBA& color) { m_Color = color; }

	TextComponent() = delete;
	virtual ~TextComponent() override = default;

	TextComponent(const TextComponent& other) = delete;
	TextComponent(TextComponent&& other) noexcept = delete;
	TextComponent& operator=(const TextComponent& other) = delete;
	TextComponent& operator=(TextComponent&& other) noexcept = delete;
protected:
	template <typename T, typename ... Args>
	friend T* dae::GameObject::AddComponent(Args&&... args);

	TextComponent(dae::GameObject& owner, std::shared_ptr<dae::Font> font);
	TextComponent(dae::GameObject& owner, const std::string& text, std::shared_ptr<dae::Font> font);
private:
	bool m_NeedsUpdate;
	std::string m_Text;
	glm::vec2 m_TextOffset;
	std::shared_ptr<dae::Font> m_Font;
	std::shared_ptr<ITexture2D> m_textTexture;
	ColorRGBA m_Color;
};