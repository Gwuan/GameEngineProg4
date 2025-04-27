#pragma once
namespace dae
{
	class GameObject;
}


class Command
{
public:
	explicit Command(dae::GameObject* object);
	virtual ~Command();
	virtual void Execute() = 0;

protected:
	dae::GameObject* GetGameObject() const { return m_pObject; }

private:
	dae::GameObject* m_pObject;
};

