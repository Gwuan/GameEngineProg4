#pragma once
#include "Component.h"
#include "Datatypes.hpp"

class ITexture2D;

class TextureComponent final : public Component
{
public:

	void FixedUpdate(const float) override {}
	void Update(const float) override {}
	void LateUpdate(const float) override {}

	void Render() const override;

	void SetTexture(const std::string& filename);

	TextureComponent() = delete;
	virtual ~TextureComponent() override = default;

	void SetTextureSourceRect(const Rectf& srcRect);

	TextureComponent(const TextureComponent& other) = delete;
	TextureComponent(TextureComponent&& other) noexcept = delete;
	TextureComponent& operator=(const TextureComponent& other) = delete;
	TextureComponent& operator=(TextureComponent&& other) noexcept = delete;
protected:
	template <typename T, typename ... Args>
	friend T* dae::GameObject::AddComponent(Args&&... args);

	TextureComponent(dae::GameObject& owner);
	TextureComponent(dae::GameObject& owner, const std::string& filename);
private:
	std::shared_ptr<ITexture2D> m_Texture = nullptr;
	Rectf m_srcRect = {};
	bool m_UsingSrc = false;
};