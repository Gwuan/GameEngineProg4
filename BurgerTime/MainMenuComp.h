#pragma once
#include <Component.h>
class TextureComponent;

class MainMenuComp : public Component
{
public:
	void BeginPlay() override;
	void Update(const float) override {}
	void FixedUpdate(const float) override {}
	void LateUpdate(const float) override {}
	void Render() const override;

protected:
	template <typename T, typename ... Args>
	friend T* dae::GameObject::AddComponent(Args&&... args);

	explicit MainMenuComp(dae::GameObject& owner);

private:
	const glm::vec2 m_ButtonSize{ 100.f, 50.f };
	TextureComponent* m_BackgroundTexture = nullptr;
};
