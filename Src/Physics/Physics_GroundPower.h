#pragma once
#pragma once

#include"Physics_MaterialPower.h"

class Physics_GroundPower :public Physics_MaterialPower
{
public:

	void Test()override;

	void CreateMap()override;

	void AddPower()override;

	void AddMoveRegist()override;

	void AddRotateRegist()override;


};