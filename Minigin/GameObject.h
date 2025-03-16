#pragma once
#include <complex>
#include <iostream>
#include <memory>
#include "Component.h"
#include "Transform.h"

#include <vector>
#include <stdexcept>
#include "glm.hpp"

class IObserver;
class Component;
class Transform;
class SubjectComponent;

namespace dae
{
	class Texture2D;

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
		template <typename T, typename ... Args>
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
				if(auto casted = dynamic_cast<T*>(com.get()))
					return casted;
			}

			return nullptr;
		}

		template <typename T>
		void RemoveComponent()
		{
			if(auto component = GetComponent<T>())
			{
				dynamic_cast<Component*>(component)->Destroy();
			}
		}
		#pragma endregion

		void SetParent(GameObject* parent, bool keepWorldPosition = false);
		GameObject* GetParent() const { return m_pParent; }
		std::vector<GameObject*> GetChildren() const { return m_Children; }

		bool NeedsDestroyed() const { return m_IsDead; }
		void Destroy() { m_IsDead = true; }

		Transform* GetTransform() const { return m_Transform.get(); }
		glm::vec2 GetWorldPosition() const { return m_Transform->GetWorldPosition(); }
		SubjectComponent* GetSubject() const { return m_Subject; }


		explicit GameObject(const glm::vec2& position = glm::vec2(0.f, 0.f));
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

		bool m_IsDead;
		GameObject* m_pParent;

		std::unique_ptr<Transform> m_Transform;
		SubjectComponent* m_Subject;

		std::vector<GameObject*> m_Children;
		std::vector<uint32_t> m_ComponentKillList;
		std::vector<std::unique_ptr<Component>> m_Components;
	};
}
