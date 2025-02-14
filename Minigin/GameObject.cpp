#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::BeginPlay()
{
	for (auto& component : m_Components)
	{
		component.get()->BeginPlay();
	}
}

void dae::GameObject::FixedUpdate(const float fixedTimeStep)
{
	for (auto& component : m_Components)
	{
		component.get()->FixedUpdate(fixedTimeStep);
	}
}

void dae::GameObject::Update(const float deltaTime)
{
	for (auto& component : m_Components)
	{
		component.get()->Update(deltaTime);
	}	
}

void dae::GameObject::Render() const
{
	for (auto& component : m_Components)
	{
		component.get()->Render();
	}
	//const auto& pos = m_transform.GetPosition();
	//Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
}

void dae::GameObject::SetTexture(const std::string& filename)
{

	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
