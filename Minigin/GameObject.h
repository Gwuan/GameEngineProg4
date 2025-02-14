#pragma once
#include <complex>
#include <memory>
#include "Transform.h"

#include <vector>
#include <stdexcept>

#include "Component.h"

class Component;

namespace dae
{
	class Texture2D;

	// todo: this should become final.
	class GameObject final
	{
	public:
		void BeginPlay();
		virtual void FixedUpdate(const float fixedTime);
		virtual void Update(const float deltaTime);
		virtual void Render() const;

		template <typename T, typename ... Args>
		std::shared_ptr<T> AddComponent(Args&&... args)
		{
			if (!std::is_base_of<Component, T>::value)
			{
				static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
			}
			try
			{
				auto component = std::make_shared<T>(this, std::forward<Args>(args)...);
				m_Components.push_back(std::static_pointer_cast<Component>(component));
				return component;
				
			}
			catch (const std::exception& e)
			{
				throw std::runtime_error(e.what());
			}
		}

		template <typename T>
		std::shared_ptr<T> GetComponent()
		{
			for (auto& com : m_Components)
			{
				// Check if the current component has the same class as the requested class
				if(auto casted = std::dynamic_pointer_cast<T>(com))
					return casted;
			}

			// If nothing is found return nullptr;
			return nullptr;
		}

		Transform GetTransform() const { return this->m_transform; }

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_transform{};
		// todo: mmm, every gameobject has a texture? Is that correct?
		std::shared_ptr<Texture2D> m_texture{};

		std::vector<std::shared_ptr<Component>> m_Components;
	};
}
