#pragma once
#include "Component.h"

namespace dae
{
	class Font;
}

class TextComponent final : public Component
{
public:
	TextComponent(dae::GameObject* owner, std::shared_ptr<dae::Font> font);
	TextComponent(dae::GameObject* owner, const std::string& text, std::shared_ptr<dae::Font> font);

	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;

	void SetText(const std::string& text);

private:
	bool m_NeedsUpdate;
	std::string m_Text;
	std::shared_ptr<dae::Font> m_Font;
	std::shared_ptr<dae::Texture2D> m_textTexture;
};

