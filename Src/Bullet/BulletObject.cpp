
#include"../BaseIncluder.h"
#include"../Frames/GameFrame.h"
#include"Bullet.h"
#include"BulletObject.h"
#include"../BaseMecha/BaseMecha.h"

#define CREATE_SMOKE_MAX_COUNT 1

void BulletObject::Init(const ChLMat& _startMat)
{
	data->InitBulletObject(_startMat ,*this);
}

unsigned long BulletObject::GetPenetration()
{ 
	return data->GetPenetration();
}

float BulletObject::GetHitSize()
{
	return data->GetHitSize();
}

void BulletObject::Update()
{
	data->UpdateBulletObject(*this);
}

void BulletObject::Move()
{
	data->MoveBulletObject(*this);

	if(createSmokeCount <= 0)frame->AddSmokeEffectObject(physics->GetPosition(), physics->GetAddMovePowerVector() * -0.01f);

	createSmokeCount = (createSmokeCount + 1) % CREATE_SMOKE_MAX_COUNT;
}

void BulletObject::Draw3D()
{
	ChLMat tmp = ChLMat();
	tmp.SetScalling(ChVec3(10.0f));
	tmp.SetPosition(physics->GetPosition());
	tmp.SetRotation(physics->GetRotation());
	ChMat_11 draw;
	draw = tmp;

	collider.SetPosition(physics->GetPosition());
	collider.SetRotation(physics->GetRotation());
	data->Draw(draw);
}
