
#include"../BaseIncluder.h"
#include"../Frames/GameFrame.h"
#include"Attack.h"
#include"AttackObject.h"
#include"../BaseMecha/BaseMecha.h"

#define CREATE_SMOKE_MAX_COUNT 10

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

	ChQua rot;

	rot.SetRotationYAxis(ChMath::ToRadian(physics->GetRotation().y));
	rot.AddRotationXAxis(ChMath::ToRadian(physics->GetRotation().x));
	rot.AddRotationZAxis(ChMath::ToRadian(physics->GetRotation().z));

	collider.SetPosition(physics->GetPosition());
	collider.SetRotation(rot);
}

void AttackObject::Draw3D()
{
	ChLMat tmp = ChLMat();
	ChQua qua;

	ChQua rot;

	rot.SetRotationYAxis(ChMath::ToRadian(physics->GetRotation().y));
	rot.AddRotationXAxis(ChMath::ToRadian(physics->GetRotation().x));
	rot.AddRotationZAxis(ChMath::ToRadian(physics->GetRotation().z));

	tmp.SetRotation(rot);

#if ATTACK_OBJECT_DEBUG_FLG
	tmp.SetScalling(ChVec3(ATTACK_OBJECT_DEBUG_SIZE));
#endif

	tmp.SetPosition(physics->GetPosition());
	ChMat_11 draw;
	draw = tmp;

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