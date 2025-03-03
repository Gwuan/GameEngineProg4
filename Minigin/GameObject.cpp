#include <string>
#include "GameObject.h"

#include <windows.h>

#include <memory>

#include "imgui_plot.h"
#include "ResourceManager.h"
#include "Renderer.h"


dae::GameObject::GameObject(const glm::vec2& position)
	: m_IsDead(false),
      m_pParent()
{
	m_Transform = new Transform(*this);
	m_Transform->SetPosition(position.x, position.y);
}

dae::GameObject::~GameObject()
{
	m_Children.clear();
	m_pParent = nullptr;
}

void dae::GameObject::AddChild(GameObject* object)
{
	m_Children.push_back(object);
}

void dae::GameObject::RemoveChild(GameObject* object)
{
    auto it = std::remove_if(m_Children.begin(), m_Children.end(),
                             [object](GameObject* child) {
                                 return child == object;
                             });

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

void dae::GameObject::DebugRender()
{
	ImGui::SetNextWindowSize(ImVec2(200, 200));
	ImGui::Begin("Debug renderer for gameObject", NULL, ImGuiWindowFlags_MenuBar);

	for (auto& component : m_Components)
	{
		component->DebugRender();
	}

	ImGui::End();
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPos)
{
	if (IsChild(parent) || parent == this || parent == m_pParent)
		return;

	if (parent == nullptr)
	{
		const auto worldPos = GetWorldPosition();
		m_Transform->SetPosition(worldPos.x, worldPos.y);
	}
	else
	{
		if (keepWorldPos)
		{
			const auto worldPos = GetWorldPosition() - parent->GetWorldPosition();
			m_Transform->SetPosition(worldPos.x, worldPos.y);
		}
		m_Transform->MarkDirty();
	}

	if (m_pParent)
	{
		m_pParent->RemoveChild(this);
	}

	m_pParent = parent;

	if(m_pParent)
	{
		m_pParent->AddChild(this);
	}
}
