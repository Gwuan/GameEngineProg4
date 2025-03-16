#pragma once
#include "Component.h"
#include <string>
#include <vector>

class IObserver;

class SubjectComponent final : public Component
{
public:
	void Update(const float) override {}
	void FixedUpdate(const float) override {}
	void LateUpdate(const float) override {}
	void Render() const override {}
	void DebugRender() override {}

	~SubjectComponent() override;

	SubjectComponent(const SubjectComponent&) = delete;
	SubjectComponent(SubjectComponent&&) noexcept = delete;
	SubjectComponent& operator=(const SubjectComponent&) = delete;
	SubjectComponent& operator=(SubjectComponent&&) noexcept = delete;


	void AddObserver(IObserver* observer);
	bool RemoveObserver(IObserver* observer);

	// use hashing
	void Notify(dae::GameObject* object, const std::string& event);

protected:
	explicit SubjectComponent(dae::GameObject& owner);

	template <typename T, typename ... Args>
	friend T* dae::GameObject::AddComponent(Args&&... args);

private:
	std::vector<IObserver*> m_Observers;
};

