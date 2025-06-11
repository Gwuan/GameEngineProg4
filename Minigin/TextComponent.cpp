#include "TextComponent.h"

#include "SDLRenderer.h"
#include "Font.h"
#include "ServiceAllocator.h"
#include "Transform.h"


TextComponent::TextComponent(dae::GameObject& owner, std::shared_ptr<dae::Font> font)
	: Component(owner),
      m_NeedsUpdate(true),
      m_Text(" "),
	  m_TextOffset(0.f, 0.f),
      m_Font(font),
      m_textTexture(nullptr)
{}

TextComponent::TextComponent(dae::GameObject& owner, const std::string& text, std::shared_ptr<dae::Font> font)
	: Component(owner),
	  m_NeedsUpdate(true),
	  m_Text(text),
	  m_TextOffset(0.f, 0.f),
	  m_Font(font),
	  m_textTexture(nullptr),
	  m_Color(Colors::White)
{}

void TextComponent::Update(const float)
{
	if (m_NeedsUpdate)
	{
		m_textTexture = ServiceAllocator::GetRenderer().CreateFontTexture(m_Text.c_str(), m_Font, m_Color);
		m_NeedsUpdate= false;
	}
}

void TextComponent::Render() const
{
	if (m_textTexture != nullptr)
	{
		const auto& pos = GetOwner().GetTransform()->GetWorldPosition() + m_TextOffset;
		ServiceAllocator::GetRenderer().RenderTexture(*m_textTexture, pos.x, pos.y);
	}
}

void TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}
