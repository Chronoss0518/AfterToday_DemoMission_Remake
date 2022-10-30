#pragma once

#include"Physics_MaterialPower.h"

class Physics_AirPower :public Physics_MaterialPower
{
public:

	void Update()override;

	virtual void Test();

	virtual void CreateMap();

	virtual void AddPower();

private:

};