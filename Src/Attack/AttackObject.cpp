
#include"../BaseIncluder.h"
#include"../Frames/GameFrame.h"
#include"Attack.h"
#include"AttackObject.h"
#include"../BaseMecha/BaseMecha.h"

#define CREATE_SMOKE_MAX_COUNT 2

void AttackObject::Init(const ChLMat& _startMat)
{
	data->InitBulletObject(_startMat ,*this);
	createSmokeCount = CREATE_SMOKE_MAX_COUNT;
}

unsigned long AttackObject::GetPenetration()
{ 
	return data->GetPenetration();
}

float AttackObject::GetHitSize()
{
	return data->GetHitSize();
}

void AttackObject::Update()
{
	data->UpdateBulletObject(*this);
}

void AttackObject::Move()
{
	data->MoveBulletObject(*this);

	return;
	
	if (nowCreateSmokeCount <= 0)
	{
		//frame->AddSmokeEffectObject(physics->GetPosition(), physics->GetAddMovePowerVector() * -0.01f);
		frame->AddSmokeEffectObject(physics->GetPosition(), physics->GetAddMovePowerVector() * -0.001f, dispersalPower);
		createSmokeCount++;
		dispersalPower *= 0.5f;
	}

	nowCreateSmokeCount = (nowCreateSmokeCount + 1) % createSmokeCount;
}

void AttackObject::Draw3D()
{
	ChLMat tmp = ChLMat();
	ChQua qua;

	ChVec3 ypr = physics->GetRotation();
	qua.SetRotationYAxis(ChMath::ToRadian(ypr.y));
	qua.AddRotationXAxis(ChMath::ToRadian(ypr.x));
	qua.AddRotationZAxis(ChMath::ToRadian(ypr.z));

	tmp.SetRotation(qua);
	tmp.SetScalling(ChVec3(10.0f));
	tmp.SetPosition(physics->GetPosition());
	ChMat_11 draw;
	draw = tmp;

	collider.SetPosition(physics->GetPosition());
	collider.SetRotation(qua);
	data->Draw(draw);
}

void AttackObject::UpdateHit()
{
	auto&& mechaList = frame->GetMechas();
	if (mechaList.size() <= mechasNo)return;
	auto&& mecha = mechaList[mechasNo];
	if (mecha.expired())return;
	mecha.lock()->SetHitEffectDrawFrame();
}