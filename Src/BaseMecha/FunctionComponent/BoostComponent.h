#pragma once

#include"FunctionComponent.h"

#include"../BoostDirection.h"

class EnergyComponent;
class BoostBrust;

class BoostComponent:public FunctionComponent
{
public:

	struct UseBoostData
	{
		BoostBrust* data = nullptr;
 	};

public:

	BoostComponent();

public:

	void ClearBoostAvoidWait(InputName _avoidType);

public:

	void Update()override;

public:

	void SetBoostAvoidWait(const unsigned long _avoidWait, InputName _avoidType);

public:

	void AddBoostData(BoostBrust* _data);

	void SubBoostData(BoostBrust* _data);

private:

	static float& GetBoostUpPowerRegister() { static float regist = 0.3f; return regist; }

	std::vector<BoostBrust*>boostBrustList;
};
