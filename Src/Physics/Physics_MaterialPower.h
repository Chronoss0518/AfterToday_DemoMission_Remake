#pragma once

#include"Physics_Gravity.h"

class Physics_MaterialPower :public PhysicsFunction
{
public:

	void AddMove()override final;

	virtual void Test() = 0;

	virtual void CreateMap() = 0;

	virtual void AddPower() = 0;

	void AddRegist()override final;

	virtual void AddMoveRegist() = 0;

	virtual void AddRotateRegist() = 0;

private:



};