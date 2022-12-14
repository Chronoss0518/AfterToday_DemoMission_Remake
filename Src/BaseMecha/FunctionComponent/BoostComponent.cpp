
#include"../../BaseIncluder.h"

#include"../../AllStruct.h"

#include"BoostComponent.h"
#include"BoostComponents.h"

void BoostComponent::Update()
{

	ChLMat tmp;

	tmp.SetRotationYAxis(ChMath::ToRadian(GetRotation().y));

	UpdateBoost(tmp);

	UpdateAvoid(tmp);

	unsigned long fps = ChSystem::SysManager().GetFPS();

	boostRotation += 5;
	boostRotation = boostRotation < 360.0f ? boostRotation : boostRotation - 360.0f;

	if (avoidUseFlg && avoidStartFlg)
	{
		SubNowEnelgy(useAvoidEnelgy);

		avoidStartFlg = false;
		nowBoostPow = 3.0f;
		boostUseFlg = false;
	}
	else if (boostStartFlg == boostUseFlg)
	{
		nowBoostPow = 0.5f;
		boostStartFlg = false;
	}

	if (boostUseFlg)
	{
		SubNowEnelgy(useBoostEnelgy);

		float tmp = 1.0f - nowBoostPow;
		nowBoostPow += tmp * tmp * 0.1f;
		nowBoostPow = nowBoostPow > 1.0f ? 1.0f : nowBoostPow;
	}
	else
	{
		float tmp = nowBoostPow;
		nowBoostPow -= tmp * tmp * 0.1f * 2.0f;
		nowBoostPow = nowBoostPow < 0.0f ? 0.0f : nowBoostPow;
		boostStartFlg = true;
	}

}

void BoostComponent::DrawBegin()
{

	ChLMat tmp;
	tmp.SetRotationYAxis(boostRotation);

	for (auto&& boostObject : boostObject)
	{
		auto testScl = boostObject->GetOutSizdTransformLMat().GetScalling().x;
		tmp.SetScalling(ChVec3(testScl >= nowBoostPow ? testScl : nowBoostPow));
		boostObject->SetOutSizdTransform(tmp);
	}

	boostUseFlg = false;
}

void BoostComponent::UpdateBoost(const ChLMat& _nowTargetPoster)
{

	if (!IsPushFlg(boostName))return;
	if (useBoostEnelgy > GetNowEnelgy())return;

	boostUseFlg = true;

	AddMoveVector(_nowTargetPoster.TransformCoord(direction) * boostPow / PhysicsMachine::GetFPS());

}

void BoostComponent::DrawEnd()
{

	ChLMat tmp;
	tmp.SetScalling(ChVec3(0.0f));

	for (auto&& boostObject : boostObject)
	{
		boostObject->SetOutSizdTransform(tmp);
	}

}

void BoostComponent::UpdateAvoid(const ChLMat& _nowTargetPoster)
{
	{
		unsigned long longMax = -1;

		nowAvoidWaitTime = (nowAvoidWaitTime - 1) % longMax;

	}

	if (!IsPushFlg(avoidName))
	{
		avoidUseFlg = false;
		avoidStartFlg = false;
		return;
	}

	if (nowAvoidWaitTime > 0)return;
	if (avoidUseFlg)return;
	if (useAvoidEnelgy > GetNowEnelgy())return;

	nowAvoidWaitTime = avoidWait;

	avoidUseFlg = true;
	avoidStartFlg = true;

	AddMoveVector(_nowTargetPoster.TransformCoord(direction) * avoidPow / PhysicsMachine::GetFPS());

}