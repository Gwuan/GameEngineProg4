#pragma once
#include <complex>
#include <iostream>
#include <memory>
#include "Transform.h"

#include <vector>
#include <stdexcept>

#include "Component.h"

class Component;

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

		template <typename T>
		T* GetComponent()
		{
			for (auto& com : m_Components)
			{
				// Check if the current component has the same class as the requested class
				if(auto casted = dynamic_cast<T*>(com.get()))
					return casted;
			}

			// If nothing is found return nullptr;
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

		Transform GetTransform() const { return this->m_transform; }

		void SetPosition(float x, float y);

		bool NeedsDestroyed() const { return m_IsDead; }
		void Destroy() { m_IsDead = true; };

		GameObject() = default;
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		void KillComponents();

		bool m_IsDead = false;
		Transform m_transform{};
		std::vector<uint32_t> m_ComponentKillList;
		std::vector<std::unique_ptr<Component>> m_Components;
	};
}
