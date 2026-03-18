
#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"../MechaPartsData/BoostBrustData.h"

#include"BoostComponent.h"
#include"EnergyComponent.h"
#include"../MechaPartsObjectFunction/BoostFunction.h"

void BoostComponent::Update()
{
	for (size_t i = 0; i < boostBrustList.size(); i++)
	{
		boostBrustList[i]->nowAvoidWait++;
		if (boostBrustList[i]->nowAvoidWait > boostBrustList[i]->data->GetAvoidWait())
			boostBrustList[i]->nowAvoidWait = boostBrustList[i]->data->GetAvoidWait() + 1;
	}

	for (int i = 0; i < DIRECTION_MAX_COUNT; i++)
	{
		useAvoidFlg[i] = false;
	}

	ChLMat tmp;
	tmp.SetRotationYAxis(ChMath::ToRadian(GetRotation().y));

	ChVec3 front = tmp.TransformCoord(ChVec3(0.0f, 0.0f, 1.0f));
	ChVec3 back = tmp.TransformCoord(ChVec3(0.0f, 0.0f, -1.0f));
	ChVec3 left = tmp.TransformCoord(ChVec3(-1.0f, 0.0f, 0.0f));
	ChVec3 right = tmp.TransformCoord(ChVec3(1.0f, 0.0f, 0.0f));
	ChVec3 up = ChVec3(0.0f, 1.0f, 0.0f);
	ChVec3 down = ChVec3(0.0f, -1.0f, 0.0f);

	UpdateAvoid(InputName::FrontAvo, Direction::Front, front);
	UpdateAvoid(InputName::BackAvo, Direction::Back, back);
	UpdateAvoid(InputName::RightAvo, Direction::Right, right);
	UpdateAvoid(InputName::LeftAvo, Direction::Left, left);
	UpdateAvoid(InputName::UpAvo, Direction::Up, up);
	UpdateAvoid(InputName::DownAvo, Direction::Down, down);

	UpdateBoost(InputName::FrontBoost, Direction::Front, front);
	UpdateBoost(InputName::BackBoost, Direction::Back, back);
	UpdateBoost(InputName::RightBoost, Direction::Right, right);
	UpdateBoost(InputName::LeftBoost, Direction::Left, left);
	UpdateBoost(InputName::UpBoost, Direction::Up, up);
	UpdateBoost(InputName::DownBoost, Direction::Down, down);
}

void BoostComponent::UpdateAvoid(InputName _avoidInput, Direction _direction, const ChVec3& _moveDirection)
{
	if (!IsPushFlg(_avoidInput))return;

	std::vector<ChPtr::Shared<UseBoostData>>useBoost;
	unsigned long useEnergy = 0;

	for (int i = 0; i < boostBrustList.size(); i++)
	{
		if (!boostBrustList[i]->data->GetBoostDirectionFlg(_direction))continue;
		if (boostBrustList[i]->data->GetAvoidWait() > boostBrustList[i]->nowAvoidWait)continue;
		useBoost.push_back(boostBrustList[i]);
		useEnergy += boostBrustList[i]->data->GetAvoidUseEnergy();
	}

	if (useBoost.empty())return;

	auto&& energy = GetComponent<EnergyComponent>();
	
	if (energy->GetNowEnergy() < useEnergy)return;

	energy->SubNowEnergy(useEnergy);
	useAvoidFlg[ChStd::EnumCast(_direction)] = true;

	float avoidPower = 0.0f;

	for (int i = 0; i < useBoost.size(); i++)
	{
		useBoost[i]->func->SetAvoidScaling();
		avoidPower += useBoost[i]->data->GetAvoidPower();
		useBoost[i]->nowAvoidWait = 0;
	}

	AddMoveVector(_moveDirection * avoidPower);

}

void BoostComponent::UpdateBoost(InputName _avoidInput, Direction _direction, const ChVec3& _moveDirection)
{
	if (!IsPushFlg(_avoidInput))return;
	if (useAvoidFlg[ChStd::EnumCast(_direction)])return;

	std::vector<ChPtr::Shared<UseBoostData>>useBoost;
	unsigned long useEnergy = 0;

	for (int i = 0; i < boostBrustList.size(); i++)
	{
		if (!boostBrustList[i]->data->GetBoostDirectionFlg(_direction))continue;
		useBoost.push_back(boostBrustList[i]);
		useEnergy += boostBrustList[i]->data->GetBoostUseEnergy();
	}

	if (useBoost.empty())return;

	auto&& energy = GetComponent<EnergyComponent>();

	if (energy->GetNowEnergy() < useEnergy)return;

	energy->SubNowEnergy(useEnergy);

	float boostPower = 0.0f;

	for (int i = 0; i < useBoost.size(); i++)
	{
		useBoost[i]->func->SetBoostScaling();
		boostPower += useBoost[i]->data->GetBoostPower();
	}

	AddMoveVector(_moveDirection * boostPower);
}

void BoostComponent::AddBoostData(BoostBrust* _data,BoostFunction* _func)
{
	auto&& newData = ChPtr::Make_S<UseBoostData>();

	newData->data = _data;
	newData->func = _func;

	boostBrustList.push_back(newData);

}

void BoostComponent::SubBoostData(BoostBrust* _data)
{
	for (int i = 0; i < boostBrustList.size(); i++)
	{
		if (boostBrustList[i]->data != _data)continue;

		boostBrustList.erase(boostBrustList.begin() + i);
		i = 0;
	}

}
