#include "TextureComponent.h"

#include "SDLRenderer.h"
#include "ResourceManager.h"
#include "ServiceAllocator.h"
#include "Transform.h"

TextureComponent::TextureComponent(dae::GameObject& owner)
	: Component(owner)
{}

TextureComponent::TextureComponent(dae::GameObject& owner, const std::string& filename)
	: Component(owner)
{
	SetTexture(filename);
}

void TextureComponent::SetTexture(const std::string& filename)
{
	m_Texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}

void TextureComponent::SetTextureSourceRect(const Rectf& srcRect)
{
	m_srcRect = srcRect;
	m_UsingSrc = true;
}

void TextureComponent::Render() const
{
	if(m_Texture)
	{
		auto pos = GetOwner().GetTransform()->GetWorldPosition();

		if (m_UsingSrc)
		{
			const Rectf dst
			{
				{
					pos.x - (m_srcRect.width / 2), 
					pos.y - (m_srcRect.height / 2)
				},
				m_srcRect.width,
				m_srcRect.height

			};
			ServiceAllocator::GetRenderer().RenderTextureRegion(*m_Texture, 
				m_srcRect,
				dst);
				
		}
		else
		{
			const auto& size = m_Texture->GetSize();
			ServiceAllocator::GetRenderer().RenderTexture(*m_Texture, 
				pos.x - ((static_cast<float>(size.x) / 2)), 
				pos.y - ((static_cast<float>(size.y) / 2)));
		}
	}
}