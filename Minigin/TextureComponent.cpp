#include "TextureComponent.h"

#include "Renderer.h"
#include "ResourceManager.h"

TextureComponent::TextureComponent(dae::GameObject* owner)
	: Component(owner)
{}

TextureComponent::TextureComponent(dae::GameObject* owner, const std::string& filename)
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
		auto& pos = GetOwner()->GetTransform().GetPosition();
		dae::Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	}
}