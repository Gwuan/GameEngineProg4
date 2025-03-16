#include "SubjectComponent.h"
#include "Observer.h"

SubjectComponent::SubjectComponent(dae::GameObject& owner)
	: Component(owner)
{}

SubjectComponent::~SubjectComponent()
{
	m_Observers.clear();
}

void SubjectComponent::AddObserver(IObserver* observer)
{
	m_Observers.push_back(observer);
}

bool SubjectComponent::RemoveObserver(IObserver* observer)
{
	if (!observer)
		return false;

	if (auto found = std::find(m_Observers.begin(), m_Observers.end(), observer); found != m_Observers.end())
	{
		m_Observers.erase(found);
		return true;
	}

	return false;
}

void SubjectComponent::Notify(dae::GameObject* object, const std::string& event)
{
	const size_t hashedEvent{ std::hash<std::string>{}(event)};

	for (auto& observer : m_Observers)
	{
		observer->OnNotify(object, hashedEvent);
	}
}

