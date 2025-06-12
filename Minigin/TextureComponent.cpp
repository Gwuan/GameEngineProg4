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

void TextureComponent::Render() const
{
	if(m_Texture)
	{
		auto pos = GetOwner().GetTransform()->GetWorldPosition();
		const auto size = m_Texture->GetSize();
		ServiceAllocator::GetRenderer().RenderTexture(*m_Texture, 
			pos.x - (size.x / 2), 
			pos.y - (size.y / 2));
	}
}