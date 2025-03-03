#pragma once

struct TransformBench
{
	float matrix[16] = { 
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1 };
};

class GameObject3D final
{
public:
	TransformBench transform;
	int ID;
};

class GameObject3DAlt final
{
public:
	int ID;
	TransformBench* transform;
};
