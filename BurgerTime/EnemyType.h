#pragma once
#include "SpriteAnimation.h"

struct EnemyType
{
	SpriteAnimation::AnimationConfig horizontalWalkAnimation{};
	SpriteAnimation::AnimationConfig verticalWalkAnimation{};
	SpriteAnimation::AnimationConfig crushAnimation{};
	SpriteAnimation::AnimationConfig dieAnimation{};

	int score = 0;
	float movementSpeed = 1.f;
};

static EnemyType g_MrHotDog{
	.horizontalWalkAnimation = {
		{
			16.f,
			16.f
		},
		3,
		2,
		1,
		.3f
	},
	.verticalWalkAnimation = {
		{
			16.f,
			16.f
		},
		2,
		2,
		4,
		.2f
	},
	.crushAnimation = {
		{
			16.f,
			16.f
		},
		4,
		3,
		0,
		.4f
	},
	.dieAnimation = {
		{
			16.f,
			16.f
		},
		2,
		3,
		4,
		.2f
	},
	.score = 100,
	.movementSpeed = 32.f
};

static EnemyType g_MrPickle{
	.horizontalWalkAnimation = {
		{
			16.f,
			16.f
		},
		2,
		4,
		2,
		.2f
	},
	.verticalWalkAnimation = {
		{
			16.f,
			16.f
		},
		2,
		4,
		4,
		.2f
	},
	.crushAnimation = {
		{
			16.f,
			16.f
		},
		4,
		5,
		0,
		.4f
	},
	.dieAnimation = {
		{
			16.f,
			16.f
		},
		2,
		5,
		4,
		.2f
	},
	.score = 200,
	.movementSpeed = 36.f
};

static EnemyType g_MrEgg{
	.horizontalWalkAnimation = {
		{
			16.f,
			16.f
		},
		2,
		6,
		1,
		.2f
	},
	.verticalWalkAnimation = {
		{
			16.f,
			16.f
		},
		2,
		6,
		4,
		.2f
	},
	.crushAnimation = {
		{
			16.f,
			16.f
		},
		4,
		7,
		0,
		.4f
	},
	.dieAnimation = {
		{
			16.f,
			16.f
		},
		2,
		7,
		4,
		.2f
	},
	.score = 300,
	.movementSpeed = 29.f
};