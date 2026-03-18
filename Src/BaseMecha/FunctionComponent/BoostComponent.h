#pragma once

#include"FunctionComponent.h"

#include"../Direction.h"

class EnergyComponent;
class BoostBrust;
class BoostFunction;

class BoostComponent:public FunctionComponent
{
public:

	struct UseBoostData
	{
		BoostFunction* func = nullptr;
		BoostBrust* data = nullptr;
		unsigned long nowAvoidWait = 0;
 	};

public:

	void Update()override;

private:

	void UpdateAvoid(InputName _avoidInput, Direction _direction,const ChVec3& _moveDirection);

	void UpdateBoost(InputName _boostInput, Direction _direction, const ChVec3& _moveDirection);

public:

	void AddBoostData(BoostBrust* _data, BoostFunction* _func);

	void SubBoostData(BoostBrust* _data);

private:

	std::vector<ChPtr::Shared<UseBoostData>>boostBrustList;
	bool useAvoidFlg[6] = { false,false,false,false,false,false };
};
