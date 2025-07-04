#pragma once
#include <string>
#include <vector>

#include "Observer.h"

namespace dae
{
	class GameObject;
}

class IObserver;
using EventID = size_t;

class Subject
{
public:
	Subject() = default;
	virtual ~Subject();

	Subject(const Subject&) = delete;
	Subject(Subject&&) noexcept = delete;
	Subject& operator=(const Subject&) = delete;
	Subject& operator=(Subject&&) noexcept = delete;

	void AddObserver(IObserver* observer);
	bool RemoveObserver(IObserver* observer);

	// use hashing
	void Notify(dae::GameObject* object, EventID id);

private:
	std::vector<IObserver*> m_Observers;
};