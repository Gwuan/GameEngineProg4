#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

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
	for (auto& component : m_Components)
	{
		component->Update(deltaTime);
	}
}

void dae::GameObject::LateUpdate(const float deltaTime)
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
			component->LateUpdate(deltaTime);
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

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}