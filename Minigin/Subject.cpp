#include "Subject.h"
#include "GameObject.h"
#include "Observer.h"
#include "Utils.hpp"

Subject::~Subject()
{
	m_Observers.clear();
}

void Subject::AddObserver(IObserver* observer)
{
	m_Observers.push_back(observer);
}

bool Subject::RemoveObserver(IObserver* observer)
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

void Subject::Notify(dae::GameObject* object, EventID id)
{
	for (auto& observer : m_Observers)
	{
		observer->OnNotify(object, id);
	}
}