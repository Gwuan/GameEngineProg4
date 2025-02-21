#include "Component.h"

void Component::BeginPlay(){}

void Component::Update(const float ){}
void Component::FixedUpdate(const float){}
void Component::Render() const{}

Component::Component(dae::GameObject& owner)
	: m_IsDead(false),
	  m_Owner(owner)
{}