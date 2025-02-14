#include "TextComponent.h"

#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"


TextComponent::TextComponent(dae::GameObject* owner, std::shared_ptr<dae::Font> font)
	: Component(owner), m_NeedsUpdate(true), m_Text(" "), m_Font(font), m_textTexture(nullptr)
{
}

TextComponent::TextComponent(dae::GameObject* owner, const std::string& text, std::shared_ptr<dae::Font> font)
	: Component(owner), m_NeedsUpdate(true), m_Text(text), m_Font(font), m_textTexture(nullptr)
{}


void TextComponent::Update(const float)
{
	if (m_NeedsUpdate)
	{
		const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_textTexture = std::make_shared<dae::Texture2D>(texture);
		m_NeedsUpdate= false;
	}
}

void TextComponent::Render() const
{
	if (m_textTexture != nullptr)
	{
		const auto& pos = GetOwner()->GetTransform().GetPosition();
		dae::Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y);
	}
}

void TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}
