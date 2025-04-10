#pragma once
#include "Command.h"

// ===================
// MovePepper
class MovePepperCommand final : public Command
{
public:
	MovePepperCommand(dae::GameObject* object, MoveDirection direction);
	void Execute() override;

private:
	MoveDirection m_Direction;
	MovementComponent* m_pMovementComp;
};

// ===================
// MoveSalt
class MoveSaltCommand final : public Command
{
public:
	MoveSaltCommand(dae::GameObject* object, MoveDirection direction);
	void Execute() override;

private:
	MoveDirection m_Direction;
	MovementComponent* m_pMovementComp;
};

// ===================	
// Die
class DieCommand final : public Command
{
public:
	DieCommand(dae::GameObject* object) : Command(object) {}
	void Execute() override;
};

// ===================
// KillHotDog
class KillHotDogCommand final : public Command
{
public:
	KillHotDogCommand(dae::GameObject* object) : Command(object) {}
	void Execute() override;
};

// ===================
// KillPickle
class KillPickleCommand final : public Command
{
public:
	KillPickleCommand(dae::GameObject* object) : Command(object) {}
	void Execute() override;
};

class SoundTestCommand final : public Command
{
public:
	SoundTestCommand(dae::GameObject* object) : Command(object) {}
	void Execute() override;
};

class StartMusicCommand final : public Command
{
public:
	StartMusicCommand(dae::GameObject* object) : Command(object) {}
	void Execute() override;
};

class StopMusicCommand final : public Command
{
public:
	StopMusicCommand(dae::GameObject* object) : Command(object) {}
	void Execute() override;
};

class PauseMusicCommand final : public Command
{
public:
	PauseMusicCommand(dae::GameObject* object) : Command(object) {}
	void Execute() override;
};

class ResumeMusicCommand final : public Command
{
public:
	ResumeMusicCommand(dae::GameObject* object) : Command(object) {}
	void Execute() override;
};

class PauseAllAudioCommand final : public Command
{
public:
	PauseAllAudioCommand(dae::GameObject* object) : Command(object) {}
	void Execute() override;
};

class ResumeAllAudioCommand final : public Command
{
public:
	ResumeAllAudioCommand(dae::GameObject* object) : Command(object) {}
	void Execute() override;
};