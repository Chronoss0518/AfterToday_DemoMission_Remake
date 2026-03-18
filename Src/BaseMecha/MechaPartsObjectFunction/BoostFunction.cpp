#include"../../BaseIncluder.h"
#include"../../AllStruct.h"

#include"BoostFunction.h"

#define BOOST_MAX_SCALE_SIZE 1.0f
#define AVOID_MAX_SCALE_SIZE 2.0f

#define REDUCED_SIZE 0.8f
#define MIN_SIZE 0.01f

void BoostFunction::Update()
{
	nowScaling *= REDUCED_SIZE;

	if (nowScaling < MIN_SIZE)
		nowScaling = 0.0f;
}

void BoostFunction::DrawBegin()
{
	if (targetBoostObject == nullptr)return;
	ChLMat tmpMat;
	tmpMat.SetScalling(nowScaling);
	targetBoostObject->SetOutSideTransform(tmpMat);
}

void BoostFunction::DrawEnd()
{
	if (targetBoostObject == nullptr)return;
	ChLMat tmpMat;
	tmpMat.SetScalling(0.0f);
	targetBoostObject->SetOutSideTransform(tmpMat);
}

void BoostFunction::SetBoostScaling()
{
	if (nowScaling > BOOST_MAX_SCALE_SIZE)return;
	
	nowScaling = BOOST_MAX_SCALE_SIZE;
}

void BoostFunction::SetAvoidScaling()
{
	if (nowScaling > AVOID_MAX_SCALE_SIZE)return;

	nowScaling = AVOID_MAX_SCALE_SIZE;
}
