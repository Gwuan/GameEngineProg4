#pragma once
#include <complex>
#include <iostream>
#include <memory>
#include <vector>
#include <stdexcept>
#include "glm.hpp"
#include "Component.h"

class IObserver;
class Subject;
class Transform;
class Component;

namespace dae
{
	class SDLTexture2D;

	class GameObject final
	{
	public:
		void BeginPlay();
		void FixedUpdate(const float fixedTime);
		void Update(const float deltaTime);
		void LateUpdate(const float deltaTime);
		void Render() const;
		void DebugRender();

#pragma region Component related functions
		template <typename T, typename... Args>
		T* AddComponent(Args&&... args)
		{
			if (!std::is_base_of_v<Component, T>)
			{
				static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
			}
			try
			{
				auto component = std::unique_ptr<T>(new T(*this, std::forward<Args>(args)...));
				T* returnValue = component.get();
				m_Components.push_back(std::move(component));
				if (m_AlreadyActive)
				{
					if constexpr (requires { std::declval<T>().BeginPlay(); })
					{
						returnValue->BeginPlay();
					}
				}
				return returnValue;
			}
			catch (const std::exception& e)
			{
				throw std::runtime_error(e.what());
			}
		}

		// Return nullptr if the desired component isn't found
		template <typename T>
		T* GetComponent()
		{
			for (auto& com : m_Components)
			{
				if (auto casted = dynamic_cast<T*>(com.get()))
					return casted;
			}

			return nullptr;
		}

		template <typename T>
		void RemoveComponent()
		{
			if (auto component = GetComponent<T>())
			{
				dynamic_cast<Component*>(component)->Destroy();
			}
		}
#pragma endregion

		void SetParent(GameObject* parent, bool keepWorldPosition = false);
		GameObject* GetParent() const { return m_pParent; }
		std::vector<GameObject*> GetChildren() const { return m_Children; }

		void SetTag(const std::string& tag) { m_Tag = tag; }
		std::string GetTag() const { return this->m_Tag; }

		bool IsStatic() const { return m_IsStatic; }

		bool NeedsDestroyed() const { return m_IsPendingDelete; }
		void Destroy() { m_IsPendingDelete = true; }

		Transform* GetTransform() const { return m_pTransform; }

		explicit GameObject(const glm::vec2& position = glm::vec2(0.f, 0.f), bool isStatic = false);
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		void AddChild(GameObject* object);
		void RemoveChild(GameObject* object);
		bool IsChild(GameObject* object);

		void KillComponents();

		bool m_IsPendingDelete;
		bool m_IsStatic;
		GameObject* m_pParent;

		Transform* m_pTransform; // only used for reference, this ptr does NOT HAVE OWNERSHIP

		std::string m_Tag;

		std::vector<GameObject*> m_Children;
		std::vector<uint32_t> m_ComponentKillList;
		std::vector<std::unique_ptr<Component>> m_Components;

		bool m_AlreadyActive = false;
	};
}
