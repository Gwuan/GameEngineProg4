#include "Component.h"

void Component::BeginPlay(){}

void Component::Update(const float ){}
void Component::FixedUpdate(const float){}
void Component::Render() const{}

Component::Component(dae::GameObject* owner)
{
	if (owner == nullptr)
		throw std::runtime_error("Owner if the component is null!");

	m_pOwner = owner;	
}

Component::~Component()
{
	m_pOwner = nullptr;
}
	