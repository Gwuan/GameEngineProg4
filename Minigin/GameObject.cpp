#include "GameObject.h"

#include <algorithm>
#include <windows.h>
#include <memory>
#include <ranges>
#include "imgui_plot.h"
#include "SDLRenderer.h"
#include "Transform.h"

dae::GameObject::GameObject(const glm::vec2& position, bool isStatic)
	: m_IsPendingDelete(false),
	  m_IsStatic(isStatic),
      m_pParent(nullptr),
	  m_pTransform(nullptr),
	  m_Tag(""),
	  m_Children(),
	  m_ComponentKillList(),
	  m_Components()
{
	m_pTransform = this->AddComponent<Transform>(position);
}

dae::GameObject::~GameObject()
{
	// First, safely remove this object from its parent
	if (m_pParent)
	{
		m_pParent->RemoveChild(this);
		m_pParent = nullptr;
	}
	
	// Make a copy of children to avoid iterator invalidation
	auto childrenCopy = m_Children;
	m_Children.clear();
	
	// Set parent to nullptr for all children
	for (auto child : childrenCopy)
	{
		if (child && child->m_pParent == this)
		{
			child->m_pParent = nullptr;
		}
	}
}

void dae::GameObject::AddChild(GameObject* object)
{
	if (!object || object == this || IsChild(object))
		return;
		
	m_Children.push_back(object);
}

void dae::GameObject::RemoveChild(GameObject* object)
{
	if (!object)
		return;
		
	auto it = std::find(m_Children.begin(), m_Children.end(), object);
	if (it != m_Children.end())
	{
		m_Children.erase(it);
	}
}

bool dae::GameObject::IsChild(GameObject* object)
{
	if (!object)
		return false;
    return std::find(m_Children.begin(), m_Children.end(), object) != m_Children.end();
}

void dae::GameObject::KillComponents()
{
	if (m_ComponentKillList.empty())
		return;
		
	// Eliminate index shifting problems
	std::ranges::sort(std::ranges::reverse_view(m_ComponentKillList));
	
	for (auto index : m_ComponentKillList)
	{
		if (index < m_Components.size())
		{
			m_Components.erase(m_Components.begin() + index);
		}
	}
	m_ComponentKillList.clear();
}

void dae::GameObject::BeginPlay()
{
	for (auto& component : m_Components)
	{
		component->BeginPlay();
	}

	m_AlreadyActive = true;
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

void dae::GameObject::DebugRender()
{
	for (auto& component : m_Components)
	{
		component->DebugRender();
	}
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPos)
{
	if (parent == this || parent == m_pParent)
		return;
		
	if (parent && IsChild(parent))
		return;
	
	glm::vec2 worldPos{};
	if (keepWorldPos && m_pTransform)
	{
		worldPos = m_pTransform->GetWorldPosition();
	}
	
	if (m_pParent)
	{
		m_pParent->RemoveChild(this);
	}
	
	m_pParent = parent;
	
	if (m_pParent)
	{
		m_pParent->AddChild(this);
		
		if (keepWorldPos && m_pTransform)
		{
			const auto parentWorldPos = m_pParent->m_pTransform->GetWorldPosition();
			const auto localPos = worldPos - parentWorldPos;
			m_pTransform->SetPosition(localPos.x, localPos.y);
		}
		m_pTransform->MarkDirty();
	}
	else if (keepWorldPos && m_pTransform)
	{
		m_pTransform->SetPosition(worldPos.x, worldPos.y);
	}
}