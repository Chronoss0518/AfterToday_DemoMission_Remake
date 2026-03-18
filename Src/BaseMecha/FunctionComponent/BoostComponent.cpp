
#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"../MechaPartsData/BoostBrustData.h"

#include"BoostComponent.h"
#include"EnergyComponent.h"

void BoostComponent::Update()
{
	for (size_t i = 0; i < boostBrustList.size(); i++)
	{
		boostBrustList[i]->nowAvoidWait++;
		if (boostBrustList[i]->nowAvoidWait > boostBrustList[i]->data->GetAvoidWait())
			boostBrustList[i]->nowAvoidWait = boostBrustList[i]->data->GetAvoidWait() + 1;
	}

	ChLMat tmp;
	tmp.SetRotationYAxis(ChMath::ToRadian(GetRotation().y));

	ChVec3 front = tmp.TransformCoord(ChVec3(0.0f, 0.0f, 1.0f));
	ChVec3 back = tmp.TransformCoord(ChVec3(0.0f, 0.0f, -1.0f));
	ChVec3 left = tmp.TransformCoord(ChVec3(-1.0f, 0.0f, 0.0f));
	ChVec3 right = tmp.TransformCoord(ChVec3(1.0f, 0.0f, 0.0f));
	ChVec3 up = ChVec3(0.0f, 1.0f, 0.0f);
	ChVec3 down = ChVec3(0.0f, -1.0f, 0.0f);

	UpdateAvoid(InputName::FrontAvo, BoostDirection::Front, front);
	UpdateAvoid(InputName::BackAvo, BoostDirection::Back, back);
	UpdateAvoid(InputName::RightAvo, BoostDirection::Right, right);
	UpdateAvoid(InputName::LeftAvo, BoostDirection::Left, left);
	UpdateAvoid(InputName::UpAvo, BoostDirection::Up, up);
	UpdateAvoid(InputName::DownAvo, BoostDirection::Down, down);

	UpdateBoost(InputName::FrontAvo, BoostDirection::Front, front);
	UpdateBoost(InputName::BackAvo, BoostDirection::Back, back);
	UpdateBoost(InputName::RightAvo, BoostDirection::Right, right);
	UpdateBoost(InputName::LeftAvo, BoostDirection::Left, left);
	UpdateBoost(InputName::UpAvo, BoostDirection::Up, up);
	UpdateBoost(InputName::DownAvo, BoostDirection::Down, down);
}

void BoostComponent::UpdateAvoid(InputName _avoidInput, BoostDirection _direction, const ChVec3& _moveDirection)
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
		avoidPower += useBoost[i]->data->GetAvoidPower();
		useBoost[i]->nowAvoidWait = 0;
	}

	AddMoveVector(_moveDirection * avoidPower);

}

void BoostComponent::UpdateBoost(InputName _avoidInput, BoostDirection _direction, const ChVec3& _moveDirection)
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
		boostPower += useBoost[i]->data->GetBoostPower();
	}

	AddMoveVector(_moveDirection * boostPower);
}

void BoostComponent::AddBoostData(BoostBrust* _data)
{
	for (int i = 0; i < boostBrustList.size(); i++)
	{
		if (boostBrustList[i]->data == _data)return;
	}

	auto&& newData = ChPtr::Make_S<UseBoostData>();

	newData->data = _data;

	boostBrustList.push_back(newData);

}

void BoostComponent::SubBoostData(BoostBrust* _data)
{
	for (int i = 0; i < boostBrustList.size(); i++)
	{
		if (boostBrustList[i]->data != _data)continue;

		boostBrustList.erase(boostBrustList.begin() + i);
		return;
	}

}
