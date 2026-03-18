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
		unsigned long nowAvoidWait = 0;
 	};

public:

	void Update()override;

private:

	void UpdateAvoid(InputName _avoidInput, BoostDirection _direction,const ChVec3& _moveDirection);

	void UpdateBoost(InputName _boostInput, BoostDirection _direction, const ChVec3& _moveDirection);

public:

	void AddBoostData(BoostBrust* _data);

	void SubBoostData(BoostBrust* _data);

private:

	std::vector<ChPtr::Shared<UseBoostData>>boostBrustList;
	bool useAvoidFlg[6] = { false,false,false,false,false,false };
};
