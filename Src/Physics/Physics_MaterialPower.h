#pragma once

#include"Physics_Gravity.h"

class Physics_MaterialPower :public Physics_Gravity
{
public:

	void Update()override;

	virtual void Test() = 0;

	virtual void CreateMap() = 0;

	virtual void AddPower() = 0;

private:

	float mass = 0.0f;

};