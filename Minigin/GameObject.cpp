#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::KillComponents()
{
	if (m_ComponentKillList.empty())
		return;

	for (auto& index : m_ComponentKillList)
	{
		m_Components.erase(m_Components.begin() + index);
	}
	m_ComponentKillList.clear();
}

void dae::GameObject::BeginPlay()
{
	for (auto& component : m_Components)
	{
		component->BeginPlay();
	}
}

void dae::GameObject::FixedUpdate(const float fixedTimeStep)
{
	for (auto& component : m_Components)
	{
		component->FixedUpdate(fixedTimeStep);
	}
}

void dae::GameObject::Update(const float deltaTime)
{
	for(uint32_t i{}; i < m_Components.size(); ++i)
	{
		auto& component = m_Components[i];
		if(component->NeedDestroyed())
		{
			m_ComponentKillList.push_back(i);
		}
		else
		{
			component->Update(deltaTime);
		}
	}

	KillComponents();
}

void dae::GameObject::Render() const
{
	for (auto& component : m_Components)
	{
		component->Render();
	}
}

void dae::GameObject::SetTexture(const std::string& filename)
{

	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
